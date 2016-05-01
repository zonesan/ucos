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


#define NO_VIDEO_DATA

#ifdef   SAVED_VIN_DATA_GLOBALS
#define  SAVED_VIN_DATA_EXT
#else
#define  SAVED_VIN_DATA_EXT  extern
#endif

/* saved video in data: 176*144 QCIF 420 */
SAVED_VIN_DATA_EXT unsigned int gSavedVinData1[];
SAVED_VIN_DATA_EXT unsigned int gSavedVinData2[];
SAVED_VIN_DATA_EXT unsigned int gSavedVinData3[];

