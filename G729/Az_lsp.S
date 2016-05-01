/*
***************************************************************************
*                  Copyright (c) 2006 vBridge Microsystem, Inc.  
*                       Unpublished & Not for Publication
*                               All Rights Reserved                   
*   
* File        	: Az_lsp.S                     
*                                                                         
* Description	: Compute the LSPs from  the LPC coefficients  (order=10)     
*             
* Date        	: Jun 7, 2006                                                           
***************************************************************************
*/
#include <mips/asm.h>
#include <mips/udi.h>
#include <mips/regdef.h>
#include "mips.h"
#include "archdefs.h"
#include "mipsudi_inst.h"

      .set noat
      .set noreorder
/*------------------------------------------------------------------------*
void Az_lsp(
  Word16 a[],        // (i) Q12 : predictor coefficients              
  Word16 lsp[],      // (o) Q15 : line spectral pairs                 
  Word16 old_lsp[]   // (i)     : old lsp[] (in case not found 10 roots) 
)
{
 Word16 i, j, nf, ip;
 Word16 xlow, ylow, xhigh, yhigh, xmid, ymid, xint;
 Word16 x, y, sign, exp;
 Word16 *coef;
 Word16 f1[M/2+1], f2[M/2+1];
 Word32 t0, L_temp;
 Flag   ovf_coef;
 Word16 (*pChebps)(Word16 x, Word16 f[], Word16 n);

//-------------------------------------------------------------
//  find the sum and diff. pol. F1(z) and F2(z)                
//    F1(z) <--- F1(z)/(1+z**-1) & F2(z) <--- F2(z)/(1-z**-1)  
//                                                             
// f1[0] = 1.0;                                                
// f2[0] = 1.0;                                                
//                                                             
// for (i = 0; i< NC; i++)                                     
// {                                                           
//   f1[i+1] = a[i+1] + a[M-i] - f1[i] ;                       
//   f2[i+1] = a[i+1] - a[M-i] + f2[i] ;                       
// }                                                           
//-------------------------------------------------------------

 ovf_coef = 0;
 pChebps = Chebps_11;

 f1[0] = 2048;          // f1[0] = 1.0 is in Q11 
 f2[0] = 2048;          // f2[0] = 1.0 is in Q11 

 for (i = 0; i< NC; i++)
 {
   Overflow = 0;
   t0 = L_mult(a[i+1], 16384);          // x = (a[i+1] + a[M-i]) >> 1        
   t0 = L_mac(t0, a[M-i], 16384);       //    -> From Q12 to Q11             
   x  = extract_h(t0);
   if ( Overflow ) {
     ovf_coef = 1;      }

   Overflow = 0;
   f1[i+1] = sub(x, f1[i]);    // f1[i+1] = a[i+1] + a[M-i] - f1[i] 
   if ( Overflow ) {
     ovf_coef = 1;      }

   Overflow = 0;
   t0 = L_mult(a[i+1], 16384);          // x = (a[i+1] - a[M-i]) >> 1        
   t0 = L_msu(t0, a[M-i], 16384);       //    -> From Q12 to Q11             
   x  = extract_h(t0);
   if ( Overflow ) {
     ovf_coef = 1;      }

   Overflow = 0;
   f2[i+1] = add(x, f2[i]);    // f2[i+1] = a[i+1] - a[M-i] + f2[i] 
   if ( Overflow ) {
     ovf_coef = 1;      }
 }

 if ( ovf_coef ) {
   // printf("===== OVF ovf_coef =====\n");

   pChebps = Chebps_10;

   f1[0] = 1024;          // f1[0] = 1.0 is in Q10 
   f2[0] = 1024;          // f2[0] = 1.0 is in Q10 

   for (i = 0; i< NC; i++)
   {
     t0 = L_mult(a[i+1], 8192);          // x = (a[i+1] + a[M-i]) >> 1        
     t0 = L_mac(t0, a[M-i], 8192);       //    -> From Q11 to Q10             
     x  = extract_h(t0);
     f1[i+1] = sub(x, f1[i]);            // f1[i+1] = a[i+1] + a[M-i] - f1[i] 

     t0 = L_mult(a[i+1], 8192);          // x = (a[i+1] - a[M-i]) >> 1        
     t0 = L_msu(t0, a[M-i], 8192);       //    -> From Q11 to Q10             
     x  = extract_h(t0);
     f2[i+1] = add(x, f2[i]);            // f2[i+1] = a[i+1] - a[M-i] + f2[i] 
   }
 }

//-------------------------------------------------------------
// find the LSPs using the Chebichev pol. evaluation           
//-------------------------------------------------------------

 nf=0;          // number of found frequencies 
 ip=0;          // indicator for f1 or f2      

 coef = f1;

 xlow = grid[0];
 ylow = (*pChebps)(xlow, coef, NC);

 j = 0;
 while ( (nf < M) && (j < GRID_POINTS) )
 {
   j =add(j,1);
   xhigh = xlow;
   yhigh = ylow;
   xlow  = grid[j];
   ylow  = (*pChebps)(xlow,coef,NC);

   L_temp = L_mult(ylow ,yhigh);
   if ( L_temp <= (Word32)0)
   {

     // divide 2 times the interval 

     for (i = 0; i < 2; i++)
     {
       xmid = add( shr(xlow, 1) , shr(xhigh, 1)); // xmid = (xlow + xhigh)/2 

       ymid = (*pChebps)(xmid,coef,NC);

       L_temp = L_mult(ylow,ymid);
       if ( L_temp <= (Word32)0)
       {
         yhigh = ymid;
         xhigh = xmid;
       }
       else
       {
         ylow = ymid;
         xlow = xmid;
       }
     }

    //-------------------------------------------------------------
    // Linear interpolation                                        
    //    xint = xlow - ylow*(xhigh-xlow)/(yhigh-ylow);            
    //-------------------------------------------------------------

     x   = sub(xhigh, xlow);
     y   = sub(yhigh, ylow);

     if(y == 0)
     {
       xint = xlow;
     }
     else
     {
       sign= y;
       y   = abs_s(y);
       exp = norm_s(y);
       y   = shl(y, exp);
       y   = div_s( (Word16)16383, y);
       t0  = L_mult(x, y);
       t0  = L_shr(t0, sub(20, exp) );
       y   = extract_l(t0);            // y= (xhigh-xlow)/(yhigh-ylow) in Q11 

       if(sign < 0) y = negate(y);

       t0   = L_mult(ylow, y);                  // result in Q26 
       t0   = L_shr(t0, 11);                    // result in Q15 
       xint = sub(xlow, extract_l(t0));         // xint = xlow - ylow*y 
     }

     lsp[nf] = xint;
     xlow    = xint;
     nf =add(nf,1);

     if(ip == 0)
     {
       ip = 1;
       coef = f2;
     }
     else
     {
       ip = 0;
       coef = f1;
     }
     ylow = (*pChebps)(xlow,coef,NC);

   }
 }

 // Check if M roots found 

 if( sub(nf, M) < 0)
 {
    for(i=0; i<M; i++)
    {
      lsp[i] = old_lsp[i];
    }

 // printf("\n !!Not 10 roots found in Az_lsp()!!!\n"); 
 }

 return;
}
*------------------------------------------------------------------------*/


 
LEAF( Az_lsp )

		addiu	sp,sp,-104
		sw	s5,84(sp)
		move	s5,a0
		li	v1,2048
		la	v0,Chebps_11
		sw	s4,80(sp)
		sw	s3,76(sp)
		sw	s2,72(sp)
		sw	ra,100(sp)
		sw	s8,96(sp)
		sw	s7,92(sp)
		sw	s6,88(sp)
		sw	s1,68(sp)
		sw	s0,64(sp)
		sh	v1,32(sp)
		sw	a1,108(sp)
		sw	a2,112(sp)
		sw	v0,56(sp)
		sh	v1,16(sp)
		move	s4,zero
		move	s3,zero
		addiu	s2,s5,20
		
