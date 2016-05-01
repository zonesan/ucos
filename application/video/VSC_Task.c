/*
***************************************************************************
*                  Copyright (c) 2006 vBridge Microsystem, Inc.  
*                        Unpublished & Not for Publication
*                               All Rights Reserved                   
*   
* File        :                      
*                                                                         
* Description :    
*             
* Date        :                                                      
***************************************************************************
*/


#include "video_cfg.h"
#ifdef DISPLAY_INCLUDE

#include "VSC_Task.h"
#include "VSC_API.h"
#include "adv7171.h"
#include "two_risc_video.h"
#include "2risc_memory_map.h"
#include "mpeg4_decoder.h"
#include "two_risc_display.h"
#include "VIN_SET.h"

extern tDisplayFormat gtDisplayFormat;

extern unsigned int gPipEnable;
    
void Video_VDE_RegInit(void);
void Video_VOUT_RegInit(void);

OS_EVENT* gSem_m4d_finished = 0;
OS_EVENT* gMbox_linux2ucos = 0;
OS_EVENT* gMbox_rvo_w = 0;
OS_EVENT* gMbox_rvo_r = 0;
static int gsPipOpened = 0;
static int gsRvoOpened = 0;
extern unsigned int gVoutClkSet;
void VscInit()
{
    *(volatile int *)0xa8010038 = 0x840;    // reset vsc & vout
    *(volatile int *)0xa8010038 = 0x000;    // no reset vsc & vout
    gVoutClkSet = FALSE;      //bacause clock has been reset, so clear the flag.
    
    rSC_IENA_WS |= 0x1c000;    // enable vsc write streams intrs    
    display_no = 0;
    if(gRvoEnable)
        AppRvoInitialize(gtRvoCurrent.width, gtRvoCurrent.height, gtRvoCurrent.format, gtRvoCurrent.interlaced, gtRvoCurrent.pixel_ratio, gtRvoCurrent.line_ratio);

#ifdef PIP_ENABLE
    if(gPipEnable)
        AppPipInitialize(gImageWidth, gImageHeight, gtPipCurrent.format, gtPipCurrent.interlaced, gtPipCurrent.pixel_ratio, gtPipCurrent.line_ratio);
#endif

#ifdef OSD_ENABLE
    if(gOsdEnable)
        AppOSDInitialize(gtOsdCurrent.width, gtOsdCurrent.height, gtOsdCurrent.start_pixel, gtOsdCurrent.start_line, gtOsdCurrent.backgroundColor, gtOsdCurrent.OSD_T_Mask, gtOsdCurrent.fill_addr);        //add by zjb Dec 1st
#endif    

    AppVscInitialize(gtRvoCurrent.width, gtRvoCurrent.height, 0, 0, 0, 0);

    Video_VDE_RegInit();																												
    Video_VOUT_RegInit();
    CctlVoutClkSet();

    //if (VDE_CTL & 0x40000000)  //if RVO is enable, open RVO stream
    if (gRvoEnable)  {  //if RVO is enable, open RVO stream
        EN_VDE_RVO();
        ScOpen(VSC_RD_Y0_S);
        ScOpen(VSC_RD_U0_S);
        ScOpen(VSC_RD_V0_S);
    } else {
        DIS_VDE_RVO();
    }

#ifdef PIP_ENABLE
    //if (VDE_CTL & 0x20000000)  //if PIP is enable, open PIP stream
    if (gPipEnable) {    //if PIP is enable, open PIP sream{    
        EN_VDE_PIP();
        ScOpen(VSC_RD_Y1_S);
        ScOpen(VSC_RD_U1_S);
        ScOpen(VSC_RD_V1_S);
    } else {
        DIS_VDE_PIP();
    }
#else
    DIS_VDE_PIP();
#endif    

#ifdef OSD_ENABLE
    //if (VDE_CTL & 0x10000000)   //if OSD is enable, open RVO stream
    if (gOsdEnable) {    // if OSD is enable, open RVO stream
        EN_VDE_OSD();
        ScOpen(VSC_RD_OSD_S);
    } else {
        DIS_VDE_OSD();
    }
#else    
    DIS_VDE_OSD();
#endif    

}

void OpenRvo()
{
    gsRvoOpened = 1;
}

void OpenPip()
{
    gsPipOpened = 1;
}

int IsRvoOpened()
{
    return gsRvoOpened;
}

int IsPipOpened()
{
    return gsPipOpened;
}

int IsRvoEnabled()
{
    return (VDE_CTL & 0x40000000);
}

int IsPipEnabled()
{
    return (VDE_CTL & 0x20000000);
}

int IsOsdEnabled()
{
    return (VDE_CTL & 0x10000000);
}

#if 0
/******************************************************************************
* VideoISR
* Discription:        It's SC interrupt handler. contians vin, vout, vsc interrupt handler.
******************************************************************************/
static void VideoISR(void)
{
    unsigned int y_done = 0, u_done = 0, v_done = 0;

    if ( (*(volatile unsigned int*)(0xa8040008) & 0x4000) != 0) 
        y_done = 1;
    if ( (*(volatile unsigned int*)(0xa8040008) & 0x8000) != 0)
        u_done = 1;
    if ( (*(volatile unsigned int*)(0xa8040008) & 0x10000) != 0)
        v_done = 1;

    if (y_done && u_done && v_done) 
        {
        y_done = u_done = v_done = 0;
        VscInterruptHandler();
        }
}

