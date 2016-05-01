/*
***************************************************************************
*                 Copyright (c) 2005 vBridge Microsystem, Inc.  
*                       Unpublished & Not for Publication
*                              All Rights Reserved                   
*   
* File:         bsp.c             
*                                                                         
* Description :       
*                                                
* By:           Gong Bin               
*
* Date:         Sep 20, 2005                                                           
***************************************************************************
*/



#define    BSP_GLOBALS
#include "includes.h"
#include "2risc_memory_map.h"

#include "chip_cfg.h"
#include "../version.h"

#ifdef VIDEO_MODULE_INCLUDE
#include "VIN_API.h"
#include "video_cfg.h"
//#include "../video/Camera.h"
#include "two_risc_video.h"
//pCamera pcurrent_camera;
#endif
#include "two_risc_system.h"

#ifdef AUDIO_MODULE_INCLUDE
#include "audio_cfg.h"
#include "../application/audio/two_risc_audio.h"
#endif

#include "two_risc_statistics.h"

#include "printf.h"

#include "comm.h"

#include "../port/os_cpu.h"

#include "exc_code.h"

#include "ucos_task_priority.h"

/*
***************************************************************************
*                         BspPrintfCp0Reg
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
void  BspPrintfCp0Reg(int to_use_print_task)
{
    unsigned int ErrorEPC_addr, ErrorEPC_instr, ErrorEPC_instr_slot;
    unsigned int EPC_addr, EPC_instr, EPC_instr_slot;

    ErrorEPC_addr = GetC0_ErrorEPC();
    EPC_addr = GetC0_EPC();
    
    ErrorEPC_instr = *(unsigned int *)(ErrorEPC_addr & 0xffffffc | 0xa0000000);
    ErrorEPC_instr_slot = *(unsigned int *)((ErrorEPC_addr & 0xffffffc) + 4 | 0xa0000000);
    
    EPC_instr = *(unsigned int *)(EPC_addr & 0xffffffc | 0xa0000000);
    EPC_instr_slot = *(unsigned int *)((EPC_addr & 0xffffffc) + 4 | 0xa0000000);


    if(to_use_print_task) {
        printf("\nRisc%d: Status = 0x%x, Cause = 0x%x, C0_BadVaddr = 0x%x, C0_Context = 0x%x, C0_EntryHi = 0x%x.\n", \
                    RiscRdID(), CP0_wGetSR(), GetCause(), GetC0_BadVaddr(), GetC0_Context(), GetC0_EntryHi());  
        printf("C0_ErrorEPC = 0x%x(0x%x,0x%x)\n", ErrorEPC_addr, ErrorEPC_instr, ErrorEPC_instr_slot);
        printf("C0_EPC = 0x%x(0x%x,0x%x)\n", EPC_addr, EPC_instr, EPC_instr_slot);
    } else {
        _PRINTF("\nRisc%d: Status = 0x%x, Cause = 0x%x, C0_BadVaddr = 0x%x, C0_Context = 0x%x, C0_EntryHi = 0x%x.\n", \
                    RiscRdID(), CP0_wGetSR(), GetCause(), GetC0_BadVaddr(), GetC0_Context(), GetC0_EntryHi());  
        _PRINTF("C0_ErrorEPC = 0x%x(0x%x,0x%x)\n", ErrorEPC_addr, ErrorEPC_instr, ErrorEPC_instr_slot);
        _PRINTF("C0_EPC = 0x%x(0x%x,0x%x)\n", EPC_addr, EPC_instr, EPC_instr_slot);
    }
}

/*
***************************************************************************
*                         uCOS_OTHER_INTHandler
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
void  uCOS_OTHER_INTHandler(void)
{	
    _PRINTF("\r\nExceptions except 0x180 occur!\r\n");

    PrintAllInUartBuf();

    while(1);  
}

/*
***************************************************************************
*                         uCOS_OTHER_INTHandler_000
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
void  uCOS_OTHER_INTHandler_000(void)
{
    BspPrintfCp0Reg(0);	
    _PRINTF("\r\nExceptions at 0x0 occur!\r\n");

    PrintAllInUartBuf();

    while(1);  
}

/*
***************************************************************************
*                         uCOS_OTHER_INTHandler_080
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
void  uCOS_OTHER_INTHandler_080(void)
{
    BspPrintfCp0Reg(0);	
    _PRINTF("\r\nExceptions at 0x80 occur!\r\n");

    PrintAllInUartBuf();

    while(1);  
}

/*
***************************************************************************
*                         uCOS_OTHER_INTHandler_100
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
void  uCOS_OTHER_INTHandler_100(void)
{
    BspPrintfCp0Reg(0);	
    _PRINTF("\r\nExceptions at 0x100 occur!\r\n");

    PrintAllInUartBuf();

    while(1);  
}

/*
***************************************************************************
*                         uCOS_OTHER_INTHandler_200
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
void  uCOS_OTHER_INTHandler_200(void)
{
    BspPrintfCp0Reg(0);	
    _PRINTF("\r\nExceptions at 0x200 occur!\r\n");

    PrintAllInUartBuf();

    while(1);  
}

/*
***************************************************************************
*                         uCOS_OTHER_INTHandler_280
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
void  uCOS_OTHER_INTHandler_280(void)
{
    BspPrintfCp0Reg(0);	
    _PRINTF("\r\nExceptions at 0x280 occur!\r\n");

    PrintAllInUartBuf();

    while(1);  
}

/*
***************************************************************************
*                         uCOS_OTHER_INTHandler_300
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
void  uCOS_OTHER_INTHandler_300(void)
{
    BspPrintfCp0Reg(0);	
    _PRINTF("\r\nExceptions at 0x300 occur!\r\n");

    PrintAllInUartBuf();

    while(1);  
}

/*
***************************************************************************
*                         uCOS_OTHER_INTHandler_380
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
void  uCOS_OTHER_INTHandler_380(void)
{
    BspPrintfCp0Reg(0);	
    _PRINTF("\r\nExceptions at 0x380 occur!\r\n");

    PrintAllInUartBuf();

    while(1);  
}

/*
***************************************************************************
*                         uCOS_OTHER_INTHandler_400
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
void  uCOS_OTHER_INTHandler_400(void)
{
    BspPrintfCp0Reg(0);	
    _PRINTF("\r\nExceptions at 0x400 occur!\r\n");

    PrintAllInUartBuf();

    while(1);  
}

/*
***************************************************************************
*                         BspPrintIntrMsgIP0
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
void  BspPrintIntrMsgIP0(void)
{
    _PRINTF("Software Interrupt 0 Occurs!");

    PrintAllInUartBuf();

    while(1);  
}

/*
***************************************************************************
*                         BspPrintIntrMsgIP1
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
void  BspPrintIntrMsgIP1(void)
{
    _PRINTF("Software Interrupt 1 Occurs!");

    PrintAllInUartBuf();

    while(1);  
}

/*
***************************************************************************
*                         BspPrintIntrMsgIP2
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
void  BspPrintIntrMsgIP2(void)
{
    /*	
    BspPrintfCp0Reg();
    _PRINTF("\r\nrRISC_INT_STATUS = 0x%x\r\n", rRISC_INT_STATUS);
    _PRINTF("\r\nrAUD_STAT = 0x%x\r\n", rAUD_STAT);
    _PRINTF("\r\nrMC_INTR_CTL = 0x%x\r\n", rMC_INTR_CTL);	
    */
    
    _PRINTF("Hardware Interrupt 0 Occurs!");

    PrintAllInUartBuf();

    while(1);  
}

