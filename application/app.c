/*
***************************************************************************
*                   Copyright (c) 2005 vBridge Microsystem, Inc.  
*                          Unpublished & Not for Publication
*                                 All Rights Reserved                   
*   
* File        : app.c             
*                                                                         
* Description : Application code. After uCOS is initialized and 
*               the first task is created, uCOS will be started.      
*                                                
* By          : Gong Bin               
*
* Date        : Oct 4, 2005                                                           
***************************************************************************
*/



#define   APP_GLOBALS

#include  "includes.h"
#include  "two_risc_comm.h"
#include  "2risc_memory_map.h"
#include  "two_risc_system.h"
#include  "../application/two_risc_system.h"
#include  "../application/two_risc_system_risc1.h"
#include "ucos_task_priority.h"

#include "chip_cfg.h"

/*
***************************************************************************
*                          GLOBAL FUNCTIONS
***************************************************************************
*/

/*
***************************************************************************
*                               AppMain
*
* Description : The main function.  
*
* Arguments   : None
*
* Returns     : None.  
*
* Notes       : 
***************************************************************************
*/
void AppMain (void)
{       
    
    OSInit();                                           // Initialize uC/OS-II
        
    _PRINTF("uCOS-ii has been initialized successfully!\r\n");

    if(RiscRdID() == RISC0)
        #ifdef CHIP_CFG_STACK_CHECK
        OSTaskCreateExt(TaskTwoRiscSystemMonitorRisc0, (void *)0, (void *)&StkTaskTwoRiscSystemMonitor[sizeof(StkTaskTwoRiscSystemMonitor) / sizeof(StkTaskTwoRiscSystemMonitor[0]) - 1], PRIOR_TASK_RISC0,\
                PRIOR_TASK_RISC0, (void *)&StkTaskTwoRiscSystemMonitor[0], sizeof(StkTaskTwoRiscSystemMonitor) / sizeof(StkTaskTwoRiscSystemMonitor[0]), (void *)0, OS_TASK_OPT_STK_CHK+OS_TASK_OPT_STK_CLR);	
        #else
        OSTaskCreate(TaskTwoRiscSystemMonitorRisc0, (void *)0, (void *)&StkTaskTwoRiscSystemMonitor[STK_SIZE_TaskTwoRiscSystemMonitor - 1], PRIOR_TASK_RISC0);	
        #endif
    else
        #ifdef CHIP_CFG_STACK_CHECK
        OSTaskCreateExt(TaskTwoRiscSystemMonitorRisc1, (void *)0, (void *)&StkTaskTwoRiscSystemMonitor[sizeof(StkTaskTwoRiscSystemMonitor) / sizeof(StkTaskTwoRiscSystemMonitor[0]) - 1], PRIOR_TASK_RISC1,\
                PRIOR_TASK_RISC1, (void *)&StkTaskTwoRiscSystemMonitor[0], sizeof(StkTaskTwoRiscSystemMonitor) / sizeof(StkTaskTwoRiscSystemMonitor[0]), (void *)0, OS_TASK_OPT_STK_CHK+OS_TASK_OPT_STK_CLR);	
        #else        
        OSTaskCreate(TaskTwoRiscSystemMonitorRisc1, (void *)0, (void *)&StkTaskTwoRiscSystemMonitor[STK_SIZE_TaskTwoRiscSystemMonitor - 1], PRIOR_TASK_RISC1);	
	 #endif
    OSStart();                                          // Start multitask
}
