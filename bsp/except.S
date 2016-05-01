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



#include "sysdefs.h"
#include "cp0regdef.h"
#include "mips_regdef.h"

#include "2risc_memory_map.h"

#include "chip_cfg.h"

/*
***************************************************************************
*                             Local Functions
***************************************************************************
*/

      .set    noat
      .set    noreorder

SLEAF( FirstRamVectors )

first_ram_int_vector_0:
      mfc0    k0, $15, 1
      andi     k0, k0, 0x1
      bnez    k0, risc1_0                         # risc1
      nop
      
      la      k0, uCOS_INTHandler       # risc0
      jr      k0
      nop

risc1_0:
      li      k0, VECTOR_EXC000
      jr      k0
      nop
      
first_ram_int_vector_80:
      mfc0    k0, $15, 1
      andi     k0, k0, 0x1
      bnez    k0, risc1_080                         # risc1
      nop
      
      la      k0, uCOS_INTHandler       # risc0
      jr      k0
      nop

risc1_080:
      li      k0, VECTOR_EXC080
      jr      k0
      nop
       
first_ram_int_vector_100:
      mfc0    k0, $15, 1
      andi     k0, k0, 0x1
      bnez    k0, risc1_100                       # risc1
      nop
      
      la      k0, uCOS_INTHandler       # risc0
      jr      k0
      nop

risc1_100:
      li      k0, VECTOR_EXC100
      jr      k0
      nop
      
first_ram_int_vector_180:
      mfc0    k0, $15, 1
      andi     k0, k0, 0x1
      bnez    k0, risc1_180                       # risc1
      nop
      
      la      k0, uCOS_INTHandler       # risc0
      jr      k0
      nop

risc1_180:
      li      k0, VECTOR_EXC180
      jr      k0
      nop
      
first_ram_int_vector_200:
      mfc0    k0, $15, 1
      andi     k0, k0, 0x1
      bnez    k0, risc1_200                       # risc1
      nop
      
      la      k0, uCOS_INTHandler       # risc0
      jr      k0
      nop

risc1_200:
      li      k0, VECTOR_EXC200
      jr      k0
      nop
      
first_ram_int_vector_280:
      mfc0    k0, $15, 1
      andi     k0, k0, 0x1
      bnez    k0, risc1_280                       # risc1
      nop
      
      la      k0, uCOS_INTHandler       # risc0
      jr      k0
      nop

risc1_280:
      li      k0, VECTOR_EXC280
      jr      k0
      nop
      
first_ram_int_vector_300:
      mfc0    k0, $15, 1
      andi     k0, k0, 0x1
      bnez    k0, risc1_300                       # risc1
      nop
      
      la      k0, uCOS_INTHandler       # risc0
      jr      k0
      nop

risc1_300:
      li      k0, VECTOR_EXC300
      jr      k0
      nop
      
first_ram_int_vector_380:
      mfc0    k0, $15, 1
      andi     k0, k0, 0x1
      bnez    k0, risc1_380                       # risc1
      nop
      
      la      k0, uCOS_INTHandler       # risc0
      jr      k0
      nop

risc1_380:
      li      k0, VECTOR_EXC380
      jr      k0
      nop
      
first_ram_int_vector_400:
      mfc0    k0, $15, 1
      andi     k0, k0, 0x1
      bnez    k0, risc1_400                       # risc1
      nop
      
      la      k0, uCOS_INTHandler       # risc0
      jr      k0
      nop

risc1_400:
      li      k0, VECTOR_EXC400
      jr      k0
      nop
      
END( FirstRamVectors )      

/*
***************************************************************************
*                              Gloabl Functions
***************************************************************************
*/
LEAF( Risc1Entry )     

      # risc1 jump from 0xbfc00000 to BOOTLOADER_B_ADDR
      li      k0, KSEG0BASE
      #li      k1, BOOTLOADER_B_ADDR
      li      k1, RISC1_ENTRY
      or      k1, k0                
      jr      k1
      nop
        
END( Risc1Entry )


// for simulation the linux platform
LEAF( Risc1IntrEntry )     

      la      k0, uCOS_INTHandler           
      jr      k0
      nop
        
END( Risc1IntrEntry )

/*
***************************************************************************
*                          ExceptInstallIntrInRam
*
* Description: This routine is used to install the INT handler in RAM.
*
* Arguments  : none.
*
* Return     : none.
*
* Note(s)    : 
***************************************************************************
*/

LEAF( ExceptInstallIntrInRam )

      # Install RAM exception vectors


