/*
***************************************************************************
*                Copyright (c) 2007 vBridge Microsystem, Inc.  
*                      Unpublished & Not for Publication
*                            All Rights Reserved                   
*   
* File        : vout_interface_cfg.h                     
*                                                                         
* Description :    
*             
* Date        : Fab.1st.2008                                                           
***************************************************************************
*/

#ifndef _VOUT_INTERFACE_CFG_H
#define _VOUT_INTERFACE_CFG_H
#include "chip_cfg.h"

#define NTSC    0
#define PAL    1
#define LCD    2

#ifdef VIDEO_MODULE_INCLUDE
                                                    
#define INCLUDE_ADV7171     1
#define INCLUDE_MTV335    1

#endif   //end of _VIDEO_MODUE_INCLUDE

#endif    //end of _VOUT_INTERFACE_CFG_H

