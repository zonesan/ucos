#include <mips/asm.h>
#include <mips/udi.h>
#include <mips/regdef.h>

#include "mipsudi_inst.h"


      .set noat
      .set noreorder

LEAF( Lsp_select_2 )
		addiu	sp,sp,-80
		sw	s3,52(sp)
		sw	s2,48(sp)
		sw	s1,44(sp)
		sw	s0,40(sp)
		sw	ra,76(sp)
		sw	s8,72(sp)
		sw	s7,68(sp)
		sw	s6,64(sp)
		sw	s5,60(sp)
		sw	s4,56(sp)
		sw	a2,88(sp)
		sw	a3,92(sp)
		addiu	s1,a1,10
		addiu	s0,a0,10
		li	s3,5
		addiu	s2,sp,26
Lsp_select_268:	lh	a0,0(s0)
		lh	a1,0(s1)
		jal	wg_sub
		addiu	s0,s0,2
		addiu	s3,s3,1
		//sll	s3,v1,0x10
		//sra	s3,s3,0x10
		slti	a0,s3,10
		sh	v0,0(s2)
		addiu	s1,s1,2
		bnez	a0,Lsp_select_268
		addiu	s2,s2,2
		lw	a1,96(sp)
		lui	v0,0x7fff
		sh	zero,0(a1)
		ori	s8,v0,0xffff
		move	s6,zero
		move	s7,zero
Lsp_select_2140:	lw	t2,92(sp)
		sll	t1,s7,0x1
		lw	a3,88(sp)
		addu	t0,t1,t2
		addiu	a2,sp,16
		addiu	s3,t0,10
		move	s5,zero
		mvp	udi_mac0acc1lo,gprs5
		mvp	udi_mac0acc1hi,gprs5
		li	s4,5
		addiu	s2,a3,10
		addiu	s1,a2,10
Lsp_select_2180:	lh	a0,0(s1)
		lh	a1,0(s3)
		jal	wg_sub
		addiu	s1,s1,2
		lh	a0,0(s2)			
		
		mpymvrr	acc0,sx0,sy0,shnone,x0,gpra0,y0,gprv0
		satacc	mac0, acc0, round_disable, sat_disable, satp32, 12
		satacc	mac0, acc0, round_disable, sat_enable, satp16, 3		
		mvp		gprt0,udi_mode
		mvp		gprt0,udi_mode	
		mvp		gprt0,udi_mode	
		mvp		gpra1,udi_mac0acc0lo		
		macmvrr	acc1,sx0,sy0,shsat,x0,gpra1,y0,gprv0
		mvp		gprt0,udi_mode	
		mvp		gprt0,udi_mode
		mvp		gprt0,udi_mode
		mvp		gprs5,udi_mac0acc1lo		
		
		addiu	s4,s4,1
		slti	t3,s4,10
		addiu	s3,s3,2
		bnez	t3,Lsp_select_2180
		addiu	s2,s2,2
		move	a0,s5
		jal	L_sub
		move	a1,s8
		bltz	v0,Lsp_select_2352
		lw	t5,96(sp)
Lsp_select_2280:	addiu	s6,s6,1
		slti	t6,s6,32
		bnez	t6,Lsp_select_2140
		addiu	s7,s7,10
		lw	ra,76(sp)
		lw	s8,72(sp)
		lw	s7,68(sp)
		lw	s6,64(sp)
		lw	s5,60(sp)
		lw	s4,56(sp)
		lw	s3,52(sp)
		lw	s2,48(sp)
		lw	s1,44(sp)
		lw	s0,40(sp)
		jr	ra
		addiu	sp,sp,80
Lsp_select_2352:	move	s8,s5
		b	Lsp_select_2280
		sh	s6,0(t5)
END( Lsp_select_2 )

