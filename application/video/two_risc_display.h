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


#ifndef __TWO_RISC_DISPLAY_H
#define __TWO_RISC_DISPLAY_H

#include "video_cfg.h"
#include "includes.h"
#ifdef DISPLAY_INCLUDE

#ifdef   TWO_RISC_DISPLAY_GLOBALS
#define  TWO_RISC_DISPLAY_EXT
#else
#define  TWO_RISC_DISPLAY_EXT  extern
#endif

#include "chip_cfg.h"
#include <video_api.h>
#include "mpeg4_decoder.h"
/*
***************************************************************************
*                              MACRO DEFINITIONS
***************************************************************************
*/
#define NTSC    0
#define PAL      1
#define LCD      2

#define DECODER_READY       0
#define DECODER_BUSY         1

#define  MPEG_DECODE_THRESHHOLD       60    // counted by mili-seconds

#define  STK_SIZE_TaskTwoRiscDisplay       4000

#define SC_VIDEO_LINUX_TO_UCOS    SOFT_WR_1_S
#define SC_VIDEO_LINUX_TO_UCOS_ADDR    SOFT_WR_6_S_ADDR
#define VIDEO_L2U_PKG_NUMBER    8

#define RVO_DISPLAY_BOTH 0
#define RVO_DISPLAY_UPPER_PART 1
#define RVO_DISPLAY_DOWN_PART 2

/*
***************************************************************************
*                           Type Definitions
***************************************************************************
*/
typedef  struct {
    unsigned int width;
    unsigned int height;
    unsigned int vout_format;    /* NTSC or PAL*/
    unsigned int vout_interlaced;    /* 1: interlaced;   0: progressive*/  
    unsigned int yuv_input_format;    /* YUV420 or YUV422 */    
    unsigned int yuv_output_format;    /* YUV420 or YUV422 */
    unsigned int pixel_ratio;
    unsigned int line_ratio;    
} tDisplayFormat;

typedef  struct {
    unsigned int start_pixel;
    unsigned int start_line;
    unsigned int stop_pixel;
    unsigned int stop_line;
//    unsigned int pixel_ratio;
//    unsigned int line_ratio; 
} tPipSetting;

typedef  struct {
    unsigned int osd_format;
    unsigned int  fill_addr;
    unsigned int  display_addr;
    unsigned int  *color_pallate;
    unsigned short width;
    unsigned short height;
    unsigned short start_pixel;  
    unsigned short start_line;   
    unsigned short stop_pixel;  
    unsigned short stop_line; 
    unsigned char backgroundColor;
    unsigned char  OSD_T_Mask;
} gtOSDSetting;

/*
***************************************************************************
*                           GLOBAL VARIABLES
***************************************************************************
*/
TWO_RISC_DISPLAY_EXT OS_STK   StkTaskTwoRiscDisplay[STK_SIZE_TaskTwoRiscDisplay];

/* for 422 format */
//TWO_RISC_DISPLAY_EXT  unsigned int gMpeg4DecodeMcIaddr[640 * 480];


/* 422
TWO_RISC_DISPLAY_EXT  unsigned int gMpeg4DecodeOptIaddr0[640 * 480 * 4 / 8];
TWO_RISC_DISPLAY_EXT  unsigned int gMpeg4DecodeOptIaddr1[640 * 480 * 4 / 8];
TWO_RISC_DISPLAY_EXT  unsigned int gMpeg4DecodeOptIaddr2[640 * 480 * 4 / 8];
*/
                      
TWO_RISC_DISPLAY_EXT  unsigned int gMpeg4DecodeMcIaddr[MPEG_DECODE_RECON_BUFSIZE / sizeof(int)] __attribute((aligned (16)));

TWO_RISC_DISPLAY_EXT  unsigned int _gMpeg4DecodeOptIaddr0[MPEG_DECODE_OUTPUT_BUFSIZE / sizeof(int) + 4];
TWO_RISC_DISPLAY_EXT  unsigned int _gMpeg4DecodeOptIaddr1[MPEG_DECODE_OUTPUT_BUFSIZE / sizeof(int) + 4];
TWO_RISC_DISPLAY_EXT  unsigned int _gMpeg4DecodeOptIaddr2[MPEG_DECODE_OUTPUT_BUFSIZE / sizeof(int) + 4];

#define gMpeg4DecodeOptIaddr0    (((unsigned int)_gMpeg4DecodeOptIaddr0 + 0x10) & 0x1ffffff0)
#define gMpeg4DecodeOptIaddr1    (((unsigned int)_gMpeg4DecodeOptIaddr1 + 0x10) & 0x1ffffff0)
#define gMpeg4DecodeOptIaddr2    (((unsigned int)_gMpeg4DecodeOptIaddr2 + 0x10) & 0x1ffffff0)

/* for 420 */

TWO_RISC_DISPLAY_EXT  unsigned int gMpeg4DecodeOutputBufNo;
TWO_RISC_DISPLAY_EXT  unsigned int gMpeg4DecodeOutputCount;

TWO_RISC_DISPLAY_EXT  volatile unsigned int gIsFirstVideoOutFrame;

