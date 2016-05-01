/*
***************************************************************************
*                  Copyright (c) 2006 vBridge Microsystem, Inc.  
*                       Unpublished & Not for Publication
*                               All Rights Reserved                   
*   
* File        : ACELP_Code_A.S                     
*                                                                         
* Description:    
*             
* Date        : May 24, 2006                                                           
***************************************************************************
*/

#include <mips/asm.h>
#include <mips/udi.h>
#include <mips/regdef.h>

#include "mipsudi_inst.h"

      .set noat
      .set noreorder

LEAF( ACELP_Code_A )
		addiu	sp,sp,-1896
		sll	t0,a2,0x10
		sw	a0,1896(sp)
		sll	a0,a3,0x10
		sra	a2,t0,0x10
		sw	a1,1900(sp)
		sra	a0,a0,0x10
		li	a1,1
		sw	ra,1892(sp)
		sw	s8,1888(sp)
		sw	s7,1884(sp)
		sw	s6,1880(sp)
		sw	s5,1876(sp)
		sw	s4,1872(sp)
		sw	s3,1868(sp)
		sw	s2,1864(sp)
		sw	s1,1860(sp)
		sw	s0,1856(sp)
		jal	wg_shl
		sw	a2,1584(sp)
		lw	v1,1584(sp)
		sw	v0,1588(sp)
		slti	v0,v1,40
		beqz	v0,ACELP_Code_A176
		sw	v0,1716(sp)
		lw	s2,1584(sp)
		lw	a1,1900(sp)
		sll	a3,s2,0x1
		addu	s0,a3,a1
		move	s1,a1
/*------------------------------------------------------------------------*
     		for (i = T0; i < L_SUBFR; i++)
       			h[i] = add(h[i], mult(h[i-T0], sharp));
*------------------------------------------------------------------------*/
		lw	s8,1588(sp)
ACELP_Code_A120:	lh	a0,0(s1)
		
		//jal	mult
		mpymvrr	acc0,sx0,sy0,shnone,x0,gpra0,y0,gprs8
		satacc	mac0, acc0, round_disable, sat_disable, satp32, 12
		satacc	mac0, acc0, round_disable, sat_enable, satp16, 3				
		mvp	gpra1,udi_mac0acc0lo
		mvp	gpra1,udi_mac0acc0lo
		mvp	gpra1,udi_mac0acc0lo
		mvp	gpra1,udi_mac0acc0lo
		addiu	s1,s1,2		
		jal	wg_add
		lh	a0,0(s0)
		addiu	s2,s2,1		
		slti	t1,s2,40
		sh	v0,0(s0)
		bnez	t1,ACELP_Code_A120
		addiu	s0,s0,2
/*------------------------------------------------------------------------*
		Cor_h(h, rr);
*------------------------------------------------------------------------*/
		
ACELP_Code_A176:	lw	s0,1900(sp)
/*------------------------------------------------------------------------*
		for(i=0; i<L_SUBFR; i++)
    			cor = L_mac(cor, H[i], H[i]);
*------------------------------------------------------------------------*/

		move	s2,zero
		move	s1,zero
		mvp	udi_mac0acc0lo,gprs2
		mvp	udi_mac0acc0hi,gprs2
ACELP_Code_A188:	lh	a1,0(s0)
		//move	a0,s2
		macmvrr	acc0,sx0,sy0,shsat,x0,gpra1,y0,gpra1
		//move	a2,a1
		addiu	t4,s1,1
		sll	s1,t4,0x10
		sra	s1,s1,0x10
		slti	t3,s1,40
		move	s2,v0
		bnez	t3,ACELP_Code_A188
		addiu	s0,s0,2
		mvp 	gprt0,udi_mode
		mvp 	gprt0,udi_mode
		mvp 	gprt0,udi_mode
		mvp 	gprs2,udi_mac0acc0lo
		sra	a0,s2,0x10
		//jal	extract_h
		//move	a0,v0
		//move	a0,v0
		jal	wg_sub
		li	a1,32000
		blez	v0,ACELP_Code_A6468
		lw	a0,1900(sp)
		move	a2,zero
		addiu	a1,sp,16
ACELP_Code_A268:	addiu	s3,a2,1
		lh	t7,0(a0)
		sll	s0,s3,0x10
		sra	a2,s0,0x10
		sra	t6,t7,0x1
		slti	t5,a2,40
		sh	t6,1312(a1)
		addiu	a0,a0,2
		bnez	t5,ACELP_Code_A268
		addiu	a1,a1,2
		addiu	s0,sp,96
ACELP_Code_A312:	addiu	t6,sp,96
		addiu	t4,sp,96
		addiu	t2,sp,96
		addiu	t1,sp,96
		addiu	a1,sp,96
		addiu	a2,sp,96
		addiu	v1,sp,96
		addiu	ra,sp,96
		addiu	t9,sp,96
		addiu	t7,s0,80
		addiu	s2,t2,464
		addiu	t5,t6,208
		addiu	t3,t4,336
		addiu	a3,t1,592
		addiu	t0,a1,720
		addiu	a0,a2,848
		addiu	v0,v1,976
		addiu	s8,ra,1104
		sw	s2,1604(sp)
		addiu	s1,sp,1328
		sw	t7,1592(sp)
		sw	t5,1596(sp)
		sw	t3,1600(sp)
		sw	a3,1608(sp)
		sw	t0,1612(sp)
		sw	a0,1616(sp)
		sw	v0,1620(sp)
		sw	s8,1624(sp)
		addiu	s6,t9,14
		addiu	s5,t9,30
		addiu	s4,t9,46
		addiu	s3,t9,62
		addiu	s2,t9,78
/*------------------------------------------------------------------------*
		for(i=0;  i<NB_POS; i++)
  		{
    			cor = L_mac(cor, *ptr_h1, *ptr_h1); ptr_h1++;
    			*p4-- = extract_h(cor);

    			cor = L_mac(cor, *ptr_h1, *ptr_h1); ptr_h1++;
    			*p3-- = extract_h(cor);

    			cor = L_mac(cor, *ptr_h1, *ptr_h1); ptr_h1++;
    			*p2-- = extract_h(cor);

    			cor = L_mac(cor, *ptr_h1, *ptr_h1); ptr_h1++;
    			*p1-- = extract_h(cor);

    			cor = L_mac(cor, *ptr_h1, *ptr_h1); ptr_h1++;
    			*p0-- = extract_h(cor);
  		}
*------------------------------------------------------------------------*/

		move	s0,zero
		move	s7,zero
		mvp	udi_mac0acc0lo,gprs7
		mvp	udi_mac0acc0hi,gprs7
ACELP_Code_A452:	
		lh	t0,0(s1)
		lh	t1,2(s1)
		lh	t2,4(s1)
		lh	t3,6(s1)
		lh	t4,8(s1)
		macmvrr	acc0,sx0,sy0,shsat,x0,gprt0,y0,gprt0
		macmvrr	acc0,sx0,sy0,shsat,x0,gprt1,y0,gprt1
		macmvrr	acc0,sx0,sy0,shsat,x0,gprt2,y0,gprt2
		macmvrr	acc0,sx0,sy0,shsat,x0,gprt3,y0,gprt3				
		rdp	gprt5,udi_mac0acc0lo
		sra	v0,t5,0x10
		sh	v0,0(s2)		
		macmvrr	acc0,sx0,sy0,shsat,x0,gprt4,y0,gprt4		
		mvp	gprt5,udi_mac0acc0lo
		sra	t1,t5,0x10
		mvp	gprt5,udi_mac0acc0lo
		sra	t2,t5,0x10
		mvp	gprt5,udi_mac0acc0lo
		sra	t3,t5,0x10
		mvp	gprt5,udi_mac0acc0lo
		sra	t4,t5,0x10
		sh	t1,0(s3)
		sh	t2,0(s4)
		sh	t3,0(s5)
		sh	t4,0(s6)
		addiu	s7,s7,1
		slti	t8,s7,8
		addiu	s2,s2,-2
		addiu	s3,s3,-2
		addiu	s4,s4,-2
		addiu	s5,s5,-2
		addiu	s1,s1,10
		bnez	t8,ACELP_Code_A452
		addiu	s6,s6,-2
		
		li	s4,63
		li	s3,62
		addiu	s1,sp,1330
		sw	s4,1632(sp)
		sw	s3,1636(sp)
		sw	s1,1628(sp)
		move	a2,zero
ACELP_Code_A684:	addiu	a3,a2,1
		lw	a0,1636(sp)
		sll	a1,a3,0x10
		lw	t0,1632(sp)
		sra	s8,a1,0x10
		lw	s5,1604(sp)
		lw	s6,1620(sp)
		lw	a2,1592(sp)
		lw	ra,1608(sp)
		sll	v1,t0,0x1
		sll	v0,a0,0x1
		slti	a0,s8,8
		addu	s4,v0,s5
		addu	s7,v1,a2
		addu	s5,v1,s6
		move	s0,zero
		addu	s6,v1,ra
		addiu	s2,sp,1328
		lw	s1,1628(sp)
		mvp	udi_mac0acc0lo,gprs0
		mvp	udi_mac0acc0hi,gprs0
		beqz	a0,ACELP_Code_A956
		move	s3,s8

