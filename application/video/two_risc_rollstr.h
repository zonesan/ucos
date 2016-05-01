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


#include "video_cfg.h"
#include "includes.h"

#ifndef TWO_RISC_ROLLSTR_H
#define TWO_RISC_ROLLSTR_H

#ifdef   TWO_RISC_ROLLSTR_GLOBALS
#define  TWO_RISC_ROLLSTR_GLOBALS_EXT
#else
#define  TWO_RISC_ROLLSTR_GLOBALS_EXT  extern
#endif


#define STK_SIZE_ROLLSTR    4000
TWO_RISC_ROLLSTR_GLOBALS_EXT unsigned int StkTaskRollStr[STK_SIZE_ROLLSTR];

/*
***************************************************************************
*                           Type Definitions
***************************************************************************
*/
typedef  struct {
    unsigned int start_pixel;
    unsigned int start_line;
    unsigned int str_width;
    unsigned int str_height;
    unsigned int char_width;
    unsigned int char_height;
    unsigned int str_len;
    unsigned char *str_addr;
    unsigned int attribute;
} tRollStrFormat;


/*
***************************************************************************
*                           GLOBAL VARIABLES
***************************************************************************
*/

TWO_RISC_ROLLSTR_GLOBALS_EXT  volatile tRollStrFormat gtRollStr;
TWO_RISC_ROLLSTR_GLOBALS_EXT char OSDStr[1000];                  //save the string into ucos local buf

void TaskRollString(void *data);

#endif

