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



#define  _EXC_STKSIZE     30*1024

#include "sysdefs.h"
#include "cp0regdef.h"
#include "mips_regdef.h"
#include "context.h"
//#include "os_cpu.h"



/*
***************************************************************************
*                              Gloabl Functions
***************************************************************************
*/
.globl   except_word
except_word:                 
      .word 8: 0x00001234 
      
      .set noreorder

/*
***************************************************************************
*                              OSStartHighRdy
*
* Description: Start multitasking
*
* Arguments  : none
*
* Return     : none.
*
* Note(s)    :  
***************************************************************************
*/
LEAF(OSStartHighRdy)

      jal     OSTaskSwHook            # Call user defined task switch hook 
      nop

      la      t0, OSRunning           # Indicate that multitasking has started 
      li      t1, 1
      sb      t1, 0(t0)               # OSRunning is a boolean 

      la      t0, OSTCBHighRdy        # Get the address of the highest priority task TCB 

      lw      t0, 0(t0)               # get stack pointer(the first address of TCB store the address of stack pointer) 
      lw      sp, 0(t0)               # load the stack pointer of the highest priority task into sp 

      RESTORE_REG_RET()               # pop the highest priority task's at, v0-v1,a0-a3,t0-t9,s0-s7,fp,ra, & pc 

END(OSStartHighRdy)

/*
***************************************************************************
*                                OSCtxSw
*
* Description: 
*
* Arguments  : none
*
* Return     : none.
*
* Note(s)    :  
***************************************************************************
*/
LEAF(OSCtxSw)

      STORE_REG_RET(ra)                 # push the preempted task's at, v0-v1,a0-a3,t0-t9,s0-s7,fp,ra, & pc
       
      # OSPrioCur = OSPrioHighRdy
      la      s0, OSPrioCur
      la      s1, OSPrioHighRdy
      lb      s2, 0(s1)
      sb      s2, 0(s0)
        
      # Get current task TCB address 
      la      s0, OSTCBCur
      lw      s1, 0(s0)
      sw      sp, 0(s1)                 # store sp in preempted tasks's TCB

      jal     OSTaskSwHook              # call Task Switch Hook
      nop

      # Get highest priority task TCB address 
      la      s1, OSTCBHighRdy
      lw      s1, 0(s1)
      lw      sp, 0(s1)                 # load the new task's stack pointer into sp register

      # OSTCBCur = OSTCBHighRdy
      sw      s1, 0(s0)                 # set new current task TCB address

      RESTORE_REG_ERET()                # pop new task's at, v0-v1,a0-a3,t0-t9,s0-s7,fp,ra, & pc
      //RESTORE_REG_RET()                # pop new task's at, v0-v1,a0-a3,t0-t9,s0-s7,fp,ra, & pc
      
END(OSCtxSw)

/*
***************************************************************************
*                                OSIntCtxSw
*
* Description: Set OSIntCtxSwFlag to 1 to tell the ISR it is time to execute 
*              task switch
*
* Arguments  : none
*
* Return     : none.
*
* Note(s)    :  
***************************************************************************
*/
LEAF(OSIntCtxSw)

      # OSIntCtxSwFlag = True 
      la      t0, OSIntCtxSwFlag
      li      t1, 1
      sw      t1, 0(t0)
      jr      ra
      nop
      
END(OSIntCtxSw)
         
/*
***************************************************************************
*                              uCOS_INTHandler
*
* Description: uCOS interrupt handler routine.
*
* Arguments  : none
*
* Return     : none.
*
* Note(s)    :  
***************************************************************************
*/
      BSS
    
      ALIGN(2)                 # 32 bit align 

      .globl  exc_stack_low
     
exc_stack_low:                 # exc_stack is the stack only used for the ISR 
      .space  _EXC_STKSIZE
      .globl  exc_stack_hi
     
exc_stack_hi:                  # exc_stack is the stack only used for the ISR

      ALIGN(4)                 # 16 bytes align
     
      .space  4                # added on Feb 16, 2006
     
exc_context:
      .space  8

LEAF(uCOS_INTHandler)
	la              k1,except_word
      addiu           k1,k1,0x0010
      lw              k0,0x0000(k1)
      bne             k0,$0,2f
      nop
      eret           
      nop