TWO_RISC_DISPLAY_EXT  unsigned int gIsFour;

TWO_RISC_DISPLAY_EXT  OS_EVENT *gptVideoLinuxToUcosMbox;

TWO_RISC_DISPLAY_EXT  OS_EVENT *gptMpegDecodeReadyToStart;
/***********************************************************************************************/

TWO_RISC_DISPLAY_EXT unsigned int frameWaitingToDecode;            //is there a frame delayed to be decoded;1:yes;0:no
TWO_RISC_DISPLAY_EXT OS_EVENT *gptRiscTimerIntrOccur;                 //sem:risc timer interrupt has occured
TWO_RISC_DISPLAY_EXT volatile unsigned int gMpegDecodeReady;   //mpeg decoder readey to decode
TWO_RISC_DISPLAY_EXT volatile unsigned int gRiscTimerCnt;            //time passed after the frame before:ms

/**********************************************************************************************/
extern tMPEG4_DECODER_PARMS mp4DecodeParams;

TWO_RISC_DISPLAY_EXT  OS_EVENT *gptMpeg4DecodeOutputReady;

TWO_RISC_DISPLAY_EXT  tYUVFrame   gtMpegDecodeOutputFrame[3];
TWO_RISC_DISPLAY_EXT  tYUVFrame   gtMpegDecodeOutputFrame0;
TWO_RISC_DISPLAY_EXT  tYUVFrame   gtMpegDecodeOutputFrame1;
TWO_RISC_DISPLAY_EXT  unsigned int   gMpegDecodeOutputFrameNo;

TWO_RISC_DISPLAY_EXT  tYUVFrame   gtStartUpPic;

TWO_RISC_DISPLAY_EXT  OS_EVENT *gptMboxMpegDecodeOutFrame;

TWO_RISC_DISPLAY_EXT  tYUVFrame gtRvoCurrent;
TWO_RISC_DISPLAY_EXT  tYUVFrame gtRvoCurrent1;      // for backup in case Rvo is interlaced but start-up picture is progressive!

TWO_RISC_DISPLAY_EXT  gtOSDSetting gtOsdCurrent;

#ifdef PIP_ENABLE
TWO_RISC_DISPLAY_EXT  OS_EVENT *gptMqueueVideoInFrame;
TWO_RISC_DISPLAY_EXT  void *arrayMsg[2];
#endif

#ifdef OSD_ENABLE
TWO_RISC_DISPLAY_EXT  OS_EVENT *gptMqueueOSDFrame;
TWO_RISC_DISPLAY_EXT  void *ArrayMsgOsd[2];
#endif

TWO_RISC_DISPLAY_EXT  tYUVFrame gtPipCurrent;


TWO_RISC_DISPLAY_EXT  unsigned int mpeg_decode_start;
TWO_RISC_DISPLAY_EXT  unsigned int gMaxDecodeCount;
TWO_RISC_DISPLAY_EXT unsigned int mpeg_decode_end;

TWO_RISC_DISPLAY_EXT  unsigned int gDecodeOddEven;

TWO_RISC_DISPLAY_EXT  unsigned int gVoutEnable;
TWO_RISC_DISPLAY_EXT  unsigned int gVoutReEnable;
TWO_RISC_DISPLAY_EXT  unsigned int gRvoEnable;
TWO_RISC_DISPLAY_EXT  unsigned int gPipEnable;
TWO_RISC_DISPLAY_EXT  unsigned int gOsdEnable;

TWO_RISC_DISPLAY_EXT  int manual_err_flag;
TWO_RISC_DISPLAY_EXT  int manual_reset_flag;
TWO_RISC_DISPLAY_EXT  int manual_stop_flag;
TWO_RISC_DISPLAY_EXT  int decode_flag;              //indicate the decode bit stream mode H263 or MPEG

TWO_RISC_DISPLAY_EXT  void *ArrayMsgDecode[2];

TWO_RISC_DISPLAY_EXT unsigned char frameToDecode[AssumedMaximumEncodeFrameSize] __attribute((aligned (16)));

#ifdef DUMP_DECODE_OUTPUT
TWO_RISC_DISPLAY_EXT  int g_dump_decode_output_en;
#endif
//TWO_RISC_DISPLAY_EXT  unsigned int gDisplay_width, gDisplay_height;
TWO_RISC_DISPLAY_EXT  tDisplayFormat gtDisplayFormat;
TWO_RISC_DISPLAY_EXT  tPipSetting gtPipSetting;

TWO_RISC_DISPLAY_EXT  unsigned int gRvoDisplayMode;     /* There are three modes till now.
                                                                                                 1. Display the whole rvo;
                                                                                                 2. Display the upper part;
                                                                                                 3. Display the down part.
                                                                                                */

void TaskTwoRiscDisplay(void *data);
int TwoRiscDisplayRecPacket(unsigned int sID, unsigned int *packet, unsigned int number);
int TwoRiscDisplaySendPacket(unsigned int sID, unsigned int *message, unsigned int len);
void TwoRiscDisplayRvoPipOsdInit();
void TwoRiscDisplayStartupPicInit(void);
#endif

#endif