/*
***************************************************************************
*                         BspPrintIntrMsgIP3
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
void  BspPrintIntrMsgIP3(void)
{
    _PRINTF("Hardware Interrupt 1 Occurs!");

    PrintAllInUartBuf();

    while(1);  
}

/*
***************************************************************************
*                         BspPrintIntrMsgIP4
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
void  BspPrintIntrMsgIP4(void)
{
    _PRINTF("Hardware Interrupt 2 Occurs!");

    PrintAllInUartBuf();

    while(1);  
}

/*
***************************************************************************
*                         BspPrintIntrMsgIP5
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
void  BspPrintIntrMsgIP5(void)
{
    _PRINTF("Hardware Interrupt 3 Occurs!");

    PrintAllInUartBuf();

    while(1);  
}

/*
***************************************************************************
*                         BspPrintIntrMsgIP6
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
void  BspPrintIntrMsgIP6(void)
{
    _PRINTF("Hardware Interrupt 4 Occurs!");

    PrintAllInUartBuf();

    while(1);  
}

/*
***************************************************************************
*                         BspPrintIntrMsgIP7
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
void  BspPrintIntrMsgIP7(void)
{
    _PRINTF("Hardware Interrupt 5 Occurs!");

    PrintAllInUartBuf();

    //while(1);  
}

/*
*********************************************************************************************
*                                       BspChangePllValue
*
* Description: This routine sets the CPU clock.
*
* Arguments  : none.
*
* Return     : none.
*
* Note(s)    : 
*********************************************************************************************
*/
void  BspChangePllValue ()
{ 

}

void delay(int m, int n)
{
    volatile int i, j;
    
    for(i = 0; i<= m; i++) {
        for (j = 0; j<= n; j++) {       	
        }
    }	
}

void  BspTimerISR (void)
{
    ExceptClrCount();  
    OSTimeTick();	
}

/*
***************************************************************************
*                            BspStartTicker
*
* Description: 
*
* Arguments  : ticks_per_sec - Time ticks per second.
*
* Return     : none.
*
* Note(s)    : 
***************************************************************************
*/
void  BspStartTicker (INT32U ticks_per_sec)
{
    unsigned int isr_base_addr;
    unsigned int compare;      // compare is the value to be writen into the CP0 Compare register
    static int TickStarted = FALSE;

    if(TickStarted == FALSE)
        TickStarted = TRUE;
    else
        return;	
    	
    if(RiscRdID()  == RISC0)
        isr_base_addr = RISC0_THIRD_VECTOR_B_ADDR | 0xa0000000;    // use phy-addr to avoid flush cache
    else
        isr_base_addr = RISC1_THIRD_VECTOR_B_ADDR | 0xa0000000;    // use phy-addr to avoid flush cache  

    // for 4KEC, Count increments every other clock, but in fact every clock
    compare = CPU_MAIN_CLOCK / (2 * ticks_per_sec);    // every other clock
    //compare = CPU_MAIN_CLOCK / (1 * ticks_per_sec);    // every clock

    *(unsigned int *)(isr_base_addr + 0x70) = (unsigned int)BspTimerISR;       // Relocate IM7 ISR
    
    ExceptEnCount_Compare(compare); // enable the Count/Compare and its interrupt, 
                                    // enable the whole interrupt(set bit0 of Status)   
                                                                        
}

