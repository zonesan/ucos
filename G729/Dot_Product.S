/*
***************************************************************************
*                  Copyright (c) 2005 vBridge Microsystem, Inc.  
*                       Unpublished & Not for Publication
*                               All Rights Reserved                   
*   
* File        : Dot_Product.S                     
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

LEAF( Dot_Product )
		addiu	sp,sp,-40
		sw	s3,28(sp)
		sll	s3,a2,0x10
		sra	s3,s3,0x10
		sw	s2,24(sp)
		move	v0,a0
		sw	ra,32(sp)
		sw	s1,20(sp)
		sw	s0,16(sp)
		move	a0,zero
		blez	s3,Dot_Product100
		move	s2,zero
		move	t0,zero
		mvp	udi_mac0acc0lo,gprt0
		mvp	udi_mac0acc0hi,gprt0
		move	s1,a1
		move	s0,v0
Dot_Product56:	lh	a1,0(s0)
		lh	a2,0(s1)
		macmvrr	acc0,sx0,sy0,shsat,x0,gpra1,y0,gpra2
		addiu	s0,s0,2
		addiu	s2,s2,1
		slt	v1,s2,s3
		bnez	v1,Dot_Product56
		addiu	s1,s1,2
		
		mvp	gprt0,udi_mode
		mvp	gprt0,udi_mode
		mvp	gprt0,udi_mode
		mvp	gpra0,udi_mac0acc0lo
Dot_Product100:	lw	ra,32(sp)
		lw	s3,28(sp)
		lw	s2,24(sp)
		lw	s1,20(sp)
		lw	s0,16(sp)
		move	v0,a0
		jr	ra
		addiu	sp,sp,40
END( Dot_Product )

