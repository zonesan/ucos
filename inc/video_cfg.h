/*
***************************************************************************
*                Copyright (c) 2005 vBridge Microsystem, Inc.  
*                      Unpublished & Not for Publication
*                            All Rights Reserved                   
*   
* File        :                      
*                                                                         
* Description :    
*             
* Date        : Oct 24, 2006                                                           
***************************************************************************
*/


#ifndef __VIDEO_CFG_H__
#define __VIDEO_CFG_H__

#include "chip_cfg.h"
#include "camera_cfg.h"

#ifdef VIDEO_MODULE_INCLUDE

                                                    
#define DISPLAY_INCLUDE     // the display task will be created!
#ifdef DISPLAY_INCLUDE
#ifndef DVR
#define OSD_ENABLE
#define PIP_ENABLE
#endif

#ifdef DVR  
#undef STARTUP_PICTURE     // DVR do not need start-up picture
#else
#define STARTUP_PICTURE     // If define STARTUP_PICTURE, the display will be started by ucos itself, NOT by linux API
#endif

#ifdef STARTUP_PICTURE
//#define STARTUP_PICTURE_VBRIDGE
#define STARTUP_PICTURE_UTSMART
//#define STARTUP_PICTURE_MOADIGITAL
#endif

#ifdef DVR
//#define EACH_VOL_RESET_DECODE
#else
#define EACH_VOL_RESET_DECODE   // if define this, decode will be reset at each vol, else just reset when resolution changed
#endif

#endif    // #ifdef DISPLAY_INCLUDE

#define AV_SYNC
#ifdef AV_SYNC
#define VIDEO_JITTER_BUFFER    // the decode input will use jitter-buffer
#define NEW_AVSYNC_ALGORITHM  //use the new avsync algorithm, not motivate by 10ms risc timer
#define NEW_JITBUF_ALGORITHM    //define it to use new jitbuffer code: 16Byte aligned, remain 16~31Bytes not used at the end of the frame
#define DEBUG_AVSYNC           // define this, will print the avsync debug information
#else
#define DEBUG_AVSYNC           // define this, will print the avsync debug information
#endif

#define BAND_WIDTH_CONTROL    // when defines this, bandwidth control works, Q value will change every frame.

#define MOTION_DETECTION
//#define MV_ENABLE           //Enable motion vector
#define LIGHT_ALARM_EN       //Enable lignt alarm

/* define buffer size */
//#define JPEG_SXGA
#ifdef JPEG_SXGA
#define MAX_VIN_WIDTH      1280
#define MAX_VIN_HEIGHT     1024
#else     // #ifdef JPEG_SXGA
#ifdef DISPLAY_INCLUDE
#define MAX_VIN_WIDTH      704
#define MAX_VIN_HEIGHT     576
#else    // #ifdef DISPLAY_INCLUDE. IPCAM can just support the maxium VGA for memory limit
#define MAX_VIN_WIDTH      640
#define MAX_VIN_HEIGHT     480
#endif     // #ifdef DISPLAY_INCLUDE
#endif     // #ifdef JPEG_SXGA

//#define MAX_MPEG_FRAME_SIZE     (MAX_VIN_WIDTH * MAX_VIN_HEIGHT / 2)

/* 
    The Vin buf size. There are only one buf, divided into three buffers! The buf starts from 16-byte-aligned address! 
        
    If Vin is 420,
        VIN_BUF_SIZE = (MAX_VIN_WIDTH * MAX_VIN_HEIGHT * 3 / 2) * 3; 
        MAX_VIN_UV = MAX_VIN_WIDTH * MAX_VIN_HEIGHT / 4;
        
    If Vin is 422,
        VIN_BUF_SIZE = (MAX_VIN_WIDTH * MAX_VIN_HEIGHT * 4 / 2) * 3; 
        MAX_VIN_UV = MAX_VIN_WIDTH * MAX_VIN_HEIGHT / 2;                
*/
#define MAX_VIN_BUF_NUM  4
#define VIN_YUV_SIZE          (MAX_VIN_WIDTH * MAX_VIN_HEIGHT * 3 / 2)
#define VIN_BUF_SIZE   ((MAX_VIN_WIDTH * MAX_VIN_HEIGHT * 3 / 2) * MAX_VIN_BUF_NUM)
#define MAX_VIN_Y_SIZE        (MAX_VIN_WIDTH * MAX_VIN_HEIGHT)
#define MAX_VIN_UV_SIZE       (MAX_VIN_WIDTH * MAX_VIN_HEIGHT / 4)

