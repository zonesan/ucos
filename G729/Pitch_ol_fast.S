/*
***************************************************************************
*                  Copyright (c) 2006 vBridge Microsystem, Inc.  
*                       Unpublished & Not for Publication
*                               All Rights Reserved                   
*   
* File        : Levinson.S                     
*                                                                         
* Description : Compute the open loop pitch lag. (fast version)    
*             
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
Word16 Pitch_ol_fast(  // output: open loop pitch lag                        
   Word16 signal[],    // input : signal used to compute the open loop pitch 
                       //     signal[-pit_max] to signal[-1] should be known 
   Word16   pit_max,   // input : maximum pitch lag                          
   Word16   L_frame    // input : length of frame to compute pitch           
)
{
  Word16  i, j;
  Word16  max1, max2, max3;
  Word16  max_h, max_l, ener_h, ener_l;
  Word16  T1, T2, T3;
  Word16  *p, *p1;
  Word32  max, sum, L_temp;

  // Scaled signal 

  Word16 scaled_signal[L_FRAME+PIT_MAX];
  Word16 *scal_sig;

  scal_sig = &scaled_signal[pit_max];


   Overflow = 0;
   sum = 0;

   for(i= -pit_max; i< L_frame; i+=2)
     sum = L_mac(sum, signal[i], signal[i]);



   if(Overflow == 1)
   {
     for(i=-pit_max; i<L_frame; i++)
     {
       scal_sig[i] = shr(signal[i], 3);
     }
   }
   else {
     L_temp = L_sub(sum, (Word32)1048576L);
     if ( L_temp < (Word32)0 )  // if (sum < 2^20) 
     {
        for(i=-pit_max; i<L_frame; i++)
        {
          scal_sig[i] = shl(signal[i], 3);
        }
     }
     else
     {
       for(i=-pit_max; i<L_frame; i++)
       {
         scal_sig[i] = signal[i];
       }
     }
   }

    // First section 

    max = MIN_32;
    T1  = 20;    // Only to remove warning from some compilers 
    for (i = 20; i < 40; i++) {
        p  = scal_sig;
        p1 = &scal_sig[-i];
        sum = 0;
        for (j=0; j<L_frame; j+=2, p+=2, p1+=2)
            sum = L_mac(sum, *p, *p1);
        L_temp = L_sub(sum, max);
        if (L_temp > 0) { max = sum; T1 = i;   }
    }

    // compute energy of maximum 

    sum = 1;                   // to avoid division by zero 
    p = &scal_sig[-T1];
    for(i=0; i<L_frame; i+=2, p+=2)
        sum = L_mac(sum, *p, *p);

    // max1 = max/sqrt(energy)                  
    // This result will always be on 16 bits !! 

    sum = Inv_sqrt(sum);            // 1/sqrt(energy),    result in Q30 
    L_Extract(max, &max_h, &max_l);
    L_Extract(sum, &ener_h, &ener_l);
    sum  = Mpy_32(max_h, max_l, ener_h, ener_l);
    max1 = extract_l(sum);

    // Second section 

    max = MIN_32;
    T2  = 40;    // Only to remove warning from some compilers 
    for (i = 40; i < 80; i++) {
        p  = scal_sig;
        p1 = &scal_sig[-i];
        sum = 0;
        for (j=0; j<L_frame; j+=2, p+=2, p1+=2)
            sum = L_mac(sum, *p, *p1);
        L_temp = L_sub(sum, max);
        if (L_temp > 0) { max = sum; T2 = i;   }
    }

    // compute energy of maximum 

    sum = 1;                   // to avoid division by zero 
    p = &scal_sig[-T2];
    for(i=0; i<L_frame; i+=2, p+=2)
        sum = L_mac(sum, *p, *p);

    // max2 = max/sqrt(energy)                  
    // This result will always be on 16 bits !! 

    sum = Inv_sqrt(sum);            // 1/sqrt(energy),    result in Q30 
    L_Extract(max, &max_h, &max_l);
    L_Extract(sum, &ener_h, &ener_l);
    sum  = Mpy_32(max_h, max_l, ener_h, ener_l);
    max2 = extract_l(sum);

    // Third section 

    max = MIN_32;
    T3  = 80;    // Only to remove warning from some compilers 
    for (i = 80; i < 143; i+=2) {
        p  = scal_sig;
        p1 = &scal_sig[-i];
        sum = 0;
        for (j=0; j<L_frame; j+=2, p+=2, p1+=2)
            sum = L_mac(sum, *p, *p1);
        L_temp = L_sub(sum, max);
        if (L_temp > 0) { max = sum; T3 = i;   }
    }

     // Test around max3 

     i = T3;
     p  = scal_sig;
     p1 = &scal_sig[-(i+1)];
     sum = 0;
     for (j=0; j<L_frame; j+=2, p+=2, p1+=2)
         sum = L_mac(sum, *p, *p1);
     L_temp = L_sub(sum, max);
     if (L_temp > 0) { max = sum; T3 = i+(Word16)1;   }

     p  = scal_sig;
     p1 = &scal_sig[-(i-1)];
     sum = 0;
     for (j=0; j<L_frame; j+=2, p+=2, p1+=2)
         sum = L_mac(sum, *p, *p1);
     L_temp = L_sub(sum, max);
     if (L_temp > 0) { max = sum; T3 = i-(Word16)1;   }

    // compute energy of maximum 

    sum = 1;                   // to avoid division by zero 
    p = &scal_sig[-T3];
    for(i=0; i<L_frame; i+=2, p+=2)
        sum = L_mac(sum, *p, *p);

    // max1 = max/sqrt(energy)                  
    // This result will always be on 16 bits !! 

    sum = Inv_sqrt(sum);            // 1/sqrt(energy),    result in Q30 
    L_Extract(max, &max_h, &max_l);
    L_Extract(sum, &ener_h, &ener_l);
    sum  = Mpy_32(max_h, max_l, ener_h, ener_l);
    max3 = extract_l(sum);

   //-----------------------*
   // Test for multiple.    *
   //-----------------------

    // if( abs(T2*2 - T3) < 5)  
    //    max2 += max3 * 0.25;  

    i = sub(shl(T2,1), T3);
    j = sub(abs_s(i), 5);
    if(j < 0)
      max2 = add(max2, shr(max3, 2));

    // if( abs(T2*3 - T3) < 7)  
    //    max2 += max3 * 0.25;  

    i = add(i, T2);
    j = sub(abs_s(i), 7);
    if(j < 0)
      max2 = add(max2, shr(max3, 2));

    // if( abs(T1*2 - T2) < 5)  
    //    max1 += max2 * 0.20;  

    i = sub(shl(T1,1), T2);
    j = sub(abs_s(i), 5);
    if(j < 0)
      max1 = add(max1, mult(max2, 6554));

    // if( abs(T1*3 - T2) < 7)  
    //    max1 += max2 * 0.20;  

    i = add(i, T1);
    j = sub(abs_s(i), 7);
    if(j < 0)
      max1 = add(max1, mult(max2, 6554));

   //--------------------------------------------------------------------*
   // Compare the 3 sections maxima.                                     *
   //--------------------------------------------------------------------

    if( sub(max1, max2) < 0 ) {max1 = max2; T1 = T2;  }
    if( sub(max1, max3) <0 )  {T1 = T3; }

    return T1;
}
*------------------------------------------------------------------------*/
LEAF( Pitch_ol_fast )
		sll	t1,a1,0x10
		sra	a3,t1,0x10
		addiu	sp,sp,-544
		negu	t0,a3
		sw	s5,524(sp)
		sw	s1,508(sp)
		sll	s5,a2,0x10
		sll	s1,t0,0x10
		sra	s1,s1,0x10
		sra	s5,s5,0x10
		sll	a1,a3,0x1
		addiu	v0,sp,24
		slt	v1,s1,s5
		sw	s7,532(sp)
		sw	s4,520(sp)
		sw	s2,512(sp)
		move	s4,a0
		sw	ra,540(sp)
		sw	s8,536(sp)
		sw	s6,528(sp)
		sw	s3,516(sp)
		sw	s0,504(sp)
		addu	s7,v0,a1		
		sw	zero,Overflow
		
		mvp 	gpra0,udi_mode
		li	a0,0x800000
		mvp 	udi_mode,gpra0
		move	a0,zero
		
		mvp	udi_mac0acc0lo,gpra0
		mvp	udi_mac0acc0hi,gpra0
		
		beqz	v1,Pitch_ol_fast156
		move	s2,s1