/*------------------------------------------------------------------------*
		for(k=0; k<NB_POS; k++) {

          		p3 = rri2i3 + l_fin_sup;
         		p2 = rri1i2 + l_fin_sup;
          		p1 = rri0i1 + l_fin_sup;
          		p0 = rri0i4 + l_fin_inf;

          		cor = 0;
          		ptr_h1 = ptr_hd;
          		ptr_h2 =  ptr_hf;

          		for(i=k+(Word16)1; i<NB_POS; i++ ) {

                  		cor = L_mac(cor, *ptr_h1, *ptr_h2); ptr_h1++; ptr_h2++;
                  		cor = L_mac(cor, *ptr_h1, *ptr_h2); ptr_h1++; ptr_h2++;
                  		*p3 = extract_h(cor);

                  		cor = L_mac(cor, *ptr_h1, *ptr_h2); ptr_h1++; ptr_h2++;
                  		*p2 = extract_h(cor);

                  		cor = L_mac(cor, *ptr_h1, *ptr_h2); ptr_h1++; ptr_h2++;
                  		*p1 = extract_h(cor);

                  		cor = L_mac(cor, *ptr_h1, *ptr_h2); ptr_h1++; ptr_h2++;
                  		*p0 = extract_h(cor);

                  		p3 -= ldec;
                  		p2 -= ldec;
                  		p1 -= ldec;
                  		p0 -= ldec;
          		}
          		cor = L_mac(cor, *ptr_h1, *ptr_h2); ptr_h1++; ptr_h2++;
          		cor = L_mac(cor, *ptr_h1, *ptr_h2); ptr_h1++; ptr_h2++;
          		*p3 = extract_h(cor);

          		cor = L_mac(cor, *ptr_h1, *ptr_h2); ptr_h1++; ptr_h2++;
          		*p2 = extract_h(cor);

          		cor = L_mac(cor, *ptr_h1, *ptr_h2); ptr_h1++; ptr_h2++;
          		*p1 = extract_h(cor);

          		l_fin_sup -= NB_POS;
          		l_fin_inf--;
          		ptr_hf += STEP;
  		}
*------------------------------------------------------------------------*/
				
ACELP_Code_A768:	
		lh	a1,0(s1)
		lh	a2,0(s2)
		lh	t1,2(s1)
		lh	t2,2(s2)
		lh	t3,4(s1)
		lh	t4,4(s2)
		lh	t5,6(s1)
		lh	t6,6(s2)
		lh	t7,8(s1)
		lh	t8,8(s2)
		macmvrr	acc0,sx0,sy0,shsat,x0,gpra1,y0,gpra2
		macmvrr	acc0,sx0,sy0,shsat,x0,gprt1,y0,gprt2
		macmvrr	acc0,sx0,sy0,shsat,x0,gprt3,y0,gprt4
		macmvrr	acc0,sx0,sy0,shsat,x0,gprt5,y0,gprt6
		macmvrr	acc0,sx0,sy0,shsat,x0,gprt7,y0,gprt8				
		mvp	gprt5,udi_mac0acc0lo
		sra	t1,t5,0x10				
		mvp	gprt5,udi_mac0acc0lo
		sra	t2,t5,0x10				
		mvp	gprt5,udi_mac0acc0lo
		sra	t3,t5,0x10				
		mvp	gprs0,udi_mac0acc0lo
		sra	t4,s0,0x10		
		sh	t1,0(s5)
		sh	t2,0(s6)
		sh	t3,0(s7)
		sh	t4,0(s4)		
		addiu	s3,s3,1
		slti	a0,s3,8
		addiu	s2,s2,10
		addiu	s1,s1,10
		addiu	s5,s5,-18
		addiu	s6,s6,-18
		addiu	s7,s7,-18
		bnez	a0,ACELP_Code_A768
		addiu	s4,s4,-18
		
ACELP_Code_A956:	
		lh	a1,0(s1)
		lh	a2,0(s2)
		lh	t1,2(s1)
		lh	t2,2(s2)
		lh	t3,4(s1)
		lh	t4,4(s2)
		lh	t5,6(s1)
		lh	t6,6(s2)
		macmvrr	acc0,sx0,sy0,shsat,x0,gpra1,y0,gpra2
		macmvrr	acc0,sx0,sy0,shsat,x0,gprt1,y0,gprt2
		macmvrr	acc0,sx0,sy0,shsat,x0,gprt3,y0,gprt4
		macmvrr	acc0,sx0,sy0,shsat,x0,gprt5,y0,gprt6
		mvp	gprt5,udi_mac0acc0lo		
		mvp	gprt5,udi_mac0acc0lo
		sra	t1,t5,0x10
		mvp	gprt5,udi_mac0acc0lo
		sra	t2,t5,0x10
		mvp	gprt5,udi_mac0acc0lo
		sra	t3,t5,0x10		
		sh	t1,0(s5)
		sh	t2,0(s6)
		sh	t3,0(s7)		
		lw	v1,1628(sp)
		addiu	ra,v1,10
		sw	ra,1628(sp)		
		move	a2,s8
		lw	t9,1636(sp)
		lw	s8,1632(sp)
		addiu	t5,t9,-1
		addiu	t8,s8,-8
		sll	t7,t8,0x10
		sll	t4,t5,0x10
		sra	t6,t7,0x10
		sra	t3,t4,0x10
		slti	s2,a2,8
		sw	t6,1632(sp)
		bnez	s2,ACELP_Code_A684
		sw	t3,1636(sp)
		
		addiu	s6,sp,1332
		li	s1,63
		li	s7,62
		sw	s6,1740(sp)
		sw	s1,1732(sp)
		sw	s7,1736(sp)
		move	a2,zero
ACELP_Code_A1156:	addiu	t4,a2,1
		sll	t3,t4,0x10
		sra	s2,t3,0x10
		sw	s2,1744(sp)
		lw	s0,1732(sp)
		lw	a0,1736(sp)
		lw	t2,1744(sp)
		lw	s5,1596(sp)
		lw	t1,1600(sp)
		lw	a1,1624(sp)
		lw	t0,1612(sp)
		lw	a2,1616(sp)
		sll	a3,s0,0x1
		sll	v0,a0,0x1
		slti	a0,t2,8
		addu	s8,a3,s5
		addu	s6,a3,a1
		addu	s5,v0,t1
		addu	s7,a3,t0
		addu	s4,v0,a2
/*------------------------------------------------------------------------*
		for(k=0; k<NB_POS; k++) {
          		p4 = rri2i4 + l_fin_sup;
          		p3 = rri1i3 + l_fin_sup;
          		p2 = rri0i2 + l_fin_sup;
          		p1 = rri1i4 + l_fin_inf;
          		p0 = rri0i3 + l_fin_inf;

          		cor = 0;
          		ptr_h1 = ptr_hd;
          		ptr_h2 =  ptr_hf;
          		for(i=k+(Word16)1; i<NB_POS; i++ ) {

                  		cor = L_mac(cor, *ptr_h1, *ptr_h2); ptr_h1++; ptr_h2++;
                  		*p4 = extract_h(cor);

                  		cor = L_mac(cor, *ptr_h1, *ptr_h2); ptr_h1++; ptr_h2++;
                  		*p3 = extract_h(cor);

                  		cor = L_mac(cor, *ptr_h1, *ptr_h2); ptr_h1++; ptr_h2++;
                  		*p2 = extract_h(cor);

                  		cor = L_mac(cor, *ptr_h1, *ptr_h2); ptr_h1++; ptr_h2++;
                  		*p1 = extract_h(cor);

                  		cor = L_mac(cor, *ptr_h1, *ptr_h2); ptr_h1++; ptr_h2++;
                  		*p0 = extract_h(cor);
                  		p4 -= ldec;
                  		p3 -= ldec;
                  		p2 -= ldec;
                  		p1 -= ldec;
                  		p0 -= ldec;
          		}
          		cor = L_mac(cor, *ptr_h1, *ptr_h2); ptr_h1++; ptr_h2++;
          		*p4 = extract_h(cor);

          		cor = L_mac(cor, *ptr_h1, *ptr_h2); ptr_h1++; ptr_h2++;
          		*p3 = extract_h(cor);

          		cor = L_mac(cor, *ptr_h1, *ptr_h2); ptr_h1++; ptr_h2++;
          		*p2 = extract_h(cor);


          		l_fin_sup -= NB_POS;
          		l_fin_inf--;
          		ptr_hf += STEP;
  		}
*------------------------------------------------------------------------*/
		
		
		move	s0,zero
		addiu	s1,sp,1328
		mvp	udi_mac0acc0lo,gprs0
		mvp	udi_mac0acc0hi,gprs0
		lw	s2,1740(sp)
		beqz	a0,ACELP_Code_A1464
		lw	s3,1744(sp)
ACELP_Code_A1256:		
		lh	a1,0(s1)
		lh	a2,0(s2)
		lh	t1,2(s1)
		lh	t2,2(s2)
		lh	t3,4(s1)
		lh	t4,4(s2)
		lh	t5,6(s1)
		lh	t6,6(s2)
		lh	t7,8(s1)
		lh	t8,8(s2)
		macmvrr	acc0,sx0,sy0,shsat,x0,gpra1,y0,gpra2
		macmvrr	acc0,sx0,sy0,shsat,x0,gprt1,y0,gprt2
		macmvrr	acc0,sx0,sy0,shsat,x0,gprt3,y0,gprt4
		macmvrr	acc0,sx0,sy0,shsat,x0,gprt5,y0,gprt6		
		rdp	gprt5,udi_mac0acc0lo
		sra	v0,t5,0x10
		sh	v0,0(s6)		
		macmvrr	acc0,sx0,sy0,shsat,x0,gprt7,y0,gprt8		
		mvp	gprt5,udi_mac0acc0lo
		sra	v0,t5,0x10
		sh	v0,0(s7)		
		mvp	gprt5,udi_mac0acc0lo
		sra	v0,t5,0x10
		sh	v0,0(s8)		
		mvp	gprs0,udi_mac0acc0lo
		sra	v0,s0,0x10
		sh	v0,0(s4)		
		mvp	gprs0,udi_mac0acc0lo
		sra	v0,s0,0x10
		sh	v0,0(s5)
		
		addiu	s3,s3,1
		slti	a0,s3,8
		addiu	s1,s1,10
		addiu	s2,s2,10
		addiu	s6,s6,-18
		addiu	s7,s7,-18
		addiu	s8,s8,-18
		addiu	s4,s4,-18
		bnez	a0,ACELP_Code_A1256
		addiu	s5,s5,-18
