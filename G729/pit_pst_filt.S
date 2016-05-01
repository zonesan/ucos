#include <mips/asm.h>
#include <mips/udi.h>
#include <mips/regdef.h>

#include "mipsudi_inst.h"

      .set noat
      .set noreorder
      
LEAF( pit_pst_filt )
		addiu	sp,sp,-72
		sll	t2,a2,0x10
		sw	s6,56(sp)
		sll	t1,a3,0x10
		lh	s6,88(sp)
		sra	t0,t2,0x10
		sra	a3,t1,0x10
		sll	a2,t0,0x1
		slt	v1,zero,s6
		slt	v0,a3,t0
		sw	s8,64(sp)
		sw	s7,60(sp)
		sw	s5,52(sp)
		sw	s4,48(sp)
		sw	ra,68(sp)
		sw	s3,44(sp)
		sw	s2,40(sp)
		sw	s1,36(sp)
		sw	s0,32(sp)
		move	s4,t0
		sw	a3,16(sp)
		move	s8,a1
		sw	a0,72(sp)
		subu	s5,a1,a2
		lui	s7,0x8000
		sw	t0,20(sp)
		bnez	v0,pit_pst_filt236
		sw	v1,24(sp)
pit_pst_filt112:	lw	v1,24(sp)
		move	s3,zero
		move	s1,s8
		move	s0,s5
		mvp	udi_mac0acc0lo,gprs3
		mvp	udi_mac0acc0hi,gprs3
		beqz	v1,pit_pst_filt184
		move	s2,zero
pit_pst_filt136:	lh	a1,0(s1)
		lh	a2,0(s0)
		macmvrr	acc0,sx0,sy0,shsat,x0,gpra1,y0,gpra2
		addiu	s2,s2,1
		slt	v1,s2,s6
		addiu	s1,s1,2
		bnez	v1,pit_pst_filt136
		addiu	s0,s0,2
		mvp	gprs3,udi_mode
		mvp	gprs3,udi_mac0acc0lo
		mvp	gprs3,udi_mac0acc0lo
		mvp	gprs3,udi_mac0acc0lo
pit_pst_filt184:	move	a0,s3
		jal	L_sub
		move	a1,s7
		blez	v0,pit_pst_filt212
		addiu	t4,s4,1
		move	s7,s3
		sw	s4,20(sp)
pit_pst_filt212:	sll	s4,t4,0x10
		lw	t3,16(sp)
		sra	s4,s4,0x10
		slt	a1,t3,s4
		beqz	a1,pit_pst_filt112
		addiu	s5,s5,-2
pit_pst_filt236:	lw	t7,20(sp)
		lw	t5,24(sp)
		sll	t6,t7,0x1
		move	s1,s8
		subu	s0,s8,t6
		li	s3,1
		li	s4,1
		bnez	t5,pit_pst_filt628
		move	s2,zero
pit_pst_filt272:	slti	v1,s7,0
pit_pst_filt276:	move	s8,zero
		movz	s8,s7,v1
		slt	v0,s8,s3
		move	a0,s3
		movz	a0,s8,v0
		slt	ra,a0,s4
		movn	a0,s4,ra
		jal	norm_l
		nop
		move	a1,v0
		move	a0,s8
		jal	L_shl
		move	s7,v0
		jal	round16
		move	a0,v0
		move	a0,s3
		move	a1,s7
		jal	L_shl
		move	s2,v0
		jal	round16
		move	a0,v0
		move	a1,s7
		move	a0,s4
		jal	L_shl
		move	s3,v0
		jal	round16
		move	a0,v0
		move	s0,v0
		move	a0,s2
		jal	L_mult
		move	a1,s2
		move	a0,s3
		move	a1,s0
		jal	L_mult
		move	s1,v0
		move	a0,v0
		jal	L_shr
		li	a1,1
		move	a1,v0
		jal	L_sub
		move	a0,s1
		bltz	v0,pit_pst_filt772
		move	a0,s2
		jal	wg_sub
		move	a1,s3
		li	s5,21845
		blez	v0,pit_pst_filt876
		li	s7,10923
pit_pst_filt468:	lw	t2,24(sp)
		beqz	t2,pit_pst_filt580
		move	s4,zero
		lw	s2,20(sp)
		lw	a0,72(sp)
		sll	a1,s2,0x1
		lw	s3,92(sp)
		subu	s2,a0,a1
		move	s1,a0