/*------------------------------------------------------------------------*
   		for(i= -pit_max; i< L_frame; i+=2)
     			sum = L_mac(sum, signal[i], signal[i]);
*------------------------------------------------------------------------*/		
		sll	a2,s1,0x1
		addu	s0,a2,s4
		mvp	gprt0,udi_mode
		mvp	gprt0,udi_mode
Pitch_ol_fast116:	lh	a1,0(s0)
		addiu	s0,s0,4
		//jal	L_mac
		macmvrr	acc0,sx0,sy0,shsat,x0,gpra1,y0,gpra1
		//move	a2,a1
		addiu	t4,s1,2
		sll	t3,t4,0x10
		sra	s1,t3,0x10
		slt	t2,s1,s5
		rdp	gprt0,udi_mode
		bnez	t2,Pitch_ol_fast116
		//move	a0,v0
		nop
		
		li	t1,1
		move 	t6,zero
		
		mvp	gprt0,udi_mode
		sra	t0,t0,20
		and	t4,t0,1
		movn	t6,t1,t4
		//la	t5,Overflow		
		//sw	t6,0(t5)
		sw	t6,Overflow
		mvp	gprt0,udi_mode
		mvp	gprt0,udi_mode
		mvp	gpra0,udi_mac0acc0lo
		
		
