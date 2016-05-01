/*
***************************************************************************
*                  Copyright (c) 2005 vBridge Microsystem, Inc.  
*                       Unpublished & Not for Publication
*                               All Rights Reserved                   
*   
* File        : mtv335.h                     
*                                                                         
* Description :   
*             
* Date        : Fab.1st.2008                                                           
***************************************************************************
*/

#ifndef _MONITOR_H
#define _MONITOR_H

#include "def_type.h"

#define REG_ID  0x6A
#define MTV335_IIC_ADDR 0x35


/* function prototype declaration*/
int HwMTV335Initialize(unsigned int video_system);
int GetMTV335ID(void);

#endif

