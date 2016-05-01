/*
****************************************************************************************
*                  Copyright (c) 2006 vBridge Microsystem, Inc.  
*                        Unpublished & Not for Publication
*                              All Rights Reserved                   
*   
* File        :  Motion_Detection.c                     
*                                                                         
* Description : caculate the average size of P frames in 1 sec.
*			  compare the current p frame size with the average p frames size	  
*			if there is a big difference, then take it as a motion happened.	  
*				  
*             
* Date        : Jan 2, 2007                                                          
****************************************************************************************
*/
#define MOTION_DETECTION_GLOBALS
#include "video_cfg.h"
#ifdef MOTION_DETECTION

#include "sun_type_defs.h"
#include "VIN_SET.h"

#include "os_cpu.h"
#include "os_cfg.h"
#include "uCOS_II.h"

#include "mpeg4_encoder.h"

#include "Motion_Detection.h"
#include "Band_adjust.h"
#include "../2risc platform/2risc_comm_protocol.h"
#include "../2risc platform/video_api.h"
#include "two_risc_comm.h"

#include "ppe.h"
#include "two_risc_video.h"
#include "Sc.h"
#include "vb1000.h"
#include "../../bsp/except.h"

#include "VSC_API.h"
#include "two_risc_display.h"

/*
***************************************************************************
*                             MDSendPacketToLinux()
*
* Description: send the alarm signal to Linux
*				
*
* Arguments  : flag:1:motion detection alarm, 2:annunciator alert (use GPIO)
*                           3:motion stop report
*
* Return     :               
*
* Note(s)    : 
*						
*            
***************************************************************************
*/ 
static void MDSendPacketToLinux(int flag)
{
    Risc_Packet_t MDPacket;
    unsigned int *pInt;

    pInt = (unsigned int *)&MDPacket;

    MDPacket.start = RISC_PKT_START;
    MDPacket.main_type = tVideo;
    MDPacket.sub_type = tGetMotionAlarm;
    MDPacket.length = 0x4;

    *(pInt + 2) = flag;
    MDPacket.end   = RISC_PKT_END;

    if (TwoRiscCommSendPacket((unsigned int *)&MDPacket, RPACKET_LEN ) == SUCCESSFUL )
        RiscFlagSet();
    else
        printf("TwoRiscCommSendPacket:MD error\r\n");	
}

static void MDSendStatusToLinux()
{
    Risc_Packet_t MDPacket;
    unsigned int *pInt;

    pInt = (unsigned int *)&MDPacket;

    MDPacket.start = RISC_PKT_START;
    MDPacket.main_type = tVideo;
    MDPacket.sub_type = tGetMotionStatus;
    MDPacket.length = 0x4;

    *(pInt + 2) = (unsigned int)&gMdCellStatus[0][0];

    MDPacket.end = RISC_PKT_END;

    if(TwoRiscCommSendPacket((unsigned int *)&MDPacket, RPACKET_LEN) == SUCCESSFUL)
        RiscFlagSet();
    else
        printf("TwoRiscCommSendPacket: MD status error!\n");
}

#ifdef SUPPORT_MD_FRAMESIZE
unsigned int StillPMaxSize[TOTALVMODE][5] = {   //for every video mode, when P frame size exceed a value, assumed a motion happened.
                                                                            {0,0,0,0,0}/*EQCIF*/,
                                                                            {0,0,0,0,0}/*SVGA*/, 
                                                                            {0,0,0,0,0}/*QCIF*/, 
                                                                            {0,0,0,0,0}/*QVGA*/, 
                                                                            {0,0,0,0,0}/*QQVGA*/,
                                                                            {0,0,0,0,0}/*CIF*/, 
                                                                            {0,0,0,0,0}/*VGA*/, 
                                                                            {0,0,0,0,0}/*SXGA*/, 
                                                                            {0,0,0,0,0}/*NTSC_D1*/, 
                                                                            {0,0,0,0,0}/*PAL_D1*/,
                                                                            {0,0,0,0,0}/*NTSC_CIF*/,
                                                                            {0,0,0,0,0}/*NTSC_QCIF*/,
                                                                            {0,0,0,0,0}/*SUB_QCIF*/
                                                                                };

unsigned int minBWneeded[TOTALVMODE] = {      //in CBR mode, for every video mode, when bandwidth too large, we will use VBR.
                                      0/*EQCIF*/,0/*SVGA*/,     256/*QCIF*/,    0/*QVGA*/,      0/*QQVGA*/,  0/*CIF*/, 0/*VGA*/,
                                      0/*SXGA*/,0/*NTSC_D1*/,0/*PAL_D1*/,0/*NTSC_CIF*/,0/*NTSC_QCIF*/,0/*SUB_QCIF*/};

unsigned int MD_I_Sensi[MAX_SENSITIVITY_LEVEL] = {   //range that I frame should be in, if exceed, assume a motion happened.
                                                thousand_50,thousand_30,thousand_25,thousand_20,thousand_15,
                                                thousand_14,thousand_13,thousand_11,thousand_10,thousand_8};

unsigned int MD_P_Sensi[3][MAX_SENSITIVITY_LEVEL] = {    //range that P frame should be in, if exceed, assume a motion happened.
                                                {Percent_200,Percent_150,Percent_100,Percent_70,Percent_50,     //if average P frame size < 2000B
                                                Percent_40,  Percent_30,  Percent_20,  Percent_10,Percent_5},
                                                {thousand_100,thousand_50,thousand_30,thousand_25,thousand_20,  //if average P frame size < 4000B
                                                 thousand_15,thousand_14,thousand_13,thousand_10,thousand_6},
                                                {thousand_50,thousand_30,thousand_25,thousand_20,thousand_15,  //if average P frame size > 6000B
                                                 thousand_13,thousand_11,thousand_10,thousand_8,thousand_5}
                                                };