Pitch_ol_fast156:	
		
		lw	t6,Overflow		
		li	t5,1
		bne	t6,t5,Pitch_ol_fast440
		nop
		slt	a0,s2,s5
		beqz	a0,Pitch_ol_fast236
		move	a2,s2
		sll	t7,s2,0x1
		addu	a0,t7,s4
/*------------------------------------------------------------------------*
   		for(i=-pit_max; i<L_frame; i++)
     		{
       			scal_sig[i] = shr(signal[i], 3);
     		}
*------------------------------------------------------------------------*/		
		addu	a1,t7,s7
Pitch_ol_fast196:	addiu	a2,a2,1
		lh	s3,0(a0)
		sra	s2,s3,0x3
		slt	s0,a2,s5
		sh	s2,0(a1)
		addiu	a0,a0,2
		bnez	s0,Pitch_ol_fast196
		addiu	a1,a1,2
		
Pitch_ol_fast236:	lui	t2,0x8000
Pitch_ol_fast240:	li	a2,20
		li	s4,20
		slt	s8,zero,s5
		li	s6,-40
		sw	t2,492(sp)
		b	Pitch_ol_fast316
		sw	a2,480(sp)
		
Pitch_ol_fast268:	lw	a1,492(sp)
Pitch_ol_fast272:
		mvp	gprt0,udi_mode
		mvp	gprt0,udi_mode
		mvp	gprt0,udi_mode
		mvp	gprs3,udi_mac0acc0lo
		
		jal	L_sub
		move	a0,s3
		blez	v0,Pitch_ol_fast296
		addiu	t7,s4,1
		sw	s3,492(sp)
		sw	s4,480(sp)
Pitch_ol_fast296:	sll	a0,t7,0x10
		sra	s4,a0,0x10
		slti	t6,s4,40
		beqz	t6,Pitch_ol_fast596
		addiu	s6,s6,-2
Pitch_ol_fast316:	move	s1,s7
		addu	s0,s6,s7
		move	s3,zero
		beqz	s8,Pitch_ol_fast268
		move	s2,zero
/*------------------------------------------------------------------------*
   		for (j=0; j<L_frame; j+=2, p+=2, p1+=2)
            		sum = L_mac(sum, *p, *p1);
*------------------------------------------------------------------------*/		
		mvp	udi_mac0acc0lo,gprs2
		mvp	udi_mac0acc0hi,gprs2
		
