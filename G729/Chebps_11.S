/*
***************************************************************************
*                  Copyright (c) 2006 vBridge Microsystem, Inc.  
*                       Unpublished & Not for Publication
*                               All Rights Reserved                   
*   
* File        	: Chebps_11.S                     
*                                                                         
* Description	: Evaluates the Chebichev polynomial series      
*             
* Date        	: Jun 7, 2006                                                           
***************************************************************************
*/

#include <mips/asm.h>
#include <mips/udi.h>
#include <mips/regdef.h>

#include "mipsudi_inst.h"

      .set noat
      .set noreorder


/*------------------------------------------------------------------------*
static Word16 Chebps_11(Word16 x, Word16 f[], Word16 n)
{
  Word16 i, cheb;
  Word16 b0_h, b0_l, b1_h, b1_l, b2_h, b2_l;
  Word32 t0;

 // Note: All computation are done in Q24. 

  b2_h = 256;                           // b2 = 1.0 in Q24 DPF 
  b2_l = 0;

  t0 = L_mult(x, 512);                  // 2*x in Q24          
  t0 = L_mac(t0, f[1], 4096);           // + f[1] in Q24       
  L_Extract(t0, &b1_h, &b1_l);          // b1 = 2*x + f[1]     

  for (i = 2; i<n; i++)
  {
    t0 = Mpy_32_16(b1_h, b1_l, x);      // t0 = 2.0*x*b1              
    t0 = L_shl(t0, 1);
    t0 = L_mac(t0,b2_h,(Word16)-32768L);// t0 = 2.0*x*b1 - b2         
    t0 = L_msu(t0, b2_l, 1);
    t0 = L_mac(t0, f[i], 4096);         // t0 = 2.0*x*b1 - b2 + f[i]; 

    L_Extract(t0, &b0_h, &b0_l);        // b0 = 2.0*x*b1 - b2 + f[i]; 

    b2_l = b1_l;                        // b2 = b1; 
    b2_h = b1_h;
    b1_l = b0_l;                        // b1 = b0; 
    b1_h = b0_h;
  }

  t0 = Mpy_32_16(b1_h, b1_l, x);        // t0 = x*b1;              
  t0 = L_mac(t0, b2_h,(Word16)-32768L); // t0 = x*b1 - b2          
  t0 = L_msu(t0, b2_l, 1);
  t0 = L_mac(t0, f[i], 2048);           // t0 = x*b1 - b2 + f[i]/2 

  t0 = L_shl(t0, 6);                    // Q24 to Q30 with saturation 
  cheb = extract_h(t0);                 // Result in Q14              


  return(cheb);
}

*------------------------------------------------------------------------*/
LEAF( Chebps_11 )
		addiu	sp,sp,-64
		sll	a0,a0,0x10
		sra	a0,a0,0x10
		sw	s7,52(sp)
		move	s7,a1
		li	a1,512
		sw	ra,56(sp)
		sw	s5,44(sp)
		sw	s4,40(sp)
		sw	s3,36(sp)
		sll	s4,a2,0x10
		sw	s2,32(sp)
		sw	s1,28(sp)
		move	s5,a0
		sw	s6,48(sp)
		sw	s0,24(sp)
		lh	t1,2(s7)
		li	t2,4096
/*------------------------------------------------------------------------*
  		t0 = L_mult(x, 512);
  		t0 = L_mac(t0, f[1], 4096);   
*------------------------------------------------------------------------*/		
		mpymvrr	acc0,sx0,sy0,shsat,x0,gpra0,y0,gpra1
		macmvrr	acc0,sx0,sy0,shsat,x0,gprt1,y0,gprt2
		mvp	gprt0,udi_mode
		mvp	gprt0,udi_mode
		mvp	gprt0,udi_mode
		mvp	gpra0,udi_mac0acc0lo
		sra	s4,s4,0x10
		li	s3,2
		sra	v1,a0,0x10
		sra	a3,a0,0x1
		sll	v0,v1,0xf
		subu	a0,a3,v0
		sh	v1,20(sp)
		sh	a0,22(sp)
		slt	v0,s3,s4
		li	s1,256
		beqz	v0,Chebps_11488
		move	s2,zero
		lhu	a0,20(sp)
		lhu	a1,22(sp)
		addiu	s6,sp,18
		addiu	s0,s7,4
		
/*------------------------------------------------------------------------*
	  	for (i = 2; i<n; i++)
	  	{
	    		t0 = Mpy_32_16(b1_h, b1_l, x);      
	    		t0 = L_shl(t0, 1);
	    		t0 = L_mac(t0,b2_h,(Word16)-32768L);
	    		t0 = L_msu(t0, b2_l, 1);
	    		t0 = L_mac(t0, f[i], 4096);         
	
	    		L_Extract(t0, &b0_h, &b0_l);      
	
	    		b2_l = b1_l;                       
	    		b2_h = b1_h;
	    		b1_l = b0_l;                      
	    		b1_h = b0_h;
	  	}   
*------------------------------------------------------------------------*/		
		li		t1,1
		li	t4,4096
		li	t5,-32768
