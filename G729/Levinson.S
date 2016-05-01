/*
***************************************************************************
*                  Copyright (c) 2006 vBridge Microsystem, Inc.  
*                       Unpublished & Not for Publication
*                               All Rights Reserved                   
*   
* File        : Levinson.S                     
*                                                                         
* Description: LEVINSON-DURBIN algorithm in double precision   
*             
* Date        : Jun 7, 2006                                                           
***************************************************************************
*/

#include <mips/asm.h>
#include <mips/udi.h>
#include <mips/regdef.h>
#include "mips.h"
#include "archdefs.h"
#include "mipsudi_inst.h"
#include "Levinson.h"

      .set noat
      .set noreorder
      
/*------------------------------------------------------------------------*

void Levinson(
  Word16 Rh[],      // (i)     : Rh[M+1] Vector of autocorrelations (msb) 
  Word16 Rl[],      // (i)     : Rl[M+1] Vector of autocorrelations (lsb) 
  Word16 A[],       // (o) Q12 : A[M]    LPC coefficients  (m = 10)       
  Word16 rc[],      // (o) Q15 : rc[M]   Reflection coefficients.         
  Word16 *Err       // (o)     : Residual energy                          
)
{
 Word16 i, j;
 Word16 hi, lo;
 Word16 Kh, Kl;                // reflection coefficient; hi and lo           
 Word16 alp_h, alp_l, alp_exp; // Prediction gain; hi lo and exponent         
 Word16 Ah[M+1], Al[M+1];      // LPC coef. in double prec.                   
 Word16 Anh[M+1], Anl[M+1];    // LPC coef.for next iteration in double prec. 
 Word32 t0, t1, t2;            // temporary variable                          


// K = A[1] = -R[1] / R[0] 

  t1  = L_Comp(Rh[1], Rl[1]);           // R[1] in Q31      
  t2  = L_abs(t1);                      // abs R[1]         
  t0  = Div_32(t2, Rh[0], Rl[0]);       // R[1]/R[0] in Q31 
  if(t1 > 0) t0= L_negate(t0);          // -R[1]/R[0]       
  L_Extract(t0, &Kh, &Kl);              // K in DPF         
  rc[0] = Kh;
  t0 = L_shr(t0,4);                     // A[1] in Q27      
  L_Extract(t0, &Ah[1], &Al[1]);        // A[1] in DPF      

//  Alpha = R[0] * (1-K**2) 

  t0 = Mpy_32(Kh ,Kl, Kh, Kl);          // K*K      in Q31 
  t0 = L_abs(t0);                       // Some case <0 !! 
  t0 = L_sub( (Word32)0x7fffffffL, t0 ); // 1 - K*K  in Q31 
  L_Extract(t0, &hi, &lo);              // DPF format      
  t0 = Mpy_32(Rh[0] ,Rl[0], hi, lo);    // Alpha in Q31    

// Normalize Alpha 

  alp_exp = norm_l(t0);
  t0 = L_shl(t0, alp_exp);
  L_Extract(t0, &alp_h, &alp_l);         // DPF format    

//--------------------------------------
//  ITERATIONS  I=2 to M                
//--------------------------------------

  for(i= 2; i<=M; i++)
  {

    // t0 = SUM ( R[j]*A[i-j] ,j=1,i-1 ) +  R[i] 

    t0 = 0;
    for(j=1; j<i; j++)
      t0 = L_add(t0, Mpy_32(Rh[j], Rl[j], Ah[i-j], Al[i-j]));

    t0 = L_shl(t0,4);                  // result in Q27 -> convert to Q31 
                                       // No overflow possible            
    t1 = L_Comp(Rh[i],Rl[i]);
    t0 = L_add(t0, t1);                // add R[i] in Q31                 

    // K = -t0 / Alpha 

    t1 = L_abs(t0);
    t2 = Div_32(t1, alp_h, alp_l);     // abs(t0)/Alpha                   
    if(t0 > 0) t2= L_negate(t2);       // K =-t0/Alpha                    
    t2 = L_shl(t2, alp_exp);           // denormalize; compare to Alpha   
    L_Extract(t2, &Kh, &Kl);           // K in DPF                        
    rc[i-1] = Kh;

    // Test for unstable filter. If unstable keep old A(z) 

    if (sub(abs_s(Kh), 32750) > 0)
    {
      for(j=0; j<=M; j++)
      {
        A[j] = old_A[j];
      }
      rc[0] = old_rc[0];        // only two rc coefficients are needed 
      rc[1] = old_rc[1];
      return;
    }

    //------------------------------------------
    //  Compute new LPC coeff. -> An[i]         
    //  An[j]= A[j] + K*A[i-j]     , j=1 to i-1 
    //  An[i]= K                                
    //------------------------------------------


    for(j=1; j<i; j++)
    {
      t0 = Mpy_32(Kh, Kl, Ah[i-j], Al[i-j]);
      t0 = L_add(t0, L_Comp(Ah[j], Al[j]));
      L_Extract(t0, &Anh[j], &Anl[j]);
    }
    t2 = L_shr(t2, 4);                  // t2 = K in Q31 ->convert to Q27  
    L_Extract(t2, &Anh[i], &Anl[i]);    // An[i] in Q27                    

    //  Alpha = Alpha * (1-K**2) 

    t0 = Mpy_32(Kh ,Kl, Kh, Kl);          // K*K      in Q31 
    t0 = L_abs(t0);                       // Some case <0 !! 
    t0 = L_sub( (Word32)0x7fffffffL, t0 ); // 1 - K*K  in Q31 
    L_Extract(t0, &hi, &lo);              // DPF format      
    t0 = Mpy_32(alp_h , alp_l, hi, lo);   // Alpha in Q31    

    // Normalize Alpha 

    j = norm_l(t0);
    t0 = L_shl(t0, j);
    L_Extract(t0, &alp_h, &alp_l);         // DPF format    
    alp_exp = add(alp_exp, j);             // Add normalization to alp_exp 

    // A[j] = An[j] 

    for(j=1; j<=i; j++)
    {
      Ah[j] =Anh[j];
      Al[j] =Anl[j];
    }
  }

  *Err = shr(alp_h, alp_exp);

  // Truncate A[i] in Q27 to Q12 with rounding 

  A[0] = 4096;
  for(i=1; i<=M; i++)
  {
    t0   = L_Comp(Ah[i], Al[i]);
    old_A[i] = A[i] = ROUND(L_shl(t0, 1));
  }
  old_rc[0] = rc[0];
  old_rc[1] = rc[1];

  return;
}
*------------------------------------------------------------------------*/



 
//LEAF( Levinson )
      .text
      .align	4
      .globl Levinson
      .ent Levinson
      
