/*
***************************************************************************
*                  Copyright (c) 2006 vBridge Microsystem, Inc.  
*                        Unpublished & Not for Publication
*                              All Rights Reserved                   
*   
* File        : Band_adjust.c                     
*                                                                         
* Description : get the preset Q value according to actual bandwidth;
*                 estimate the I and P frame size;
*                 compare an actual size of a frame to the estimate one and decide whether 
*                 the Q should be increased or decreased, or drop a frame.
*             
* Date        : Dec 8, 2006 H
***************************************************************************
*/

#define Band_adjust_GLOBALS

#include "video_cfg.h"
#include "Band_adjust.h"

#ifdef BAND_WIDTH_CONTROL
#include "mpeg4_encoder.h"
#include "two_risc_video.h"
#include "VIN_SET.h"
#include "comm.h"

/*
***************************************************************************
*                             Percent
*
* Description: 
*
* Arguments  : 
*
* Return     :  
*                         
*              
*
* Note(s)    : 
*            
***************************************************************************
*/
unsigned int Percent (unsigned int PassSource, unsigned int PassPercentage)
{
    return (PassSource * PassPercentage /100);
}

/*
***************************************************************************
*                             IPProportionRetrieve
*
* Description: change the gIPProportion dynamicly.
*
* Arguments  : 
*
* Return     :    
* Note(s)    : 
***************************************************************************
*/
static void CalIPProportion(cbr_info *info)
{
    unsigned int tmp;

    if((info->pframesize != 0) && (info->iframenum != 0)) {
        tmp = (info->iframesize * info->pframenum) / (info->pframesize * info->iframenum);
        if(tmp == 0)
            tmp = 1;
        info->ipproportion = tmp;
    }else {
        printf("error occured in CalIPProportion,%d,%d\n", info->pframesize, info->pframenum);
    }
}


/*
***************************************************************************
*                             CalEstimateIPSize()
*
* Description: caculate the esimate P frame size and estimateP frame sizeaccording to bandwidth, 
*                   proportion of I frame size to P frame size, and Frame rate.
*
* Arguments  : 
*
* Return     :  null
*
* Note(s)    : 1. the ipproportion should be known
*                  2.video band width should be known
*                  3. actual frame rate should be known
*            
***************************************************************************
*/
static void  CalEstimateIPSize(cbr_info *info)
{
    unsigned int tmp = 0;
    
    tmp = info->videobw * 1024 / 8;
    if((info->framerate + info->ipproportion) != 1) {
        info->estimatePsize= tmp / ( info->framerate -1 + info->ipproportion);
        info->estimateIsize= info->estimatePsize * info->ipproportion;
    }else {
        printf("fatal error occurd in CalEstimateIpSize().\n");
        printf("framerate=%d, ipproportion=%d\n", info->framerate, info->ipproportion);
    }
    
    if(info->estimatePsize== 0) {
        printf("fatal error occurd  in CalEstimateIPSize().\n");
        printf("bw=%d, framerate=%d,ipproportion=%d\n", info->videobw, info->framerate, info->ipproportion);
    }
}

static void IncreaseQp(unsigned int *qp, char n)
{
    *qp+= n;
    if(*qp > gtcbrinfo.max_qp)
        *qp = gtcbrinfo.max_qp;
    if(*qp == 24)
        *qp++;
}

static void DecreaseQp(unsigned int *qp, char n)
{
    if(*qp > n)
        *qp -= n;
    if(*qp == 24)
        *qp--;
    if(*qp < gtcbrinfo.min_qp)
        *qp = gtcbrinfo.min_qp;
}

