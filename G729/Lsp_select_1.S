#include <mips/asm.h>
#include <mips/udi.h>
#include <mips/regdef.h>

#include "mipsudi_inst.h"


      .set noat
      .set noreorder

LEAF( Lsp_select_1 )
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
		move	s1,a1
		move	s0,a0
		move	s2,zero
		addiu	s3,sp,16
		
		
		li	t2,0xff
/*		
Lsp_select_168:	lh	a0,0(s0)
		lh	a1,0(s1)
		jal	wg_sub
		addiu	s0,s0,2
		
		subu	a0,a0,a1
		mvp	udi_mac0acc0lo,gpra0
		srl	v0,a0,31
		move	t5,zero
		movn	t5,t2,v0		
		mvp	udi_mac0acc0hi,gprt5			
		satacc		mac0,acc0,round_disable, sat_enable, satp16, 0
		mvp		gprt0,udi_mode
		mvp		gprt0,udi_mode
		mvp		gprt0,udi_mode		
		mvp		gprv0,udi_mac0acc0lo
		
		addiu	s2,s2,1
		//sll	s2,v1,0x10
		//sra	s2,s2,0x10
		slti	a0,s2,5
		sh	v0,0(s3)
		addiu	s1,s1,2
		bnez	a0,Lsp_select_168
		addiu	s3,s3,2
*/

Lsp_select_168:	lh	a0,0(s0)
		lh	a1,0(s1)
		lh	t0,2(s0)
		lh	t1,2(s1)
		lh	t3,4(s0)
		lh	t4,4(s1)
		lh	t6,6(s0)
		lh	t7,6(s1)
		lh	t8,8(s0)
		lh	t9,8(s1)
		subu	a0,a0,a1
		subu	t0,t0,t1
		subu	t3,t3,t4
		subu	t6,t6,t7
		subu	t8,t8,t9
		mvp	udi_mac0acc0lo,gpra0
		srl	v0,a0,31
		move	t5,zero
		movn	t5,t2,v0		
		mvp	udi_mac0acc0hi,gprt5			
		satacc		mac0,acc0,round_disable, sat_enable, satp16, 0		
		mvp	udi_mac0acc1lo,gprt0
		srl	v0,t0,31
		move	t5,zero
		movn	t5,t2,v0		
		mvp	udi_mac0acc1hi,gprt5
		satacc		mac0,acc1,round_disable, sat_enable, satp16, 0
		mvp		gprv0,udi_mac0acc0lo
		sh	v0,0(s3)
		
		mvp	udi_mac1acc0lo,gprt3
		srl	v0,t3,31
		move	t5,zero
		movn	t5,t2,v0		
		mvp	udi_mac1acc0hi,gprt5
		mvp		gprv0,udi_mac0acc1lo		
		sh	v0,2(s3)		
			
		satacc		mac1,acc0,round_disable, sat_enable, satp16, 0
		mvp	udi_mac1acc1lo,gprt6
		srl	v0,t6,31
		move	t5,zero
		movn	t5,t2,v0		
		mvp	udi_mac1acc1hi,gprt5
		satacc		mac1,acc1,round_disable, sat_enable, satp16, 0		
		mvp		gprv0,udi_mac1acc0lo		
		sh	v0,4(s3)
				
		
		mvp	udi_mac0acc0lo,gprt8
		srl	v0,t8,31
		move	t5,zero
		movn	t5,t2,v0		
		mvp	udi_mac0acc0hi,gprt5
		satacc		mac0,acc0,round_disable, sat_enable, satp16, 0				
		mvp		gprv0,udi_mac1acc1lo		
		sh	v0,6(s3)				
		mvp		gprt0,udi_mode
		mvp		gprt0,udi_mode		
		mvp		gprv0,udi_mac0acc0lo		
		sh	v0,8(s3)
		
		lw	a1,96(sp)
		lui	v0,0x7fff
		sh	zero,0(a1)
		ori	s8,v0,0xffff
		move	s6,zero
		move	s7,zero
Lsp_select_1140:	lw	a2,92(sp)
		sll	a3,s7,0x1
		lw	s2,88(sp)
		addu	s3,a3,a2
		move	s5,zero
		mvp	udi_mac0acc1lo,gprs5
		mvp	udi_mac0acc1hi,gprs5
		move	s4,zero
		addiu	s1,sp,16
Lsp_select_1168:	lh	a0,0(s1)
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
		slti	t0,s4,5
		addiu	s3,s3,2
		bnez	t0,Lsp_select_1168
		addiu	s2,s2,2
		move	a0,s5
		jal	L_sub
		move	a1,s8
		bltz	v0,Lsp_select_1340
		lw	t2,96(sp)
Lsp_select_1268:	addiu	t4,s6,1
		sll	s6,t4,0x10
		sra	s6,s6,0x10
		slti	t3,s6,32
		bnez	t3,Lsp_select_1140
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
Lsp_select_1340:	move	s8,s5
		b	Lsp_select_1268
		sh	s6,0(t2)
END( Lsp_select_1 )

