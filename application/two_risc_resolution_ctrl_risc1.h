/*
***************************************************************************
*                  Copyright (c) 2007 vBridge Microsystem, Inc.  
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

#ifndef __TWO_RISC_RESOLUTION_CONTROL_H
#define __TWO_RISC_RESOLUTION_CONTROL_H

#ifdef  TWO_RISC_RESOLUTION_CONTROL_GLOBALS
#define TWO_RISC_RESOLUTION_CONTROL_EXT
#else
#define TWO_RISC_RESOLUTION_CONTROL_EXT extern
#endif

/*configuration for task TaskResolutionControl*/
#define STK_SIZE_TaskResolutionControl 1000
TWO_RISC_RESOLUTION_CONTROL_EXT OS_STK StkTaskResolutionControl[STK_SIZE_TaskResolutionControl];

/*
***************************************************************************
*                              FUCTION PROTYPES
***************************************************************************
*/
void TaskResolutionControl(void *data);

#endif