unsigned int senindex = 0;
unsigned int MD_Q_Sensi[MAX_SENSITIVITY_LEVEL] = {15,12,10,8,6,5,4,3,2,1};       //in CBR mode, when Q exceed, assume motion happened

unsigned int RefIFrameSize[MAX_Q_VALUE] = {0,};   //in CBR mode, I frame refer size for each Q.

static unsigned int MDMode = MODE_VBR;    //1:CBR,2:VBR
static ImageMode vmode = QVGA;              //video mode, default is QVGA mode
static unsigned int totalPsize[2] = {0,0};    //in VBR mode, the p frame in one second is divided into two parts:the first 3 frames, and the remains
static unsigned int Pnum = 0;                     //in VBR mode, indicate the frame should in which part

tMDInfo gtMDInfo;

extern int Bandwidth_Ctl_En;

#define PERCENT(data, percent) (data + data * percent / 100)
#define THOUSAND(data, percent) (data + data * percent / 1000)

static int IsOutThousandRange(unsigned int data1, unsigned int data2, unsigned int thousand)
{
    if(data1 > (data2 + data2 * thousand / 1000))
        return 1;
    if(data1 < (data2 - data2 * thousand / 1000))
        return 1;
    return 0;
}
/*
***************************************************************************
*                             MDInitParams()
*
* Description: initialise some variables for Motion detection
*
* Arguments  : none
*
* Return     :  None                    
*
* Note(s)    : 	
*            
***************************************************************************
*/	
static void MDInitParams(void)
{
/**************VBR parameters******************************/
    gtMDInfo.avePsize[0] = 0;
    gtMDInfo.avePsize[1] = 0;
    Pnum = 0;
    totalPsize[0] = 0;
    totalPsize[1] = 1;
    gtMDInfo.lastIsize = 0;
    gtMDInfo.initflag = 1;
    gtMDInfo.encodeQ = mp4EncodeParams.quant;
/**************CBR parameters*****************************/
    gtMDInfo.minQ = 0;
    gtMDInfo.maxQ = 0;
    gtMDInfo.maxQindex = 0;
    gtMDInfo.minQindex = 0;
#if 0
    vmode = GetImageMode(gImageWidth, gImageHeight);
    if(vmode == -1)
        gIsMDEnabled = FALSE;
#endif
}