/*------------------------------------------------------------------------*
  		for (i = 0; i< NC; i++)
 		{
   			Overflow = 0;
   			t0 = L_mult(a[i+1], 16384);        
   			t0 = L_mac(t0, a[M-i], 16384);       
   			x  = extract_h(t0);

   			f1[i+1] = sub(x, f1[i]);    

   			t0 = L_mult(a[i+1], 16384);       
   			t0 = L_msu(t0, a[M-i], 16384);     
   			x  = extract_h(t0);

   			f2[i+1] = add(x, f2[i]); 
   			if ( Overflow ) {
     				ovf_coef = 1;      }
 		}      
*------------------------------------------------------------------------*/		
Az_lsp92:	sll	s6,s3,0x1
		addu	s1,s6,s5
		
		
		lh	a0,2(s1)
		li	a1,16384
		sw	zero,Overflow		
		lh	t6,0(s2)
		mpymvrr	acc0,sx0,sy0,shsat,x0,gpra0,y0,gpra1
		macmvrr	acc0,sx0,sy0,shsat,x0,gprt6,y0,gpra1
		mvp	gprt0,udi_mode
		mvp	gprt0,udi_mode
		mvp	gprt0,udi_mode		
		mvp	gprv0,udi_mac0acc0lo
		addiu	t1,sp,16
		addu	s0,s6,t1
		sra	t2,v0,0x10
		lh	t0,0(s0)		
		subu	t2,t2,t0
		mvp	udi_mac0acc0lo,gprt2
		srl	v0,t2,31		
		move	t5,zero
		li	t2,0xff
		movn	t5,t2,v0		
		mvp	udi_mac0acc0hi,gprt5			
		satacc	mac0,acc0,round_disable, sat_enable, satp16, 0
		mpymvrr	acc0,sx0,sy0,shsat,x0,gpra0,y0,gpra1
		msumvrr	acc0,sx0,sy0,shsat,x0,gpra1,y0,gprt6
		mvp	gprt0,udi_mode		
		mvp	gprv0,udi_mac0acc0lo
		sh	v0,2(s0)		
		mvp	gprt0,udi_mode		
		mvp	gprv0,udi_mac0acc0lo
		sra	a0,v0,0x10
		lh	a1,16(s0)
		addu	a0,a0,a1
		mvp	udi_mac0acc0lo,gpra0
		srl	v0,a0,31		
		move	t5,zero
		li	t2,0xff
		movn	t5,t2,v0		
		mvp	udi_mac0acc0hi,gprt5			
		satacc	mac0,acc0,round_disable, sat_enable, satp16, 0
		mvp	gprt0,udi_mode
		mvp	gprt0,udi_mode
		mvp	gprt0,udi_mode		
		mvp	gprv0,udi_mac0acc0lo
		addiu	t7,s3,1
		sll	t6,t7,0x10
		sra	s3,t6,0x10
		la	a2,Overflow
		lw	a3,0(a2)
		li	a2,1
		slti	a1,s3,5
		addiu	s2,s2,-2
		sh	v0,18(s0)
		bnez	a1,Az_lsp92
		movn	s4,a2,a3
		
		
		beqzl	s4,Az_lsp440
		nop
		li	t2,1024
		la	t3,Chebps_10
		sh	t2,32(sp)
		sw	t3,56(sp)
		sh	t2,16(sp)
		move	s3,zero
		addiu	s2,s5,20
		
