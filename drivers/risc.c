/*
***************************************************************************
*                  Copyright (c) 2005 vBridge Microsystem, Inc.
*                       Unpublished & Not for Publication
*                             All Rights Reserved
*
* File        : risc.c
*
* Description :
*
* Date        : Mar 10, 2006
***************************************************************************
*/



#include "risc.h"
#include "Sc.h"
#include "comm.h"

#include    "../port/os_cpu.h"

#include "2risc_memory_map.h"
#include "2risc_comm_protocol.h"
#include "two_risc_comm.h"
#include "Mc.h"
#include "../bsp/except.h"

#include "video_api.h"

#include "chip_cfg.h"
#include "video_cfg.h"

#include "../port/os_cpu.h"
#include "../uCOS Configure/os_cfg.h"
#include "../uCOS2.52/uCOS_II.h"

#include "../application/two_risc_system.h"
#include "2risc_memory_map.h"
#include "two_risc_display.h"
#ifdef TASK_STATISTIC_EN
#include "two_risc_statistics.h"
#endif

#ifdef AUDIO_MODULE_INCLUDE
#include "two_risc_audio.h"
#include "audio.h"
#include "audio_cfg.h"

#ifdef AUDIO_TEST_ENABLE
#include "test.h"
#endif
#endif

#include "timestamp.h"
void Risc1Entry(void);
void Risc1IntrEntry(void);

#include"../application/video/keypad_remote_control/remote_control.h"
#include"../application/video/keypad_remote_control/keypad.h"
#include"gpio.h"
#include"../application/video/techwell/hw2835_md.h"
#include"../application/video/alarm.h"

/*
***************************************************************************
*                         RiscRdCNT
*
* Description: 
*
* Arguments  : 
*
* Return     : none.
*
* Note(s)    : 
***************************************************************************
*/
unsigned int RiscRdCNT(int id)
{
    return(*(volatile unsigned *)(RISC_BASE_ADDR + 0x40 + 4 * id));
}

/*
***************************************************************************
*                         RiscIncCNT
*
* Description: 
*
* Arguments  : 
*
* Return     : none.
*
* Note(s)    : 
***************************************************************************
*/
void RiscIncCNT(int id, unsigned int value)
{
    unsigned int cnt;
    
    cnt = RiscRdCNT(id);
    if((cnt + value) < 256)
        *(volatile unsigned *)(RISC_BASE_ADDR + 0x40 + 4 * id) = (0x2<<8) | value;
    else
        printf("RiscIncCNT:cnt=%d,value=%d\n", cnt, value);
}

/*
***************************************************************************
*                         RiscDecCNT
*
* Description: 
*
* Arguments  : 
*
* Return     : none.
*
* Note(s)    : 
***************************************************************************
*/
void RiscDecCNT(int id, unsigned int value)
{
    unsigned int cnt;
    
    cnt = RiscRdCNT(id);
    if(cnt >= value)          
        *(volatile unsigned *)(RISC_BASE_ADDR + 0x40 + 4 * id) = (0x3<<8) | value;
    else
        printf("RiscDecCNT: cnt=%d,value=%d\n", cnt, value);
}

/*
***************************************************************************
*                         RiscClrCNT
*
* Description: 
*
* Arguments  : 
*
* Return     : none.
*
* Note(s)    : 
***************************************************************************
*/
void RiscClrCNT(int id)
{
    *(volatile unsigned *)(RISC_BASE_ADDR + 0x40 + 4 * id) = 0x1<<8;
}

/*
***************************************************************************
*                         RiscRdVCNT
*
* Description: rRISC_VCNT counts the voice samples. 
*              It¡¯s incremented by the audio frame sync pulse.
*
* Arguments  : 
*
* Return     : none.
*
* Note(s)    : 
***************************************************************************
*/
int RiscRdVCNT(void)
{
    return(rRISC_VCNT);
}

/*
***************************************************************************
*                         RiscClrVCNT
*
* Description: clear rRISC_VCNT.
*
* Arguments  : 
*
* Return     : none.
*
* Note(s)    : 
***************************************************************************
*/
int RiscClrVCNT(void)
{
    //rRISC_VCNT = (rRISC_VCNT & 0xfff) | (0x1<<14);
    rRISC_VCNT = 0x1<<14;
}

/*
***************************************************************************
*                         RiscIncVCNT
*
* Description: increment rRISC_VCNT.
*
* Arguments  : 
*
* Return     : none.
*
* Note(s)    : 
***************************************************************************
*/
int RiscIncVCNT(int value)
{
    //rRISC_VCNT = (rRISC_VCNT & 0xfff) | (0x2<<14);
    rRISC_VCNT = value | (0x2<<14);
}

/*
***************************************************************************
*                         RiscDecVCNT
*
* Description: decrement rRISC_VCNT.
*
* Arguments  : 
*
* Return     : none.
*
* Note(s)    : 
***************************************************************************
*/
int RiscDecVCNT(int value)
{
    //rRISC_VCNT = (rRISC_VCNT & 0xfff) | (0x3<<14);
    rRISC_VCNT = value | (0x3<<14);
}

/*
***************************************************************************
*                         RiscSetVCNT_VAL
*
* Description: rRISC_VCNT_VAL controls the interrupt period of the voice samples. 
*              The RISC_VCNT compares with this value. 
*              An interrupt is generated when the count exceeds this value. 
*              Software can reduce the counter to clear the interrupt.
*
* Arguments  : 
*
* Return     : none.
*
* Note(s)    : vint_val should not exceed 4096.
***************************************************************************
*/
void RiscSetVCNT_VAL(int vint_val)
{
    rRISC_VCNT_VAL = vint_val;
}