/*
***************************************************************************
*                         BspInitIntHandlers
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
void  BspInitIntrHandlers (void)
{
    unsigned int isr_base_addr;
    	
    if(RiscRdID()  == RISC0)
        isr_base_addr = RISC0_THIRD_VECTOR_B_ADDR | 0xa0000000;    // use phy-addr to avoid flush cache
    else
        isr_base_addr = RISC1_THIRD_VECTOR_B_ADDR | 0xa0000000;    // use phy-addr to avoid flush cache
        	  
    *(unsigned int *)isr_base_addr          = (unsigned int)BspPrintIntrMsgIP0;       // Software Interrupt 0
    *(unsigned int *)(isr_base_addr + 0x10) = (unsigned int)BspPrintIntrMsgIP1;       // Software Interrupt 1
    *(unsigned int *)(isr_base_addr + 0x20) = (unsigned int)BspPrintIntrMsgIP2;       // Hardware Interrupt 0
    *(unsigned int *)(isr_base_addr + 0x30) = (unsigned int)BspPrintIntrMsgIP3;       // Hardware Interrupt 1
    *(unsigned int *)(isr_base_addr + 0x40) = (unsigned int)BspPrintIntrMsgIP4;       // Hardware Interrupt 2
    *(unsigned int *)(isr_base_addr + 0x50) = (unsigned int)BspPrintIntrMsgIP5;       // Hardware Interrupt 3
    *(unsigned int *)(isr_base_addr + 0x60) = (unsigned int)BspPrintIntrMsgIP6;       // Hardware Interrupt 4
    *(unsigned int *)(isr_base_addr + 0x70) = (unsigned int)BspPrintIntrMsgIP7;       // Hardware Interrupt 5
}

/*
***************************************************************************
*                            BspIntrHandler
*
* Description: This routine is the interupt handler. 
*
* Arguments  : none.
*
* Return     : none.
*
* Note(s)    : 
***************************************************************************
*/
unsigned int g_excpt_id;
void  _BspIntrHandler (CP0_STATUS_CAUSE *pCP0_status_cause)
{  
    int i;
    unsigned int int_pending, isr_base_addr;
    unsigned int exc_code;
    unsigned char err;
    static int exception_addr = -1;
    	
    if(RiscRdID()  == RISC0)
        isr_base_addr = RISC0_THIRD_VECTOR_B_ADDR;
    else
        isr_base_addr = RISC1_THIRD_VECTOR_B_ADDR;
    
    int_pending = (pCP0_status_cause->status & pCP0_status_cause->cause & 0xff00) >> 8;
    
    /* handle the interrupt from IM7 to IM0
    for(i = 7; i >= 0 ; i--) {
        if(int_pending  & (1 << i) )	
            ((void(*)(void))(*(unsigned int *)(isr_base_addr + i * 16)))();
    }        */

    /* handle the interrupt by priority */
    /* the first priority: GPIO intr */
    if(int_pending & (1 << 5))	 {
        ((void(*)(void))(*(unsigned int *)(isr_base_addr + 5 * 16)))(); 
        g_excpt_id = 0x0305;
        return;
    }

    /* the second priority: RISC modules intr */
    if(int_pending & (1 << 2))	 {
        ((void(*)(void))(*(unsigned int *)(isr_base_addr + 2 * 16)))(); 
        return;
    }

    /* the third priority: Mips timer intr */
    if(int_pending & (1 << 7))	 {
        ((void(*)(void))(*(unsigned int *)(isr_base_addr + 7 * 16)))();
         g_excpt_id = 0x0307;
        return;
    }
    
    /* the fourth priority: Flag intr */
    if(int_pending & (1 << 3))	 {
        ((void(*)(void))(*(unsigned int *)(isr_base_addr + 3 * 16)))();
         g_excpt_id = 0x0303;
        return;
    }

    /* the lowest priority: reserved intrs */
    if(int_pending & (1 << 4))	 {
        ((void(*)(void))(*(unsigned int *)(isr_base_addr + 4 * 16)))(); 
         g_excpt_id = 0x0304;
        return;
    }

    if(int_pending & (1 << 6))	 {
        ((void(*)(void))(*(unsigned int *)(isr_base_addr + 6 * 16)))(); 
         g_excpt_id = 0x0306;
        return;
    }

    if(int_pending & (1 << 0))	 {
        ((void(*)(void))(*(unsigned int *)(isr_base_addr + 0 * 16)))();
         g_excpt_id = 0x0300;
        return;
    }

    if(int_pending & (1 << 1))	 {
        ((void(*)(void))(*(unsigned int *)(isr_base_addr + 1 * 16)))(); 
         g_excpt_id = 0x0301;
        return;
    }

    g_excpt_id = 0xffff;

    exc_code = (pCP0_status_cause->cause & 0x7c) >> 2;

    printf("\n\n_BspIntrHandler: an exception occur! ExcCode = %d, pCP0_status_cause->status = 0x%x, pCP0_status_cause->cause = 0x%x!\n", \
                            exc_code, pCP0_status_cause->status, pCP0_status_cause->cause);
    BspPrintfCp0Reg(1);	

    if(pCP0_status_cause->status & 0x4) {    // ERL is set?
#ifdef AUDIO_MODULE_INCLUDE    
        /* Close all the channels! Stop linux reset because of MC intr! */
        AppAudioChannelClose(0);
        AppAudioChannelClose(1);
        AppAudioChannelClose(2);
        AppAudioChannelClose(3);         
#endif
        printf("_BspIntrHandler: ERL is set!\n");
                    
        TaskStatisticsPrintTaskStackInfo(OSPrioCur);                         

        /* Reset, Soft Reset, NMI or Cache Error exception */
        if(OSRunning == TRUE)
            TaskStatisticsPrintTaskInfo();

        PrintAllInUartBuf();

        while(1);        
    }

    gtStatistics.cnt_all_excpts++;

    if(exc_code) {    // ExcCode is non-zero, an exception occur!

        if(OSIntNesting > 1) {   // intr or exception nested! It's not permitted!    
#ifdef AUDIO_MODULE_INCLUDE    
            /* Close all the channels! Stop linux reset because of MC intr! */
            AppAudioChannelClose(0);
            AppAudioChannelClose(1);
            AppAudioChannelClose(2);
            AppAudioChannelClose(3);                   
#endif
            printf("_BspIntrHandler: intr or exception nested!\n");
             
            TaskStatisticsPrintTaskStackInfo(OSPrioCur);   
            
            if(OSRunning == TRUE)
                TaskStatisticsPrintTaskInfo();

            PrintAllInUartBuf();

            while(1);
        } else {

            TaskStatisticsPrintTaskStackInfo(OSPrioCur);

            if(exception_addr == GetC0_EPC()) {
                exception_addr = -1;
                gtStatistics.cnt_all_excpts_not_caused_by_icache++;     /* cnt_all_excpts_not_caused_by_icache should be the sum of 
                                                                                                        cnt_excpt_tlb, cnt_excpt_ade,  cnt_excpt_ri, cnt_excpt_cpu, cnt_excpt_others! */
            } else {
                exception_addr = GetC0_EPC();
                cache_invalidate_I(exception_addr, 16);
                printf("_BspIntrHandler: i-cache refill 0x%x!\n", exception_addr);                
                return;
            }
                        
            switch(exc_code) {
                case EXC_TLBL:
                case EXC_TLBS:
                    gtStatistics.cnt_excpt_tlb++;
                    break;

                case EXC_ADEL:
                case EXC_ADES:
                    gtStatistics.cnt_excpt_ade++;
                    break;

                case EXC_RI:
                    gtStatistics.cnt_excpt_ri++;
                    break;                    

                case EXC_CPU:
                    gtStatistics.cnt_excpt_cpu++;
                    break;                    

                default:
                    gtStatistics.cnt_excpt_others++;                    
                    break;
            }

            /* Because OSTaskDel cannot be called in ISR, so suspend the task here, then delete & re-create it outside! */
            err = OSTaskSuspend(OSPrioCur);
            if(err == OS_NO_ERR)
                printf("\n_BspIntrHandler: OSTaskSuspend(%d) ok\n", OSPrioCur);
            else {
                printf("\n_BspIntrHandler: OSTaskSuspend(%d) failed(%d)!\n", OSPrioCur, err);                

                PrintAllInUartBuf();

                while(1);
            }

            switch(OSPrioCur) {
                case PRIOR_TASK_ROLLSTR:
                    err = OSQPost(gptQMsgTwoRiscSystemMonitor, (void *)TWO_RISC_SYSTEM_REOPEN_TASK_ROLLSTR);
                    if(err != OS_NO_ERR)            
                        printf("\n_BspIntrHandler: OSQPost error! err = %d!\n", err);                      
                    break;
                    
                case PRIOR_TASK_PRINT:
                    PrintAllInUartBuf();
                    break;

                case PRIOR_TASK_TWO_RISC_SEND_MPEG_H263_ENCODE:
                case PRIOR_TASK_IPCAM:
                    if(OSQPost(gptQMsgTwoRiscSystemMonitor, (void *)TWO_RISC_SYSTEM_REOPEN_VIN_TASK) != OS_NO_ERR)            
                        printf("\n_BspIntrHandler: OSQPost error!\n");                                                             
                    break;
                    
#ifdef AUDIO_MODULE_INCLUDE
                case PRIOR_TASK_TA:
                    /* Close all the channels! Stop linux reset because of MC intr! */
                    AppAudioChannelClose(0);
                    AppAudioChannelClose(1);
                    AppAudioChannelClose(2);
                    AppAudioChannelClose(3);                    
                    break;
#endif

                default:
                    break;
            }
            
        }
        
    } else {    // ExcCode is zero, unknow MIPS intr type occur! Maybe caused by GPIO
        printf("\n_BspIntrHandler: unknow MIPS intr type occur!\n");
        printf("\nrRISC_INT_STATUS = 0x%x, rRISC_GPIO = 0x%x, rRISC_GPIO_DIR = 0x%x, rRISC0_GPIO_INTR_MASK = 0x%x, rRISC1_GPIO_INTR_MASK = 0x%x, rRISC_GPIO_POL = 0x%x\n", \
                    rRISC_INT_STATUS, rRISC_GPIO, rRISC_GPIO_DIR, rRISC0_GPIO_INTR_MASK, rRISC1_GPIO_INTR_MASK, rRISC_GPIO_POL);
        gtStatistics.cnt_unknow_mips_intr++;
    }
    
}

