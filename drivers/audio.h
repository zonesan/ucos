/*
***************************************************************************
*                  Copyright (c) 2005 vBridge Microsystem, Inc.  
*                        Unpublished & Not for Publication
*                              All Rights Reserved                   
*   
* File        : audio.h                     
*                                                                         
* Description : audio drivers
*
* Date        :
***************************************************************************
*/
#ifndef AUDIO_H
#define AUDIO_H


#ifdef   AUDIO_GLOBALS
#define  AUDIO_EXT
#else
#define  AUDIO_EXT  extern
#endif

#include "risc.h"
#include "chip_cfg.h"


/*
*************************************************************************************
*          The values is to be changed by users to meet different requirements
*************************************************************************************
*/
#ifdef   DVR 
#if 1  //audio use external clock(from tw2835)---DVR 16ch board need to modify harware
#define  CLK_IO       0x0         /* 0-ACLK is input; 1-ACLK is output */
#define  FS_IO        0x0         /* 0-AFS is input; 1-AFS is output */
#define  FS_AL        0x1         /* 0-AFS is aligned with ACLK rising edge;
                                            1-AFS is aligned with ACLK falling edge;*/
#endif  //audio use internal clock(from VB2100)                                        
//#define  CLK_IO       0x1         /* 0-ACLK is input; 1-ACLK is output */
//#define  FS_IO        0x1         /* 0-AFS is input; 1-AFS is output */
//#define  FS_AL        0x0         /* 0-AFS is aligned with ACLK rising edge;*/                                         
#else
#define  CLK_IO       0x1         /* 0-ACLK is input; 1-ACLK is output */
#define  FS_IO        0x1         /* 0-AFS is input; 1-AFS is output */
#define  FS_AL        0x0         /* 0-AFS is aligned with ACLK rising edge;*/
#endif            

//#define  FS_IO        0x1         /* 0-AFS is input; 1-AFS is output */
#define  FS_W         0x0         /* 0-FS is single pulse; 1-FS is half period */
#define  FS_P         0x0         /* 0-FS is active high; 1-FS is active low  */
//#define  CLK_IO       0x1         /* 0-ACLK is input; 1-ACLK is output */
//#define  CLK_IO       0x0         /* 0-ACLK is input; 1-ACLK is output */    //; for test aclk input

//#define  DI_DLY       0x1         /* 0-AIN has no delay; 1-AIN has one cycle delay */
#define  DI_DLY       0x1
//#define  DO_DLY       0x1         /* 0-AOUT has no delay; 1-AOUT has one cycle delay */
#define  DO_DLY       0x1
//#define  FS_AL        0x0         /* 0-AFS is aligned with ACLK rising edge;
//                                     1-AFS is aligned with ACLK falling edge;*/
#define  DI_AL        0x1         /* 0-AIN is aligned with ACLK rising edge;
                                     1-AIN is aligned with ACLK falling edge;*/
#define  DO_AL        0x0         /* 0-AOUT is sampled with ACLK rising edge;
                                     1-AOUT is sampled with ACLK falling edge;*/
//#define  CLK_DIV      0x2         /* Divider value for generating ACLK, the range is from 0 to 255 */
#define  CLK_DIV      0x0
#define  FS_DIV       0xf         /* Divider value for generating AFS, the range is from 0 to 255 */
 
//#define  CLK_DIV      0x0         /* Divider value for generating ACLK, the range is from 0 to 255 */
//#define  FS_DIV       0xb         /* Divider value for generating AFS, the range is from 0 to 255 */

//#define  ITS0         0x0         /* Time slot number for channel 0 for input direction */ 
#define  ITS0         0
#define  ITS1         1         /* Time slot number for channel 1 for input direction */
#define  ITS2         3         /* Time slot number for channel 2 for input direction */
#define  ITS3         4         /* Time slot number for channel 3 for input direction */

//#define  OTS0         0x0         /* Time slot number for channel 0 for output direction */ 
#define  OTS0         0x1e
#define  OTS1         0          /* Time slot number for channel 1 for output direction */
#define  OTS2         4         /* Time slot number for channel 2 for output direction */
#define  OTS3         5         /* Time slot number for channel 3 for output direction */

