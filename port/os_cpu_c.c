/*
*********************************************************************************************************
*                                Copyright (c) 2005 vBridge Microsystem, Inc.  
*                                      Unpublished & Not for Publication
*                                            All Rights Reserved                   
*   
* File:         
*                                                                         
* Description :                     
*
* Date:         Sep 20, 2005                                                           
**********************************************************************************************************
*/


#define  OS_CPU_GLOBALS
#include "includes.h"

unsigned int os_enter_critical_t1, os_enter_critical_t2;

/*
*********************************************************************************************************
*                                       OS INITIALIZATION HOOK
*                                            (BEGINNING)
*
* Description: This function is called by OSInit() at the beginning of OSInit().
*
* Arguments  : none
*
* Note(s)    : 1) Interrupts should be disabled during this call.
*********************************************************************************************************
*/
#if OS_CPU_HOOKS_EN > 0 && OS_VERSION > 203
void  OSInitHookBegin (void)
{
}
#endif

/*
*********************************************************************************************************
*                                       OS INITIALIZATION HOOK
*                                               (END)
*
* Description: This function is called by OSInit() at the end of OSInit().
*
* Arguments  : none
*
* Note(s)    : 1) Interrupts should be disabled during this call.
*********************************************************************************************************
*/
#if OS_CPU_HOOKS_EN > 0 && OS_VERSION > 203
void  OSInitHookEnd (void)
{
}
#endif

/*
*********************************************************************************************************
*                                          TASK CREATION HOOK
*
* Description: This function is called when a task is created.
*
* Arguments  : ptcb   is a pointer to the task control block of the task being created.
*
* Note(s)    : 1) Interrupts are disabled during this call.
*********************************************************************************************************
*/
#if OS_CPU_HOOKS_EN > 0 
void  OSTaskCreateHook (OS_TCB *ptcb)
{
    ptcb = ptcb;                       /* Prevent compiler warning                                     */
}
#endif


/*
*********************************************************************************************************
*                                           TASK DELETION HOOK
*
* Description: This function is called when a task is deleted.
*
* Arguments  : ptcb   is a pointer to the task control block of the task being deleted.
*
* Note(s)    : 1) Interrupts are disabled during this call.
*********************************************************************************************************
*/
#if OS_CPU_HOOKS_EN > 0 
void  OSTaskDelHook (OS_TCB *ptcb)
{
    ptcb = ptcb;                       /* Prevent compiler warning                                     */
}
#endif

/*
*********************************************************************************************************
*                                             IDLE TASK HOOK
*
* Description: This function is called by the idle task.  This hook has been added to allow you to do  
*              such things as STOP the CPU to conserve power.
*
* Arguments  : none
*
* Note(s)    : 1) Interrupts are enabled during this call.
*********************************************************************************************************
*/
#if OS_CPU_HOOKS_EN > 0 && OS_VERSION >= 251
void  OSTaskIdleHook (void)
{
    //debug("\r\nRisc%d: Status = 0x%x, Cause = 0x%x.\r\n", RiscRdID(), CP0_wGetSR(), GetCause());

    //delay(200, 200);
}
#endif

/*
*********************************************************************************************************
*                                           STATISTIC TASK HOOK
*
* Description: This function is called every second by uC/OS-II's statistics task.  This allows your 
*              application to add functionality to the statistics task.
*
* Arguments  : none
*********************************************************************************************************
*/

#if OS_CPU_HOOKS_EN > 0 
void  OSTaskStatHook (void)
{
    printf("\n\nOSCPUUsage = %d%%!\n\n", OSCPUUsage);
}
#endif

/*
*********************************************************************************************************
*                                        INITIALIZE A TASK'S STACK
*
* Description: This function is called by either OSTaskCreate() or OSTaskCreateExt() to initialize the
*              stack frame of the task being created.  This function is highly processor specific.
*
* Arguments  : task          is a pointer to the task code
*
*              pdata         is a pointer to a user supplied data area that will be passed to the task
*                            when the task first executes.
*
*              ptos          is a pointer to the top of stack.  It is assumed that 'ptos' points to
*                            a 'free' entry on the task stack.  If OS_STK_GROWTH is set to 1 then 
*                            'ptos' will contain the HIGHEST valid address of the stack.  Similarly, if
*                            OS_STK_GROWTH is set to 0, the 'ptos' will contains the LOWEST valid address
*                            of the stack.
*
*              opt           specifies options that can be used to alter the behavior of OSTaskStkInit().
*                            (see uCOS_II.H for OS_TASK_OPT_???).
*
* Returns    : Always returns the location of the new top-of-stack' once the processor registers have
*              been placed on the stack in the proper order.
*
* Note(s)    : 
*********************************************************************************************************
*/

// gb register U32 $GP __asm__ ("$28");

