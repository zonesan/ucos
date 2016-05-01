/*
***************************************************************************
*                  Copyright (c) 2005 vBridge Microsystem, Inc.  
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



#ifdef   TWO_RISC_VIDEO_GLOBALS
#define  TWO_RISC_VIDEO_EXT
#else
#define  TWO_RISC_VIDEO_EXT  extern
#endif

#include <video_api.h>
#include "video_cfg.h"

#include "os_cpu.h"
#include "os_cfg.h"
#include "uCOS_II.h"

#include  "2risc_memory_map.h"

//#define SAVE_REALTIMESHOW_SAME


/*
***************************************************************************
*                          MACRO DEFINITIONS
***************************************************************************
*/
//#define PRIOR_TASK_IPCAM    10


/*
***************************************************************************
*                           Type Definitions
***************************************************************************
*/
typedef  struct {
    unsigned int addr;
    unsigned int len;
    unsigned int pframe;
    unsigned int timestamp;     //added by wy
    unsigned int framenum;     //added by wy
} tMpegEncodeFrame;

typedef  struct {
    unsigned int addr;
    unsigned int len;
    unsigned int fg_and_idx;
    unsigned int tstamp;	
    //unsigned int stuff_byte_count;	
} tVideoUcosToLinux;

typedef  struct {
    unsigned int addr;
    unsigned int len;
    unsigned int tstamp;
    unsigned int dropflag;
    //unsigned int stuff_byte_count;	
} tVideoLinuxToUcos;

typedef  struct {
    unsigned int throw_frame_enable;	
    unsigned int invalid_frame;
    unsigned int frame_rate;    
} tMpegEncodeThrowFrame;

typedef  struct {
    unsigned int width;
    unsigned int height;
    unsigned int camera_format;    /* NTSC or PAL*/
    unsigned int yuv_format;    /* YUV422 or YUV420 */
    unsigned int interlaced;    /* 1: interlaced;   0: progressive*/   
} tVinFormat;

/*
***************************************************************************
*                           GLOBAL VARIABLES
***************************************************************************
*/
TWO_RISC_VIDEO_EXT  OS_EVENT *gptSemMpegEncodeReady;
TWO_RISC_VIDEO_EXT  OS_EVENT *gptMboxVideoInFrame;
TWO_RISC_VIDEO_EXT  OS_EVENT *gptMboxMpegEncodeOutFrame;

#if 0
#ifdef PIP_ENABLE
TWO_RISC_VIDEO_EXT  OS_EVENT *gptMqueueVideoInFrame;
TWO_RISC_VIDEO_EXT  void *arrayMsg[2];
#endif
#endif

TWO_RISC_VIDEO_EXT  OS_EVENT *gptVideoUcosToLinuxMbox;

TWO_RISC_VIDEO_EXT  tYUVFrame gtVideoInFrame[MAX_VIN_BUF_NUM];

TWO_RISC_VIDEO_EXT unsigned int gMpegEncodeFrameNumber;
TWO_RISC_VIDEO_EXT  tMpegEncodeFrame gtMpegEncodeFrame[3];

TWO_RISC_VIDEO_EXT  tVideoUcosToLinux gtVideoUcosToLinux;

TWO_RISC_VIDEO_EXT  tVideoLinuxToUcos gtVideoLinuxToUcos;

TWO_RISC_VIDEO_EXT  int gMpegEncodeOutBufNo;
TWO_RISC_VIDEO_EXT  OS_EVENT *gptSemMp4EncodeOutBufReady;

TWO_RISC_VIDEO_EXT  int gMaxCntIPCAMVideoShareBuf;
TWO_RISC_VIDEO_EXT  int gMaxCntVideoUcosToLinux;

TWO_RISC_VIDEO_EXT  int gMaxFrameSizeVideoUcosToLinux;

TWO_RISC_VIDEO_EXT  int gCntVideoUcosToLinuxFull;
TWO_RISC_VIDEO_EXT  int gCntVideoUcosToLinuxFullBeforeClearByUcos;
TWO_RISC_VIDEO_EXT  int gCntVideoUcosToLinuxFullClearByUcos;

TWO_RISC_VIDEO_EXT  int gCntMpegH263TooBig;     // encode frame exceed AssumedMaximumEncodeFrameSize
TWO_RISC_VIDEO_EXT  int gCntMpegH263TooTooBig;      // encode frame exceed AssumedMaximumEncodeFrameSize * 2
TWO_RISC_VIDEO_EXT  int gCntMpegH263TooBigRuinRecon;    // encode frame exceed AssumedMaximumEncodeFrameSize, which will ruin the reconstruction buffer
TWO_RISC_VIDEO_EXT  int gCntTaskTwoRiscSendMpegH263EncodeBusy;
TWO_RISC_VIDEO_EXT  int gCntMpegBusy;
TWO_RISC_VIDEO_EXT  int gCntMpegBusy1;
TWO_RISC_VIDEO_EXT  int gCntMpegEncodeH263TimeOut;
TWO_RISC_VIDEO_EXT  int gCntScVideoUcosToLinuxFull;