Levinson:
		addiu	sp,sp,-184
		sw	s1,148(sp)
		sw	ra,180(sp)
		sw	s8,176(sp)
		sw	s7,172(sp)
		sw	s6,168(sp)
		sw	s5,164(sp)
		sw	s4,160(sp)
		sw	s3,156(sp)
		sw	s2,152(sp)
		sw	s0,144(sp)
		sw	a0,184(sp)
		lh	a0,2(a0)
		sw	a1,188(sp)
		lh	a1,2(a1)
		sw	a2,192(sp)
		sll	a0,a0,0x10
		li	a2,1
		jal	L_mac
		sw	a3,196(sp)
		move	s1,v0
		lui	v1,0x8000
		lui	v0,0x7fff
		beq	s1,v1,Levinson112
		ori	a0,v0,0xffff
		sra	a1,s1,0x1f
		xor	a2,a1,s1
		subu	a0,a2,a1
Levinson112:	lw	t0,184(sp)
		lw	a3,188(sp)
		lh	a1,0(t0)
		jal	Div_32
		lh	a2,0(a3)
		blez	s1,Levinson152
		move	s0,v0
		jal	L_negate
		move	a0,v0
		move	s0,v0
Levinson152:	
		sra	v1,s0,0x10
		sra	a3,s0,0x1
		sll	v0,v1,0xf
		subu	a0,a3,v0
		sh	v1,20(sp)
		sh	a0,22(sp)
		
		lw	s2,196(sp)
		sra	a0,s0,0x4		
		sh	v1,0(s2)
		
		sra	v1,a0,0x10
		sra	a3,a0,0x1
		sll	v0,v1,0xf
		subu	a0,a3,v0
		sh	v1,34(sp)
		sh	a0,58(sp)		
		
		lh	a1,22(sp)
		lh	a0,20(sp)