/*
***************************************************************************************
*                                               MD_VBR_I()
* Parameter: framelen: I frame length
*
* Return :  > 0: motion detected
*              = 0: no motion
* Note    : bacause for a still picture, I frame size changes little. and for different picture, I frame size changes
*             dramatically. if picture changes very slowly, the I frame size changes very little, but for a long run,
*             maybe it changs greatly. so there two comparisions: 1) compare the current I frame size with the
*             last I frame size; 2) compare the min I frame size with the max I frame size, once a motion detected,
*             min I frame size and max I frame size reset to 0.
***************************************************************************************
*/
static unsigned char MD_VBR_I(unsigned int framelen)
{
    int i;
    static unsigned int maxI = 0, minI = 0;
    static unsigned int num = 0;

    if(gtMDInfo.lastIsize == 0) {          //the first I frame
        gtMDInfo.lastIsize = framelen;
        maxI = framelen;
        minI = framelen;
        num = 0;
        return 0;
    }

    i = IsOutThousandRange(framelen, gtMDInfo.lastIsize, gtMDInfo.IFrameSensi);
    if(i) {  //compared with last I frame
        maxI = 0;
        minI = 0;
        num++;
        gtMDInfo.lastIsize = framelen;      //updata last I frame, ready for next comparision
        if(num > 2) {                                  //if 3 I frames, then a motion detected. maybe need modify
            return 1;
        }else {
            return 0;
        }
    }else {
        gtMDInfo.lastIsize = framelen;      //updata last I frame, ready for next comparision
        num = 0;
    }

    if((maxI == 0)||(minI == 0)) {
        maxI = framelen;
        minI = framelen;
    }

    if(framelen > maxI) {
        if(framelen > THOUSAND(minI, thousand_50)) {  //thousand_50 may need modified,but i don't know exactly which one 
            maxI = 0;
            minI = 0;
            return 2;
        }else {
            maxI = framelen;
        }
    }else if(framelen < minI) {
        if(maxI > THOUSAND(framelen, thousand_50)) {
            maxI =0;
            minI = 0;
            return 2;
        }else {
            minI = framelen;
        }
    }

    return 0;
}
/*
***************************************************************************
*                             MD_VBR()
*
* Description: copare the current frame with some referrence frame size, to judge whether a
*                   motion happened.
*
* Arguments :enocded frame size(Byte) : ptMsg2->len * 4
*
* Return     :  None                    
*
* Note(s)    : when a motion detected, send alarm msg to linux
*						
*            
***************************************************************************
*/
static void MD_VBR(int framelen)
{
    unsigned int i;
    int source = 0;
    unsigned int framesize;
    int moveflag = 0;               //when motion detected, set it.
    static int flag = 0;               //when motion detected in one second, set it.
    static unsigned int num = 0;

#ifdef DEBUG_MD
    int size = 0;
    static unsigned int maxpsize = 0, minpsize = 0;
    unsigned int percent;
    static unsigned int eachP[50] = {0,};               //save each P frame size between two I frames
#endif

    if(gtMDInfo.encodeQ != mp4EncodeParams.quant)   //Q value changed, need initialize parameters
        MDInitParams();

    if (mp4EncodeParams.isPFrame) {  //P frame.
        if((gtMDInfo.initflag) && (gtMDInfo.lastIsize == 0))   //first frame must be I frame
            return;
        if(gtMDInfo.initflag) {                   //the first P frame since initialized, and I frame has appeared
            totalPsize[0] += framelen;
            Pnum++;
            gtMDInfo.initflag = 0;
#ifdef DEBUG_MD
            eachP[0] = framelen;
#endif
        }else {
#ifdef DEBUG_MD
            eachP[Pnum] = framelen;
#endif
            Pnum++;
            //see if P frame size exceed the largest size get from statistic data, now we haven't gotten the statistic data
            if((StillPMaxSize[vmode][mp4EncodeParams.quant] != 0) && (framelen > StillPMaxSize[vmode][mp4EncodeParams.quant])) 
            {
                moveflag = 1;
                flag = 1;
                source = 1;
            }else {
                framesize = PERCENT(gtMDInfo.avePsize[0], Percent_100);  //Percent_100 may need modified
                if((framelen > framesize) &&(framesize != 0)) {
                    moveflag = 1;
                    flag = 1;
                }
                if(Pnum < 4) {          //the first 3 p frames 
                    framesize = PERCENT(gtMDInfo.avePsize[0], Percent_100);
                    if((framelen > framesize) && (framesize != 0)) {       //compare with statistic p frame size
                        moveflag = 1;
                        flag = 1;
                        source = 2;
#ifdef DEBUG_MD
                        size = gtMDInfo.avePsize[0];
#endif
                    }
                }else {                 //the next p frames
                    framesize = PERCENT(gtMDInfo.avePsize[1], gtMDInfo.PFrameSensi);
                    if((framelen > framesize) && (framesize != 0)) {       //compare with statistic p frame size
                        num++;
                        if(num >= gtMDInfo.pframe_outrange_num) {
                            moveflag = 1;
                            flag = 1;
                            num = gtMDInfo.pframe_outrange_num - 1;
                            source = 3;
#ifdef DEBUG_MD
                            size = gtMDInfo.avePsize[1];
#endif                           
                        }
                    }else {
                        num = 0;
                    }
                }
                
                if(flag == 0) {       //update statistic P frame size
                    if(Pnum < 4)
                        totalPsize[0] += framelen;
                    else {
                        totalPsize[1] += framelen;
#ifdef DEBUG_MD
                        if((maxpsize < framelen) || (maxpsize == 0))
                            maxpsize = framelen;
                        if((minpsize > framelen) || (minpsize == 0))
                            minpsize = framelen;
#endif
                    }
                }
            }
        }
    }
    else {                   //I frame

 #ifdef DEBUG_MD
        size = gtMDInfo.lastIsize;
#endif

        moveflag = MD_VBR_I(framelen);
        if(moveflag == 1) {
            source = 4;
        }else if(moveflag == 2) {
            source = 5;
        }

        if((flag == 0) && (Pnum > 0)) {      //if in this second, no motion detected, update statistics data
            if(Pnum > 3) {
                gtMDInfo.avePsize[0] = totalPsize[0] / 3;
                gtMDInfo.avePsize[1] = totalPsize[1] / (Pnum - 3);
                if(gtMDInfo.avePsize[1] < 2000)
                    senindex = 0;
                else if(gtMDInfo.avePsize[1] < 4000)
                    senindex = 1;
                else 
                    senindex = 2;
                i = (gMotionSensi - 1) / 10;
                gtMDInfo.PFrameSensi = MD_P_Sensi[senindex][i];
            }else {
                gtMDInfo.avePsize[0] = totalPsize[0] / Pnum;
                gtMDInfo.avePsize[1] = 0;
            }
#ifdef DEBUG_MD
            if(minpsize != 0)
                percent = (maxpsize - minpsize) * 100 / minpsize;
            printf("\naveP=%d,maxP=%d,minP=%d,max-min=%d,", gtMDInfo.avePsize[1], maxpsize, minpsize, percent);
            if(gtMDInfo.avePsize[1] != 0)
                percent = (maxpsize - gtMDInfo.avePsize[1]) * 100 / gtMDInfo.avePsize[1];
            printf("max-ave=%d,", percent);
#endif
        }

#ifdef DEBUG_MD
        if(size > framelen)
            percent = (size - framelen) * 1000 / size;
        else if(size > 0)
            percent = (framelen - size) * 1000 / size;
        printf("thousand I=%d!\n", percent);

        minpsize = 0;
        maxpsize = 0;

        for(i = 0; i< Pnum; i++) {
            if(eachP[i] > gtMDInfo.avePsize[1])
                percent = (eachP[i] - gtMDInfo.avePsize[1]) * 1000 / gtMDInfo.avePsize[1];
            else
                percent = (gtMDInfo.avePsize[1] - eachP[i]) * 1000 / gtMDInfo.avePsize[1];
            printf("%d-", percent);
        }
        printf("\n");
#endif
        flag = 0;
        num = 0;
        Pnum = 0;
        totalPsize[0] = 0;
        totalPsize[1] = 0;
    }

    if(moveflag) {
#ifdef SEND_MD_TO_LINUX
        MDSendPacketToLinux(moveflag);
#endif
        printf("\nvbr motion detected,source=%d!~~~~~~~~~~~~~~~~\n", source);
#ifdef DEBUG_MD
        printf("sr=%d,frame len=%d,ref frame len=%d\n", source, framelen, size);
        printf("psensi=%d,Isensi=%d\n", gtMDInfo.PFrameSensi, gtMDInfo.IFrameSensi);
#endif
    }

#ifdef DEBUG_MD
    if(mp4EncodeParams.isPFrame)
        printf("(P:%d)", framelen);
    else {
        printf("(I:%d)\r\n", framelen);
    }
#endif
}

