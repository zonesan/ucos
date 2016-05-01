#include <mips/asm.h>
#include <mips/udi.h>
#include <mips/regdef.h>

#include "mipsudi_inst.h"


      .set noat
      .set noreorder

LEAF( Weight_Az )
		addiu	sp,sp,-56
		sw	s7,44(sp)
		sw	s6,40(sp)
		sw	s5,36(sp)
		sw	s4,32(sp)
		sw	s3,28(sp)
		sw	s1,20(sp)
		sw	ra,48(sp)
		sw	s2,24(sp)
		sw	s0,16(sp)
		sll	s4,a2,0x10
		sra	s4,s4,0x10
		lhu	v1,0(a0)
		li	s3,1
		sll	s5,a1,0x10
		sra	s5,s5,0x10
		slt	v0,s3,s4
		move	s6,a0
		move	s7,a3
		sh	v1,0(a3)
		beqz	v0,Weight_Az172
		move	s1,s5
		addiu	s2,a3,2
		addiu	s0,a0,2
Weight_Az96:	lh	a0,0(s0)
		
		mpymvrr	acc0,sx0,sy0,shsat,x0,gpra0,y0,gprs1
		satacc	mac0, acc0, round_enable, sat_enable, satp16, 16		
		mpymvrr	acc1,sx0,sy0,shsat,x0,gprs1,y0,gprs5
		satacc	mac0, acc1, round_enable, sat_enable, satp16, 16
		mvp	gprt0,udi_mode
		mvp	gprv0,udi_mac0acc0lo
		sh	v0,0(s2)	
		
		mvp	gprt0,udi_mode
		mvp	gprs1,udi_mac0acc1lo
		addiu	s3,s3,1
		slt	a0,s3,s4
		addiu	s0,s0,2
		bnez	a0,Weight_Az96
		addiu	s2,s2,2
		
Weight_Az172:	sll	s2,s4,0x1
		addu	a2,s2,s6
		lh	a0,0(a2)
		jal	L_mult
		move	a1,s1
		jal	round16
		move	a0,v0
		addu	s0,s2,s7
		sh	v0,0(s0)
		lw	ra,48(sp)
		lw	s7,44(sp)
		lw	s6,40(sp)
		lw	s5,36(sp)
		lw	s4,32(sp)
		lw	s3,28(sp)
		lw	s2,24(sp)
		lw	s1,20(sp)
		lw	s0,16(sp)
		jr	ra
		addiu	sp,sp,56
END( Weight_Az )


