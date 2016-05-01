#include <mips/asm.h>
#include <mips/udi.h>
#include <mips/regdef.h>

#include "mipsudi_inst.h"


      .set noat
      .set noreorder

LEAF( Lsp_pre_select )
		addiu	sp,sp,-56
		lui	v0,0x7fff
		sw	s8,48(sp)
		sw	s7,44(sp)
		sw	s6,40(sp)
		sw	s5,36(sp)
		sw	s4,32(sp)
		sw	ra,52(sp)
		sw	s3,28(sp)
		sw	s2,24(sp)
		sw	s1,20(sp)
		sw	s0,16(sp)
		move	s8,a2
		move	s7,a0
		move	s6,a1
		sh	zero,0(a2)
		ori	s5,v0,0xffff
		move	s4,zero
		sll	a1,s4,0x2
Lsp_pre_select76:	addu	a0,a1,s4
		sll	v1,a0,0x2
		addu	s1,v1,s6
		move	s3,zero
		mvp	udi_mac0acc0lo,gprs3
		mvp	udi_mac0acc0hi,gprs3
		move	s2,zero
		move	s0,s7
Lsp_pre_select100:	lh	a0,0(s0)
		lh	a1,0(s1)
		jal	wg_sub
		addiu	s0,s0,2
		macmvrr	acc0,sx0,sy0,shsat,x0,gprv0,y0,gprv0
		addiu	s2,s2,1
		slti	a2,s2,10
		bnez	a2,Lsp_pre_select100
		addiu	s1,s1,2
		
		mvp		gprt0,udi_mode	
		mvp		gprt0,udi_mode
		mvp		gprt0,udi_mode
		mvp		gprs3,udi_mac0acc0lo
		
		move	a0,s3
		jal	L_sub
		move	a1,s5
		bltzl	v0,Lsp_pre_select252
		move	s5,s3
Lsp_pre_select180:	addiu	t1,s4,1
		sll	s4,t1,0x10
		sra	s4,s4,0x10
		slti	t0,s4,128
		bnezl	t0,Lsp_pre_select76
		sll	a1,s4,0x2
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
Lsp_pre_select252:	b	Lsp_pre_select180
		sh	s4,0(s8)
END( Lsp_pre_select )

