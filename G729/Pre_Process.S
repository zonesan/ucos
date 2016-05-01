/*
***************************************************************************
*                  Copyright (c) 2006 vBridge Microsystem, Inc.  
*                       Unpublished & Not for Publication
*                               All Rights Reserved                   
*   
* File        : Pre_Process.S                     
*                                                                         
* Description:	Function Pre_Process()     
*  Preprocessing of input speech.                                            
*  - 2nd order high pass filter with cut off frequency at 140 Hz.  
*  - Divide input by two. 
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

      .set noat
      .set noreorder


/*------------------------------------------------------------------------*
 * 2nd order high pass filter with cut off frequency at 140 Hz.           *
 * Designed with SPPACK efi command -40 dB att, 0.25 ri.                  *
 *                                                                        *
 * Algorithm:                                                             *
 *                                                                        *
 *  y[i] = b[0]*x[i]/2 + b[1]*x[i-1]/2 + b[2]*x[i-2]/2                    *
 *                     + a[1]*y[i-1]   + a[2]*y[i-2];                     *
 *                                                                        *
 *     b[3] = {0.92727435E+00, -0.18544941E+01, 0.92727435E+00};          *
 *     a[3] = {0.10000000E+01, 0.19059465E+01, -0.91140240E+00};          *
 *                                                                        *
 *  Input are divided by two in the filtering process.                    *
 *-----------------------------------------------------------------------*/
 
/*------------------------------------------------------------------------*
void Pre_Process(
  Word16 signal[],    
  Word16 lg)          
{
  Word16 i, x2;
  Word32 L_tmp, L_tmp1, L_tmp2;
  Word32 L_var1, L_var_out;
  Word16 var2;
  Word32 L_arrondi;

  for(i=0; i<lg; i++)
  {
     x2 = x1;
     x1 = x0;
     x0 = signal[i];

     L_tmp     = Mpy_32_16(y1_hi, y1_lo, a140[1]);
     L_tmp     = L_add(L_tmp, Mpy_32_16(y2_hi, y2_lo, a140[2]));
     L_tmp     = L_mac(L_tmp, x0, b140[0]);
     L_tmp     = L_mac(L_tmp, x1, b140[1]);
     L_tmp     = L_mac(L_tmp, x2, b140[2]);
     
     var2 = 3;    
     for(;var2>0;var2--)
     {
         if (L_tmp > (Word32) 0X3fffffffL)
         {
            Overflow = 1;
            L_tmp = MAX_32;
            break;
         }
         else if (L_tmp < (Word32) 0xc0000000L)
         {
               Overflow = 1;
               L_tmp = MIN_32;
               break;            
         }
         else
         {
         	L_tmp *= 2;
         }
     }
     
     L_arrondi = L_add(L_tmp, (Word32)0x00008000);
     signal[i] = extract_h(L_arrondi);  

     y2_hi = y1_hi;
     y2_lo = y1_lo;
     y1_hi   = extract_h(L_tmp);
     
     if (L_tmp<0)
     {
     	L_tmp1 = ~((~L_tmp) >> 1);
     }
     else
     {
        L_tmp1 = L_tmp >> 1;
     }
     
     L_tmp2  = L_msu( L_tmp1 , y1_hi, 16384 );
     y1_lo   = extract_l( L_tmp2 );

  }
  return;
}
*------------------------------------------------------------------------*/ 




LEAF( Pre_Process )

		addiu	sp,sp,-64
		sw	s6,40(sp)
		sll	s6,a1,0x10
		sra	s6,s6,0x10
		sw	s3,28(sp)
		sw	t9,56(sp)
		sw	s8,52(sp)
		sw	ra,48(sp)
		sw	s7,44(sp)
		sw	s5,36(sp)
		sw	s4,32(sp)
		sw	s2,24(sp)
		sw	s1,20(sp)
		sw	s0,16(sp)
		blez	s6,Pre_Process356
		move	s3,zero
		lui	v0,0x3fff
 		ori	s5,v0,0xffff
		move	s2,a0
		lui	s4,0xc000
		la	s8,a140
		la	t9,b140
		
		li	s7,1
		la	t7,x0_11
		la	t6,x1_12
		la	ra,y1_hi
		la	t5,y2_hi
		la	t0,y1_lo
		la	t8,y2_lo		
		lhu	a2,2(s8)
		lhu	a0,4(s8)
		sll	a0,a0,0x10
		or	t2,a0,a2
