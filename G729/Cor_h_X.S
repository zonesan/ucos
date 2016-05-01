/*
***************************************************************************
*                  Copyright (c) 2006 vBridge Microsystem, Inc.  
*                       Unpublished & Not for Publication
*                               All Rights Reserved                   
*   
* File        : Cor_h_X.S                     
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

LEAF( Cor_h_X )
		addiu	sp,sp,-216
		sw	s8,208(sp)
		addiu	s8,sp,16
		sw	s7,204(sp)
		sw	s6,200(sp)
		sw	s5,196(sp)
		sw	s4,192(sp)
		sw	s3,188(sp)
		sw	ra,212(sp)
		sw	s2,184(sp)
		sw	s1,180(sp)
		sw	s0,176(sp)
		move	s7,a0
		move	s6,a1
		sw	a2,224(sp)
		move	s5,zero
		move	s3,zero
		li	v1,1
		move	s4,s8
Cor_h_X76:	move	a3,zero
		beqz	v1,Cor_h_X148
		move	s2,s3
		move	t0,zero
		mvp	udi_mac0acc0lo,gprt0
		mvp	udi_mac0acc0hi,gprt0
		sll	v0,s3,0x1
		addu	s1,v0,s6
		move	s0,s7
Cor_h_X100:	lh	a1,0(s1)
		lh	a2,0(s0)
		macmvrr	acc0,sx0,sy0,shsat,x0,gpra1,y0,gpra2
		addiu	s2,s2,1
		slti	v1,s2,40
		addiu	s1,s1,2
		bnez	v1,Cor_h_X100
		addiu	s0,s0,2
		
		mvp	gprt0,udi_mode
		mvp	gprt0,udi_mode
		mvp	gprt0,udi_mode
		mvp	gpra3,udi_mac0acc0lo
		
Cor_h_X148:	move	a0,a3
		jal	L_abs
		sw	a3,0(s4)
		move	a0,v0
		move	a1,s5
		jal	L_sub
		move	s0,v0
		addiu	s3,s3,1
		move	s1,zero
		slt	a1,s1,v0
		slti	v1,s3,40
		movn	s5,s0,a1
		bnez	v1,Cor_h_X76
		addiu	s4,s4,4
		jal	norm_l
		move	a0,s5
		move	a0,v0
		li	a1,16
		jal	wg_sub
		move	s4,v0
		slt	a3,s1,v0
		li	a1,16
		movz	a1,s4,a3
		jal	wg_sub
		li	a0,18
		lw	s1,224(sp)
		move	s3,v0
		move	s0,s8
		move	s2,zero
Cor_h_X272:	lw	t3,0(s0)
		addiu	s0,s0,4
		srav	a0,t3,s3		
		sll	v0,a0,0x10
		sra	v0,v0,0x10		
		addiu	s2,s2,1
		slti	t0,s2,40
		sh	v0,0(s1)
		bnez	t0,Cor_h_X272
		addiu	s1,s1,2
		lw	ra,212(sp)
		lw	s8,208(sp)
		lw	s7,204(sp)
		lw	s6,200(sp)
		lw	s5,196(sp)
		lw	s4,192(sp)
		lw	s3,188(sp)
		lw	s2,184(sp)
		lw	s1,180(sp)
		lw	s0,176(sp)
		jr	ra
		addiu	sp,sp,216
END( Cor_h_X )