#define  ICH3_CNT     0x0         /* Number of active time slots for input channel 3, the range is 0,1,2,3 */
#define  ICH2_CNT     0x0         /* Number of active time slots for input channel 2, the range is 0,1,2,3 */
#define  ICH1_CNT     0x0         /* Number of active time slots for input channel 1, the range is 0,1,2,3 */
#define  ICH0_CNT     0x1         /* Number of active time slots for input channel 0, the range is 0,1,2,3 */

#define  OCH3_CNT     0x0        /* Number of active time slots for output channel 3, the range is 0,1,2,3 */
#define  OCH2_CNT     0x0        /* Number of active time slots for output channel 2, the range is 0,1,2,3 */
#define  OCH1_CNT     0x0         /* Number of active time slots for output channel 1, the range is 0,1,2,3 */
#define  OCH0_CNT     0x1          /* Number of active time slots for output channel 0, the range is 0,1,2,3 */

#define  OCH3_PF      0x0         /* Specify whether output channel 3 need pre-fetch or not 
			 						 1-need; 0- no need */
#define  OCH2_PF      0x0         /* Specify whether output channel 2 need pre-fetch or not 
			                				 1-need; 0- no need */
#define  OCH1_PF      0x0          /* Specify whether output channel 1 need pre-fetch or not 
									 1-need; 0- no need */
//#define  OCH0_PF      0x1          /* Specify whether output channel 0 need pre-fetch or not 
					//				 1-need; 0- no need */
#define  OCH0_PF      1

#define  OCH_VAL      0x0         /* 1-All the output time slots within an AFS are all valid */
#define  ICH_VAL      0x0         /* 1-All the input time slots within an AFS are all valid  */

/*
***************************************************************************
*                        Audio register definition
***************************************************************************
*/
#define AUD_BASE_ADDR    BLOCK_BASE_ADDR(AUD_BLOCK) 

#define rAUD_CTL      (*(volatile unsigned *)(AUD_BASE_ADDR + 0x0))   /* Audio control register */
#define rAUD_INTS     (*(volatile unsigned *)(AUD_BASE_ADDR + 0x4))   /* Audio input time slots */
#define rAUD_OUTTS    (*(volatile unsigned *)(AUD_BASE_ADDR + 0x8))   /* Audio output time slots */
#define rAUD_SID      (*(volatile unsigned *)(AUD_BASE_ADDR + 0xC))   /* Audio stream ID register */
#define rAUD_STAT     (*(volatile unsigned *)(AUD_BASE_ADDR + 0x10))  /* Audio status register */

/*
***************************************************************************
*                        Audio register bit definitions
***************************************************************************
*/
// Audio control register AUD_CTL bit definition
#define BIT_ENA          (0x1<<31)
#define BIT_FS_IO        (FS_IO<<30)
#define BIT_FS_W         (FS_W<<29)
#define BIT_FS_P         (FS_P<<28)
#define BIT_CLK_IO       (CLK_IO<<27)
#define BIT_DI_DLY       (DI_DLY<<26)
#define BIT_DO_DLY       (DO_DLY<<25)
#define BIT_FS_AL        (FS_AL<<24)
#define BIT_DI_AL        (DI_AL<<23)
#define BIT_DO_AL        (DO_AL<<22)
#define BIT_CLK_DIV      (CLK_DIV<<8)
#define BIT_FS_DIV       (FS_DIV<<0)

// Audio input time slots register AUD_INTS bit definition
#define BIT_ITS3         (ITS3<<24)
#define BIT_ITS2         (ITS2<<16)
#define BIT_ITS1         (ITS1<<8)
#define BIT_ITS0         (ITS0<<0)

// Audio output time slots register AUD_OUTTS bit definition
#define BIT_OTS3         (OTS3<<24)
#define BIT_OTS2         (OTS2<<16)
#define BIT_OTS1         (OTS1<<8)
#define BIT_OTS0         (OTS0<<0)