/*
***************************************************************************
*                             MD_CBR()
*
* Description: 
*
* Arguments  : framelen: data length of enocded frame : ptMsg2->len * 4
*
* Return     :  None                    
*
* Note(s)    : 
*						
*            
***************************************************************************
*/    
static void MD_CBR(int framelen)
{
    int moveflag = 0;
    static unsigned int index = 0;

#ifdef DEBUG_MD
    unsigned int maxQ;
    unsigned int minQ;
#endif

    if(mp4EncodeParams.isPFrame) {   //P frame
        index++;
        if(gtMDInfo.minQ == 0) {       //
            index = 1;
            gtMDInfo.maxQ = mp4EncodeParams.quant;
            gtMDInfo.minQ = mp4EncodeParams.quant;
            gtMDInfo.maxQindex = index;
            gtMDInfo.minQindex = index;
        }else if((gtMDInfo.minQ != 0) && (gtMDInfo.maxQ == 0)) {
            gtMDInfo.maxQ = mp4EncodeParams.quant;
            gtMDInfo.maxQindex = index;
            if((gtMDInfo.maxQ > gtMDInfo.minQ + gtMDInfo.QSensi) && (gtMDInfo.maxQindex > gtMDInfo.minQindex))
            {
                moveflag = 1;
                gtMDInfo.maxQ = 0;
            }else if((gtMDInfo.maxQ <QFluctuationDownBound + gtMDInfo.QSensi) && (gtMDInfo.maxQindex > gtMDInfo.minQindex))
            {
                gtMDInfo.minQ = 0;
                index = 0;
            }
        }else {
            if(gtMDInfo.maxQ < mp4EncodeParams.quant) {
                gtMDInfo.maxQ = mp4EncodeParams.quant;
                gtMDInfo.maxQindex = index;
            }
            if(gtMDInfo.minQ > mp4EncodeParams.quant) {
                gtMDInfo.minQ = mp4EncodeParams.quant;
                gtMDInfo.minQindex = index;
            }
            if(gtMDInfo.maxQindex < gtMDInfo.minQindex) {
                gtMDInfo.maxQ = gtMDInfo.minQ;
                gtMDInfo.maxQindex = gtMDInfo.minQindex;
            }
            if((gtMDInfo.maxQ > gtMDInfo.minQ + gtMDInfo.QSensi) && (gtMDInfo.maxQindex > gtMDInfo.minQindex))
            {
                moveflag = 1;
                gtMDInfo.maxQ = 0;
            }else if((gtMDInfo.maxQ == QFluctuationDownBound) && (index > 3600)) {  //if Q of 60
                index = 0;
                MDMode = MODE_VBR;
            }else if((gtMDInfo.minQ > QFluctuationUpBound - 5) && (index > 3600)) {    //
                index = 0;
                MDMode = MODE_VBR;
            }
        }
    }
    else {   //I frame
        if(RefIFrameSize[mp4EncodeParams.quant] == 0)  //I frame not exit
            RefIFrameSize[mp4EncodeParams.quant] = framelen;
        else {
            if(framelen > PERCENT(RefIFrameSize[mp4EncodeParams.quant], gtMDInfo.IFrameSensi)) {   //bigger out of range
                moveflag = 1;
            }
            RefIFrameSize[mp4EncodeParams.quant] = framelen;
        }
    }

    if(moveflag) {
#ifdef SEND_MD_TO_LINUX
        MDSendPacketToLinux(moveflag);
#endif
        printf("\ncbr move!,QSensi=%d\n", gtMDInfo.QSensi);
    }
#ifdef DEBUG_MD
    printf("(%d)", mp4EncodeParams.quant);
#endif
}	
void MdFramesize(int framelen)
{
    if(MDMode == MODE_CBR) {
        MD_CBR(framelen);
    }else {
        MD_VBR(framelen);
    }
}

/*
***************************************************************************
*                             MDFrameSizeInit()
*
* Description: set motion detection mode, and initialise variables to default values
*			
*
* Arguments  : Sensi: the sensitivity 1~100, bigger, more sensitive
*
* Return     :  None                    
*
* Note(s)    : there are two cases: CBR mode, and VBR mode, which use different agorithim
*						
*            
***************************************************************************
*/
void MDFrameSizeInit(int sensi)
{
    int i;

    if(sensi <= 0) {
        gIsMDEnabled = FALSE;
        gMotionSensi = 0;
        printf("Motion Detect disabled, sensitivity=%d!\n", sensi);
        return;
    }

    if(Bandwidth_Ctl_En) {        //CBR mode
        MDMode = MODE_CBR;
        printf("Motion Detect use CBR mode,sensitivity=%d!\r\n", sensi);
    }else {                                //VBR mode
        MDMode = MODE_VBR;
        printf("Motion Detect use VBR mode,sensitivity=%d!\r\n", sensi);
    }

    gIsMDEnabled = TRUE;
    if(sensi > 100) 
        gMotionSensi = 100;
    else
        gMotionSensi = sensi;

    i = (gMotionSensi - 1) / 10;

    gtMDInfo.IFrameSensi = MD_I_Sensi[i];
    gtMDInfo.PFrameSensi = MD_P_Sensi[senindex][i];
    switch(i)
    {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
            gtMDInfo.pframe_outrange_num = 2;
            break;
        case 8:
            gtMDInfo.pframe_outrange_num = 3;
            break;
        case 9:
            gtMDInfo.pframe_outrange_num = 4;
            break;
        default:
            gtMDInfo.pframe_outrange_num = 2;
            break;
    }

    gtMDInfo.QSensi = MD_Q_Sensi[i];

    MDInitParams();
}

/*
***************************************************************************
*                                      Motion_Detection
*
* Description: do motion detection according to MD mode 
*			
*
* Arguments  : framelen: the length of the frame
*
* Return     :  None                    
*
* Note(s)    :
*						
*            
***************************************************************************
*/
void	Motion_Detection(int framelen)
{
    if(gMdType == mdframesize)
        MdFramesize(framelen);
}

/*
***************************************************
*                                          MDInit()
***************************************************
*/
void MDInit(int sensi)
{
    if(gMdType == mdframesize)
        MDFrameSizeInit(sensi);
}

#endif

/***********************************temporal difference***********************************/
unsigned int sens_level[10][5] = {{LVMIN+10000, 1, 5, 1, 2}, {LVMIN+8000, 2, 5, 1, 2}, {LVMIN+7000, 3, 4, 1, 2}, {LVMIN+6000, 4, 4, 1, 2}, {LVMIN+5000, 5, 3, 1, 1},
                                                    {LVMIN+4000, 6, 3, 1, 1}, {LVMIN+3000, 7, 2, 1, 1}, {LVMIN+2000, 8, 2, 1, 1}, {LVMIN+1000, 9, 2, 1, 1}, {LVMIN, 10, 1, 1, 1}};