/*------------------------------------------------------------------------*
  		t0 = Mpy_32(Kh ,Kl, Kh, Kl);
*------------------------------------------------------------------------*/		
		li		t1,1
		mpymvrr		acc0,sx0,sy0,shnone,x0,gpra0,y0,gpra1
		mpymvrr		acc1,sx0,sy0,shnone,x0,gpra1,y0,gpra0
		satacc		mac0,acc0,round_disable, sat_disable, satp32, 12
		satacc		mac0,acc1,round_disable, sat_disable, satp32, 12
		satacc		mac0,acc0,round_disable, sat_enable, satp16, 3
		satacc		mac0,acc1,round_disable, sat_enable, satp16, 3
		mvp		gprt0,udi_mode
		mvp		gprt0,udi_mode
		mvp		gprt2,udi_mac0acc0lo
		mvp		gprt3,udi_mac0acc1lo
		mpymvrr		acc0,sx0,sy0,shsat,x0,gpra0,y0,gpra0
		macmvrr		acc0,sx0,sy0,shsat,x0,gprt2,y0,gprt1
		macmvrr		acc0,sx0,sy0,shsat,x0,gprt3,y0,gprt1
		mvp		gprt0,udi_mode
		mvp		gprt0,udi_mode
		mvp		gprt0,udi_mode		
		mvp		gprv0,udi_mac0acc0lo
		
		jal	L_abs
		move	a0,v0
		lui	s1,0x7fff
		ori	a0,s1,0xffff
		jal	L_sub
		move	a1,v0
		
		sra	v1,v0,0x10
		sra	a3,v0,0x1
		sll	v0,v1,0xf
		subu	a0,a3,v0
		sh	v1,16(sp)
		sh	a0,18(sp)
		
		lw	t7,188(sp)
		lw	t6,184(sp)
		lh	a1,0(t7)
		lh	a0,0(t6)
		lh	a2,16(sp)
		lh	a3,18(sp)
		li	s5,2
/*------------------------------------------------------------------------*
  		t0 = Mpy_32(Rh[0] ,Rl[0], hi, lo);
*------------------------------------------------------------------------*/
		li		t1,1
		mpymvrr		acc0,sx0,sy0,shnone,x0,gpra0,y0,gpra3
		mpymvrr		acc1,sx0,sy0,shnone,x0,gpra1,y0,gpra2
		satacc		mac0,acc0,round_disable, sat_disable, satp32, 12
		satacc		mac0,acc1,round_disable, sat_disable, satp32, 12
		satacc		mac0,acc0,round_disable, sat_enable, satp16, 3
		satacc		mac0,acc1,round_disable, sat_enable, satp16, 3
		mvp		gprt0,udi_mode
		mvp		gprt0,udi_mode
		mvp		gprt2,udi_mac0acc0lo
		mvp		gprt3,udi_mac0acc1lo
		mpymvrr		acc0,sx0,sy0,shsat,x0,gpra0,y0,gpra2
		macmvrr		acc0,sx0,sy0,shsat,x0,gprt2,y0,gprt1
		macmvrr		acc0,sx0,sy0,shsat,x0,gprt3,y0,gprt1
		mvp		gprt0,udi_mode
		mvp		gprt0,udi_mode
		mvp		gprt0,udi_mode		
		mvp		gprv0,udi_mac0acc0lo
		
		move	a0,v0
		jal	norm_l
		move	s0,v0
		move	a1,v0
		move	a0,s0
		jal	L_shl
		sw	v0,128(sp)
		
		sra	v1,v0,0x10
		sra	a3,v0,0x1
		sll	v0,v1,0xf
		subu	a0,a3,v0
		sh	v1,24(sp)
		sh	a0,26(sp)			
		
		lw	t5,188(sp)
		lw	t3,184(sp)
		addiu	t4,t5,4
		addiu	t2,t3,4
		li	t1,4
		sw	t4,132(sp)
		sw	t2,136(sp)
		sw	t1,140(sp)
Levinson364:	li	s4,1
		slt	s6,s4,s5
		beqz	s6,Levinson476
		move	s3,zero
		lw	s8,140(sp)
		lw	t8,188(sp)
		lw	s7,184(sp)
		addu	t9,sp,s8
		addiu	s0,t9,14
		addiu	s2,t8,2
		addiu	s1,s7,2
		