/*------------------------------------------------------------------------*
  		for (i = 0; i< NC; i++)
   		{
     			t0 = L_mult(a[i+1], 8192); 
     			t0 = L_mac(t0, a[M-i], 8192); 
     			x  = extract_h(t0);
     			f1[i+1] = sub(x, f1[i]); 

     			t0 = L_mult(a[i+1], 8192);  
     			t0 = L_msu(t0, a[M-i], 8192);  
     			x  = extract_h(t0);
     			f2[i+1] = add(x, f2[i]); 
   		}     
*------------------------------------------------------------------------*/		
		
Az_lsp292:	sll	s8,s3,0x1
		addu	s7,s8,s5
		lh	a0,2(s7)
		li	a1,8192
		lh	t6,0(s2)		
		mpymvrr	acc0,sx0,sy0,shsat,x0,gpra0,y0,gpra1
		macmvrr	acc0,sx0,sy0,shsat,x0,gprt6,y0,gpra1
		mvp		gprt0,udi_mode
		mvp		gprt0,udi_mode
		mvp		gprt0,udi_mode		
		mvp		gprv0,udi_mac0acc0lo
		addiu	t8,sp,16
		addu	s4,s8,t8
		sra	t2,v0,0x10
		lh	t5,0(s4)
		subu	t2,t2,t5
		mvp	udi_mac0acc0lo,gprt2
		srl	v0,t2,31		
		move	t5,zero
		li	t2,0xff
		movn	t5,t2,v0		
		mvp	udi_mac0acc0hi,gprt5			
		satacc		mac0,acc0,round_disable, sat_enable, satp16, 0
		mpymvrr	acc0,sx0,sy0,shsat,x0,gpra0,y0,gpra1
		msumvrr	acc0,sx0,sy0,shsat,x0,gpra1,y0,gprt6
		mvp		gprt0,udi_mode		
		mvp		gprv0,udi_mac0acc0lo
		sh	v0,2(s4)		
		mvp		gprt0,udi_mode		
		mvp		gprv0,udi_mac0acc0lo
		sra	a0,v0,0x10
		lh	a1,16(s4)
		addu	a0,a0,a1
		mvp	udi_mac0acc0lo,gpra0
		srl	v0,a0,31		
		move	t5,zero
		li	t2,0xff
		movn	t5,t2,v0		
		mvp	udi_mac0acc0hi,gprt5			
		satacc		mac0,acc0,round_disable, sat_enable, satp16, 0
		mvp		gprt0,udi_mode
		mvp		gprt0,udi_mode
		mvp		gprt0,udi_mode		
		mvp		gprv0,udi_mac0acc0lo
		addiu	t7,s3,1
		sll	t6,t7,0x10
		sra	s3,t6,0x10
		slti	t5,s3,5
		addiu	s2,s2,-2
		bnez	t5,Az_lsp292
		sh	v0,18(s4)
		
		
