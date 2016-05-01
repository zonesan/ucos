#include <mips/asm.h>
#include <mips/udi.h>
#include <mips/regdef.h>

#include "mipsudi_inst.h"

      .set noat
      .set noreorder

LEAF( Div_32 )
		addiu	sp,sp,-48
		sll	a1,a1,0x10
		sll	v1,a2,0x10
		sra	a1,a1,0x10
		sw	s3,36(sp)
		move	s3,a0
		li	a0,16383
		sw	ra,40(sp)
		sw	s2,32(sp)
		sw	s1,28(sp)
		sw	s0,24(sp)
		sra	s1,v1,0x10
		jal	div_s
		move	s0,a1
		move	s2,v0
		
		li	t1,1
		mpymvrr	acc0,sx0,sy0,shsat,x0,gprs0,y0,gprv0
		mpymvrr	acc1,sx0,sy0,shnone,x0,gprs1,y0,gprv0
		mvp	gprt0,udi_mode
		mvp	gprt0,udi_mode
		satacc	mac0,acc1,round_disable, sat_disable, satp32, 12
		mvp	gprt0,udi_mode
		mvp	gprt0,udi_mode
		satacc	mac0,acc1,round_disable, sat_enable, satp16, 3
		mvp	gprt0,udi_mode
		mvp	gprt0,udi_mode
		mvp	gprt2,udi_mac0acc0lo
		mvp	gprt3,udi_mac0acc1lo
		macmvrr	acc0,sx0,sy0,shsat,x0,gprt3,y0,gprt1
		mvp	gprt0,udi_mode
		mvp	gprt0,udi_mode
		mvp	gprt0,udi_mode
		mvp	gpra1,udi_mac0acc0lo
		
		lui	a0,0x7fff
		ori	v0,a0,0xffff
		subu	a0,v0,a1
		sra	v1,a0,0x10
		sra	a3,a0,0x1
		sll	v0,v1,0xf
		subu	a0,a3,v0
		
		mpymvrr	acc0,sx0,sy0,shsat,x0,gprv1,y0,gprs2
		mpymvrr	acc1,sx0,sy0,shnone,x0,gpra0,y0,gprs2
		mvp	gprt0,udi_mode
		mvp	gprt0,udi_mode
		satacc	mac0,acc1,round_disable, sat_disable, satp32, 12
		mvp	gprt0,udi_mode
		mvp	gprt0,udi_mode
		satacc	mac0,acc1,round_disable, sat_enable, satp16, 3
		mvp	gprt0,udi_mode
		mvp	gprt0,udi_mode
		mvp	gprt2,udi_mac0acc0lo
		mvp	gprt3,udi_mac0acc1lo
		macmvrr	acc0,sx0,sy0,shsat,x0,gprt3,y0,gprt1
		mvp	gprt0,udi_mode
		mvp	gprt0,udi_mode
		mvp	gprt0,udi_mode
		mvp	gpra0,udi_mac0acc0lo		
			
		sra	v1,a0,0x10
		sra	a3,a0,0x1
		sll	v0,v1,0xf
		subu	a0,a3,v0
			
		sra	a1,s3,0x10
		sra	a3,s3,0x1
		sll	v0,a1,0xf
		subu	a2,a3,v0
		
		mpymvrr	acc0,sx0,sy0,shnone,x0,gpra1,y0,gpra0
		mpymvrr	acc1,sx0,sy0,shnone,x0,gpra2,y0,gprv1
		mvp	gprt0,udi_mode
		mvp	gprt0,udi_mode
		satacc	mac0,acc0,round_disable, sat_disable, satp32, 12
		satacc	mac0,acc1,round_disable, sat_disable, satp32, 12
		mvp	gprt0,udi_mode
		mvp	gprt0,udi_mode
		satacc	mac0,acc0,round_disable, sat_enable, satp16, 3
		satacc	mac0,acc1,round_disable, sat_enable, satp16, 3
		mvp	gprt0,udi_mode
		mvp	gprt0,udi_mode
		mvp	gprt2,udi_mac0acc0lo
		mvp	gprt3,udi_mac0acc1lo
		mpymvrr	acc0,sx0,sy0,shsat,x0,gpra1,y0,gprv1
		macmvrr	acc0,sx0,sy0,shsat,x0,gprt2,y0,gprt1
		macmvrr	acc0,sx0,sy0,shsat,x0,gprt3,y0,gprt1
		mvp	gprt0,udi_mode
		mvp	gprt0,udi_mode
		mvp	gprt0,udi_mode
		mvp	gpra0,udi_mac0acc0lo	
		
		lui	v0,0x3fff
		ori	t1,v0,0xffff
		lui	t0,0xc000
		lui	t3,0x7fff
		slt	t2,t1,a0
		la	a2,Overflow
		lw	a3,0(a2)	
		ori	a2,t3,0xffff
		bnez	t2,Div_32288
		slt	a1,a0,t0
		bnez	a1,Div_32296
		sll	v0,a0,0x1
		slt	v1,v0,t0
		slt	t0,t1,v0
		bnezl	t0,Div_32252
		li	a3,1
		bnez	v1,Div_32296
		sll	a2,a0,0x2
Div_32252:	lw	ra,40(sp)
		lw	s3,36(sp)
		lw	s2,32(sp)
		lw	s1,28(sp)
		lw	s0,24(sp)
		move	v0,a2
		addiu	sp,sp,48
		la	t0,Overflow
		jr	ra
		sw	a3,0(t0)
Div_32288:	b	Div_32252
		li	a3,1
Div_32296:	li	a3,1
		b	Div_32252
		lui	a2,0x8000
END( Div_32 )