/*
*******************************************************************************************
*
*******************************************************************************************
*/
#ifdef DEBUG_CBR    //the macro is defined in band_adjust.h
static void cbrlog(void)
{
    static unsigned int i = 1;
    static unsigned int bw = 0;
    static unsigned int proportion = 0;
    int j;

    bw += gtcbrinfo.usedbw;
    proportion += gtcbrinfo.ipproportion;
    if(i < gtcbrinfo.duration) {
        i++;
        return;
    }
    
    if(gtcbrinfo.cbrframenum > 0)
        gtcbrinfo.total_Q_Ps = (gtcbrinfo.total_Q_Ps * 10) / gtcbrinfo.cbrframenum;
    bw = bw / gtcbrinfo.duration * 8 / 1024;
    proportion = proportion / gtcbrinfo.duration;
    gtcbrinfo.invalideFm_Ps = gtcbrinfo.invalideFm_Ps / gtcbrinfo.duration;
    gtcbrinfo.cbraveframesize = gtcbrinfo.cbrallframesize / gtcbrinfo.cbrframenum;
    for(j=0; j<5; j++)
    {
        gtcbrinfo.staticInt[j] = gtcbrinfo.staticInt[j] / gtcbrinfo.duration;
    }

    printf("encodeframerate=%d, Ifr=%d, Pfr=%d\n", gtcbrinfo.cbrframenum/gtcbrinfo.duration, gtcbrinfo.iframenum, gtcbrinfo.pframenum);
    printf("BW=%d,Pr=%d,Qu=%d,IVF=%d\n", bw, proportion, gtcbrinfo.total_Q_Ps, gtcbrinfo.invalideFm_Ps);
    printf("aveframesize = %d\n", gtcbrinfo.cbraveframesize);
#ifdef NEW_CBR_ALGORITHM
    printf("calmaxskip=%d, actualmaxskip=%d\n", gtcbrinfo.calmaxskip, gtcbrinfo.actualmaxskip);
#else
    printf("[%d, %d, %d, %d, %d]\n",gtcbrinfo.staticInt[0],gtcbrinfo.staticInt[1],gtcbrinfo.staticInt[2],gtcbrinfo.staticInt[3],gtcbrinfo.staticInt[4]);
#endif

    i = 1;
    bw = 0;
    proportion = 0;
    gtcbrinfo.staticInt[0] = 0;
    gtcbrinfo.staticInt[1] = 0;
    gtcbrinfo.staticInt[2] = 0;
    gtcbrinfo.staticInt[3] = 0;
    gtcbrinfo.staticInt[4] = 0;
    gtcbrinfo.total_Q_Ps = 0;
    gtcbrinfo.invalideFm_Ps = 0;
    gtcbrinfo.cbrframenum = 0;
    gtcbrinfo.actualmaxskip = 0;
    gtcbrinfo.calmaxskip = 0;
    gtcbrinfo.cbrallframesize = 0;
}
#endif

/*
***************************************************************************
*                             Q_preset_range_protect
*
* Description: get the up-limit and down-limit of Q, and make sure it remins between
*                   1 - 30, 1 and 30 is included
* Arguments  : QFluctuationUpBound, QFluctuationDownBound
*
* Return     :  
*                         
*              
*
* Note(s)    : 
*            
***************************************************************************
*/      
int set_qp_range( int maxq, int minq )
{
    if(maxq < minq){
        printf("Q set error!\n");
        return 1;
    }

    gtcbrinfo.max_qp= maxq;
    gtcbrinfo.min_qp= minq;  
    
    if(maxq > MAX_Q){
	printf ("Q must be %d~~%d.\r\n", MIN_Q, MAX_Q);
	gtcbrinfo.max_qp= MAX_Q;
    }

    if(minq < MIN_Q){
	printf ("Q must be %d~~%d.r\n", MIN_Q, MAX_Q);
	gtcbrinfo.min_qp= MIN_Q;
    }

    if(gtcbrinfo.cbr_enabled)
        mp4EncodeParams.quant = gtcbrinfo.max_qp;
    
    return 0;
}       		

