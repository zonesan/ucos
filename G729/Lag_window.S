/*
***************************************************************************
*                  Copyright (c) 2006 vBridge Microsystem, Inc.  
*                       Unpublished & Not for Publication
*                               All Rights Reserved                   
*   
* File        : Lag_window.S                     
*                                                                         
* Description:	Function Lag_window() 
*    		Lag_window on autocorrelations. 
*		r[i] *= lag_wind[i]   
*
*		r[i] and lag_wind[i] are in special double precision.
*      		See "oper_32b.c" for the format    
* Date        : Jun 7, 2006                                                           
***************************************************************************
*/

#include <mips/asm.h>
#include <mips/udi.h>
#include <mips/regdef.h>

#include "mipsudi_inst.h"

      .set noat
      .set noreorder
      
/*------------------------------------------------------------------------*
void Lag_window(
  	Word16 m,         // (i)     : LPC order                        
  	Word16 r_h[],     // (i/o)   : Autocorrelations  (msb)          
  	Word16 r_l[]      // (i/o)   : Autocorrelations  (lsb)          
)
{
  	Word16 i;
  	Word32 x;

	for(i=1; i<=m; i++)
	{
	     	x  = Mpy_32(r_h[i], r_l[i], lag_h[i-1], lag_l[i-1]);
	     	L_Extract(x, &r_h[i], &r_l[i]);
	}
	return;
}
*------------------------------------------------------------------------*/


LEAF( Lag_window )

		addiu	sp,sp,-56
		sw	s5,36(sp)
		sll	s5,a0,0x10
		sra	s5,s5,0x10
		sw	s7,44(sp)
		sw	s6,40(sp)
		sw	s2,24(sp)
		sw	ra,48(sp)
		sw	s4,32(sp)
		sw	s3,28(sp)
		sw	s1,20(sp)
		sw	s0,16(sp)
		move	s7,a1
		move	s6,a2
		blez	s5,Lag_window164
		li	s2,1
		la	v1,lag_h
 		la	a0,lag_l
		addiu	s4,a0,2
		addiu	s3,v1,2
		li	t1,1
		li	t5,0x9fff8400
		li	t7,0x2100
		
		
Lag_window88:	sll	t0,s2,0x1
		addu	s1,t0,s6
		addu	s0,t0,s7
		lh	a3,-2(s4)
		lh	a2,-2(s3)
		lh	a0,0(s0)
		lh	a1,0(s1)
		addiu	s3,s3,2
		
		mpymvrr	acc0,sx0,sy0,shnone,x0,gpra0,y0,gpra3
		satacc	mac0,acc0,round_disable, sat_disable, satp32, 12
		satacc	mac0,acc0,round_disable, sat_enable, satp16, 3
		mpymvrr	acc1,sx0,sy0,shnone,x0,gpra1,y0,gpra2
		satacc	mac0,acc1,round_disable, sat_disable, satp32, 12
		satacc	mac0,acc1,round_disable, sat_enable, satp16, 3
		mvp	gprt2,udi_mac0acc0lo
		mvp	gprt0,udi_mode
		mpymvrr	acc0,sx0,sy0,shsat,x0,gpra0,y0,gpra2
		mvp	gprt3,udi_mac0acc1lo		
		macmvrr	acc0,sx0,sy0,shsat,x0,gprt2,y0,gprt1
		macmvrr	acc0,sx0,sy0,shsat,x0,gprt3,y0,gprt1
		mvp	gprt0,udi_mode
		mvp	gprt0,udi_mode
		mvp	gprt0,udi_mode
		mvp	gprv0,udi_mac0acc0lo
		
		addiu	s2,s2,1
		
 		sw	v0,0(t5)
 		move 	t0,zero
 		mvp	udi_dagy_rdptr,gprt0
 		mvp	udi_dagy_rd,gprt7
 		mvdry	x0,gpra0,y0,rd32dpf
 		mvp	gprt0,udi_mode
 		mvp	gprt0,udi_y0
 		sh	t0,0(s1)
 		srl	a1,t0,0x10
 		sh	a1,0(s0)
		
		
		slt	a2,s5,s2
		beqz	a2,Lag_window88
		addiu	s4,s4,2
Lag_window164:	lw	ra,48(sp)
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
END( Lag_window )
