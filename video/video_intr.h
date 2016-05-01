/*
***************************************************************************
*                  Copyright (c) 2005 vBridge Microsystem, Inc.  
*                       Unpublished & Not for Publication
*                               All Rights Reserved                   
*   
* File        :                    
*                                                                         
* Description : 
*             
* Date        :                                                      
***************************************************************************
*/



#ifdef VIDEO_INTR_GLOBALS
#define VIDEO_INTR_EXT
#else 
#define VIDEO_INTR_EXT extern
#endif



VIDEO_INTR_EXT unsigned int giVideoInFirstFrame;

void  VinIntrEn(void);
void  VinISR(void);
void VoutIntrEn(void);
void  VoutISR(void);