Az_lsp440:	la	s5,grid
		lh	s4,0(s5)
		lw	s2,56(sp)
		addiu	s8,sp,16
		move	a0,s4
		move	a1,s8
		jalr	s2		//ylow = (*pChebps)(xlow, coef, NC);
		li	a2,5
		move	s2,v0
		move	s7,zero
		sw	zero,48(sp)
		b	Az_lsp508
		sw	zero,52(sp)
		
Az_lsp488:	slti	t9,s7,50
		slti	s5,ra,10
		and	t8,s5,t9
		beqz	t8,Az_lsp856
		lw	a0,48(sp)
Az_lsp508:	
		addu	v0,s7,1
		la	v1,grid
		move	s7,v0
		sll	v0,v0,0x1
		addu	ra,v0,v1
		move	s5,s4
		lh	s4,0(ra)
		lw	t9,56(sp)
		move	a1,s8
		move	a0,s4
		jalr	t9		//ylow  = (*pChebps)(xlow,coef,NC);
		li	a2,5
/*------------------------------------------------------------------------*
  		L_temp = L_mult(ylow ,yhigh);   
*------------------------------------------------------------------------*/
		move	s3,s2
		move	s2,v0		
		mpymvrr	acc0,sx0,sy0,shsat,x0,gprv0,y0,gprs3
		mvp		gprt0,udi_mode
		mvp		gprt0,udi_mode
		mvp		gprt0,udi_mode		
		mvp		gprv0,udi_mac0acc0lo		
		bgtz	v0,Az_lsp488
		lw	ra,48(sp)
		move	s6,zero
/*------------------------------------------------------------------------*
		for (i = 0; i < 2; i++)
     		{
       			xmid = add( shr(xlow, 1) , shr(xhigh, 1));
       			ymid = (*pChebps)(xmid,coef,NC);
       			L_temp = L_mult(ylow,ymid);
       			if ( L_temp <= (Word32)0)
       			{
         			yhigh = ymid;
         			xhigh = xmid;
       			}
       			else
       			{
         			ylow = ymid;
         			xlow = xmid;
       			}
       		}
*------------------------------------------------------------------------*/

		sra	t0,s4,0x1
Az_lsp604:	sra	a2,s5,0x1
		sll	s0,t0,0x10
		sll	a1,a2,0x10
		sra	a0,s0,0x10
		jal	wg_add
		sra	a1,a1,0x10
		lw	a3,56(sp)
		move	a0,v0
		move	a1,s8
		li	a2,5
		jalr	a3		//ymid = (*pChebps)(xmid,coef,NC);
		move	s1,v0
		move	s0,v0		
		mpymvrr	acc0,sx0,sy0,shsat,x0,gprv0,y0,gprs2
		mvp		gprt0,udi_mode
		mvp		gprt0,udi_mode
		mvp		gprt0,udi_mode		
		mvp		gprv0,udi_mac0acc0lo		
		blezl	v0,Az_lsp928
		move	s3,s0
		
		move	s2,s0
		move	s4,s1
Az_lsp684:	addiu	t1,s6,1
		sll	s6,t1,0x10
		sra	s6,s6,0x10
		slti	s1,s6,2
		bnez	s1,Az_lsp604
		sra	t0,s4,0x1
/*------------------------------------------------------------------------*
  		x   = sub(xhigh, xlow);
     		y   = sub(yhigh, ylow);  
*------------------------------------------------------------------------*/		
				
		subu	a0,s5,s4
		mvp	udi_mac0acc0lo,gpra0
		srl	v0,a0,31		
		move	t5,zero
		li	t2,0xff
		movn	t5,t2,v0		
		mvp	udi_mac0acc0hi,gprt5			
		satacc		mac0,acc0,round_disable, sat_enable, satp16, 0
		subu	a1,s3,s2
		mvp	udi_mac1acc0lo,gpra1
		srl	v0,a1,31		
		move	t5,zero
		li	t2,0xff
		movn	t5,t2,v0		
		mvp	udi_mac1acc0hi,gprt5			
		satacc		mac1,acc0,round_disable, sat_enable, satp16, 0				
		mvp		gprs5,udi_mac0acc0lo		
		mvp		gprt0,udi_mode
		mvp		gprt0,udi_mode		
		mvp		gprv0,udi_mac1acc0lo
		/*
		subu	s5,s5,s4
		subu	v0,s3,s2
		*/
			
		move	s3,v0
		bnez	v0,Az_lsp936
		move	s0,s4
