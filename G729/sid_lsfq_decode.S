#include <mips/asm.h>
#include <mips/udi.h>
#include <mips/regdef.h>

#include "mipsudi_inst.h"


      .set noat
      .set noreorder

LEAF( sid_lsfq_decode )
		addiu	sp,sp,-112
		sw	s8,104(sp)
		sw	s7,100(sp)
		sw	s6,96(sp)
		sw	s5,92(sp)
		sw	s4,88(sp)
		sw	s3,84(sp)
		sw	s2,80(sp)
		sw	s1,76(sp)
		sw	ra,108(sp)
		sw	s0,72(sp)
		lh	s0,2(a0)
		sll	t5,s0,0x1
		la	t6,PtrTab_1
		addu	t4,t5,t6
		lh	t3,0(t4)
		sll	t2,t3,0x2
		addu	t1,t2,t3
		sll	a3,t1,0x2
		move	s3,a0
		addiu	s6,sp,48
		la	a0,lspcb1
		addu	a0,a3,a0
		move	s8,a1
		move	s7,a2
		move	a1,s6
		jal	Copy
		li	a2,10
		addiu	s5,sp,24
		la	s4,PtrTab_2
		la	s2,lspcb2
		move	s0,zero
		move	s1,s5
sid_lsfq_decode148:	lh	t4,4(s3)
		lh	a0,24(s1)
		sll	t3,t4,0x1
		addu	t2,t3,s4
		lh	t1,0(t2)
		sll	t0,t1,0x2
		addu	a3,t0,t1
		sll	v0,a3,0x1
		addu	v1,v0,s0
		sll	ra,v1,0x1
		addu	t9,ra,s2
		jal	wg_add
		lh	a1,0(t9)
		addiu	s0,s0,1
		slti	a1,s0,5
		sh	v0,24(s1)
		bnez	a1,sid_lsfq_decode148
		addiu	s1,s1,2
		
		li	s1,5
		addiu	s0,s5,10
sid_lsfq_decode236:	lh	a3,4(s3)
		lh	a0,24(s0)
		sll	v0,a3,0x1
		addu	v1,v0,s4
		lh	ra,32(v1)
		sll	t9,ra,0x2
		addu	t8,t9,ra
		sll	a2,t8,0x1
		addu	a1,a2,s1
		sll	t7,a1,0x1
		addu	t6,t7,s2
		jal	wg_add
		lh	a1,0(t6)
		addiu	s1,s1,1
		slti	a0,s1,10
		sh	v0,24(s0)
		bnez	a0,sid_lsfq_decode236
		addiu	s0,s0,2
		addiu	s1,s5,2
		li	s4,16384
		li	s5,10
		li	s2,1
sid_lsfq_decode324:	lh	t0,22(s1)
		
		lh	t1,24(s1)
		li	a2,-16384
		
		li	a3,16384
		mpymvrr	acc0,sx0,sy0,shsat,x0,gprt0,y0,gprs4		
		macmvrr	acc0,sx0,sy0,shsat,x0,gpra2,y0,gprt1		
		macmvrr	acc0,sx0,sy0,shsat,x0,gprs5,y0,gpra3
		mvp	gprv0,udi_mode
		mvp	gprv0,udi_mode
		mvp	gprv0,udi_mode
		mvp	gpra0,udi_mac0acc0lo
		sra	v0,a0,0x10
		move	s0,v0
		blez	v0,sid_lsfq_decode416
		move	a1,v0
		jal	wg_sub
		lh	a0,22(s1)
		lh	a0,24(s1)
		move	a1,s0
		jal	wg_add
		sh	v0,22(s1)
		sh	v0,24(s1)
sid_lsfq_decode416:	addiu	s2,s2,1
		slti	t4,s2,10
		bnez	t4,sid_lsfq_decode324
		addiu	s1,s1,2
		
		lh	t5,0(s3)
		sll	s1,t5,0x2
		addu	a0,s1,t5
		sll	t3,a0,0x4
		la	t2,noise_fg
		la	t0,noise_fg_sum
		sll	s0,a0,0x2
		addu	s3,s0,t0
		addu	a2,t3,t2
		move	a3,s7
		move	a0,s6
		addiu	a1,sp,24
		jal	Lsp_prev_compose
		sw	s3,16(sp)
		move	a1,s7
		jal	Lsp_prev_update
		move	a0,s6
		jal	Lsp_stability
		addiu	a0,sp,24
		move	a1,s8
		addiu	a0,sp,24
		jal	Lsf_lsp2
		li	a2,10
		lw	ra,108(sp)
		lw	s8,104(sp)
		lw	s7,100(sp)
		lw	s6,96(sp)
		lw	s5,92(sp)
		lw	s4,88(sp)
		lw	s3,84(sp)
		lw	s2,80(sp)
		lw	s1,76(sp)
		lw	s0,72(sp)
		jr	ra
		addiu	sp,sp,112
END( sid_lsfq_decode )


