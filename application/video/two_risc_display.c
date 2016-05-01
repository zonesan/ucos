/*
***************************************************************************
*                  Copyright (c) 2005 vBridge Microsystem, Inc.  
*                        Unpublished & Not for Publication
*                               All Rights Reserved                   
*   
* File        : two_risc_display.c                     
*                                                                         
* Description : VSC, MPEG decode integration   
*             
* Date        : Oct 25, 2006
***************************************************************************
*/


#include "video_cfg.h"
#ifdef DISPLAY_INCLUDE

#define   TWO_RISC_DISPLAY_GLOBALS

#include "os_cpu.h"
#include "os_cfg.h"
#include "uCOS_II.h"

#include "mpeg4_decoder.h"

#include "two_risc_video.h"
#include "two_risc_display.h"
#include "2risc_memory_map.h"

#include "m4v.h"


#include "ftuart.h"

#include "Sc.h"

#include "comm.h"
#include "../../video/VIN_API.h"
#include "VIN_SET.h"

#include "video_api.h"

#include "timestamp.h"

#include "2risc_comm_protocol.h"

#include "../../mpeg/mpeg4_intr.h"

#include "VSC_Task.h"
#include "../../video/video_intr.h"
#include "../../video/VSC_API.h"

#include "../../bsp/except.h"

#include "video_buffer.h"
#include "risc.h" 
#include "startup_picture.h"
#include "timestamp.h"
#include "vout_interface.h"

int HwADV7171Initialize(BOOL video_system);

unsigned int linuxToUcosFrameNum = 0;
volatile unsigned int decodeFrameNum = 0;
volatile unsigned int VscFrameNum = 0;

