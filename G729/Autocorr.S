/*
***************************************************************************
*                  Copyright (c) 2006 vBridge Microsystem, Inc.  
*                       Unpublished & Not for Publication
*                               All Rights Reserved                   
*   
* File        : Autocorr.S                     
*                                                                         
* Description:  Compute autocorrelations of signal with windowing  
*             
* Date        : Jun 7, 2006                                                           
***************************************************************************
*/

/*------------------------------------------------------------------------*
void Autocorr(
  Word16 x[],      // (i)    : Input signal                      
  Word16 m,        // (i)    : LPC order                         
  Word16 r_h[],    // (o)    : Autocorrelations  (msb)           
  Word16 r_l[],    // (o)    : Autocorrelations  (lsb)           
  Word16 *exp_R0
)
{
  Word16 i, j, norm;
  Word16 y[L_WINDOW];
  Word32 sum;

  extern Flag Overflow;

  // Windowing of signal 

  for(i=0; i<L_WINDOW; i++)
  {
    y[i] = mult_r(x[i], hamwindow[i]);
  }

  // Compute r[0] and test for overflow 

  *exp_R0 = 1;

  do {
    Overflow = 0;
    sum = 1;                   // Avoid case of all zeros 
    for(i=0; i<L_WINDOW; i++)
      sum = L_mac(sum, y[i], y[i]);

    // If overflow divide y[] by 4 

    if(Overflow != 0)
    {
      for(i=0; i<L_WINDOW; i++)
      {
        //y[i] = shr(y[i], 2);
        if (y[i] < 0)
        {
        	y[i] = ~(( ~y[i]) >> 2 );
        }
        else
        {
           	y[i] = y[i] >> 2;
        }
      }
      *exp_R0 = add((*exp_R0), 4);
      Overflow = 1;
    }
  }while (Overflow != 0);

  // Normalization of r[0] 

  norm = norm_l(sum);
  sum  = L_shl(sum, norm);
  L_Extract(sum, &r_h[0], &r_l[0]);     // Put in DPF format (see oper_32b) 
  *exp_R0 = sub(*exp_R0, norm);

  // r[1] to r[m] 

  for (i = 1; i <= m; i++)
  {
    sum = 0;
    for(j=0; j<L_WINDOW-i; j++)
      sum = L_mac(sum, y[j], y[j+i]);

    sum = L_shl(sum, norm);
    L_Extract(sum, &r_h[i], &r_l[i]);
  }
  return;
}

*------------------------------------------------------------------------*/

#include <mips/asm.h>
#include <mips/udi.h>
#include <mips/regdef.h>
#include "mipsudi_inst.h"

      .set noat
      .set noreorder

LEAF( Autocorr )
		addiu	sp,sp,-552
		sw	s6,536(sp)
		sw	s4,528(sp)
		sll	s6,a1,0x10
		lw	s4,568(sp)
		sw	s8,544(sp)
		sw	s7,540(sp)
		sw	s3,524(sp)
		sw	s2,520(sp)
		sw	s1,516(sp)
		sw	s0,512(sp)
		sw	ra,548(sp)
		sw	s5,532(sp)
		move	s7,a2
		move	s8,a3
		sra	s6,s6,0x10
		la	s3,hamwindow
		move	s1,a0
		move	s0,zero
		addiu	s2,sp,16