unsigned int lvThresh = 6000;
unsigned int hori_block = MAX_HORI_BLOCK;
unsigned int verti_block = MAX_VERT_BLOCK;
unsigned int hori_cell = (MAX_HORI_BLOCK +1)/2;
unsigned int verti_cell = (MAX_VERT_BLOCK + 1)/2;
tMaskRange maskrange = {0, 0, 0, 0};
unsigned int blockcell = 0;           //1: use cell as a unit(contain 4 block), 0: use block as a unit.
/*
*********************************************************************************************
*                                      SetMDMask()
* argument: startx: the start x index of blocks
*                 starty: the start y index of blocks
*                 endx:  the end x index of blocks
*                 endy:  the end y index of blocks
* return: 
* note:       x: horizontal direction; y: vertical direction
*********************************************************************************************
*/
void SetMDMask(int startx, int starty, int endx, int endy)
{
    int i, j;

    if(startx < 0)
        startx = 0;
    if(starty < 0)
        starty = 0;
    if(endx < 0)
        endx = 0;
    if(endy < 0)
        endy = 0;
    if(startx > MAX_IMAGE_WIDTH / BLOCK_WIDTH)
        startx = MAX_IMAGE_WIDTH / BLOCK_WIDTH;
    if(endx > MAX_IMAGE_WIDTH / BLOCK_WIDTH)
        endx = MAX_IMAGE_WIDTH / BLOCK_WIDTH;
    if(starty > MAX_IMAGE_HEIGHT / BLOCK_HEIGHT)
        starty = MAX_IMAGE_HEIGHT / BLOCK_HEIGHT;
    if(endy > MAX_IMAGE_HEIGHT / BLOCK_HEIGHT)
        endy = MAX_IMAGE_HEIGHT / BLOCK_HEIGHT;

    maskrange.startx = (startx < endx)? startx : endx;
    maskrange.starty = (starty < endy)? starty : endy;
    maskrange.endx = (startx > endx)? startx : endx;
    maskrange.endy = (starty > endy)? starty : endy;

    for(i=maskrange.starty; i<maskrange.endy; i++)    //if startx == endx, then no block is masked
        for(j=maskrange.startx; j<maskrange.endx; j++)  //if starty == endy, then no block is masked
            gMdBlockMask[i][j] = 1;
    printf("ucos set MD mask range, startpoint=(%d,%d), endpoint=(%d,%d)\n", maskrange.startx, maskrange.starty, maskrange.endx, maskrange.endy);
}

/*********************************************************************************************
*                                            SetMDMask_AreaVectors
* Notes: each cell's mask information is stored in memory, the start address is gpAreaVectorAdddr, the
* max cell number is 132((704/32*576/24)/4), if horizontal block number is odd, then the last one or two
* is see as a cell, for vertical, it's the same.
* 1: masked; 0: not masked
**********************************************************************************************/
unsigned int using_areavectors = TRUE;
void SetMDMask_AreaVectors(void)
{
    int hor_cell_num, ver_cell_num;
    unsigned char *pdata;
    int i, j;

    for(i=0; i<MAX_VERT_CELL; i++)
        for(j=0; j<MAX_HORI_CELL; j++)
            gMdCellMask[i][j] = 0;

    if(gpAreaVectorAddr == 0) {
        printf("area vector address error=0x%x\n", gpAreaVectorAddr);
        printf("not use motion detection area vectors\n");
        using_areavectors = FALSE;
        return;
    }
    printf("set motion detection area vectors:");
    using_areavectors = TRUE;
    pdata = (unsigned char *)gpAreaVectorAddr;

    hor_cell_num = (gImageWidth+BLOCK_WIDTH) / BLOCK_WIDTH / 2;
    ver_cell_num = (gImageHeight+BLOCK_HEIGHT) / BLOCK_HEIGHT / 2;
    printf("imagesize=%d * %d, hor_cell=%d, ver_cell=%d\n", gImageWidth, gImageHeight, hor_cell_num, ver_cell_num);

    for(j=0; j<ver_cell_num; j++)
    {
        for(i=0; i<hor_cell_num; i++)
        {
            if(*(pdata + j*hor_cell_num + i) == 0)
                gMdCellMask[j][i] = 1;           //masked
            printf("%d, ", *(pdata+j*hor_cell_num+i));
        }
        printf("\n");
    }
}

/*********************************************************************************************
*                                                           NOTE!
* ClearMotionRange(), DrawRectangle(), DrawMotionRange() is for the use of ucos loopback
*
* and OSD should be opened.
**********************************************************************************************/

unsigned int imgstartx = 0;
unsigned int imgstarty = 0;
unsigned int unitwidth = 64;
unsigned int unitheight = 48;
unsigned int blockcelldataaddr = (unsigned int)&gMdCellStatus[0][0];

#ifdef MD_UCOS_LOOPBACK                //this macro difined in motion_detection.h
void ClearRectangle(int sx, int sy, int ex, int ey)
{
    gtClearScreenSetting clearrgn;

    if((sx + imgstartx) > 32)
        sx += imgstartx - 32;
    ex += imgstartx - 32;
    sy += imgstarty;
    ey += imgstarty;

    clearrgn.attribute = 0;
    clearrgn.line_start =sy;
    clearrgn.pixel_start = sx;
    clearrgn.height = ey - sy + 1;
    clearrgn.width = ex - sx + 1;
    if(gtDisplayFormat.vout_format == NTSC)
    {
        if((clearrgn.line_start + clearrgn.height) > 480)
            clearrgn.height = 480 - clearrgn.line_start;
    }
    else if(gtDisplayFormat.vout_format == PAL)
    {
        if((clearrgn.line_start + clearrgn.height) > 576)
            clearrgn.height = 576 - clearrgn.line_start;
    }
    if(clearrgn.pixel_start + clearrgn.width > 704)
        clearrgn.width = 704 - clearrgn.pixel_start;

    Vb2100ClearScreen(0, 0, clearrgn);
}