Pre_Process76:	
		lhu	a0,0(t7)
		lh	s1,0(t6)
		sh	a0,0(t6)
		lhu	v1,0(s2)
		sh	v1,0(t7)	
	
		lhu	a0,0(ra)
		lhu	v1,0(t5)
		sll	t3,v1,0x10		
		or	t1,a0,t3		
		
		lhu	a0,0(t0)
		lhu	v1,0(t8)
		sll	t4,v1,0x10
		or	t3,a0,t4		
/*------------------------------------------------------------------------*
		L_tmp     = Mpy_32_16(y1_hi, y1_lo, a140[1]);
		L_tmp     = L_add(L_tmp, Mpy_32_16(y2_hi, y2_lo, a140[2]));
		L_tmp     = L_mac(L_tmp, x0, b140[0]);
		L_tmp     = L_mac(L_tmp, x1, b140[1]);
		L_tmp     = L_mac(L_tmp, x2, b140[2]);
*------------------------------------------------------------------------*/		
		li	t4,0x10001		
		
		mpymvrr	acc1, sx0,sy0,shnone,x0,gprt2,y0,gprt3
		mpymvrr	acc0, sx0,sy0,shsat,x0,gprt2,y0,gprt1
		mvp	gprt3,udi_mac0acc0lo
		mvp	gprt3,udi_mac0acc0lo
		mvp	gpra0,udi_mac0acc1lo
		mvp	gprt3,udi_mac1acc1lo
		li	t1,0xffff		
		sra	t3,t3,15
		and	t3,t3,t1
		sra	a0,a0,15
		sll	a0,a0,0x10
		or	t3,t3,a0		
		mpymvrr	acc1, sx0,sy0,shsat,x0,gprt3,y0,gprt4
		mvp	gprt3,udi_mac0acc1lo
		mvp	gprt3,udi_mac0acc1lo
		mvp	gprt3,udi_mac0acc1lo		
		addacc	acc1,acc0
		addacc	acc0,acc1
		mvp	gprt3,udi_mac1acc0lo
		mvp	gprt3,udi_mac0acc1lo
		mvp	gprt4,udi_mac0acc1lo	
		
		li	t3,0x800000
		mvp	udi_mode,gprt3
		mvp	udi_mac1acc0lo,gprt4
		rdp	gprt3,udi_mode
		addacc	acc0,acc0
		mvp	gprt3,udi_mode
		mvp	gprt3,udi_mode
		mvp	gprt3,udi_mode
		mvp	gprt3,udi_mac0acc0lo
		
		srl	a0,t3,31
		move	t4,zero
		li	a1,0xff
		li	t3,0x800000
		movn	t4,a1,a0
		mvp	udi_mode,gprt3	
		mvp	udi_mac0acc0hi,gprt4
				
		lh	t1,0(t7)
		lh	a1,0(t9)
		lh	t4,0(t6)
		lh	a0,2(t9)
		lh	t3,4(t9)
		
		macmvrr	acc0,sx0,sy0,shsat,x0,gprt1,y0,gpra1
		
		macmvrr	acc0,sx0,sy0,shsat,x0,gprt4,y0,gpra0
		
		macmvrr	acc0,sx0,sy0,shsat,x0,gprs1,y0,gprt3
		mvp	gprt3,udi_mac0acc0lo
		mvp	gprt3,udi_mac0acc0lo
		mvp	gprt3,udi_mac0acc0lo
		mvp	gprs0,udi_mac0acc0lo