/*
***************************************************************************************
* 
***************************************************************************************
*/
void Updatecbr(void)
{
    if(gtcbrinfo.cbr_enabled == FALSE)
        return;

    if((gtcbrinfo.iframenum > 0) && (gtcbrinfo.pframenum > 0))
    {
        CalIPProportion(&gtcbrinfo);
        gtcbrinfo.framerate = gtcbrinfo.framenum / (gSkipFrame + 1);
        CalEstimateIPSize(&gtcbrinfo);
    }
    
#ifdef DEBUG_CBR
    cbrlog();
    printf("allocatedbw=%d, u=%d\n", gtcbrinfo.allocatedbw * 8 / 1024, gtcbrinfo.usedbw * 8 / 1024);
#endif

    gtcbrinfo.iframesize= 0;
    gtcbrinfo.iframenum= 0;
    gtcbrinfo.pframesize= 0;
    gtcbrinfo.pframenum= 0;
    gtcbrinfo.allocatedbw= 0;
    gtcbrinfo.usedbw= 0;
    gtcbrinfo.framenum = 0;
}

/*
****************************************************************************************
*                             QAutoAdjust
*
* Description: compare the actual size of an I or P frame to the corresponded frame, and change Q value
*               during an designated arrange, such as: change the Q between 5 and 25.
*
* Arguments  : framesize(byte):  actual size of a frame has been encoded
*                     type: frame type, P or I frame
*
* Return     :    None                    
*
* Note(s)    : 1. when bandwidth is not enough, increase the Qp first
*                  2. if Qp has been reached its largest value, and bandwidth is still not enough, drop frames
*                  3. if need to drop some frames, must keep video smooth
*            
***************************************************************************************
*/
void QAutoAdjust(unsigned int framesize, unsigned int type)        // get the parameter of estimated size of an I frame or a P frame, and the actual size of a frame.     
{
    unsigned int budgetframesize;
    unsigned int percent_index;
    unsigned int skip = 0;
    unsigned int tmp;

    if(gtcbrinfo.estimatePsize == 0) {
        printf("EstimatePsize error!BW=%d,IP=%d,FR=%d\n", gtcbrinfo.videobw, gtcbrinfo.ipproportion, gtcbrinfo.framerate);
        return;
    }
    
    if(type) {                                                          //P frame
        gtcbrinfo.pframenum++;
        gtcbrinfo.pframesize+= framesize;
        budgetframesize = gtcbrinfo.estimatePsize;
    }else {                                                              //I frame
        gtcbrinfo.iframenum++;
        gtcbrinfo.iframesize= framesize;
        budgetframesize = gtcbrinfo.estimateIsize;
    }
    
    tmp = gtcbrinfo.framenum / (gSkipFrame + 1);
    gtcbrinfo.allocatedbw= gtcbrinfo.iframenum * gtcbrinfo.estimateIsize + (tmp - gtcbrinfo.iframenum) * gtcbrinfo.estimatePsize;
    gtcbrinfo.usedbw+= framesize;
    
#ifdef DEBUG_CBR
    gtcbrinfo.total_Q_Ps += mp4EncodeParams.quant;
    gtcbrinfo.cbrframenum++;
    gtcbrinfo.cbrallframesize += framesize;
#endif

#ifdef NEW_CBR_ALGORITHM
    if(gtcbrinfo.usedbw > gtcbrinfo.allocatedbw) {
        skip = (gtcbrinfo.usedbw - gtcbrinfo.allocatedbw) / gtcbrinfo.estimatePsize;
        if(mp4EncodeParams.quant < gtcbrinfo.max_qp) {
            if(skip < 1) {
                gtcbrinfo.qpstep = 1;
                IncreaseQp(&mp4EncodeParams.quant, gtcbrinfo.qpstep);
            }else if(skip <2) {
                gtcbrinfo.qpstep = 2;
                IncreaseQp(&mp4EncodeParams.quant, gtcbrinfo.qpstep);
            }else {
                gtcbrinfo.qpstep = 4;
                IncreaseQp(&mp4EncodeParams.quant, gtcbrinfo.qpstep);
            }
        }else {
            if(skip > 0) {
                gtMpegEncodeThrowFrame.throw_frame_enable = 1;
                if(skip > gtcbrinfo.maxskipframe)
                    gtMpegEncodeThrowFrame.invalid_frame = gtcbrinfo.maxskipframe;
                else
                    gtMpegEncodeThrowFrame.invalid_frame = skip;
                tmp = skip - gtMpegEncodeThrowFrame.invalid_frame;
                if(tmp >= gtcbrinfo.frametoskip)
                    gtcbrinfo.maxskipframe++;
                else {
                    if(gtcbrinfo.maxskipframe > 1)
                        gtcbrinfo.maxskipframe--;
                }
                gtcbrinfo.frametoskip = tmp;
            }else {
                if(gtcbrinfo.maxskipframe > 1)
                gtcbrinfo.maxskipframe--;
            }
        }
    }else {
        DecreaseQp(&mp4EncodeParams.quant, 1);
        if(gtcbrinfo.maxskipframe > 1)
            gtcbrinfo.maxskipframe--;
    }
#ifdef DEBUG_CBR
    if(gtMpegEncodeThrowFrame.invalid_frame >gtcbrinfo.actualmaxskip)
        gtcbrinfo.actualmaxskip = gtMpegEncodeThrowFrame.invalid_frame;
    if(skip > gtcbrinfo.calmaxskip)
        gtcbrinfo.calmaxskip = skip;
    gtcbrinfo.invalideFm_Ps += gtMpegEncodeThrowFrame.invalid_frame;
    if(skip > 0)
        printf("(%d, %d, %d,%d)", skip, gtMpegEncodeThrowFrame.invalid_frame, gtcbrinfo.usedbw, gtcbrinfo.allocatedbw);
#endif

#else         //end of NEW_CBR_ALGORITHM

    if(framesize <= (budgetframesize + Percent(budgetframesize, Percent_10))) {       //<=10%
        if(framesize >= (budgetframesize -Percent(budgetframesize, Percent_10))) {     //-10% ~10%
            if(gtcbrinfo.allocatedbw >= gtcbrinfo.usedbw) {
                DecreaseQp(&mp4EncodeParams.quant, 1);
            }else {
                ;
            }
            percent_index = 1;
        }else {    //<-10%
            percent_index = 0;
            DecreaseQp(&mp4EncodeParams.quant, 1);          			
        }
    }
    else if(framesize < ( budgetframesize + Percent(budgetframesize, Percent_150))) {     //10%~250%                     
        if(framesize < (budgetframesize + Percent(budgetframesize, Percent_50))) {      // 10%~150%
            percent_index = 2;
            if(mp4EncodeParams.quant < gtcbrinfo.max_qp) {
                IncreaseQp(&mp4EncodeParams.quant, 1);
            }else {          //Q has reached its largest value, but still large
                if (gtcbrinfo.allocatedbw < gtcbrinfo.usedbw) {
                    skip = (gtcbrinfo.usedbw - gtcbrinfo.allocatedbw) / gtcbrinfo.estimatePsize;
                }
            }
        }
        else {               //150%~250%
            percent_index = 3;
            if(mp4EncodeParams.quant < gtcbrinfo.max_qp) {		
                IncreaseQp(&mp4EncodeParams.quant, 2);
            }else {        //Q has reached its largest value, but still larger
                if(gtcbrinfo.allocatedbw < gtcbrinfo.usedbw) {
                    skip = (gtcbrinfo.usedbw - gtcbrinfo.allocatedbw) / gtcbrinfo.estimatePsize;
                }
            }
        }
    }
    else{                  //>250%
        percent_index = 4;
        if(mp4EncodeParams.quant < gtcbrinfo.max_qp) {                				
            IncreaseQp(&mp4EncodeParams.quant, 4);
        }else {
            if(gtcbrinfo.allocatedbw < gtcbrinfo.usedbw) {                       
                skip = (gtcbrinfo.usedbw- gtcbrinfo.allocatedbw) / gtcbrinfo.estimatePsize;      
            }
        }
    } 

    tmp = skip + gtcbrinfo.frametoskip;
    if(tmp > 0) {
        gtMpegEncodeThrowFrame.throw_frame_enable = 1;
        if(tmp > gtcbrinfo.maxskipframe)
            gtMpegEncodeThrowFrame.invalid_frame = gtcbrinfo.maxskipframe;
        else
            gtMpegEncodeThrowFrame.invalid_frame = tmp;
        gtcbrinfo.frametoskip = tmp - gtMpegEncodeThrowFrame.invalid_frame;
        if(gtcbrinfo.frametoskip > (tmp - skip))
            gtcbrinfo.maxskipframe++;
        else if(gtcbrinfo.frametoskip < (tmp - skip))
            gtcbrinfo.maxskipframe--;
    }else {
        gtcbrinfo.maxskipframe--;
    }

#ifdef DEBUG_CBR
    gtcbrinfo.staticInt[percent_index]++;
    gtcbrinfo.invalideFm_Ps += gtMpegEncodeThrowFrame.invalid_frame;
    if(skip > 0)
        printf("(%d,%d,%d,%d)", percent_index, skip, gtcbrinfo.usedbw, gtcbrinfo.allocatedbw);
#endif
#endif
}