Pitch_ol_fast336:	lh	a1,0(s1)
		lh	a2,0(s0)
		macmvrr	acc0,sx0,sy0,shsat,x0,gpra1,y0,gpra2
		addiu	s2,s2,2
		slt	t3,s2,s5
		//beqz	t3,Pitch_ol_fast268
		//lh	a1,4(s1)
		//lh	a2,4(s0)
		//macmvrr	acc0,sx0,sy0,shsat,x0,gpra1,y0,gpra2
		//addiu	s2,s2,2
		//slt	t3,s2,s5
		addiu	s1,s1,4
		bnez	t3,Pitch_ol_fast336
		addiu	s0,s0,4
		b	Pitch_ol_fast272		
		lw	a1,492(sp)
Pitch_ol_fast440:	jal	L_sub
		lui	a1,0x10
		bltz	v0,Pitch_ol_fast520
		slt	a1,s2,s5
		beqz	a1,Pitch_ol_fast236
		move	a2,s2
		sll	v0,s2,0x1
		addu	a0,v0,s4
		addu	a1,v0,s7
Pitch_ol_fast476:	addiu	t1,a2,1
		sll	t0,t1,0x10
		lhu	s1,0(a0)
		sra	a2,t0,0x10
		slt	a3,a2,s5
		sh	s1,0(a1)
		addiu	a0,a0,2
		bnez	a3,Pitch_ol_fast476
		addiu	a1,a1,2
		b	Pitch_ol_fast240
		lui	t2,0x8000
Pitch_ol_fast520:	slt	t8,s2,s5
		beqz	t8,Pitch_ol_fast236
		move	s3,s2
		sll	t9,s2,0x1
		addu	s1,t9,s4
		addu	s0,t9,s7
Pitch_ol_fast544:	lh	a0,0(s1)
		jal	wg_shl
		li	a1,3
		addiu	v1,s3,1
		sll	ra,v1,0x10
		sra	s3,ra,0x10
		slt	s8,s3,s5
		sh	v0,0(s0)
		addiu	s1,s1,2
		bnez	s8,Pitch_ol_fast544
		addiu	s0,s0,2
		b	Pitch_ol_fast240
		lui	t2,0x8000
		
Pitch_ol_fast596:	lw	s0,480(sp)
		li	a0,1
		sll	s2,s0,0x1
		subu	s0,s7,s2
		
		move	t0,zero
		mvp	udi_mac0acc0lo,gprt0
		mvp	udi_mac0acc0hi,gprt0
		
		bnez	s8,Pitch_ol_fast892
		move	s1,zero
Pitch_ol_fast620:
	
		mvp	gprt0,udi_mode
		mvp	gprt0,udi_mode
		mvp	gprt0,udi_mode
		mvp	gpra0,udi_mac0acc0lo
	
		jal	Inv_sqrt
		li	s4,40
		lw	a0,492(sp)
		move	s1,v0
		addiu	a1,sp,16
		jal	L_Extract
		addiu	a2,sp,18
		move	a0,s1
		addiu	a1,sp,20
		jal	L_Extract
		addiu	a2,sp,22
		lh	a0,16(sp)
		lh	a1,18(sp)
		lh	a2,20(sp)
		lh	a3,22(sp)
		lui	t9,0x8000
		jal	Mpy_32
		sw	t9,496(sp)
		jal	extract_l
		move	a0,v0
		li	t8,40
		li	s6,-80
		sw	v0,472(sp)
		b	Pitch_ol_fast768
		sw	t8,484(sp)
Pitch_ol_fast720:	lw	a1,496(sp)
Pitch_ol_fast724:
		mvp	gprt0,udi_mode
		mvp	gprt0,udi_mode
		mvp	gprt0,udi_mode
		mvp	gprs3,udi_mac0acc0lo		
		jal	L_sub
		move	a0,s3
		blez	v0,Pitch_ol_fast748
		addiu	t0,s4,1
		sw	s3,496(sp)
		sw	s4,484(sp)