#define MIN_WIDTH      64
#define MIN_HEIGHT     36

#ifdef JPEG_SXGA
#define MAX_MPEG_ENCODE_WIDTH   704
#define MAX_MPEG_ENCODE_HEIGHT  576
#else
#define MAX_MPEG_ENCODE_WIDTH   MAX_VIN_WIDTH
#define MAX_MPEG_ENCODE_HEIGHT  MAX_VIN_HEIGHT
#endif

#define MAX_MPEG_FRAME_SIZE     (MAX_MPEG_ENCODE_WIDTH * MAX_MPEG_ENCODE_HEIGHT / 2)

#ifdef JPEG_SXGA
#define DISPLAY_INCLUDE

#define MAX_MPEG_DECODE_WIDTH   1
#define MAX_MPEG_DECODE_HEIGHT  1
#else   // the maxium decode size should be 704*576 according to GUC manual even if the maxium Vin size is much smaller.
#define MAX_MPEG_DECODE_WIDTH   704
#define MAX_MPEG_DECODE_HEIGHT  576    //new memmap:576, old memmap:480
#endif     // #ifdef JPEG_SXGA

/* The mpeg encode output buf size. There are two bufs! The bufs start from 16-byte-aligned address! */
#define MPEG_ENCODE_OUTPUT_BUFSIZE    (MAX_MPEG_ENCODE_WIDTH * MAX_MPEG_ENCODE_HEIGHT / 2)

/* The mpeg encode reconstruct buf size. There is one buf, but divided into two parts! */
#define MPEG_ENCODE_RECON_BUFSIZE    (MAX_MPEG_ENCODE_WIDTH * MAX_MPEG_ENCODE_HEIGHT * 3)

/* 
    The mpeg decode output buf size. There are three bufs! The bufs start from 16-byte-aligned address! 
    MAX_WIDTH * MAX_HEIGHT / 4 should be multiple of 256! 
    
    If decode output is 420, 
        MPEG_DECODE_OUTPUT_BUFSIZE = MAX_WIDTH * MAX_HEIGHT * 3 / 2;
        MPEG_DECODE_OUTPUT_V_OFFSET = MAX_WIDTH * MAX_HEIGHT / 4;
        
    If decode output is 422, 
        MPEG_DECODE_OUTPUT_BUFSIZE = MAX_WIDTH * MAX_HEIGHT * 4 / 2;
        MPEG_DECODE_OUTPUT_V_OFFSET = MAX_WIDTH * MAX_HEIGHT /2;        
*/
#define MPEG_DECODE_OUTPUT_BUFSIZE    (MAX_MPEG_DECODE_WIDTH * MAX_MPEG_DECODE_HEIGHT * 3 / 2)
#define MPEG_DECODE_OUTPUT_UV_SIZE    (MAX_MPEG_DECODE_WIDTH * MAX_MPEG_DECODE_HEIGHT / 4)

/* The mpeg decode reconstruct buf size. There is one buf, but divided into two parts! */
#define MPEG_DECODE_RECON_BUFSIZE    (MAX_MPEG_DECODE_WIDTH * MAX_MPEG_DECODE_HEIGHT * 3)


/* Macro definitions just for debug. Please do not change them when release! */
#define EACH_I_VOL    // each I frame will have a vol

//#define DUMP_M4V    // dump m4v from uart, the m4v file will be about 512k. Note: you will not see pictures when dumping. 
//#define DUMP_YUV      // dump YUV frame in sc isr.
//#define DUMP_YUV_JPEG      // dump YUV frame in IPCAM task in JPEG mode
//#define DUMP_DECODE_OUTPUT

#ifdef DUMP_M4V
#define DUMP_M4V_FRAME_CNT    10  // how many frames to dump?
#endif

//#define PRINT_MPEG_ENCODE_PERFORMANCE   // print how long encode each frame
//#define PRINT_MPEG_DECODE_PERFORMANCE   // print how long encode each frame

#define AssumedMaximumEncodeFrameSize    85008    /* assume the maximum mpeg size is 85k. 85008 is multiple of 16.
                                                                                            For 80000 & pal(continuous running for 13 hours, uncache copy, no checksum), 
                                                                                                mpeg size about 0.5 percent will exceed 80000. About 0.09 percent mpeg will be busy.   
                                                                                        */

/* obsolete definitions */
#define EVEN_ODD    // just for interlaced Vin. if define it, a frame will be EVEN + ODD(you will see two half-picture!).

#endif    // #ifdef VIDEO_MODULE_INCLUDE
#endif    // #ifndef __VIDEO_CFG_H__