void  BspIntrHandler (CP0_STATUS_CAUSE *pCP0_status_cause)
{
    unsigned int tmp1,tmp2;

    tmp1 = GetCount();
    
    _BspIntrHandler(pCP0_status_cause);

    tmp2 = GetCount();

    if(tmp2 >= tmp1)
        tmp2 -= tmp1;
   else{
      tmp2 = tmp1 - CPU_MAIN_CLOCK / (2 * OS_TICKS_PER_SEC)  + tmp2;
   }
    if(tmp2 > gtStatistics.time_in_intr){
            gtStatistics.time_in_intr = tmp2;
            gtStatistics.ISR_ID = g_excpt_id;
    }  
    
    return;
}
/*
***************************************************************************
*                            BspPrintRegs
*
* Description: This routine will print regs like sc, mc, cctl to make sure the right bios code.
*                   (Somebody will change the bios(for example the sdram clock) for some reason
                        but users do not know the change!)    
*
* Arguments  : none.
*
* Return     : none.
*
* Note(s)    : 
***************************************************************************
*/
void  BspPrintRegs (void)
{  
    unsigned int i;

    /* Risc regs */
    _PRINTF("\r\n\r\nRisc regs:\r\n");
    for(i = 0xa8000000; i <= 0xa8000100 ; i+=4) {
        _PRINTF("0x%x = 0x%x\r\n", i, *(volatile unsigned int *)i);
    }        

    /* Cctl regs */
    _PRINTF("\r\n\r\nCctl regs:\r\n");
    for(i = 0xa8010000; i <= 0xa8010040 ; i+=4) {
        _PRINTF("0x%x = 0x%x\r\n", i, *(volatile unsigned int *)i);
    }    

    /* Mc regs */
    _PRINTF("\r\n\r\nMc regs:\r\n");
    for(i = 0xa8020000; i <= 0xa80206fc ; i+=4) {
        _PRINTF("0x%x = 0x%x\r\n", i, *(volatile unsigned int *)i);
    }     

    /* Sc regs */
    _PRINTF("\r\n\r\nSc regs:\r\n");
    for(i = 0xa8040000; i <= 0xa80401fc ; i+=4) {
        _PRINTF("0x%x = 0x%x\r\n", i, *(volatile unsigned int *)i);
    }     

    _PRINTF("\r\n\r\n");
    
}