/*------------------------------------------------------------------------*
  		for(i=0; i<L_WINDOW; i++)
  		{
    			y[i] = mult_r(x[i], hamwindow[i]);
  		}
*------------------------------------------------------------------------*/		
Autocorr84:			
		lw	a0,0(s1)
		lw	a1,0(s3)
		lw	t0,4(s1)
		lw	t1,4(s3)
		lw	t2,8(s1)
		lw	t3,8(s3)
		lw	t4,12(s1)
		lw	t5,12(s3)		
		mpymvrr	acc0,sx0,sy0,shnone,x0,gpra0,y0,gpra1	
		mpymvrr	acc1,sx0,sy0,shnone,x0,gprt0,y0,gprt1
		satacc	mac0,acc0,round_disable, sat_disable, satp32, 12
		satacc	mac1,acc0,round_disable, sat_disable, satp32, 12
		satacc	mac0,acc1,round_disable, sat_disable, satp32, 12
		satacc	mac1,acc1,round_disable, sat_disable, satp32, 12
		satacc	mac0,acc0,round_enable, sat_enable, satp16, 3
		satacc	mac1,acc0,round_enable, sat_enable, satp16, 3
		satacc	mac0,acc1,round_enable, sat_enable, satp16, 3
		satacc	mac1,acc1,round_enable, sat_enable, satp16, 3	
		mvp	gpra0,udi_mac0acc0lo
		mvp	gpra1,udi_mac1acc0lo
		mvp	gprt0,udi_mac0acc1lo
		mvp	gprt1,udi_mac1acc1lo
		sh	a0,0(s2)
		sh	a1,2(s2)
		sh	t0,4(s2)
		sh	t1,6(s2)		
		addiu	s0,s0,8		
		mpymvrr	acc0,sx0,sy0,shnone,x0,gprt2,y0,gprt3
		mpymvrr	acc1,sx0,sy0,shnone,x0,gprt4,y0,gprt5
		satacc	mac0,acc0,round_disable, sat_disable, satp32, 12
		satacc	mac1,acc0,round_disable, sat_disable, satp32, 12
		satacc	mac0,acc1,round_disable, sat_disable, satp32, 12
		satacc	mac1,acc1,round_disable, sat_disable, satp32, 12
		satacc	mac0,acc0,round_enable, sat_enable, satp16, 3
		satacc	mac1,acc0,round_enable, sat_enable, satp16, 3
		satacc	mac0,acc1,round_enable, sat_enable, satp16, 3
		satacc	mac1,acc1,round_enable, sat_enable, satp16, 3	
		mvp	gprt2,udi_mac0acc0lo
		mvp	gprt3,udi_mac1acc0lo
		mvp	gprt4,udi_mac0acc1lo
		mvp	gprt5,udi_mac1acc1lo
		sh	t2,8(s2)
		sh	t3,10(s2)
		sh	t4,12(s2)		
		slti	a1,s0,240
		sh	t5,14(s2)
		addiu	s1,s1,16
		addiu	s2,s2,16
		bnez	a1,Autocorr84
		addiu	s3,s3,16
		
		li	v1,1
		sh	v1,0(s4)
Autocorr336:	li	s2,1
		move	s0,zero
		addiu	s1,sp,16
		mvp	udi_mac0acc0lo,gprs2
		mvp	udi_mac0acc0hi,gprs0
		mvp	gprt4,udi_mode
		li	t4,0x800000
		sw	s0,Overflow
		mvp	udi_mode,gprt4	
			
/*------------------------------------------------------------------------*
		for(i=0; i<L_WINDOW; i++)
      			sum = L_mac(sum, y[i], y[i]);
*------------------------------------------------------------------------*/		
Autocorr352:	lh	a1,0(s1)		
		lh	t0,2(s1)
		lh	t1,4(s1)
		lh	t2,6(s1)
		lh	t3,8(s1)
		lh	t4,10(s1)
		lh	t5,12(s1)
		lh	t6,14(s1)
		addiu	s0,s0,8				
		macmvrr	acc0,sx0,sy0,shsat,x0,gpra1,y0,gpra1
		macmvrr	acc0,sx0,sy0,shsat,x0,gprt0,y0,gprt0
		macmvrr	acc0,sx0,sy0,shsat,x0,gprt1,y0,gprt1
		macmvrr	acc0,sx0,sy0,shsat,x0,gprt2,y0,gprt2
		macmvrr	acc0,sx0,sy0,shsat,x0,gprt3,y0,gprt3
		macmvrr	acc0,sx0,sy0,shsat,x0,gprt4,y0,gprt4
		macmvrr	acc0,sx0,sy0,shsat,x0,gprt5,y0,gprt5
		macmvrr	acc0,sx0,sy0,shsat,x0,gprt6,y0,gprt6		
		slti	a3,s0,240		
		bnez	a3,Autocorr352
		addiu	s1,s1,16
		
		mvp	gprt7,udi_mode
		mvp	gprt8,udi_mode
		mvp	gprt8,udi_mode
		mvp	gprv0,udi_mac0acc0lo
		
		srl	t7,t7,20
		and	t4,t7,1	
		move	a0,zero
		li	a2,1
		movn	a0,a2,t4
		sw	a0,Overflow		
		move	s2,v0		
		beqz	a0,Autocorr908
		move	a2,zero
		b	Autocorr644
		addiu	a0,sp,16
Autocorr612:	addiu	a2,v1,1
		sra	t0,t7,0x2
		slti	ra,a2,240
		sh	t0,14(a0)
		beqz	ra,Autocorr880
		addiu	a0,a3,2
Autocorr644:	lh	t9,0(a0)
		lh	t6,2(a0)
		sra	t8,t9,0x2
		sra	t5,t6,0x2
		sh	t8,0(a0)
		sh	t5,2(a0)	
		lh	t1,4(a0)
		lh	s2,6(a0)
		lh	ra,8(a0)				
		sra	v0,t1,0x2
		sra	s5,s2,0x2
		sra	t9,ra,0x2		
		sh	v0,4(a0)
		sh	s5,6(a0)
		sh	t9,8(a0)	
		lh	t5,10(a0)		
		sra	t4,t5,0x2
		sh	t4,10(a0)
		lh	t3,12(a0)		
		sra	v0,t3,0x2
		sh	v0,12(a0)		
		addiu	v1,a2,7
		lh	t7,14(a0)
		bgez	t7,Autocorr612
		addiu	a3,a0,14				
		addiu	a2,v1,1
		sra	s5,t7,0x2
		slti	ra,a2,240
		sh	s5,14(a0)
		bnez	ra,Autocorr644
		addiu	a0,a3,2
