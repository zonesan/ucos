#include <mips/asm.h>
#include <mips/udi.h>
#include <mips/regdef.h>

#include "mipsudi_inst.h"

      .set noat
      .set noreorder

LEAF( Get_lsp_pol )
		addiu	sp,sp,-48
		sw	s1,20(sp)
		sw	s0,16(sp)
		move	s1,a0
		move	s0,a1
		li	a0,4096
		li	a1,2048
		sw	ra,44(sp)
		sw	s6,40(sp)
		sw	s4,32(sp)
		sw	s3,28(sp)
		sw	s5,36(sp)
		//jal	L_mult
		move	t0,zero
		sw	s2,24(sp)
		lh	t1,0(s1)
		li	a2,512
		mpymvrr	acc0,sx0,sy0,shsat,x0,gpra0,y0,gpra1
		mvp	udi_mac0acc1lo,gprt0
		mvp	udi_mac0acc1hi,gprt0
		msumvrr	acc1,sx0,sy0,shsat,x0,gprt1,y0,gpra2
		mvp	gprv0,udi_mac0acc0lo
		mvp	gprt0,udi_mode
		sw	v0,0(s0)
		mvp	gprt0,udi_mode
		mvp	gprv0,udi_mac0acc1lo
		
		
		
		//jal	L_msu
		
		sw	v0,4(s0)
		lui	v0,0x3fff
		addiu	s0,s0,8
		addiu	s3,s1,4
		ori	s6,v0,0xffff
		li	s4,2
Get_lsp_pol100:	lw	a0,-8(s0)
		li	s2,1
		slt	v1,s2,s4
		beqz	v1,Get_lsp_pol256
		sw	a0,0(s0)
		move	s5,s6
		li			t6,1
Get_lsp_pol124:	lw	t3,-4(s0)
		lh	a2,0(s3)
		sra	a0,t3,0x10
		sll	t2,a0,0xf
		sra	t1,t3,0x1
		subu	t0,t1,t2
		sll	a1,t0,0x10
		//jal	Mpy_32_16
		sra	a1,a1,0x10
		
		
		
		mpymvrr		acc1,sx0,sy0,shnone,x0,gpra1,y0,gpra2
		satacc		mac0,acc1,round_disable, sat_disable, satp32, 12
		satacc		mac0,acc1,round_disable, sat_enable, satp16, 3
		//mvp			gprt0,udi_mode
		mpymvrr		acc0,sx0,sy0,shsat,x0,gpra0,y0,gpra2
		mvp			gprt0,udi_mode
		mvp			gprt2,udi_mac0acc0lo
		mvp			gprt3,udi_mac0acc1lo
		macmvrr		acc0,sx0,sy0,shsat,x0,gprt3,y0,gprt6
		mvp			gprt0,udi_mode
		mvp			gprt0,udi_mode
		mvp			gprt0,udi_mode
		mvp			gprv0,udi_mac0acc0lo
				
		slt	a2,s5,v0
		lui	a0,0xc000
		lui	a3,0x7fff
		slt	a1,v0,a0
		ori	s1,a3,0xffff
		bnez	a2,Get_lsp_pol196
		sll	a0,v0,0x1
		lui	s1,0x8000
		movz	s1,a0,a1
Get_lsp_pol196:	lw	a0,0(s0)
		jal	L_add
		lw	a1,-8(s0)
		sw	v0,0(s0)
		move	a0,v0
		jal	L_sub
		move	a1,s1
		addiu	t5,s2,1
		sll	s2,t5,0x10
		sra	s2,s2,0x10
		slt	t4,s2,s4
		sw	v0,0(s0)
		bnez	t4,Get_lsp_pol124
		addiu	s0,s0,-4
		
		lw	a0,0(s0)
Get_lsp_pol256:	lh	a1,0(s3)
		li	a2,512
		jal	L_msu
		addiu	s5,s4,1
		sll	t7,s4,0x2
		sll	s4,s5,0x10
		sra	s4,s4,0x10
		slti	t6,s4,6
		sw	v0,0(s0)
		addiu	s3,s3,4
		bnez	t6,Get_lsp_pol100
		addu	s0,s0,t7
		lw	ra,44(sp)
		lw	s6,40(sp)
		lw	s5,36(sp)
		lw	s4,32(sp)
		lw	s3,28(sp)
		lw	s2,24(sp)
		lw	s1,20(sp)
		lw	s0,16(sp)
		jr	ra
		addiu	sp,sp,48
END( Get_lsp_pol )