ACELP_Code_A1464:	
		lh	a1,0(s1)
		lh	a2,0(s2)
		lh	t1,2(s1)
		lh	t2,2(s2)
		lh	t3,4(s1)
		lh	t4,4(s2)
		macmvrr	acc0,sx0,sy0,shsat,x0,gpra1,y0,gpra2
		macmvrr	acc0,sx0,sy0,shsat,x0,gprt1,y0,gprt2
		macmvrr	acc0,sx0,sy0,shsat,x0,gprt3,y0,gprt4
		mvp	gprt5,udi_mac0acc0lo		
		mvp	gprt5,udi_mac0acc0lo
		sra	v0,t5,0x10
		sh	v0,0(s6)		
		mvp	gprt5,udi_mac0acc0lo
		sra	v0,t5,0x10
		sh	v0,0(s7)		
		mvp	gprt5,udi_mac0acc0lo
		sra	v0,t5,0x10
		sh	v0,0(s8)				
		lw	s6,1732(sp)
		lw	s1,1736(sp)
		lw	t9,1740(sp)
		lw	a2,1744(sp)
		addiu	s7,s6,-8
		addiu	s4,s1,-1
		sll	v1,s7,0x10
		sll	s3,s4,0x10
		sra	ra,v1,0x10
		sra	s8,s3,0x10
		addiu	t8,t9,10
		slti	t7,a2,8
		sw	ra,1732(sp)
		sw	s8,1736(sp)
		bnez	t7,ACELP_Code_A1156
		sw	t8,1740(sp)
		li	a2,62
		addiu	t0,sp,1334
		li	v0,63
		sw	a2,1752(sp)
		sw	t0,1756(sp)
		sw	v0,1748(sp)
		move	a2,zero
ACELP_Code_A1648:	addiu	t9,a2,1
		sll	t8,t9,0x10
		sra	t7,t8,0x10
		sw	t7,1760(sp)
		lw	a0,1748(sp)
		lw	t6,1752(sp)
		lw	t5,1760(sp)
		lw	t4,1600(sp)
		lw	t3,1596(sp)
		lw	t2,1616(sp)
		lw	a1,1624(sp)
		lw	a3,1612(sp)
		sll	s2,a0,0x1
		sll	t1,t6,0x1
		slti	a0,t5,8
		addu	s8,s2,t4
		addu	s7,s2,t2
		addu	s6,t1,t3
		addu	s5,t1,a1
		addu	s4,t1,a3
		addiu	s1,sp,1328
		lw	s2,1756(sp)
/*------------------------------------------------------------------------*
		for(k=0; k<NB_POS; k++) {

          	p4 = rri1i4 + l_fin_sup;
          	p3 = rri0i3 + l_fin_sup;
          	p2 = rri2i4 + l_fin_inf;
          	p1 = rri1i3 + l_fin_inf;
          	p0 = rri0i2 + l_fin_inf;

          	ptr_h1 = ptr_hd;
          	ptr_h2 =  ptr_hf;
          	cor = 0;
          	for(i=k+(Word16)1; i<NB_POS; i++ ) {

                  	cor = L_mac(cor, *ptr_h1, *ptr_h2); ptr_h1++; ptr_h2++;
                  	*p4 = extract_h(cor);

                  	cor = L_mac(cor, *ptr_h1, *ptr_h2); ptr_h1++; ptr_h2++;
                  	*p3 = extract_h(cor);

                  	cor = L_mac(cor, *ptr_h1, *ptr_h2); ptr_h1++; ptr_h2++;
                  	*p2 = extract_h(cor);

                  	cor = L_mac(cor, *ptr_h1, *ptr_h2); ptr_h1++; ptr_h2++;
                  	*p1 = extract_h(cor);

                  	cor = L_mac(cor, *ptr_h1, *ptr_h2); ptr_h1++; ptr_h2++;
                  	*p0 = extract_h(cor);

                  	p4 -= ldec;
                  	p3 -= ldec;
                  	p2 -= ldec;
                  	p1 -= ldec;
                  	p0 -= ldec;
          	}
          	cor = L_mac(cor, *ptr_h1, *ptr_h2); ptr_h1++; ptr_h2++;
          	*p4 = extract_h(cor);

          	cor = L_mac(cor, *ptr_h1, *ptr_h2); ptr_h1++; ptr_h2++;
          	*p3 = extract_h(cor);

          	l_fin_sup -= NB_POS;
          	l_fin_inf--;
          	ptr_hf += STEP;
  	}
*------------------------------------------------------------------------*/
				
		move	s0,zero
		mvp	udi_mac0acc0lo,gprs0
		mvp	udi_mac0acc0hi,gprs0		
		beqz	a0,ACELP_Code_A1956
		lw	s3,1760(sp)		
ACELP_Code_A1748:		
		lh	a1,0(s1)
		lh	a2,0(s2)
		lh	t1,2(s1)
		lh	t2,2(s2)
		lh	t3,4(s1)
		lh	t4,4(s2)
		lh	t5,6(s1)
		lh	t6,6(s2)
		lh	t7,8(s1)
		lh	t8,8(s2)
		macmvrr	acc0,sx0,sy0,shsat,x0,gpra1,y0,gpra2
		macmvrr	acc0,sx0,sy0,shsat,x0,gprt1,y0,gprt2
		macmvrr	acc0,sx0,sy0,shsat,x0,gprt3,y0,gprt4
		macmvrr	acc0,sx0,sy0,shsat,x0,gprt5,y0,gprt6		
		rdp	gprt5,udi_mac0acc0lo
		sra	v0,t5,0x10
		sh	v0,0(s7)		
		macmvrr	acc0,sx0,sy0,shsat,x0,gprt7,y0,gprt8		
		mvp	gprt5,udi_mac0acc0lo
		sra	v0,t5,0x10
		sh	v0,0(s8)		
		mvp	gprt5,udi_mac0acc0lo
		sra	v0,t5,0x10
		sh	v0,0(s5)		
		mvp	gprs0,udi_mac0acc0lo
		sra	v0,s0,0x10
		sh	v0,0(s4)		
		mvp	gprs0,udi_mac0acc0lo
		sra	v0,s0,0x10
		sh	v0,0(s6)		
		addiu	s3,s3,1
		slti	a0,s3,8
		addiu	s1,s1,10
		addiu	s2,s2,10
		addiu	s7,s7,-18
		addiu	s8,s8,-18
		addiu	s5,s5,-18
		addiu	s4,s4,-18
		bnez	a0,ACELP_Code_A1748
		addiu	s6,s6,-18
ACELP_Code_A1956:		
		lh	a1,0(s1)
		lh	a2,0(s2)
		lh	t1,2(s1)
		lh	t2,2(s2)
		macmvrr	acc0,sx0,sy0,shsat,x0,gpra1,y0,gpra2
		macmvrr	acc0,sx0,sy0,shsat,x0,gprt1,y0,gprt2
		mvp	gprt5,udi_mac0acc0lo
		mvp	gprt5,udi_mac0acc0lo		
		mvp	gprt5,udi_mac0acc0lo
		sra	v0,t5,0x10
		sh	v0,0(s7)		
		mvp	gprt5,udi_mac0acc0lo
		sra	v0,t5,0x10
		sh	v0,0(s8)	
		lw	t1,1748(sp)
		lw	a3,1752(sp)
		lw	v1,1756(sp)
		lw	a2,1760(sp)
		addiu	t0,t1,-8
		addiu	s6,a3,-1
		sll	s1,s6,0x10
		sll	v0,t0,0x10
		sra	s5,v0,0x10
		sra	s7,s1,0x10
		addiu	s8,v1,10
		slti	s4,a2,8
		sw	s5,1748(sp)
		sw	s7,1752(sp)
		bnez	s4,ACELP_Code_A1648
		sw	s8,1756(sp)
		
		li	a2,62
		addiu	t2,sp,1336
		li	a1,63
		sw	a2,1768(sp)
		sw	t2,1772(sp)
		sw	a1,1764(sp)
		move	a2,zero
ACELP_Code_A2108:	addiu	ra,a2,1
		lw	a0,1764(sp)
		sll	t9,ra,0x10
		lw	t8,1768(sp)
		sra	s8,t9,0x10
		lw	s2,1608(sp)
		lw	t6,1604(sp)
		lw	t5,1592(sp)
		lw	t4,1620(sp)
		sll	t3,t8,0x1
		sll	t7,a0,0x1
		slti	a0,s8,8
		addu	s4,t3,s2
		addu	s7,t7,t6
		addu	s6,t3,t5
		addu	s5,t3,t4
		addiu	s1,sp,1328
		lw	s2,1772(sp)
/*------------------------------------------------------------------------*
 		for(k=0; k<NB_POS; k++) {

          		p3 = rri0i4 + l_fin_sup;
          		p2 = rri2i3 + l_fin_inf;
          		p1 = rri1i2 + l_fin_inf;
          		p0 = rri0i1 + l_fin_inf;

          		ptr_h1 = ptr_hd;
          		ptr_h2 =  ptr_hf;
          		cor = 0;
          		for(i=k+(Word16)1; i<NB_POS; i++ ) {

                  		cor = L_mac(cor, *ptr_h1, *ptr_h2); ptr_h1++; ptr_h2++;
                  		*p3 = extract_h(cor);

                  		cor = L_mac(cor, *ptr_h1, *ptr_h2); ptr_h1++; ptr_h2++;
                  		cor = L_mac(cor, *ptr_h1, *ptr_h2); ptr_h1++; ptr_h2++;
                  		*p2 = extract_h(cor);

                  		cor = L_mac(cor, *ptr_h1, *ptr_h2); ptr_h1++; ptr_h2++;
                  		*p1 = extract_h(cor);

                  		cor = L_mac(cor, *ptr_h1, *ptr_h2); ptr_h1++; ptr_h2++;
                  		*p0 = extract_h(cor);

                  		p3 -= ldec;
                  		p2 -= ldec;
                  		p1 -= ldec;
                  		p0 -= ldec;
          		}
          		cor = L_mac(cor, *ptr_h1, *ptr_h2); ptr_h1++; ptr_h2++;
          		*p3 = extract_h(cor);

          		l_fin_sup -= NB_POS;
          		l_fin_inf--;
          		ptr_hf += STEP;
  		}
*------------------------------------------------------------------------*/
		
		move	s0,zero
		mvp	udi_mac0acc0lo,gprs0
		mvp	udi_mac0acc0hi,gprs0
		beqz	a0,ACELP_Code_A2380
		move	s3,s8		