Autocorr880:	lh	a0,0(s4)
/*
		addu	a1,a0,4
		srl	v0,a1,31
		move	a0,zero
		li	s0,0xff
		movn	a0,s0,v0		
		mvp	udi_mac0acc0lo,gpra1
		mvp	udi_mac0acc0hi,gpra0
		satacc	mac0,acc0,round_enable, sat_enable, satp16, 0
		mvp	gprv0,udi_mac0acc0lo
		mvp	gprv0,udi_mac0acc0lo
		mvp	gprv0,udi_mac0acc0lo
		mvp	gprv0,udi_mac0acc0lo
		
		li	a1,1
		sh	v0,0(s4)
		la	s0,Overflow		
		b	Autocorr336
		sw	a1,0(s0)
*/
		jal	wg_add
		li	a1,4
		li	a1,1
		
		//la	s0,Overflow
		lw	a0,Overflow		
		b	Autocorr336
		sh	v0,0(s4)
		//lw	a0,0(s0)
		
Autocorr908:	jal	norm_l
		move	a0,s2
		move	a1,v0
		move	a0,s2
		jal	L_shl
		sw	v0,496(sp)
		sra	t0,v0,0x10
		sh	t0,0(s7)
		sra	v0,v0,1
		lh	a1,0(s7)
		li	a2,16384
		
		srl	a0,v0,31
		move	t5,zero
		li	t2,0xff
		movn	t5,t2,a0		
		mvp	udi_mac0acc0lo,gprv0
		mvp	udi_mac0acc0hi,gprt5
		msumvrr	acc0,sx0,sy0,shsat,x0,gpra1,y0,gpra2
		mvp	gprt4,udi_mode
		mvp	gprt4,udi_mode
		mvp	gprt4,udi_mode
		mvp	gprv0,udi_mac0acc0lo		
		
		sll	v0,v0,0x10
		sra	v0,v0,0x10
		sh	v0,0(s8)
		lh	a0,0(s4)
		jal	wg_sub
		lw	a1,496(sp)
		li	a2,1
		sh	v0,0(s4)
		blez	s6,Autocorr1772
		sw	a2,504(sp)
		li	a3,2
		li	s5,239
		sw	a3,500(sp)
Autocorr1028:	blez	s5,Autocorr1652
		move	a0,zero
		lw	a1,500(sp)
		addiu	a0,sp,16
		addu	s4,a1,a0
		lh	a2,0(s4)
		lh	a1,16(sp)
		addiu	v1,s5,-1
/*------------------------------------------------------------------------*
		for(j=0; j<L_WINDOW-i; j++)
      			sum = L_mac(sum, y[j], y[j+i]);
*------------------------------------------------------------------------*/
		move	a0,zero
		li	s0,1
		mvp	udi_mac0acc0lo,gpra0
		mvp	udi_mac0acc0hi,gpra0
		macmvrr	acc0,sx0,sy0,shsat,x0,gpra1,y0,gpra2
		andi	s1,v1,0x7
		slt	ra,s0,s5
		addiu	v0,sp,16
		addiu	s2,v0,2
		beqz	ra,Autocorr1652
		addiu	s3,s4,2
		beqzl	s1,Autocorr1416
		lh	a1,0(s2)
		beq	s1,s0,Autocorr1368
		li	a2,2
		beq	s1,a2,Autocorr1332
		li	a3,3
		beq	s1,a3,Autocorr1296
		li	t0,4
		beq	s1,t0,Autocorr1260
		li	t1,5
		beq	s1,t1,Autocorr1224
		li	t2,6
		beql	s1,t2,Autocorr1192
		lh	a1,0(s2)
		lh	a1,18(sp)
		lh	a2,2(s4)
		macmvrr	acc0,sx0,sy0,shsat,x0,gpra1,y0,gpra2
		addiu	s3,s4,4
		addiu	t3,sp,16
		li	s0,2
		addiu	s2,t3,4
		lh	a1,0(s2)
Autocorr1192:	lh	a2,0(s3)
		macmvrr	acc0,sx0,sy0,shsat,x0,gpra1,y0,gpra2
		addiu	s2,s2,2
		addiu	s0,s0,1
		addiu	s3,s3,2
Autocorr1224:	lh	a1,0(s2)
		lh	a2,0(s3)
		macmvrr	acc0,sx0,sy0,shsat,x0,gpra1,y0,gpra2
		addiu	s2,s2,2
		addiu	s0,s0,1
		addiu	s3,s3,2
