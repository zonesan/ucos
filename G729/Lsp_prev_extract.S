/*
***************************************************************************
*                  Copyright (c) 2006 vBridge Microsystem, Inc.  
*                       Unpublished & Not for Publication
*                               All Rights Reserved                   
*   
* File        	: Lsp_prev_extract.S                     
*                                                                         
* Description	: extract elementary LSP from composed LSP with previous LSP
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
void Lsp_prev_extract(
  Word16 lsp[M],                // (i) Q13 : unquantized LSP parameters  
  Word16 lsp_ele[M],            // (o) Q13 : target vector               
  Word16 fg[MA_NP][M],          // (i) Q15 : MA prediction coef.         
  Word16 freq_prev[MA_NP][M],   // (i) Q13 : previous LSP vector         
  Word16 fg_sum_inv[M]          // (i) Q12 : inverse previous LSP vector 
)
{
  Word16 j, k;
  Word32 L_temp;                // Q19 
  Word16 temp;                  // Q13 


  for ( j = 0 ; j < M ; j++ ) {
    L_temp = L_deposit_h(lsp[j]);
    for ( k = 0 ; k < MA_NP ; k++ )
      L_temp = L_msu( L_temp, freq_prev[k][j], fg[k][j] );

    temp = extract_h(L_temp);
    L_temp = L_mult( temp, fg_sum_inv[j] );
    lsp_ele[j] = extract_h( L_shl( L_temp, 3 ) );

  }
  return;
}

*------------------------------------------------------------------------*/

LEAF( Lsp_prev_extract )
		addiu	sp,sp,-56
		sw	s5,36(sp)
		lui	v0,0x3fff
		lw	s5,72(sp)
		sw	s8,48(sp)
		sw	s7,44(sp)
		sw	s6,40(sp)
		sw	s4,32(sp)
		sw	s3,28(sp)
		sw	ra,52(sp)
		sw	s2,24(sp)
		sw	s1,20(sp)
		sw	s0,16(sp)
		sw	a2,64(sp)
		sw	a3,68(sp)
		move	s4,a1
		move	s3,a0
		ori	s8,v0,0xffff
		move	s7,zero
		move	s6,zero
Lsp_prev_extract80:	lh	a2,0(s3)
		lw	a1,64(sp)
		lw	v1,68(sp)
		move	s2,zero
		sll	a0,a2,0x10
		
		
		mvp	udi_mac0acc0lo,gpra0
		srl	v0,a0,31		
		move	t5,zero
		li	t2,0xff
		movn	t5,t2,v0		
		mvp	udi_mac0acc0hi,gprt5
		
		addu	s1,s6,a1
		addu	s0,s6,v1
		/*
Lsp_prev_extract108:	lh	t1,0(s0)
		lh	t2,0(s1)
		msumvrr	acc0,sx0,sy0,shsat,x0,gprt1,y0,gprt2
		addiu	s0,s0,20
		addiu	s2,s2,1
		slti	a3,s2,4
		bnez	a3,Lsp_prev_extract108
		addiu	s1,s1,20
		*/
		lh	t0,0(s0)
		lh	t1,0(s1)
		lh	t2,20(s0)
		lh	t3,20(s1)
		lh	t4,40(s0)
		lh	t5,40(s1)
		lh	t6,60(s0)
		lh	t7,60(s1)
		msumvrr	acc0,sx0,sy0,shsat,x0,gprt0,y0,gprt1
		msumvrr	acc0,sx0,sy0,shsat,x0,gprt3,y0,gprt2
		msumvrr	acc0,sx0,sy0,shsat,x0,gprt4,y0,gprt5
		msumvrr	acc0,sx0,sy0,shsat,x0,gprt6,y0,gprt7
		
		satacc	mac0, acc0, round_disable, sat_disable, satp32,16
		mvp	gpra0,udi_mac0acc0lo
		mvp	gpra0,udi_mac0acc0lo
		mvp	gpra0,udi_mac0acc0lo
		mvp	gpra0,udi_mac0acc0lo
		//sra	a0,v0,0x10
		lh	a1,0(s5)
		mpymvrr	acc0,sx0,sy0,shsat,x0,gpra0,y0,gpra1
		mvp	gpra0,udi_mac0acc0lo
		mvp	gpra0,udi_mac0acc0lo
		mvp	gpra0,udi_mac0acc0lo
		mvp	gpra0,udi_mac0acc0lo
		//lw	a2,-32560(gp)
		//move	a0,v0
		li	a1,3
		move	t0,s8
		lui	t1,0x7fff
		lui	a3,0xc000
		slt	t2,t0,a0
Lsp_prev_extract200:	beqz	t2,Lsp_prev_extract332
		slt	v1,a0,a3
		li	a2,1
		ori	a0,t1,0xffff
Lsp_prev_extract216:	addiu	t4,a1,-1
		sll	t3,t4,0x10
		sra	a1,t3,0x10
		bgtz	a1,Lsp_prev_extract200
		slt	t2,t0,a0
		sra	v0,a0,0x10
		//sw	a2,-32560(gp)
		addiu	s7,s7,1
		slti	a0,s7,10
		sh	v0,0(s4)
		addiu	s6,s6,2
		addiu	s3,s3,2
		addiu	s5,s5,2
		bnez	a0,Lsp_prev_extract80
		addiu	s4,s4,2
		lw	ra,52(sp)
		lw	s8,48(sp)
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
Lsp_prev_extract332:	beqz	v1,Lsp_prev_extract216
		sll	a0,a0,0x1
		li	a2,1
		b	Lsp_prev_extract216
		lui	a0,0x8000
END( Lsp_prev_extract )