ACELP_Code_A2192:	
		lh	a1,0(s1)
		lh	a2,0(s2)
		lh	t1,2(s1)
		lh	t2,2(s2)
		lh	t3,4(s1)
		lh	t4,4(s2)
		lh	t5,6(s1)
		lh	t6,6(s2)
		lh	t7,8(s1)
		lh	t8,8(s2)
		macmvrr	acc0,sx0,sy0,shsat,x0,gpra1,y0,gpra2
		macmvrr	acc0,sx0,sy0,shsat,x0,gprt1,y0,gprt2
		macmvrr	acc0,sx0,sy0,shsat,x0,gprt3,y0,gprt4
		macmvrr	acc0,sx0,sy0,shsat,x0,gprt5,y0,gprt6		
		mvp	gprt5,udi_mac0acc0lo
		sra	v0,t5,0x10
		sh	v0,0(s7)		
		macmvrr	acc0,sx0,sy0,shsat,x0,gprt7,y0,gprt8		
		mvp	gprt5,udi_mac0acc0lo
		sra	v0,t5,0x10
		sh	v0,0(s5)				
		mvp	gprt5,udi_mac0acc0lo
		sra	v0,t5,0x10
		sh	v0,0(s4)				
		mvp	gprs0,udi_mac0acc0lo		
		mvp	gprs0,udi_mac0acc0lo
		sra	v0,s0,0x10
		sh	v0,0(s6)
				
		addiu	s3,s3,1
		addiu	s7,s7,-18
		slti	a0,s3,8
		addiu	s1,s1,10
		addiu	s2,s2,10
		addiu	s5,s5,-18
		addiu	s4,s4,-18
		bnez	a0,ACELP_Code_A2192
		addiu	s6,s6,-18
ACELP_Code_A2380:
		lh	a1,0(s1)
		lh	a2,0(s2)
		macmvrr	acc0,sx0,sy0,shsat,x0,gpra1,y0,gpra2
		mvp	gprt5,udi_mac0acc0lo
		mvp	gprt5,udi_mac0acc0lo
		mvp	gprt5,udi_mac0acc0lo
		mvp	gprt5,udi_mac0acc0lo
		sra	v0,t5,0x10
		sh	v0,0(s7)		
		lw	a1,1764(sp)
		lw	s3,1768(sp)
		addiu	t1,a1,-8
		lw	s7,1772(sp)
		addiu	s5,s3,-1
		sll	a3,t1,0x10
		sll	s6,s5,0x10
		move	a2,s8
		slti	s4,s8,8
		sra	v0,a3,0x10
		sra	s1,s6,0x10
		addiu	s8,s7,10
		sw	v0,1764(sp)
		sw	s1,1768(sp)
		bnez	s4,ACELP_Code_A2108
		sw	s8,1772(sp)
		lw	a1,1896(sp)
		lw	a0,1900(sp)
		jal	Cor_h_X
		addiu	a2,sp,16
		addiu	s7,sp,432
		addiu	s8,sp,816
/*------------------------------------------------------------------------*
		static Word16 D4i40_17_fast(
		  	Word16 dn[],         
		  	Word16 rr[],          
		  	Word16 h[],         
		  	Word16 cod[],        
		  	Word16 y[],          
		  	Word16 *sign         
		)
*------------------------------------------------------------------------*/		
		
		
		move	s1,zero
		li	s3,32767
		li	s2,-32768
		b	ACELP_Code_A2548
		addiu	s0,sp,16
		
ACELP_Code_A2516:	addiu	t4,s1,1
		sll	t3,t4,0x10
		sra	s1,t3,0x10
		slti	t2,s1,40
		sh	s3,1392(s0)
		sh	s2,1472(s0)
		beqz	t2,ACELP_Code_A2600
		addiu	s0,s0,2
ACELP_Code_A2548:	lh	a2,0(s0)
		bgez	a2,ACELP_Code_A2516
		move	a0,a2
		sh	s2,1392(s0)
		jal	negate
		sh	s3,1472(s0)
		addiu	t4,s1,1
		sll	t3,t4,0x10
		sra	s1,t3,0x10
		slti	t2,s1,40
		sh	v0,0(s0)
		bnez	t2,ACELP_Code_A2548
		addiu	s0,s0,2
ACELP_Code_A2600:	addiu	s4,sp,176
		addiu	s3,sp,304
		move	s2,s7
		addiu	s1,sp,560
		sw	zero,1640(sp)
		addiu	s6,sp,1408
/*------------------------------------------------------------------------*
  		for(i0=0; i0<L_SUBFR; i0+=STEP)
  		{
    			psign = sign_dn;
    			if (psign[i0] < 0) psign = sign_dn_inv;

    			for(i1=1; i1<L_SUBFR; i1+=STEP)
    			{
      				*p0++ = mult(*p0, psign[i1]);
      				*p1++ = mult(*p1, psign[i1+1]);
      				*p2++ = mult(*p2, psign[i1+2]);
      				*p3++ = mult(*p3, psign[i1+3]);
    			}
  		}
*------------------------------------------------------------------------*/		
ACELP_Code_A2624:	lw	t9,1640(sp)
		addiu	t5,sp,1488
		sll	t8,t9,0x1
		addu	a0,t8,s6
		lh	t7,0(a0)
		li	s5,1
		slti	t6,t7,0
		movz	t5,s6,t6
		addiu	s0,t5,2
ACELP_Code_A2660:
		lh	a0,0(s4)
		lh	a1,0(s0)
		lh	t0,0(s3)
		lh	t1,2(s0)
		lh	t2,0(s2)
		lh	t3,4(s0)
		lh	t4,0(s1)
		lh	t5,6(s0)
		mpymvrr	acc0,sx0,sy0,shnone,x0,gpra0,y0,gpra1
		satacc	mac0, acc0, round_disable, sat_disable, satp32, 12
		satacc	mac0, acc0, round_disable, sat_enable, satp16, 3		
		mpymvrr	acc1,sx0,sy0,shnone,x0,gprt0,y0,gprt1
		satacc	mac0, acc1, round_disable, sat_disable, satp32, 12
		satacc	mac0, acc1, round_disable, sat_enable, satp16, 3		
		mvp	gprt0,udi_mac0acc0lo		
		mpymvrr	acc0,sx0,sy0,shnone,x0,gprt2,y0,gprt3
		satacc	mac0, acc0, round_disable, sat_disable, satp32, 12
		satacc	mac0, acc0, round_disable, sat_enable, satp16, 3		
		mvp	gprt1,udi_mac0acc1lo		
		mpymvrr	acc1,sx0,sy0,shnone,x0,gprt4,y0,gprt5
		satacc	mac0, acc1, round_disable, sat_disable, satp32, 12
		satacc	mac0, acc1, round_disable, sat_enable, satp16, 3		
		mvp	gprt2,udi_mac0acc0lo
		mvp	gprv0,udi_mac0acc1lo
		mvp	gprv0,udi_mac0acc1lo
		mvp	gprv0,udi_mac0acc1lo
		sh	t0,0(s4)
		sh	t1,0(s3)
		sh	t2,0(s2)
		sh	v0,0(s1)		
		addiu	s4,s4,2		
		addiu	s3,s3,2		
		addiu	s2,s2,2
		addiu	s5,s5,5
		slti	ra,s5,40
		addiu	s0,s0,10
		bnez	ra,ACELP_Code_A2660
		addiu	s1,s1,2
		lw	t1,1640(sp)
		addiu	a3,t1,5
		sll	v0,a3,0x10
		sra	s5,v0,0x10
		slti	s0,s5,40
		bnez	s0,ACELP_Code_A2624
		sw	s5,1640(sp)
		
/*------------------------------------------------------------------------*
  		for(i1=1; i1<L_SUBFR; i1+=STEP)
  		{
    			psign = sign_dn;
    			if (psign[i1] < 0) psign = sign_dn_inv;

    			for(i2=2; i2<L_SUBFR; i2+=STEP)
    			{
      				*p0++ = mult(*p0, psign[i2]);
      				*p1++ = mult(*p1, psign[i2+1]);
      				*p2++ = mult(*p2, psign[i2+2]);
    			}
  		}
*------------------------------------------------------------------------*/		
		addiu	s3,sp,688
		move	s2,s8
		addiu	s1,sp,944
		li	s5,1
		sll	t3,s5,0x1
ACELP_Code_A2812:	addu	t2,t3,s6
		lh	a2,0(t2)
		addiu	s4,sp,1488
		slti	a1,a2,0
		movz	s4,s6,a1
		addiu	s0,s4,4
		li	s4,2
ACELP_Code_A2840:		
		lh	a0,0(s3)
		lh	a1,0(s0)
		lh	t0,0(s2)
		lh	t1,2(s0)
		lh	t2,0(s1)
		lh	t3,4(s0)		
		mpymvrr	acc0,sx0,sy0,shnone,x0,gpra0,y0,gpra1
		satacc	mac0, acc0, round_disable, sat_disable, satp32, 12
		satacc	mac0, acc0, round_disable, sat_enable, satp16, 3				
		mpymvrr	acc1,sx0,sy0,shnone,x0,gprt0,y0,gprt1
		satacc	mac0, acc1, round_disable, sat_disable, satp32, 12
		satacc	mac0, acc1, round_disable, sat_enable, satp16, 3				
		mvp	gprt0,udi_mac0acc0lo				
		mpymvrr	acc0,sx0,sy0,shnone,x0,gprt2,y0,gprt3
		satacc	mac0, acc0, round_disable, sat_disable, satp32, 12
		satacc	mac0, acc0, round_disable, sat_enable, satp16, 3				
		mvp	gprt1,udi_mac0acc1lo		
		mvp	gprv0,udi_mac0acc0lo
		mvp	gprv0,udi_mac0acc0lo
		mvp	gprv0,udi_mac0acc0lo
		
		sh	t0,0(s3)
		sh	t1,0(s2)
		sh	v0,0(s1)				
		addiu	s3,s3,2		
		addiu	s2,s2,2
		addiu	s4,s4,5
		slti	t4,s4,40
		addiu	s0,s0,10
		bnez	t4,ACELP_Code_A2840
		addiu	s1,s1,2
		addiu	s5,s5,5
		slti	t7,s5,40
		bnez	t7,ACELP_Code_A2812
		sll	t3,s5,0x1
		addiu	s2,sp,1072
		addiu	s1,sp,1200
		li	s4,2
		