/*
*****************************************************************************************
*
*
*****************************************************************************************
*/
void cbr_init(unsigned int bw)
{
    gtcbrinfo.bw = bw;

    if(gtcbrinfo.bw < 64) {
        gtcbrinfo.cbr_enabled = FALSE;
        gtcbrinfo.videobw = 0;
        printf("Close CBR\r\n");
        return;
    }else {
        gtcbrinfo.videobw = gtcbrinfo.bw - gtcbrinfo.audiobw - IP_HEAD * 8 / 1024;
        gtcbrinfo.cbr_enabled = TRUE;
        printf("Open CBR\r\n");
    }

    gtcbrinfo.qpstep = 1;
    gtcbrinfo.currentqp = gtcbrinfo.max_qp;
    mp4EncodeParams.quant = gtcbrinfo.max_qp;
    gtcbrinfo.framerate = gFrameRate / (gSkipFrame + 1);
    gtcbrinfo.ipproportion = DefaultProportion;
    CalEstimateIPSize(&gtcbrinfo);

    printf("*******************************************\r\n");
    printf("Video Bandwidth:%d\r\n", gtcbrinfo.videobw);
    printf("Start Q from:%d\r\n", mp4EncodeParams.quant);
    printf("*******************************************\r\n");

    gtcbrinfo.framenum = 0;
    gtcbrinfo.pframenum = 0;
    gtcbrinfo.pframesize = 0;
    gtcbrinfo.iframenum = 0;
    gtcbrinfo.iframesize = 0;
    gtcbrinfo.allocatedbw= 0;
    gtcbrinfo.usedbw= 0;
}

/*
***********************************************************************************
* 
***********************************************************************************
*/
void set_cbr_default(void)
{
    gtcbrinfo.bw = DEFAULT_BANDWIDTH;
    gtcbrinfo.audiobw = 24;                         //may be set according to the audio codec type
    gtcbrinfo.videobw = gtcbrinfo.bw - gtcbrinfo.audiobw - IP_HEAD * 8 /1024;
    if(gtcbrinfo.videobw > 0)
        gtcbrinfo.cbr_enabled = TRUE;
    else
        gtcbrinfo.cbr_enabled = FALSE;
    gtcbrinfo.max_qp= MAX_Q;
    gtcbrinfo.min_qp= MIN_Q;
    gtcbrinfo.qpstep = 1;
    gtcbrinfo.currentqp = MAX_Q;
    gtcbrinfo.maxskipframe = 1;
    gtcbrinfo.frametoskip = 0;

#ifdef DEBUG_CBR
    gtcbrinfo.duration = 10;
    gtcbrinfo.calmaxskip = 0;
    gtcbrinfo.actualmaxskip = 0;
    gtcbrinfo.cbrallframesize = 0;
#endif
}

#endif