#ifdef AV_SYNC
/*
***************************************************************************
*                         TaskTwoRiscDisplay
*
* Description: 
*
* Arguments  : 
*
* Return     : 
*
* Note(s)    : 
***************************************************************************
*/
void TaskTwoRiscDisplay(void *data)
{
    unsigned int i, cnt = 0, ip_count = 0, t1, tmp;//, tttt[500];
    unsigned char err;
    int error;
    int h263_I_P;                                              //20071129;the value of *(unsigned int *)(mp4DecodeParams.ipt_iaddr+4)
    int head_mpeg_h263;                                 //20071130;the value of *(unsigned int *)(mp4DecodeParams.ipt_iaddr)
    //decode_flag = MPEG_ENCODE;                     //20071129;indicate the encode mode of the bit stream default is MPEG mode
    tVideoLinuxToUcos *ptVideoLinuxToUcos, tVideoLinuxToUcos;
    OS_Q_DATA pdata;

    static unsigned int count, frame_count = 0;

    extern unsigned int gVideoStatus;
    OS_CPU_SR cpu_sr; 

#ifdef VIDEO_JITTER_BUFFER
    VBufInit(&VideoBuffer1);
#endif
    
    manual_err_flag = 0;    
    gtStatistics.cnt_decodetimeout = 0;
    McIntrEn();

    ScInit(SC_VIDEO_LINUX_TO_UCOS, sBufUse_Internal, sDBSize_4Byte, sBufSize_128Byte, sSBSize_4Byte, sMBSize_32Byte, sSwap_no, sLast_Disable, SC_VIDEO_LINUX_TO_UCOS_ADDR);
    ScOpen(SC_VIDEO_LINUX_TO_UCOS);
    
    gMpeg4DecodeOutputBufNo = 0;
    mpeg4IntrInit();   //  init mpeg4(encode, decode, & mv) intr, enable risc vcodec intr       	    
    mpeg4DecoderInit();

    gptMpeg4DecodeOutputReady = OSQCreate(ArrayMsgDecode, 1);   // if there are n buffers, the msg queu can just contain n-2 msgs to avoid read & write confict..   

#ifdef PIP_ENABLE
    gptMqueueVideoInFrame = OSQCreate(arrayMsg, 1);    // if there are n buffers, the msg queu can just contain n-2 msgs to avoid read & write confict..
#endif
    
    /* the state of beginning , 1: open; 0: close*/
    gVoutEnable = 1; 
    gRvoEnable = 1;
    gPipEnable = 0; 
    gOsdEnable = 0; 

    GetVoutInterface();
    VoutInterfaceIntialize(gtDisplayFormat.vout_format);
    /*
    printf("Init Adv7171...");  
    if(HwADV7171Initialize(gtDisplayFormat.vout_format) == SUCCESSFUL)    // 0: NTSC mode,1: pal mode           
        printf("ok\n");
    else 
        printf("failed\n");
    */

    OSTimeDlyHMSM(0, 0, 0, 100);
    VscInit();
    VoutIntrEn();   
    ScIntrEn();    // locate sc ISR, enable risc sc intr.     

#if 1
    /* Vin */    
    printf("\r\nVin");  
    for(i = 0; i <= 0x8; i +=  4)
        printf("\r\n0x%x = 0x%x!", 0xa8050000 + i, *(int *)(0xa8050000 + i));           
#endif

#if 0
    /* Vout */
    printf("\r\nVout"); 
    for(i = 0; i <= 0x10; i +=  4)
        printf("\r\n0x%x = 0x%x!", 0xa8060000 + i, *(int *)(0xa8060000 + i));   	

    /* Vsc */
    printf("\r\nVsc");  	
    for(i = 0; i <= 0x44; i +=  4)
        printf("\r\n0x%x = 0x%x!", 0xa80b0000 + i, *(int *)(0xa80b0000 + i));   

    printf("\r\nVin    exContext: 0x%x = 0x%x!", 0xa8023000, *(int *)0xa8023000);   
    printf("\r\nVout  exContext: 0x%x = 0x%x!", 0xa8023004, *(int *)0xa8023004);    
    printf("\r\nVsc0  exContext: 0x%x = 0x%x!", 0xa8023008, *(int *)0xa8023008);    
    printf("\r\nVsc1  exContext: 0x%x = 0x%x!", 0xa802300c, *(int *)0xa802300c);    
    printf("\r\nOSD  exContext: 0x%x = 0x%x!", 0xa8023010, *(int *)0xa8023010);     
    printf("\r\nVscW exContext: 0x%x = 0x%x!", 0xa8023014, *(int *)0xa8023014);     
    
#endif

#if 0
    /* Decode */
    printf("\r\nDecode\r\n");           
    for(i = 0; i <= 0x58; i+=4 )
        printf("0x%x = 0x%x\r\n", i + 0xa8120200, *(int *)(i + 0xa8120200));    

    printf("0xa8120400 = 0x%x\r\n", *(int *)(0xa8120400));  	
    printf("0xa8120404 = 0x%x\r\n", *(int *)(0xa8120404));  
    printf("0xa8120408 = 0x%x\r\n", *(int *)(0xa8120408));
#endif

    AppVscStart();  
    gptRiscTimerIntrOccur = OSSemCreate(0);
#ifdef NEW_AVSYNC_ALGORITHM
    gptMpegDecodeReadyToStart = OSSemCreate(1);
#endif
    
    gptVideoLinuxToUcosMbox = OSSemCreate(0);   

    // This should be done by linux!
    //RiscClrCNT(VIDEO_LINUX_TO_UCOS_CNT);
    
    /* global parameters initialized */
    gIsFirstVideoOutFrame = 1;    
    gMpeg4DecodeOutputCount = 1;

    frameWaitingToDecode = 0;
    gMpegDecodeReady = DECODER_READY;
    gRiscTimerCnt = 0;
    SetMiniFrameNumInBuf(0);
    SetMaxFrameNumInBuf(16);
    avsync_init(80, 400);
    RiscTimerInit(5000);  
    
    i = 0;  
    gVideoStatus |= tVoutEn;
    
    while(1)
    {
#ifdef NEW_AVSYNC_ALGORITHM
        OSSemPend(gptMpegDecodeReadyToStart, 200*OS_TICKS_PER_SEC/1000, &err);
        switch(err)
        {
            case OS_TIMEOUT:    // reset decoder
                gIsFirstVideoOutFrame = 1;		
                printf("warning, Decode Timeout!");
                gtStatistics.cnt_decodetimeout++;
                RiscDecCNT(VIDEO_LINUX_TO_UCOS_CNT, 1); //release the frame that causes decode timeout.
                ReleaseCursor(&VideoBuffer1, VideoFrameReceive.addr);
                mpeg4DecoderInit();
                break;

            case OS_NO_ERR:
                if(manual_reset_flag) {
                    manual_reset_flag = 0;
                    gIsFirstVideoOutFrame = 1;
                    printf("\r\nmanually reset!\r\n");
                    mpeg4DecoderInit();
                }
                break;

            default:
                printf("TaskTwoRiscDisplay: OSSemPend gptMpegDecodeReadyToStart err!");
                continue;
                break;
        }

        /* wait for linux send video mpeg frame to ucos */ 
        OSSemPend(gptVideoLinuxToUcosMbox, OS_TICKS_PER_SEC, &err);
        switch(err)
        {
            case OS_NO_ERR:
#ifndef VIDEO_JITTER_BUFFER
                if(TwoRiscDisplayRecPacket(SC_VIDEO_LINUX_TO_UCOS, (unsigned int *)&tVideoLinuxToUcos, /* 3 * sizeof(int) */sizeof(tVideoLinuxToUcos)) == FAILED)
                    printf("*");
                if(tVideoLinuxToUcos.len <= sizeof(frameToDecode)) {            
                    tmp = (unsigned int)frameToDecode |0x80000000;
                    cache_writeback_invalidate_D((void *)tVideoLinuxToUcos.addr, tVideoLinuxToUcos.len);
                    memcpy((void *)tmp, (void *)(tVideoLinuxToUcos.addr & 0x1fffffff | 0x80000000), tVideoLinuxToUcos.len);
                    cache_writeback_D(tmp, tVideoLinuxToUcos.len);
                    tVideoLinuxToUcos.addr = tmp;   
                } else {
                    printf("\nTaskTwoRiscDisplay: warning: framesize(%d) exceeds size of frameToDecode(%d)\n", tVideoLinuxToUcos.len, sizeof(frameToDecode));
                }
#else   //use buffer
                error = VBufGetOneFrame(&VideoBuffer1, &VideoFrameReceive);
                if(error){
                    printf("ucos VBufGetOneFrame error! err = %d\n", error);
                    if(OSSemPost(gptMpegDecodeReadyToStart) != OS_NO_ERR)
                        printf("*");
                    continue;
                }
                tVideoLinuxToUcos.addr = VideoFrameReceive.addr;
                tVideoLinuxToUcos.len = VideoFrameReceive.size;
                tVideoLinuxToUcos.tstamp = VideoFrameReceive.TimeStamp;
                tVideoLinuxToUcos.dropflag = VideoFrameReceive.dropflag;
#ifdef DEBUG_AVSYNC
                gtStatistics.video_jitbuf_depth = VideoFrameReceive.CurrentDepth;
#endif
#endif
                break;

            case OS_TIMEOUT:
                if((tmp = RiscRdCNT(VIDEO_LINUX_TO_UCOS_CNT)) > 0) {
                    logprintf(FATAL_ERROR, MP4_DECODE, "\r\nLinux to ucos video buffer is NOT empty, Cnt = %d. !\r\n", tmp);
                    RiscClrCNT(VIDEO_LINUX_TO_UCOS_CNT);
                    VideoBufferReset(&VideoBuffer1);
                }
                if(OSSemPost(gptMpegDecodeReadyToStart) != OS_NO_ERR)
                    logprintf(FATAL_ERROR, MP4_DECODE, "TaskTwoRiscDisplay: OSSemPost gptMpegDecodeReadyToStart err!");
                continue;
                break;

            default:
                debug("\r\nTaskTwoRiscDisplay: OSSemPend gptVideoLinuxToUcosMbox err!\r\n");
                if(OSSemPost(gptMpegDecodeReadyToStart) != OS_NO_ERR)
                    logprintf(FATAL_ERROR, MP4_DECODE, "TaskTwoRiscDisplay: OSSemPost gptMpegDecodeReadyToStart err!");
                continue;
                break;
        }

        /* check the frame length */
        if((tVideoLinuxToUcos.len & 0x3) || (tVideoLinuxToUcos.len < 8) || (tVideoLinuxToUcos.len > 150000)) // the length error
        {
            logprintf(WARNING, MP4_DECODE, "\r\nTaskTwoRiscDisplay: The frame length error!\r\n");

#ifndef VIDEO_JITTER_BUFFER
            RiscDecCNT(VIDEO_LINUX_TO_UCOS_CNT, 1);     
#else
            RiscDecCNT(VIDEO_LINUX_TO_UCOS_CNT, 1);
            ReleaseCursor(&VideoBuffer1, VideoFrameReceive.addr);
#endif
            if(OSSemPost(gptMpegDecodeReadyToStart) != OS_NO_ERR)
                printf("*");
            gIsFirstVideoOutFrame = 1;
            continue;
        }

        /* do avsync */
 #ifdef AV_SYNC       
        do {
            if((gtavsync.syncmode&(~SYNCMASK)) == SELFSYNC) {
                if(gIsFirstVideoOutFrame == 1)  //no need to do selfsync for the first frame
                    break;
                tmp = VideoSelfSync(VideoFrameReceive.CurrentDepth, tVideoLinuxToUcos.tstamp);
                //printf("%d,%d,%d\n", gRiscTimerCnt, tVideoLinuxToUcos.tstamp, VideoBuffer1.CurCnt);
                switch(tmp)
                {
                    case VIDEO_NORMAL:        //decode the frame
                        frameWaitingToDecode = 0;
                        break;
                    case VIDEO_EARLY_L1:            //delay 10ms
                        frameWaitingToDecode = 1;
                        break;
                    case VIDEO_EARLY_L2:         //too early,drop it after decode
                        frameWaitingToDecode = 0;
                        SetVideoJudegDrop();
                        break;
                    case VIDEO_LATE_L1:           //late, decode
                        frameWaitingToDecode = 0;
                        break;
                    case VIDEO_LATE_L2:         //too late,drop after decode
                        frameWaitingToDecode = 0;
                        SetVideoJudegDrop();
                        break;
                    default:
                        printf("SelfSyncVideo return error!%d\n" , tmp);
                        break;
                }
            }
            else if((gtavsync.syncmode&(~SYNCMASK)) == AVSYNC){
                ReflashTimeStampVideo(tVideoLinuxToUcos.tstamp);     //save video timestamp
                OSQQuery(gptMpeg4DecodeOutputReady, &pdata);
                AdjustVideoTs(pdata.OSNMsgs * 3, 1);
                tmp = AvSyncHandler(VideoBuffer1.CurCnt);
                switch(tmp)
                {
                    case VIDEO_NORMAL:
                        frameWaitingToDecode = 0;
                        break;
                    case VIDEO_EARLY_L1:
                        frameWaitingToDecode = 1;
                        break;
                    case VIDEO_EARLY_L2:
                        frameWaitingToDecode = 0;
                        SetVideoJudegDrop();
                        printf("*");
                        break;
                    case VIDEO_LATE_L1:
                        frameWaitingToDecode = 0;
                        break;
                    case VIDEO_LATE_L2:      //too late,and for VGA, decode need too much time, so drop it before decode
                        frameWaitingToDecode = 0;
                        gIsFirstVideoOutFrame = 1;  //drop the following P frames until next I frame.
                        printf("&");
                        break;
                    default:
                        printf("AvSyncHandler return error!%d\n", tmp);
                        break;
                }
            }
            else if((gtavsync.syncmode&(~SYNCMASK)) == NOSYNC) {
                ReflashTimeStampVideo(tVideoLinuxToUcos.tstamp);     //save video timestamp
                frameWaitingToDecode = 0;
            }

            /* if video frame need delay */
            if(frameWaitingToDecode == 1) {
                while(OSSemAccept(gptRiscTimerIntrOccur) > 0)  //clear semaphore
                {;}
                OSSemPend(gptRiscTimerIntrOccur, 200*OS_TICKS_PER_SEC/1000, &err);  //wait for next semaphore
                switch(err)
                {
                    case OS_NO_ERR:
                        break;
                    case OS_TIMEOUT:
                        printf("\nucos SemPend gptRiscTimerOccur timeout!\n");
                        break;
                    default:
                        printf("avsync risc timer err!\n");
                        break;
                }
            }
        }while(frameWaitingToDecode);
#endif

#else   //end of using new avsync algorithm.

        OSSemPend(gptRiscTimerIntrOccur, 2 + MPEG_DECODE_THRESHHOLD * OS_TICKS_PER_SEC/1000, &err);
        if(err == OS_TIMEOUT){           
            printf("\nucos Sem Pend gptRiscTimerOccur time out!\n");
            continue;
        }else if(err == OS_NO_ERR){
            if(gMpegDecodeReady == DECODER_BUSY) {  //wait for next risc timer interrupt
                if(gRiscTimerCnt > 200){
                    gIsFirstVideoOutFrame = 1;    			
                    printf("Warning, Decode Timeout!\n");
                    gtStatistics.cnt_decodetimeout++;
                    RiscDecCNT(VIDEO_LINUX_TO_UCOS_CNT, 1);
                    ReleaseCursor(&VideoBuffer1, VideoFrameReceive.addr);
                    mpeg4DecoderInit();
                    gMpegDecodeReady = DECODER_READY;
                }
                continue;
            }
        }else{
            printf("\nucos sem gptRiscTimerOccur pend err!\n");
            continue;
        }

        if(manual_reset_flag) {
            manual_reset_flag = 0;      			
            gIsFirstVideoOutFrame = 1;
            printf("\r\nmanually reset!\r\n");  	
            mpeg4DecoderInit();     		
        }
        
/*
        OSSemPend(gptMpegDecodeReadyToStart, 2 + MPEG_DECODE_THRESHHOLD * OS_TICKS_PER_SEC/1000, &err);

        switch(err) {
            case OS_TIMEOUT:    // reset cdecode
                gIsFirstVideoOutFrame = 1;
                //writeMpeg4DecoderReg(eMP4_DEC_STOP, 1);    // stop decode if the decode is busy       		
                printf("DecodeTimeout ");
                //manual_err_flag = 0;      		
                mpeg4DecoderInit();     		
                break;      		

            case OS_NO_ERR:
                if(manual_reset_flag) {
                    manual_reset_flag = 0;      			
                    gIsFirstVideoOutFrame = 1;
                    printf("\r\nmanually reset!\r\n");  	
                    mpeg4DecoderInit();     		
                }       		
                break;      	

            default:
                printf("TaskTwoRiscDisplay: OSSemPend gptMpegDecodeReadyToStart err!");
                continue;
                break;          				
        }   	
 */     
        
        if(frameWaitingToDecode == 0){       //no frame is waiting,fetch the next frame
            if(OSSemAccept(gptVideoLinuxToUcosMbox))    // wait for linux send video mpeg frame to ucos
            {
#ifndef VIDEO_JITTER_BUFFER
                if(TwoRiscDisplayRecPacket(SC_VIDEO_LINUX_TO_UCOS, (unsigned int *)&tVideoLinuxToUcos, /* 3 * sizeof(int) */sizeof(tVideoLinuxToUcos)) == FAILED)
                {
                    printf("ucos TwoRiscDisplayRecPacket error!\n");
                    continue;
                }
                if(tVideoLinuxToUcos.len <= sizeof(frameToDecode)) {            
                    tmp = (unsigned int)frameToDecode|0xa0000000;
                    memcpy((void *)tmp, (void *)tVideoLinuxToUcos.addr, tVideoLinuxToUcos.len);
                    tVideoLinuxToUcos.addr = tmp;   
                } else {
                    printf("\nTaskTwoRiscDisplay: warning: framesize(%d) exceeds size of frameToDecode(%d)\n", tVideoLinuxToUcos.len, sizeof(frameToDecode));
                }
#else   //use buffer
                error = VBufGetOneFrame(&VideoBuffer1, &VideoFrameReceive);
                if(error){
                    printf("ucos VBufGetOneFrame error! err = %d\n", error);
                    if(VideoBuffer1.CurCnt > 0)
                        logprintf(WARNING, MP4_DECODE, "\r\nError Occured!video jitbuffer has %d frames but can't get one!\r\n", VideoBuffer1.CurCnt);
                    continue;
                }
                tVideoLinuxToUcos.addr = VideoFrameReceive.addr;
                tVideoLinuxToUcos.len = VideoFrameReceive.size;
                tVideoLinuxToUcos.tstamp = VideoFrameReceive.TimeStamp;
                tVideoLinuxToUcos.dropflag = VideoFrameReceive.dropflag;
#ifdef DEBUG_AVSYNC
                gtStatistics.video_jitbuf_depth = VideoFrameReceive.CurrentDepth;
#endif
#endif
            }
            else{
#if 0
                if(VideoBuffer1.CurCnt > 0) {
                    logprintf(DEBUG_MESSAGE, MP4_DECODE, "\r\nError Occured!video jitbuffer has %d frames but no semaphore!\r\n", VideoFrameReceive.CurrentDepth);
                    if(OSSemPost(gptVideoLinuxToUcosMbox) != OS_NO_ERR)
                        printf("Risc0:OSMboxPost error!\r\n");  
                }
#endif
                continue;
            }

            /* check the frame length */
            if((tVideoLinuxToUcos.len & 0x3) || (tVideoLinuxToUcos.len < 8) || (tVideoLinuxToUcos.len > 150000)) {    // the length error
                logprintf(WARNING, MP4_DECODE, "\r\nTaskTwoRiscDisplay: The frame length error!\r\n");

#ifndef VIDEO_JITTER_BUFFER
                RiscDecCNT(VIDEO_LINUX_TO_UCOS_CNT, 1);     
#else
                RiscDecCNT(VIDEO_LINUX_TO_UCOS_CNT, 1);
                ReleaseCursor(&VideoBuffer1, VideoFrameReceive.addr);
#endif
/*
                gRiscTimerCnt = 0;
                if(OSSemPost(gptRiscTimerIntrOccur) != OS_NO_ERR)
                    printf("*");
*/
                gIsFirstVideoOutFrame = 1;

                continue;
            }
        }
        
#ifdef AV_SYNC        
        if((gtavsync.syncmode&(~SYNCMASK)) == SELFSYNC){         
//            printf("%d,%d\n", tVideoLinuxToUcos.tstamp, gtavsync.videots);
            tmp = VideoSelfSync(VideoFrameReceive.CurrentDepth, tVideoLinuxToUcos.tstamp);
            switch(tmp)
            {
                  case VIDEO_NORMAL:        //decode the frame
                    frameWaitingToDecode = 0;
                    break;          
                case VIDEO_EARLY_L1:            //delay 10ms
                    frameWaitingToDecode = 1;
                    continue;
                    break;
                case VIDEO_EARLY_L2:         //too early,drop it after decode
                    frameWaitingToDecode = 0;
                    SetVideoJudegDrop();
                    break;
                case VIDEO_LATE_L1:           //late, decode
                    frameWaitingToDecode = 0;
                    break;
                case VIDEO_LATE_L2:         //too late,drop after decode
                    frameWaitingToDecode = 0;
                    SetVideoJudegDrop();
                    break;
                default:
                    printf("SelfSyncVideo return error!%d\n" , tmp);
                    break;
            }
        }
        else if((gtavsync.syncmode&(~SYNCMASK)) == AVSYNC){
            ReflashTimeStampVideo(tVideoLinuxToUcos.tstamp);     //save video timestamp
/*
#ifdef DEBUG_AVSYNC
            printf("audio ts =%d, video ts =%d\n", gtavsync.refts, gtavsync.videots);
#endif
//            tmp = AvSyncHandler(VideoBuffer1.CursorNumber - VideoBuffer1.CurCnt);      //
*/
            OSQQuery(gptMpeg4DecodeOutputReady, &pdata);
            AdjustVideoTs(pdata.OSNMsgs * 3, 1);
            tmp = AvSyncHandler(VideoBuffer1.CurCnt);
            switch(tmp)
            {
                case VIDEO_NORMAL:
                    frameWaitingToDecode = 0;
                    break;
                case VIDEO_EARLY_L1:
                    frameWaitingToDecode = 1;
                    continue;
                    break;
                case VIDEO_EARLY_L2:
                    frameWaitingToDecode = 0;
                    printf("*");
                    SetVideoJudegDrop();
                    break;
                case VIDEO_LATE_L1:
                    frameWaitingToDecode = 0;
                    break;
                case VIDEO_LATE_L2:      //too late,and for VGA, decode need too much time, so drop it before decode
                    frameWaitingToDecode = 0;
                    printf("&");
#ifndef VIDEO_JITTER_BUFFER
                    RiscDecCNT(VIDEO_LINUX_TO_UCOS_CNT, 1);     
#else
                    RiscDecCNT(VIDEO_LINUX_TO_UCOS_CNT, 1);
                    ReleaseCursor(&VideoBuffer1, VideoFrameReceive.addr);
#endif
                    gIsFirstVideoOutFrame = 1;  //drop the following P frames until next I frame.
                    continue;
                    break;
                default:
                    printf("AvSyncHandler return error!%d\n", tmp);
                    break;
            }
        }
        else if((gtavsync.syncmode&(~SYNCMASK)) == NOSYNC) {
            ReflashTimeStampVideo(tVideoLinuxToUcos.tstamp);     //save video timestamp
            frameWaitingToDecode = 0;
/*
#ifdef DEBUG_AVSYNC
            ReflashTimeStampVideo(tVideoLinuxToUcos.tstamp);     //save video timestamp
            printf("local ts =%d, audio ts =%d, video ts =%d\n", gtavsync.systemts, gtavsync.refts, gtavsync.videots);
#endif
*/
        }
#endif
#endif
#if 0
            extern mpeg_count;
                /* compute the time of a frame */
                count = GetCount();
                if(count > mpeg_count)
                    t1 = count - mpeg_count;
                else
                    t1 = CPU_MAIN_CLOCK / (2 * OS_TICKS_PER_SEC) + count - mpeg_count;  
                printf("<q%d>", t1 / (CPU_MAIN_CLOCK/(2*10000)));    // counted by mili-seconds/10
#endif

        cnt++;      	

        // Set input bitstream block initial and end address
        mp4DecodeParams.ipt_iaddr = tVideoLinuxToUcos.addr;

        if(gChecksumEnabled == TRUE) {
            mp4DecodeParams.ipt_eaddr = (tVideoLinuxToUcos.addr) + (tVideoLinuxToUcos.len-16 - 4); //wy:bitstream end address
            if(tVideoLinuxToUcos.dropflag == TRUE)
            {
#ifndef VIDEO_JITTER_BUFFER
                RiscDecCNT(VIDEO_LINUX_TO_UCOS_CNT, 1);     
#else
                RiscDecCNT(VIDEO_LINUX_TO_UCOS_CNT, 1);
                ReleaseCursor(&VideoBuffer1, VideoFrameReceive.addr);
#endif 
                gIsFirstVideoOutFrame = 1;

#ifdef NEW_AVSYNC_ALGORITHM
                if(OSSemPost(gptMpegDecodeReadyToStart) != OS_NO_ERR)
                    logprintf(FATAL_ERROR, MP4_DECODE, "*,");     
#endif

                continue;
            }
#if 0
            tmp=encodeDataChksum(mp4DecodeParams.ipt_iaddr, tVideoLinuxToUcos.len);
            if(tmp){

#ifndef VIDEO_JITTER_BUFFER
                RiscDecCNT(VIDEO_LINUX_TO_UCOS_CNT, 1);     
#else
                RiscDecCNT(VIDEO_LINUX_TO_UCOS_CNT, 1);
                ReleaseCursor(&VideoBuffer1, VideoFrameReceive.addr);
#endif 
                gIsFirstVideoOutFrame = 1;

#ifdef NEW_AVSYNC_ALGORITHM
            if(OSSemPost(gptMpegDecodeReadyToStart) != OS_NO_ERR)
                logprintf(FATAL_ERROR, MP4_DECODE, "*,");     
#endif

                continue;
            } 
#endif
        }
        else {
            mp4DecodeParams.ipt_eaddr = (tVideoLinuxToUcos.addr) + (tVideoLinuxToUcos.len - 4); //wy:bitstream end address
        }

        writeMpeg4DecoderReg(eMP4_DEC_BIT_BLK_INIT_ADDR, mp4DecodeParams.ipt_iaddr);

        if(manual_err_flag) {
            if(ip_count++ == 1){ 
                ip_count = 0;               		
                *(int *)(mp4DecodeParams.ipt_iaddr + 100) = 0x55555555;
            }
        } 
        writeMpeg4DecoderReg(eMP4_DEC_BIT_BLK_END_ADDR, mp4DecodeParams.ipt_eaddr); 

        h263_I_P = *(unsigned int *)(mp4DecodeParams.ipt_iaddr+4);
        head_mpeg_h263 = *(unsigned int *)(mp4DecodeParams.ipt_iaddr);

        if((head_mpeg_h263 & 0x3fcffff) == 0x2800000 ) {     //decode H263 bit stream
            if(decode_flag != H263_ENCODE) {           //switch frome MPEG bit stream to H263 bit stream
                gIsFirstVideoOutFrame = 1; 
                decode_flag = H263_ENCODE;    
            }
        } else {                                                              //decode MPEG bit stream
            if(decode_flag != MPEG_ENCODE) {           //switch frome H263 bit stream to MPEG bit stream
                gIsFirstVideoOutFrame = 1; 
                decode_flag = MPEG_ENCODE;    
            }
        }

        if(gIsFirstVideoOutFrame == 0) {    // NOT the first decode video frame
            if((((head_mpeg_h263 & 0x3fcffff) == 0x2800000)&&((h263_I_P & 0x2) == 0x2/*not I-frame?*/))||(head_mpeg_h263 == 0xb6010000)) {    //the frame is a P-frame of H263 or a I/P-frame of mpeg     	
                if(gtavsync.speed.flag == 1)       //P frame should be skipped for speed up
                    continue;
#ifdef PRINT_MPEG_DECODE_PERFORMANCE
                OS_ENTER_CRITICAL();
                mpeg_decode_start = GetCount();
                *( volatile unsigned *)0xa8120204 = 4;  
                OS_EXIT_CRITICAL();
                if(mpeg_decode_end < mpeg_decode_start)
                    mpeg_decode_end = mpeg_decode_start - mpeg_decode_end;
                else
                    mpeg_decode_end = CPU_MAIN_CLOCK / (2 * OS_TICKS_PER_SEC) + mpeg_decode_start - mpeg_decode_end; 
                printf("<mde=%d>", mpeg_decode_end / (CPU_MAIN_CLOCK/(2*1000000)));    // counted by microsecond
#else
                *( volatile unsigned *)0xa8120204 = 4;
#endif
                gMpegDecodeReady = DECODER_BUSY;
                gRiscTimerCnt = 0;
            } else if((((head_mpeg_h263 & 0x3fcffff) == 0x2800000)&&((h263_I_P & 0x2) == 0)/* I-frame?*/)||(head_mpeg_h263 == 0x00010000)) {    // every vol will reset decode
#ifdef EACH_VOL_RESET_DECODE
                 /* Each vol has reset! */
                tmp = readMpeg4DecoderReg(eMP4_DEC_OUT_BUF_BASE_ADDR);
                    
                mpeg4DecoderInit();

                /* give the input bit-stream address */
                writeMpeg4DecoderReg(eMP4_DEC_BIT_BLK_INIT_ADDR, mp4DecodeParams.ipt_iaddr);
                writeMpeg4DecoderReg(eMP4_DEC_BIT_BLK_END_ADDR, mp4DecodeParams.ipt_eaddr);

                /* give the decode output address */
                writeMpeg4DecoderReg(eMP4_DEC_OUT_BUF_BASE_ADDR, tmp);

#ifdef PRINT_MPEG_DECODE_PERFORMANCE
                OS_ENTER_CRITICAL();
                mpeg_decode_start = GetCount();
                *( volatile unsigned *)0xa8120204 = 7; 
                OS_EXIT_CRITICAL();
                if(mpeg_decode_end < mpeg_decode_start)
                    mpeg_decode_end = mpeg_decode_start - mpeg_decode_end;
                else
                    mpeg_decode_end = CPU_MAIN_CLOCK / (2 * OS_TICKS_PER_SEC) + mpeg_decode_start - mpeg_decode_end; 
                printf("<mde=%d>", mpeg_decode_end / (CPU_MAIN_CLOCK/(2*1000000)));    // counted by microsecond
#else
                *( volatile unsigned *)0xa8120204 = 7; 
#endif
#else
                /* even vol has no reset! */
#ifdef PRINT_MPEG_DECODE_PERFORMANCE
                OS_ENTER_CRITICAL();
                mpeg_decode_start = GetCount();
                *( volatile unsigned *)0xa8120204 = 4; 
                OS_EXIT_CRITICAL();
#else
                *( volatile unsigned *)0xa8120204 = 4;
#endif
#endif
                gMpegDecodeReady = DECODER_BUSY;
                gRiscTimerCnt = 0;
            } else {
                logprintf(WARNING, MP4_DECODE, "\r\nTaskTwoRiscDisplay: The frame head error, 0x%x instead!\r\n", head_mpeg_h263);

#ifndef VIDEO_JITTER_BUFFER
                RiscDecCNT(VIDEO_LINUX_TO_UCOS_CNT, 1);     
#else
                RiscDecCNT(VIDEO_LINUX_TO_UCOS_CNT, 1);
                ReleaseCursor(&VideoBuffer1, VideoFrameReceive.addr);
#endif                         

#ifdef NEW_AVSYNC_ALGORITHM
                if(OSSemPost(gptMpegDecodeReadyToStart) != OS_NO_ERR)
                    printf("*");
#endif

                gIsFirstVideoOutFrame = 1;
            }
        } 
        else {   // the first decode video frame
            if(!(((head_mpeg_h263 & 0x3fcffff) == 0x2800000)&&((h263_I_P & 0x2) == 0)) && (head_mpeg_h263 != 0x00010000)) {    //the first decode frame is not an I-frame
                logprintf(WARNING, MP4_DECODE, "\r\nTaskTwoRiscDisplay: The first decode frame is NOT vol, 0x%x instead!0x%x\r\n",head_mpeg_h263,h263_I_P);

#ifndef VIDEO_JITTER_BUFFER
                RiscDecCNT(VIDEO_LINUX_TO_UCOS_CNT, 1);     
#else
                RiscDecCNT(VIDEO_LINUX_TO_UCOS_CNT, 1);
                ReleaseCursor(&VideoBuffer1, VideoFrameReceive.addr);
#endif                         
#ifdef NEW_AVSYNC_ALGORITHM
                if(OSSemPost(gptMpegDecodeReadyToStart) != OS_NO_ERR)
                    printf("*");
#endif
            } else {
                logprintf(WARNING, MP4_DECODE, "\r\nTaskTwoRiscDisplay: The first decode frame!\r\n");
                mpeg4DecoderInit();
#ifdef PRINT_MPEG_DECODE_PERFORMANCE
                OS_ENTER_CRITICAL();
                mpeg_decode_start = GetCount();
                *( volatile unsigned *)0xa8120204 = 7;
                OS_EXIT_CRITICAL();
                if(mpeg_decode_end < mpeg_decode_start)
                    mpeg_decode_end = mpeg_decode_start - mpeg_decode_end;
                else
                    mpeg_decode_end = CPU_MAIN_CLOCK / (2 * OS_TICKS_PER_SEC) + mpeg_decode_start - mpeg_decode_end; 
                printf("<mde=%d>", mpeg_decode_end / (CPU_MAIN_CLOCK/(2*1000000)));    // counted by microsecond
#else
                *( volatile unsigned *)0xa8120204 = 7;
#endif               
                gMpegDecodeReady = DECODER_BUSY;
                gRiscTimerCnt = 0;
            }
        }                            
    }
}

