/*
***************************************************************************
*                  Copyright (c) 2005 vBridge Microsystem, Inc.  
*                       Unpublished & Not for Publication
*                               All Rights Reserved                   
*   
* File        :                    
*                                                                         
* Description : This file is to clear the mpeg intr according to 
*               David's email(on May 23, 2006)
*             
* Date        : May 27, 2006                                                           
***************************************************************************
*/


#define  MPEG4_ENCODER_INTR_CLR_GLOBALS
#include "includes.h"
#include "mpeg4_encoder.h"
#include "two_risc_video.h"
#include "risc.h"
#include "chip_cfg.h"
#include "2risc_memory_map.h"

#include "../video_data/saved_vin_data.h"

#include "video_cfg.h"
#include "mpeg4_intr.h"

#include "../video/VIN_API.h"

#include "../mpeg/mpeg4_decoder.h"
#include "two_risc_display.h"
#include "two_risc_statistics.h"
#include"2risc_comm_protocol.h"
#include"video_api.h"

#include "timestamp.h"

#include "video_buffer.h"
extern unsigned int gVideoEnableDisable;
#ifdef MV_ENABLE
unsigned int Mv_count=0;
unsigned int MbCnt=0;
unsigned int MvFrameCnt=0;
int updatetime=0;
#endif

#ifdef DISPLAY_INCLUDE
extern unsigned int linuxToUcosFrameNum;
extern volatile unsigned int decodeFrameNum;
#endif

#ifdef PRINT_MPEG_DECODE_PERFORMANCE
unsigned int gTDecodeSumMd2 = 0;
unsigned int gTDecodeCntMd2 = 0;
unsigned int gTDecodeSumMd4 = 0;
unsigned int gTDecodeCntMd4 = 0;
#endif

#ifdef PRINT_MPEG_ENCODE_PERFORMANCE
unsigned int gTEncodeISum = 0;
unsigned int gTEncodeICnt = 0;
unsigned int gTEncodePSum = 0;
unsigned int gTEncodePCnt = 0;
#endif

void mpeg4EncodeIntrClr()
{
    unsigned int tmp;
    
    // clear mpeg4 interrupt register
    tmp = *(volatile unsigned int *)0xa812003c;	
     *(volatile unsigned int *)0xa812003c = tmp;	
     
    // write 1 to the corresponding interrupt status bit to clear the interrupt
    // 21: encode; 22: encode mv; 27: deocde
    rRISC_INT_STATUS = (0x1<<22);
}

void mpeg4MvIntrClr()
{
    unsigned int tmp;
    
    // read to clear mpeg4 mv interrupt register
    tmp = *(volatile unsigned int *)0xa8120038;	
     
    // write 1 to the corresponding interrupt status bit to clear the interrupt
    // 21: encode; 22: encode mv; 27: deocde
    //rRISC_INT_STATUS = (0x1<<21);
    rRISC_INT_STATUS = (0x1<<23);
}

void mpeg4DecodeIntrClr()
{
    unsigned int tmp;
    
    // clear mpeg4 interrupt register
    tmp = *(volatile unsigned int *)0xa8120224;	
     *(volatile unsigned int *)0xa8120224 = tmp;	

    //printf("D%x", *(volatile int *)0xa8120224); 
	 
    // write 1 to the corresponding interrupt status bit to clear the interrupt
    // 21: encode; 22: encode mv; 27: deocde
    rRISC_INT_STATUS = (0x1<<27);

    //printf("E%x", *(volatile int *)0xa8120224); 	
}

extern unsigned int isFirstVinFrame;
extern unsigned int *gpMpegH263UcosToLinuxShareBuf;