2:      
      cache           21,0x0000(k1)
      nop      

      # store the interrupted task's context
      MFC0(   k0, C0_EPC )        # k0 stores the interrupt return address      
      STORE_REG_RET(k0)           # the value of k0 will be loaded to PC in the next task switch                           

      la      k1, exc_context
      MFC0(   t0, C0_STATUS )
      sw      t0, 0(k1)           # the address of exc_context stores C0_STATUS
      MFC0(   t0, C0_CAUSE )
      sw      t0, 4(k1)           # the address of (exc_context+4) stores C0_CAUSE
        
      move    k0, sp              # k0 store the interrupted task's sp pointer

      # call low level exception handler  
      la      gp, _gp

      la      sp, exc_stack_hi    # load the stack pointer of ISR into sp
      jal     OSIntEnter          # It is imperative that k1 survives 
      nop
        
      # by gb
      #li      t0, 0x30d40         # 20Mhz, an interrupt/10mS
      #MTC0(   t0, C0_Compare)
     
      #li      t0, 0x0
      #MTC0(   t0, C0_Count ) 
      # by gb 
        
      jal     BspIntrHandler      # jump into the interrupt service routine 
      move    a0, k1              # k1 is the parameter 
                                  # [k1]   stores the values of SR 
                                  # [k1+4] stores the values of CAUSE 
      jal     OSIntExit
      nop

      la      t0, OSIntCtxSwFlag
      lw      t1, 0(t0)
      beq     t1, 1, _IntCtxSw
      nop

      move    sp, k0              # restore the interrupted task's sp pointer 
      RESTORE_REG_ERET()          # restore the interrupted task's context 

_IntCtxSw:

      sw      zero, 0(t0)         # Clear OSIntCtxSwFlag to zero 

      # OSPrioCur = OSPrioHighRdy 
      la      t0, OSPrioCur
      la      t1, OSPrioHighRdy
      lb      t1, 0(t1)
      sb      t1, 0(t0)
        
      # Get current task TCB address
      la      t0, OSTCBCur
      lw      t1, 0(t0)
      sw      k0, 0(t1)           # store the old sp in the interrupted tasks's TCB

      jal     OSTaskSwHook        # call Task Switch Hook 
      nop

      # Get highest priority task TCB address 
      la     t2, OSTCBHighRdy
      lw     t2, 0(t2)
      lw     sp, 0(t2)            # Load the new task's stack pointer into sp register

      # OSTCBCur = OSTCBHighRdy 
      sw t2,0(t0)                 # set new current task TCB address 

      RESTORE_REG_ERET()          # restore the new task's context 

END(uCOS_INTHandler)

/*
**********************************************************************************************************
*                                          OSCPUSaveSR
*
* Description: CRITICAL SECTION METHOD 3 FUNCTIONS. Disable/Enable interrupts by preserving the state of interrupts.  Generally speaking you
*              would store the state of the interrupt disable flag in the local variable 'cpu_sr' and then
*              disable interrupts.  'cpu_sr' is allocated in all of uC/OS-II's functions that need to 
*              disable interrupts.  You would restore the interrupt disable state by copying back 'cpu_sr'
*              into the CPU's status register.
*
*              OS_CPU_SR OSCPUSaveSR()
* Arguments  : none
*
* Returns    : OS_CPU_SR
*
*              OSCPURestoreSR(OS_CPU_SR cpu_sr)
* Arguments  : OS_CPU_SR
*
* Returns    : none
*
* Note(s)    : These functions are used in general like this,
*
*            void Task (void *data)
*            {
*                    #if OS_CRITICAL_METHOD == 3 // Allocate storage for CPU status register
*                          OS_CPU_SR  cpu_sr;
*                    #endif
*                         :
*                         :
*                    OS_ENTER_CRITICAL(); // cpu_sr = OSCPUSaveSR();
*                         :
*                         :
*                    OS_EXIT_CRITICAL();  // OSCPURestoreSR(cpu_sr);
*                         :
*                         :
*            }
**********************************************************************************************************
*/
LEAF(OSCPUSaveSR)

      MFC0(   v0, C0_STATUS )
      and     v1, v0, 0xfffffffe      # clear IEc to disable all the interrupts
      MTC0(   v1, C0_STATUS )
      jr      ra
      nop
      
END(OSCPUSaveSR)  

LEAF(OSCPURestoreSR)

      MTC0(   a0, C0_STATUS )
      jr      ra
      nop
      
END(OSCPURestoreSR)

/*
***************************************************************************
*                                       CP0_wGetSR
*
* Description: Reads CP0-status.
*
* Arguments  : none.
*
* Return     : CP0-status.
*
* Note(s)    : 
***************************************************************************
*/
LEAF(CP0_wGetSR)

      MFC0(   v0, C0_STATUS )
      jr      ra
      nop
      
END(CP0_wGetSR)  

/*
***************************************************************************
*                                GetCause
*
* Description: Reads CP0-cause register.
*
* Arguments  : none.
*
* Return     : CP0-cause.
*
* Note(s)    : 
***************************************************************************
*/
LEAF(GetCause)

      MFC0(   v0, C0_CAUSE )
      jr      ra
      nop
      
END(GetCause)  

/*  
***************************************************************************
*                                       GetGP
*
* Description: Reads the value of gp
*
* Arguments  : none.
*
* Return     : the value of gp
*
* Note(s)    : added on Oct. 15
***************************************************************************
*/
LEAF(GetGP)

      move    v0, gp
      jr      ra
      nop
      
END(GetGP) 

/*  
***************************************************************************
*                                       GetC0_ErrorEPC
*
* Description: Reads C0_ErrorEPC
*
* Arguments  : none.
*
* Return     : C0_ErrorEPC
*
***************************************************************************
*/
LEAF(GetC0_ErrorEPC)

      MFC0(   v0, C0_ErrorEPC )
      jr      ra
      nop
      
