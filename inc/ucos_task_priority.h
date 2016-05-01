/*
***************************************************************************
*                Copyright (c) 2007 vBridge Microsystem, Inc.  
*                      Unpublished & Not for Publication
*                            All Rights Reserved                   
*   
* File        :                      
*                                                                         
* Description : list the task priorities of all the ucos tasks(except the
*               ucos system tasks)
*             
* Date        : Nov 1, 2007                                                           
***************************************************************************
*/


/*
uCOS can manage up to 64 tasks, however, uCOS uses two tasks for system use.
They are the IDLE & STATISTICS tasks. Each task must be assigned a unique 
priority level from 0 to 61. 
The lower the priority number, the higher the priority of the task. 
uCOS always executes the highest priority task ready to run.
*/

/* Note: the priority number should be from 0 to 61! */

#ifndef __UCOS_TASK_PRIORITY_H__
#define __UCOS_TASK_PRIORITY_H__

#include "chip_cfg.h"

//#define PRIOR_TASK_TA    3
//#define PRIOR_TASK_SLIC  4
#define PRIOR_TASK_DISPLAY        3
#define PRIOR_TASK_TA    4
#define PRIOR_TASK_SLIC  5

#define PRIOR_TASK_IPCAM    2
#ifdef DVR
//DVR project use cpu 60%~70%,so we need to set this task priority higher for now! 
//----------by vincent 2008.01.15
#define PRIOR_TASK_TWO_RISC_SEND_MPEG_H263_ENCODE    15
#else
#define PRIOR_TASK_TWO_RISC_SEND_MPEG_H263_ENCODE    27
#endif
#define PRIOR_TASK_RISC1IPCAM    10

//#define PRIOR_TASK_DISPLAY        11

#define PRIOR_TASK_MD_START        12
#define PRIOR_TASK_MD_PROCESS    26

#define PRIOR_TASK_RISC0 16
#define PRIOR_TASK_RISC1 16

#define PRIOR_TASK_ROLLSTR         20

#define PRIOR_TASK_GPIO              24
#define PRIO_TASK_REMOTECTL     23
#define PRIO_TASK_KEYPAD            25

#define PRIOR_TASK_STATISTICS   28
#define PRIOR_TASK_PRINT 30

/* Run on risc1, just for test */
#define PRIOR_TASK_PIP_CONTROL 31
#define PRIOR_TASK_RESOLUTION_CONTROL 32

#ifdef VIDEO_MODULE_INCLUDE

#endif

#ifdef VIDEO_MODULE_INCLUDE
                                                    
#endif

#ifdef DVR
                                                    
#endif

#endif