// Audio stream ID register AUD_SID bit definition
#define BIT_ICH3_CNT     (ICH3_CNT<<30)
#define BIT_ICH2_CNT     (ICH2_CNT<<28)
#define BIT_ICH1_CNT     (ICH1_CNT<<26)
#define BIT_ICH0_CNT     (ICH0_CNT<<24)

#define BIT_OCH3_CNT     (OCH3_CNT<<22)
#define BIT_OCH2_CNT     (OCH2_CNT<<20)
#define BIT_OCH1_CNT     (OCH1_CNT<<18)
#define BIT_OCH0_CNT     (OCH0_CNT<<16)

#define BIT_OCH3_PF      (OCH3_PF<<15)
#define BIT_OCH2_PF      (OCH2_PF<<14)
#define BIT_OCH1_PF      (OCH1_PF<<13)
#define BIT_OCH0_PF      (OCH0_PF<<12)

#define BIT_OCH_VAL      (OCH_VAL<<11)
#define BIT_ICH_VAL      (ICH_VAL<<10)

// Audio status register AUD_STAT bit definition
#define BIT_INTR_EN      (0x1<<31)
#define BIT_INTR_WR      (0x1<<30)
#define BIT_INTR_RD      (0x1<<29)

/*
*********************************************************************************************************
*                              Miscellaneous
*********************************************************************************************************
*/
#define EN_AUDIO()       rAUD_CTL  |=  BIT_ENA          // Enable audio interface.
#define DIS_AUDIO()      rAUD_CTL  &= ~BIT_ENA          // Disable audio interface.

#define INTR_EN()        rAUD_STAT |=  BIT_INTR_EN      // Enable the audio interrupts
#define INTR_DIS()       rAUD_STAT &= ~BIT_INTR_EN    // Disable the audio interrupts  

#define CLEAR_INTR_WR()  rAUD_STAT |=  BIT_INTR_WR
#define CLEAR_INTR_RD()  rAUD_STAT |=  BIT_INTR_RD

/*
***************************************************************************
*                           LOCAL VARIABLES
***************************************************************************
*/



/*
***************************************************************************
*                           Macro Definitions
***************************************************************************
*/
#define PREFETCH    1
#define NON_PREFETCH    0

#define AUDIO_OPENED    1
#define AUDIO_CLOSED    0

/*
***************************************************************************
*                           Type Definitions
***************************************************************************
*/
typedef  struct {
    unsigned int time_input_slot_number;
    unsigned int time_output_slot_number;
    unsigned int time_slot_count;
    unsigned int prefetch;    
} tAudioChannelPara;

/*
***************************************************************************
*                           FUCTION PROTYPES
***************************************************************************
*/
AUDIO_EXT unsigned int channel_history_pointer_initialized[4];
AUDIO_EXT unsigned int channel_history_pointer_recount[4];
AUDIO_EXT unsigned int channel_history_pointer[4];
AUDIO_EXT unsigned int time_stamp[4];
AUDIO_EXT unsigned int his_len[4];

/*
***************************************************************************
*                           FUCTION PROTYPES
***************************************************************************
*/
void  AudioStart   (void);
void  AudioISR     (void);
void  AudioRegInit (void);
void  AudioIntrEn(void);
void  AudioIntrDis(void);

int AppAudioTimingSet(void);
int AppAudioChannelOpen(int channel_number, tAudioChannelPara *ptPara);
int AppAudioChannelClose(int channel_number);
int AppAudioOpen(void);
int AppAudioClose(void);
void AppAudioReset(void);
void AppAudioClkOpen(void);
void AppAudioResetReEnable(void);
int AppAudioWriteData(int channel_number, int *data, int length);
int AppAudioGetStatus(void);
int AppAudioReadOutputCount(unsigned int channel_number);
//int AppAudioReadHistoryData(int channel_number, int *data, int length, int time_stamp);
int AppAudioReadHistoryData(int channel_number, int *data, int length);
int AppAudioReadData(int channel_number, int *data, int length);

#endif

