/*
***************************************************************************
*                  Copyright (c) 2005 vBridge Microsystem, Inc.  
*                       Unpublished & Not for Publication
*                               All Rights Reserved                   
*   
* File        : watch_dog.c                     
*                                                                         
* Description : 
*             
* Date        : Mar 8, 2006                                                           
***************************************************************************
*/



#include "watch_dog.h"
#include "cctl.h"



/*
***************************************************************************
*                            WdInit
*
* Description: Initialize the watch dog registers.
*              Following steps are a summary of the programming sequence:
*                  1. Disable Watch Dog.
*                  2. Set WdLoad register.
*                  3. Write 0x5AB9 to WdRestart register.
*                  4. Set WdCR[Clock] bit.
*                  (5. Enable Watch Dog.)
*
* Arguments  : period_timer  is the period of Watch Dog timer reset. 
*                            Counted by seconds. 
*              wd_rst    Watch Dog timer System Reset signal.
*                          0: no effect
*                          1: Watch Dog timer system reset
*              wd_intr   Watch Dog timer System Interrupt signal.
*                          0: no effect
*                          1: Watch Dog timer system interrupt
*              wd_ext    Watch Dog timer External Interrupt signal.
*                          0: no effect
*                          1: Watch Dog timer external interrupt
*              wd_clk    Watch Dog Timer Clock Source
*                          0: PCLK
*                          1: EXTCLK
*
* Return     : none
*
* Note(s)    : 
***************************************************************************
*/
int WdInit(int period_timer, int wd_rst, int wd_intr, int wd_ext, int wd_clk)
{
    WdDis();                             // 1. Disable Watch Dog
    
    rWdClear = 1;       // Clear rWdStatus to 0
    
    WdPeriodSet(period_timer, wd_clk);   // 2. Set WdLoad register
    WdRestart();                         // 3. Write 0x5AB9 to WdRestart register
    WdCRSet(wd_rst, wd_intr, wd_ext, wd_clk);   // 4. Set WdCR[Clock] bit.
}

/*
***************************************************************************
*                            WdPeriodSet
*
* Description: Set the period of Watch Dog timer reset.
*
* Arguments  : second    is the period of Watch Dog timer reset. Counted by
*                        seconds. 
*              wd_clk    decide the watch dog timer clock source:
*                          0: PCLK
*                          1: EXTCLK
*  
* Return     : none
*
* Note(s)    : Assume the watch dog timer is driven by PCLK.
***************************************************************************
*/
int WdPeriodSet(int second, int wd_clk)
{
    if(wd_clk == WD_PCLK)       // watch dog timer clock source - PCLK
        rWdLoad = second * CPU_MAIN_CLOCK;
    else                        // watch dog timer clock source - EXTCLK
        rWdLoad = second * EXT_CLOCK;
}

/*
***************************************************************************
*                            WdCRSet
*
* Description: Set the rWdCR register.
*
* Arguments  : wd_rst    Watch Dog timer System Reset signal.
*                          0: no effect
*                          1: Watch Dog timer system reset
*              wd_intr   Watch Dog timer System Interrupt signal.
*                          0: no effect
*                          1: Watch Dog timer system interrupt
*              wd_ext    Watch Dog timer External Interrupt signal.
*                          0: no effect
*                          1: Watch Dog timer external interrupt
*              wd_clk    Watch Dog Timer Clock Source
*                          0: PCLK
*                          1: EXTCLK
* Return     : none
*
* Note(s)    : 
***************************************************************************
*/
int WdCRSet(int wd_rst, int wd_intr, int wd_ext, int wd_clk)
{
    rWdCR = (wd_rst<<1) | (wd_intr<<2) | (wd_ext<<3) | (wd_clk<<4);
}