#define  ADDR                      a0  /* also used as arg to sys_cache..() */
#define  STATUS_INT                t3
#define  RA                        t4

      move    RA, ra

      # Clear the bit 0 of C0_Status to disable interrupts
      #MFC0(   t1, C0_Status )
      #and     STATUS_INT, t1, 0x1
      #xor     t1, STATUS_INT        
      #MTC0(   t1, C0_Status )

      # judge risc0 or risc1
      lui     k0, 0xa800      
      ori     k0, k0, 8
      lw      k0, 0(k0)
      andi    k0, k0, 0x1
      bnez    k0, clear_bev                         # risc1
      nop

      # install first-ram-vectors. 
      la      t2, first_ram_int_vector_0
      li      ADDR, FIRST_VECTOR_EXC000 
      jal     setup_first_ram_vectors
      nop  

      la      t2, first_ram_int_vector_80
      li      ADDR, FIRST_VECTOR_EXC080 
      jal     setup_first_ram_vectors
      nop  
      
      la      t2, first_ram_int_vector_100
      li      ADDR, FIRST_VECTOR_EXC100 
      jal     setup_first_ram_vectors
      nop  
      
      la      t2, first_ram_int_vector_180
      li      ADDR, FIRST_VECTOR_EXC180 
      jal     setup_first_ram_vectors
      nop  
      
      la      t2, first_ram_int_vector_200
      li      ADDR, FIRST_VECTOR_EXC200 
      jal     setup_first_ram_vectors
      nop              
      
      la      t2, first_ram_int_vector_280
      li      ADDR, FIRST_VECTOR_EXC280 
      jal     setup_first_ram_vectors
      nop         
            
      la      t2, first_ram_int_vector_300
      li      ADDR, FIRST_VECTOR_EXC300 
      jal     setup_first_ram_vectors
      nop               
      
      la      t2, first_ram_int_vector_380
      li      ADDR, FIRST_VECTOR_EXC380 
      jal     setup_first_ram_vectors
      nop         
            
      la      t2, first_ram_int_vector_400
      li      ADDR, FIRST_VECTOR_EXC400 
      jal     setup_first_ram_vectors
      nop               

clear_bev:                                    
      # Clear BEV so that ram handlers are used
      MFC0(   a0, C0_Status)
      li      t2, ~(0x1<<22)
      and     a0, t2
      MTC0(   a0, C0_Status)

      # Restore the bit 0 of C0_Status to enable the interrupt
      #or      a0, STATUS_INT
      #MTC0(   a0, C0_Status )

      move    ra, RA
      
      jr      ra
      nop

setup_first_ram_vectors:
      
      # use physical addr to avoid flush cache!
      lui     t5, 0xa000
      or      ADDR, ADDR, t5       
      
      # copy interrupt entry routine
      lw      t1, 0(t2)
      sw      t1, 0(ADDR)
      lw      t1, 4(t2)
      sw      t1, 4(ADDR)
      lw      t1, 8(t2)
      sw      t1, 8(ADDR)
      lw      t1, 12(t2)
      sw      t1, 12(ADDR)
      
      lw      t1, 16(t2)
      sw      t1, 16(ADDR)
      lw      t1, 20(t2)
      sw      t1, 20(ADDR)
      lw      t1, 24(t2)
      sw      t1, 24(ADDR)
      lw      t1, 28(t2)
      sw      t1, 28(ADDR)

      lw      t1, 32(t2)
      sw      t1, 32(ADDR)
      lw      t1, 36(t2)
      sw      t1, 36(ADDR)
      lw      t1, 40(t2)
      sw      t1, 40(ADDR)
      lw      t1, 44(t2)
      sw      t1, 44(ADDR)
      
      move    t2, ra

      //jal     DCACHE_vFlushAddr
      //nop
      //jal     ICACHE_vInvalidateAddr
      //move    a1, zero /* No need to flush pipeline */

      jr      t2
      nop

END( ExceptInstallIntrInRam )

/*
***************************************************************************
*                          ExceptEnCount_Compare
*
* Description: enable the Count/Compare and its interrupt, 
*              enable the whole interrupt(set bit0 of Status).
*
* Arguments  : none.
*
* Return     : none.
*
* Note(s)    : 
***************************************************************************
*/
LEAF( ExceptEnCount_Compare )

      # Clear C0_Count and Set C0_Compare
      MTC0(   zero, C0_Count)    
      MTC0(   a0, C0_Compare)

      # Clear bit27 of CAUSE register to enable the Count. This is not needed for au1550.
      MFC0(   t0, C0_CAUSE)
      lui     AT, 0xf7ff
      ori     AT, AT, 0xffff
      and     t0, t0, AT
      MTC0(   t0, C0_CAUSE)      

      # Enable the Count/Compare interrupt
      MFC0(   t0, C0_STATUS)
      or      t0, 0x8001         
      MTC0(   t0, C0_STATUS)
      
      jr      ra
      nop
      
