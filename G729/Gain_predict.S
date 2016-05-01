#include <mips/asm.h>
#include <mips/udi.h>
#include <mips/regdef.h>

#include "mipsudi_inst.h"


      .set noat
      .set noreorder

LEAF( Gain_predict )
		addiu	sp,sp,-56
		sw	s2,32(sp)
		sll	s2,a2,0x10
		sra	s2,s2,0x10
		sw	s5,44(sp)
		sw	s4,40(sp)
		sw	s1,28(sp)
		move	s4,a0
		sw	ra,48(sp)
		sw	s3,36(sp)
		sw	s0,24(sp)
		move	s5,a3
		move	a0,zero
		blez	s2,Gain_predict108
		move	s1,zero
		mvp	udi_mac0acc0lo,gpra0
		mvp	udi_mac0acc0hi,gpra0
		move	s0,a1
		lh	a1,0(s0)
Gain_predict68:	addiu	s0,s0,2
		macmvrr	acc0,sx0,sy0,shsat,x0,gpra1,y0,gpra1
		addiu	s1,s1,1
		slt	v1,s1,s2
		bnezl	v1,Gain_predict68
		lh	a1,0(s0)
		mvp	gpra0,udi_mode
		mvp	gpra0,udi_mode
		mvp	gpra0,udi_mode
		mvp	gpra0,udi_mac0acc0lo
		
		
Gain_predict108:	addiu	s3,sp,18
		addiu	a1,sp,16
		jal	Log2
		move	a2,s3
		lh	a0,16(sp)
		lh	a1,18(sp)
		li	a2,-24660
		li	t5,32588
		li	t6,32
		li			t1,1
		mpymvrr		acc0,sx0,sy0,shsat,x0,gpra0,y0,gpra2
		mpymvrr		acc1,sx0,sy0,shnone,x0,gpra1,y0,gpra2
		satacc		mac0,acc1,round_disable, sat_disable, satp32, 12
		satacc		mac0,acc1,round_disable, sat_enable, satp16, 3
		mvp			gprt0,udi_mode
		mvp			gprt0,udi_mode
		mvp			gprt2,udi_mac0acc0lo
		mvp			gprt3,udi_mac0acc1lo
		macmvrr		acc0,sx0,sy0,shsat,x0,gprt3,y0,gprt1		
		macmvrr	acc0,sx0,sy0,shsat,x0,gprt5,y0,gprt6
		mvp			gprt0,udi_mode
		mvp			gprt0,udi_mode
		mvp			gprt0,udi_mode
		mvp			gpra0,udi_mac0acc0lo		
		jal	L_shl
		li	a1,10				
		mvp	udi_mac0acc0lo,gprv0
		srl	a0,v0,31		
		move	t5,zero
		li	t2,0xff
		movn	t5,t2,a0		
		mvp	udi_mac0acc0hi,gprt5		
		move	s0,s4
		move	s2,zero
		la	s1,pred
Gain_predict184:	lh	a1,0(s1)
		lh	a2,0(s0)
		macmvrr	acc0,sx0,sy0,shsat,x0,gpra1,y0,gpra2
		addiu	s1,s1,2
		addiu	s2,s2,1
		slti	a1,s2,4
		bnez	a1,Gain_predict184
		addiu	s0,s0,2
		
		mvp	gpra0,udi_mode
		mvp	gpra0,udi_mode
		mvp	gpra0,udi_mode
		mvp	gprv0,udi_mac0acc0lo		
		sra	a0,v0,0x10
		sh	a0,0(s5)
		//jal	L_mult
		li	a1,5439
		mpymvrr	acc0,sx0,sy0,shsat,x0,gpra0,y0,gpra1
		satacc		mac0,acc0,round_disable, sat_disable, satp32, 8
		mvp	gpra0,udi_mode
		mvp	gpra0,udi_mode
		mvp	gpra0,udi_mode
		mvp	gpra0,udi_mac0acc0lo
		//move	a0,v0
		move	a2,s3
		jal	L_Extract
		addiu	a1,sp,16
		/*
		
		sra	v1,v0,0x10
		sra	a3,v0,0x1
		sll	v0,v1,0xf
		subu	a1,a3,v0
		sh	v1,16(sp)
		sh	a1,0(s3)	
		*/
		
		lh	a1,18(sp)
		jal	Pow2
		li	a0,14
		sll	a1,v0,0x10
		sra	v0,a1,0x10
		lh	a1,16(sp)
		li	a0,14
		jal	wg_sub
		sh	v0,0(s5)
		lw	a0,72(sp)
		lw	ra,48(sp)
		lw	s5,44(sp)
		lw	s4,40(sp)
		lw	s3,36(sp)
		lw	s2,32(sp)
		lw	s1,28(sp)
		lw	s0,24(sp)
		addiu	sp,sp,56
		jr	ra
		sh	v0,0(a0)
END( Gain_predict )

