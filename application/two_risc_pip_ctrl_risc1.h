/*
***************************************************************************
*                  Copyright (c) 2007 vBridge Microsystem, Inc.  
*                        Unpublished & Not for Publication
*                               All Rights Reserved                   
*   
* File        :two_risc_pip_ctrl_risc1.h                      
*                                                                         
* Description :    
*             
* Date        :                                                      
***************************************************************************
*/

#ifndef __TWO_RISC_PIP_CONTROL_H
#define __TWO_RISC_PIP_CONTROL_H

#ifdef  TWO_RISC_PIP_CONTROL_GLOBALS
#define TWO_RISC_PIP_CONTROL_EXT
#else
#define TWO_RISC_PIP_CONTROL_EXT extern
#endif

/*configuraton for task TaskPIPControl*/
#define STK_SIZE_TaskPIPControl  1000
TWO_RISC_PIP_CONTROL_EXT OS_STK  StkTaskPIPControl[STK_SIZE_TaskPIPControl];

/*
***************************************************************************
*                              FUCTION PROTYPES
***************************************************************************
*/
void TaskPIPControl(void *data);

#endif