Autocorr1260:	lh	a1,0(s2)
		lh	a2,0(s3)
		macmvrr	acc0,sx0,sy0,shsat,x0,gpra1,y0,gpra2
		addiu	s0,s0,1
		addiu	s2,s2,2
		addiu	s3,s3,2
Autocorr1296:	lh	a1,0(s2)
		lh	a2,0(s3)
		macmvrr	acc0,sx0,sy0,shsat,x0,gpra1,y0,gpra2
		addiu	s2,s2,2
		addiu	s0,s0,1
		addiu	s3,s3,2
Autocorr1332:	lh	a1,0(s2)
		lh	a2,0(s3)
		macmvrr	acc0,sx0,sy0,shsat,x0,gpra1,y0,gpra2
		addiu	s2,s2,2
		addiu	s0,s0,1
		addiu	s3,s3,2
Autocorr1368:	lh	a1,0(s2)
		lh	a2,0(s3)
		macmvrr	acc0,sx0,sy0,shsat,x0,gpra1,y0,gpra2
		addiu	s2,s2,2
		addiu	s0,s0,1
		slt	v0,s0,s5
		beqz	v0,Autocorr1652
		addiu	s3,s3,2
Autocorr1412:	lh	a1,0(s2)
Autocorr1416:	lh	a2,0(s3)		
		lh	t2,2(s2)
		lh	t3,2(s3)
		lh	t4,4(s2)
		lh	t5,4(s3)
		lh	t6,6(s2)
		lh	t7,6(s3)
		lh	t8,8(s2)
		lh	t9,8(s3)
				
		macmvrr	acc0,sx0,sy0,shsat,x0,gpra1,y0,gpra2
		macmvrr	acc0,sx0,sy0,shsat,x0,gprt2,y0,gprt3
		macmvrr	acc0,sx0,sy0,shsat,x0,gprt4,y0,gprt5
		macmvrr	acc0,sx0,sy0,shsat,x0,gprt6,y0,gprt7
		lh	t2,10(s2)
		lh	t3,10(s3)
		lh	t4,12(s2)
		lh	t5,12(s3)
		lh	t6,14(s3)
		lh	t7,14(s2)
		macmvrr	acc0,sx0,sy0,shsat,x0,gprt8,y0,gprt9
		macmvrr	acc0,sx0,sy0,shsat,x0,gprt2,y0,gprt3
		macmvrr	acc0,sx0,sy0,shsat,x0,gprt4,y0,gprt5
		macmvrr	acc0,sx0,sy0,shsat,x0,gprt6,y0,gprt7
		
		addiu	s0,s0,8
		slt	a2,s0,s5		
		addiu	s2,s2,16
		bnez	a2,Autocorr1412
		addiu	s3,s3,16
		
		mvp	gprt9,udi_mode
		mvp	gprt8,udi_mode
		mvp	gprt8,udi_mode
		mvp	gprv0,udi_mac0acc0lo
		move	a0,v0
		
Autocorr1652:	lw	a1,496(sp)
		jal	L_shl
		addiu	s5,s5,-1
		move	s4,v0
		sra	v0,v0,0x10
		lw	t9,500(sp)
		sra	s3,s4,0x1
		addu	t8,t9,s7
		sh	v0,0(t8)
		lh	a1,0(t8)
		li	a2,16384
		
		srl	a0,s3,31
		move	t5,zero
		li	t2,0xff
		movn	t5,t2,a0		
		mvp	udi_mac0acc0lo,gprs3
		mvp	udi_mac0acc0hi,gprt5
		msumvrr	acc0,sx0,sy0,shsat,x0,gpra1,y0,gpra2
		mvp	gprt4,udi_mode
		mvp	gprt4,udi_mode
		mvp	gprt4,udi_mode
		mvp	gprv0,udi_mac0acc0lo
		
		
		sll	v0,v0,0x10
		sra	v0,v0,0x10
		lw	s2,504(sp)
		lw	t5,500(sp)
		addiu	t6,s2,1
		sw	t6,504(sp)
		lw	t2,500(sp)
		lw	t4,504(sp)
		addu	t3,t5,s8
		addiu	t1,t2,2
		slt	t0,s6,t4
		sh	v0,0(t3)
		beqz	t0,Autocorr1028
		sw	t1,500(sp)
Autocorr1772:	lw	ra,548(sp)
		lw	s8,544(sp)
		lw	s7,540(sp)
		lw	s6,536(sp)
		lw	s5,532(sp)
		lw	s4,528(sp)
		lw	s3,524(sp)
		lw	s2,520(sp)
		lw	s1,516(sp)
		lw	s0,512(sp)
		jr	ra
		addiu	sp,sp,552
		
END( Autocorr )