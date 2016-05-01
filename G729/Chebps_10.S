/*
***************************************************************************
*                  Copyright (c) 2006 vBridge Microsystem, Inc.  
*                       Unpublished & Not for Publication
*                               All Rights Reserved                   
*   
* File        	: Chebps_10.S                     
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
static Word16 Chebps_10(Word16 x, Word16 f[], Word16 n)
{
  Word16 i, cheb;
  Word16 b0_h, b0_l, b1_h, b1_l, b2_h, b2_l;
  Word32 t0;

 // Note: All computation are done in Q23. 

  b2_h = 128;                           // b2 = 1.0 in Q23 DPF 
  b2_l = 0;

  t0 = L_mult(x, 256);                  // 2*x in Q23          
  t0 = L_mac(t0, f[1], 4096);           // + f[1] in Q23       
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

  t0 = L_shl(t0, 7);                    // Q23 to Q30 with saturation 
  cheb = extract_h(t0);                 // Result in Q14              


  return(cheb);
}

*------------------------------------------------------------------------*/

LEAF( Chebps_10 )
		addiu	sp,sp,-64
		sll	a0,a0,0x10
		sra	a0,a0,0x10
		sw	a1,68(sp)
		li	a1,256
		sw	ra,60(sp)
		sw	s5,44(sp)
		sw	s4,40(sp)
		sw	s3,36(sp)
		sll	s4,a2,0x10
		sw	s2,32(sp)
		sw	s1,28(sp)
		move	s5,a0
		sw	s8,56(sp)
		sw	s7,52(sp)
		sw	s6,48(sp)
		sw	s0,24(sp)
		lw	v1,68(sp)
		lh	t1,2(v1)
/*------------------------------------------------------------------------*
  		t0 = L_mult(x, 256);          
  		t0 = L_mac(t0, f[1], 4096);  
*------------------------------------------------------------------------*/		
		li	t2,4096		
		mpymvrr	acc0,sx0,sy0,shsat,x0,gpra0,y0,gpra1		
		macmvrr	acc0,sx0,sy0,shsat,x0,gprt1,y0,gprt2
		mvp	gprt0,udi_mode
		mvp	gprt0,udi_mode
		mvp	gprt0,udi_mode
		mvp	gpra0,udi_mac0acc0lo
		sra	s4,s4,0x10
		li	s2,2
		sra	v1,a0,0x10
		sra	a3,a0,0x1
		sll	v0,v1,0xf
		subu	a0,a3,v0
		sh	v1,20(sp)
		sh	a0,22(sp)			
		
		slt	v0,s2,s4
		li	s1,128
		beqz	v0,Chebps_10660
		move	s3,zero
		lw	a2,68(sp)
		lui	a3,0x3fff
		lhu	a0,20(sp)
		lhu	a1,22(sp)
		ori	s7,a3,0xffff
		addiu	s6,sp,18
		li	s8,1
		//b	Chebps_10280
		addiu	s0,a2,4

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
		li	t1,1
		li	a2,-32768
		li	t5,4096
Chebps_10280:	//sll	t3,a0,0x10
		//sll	a1,a1,0x10
		//sra	a0,t3,0x10
		//sra	a1,a1,0x10		
		
		mpymvrr	acc0,sx0,sy0,shsat,x0,gpra0,y0,gprs5
		mpymvrr	acc1,sx0,sy0,shnone,x0,gpra1,y0,gprs5
		satacc	mac0,acc1,round_disable, sat_disable, satp32, 12
		satacc	mac0,acc1,round_disable, sat_enable, satp16, 3
		mvp	gprt0,udi_mode
		mvp	gprt0,udi_mode
		mvp	gprt2,udi_mac0acc0lo
		mvp	gprt3,udi_mac0acc1lo
		macmvrr	acc0,sx0,sy0,shsat,x0,gprt3,y0,gprt1
		mvp	gprt0,udi_mode
		mvp	gprt0,udi_mode
		mvp	gprt0,udi_mode
		mvp	gprv0,udi_mac0acc0lo		
		/*
		slt	t0,s7,v0
		lui	t2,0xc000
		lui	t1,0x7fff
		slt	a2,v0,t2
		bnez	t0,Chebps_10173
		ori	a0,t1,0xffff
		beqz	a2,Chebps_10173
		sll	a0,v0,0x1		
		lui	a0,0x8000
		*/
		sll	a0,v0,0x1		