Pitch_ol_fast748:	sll	a3,t0,0x10
		sra	s4,a3,0x10
		slti	v0,s4,80
		beqz	v0,Pitch_ol_fast980
		addiu	s6,s6,-2
Pitch_ol_fast768:	move	s1,s7
		addu	s0,s6,s7
		move	s3,zero
		mvp	udi_mac0acc0lo,gprs3
		mvp	udi_mac0acc0hi,gprs3		
		beqz	s8,Pitch_ol_fast720
		move	s2,zero		
		
Pitch_ol_fast788:	lh	a1,0(s1)
		lh	a2,0(s0)
		macmvrr	acc0,sx0,sy0,shsat,x0,gpra1,y0,gpra2
		addiu	s2,s2,2
		slt	ra,s2,s5
		move	s3,v0
		//addiu	s1,s1,4
		beqz	ra,Pitch_ol_fast720
		//addiu	s0,s0,4
		lh	a1,4(s1)
		lh	a2,4(s0)
		macmvrr	acc0,sx0,sy0,shsat,x0,gpra1,y0,gpra2
		addiu	s2,s2,2
		slt	ra,s2,s5
		addiu	s1,s1,8
		bnez	ra,Pitch_ol_fast788
		addiu	s0,s0,8
		b	Pitch_ol_fast724
		lw	a1,496(sp)
/*------------------------------------------------------------------------*
   		for(i=0; i<L_frame; i+=2, p+=2)
        		sum = L_mac(sum, *p, *p);
*------------------------------------------------------------------------*/		
Pitch_ol_fast892:	lh	a1,0(s0)
		addiu	s6,s1,2
		sll	s4,s6,0x10
		macmvrr	acc0,sx0,sy0,shsat,x0,gpra1,y0,gpra1
		sra	s1,s4,0x10
		slt	s3,s1,s5
		//beqz	s3,Pitch_ol_fast620
		//addiu	s0,s0,4
		//lh	a1,0(s0)
		//addiu	s6,s1,2
		//sll	s4,s6,0x10
		//macmvrr	acc0,sx0,sy0,shsat,x0,gpra1,y0,gpra1
		//sra	s1,s4,0x10
		//slt	s3,s1,s5
		bnez	s3,Pitch_ol_fast892
		addiu	s0,s0,4
		b	Pitch_ol_fast620
		nop
		
Pitch_ol_fast980:	lw	a2,484(sp)
		li	a0,1
		sll	t1,a2,0x1
		subu	s0,s7,t1
		move	t0,zero
		
		mvp	udi_mac0acc0lo,gprt0
		mvp	udi_mac0acc0hi,gprt0
		bnez	s8,Pitch_ol_fast1276
		move	s1,zero
		
Pitch_ol_fast1004:
		mvp	gprt0,udi_mode
		mvp	gprt0,udi_mode
		mvp	gprt0,udi_mode
		mvp	gpra0,udi_mac0acc0lo		
		jal	Inv_sqrt
		li	s4,80
		lw	a0,496(sp)
		move	s2,v0
		addiu	a1,sp,16
		jal	L_Extract
		addiu	a2,sp,18
		move	a0,s2
		addiu	a1,sp,20
		jal	L_Extract
		addiu	a2,sp,22
		lh	a0,16(sp)
		lh	a1,18(sp)
		lh	a2,20(sp)
		lh	a3,22(sp)
		lui	t6,0x8000
		jal	Mpy_32
		sw	t6,500(sp)
		jal	extract_l
		move	a0,v0
		li	t5,80
		li	s6,-160
		sw	v0,476(sp)
		b	Pitch_ol_fast1152
		sw	t5,488(sp)
Pitch_ol_fast1104:	lw	a1,500(sp)
Pitch_ol_fast1108:
		mvp	gprt0,udi_mode
		mvp	gprt0,udi_mode
		mvp	gprt0,udi_mode
		mvp	gprs3,udi_mac0acc0lo		
		jal	L_sub
		move	a0,s3
		blez	v0,Pitch_ol_fast1132
		addiu	t9,s4,2
		sw	s3,500(sp)
		sw	s4,488(sp)
