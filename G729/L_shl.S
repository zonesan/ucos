/*
***************************************************************************
*                  Copyright (c) 2006 vBridge Microsystem, Inc.  
*                       Unpublished & Not for Publication
*                               All Rights Reserved                   
*   
* File        : L_shl_1.S                     
*                                                                         
* Description:    
*             
* Date        : Jul 14, 2006                                                           
***************************************************************************
*/

#include <mips/asm.h>
#include <mips/udi.h>
#include <mips/regdef.h>

#include "mipsudi_inst.h"


      .set noat
      .set noreorder
LEAF( L_shl )
		sll	a1,a1,0x10
		addiu	sp,sp,-24
		sra	a1,a1,0x10
		sw	ra,16(sp)
		blez	a1,L_shl148
		move	a2,a0
		lui	a0,0x3fff
		ori	t0,a0,0xffff
		lui	a3,0xc000
L_shl36:		addiu	t3,a1,-1
		sll	t2,t3,0x10
		slt	t1,t0,a2
		slt	v0,a2,a3
		sll	a2,a2,0x1
		sra	a1,t2,0x10
		bnez	t1,L_shl92
		move	a0,a2
		bnez	v0,L_shl124
		lw	ra,16(sp)
		bgtz	a1,L_shl36
		move	v0,a0
		jr	ra
		addiu	sp,sp,24
L_shl92:		lui	a3,0x7fff
		ori	a0,a3,0xffff
		lw	ra,16(sp)
		li	a2,1
		move	v0,a0
		sw	a2,Overflow
		jr	ra
		addiu	sp,sp,24
		
L_shl124:		lui	a0,0x8000
		li	t0,1
		move	v0,a0
		sw	t0,Overflow
		jr	ra
		addiu	sp,sp,24
		
L_shl148:		negu	v1,a1
		sll	v0,v1,0x10
		jal	L_shr
		sra	a1,v0,0x10
		move	a0,v0
		lw	ra,16(sp)
		move	v0,a0
		jr	ra
		addiu	sp,sp,24
END( L_shl )