Chebps_10173:				
		mvp	udi_mac0acc0lo,gpra0			
		lh	a1,0(s0)
		macmvrr	acc0,sx0,sy0,shsat,x0,gprs1,y0,gpra2		
		msumvrr	acc0,sx0,sy0,shsat,x0,gprs3,y0,gprt1
		macmvrr	acc0,sx0,sy0,shsat,x0,gpra1,y0,gprt5
		mvp	gprt0,udi_mode
		mvp	gprt0,udi_mode
		mvp	gprt0,udi_mode
		mvp	gprt0,udi_mac0acc0lo
		
		sra	a0,t0,0x10
		sra	a3,t0,0x1
		sll	v0,a0,0xf
		subu	a1,a3,v0		
		addiu	t0,s2,1
		sll	s2,t0,0x10
		sra	s2,s2,0x10
		slt	t4,s2,s4
		lh	s3,22(sp)
		lh	s1,20(sp)
		addiu	s0,s0,2
		sh	a1,22(sp)
		bnez	t4,Chebps_10280
		sh	a0,20(sp)
/*------------------------------------------------------------------------*
  		t0 = Mpy_32_16(b1_h, b1_l, x);       
  		t0 = L_mac(t0, b2_h,(Word16)-32768L); 
  		t0 = L_msu(t0, b2_l, 1);
  		t0 = L_mac(t0, f[i], 2048);           
    		for(j=7; j>0; j--)
    		{
         		if (t0 > (Word32) 0X3fffffffL)
         		{
            			Overflow = 1;
            			t0 = MAX_32;
            			break;
         		}
         		else
         		{
            			if (t0 < (Word32) 0xc0000000L)
              			{
               				Overflow = 1;
               				t0 = MIN_32;
               				break;
              			}
         		}
	  		t0 <<= 1;
    		}
  
  		cheb = extract_h(t0);               

  		return(cheb);  
*------------------------------------------------------------------------*/		
		
Chebps_10452:	sll	s7,a0,0x10
		sll	s6,a1,0x10
		sra	a0,s7,0x10
		sra	a1,s6,0x10
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
		
		li	a2,-32768
		li	t2,1
		li	t3,2048			
		lw	s1,68(sp)
		sll	s0,s2,0x1
		addu	t7,s0,s1
		lh	a1,0(t7)
		macmvrr	acc0,sx0,sy0,shsat,x0,gprs1,y0,gpra2		
		msumvrr	acc0,sx0,sy0,shsat,x0,gprs3,y0,gprt2
		macmvrr	acc0,sx0,sy0,shsat,x0,gpra1,y0,gprt3
		mvp	gprt0,udi_mode
		mvp	gprt0,udi_mode
		mvp	gprt0,udi_mode
		mvp	gpra0,udi_mac0acc0lo
		
		lui	a3,0x4000
		li	v0,7
		lui	a2,0xc000
		addiu	t8,v0,-1
Chebps_10564:	sll	s5,t8,0x10
		slt	s3,a3,a0
		slt	a1,a0,a2
		sra	v0,s5,0x10
		bnez	s3,Chebps_10672
		sll	a0,a0,0x1
		bnezl	a1,Chebps_10740
		li	t0,1
		bgtz	v0,Chebps_10564
		addiu	t8,v0,-1
		lw	s8,56(sp)
		lw	s7,52(sp)
		sra	v0,a0,0x10
		lw	ra,60(sp)		
		lw	s6,48(sp)
		lw	s5,44(sp)
		lw	s4,40(sp)
		lw	s3,36(sp)
		lw	s2,32(sp)
		lw	s1,28(sp)
		lw	s0,24(sp)
		jr	ra
		addiu	sp,sp,64
		
Chebps_10660:	lhu	a0,20(sp)
		b	Chebps_10452
		lhu	a1,22(sp)
Chebps_10672:	lui	t9,0x7fff
		lw	s8,56(sp)
		lw	s7,52(sp)
		ori	a0,t9,0xffff
		lw	s6,48(sp)
		lw	s5,44(sp)
		sra	v0,a0,0x10
		lw	ra,60(sp)		
		lw	s4,40(sp)
		lw	s3,36(sp)
		lw	s2,32(sp)
		lw	s1,28(sp)
		lw	s0,24(sp)
		jr	ra
		addiu	sp,sp,64
Chebps_10740:	lui	a0,0x8000
		lw	s8,56(sp)
		lw	s7,52(sp)
		sra	v0,a0,0x10		
		lw	ra,60(sp)		
		lw	s6,48(sp)
		lw	s5,44(sp)
		lw	s4,40(sp)
		lw	s3,36(sp)
		lw	s2,32(sp)
		lw	s1,28(sp)
		lw	s0,24(sp)
		jr	ra
		addiu	sp,sp,64
END( Chebps_10 )