Pitch_ol_fast1132:	sll	s3,t9,0x10
		sra	s4,s3,0x10
		slti	s0,s4,143
		beqz	s0,Pitch_ol_fast1364
		addiu	s6,s6,-4
Pitch_ol_fast1152:	move	s1,s7
		addu	s0,s6,s7
		move	s3,zero
		
		mvp	udi_mac0acc0lo,gprs3
		mvp	udi_mac0acc0hi,gprs3
		beqz	s8,Pitch_ol_fast1104
		move	s2,zero
Pitch_ol_fast1172:	lh	a1,0(s1)
		lh	a2,0(s0)
		macmvrr	acc0,sx0,sy0,shsat,x0,gpra1,y0,gpra2
		addiu	s2,s2,2
		slt	a0,s2,s5
		addiu	s1,s1,4
		beqz	a0,Pitch_ol_fast1104
		addiu	s0,s0,4
		lh	a1,0(s1)
		lh	a2,0(s0)
		macmvrr	acc0,sx0,sy0,shsat,x0,gpra1,y0,gpra2
		addiu	s2,s2,2
		slt	a0,s2,s5
		addiu	s1,s1,4
		bnez	a0,Pitch_ol_fast1172
		addiu	s0,s0,4
		b	Pitch_ol_fast1108
		lw	a1,500(sp)
Pitch_ol_fast1276:	lh	a1,0(s0)
		//addiu	s0,s0,4
		macmvrr	acc0,sx0,sy0,shsat,x0,gpra1,y0,gpra1
		addiu	s1,s1,2
		slt	t2,s1,s5
		beqz	t2,Pitch_ol_fast1004
		//move	a0,v0
		lh	a1,4(s0)
		addiu	s0,s0,8
		macmvrr	acc0,sx0,sy0,shsat,x0,gpra1,y0,gpra1
		addiu	s1,s1,2
		slt	t2,s1,s5
		bnez	t2,Pitch_ol_fast1276
		move	a0,v0
		b	Pitch_ol_fast1004
		nop
Pitch_ol_fast1364:	lw	ra,488(sp)
		move	s0,s7
		addiu	s4,ra,1
		sll	s6,s4,0x1
		subu	s1,s7,s6
		move	s3,zero
		move	s6,ra
		move	t0,zero
		mvp	udi_mac0acc0lo,gprt0
		mvp	udi_mac0acc0hi,gprt0
		bnez	s8,Pitch_ol_fast1996
		move	s2,zero
Pitch_ol_fast1400:	lw	a1,500(sp)
Pitch_ol_fast1404:	

		mvp	gprt0,udi_mode
		mvp	gprt0,udi_mode
		mvp	gprt0,udi_mode
		mvp	gprs3,udi_mac0acc0lo
		
		jal	L_sub
		move	a0,s3
		blez	v0,Pitch_ol_fast1432
		sll	s1,s4,0x10
		sra	s4,s1,0x10
		sw	s3,500(sp)
		sw	s4,488(sp)
Pitch_ol_fast1432:	li	t1,1
		subu	t0,t1,s6
		sll	a3,t0,0x1
		addu	s1,a3,s7
		move	s0,s7
		move	s3,zero
		mvp	udi_mac0acc0lo,gprs3
		mvp	udi_mac0acc0hi,gprs3		
		bnez	s8,Pitch_ol_fast1892
		move	s2,zero
Pitch_ol_fast1464:	lw	a1,500(sp)
Pitch_ol_fast1468:	
		
		mvp	gprt0,udi_mode
		mvp	gprt0,udi_mode
		mvp	gprt0,udi_mode
		mvp	gprs3,udi_mac0acc0lo
		
		
		jal	L_sub
		move	a0,s3
		blez	v0,Pitch_ol_fast1500
		addiu	t6,s6,-1
		sll	t5,t6,0x10
		sra	t4,t5,0x10
		sw	s3,500(sp)
		sw	t4,488(sp)
Pitch_ol_fast1500:	lw	a0,488(sp)
		move	s1,zero
		sll	s2,a0,0x1
		subu	s0,s7,s2
		
		move	t0,zero
		mvp	udi_mac0acc0lo,gprt0
		mvp	udi_mac0acc0hi,gprt0
		
		bnez	s8,Pitch_ol_fast2100
		li	a0,1