/*
***************************************************************************
*                                RiscRdID
*
* Description: read the risc ID
*
* Arguments  : NONE
*
* Return     : the risc ID:
*                  RISC0
*                  RISC1
*
* Note(s)    : 
***************************************************************************
*/
int RiscRdID(void)
{
    return(rRISC_COM_CTL & 0x1);	
}

/*
***************************************************************************
*                                RiscRdIntrStatus
*
* Description: read the interrupt status of each interrupt source.
*
* Arguments  : NONE
*
* Return     : the value of rRIST_INT_STATUS
*
* Note(s)    : 
***************************************************************************
*/
int RiscRdIntrStatus(void)
{
    return(rRISC_INT_STATUS);	
}

/*
***************************************************************************
*                                RiscIntrEn
*
* Description: enable the corresponding interrupt.
*
* Arguments  : intr_source    is the corresponding interrupt source
*
* Return     : SUCCESSFUL  shows the operation succeeded.
*              FAILED      shows the operation failed.
*
* Note(s)    : 
***************************************************************************
*/
int RiscIntrEn(unsigned int intr_source)
{    	
    unsigned int risc_ctl;
    	
    intr_source = intr_source << 12;
    	
    // read the risc ID 
    if(RiscRdID() == RISC0) {
        risc_ctl = rRISC0_CTL;
        risc_ctl |= intr_source;
        rRISC0_CTL = risc_ctl;
        if(rRISC0_CTL == risc_ctl)
            return SUCCESSFUL;
        return FAILED;
    } else {
        risc_ctl = rRISC1_CTL;
        risc_ctl |= intr_source;
        rRISC1_CTL = risc_ctl;
        if(rRISC1_CTL == risc_ctl)
            return SUCCESSFUL;
        return FAILED;    	
    }
}

/*
***************************************************************************
*                                RiscIntrDis
*
* Description: disable the corresponding interrupt.
*
* Arguments  : intr_source    is the corresponding interrupt source
*
* Return     : SUCCESSFUL  shows the operation succeeded.
*              FAILED      shows the operation failed.
*
* Note(s)    : 
***************************************************************************
*/
int RiscIntrDis(unsigned int intr_source)
{    	
    unsigned int risc_ctl;
    	
    intr_source = intr_source << 12;	
    	
    // read the risc ID 
    if(RiscRdID() == RISC0) {
        risc_ctl = rRISC0_CTL;
        risc_ctl &= (~intr_source);
        rRISC0_CTL = risc_ctl;
        if(rRISC0_CTL == risc_ctl)
            return SUCCESSFUL;
        return FAILED;
    } else {
        risc_ctl = rRISC1_CTL;
        risc_ctl &= (~intr_source);
        rRISC1_CTL = risc_ctl;
        if(rRISC1_CTL == risc_ctl)
            return SUCCESSFUL;
        return FAILED;    	
    }
}

/*
***************************************************************************
*                                RiscSemLock
*
* Description: lock the semaphore. 
*
* Arguments  : SemID    is the semaphore ID. Can be 0 ~ 7
*
* Return     : 
*
* Note(s)    : 
***************************************************************************
*/
void RiscSemLock(int SemID)
{
    int lock;	
	
    lock = *(volatile unsigned *)(RISC_BASE_ADDR + 0x20 + SemID * 4);
}

/*
***************************************************************************
*                                RiscSemPend
*
* Description: apply for the semaphore. Wait till it is available.
*
* Arguments  : SemID    is the semaphore ID. Can be 0 ~ 7
*
* Return     : 
*
* Note(s)    : 
***************************************************************************
*/
void RiscSemPend(int SemID)
{
    int lock;	
	
    do{
	lock = *(volatile unsigned *)(RISC_BASE_ADDR + 0x20 + SemID * 4);
    }while(lock);
}

/*
***************************************************************************
*                                RiscSemPend1
*
* Description: apply for the semaphore. Wait till it is available.
*
* Arguments  : SemID    is the semaphore ID. Can be 0 ~ 7                     
*
* Return     : 
*
* Note(s)    : RiscSemPend will wait forever, but RiscSemPend1 will wait for some time!
***************************************************************************
*/
int RiscSemPend1(int SemID)
{
    int lock;	
    unsigned int timeout = 0xffffffff;    // timeout     is the the longest time to wait!
	
    do {
	lock = *(volatile unsigned *)(RISC_BASE_ADDR + 0x20 + SemID * 4);
    } while(lock && timeout--);

    if(lock)
        return FAILED;
    else {      
        //printf("\nRiscSemPend1(%d) OK!\n", SemID);//by vincent 2008.01.18
        return SUCCESSFUL;
    }
}
//#ifdef DVR1
//#ifdef DVR
void RiscChgCnt(int SemID, int main_type, int sub_type, signed char direction)
{
    volatile unsigned char *pCount = (volatile unsigned char *)((COMM_COUNTBUF_B_ADDR | 0xa0000000) + (main_type -1) * 0x100 + (sub_type -1));
    unsigned char tmp;
    OS_CPU_SR cpu_sr;

    if(direction == 0)
        return;  
   // printf("\nRiscChgCnt: pCount = 0x%x! *pCount = 0x%x! \n", (unsigned int)pCount, *pCount);
    OS_ENTER_CRITICAL();
    
    if(RiscSemPend1(SemID) == SUCCESSFUL) {
        tmp = *pCount;
        if(direction < 0) {
            if((signed char)(tmp + direction) < 0) {
                printf("\nRiscChgCnt: cnt error(cnt =% d, direction = %d)\n", tmp, direction);
                RiscSemUnLock(SemID);
                OS_EXIT_CRITICAL();
                return;
            } else
                tmp += direction;
        } else {
            if(tmp + direction < tmp) {
                printf("\nRiscChgCnt: cnt error(cnt =% d, direction = %d)\n", tmp, direction);
                RiscSemUnLock(SemID);                
                OS_EXIT_CRITICAL();
                return;
            } else
                tmp += direction;
         }
         *pCount = tmp;         
         if(*pCount != tmp)
             printf("\nRiscChgCnt: cnt NOT change!\n");
         RiscSemUnLock(SemID);
    } else 
        printf("\nRiscChgCnt: RiscSemPend1 time out!\n");
   // printf("\nRiscChgCnt: *pCount = 0x%x!\n", *pCount);//by vincent 2008.01.18
    OS_EXIT_CRITICAL();
}
//#endif