#else
/*
***************************************************************************
*                         TaskTwoRiscDisplay
*
* Description: 
*
* Arguments  : 
*
* Return     : 
*
* Note(s)    : 
***************************************************************************
*/
void TaskTwoRiscDisplay(void *data)
{
    unsigned int i, cnt = 0, ip_count = 0, t1, tmp;//, tttt[500];
    unsigned char err;
    int error;
    int h263_I_P;                                              //20071129;the value of *(unsigned int *)(mp4DecodeParams.ipt_iaddr+4)
    int head_mpeg_h263;                                 //20071130;the value of *(unsigned int *)(mp4DecodeParams.ipt_iaddr)
    //decode_flag = MPEG_ENCODE;                     //20071129;indicate the encode mode of the bit stream default is MPEG mode
    tVideoLinuxToUcos *ptVideoLinuxToUcos, tVideoLinuxToUcos;

    static unsigned int count, frame_count = 0;
    
    extern unsigned int gVideoStatus;
    OS_CPU_SR cpu_sr; 

    gptMpegDecodeReadyToStart = OSSemCreate(1);

    gptVideoLinuxToUcosMbox = OSSemCreate(0);   

    //gptMpeg4DecodeOutputReady = OSMboxCreate((void *)0);  
    gptMpeg4DecodeOutputReady = OSQCreate(ArrayMsgDecode, 1);   // if there are n buffers, the msg queu can just contain n-2 msgs to avoid read & write confict..   
#ifdef PIP_ENABLE
    gptMqueueVideoInFrame = OSQCreate(arrayMsg, 1);    // if there are n buffers, the msg queu can just contain n-2 msgs to avoid read & write confict..
    //gptMqueueVideoInFrame = OSMboxCreate((void *)0);
#endif
    // This should be done by linux!
    //RiscClrCNT(VIDEO_LINUX_TO_UCOS_CNT);
    
    /* global parameters initialized */
    gIsFirstVideoOutFrame = 1;    
    gMpeg4DecodeOutputCount = 1;

#ifdef VIDEO_JITTER_BUFFER
    VBufInit(&VideoBuffer1);
#endif
    
    manual_err_flag = 0;    

    McIntrEn();

    ScInit(SC_VIDEO_LINUX_TO_UCOS, sBufUse_Internal, sDBSize_4Byte, sBufSize_128Byte, sSBSize_4Byte, sMBSize_32Byte, sSwap_no, sLast_Disable, SC_VIDEO_LINUX_TO_UCOS_ADDR);
    ScOpen(SC_VIDEO_LINUX_TO_UCOS);
    
    gMpeg4DecodeOutputBufNo = 0;
    mpeg4IntrInit();   //  init mpeg4(encode, decode, & mv) intr, enable risc vcodec intr       	    
    mpeg4DecoderInit();
 
    /* the state of beginning , 1: open; 0: close*/
    gVoutEnable = 1; 
    gRvoEnable = 1;
    gPipEnable = 0; //close pip when display startup picture
    gOsdEnable = 0; 

    GetVoutInterface();
    VoutInterfaceIntialize(gtDisplayFormat.vout_format);
    /*
    printf("Init Adv7171...");  
    if(HwADV7171Initialize(gtDisplayFormat.vout_format) == SUCCESSFUL)    // 0: NTSC mode,1: pal mode           
        printf("ok\n");
    else 
        printf("failed\n");
    */

    OSTimeDlyHMSM(0, 0, 0, 100);
    VscInit();
    VoutIntrEn();   	
    ScIntrEn();    // locate sc ISR, enable risc sc intr.     

#if 1
    /* Vin */    
    printf("\r\nVin");  
    for(i = 0; i <= 0x8; i +=  4)
        printf("\r\n0x%x = 0x%x!", 0xa8050000 + i, *(int *)(0xa8050000 + i));           
#endif

#if 0
    /* Vout */
    printf("\r\nVout"); 
    for(i = 0; i <= 0x10; i +=  4)
        printf("\r\n0x%x = 0x%x!", 0xa8060000 + i, *(int *)(0xa8060000 + i));   	

    /* Vsc */
    printf("\r\nVsc");  	
    for(i = 0; i <= 0x44; i +=  4)
        printf("\r\n0x%x = 0x%x!", 0xa80b0000 + i, *(int *)(0xa80b0000 + i));   

    printf("\r\nVin    exContext: 0x%x = 0x%x!", 0xa8023000, *(int *)0xa8023000);   
    printf("\r\nVout  exContext: 0x%x = 0x%x!", 0xa8023004, *(int *)0xa8023004);    
    printf("\r\nVsc0  exContext: 0x%x = 0x%x!", 0xa8023008, *(int *)0xa8023008);    
    printf("\r\nVsc1  exContext: 0x%x = 0x%x!", 0xa802300c, *(int *)0xa802300c);    
    printf("\r\nOSD  exContext: 0x%x = 0x%x!", 0xa8023010, *(int *)0xa8023010);     
    printf("\r\nVscW exContext: 0x%x = 0x%x!", 0xa8023014, *(int *)0xa8023014);     
    
#endif

#if 0
    /* Decode */
    printf("\r\nDecode\r\n");           
    for(i = 0; i <= 0x58; i+=4 )
        printf("0x%x = 0x%x\r\n", i + 0xa8120200, *(int *)(i + 0xa8120200));    

    printf("0xa8120400 = 0x%x\r\n", *(int *)(0xa8120400));  	
    printf("0xa8120404 = 0x%x\r\n", *(int *)(0xa8120404));  
    printf("0xa8120408 = 0x%x\r\n", *(int *)(0xa8120408));
#endif

    AppVscStart();  

#if 0    
    gptMpegDecodeReadyToStart = OSSemCreate(1);

    gptVideoLinuxToUcosMbox = OSSemCreate(0);   

    //gptMpeg4DecodeOutputReady = OSMboxCreate((void *)0);  
    gptMpeg4DecodeOutputReady = OSQCreate(ArrayMsgDecode, 1);   // if there are n buffers, the msg queu can just contain n-2 msgs to avoid read & write confict..   

    // This should be done by linux!
    //RiscClrCNT(VIDEO_LINUX_TO_UCOS_CNT);
    
    /* global parameters initialized */
    gIsFirstVideoOutFrame = 1;    
    gMpeg4DecodeOutputCount = 1;
#endif

    i = 0;  
    
    gVideoStatus |= tVoutEn;
    
    while(1) {

        OSSemPend(gptMpegDecodeReadyToStart, 2 + MPEG_DECODE_THRESHHOLD * OS_TICKS_PER_SEC/1000, &err);

        switch(err) {
            case OS_TIMEOUT:    // reset cdecode
                gIsFirstVideoOutFrame = 1;
                //writeMpeg4DecoderReg(eMP4_DEC_STOP, 1);    // stop decode if the decode is busy       		
                printf("DecodeTimeout ");
                //manual_err_flag = 0;      	
                RiscDecCNT(VIDEO_LINUX_TO_UCOS_CNT, 1);	
                mpeg4DecoderInit();     		
                break;      		

            case OS_NO_ERR:
                if(manual_reset_flag) {
                    manual_reset_flag = 0;      			
                    gIsFirstVideoOutFrame = 1;
                    printf("\r\nmanually reset!\r\n");  	
                    mpeg4DecoderInit();     		
                }       		
                break;      	

            default:
                printf("TaskTwoRiscDisplay: OSSemPend gptMpegDecodeReadyToStart err!");
                continue;
                break;          				
        }   	
        
        /* wait for linux send video mpeg frame to ucos */ 
        OSSemPend(gptVideoLinuxToUcosMbox, OS_TICKS_PER_SEC, &err);          
        
        /* start mpeg decode */
        switch(err) {
            case OS_NO_ERR:
                
#ifndef VIDEO_JITTER_BUFFER
                if(TwoRiscDisplayRecPacket(SC_VIDEO_LINUX_TO_UCOS, (unsigned int *)&tVideoLinuxToUcos, /* 3 * sizeof(int) */sizeof(tVideoLinuxToUcos)) == FAILED)
                    printf("*");
                if(tVideoLinuxToUcos.len <= sizeof(frameToDecode)) {            
                    tmp = (unsigned int)frameToDecode |0x80000000;
                    cache_writeback_invalidate_D((void *)tVideoLinuxToUcos.addr, tVideoLinuxToUcos.len);
                    memcpy((void *)tmp, (void *)(tVideoLinuxToUcos.addr & 0x1fffffff | 0x80000000), tVideoLinuxToUcos.len);
                    cache_writeback_D(tmp, tVideoLinuxToUcos.len);
                    tVideoLinuxToUcos.addr = tmp;   
                } else {
                    printf("\nTaskTwoRiscDisplay: warning: framesize(%d) exceeds size of frameToDecode(%d)\n", tVideoLinuxToUcos.len, sizeof(frameToDecode));
                }
#else   //use buffer
                error = VBufGetOneFrame(&VideoBuffer1, &VideoFrameReceive);
                if(error)
                {
                    printf("get video frame error! err = %d\n", err);
                    //if(OSSemPost(gptMpegDecodeReadyToStart) != OS_NO_ERR)
                    //    printf("*");
                    continue;
                }
                //printf("r = 0x%8.8x, %d, %d\n", VideoFrameReceive.addr, VideoFrameReceive.size, VideoFrameReceive.TimeStamp);
                tVideoLinuxToUcos.addr = VideoFrameReceive.addr;
                tVideoLinuxToUcos.len = VideoFrameReceive.size;
#endif
                /* check the frame length */
                if((tVideoLinuxToUcos.len & 0x3) || (tVideoLinuxToUcos.len < 8) || (tVideoLinuxToUcos.len > 150000)) {    // the length error
                        logprintf(WARNING, MP4_DECODE, "\r\nTaskTwoRiscDisplay: The frame length error!\r\n");

#ifndef VIDEO_JITTER_BUFFER
                        RiscDecCNT(VIDEO_LINUX_TO_UCOS_CNT, 1);     
#else                        
                        ReleaseCursor(&VideoBuffer1, VideoFrameReceive.addr);
#endif

                        if(OSSemPost(gptMpegDecodeReadyToStart) != OS_NO_ERR)
                            printf("*");
                        gIsFirstVideoOutFrame = 1;                        
                        continue;
                }
                
#ifdef AV_SYNC
            ReflashTimeStampVideo(VideoFrameReceive.TimeStamp);     //save video timestamp
            if(AVSyncVideoWait(VideoFrameReceive.CurrentSpace) != VIDEO_NORMAL)    //wait audio until the permit range
                SetVideoJudegDrop();
#endif

#if 0
            extern mpeg_count;
                /* compute the time of a frame */
                count = GetCount();
                if(count > mpeg_count)
                    t1 = count - mpeg_count;
                else
                    t1 = CPU_MAIN_CLOCK / (2 * OS_TICKS_PER_SEC) + count - mpeg_count;  
                printf("<q%d>", t1 / (CPU_MAIN_CLOCK/(2*10000)));    // counted by mili-seconds/10
#endif

                cnt++;      	

                // Set input bitstream block initial and end address
                mp4DecodeParams.ipt_iaddr = tVideoLinuxToUcos.addr;

                if(gChecksumEnabled == TRUE) {
                    mp4DecodeParams.ipt_eaddr = (tVideoLinuxToUcos.addr) + \
                                                (tVideoLinuxToUcos.len-16 - 4); //wy:bitstream end address  

                    tmp=encodeDataChksum(mp4DecodeParams.ipt_iaddr, tVideoLinuxToUcos.len);
                    if(tmp==1) {

#ifndef VIDEO_JITTER_BUFFER
                        RiscDecCNT(VIDEO_LINUX_TO_UCOS_CNT, 1);     
#else
                        ReleaseCursor(&VideoBuffer1, VideoFrameReceive.addr);
#endif                
                        if(OSSemPost(gptMpegDecodeReadyToStart) != OS_NO_ERR)
                            logprintf(FATAL_ERROR, MP4_DECODE, "*,");                    
                        continue;
                    } 
                    else if(tmp==2) {

#ifndef VIDEO_JITTER_BUFFER
                        RiscDecCNT(VIDEO_LINUX_TO_UCOS_CNT, 1);     
#else
                        ReleaseCursor(&VideoBuffer1, VideoFrameReceive.addr);
#endif
                        if(OSSemPost(gptMpegDecodeReadyToStart) != OS_NO_ERR)
                            logprintf(FATAL_ERROR, MP4_DECODE, "*,");
                        continue;                
                    }else  {
                        logprintf(DEBUG_MESSAGE, MP4_DECODE, "O,");
                    }
                }
                else {
                    mp4DecodeParams.ipt_eaddr = (tVideoLinuxToUcos.addr) + (tVideoLinuxToUcos.len - 4); //wy:bitstream end address
                }
            
                writeMpeg4DecoderReg(eMP4_DEC_BIT_BLK_INIT_ADDR, mp4DecodeParams.ipt_iaddr);

                if(ip_count++ == 1) {
                    ip_count = 0;
                    if(manual_err_flag)     		
                        *(int *)(mp4DecodeParams.ipt_iaddr + 100) = 0x55555555;                
                    writeMpeg4DecoderReg(eMP4_DEC_BIT_BLK_END_ADDR, mp4DecodeParams.ipt_eaddr);
                } else
                    writeMpeg4DecoderReg(eMP4_DEC_BIT_BLK_END_ADDR, mp4DecodeParams.ipt_eaddr);                 
                
                h263_I_P = *(unsigned int *)(mp4DecodeParams.ipt_iaddr+4);
                head_mpeg_h263 = *(unsigned int *)(mp4DecodeParams.ipt_iaddr);

                if((head_mpeg_h263 & 0x3fcffff) == 0x2800000 ) {     //decode H263 bit stream
                    if(decode_flag != H263_ENCODE) {           //switch frome MPEG bit stream to H263 bit stream
                        gIsFirstVideoOutFrame = 1; 
                        decode_flag = H263_ENCODE;    
                    }
                } else {                                                              //decode MPEG bit stream
                    if(decode_flag != MPEG_ENCODE) {           //switch frome H263 bit stream to MPEG bit stream
                        gIsFirstVideoOutFrame = 1;
                        decode_flag = MPEG_ENCODE;     
                    }
                }
                
                if(gIsFirstVideoOutFrame == 0) {    // Is NOT the first decode video frame?
                    if((((head_mpeg_h263 & 0x3fcffff) == 0x2800000)&&((h263_I_P & 0x2) == 0x2/*not I-frame?*/))||(head_mpeg_h263 == 0xb6010000)) {    //the frame is a P-frame of H263 or a I/P-frame of mpeg     	
#ifdef PRINT_MPEG_DECODE_PERFORMANCE
                        OS_ENTER_CRITICAL();
                        mpeg_decode_start = GetCount();
                        *( volatile unsigned *)0xa8120204 = 4;      	
                        OS_EXIT_CRITICAL();
#else
                        *( volatile unsigned *)0xa8120204 = 4;
#endif
                    } else if((((head_mpeg_h263 & 0x3fcffff) == 0x2800000)&&((h263_I_P & 0x2) == 0)/* I-frame?*/)||(head_mpeg_h263 == 0x00010000)) {    // every vol will reset decode
#ifdef EACH_VOL_RESET_DECODE
                        /* Each vol has reset! */
                        tmp = readMpeg4DecoderReg(eMP4_DEC_OUT_BUF_BASE_ADDR);
                    
                        mpeg4DecoderInit();

                        /* give the input bit-stream address */
                        writeMpeg4DecoderReg(eMP4_DEC_BIT_BLK_INIT_ADDR, mp4DecodeParams.ipt_iaddr);
                        writeMpeg4DecoderReg(eMP4_DEC_BIT_BLK_END_ADDR, mp4DecodeParams.ipt_eaddr);

                        /* give the decode output address */
                        writeMpeg4DecoderReg(eMP4_DEC_OUT_BUF_BASE_ADDR, tmp);

#ifdef PRINT_MPEG_DECODE_PERFORMANCE
                        OS_ENTER_CRITICAL();
                        mpeg_decode_start = GetCount();
                        *( volatile unsigned *)0xa8120204 = 7; 
                        OS_EXIT_CRITICAL();
#else
                        *( volatile unsigned *)0xa8120204 = 7; 
#endif
#else
                        /* even vol has no reset! */
#ifdef PRINT_MPEG_DECODE_PERFORMANCE
                        OS_ENTER_CRITICAL();
                        mpeg_decode_start = GetCount();
                        *( volatile unsigned *)0xa8120204 = 4; 
                        OS_EXIT_CRITICAL();
#else
                        *( volatile unsigned *)0xa8120204 = 4;
#endif
#endif
                    } else {
                        logprintf(WARNING, MP4_DECODE, "\r\nTaskTwoRiscDisplay: The frame head error, 0x%x instead!\r\n", head_mpeg_h263);
#ifndef VIDEO_JITTER_BUFFER
                        RiscDecCNT(VIDEO_LINUX_TO_UCOS_CNT, 1);     
#else
                        ReleaseCursor(&VideoBuffer1, VideoFrameReceive.addr);
#endif                         
                        if(OSSemPost(gptMpegDecodeReadyToStart) != OS_NO_ERR)
                            printf("*");
                        gIsFirstVideoOutFrame = 1;
                    }
                } else {    // is the first decode frame
                    if(!(((head_mpeg_h263 & 0x3fcffff) == 0x2800000)&&((h263_I_P & 0x2) == 0)) && (head_mpeg_h263 != 0x00010000)) {    //the first decode frame is not an I-frame
                        logprintf(WARNING, MP4_DECODE, "\r\nTaskTwoRiscDisplay: The first decode frame is NOT vol, 0x%x instead!0x%x\r\n", head_mpeg_h263,h263_I_P);
#ifndef VIDEO_JITTER_BUFFER
                        RiscDecCNT(VIDEO_LINUX_TO_UCOS_CNT, 1);     
#else
                        ReleaseCursor(&VideoBuffer1, VideoFrameReceive.addr);
#endif                         

                        if(OSSemPost(gptMpegDecodeReadyToStart) != OS_NO_ERR)
                            printf("*"); 
                    } else {
                        logprintf(WARNING, MP4_DECODE, "\r\nTaskTwoRiscDisplay: The first decode frame!\r\n");
                        mpeg4DecoderInit();
#ifdef PRINT_MPEG_DECODE_PERFORMANCE
                        OS_ENTER_CRITICAL();
                        mpeg_decode_start = GetCount();
                        *( volatile unsigned *)0xa8120204 = 7;
                        OS_EXIT_CRITICAL();
#else
                        *( volatile unsigned *)0xa8120204 = 7;
#endif                        
                    }
                }   
                break;

            case OS_TIMEOUT:
                if((tmp = RiscRdCNT(VIDEO_LINUX_TO_UCOS_CNT)) > 0) {
                    logprintf(FATAL_ERROR, MP4_DECODE, "\r\nLinux to ucos video buffer is NOT empty, Cnt = %d. The LinuxToUcos stream has %d bytes!\r\n", tmp, ScRdCount(RISC0_RX));
                    RiscClrCNT(VIDEO_LINUX_TO_UCOS_CNT);
                }
                if(OSSemPost(gptMpegDecodeReadyToStart) != OS_NO_ERR)
                    logprintf(FATAL_ERROR, MP4_DECODE, "TaskTwoRiscDisplay: OSSemPost gptMpegDecodeReadyToStart err!");
                continue;                
                break;

            default:
                debug("\r\nTaskTwoRiscDisplay: OSSemPend gptVideoLinuxToUcosMbox err!\r\n");
                if(OSSemPost(gptMpegDecodeReadyToStart) != OS_NO_ERR)
                    logprintf(FATAL_ERROR, MP4_DECODE, "TaskTwoRiscDisplay: OSSemPost gptMpegDecodeReadyToStart err!");
                continue;                
                break;
        }
            
    }
}

