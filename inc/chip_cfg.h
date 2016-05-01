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
* Date        : Oct 21, 2006                                                           
***************************************************************************
*/



#ifndef __CHIP_CFG_H__
#define __CHIP_CFG_H__



//#define    GNU_SIMULATOR
#define    DEBUG

#define PRINT_IN_TASK

/* single-cpu mode or dual-cpu mode */
//#define SINGLE_CPU_MODE
#define DUAL_CPU_MODE

#ifdef DUAL_CPU_MODE

/* risc1 run ucos or linux*/
//#define RISC1_UCOS
#define RISC1_LINUX		        							        

/* daisy-chain or non-daisy-chain */
#define DAISY_CHAIN

/* ucos boot risc1 or not */
#ifdef RISC1_UCOS
#define UCOS_BOOT_RISC1    // risc1 will be booted by ucos
#endif

#endif

/* uart0 or uart1 */
#define UART_CHANNEL    0x0

/* risc1 entry. Only valid if define RISC1_UCOS */
#define RISC1_ENTRY    0xb00000
//#define RISC1_ENTRY    0x20000


/* chip or fpga */
//#define FPGA
#define CHIP

/* cpu main clock */
#ifdef FPGA
#define CPU_MAIN_CLOCK    25000000
#endif

#ifdef CHIP
#define CPU_MAIN_CLOCK    200000000
#endif


/* Audio Module Include */
#ifdef VB_AUDIO      /* define in makefile */
#define AUDIO_MODULE_INCLUDE
#endif

/* Video Module Include */
#ifdef VB_VIDEO
#define VIDEO_MODULE_INCLUDE
#endif

/* DVR Module Include */
#ifdef VB_DVR
#define DVR
#define DVR_BOARD   // if use dvr board(integrate 4 vb2100 & 4 tech2835), please define it. If use tech2835 evalution board, undefine it. 
//#define TELEFORCE_DVR
#endif

/* TaskStatistics will be created or not. This task will statistilize the frame rate, etc. */
#define TASK_STATISTIC_EN

//===================BY VINCENT 2007.08.30==================
#define GPIO_TEST

//===================BY VINCENT 2007.08.30==================

/* to enable stack check */
#define CHIP_CFG_STACK_CHECK

#endif


