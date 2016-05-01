/*
***************************************************************************
*                  Copyright (c) 2005 vBridge Microsystem, Inc.  
*                       Unpublished & Not for Publication
*                               All Rights Reserved                   
*   
* File        : watch_dog.h                     
*                                                                         
* Description : 
*             
* Date        : Mar 8, 2006                                                           
***************************************************************************
*/



#include "risc.h"



/* Memory Controller register specifications */
#define  UART_BASE_ADDR    BLOCK_BASE_ADDR(UART_BLOCK)

#define  rWdCounter    *(volatile unsigned *)(UART_BASE_ADDR + 0x140)   
#define  rWdLoad       *(volatile unsigned *)(UART_BASE_ADDR + 0x144)   
#define  rWdRestart    *(volatile unsigned *)(UART_BASE_ADDR + 0x148)   
#define  rWdCR         *(volatile unsigned *)(UART_BASE_ADDR + 0x14c)   
#define  rWdStatus     *(volatile unsigned *)(UART_BASE_ADDR + 0x150)   
#define  rWdClear      *(volatile unsigned *)(UART_BASE_ADDR + 0x154)   
#define  rWdIntrlen    *(volatile unsigned *)(UART_BASE_ADDR + 0x158)   

#define  rUART0_CTL    *(volatile unsigned *)(UART_BASE_ADDR + 0x80)   
#define  rUART0_IER    *(volatile unsigned *)(UART_BASE_ADDR + 0x4) 

/*
***************************************************************************
*                          Miscellaneous
***************************************************************************
*/
#define WdDis()    rWdCR &= 0xfffffffe;
#define WdEn()     rWdCR |= 0x1;

#define WdRestart()    rWdRestart = WD_RESTART_KEY

#define WD_RESTART_KEY    0x5ab9

#define WD_RESET_ENABLE     1
#define WD_RESET_DISABLE    0

#define WD_SYS_INTR_ENABLE     1
#define WD_SYS_INTR_DISABLE    0

#define WD_EXTERNAL_SYGNAL_ENABLE     1
#define WD_EXTERNAL_SYGNAL_DISABLE    0

#define WD_PCLK        0      // watch dog timer clock source - PCLK
#define WD_EXTCLK      1      // watch dog timer clock source - EXTCLK



/*
***************************************************************************
*                              FUCTION PROTYPES
***************************************************************************
*/
int WdInit(int period_timer, int wd_rst, int wd_intr, int wd_ext, int wd_clk);
int WdPeriodSet(int second, int wd_clk);
int WdCRSet(int wd_rst, int wd_intr, int wd_ext, int wd_clk);


