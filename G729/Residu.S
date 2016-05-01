/*
***************************************************************************
*                  Copyright (c) 2006 vBridge Microsystem, Inc.  
*                       Unpublished & Not for Publication
*                               All Rights Reserved                   
*   
* File        : Pitch_ol_fast.S                     
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

LEAF( Residu )
		addiu	sp,sp,-56
		sw	s7,44(sp)
		sll	s7,a3,0x10
		sra	s7,s7,0x10
		sw	s8,48(sp)
		sw	s6,40(sp)
		sw	ra,52(sp)
		sw	s5,36(sp)
		sw	s4,32(sp)
		sw	s3,28(sp)
		sw	s2,24(sp)
		sw	s1,20(sp)
		sw	s0,16(sp)
		move	s8,a0
		blez	s7,Residu200
		move	s6,zero
		move	s4,a1
		move	s5,a2
		move	s3,a1
Residu76:	lh	a0,0(s4)
		lh	a1,0(s8)
		li	s2,1
		mpymvrr	acc0,sx0,sy0,shsat,x0,gpra0,y0,gpra1
		addiu	s1,s3,-2
		addiu	s0,s8,2
Residu104:	lh	a1,0(s0)
		lh	a2,0(s1)
		macmvrr	acc0,sx0,sy0,shsat,x0,gpra1,y0,gpra2
		addiu	s0,s0,2
		addiu	s2,s2,1
		slti	v1,s2,11
		bnez	v1,Residu104
		addiu	s1,s1,-2
		
		mvp	gprt0,udi_mode
		mvp	gprt0,udi_mode
		mvp	gprt0,udi_mode
		mvp	gpra0,udi_mac0acc0lo
		jal	L_shl
		li	a1,3
		jal	round16
		move	a0,v0
		addiu	s6,s6,1
		sh	v0,0(s5)
		addiu	s3,s3,2
		slt	a0,s6,s7		
		addiu	s4,s4,2
		bnez	a0,Residu76
		addiu	s5,s5,2
Residu200:	lw	ra,52(sp)
		lw	s8,48(sp)
		lw	s7,44(sp)
		lw	s6,40(sp)
		lw	s5,36(sp)
		lw	s4,32(sp)
		lw	s3,28(sp)
		lw	s2,24(sp)
		lw	s1,20(sp)
		lw	s0,16(sp)
		jr	ra
		addiu	sp,sp,56
END( Residu )