/*
***************************************************************************
*                                RiscSemUnLock
*
* Description: post the semaphore.
*
* Arguments  : SemID    is the semaphore ID. Can be 0 ~ 7
*
* Return     : 
*
* Note(s)    : 
***************************************************************************
*/
void RiscSemUnLock(int SemID)
{
    *(volatile unsigned *)(RISC_BASE_ADDR + 0x20 + SemID * 4) = 1;
    //printf("RiscSemUnLock %d ok!", SemID);
}

/*
***************************************************************************
*                                RiscBootRisc1
*
* Description: Boot risc1
*
* Arguments  : 
*
* Return     : 
*
* Note(s)    : 
***************************************************************************
*/
void RiscBootRisc1(void)
{
    unsigned int *pTmp;	    	
    unsigned int i, tmp;	

#ifdef RISC1_UCOS
    // copy "jump" to risc1 start address   
    _PRINTF("\r\ncopy entry instrc to risc1 start address\r\n");	    
    pTmp = (unsigned int *)Risc1Entry;    
    for(i = 0; i < 10; i++) {
        *(unsigned int *)(0xa0000000 + i*4) = *pTmp++;    // no need flush cache because 0xa0000000 area is used
    }
#endif	
	
    _PRINTF("\r\nboot risc1...\r\n");

#ifdef DAISY_CHAIN
    _PRINTF("\r\nset daisy chain...\r\n");	    
    tmp = *(unsigned int *)0xa8010010;
    tmp |= 0x10;
    *(unsigned int *)0xa8010010 = tmp;
#endif
    
    _PRINTF("\r\nset sdram as risc1 boot source...\r\n");	
    *(unsigned int *)0xa8000004 = 0x4;
}

/*
***************************************************************************
*                                Risc1Copy180Entry
*
* Description: Boot risc1
*
* Arguments  : 
*
* Return     : 
*
* Note(s)    : 
***************************************************************************
*/
void Risc1Copy180Entry(void)
{
    unsigned int *pTmp, tmp;		

    tmp = VECTOR_EXC180 | 0xa0000000;    // use the physical address to
                                         // avoid flush cache!
                                         
    // copy interrupt entry routine to LINUX_B_ADDR, just for risc1 run ucos
    _PRINTF("\r\ncopy intr entry to risc1 LINUX_B_ADDR\r\n");	    
    pTmp = (unsigned int *)Risc1IntrEntry; 
    *(unsigned int *)(tmp) = *pTmp++;
    *(unsigned int *)(tmp + 4) = *pTmp++;
    *(unsigned int *)(tmp + 8) = *pTmp++;
    *(unsigned int *)(tmp + 12) = *pTmp;  
}    

/*
***************************************************************************
*                                Risc2CpuStart
*
* Description: Boot risc1
*
* Arguments  : 
*
* Return     : 
*
* Note(s)    : just for lx60 or later vb1100, vb2100
***************************************************************************
*/
void Risc2CpuStart(void)
{
    if(RiscRdID() == RISC0) {        
        RiscSemLock(SEM_LINUX_OK);    // lock the semaphore(risc1 will unlock it)
#ifdef UCOS_BOOT_RISC1
        RiscBootRisc1();
#endif
    } else
        Risc1Copy180Entry();   // copy interrupt entry routine to LINUX_B_ADDR, just for risc1 run ucos

    if(RiscRdID() == RISC1) {        
        RiscSemUnLock(SEM_LINUX_OK);
    } 
    
    if(RiscRdID() == RISC0) {        
    	RiscSemPend(SEM_LINUX_OK);    // wait risc1 unlock the semaphore	
    }            
}

/*
***************************************************************************
*                                Risc2CpuStartLx160
*
* Description: Boot risc1
*
* Arguments  : 
*
* Return     : 
*
* Note(s)    : just for lx160 or 2-cpu image without boot
***************************************************************************
*/
void Risc2CpuStartLx160(void)
{
    if(RiscRdID() == RISC0) {        
    	RiscSemLock(SEM_LINUX_OK);    // lock the semaphore(risc1 will unlock it)
        //RiscBootRisc1();
    } else
        Risc1Copy180Entry();   // copy interrupt entry routine to LINUX_B_ADDR, just for risc1 run ucos

    if(RiscRdID() == RISC1) {        
        RiscSemUnLock(SEM_LINUX_OK);
    } 
    
    if(RiscRdID() == RISC0) {        
    	RiscSemPend(SEM_LINUX_OK);    // wait risc1 unlock the semaphore	
    }            
}

