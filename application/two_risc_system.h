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



#ifndef TWO_RISC_SYSTEM_H
#define TWO_RISC_SYSTEM_H

#ifdef   TWO_RISC_SYSTEM_GLOBALS
#define  TWO_RISC_SYSTEM_EXT
#else
#define  TWO_RISC_SYSTEM_EXT  extern
#endif


/*
***************************************************************************
*                           Type Definitions
***************************************************************************
*/
typedef struct {
    int en_print_all_msg;   // 1: on; 0: off
    int en_print_periodical_msg;    // 1: on; 0: off
} printmsg_control;


/*
***************************************************************************
*                              MACRO DEFINITIONS
***************************************************************************
*/
#define  TWO_RISC_SYSTEM_CLOSE_VOICE    1
#define  TWO_RISC_SYSTEM_CLOSE_VOUT    2
#define  TWO_RISC_SYSTEM_OPEN_VOUT    3
#define  TWO_RISC_SYSTEM_CLOSE_PIP    4
#define  TWO_RISC_SYSTEM_OPEN_PIP    5
#define  TWO_RISC_SYSTEM_CLOSE_DISPLAY_TASK    6
#define  TWO_RISC_SYSTEM_OPEN_VIN    7
#define  TWO_RISC_SYSTEM_CLOSE_OSD    8
#define  TWO_RISC_SYSTEM_OPEN_OSD    9
#define  TWO_RISC_SYSTEM_REOPEN_VIN_TASK    10
#define  TWO_RISC_SYSTEM_DEL_VIN_TASK   11
#define TWO_RISC_SYSTEM_OPEN_RVO    12
#define TWO_RISC_SYSTEM_CLOSE_RVO    13
#define TWO_RISC_SYSTEM_DVR_READ_MOTION_DETECTION    14

#define TWO_RISC_SYSTEM_REOPEN_TASK_ROLLSTR    15
#define TWO_RISC_SYSTEM_OPEN_TASK_ROLLSTR    16
#define TWO_RISC_SYSTEM_CLOSE_TASK_ROLLSTR    17

#define TWO_RISC_SYSTEM_SET_CLOCK 18

#define  TWO_RISC_SYSTEM_RECEIVE_LINUX_MSG    88
/*
***************************************************************************
*                           GLOBAL VARIABLES
***************************************************************************
*/
#define  STK_SIZE_TaskTwoRiscSystemMonitor       2000

TWO_RISC_SYSTEM_EXT OS_STK   StkTaskTwoRiscSystemMonitor[STK_SIZE_TaskTwoRiscSystemMonitor];

#ifdef TASK_STATISTIC_EN
#define  STK_SIZE_TaskStatistics    1000

TWO_RISC_SYSTEM_EXT OS_STK    StkTaskStatistics[STK_SIZE_TaskStatistics];

#define PRIOR_TASK_STATISTICS    28
#endif


#define ArrayQMsgSysMonitor_MAX     640 //8

TWO_RISC_SYSTEM_EXT  OS_EVENT *gptQMsgTwoRiscSystemMonitor;
TWO_RISC_SYSTEM_EXT void *ArrayQMsgSysMonitor[ArrayQMsgSysMonitor_MAX];

TWO_RISC_SYSTEM_EXT  unsigned int  gTwoRiscSystemMonitorMsg;

extern unsigned int SetVMode;

TWO_RISC_SYSTEM_EXT unsigned int  StasticFrameRate;

TWO_RISC_SYSTEM_EXT  volatile printmsg_control gtPrintMsgControl;

/*
***************************************************************************
*                              FUCTION PROTYPES
***************************************************************************
*/
void TaskTwoRiscSystemMonitorRisc0(void *data);
void TaskTwoRiscSystemMonitorRisc1(void *data);

#endif