#endif
/*
***************************************************************************
*                            TwoRiscDisplayRecPacket
*
* Description: 
*
* Arguments  : 
*
* Return     :     
*
* Note(s)    : number is counted by bytes, but should be multiple of sizeof(int)!
***************************************************************************
*/
int TwoRiscDisplayRecPacket(unsigned int sID, unsigned int *packet, unsigned int number)
{
    int i;       
    
    if(ScRdCount(sID) < number)         
            return FAILED;
       
    for(i = 0; i < number; i += sizeof(int)) {        
        if(ScRd(sID, packet++) == FAILED)
            return FAILED;   
    }
    
    return SUCCESSFUL;        
}

/*
***************************************************************************
*                            TwoRiscDisplaySendPacket
*
* Description: 
*
* Arguments  : 
*
* Return     : 
*
* Note(s)    : len is counted by bytes, NOT by words, but should be multiple of sizeof(int)!
***************************************************************************
*/
int TwoRiscDisplaySendPacket(unsigned int sID, unsigned int *message, unsigned int len)
{
    unsigned int i;
        
    if(ScRdBlankCount(sID) < len)    
        return FAILED; 
    
    for(i = 0; i < len; i += sizeof(int)) {        
        if(ScWr(sID, *message++) == FAILED)
            return FAILED;
    }
    
    return SUCCESSFUL;
}