/******************************************************************************
* VscInterruptHander
* Discription:        When Y, U, V three streams all sended to Vout, Vsc Interrupt is generated
******************************************************************************/



unsigned int vsc_y_0[640*480 * 3 / 4];   
   

void VscInterruptHandler(void)
{        
    *(volatile unsigned *)(0xa8040008)|= 0x1c000;        // Clear interrupt status
    static VideoFrame old_mem = {0};
    static VideoFrame* pWritable_mem = 0;    
    VideoFrame rvoFrame[2] [3]= {{RVO_Y_memory_0, RVO_U_memory_0, RVO_V_memory_0},
                                {RVO_Y_memory_1, RVO_U_memory_1, RVO_V_memory_1}};                          
    static int displayedRvoFrameN = 0;
    VideoFrame* vframe = 0;
    
    if (IsRvoEnabled()) 
        {
        //MPEG4
        /*
        vframe = (VideoFrame *)OSMboxAccept(gMbox_rvo_r);
        printf("Accept gMbox_rvo_r\r\n");
        if (vframe) 
            {
            McVideoInit(VSC_RD_Y0_M, 0, 0, vframe->y_mem);
            McVideoInit(VSC_RD_U0_M, 0, 0, vframe->u_mem);
            McVideoInit(VSC_RD_V0_M, 0, 0, vframe->v_mem);
            old_mem.y_mem = vframe->y_mem;
            old_mem.u_mem = vframe->u_mem;
            old_mem.v_mem = vframe->v_mem;
            // If mem_0 is readable, post mem_1. Vice versa.
            pWritable_mem = (vframe->y_mem == RVO_Y_memory_0) ? &rvoFrame[1]: &rvoFrame[0];                    
            OSMboxPost_w(gMbox_rvo_w, &pWritable_mem);
            displayedRvoFrameN++;
            }
        else 
            {
            // Display old_mem picture
            ASSERT(old_mem.y_mem != 0);
            McVideoInit(VSC_RD_Y0_M, 0, 0, old_mem.y_mem);
            McVideoInit(VSC_RD_U0_M, 0, 0, old_mem.u_mem);
            McVideoInit(VSC_RD_V0_M, 0, 0, old_mem.v_mem);
            }
            
        //MPEG4
        */
            
            McVideoInit(VSC_RD_Y0_M, 0, 0, VIN_Y_memory_0);
            McVideoInit(VSC_RD_U0_M, 0, 0, VIN_U_memory_0);
            McVideoInit(VSC_RD_V0_M, 0, 0, VIN_V_memory_0);
            
            ScOpen(VSC_RD_Y0_S);
            ScOpen(VSC_RD_U0_S);
            ScOpen(VSC_RD_V0_S);        
        }
    if (IsPipEnabled()) 
        {    
            if (VinBufferNumber == 0)
                {
                McVideoInit(VSC_RD_Y1_M, 0, 0, VIN_Y_memory_0);
                McVideoInit(VSC_RD_U1_M, 0, 0, VIN_U_memory_0);
                McVideoInit(VSC_RD_V1_M, 0, 0,VIN_V_memory_0);
                }
            else if (VinBufferNumber == 1)
                {
                McVideoInit(VSC_RD_Y1_M, 0, 0, VIN_Y_memory_1);
                McVideoInit(VSC_RD_U1_M, 0, 0, VIN_U_memory_1);
                McVideoInit(VSC_RD_V1_M, 0, 0,VIN_V_memory_1);
                }
            ScOpen(VSC_RD_Y1_S);
            ScOpen(VSC_RD_U1_S);
            ScOpen(VSC_RD_V1_S);
        }
    if (IsOsdEnabled()) 
        {
            McVideoInit(VSC_RD_OSD_M, 0, 0, OSD_memory);
            ScOpen(VSC_RD_OSD_S);
        }

    // Start Vsc itself            
    ScOpen(VSC_WR_Y_S);
    ScOpen(VSC_WR_U_S);
    ScOpen(VSC_WR_V_S);             
}



/******************************************************************************
* DisplayTaskInit
* Param:     none
* Return:        none
* Discription:        Do task initial things including Enable Interrupt, call mpeg4DecoderInit(), 
*                VinInit(), VscInit() if necessary. And set global variary gFrameCount to 0
* Note:            
******************************************************************************/
void VscTaskInit()
{
    //RiscIntrInit();            // Already done in VIN interupt
    //MipsIntrReqEn(IP2);        // Already done in VIN interupt
    //RiscIntrEn(SC_INTR);        // Already done in VIN interupt
    //*(unsigned int *)(RISC0_FOURTH_VECTOR_B_ADDR + SC_BLOCK * 16) = (unsigned int)&VideoISR;     // Already done in VIN interupt
    //MipsIntrEn();                  // Already done in VIN interupt

    if (IsRvoEnabled())        //mpeg4
    {
    mpeg4DecoderInit(0);        //mpeg4
    printf("Mpeg4DecoderInit\r\n");
    }
    
}