/*------------------------------------------------------------------------*
		for(i2=2; i2<L_SUBFR; i2+=STEP)
		{
		    	psign = sign_dn;
		    	if (psign[i2] < 0) psign = sign_dn_inv;
		
		    	for(i3=3; i3<L_SUBFR; i3+=STEP)
		    	{
		      		*p0++ = mult(*p0, psign[i3]);
		      		*p1++ = mult(*p1, psign[i3+1]);
		    	}
		}
*------------------------------------------------------------------------*/
		
		sll	v1,s4,0x1
ACELP_Code_A2964:	addu	t0,v1,s6
		lh	ra,0(t0)
		addiu	s3,sp,1488
		slti	t9,ra,0
		movz	s3,s6,t9
		addiu	s0,s3,6
		li	s3,3
ACELP_Code_A2992:
		lh	a0,0(s2)
		lh	a1,0(s0)
		lh	t0,0(s1)
		lh	t1,2(s0)		
		mpymvrr	acc0,sx0,sy0,shnone,x0,gpra0,y0,gpra1
		satacc	mac0, acc0, round_disable, sat_disable, satp32, 12
		satacc	mac0, acc0, round_disable, sat_enable, satp16, 3						
		mpymvrr	acc1,sx0,sy0,shnone,x0,gprt0,y0,gprt1
		satacc	mac0, acc1, round_disable, sat_disable, satp32, 12
		satacc	mac0, acc1, round_disable, sat_enable, satp16, 3						
		mvp	gprt0,udi_mac0acc0lo			
		mvp	gprv0,udi_mac0acc1lo
		mvp	gprv0,udi_mac0acc1lo
		mvp	gprv0,udi_mac0acc1lo
		sh	t0,0(s2)
		sh	v0,0(s1)
		
		addiu	s2,s2,2
		addiu	s3,s3,5
		slti	s5,s3,40
		addiu	s0,s0,10
		bnez	s5,ACELP_Code_A2992
		addiu	s1,s1,2
		addiu	v0,s4,5
		sll	s4,v0,0x10
		sra	s4,s4,0x10
		slti	s0,s4,40
		bnez	s0,ACELP_Code_A2964
		sll	v1,s4,0x1
		
		li	s2,3
		sw	s7,1700(sp)
		sw	s8,1704(sp)
		li	s7,1
		addiu	a1,sp,1072
		addiu	s6,sp,144
		li	s1,-1
		li	s8,2
		sw	a1,1708(sp)
		sw	s6,1712(sp)
		sw	s1,1676(sp)
		sw	s7,1688(sp)
		sw	zero,1644(sp)
		sw	s7,1648(sp)
		sw	s8,1652(sp)
		sw	s2,1656(sp)
		sw	zero,1660(sp)
		sw	zero,1664(sp)
		sw	zero,1680(sp)
		sw	s2,1668(sp)
ACELP_Code_A3160:	lw	t2,1668(sp)
		li	t3,-1
		li	t4,1
		slti	a2,t2,40
		sw	t3,1684(sp)
		sw	t4,1692(sp)
		sw	t3,1672(sp)
		sw	zero,1776(sp)
		sw	a2,1720(sp)
ACELP_Code_A3196:	addiu	t5,sp,16
		li	s2,-1
		li	s1,2
		addiu	s0,t5,4
ACELP_Code_A3212:	lh	a0,0(s0)
		jal	wg_sub
		move	a1,s2
		move	a1,s1
		blez	v0,ACELP_Code_A3260
		lw	a0,1672(sp)
		jal	wg_sub
		nop
		beqz	v0,ACELP_Code_A3264
		addiu	a0,s1,5
		lh	s2,0(s0)
		sw	s1,1640(sp)
ACELP_Code_A3260:	addiu	a0,s1,5
ACELP_Code_A3264:	sll	t7,a0,0x10
		sra	s1,t7,0x10
		slti	t6,s1,40
		bnez	t6,ACELP_Code_A3212
		addiu	s0,s0,10
		lw	a0,1640(sp)
		jal	mult
		li	a1,6554
		move	s5,v0
		lw	t1,1640(sp)
		sll	s4,v0,0x1
		addiu	v0,sp,128
		addu	s0,s4,v0
		sll	v1,t1,0x1
		addiu	a3,sp,16
		lh	a0,0(s0)
		addu	t0,v1,a3
		li	a1,8192
		//jal	L_mult
		lh	s8,0(t0)
		mpymvrr	acc0,sx0,sy0,shsat,x0,gpra0,y0,gpra1
		mvp	gprs7,udi_mac0acc0lo
		mvp	gprs7,udi_mac0acc0lo
		mvp	gprs7,udi_mac0acc0lo
		mvp	gprs7,udi_mac0acc0lo
		mvp	gpra3,udi_mac0acc0lo
		//move	s7,v0
		
		move	a0,s5
		jal	wg_shl
		li	a1,3
		lw	t9,1708(sp)
		lw	s3,1640(sp)
		lw	t8,1720(sp)
		sll	ra,v0,0x1
		addu	s6,ra,t9
		sw	s3,1672(sp)
		lw	s5,1712(sp)
		beqz	t8,ACELP_Code_A3580
		lw	s2,1668(sp)
		sll	a1,s2,0x1
		addiu	s1,sp,16
		addu	s4,a1,s1
		lh	a1,0(s4)
ACELP_Code_A3412:	jal	wg_add
		move	a0,s8
		mvp	udi_mac0acc0lo,gprs7
		mvp	udi_mac0acc0hi,gpra3
		lh	a1,0(s6)
		li	a2,16384
		macmvrr	acc0,sx0,sy0,shsat,x0,gpra1,y0,gpra2
		lh	t1,0(s5)
		li	t2,8192
		move	s1,v0
		macmvrr	acc0,sx0,sy0,shsat,x0,gprt1,y0,gprt2
		satacc	mac0, acc0, round_enable, sat_enable, satp16, 16
		
		mpymvrr	acc1,sx0,sy0,shnone,x0,gprs1,y0,gprs1
		satacc	mac0, acc1, round_disable, sat_disable, satp32, 12
		satacc	mac0, acc1, round_disable, sat_enable, satp16, 3
		
		mvp	gprs0,udi_mac0acc0lo
		
		mvp	gprs3,udi_mac0acc1lo
		mvp	gprs3,udi_mac0acc1lo
		mvp	gprs3,udi_mac0acc1lo
		
		lw	a0,1692(sp)
		lw	a1,1684(sp)
		mpymvrr	acc0,sx0,sy0,shsat,x0,gpra0,y0,gprs3
		move	a2,s0
		msumvrr	acc0,sx0,sy0,shsat,x0,gpra1,y0,gpra2
		
		mvp	gprv0,udi_mac0acc0lo		
		mvp	gprv0,udi_mac0acc0lo
		mvp	gprv0,udi_mac0acc0lo
		mvp	gprv0,udi_mac0acc0lo
		
		addiu	s6,s6,2
		addiu	s5,s5,2
		addiu	v1,s2,5
		blez	v0,ACELP_Code_A3560
		addiu	s4,s4,10
		lw	a2,1640(sp)
		sw	s3,1684(sp)
		sw	s1,1680(sp)
		sw	s0,1692(sp)
		sw	a2,1660(sp)
		sw	s2,1664(sp)
ACELP_Code_A3560:	sll	s2,v1,0x10
		sra	s2,s2,0x10
		slti	t2,s2,40
		bnezl	t2,ACELP_Code_A3412
		lh	a1,0(s4)
ACELP_Code_A3580:	lw	t4,1776(sp)
		addiu	t3,t4,1
		sll	s6,t3,0x10
		sra	s7,s6,0x10
		slti	s8,s7,2
		bnez	s8,ACELP_Code_A3196
		sw	s7,1776(sp)
		lw	a0,1664(sp)
		jal	mult
		li	a1,6554
		move	a0,v0
		jal	wg_shl
		li	a1,3
		
		lw	a0,1692(sp)
		
		
		jal	L_mult
		li	a1,8192
		sw	v0,1696(sp)
		
		lw	a0,1660(sp)		
		jal	mult
		li	a1,6554
		
		lw	a0,1664(sp)
		sll	ra,v0,0x1
		addiu	t9,sp,688
		addu	s2,ra,t9
		jal	mult
		li	a1,6554
		lw	t8,1704(sp)
		lw	a0,1660(sp)
		lw	t7,1664(sp)
		sll	s3,v0,0x1
		li	t6,-1
		li	t5,1
		addu	s1,s3,t8
		sw	a0,1800(sp)
		sw	t7,1804(sp)
		sw	t6,1796(sp)
		sw	t5,1792(sp)
		addiu	s0,sp,112
		addiu	s4,sp,1568
		li	s3,1
		li	a1,8192
		li	t2,4096
ACELP_Code_A3740:	
		lh	a0,0(s2)		
		lh	t0,0(s1)
		lh	t1,0(s0)		
		mpymvrr	acc0,sx0,sy0,shsat,x0,gpra0,y0,gpra1
		macmvrr	acc0,sx0,sy0,shsat,x0,gprt0,y0,gpra1
		macmvrr	acc0,sx0,sy0,shsat,x0,gprt1,y0,gprt2
		satacc	mac0, acc0, round_enable, sat_enable, satp16, 16
		mvp	gprv0,udi_mac0acc0lo
		mvp	gprv0,udi_mac0acc0lo
		mvp	gprv0,udi_mac0acc0lo
		mvp	gprv0,udi_mac0acc0lo		
		addiu	s3,s3,5
		slti	s5,s3,40
		sh	v0,0(s4)
		addiu	s2,s2,16
		addiu	s1,s1,16
		addiu	s0,s0,2
		bnez	s5,ACELP_Code_A3740
		addiu	s4,s4,2
		
		lw	a0,1660(sp)
		jal	mult
		li	a1,6554
		sll	t2,v0,0x1
		addiu	a2,sp,304
		lw	a0,1664(sp)
		addu	s0,t2,a2
		li	a1,6554
		jal	mult
		sw	s0,1780(sp)
		lw	s1,1700(sp)
		sll	v0,v0,0x1
		addu	s4,v0,s1
		addiu	t1,sp,96
		addiu	a3,sp,16
		sw	s4,1784(sp)
		sw	t1,1788(sp)		
		
		addiu	s4,sp,176
		move	s8,zero
		sw	a3,1728(sp)