/*------------------------------------------------------------------------*
  		for(j=1; j<i; j++)
      			t0 = L_add(t0, Mpy_32(Rh[j], Rl[j], Ah[i-j], Al[i-j]));
*------------------------------------------------------------------------*/		
		
Levinson408:	lh	a0,0(s1)
		lh	a1,0(s2)
		lh	a2,16(s0)
		lh	a3,40(s0)
		addiu	s1,s1,2		
		li		t1,1
		mpymvrr		acc0,sx0,sy0,shnone,x0,gpra0,y0,gpra3
		mpymvrr		acc1,sx0,sy0,shnone,x0,gpra1,y0,gpra2
		satacc		mac0,acc0,round_disable, sat_disable, satp32, 12
		satacc		mac0,acc1,round_disable, sat_disable, satp32, 12
		satacc		mac0,acc0,round_disable, sat_enable, satp16, 3
		satacc		mac0,acc1,round_disable, sat_enable, satp16, 3
		mvp		gprt0,udi_mode
		mvp		gprt0,udi_mode
		mvp		gprt2,udi_mac0acc0lo
		mvp		gprt3,udi_mac0acc1lo
		mpymvrr		acc0,sx0,sy0,shsat,x0,gpra0,y0,gpra2
		macmvrr		acc0,sx0,sy0,shsat,x0,gprt2,y0,gprt1
		macmvrr		acc0,sx0,sy0,shsat,x0,gprt3,y0,gprt1
		mvp		gprt0,udi_mode
		mvp		gprt0,udi_mode
		mvp		udi_mac1acc0lo,gprs3		
		addacc		acc0,acc0
		satacc		mac0,acc1,round_disable, sat_enable, satp32, 0
		mvp		gprt0,udi_mode
		mvp		gprt0,udi_mode
		mvp		gprt0,udi_mode
		mvp		gprs3,udi_mac0acc0lo
		addiu	s4,s4,1
		
		slt	ra,s4,s5
		addiu	s2,s2,2
		bnez	ra,Levinson408
		addiu	s0,s0,-2
Levinson476:	
		li	a2,0x3fffffff
		li	v0,4
		lui	a1,0xc000
		addiu	t0,v0,-1
Levinson500:	sll	v0,t0,0x10
		slt	a3,a2,s3
		slt	a0,s3,a1
		sra	v0,v0,0x10
		bnez	a3,Levinson1384
		sll	s3,s3,0x1
		bnez	a0,Levinson1404
		li	t9,1
		bgtz	v0,Levinson500
		addiu	t0,v0,-1
Levinson540:	lw	a2,132(sp)
		lw	t1,136(sp)
		lh	a1,0(a2)
		jal	L_Comp
		lh	a0,0(t1)
		move	a1,v0
		jal	L_add
		move	a0,s3
		move	a0,v0
		jal	L_abs
		move	s0,v0
		lh	a1,24(sp)
		lh	a2,26(sp)
		jal	Div_32
		move	a0,v0
		blez	s0,Levinson616
		nop
		jal	L_negate
		move	a0,v0
Levinson616:	lw	a1,128(sp)
		jal	L_shl
		move	a0,v0
		move	a0,v0
		move	s8,v0
		
		sra	v1,a0,0x10
		sra	a3,a0,0x1
		sll	v0,v1,0xf
		subu	a0,a3,v0
		sh	v1,20(sp)
		sh	a0,22(sp)			
		
		lhu	t3,20(sp)
		lw	t6,140(sp)
		lw	t5,196(sp)
		sll	t2,t3,0x10
		addu	t4,t6,t5
		sra	a0,t2,0x10
		jal	abs_s
		sh	t3,-2(t4)
		move	a0,v0
		jal	sub
		li	a1,32750
		bgtz	v0,Levinson1416
		li	s3,1
		slt	v1,s3,s5
		addiu	s6,sp,80
		beqz	v1,Levinson828
		addiu	s4,sp,104
		lw	a1,140(sp)
		addiu	s7,sp,16
		addu	a0,a1,s7
		addiu	s2,a0,-2
		
/*------------------------------------------------------------------------*
  		for(j=1; j<i; j++)
    		{
      			t0 = Mpy_32(Kh, Kl, Ah[i-j], Al[i-j]);
      			t0 = L_add(t0, L_Comp(Ah[j], Al[j]));
      			L_Extract(t0, &Anh[j], &Anl[j]);
    		}
*------------------------------------------------------------------------*/		
		