/*
***************************************************************************
*                                RiscFlagSet
*
* Description: Set risc_flag to interrupt the other risc.
*
* Arguments  : 
*
* Return     : 
*
* Note(s)    : 
***************************************************************************
*/
void RiscFlagSet(void)
{    	
    if(RiscRdID() == RISC0) {
        //RiscSemPend(UCOS_TO_LINUX_SEMAPHORE);
        //if(rRISC1_FLAG == 0x0)         
            rRISC1_FLAG = 0x1;
        //RiscSemPost(UCOS_TO_LINUX_SEMAPHORE);
    } else {
        //RiscSemPend(LINUX_TO_UCOS_SEMAPHORE); 
        //if(rRISC0_FLAG == 0x0)     	
            rRISC0_FLAG = 0x1;       
        //RiscSemPost(LINUX_TO_UCOS_SEMAPHORE); 
    }         
}

/*
***************************************************************************
*                                RiscFlagClr
*
* Description: Clear risc_flag interrupt.
*
* Arguments  : 
*
* Return     : 
*
* Note(s)    : 
***************************************************************************
*/
void RiscFlagClr(void)
{
    if(RiscRdID() == RISC0) {
    	//RiscSemPend(UCOS_TO_LINUX_SEMAPHORE);
        rRISC0_FLAG = 0x1;
        //RiscSemPost(UCOS_TO_LINUX_SEMAPHORE);
    } else {
    	//RiscSemPend(LINUX_TO_UCOS_SEMAPHORE); 
        rRISC1_FLAG = 0x1;
        //RiscSemPost(LINUX_TO_UCOS_SEMAPHORE); 
    }         
}

/*
***************************************************************************
*                                RiscFlagIntrInit
*
* Description: 1. Enable risc_flag interrupt; 2. // Relocate risc flag ISR
*
* Arguments  : 
*
* Return     : 
*
* Note(s)    : 
***************************************************************************
*/
int RiscFlagIntrInit(void)
{
    unsigned int isr_base_addr;
    static int RiscFlagIntrInited = FALSE;	
    
    if(RiscFlagIntrInited == FALSE)
        RiscFlagIntrInited = TRUE;
    else
        return FAILED;
    	
    if(RiscRdID()  == RISC0)
        isr_base_addr = RISC0_THIRD_VECTOR_B_ADDR | 0xa0000000;    // phy-addr is used to avoid flush cache
    else
        isr_base_addr = RISC1_THIRD_VECTOR_B_ADDR | 0xa0000000;    // phy-addr is used to avoid flush cache
	
    MipsIntrReqEn(IP3);       
    *(unsigned int *)(isr_base_addr + 0x30) = (unsigned int)RiscFlagISR;    // Relocate risc flag ISR  
    return SUCCESSFUL;
}

/*
***************************************************************************
*                                RiscFlagIntrDis
*
* Description: Disable risc_flag interrupt.
*
* Arguments  : 
*
* Return     : 
*
* Note(s)    : The function is not ok.
***************************************************************************
*/
void RiscFlagIntrDis(void)
{
    MipsIntrReqDis(IP3);       
}

/*
***************************************************************************
*                            RiscIntrInit
*
* Description: Do the following:
*                  1. enable the Mips req interrupt(IP2)
*                  2. call RiscInitIntrHandlers
* Arguments  : 
*
* Return     : 
*
* Note(s)    : This function do not enable any block's interrupt.
***************************************************************************
*/
void  RiscIntrInit(void)
{
    static int RiscIntrInited = FALSE;

    if(RiscIntrInited == FALSE)
        RiscIntrInited = TRUE;
    else
        return;		
		
    MipsIntrReqEn(IP2);    // Enable MIPS hardware request interrupt IP2
    
    RiscInitIntrHandlers();    // Locate the interrupt service routine.
}

/*
***************************************************************************
*                            RiscIntrHandler
*
* Description: Risc interrupt service routine. Risc interrupt is connected
*              to Mips IP2.
*
* Arguments  : 
*
* Return     : 
*
* Note(s)    : 
***************************************************************************
*/
extern unsigned int g_excpt_id;
void  RiscIntrHandler(void)
{ 
    int i,j;	   
    unsigned int intr_status, intr_mask, isr_base_addr;
    
    intr_status = RiscRdIntrStatus();
    
    if(RiscRdID() == RISC0) {
        intr_mask = rRISC0_CTL;
        isr_base_addr = RISC0_FOURTH_VECTOR_B_ADDR;
    } else {
        intr_mask = rRISC1_CTL;         
        isr_base_addr = RISC1_FOURTH_VECTOR_B_ADDR;
    }
        
    intr_mask = (intr_mask >> 12);
    
    intr_status &= intr_mask;

    /* handle intrs one by one             
    for(i = 19; i >= 0 ; i--) {
        if(intr_status & (1 << i))	
            ((void(*)(void))(*(unsigned int *)(isr_base_addr + i * 16)))();
    }*/

    /* handle intrs by priority */             
    /* the first priority: sc intr */
    if(intr_status & (1 << 4)) {	
        ((void(*)(void))(*(unsigned int *)(isr_base_addr + 4 * 16)))();
        g_excpt_id = 0x0404;
        return;
    }

    /* the second priority: vcnt intr */
    if(intr_status & (1 << 0)) {	
        ((void(*)(void))(*(unsigned int *)(isr_base_addr + 0 * 16)))();
        g_excpt_id = 0x0400;
        return;
    }    

    /* handle the following intrs one by one */
    for(i = 19; i >= 0 ; i--) {
        if((i == 4) || (i == 0))
            continue;
        if(intr_status & (1 << i)) { 
            ((void(*)(void))(*(unsigned int *)(isr_base_addr + i * 16)))();
            g_excpt_id = 0x0400|i;
            return;
        }
    }    

    g_excpt_id = 0x04ff;    
    
}