END(GetC0_ErrorEPC) 

/*  
***************************************************************************
*                                       GetC0_EPC
*
* Description: Reads C0_EPC
*
* Arguments  : none.
*
* Return     : C0_EPC
*
***************************************************************************
*/
LEAF(GetC0_EPC)

      MFC0(   v0, C0_EPC )
      jr      ra
      nop
      
END(GetC0_EPC)

/*  
***************************************************************************
*                                       GetC0_BadVaddr
*
* Description: Reads C0_BadVaddr
*
* Arguments  : none.
*
* Return     : C0_BadVaddr
*
***************************************************************************
*/
LEAF(GetC0_BadVaddr)

      MFC0(   v0, C0_BadVaddr )
      jr      ra
      nop
      
END(GetC0_BadVaddr)

/*  
***************************************************************************
*                                       GetC0_Context
*
* Description: Reads C0_Context
*
* Arguments  : none.
*
* Return     : C0_Context
*
***************************************************************************
*/
LEAF(GetC0_Context)

      MFC0(   v0, C0_Context )
      jr      ra
      nop
      
END(GetC0_Context)

/*  
***************************************************************************
*                                       GetC0_EntryHi
*
* Description: Reads C0_EntryHi
*
* Arguments  : none.
*
* Return     : C0_EntryHi
*
***************************************************************************
*/
LEAF(GetC0_EntryHi)

      MFC0(   v0, C0_EntryHi )
      jr      ra
      nop
      
END(GetC0_EntryHi)

/* get link information */
/*  
***************************************************************************
*                                       GetLink_start
*
* Description: Reads _start
*
* Arguments  : none.
*
* Return     : _start
*
***************************************************************************
*/
LEAF(GetLink_start)

      la      v0, _start
      nop
      jr      ra
      nop
      
END(GetLink_start)

/*  
***************************************************************************
*                                       GetLink_ftext
*
* Description: Reads _ftext
*
* Arguments  : none.
*
* Return     : _ftext
*
***************************************************************************
*/
LEAF(GetLink_ftext)

      la      v0, _ftext
      nop
      jr      ra
      nop
      
END(GetLink_ftext)

/*  
***************************************************************************
*                                       GetLink_ecode
*
* Description: Reads _ecode
*
* Arguments  : none.
*
* Return     : _ecode
*
***************************************************************************
*/
LEAF(GetLink_ecode)

      la      v0, _ecode
      nop
      jr      ra
      nop
      
END(GetLink_ecode)

/*  
***************************************************************************
*                                       GetLink_etext
*
* Description: Reads _etext
*
* Arguments  : none.
*
* Return     : _etext
*
***************************************************************************
*/
LEAF(GetLink_etext)

      la      v0, _etext
      nop
      jr      ra
      nop
      
END(GetLink_etext)

/*  
***************************************************************************
*                                       GetLink_fdata
*
* Description: Reads _fdata
*
* Arguments  : none.
*
* Return     : _fdata
*
***************************************************************************
*/
LEAF(GetLink_fdata)

      la      v0, _fdata
      nop
      jr      ra
      nop
      
END(GetLink_fdata)

/*  
***************************************************************************
*                                       GetLink_gp
*
* Description: Reads _gp
*
* Arguments  : none.
*
* Return     : _gp
*
***************************************************************************
*/
LEAF(GetLink_gp)

      la      v0, _gp
      nop
      jr      ra
      nop
      
END(GetLink_gp)

/*  
***************************************************************************
*                                       GetLink_edata
*
* Description: Reads _edata
*
* Arguments  : none.
*
* Return     : _edata
*
***************************************************************************
*/
LEAF(GetLink_edata)

      la      v0, _edata
      nop
      jr      ra
      nop
      
END(GetLink_edata)

/*  
***************************************************************************
*                                       GetLink_fbss
*
* Description: Reads _fbss
*
* Arguments  : none.
*
* Return     : _fbss
*
***************************************************************************
*/
LEAF(GetLink_fbss)

      la      v0, _fbss
      nop
      jr      ra
      nop
      
END(GetLink_fbss)

/*  
***************************************************************************
*                                       GetLink_freemem
*
* Description: Reads _freemem
*
* Arguments  : none.
*
* Return     : _freemem
*
***************************************************************************
*/
LEAF(GetLink_freemem)

      la      v0, _freemem
      nop
      jr      ra
      nop
      
END(GetLink_freemem)

/*  
***************************************************************************
*                                       GetLink_end
*
* Description: Reads _end
*
* Arguments  : none.
*
* Return     : _end
*
***************************************************************************
*/
LEAF(GetLink_end)

      la      v0, _end
      nop
      jr      ra
      nop
      
END(GetLink_end)

/*  
***************************************************************************
*                                       GetRa
*
* Description: read current pc value
*
* Arguments  : none.
*
* Return     : ra
*
***************************************************************************
*/
LEAF(GetRa)
	addi v0, ra, 0
	jr ra
	nop
	
END(GetRa)

      .set reorder
