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

#include "chip_cfg.h"
#ifndef __AUDIO_CFG_H__
#define __AUDIO_CFG_H__



#define VB1100
//#define VB2100

#ifdef VB1100
//#define SLIC_CS    6  //slic 1
#define SLIC_CS    7  //slic 2
#define RESET_GPIO	10
#else

#ifdef VB2100
#define SLIC_CS    2  //VB2100 slic
#define RESET_GPIO    16
#endif

#endif

// Stone add for selecting winbond and si3210 slic
//#define INCLUDE_WINBOND_SLIC	1
#define INCLUDE_SI3210	1

#ifdef DVR
#define MAX_AUDIO_CHANNEL 4
#define MAX_TALKER_PER_CHANNEL 1
#else
#define MAX_AUDIO_CHANNEL 2
#define MAX_TALKER_PER_CHANNEL 4
#endif


#define MIPS_CHECK
#undef MIPS_CHECK

#define DUMP_ENABLE
#undef DUMP_ENABLE

#define JTB_TIME_CHECK
#undef JTB_TIME_CHECK

#ifdef JTB_TIME_CHECK
#define RIS0_TIMER_ADDR 0xA8000068
#define RestRisc0Timer() *((ULONG *) RIS0_TIMER_ADDR) = 0xffffffff
#define GetRisc0Timer() *((ULONG *) RIS0_TIMER_ADDR)
#endif
#ifdef DVR
//#define UCOS_USE_AUDIO_JITBUF     //define it to use audio jitbuffer
#endif
#define AUDIO_TEST_ENABLE
//#undef AUDIO_TEST_ENABLE

/* audio codec chip select */
//#define CODEC_TLV320AIC23B
//#define CODEC_TW2835


#define AUDIO_G723_ENABLE  //in fact, 30ms interrupt, 10ms interrupt must be enabled
//#undef AUDIO_G723_ENABLE

#define AUDIO_AID_DATA_ENABLE
//#undef AUDIO_AID_DATA_ENABLE

#endif
