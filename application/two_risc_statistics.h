/*
***************************************************************************
*                  Copyright (c) 2007 vBridge Microsystem, Inc.  
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

#ifndef __TWO_RISC_STATISTICS_H
#define __TWO_RISC_STATISTICS_H


#ifdef   TWO_RISC_STATISTICS_GLOBALS
#define  TWO_RISC_STATISTICS_EXT
#else
#define  TWO_RISC_STATISTICS_EXT  extern
#endif



/*
***************************************************************************
*                           Type Definitions
***************************************************************************
*/
typedef  struct {
    unsigned int video_in_fame_rate;
    unsigned int md_frame_rate;
    unsigned int frameEncoded_ps;           //encode rate
    unsigned int frameDecoded_ps;           //decode rate
    unsigned int VideoFrameUcosToLinux_ps;  //
    unsigned int vout_frame_ps;
    unsigned int time_in_intr;                      // max time in ISR
    unsigned int time_in_critical;                // max time of intr disabled
    unsigned int time_in_critical_ps;          //max time of intr disable in one second
    unsigned int pc_critical;                        //the pc value when max time of intr disable
    unsigned int frame_linux_to_ucos_ps;
    unsigned int audio_linux_to_ucos_ps;
    unsigned int flagISR_rate;
    unsigned int ISR_ID;

    unsigned int cnt_video_in;      // count the video in frames. Do not clear since starting work. For 30 framerate, it will overflow after 1657 days.
    unsigned int cnt_video_in_even_odd_reverse;    // count the video in even odd reverse. Just valid for vin interlaced. Do not clear since starting work.

    unsigned int cnt_video_linux_to_ucos;      // count the video frames from linux. Do not clear since starting work. For 30 framerate, it will overflow after 1657 days.
    
    /* the three element will add in encodeDataChksum. donot clear since start working. */
    unsigned int cnt_checksum_err;
    unsigned int cnt_framelength_err;
    unsigned int cnt_frame_number_err;

    unsigned int cnt_decodetimeout;             //count the number of decoder timeout

    unsigned int audioChannel[4];
    unsigned int audioInfo[4][10];
#ifdef DEBUG_AVSYNC
    unsigned int audio_out_ts;
    unsigned int video_out_ts;
    unsigned int audio_in_ts;
    unsigned int video_in_ts;

    unsigned int audio_out_cnt;
    unsigned int video_out_cnt;
    unsigned int audio_jitbuf_depth;
    unsigned int video_jitbuf_depth;
#endif
    unsigned int averFrameLen;           //average frame length in bytes during one second

    /* count the unexpected intrs & exceptions */
    unsigned int cnt_all_excpts;     // cnt_all_excpts counts all the exceptions(intr excluded) & unknow mips intr     
    unsigned int cnt_all_excpts_not_caused_by_icache;     // cnt_all_excpts_not_caused_by_icache counts all the exceptions(intr excluded) & unknow mips intr, which are not caused by i cache fill error!
    unsigned int cnt_unknow_mips_intr;
    unsigned int cnt_vout_intr;
    unsigned int cnt_vin_intr;
    unsigned int cnt_mc_intr;
    unsigned int cnt_mc_intr_by_audio;
    unsigned int cnt_excpt_tlb;     // TLBL & TLBS
    unsigned int cnt_excpt_ade;     // ADEL & ADES  
    unsigned int cnt_excpt_ri;     // reserve instruction exception
    unsigned int cnt_excpt_cpu;     // co-processor unusable
    unsigned int cnt_excpt_others;     // other exceptions
} tStatistics;



/*
***************************************************************************
*                           GLOBAL VARIABLES
***************************************************************************
*/
//TWO_RISC_STATISTICS_EXT int vin_frame_ps;
TWO_RISC_STATISTICS_EXT volatile tStatistics gtStatistics;

/*
***************************************************************************
*                              FUCTION PROTYPES
***************************************************************************
*/
void TaskStatisticsPrintTaskStackInfo(int task_prior);
void TaskStatisticsPrintTaskInfo();
void TaskStatistics(void *data);

#endif