OS_STK *OSTaskStkInit (void (*task)(void *pd), void *pdata, OS_STK *ptos, INT16U opt)
{
    OS_STK *stk;
    static INT32U wSR=0;
    static INT32U wGP;
    
    if(wSR == 0) {
        wSR  = CP0_wGetSR();
        //wSR &= 0xfffffffe;    // disable the interrupt
          
        if(RiscRdID() == RISC1)
            wSR |= 0xac01;      // do not enable IP4 (risc timer intr)                
        else
            wSR |= 0xbc01;                
        /* 
        Make sure the interrupts are enabled:
             1. IP7 (Count/Comapre timer interrupt)
             2. IP2 (risc modules intr)
             3. IP3 (risc flag intr)
             4. IP4 (risc timer intr)
             5. IP5 (risc gpio intr, but should also set RISC0_GPIO_INTR_MASK or RISC1_GPIO_INTR_MASK to enable it!)
        */                              
        wGP  = GetGP();
    }

    opt      = opt;                         /* 'opt' is not used, prevent warning */
    stk      = ptos;                        /* Load stack pointer                 */
    //*(stk)   = (OS_STK)task;                /* pc: Entry Point                    */
    *(--stk)   = (OS_STK)task;                /* pc: Entry Point                    */
    *(--stk) = (INT32U)wSR;                 /* C0_SR: HW0 = En, IE = En           */
    *(--stk) = (INT32U)0;                   /* lo                                 */        
    *(--stk) = (INT32U)0;                   /* hi                                 */        
    *(--stk) = (INT32U)0;                   /* at                                 */
    *(--stk) = (INT32U)0;                   /* v0                                 */
    *(--stk) = (INT32U)0;                   /* v1                                 */
    *(--stk) = (INT32U)pdata;               /* a0: argument                       */
    *(--stk) = (INT32U)0;                   /* a1                                 */
    *(--stk) = (INT32U)0;                   /* a2                                 */
    *(--stk) = (INT32U)0;                   /* a3                                 */
    *(--stk) = (INT32U)0;                   /* t0                                 */
    *(--stk) = (INT32U)0;                   /* t1                                 */
    *(--stk) = (INT32U)0;                   /* t2                                 */
    *(--stk) = (INT32U)0;                   /* t3                                 */
    *(--stk) = (INT32U)0;                   /* t4                                 */
    *(--stk) = (INT32U)0;                   /* t5                                 */
    *(--stk) = (INT32U)0;                   /* t6                                 */
    *(--stk) = (INT32U)0;                   /* t7                                 */
    *(--stk) = (INT32U)0;                   /* s0                                 */
    *(--stk) = (INT32U)0;                   /* s1                                 */
    *(--stk) = (INT32U)0;                   /* s2                                 */
    *(--stk) = (INT32U)0;                   /* s3                                 */
    *(--stk) = (INT32U)0;                   /* s4                                 */
    *(--stk) = (INT32U)0;                   /* s5                                 */
    *(--stk) = (INT32U)0;                   /* s6                                 */
    *(--stk) = (INT32U)0;                   /* s7                                 */
    *(--stk) = (INT32U)0;                   /* t8                                 */
    *(--stk) = (INT32U)0;                   /* t9                                 */
    *(--stk) = (INT32U)wGP;                 /* gp                                 */
    *(--stk) = (INT32U)0;                   /* fp                                 */
    *(--stk) = (INT32U)0;                   /* ra                                 */

    return (stk);
}

/*
*********************************************************************************************************
*                                           TASK SWITCH HOOK
*
* Description: This function is called when a task switch is performed.  This allows you to perform other
*              operations during a context switch.
*
* Arguments  : none
*
* Note(s)    : 1) Interrupts are disabled during this call.
*              2) It is assumed that the global pointer 'OSTCBHighRdy' points to the TCB of the task that
*                 will be 'switched in' (i.e. the highest priority task) and, 'OSTCBCur' points to the 
*                 task being switched out (i.e. the preempted task).
*********************************************************************************************************
*/
#if OS_CPU_HOOKS_EN > 0 
void  OSTaskSwHook (void)
{
}
#endif

/*
*********************************************************************************************************
*                                           OSTCBInit() HOOK
*
* Description: This function is called by OS_TCBInit() after setting up most of the TCB.
*
* Arguments  : ptcb    is a pointer to the TCB of the task being created.
*
* Note(s)    : 1) Interrupts may or may not be ENABLED during this call.
*********************************************************************************************************
*/
#if OS_CPU_HOOKS_EN > 0 && OS_VERSION > 203
void  OSTCBInitHook (OS_TCB *ptcb)
{
    ptcb = ptcb;                                           /* Prevent Compiler warning                 */
}
#endif


/*
*********************************************************************************************************
*                                               TICK HOOK
*
* Description: This function is called every tick.
*
* Arguments  : none
*
* Note(s)    : 1) Interrupts may or may not be ENABLED during this call.
*********************************************************************************************************
*/
#if OS_CPU_HOOKS_EN > 0 
void  OSTimeTickHook (void)
{
}
#endif


INT32U        OSIntCtxSwFlag = 0;           /* Used to flag a context switch                           */