/*
***************************************************************************
*                         RiscInitIntrHandlers
*
* Description:  
*
* Arguments  : none.
*
* Return     : none.
*
* Note(s)    : 
***************************************************************************
*/
void  RiscInitIntrHandlers (void)
{
    unsigned int isr_base_addr;
    unsigned int data;

    if(RiscRdID()  == RISC0)
        isr_base_addr = RISC0_THIRD_VECTOR_B_ADDR | 0xa0000000;    // phy-addr is used to avoid flush cache
    else
        isr_base_addr = RISC1_THIRD_VECTOR_B_ADDR | 0xa0000000;    // phy-addr is used to avoid flush cache
        	  
    *(unsigned int *)(isr_base_addr + 0x20) = (unsigned int)RiscIntrHandler;    // Relocate IP2 ISR
    
    // the fourth-interrupt table address	
    if(RiscRdID()  == RISC0)
        isr_base_addr = RISC0_FOURTH_VECTOR_B_ADDR | 0xa0000000;    // phy-addr is used to avoid flush cache
    else
        isr_base_addr = RISC1_FOURTH_VECTOR_B_ADDR | 0xa0000000;    // phy-addr is used to avoid flush cache
        	  
    *(unsigned int *)isr_base_addr          = (unsigned int)RiscPrintIntrMsg;       
    *(unsigned int *)(isr_base_addr + 0x10) = (unsigned int)RiscPrintIntrMsg; 
    *(unsigned int *)(isr_base_addr + 0x20) = (unsigned int)RiscPrintIntrMsg; 
    *(unsigned int *)(isr_base_addr + 0x30) = (unsigned int)RiscPrintIntrMsg; 
    *(unsigned int *)(isr_base_addr + 0x40) = (unsigned int)RiscPrintIntrMsg; 
    *(unsigned int *)(isr_base_addr + 0x50) = (unsigned int)RiscPrintIntrMsg; 
    *(unsigned int *)(isr_base_addr + 0x60) = (unsigned int)RiscPrintIntrMsg; 
    *(unsigned int *)(isr_base_addr + 0x70) = (unsigned int)RiscPrintIntrMsg; 
    *(unsigned int *)(isr_base_addr + 0x80) = (unsigned int)RiscPrintIntrMsg; 
    *(unsigned int *)(isr_base_addr + 0x90) = (unsigned int)RiscPrintIntrMsg; 
    *(unsigned int *)(isr_base_addr + 0xa0) = (unsigned int)RiscPrintIntrMsg; 
    *(unsigned int *)(isr_base_addr + 0xb0) = (unsigned int)RiscPrintIntrMsg; 
    *(unsigned int *)(isr_base_addr + 0xc0) = (unsigned int)RiscPrintIntrMsg; 
    *(unsigned int *)(isr_base_addr + 0xd0) = (unsigned int)RiscPrintIntrMsg; 
    *(unsigned int *)(isr_base_addr + 0xe0) = (unsigned int)RiscPrintIntrMsg; 
    *(unsigned int *)(isr_base_addr + 0xf0) = (unsigned int)RiscPrintIntrMsg; 
    *(unsigned int *)(isr_base_addr + 0x100)= (unsigned int)RiscPrintIntrMsg; 
    *(unsigned int *)(isr_base_addr + 0x110)= (unsigned int)RiscPrintIntrMsg; 
    *(unsigned int *)(isr_base_addr + 0x120)= (unsigned int)RiscPrintIntrMsg; 
    *(unsigned int *)(isr_base_addr + 0x130)= (unsigned int)RiscPrintIntrMsg; 
}

/*
***************************************************************************
*                         RiscPrintIntrMsg
*
* Description: 
*
* Arguments  : 
*
* Return     : none.
*
* Note(s)    : 
***************************************************************************
*/
void  RiscPrintIntrMsg(void)
{
    _PRINTF("Risc Interrupt Occurs!");
    while(1);
}

/*
***************************************************************************
*                            RiscFlagISR
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
void  RiscFlagISR(void)
{   
    unsigned char err;
    
    RiscFlagClr();

    err = OSQPost(gptQMsgTwoRiscSystemMonitor, (void *)TWO_RISC_SYSTEM_RECEIVE_LINUX_MSG);                            
    if(err != OS_NO_ERR)            
        debug("\nRiscFlagISR: OSQPost error! err = %d!\n", err); 
    gtStatistics.flagISR_rate++;
    return;  

    //CommInqSendChar(UART_CHANNEL, '#');
    while(1) {
        if(TwoRiscCommRecPacket() != SUCCESSFUL) {
            ;//debug("RiscFlagISR: failed!\r\n");
            break;                
        } else
            ;//debug("RiscFlagISR: ok!\r\n");
    }        		        
}
//#ifdef DVR
/*
***************************************************************************
*                            RiscGPIOISR
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
void  RiscGPIOISR(void)
{   
    unsigned int i;

    /* Clear the intr source */
