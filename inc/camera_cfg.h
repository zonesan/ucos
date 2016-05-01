/*
***************************************************************************
*                Copyright (c) 2007 vBridge Microsystem, Inc.  
*                      Unpublished & Not for Publication
*                            All Rights Reserved                   
*   
* File        :                      
*                                                                         
* Description :    
*             
* Date        : Nov 1, 2007                                                           
***************************************************************************
*/


#ifndef __CAMERA_CFG_H__
#define __CAMERA_CFG_H__

#include "chip_cfg.h"

#ifdef VIDEO_MODULE_INCLUDE
                                                    
/* Cameras Support. */
/* for analog camera */
#define INCLUDE_ADV7181     1
#define INCLUDE_TVP5150A    1

/* for digital cameras */
#define INCLUDE_OV7720      1
#define INCLUDE_OV7670 	    1
#define INCLUDE_OV7650      1
#define INCLUDE_MT9M111	    1
//#define INCLUDE_MT9M112   1
#define INCLUDE_PO5130N     1

#endif
#endif

