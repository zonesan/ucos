#include <mips/asm.h>
#include <mips/udi.h>
#include <mips/regdef.h>

#include "mipsudi_inst.h"

      .set noat
      .set noreorder

LEAF( Lsf_lsp2 )
		addiu	sp,sp,-56
		sw	s6,40(sp)
		sll	s6,a2,0x10
		sra	s6,s6,0x10
		sw	s5,36(sp)
		sw	ra,52(sp)
		sw	s8,48(sp)
		sw	s7,44(sp)
		sw	s4,32(sp)
		sw	s3,28(sp)
		sw	s2,24(sp)
		sw	s1,20(sp)
		sw	s0,16(sp)
		blez	s6,Lsf_lsp2212
		move	s5,zero
		la	s8,slope_cos
		la	s7,table2
		move	s4,a1
		move	s3,a0
Lsf_lsp284:	lh	a0,0(s3)
		jal	mult
		li	a1,20861
		sra	t3,v0,0x8
		sll	t2,t3,0x10
		sra	s0,t2,0x10
		//and	s0,t3,0x00ff
		move	a0,s0
		li	a1,63
		jal	wg_sub
		andi	s2,v0,0xff
		slt	t0,zero,v0
		li	a3,63
		movz	a3,s0,t0
		sll	s1,a3,0x1
		addu	a2,s1,s8
		lh	a0,0(a2)
		jal	L_mult
		move	a1,s2
		sra	a0,v0,0xd
		jal	extract_l
		addu	s0,s1,s7
		lh	a0,0(s0)
		jal	wg_add
		move	a1,v0
		addiu	a1,s5,1
		sll	s5,a1,0x10
		sra	s5,s5,0x10
		slt	a0,s5,s6
		sh	v0,0(s4)
		addiu	s3,s3,2
		bnez	a0,Lsf_lsp284
		addiu	s4,s4,2
Lsf_lsp2212:	lw	ra,52(sp)
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
END( Lsf_lsp2 )
