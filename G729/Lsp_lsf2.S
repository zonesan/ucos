/*
***************************************************************************
*                  Copyright (c) 2006 vBridge Microsystem, Inc.  
*                       Unpublished & Not for Publication
*                               All Rights Reserved                   
*   
* File        	: Lsp_lsf2.S                     
*                                                                         
* Description	: Transformation lsp to lsf, The transformation from lsp[i] to       
*             	lsf[i] and lsf[i] to lsp[i] are approximated by a 
*		look-up table and interpolation. 
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
void Lsp_lsf2(
  Word16 lsp[],    // (i) Q15 : lsp[m] (range: -1<=val<1)   
  Word16 lsf[],    // (o) Q13 : lsf[m] (range: 0.0<=val<PI) 
  Word16 m         // (i)     : LPC order                   
)
{
  Word16 i, ind;
  Word16 offset;   // in Q15 
  Word16 freq;     // normalized frequency in Q16 
  Word32 L_tmp;

  ind = 63;           // begin at end of table2 -1 

  for(i= m-(Word16)1; i >= 0; i--)
  {
    
    while( sub(table2[ind], lsp[i]) < 0 )
    {
      ind = sub(ind,1);
      if ( ind <= 0 )
        break;
    }

    offset = sub(lsp[i], table2[ind]);

    L_tmp  = L_mult( slope_acos[ind], offset );  
    freq = add(shl(ind, 9), extract_l(L_shr(L_tmp, 12)));
    lsf[i] = mult(freq, 25736); 

  }
  return;
}

*------------------------------------------------------------------------*/
LEAF( Lsp_lsf2 )
                addiu   sp,sp,-56
                addiu   a2,a2,-1
                sw      s3,28(sp)
                sll     s3,a2,0x10
                sra     s3,s3,0x10
                sw      s5,36(sp)
                sw      ra,48(sp)
                sw      s7,44(sp)
                sw      s6,40(sp)
                sw      s4,32(sp)
                sw      s2,24(sp)
                sw      s1,20(sp)
                sw      s0,16(sp)
                li      s0,25736
                bltz    s3,Lsp_lsf2360
                li      s5,63
                sll     v0,s3,0x1
                addu    s1,v0,a0
                la      s6,table2
                la      s7,slope_acos
                addu    s2,v0,a1
                //b       Lsp_lsf2200
                li      s4,126

Lsp_lsf2200:    addu    a1,s4,s6
                lh      a0,0(a1)
                lh      a1,0(s1)
                bge     a0,a1,Lsp_lsf2961
                addu    t2,s4,s6
                subu    s5,s5,1
                //move    s5,v0
                bgtz    s5,Lsp_lsf2200
                sll     s4,s5,0x1
                //sll     s4,s5,0x1
                addu    t2,s4,s6
Lsp_lsf2961:             lh      a1,0(t2)
                lh      a0,0(s1)
                subu    v0,a0,a1
                addiu   s1,s1,-2
                addu    t1,s4,s7
                lh      a0,0(t1)
                mpymvrr	acc0,sx0,sy0,shsat,x0,gpra0,y0,gprv0
                //satacc	mac0, acc0, round_disable, sat_disable, satp32, 12
                //satacc	mac0, acc0, round_disable, sat_disable, satp32, -16
                //satacc	mac0, acc0, round_disable, sat_disable, satp32, 16
                mvp	gprv0,udi_mac0acc0lo
                mvp	gprv0,udi_mac0acc0lo
                mvp	gprv0,udi_mac0acc0lo
                mvp	gprv0,udi_mac0acc0lo
                sra     a0,v0,0xc
                sll     t0,a0,0x10
                sra     a1,t0,0x10                
                jal     wg_add
                sll     a0,s5,9
                mpymvrr	acc0,sx0,sy0,shnone,x0,gprv0,y0,gprs0
		satacc	mac0, acc0, round_disable, sat_disable, satp32, 12
		satacc	mac0, acc0, round_disable, sat_enable, satp16, 3

                mvp	gprv0,udi_mac0acc0lo
                mvp	gprv0,udi_mac0acc0lo
                mvp	gprv0,udi_mac0acc0lo
                mvp	gprv0,udi_mac0acc0lo
                addiu   t0,s3,-1
                sll     a3,t0,0x10
                sra     s3,a3,0x10
                sh      v0,0(s2)
                bgez    s3,Lsp_lsf2200
                addiu   s2,s2,-2
Lsp_lsf2360:    lw      ra,48(sp)
                lw      s7,44(sp)
                lw      s6,40(sp)
                lw      s5,36(sp)
                lw      s4,32(sp)
                lw      s3,28(sp)
                lw      s2,24(sp)
                lw      s1,20(sp)
                lw      s0,16(sp)
                jr      ra
                addiu   sp,sp,56
END( Lsp_lsf2 )

