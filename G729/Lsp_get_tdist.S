#include <mips/asm.h>
#include <mips/udi.h>
#include <mips/regdef.h>

#include "mipsudi_inst.h"


      .set noat
      .set noreorder

LEAF( Lsp_get_tdist )
		addiu	sp,sp,-48
		sw	s3,28(sp)
		lw	s3,64(sp)
		sw	s6,40(sp)
		sw	s5,36(sp)
		sw	s4,32(sp)
		sw	s2,24(sp)
		sw	s1,20(sp)
		sw	ra,44(sp)
		sw	s0,16(sp)
		move	s6,a2
		sw	zero,0(a2)
		move	t0,zero
		mvp	udi_mac0acc1lo,gprt0
		mvp	udi_mac0acc1hi,gprt0
		move	s4,a0
		move	s2,a3
		move	s1,a1
		move	s5,zero
Lsp_get_tdist64:	lh	a0,0(s1)
		lh	a1,0(s2)
		jal	wg_sub
		addiu	s1,s1,2
		lh	a1,0(s3)	
		mpymvrr	acc0,sx0,sy0,shnone,x0,gprv0,y0,gpra1
		satacc	mac0, acc0, round_disable, sat_disable, satp32, 12
		satacc	mac0, acc0, round_disable, sat_enable, satp16, 3
		mvp	gpra1,udi_mode
		mvp	gpra1,udi_mode
		mvp	gpra1,udi_mode
		mvp	gprs0,udi_mac0acc0lo
		lh	a0,0(s4)
		mpymvrr	acc0,sx0,sy0,shsat,x0,gpra0,y0,gprs0
		mvp	gpra1,udi_mode
		mvp	gpra1,udi_mode
		mvp	gpra1,udi_mode
		mvp	gpra0,udi_mac0acc0lo		
		jal	L_shl
		li	a1,4
		sra	a1,v0,0x10
		macmvrr	acc1,sx0,sy0,shsat,x0,gpra1,y0,gprs0
		mvp	gprt0,udi_mode
		mvp	gprt0,udi_mode
		mvp	gprt0,udi_mode
		mvp	gprv0,udi_mac0acc1lo
		addiu	s5,s5,1
		slti	a0,s5,10
		addiu	s2,s2,2
		sw	v0,0(s6)
		addiu	s3,s3,2
		bnez	a0,Lsp_get_tdist64
		addiu	s4,s4,2
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
END( Lsp_get_tdist )