Pitch_ol_fast1524:	
		
		mvp	gprt0,udi_mode
		mvp	gprt0,udi_mode
		mvp	gprt0,udi_mode
		mvp	gpra0,udi_mac0acc0lo
		jal	Inv_sqrt
		nop
		lw	a0,500(sp)
		move	s5,v0
		addiu	a2,sp,18
		jal	L_Extract
		addiu	a1,sp,16
		move	a0,s5
		addiu	a1,sp,20
		jal	L_Extract
		addiu	a2,sp,22
		lh	a2,20(sp)
		lh	a3,22(sp)
		lh	a1,18(sp)
		jal	Mpy_32
		lh	a0,16(sp)
		jal	extract_l
		move	a0,v0
		lw	a0,484(sp)
		li	a1,1
		jal	wg_shl
		move	s1,v0
		lw	a1,488(sp)
		jal	wg_sub
		move	a0,v0
		move	a0,v0
		jal	abs_s
		move	s0,v0
		move	a0,v0
		jal	wg_sub
		li	a1,5
		bltz	v0,Pitch_ol_fast2188
		move	a0,s1
		lw	a1,484(sp)
		jal	wg_add
		move	a0,s0
		jal	abs_s
		move	a0,v0
		move	a0,v0
		jal	wg_sub
		li	a1,7
		bltzl	v0,Pitch_ol_fast2256
		move	a0,s1
		lw	a0,480(sp)
Pitch_ol_fast1700:	jal	wg_shl
		li	a1,1
		lw	a1,484(sp)
		jal	wg_sub
		move	a0,v0
		move	a0,v0
		jal	abs_s
		move	s0,v0
		move	a0,v0
		jal	wg_sub
		li	a1,5
		bltz	v0,Pitch_ol_fast2340
		lw	a0,476(sp)
		lw	a1,480(sp)
Pitch_ol_fast1756:	jal	wg_add
		move	a0,s0
		jal	abs_s
		move	a0,v0
		move	a0,v0
		jal	wg_sub
		li	a1,7
		bltz	v0,Pitch_ol_fast2408
		lw	a0,476(sp)
		lw	a0,472(sp)
Pitch_ol_fast1796:	jal	wg_sub
		lw	a1,476(sp)
		bltz	v0,Pitch_ol_fast2456
		lw	s0,476(sp)
		lw	a0,472(sp)
Pitch_ol_fast1816:	jal	wg_sub
		move	a1,s1
		lw	s3,488(sp)
		lw	t9,480(sp)
		slti	ra,v0,0
		movz	s3,t9,ra
		move	v0,s3
		lw	ra,540(sp)
		lw	s8,536(sp)
		lw	s7,532(sp)
		lw	s6,528(sp)
		lw	s5,524(sp)
		lw	s4,520(sp)
		lw	s3,516(sp)
		lw	s2,512(sp)
		lw	s1,508(sp)
		lw	s0,504(sp)
		jr	ra
		addiu	sp,sp,544
Pitch_ol_fast1892:	lh	a2,0(s1)
		lh	a1,0(s0)
		macmvrr	acc0,sx0,sy0,shsat,x0,gpra1,y0,gpra2
		addiu	s2,s2,2
		slt	a2,s2,s5
		addiu	s0,s0,4
		beqz	a2,Pitch_ol_fast1464
		addiu	s1,s1,4
		lh	a2,0(s1)
		lh	a1,0(s0)
		macmvrr	acc0,sx0,sy0,shsat,x0,gpra1,y0,gpra2
		addiu	s2,s2,2
		slt	a2,s2,s5
		addiu	s0,s0,4
		bnez	a2,Pitch_ol_fast1892
		addiu	s1,s1,4
		b	Pitch_ol_fast1468
		lw	a1,500(sp)
