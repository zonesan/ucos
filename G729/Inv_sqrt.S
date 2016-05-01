#include <mips/asm.h>
#include <mips/udi.h>
#include <mips/regdef.h>

#include "mipsudi_inst.h"


      .set noat
      .set noreorder

LEAF( Inv_sqrt )

		addiu	sp,sp,-32
		sw	s1,20(sp)
		sw	ra,28(sp)
		sw	s2,24(sp)
		sw	s0,16(sp)
		blez	a0,Inv_sqrt224
		move	s1,a0
		jal	norm_l
		nop
		move	s0,v0
		move	a0,s1
		jal	L_shl
		move	a1,v0
		move	a1,s0
		li	a0,30
		jal	sub
		move	s2,v0
		sra	t0,v0,0x1
		andi	t1,v0,0x1
		sll	a3,t0,0x10
		sra	s0,s2,0x1
		movn	s0,s2,t1
		sra	a0,a3,0x10
		jal	add
		li	a1,1
		sra	a0,s0,0x9
		jal	extract_h
		move	s2,v0
		move	s1,v0
		jal	extract_l
		sra	a0,s0,0xa
		move	a0,s1
		li	a1,16
		jal	sub
		andi	s1,v0,0x7fff
		//lui	a2,0x8000
		//addiu	a1,a2,996
		la	a1,tabsqr
		sll	a0,v0,0x1
		addu	v0,a0,a1
		lh	v1,0(v0)
		lh	a1,2(v0)
		move	a0,v1
		jal	sub
		sll	s0,v1,0x10
		move	a1,v0
		move	a0,s0
		jal	L_msu
		move	a2,s1
		move	a0,v0
		move	a1,s2
		lw	ra,28(sp)
		lw	s2,24(sp)
		lw	s1,20(sp)
		lw	s0,16(sp)
		j	L_shr
		addiu	sp,sp,32
Inv_sqrt224:	lui	t2,0x3fff
		lw	ra,28(sp)
		lw	s2,24(sp)
		lw	s1,20(sp)
		lw	s0,16(sp)
		ori	v0,t2,0xffff
		jr	ra
		addiu	sp,sp,32

END( Inv_sqrt )