/*------------------------------------------------------------------------*
     		var2 = 3;
     		for(;var2>0;var2--)
     		{
         		if (L_tmp > (Word32) 0X3fffffffL)
         		{
            			Overflow = 1;
            			L_tmp = MAX_32;
            			break;
         		}
         		else if (L_tmp < (Word32) 0xc0000000L)
         		{
               			Overflow = 1;
               			L_tmp = MIN_32;
               			break;            
         		}
         		else
         		{
         			L_tmp *= 2;
         		}
     		}
*------------------------------------------------------------------------*/			
				
		li	v0,3
		addiu	a3,v0,-1
Pre_Process204:	sll	a2,a3,0x10
		slt	a1,s5,s0
		slt	a0,s0,s4
		sra	v0,a2,0x10
		bnez	a1,Pre_Process400
		sll	s0,s0,0x1
		bnez	a0,Pre_Process416
		nop
		bgtz	v0,Pre_Process204
		addiu	a3,v0,-1
Pre_Process244:	
/*------------------------------------------------------------------------*
		L_arrondi = L_add(L_tmp, (Word32)0x00008000);
     		signal[i] = extract_h(L_arrondi); 
*------------------------------------------------------------------------*/							
		move	t4,s0
		li	t3,0x8000
		addu	v0,t3,t4
		/*
		mvp	udi_mac0acc0lo,gprt3
		mvp	udi_mac1acc0lo,gprt4
		addacc	acc0,acc0
		mvp	gprt3,udi_mac0acc0lo
		mvp	gprt3,udi_mac0acc0lo
		mvp	gprt3,udi_mac0acc0lo
		mvp	gprt3,udi_mac0acc0lo
		*/
		//move	v0,t3	
		
		sra	v0,v0,0x10	
		sh	v0,0(s2) 
		lhu	t1,0(ra) 
		lhu	a0,0(t0)
		sh	t1,0(t5)
		sh	a0,0(t8)
		sra	v0,s0,0x10	
		sh	v0,0(ra)
		bltz	s0,Pre_Process308
		sra	a0,s0,0x1
		sra	a0,s0,0x1
Pre_Process308:	
/*------------------------------------------------------------------------*
		L_tmp2  = L_msu( L_tmp1 , y1_hi, 16384 );
     		y1_lo   = extract_l( L_tmp2 );
*------------------------------------------------------------------------*/			
		lh	t1,0(ra)
		srl	t3,a0,31
		move	t4,zero
		li	v0,0xff
		movn	t4,v0,t3
		li	t3,0x800000
		mvp	udi_mode,gprt3	
		mvp	udi_mac0acc0hi,gprt4	
		mvp	udi_mac0acc0lo,gpra0
		li	t4,16384
		mvp	gprt3,udi_mode
		
		msumvrr	acc0,sx0,sy0,shsat,x0,gprt1,y0,gprt4
		mvp	gprt3,udi_mac0acc0lo
		mvp	gprt3,udi_mac0acc0lo
		mvp	gprt3,udi_mac0acc0lo		
		mvp	gprv0,udi_mac0acc0lo
						
		sll	t3,v0,0x10	
		sra	v0,t3,0x10
		addiu	s3,s3,1
		slt	t3,s3,s6
		sh	v0,0(t0) 
		bnez	t3,Pre_Process76
		addiu	s2,s2,2
Pre_Process356:	
		lw	t9,56(sp)
		lw	s8,52(sp)
		lw	ra,48(sp)
		lw	s7,44(sp)
		lw	s6,40(sp)
		lw	s5,36(sp)
		lw	s4,32(sp)
		lw	s3,28(sp)
		lw	s2,24(sp)
		lw	s1,20(sp)
		lw	s0,16(sp)
		jr	ra
		addiu	sp,sp,64
Pre_Process400:	lui	t4,0x7fff
		ori	s0,t4,0xffff
		la	t4,Overflow
		
		b	Pre_Process244 
		sw	s7,0(t4)
Pre_Process416:	lui	s0,0x8000
		la	t4,Overflow
		b	Pre_Process244
		sw	s7,0(t4) 
END( Pre_Process )

		