Levinson732:	lh	a3,40(s2)
		lh	a2,16(s2)
		lh	a0,20(sp)
		lh	a1,22(sp)
		sll	s0,s3,0x1		
		li		t1,0x10001		
		mpymvrr		acc0,sx0,sy0,shnone,x0,gpra0,y0,gpra3
		mpymvrr		acc1,sx0,sy0,shnone,x0,gpra1,y0,gpra2
		satacc		mac0,acc0,round_disable, sat_disable, satp32, 12
		satacc		mac0,acc1,round_disable, sat_disable, satp32, 12
		satacc		mac0,acc0,round_disable, sat_enable, satp16, 3
		satacc		mac0,acc1,round_disable, sat_enable, satp16, 3
		mvp		gprt0,udi_mode
		mvp		gprt0,udi_mode
		mvp		gprt2,udi_mac0acc0lo
		mvp		gprt3,udi_mac0acc1lo
		mpymvrr		acc0,sx0,sy0,shsat,x0,gpra0,y0,gpra2
		macmvrr		acc0,sx0,sy0,shsat,x0,gprt2,y0,gprt1
		macmvrr		acc0,sx0,sy0,shsat,x0,gprt3,y0,gprt1	
		
		addu	a2,s0,s7
		lh	a1,40(a2)
		lh	a0,16(a2)		
		sll	a0,a0,0x10
				
		srl	v0,a0,31		
		move	t5,zero
		li	t2,0xff
		movn	t5,t2,v0		
		mvp	udi_mac1acc1hi,gprt5
				
		sll	a1,a1,0x10
		mvp		udi_mac1acc1lo,gpra0
		macmvrr		acc1,sx0,sy0,shsat,x0,gpra1,y0,gprt1
		mvp		gprt0,udi_mode
		mvp		gprt0,udi_mode
		mvp		gprt0,udi_mode		
		addacc		acc0,acc1
		satacc		mac0,acc1,round_disable, sat_enable, satp32, 0
		mvp		gprt0,udi_mode
		mvp		gprt0,udi_mode
		mvp		gprt0,udi_mode
		mvp		gpra0,udi_mac0acc0lo
		
		addu	a2,s4,s0
		addu	a1,s6,s0		
		sra	v1,a0,0x10
		sra	a3,a0,0x1
		sll	v0,v1,0xf
		subu	a0,a3,v0
		sh	v1,0(a1)
		sh	a0,0(a2)	
				
		addiu	s3,s3,1
		slt	a3,s3,s5
		bnez	a3,Levinson732
		addiu	s2,s2,-2
		
Levinson828:	lw	t2,140(sp)
		sra	a0,s8,0x4
		addu	a2,s4,t2
		//jal	L_Extract
		addu	a1,s6,t2
		
		sra	v1,a0,0x10
		sra	a3,a0,0x1
		sll	v0,v1,0xf
		subu	a0,a3,v0
		sh	v1,0(a1)
		sh	a0,0(a2)			
		
		lh	a1,22(sp)
		lh	a0,20(sp)
		move	a3,a1
		//jal	Mpy_32
		move	a2,a0
/*------------------------------------------------------------------------*
  		t0 = Mpy_32(Kh ,Kl, Kh, Kl);
*------------------------------------------------------------------------*/		
		li			t1,1
		mpymvrr		acc0,sx0,sy0,shnone,x0,gpra0,y0,gpra3
		mpymvrr		acc1,sx0,sy0,shnone,x0,gpra1,y0,gpra2
		satacc		mac0,acc0,round_disable, sat_disable, satp32, 12
		satacc		mac0,acc1,round_disable, sat_disable, satp32, 12
		satacc		mac0,acc0,round_disable, sat_enable, satp16, 3
		satacc		mac0,acc1,round_disable, sat_enable, satp16, 3
		mvp			gprt0,udi_mode
		mvp			gprt0,udi_mode
		mvp			gprt2,udi_mac0acc0lo
		mvp			gprt3,udi_mac0acc1lo
		mpymvrr		acc0,sx0,sy0,shsat,x0,gpra0,y0,gpra2
		macmvrr		acc0,sx0,sy0,shsat,x0,gprt2,y0,gprt1
		macmvrr		acc0,sx0,sy0,shsat,x0,gprt3,y0,gprt1
		mvp			gprt0,udi_mode
		mvp			gprt0,udi_mode
		mvp			gprt0,udi_mode
		mvp		gprv0,udi_mac0acc0lo
		
		jal	L_abs
		move	a0,v0
		lui	t1,0x7fff
		ori	a0,t1,0xffff
		jal	L_sub
		move	a1,v0
		move	a0,v0		
		sra	v1,a0,0x10
		sra	a3,a0,0x1
		sll	v0,v1,0xf
		subu	a0,a3,v0
		sh	v1,16(sp)
		sh	a0,18(sp)
		lh	a3,18(sp)
		lh	a2,16(sp)
		lh	a1,26(sp)
		lh	a0,24(sp)