#ifdef DVR    
    if(rRISC0_GPIO_INTR_MASK&(0x1<<GPIO_REMOTE_CONTROL_BIT)){
        set_gpio_in(GPIO_REMOTE_CONTROL_BIT);
        if(rRISC_GPIO_POL&(0x1<<GPIO_REMOTE_CONTROL_BIT)) {
            if(((rRISC_GPIO>>GPIO_REMOTE_CONTROL_BIT)&0x1)==0){
                RemoteControlGpioISR();
            }
        }
        else if((rRISC_GPIO>>GPIO_REMOTE_CONTROL_BIT)&0x1){
                RemoteControlGpioISR();
        }
    } 
#endif

#ifdef VIDEO_MODULE_INCLUDE 
    for(i=0;i<32;i++){
        if((i<5 || i==12 ||i==13 || i==23 ||i==26)==0){
            if(alarm_gpio_status&(0x1<<i)){
                alarm_gpio_isr_no = i;
                if(rRISC0_GPIO_INTR_MASK&(0x1<<alarm_gpio_isr_no)){
                    set_gpio_in(alarm_gpio_isr_no);
                    if(rRISC_GPIO_POL&(0x1<<(alarm_gpio_isr_no))) {
                        if(((rRISC_GPIO>>alarm_gpio_isr_no)&0x1)==0){
                            if(alarm_type[alarm_gpio_isr_no]==0){ 
                                AlarmLevelTriggeredGpioISR();
                                break;
                            } else if(alarm_type[alarm_gpio_isr_no]==1){
                                AlarmEdgeTriggeredGpioISR();
                                break;
                            } else{ 
                                AlarmPulseCounterGpioISR();
                                break;
                            }                       
                        }
                    } else if((rRISC_GPIO>>alarm_gpio_isr_no)&0x1){
                        if(alarm_type[alarm_gpio_isr_no]==0){
                            AlarmLevelTriggeredGpioISR();
                            break;
                        } else if(alarm_type[alarm_gpio_isr_no]==1){ 
                            AlarmEdgeTriggeredGpioISR();
                            break;
                        } else{
                            AlarmPulseCounterGpioISR();
                            break;
                        }
                    }
                }
            }
        } else 
#ifdef DVR        
            {;}//debug("\nThe gpio no is used!\n");   //by vincent 2008.01.16  
#else
            debug("\nThe gpio no is used!\n");
#endif
    }
#endif
    
#ifdef  DVR
    if(rRISC0_GPIO_INTR_MASK&(0x1f<<GPIO_KB0_BIT)){
        set_n_gpio_in(GPIO_KB_NUM, GPIO_KB0_BIT);
        if(rRISC_GPIO_POL&(0x1f<<GPIO_KB0_BIT)) {
            if(((rRISC_GPIO>>GPIO_KB0_BIT)&0x1)==0){
                KeypadGpioISR();
            } else if(((rRISC_GPIO>>GPIO_KB0_BIT)&0x2)==0){
                KeypadGpioISR();
            } else if(((rRISC_GPIO>>GPIO_KB0_BIT)&0x4)==0){
                KeypadGpioISR();
            } else if(((rRISC_GPIO>>GPIO_KB0_BIT)&0x8)==0){
                KeypadGpioISR();
            } else if(((rRISC_GPIO>>GPIO_KB0_BIT)&0x10)==0){
                KeypadGpioISR();
            } 
        }
    }

    if(rRISC0_GPIO_INTR_MASK&(0x1<<GPIO_MOYION_DETECTOR_BIT)){
        set_gpio_in(GPIO_MOYION_DETECTOR_BIT);
        if((rRISC_GPIO_POL&(0x1<<GPIO_MOYION_DETECTOR_BIT))==0){
            if(((rRISC_GPIO>>GPIO_MOYION_DETECTOR_BIT)&0x1)==1){
                HW2835MotionDetectorGpioISR();
                }
            }
    }
#endif
}


/*
***************************************************************************
*                                RiscGPIOIntrInit
*
* Description: 1. Enable gpio interrupt; 2. Relocate risc gpio ISR
*
* Arguments  : 
*
* Return     : 
*
* Note(s)    : 
***************************************************************************
*/

int RiscGPIOIntrInit(void)
{
    static int flag_gpio_intr_inited = 0;;
    int isr_base_addr;

    if(flag_gpio_intr_inited)
        return 0;

    flag_gpio_intr_inited = 1;

	/* locate gpio ISR */    	
    if(RiscRdID()  == RISC0)
        isr_base_addr = RISC0_THIRD_VECTOR_B_ADDR | 0xa0000000;    // phy-addr is used to avoid flush cache
    else
        isr_base_addr = RISC1_THIRD_VECTOR_B_ADDR | 0xa0000000;    // phy-addr is used to avoid flush cache
	
	*(unsigned int *)(isr_base_addr + 0x50) = (unsigned int)RiscGPIOISR;    // locate gpio ISR 

    MipsIntrReqEn(IP5);  
     
    return SUCCESSFUL;
}
//#endif