void ClearMotionRange(void)
{
    int startx = 0, endx = 0;
    int starty = 0, endy = 0;
    int w,h,maxw;
    int i, j;

    if(blockcell == 0) {
        w = hori_block;
        h = verti_block;
        maxw = MAX_HORI_BLOCK;
    }else {
        w = (hori_block + 1) / 2;
        h = (verti_block + 1) / 2;
        maxw = MAX_HORI_CELL;
    }
    
    for(i=0; i<h; i++)
    {
        startx = endx = 0;
        starty = endy = i * unitheight;
        for(j=0; j<w; j++)
        {
            if(*((char *)blockcelldataaddr+i*maxw+j) == 1) {   
                if(endx == 0) {                   //first motion cell
                    startx = j * unitwidth;
                    endx = startx + unitwidth;
                    starty = i * unitheight;
                    endy = starty + unitheight;
                }else {
                    if(endx == j * unitwidth) {
                        endx = endx + unitwidth;
                    }else {
                        ClearRectangle(startx, starty, endx, endy);
                        startx = endx = 0;
                    }
                }
            }
        }
        if(endx != 0)
            ClearRectangle(startx, starty, endx, endy);
    }

}

void DrawRectangle(int sx, int sy, int ex, int ey)
{
    gtDrawLineSetting line;
    int i;

    if((sx + imgstartx) > 32)
        sx += imgstartx - 32;
    ex += imgstartx - 32;
    sy += imgstarty;
    ey += imgstarty;

    line.attribute = 0;
    line.start_x_pos = sx;
    line.start_y_pos = sy;
    line.end_x_pos = ex;
    line.end_y_pos = sy;
    Vb2100DrawLine(0, 0, line);

    line.start_y_pos = ey;
    line.end_y_pos = ey;
    Vb2100DrawLine(0, 0, line);

    line.start_x_pos = line.end_x_pos = sx;
    line.start_y_pos = sy;
    line.end_y_pos = ey;
    i = (ex - sx) / unitwidth;
    do {
        line.start_x_pos = line.end_x_pos = sx + i * unitwidth;
        Vb2100DrawLine(0, 0, line);
    }while(i--);
}

void DrawMotionRange(void)
{
    int startx = 0, endx = 0;
    int starty = 0, endy = 0;
    int w,h,maxw;
    int i, j;

    if(blockcell == 0) {
        w = hori_block;
        h = verti_block;
        maxw = MAX_HORI_BLOCK;
    }else {
        w = (hori_block + 1) / 2;
        h = (verti_block + 1) / 2;
        maxw = MAX_HORI_CELL;
    }
    
    for(i=0; i<h; i++)
    {
        startx = endx = 0;
        starty = endy = i * unitheight;
        for(j=0; j<w; j++)
        {
            if(*((char *)blockcelldataaddr+i*maxw+j) == 1) {   
                if(endx == 0) {                   //first motion cell
                    startx = j * unitwidth;
                    endx = startx + unitwidth;
                    starty = i * unitheight;
                    endy = starty + unitheight;
                }else {
                    if(endx == j * unitwidth) {
                        endx = endx + unitwidth;
                    }else {
                        DrawRectangle(startx, starty, endx, endy);
                        startx = endx = 0;
                    }
                }
            }
        }
        if(endx != 0)
            DrawRectangle(startx, starty, endx, endy);
    }

}

