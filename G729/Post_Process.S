#include <mips/asm.h>
#include <mips/udi.h>
#include <mips/regdef.h>

#include "mipsudi_inst.h"


      .set noat
      .set noreorder

LEAF( Post_Process )
		addiu	sp,sp,-40
		sw	s4,32(sp)
		sll	s4,a1,0x10
		sra	s4,s4,0x10
		sw	s3,28(sp)
		sw	ra,36(sp)
		sw	s2,24(sp)
		sw	s1,20(sp)
		sw	s0,16(sp)
		blez	s4,Post_Process260
		move	s3,zero
		move	s2,a0
Post_Process48:	
		lhu	a0,de_x0
		lh	s1,de_x1
		sh	a0,de_x1
		lhu	t1,0(s2)
		
		lh	a0,de_y1_hi
		lh	a1,de_y1_lo
		la	a3,a100
		sh	t1,de_x0
		lh	a2,2(a3)
		lh	t4,4(a3)
		lh	t5,de_y2_hi
		lh	t6,de_y2_lo
		li		t1,0x10001
		
		mpymvrr		acc1,sx0,sy0,shnone,x0,gpra1,y0,gpra2
		satacc		mac0,acc1,round_disable, sat_disable, satp32, 12
		satacc		mac0,acc1,round_disable, sat_enable, satp16, 3
		mpymvrr		acc0,sx0,sy0,shsat,x0,gpra0,y0,gpra2
		
		mpymvrr		acc1,sx0,sy0,shnone,x0,gprt6,y0,gprt4
		satacc		mac0,acc1,round_disable, sat_disable, satp32, 12
		
		mvp		gprt3,udi_mac0acc1lo
		macmvrr		acc0,sx0,sy0,shsat,x0,gprt3,y0,gprt1
		satacc		mac0,acc1,round_disable, sat_enable, satp16, 3
		mpymvrr		acc0,sx0,sy0,shsat,x0,gprt5,y0,gprt4
		mvp		gprt0,udi_mode
		mvp		gprs0,udi_mac0acc0lo
		mvp		gprt3,udi_mac0acc1lo
		macmvrr		acc0,sx0,sy0,shsat,x0,gprt3,y0,gprt1
		mvp		gprt0,udi_mac0acc0lo
		mvp		udi_mac1acc0lo,gprs0
		mvp		udi_mac1acc0hi,gprt0
		
		addacc		acc0, acc0
		mvp		gprt0,udi_mode
		mvp		gprt0,udi_mode
		mvp		gprt0,udi_mode
		mvp		gprt3,udi_mac0acc0lo
		
		srl	a0,t3,31
		move	t4,zero
		li	a1,0xff
		movn	t4,a1,a0
		mvp	udi_mac0acc0hi,gprt4
		
		lh	a1,de_x0
		la	a3,b100
		lh	a2,0(a3)
		lh	t2,2(a3)
		lh	t1,de_x1
		lh	t3,4(a3)
		macmvrr		acc0,sx0,sy0,shsat,x0,gpra1,y0,gpra2
		macmvrr		acc0,sx0,sy0,shsat,x0,gprt1,y0,gprt2
		macmvrr		acc0,sx0,sy0,shsat,x0,gprt3,y0,gprs1
		mvp		gprt0,udi_mode
		mvp		gprt0,udi_mode
		mvp		gprt0,udi_mode
		mvp		gprv0,udi_mac0acc0lo
		
		move	a0,v0
		jal	L_shl_n
		li	a1,2
		move	a0,v0
		li	a1,1
		jal	L_shl_1
		move	s0,v0
		jal	round16
		move	a0,v0
		sh	v0,0(s2)
		lhu	t0,de_y1_hi
		lhu	t1,de_y1_lo
		
		sra	v1,s0,0x10
		sra	a3,s0,0x1
		sll	v0,v1,0xf
		sh	t0,de_y2_hi
		sh	t1,de_y2_lo
		subu	a0,a3,v0			
		sh	v1,de_y1_hi
		sh	a0,de_y1_lo		
		addiu	s3,s3,1
		slt	v1,s3,s4
		bnez	v1,Post_Process48
		addiu	s2,s2,2
Post_Process260:	lw	ra,36(sp)
		lw	s4,32(sp)
		lw	s3,28(sp)
		lw	s2,24(sp)
		lw	s1,20(sp)
		lw	s0,16(sp)
		jr	ra
		addiu	sp,sp,40
END( Post_Process )