#ifdef DISPLAY_INCLUDE
/*
***************************************************************************
*                            RiscTimerISR
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
void  RiscTimerISR(void)
{
    unsigned int ret;
    OS_SEM_DATA data;
    if(RiscRdID()  == RISC0) {
        if((rRISC_COM_CTL& 0x300) == 0x200) {    // risc1 timer intr risc0?
            printf("\nRiscTimerISR: risc1 timer intr risc0!\n");
            rRISC_COM_CTL = rRISC_COM_CTL & ~0x200;
            rRISC1_TIMER &= ~(0x1<<31);     // clear the intr 
        } 
        else if((rRISC_COM_CTL & 0x300) == 0x100) {    // risc0 timer intr risc0?        
            if(rRISC0_TIMER | (0x1<<31)) {
                rRISC0_TIMER &= ~(0x1<<31);     // clear the intr
                
#ifdef DISPLAY_INCLUDE
                gRiscTimerCnt += 5;
                if(gRiscTimerCnt >= gtavsync.decodeinterval){
                    OSSemQuery(gptRiscTimerIntrOccur, &data);
                    if(data.OSCnt == 0) {
                        ret = OSSemPost(gptRiscTimerIntrOccur); 
                        if(ret != OS_NO_ERR)
                            printf("ucos post sem gptRiscTimerOccur err!\n");
                    }
                }
                rRISC0_TIMER = ((CPU_MAIN_CLOCK / 1000000)  * 5000 /*time_interval*/ | (0x1<<31));
#endif
                
            } else
                printf("\nRiscTimerISR: risc0 timer intr risc0, but risc0 timer is disabled!\n");            
        }
    } else {    // to do
        //rRISC1_TIMER = (CPU_MAIN_CLOCK / 1000000) * time_interval | (0x1<<31);
        //rRISC_COM_CTL = (rRISC_COM_CTL & ~0xc00) | 0x800;    // risc1 timer1 intr risc1
    }
}

/*
***************************************************************************
*                            RiscTimerInit
*
* Description: 
*
* Arguments  : time_interval    is the intr interval. Counted by micro-second
*
* Return     : 
*
* Note(s)    : 
***************************************************************************
*/
int  RiscTimerInit(unsigned int time_interval)
{
    unsigned int isr_base_addr;
    static int RiscTimerIntrInited = FALSE;	
    
    if(RiscTimerIntrInited == FALSE)
        RiscTimerIntrInited = TRUE;
    else
        return FAILED;
    	
    if(RiscRdID()  == RISC0)
        isr_base_addr = RISC0_THIRD_VECTOR_B_ADDR | 0xa0000000;    // phy-addr is used to avoid flush cache
    else
        isr_base_addr = RISC1_THIRD_VECTOR_B_ADDR | 0xa0000000;    // phy-addr is used to avoid flush cache
	
    MipsIntrReqEn(IP4);       
    *(unsigned int *)(isr_base_addr + 0x40) = (unsigned int)RiscTimerISR;    // Relocate risc flag ISR  

    if(RiscRdID()  == RISC0) {
        rRISC0_TIMER &= ~(0x1<<31);     // clear the intr        
        rRISC_COM_CTL = (rRISC_COM_CTL & ~0x300) | 0x100;    // risc0 timer intr risc0
        rRISC0_TIMER = (CPU_MAIN_CLOCK / 1000000) * time_interval | (0x1<<31);
    } else {
        rRISC1_TIMER &= ~(0x1<<31);     // clear the intr            
        rRISC1_TIMER = (CPU_MAIN_CLOCK / 1000000) * time_interval | (0x1<<31);
        rRISC_COM_CTL = (rRISC_COM_CTL & ~0xc00) | 0x800;    // risc1 timer1 intr risc1
    }
    
    return SUCCESSFUL;
}
#endif

/*
***************************************************************************
*                            RiscVCNTIntrEn
*
* Description: enable the VCNT Intr
*
* Arguments  : 
*
* Return     : 
*
* Note(s)    : 
***************************************************************************
*/
void  RiscVCNTIntrEn(int vcnt_val)
{ 
    unsigned int isr_base_addr;
    
    ResetSysTimeStamp();
    
    // the fourth-interrupt table address	
    if(RiscRdID()  == RISC0)
        isr_base_addr = RISC0_FOURTH_VECTOR_B_ADDR | 0xa0000000;    // phy-addr is used to avoid flush cache
    else
        isr_base_addr = RISC1_FOURTH_VECTOR_B_ADDR | 0xa0000000;    // phy-addr is used to avoid flush cache     
        
    *(unsigned int *)(isr_base_addr + 0x0) = (unsigned int)RiscVCNT_ISR; 
    
    RiscSetVCNT_VAL(vcnt_val);    // set sample threshhold
    
    RiscClrVCNT();     // clear VCNT
    
    RiscIntrEn(VCNT_INTR);
}

#ifdef AUDIO_MODULE_INCLUDE
/*
***************************************************************************
*                            RiscVCNTIntrDis
*
* Description: disable the VCNT Intr
*
* Arguments  : 
*
* Return     : 
*
* Note(s)    : 
***************************************************************************
*/
void  RiscVCNTIntrDis()
{     
    RiscIntrDis(VCNT_INTR);
}

/*
***************************************************************************
*                            HandleHistoryPointer
*
* Description: Handle All channel's History Pointer
*
* Arguments  : 
*
* Return     : 
*
* Note(s)    : 
***************************************************************************
*/

