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



#define   M4V_GLOBAL
#include  "m4v.h"
 
 
unsigned int m4v_frame_count[60] = {
    0x000002A4, 0x00000118, 0x0000004C, 0x00000014,
    0x00000014, 0x0000001C, 0x00000014, 0x00000014,
    0x00000024, 0x00000018, 0x00000014, 0x00000018, 
    0x00000014, 0x00000018, 0x00000020, 0x00000290, 
    0x0000011C, 0x00000068, 0x00000030, 0x0000002C, 
    0x00000028, 0x00000028, 0x0000001C, 0x0000001C, 
    0x00000020, 0x0000001C, 0x0000001C, 0x00000028, 
    0x00000018, 0x00000020, 0x0000028C, 0x00000100, 
    0x0000005C, 0x00000034, 0x0000001C, 0x0000001C, 
    0x00000030, 0x0000002C, 0x00000030, 0x00000038, 
    0x00000024, 0x0000001C, 0x00000018, 0x00000020, 
    0x00000018, 0x00000284, 0x00000108, 0x00000050, 
    0x00000024, 0x00000014, 0x00000014, 0x00000014, 
    0x00000014, 0x0000001C, 0x00000018, 0x00000020, 
    0x00000020, 0x00000020, 0x00000014, 0x00000008	
};

#ifdef NO_MPEG_DATA
unsigned int m4v[SIZE_M4V] = {};
#endif


#ifdef JAPAN
#include "japan.h"
#endif

#ifdef USA_WOMAN
#include "usa_woman.h"
#endif

#ifdef USA_WORKER
#include "usa_worker.h"
#endif


