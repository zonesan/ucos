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



#ifndef __VOUT_H
#define __VOUT_H

#ifdef  VOUT_GLOBALS
#define VOUT_EXT
#else
#define VOUT_EXT  extern
#endif

#include "risc.h"



/*
***************************************************************************
*                          MACRO DEFINITIONS
***************************************************************************
*/
#define  VOUT_DISPLAY_WIDTH VOUT_DISPLAY_NTSC_WIDTH
#define  VOUT_DISPLAY_HEIGHT VOUT_DISPLAY_NTSC_HEIGHT

#define VOUT_DISPLAY_NTSC_WIDTH     704
#define VOUT_DISPLAY_NTSC_HEIGHT    480

#define VOUT_DISPLAY_PAL_WIDTH     704
#define VOUT_DISPLAY_PAL_HEIGHT    576


/* for NTSC(720 * 480) */
#define VOUT_L_BLK_NTSC     ((854 - (VOUT_L_ACT_NTSC + 1) * 2) / 2 -1)
#define VOUT_L_ACT_NTSC     (VOUT_DISPLAY_WIDTH / 2 -1)

#define VOUT_TOP_BLK_NTSC   9
#define VOUT_ACT_NTSC       (VOUT_DISPLAY_HEIGHT - 1)
#define VOUT_BOT_BLK_NTSC   11

/* Video Output Control Registers Specifications */
#define  VOUT_BASE_ADDR    BLOCK_BASE_ADDR(VOUT_BLOCK)

#define rVOUT_CTL      	 (*(volatile unsigned *)(VOUT_BASE_ADDR + 0x0))    	/* Output Video control register */
#define rVOUT_LFMT       (*(volatile unsigned *)(VOUT_BASE_ADDR + 0x4))     /* Video output line format */
#define rVOUT_VFMT0      (*(volatile unsigned *)(VOUT_BASE_ADDR + 0x8))     /* Video output field 0 format */
#define rVOUT_VFMT1      (*(volatile unsigned *)(VOUT_BASE_ADDR + 0xC))     /* Video output field 1 format */
#define rVOUT_INTR_STS   (*(volatile unsigned *)(VOUT_BASE_ADDR + 0x10))    /* Video output interrupt status */



#endif
