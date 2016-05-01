#include <mips/asm.h>
#include <mips/udi.h>
#include <mips/regdef.h>

#include "mipsudi_inst.h"


      .set noat
      .set noreorder

LEAF( Lsp_prev_compose )
		addiu	sp,sp,-56
		sw	s5,36(sp)
		lw	s5,72(sp)
		sw	s8,48(sp)
		sw	s7,44(sp)
		sw	s6,40(sp)
		sw	s4,32(sp)
		sw	s3,28(sp)
		sw	ra,52(sp)
		sw	s2,24(sp)
		sw	s1,20(sp)
		sw	s0,16(sp)
		sw	a2,64(sp)
		move	s8,a3
		move	s4,a1
		move	s3,a0
		move	s7,zero
		move	s6,zero
Lsp_prev_compose72:	lh	a0,0(s3)
		lh	a1,0(s5)
		move	s2,zero
		mpymvrr	acc0,sx0,sy0,shsat,x0,gpra0,y0,gpra1
		move	a0,v0
		lw	v0,64(sp)
		addu	s0,s6,s8
		addu	s1,s6,v0
Lsp_prev_compose104:	lh	a1,0(s0)
		lh	a2,0(s1)
		addiu	s0,s0,20
		macmvrr	acc0,sx0,sy0,shsat,x0,gpra1,y0,gpra2
		addiu	s2,s2,1
		slti	v1,s2,4
		move	a0,v0
		bnez	v1,Lsp_prev_compose104
		addiu	s1,s1,20
		mvp	gpra0,udi_mode
		mvp	gpra0,udi_mode
		mvp	gpra0,udi_mode
		mvp	gpra0,udi_mac0acc0lo		
		sra	v0,a0,0x10
		addiu	s6,s6,2
		addiu	s7,s7,1
		slti	a0,s7,10
		sh	v0,0(s4)
		addiu	s3,s3,2
		addiu	s5,s5,2
		bnez	a0,Lsp_prev_compose72
		addiu	s4,s4,2
		lw	ra,52(sp)
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
END( Lsp_prev_compose )