/*------------------------------------------------------------------------*
  		t0 = Mpy_32(alp_h , alp_l, hi, lo);
*------------------------------------------------------------------------*/		
		li		t1,1
		mpymvrr		acc0,sx0,sy0,shnone,x0,gpra0,y0,gpra3
		mpymvrr		acc1,sx0,sy0,shnone,x0,gpra1,y0,gpra2
		satacc		mac0,acc0,round_disable, sat_disable, satp32, 12
		satacc		mac0,acc1,round_disable, sat_disable, satp32, 12
		satacc		mac0,acc0,round_disable, sat_enable, satp16, 3
		satacc		mac0,acc1,round_disable, sat_enable, satp16, 3
		mvp		gprt0,udi_mode
		mvp		gprt0,udi_mode
		mvp		gprt2,udi_mac0acc0lo
		mvp		gprt3,udi_mac0acc1lo
		mpymvrr		acc0,sx0,sy0,shsat,x0,gpra0,y0,gpra2
		macmvrr		acc0,sx0,sy0,shsat,x0,gprt2,y0,gprt1
		macmvrr		acc0,sx0,sy0,shsat,x0,gprt3,y0,gprt1
		mvp		gprt0,udi_mode
		mvp		gprt0,udi_mode
		mvp		gprt0,udi_mode
		mvp		gprv0,udi_mac0acc0lo		
	
		
		move	a0,v0
		jal	norm_l
		move	s2,v0
		move	a1,v0
		move	a0,s2
		jal	L_shl
		move	s4,v0
		move	a0,v0		
		sra	v1,a0,0x10
		sra	a3,a0,0x1
		sll	v0,v1,0xf
		subu	a0,a3,v0
		sh	v1,24(sp)
		sh	a0,26(sp)	
		
		lw	a0,128(sp)
		jal	wg_add
		move	a1,s4
		sw	v0,128(sp)
		blez	s5,Levinson1040
		li	a2,1
		addiu	a1,sp,18
Levinson1000:	addiu	t7,a2,1
		sll	t6,t7,0x10
		lhu	t5,64(a1)
		lhu	t4,88(a1)
		sra	a2,t6,0x10
		slt	t3,s5,a2
		sh	t5,16(a1)
		sh	t4,40(a1)
		beqz	t3,Levinson1000
		addiu	a1,a1,2
Levinson1040:	addiu	v1,s5,1
		lw	ra,140(sp)
		lw	t9,136(sp)
		lw	s7,132(sp)
		sll	s5,v1,0x10
		sra	s5,s5,0x10
		addiu	s8,ra,2
		addiu	t8,t9,2
		addiu	s6,s7,2
		slti	s3,s5,11
		sw	s8,140(sp)
		sw	t8,136(sp)
		bnez	s3,Levinson364
		sw	s6,132(sp)
		lh	a0,24(sp)
		lw	a1,128(sp)
		sra	v0,a0,a1
		li	s2,1
		lw	s1,200(sp)
		lw	s0,192(sp)
		lw	a3,192(sp)
		sh	v0,0(s1)
		lui	t0,0x3fff
		li	v0,4096
		la	a0,old_A
		sh	v0,0(s0)
		ori	s6,t0,0xffff
		addiu	s0,a3,2
		addiu	s3,a0,2
		li	s7,1
		lui	s5,0x7fff
		lui	s4,0xc000
		b	Levinson1228
		addiu	s1,sp,18
Levinson1180:	
		la	t0,Overflow
		sw	s7,0(t0)