void TwoRiscDisplayRvoPipOsdInit()
{
    static int initialized = 0;

    if(initialized)
        return;

    initialized = 1;
    
    /* Display format inited */
#ifdef DVR    
    gtDisplayFormat.vout_format = PAL;    // NTSC; PAL; LCD    
#else
    gtDisplayFormat.vout_format = NTSC;    // NTSC; PAL; LCD    
#endif

    if(gtDisplayFormat.vout_format == LCD) {
        gtDisplayFormat.vout_interlaced = FALSE;            
        gtDisplayFormat.width = 480;
        gtDisplayFormat.height = 240;
    } else {  // NTSC or PAL
        gtDisplayFormat.vout_interlaced = TRUE;    
    }
    
#ifdef STARTUP_PICTURE

    TwoRiscDisplayStartupPicInit();

#else

    gtRvoCurrent.width = 320;
    gtRvoCurrent.height = 240;
    
    gtRvoCurrent.format = YUV420;
#ifdef DVR
    gtRvoCurrent.interlaced = TRUE;
#else
    gtRvoCurrent.interlaced = FALSE;
#endif
    gtRvoCurrent1.interlaced = gtRvoCurrent.interlaced; 

    gtRvoCurrent.pixel_ratio = horizontal_no_scale;   
    gtRvoCurrent.line_ratio = vertical_no_scale;

#endif
    gtRvoCurrent1.pixel_ratio = gtRvoCurrent.pixel_ratio;   
    gtRvoCurrent1.pixel_ratio = gtRvoCurrent.line_ratio;
    
#ifdef OSD_ENABLE
    if(gtDisplayFormat.vout_format == NTSC){
        gtOsdCurrent.osd_format = NTSC;
        gtOsdCurrent.width = 704;
        gtOsdCurrent.height = 480;
    } else if(gtDisplayFormat.vout_format == PAL){
        gtOsdCurrent.osd_format = PAL;
        gtOsdCurrent.width = 704;
        gtOsdCurrent.height = 576;
    } else if(gtDisplayFormat.vout_format == LCD){
        gtOsdCurrent.osd_format = LCD;
        gtOsdCurrent.width = 480;
        gtOsdCurrent.height = 240;
    }
    gtOsdCurrent.start_pixel = 0;
    gtOsdCurrent.start_line = 0;
    gtOsdCurrent.backgroundColor = 15;
    gtOsdCurrent.OSD_T_Mask = 15;
    gtOsdCurrent.fill_addr = (((unsigned int)OSD_memory_0)&0x1fffffff);
    gtOsdCurrent.display_addr = (((unsigned int)OSD_memory_0)&0x1fffffff);
    InitOsdMemory(gtOsdCurrent.start_pixel ,gtOsdCurrent.start_line, gtOsdCurrent.width, gtOsdCurrent.height , gtOsdCurrent.backgroundColor, gtOsdCurrent.fill_addr, gtOsdCurrent.OSD_T_Mask);

    gptMqueueOSDFrame = OSQCreate(ArrayMsgOsd, 1);   // if there are n buffers, the msg queu can just contain n-2 msgs to avoid read & write confict..  
    
#endif

#ifdef PIP_ENABLE

    gtPipSetting.start_pixel = 528;     // 16, 528, 320
    gtPipSetting.start_line = 328;      // 0, 360, 120

    gtPipCurrent.pixel_ratio = horizontal_div_2;    
    gtPipCurrent.line_ratio = vertical_div_2;    

    if((gtVinFormat.interlaced == TRUE) && (gImage_Mode == MERGED_IMAGE))
        gtPipCurrent.interlaced = FALSE;      
    else
        gtPipCurrent.interlaced = gtVinFormat.interlaced;     
    
    gtPipCurrent.format = gtVinFormat.yuv_format;
#endif

}
#ifdef STARTUP_PICTURE
void TwoRiscDisplayStartupPicInit(void)
{
    gtRvoCurrent.width = gtStartupPic.width;
    gtRvoCurrent.height = gtStartupPic.height; 

    gtRvoCurrent.format = YUV420;

    gtRvoCurrent.interlaced = FALSE;

    gtRvoCurrent.pixel_ratio = horizontal_no_scale;   
    gtRvoCurrent.line_ratio = vertical_no_scale;
    
}
#endif
#endif

