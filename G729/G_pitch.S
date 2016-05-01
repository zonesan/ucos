#include <mips/asm.h>
#include <mips/udi.h>
#include <mips/regdef.h>

#include "mipsudi_inst.h"


      .set noat
      .set noreorder

LEAF( G_pitch )
		addiu	sp,sp,-136
		sw	s4,112(sp)
		sll	s4,a3,0x10
		sra	s4,s4,0x10
		sw	s5,116(sp)
		slt	s5,zero,s4
		sw	s8,128(sp)
		sw	a2,144(sp)
		sw	ra,132(sp)
		sw	s7,124(sp)
		sw	s6,120(sp)
		sw	s3,108(sp)
		sw	s2,104(sp)
		sw	s1,100(sp)
		sw	s0,96(sp)
		sw	a0,136(sp)
		move	s8,a1
		beqz	s5,G_pitch124
		move	a2,zero
		move	a0,a1
		addiu	a1,sp,16
G_pitch84:	addiu	a2,a2,1
		lh	a3,0(a0)		
		sra	v1,a3,0x2
		slt	v0,a2,s4
		sh	v1,0(a1)
		addiu	a0,a0,2
		bnez	v0,G_pitch84
		addiu	a1,a1,2
		
G_pitch124:	
		sw	zero,Overflow
		li	s2,1
		beqz	s5,G_pitch188
		move	s1,zero
		move	s0,s8
G_pitch144:	lh	a1,0(s0)
		move	a0,s2
		jal	L_mac
		move	a2,a1
		addiu	a1,s1,1
		sll	s1,a1,0x10
		sra	s1,s1,0x10
		slt	a0,s1,s4
		move	s2,v0
		bnez	a0,G_pitch144
		addiu	s0,s0,2		
G_pitch188:	
		lw	t1,Overflow
		beqz	t1,G_pitch808
		nop
		li	s2,1
		beqz	s5,G_pitch260
		move	s1,zero
		addiu	s0,sp,16
G_pitch216:	lh	a1,0(s0)
		move	a0,s2
		jal	L_mac
		move	a2,a1
		addiu	t4,s1,1
		sll	t3,t4,0x10
		sra	s1,t3,0x10
		slt	t2,s1,s4
		move	s2,v0
		bnez	t2,G_pitch216
		addiu	s0,s0,2
		
G_pitch260:	jal	norm_l
		move	a0,s2
		move	a1,v0
		move	a0,s2
		jal	L_shl
		move	s0,v0
		jal	round16
		move	a0,v0
		move	a0,s0
		li	a1,4
		jal	wg_sub
		move	s7,v0
		move	s6,v0
		
G_pitch312:	//sw	zero,-32560(gp)
		//la	t1,Overflow
		sw	zero,Overflow
		
		move	s3,zero
		beqz	s5,G_pitch384
		move	s2,zero
		lw	s0,136(sp)
		move	s1,s8
G_pitch336:	lh	a1,0(s0)
		lh	a2,0(s1)
		jal	L_mac
		move	a0,s3
		addiu	t6,s2,1
		sll	s2,t6,0x10
		sra	s2,s2,0x10
		slt	t5,s2,s4
		move	s3,v0
		addiu	s0,s0,2
		bnez	t5,G_pitch336
		addiu	s1,s1,2
		
G_pitch384:	//lw	t7,-32560(gp)
		//la	t7,Overflow
		lw	t7,Overflow
		
		beqz	t7,G_pitch768
		nop
		move	s3,zero
		beqz	s5,G_pitch464
		move	s2,zero
		lw	s0,136(sp)
		addiu	s1,sp,16
G_pitch416:	lh	a1,0(s0)
		lh	a2,0(s1)
		jal	L_mac
		move	a0,s3
		addiu	t9,s2,1
		sll	t8,t9,0x10
		sra	s2,t8,0x10
		slt	s5,s2,s4
		move	s3,v0
		addiu	s0,s0,2
		bnez	s5,G_pitch416
		addiu	s1,s1,2
		
G_pitch464:	jal	norm_l
		move	a0,s3
		move	a1,v0
		move	a0,s3
		jal	L_shl
		move	s8,v0
		jal	round16
		move	a0,v0
		move	a0,s8
		li	a1,2
		jal	wg_sub
		move	s1,v0
		move	s2,v0
G_pitch516:	lw	ra,144(sp)
		li	a0,15
		sh	s7,0(ra)
		jal	wg_sub
		move	a1,s6
		lw	s3,144(sp)
		li	a0,15
		sh	v0,2(s3)
		sh	s1,4(s3)
		jal	wg_sub
		move	a1,s2
		lw	a0,144(sp)
		blez	s1,G_pitch704
		sh	v0,6(a0)
		move	a0,s1
		jal	wg_shr
		li	a1,1
		move	a0,v0
		jal	div_s
		move	a1,s7
		move	a0,s2
		move	a1,s6
		jal	wg_sub
		move	s1,v0
		move	a1,v0
		jal	wg_shr
		move	a0,s1
		move	a0,v0
		li	a1,19661
		jal	wg_sub
		move	s7,v0
		slt	s4,zero,v0
		li	v1,19661
		movz	v1,s7,s4
		lw	ra,132(sp)
		lw	s8,128(sp)
		lw	s7,124(sp)
		lw	s6,120(sp)
		lw	s5,116(sp)
		lw	s4,112(sp)
		lw	s3,108(sp)
		lw	s2,104(sp)
		lw	s1,100(sp)
		lw	s0,96(sp)
		move	v0,v1
		jr	ra
		addiu	sp,sp,136
G_pitch704:	li	s6,-15
		move	v1,zero
		sh	s6,6(a0)
		lw	ra,132(sp)
		lw	s8,128(sp)
		lw	s7,124(sp)
		lw	s6,120(sp)
		lw	s5,116(sp)
		lw	s4,112(sp)
		lw	s3,108(sp)
		lw	s2,104(sp)
		lw	s1,100(sp)
		lw	s0,96(sp)
		move	v0,v1
		jr	ra
		addiu	sp,sp,136
G_pitch768:	jal	norm_l
		move	a0,s3
		move	a1,v0
		move	a0,s3
		jal	L_shl
		move	s2,v0
		jal	round16
		move	a0,v0
		b	G_pitch516
		move	s1,v0
G_pitch808:	jal	norm_l
		move	a0,s2
		move	a1,v0
		move	a0,s2
		jal	L_shl
		move	s6,v0
		jal	round16
		move	a0,v0
		b	G_pitch312
		move	s7,v0
END( G_pitch )

