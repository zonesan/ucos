/*
***************************************************************************
*                  Copyright (c) 2006 vBridge Microsystem, Inc.  
*                       Unpublished & Not for Publication
*                               All Rights Reserved                   
*   
* File        : Pred_lt_3.S                     
*                                                                         
* Description:    
*             
* Date        : May 17, 2006                                                           
***************************************************************************
*/

#include <mips/asm.h>
#include <mips/udi.h>
#include <mips/regdef.h>

#include "mipsudi_inst.h"

      .set noat
      .set noreorder

LEAF( Pred_lt_3 )
		addiu	sp,sp,-64
		sll	t1,a1,0x10
		sw	s0,24(sp)
		sra	t0,t1,0x10
		move	s0,a0
		sll	a2,a2,0x10
		sll	a0,a3,0x10
		sll	a1,t0,0x1
		sra	a3,a0,0x10
		li	v1,32767
		sra	a2,a2,0x10
		li	v0,-32768
		sw	s6,48(sp)
		sw	ra,60(sp)
		sw	s8,56(sp)
		sw	s7,52(sp)
		sw	s5,44(sp)
		sw	s4,40(sp)
		sw	s3,36(sp)
		sw	s2,32(sp)
		sw	s1,28(sp)
		sw	a3,16(sp)
		subu	s6,s0,a1
		beq	a2,v0,Pred_lt_3120
		sw	v1,20(sp)
		negu	t3,a2
		sll	t2,t3,0x10
		sra	v0,t2,0x10
		bltz	v0,Pred_lt_3368
		sw	v0,20(sp)
Pred_lt_3120:	lw	t4,16(sp)
		blez	t4,Pred_lt_3320
		move	s7,zero
		lw	s1,20(sp)
		la	t5,inter_3l
		sll	t6,s1,0x1
		addu	s8,t6,t5
		move	s5,s0
Pred_lt_3156:	lw	a1,20(sp)
		jal	wg_sub
		li	a0,3
		move	s0,s6
		sll	s4,v0,0x1
		la	s2,inter_3l
		addiu	s6,s6,2
		addu	s3,s4,s2
		move	a0,zero
		mvp	udi_mac0acc0lo,gpra0
		mvp	udi_mac0acc0hi,gpra0
		move	s2,s0
		move	s4,zero
		move	s1,s8
		move	s0,s6
Pred_lt_3212:	lh	a1,0(s2)
		lh	a2,0(s1)
		macmvrr	acc0,sx0,sy0,shsat,x0,gpra1,y0,gpra2
		addiu	s2,s2,-2
		lh	a1,0(s0)
		lh	a2,0(s3)
		macmvrr	acc0,sx0,sy0,shsat,x0,gpra1,y0,gpra2
		addiu	s4,s4,1
		slti	t8,s4,10
		addiu	s0,s0,2
		addiu	s1,s1,6
		bnez	t8,Pred_lt_3212
		addiu	s3,s3,6
		
		mvp	gprt0,udi_mode
		mvp	gprt0,udi_mode
		mvp	gprt0,udi_mode		
		satacc	mac0, acc0, round_enable, sat_enable, satp16, 16
		mvp	gprt0,udi_mode
		mvp	gprt0,udi_mode
		mvp	gprt0,udi_mode
		mvp	gprv0,udi_mac0acc0lo
		addiu	s7,s7,1
		lw	v1,16(sp)
		slt	a1,s7,v1
		sh	v0,0(s5)
		bnez	a1,Pred_lt_3156
		addiu	s5,s5,2
Pred_lt_3320:	lw	ra,60(sp)
		lw	s8,56(sp)
		lw	s7,52(sp)
		lw	s6,48(sp)
		lw	s5,44(sp)
		lw	s4,40(sp)
		lw	s3,36(sp)
		lw	s2,32(sp)
		lw	s1,28(sp)
		lw	s0,24(sp)
		jr	ra
		addiu	sp,sp,64
Pred_lt_3368:	move	a0,v0
		jal	wg_add
		li	a1,3
		addiu	s6,s6,-2
		b	Pred_lt_3120
		sw	v0,20(sp)
END( Pred_lt_3 )

