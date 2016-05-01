/*
***************************************************************************
*                Copyright (c) 2005 vBridge Microsystem, Inc.  
*                      Unpublished & Not for Publication
*                            All Rights Reserved                   
*   
* File        :                      
*                                                                         
* Description :    
*             
* Date        : Feb 15, 2006                                                           
***************************************************************************
*/

//#define C0_COUNT
#define TW_SLV_ADDR 0xff
#define TV_MODE_NTSC 0
#define TV_MODE_PAL 1
#define TV_MODE TV_MODE_NTSC 

void AppMain(void);

void tw2835_io_init(void);
void tw2835_reset();
int select_chip(unsigned int cs);




