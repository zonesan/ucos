/*
*********************************************************************************************************
*                                Copyright (c) 2005 vBridge Microsystem, Inc.  
*                                      Unpublished & Not for Publication
*                                            All Rights Reserved                   
*   
* File        : os_cpu.h             
*                                                                         
* Description : uCOS-ii port file, contains data type define, how to enter into and exit critical section.                      
*                                                
* By          : Gong Bin               
*
* Date        : July 11, 2005                                                           
*********************************************************************************************************
*/



#ifndef __OS_CPU_H__
#define __OS_CPU_H__



#ifdef  OS_CPU_GLOBALS
#define OS_CPU_EXT
#else
#define OS_CPU_EXT  extern
#endif

#include "../application/two_risc_statistics.h"

extern unsigned int os_enter_critical_t1;
extern unsigned int os_enter_critical_t2;

/*
*********************************************************************************************************
*                                              DATA TYPES
*                                         (Compiler Specific)
*********************************************************************************************************
*/
typedef unsigned char  BOOLEAN;
typedef unsigned char  INT8U;                    /* Unsigned  8 bit quantity                           */
typedef signed   char  INT8S;                    /* Signed    8 bit quantity                           */
typedef unsigned short INT16U;                   /* Unsigned 16 bit quantity                           */
typedef signed   short INT16S;                   /* Signed   16 bit quantity                           */
typedef unsigned int   INT32U;                   /* Unsigned 32 bit quantity                           */
typedef signed   int   INT32S;                   /* Signed   32 bit quantity                           */
typedef float          FP32;                     /* Single precision floating point                    */
typedef double         FP64;                     /* Double precision floating point                    */

typedef unsigned int   OS_STK;                   /* Each stack entry is 16-bit wide                    */
typedef unsigned int   OS_CPU_SR;                /* Define size of CPU status register (PSR = 32 bits) */



/* 
*********************************************************************************************************
*                              ARM
*
* Method #1:  Disable/Enable interrupts using simple instructions.  After critical section, interrupts
*             will be enabled even if they were disabled before entering the critical section.
*
* Method #2:  Disable/Enable interrupts by preserving the state of interrupts.  In other words, if 
*             interrupts were disabled before entering the critical section, they will be disabled when
*             leaving the critical section.
*
* Method #3:  Disable/Enable interrupts by preserving the state of interrupts.  Generally speaking you
*             would store the state of the interrupt disable flag in the local variable 'cpu_sr' and then
*             disable interrupts.  'cpu_sr' is allocated in all of uC/OS-II's functions that need to 
*             disable interrupts.  You would restore the interrupt disable state by copying back 'cpu_sr'
*             into the CPU's status register.
*
* Note     :  In this ARM7 Port: Method #1 not implemeted.
*             ------------------------------
*            | Method | SDT 2.51 | ADS 1.2  |
*            |========+==========+==========|
*            |   1    |   N/I    |   N/I    |
*            |   2    |   yes    |   no     |
*            |   3    |   yes    |   yes    |
*             ------------------------------
*
*********************************************************************************************************
*/
#define  OS_CRITICAL_METHOD    3

#if      OS_CRITICAL_METHOD == 2
#define  OS_ENTER_CRITICAL() IRQFIQDE                     // Disable interrupts                        
/* Note: R0 register need not be saved, for it gets saved outside. Refer SDT userguide 6-5 */
#define IRQFIQDE __asm                       \
{                                            \
        mrs r0,CPSR;                         \
        stmfd sp!,{r0};                      \
        orr r0,r0,#NOINT;                    \
        msr CPSR_c,r0;                       \
}

#define  OS_EXIT_CRITICAL() IRQFIQRE                      // Restore  interrupts                       
#define IRQFIQRE __asm                       \
{                                            \
        ldmfd sp!,{r0};                      \
        msr CPSR_c,r0;                       \
}
#endif

#if      OS_CRITICAL_METHOD == 3
#define  OS_ENTER_CRITICAL()  \
    do {\
        cpu_sr = OSCPUSaveSR();\
        os_enter_critical_t1 = GetCount();\
    } while(0)  // Disable interrupts
                      
#define  OS_EXIT_CRITICAL()   \
    do {\
        os_enter_critical_t2 = GetCount();\
        if((os_enter_critical_t2 = os_enter_critical_t2 - os_enter_critical_t1) > gtStatistics.time_in_critical)\
        {\
            gtStatistics.time_in_critical = os_enter_critical_t2;        \
            gtStatistics.pc_critical = GetRa();\
        }\
        if(os_enter_critical_t2 > gtStatistics.time_in_critical_ps)\
            gtStatistics.time_in_critical_ps = os_enter_critical_t2;        \
        OSCPURestoreSR(cpu_sr);\
    } while(0)    // Restore  interrupts                      
#endif

/*
*********************************************************************************************************
*                           ARM Miscellaneous
*********************************************************************************************************
*/

#define  OS_STK_GROWTH        1                       /* Stack grows from HIGH to LOW memory on 80x86  */

#define  OS_TASK_SW()         OSCtxSw()

/*
*********************************************************************************************************
*                                            GLOBAL VARIABLES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                              PROTOTYPES
*********************************************************************************************************
*/
void UCOS_IRQHandler(void);
void OSCtxSw(void);
void OSIntCtxSw(void);

#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
OS_CPU_SR  OSCPUSaveSR(void);
void       OSCPURestoreSR(OS_CPU_SR cpu_sr);
#endif

int   GetGP                 (void);
int   CP0_wGetSR            (void);
int   GetCause              (void);

int   GetC0_ErrorEPC        (void);
int   GetC0_EPC             (void);
int   GetC0_BadVaddr(void);
int   GetC0_Context(void);
int   GetC0_EntryHi(void);
int    GetRa(void);

int GetLink_start(void);
int GetLink_ftext(void);
int GetLink_ecode(void);
int GetLink_etext(void);
int GetLink_fdata(void);
int GetLink_gp(void);
int GetLink_edata(void);
int GetLink_fbss(void);
int GetLink_freemem(void);
int GetLink_end(void);

#endif /*__OS_CPU_H__*/