void GetImgStartPoint(void)
{
    imgstartx = (704 - gImageWidth) / 2;
    
    if(gtDisplayFormat.vout_format == NTSC)
        imgstarty = (480 -gImageHeight) / 2;
    else if(gtDisplayFormat.vout_format == PAL)
        imgstarty = (576 - gImageHeight) / 2;
    if((gImgEncodeMode==ENCODEDOWN) && (gImage_Mode==EVENODD_IMAGE) && (gtVinFormat.interlaced==TRUE))
        imgstarty += gImageHeight / 4;
    if((gImgEncodeMode==ENCODEUP) && (gImage_Mode==EVENODD_IMAGE) && (gtVinFormat.interlaced==TRUE))
        imgstarty += gImageHeight / 4;
}
#endif
/******************************************************************************/
/*
*********************************************************************************
*                                   MDTemporalDif()
*
* arguments:  the pointer of the start address that the Y difference of successive frames saved 
*
* return: null
*
* note:
*
*********************************************************************************
*/
void MDTemporalDif(int *data)
{
    unsigned int i, j, k = 0;
    int moveflag;
    int value;
    unsigned int nblockmove = 0;
    unsigned int ncellmove = 0;
    static unsigned int nframemotion = 0;

    moveflag = 0;
    
#ifdef MD_UCOS_LOOPBACK
    ClearMotionRange();
#endif
    
    //setup the status of each block
    for(i=0; i<verti_block; i++)
    {
        for(j=0; j<hori_block; j++)
        {
            value = *data++;
            if(value < 0)  value = -value;
            if(value < lvThresh)
                gMdBlockStatus[i][j] = 0;
            else {
                if(gMdBlockMask[i][j] == 1) {
                    //printf("(%d,%d)masked)", i, j);
                    gMdBlockStatus[i][j] = 0;
                }else {
                    gMdBlockStatus[i][j] = 1;
                    nblockmove++;
#ifdef DEBUG_MD
                    printf("(%d,%d,value=%d)", i, j, value);
#endif

                }
            }
        }
    }

    if(blockcell == 0)        //use block as a unit
    {
        for(i=0; i<verti_block; i++)        
        {
            for(j=0; j<hori_block; j++)
            {
                if(gMdBlockStatus[i][j] == 1) {
                    ncellmove++;
                    printf("(%d,%d)", i, j);
                }
            }
        }
    }
    else                       //use cell as a unit(each contain 4 blocks)
    {
    //setup the status of each cell according to each sub-cell(block)
        for(i=0; i<verti_block; i+=2)        
        {
            for(j=0; j<hori_block; j+=2)
            {
                if(gMdCellMask[i>>1][j>>1] == 1) {  //this cell is masked
#ifdef MD_UCOS_LOOPBACK
                    gMdCellStatus[i>>1][j>>1] = 0;
#else
                    *((unsigned char *)blockcelldataaddr + (i>>1)*hori_cell + (j>>1)) = 0;
#endif
                    continue;
                }
            
                if(gMdBlockStatus[i][j] == 1)
                    k++;

                if(((j+1) < hori_block) && ((i+1) < verti_block)) {
                    if(gMdBlockStatus[i][j+1] == 1)
                        k++;
                    if(gMdBlockStatus[i+1][j] ==1)
                        k++;
                    if(gMdBlockStatus[i+1][j+1] == 1)
                        k++;

                    if(k >= gMdCelsens) {                          //set cell status
                        gMdCellStatus[i>>1][j>>1] = 1;
                        ncellmove++;
                    }else {
                        gMdCellStatus[i>>1][j>>1] = 0;
                    }
                }else {
                    if(((j+1) == hori_block) && ((i+1) < verti_block)) {       //right boundary
                        if(gMdBlockStatus[i+1][j] ==1)
                            k++;
                    }else if(((j+1) < hori_block) && ((i+1) == verti_block)) { ////down boundary
                        if(gMdBlockStatus[i][j+1] == 1)
                            k++;
                    }

                    if(k >= 1) {
                        gMdCellStatus[i>>1][j>>1] = 1;
                            ncellmove++;
                    }else {
                        gMdCellStatus[i>>1][j>>1] = 0;
                    }
                }
                
#ifdef MD_UCOS_LOOPBACK                   
                if(k >= 1) {                                          //set cell status
                    gMdCellStatus[i>>1][j>>1] = 1;
                    ncellmove++;
                    printf("(%d,%d)", i>>1, j>>1);
                }else {
                    gMdCellStatus[i>>1][j>>1] = 0;
                }
#else
                if(k >= 1) {                                          //set cell status
                    *((unsigned char *)blockcelldataaddr + (i>>1)*hori_cell + (j>>1)) = 1;
                    ncellmove++;
                    printf("(%d,%d)", i>>1, j>>1);
                }else {
                    *((unsigned char *)blockcelldataaddr + (i>>1)*hori_cell + (j>>1)) = 0;
                }
#endif
                k = 0;
            }
        }
    }

#ifdef DEBUG_MD
    printf("nblock=%d, ncell=%d\n", nblockmove, ncellmove);
#endif

    if(ncellmove >= gMdSpsens) {            //spatial sensitivity
        nframemotion++;
        if(nframemotion >= gMdTmpsens)  //temporal sensitivity
            moveflag = 1;
    }else {
        nframemotion = 0;
    }

    if(moveflag) {
#ifdef SEND_MD_TO_LINUX
#if 0                                      //becase has not tested yet.
        if(using_areavectors == TRUE)
            MDSendStatusToLinux();                  //send area vector to linux.
        else
            MDSendPacketToLinux(moveflag);   //
#else
        MDSendPacketToLinux(moveflag);   //
#endif
#endif
        gMDPlayTime = gtMDFrameRateCtrl.motionduration;
        printf("\nmotion detected!~~~~~~~~~~~~~~~~\n");
    }else {
        if(gMDPlayTime == 0) {
            MDSendPacketToLinux(3);              //send motion stop report to linux
            gMDPlayTime = -1;                         //to prevent sending message all the time
            printf("motion stopped``````````````\n");
        }
    }
    
#ifdef MD_UCOS_LOOPBACK
    DrawMotionRange();
#endif
    
}
/*
******************************************
*
******************************************
*/
void MDTemporalDifInit(int sensi)
{
    int i, j;

    if(sensi <= 0) {
        gIsMDEnabled = FALSE;
        gMotionSensi = 0;
        printf("Motion Detect disabled, sensitivity=%d!\n", sensi);
        return;
    }

    gIsMDEnabled = TRUE;
    if(sensi > 100) 
        gMotionSensi = 100;
    else
        gMotionSensi = sensi;

    i = (gMotionSensi - 1) / 10;
    lvThresh = sens_level[i][0];           //lum level threshold
    gMdLvsens = sens_level[i][1];
    gMdSpsens = sens_level[i][2];
    gMdCelsens = sens_level[i][3];
    gMdTmpsens = sens_level[i][4];
    gMdSpeed = 1;

    gMDPlayTime = -1;
#ifdef MD_UCOS_LOOPBACK
    for(i=0; i<MAX_VERT_BLOCK; i++)
        for(j=0; j<MAX_HORI_BLOCK; j++)
            gMdBlockMask[i][j] = 0;
        
    for(i=maskrange.startx; i<maskrange.endx; i++)
        for(j=maskrange.starty; j<maskrange.endy; j++)
            gMdBlockMask[i][j] = 1;
#else
    SetMDMask_AreaVectors();
#endif

    hori_block = gImageWidth / BLOCK_WIDTH;
    verti_block = gImageHeight / BLOCK_HEIGHT;
    
    if((gImgEncodeMode==ENCODEDOWN) && (gImage_Mode==EVENODD_IMAGE) && (gtVinFormat.interlaced==TRUE))
        verti_block = verti_block / 2;
    if((gImgEncodeMode==ENCODEUP) && (gImage_Mode==EVENODD_IMAGE) && (gtVinFormat.interlaced==TRUE))
        verti_block = verti_block / 2;

    hori_cell = (hori_block + 1) / 2;
    verti_cell = (verti_block + 1) / 2;

#ifdef MD_UCOS_LOOPBACK
    if((hori_block <= 5) && (verti_block <= 6))
        blockcell = 0;
    else
#else
        blockcell = 1;              //because has no protocal with linux, so always set to 1
#endif

#ifdef MD_UCOS_LOOPBACK     //this macro defined in motion_detection.h
    GetImgStartPoint();
    if(blockcell == 0) {
        unitwidth = 32;
        unitheight = 24;
        blockcelldataaddr = (unsigned int)&(gMdBlockStatus[0][0]);
    }else {
        unitwidth = 64;
        unitheight = 48;
        blockcelldataaddr = (unsigned int)&(gMdCellStatus[0][0]);
    }
#endif
}