void Main (void)
{       
    char tmp;
    	
    // From init.S, interrupt should be disabled. But No need disable the interrupt, because this has been done in init.S
    
    BspChangePllValue();    // Change the clock again. This need not be done for FPGA.    
    
    if(CommInit(UART_CHANNEL, 115200) == FALSE) {      // Baudrate is 115200. 
        while(1);
    }
    delay(100, 100);        // Delay to ensure Uart0 setup succeeds    
    
    _PRINTF("\r\nRisc%d\r\n", RiscRdID());        
    _PRINTF("\r\n");   
    _PRINTF("*********************************************************************************************************\r\n");
    _PRINTF("*                                Copyright (c) 2006 VBridge Microsystem, Inc.                           *\r\n");
    _PRINTF("*                                      All Rights Reserved                                              *\r\n");
    _PRINTF("*********************************************************************************************************\r\n");
    _PRINTF("\r\n");
    _PRINTF("Preparing to enter into uCOS-II initialization! ...\r\n");    

    _PRINTF("\r\nVbridgeRealtimeProject %s.\r\n\r\n", version);

    if(RiscRdID() == RISC0) { 
        _PRINTF("ucos link information:\r\n");

        _PRINTF("\r\nEntry point of application: _start = 0x%x!\r\n", GetLink_start());

        _PRINTF("\r\nStart of code and read-only data: _ftext = 0x%x!\r\n", GetLink_ftext());
        _PRINTF("End of code: _ecode = 0x%x!\r\n", GetLink_ecode());
        _PRINTF("End of code and read-only data: _etext = 0x%x!\r\n", GetLink_etext());

        _PRINTF("\r\nStart of initialised data: _fdata = 0x%x!\r\n", GetLink_fdata());
        _PRINTF("End of initialised data: _edata = 0x%x!\r\n", GetLink_edata());

        _PRINTF("\r\nStart of uninitialised data: _fbss = 0x%x!\r\n", GetLink_fbss());
        _PRINTF("End of unitialised data: _end = 0x%x!\r\n", GetLink_end());    

        _PRINTF("\r\nBase of small data(gp): _gp = 0x%x!\r\n\r\n", GetLink_gp());
    
        _PRINTF("stack bottom: _freemem = 0x%x!\r\n", GetLink_freemem());    
        _PRINTF("stack size: _SYS_STKSIZE = 0x800!\r\n");    
        _PRINTF("stack top(top of sdram): 0x%x!\r\n", GetLink_freemem() + 0x800);

        _PRINTF("\r\nUCOS_E_ADDR = 0x%x!\r\n", UCOS_E_ADDR | 0x80000000);

#ifdef AUDIO_MODULE_INCLUDE
        _PRINTF("\r\nAudio Output Buffer(MAX_AUDIO_CHANNEL is %d) from 0x%x to 0x%x!\r\n", MAX_AUDIO_CHANNEL, (UCOS_E_ADDR | 0x80000000) - CHANNEL_BUFFER_SIZE * 2 * MAX_AUDIO_CHANNEL, UCOS_E_ADDR | 0x80000000);
        if(GetLink_freemem() + 0x800 < (UCOS_E_ADDR | 0x80000000) - CHANNEL_BUFFER_SIZE * 2 * MAX_AUDIO_CHANNEL)
            ;//_PRINTF("\nMemory Map OK!\n");  
        else {
            _PRINTF("\nMemory Map Failed! Risc0 Stop here!\n");          
            while(1);
        }
#else
        _PRINTF("\nNo Audio Output Buffer!\n");
        if(GetLink_freemem() + 0x800 < (UCOS_E_ADDR & 0xfffffff) | 0x80000000)
            ;//_PRINTF("\nMemory Map OK!\n");  
        else {
            _PRINTF("\nMemory Map Failed! Risc0 Stop here!\n");                  
            while(1);
        }
#endif

        /* check the share buffers! */
#ifdef AUDIO_MODULE_INCLUDE        
        _PRINTF("\nAUDIO_UCOS_TO_LINUX_B_ADDR = 0x%x, AUDIO_UCOS_TO_LINUX_E_ADDR = 0x%x!\n", \
                   AUDIO_UCOS_TO_LINUX_B_ADDR,        AUDIO_UCOS_TO_LINUX_E_ADDR);
        _PRINTF("\nAUDIO_LINUX_TO_UCOS_B_ADDR = 0x%x, AUDIO_LINUX_TO_UCOS_E_ADDR = 0x%x!\n", \
                   AUDIO_LINUX_TO_UCOS_B_ADDR,        AUDIO_LINUX_TO_UCOS_E_ADDR);   
        _PRINTF("\nAUDIO_PARA_B_ADDR = 0x%x, AUDIO_PARA_E_ADDR = 0x%x!\n", \
                   AUDIO_PARA_B_ADDR,        AUDIO_PARA_E_ADDR);         
        _PRINTF("\nSLIC_UCOS_TO_LINUX_B_ADDR = 0x%x, SLIC_UCOS_TO_LINUX_E_ADDR = 0x%x!\n", \
                   SLIC_UCOS_TO_LINUX_B_ADDR,        SLIC_UCOS_TO_LINUX_E_ADDR);
#endif                   
        _PRINTF("\nCOMM_COUNTBUF_B_ADDR = 0x%x, COMM_COUNTBUF_E_ADDR = 0x%x!\n", \
                   COMM_COUNTBUF_B_ADDR,        COMM_COUNTBUF_E_ADDR);     
                   
#ifdef VIDEO_MODULE_INCLUDE                           
        _PRINTF("\nVIDEO_UCOS_TO_LINUX_B_ADDR = 0x%x, VIDEO_UCOS_TO_LINUX_E_ADDR = 0x%x!\n", \
                   VIDEO_UCOS_TO_LINUX_B_ADDR,        VIDEO_UCOS_TO_LINUX_E_ADDR);   
        _PRINTF("\nVIDEO_LINUX_TO_UCOS_B_ADDR = 0x%x, VIDEO_LINUX_TO_UCOS_E_ADDR = 0x%x!\n", \
                   VIDEO_LINUX_TO_UCOS_B_ADDR,        VIDEO_LINUX_TO_UCOS_E_ADDR);
#endif                   
        
#ifdef NEW_MEMORY_MAP
        _PRINTF("\n\nuCOS will use the new memory map(audio share buffer is in the memory outside of each risc, but video share buffer is in the memory of each risc!)!\n\n");
#ifdef AUDIO_MODULE_INCLUDE        
        if(((unsigned int)AUDIO_UCOS_TO_LINUX_B_ADDR | 0xa0000000) > ((unsigned int)UCOS_B_ADDR | 0xa0000000) && \
           ((unsigned int)AUDIO_UCOS_TO_LINUX_B_ADDR | 0xa0000000) < ((unsigned int)UCOS_E_ADDR | 0xa0000000)) {
            _PRINTF("\nAUDIO_UCOS_TO_LINUX_B_ADDR is bad! Risc0 Stop here!\n");
            while(1);
        }

        if(((unsigned int)AUDIO_UCOS_TO_LINUX_E_ADDR | 0xa0000000) > ((unsigned int)UCOS_B_ADDR | 0xa0000000) && \
           ((unsigned int)AUDIO_UCOS_TO_LINUX_E_ADDR | 0xa0000000) < ((unsigned int)UCOS_E_ADDR | 0xa0000000)) {
            _PRINTF("\nAUDIO_UCOS_TO_LINUX_E_ADDR is bad! Risc0 Stop here!\n");
            while(1);
        }

        if(((unsigned int)AUDIO_LINUX_TO_UCOS_B_ADDR | 0xa0000000) > ((unsigned int)UCOS_B_ADDR | 0xa0000000) && \
           ((unsigned int)AUDIO_LINUX_TO_UCOS_B_ADDR | 0xa0000000) < ((unsigned int)UCOS_E_ADDR | 0xa0000000)) {
            _PRINTF("\nAUDIO_LINUX_TO_UCOS_B_ADDR is bad! Risc0 Stop here!\n");
            while(1);
        }

        if(((unsigned int)AUDIO_LINUX_TO_UCOS_E_ADDR | 0xa0000000) > ((unsigned int)UCOS_B_ADDR | 0xa0000000) && \
           ((unsigned int)AUDIO_LINUX_TO_UCOS_E_ADDR | 0xa0000000) < ((unsigned int)UCOS_E_ADDR | 0xa0000000)) {
            _PRINTF("\nAUDIO_LINUX_TO_UCOS_E_ADDR is bad! Risc0 Stop here!\n");
            while(1);
        }

        if(((unsigned int)AUDIO_PARA_B_ADDR | 0xa0000000) > ((unsigned int)UCOS_B_ADDR | 0xa0000000) && \
           ((unsigned int)AUDIO_PARA_B_ADDR | 0xa0000000) < ((unsigned int)UCOS_E_ADDR | 0xa0000000)) {
            _PRINTF("\nAUDIO_PARA_B_ADDR is bad! Risc0 Stop here!\n");
            while(1);
        }        

        if(((unsigned int)AUDIO_PARA_E_ADDR | 0xa0000000) > ((unsigned int)UCOS_B_ADDR | 0xa0000000) && \
           ((unsigned int)AUDIO_PARA_E_ADDR | 0xa0000000) < ((unsigned int)UCOS_E_ADDR | 0xa0000000)) {
            _PRINTF("\nAUDIO_PARA_E_ADDR is bad! Risc0 Stop here!\n");
            while(1);
        }    

        if(((unsigned int)SLIC_UCOS_TO_LINUX_B_ADDR | 0xa0000000) > ((unsigned int)UCOS_B_ADDR | 0xa0000000) && \
           ((unsigned int)SLIC_UCOS_TO_LINUX_B_ADDR | 0xa0000000) < ((unsigned int)UCOS_E_ADDR | 0xa0000000)) {
            _PRINTF("\nSLIC_UCOS_TO_LINUX_B_ADDR is bad! Risc0 Stop here!\n");
            while(1);
        }        

        if(((unsigned int)SLIC_UCOS_TO_LINUX_E_ADDR | 0xa0000000) > ((unsigned int)UCOS_B_ADDR | 0xa0000000) && \
           ((unsigned int)SLIC_UCOS_TO_LINUX_E_ADDR | 0xa0000000) < ((unsigned int)UCOS_E_ADDR | 0xa0000000)) {
            _PRINTF("\nSLIC_UCOS_TO_LINUX_E_ADDR is bad! Risc0 Stop here!\n");
            while(1);
        }     
#endif             

        if(((unsigned int)COMM_COUNTBUF_B_ADDR | 0xa0000000) > ((unsigned int)UCOS_B_ADDR | 0xa0000000) && \
           ((unsigned int)COMM_COUNTBUF_B_ADDR | 0xa0000000) < ((unsigned int)UCOS_E_ADDR | 0xa0000000)) {
            _PRINTF("\nCOMM_COUNTBUF_B_ADDR is bad! Risc0 Stop here!\n");
            while(1);
        }  

        if(((unsigned int)COMM_COUNTBUF_E_ADDR | 0xa0000000) > ((unsigned int)UCOS_B_ADDR | 0xa0000000) && \
           ((unsigned int)COMM_COUNTBUF_E_ADDR | 0xa0000000) < ((unsigned int)UCOS_E_ADDR | 0xa0000000)) {
            _PRINTF("\nCOMM_COUNTBUF_E_ADDR is bad! Risc0 Stop here!\n");
            while(1);
        }         

#ifdef VIDEO_MODULE_INCLUDE1
        if(!(((unsigned int)VIDEO_UCOS_TO_LINUX_B_ADDR | 0xa0000000) > ((unsigned int)UCOS_B_ADDR | 0xa0000000) && \
             ((unsigned int)VIDEO_UCOS_TO_LINUX_B_ADDR | 0xa0000000) < ((unsigned int)UCOS_E_ADDR | 0xa0000000))) {
            _PRINTF("\nVIDEO_UCOS_TO_LINUX_B_ADDR is bad! Risc0 Stop here!\n");
            while(1);
        } 

        if(((unsigned int)VIDEO_UCOS_TO_LINUX_E_ADDR | 0xa0000000) > ((unsigned int)UCOS_B_ADDR | 0xa0000000) && \
           ((unsigned int)VIDEO_UCOS_TO_LINUX_E_ADDR | 0xa0000000) < ((unsigned int)UCOS_E_ADDR | 0xa0000000)) {
            _PRINTF("\nVIDEO_UCOS_TO_LINUX_E_ADDR is bad! Risc0 Stop here!\n");
            while(1);
        }  
#endif               
        
#else
        _PRINTF("\n\nuCOS will use the old memory map(both audio and video share buffer are in the memory outside of each risc!)!\n\n");
#ifdef AUDIO_MODULE_INCLUDE
        if(((unsigned int)AUDIO_UCOS_TO_LINUX_B_ADDR | 0xa0000000) > ((unsigned int)UCOS_B_ADDR | 0xa0000000) && \
           ((unsigned int)AUDIO_UCOS_TO_LINUX_B_ADDR | 0xa0000000) < ((unsigned int)UCOS_E_ADDR | 0xa0000000)) {
            _PRINTF("\nAUDIO_UCOS_TO_LINUX_B_ADDR is bad! Risc0 Stop here!\n");
            while(1);
        }

        if(((unsigned int)AUDIO_UCOS_TO_LINUX_E_ADDR | 0xa0000000) > ((unsigned int)UCOS_B_ADDR | 0xa0000000) && \
           ((unsigned int)AUDIO_UCOS_TO_LINUX_E_ADDR | 0xa0000000) < ((unsigned int)UCOS_E_ADDR | 0xa0000000)) {
            _PRINTF("\nAUDIO_UCOS_TO_LINUX_E_ADDR is bad! Risc0 Stop here!\n");
            while(1);
        }

        if(((unsigned int)AUDIO_LINUX_TO_UCOS_B_ADDR | 0xa0000000) > ((unsigned int)UCOS_B_ADDR | 0xa0000000) && \
           ((unsigned int)AUDIO_LINUX_TO_UCOS_B_ADDR | 0xa0000000) < ((unsigned int)UCOS_E_ADDR | 0xa0000000)) {
            _PRINTF("\nAUDIO_LINUX_TO_UCOS_B_ADDR is bad! Risc0 Stop here!\n");
            while(1);
        }

        if(((unsigned int)AUDIO_LINUX_TO_UCOS_E_ADDR | 0xa0000000) > ((unsigned int)UCOS_B_ADDR | 0xa0000000) && \
           ((unsigned int)AUDIO_LINUX_TO_UCOS_E_ADDR | 0xa0000000) < ((unsigned int)UCOS_E_ADDR | 0xa0000000)) {
            _PRINTF("\nAUDIO_LINUX_TO_UCOS_E_ADDR is bad! Risc0 Stop here!\n");
            while(1);
        }

        if(((unsigned int)AUDIO_PARA_B_ADDR | 0xa0000000) > ((unsigned int)UCOS_B_ADDR | 0xa0000000) && \
           ((unsigned int)AUDIO_PARA_B_ADDR | 0xa0000000) < ((unsigned int)UCOS_E_ADDR | 0xa0000000)) {
            _PRINTF("\nAUDIO_PARA_B_ADDR is bad! Risc0 Stop here!\n");
            while(1);
        }        

        if(((unsigned int)AUDIO_PARA_E_ADDR | 0xa0000000) > ((unsigned int)UCOS_B_ADDR | 0xa0000000) && \
           ((unsigned int)AUDIO_PARA_E_ADDR | 0xa0000000) < ((unsigned int)UCOS_E_ADDR | 0xa0000000)) {
            _PRINTF("\nAUDIO_PARA_E_ADDR is bad! Risc0 Stop here!\n");
            while(1);
        }    

        if(((unsigned int)SLIC_UCOS_TO_LINUX_B_ADDR | 0xa0000000) > ((unsigned int)UCOS_B_ADDR | 0xa0000000) && \
           ((unsigned int)SLIC_UCOS_TO_LINUX_B_ADDR | 0xa0000000) < ((unsigned int)UCOS_E_ADDR | 0xa0000000)) {
            _PRINTF("\nSLIC_UCOS_TO_LINUX_B_ADDR is bad! Risc0 Stop here!\n");
            while(1);
        }        

        if(((unsigned int)SLIC_UCOS_TO_LINUX_E_ADDR | 0xa0000000) > ((unsigned int)UCOS_B_ADDR | 0xa0000000) && \
           ((unsigned int)SLIC_UCOS_TO_LINUX_E_ADDR | 0xa0000000) < ((unsigned int)UCOS_E_ADDR | 0xa0000000)) {
            _PRINTF("\nSLIC_UCOS_TO_LINUX_E_ADDR is bad! Risc0 Stop here!\n");
            while(1);
        }          
#endif
#ifdef COMM_COUNTBUF
        if(((unsigned int)COMM_COUNTBUF_B_ADDR | 0xa0000000) > ((unsigned int)UCOS_B_ADDR | 0xa0000000) && \
           ((unsigned int)COMM_COUNTBUF_B_ADDR | 0xa0000000) < ((unsigned int)UCOS_E_ADDR | 0xa0000000)) {
            _PRINTF("\nCOMM_COUNTBUF_B_ADDR is bad! Risc0 Stop here!\n");
            while(1);
        }  

        if(((unsigned int)COMM_COUNTBUF_E_ADDR | 0xa0000000) > ((unsigned int)UCOS_B_ADDR | 0xa0000000) && \
           ((unsigned int)COMM_COUNTBUF_E_ADDR | 0xa0000000) < ((unsigned int)UCOS_E_ADDR | 0xa0000000)) {
            _PRINTF("\nCOMM_COUNTBUF_E_ADDR is bad! Risc0 Stop here!\n");
            while(1);
        }         
#endif
#ifdef VIDEO_MODULE_INCLUDE
        if(((unsigned int)VIDEO_UCOS_TO_LINUX_B_ADDR | 0xa0000000) > ((unsigned int)UCOS_B_ADDR | 0xa0000000) && \
           ((unsigned int)VIDEO_UCOS_TO_LINUX_B_ADDR | 0xa0000000) < ((unsigned int)UCOS_E_ADDR | 0xa0000000)) {
            _PRINTF("\nVIDEO_UCOS_TO_LINUX_B_ADDR is bad! Risc0 Stop here!\n");
            while(1);
        } 

        if(((unsigned int)VIDEO_UCOS_TO_LINUX_E_ADDR | 0xa0000000) > ((unsigned int)UCOS_B_ADDR | 0xa0000000) && \
           ((unsigned int)VIDEO_UCOS_TO_LINUX_E_ADDR | 0xa0000000) < ((unsigned int)UCOS_E_ADDR | 0xa0000000)) {
            _PRINTF("\nVIDEO_UCOS_TO_LINUX_E_ADDR is bad! Risc0 Stop here!\n");
            while(1);
        }         
#endif
     
#endif        
    }
#if 0 //error when compile "UCOS_B_ADDR_RISC1 not define"
    if(RiscRdID() == RISC1) { 
#ifdef NEW_MEMORY_MAP
        _PRINTF("\n\nuCOS(risc1) will use the new memory map!\n\n");
#ifdef AUDIO_MODULE_INCLUDE        
        if(((unsigned int)AUDIO_UCOS_TO_LINUX_B_ADDR | 0xa0000000) > ((unsigned int)UCOS_B_ADDR_RISC1 | 0xa0000000) && \
           ((unsigned int)AUDIO_UCOS_TO_LINUX_B_ADDR | 0xa0000000) < ((unsigned int)UCOS_E_ADDR_RISC1 | 0xa0000000)) {
            _PRINTF("\nAUDIO_UCOS_TO_LINUX_B_ADDR is bad! Risc1 Stop here!\n");
            while(1);
        }

        if(((unsigned int)AUDIO_UCOS_TO_LINUX_E_ADDR | 0xa0000000) > ((unsigned int)UCOS_B_ADDR_RISC1 | 0xa0000000) && \
           ((unsigned int)AUDIO_UCOS_TO_LINUX_E_ADDR | 0xa0000000) < ((unsigned int)UCOS_E_ADDR_RISC1 | 0xa0000000)) {
            _PRINTF("\nAUDIO_UCOS_TO_LINUX_E_ADDR is bad! Risc1 Stop here!\n");
            while(1);
        }

        if(((unsigned int)AUDIO_LINUX_TO_UCOS_B_ADDR | 0xa0000000) > ((unsigned int)UCOS_B_ADDR_RISC1 | 0xa0000000) && \
           ((unsigned int)AUDIO_LINUX_TO_UCOS_B_ADDR | 0xa0000000) < ((unsigned int)UCOS_E_ADDR_RISC1 | 0xa0000000)) {
            _PRINTF("\nAUDIO_LINUX_TO_UCOS_B_ADDR is bad! Risc1 Stop here!\n");
            while(1);
        }

        if(((unsigned int)AUDIO_LINUX_TO_UCOS_E_ADDR | 0xa0000000) > ((unsigned int)UCOS_B_ADDR_RISC1 | 0xa0000000) && \
           ((unsigned int)AUDIO_LINUX_TO_UCOS_E_ADDR | 0xa0000000) < ((unsigned int)UCOS_E_ADDR_RISC1 | 0xa0000000)) {
            _PRINTF("\nAUDIO_LINUX_TO_UCOS_E_ADDR is bad! Risc1 Stop here!\n");
            while(1);
        }

        if(((unsigned int)AUDIO_PARA_B_ADDR | 0xa0000000) > ((unsigned int)UCOS_B_ADDR_RISC1 | 0xa0000000) && \
           ((unsigned int)AUDIO_PARA_B_ADDR | 0xa0000000) < ((unsigned int)UCOS_E_ADDR_RISC1 | 0xa0000000)) {
            _PRINTF("\nAUDIO_PARA_B_ADDR is bad! Risc1 Stop here!\n");
            while(1);
        }        

        if(((unsigned int)AUDIO_PARA_E_ADDR | 0xa0000000) > ((unsigned int)UCOS_B_ADDR_RISC1 | 0xa0000000) && \
           ((unsigned int)AUDIO_PARA_E_ADDR | 0xa0000000) < ((unsigned int)UCOS_E_ADDR_RISC1 | 0xa0000000)) {
            _PRINTF("\nAUDIO_PARA_E_ADDR is bad! Risc1 Stop here!\n");
            while(1);
        }    

        if(((unsigned int)SLIC_UCOS_TO_LINUX_B_ADDR | 0xa0000000) > ((unsigned int)UCOS_B_ADDR_RISC1 | 0xa0000000) && \
           ((unsigned int)SLIC_UCOS_TO_LINUX_B_ADDR | 0xa0000000) < ((unsigned int)UCOS_E_ADDR_RISC1 | 0xa0000000)) {
            _PRINTF("\nSLIC_UCOS_TO_LINUX_B_ADDR is bad! Risc1 Stop here!\n");
            while(1);
        }        

        if(((unsigned int)SLIC_UCOS_TO_LINUX_E_ADDR | 0xa0000000) > ((unsigned int)UCOS_B_ADDR_RISC1 | 0xa0000000) && \
           ((unsigned int)SLIC_UCOS_TO_LINUX_E_ADDR | 0xa0000000) < ((unsigned int)UCOS_E_ADDR_RISC1 | 0xa0000000)) {
            _PRINTF("\nSLIC_UCOS_TO_LINUX_E_ADDR is bad! Risc1 Stop here!\n");
            while(1);
        }     
#endif             

        if(((unsigned int)COMM_COUNTBUF_B_ADDR | 0xa0000000) > ((unsigned int)UCOS_B_ADDR_RISC1 | 0xa0000000) && \
           ((unsigned int)COMM_COUNTBUF_B_ADDR | 0xa0000000) < ((unsigned int)UCOS_E_ADDR_RISC1 | 0xa0000000)) {
            _PRINTF("\nCOMM_COUNTBUF_B_ADDR is bad! Risc1 Stop here!\n");
            while(1);
        }  

        if(((unsigned int)COMM_COUNTBUF_E_ADDR | 0xa0000000) > ((unsigned int)UCOS_B_ADDR_RISC1 | 0xa0000000) && \
           ((unsigned int)COMM_COUNTBUF_E_ADDR | 0xa0000000) < ((unsigned int)UCOS_E_ADDR_RISC1 | 0xa0000000)) {
            _PRINTF("\nCOMM_COUNTBUF_E_ADDR is bad! Risc1 Stop here!\n");
            while(1);
        }         

#ifdef VIDEO_MODULE_INCLUDE
        if(((unsigned int)VIDEO_UCOS_TO_LINUX_B_ADDR | 0xa0000000) > ((unsigned int)UCOS_B_ADDR_RISC1 | 0xa0000000) && \
           ((unsigned int)VIDEO_UCOS_TO_LINUX_B_ADDR | 0xa0000000) < ((unsigned int)UCOS_E_ADDR_RISC1 | 0xa0000000)) {
            _PRINTF("\nVIDEO_UCOS_TO_LINUX_B_ADDR is bad! Risc1 Stop here!\n");
            while(1);
        } 

        if(((unsigned int)VIDEO_UCOS_TO_LINUX_E_ADDR | 0xa0000000) > ((unsigned int)UCOS_B_ADDR_RISC1 | 0xa0000000) && \
             ((unsigned int)VIDEO_UCOS_TO_LINUX_E_ADDR | 0xa0000000) < ((unsigned int)UCOS_E_ADDR_RISC1 | 0xa0000000)) {
            _PRINTF("\nVIDEO_UCOS_TO_LINUX_E_ADDR is bad! Risc1 Stop here!\n");
            while(1);
        }  
#endif               
        
#else
        _PRINTF("\n\nuCOS(risc1) will use the old memory map!\n\n");     
#endif     
    }
#endif    
    if(RiscRdID() == RISC0) { 

#ifdef RISC1_UCOS
        _PRINTF("\r\nrisc1 will run ucos!\r\n");
#endif

#ifdef RISC1_LINUX
        _PRINTF("\r\nrisc1 will run linux!\r\n");
#endif

#ifdef UCOS_BOOT_RISC1
        _PRINTF("\r\nucos will boot risc1!\r\n");
#else
        _PRINTF("\r\nucos will NOT boot risc1!\r\n");
#endif

#if MEMORY_MAP == 16
        _PRINTF("\r\n2-RISC memory map is for 16M!\r\n");
#endif

#ifdef DAISY_CHAIN
        _PRINTF("\r\nDAISY_CHAIN mode!\r\n");
#else
        _PRINTF("\r\nnon-DAISY_CHAIN mode!\r\n");
#endif


    }

#ifdef DUAL_CPU_MODE    
    /* added on July 5th, 2006 */
    if(RiscRdID() == RISC0) {         
        RiscSemLock(7);    // lock the semaphore so that risc1 will wait
#ifdef VIDEO_MODULE_INCLUDE
#if 0           //wy:checksum is enable/disable by api
#ifdef CHECK_SUM
        _DEBUG("\r\nMpeg data adds checksum!\r\n");
#else
        _DEBUG("\r\nMpeg data does NOT add checksum!\r\n");
#endif
#endif
#endif /* VIDEO_MODULE_INCLUDE */

       // BspPrintRegs();

    }
	
    Risc2CpuStart();     	
    	       
    // below is just used in lx160
    //Risc2CpuStartLx160();    
#endif      	
    
    ExceptInstallIntrInRam();    // install the INT handler in RAM for risc0 & risc1
    
    BspInitIntrHandlers();          
   
#ifdef DUAL_CPU_MODE        
    /* added on July 5th */
    if(RiscRdID() == RISC1) {        
        RiscSemPend(7);    // pend the semaphore(risc0 will unlock it)            	
        RiscSemUnLock(5);  // Unlock spi to slic
    }
#endif            

    if(RiscRdID() == RISC0) {  
        ;//TechwellAppMain();
    }
    AppMain();
    

    // When uCOS properly run, CPU will never come here.	
    _PRINTF("uCOS failed!");  	
    while(1);        
}

