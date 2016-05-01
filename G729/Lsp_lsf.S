#include <mips/asm.h>
#include <mips/udi.h>
#include <mips/regdef.h>

#include "mipsudi_inst.h"

#define	udi_nop		mvp gprt0,udi_mac0acc0lo	

      .set noat
      .set noreorder
      
LEAF( Lsp_lsf )		
                addiu	sp,sp,-56
	        addiu	a2,a2,-1
		sw	s3,28(sp)
		sll	s3,a2,0x10
		sra	s3,s3,0x10
		sw	s5,36(sp)
		sw	ra,48(sp)
		sw	s7,44(sp)
		sw	s6,40(sp)
		sw	s4,32(sp)
		sw	s2,24(sp)
		sw	s1,20(sp)
		sw	s0,16(sp)
		bltz	s3,Lsp_lsf240
		li	s5,63
		sll	v0,s3,0x1
		addu	s1,v0,a0
		la      s6,table
		la      s7,slope
		addu	s2,v0,a1
		b	Lsp_lsf192
		li	s4,126
		
Lsp_lsf96:	lh	a0,0(s1)
		lh	a1,0(s0)
		subu	a0,a0,a1
		addiu	s1,s1,-2
		addu	t2,s4,s7
		lh	v0,0(t2)
		mpymvrr acc0,sx0,sy0,shsat,x0,gpra0,y0,gprv0
		udi_nop
		udi_nop
		udi_nop
		mvp     gprv0,udi_mac0acc0lo
		move	a0,v0
		jal	L_shl
		li	a1,3
		jal	round16
		move	a0,v0
		sll	t1,s5,0x8
		sll	t0,t1,0x10
		sra	a1,t0,0x10
		jal	add
		move	a0,v0
		addiu	a3,s3,-1
		sll	a1,a3,0x10
		sra	s3,a1,0x10
		sh	v0,0(s2)
		bltz	s3,Lsp_lsf240
		addiu	s2,s2,-2

Lsp_lsf192:	addu	s0,s4,s6
		lh	a1,0(s1)
		lh	a0,0(s0)
		bge	a0,a1,Lsp_lsf96
		//addu    s0,s4,s6
		subu    s5,s5,1
		b	Lsp_lsf192
		sll	s4,s5,0x1
Lsp_lsf240:	lw	ra,48(sp)
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
END( Lsp_lsf )