ACELP_Code_A3912:	lw	s7,1728(sp)
		lw	a0,1680(sp)		
		
		lh	a1,0(s7)
		jal	wg_add
		addiu	s5,sp,1568
		lw	t8,1780(sp)
		lw	a0,1696(sp)
		move	s7,v0
		
		mvp	udi_mac0acc0lo,gpra0
		srl	v0,a0,31		
		move	t5,zero
		li	t2,0xff
		movn	t5,t2,v0		
		mvp	udi_mac0acc0hi,gprt5
		
		lh	a1,0(t8)
		li	a2,4096
		lw	t0,1784(sp)
		lh	t1,0(t0)
		lw	s6,1788(sp)
		lh	t2,0(s6)
		li	t3,2048
		
		macmvrr	acc0,sx0,sy0,shsat,x0,gpra1,y0,gpra2
		macmvrr	acc0,sx0,sy0,shsat,x0,gprt1,y0,gpra2
		macmvrr	acc0,sx0,sy0,shsat,x0,gprt2,y0,gprt3
		mvp	gprv0,udi_mac0acc0lo
		mvp	gprv0,udi_mac0acc0lo
		mvp	gprv0,udi_mac0acc0lo
		mvp	gprs6,udi_mac0acc0lo
		mvp	gpra3,udi_mac0acc0hi
		
		lw	t7,1780(sp)
		lw	t5,1784(sp)
		lw	t3,1788(sp)
		addiu	t6,t7,16
		addiu	s2,t3,2
		addiu	t4,t5,16
		addiu	a1,sp,16
		sw	s2,1788(sp)
		//move	s6,v0
		sw	t6,1780(sp)
		sw	t4,1784(sp)
		li	s3,1
		addiu	s2,a1,2
		lh	a1,0(s2)
ACELP_Code_A4052:	jal	wg_add
		move	a0,s7
		lh	a1,0(s4)		
		mvp	udi_mac0acc0lo,gprs6
		mvp	udi_mac0acc0hi,gpra3
		li	a2,4096
		move	s1,v0
		lh	t1,0(s5)
		li	t2,16384
		macmvrr	acc0,sx0,sy0,shsat,x0,gpra1,y0,gpra2
		macmvrr	acc0,sx0,sy0,shsat,x0,gprt1,y0,gprt2		
		//move	a0,s1
		//move	a1,s1
		satacc	mac0, acc0, round_enable, sat_enable, satp16, 16
		mpymvrr	acc1,sx0,sy0,shnone,x0,gprs1,y0,gprs1
		satacc	mac0, acc1, round_disable, sat_disable, satp32, 12
		satacc	mac0, acc1, round_disable, sat_enable, satp16, 3		
		
		mvp	gprs0,udi_mac0acc0lo
		mvp	gprs1,udi_mac0acc1lo
		mvp	gprs1,udi_mac0acc1lo
		mvp	gprs1,udi_mac0acc1lo
		lw	a0,1792(sp)
		lw	a1,1796(sp)
		//move	a2,s0
		mpymvrr	acc0,sx0,sy0,shsat,x0,gpra0,y0,gprs1
		msumvrr	acc0,sx0,sy0,shsat,x0,gpra1,y0,gprs0
		mvp	gprv0,udi_mac0acc0lo
		mvp	gprv0,udi_mac0acc0lo
		mvp	gprv0,udi_mac0acc0lo
		mvp	gprv0,udi_mac0acc0lo
		
		addiu	s4,s4,2
		addiu	s5,s5,2
		addiu	v1,s3,5
		blez	v0,ACELP_Code_A4192
		addiu	s2,s2,10
		sw	s1,1796(sp)
		sw	s0,1792(sp)
		sw	s8,1660(sp)
		sw	s3,1664(sp)
ACELP_Code_A4192:	//sll	s3,v1,0x10
		//sra	s3,s3,0x10
		move	s3,v1
		slti	t9,s3,40
		bnezl	t9,ACELP_Code_A4052
		lh	a1,0(s2)
		addiu	s8,s8,5
		lw	t0,1728(sp)
		//sll	s8,v1,0x10
		//sra	s8,s8,0x10
		addiu	s5,t0,10
		slti	ra,s8,40
		bnez	ra,ACELP_Code_A3912
		sw	s5,1728(sp)
		lw	a0,1688(sp)
		jal	L_mult
		lw	a1,1796(sp)
		lw	a1,1676(sp)
		lw	a2,1792(sp)
		jal	L_msu
		move	a0,v0
		blez	v0,ACELP_Code_A4324
		lw	t2,1796(sp)
		lw	a2,1792(sp)
		lw	v0,1804(sp)
		lw	s4,1800(sp)
		lw	t1,1660(sp)
		lw	a3,1664(sp)
		sw	t2,1676(sp)
		sw	a2,1688(sp)
		sw	v0,1656(sp)
		sw	s4,1652(sp)
		sw	t1,1644(sp)
		sw	a3,1648(sp)
ACELP_Code_A4324:	li	a1,-1
		li	s2,1
		sw	a1,1816(sp)
		sw	s2,1812(sp)
		sw	a1,1808(sp)
		sw	zero,1820(sp)
ACELP_Code_A4348:	lw	t3,1720(sp)
		li	s2,-1
		beqz	t3,ACELP_Code_A4448
		lw	s0,1668(sp)
		sll	t5,s0,0x1
		addiu	t4,sp,16
		addu	s1,t5,t4
ACELP_Code_A4376:	lh	a0,0(s1)
		jal	wg_sub
		move	a1,s2
		move	a1,s0
		blez	v0,ACELP_Code_A4424
		lw	a0,1808(sp)
		jal	wg_sub
		nop
		beqz	v0,ACELP_Code_A4428
		addiu	s6,s0,5
		lh	s2,0(s1)
		sw	s0,1800(sp)
ACELP_Code_A4424:	addiu	s6,s0,5
ACELP_Code_A4428:	sll	t7,s6,0x10
		sra	s0,t7,0x10
		slti	t6,s0,40
		bnez	t6,ACELP_Code_A4376
		addiu	s1,s1,10
ACELP_Code_A4448:	lw	a0,1800(sp)
		jal	mult
		li	a1,6554
		lw	s5,1712(sp)
		sll	s0,v0,0x1
		lw	s3,1800(sp)
		addu	ra,s0,s5
		lh	a0,0(ra)
		sll	s1,s3,0x1
		addiu	t9,sp,16
		addu	s7,s1,t9
		li	a1,8192
		jal	L_mult
		lh	s8,0(s7)
		lw	t8,1700(sp)
		lw	a0,1800(sp)
		move	s7,v0
		addu	s5,s0,t8
		sw	a0,1808(sp)
		addiu	s4,sp,96
		move	s6,zero
		addiu	s3,sp,16
		lh	a1,0(s3)
ACELP_Code_A4540:	jal	wg_add
		move	a0,s8
		move	a0,s7
		move	s1,v0
		
		mvp	udi_mac0acc0lo,gpra0
		srl	v0,a0,31		
		move	t5,zero
		li	t2,0xff
		movn	t5,t2,v0		
		mvp	udi_mac0acc0hi,gprt5
		
		lh	a1,0(s5)
		li	a2,16384
		li	t0,8192
		lh	t1,0(s4)
		
		macmvrr	acc0,sx0,sy0,shsat,x0,gpra1,y0,gpra2
		macmvrr	acc0,sx0,sy0,shsat,x0,gprt1,y0,gprt0
		//move	a0,s1
		//move	a1,s1
		satacc	mac0, acc0, round_enable, sat_enable, satp16, 16
		mpymvrr	acc1,sx0,sy0,shnone,x0,gprs1,y0,gprs1
		satacc	mac0, acc1, round_disable, sat_disable, satp32, 12
		satacc	mac0, acc1, round_disable, sat_enable, satp16, 3		
		
		mvp	gprs0,udi_mac0acc0lo
		mvp	gprs2,udi_mac0acc1lo
		mvp	gprs2,udi_mac0acc1lo
		mvp	gprs2,udi_mac0acc1lo
		lw	a0,1812(sp)
		lw	a1,1816(sp)
		//move	a2,s0
		mpymvrr	acc0,sx0,sy0,shsat,x0,gpra0,y0,gprs2
		msumvrr	acc0,sx0,sy0,shsat,x0,gpra1,y0,gprs0
		mvp	gprv0,udi_mac0acc0lo
		mvp	gprv0,udi_mac0acc0lo
		mvp	gprv0,udi_mac0acc0lo
		mvp	gprv0,udi_mac0acc0lo
		
		addiu	s5,s5,16
		addiu	s4,s4,2
		addiu	v1,s6,5
		blez	v0,ACELP_Code_A4688
		addiu	s3,s3,10
		lw	t0,1800(sp)
		sw	s2,1816(sp)
		sw	s1,1680(sp)
		sw	s0,1812(sp)
		sw	t0,1660(sp)
		sw	s6,1664(sp)
ACELP_Code_A4688:	//sll	a3,v1,0x10
		//sra	s6,a3,0x10
		move	s6,v1
		slti	v1,s6,40
		bnezl	v1,ACELP_Code_A4540
		lh	a1,0(s3)
		lw	a2,1820(sp)
		addiu	v0,a2,1
		sll	s4,v0,0x10
		sra	t1,s4,0x10
		slti	s8,t1,2
		bnez	s8,ACELP_Code_A4348
		sw	t1,1820(sp)
		lw	a0,1664(sp)
		jal	mult
		li	a1,6554
		move	a0,v0
		jal	wg_shl
		li	a1,3
		lw	a0,1812(sp)
		li	a1,8192
		jal	L_mult
		move	s6,v0
		lw	a0,1660(sp)
		li	a1,6554
		jal	mult
		sw	v0,1828(sp)
		lw	t5,1708(sp)
		lw	t4,1660(sp)
		lw	t3,1664(sp)
		li	s2,-1
		sll	t6,v0,0x1
		sll	s0,s6,0x1
		li	a1,1
		addiu	t2,sp,304
		sw	s2,1844(sp)
		addu	s3,t6,t5
		sw	t4,1640(sp)
		sw	t3,1848(sp)
		sw	a1,1840(sp)
		addu	s2,s0,t2
		addiu	s1,sp,128
		addiu	s5,sp,1568
		li	s4,2
		li	a1,8192
		li	t2,4096	