Pitch_ol_fast1996:	lh	a1,0(s0)
		lh	a2,0(s1)
		macmvrr	acc0,sx0,sy0,shsat,x0,gpra1,y0,gpra2
		addiu	s2,s2,2
		slt	v1,s2,s5
		addiu	s0,s0,4
		beqz	v1,Pitch_ol_fast1400
		addiu	s1,s1,4
		lh	a1,0(s0)
		lh	a2,0(s1)
		macmvrr	acc0,sx0,sy0,shsat,x0,gpra1,y0,gpra2
		addiu	s2,s2,2
		slt	v1,s2,s5
		addiu	s0,s0,4
		bnez	v1,Pitch_ol_fast1996
		addiu	s1,s1,4
		b	Pitch_ol_fast1404
		lw	a1,500(sp)
Pitch_ol_fast2100:	lh	a1,0(s0)
		addiu	s0,s0,4
		macmvrr	acc0,sx0,sy0,shsat,x0,gpra1,y0,gpra1
		addiu	s1,s1,2
		slt	s7,s1,s5
		beqz	s7,Pitch_ol_fast1524
		move	a0,v0
		lh	a1,0(s0)
		addiu	s0,s0,4
		macmvrr	acc0,sx0,sy0,shsat,x0,gpra1,y0,gpra1
		addiu	s1,s1,2
		slt	s7,s1,s5
		bnez	s7,Pitch_ol_fast2100
		move	a0,v0
		b	Pitch_ol_fast1524
		nop
Pitch_ol_fast2188:	jal	wg_shr
		li	a1,2
		lw	a0,476(sp)
		jal	wg_add
		move	a1,v0
		lw	a1,484(sp)
		move	a0,s0
		jal	wg_add
		sw	v0,476(sp)
		jal	abs_s
		move	a0,v0
		move	a0,v0
		jal	wg_sub
		li	a1,7
		bgezl	v0,Pitch_ol_fast1700
		lw	a0,480(sp)
		move	a0,s1
Pitch_ol_fast2256:	jal	wg_shr
		li	a1,2
		lw	a0,476(sp)
		jal	wg_add
		move	a1,v0
		lw	a0,480(sp)
		li	a1,1
		jal	wg_shl
		sw	v0,476(sp)
		lw	a1,484(sp)
		jal	wg_sub
		move	a0,v0
		move	a0,v0
		jal	abs_s
		move	s0,v0
		move	a0,v0
		jal	wg_sub
		li	a1,5
		bgezl	v0,Pitch_ol_fast1756
		lw	a1,480(sp)
		lw	a0,476(sp)
Pitch_ol_fast2340:	jal	mult
		li	a1,6554
		lw	a0,472(sp)
		jal	wg_add
		move	a1,v0
		lw	a1,480(sp)
		move	a0,s0
		jal	wg_add
		sw	v0,472(sp)
		jal	abs_s
		move	a0,v0
		move	a0,v0
		jal	wg_sub
		li	a1,7
		bgez	v0,Pitch_ol_fast1796
		lw	a0,472(sp)
		lw	a0,476(sp)
Pitch_ol_fast2408:	jal	mult
		li	a1,6554
		lw	a0,472(sp)
		jal	wg_add
		move	a1,v0
		sw	v0,472(sp)
		lw	a0,472(sp)
		jal	wg_sub
		lw	a1,476(sp)
		bgez	v0,Pitch_ol_fast1816
		lw	a0,472(sp)
		lw	s0,476(sp)
Pitch_ol_fast2456:	lw	t8,484(sp)
		sw	s0,472(sp)
		lw	a0,472(sp)
		move	a1,s1
		jal	wg_sub
		sw	t8,480(sp)
		lw	s3,488(sp)
		lw	t9,480(sp)
		slti	ra,v0,0
		movz	s3,t9,ra
		move	v0,s3
		lw	ra,540(sp)
		lw	s8,536(sp)
		lw	s7,532(sp)
		lw	s6,528(sp)
		lw	s5,524(sp)
		lw	s4,520(sp)
		lw	s3,516(sp)
		lw	s2,512(sp)
		lw	s1,508(sp)
		lw	s0,504(sp)
		jr	ra
		addiu	sp,sp,544

END( Pitch_ol_fast )

