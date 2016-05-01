/*
*********************************************************************************************************
*                                Copyright (c) 2005 vBridge Microsystem, Inc.  
*                                      Unpublished & Not for Publication
*                                            All Rights Reserved                   
*   
* File        : bsp.h             
*                                                                         
* Description :                      
*                                                
* By          : Gong Bin               
*
* Date        : July 11, 2005                                                           
*********************************************************************************************************
*/

#ifndef BSP_H
#define BSP_H

#ifdef   BSP_GLOBALS
#define  BSP_EXT
#else
#define  BSP_EXT  extern
#endif

#define  ISR_ADDR_BASE    0x80000300

/*
*********************************************************************************************************
*                                               TYPE DEFINITIONS
*********************************************************************************************************
*/
typedef struct {
    unsigned int status;
    unsigned int cause;
} CP0_STATUS_CAUSE;

/*
*********************************************************************************************************
*                                               FUCTION PROTYPES
*********************************************************************************************************
*/
void  BspPrintRegs (void);
void  AppMain        (void);
void  BspStartTicker (unsigned int ticks_per_sec);
void  delay          (int m, int n);
void  MaskAllInts    (void);
void  uCOS_OTHER_INTHandler(void);
void  BspPrintfCp0Reg(int to_use_print_task);

void  BspPrintIntrMsgIP0(void);
void  BspPrintIntrMsgIP1(void);
void  BspPrintIntrMsgIP2(void);
void  BspPrintIntrMsgIP3(void);
void  BspPrintIntrMsgIP4(void);
void  BspPrintIntrMsgIP5(void);
void  BspPrintIntrMsgIP6(void);
void  BspPrintIntrMsgIP7(void);

#endif

