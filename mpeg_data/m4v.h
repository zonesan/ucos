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



#ifdef M4V_GLOBAL
#define M4V_EXT 
#else
#define M4V_EXT extern
#endif


#define NO_MPEG_DATA
//#define JAPAN
//#define USA_WOMAN
//#define USA_WORKER 
 
#ifdef NO_MPEG_DATA
#define SIZE_M4V    1
#endif

#ifdef JAPAN
#define SIZE_M4V    1382
#endif

#ifdef USA_WOMAN
#define SIZE_M4V    67247
#endif

#ifdef USA_WORKER
#define SIZE_M4V    269698
#endif

extern unsigned int m4v[SIZE_M4V];

extern unsigned int m4v_frame_count[60];