void VscTASK(void *data)
{
    memblock_t M4E_data;
    memblock_t* pM4E_data = 0;
    tVideoInFrame* pVin_data = 0;
    VideoFrame* pM4dFrame = 0;
    INT8U tmp;
    int m4dStarted = 0;
    int VSCstarted = 0;
    int i=0;
    
       gMbox_rvo_r = OSMboxCreate((void *)0);
    gMbox_rvo_w = OSMboxCreate((void *)0);  
    gSem_m4d_finished = OSSemCreate(0);
    
    tVideoInFrame *ptMsg1;
    INT8U err;
    //mpeg4DecoderIntrInit();        //mpeg4
    VscInit();
    printf("VSC init\r\n");
    //VscTaskInit();
    printf("VSC Task init\r\n");
    AppVscStart();
    
    //VideoFrame initFrameInfo = {RVO_Y_memory_1, RVO_U_memory_1, RVO_V_memory_1};        //mpeg4
    //printf("Before Mpeg4 post gMbox_rvo_w \r\n");
    //OSMboxPost_w(gMbox_rvo_w, &initFrameInfo);  // To M4D                                  //mpeg4
    //printf("Mpeg4 post gMbox_rvo_w \r\n");
    while(1) 
        {

            /*
            //mpeg4
            if (IsRvoEnabled()) 
                {
                //pM4E_data = (memblock_t*)OSMboxAccept(gMbox_linux2ucos);
            
                printf("start while 1\r\n");
                M4E_data.addr = vga_m4v_1frame;
                M4E_data.len = 4356;
                
                if(1)//if (pM4E_data)
                    {
                    printf("pM4e contains data\r\n");
                    Mpeg4DecoderUpdateInAddr(&M4E_data);
                    printf("Pend Mpeg4 in address\r\n");
                    pM4dFrame = (VideoFrame *)OSMboxPend_w(gMbox_rvo_w, 0, &err);
                    printf("Mpeg4 in  address accepted\r\n");
                    Mpeg4DecoderUpdateOutAddr(pM4dFrame);
    
                    // As long as got the memory semaphore, start M4D to write data to memory
                    if (!m4dStarted) 
                        {
                        printf("Mpeg4 decode start\r\n");
                        mpeg4DecoderStart();
                        m4dStarted = 1;
                        }
                    else 
                        {
                        printf("Mpeg4 decode Restart\r\n");
                        mpeg4DecoderStartReOder();
                        }
                        
                
                    // Wait for M4D to finish writing
                    printf("Pend Mpeg4 decode finish\r\n");
                    OSSemPend_w(gSem_m4d_finished, 0, &err); 
                    printf("Mpeg4 decode finish\r\n");
                    OSMboxPost_w(gMbox_rvo_r, pM4dFrame);
                    printf("Post gMbox_rvo_r\r\n");
                    if (!VSCstarted)
                        {
                        AppVscStart();
                        printf("VSC start\r\n");
                        VSCstarted = 1;
                        }
                    }
                }
            if (!IsRvoOpened()) 
                {
                ASSERT(pM4dFrame->y_mem == RVO_Y_memory_1);
                OpenRvo();
                }
        //mpeg4
        */
        printf("while 1 end\r\n");
        OSTimeDlyHMSM(0, 0, 1, 0);
        }
}



void M4DISR(void)
{
    unsigned int t;

    // write 1 to the corresponding interrupt status bit to clear the interrupt
    // 21: encode; 22: encode mv; 27: deocde

      if(rRISC_INT_STATUS & (0x1<<21)) 
          {    // encode isr
          printf("Mpeg4 encode intr occur\r\n");
              Mp4ISR();
              }

      if(rRISC_INT_STATUS & (0x1<<27)) 
          {    // decode isr
        printf("Mpeg4 decode intr occur\r\n");
        // clear the RISC interrupt status bit
        t = *(volatile unsigned int*)0xa8000010; //RISC_INT_STATUS
        *(volatile unsigned int*)0xa8000010 = t;
    
        // Tell DisplayTask that M4D has finished
    
        if (OSSemPost(gSem_m4d_finished) != OS_NO_ERR)
            printf("M4DISR() post semaphore to Task() failed.\n");    
              }
}


void mpeg4DecoderIntrInit()
{
    unsigned int isr_base_addr;

    // the fourth-interrupt table address    
    if(RiscRdID()  == RISC0)
        isr_base_addr = RISC0_FOURTH_VECTOR_B_ADDR | 0xa0000000;    // phy-addr is used to avoid flush cache
    else
        isr_base_addr = RISC1_FOURTH_VECTOR_B_ADDR | 0xa0000000;    // phy-addr is used to avoid flush cache     
    
    *(unsigned int *)(isr_base_addr + 0x120) = (unsigned int)&M4DISR;   
    
    RiscIntrEn(VCODEC_INTR);  // Enable the interrupt of encode
}

#endif
#endif