Chebps_11140:	//sll	a3,a0,0x10
		//sll	a2,a1,0x10
		//sra	a0,a3,0x10
		//sra	a1,a2,0x10
		move	a2,s5		
		
		mpymvrr		acc0,sx0,sy0,shsat,x0,gpra0,y0,gpra2
		mpymvrr		acc1,sx0,sy0,shnone,x0,gpra1,y0,gpra2
		satacc		mac0,acc1,round_disable, sat_disable, satp32, 12
		satacc		mac0,acc1,round_disable, sat_enable, satp16, 3
		mvp		gprt0,udi_mode
		mvp		gprt0,udi_mode
		mvp		gprt2,udi_mac0acc0lo
		mvp		gprt3,udi_mac0acc1lo
		macmvrr		acc0,sx0,sy0,shsat,x0,gprt3,y0,gprt1
		mvp		gprt0,udi_mode
		mvp		gprt0,udi_mode
		mvp		gprt0,udi_mode
		mvp		gpra0,udi_mac0acc0lo
		sll	v0,a0,1			
		mvp	udi_mac0acc0lo,gprv0			
		lh	a1,0(s0)
		macmvrr	acc0,sx0,sy0,shsat,x0,gprs1,y0,gprt5		
		msumvrr	acc0,sx0,sy0,shsat,x0,gprs2,y0,gprt1
		macmvrr	acc0,sx0,sy0,shsat,x0,gpra1,y0,gprt4
		mvp	gprt0,udi_mode
		mvp	gprt0,udi_mode
		mvp	gprt0,udi_mode
		mvp	gprt0,udi_mac0acc0lo
		
		sra	a0,t0,0x10
		sra	a3,t0,0x1
		sll	v0,a0,0xf
		subu	a1,a3,v0
		
				
		addiu	t0,s3,1
		sll	s3,t0,0x10
		sra	s3,s3,0x10
		//lhu	a1,18(sp)
		//lhu	a0,16(sp)
		slt	v1,s3,s4
		lh	s2,22(sp)
		lh	s1,20(sp)
		addiu	s0,s0,2
		sh	a1,22(sp)
		bnez	v1,Chebps_11140
		sh	a0,20(sp)
		
		//sh	a0,16(sp)
		//sh	a1,18(sp)
/*------------------------------------------------------------------------*
	  	t0 = Mpy_32_16(b1_h, b1_l, x);  
  		t0 = L_mac(t0, b2_h,(Word16)-32768L); 
  		t0 = L_msu(t0, b2_l, 1);
  		t0 = L_mac(t0, f[i], 2048);            
*------------------------------------------------------------------------*/		
		
Chebps_11288:	sll	t4,a0,0x10
		sll	t3,a1,0x10
		sra	a0,t4,0x10
		sra	a1,t3,0x10
		move	a2,s5		
		li		t1,1
		mpymvrr		acc0,sx0,sy0,shsat,x0,gpra0,y0,gpra2
		mpymvrr		acc1,sx0,sy0,shnone,x0,gpra1,y0,gpra2
		satacc		mac0,acc1,round_disable, sat_disable, satp32, 12
		satacc		mac0,acc1,round_disable, sat_enable, satp16, 3
		mvp		gprt0,udi_mode
		mvp		gprt0,udi_mode
		mvp		gprt2,udi_mac0acc0lo
		mvp		gprt3,udi_mac0acc1lo
		macmvrr		acc0,sx0,sy0,shsat,x0,gprt3,y0,gprt1
		mvp		gprt0,udi_mode
		mvp		gprt0,udi_mode
		mvp		gprt0,udi_mode
		li	a2,-32768
		li	t2,1
		li	t3,2048	
		sll	t4,s3,0x1
		addu	t5,t4,s7
		lh	a1,0(t5)
		macmvrr	acc0,sx0,sy0,shsat,x0,gprs1,y0,gpra2		
		msumvrr	acc0,sx0,sy0,shsat,x0,gprs2,y0,gprt2
		macmvrr	acc0,sx0,sy0,shsat,x0,gpra1,y0,gprt3
		mvp	gprt0,udi_mode
		mvp	gprt0,udi_mode
		mvp	gprt0,udi_mode
		mvp	gprv0,udi_mac0acc0lo
		
		lui	t1,0x3fff
		la	a0,Overflow
		lw	t0,0(a0)
		move	a0,v0
		ori	a3,t1,0xffff
		li	v0,6
		lui	a2,0xc000
		addiu	t7,v0,-1
Chebps_11396:	sll	t6,t7,0x10
		slt	t5,a3,a0
		slt	a1,a0,a2
		sra	v0,t6,0x10
		bnez	t5,Chebps_11500
		sll	a0,a0,0x1
		bnezl	a1,Chebps_11564
		li	t0,1
		bgtz	v0,Chebps_11396
		addiu	t7,v0,-1
		la	s0,Overflow
		sra	v0,a0,0x10
		sw	t0,0(s0)
		lw	ra,56(sp)
		lw	s7,52(sp)
		lw	s6,48(sp)
		lw	s5,44(sp)
		lw	s4,40(sp)
		lw	s3,36(sp)
		lw	s2,32(sp)
		lw	s1,28(sp)
		lw	s0,24(sp)
		jr	ra
		addiu	sp,sp,64
Chebps_11488:	lhu	a0,20(sp)
		b	Chebps_11288
		lhu	a1,22(sp)
Chebps_11500:	lui	s0,0x7fff
		ori	a0,s0,0xffff
		li	t0,1
		la	s0,Overflow
		sra	v0,a0,0x10
		sw	t0,0(s0)
		lw	ra,56(sp)
		lw	s7,52(sp)
		lw	s6,48(sp)
		lw	s5,44(sp)
		lw	s4,40(sp)
		lw	s3,36(sp)
		lw	s2,32(sp)
		lw	s1,28(sp)
		lw	s0,24(sp)
		jr	ra
		addiu	sp,sp,64
Chebps_11564:	lui	a0,0x8000
		la	s0,Overflow
		sra	v0,a0,0x10
		sw	t0,0(s0)
		lw	ra,56(sp)
		lw	s7,52(sp)
		lw	s6,48(sp)
		lw	s5,44(sp)
		lw	s4,40(sp)
		lw	s3,36(sp)
		lw	s2,32(sp)
		lw	s1,28(sp)
		lw	s0,24(sp)
		jr	ra
		addiu	sp,sp,64
END( Chebps_11 )