extern volatile unsigned int gFrameNum;
extern volatile unsigned int gTimeStamp;
void Mp4ISR()
{
#ifdef PRINT_MPEG_ENCODE_PERFORMANCE
    unsigned int t_encode;
#endif

#ifdef PRINT_MPEG_DECODE_PERFORMANCE
    unsigned int t_decode;
#endif
    unsigned int risc_intr_status;
    static unsigned int cnt = 0;    
    unsigned int count;
    unsigned int *pInt,packet[RPACKET_LEN/4];   
    int i;
    int drop_pkg;
 #ifndef AUDIO_MODULE_INCLUDE
    //IncSysTimeStamp();
#endif
    
    tYUVFrame *pTmp;
    unsigned char err;
    
   // printf("[ME%d]", GetCount());

    risc_intr_status = rRISC_INT_STATUS;
    
    /* encode interrupt */
    if(risc_intr_status & (0x1<<22)) {              
#ifdef  MV_ENABLE
                                                                                        //RiscClrCNT(MOTION_UCOS_TO_LINUX_CNT); 
    pInt = packet;
       *pInt = RISC_PKT_START | (tVideo<<8) | (tVideoTransferUcosToLinux<<16) | (0x8<<24);
       *(pInt + 2) = (unsigned int)MvStartWrAddr;                      // address item
       *(pInt + 3) = (unsigned int)Mv_count;                            // length item      
        * ((unsigned short*)(pInt + 4))=1;                                  //motion vector
        * ( (unsigned short*)(pInt + 4)+1)=MvFrameCnt;             //Mv Frame No.
       *(pInt + 7) = 0xffffffff;                                              // end flag          
                                                                                         //if(flag_mv_enable&&(RiscRdCNT(MOTION_UCOS_TO_LINUX_CNT)<35))
       if(flag_mv_enable&RiscRdCNT(MOTION_UCOS_TO_LINUX_CNT)<35)
        {
             if(TwoRiscCommSendPacket(packet, RPACKET_LEN) == SUCCESSFUL) {
               RiscIncCNT(MOTION_UCOS_TO_LINUX_CNT, 1);
    
                        RiscFlagSet();                                       // Set risc1 flag to interrupt risc1            
                    } else 
                       printf("zjb"); 
                    }

        Mv_count=0;
        updatetime=0;
        MvFrameCnt++;
#endif
        mpeg4EncodeIntrClr();    

#ifdef PRINT_MPEG_ENCODE_PERFORMANCE
        /* compute the time of a frame */
        t_encode = GetCount();      
        if(t_encode > mpeg_count)
        t_encode = t_encode - mpeg_count;
        else
            t_encode = CPU_MAIN_CLOCK / (2 * OS_TICKS_PER_SEC) + t_encode - mpeg_count; 
        //printf("<me=%d>", t_encode / (CPU_MAIN_CLOCK/(2*1000)));    // counted by mili-seconds
        printf("<me=%d>", t_encode / (CPU_MAIN_CLOCK/(2*1000000)));    // counted by microsecond
            
        if (mp4EncodeParams.isPFrame) {
            gTEncodePSum += t_encode / (CPU_MAIN_CLOCK/(2*1000000));     //calculate the sum of the encode time
            gTEncodePCnt++;                                 //indicate the occurs of me
        } else {
            gTEncodeISum += t_encode / (CPU_MAIN_CLOCK/(2*1000000));     //calculate the sum of the encode time
            gTEncodeICnt++;                                 //indicate the occurs of me
        }
#endif       

        /* need not be done now!
        if(OSSemAccept(gptSemMp4EncodeOutBufReady)) {
            gtMpegEncodeFrame.addr = mp4EncodeParams.out_bits | 0xa0000000;
            gtMpegEncodeFrame.len = (readMpeg4EncoderReg(eMPEG4_ENCODER_FRAME_WORD_COUNT) & 
                                     FRAME_WORD_COUNT_MASK);                    
        } else {
            isFirstVinFrame = 1;
            gCntMpegToUcosFull++;
            debug("\n\nMp4ISR: TaskTwoRiscSendMpegH263Encode is busy!\n\n");
        }*/

/*
do not need worry the share buffer is full! This has been checked in ScISR!
*/

        gtMpegEncodeFrame[gMpegEncodeFrameNumber].addr = mp4EncodeParams.out_bits | 0xa0000000;
        gtMpegEncodeFrame[gMpegEncodeFrameNumber].len = (readMpeg4EncoderReg(eMPEG4_ENCODER_FRAME_WORD_COUNT) & 
                                     FRAME_WORD_COUNT_MASK);
        gtMpegEncodeFrame[gMpegEncodeFrameNumber].framenum = gFrameNum++;
        gtMpegEncodeFrame[gMpegEncodeFrameNumber].timestamp = gTimeStamp;
        gtMpegEncodeFrame[gMpegEncodeFrameNumber].pframe= mp4EncodeParams.isPFrame;

        if(gtMpegEncodeFrame[gMpegEncodeFrameNumber].len * sizeof(int) < gMaxFrameSizeVideoUcosToLinux - 40) {    // less than the maxium size? (leave 40 bytes to hold checksum, frame number, etc.)
            if((err = OSMboxPost(gptMboxMpegEncodeOutFrame, &gtMpegEncodeFrame[gMpegEncodeFrameNumber])) != OS_NO_ERR) {
                isFirstVinFrame = 1;
                gCntTaskTwoRiscSendMpegH263EncodeBusy++;
                debug("\n\nMp4ISR: OSMboxPost error(%d)! TaskTwoRiscSendMpegH263Encode is still busy!^_^\n\n", err);
            } else {

                if(++gMpegEncodeFrameNumber >= sizeof(gtMpegEncodeFrame) / sizeof(tMpegEncodeFrame))
                    gMpegEncodeFrameNumber = 0;
                
                /* gpMpegH263UcosToLinuxShareBuf point to the next block! */
                gpMpegH263UcosToLinuxShareBuf = (unsigned int)gpMpegH263UcosToLinuxShareBuf + gMaxFrameSizeVideoUcosToLinux;
                     
                /* judge if reach the bottom of the share buffer */
                if((unsigned int)gpMpegH263UcosToLinuxShareBuf + gMaxFrameSizeVideoUcosToLinux > (unsigned int)VIDEO_UCOS_TO_LINUX_E_ADDR) {
                    gpMpegH263UcosToLinuxShareBuf = (unsigned int *)VIDEO_UCOS_TO_LINUX_B_ADDR;
                }
            }   
        } else {
            gCntMpegH263TooBig++;
            /* to see more details! */
            if(gtMpegEncodeFrame[gMpegEncodeFrameNumber].len * sizeof(int) > gMaxFrameSizeVideoUcosToLinux * 2) {
                gCntMpegH263TooTooBig++;    // if this occur, the encoded frame can be ruined!
            }
            if(gtMpegEncodeFrame[gMpegEncodeFrameNumber].addr + gtMpegEncodeFrame[gMpegEncodeFrameNumber].len * sizeof(int) > \
                (unsigned int)VIDEO_UCOS_TO_LINUX_E_ADDR | 0xa0000000) {
                gCntMpegH263TooBigRuinRecon++;
            }
            
            isFirstVinFrame = 1;
            printf("\n\nMp4ISR: gtMpegEncodeFrame.len is too big(%d)!", gtMpegEncodeFrame[gMpegEncodeFrameNumber].len * sizeof(int));
            if(mp4EncodeParams.isPFrame) {
                printf("P frame\n\n^_^");    
            } else {
                printf("I frame\n\n^_^");                    
            }
        }
                                
        if(OSSemPost(gptSemMpegEncodeReady) != OS_NO_ERR)
            debug("\n\nMp4ISR: OSSemPost error!\n\n");                                    
        
    }
    
    /* encode mv interrupt */
        if(risc_intr_status & (0x1<<23)){ 
            
#ifdef MV_ENABLE
                                                                                                                   //printf("&");
                  if(RiscRdCNT(MOTION_UCOS_TO_LINUX_CNT)<24)                     //protect overwrite
                    {
                                                                                                            //printf("0x%x",RiscRdCNT(MOTION_UCOS_TO_LINUX_CNT));
                          if((*(volatile unsigned int*)0xa8120038)&0x01)
                            {   updatetime++;
                          *(volatile unsigned int*)(MvStartWrAddr+Mv_count*4)=((*(volatile unsigned int*)0xa8120034)&0x3F3F7F7F);
                                                                                            //printf("0x%x,0x%x,",MvStartWrAddr+Mv_count*4,((*(volatile unsigned int*)0xa8120034)&0x3F3F7F7F));
                          }
                    }
                                                                                                      //printf("ox%x,",Mv_count);
                Mv_count++;//record the 
#endif
                    mpeg4MvIntrClr();
    }
        
#ifdef DISPLAY_INCLUDE
    /* decode interrupt */
    if(risc_intr_status & (0x1<<27)) {        

        unsigned int tmp;
        tmp = *(volatile int *)0xa8120224;
        
#ifdef PRINT_MPEG_DECODE_PERFORMANCE
        /* compute the time of a frame */
     t_decode = GetCount();     
     if(t_decode > mpeg_decode_start)
         t_decode = t_decode - mpeg_decode_start;
     else
         t_decode = CPU_MAIN_CLOCK / (2 * OS_TICKS_PER_SEC) + t_decode - mpeg_decode_start; 
         //printf("<md%d=%d>", tmp, t_decode / (CPU_MAIN_CLOCK/(2*1000)));    // counted by mili-seconds
     printf("<md%d=%d>", tmp, t_decode / (CPU_MAIN_CLOCK/(2*1000000)));    // counted by microsecond
     
     if (tmp == 2) {
         gTDecodeSumMd2 +=  t_decode / (CPU_MAIN_CLOCK/(2*1000000));  //calculate the sum of md2
         gTDecodeCntMd2++;                                                          //indicate the occurs of md2
     } else if (tmp == 4) {
         gTDecodeSumMd4 +=  t_decode / (CPU_MAIN_CLOCK/(2*1000000));   //calculate the sum of md4
         gTDecodeCntMd4++;                                                              //indicate the occurs of md4
        mpeg_decode_end = GetCount();
     }   
#endif

        while(1) {
            if(gIsFirstVideoOutFrame) {
                gIsFirstVideoOutFrame = 0;
            }            
#if 0
extern StayFrame;
                
           if(cnt++ == StayFrame) {
          if(StayFrame != 8) {  
                    mpeg4DecodeIntrClr();
                 return;                
          }
            }       
#endif

            mpeg4DecodeIntrClr();

            if(tmp == 0x4) {            
                logprintf(DEBUG_MESSAGE, MP4_DECODE, "V4,");
#ifdef AV_SYNC
                gMpegDecodeReady = DECODER_READY; 
#ifdef NEW_AVSYNC_ALGORITHM
                if(OSSemPost(gptMpegDecodeReadyToStart) != OS_NO_ERR)
                    printf("*");
#endif
#else
                if(OSSemPost(gptMpegDecodeReadyToStart) != OS_NO_ERR)
                    printf("*");
#endif

#ifdef VIDEO_JITTER_BUFFER
                RiscDecCNT(VIDEO_LINUX_TO_UCOS_CNT, 1);
                i = ReleaseCursor(&VideoBuffer1, VideoFrameReceive.addr);
#else
                RiscDecCNT(VIDEO_LINUX_TO_UCOS_CNT, 1);
#endif
                decodeFrameNum = linuxToUcosFrameNum;
                logprintf(DEBUG_MESSAGE, MP4_DECODE, "{%d}", RiscRdCNT(VIDEO_LINUX_TO_UCOS_CNT));
                return;
                
            }    else if(tmp == 0x2) {          
                logprintf(DEBUG_MESSAGE, MP4_DECODE, "V2,"); 
                break;
                
            }    else if(tmp == 0x6) {
                logprintf(FATAL_ERROR, MP4_DECODE, "V6,"); 
                break;
            
/*            
                if(OSSemPost(gptMpegDecodeReadyToStart) != OS_NO_ERR)
                    printf("*");                        
                logprintf(FATAL_ERROR, MP4_DECODE, "V6,");
                return;
*/                
                
            }    else if(tmp == 0x1) {  // decode is complete, decode should write 7 to re-start
                gIsFirstVideoOutFrame = 1;
                logprintf(FATAL_ERROR, MP4_DECODE, "V1"); 
#ifdef AV_SYNC
                gMpegDecodeReady = DECODER_READY; 
#else
                if(OSSemPost(gptMpegDecodeReadyToStart) != OS_NO_ERR)
                    printf("*");
#endif
                decodeFrameNum = linuxToUcosFrameNum;
                return;
                
            }    else {     
                logprintf(FATAL_ERROR, MP4_DECODE, "V%d,", tmp);    
#ifdef AV_SYNC
                gMpegDecodeReady = DECODER_READY; 
#endif
                return;
            }           
        }


#if 0
        if(!(gMpegDecodeOutputFrameNo & 0x1)) {         
            gtMpegDecodeOutputFrame0.y_addr = *(unsigned int *)0xa8120214;
            gtMpegDecodeOutputFrame0.u_addr = (gtMpegDecodeOutputFrame0.y_addr) + ((*(unsigned int *)0xa8120254)<<8);
            gtMpegDecodeOutputFrame0.v_addr = (gtMpegDecodeOutputFrame0.y_addr) + ((*(unsigned int *)0xa8120258)<<8);
        } else {
            gtMpegDecodeOutputFrame1.y_addr = *(unsigned int *)0xa8120214;
            gtMpegDecodeOutputFrame1.u_addr = (gtMpegDecodeOutputFrame1.y_addr) + ((*(unsigned int *)0xa8120254)<<8);
            gtMpegDecodeOutputFrame1.v_addr = (gtMpegDecodeOutputFrame1.y_addr) + ((*(unsigned int *)0xa8120258)<<8);
        }               
        
        if(gMpeg4DecodeOutputCount == 1) {

            if(!(gMpegDecodeOutputFrameNo & 0x1)) {         
                if(OSMboxPost(gptMpeg4DecodeOutputReady, &gtMpegDecodeOutputFrame0) != OS_NO_ERR) {
            CommInqSendString(UART_CHANNEL, "P");
            return;             
                } else {
                    gMpegDecodeOutputFrameNo++;
                    gMpeg4DecodeOutputCount++;                      
                }
         } else {
                if(OSMboxPost(gptMpeg4DecodeOutputReady, &gtMpegDecodeOutputFrame1) != OS_NO_ERR) {
            CommInqSendString(UART_CHANNEL, "P");
            return;             
                } else {
                    gMpegDecodeOutputFrameNo++;
                    gMpeg4DecodeOutputCount++;                      
                }
            }               
            
            switch(gMpeg4DecodeOutputBufNo) {
                case 0:
                    //CommInqSendString(UART_CHANNEL, "A");                 
                    gMpeg4DecodeOutputBufNo = 1;
                    mp4DecodeParams.opt_iaddr = ((unsigned int)gMpeg4DecodeOptIaddr1) & 0x1fffffff;                     
                    break;                  
                    
                case 1:
                    //CommInqSendString(UART_CHANNEL, "B");                 
                    gMpeg4DecodeOutputBufNo = 2;
                    mp4DecodeParams.opt_iaddr = ((unsigned int)gMpeg4DecodeOptIaddr2) & 0x1fffffff;                     
                    break;  

                case 2:
                    //CommInqSendString(UART_CHANNEL, "C");                 
                    gMpeg4DecodeOutputBufNo = 0;
                    mp4DecodeParams.opt_iaddr = ((unsigned int)gMpeg4DecodeOptIaddr0) & 0x1fffffff;                     
                    break;                  
                    
                default:
                    CommInqSendString(UART_CHANNEL, "*");                   
                    break;
                    
            }       

            writeMpeg4DecoderReg(eMP4_DEC_OUT_BUF_BASE_ADDR, mp4DecodeParams.opt_iaddr);
            
        } else {    
            CommInqSendString(UART_CHANNEL, "p");
        }
#endif
        gtStatistics.frameDecoded_ps++;
        if(gVideoEnableDisable == 0)   //close video command has received
            return;
        switch(gMpeg4DecodeOutputBufNo) {
            case 0:
                gMpeg4DecodeOutputBufNo = 1;
                mp4DecodeParams.opt_iaddr = gMpeg4DecodeOptIaddr1;
                    
                gtMpegDecodeOutputFrame[0].y_addr = gMpeg4DecodeOptIaddr0;
                gtMpegDecodeOutputFrame[0].u_addr = gMpeg4DecodeOptIaddr0 + ((*(unsigned int *)0xa8120254)<<8);
                gtMpegDecodeOutputFrame[0].v_addr = gMpeg4DecodeOptIaddr0 + ((*(unsigned int *)0xa8120258)<<8);
                gtMpegDecodeOutputFrame[0].width = (*(volatile unsigned int *)0xa8120228 & 0xffff0000) >> 16;
                gtMpegDecodeOutputFrame[0].height = *(volatile unsigned int *)0xa8120228 & 0xffff;
                
                pTmp = &gtMpegDecodeOutputFrame[0];

                break;                  
                    
            case 1:
                gMpeg4DecodeOutputBufNo = 2;
                mp4DecodeParams.opt_iaddr = gMpeg4DecodeOptIaddr2; 
                    
                gtMpegDecodeOutputFrame[1].y_addr = gMpeg4DecodeOptIaddr1;
                gtMpegDecodeOutputFrame[1].u_addr = gMpeg4DecodeOptIaddr1 + ((*(unsigned int *)0xa8120254)<<8);
                gtMpegDecodeOutputFrame[1].v_addr = gMpeg4DecodeOptIaddr1 + ((*(unsigned int *)0xa8120258)<<8);
                gtMpegDecodeOutputFrame[1].width = (*(volatile unsigned int *)0xa8120228 & 0xffff0000) >> 16;
                gtMpegDecodeOutputFrame[1].height = *(volatile unsigned int *)0xa8120228 & 0xffff;

                pTmp = &gtMpegDecodeOutputFrame[1];

                break;  

            case 2:
                gMpeg4DecodeOutputBufNo = 0;
                mp4DecodeParams.opt_iaddr = gMpeg4DecodeOptIaddr0; 
                  
                gtMpegDecodeOutputFrame[2].y_addr = gMpeg4DecodeOptIaddr2;
                gtMpegDecodeOutputFrame[2].u_addr = gMpeg4DecodeOptIaddr2 + ((*(unsigned int *)0xa8120254)<<8);
                gtMpegDecodeOutputFrame[2].v_addr = gMpeg4DecodeOptIaddr2 + ((*(unsigned int *)0xa8120258)<<8);
                gtMpegDecodeOutputFrame[2].width = (*(volatile unsigned int *)0xa8120228 & 0xffff0000) >> 16;
                gtMpegDecodeOutputFrame[2].height = *(volatile unsigned int *)0xa8120228 & 0xffff;

                pTmp = &gtMpegDecodeOutputFrame[2];

                break;                  
                    
            default:
                printf("*");                    
                break;
                    
        }
        pTmp->framenum = decodeFrameNum;
        drop_pkg = 0;

        switch(gRvoDisplayMode) {
            case RVO_DISPLAY_UPPER_PART:
                pTmp->height = pTmp->height / 2;  
                break;

            case RVO_DISPLAY_DOWN_PART:
                pTmp->height = pTmp->height / 2;
                pTmp->y_addr = pTmp->y_addr + pTmp->height * pTmp->width;
                pTmp->u_addr = pTmp->u_addr + pTmp->height * pTmp->width / 4;
                pTmp->v_addr = pTmp->v_addr + pTmp->height * pTmp->width / 4;
                break;

            case RVO_DISPLAY_BOTH:
            default:
                break;
        }

        
#ifdef AV_SYNC
        if(IsVideoJudgeDrop())
        {
            drop_pkg = 1;
            //printf("X\n");
            //to do : drop the video package
        }else
#endif
        {
            if(OSQPost(gptMpeg4DecodeOutputReady, pTmp) != OS_NO_ERR) {
            //if(OSMboxPost(gptMqueueVideoInFrame, tmp) != OS_NO_ERR) {
                printf("p(%d)\n", pTmp->framenum);
    
                switch(gMpeg4DecodeOutputBufNo) {
                    case 0:                 
                        gMpeg4DecodeOutputBufNo = 2;              
                        break;                  
                        
                    case 1:                 
                        gMpeg4DecodeOutputBufNo = 0;
                        break;  
    
                    case 2:                 
                        gMpeg4DecodeOutputBufNo = 1;
                        break;                  
                        
                    default:
                        printf("*");            
                        break;                  
                }   
            } 
            else {          
                writeMpeg4DecoderReg(eMP4_DEC_OUT_BUF_BASE_ADDR, mp4DecodeParams.opt_iaddr);
#ifdef DUMP_DECODE_OUTPUT
                if(g_dump_decode_output_en) {
                    _PRINTF("\n\n%d*%d\n\n", pTmp->width, pTmp->height);
                    _PRINTF("Y:\n\n");
                    for(i = 0; i < pTmp->width * pTmp->height; i += 4) {
                        _PRINTF("0x%x,", *(int *)(pTmp->y_addr + i | 0xa0000000));                    
                    }

                    _PRINTF("U:\n\n");
                    for(i = 0; i < pTmp->width * pTmp->height / 4; i += 4) {
                        _PRINTF("0x%x,", *(int *)(pTmp->u_addr + i | 0xa0000000));                    
                    }

                    _PRINTF("V:\n\n");
                    for(i = 0; i < pTmp->width * pTmp->height / 4; i += 4) {
                        _PRINTF("0x%x,", *(int *)(pTmp->v_addr + i | 0xa0000000));                    
                    }

                    _PRINTF("\n\nThe end!\n\n");        
                }
#endif
            }
        }

        if(pTmp->width != gtRvoCurrent.width || pTmp->height != gtRvoCurrent.height || gtRvoCurrent.interlaced != gtRvoCurrent1.interlaced) {                
            if(pTmp->width < MIN_WIDTH || pTmp->width > MAX_MPEG_DECODE_WIDTH ||
                pTmp->height < MIN_HEIGHT || pTmp->height > MAX_MPEG_DECODE_HEIGHT ||
               (pTmp->width & 7) != 0 ) {    // invalid width or height
                logprintf(FATAL_ERROR, MP4_DECODE, "\nMp4ISR: decode output invalid width or height!\n");
            } else {    // valid width & height 
                /* If the width or height is changed, the decode should be reset, 
                    other wise the decode output maybe NOT be ok(although the width & height are right)! */
                gRvoEnable = 1;
                if(gVoutEnable) {
                    gVoutReEnable = 1;
                }
                gtRvoCurrent.width = pTmp->width;
                gtRvoCurrent.height = pTmp->height;
                gtRvoCurrent.interlaced = gtRvoCurrent1.interlaced;   
                gtRvoCurrent.pixel_ratio = gtRvoCurrent1.pixel_ratio;
                gtRvoCurrent.line_ratio = gtRvoCurrent1.line_ratio;
                
                printf("RVO size changed in mp4ISR(%d,%d)!\n", gtRvoCurrent.width,gtRvoCurrent.height);

#ifndef EACH_VOL_RESET_DECODE                
                manual_reset_flag = 1;
#endif                

#ifdef PRINT_MPEG_DECODE_PERFORMANCE
                gTDecodeSumMd2 = 0;                    //clear the sum and occurs of md2 and md4
                gTDecodeCntMd2 = 0;
                gTDecodeSumMd4 = 0;
                gTDecodeCntMd4 = 0;
#endif
            }                    
        }        
        
        *(volatile int *)0xa8120204 = 0x2;      
#ifdef PRINT_MPEG_DECODE_PERFORMANCE
        mpeg_decode_start = GetCount();
#endif        
        
    }   
    
#endif
}    

