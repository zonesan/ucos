#include <mips/asm.h>
#include <mips/udi.h>
#include <mips/regdef.h>

#include "mipsudi_inst.h"


      .set noat
      .set noreorder

LEAF( Init_lsfq_noise )
		addiu	sp,sp,-48
		sw	s6,40(sp)
		sw	s5,36(sp)
		sw	s1,20(sp)
		sw	s0,16(sp)
		sw	ra,44(sp)
		sw	s4,32(sp)
		sw	s3,28(sp)
		sw	s2,24(sp)
		la	s5,fg
		la	s6,noise_fg
		move	s1,zero
		move	s0,zero
Init_lsfq_noise60:	addu	a0,s0,s5
		addu	a1,s0,s6
		jal	Copy
		li	a2,10
		addiu	s1,s1,1
		slti	a0,s1,4
		bnez	a0,Init_lsfq_noise60
		addiu	s0,s0,20
		
		move	s4,zero
		sll	t0,s4,0x2
		li	a1,19660
		li	a0,13107
Init_lsfq_noise108:	addu	a3,t0,s4
		sll	a2,a3,0x2
		move	s2,a2
		move	s3,zero
		addu	s1,a2,s6
Init_lsfq_noise128:	addu	s0,s2,s5
		lh	t1,0(s0)
		lh	t0,80(s0)
		mpymvrr	acc0,sx0,sy0,shsat,x0,gprt1,y0,gpra1		
		macmvrr	acc0,sx0,sy0,shsat,x0,gprt0,y0,gpra0
		mvp	gprt0,udi_mode
		mvp	gprt0,udi_mode
		mvp	gprt0,udi_mode
		mvp	gprt0,udi_mac0acc0lo		
		addiu	s3,s3,1
		sra	v0,t0,0x10
		slti	t1,s3,10
		addiu	s2,s2,2
		sh	v0,80(s1)		
		bnez	t1,Init_lsfq_noise128
		addiu	s1,s1,2
		addiu	s4,s4,1
		slti	t3,s4,4
		bnez	t3,Init_lsfq_noise108
		sll	t0,s4,0x2
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
END( Init_lsfq_noise )
