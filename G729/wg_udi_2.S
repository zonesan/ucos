#include <mips/asm.h>
#include <mips/udi.h>
#include <mips/regdef.h>

#include "mipsudi_inst.h"

      .set noat
      .set noreorder

LEAF( wg_udi_2 )
		addiu	sp,sp,-56
		lw	v1,72(sp)
		sw	s7,44(sp)
		sw	s6,40(sp)
		sll	s6,a1,0x10
		lh	s7,76(sp)
		sw	s5,36(sp)
		sw	s4,32(sp)
		sw	s3,28(sp)
		sw	s2,24(sp)
		sw	s1,20(sp)
		sw	ra,48(sp)
		sw	s0,16(sp)
		sra	s6,s6,0x10
		la	s5,mem_w0
		addiu	s3,v1,60
		addiu	s2,a3,60
		addiu	s1,a2,60
		li	s4,30
wg_udi_280:	lh	a0,0(s1)
		lh	a1,0(s2)
		mpymvrr	acc0,sx0,sy0,shsat,x0,gpra0,y0,gprs6
		mpymvrr	acc0,sx0,sy0,shsat,x0,gpra1,y0,gprs7
		mvp	gprt0,udi_mode
		mvp	gprt0,udi_mode
		mvp	gpra0,udi_mac0acc0lo
		mvp	gprs0,udi_mac0acc0lo
		jal	L_shl
		li	a1,1
		move	a0,s0
		sra	s0,v0,0x10		
		jal	L_shl
		li	a1,2
		sra	a1,v0,0x10
		jal	wg_add
		move	a0,s0
		lh	a0,0(s3)
		jal	wg_sub
		move	a1,v0
		addiu	s4,s4,1
		slti	a0,s4,40
		sh	v0,0(s5)
		addiu	s1,s1,2
		addiu	s2,s2,2
		addiu	s3,s3,2
		bnez	a0,wg_udi_280
		addiu	s5,s5,2
		lw	ra,48(sp)
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
END( wg_udi_2 )