ACELP_Code_A4860:	
		lh	a0,0(s3)
				
		lh	t0,0(s2)
		lh	t1,0(s1)
			
		mpymvrr	acc0,sx0,sy0,shsat,x0,gpra0,y0,gpra1
		macmvrr	acc0,sx0,sy0,shsat,x0,gprt0,y0,gpra1
		macmvrr	acc0,sx0,sy0,shsat,x0,gprt1,y0,gprt2
		satacc	mac0, acc0, round_enable, sat_enable, satp16, 16
		mvp	gprv0,udi_mac0acc0lo
		mvp	gprv0,udi_mac0acc0lo
		mvp	gprv0,udi_mac0acc0lo
		mvp	gprv0,udi_mac0acc0lo
		
		addiu	s4,s4,5
		slti	t7,s4,40
		sh	v0,0(s5)
		addiu	s3,s3,16
		addiu	s2,s2,2
		addiu	s1,s1,2
		bnez	t7,ACELP_Code_A4860
		addiu	s5,s5,2
		lw	a0,1660(sp)
		jal	mult
		li	a1,6554
		lw	v1,1704(sp)
		sll	t0,v0,0x1
		addiu	ra,sp,176
		addiu	s1,sp,16
		addu	s5,t0,v1
		addu	s3,s0,ra
		addiu	t9,sp,112
		addiu	s7,s1,2
		sw	s5,1824(sp)
		sw	s3,1832(sp)
		sw	t9,1836(sp)
		addiu	s4,sp,688
		li	s8,1
		sw	s7,1724(sp)
ACELP_Code_A5020:	lw	t5,1724(sp)
		lw	a0,1680(sp)
		lh	a1,0(t5)
		jal	wg_add
		addiu	s0,sp,16
		lw	t4,1824(sp)
		lw	a0,1828(sp)
		move	s7,v0
		
		mvp	udi_mac0acc0lo,gpra0
		srl	v0,a0,31		
		move	t5,zero
		li	t2,0xff
		movn	t5,t2,v0		
		mvp	udi_mac0acc0hi,gprt5
		
		lh	a1,0(t4)
		li	a2,4096
		lw	t0,1832(sp)
		lh	t1,0(t0)
		lw	s2,1836(sp)
		lh	t2,0(s2)
		li	t3,2048
		
		macmvrr	acc0,sx0,sy0,shsat,x0,gpra1,y0,gpra2
		macmvrr	acc0,sx0,sy0,shsat,x0,gprt1,y0,gpra2
		macmvrr	acc0,sx0,sy0,shsat,x0,gprt2,y0,gprt3
		mvp	gprv0,udi_mac0acc0lo
		mvp	gprv0,udi_mac0acc0lo
		mvp	gprv0,udi_mac0acc0lo
		mvp	gprs6,udi_mac0acc0lo
		mvp	gpra3,udi_mac0acc0hi
		
		lw	a1,1824(sp)
		lw	a2,1832(sp)
		lw	t1,1836(sp)
		addiu	t2,a1,16
		addiu	v0,a2,2
		addiu	t1,t1,2
		sw	t2,1824(sp)
		sw	v0,1832(sp)
		sw	t1,1836(sp)
		addiu	s5,sp,1568
		li	s3,2
		addiu	s2,s0,4
		lh	a1,0(s2)
ACELP_Code_A5160:	jal	wg_add
		move	a0,s7
		lh	a1,0(s4)		
		mvp	udi_mac0acc0lo,gprs6
		mvp	udi_mac0acc0hi,gpra3
		li	a2,4096
		move	s1,v0
		lh	t1,0(s5)
		li	t2,16384
		macmvrr	acc0,sx0,sy0,shsat,x0,gpra1,y0,gpra2
		macmvrr	acc0,sx0,sy0,shsat,x0,gprt1,y0,gprt2		
		move	a0,s1
		move	a1,s1
		satacc	mac0, acc0, round_enable, sat_enable, satp16, 16
		mpymvrr	acc1,sx0,sy0,shnone,x0,gpra0,y0,gpra1
		satacc	mac0, acc1, round_disable, sat_disable, satp32, 12
		satacc	mac0, acc1, round_disable, sat_enable, satp16, 3		
		
		mvp	gprs0,udi_mac0acc0lo
		mvp	gprs1,udi_mac0acc1lo
		mvp	gprs1,udi_mac0acc1lo
		mvp	gprs1,udi_mac0acc1lo
		lw	a0,1840(sp)
		lw	a1,1844(sp)
		move	a2,s0
		mpymvrr	acc0,sx0,sy0,shsat,x0,gpra0,y0,gprs1
		msumvrr	acc0,sx0,sy0,shsat,x0,gpra1,y0,gpra2
		mvp	gprv0,udi_mac0acc0lo
		mvp	gprv0,udi_mac0acc0lo
		mvp	gprv0,udi_mac0acc0lo
		mvp	gprv0,udi_mac0acc0lo		
		
		addiu	s4,s4,2
		addiu	s5,s5,2
		addiu	v1,s3,5
		blez	v0,ACELP_Code_A5300
		addiu	s2,s2,10
		sw	s1,1844(sp)
		sw	s0,1840(sp)
		sw	s8,1660(sp)
		sw	s3,1664(sp)
ACELP_Code_A5300:	sll	t7,v1,0x10
		sra	s3,t7,0x10
		slti	t6,s3,40
		bnezl	t6,ACELP_Code_A5160
		lh	a1,0(s2)
		addiu	s7,s8,5
		lw	t8,1724(sp)
		sll	s8,s7,0x10
		sra	s8,s8,0x10
		addiu	a0,t8,10
		slti	s6,s8,40
		bnez	s6,ACELP_Code_A5020
		sw	a0,1724(sp)
		lw	a0,1688(sp)
		jal	L_mult
		lw	a1,1844(sp)
		lw	a1,1676(sp)
		lw	a2,1840(sp)
		jal	L_msu
		move	a0,v0
		blez	v0,ACELP_Code_A5432
		lw	t0,1844(sp)
		lw	s5,1840(sp)
		lw	ra,1848(sp)
		lw	s3,1640(sp)
		lw	t9,1660(sp)
		lw	s4,1664(sp)
		sw	t0,1676(sp)
		sw	s5,1688(sp)
		sw	ra,1644(sp)
		sw	s3,1656(sp)
		sw	t9,1648(sp)
		sw	s4,1652(sp)
ACELP_Code_A5432:	lw	t3,1668(sp)
		addiu	a2,sp,560
		addiu	s2,t3,1
		sll	a1,s2,0x10
		sra	t2,a1,0x10
		addiu	v0,sp,944
		addiu	t1,sp,1200
		addiu	a3,sp,160
		slti	v1,t2,5
		sw	t2,1668(sp)
		sw	a2,1700(sp)
		sw	v0,1704(sp)
		sw	t1,1708(sp)
		bnez	v1,ACELP_Code_A3160
		sw	a3,1712(sp)
		lw	a0,1652(sp)
		lw	s6,1656(sp)
		lw	s8,1644(sp)
		lw	t8,1648(sp)
		sll	t0,a0,0x1
		sll	t1,s6,0x1
		addiu	t7,sp,16
		addiu	t6,sp,16
		sll	a2,s8,0x1
		sll	a3,t8,0x1
		addu	t4,t1,t7
		addu	s1,a2,t7
		addu	s0,a3,t6
		addu	t5,t0,t6
		lh	s3,1392(s1)
		lh	s4,1392(s0)
		lh	s5,1392(t5)
		lh	s6,1392(t4)
		lw	a0,1912(sp)
		move	v0,zero
ACELP_Code_A5572:	addiu	ra,v0,1
		sll	t9,ra,0x10
		sra	v0,t9,0x10
		slti	s7,v0,40
		sh	zero,0(a0)
		bnez	s7,ACELP_Code_A5572
		addiu	a0,a0,2
		lw	t3,1912(sp)
		sra	v0,s3,0x2
		addu	s2,t0,t3
		lw	t0,1644(sp)
		addu	a2,a2,t3
		addu	a3,a3,t3
		addu	t1,t1,t3
		sra	v1,s4,0x2
		sra	a1,s5,0x2
		sra	t2,s6,0x2
		sh	v0,0(a2)
		sh	v1,0(a3)
		move	v0,zero
		sh	a1,0(s2)
		blez	t0,ACELP_Code_A5700
		sh	t2,0(t1)
		lw	a0,1916(sp)
ACELP_Code_A5668:	addiu	s1,v0,1
		sll	s0,s1,0x10
		lw	t5,1644(sp)
		sra	v0,s0,0x10
		slt	t4,v0,t5
		sh	zero,0(a0)
		bnez	t4,ACELP_Code_A5668
		addiu	a0,a0,2
ACELP_Code_A5700:	blez	s3,ACELP_Code_A6804
		lw	t2,1644(sp)
		slti	t6,t2,40
		beqz	t6,ACELP_Code_A5772
		move	a2,t2
		lw	t7,1916(sp)
		sll	a0,t2,0x1
		addu	a1,a0,t7
		lw	a0,1900(sp)
ACELP_Code_A5736:	addiu	t9,a2,1
		sll	s7,t9,0x10
		lhu	s8,0(a0)
		sra	a2,s7,0x10
		slti	t8,a2,40
		sh	s8,0(a1)
		addiu	a0,a0,2
		bnez	t8,ACELP_Code_A5736
		addiu	a1,a1,2
ACELP_Code_A5772:	blez	s4,ACELP_Code_A6720
		lw	t2,1648(sp)
		slti	a3,t2,40
		beqz	a3,ACELP_Code_A5852
		move	s2,t2
		lw	v1,1916(sp)
		sll	a2,t2,0x1
		lw	s1,1900(sp)
		addu	s0,a2,v1
