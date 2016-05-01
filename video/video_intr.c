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



#define VIDEO_INTR_GLOBALS

#include "video_intr.h"
#include "def_type.h"
#include "risc.h"
#include "2risc_memory_map.h"
#include "comm.h"

#include "../port/os_cpu.h"
#include "../uCOS Configure/os_cfg.h"
#include "../uCOS2.52/uCOS_II.h"

#include "../application/two_risc_system.h"

#include "../video/VIN_API.h"

#include "video_cfg.h"

/*
***************************************************************************
*                            VinIntrEn
*
* Description: 
*
* Arguments  : 
*
* Return     : 
*
* Note(s)    : 
***************************************************************************
*/
void  VinIntrEn(void)
{
    unsigned int isr_base_addr;

    // the fourth-interrupt table address	
    if(RiscRdID()  == RISC0)
        isr_base_addr = RISC0_FOURTH_VECTOR_B_ADDR | 0xa0000000;    // phy-addr is used to avoid flush cache
    else
        isr_base_addr = RISC1_FOURTH_VECTOR_B_ADDR | 0xa0000000;    // phy-addr is used to avoid flush cache      
        
    *(unsigned int *)(isr_base_addr + 0x50) = (unsigned int)VinISR; 
    
    RiscIntrEn(VIN_INTR);
}

/*
***************************************************************************
*                            VinISR
*
* Description: 
*
* Arguments: 
*
* Return     : 
*
* Note(s)    : 
***************************************************************************
*/
void  VinISR(void)
{
    extern OS_EVENT *gptQMsgTwoRiscSystemMonitor;
    DIS_VIN_VIDEO();
    *(volatile unsigned *)(0xa8050008)|= 0x80000000;    // Clear Vin interrupt
    debug("\r\n@: VinISR\r\n");
	
    *(volatile int *)0xa8010038 = 0x20;    // reset vin	
    *(volatile int *)0xa8010038 = 0x0;    // reset vin

    gtStatistics.cnt_vin_intr++;

    OSQPost(gptQMsgTwoRiscSystemMonitor, (void *)TWO_RISC_SYSTEM_REOPEN_VIN_TASK);
    debug("#0xa8040020 = 0x%x!#", *(unsigned int *)0xa8040020);    // check sc status!
}

#ifdef DISPLAY_INCLUDE
/*
***************************************************************************
*                            VoutIntrEn
*
* Description: 
*
* Arguments: 
*
* Return     : 
*
* Note(s)    : 
***************************************************************************
*/
void VoutIntrEn(void)
{
    unsigned int isr_base_addr;

    // the fourth-interrupt table address	
    if(RiscRdID()  == RISC0)
        isr_base_addr = RISC0_FOURTH_VECTOR_B_ADDR | 0xa0000000;    // phy-addr is used to avoid flush cache
    else
        isr_base_addr = RISC1_FOURTH_VECTOR_B_ADDR | 0xa0000000;    // phy-addr is used to avoid flush cache      
        
    *(unsigned int *)(isr_base_addr + 0x60) = (unsigned int)VoutISR; 
    
    RiscIntrEn(VOUT_INTR);    
}

/*
***************************************************************************
*                            VoutISR
*
* Description: 
*
* Arguments: 
*
* Return     : 
*
* Note(s)    : 
***************************************************************************
*/

extern  unsigned int gVoutEnable;

void  VoutISR(void)
{    
    debug("\r\nVoutISR\r\n");

    gtStatistics.cnt_vout_intr++;

    /* clear intr */
    rRISC_INT_STATUS = (0x1<<6);	
    *(volatile unsigned int *)0xa8060010 |= 0x80000000;

    gVoutEnable = 0;
    *(volatile int *)(0xa80b0000) = (*(int *)(0xa80b0000)) & 0x7fffffff;    // disable vsc  
    *(volatile int *)(0xa8060000) = (*(int *)(0xa8060000)) & 0x7fffffff;    // disable vout

    *(volatile int *)0xa8010038 = 0x840;    // reset vsc & vout
    *(volatile int *)0xa8010038 = 0x000;    // no reset vsc & vout	

    OSQPost(gptQMsgTwoRiscSystemMonitor, (void *)TWO_RISC_SYSTEM_OPEN_VOUT);
	
}
#endif