void HandleHistoryPointer(void)
{
    int i,j;
    unsigned int tmp, difference;	
    unsigned int queue_start_addr;
    unsigned int queue_end_addr;
    unsigned int history_pointer;
    unsigned int queue_in_m_addr;
    extern tAudioCharacter channelAC[MAX_AUDIO_CHANNEL];


    for (i = 0; i < MAX_AUDIO_CHANNEL; i++)
    {
	if (!channelAC[i].bChannelEnable)
	    continue;

	switch (i)
	{
	    case 0: 
		queue_start_addr = AUDIO_OUT_CH0_M_ADDR | 0xa0000000;
		queue_in_m_addr = AUDIO_IN_CH0_M_ADDR | 0xa0000000;
		break;
	    case 1: queue_start_addr = AUDIO_OUT_CH1_M_ADDR | 0xa0000000;
		    queue_in_m_addr = AUDIO_IN_CH1_M_ADDR | 0xa0000000;
		    break;
	    case 2: queue_start_addr = AUDIO_OUT_CH2_M_ADDR | 0xa0000000;
		    queue_in_m_addr = AUDIO_IN_CH2_M_ADDR | 0xa0000000;
		    break;
	    case 3: queue_start_addr = AUDIO_OUT_CH3_M_ADDR | 0xa0000000;
		    queue_in_m_addr = AUDIO_IN_CH3_M_ADDR | 0xa0000000;
		    break;
	    default:
		    queue_start_addr = AUDIO_OUT_CH0_M_ADDR | 0xa0000000;
		    queue_in_m_addr = AUDIO_IN_CH0_M_ADDR | 0xa0000000;
		    break;
	}


	if(channel_history_pointer_initialized[i] == 0) {
	    debug("|");
	    queue_end_addr = queue_start_addr + McRdQueueActualBufsize(i + 4 /*queue_out_id*/) - 4;

	    if(channel_history_pointer_recount[i] == 1) 
	    {   // this method is not accurate.  But if underflow & overflow occired, this method should be used!   
		tmp = McRdQueueReadAddr(i + 4 /*queue_out_id*/) | 0xa0000000;
		debug("%x, %x.", tmp, (McRdQueueReadAddr(i /* queue_id */) | 0xa0000000));
		for(j = 0; j < (time_stamp[i] + his_len[i] + 0x20); j +=4) {    // 0x20 is the size of audio output stream
		    tmp -= 4;
		    if(tmp < queue_start_addr)
			tmp = queue_end_addr;
		}
		channel_history_pointer_recount[i] = 0;
	    } else {    // this method is accurat6e. history pointer counted from 0
		tmp = queue_start_addr;
		for(j = 0; j < time_stamp[i]; j +=4) {
		    tmp -= 4;
		    if(tmp < queue_start_addr)
			tmp = queue_end_addr;
		}
	    }

	    channel_history_pointer_initialized[i] = 1;

	    channel_history_pointer[i] =  tmp;
	}

	// printf history-pointer & the differences of audio-out pointer
	history_pointer = channel_history_pointer[i];
	tmp = (McRdQueueReadAddr(i /* queue_id */) | 0xa0000000)/* - 0x20 */;    // 0x20 is the size of audio autofetch stream

	if(tmp <  queue_in_m_addr)
	    tmp =  tmp + 0x2000;
	tmp = tmp - queue_in_m_addr;
	history_pointer -= queue_start_addr;
	if(tmp >=  history_pointer)
	    difference = tmp - history_pointer;
	else
	    difference = 0x2000 + tmp - history_pointer;

	if(OSSemQuery(gptAudioTaskReady, &gtAudioTaskReady) == OS_NO_ERR) {
	    if(gtAudioTaskReady.OSCnt != 0)
		printf("@%d", gtAudioTaskReady.OSCnt);
	} else
	    printf("@*");
	if(difference !=  time_stamp[i])
	{
#ifdef AUDIO_TEST_ENABLE
	    sys_info.history_difference = difference;
#else	    
	    //debug("#%x ", difference);             
#endif
	}
	    
    }
}


/*
***************************************************************************
*                            RiscVCNT_ISR
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
void  _RiscVCNT_ISR(void)
{
    int i;
    
    extern LONG glwInterruptTimeScale; //for 10ms, 1; for 20ms, 2; for 30ms, 3
    extern LONG glwCount_RiscVCNT_ISR_times;

    IncSysTimeStamp();

    extern tAudioCharacter channelAC[MAX_AUDIO_CHANNEL];

    int k = RiscRdCNT(AUDIO_LINUX_TO_UCOS_CNT);
    if (k > 1)
	debug("##%d\n",k);

    //test1/////////////
    //debug("***%d\n",RiscRdVCNT());
    ////////////////////

    //check if all channel is not enabled
    for (i = 0; i < MAX_AUDIO_CHANNEL; i++)
    {
	if (channelAC[i].bChannelEnable)
	    break;
    }
	
    if (i < MAX_AUDIO_CHANNEL)
    {

	if (glwCount_RiscVCNT_ISR_times == (glwInterruptTimeScale -1 )) //(glwInterruptTimeScale -1 ) = 2 , 30ms; (glwInterruptTimeScale -1) =1, 20ms; (glwInterruptTimeScale -1) = 0; 10ms 
	{
	    if(OSSemPost(gptAudioTaskReady) != OS_NO_ERR)
	    {
		debug("\r\nRiscVCNT_ISR: OSSemPost error!\r\n");
	    }
	    glwCount_RiscVCNT_ISR_times = 0;
	}
	else
	{
	    glwCount_RiscVCNT_ISR_times++;
	}
    }

    RiscDecVCNT(AUDIO_MAX_FRAME_LEN);	
	
    HandleHistoryPointer(); 
}

#endif

void  RiscVCNT_ISR(void)
{
    int i;
    
#ifdef AUDIO_MODULE_INCLUDE
    i = AppAudioGetStatus();
    if(i == AUDIO_OPENED){
        _RiscVCNT_ISR();
    }else
#endif
    {
        IncSysTimeStamp();
        RiscDecVCNT(AUDIO_MAX_FRAME_LEN);
    }
}