TWO_RISC_VIDEO_EXT  int gCntFrameFromUcosToLinuxOver;


TWO_RISC_VIDEO_EXT  tMpegEncodeThrowFrame gtMpegEncodeThrowFrame;
TWO_RISC_VIDEO_EXT  unsigned int gVinFrameCount;

TWO_RISC_VIDEO_EXT  unsigned int gSkipFrame;    /* if gSkipFrame = 0, do not give up any vin frames!
                                                                                    if gSkipFrame = 1, give up 1 vin frame, then accept 1 vin frame!
                                                                                    if gSkipFrame = 2, give up 2 vin frames, then accept 1 vin frame!
                                                                                    if gSkipFrame = 3, give up 3 vin frames, then accept 1 vin frame!
                                                                                    ...
                                                                                    So, if the Vin frame rate is 30,  the soft Vin framerate can just 30, 15, 10, 7.5, ...
                                                                                */

TWO_RISC_VIDEO_EXT  tVinFormat gtVinFormat;

/*
put video(ucos->linux) share buffer in front of the mpeg encode reconstruct buffer. 
So that even the share buffer is overflowed, just the reconstruct buffer is ruined(The probability is very little!). The system is safe!
*/
#ifdef NEW_MEMORY_MAP
TWO_RISC_VIDEO_EXT  unsigned int gMpeg4EncodeReconAndVideoShareBuffer[(MPEG_ENCODE_RECON_BUFSIZE + VIDEO_MAX_DATABUF_SIZE) / sizeof(int)] __attribute((aligned (16)));
//TWO_RISC_VIDEO_EXT  unsigned int gMpeg4EncodeReconBase[MPEG_ENCODE_RECON_BUFSIZE / sizeof(int)];
#else
TWO_RISC_VIDEO_EXT  unsigned int gMpeg4EncodeReconBase[MPEG_ENCODE_RECON_BUFSIZE / sizeof(int)];
#endif

/* Tasks Stacks */
#define  TaskTwoRiscMpegEncode_STK_SIZE      3000
#define  TaskTwoRiscSendMpegh263Encode_STK_SIZE      3000

TWO_RISC_VIDEO_EXT OS_STK   StkTaskTwoRiscMpegEncode[TaskTwoRiscMpegEncode_STK_SIZE];

TWO_RISC_VIDEO_EXT OS_STK   StkTaskTwoRisc0IPCamera[TaskTwoRiscMpegEncode_STK_SIZE];
TWO_RISC_VIDEO_EXT OS_STK   StkTaskTwoRisc1IPCamera[TaskTwoRiscMpegEncode_STK_SIZE];

TWO_RISC_VIDEO_EXT OS_STK   StkTaskTwoRiscSendMpegH263Encode[TaskTwoRiscSendMpegh263Encode_STK_SIZE];

TWO_RISC_VIDEO_EXT  unsigned int gFreqFromCrystal;  //TRUE:camera use external crystal clock,FALSE: camera use vout clock


#define MPEG4MODE   1
#define MJPEGMODE    2
#define MPEG_JPEG_MODE    3
#define H263_MODE    4                   //this macro definiton is for H.263 communication protocol add by xyx on Nov.13rd 2007

#define ENCODEBOTH 0
#define ENCODEUP 1
#define ENCODEDOWN 2

#define MPEG_ENCODE  0                 //macro definition for mpeg encode mode
#define H263_ENCODE  1                 //macro definition for H263 encode mode
TWO_RISC_VIDEO_EXT int gImgEncodeMode;
TWO_RISC_VIDEO_EXT int gEncodeMode;              /*the global var for encode mode selection add by xyx on Nov.13rd 2007
                                                                               *when the value is 0,the encode mode is mpeg, if the value is 1,the H.263
                                                                               *mode will be used as the encode mode.
                                                                               */


extern unsigned int VMode;

TWO_RISC_VIDEO_EXT  unsigned int UsedBandWidth;
TWO_RISC_VIDEO_EXT  unsigned int gChecksumEnabled;  //TRUE:enable, FALSE:disable

TWO_RISC_VIDEO_EXT unsigned int skip_no;

/*
***************************************************************************
*                              FUCTION PROTYPES
***************************************************************************
*/
void TaskTwoRisc0IPCameraFake(void *data);
void TaskTwoRisc1IPCamera(void *data);

void TaskTwoRiscVinMpegEncode(void *data);
void TaskTwoRiscSendMpegH263Encode(void);
void TaskTwoRiscVinQcifMpegEncode(void *data);
void TaskTwoRiscMpegEncode(void *data);
void TaskTwoRiscVgaMpegEncode(void *data);
void SetFrameRate(int frame_rate);