END( ExceptEnCount_Compare )

/*
***************************************************************************
*                          ExceptDisCount
*
* Description: disable the Count.
*
* Arguments  : none.
*
* Return     : none.
*
* Note(s)    : 
***************************************************************************
*/
LEAF( ExceptDisCount )

      # Set bit27 of CAUSE register to disable the Count. 
      MFC0(   t0, C0_CAUSE)
      lui     AT, 0x800 
      or      t0, t0, AT
      MTC0(   t0, C0_CAUSE)      
      
      jr      ra
      nop
      
END( ExceptDisCount )

/*
***************************************************************************
*                          ExceptClrCount
*
* Description: This routine is used to enable the Count/Compare interrupts.
*
* Arguments  : none.
*
* Return     : none.
*
* Note(s)    : 
***************************************************************************
*/
LEAF( ExceptClrCount )

      # Set C0_Compare again to clear the Count/Compare interrupt 
      MTC0(   zero, C0_Count)     
      MFC0(   t0, C0_Compare)     
      MTC0(   t0, C0_Compare)     
      jr      ra
      nop
      
END( ExceptClrCount )

/*  
***************************************************************************
*                                GetCount
*
* Description: Reads the value of count
*
* Arguments  : none.
*
* Return     : the value of Count
*
* Note(s)    : added on Oct. 15
***************************************************************************
*/
LEAF(GetCount)

      mfc0   v0, C0_Count
      nop
      nop
      jr      ra
      nop      
      
END(GetCount) 

/*
***************************************************************************
*                            MipsIntrReqEn
*
* Description: Enable specific interrupt: set IM[x] bit in CP0-status.
*
* Arguments  : intr_source  can be:
*                             IP7 -  Hardware interrupt 5
*                             IP6 -  Hardware interrupt 4
*                             IP5 -  Hardware interrupt 3
*                             IP4 -  Hardware interrupt 2
*                             IP3 -  Hardware interrupt 1
*                             IP2 -  Hardware interrupt 0
*                             IP1 -  Request software interrupt 1
*                             IP0 -  Request software interrupt 0
*
* Return     : none.
*
* Note(s)    : 
***************************************************************************
*/
LEAF(MipsIntrReqEn)

      MFC0(   v0, C0_STATUS )
      or      v0, a0
      MTC0(   v0, C0_STATUS )
      j       ra
      nop

END(MipsIntrReqEn)

/*
***************************************************************************
*                            MipsIntrReqDis
*
* Description: Disable specific interrupt: set IM[x] bit in CP0-status.
*
* Arguments  : intr_source  can be:
*                             IP7 -  Hardware interrupt 5
*                             IP6 -  Hardware interrupt 4
*                             IP5 -  Hardware interrupt 3
*                             IP4 -  Hardware interrupt 2
*                             IP3 -  Hardware interrupt 1
*                             IP2 -  Hardware interrupt 0
*                             IP1 -  Request software interrupt 1
*                             IP0 -  Request software interrupt 0
*
* Return     : none.
*
* Note(s)    : The function is not ok.
***************************************************************************
*/
LEAF(MipsIntrReqDis)

      MFC0(   v0, C0_STATUS )
      not    a0, a0
      and   v0, a0
      MTC0(   v0, C0_STATUS )
      j       ra
      nop

END(MipsIntrReqDis)

/*
***************************************************************************
*                              MipsIntrEn
*
* Description: Enable specific interrupt: set IM[x] bit in CP0-status.
*
* Arguments  : intr_source  can be:
*
*
* Return     : none.
*
* Note(s)    : 
***************************************************************************
*/
LEAF(MipsIntrEn)

      MFC0(   v0, C0_STATUS )
      or      v0, 0x1
      MTC0(   v0, C0_STATUS )
      j       ra
      nop

END(MipsIntrEn)

/*
***************************************************************************
*                             MipsIntrDis
*
* Description: Enable specific interrupt: set IM[x] bit in CP0-status.
*
* Arguments  : 
*
* Return     : none.
*
* Note(s)    : 
***************************************************************************
*/
LEAF(MipsIntrDis)
      
      MFC0(   t0, C0_STATUS )
      and     t1, t0, 0x1
      xor     t0, t1
      MTC0(   t0, C0_STATUS )
      j       ra
      nop

END(MipsIntrDis)

/*
***************************************************************************
*                             MipsCacheEn
*
* Description: Enable the cache.
*
* Arguments  : 
*
* Return     : none.
*
* Note(s)    : 
***************************************************************************
*/
LEAF(MipsCacheEn)
      


END(MipsCacheEn)

      .set    at
      .set    reorder

