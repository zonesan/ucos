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



#ifdef   MPEG4_ENCODER_INTR_CLR_GLOBALS
#define  MPEG4_ENCODER_INTR_CLR__EXT
#else
#define  MPEG4_ENCODER_INTR_CLR__EXT  extern
#endif

#ifdef PRINT_MPEG_ENCODE_PERFORMANCE
MPEG4_ENCODER_INTR_CLR__EXT unsigned int mpeg_count;
#endif

#define MPEG_ENCODE_OUTPUT_PING_PONG_0    0x200000
#define MPEG_ENCODE_OUTPUT_PING_PONG_1    0x300000
//#define COMPUTERTIME			

extern  unsigned int Mv_count;
extern unsigned int MbCnt;//记录在MB中的位置
#define MbSize  5*1024
extern unsigned int MvFrameCnt;
#define MvStartWrAddr  (0xa0000000|MOTION_UCOS_TO_LINUX_B_ADDR+(MvFrameCnt%24)*MbSize)
#define MvStartAddr       (0xa0000000|MOTION_UCOS_TO_LINUX_B_ADDR)
void mpeg4EncodeIntrClr();
void mpeg4MvIntrClr();
void mpeg4DecodeIntrClr();

void Mp4ISR();
void mpeg4IntrInit();

void mpeg4EncoderIntrInit1();
void Mp4EncodeISR1();