Az_lsp748:	lw	a0,48(sp)
		lw	t4,108(sp)
		sll	t3,a0,0x1
		addu	t2,t3,t4
		//lw	a0,48(sp)
		sh	s0,0(t2)
		addu	v0,a0,1		//nf =add(nf,1);
		lw	s3,52(sp)
		sw	v0,48(sp)
		bnez	s3,Az_lsp916
		move	s4,s0
		li	t6,1
		sw	t6,52(sp)
		addiu	s8,sp,32
Az_lsp808:	lw	t7,56(sp)
		move	a0,s0
		move	a1,s8
		jalr	t7		//ylow = (*pChebps)(xlow,coef,NC);
		li	a2,5
		lw	a0,48(sp)
		slti	t9,s7,50
		slti	s5,a0,10
		and	t8,s5,t9
		bnez	t8,Az_lsp508
		move	s2,v0
		//lw	a0,48(sp)
		
Az_lsp856:	
		li	a1,10
		bltu	a0,a1,Az_lsp1080
		lw	ra,100(sp)
		lw	s8,96(sp)
		lw	s7,92(sp)
		lw	s6,88(sp)
		lw	s5,84(sp)
		lw	s4,80(sp)
		lw	s3,76(sp)
		lw	s2,72(sp)
		lw	s1,68(sp)
		lw	s0,64(sp)
		jr	ra
		addiu	sp,sp,104
Az_lsp916:	addiu	s8,sp,16
		b	Az_lsp808
		sw	zero,52(sp)
Az_lsp928:	
		b	Az_lsp684
		move	s5,s1
Az_lsp936:	jal	abs_s
		move	a0,v0
		move	a0,v0
		jal	norm_s
		move	s0,v0
		move	a1,v0
		move	a0,s0
		jal	wg_shl
		move	s1,v0
		move	a1,v0
		jal	div_s
		li	a0,16383
/*------------------------------------------------------------------------*
  		t0  = L_mult(x, y); 
*------------------------------------------------------------------------*/		

		mpymvrr	acc0,sx0,sy0,shsat,x0,gprv0,y0,gprs5
		mvp		gprt0,udi_mode
		mvp		gprt0,udi_mode
		mvp		gprt0,udi_mode		
		mvp		gprv0,udi_mac0acc0lo
		move	a1,s1
		li	a0,20
		jal	wg_sub		
		move	s8,v0
		move	a1,v0
		jal	L_shr
		move	a0,s8
		sll	a0,v0,0x10
		sra	v0,a0,0x10
		bltz	s3,Az_lsp1176
		nop
		move	a1,v0
		
/*------------------------------------------------------------------------*
       		t0   = L_mult(ylow, y);  
       		t0   = L_shr(t0, 11); 
       		xint = sub(xlow, extract_l(t0));             
*------------------------------------------------------------------------*/		
		
		
Az_lsp1044:			
		mpymvrr	acc0,sx0,sy0,shsat,x0,gprs2,y0,gpra1
		mvp		gprt0,udi_mode
		mvp		gprt0,udi_mode
		mvp		gprt0,udi_mode		
		mvp		gprv0,udi_mac0acc0lo
		sra	a0,v0,0xb
		sll	v0,a0,0x10
		sra	a1,v0,0x10
		jal	wg_sub
		move	a0,s4
		b	Az_lsp748
		move	s0,v0
Az_lsp1080:	lw	a2,112(sp)
		lw	a1,108(sp)
		move	a0,zero
Az_lsp1092:	addiu	v0,a0,1
		sll	s2,v0,0x10
		lhu	s7,0(a2)
		sra	a0,s2,0x10
		slti	s4,a0,10
		sh	s7,0(a1)
		addiu	a2,a2,2
		bnez	s4,Az_lsp1092
		addiu	a1,a1,2
		
		lw	ra,100(sp)
		lw	s8,96(sp)
		lw	s7,92(sp)
		lw	s6,88(sp)
		lw	s5,84(sp)
		lw	s4,80(sp)
		lw	s3,76(sp)
		lw	s2,72(sp)
		lw	s1,68(sp)
		lw	s0,64(sp)
		jr	ra
		addiu	sp,sp,104
Az_lsp1176:	jal	negate
		move	a0,v0
		b	Az_lsp1044
		move	a1,v0
END( Az_lsp )