Levinson1184:	
		addiu	s1,s1,2

		srl	v0,a0,31		
		move	t5,zero
		li	t2,0xff
		movn	t5,t2,v0		
		mvp	udi_mac0acc0hi,gprt5		
		mvp	udi_mac0acc0lo,gpra0
		satacc	mac0, acc0, round_enable, sat_enable, satp32, 16
		mvp	gprt1,udi_mode
		mvp	gprt1,udi_mode
		mvp	gprt1,udi_mode
		mvp	gprv0,udi_mac0acc0lo

		addiu	t2,s2,1
		sll	s2,t2,0x10
		sra	s2,s2,0x10
		slti	t1,s2,11
		sh	v0,0(s0)
		sh	v0,0(s3)
		addiu	s0,s0,2
		beqz	t1,Levinson1316
		addiu	s3,s3,2
		
/*------------------------------------------------------------------------*
  		for(i=1; i<=M; i++)
  		{
    			t0   = L_Comp(Ah[i], Al[i]);
    			old_A[i] = A[i] = ROUND(L_shl(t0, 1));
  		}
*------------------------------------------------------------------------*/		
		
Levinson1228:	lh	a0,16(s1)
		jal	L_Comp
		lh	a1,40(s1)
		slt	a2,s6,v0
		slt	a1,v0,s4
		bnez	a2,Levinson1180
		ori	a0,s5,0xffff
		beqz	a1,Levinson1184
		sll	a0,v0,0x1
		lui	a0,0x8000
		la	t0,Overflow
		sw	s7,0(t0)		
		srl	v0,a0,31		
		move	t5,zero
		li	t2,0xff
		movn	t5,t2,v0		
		mvp	udi_mac0acc0hi,gprt5		
		mvp	udi_mac0acc0lo,gpra0
		satacc	mac0, acc0, round_enable, sat_enable, satp16, 16
		mvp	gprt1,udi_mode
		mvp	gprt1,udi_mode
		mvp	gprt1,udi_mode
		mvp	gprv0,udi_mac0acc0lo
		
		addiu	t2,s2,1
		sll	s2,t2,0x10
		sra	s2,s2,0x10
		slti	t1,s2,11
		sh	v0,0(s0)
		addiu	s1,s1,2
		sh	v0,0(s3)
		addiu	s0,s0,2
		bnez	t1,Levinson1228
		addiu	s3,s3,2
Levinson1316:	lw	t4,196(sp)
		lw	ra,180(sp)
		lhu	t5,0(t4)
		lw	s8,176(sp)
		la	s0,old_rc
		sh	t5,0(s0)
		lhu	s4,2(t4)
		lw	s7,172(sp)
		sh	s4,2(s0)
		lw	s6,168(sp)
		lw	s5,164(sp)
		lw	s4,160(sp)
		lw	s3,156(sp)
		lw	s2,152(sp)
		lw	s1,148(sp)
		lw	s0,144(sp)
		jr	ra
		addiu	sp,sp,184
		
Levinson1384:	lui	ra,0x7fff
		li	s8,1
		ori	s3,ra,0xffff
		la	t0,Overflow
		sw	s8,0(t0)
		b	Levinson540
		nop
Levinson1404:	lui	s3,0x8000
		la	t0,Overflow
		sw	t9,0(t0)
		b	Levinson540
		nop
Levinson1416:	
		lw	a2,192(sp)
		la	a1,old_A
		move	a0,zero
Levinson1432:	addiu	s3,a0,1
		sll	s2,s3,0x10
		lhu	s1,0(a1)
		sra	a0,s2,0x10
		slti	t7,a0,11
		sh	s1,0(a2)
		addiu	a1,a1,2
		bnez	t7,Levinson1432
		addiu	a2,a2,2
		lw	s6,196(sp)
		la	s1,old_rc
		lhu	t8,0(s1)
		lw	ra,180(sp)
		sh	t8,0(s6)
		lhu	s7,2(s1)
		lw	s8,176(sp)
		sh	s7,2(s6)
		lw	s5,164(sp)
		lw	s7,172(sp)
		lw	s6,168(sp)
		lw	s4,160(sp)
		lw	s3,156(sp)
		lw	s2,152(sp)
		lw	s1,148(sp)
		lw	s0,144(sp)
		jr	ra
		addiu	sp,sp,184
//END( Levinson )
		.end Levinson
