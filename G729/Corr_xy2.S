#include <mips/asm.h>
#include <mips/udi.h>
#include <mips/regdef.h>

#include "mipsudi_inst.h"

      .set noat
      .set noreorder

LEAF( Corr_xy2 )
		addiu	sp,sp,-136
		sw	s6,120(sp)
		sw	s4,112(sp)
		lw	s6,152(sp)
		addiu	s4,sp,16
		sw	s7,124(sp)
		sw	s5,116(sp)
		sw	s3,108(sp)
		move	s7,a1
		move	s3,a0
		sw	ra,128(sp)
		sw	s2,104(sp)
		sw	s1,100(sp)
		sw	s0,96(sp)
		move	s5,a3
		move	a0,zero
		move	a1,s4
Corr_xy268:	addiu	a0,a0,1
		lh	a3,0(a2)
		sra	v1,a3,0x3
		slti	v0,a0,40
		sh	v1,0(a1)
		addiu	a2,a2,2
		bnez	v0,Corr_xy268
		addiu	a1,a1,2
		
		li	s2,1		
		move	s1,zero
		mvp	udi_mac0acc0lo,gprs2
		mvp	udi_mac0acc0hi,gprs0
		move	s0,s4	
		
Corr_xy2120:	lh	a1,0(s0)
		macmvrr	acc0,sx0,sy0,shsat,x0,gpra1,y0,gpra1
		addiu	s1,s1,1
		slti	a1,s1,40
		bnez	a1,Corr_xy2120
		addiu	s0,s0,2
		mvp	gprv0,udi_mac0acc0lo
		mvp	gprv0,udi_mac0acc0lo
		mvp	gprv0,udi_mac0acc0lo
		mvp	gprs2,udi_mac0acc0lo

		jal	norm_l
		move	a0,s2
		move	a1,v0
		move	a0,s2
		jal	L_shl
		move	s1,v0
		jal	round16
		move	a0,v0
		move	a0,s1
		li	a1,3
		jal	wg_add
		move	s0,v0
		move	s1,s3
		sh	s0,4(s5)
		li	s3,1
		sh	v0,4(s6)
		move	s2,zero
		
		mvp	udi_mac0acc0lo,gprs3
		mvp	udi_mac0acc0hi,gprs2
		move	s0,s4
Corr_xy2236:	lh	a1,0(s1)
		lh	a2,0(s0)
		macmvrr	acc0,sx0,sy0,shsat,x0,gpra1,y0,gpra2
		addiu	s2,s2,1
		slti	t1,s2,40
		addiu	s1,s1,2
		bnez	t1,Corr_xy2236
		addiu	s0,s0,2
		
		mvp	gprv0,udi_mac0acc0lo
		mvp	gprv0,udi_mac0acc0lo
		mvp	gprv0,udi_mac0acc0lo
		mvp	gprs3,udi_mac0acc0lo
		
		jal	norm_l
		move	a0,s3
		move	a1,v0
		move	a0,s3
		jal	L_shl
		move	s0,v0
		jal	round16
		move	a0,v0
		move	s1,v0
		move	a0,s0
		jal	wg_add
		li	a1,-6
		move	a0,s1
		jal	negate
		move	s3,v0
		sh	v0,6(s5)
		move	a0,s3
		jal	wg_sub
		li	a1,1
		sh	v0,6(s6)
		move	s1,s4
		move	s0,s7
		li	s3,1
		move	s2,zero
		
		mvp	udi_mac0acc0lo,gprs3
		mvp	udi_mac0acc0hi,gprs2
Corr_xy2380:	lh	a1,0(s0)
		lh	a2,0(s1)
		macmvrr	acc0,sx0,sy0,shsat,x0,gpra1,y0,gpra2
		addiu	s2,s2,1
		slti	t3,s2,40
		addiu	s0,s0,2
		bnez	t3,Corr_xy2380
		addiu	s1,s1,2
		
		mvp	gprv0,udi_mac0acc0lo
		mvp	gprv0,udi_mac0acc0lo
		mvp	gprv0,udi_mac0acc0lo
		mvp	gprs3,udi_mac0acc0lo
		
  		jal	norm_l
		move	a0,s3
		move	a1,v0
		move	a0,s3
		jal	L_shl
		move	s7,v0
		jal	round16
		move	a0,v0
		move	a0,s7
		li	a1,-6
		jal	wg_add
		move	s4,v0
		move	a0,v0
		sh	s4,8(s5)
		jal	wg_sub
		li	a1,1
		sh	v0,8(s6)
		lw	ra,128(sp)
		lw	s7,124(sp)
		lw	s6,120(sp)
		lw	s5,116(sp)
		lw	s4,112(sp)
		lw	s3,108(sp)
		lw	s2,104(sp)
		lw	s1,100(sp)
		lw	s0,96(sp)
		jr	ra
		addiu	sp,sp,136
END( Corr_xy2 )

