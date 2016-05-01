#include <mips/asm.h>
#include <mips/udi.h>
#include <mips/regdef.h>

#include "mipsudi_inst.h"


      .set noat
      .set noreorder

LEAF( agc )
		addiu	sp,sp,-144
		sw	s4,120(sp)
		sll	s4,a2,0x10
		sra	s4,s4,0x10
		sw	s5,124(sp)
		slt	s5,zero,s4
		sw	s8,136(sp)
		sw	s6,128(sp)
		sw	s3,116(sp)
		sw	s2,112(sp)
		sw	ra,140(sp)
		sw	s7,132(sp)
		sw	s1,108(sp)
		sw	s0,104(sp)
		move	s6,a0
		move	s8,a1
		move	s3,zero
		beqz	s5,agc152
		move	s2,zero
		
		move	s0,a1
		mvp	udi_mac0acc0lo,gprs2
		mvp	udi_mac0acc0hi,gprs2
		addiu	s1,sp,16
agc84:		lh	a1,0(s0)
		sra	a3,a1,0x2
		sh	a3,0(s1)
		macmvrr	acc0,sx0,sy0,shsat,x0,gpra3,y0,gpra3
		addiu	s2,s2,1
		slt	v1,s2,s4
		addiu	s1,s1,2
		bnez	v1,agc84
		addiu	s0,s0,2
		mvp	gprs3,udi_mode
		mvp	gprs3,udi_mac0acc0lo
		mvp	gprs3,udi_mac0acc0lo
		mvp	gprs3,udi_mac0acc0lo
		bnez	s3,agc204
		nop
agc152:		lw	ra,140(sp)
		lw	s8,136(sp)
		lw	s7,132(sp)
		lw	s6,128(sp)
		lw	s5,124(sp)
		lw	s4,120(sp)
		lw	s3,116(sp)
		lw	s2,112(sp)
		lw	s1,108(sp)
		lw	s0,104(sp)
		sh	zero,past_gain
		jr	ra
		addiu	sp,sp,144
		
agc204:		jal	norm_l
		move	a0,s3
		move	a0,v0
		jal	wg_sub
		li	a1,1
		move	a1,v0
		move	a0,s3
		jal	L_shl
		move	s7,v0
		jal	round16
		move	a0,v0
		sw	v0,96(sp)
		move	s3,zero
		beqz	s5,agc344
		move	s2,zero
		move	s0,s6
		
		mvp	udi_mac0acc0lo,gprs2
		mvp	udi_mac0acc0hi,gprs2
		addiu	s1,sp,16
agc272:		lh	t2,0(s0)
		sra	t1,t2,0x2
		sh	t1,0(s1)
		macmvrr	acc0,sx0,sy0,shsat,x0,gprt1,y0,gprt1
		addiu	s2,s2,1
		slt	a0,s2,s4
		addiu	s1,s1,2
		bnez	a0,agc272
		addiu	s0,s0,2
		
		mvp	gprs3,udi_mode
		mvp	gprs3,udi_mac0acc0lo
		mvp	gprs3,udi_mac0acc0lo
		mvp	gprs3,udi_mac0acc0lo
		
		bnez	s3,agc500
		nop
		move	s3,zero
agc344:		
		lh	s1,past_gain
		beqz	s5,agc448
		move	s2,zero
		move	s0,s8
agc360:		move	a0,s1
		jal	mult
		li	a1,29491
		move	a0,v0
		jal	wg_add
		move	a1,s3
		lh	a0,0(s0)
		move	a1,v0
		jal	L_mult
		move	s1,v0
		move	a0,v0
		jal	L_shl
		li	a1,3
		jal	extract_h
		move	a0,v0
		addiu	t5,s2,1
		sll	t4,t5,0x10
		sra	s2,t4,0x10
		slt	t3,s2,s4
		sh	v0,0(s0)
		bnez	t3,agc360
		addiu	s0,s0,2
agc448:		//sh	s1,-32456(gp)
		sh	s1,past_gain
		lw	ra,140(sp)
		lw	s8,136(sp)
		lw	s7,132(sp)
		lw	s6,128(sp)
		lw	s5,124(sp)
		lw	s4,120(sp)
		lw	s3,116(sp)
		lw	s2,112(sp)
		lw	s1,108(sp)
		lw	s0,104(sp)
		jr	ra
		addiu	sp,sp,144
agc500:		jal	norm_l
		move	a0,s3
		move	a1,v0
		move	a0,s3
		jal	L_shl
		move	s6,v0
		jal	round16
		move	a0,v0
		move	a0,s7
		move	a1,s6
		jal	wg_sub
		move	s1,v0
		lw	a0,96(sp)
		move	a1,s1
		jal	div_s
		move	s0,v0
		move	a0,v0
		jal	L_shl
		li	a1,7
		move	a0,v0
		jal	L_shr
		move	a1,s0
		jal	Inv_sqrt
		move	a0,v0
		move	a0,v0
		jal	L_shl
		li	a1,9
		jal	round16
		move	a0,v0
		move	a0,v0
		jal	mult
		li	a1,3276
		b	agc344
		move	s3,v0

END( agc )