#ifdef DEBUG_PPE_PERFORMANCE
unsigned int start = 0;
unsigned int end = 0;
#endif
unsigned int ppe_run = 0;    //
tMDFrameRateCtrl gtMDFrameRateCtrl = {30, 30, 5};
unsigned int gVinSoftwareFrameRate = 0;               //to count the vin software frame rate, not fields  
int gMDPlayTime = -1;                                             //0: no motion occured, encode fps = framerate of no-motion
                                                                              //>0: motion occured, encode fps = framerate of motion
                                                                              //-1: have send motion stop message to linux
int gMDStartDelay = STARTTIME;                  //seconde: when VIN opened, wait vin to keep stable
/*
***************************************************************************
*                                      TaskMotionDetection()
*
* Description: 
*			
*
* Arguments  : 
*
* Return     :  None                    
*
* Note(s)    :
*						
*            
***************************************************************************
*/
void TaskMotionDetection(void *data)
{
    unsigned int lastframeaddr;
    unsigned int *vinaddr;
    char err;

    gpMboxPpeNewVinFrame = OSMboxCreate((void *)0);
    gpVinFrameAddr = 0;
    gMDVinFirstFrame = 0;
    
    HwPpeIntrInit();
    HwPpeInitY();
    
    while(1)
    {
        //wait for new vin frame
        vinaddr = OSMboxPend(gpMboxPpeNewVinFrame, 0, &err);

        if(gMDStartDelay > 0) {
            //printf("a");
            continue;
        }

        if(gMDVinFirstFrame == 0) {
            lastframeaddr = *vinaddr;
            gMDVinFirstFrame = 1;
            continue;
        }

#ifdef DEBUG_PPE_PERFORMANCE
        start = GetCount();
#endif

        if(ppe_run == 1) {  //ppe compare y and ucos processing data is not finished yet ?
            lastframeaddr = *vinaddr;
            continue;
        }

        if((gImgEncodeMode==ENCODEDOWN) && (gImage_Mode==EVENODD_IMAGE) && (gtVinFormat.interlaced==TRUE))
        {
            HwPpeCompareY(lastframeaddr, *vinaddr, gImageWidth, gImageHeight / 2);
        }
        else if((gImgEncodeMode==ENCODEUP) && (gImage_Mode==EVENODD_IMAGE) && (gtVinFormat.interlaced==TRUE))
        {
            HwPpeCompareY(lastframeaddr, *vinaddr, gImageWidth, gImageHeight / 2);
        }
        else {
            HwPpeCompareY(lastframeaddr, *vinaddr, gImageWidth, gImageHeight);
        }

        lastframeaddr = *vinaddr;
        ppe_run = 1;

    }
}
/*
***************************************************************************
*                                      TaskMDProcess()
*
* Description: 
*			
*
* Arguments  : 
*
* Return     :  None                    
*
* Note(s)    :
*						
*            
***************************************************************************
*/
void TaskMDProcess(void *pdata)
{
    volatile int *SpramPtr = (volatile int*)0xbfa03000;
    char err;

#ifdef DEBUG_MD
    unsigned int blocknum = 0;
    unsigned int total = 0; 
    int maxcellvalue = 0;
    int avecellvalue = 0;
    int value;
    unsigned int i;

    blocknum = gImageWidth / 32 * gImageHeight / 24;
#endif

    gpSemPpeCompareYFinished = OSSemCreate(0);
    MDTemporalDifInit(gMotionSensi);

    while(1)
    {
         //wait for Y compare finished
        OSSemPend(gpSemPpeCompareYFinished, 1000 * OS_TICKS_PER_SEC /1000, &err);  //sometimes framerate is too low, so 1s may be enough

        switch(err)
        {
            case OS_TIMEOUT:
                if(ppe_run == 1) {
                    if(rdl(PPE_REGS_ADDR(PPE_DEBUG)) & 0x80000000) {  //see if program is stopped
                        printf("PPE CPU program stopped\n");
                    }else {
                        printf("PPE compare Y timeout!\n");
                        break;
                    }
                }else {
                    break;
                }

            case OS_NO_ERR:

#ifdef DEBUG_MD
                for(i=0; i<blocknum; i++)
                {
#if 0
                    printf("%d,", SpramPtr[i]);
                    if((i+1) % 20 == 0)
                    printf("\n");
#endif

                    if(SpramPtr[i] < 0) 
                        total -= SpramPtr[i];
                    else 
                        total += SpramPtr[i];
                    value = SpramPtr[i];
                    if(value < 0) value = -value;
                    if(value > maxcellvalue)
                        maxcellvalue = value;
                }

                avecellvalue = total / blocknum;
                printf("totalblock=%d, average=%d, maxvalue=%d\n", blocknum, avecellvalue, maxcellvalue);
#endif

                MDTemporalDif(SpramPtr);             //process motion detection

                memset(SpramPtr, 0x0, 0x1000);    //clear Y difference value

#ifdef DEBUG_PPE_PERFORMANCE
                end = GetCount();
                if(end > start)
                    end = end - start;
                else
                    end = CPU_MAIN_CLOCK / (2 * OS_TICKS_PER_SEC) + end - start; 
                printf("<ppe=%d>\n", end / (CPU_MAIN_CLOCK/(2*1000000)));    // counted by microsecond
#endif
                break;

            default:
                printf("OSSemPend gpSemPpeCompareYFinished error=%d\n", err);
                break;
        }
        ppe_run = 0;
    }
}

#endif        
