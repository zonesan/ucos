#include <mips/asm.h>
#include <mips/udi.h>
#include <mips/regdef.h>

#include "mipsudi_inst.h"

      .set noat
      .set noreorder

LEAF( preemphasis )
		sll	a3,a2,0x10
		sra	a2,a3,0x10
		addiu	sp,sp,-48
		sll	v1,a2,0x1
		addu	v0,v1,a0
		sw	s4,32(sp)
		sw	s3,28(sp)
		sw	s2,24(sp)
		sw	s1,20(sp)
		sw	s0,16(sp)
		sw	ra,40(sp)
		sw	s5,36(sp)
		sll	s4,a1,0x10
		addiu	s3,a2,-2
		sra	s4,s4,0x10
		lh	s5,-2(v0)
		addiu	s1,v0,-2
		addiu	s0,v0,-4
		bltz	s3,preemphasis136
		move	s2,zero
preemphasis80:	lh	a1,0(s0)
		mpymvrr	acc0,sx0,sy0,shnone,x0,gprs4,y0,gpra1
		satacc	mac0, acc0, round_disable, sat_disable, satp32, 12
		satacc	mac0, acc0, round_disable, sat_enable, satp16, 3
		mvp	gprv0,udi_mode
		mvp	gprv0,udi_mode
		mvp	gprv0,udi_mode
		mvp	gpra1,udi_mac0acc0lo
		
		jal	wg_sub
		lh	a0,0(s1)
		//move	a1,v0
		addiu	s2,s2,1
		slt	a0,s3,s2
		sh	v0,0(s1)
		addiu	s0,s0,-2
		beqz	a0,preemphasis80
		addiu	s1,s1,-2
preemphasis136:	
		lh	a1,mem_pre
		jal	mult
		move	a0,s4
		lh	a0,0(s1)
		jal	wg_sub
		move	a1,v0
		sh	v0,0(s1)
		lw	ra,40(sp)
		sh	s5,mem_pre
		lw	s4,32(sp)
		lw	s5,36(sp)
		lw	s3,28(sp)
		lw	s2,24(sp)
		lw	s1,20(sp)
		lw	s0,16(sp)
		jr	ra
		addiu	sp,sp,48
END( preemphasis )

