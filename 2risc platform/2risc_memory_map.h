/*
***************************************************************************
*                  Copyright (c) 2005 vBridge Microsystem, Inc.  
*                       Unpublished & Not for Publication
*                               All Rights Reserved                   
*   
* File        : risc.h                     
*                                                                         
* Description :    
*             
* Date        : Sep 28, 2006                                                           
***************************************************************************
*/

#include "video_cfg.h"

#ifndef __2RISC_MEMORY_MAP_H__
#define __2RISC_MEMORY_MAP_H__

#define NEW_MEMORY_MAP

#ifdef SYSVB1100
#include "2risc_memory_map_16M.h"   // for VB1100 TA
#else 

#ifndef DISPLAY_INCLUDE

#ifndef AUDIO_MODULE_INCLUDE
#ifdef NEW_MEMORY_MAP
#include "2risc_memory_map_16M_IPC_No_Display_new_memory_map.h"    // for IPCAM only
#else
#include "2risc_memory_map_16M_IPC_No_Display.h"    // for IPCAM only
#endif
#else
#ifdef NEW_MEMORY_MAP
#include "2risc_memory_map_IPC_new_memory_map.h"   // for IPCAM + DISPLAY(with or without audio) or VB2100 TA
#else
#include "2risc_memory_map_IPC.h"   // for IPCAM + DISPLAY(with or without audio) or VB2100 TA
#endif
#endif

#else

#ifdef DVR
#ifdef NEW_MEMORY_MAP
#include "2risc_memory_map_dvr_new_memory_map.h"
#else
#include "2risc_memory_map_dvr.h"
#endif
#else
#ifdef NEW_MEMORY_MAP
#include "2risc_memory_map_IPC_new_memory_map.h"   // for IPCAM + DISPLAY(with or without audio) or VB2100 TA
#else
#include "2risc_memory_map_IPC.h"   // for IPCAM + DISPLAY(with or without audio) or VB2100 TA
#endif
#endif

#endif

#endif  // end of #ifdef SYSVB1100

#endif  // end of #ifndef __2RISC_MEMORY_MAP_H__