ACELP_Code_A5808:	lh	a0,0(s0)
		lh	a1,0(s1)
		jal	wg_add
		addiu	s1,s1,2
		addiu	t5,s2,1
		sll	t4,t5,0x10
		sra	s2,t4,0x10
		slti	t3,s2,40
		sh	v0,0(s0)
		bnez	t3,ACELP_Code_A5808
		addiu	s0,s0,2
ACELP_Code_A5852:	blez	s5,ACELP_Code_A6636
		lw	t2,1652(sp)
		slti	s0,t2,40
		beqz	s0,ACELP_Code_A5932
		move	s2,t2
		lw	s8,1916(sp)
		sll	s7,t2,0x1
		lw	s1,1900(sp)
		addu	s0,s7,s8
ACELP_Code_A5888:	lh	a0,0(s0)
		lh	a1,0(s1)
		jal	wg_add
		addiu	s1,s1,2
		addiu	t0,s2,1
		sll	ra,t0,0x10
		sra	s2,ra,0x10
		slti	t9,s2,40
		sh	v0,0(s0)
		bnez	t9,ACELP_Code_A5888
		addiu	s0,s0,2
ACELP_Code_A5932:	blez	s6,ACELP_Code_A6552
		lw	t2,1656(sp)
		slti	a2,t2,40
		beqz	a2,ACELP_Code_A6012
		move	s2,t2
		lw	t3,1916(sp)
		sll	t4,t2,0x1
		lw	s1,1900(sp)
		addu	s0,t4,t3
ACELP_Code_A5968:	lh	a0,0(s0)
		lh	a1,0(s1)
		jal	wg_add
		addiu	s1,s1,2
		addiu	t7,s2,1
		sll	t6,t7,0x10
		sra	s2,t6,0x10
		slti	t5,s2,40
		sh	v0,0(s0)
		bnez	t5,ACELP_Code_A5968
		addiu	s0,s0,2
ACELP_Code_A6012:	blez	s3,ACELP_Code_A6032
		move	a0,zero
		jal	wg_add
		li	a1,1
		move	a0,v0
ACELP_Code_A6032:	blez	s4,ACELP_Code_A6052
		nop
		jal	wg_add
		li	a1,2
		move	a0,v0
ACELP_Code_A6052:	blez	s5,ACELP_Code_A6072
		nop
		jal	wg_add
		li	a1,4
		move	a0,v0
ACELP_Code_A6072:	blez	s6,ACELP_Code_A6096
		lw	t9,1920(sp)
		jal	wg_add
		li	a1,8
		move	a0,v0
		lw	t9,1920(sp)
ACELP_Code_A6096:	li	a1,6554
		sh	a0,0(t9)
		lw	a0,1644(sp)
		lw	t0,1648(sp)
		lw	t1,1652(sp)
		lw	t2,1656(sp)
		mpymvrr	acc1,sx0,sy0,shnone,x0,gpra0,y0,gpra1
		satacc	mac0, acc1, round_disable, sat_disable, satp32, 12
		satacc	mac0, acc1, round_disable, sat_enable, satp16, 3
		
		mpymvrr	acc0,sx0,sy0,shnone,x0,gprt0,y0,gpra1
		satacc	mac0, acc0, round_disable, sat_disable, satp32, 12
		satacc	mac0, acc0, round_disable, sat_enable, satp16, 3
		mvp	gprs7,udi_mac0acc1lo
		
		mpymvrr	acc1,sx0,sy0,shnone,x0,gprt1,y0,gpra1
		satacc	mac0, acc1, round_disable, sat_disable, satp32, 12
		satacc	mac0, acc1, round_disable, sat_enable, satp16, 3
		mvp	gprs2,udi_mac0acc0lo
		
		mpymvrr	acc0,sx0,sy0,shnone,x0,gprt2,y0,gpra1
		satacc	mac0, acc0, round_disable, sat_disable, satp32, 12
		satacc	mac0, acc0, round_disable, sat_enable, satp16, 3
		mvp	gprs8,udi_mac0acc1lo
		mvp	gprv0,udi_mac0acc0lo
		mvp	gprv0,udi_mac0acc0lo
		mvp	gprv0,udi_mac0acc0lo
		
		move	a0,v0
		li	a1,2
		jal	wg_shl
		move	s4,v0
		move	a1,v0
		jal	wg_add
		move	a0,s4
		move	a0,v0
		jal	wg_add
		li	a1,3
		lw	a0,1656(sp)
		jal	wg_sub
		move	a1,v0
		move	a0,s4
		li	a1,1
		jal	wg_shl
		move	s1,v0
		move	a0,v0
		jal	wg_add
		move	a1,s1
		move	a0,s2
		li	a1,3
		jal	wg_shl
		move	s6,v0
		move	a1,v0
		jal	wg_add
		move	a0,s7
		move	a0,s8
		li	a1,6
		jal	wg_shl
		move	s0,v0
		move	a1,v0
		jal	wg_add
		move	a0,s0
		move	a0,s6
		li	a1,9
		jal	wg_shl
		move	s5,v0
		move	a1,v0
		jal	wg_add
		move	a0,s5
		lw	s4,1716(sp)
		sll	s3,v0,0x10
		beqz	s4,ACELP_Code_A6416
		sra	s3,s3,0x10
		lw	s2,1584(sp)
		lw	ra,1912(sp)
		sll	t0,s2,0x1
		addu	s0,t0,ra
		move	s1,ra
		lw	s8,1588(sp)
ACELP_Code_A6360:	lh	a0,0(s1)		
		mpymvrr	acc0,sx0,sy0,shnone,x0,gpra0,y0,gprs8
		satacc	mac0, acc0, round_disable, sat_disable, satp32, 12
		satacc	mac0, acc0, round_disable, sat_enable, satp16, 3
		mvp	gprv0,udi_mode
		mvp	gprv0,udi_mode
		mvp	gprv0,udi_mode
		mvp	gpra1,udi_mac0acc0lo
		addiu	s1,s1,2		
		jal	wg_add
		lh	a0,0(s0)
		addiu	s2,s2,1
		slti	t1,s2,40
		sh	v0,0(s0)
		bnez	t1,ACELP_Code_A6360
		addiu	s0,s0,2
		

ACELP_Code_A6416:	move	v0,s3
		lw	ra,1892(sp)
		lw	s8,1888(sp)
		lw	s7,1884(sp)
		lw	s6,1880(sp)
		lw	s5,1876(sp)
		lw	s4,1872(sp)
		lw	s3,1868(sp)
		lw	s2,1864(sp)
		lw	s1,1860(sp)
		lw	s0,1856(sp)
		jr	ra
		addiu	sp,sp,1896
ACELP_Code_A6468:	jal	norm_l
		move	a0,s2
		sra	s5,v0,0x1
		sll	s4,s5,0x10
		lw	s0,1900(sp)
		sra	s3,s4,0x10
		move	s2,zero
		addiu	s1,sp,16
ACELP_Code_A6500:	lh	a0,0(s0)
		jal	wg_shl
		move	a1,s3
		addiu	t8,s2,1
		sll	s7,t8,0x10
		sra	s2,s7,0x10
		slti	s6,s2,40
		sh	v0,1312(s1)
		addiu	s0,s0,2
		bnez	s6,ACELP_Code_A6500
		addiu	s1,s1,2
		b	ACELP_Code_A312
		addiu	s0,sp,96
ACELP_Code_A6552:	lw	a0,1656(sp)
		slti	v0,a0,40
		beqz	v0,ACELP_Code_A6012
		move	s2,a0
		lw	s1,1916(sp)
		sll	t8,a0,0x1
		addu	s0,t8,s1
		lw	s1,1900(sp)
ACELP_Code_A6584:	addiu	s7,s2,1
		lh	a0,0(s0)
		lh	a1,0(s1)
		sll	s8,s7,0x10
		jal	wg_sub
		sra	s2,s8,0x10
		slti	a0,s2,40
		sh	v0,0(s0)
		addiu	s1,s1,2
		bnez	a0,ACELP_Code_A6584
		addiu	s0,s0,2
		b	ACELP_Code_A6012
		nop
ACELP_Code_A6636:	lw	a0,1652(sp)
		slti	t1,a0,40
		beqz	t1,ACELP_Code_A5932
		move	s2,a0
		lw	t2,1916(sp)
		sll	a1,a0,0x1
		lw	s1,1900(sp)
		addu	s0,a1,t2
ACELP_Code_A6668:	lh	a0,0(s0)
		lh	a1,0(s1)
		jal	wg_sub
		addiu	s1,s1,2
		addiu	v1,s2,1
		sll	s2,v1,0x10
		sra	s2,s2,0x10
		slti	a3,s2,40
		sh	v0,0(s0)
		bnez	a3,ACELP_Code_A6668
		addiu	s0,s0,2
		b	ACELP_Code_A5932
		nop
ACELP_Code_A6720:	lw	a0,1648(sp)
		slti	v0,a0,40
		beqz	v0,ACELP_Code_A5852
		move	s2,a0
		lw	s1,1916(sp)
		sll	t6,a0,0x1
		addu	s0,t6,s1
		lw	s1,1900(sp)
ACELP_Code_A6752:	lh	a0,0(s0)
		lh	a1,0(s1)
		jal	wg_sub
		addiu	s1,s1,2
		addiu	t8,s2,1
		sll	a0,t8,0x10
		sra	s2,a0,0x10
		slti	t7,s2,40
		sh	v0,0(s0)
		bnez	t7,ACELP_Code_A6752
		addiu	s0,s0,2
		b	ACELP_Code_A5852
		nop
ACELP_Code_A6804:	lw	a0,1644(sp)
		slti	ra,a0,40
		beqz	ra,ACELP_Code_A5772
		move	s2,a0
		lw	t0,1916(sp)
		sll	t1,a0,0x1
		lw	s0,1900(sp)
		addu	s1,t1,t0
ACELP_Code_A6836:	lh	a0,0(s0)
		jal	negate
		addiu	s0,s0,2
		addiu	a1,s2,1
		sll	s2,a1,0x10
		sra	s2,s2,0x10
		slti	t2,s2,40
		sh	v0,0(s1)
		bnez	t2,ACELP_Code_A6836
		addiu	s1,s1,2
		b	ACELP_Code_A5772
		nop
END( ACELP_Code_A )