void mpeg4IntrInit()
{
    static unsigned int mp4_intr_initialized = 0;
    unsigned int isr_base_addr;
    
#ifndef AUDIO_MODULE_INCLUDE
    //ResetSysTimeStamp();
#endif

    if(mp4_intr_initialized)
        return;

    mp4_intr_initialized = 1;

    // the fourth-interrupt table address	
    if(RiscRdID()  == RISC0)
        isr_base_addr = RISC0_FOURTH_VECTOR_B_ADDR | 0xa0000000;    // phy-addr is used to avoid flush cache
    else
        isr_base_addr = RISC1_FOURTH_VECTOR_B_ADDR | 0xa0000000;    // phy-addr is used to avoid flush cache     
    
    *(unsigned int *)(isr_base_addr + 0x120) = (unsigned int)Mp4ISR;   

    mpeg4EncodeIntrClr();    // clear the encode interrupt source
    
    RiscIntrEn(VCODEC_INTR);  // Enable the interrupt of encode
    	   
}

#if 0
void mpeg4EncoderIntrInit1()
{
    unsigned int isr_base_addr;

    // the fourth-interrupt table address	
    if(RiscRdID()  == RISC0)
        isr_base_addr = RISC0_FOURTH_VECTOR_B_ADDR | 0xa0000000;    // phy-addr is used to avoid flush cache
    else
        isr_base_addr = RISC1_FOURTH_VECTOR_B_ADDR | 0xa0000000;    // phy-addr is used to avoid flush cache     
    
    *(unsigned int *)(isr_base_addr + 0x120) = (unsigned int)Mp4EncodeISR1;   

    mpeg4EncodeIntrClr();    // clear the encode interrupt source
    
    RiscIntrEn(VCODEC_INTR);  // Enable the interrupt of encode
    	   
}

