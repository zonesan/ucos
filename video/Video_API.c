/*
*********************************************************************************************************
*                Copyright (c) 2005 vBridge Microsystem, Inc.  
*                      Unpublished & Not for Publication
*                            All Rights Reserved                   
*   
* File        :   Video_API.c                   
*                                                                         
* Description :   This file contains the application interface of video input
*									output, VSC, I2C interface to camera sensor and ADV7171 
*
* By					:		Eric
*             
* Date        : 	Apr 11, 2006                                                           
*********************************************************************************************************
*/



#include "video_cfg.h"
#ifdef DISPLAY_INCLUDE

//#include "Video_API.h"
#include "VOUT_API.h"
#include "Vout.h"
#include "comm.h"

#include "../mpeg/mpeg4_decoder.h"

#include "../port/os_cpu.h"
#include "../uCOS Configure/os_cfg.h"
#include "../uCOS2.52/uCOS_II.h"
#include "two_risc_display.h"



VOID Video_VOUT_RegInit(VOID)
{
    unsigned int lfmt_l_act, lfmt_l_blk;
    unsigned int vfmt_act, vfmt_top_blk, vfmt_bot_blk;

    /* clear bit27~bit30, then set */
    VOUT_CTL = VOUT_CTL & 0x87ffffff | BIT_VOUT_CLK_POLORITY | ((gtDisplayFormat.vout_interlaced == FALSE)<<29) | BIT_VOUT_FIRST_FIELD_ID | BIT_VOUT_CLIP_ENABLE;    

    if(gtDisplayFormat.vout_format == NTSC) {
        lfmt_l_act = VOUT_DISPLAY_NTSC_WIDTH / 2 -1;
        lfmt_l_blk= (854 - (lfmt_l_act + 1) * 2) / 2 -1;   // why 854?
        
        vfmt_act = (VOUT_DISPLAY_NTSC_HEIGHT >> (gtDisplayFormat.vout_interlaced == TRUE)) - 1;
        vfmt_top_blk = 9;   // for progressive, is 9?
        vfmt_bot_blk = 11;  // for progressive, is11?   
        
    } else if(gtDisplayFormat.vout_format == PAL) {
        lfmt_l_act = VOUT_DISPLAY_PAL_WIDTH / 2 -1;
        lfmt_l_blk = (854 - (lfmt_l_act + 1) * 2) / 2 -1;
        
        vfmt_act = (VOUT_DISPLAY_PAL_HEIGHT >> (gtDisplayFormat.vout_interlaced == TRUE)) - 1;
        vfmt_top_blk = 10;   // is 9?
        vfmt_bot_blk = 12;  // is11?        
    } else if(gtDisplayFormat.vout_format == LCD) {
        lfmt_l_act = gtDisplayFormat.width / 2 -1;
        lfmt_l_blk = (854 - (lfmt_l_act + 1) * 2) / 2 -1;
        
        vfmt_act = (gtDisplayFormat.height >> (gtDisplayFormat.vout_interlaced == TRUE)) - 1;
        vfmt_top_blk = 9;//189;//70;//10;   // is 9?
        vfmt_bot_blk = 17;//100;//72;//12;  // is11?    
    } else
        logprintf(FATAL_ERROR, VSC, "\ndisplay format error!\n");
    
    VOUT_LFMT = (lfmt_l_blk<<20) | (lfmt_l_act<<8);
    VOUT_VFMT0 = (vfmt_top_blk<<22) | (vfmt_act<<10) | vfmt_bot_blk;   
    VOUT_VFMT1 = (vfmt_top_blk<<22) | (vfmt_act<<10) | vfmt_bot_blk+1;   
    
    //VOUT_CLOCK_CTL = BIT_VOUT_CLOCK_ENABLE | BIT_VOUT_PLL_SOURCE | BIT_VOUT_DIVIDER_TYPE | BIT_VOUT_DIVIDER_VALUE;    

}          
#endif


