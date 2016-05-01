/*
***************************************************************************
*                Copyright (c) 2005 vBridge Microsystem, Inc.  
*                       Unpublished & Not for Publication
*                             All Rights Reserved                   
*   
* File:         start up code 
*                                                                         
* Description :                     
*
* Date:         Sep 20, 2005                                                           
***************************************************************************
*/



#define  _SYS_STKSIZE    0x800

#define CACHE_INIT


#include  "cp0regdef.h"	
#include  "mips_regdef.h"



/*
***************************************************************************
*                           Gloabl Functions
***************************************************************************
*/
      .text
      .set noreorder

/*
***************************************************************************
*                                       _start
*
* Description: This routine initializes CPU, sp and gp registers, 
*              clears bss section and starts the application C code.
*
* Arguments  : none
*
* Return     : none.
*
* Note(s)    : _start is the entry of the whole program. 
***************************************************************************
*/
      .globl  _start
      .ent    _start
	
_start:	

      mtc0    zero, C0_Cause
      nop
      nop
      nop
      nop

      b       reset

      .org 0x100                   # leave space to store something for the future
                           
reset:

      # Establish Status Register  
      # d31-28 = 0000 Disable CU3-CU0 controll access
      # d27    = 0    Turn off Reduce Power bit
      # d26    = 0    reserved
      # d25    = 0    Turn off reverse endian memory reference
      # d24-23 = 00   reserved
      # d22    = 0    (BEV) To use normal exception vectors in kseg0
      # d21    = 0    reserved for 4Kp (TLB shutdown)
      # d20-19 = 00   Clear SR, NMI reset cause bits
      # d18-16 = 000  reserved
      # d15-08 = 0[8] Clear Interrupt Masks
      # d07-05 = 000  reserved
      # d04    = 0    (User Mode) In kernel mode
      # d03    = 0    reserved
      # d02    = 0    Set ERL to normal level
      # d01    = 0    Set EXL to normal level
      # d00    = 0    Disable Interrupts          
      li      t0, 0x0      
      mtc0    t0, C0_Status
      nop
      nop
      nop
      nop
      
      # Disable watch exceptions?
      mtc0    t0, C0_WatchLo
      nop
      nop
      nop
      nop     
      mtc0    t0, C0_WatchHi
      nop
      nop
      nop
      nop

#ifdef CACHE_INIT

	# Determine how big the I$ is
/*
 ************************************************************************
 *         C O N F I G 1   R E G I S T E R   ( 1 6, SELECT 1 )          *
 ************************************************************************
 * 	
 *  3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
 *  1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |M|  MMU Size |  IS |  IL |  IA |  DS |  DL |  DA |Rsvd |W|C|E|F| Config1
 * | |           |     |     |     |     |     |     |     |R|A|P|P|
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */
	mfc0	t2, C0_CONFIG, 1		# .word 0x400a8001

	# Isolate I$ Line Size
	sll	t3, t2, 10
	srl	t3, 29

	# Skip ahead if No I$
	beq	t3, zero, 10f
	nop

	li	t6, 2
	sllv	t3, t6, t3		# Now have true I$ line size in bytes

	sll	t4, t2, 7
	srl	t4, 29
	li	t6, 64
	sllv	t4, t6, t4		# I$ Sets per way

	sll	t5, t2, 13
	srl	t5, 29			# I$ Assoc (-1)
	add	t5, 1
	mul	t4, t4, t5		# Total number of sets

	lui	t6, 0x8000		# Get a KSeg0 address for cacheops

	# Clear TagLo/TagHi registers
	mtc0	zero, C0_TAGLO
	mtc0	zero, C0_TAGHI

	move	t7, t4	

	# Index Store Tag Cache Op
	# Will invalidate the tag entry, clear the lock bit, and clear the LRF bit
1:	cache	0x8, 0(t6)
	add	t7, -1			# Decrement set counter

	bne	t7, zero, 1b
	add	t6, t3		# Get next line address

	# Now go through and invalidate the D$
	# Now that the I$ has been flushed, the rest of the code can be
	# moved to kseg0 and run from the cache to go faster
10:	
	
	# Isolate D$ Line Size
	sll	t3, t2, 19
	srl	t3, 29

	# Skip ahead if No D$
	beq	t3, zero, 10f
	nop

	li	t6, 2
	sllv	t3, t6, t3		# Now have true D$ line size in bytes

	sll	t4, t2, 16
	srl	t4, 29
	li	t6, 64
	sllv	t4, t6, t4		# D$ Sets per way

	sll	t5, t2, 22
	srl	t5, 29			# D$ Assoc (-1)
	add	t5, 1

	mul	t4, t4, t5		# Get total number of sets
	
	lui	t6, 0x8000		# Get a KSeg0 address for cacheops

	# Clear TagLo/TagHi registers
	mtc0	zero, C0_TAGLO
	mtc0	zero, C0_TAGHI

	move	t7, t4	

	# Index Store Tag Cache Op
	# Will invalidate the tag entry, clear the lock bit, and clear the LRF bit
1:	cache	0x9, 0(t6)
	add	t7, -1			# Decrement set counter

	bne	t7, zero, 1b
	add	t6, t3		# Get next line address

#endif
	# Now both caches have been flushed
10:
      
      # Set KSeg0(0x80000000 ~ 0xa0000000) to cacheable/uncacheable
      # Config.K0
      mfc0    t2, C0_Config
      nop
      li      t3, 0x7
      nop
      not     t3
      and     t2, t3
      or      t2, 0x3               # 0x3 cacheable(write back); 0x1 cacheable(write through); 0x7: uncacheable
      mtc0    t2, C0_Config

      # Establish Cause
      # Bit 27: disable counting of Count register
      # Bit 23:  1: use the special interrupt vector (0x200)
      #          0: use the general exception vector (0x180)
      li      t1, 0x08000000
      mtc0    t1, C0_Cause
      nop
      nop
      nop
      nop
      
      # Clear Count register
      mtc0    zero, C0_Count
      nop
      nop
      nop
      nop

      # Set compare to -1 to delay 1st count=compare
      # Also, clears timer interrupt
      li    t2, -1
      mtc0  t2, C0_Compare
        
      # At this point the processor has been initialized
      # Then prepare C function environment  
      # Setup sp (top of memory) and gp 
      la    gp, _gp                  # _gp and _freemem are generated from Link.xn
      la    sp, _freemem
      li    t0, _SYS_STKSIZE 
      addu  sp, t0
          
      # Clear bss
      la    t0, _fbss	             # First address 
      la    t1, _end	             # Last  address
1:
      sw    zero, 0(t0)
      bne   t0, t1, 1b
      addiu t0, 4

      # Get ready to jump to Main
      la    t0, Main
      jal   t0
      nop

      .set    reorder

      .end    _start