pit_pst_filt504:	lh	a1,0(s1)
		lh	t1,0(s2)
		mpymvrr	acc1,sx0,sy0,shnone,x0,gprs5,y0,gpra1
		satacc	mac0, acc1, round_disable, sat_disable, satp32, 12
		satacc	mac0, acc1, round_disable, sat_enable, satp16, 3		
		mpymvrr	acc0,sx0,sy0,shnone,x0,gprs7,y0,gprt1
		satacc	mac0, acc0, round_disable, sat_disable, satp32, 12
		satacc	mac0, acc0, round_disable, sat_enable, satp16, 3
		mvp	gpra0,udi_mac0acc1lo
		mvp	gprt0,udi_mac0acc1hi
		mvp	udi_mac1acc0lo,gpra0
		mvp	udi_mac1acc0hi,gprt0
		addacc	acc0,acc0
		mvp	gprv0,udi_mode
		mvp	gprv0,udi_mac0acc0lo
		mvp	gprv0,udi_mac0acc0lo
		mvp	gprv0,udi_mac0acc0lo
		addiu	s4,s4,1
		slt	t3,s4,s6
		sh	v0,0(s3)
		addiu	s1,s1,2
		addiu	s2,s2,2
		bnez	t3,pit_pst_filt504
		addiu	s3,s3,2
pit_pst_filt580:	lw	ra,68(sp)
		lw	s8,64(sp)
		lw	s7,60(sp)
		lw	s6,56(sp)
		lw	s5,52(sp)
		lw	s4,48(sp)
		lw	s3,44(sp)
		lw	s2,40(sp)
		lw	s1,36(sp)
		lw	s0,32(sp)
		jr	ra
		addiu	sp,sp,72
pit_pst_filt628:	lh	a1,0(s0)
		move	a0,s3
		jal	L_mac
		move	a2,a1
		lh	a1,0(s1)
		move	a0,s4
		move	a2,a1
		jal	L_mac
		move	s3,v0
		addiu	t9,s2,1
		sll	t8,t9,0x10
		sra	s2,t8,0x10
		slt	s5,s2,s6
		move	s4,v0
		addiu	s0,s0,2
		beqz	s5,pit_pst_filt272
		addiu	s1,s1,2
		lh	a1,0(s0)
		move	a0,s3
		jal	L_mac
		move	a2,a1
		lh	a1,0(s1)
		move	a0,s4
		move	a2,a1
		jal	L_mac
		move	s3,v0
		addiu	t9,s2,1
		sll	t8,t9,0x10
		sra	s2,t8,0x10
		slt	s5,s2,s6
		move	s4,v0
		addiu	s0,s0,2
		bnez	s5,pit_pst_filt628
		addiu	s1,s1,2
		b	pit_pst_filt276
		slti	v1,s7,0
pit_pst_filt772:	lw	s3,24(sp)
		beqz	s3,pit_pst_filt580
		move	a2,zero
		lw	a0,72(sp)
		lw	a1,92(sp)
pit_pst_filt792:	addiu	t1,a2,1
		sll	a2,t1,0x10
		lhu	t0,0(a0)
		sra	a2,a2,0x10
		slt	a3,a2,s6
		sh	t0,0(a1)
		addiu	a0,a0,2
		bnez	a3,pit_pst_filt792
		addiu	a1,a1,2
		lw	ra,68(sp)
		lw	s8,64(sp)
		lw	s7,60(sp)
		lw	s6,56(sp)
		lw	s5,52(sp)
		lw	s4,48(sp)
		lw	s3,44(sp)
		lw	s2,40(sp)
		lw	s1,36(sp)
		lw	s0,32(sp)
		jr	ra
		addiu	sp,sp,72
pit_pst_filt876:	move	a0,s2
		jal	mult
		li	a1,16384
		sra	s0,v0,1
		sra	a1,s3,1
		jal	wg_add
		move	a0,s0
		li	s5,32767
		blez	v0,pit_pst_filt468
		move	s7,zero
		move	a0,s0
		jal	div_s
		move	a1,v0
		move	a1,v0
		li	a0,32767
		jal	wg_sub
		move	s7,v0
		b	pit_pst_filt468
		move	s5,v0

END( pit_pst_filt )