void Mp4EncodeISR1()
{    
    	
    mpeg4EncodeIntrClr();    

    if(OSSemAccept(gptSemMp4EncodeOutBufReady)) {
        if(gMpegEncodeOutBufNo == 0) {
            gMpegEncodeOutBufNo = 1;    // buffer 0 has data, switch to buffer 1                       
        } else if (gMpegEncodeOutBufNo == 1) {
            gMpegEncodeOutBufNo = 0;    // buffer 1 has data, switch to buffer 0                    
        }
        gtMpegEncodeFrame.addr = mp4EncodeParams.out_bits | 0xa0000000;
        gtMpegEncodeFrame.len = (readMpeg4EncoderReg(eMPEG4_ENCODER_FRAME_WORD_COUNT) & 
                                  FRAME_WORD_COUNT_MASK);		
        if(OSMboxPost(gptMboxMpegEncodeOutFrame, &gtMpegEncodeFrame) != OS_NO_ERR) {
            debug("\r\nMicrosOS post message error!\r\n");
        }			

        switch(VinBufferNumber) {        
            case 1:                            
                gtVideoInFrame.video_in_frame_y_addr = (unsigned int)gSavedVinData1;
                gtVideoInFrame.video_in_frame_u_addr = (unsigned int)gSavedVinData1 + 176*144;
                gtVideoInFrame.video_in_frame_v_addr = (unsigned int)gSavedVinData1 + 176*144* 5/4; 
                
                VinBufferNumber = 2;    // VIN buffer 1 has data, switch to buffer 2    
                
                break;
                
            case 2:                            
                gtVideoInFrame.video_in_frame_y_addr = (unsigned int)gSavedVinData2;
                gtVideoInFrame.video_in_frame_u_addr = (unsigned int)gSavedVinData2 + 176*144;
                gtVideoInFrame.video_in_frame_v_addr = (unsigned int)gSavedVinData2 + 176*144* 5/4; 
                
                VinBufferNumber = 3;    // VIN buffer 2 has data, switch to buffer 3    
                
                break;
                
            case 3:                            
                gtVideoInFrame.video_in_frame_y_addr = (unsigned int)gSavedVinData3;
                gtVideoInFrame.video_in_frame_u_addr = (unsigned int)gSavedVinData3 + 176*144;
                gtVideoInFrame.video_in_frame_v_addr = (unsigned int)gSavedVinData3 + 176*144* 5/4; 
                
                VinBufferNumber = 1;    // VIN buffer 3 has data, switch to buffer 1    
                
                break;
                
            default:
                break;
        }
        
        if(OSMboxPost(gptMboxVideoInFrame, &gtVideoInFrame) != OS_NO_ERR) {
            debug("\r\nMicrosOS post message error!\r\n");	
        }                                              
        
    } else {
        gCntMpegToUcosFull++;    
        debug("\r\nMp4EncodeISR: Mp4 Encode OutBuf is not ready!\r\n");
    }
                                
    //if(OSSemPost(gptSemMpegEncodeReady) != OS_NO_ERR)
    //    debug("\r\nMicrosOS post SEMAPHORE error!\r\n");                                    
    	
}
#endif

