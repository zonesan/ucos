/*
***************************************************************************
*                  Copyright (c) 2006 vBridge Microsystem, Inc.  
*                       Unpublished & Not for Publication
*                               All Rights Reserved                   
*   
* File        : Syn_filt.S                     
*                                                                         
* Description : Do the synthesis filtering 1/A(z).    
*             
* Date        : May 18, 2006                                                           
***************************************************************************
*/

#include <mips/asm.h>
#include <mips/udi.h>
#include <mips/regdef.h>

#include "mipsudi_inst.h"

      .set noat
      .set noreorder

/*------------------------------------------------------------------------*
void Syn_filt(
  Word16 a[],    
  Word16 x[],    
  Word16 y[],    
  Word16 lg,     
  Word16 mem[],   
  Word16 update   
)
{
  Word16 i, j;
  Word32 s;
  Word16 tmp[100];     
  Word16 *yy;

  yy = tmp;

  for(i=0; i<M; i++)
  {
    *yy++ = mem[i];
  }

  for (i = 0; i < lg; i++)
  {
    s = L_mult(x[i], a[0]);
    for (j = 1; j <= M; j++)
      s = L_msu(s, a[j], yy[-j]);

    s = L_shl(s, 3);
    *yy++ = ROUND(s);
  }

  for(i=0; i<lg; i++)
  {
    y[i] = tmp[i+M];
  }

  if(update != 0)
     for (i = 0; i < M; i++)
     {
       mem[i] = y[lg-M+i];
     }

 return;
}
*------------------------------------------------------------------------*/
LEAF( Syn_filt )
		addiu	sp,sp,-264
		lh	v0,284(sp)
		sw	s7,252(sp)
		sw	s6,248(sp)
		move	s7,a0
		sll	s6,a3,0x10
		lw	a0,280(sp)
		sw	a2,272(sp)
		sw	ra,260(sp)
		sw	s8,256(sp)
		sw	s5,244(sp)
		sw	s4,240(sp)
		sw	s3,236(sp)
		sw	s2,232(sp)
		sw	s1,228(sp)
		sw	s0,224(sp)
		sw	v0,216(sp)
		sra	s6,s6,0x10
		addiu	a3,sp,16
		move	a2,zero
Syn_filt80:	addiu	t1,a2,1
		sll	a2,t1,0x10
		lhu	v1,0(a0)
		sra	a2,a2,0x10
		slti	t0,a2,10
		sh	v1,0(a3)
		addiu	a0,a0,2
		bnez	t0,Syn_filt80
		addiu	a3,a3,2
		
		addiu	a0,sp,16
		slt	s8,zero,s6
		addiu	s3,a0,20
		beqz	s8,Syn_filt312
		move	s5,zero
		move	s4,a1
/*------------------------------------------------------------------------*

  		for (i = 0; i < lg; i++)
  		{
    			s = L_mult(x[i], a[0]);
    			for (j = 1; j <= M; j++)
      				s = L_msu(s, a[j], yy[-j]);

    			s = L_shl(s, 3);
    			*yy++ = ROUND(s);
  		}

*------------------------------------------------------------------------*/		
		mvp	gpra0,udi_mode
		li	a0,0x800000
		mvp 	udi_mode,gpra0
		
		mvp	gprt0,udi_mode
		mvp	gprt0,udi_mode
		
Syn_filt140:	lh	a0,0(s4)
		lh	a1,0(s7)
		mpymvrr	acc0,sx0,sy0,shsat,x0,gpra0,y0,gpra1
		li	s2,1
		addiu	s1,s3,-2
		addiu	s0,s7,2
Syn_filt168:	lh	a1,0(s0)
		lh	a2,0(s1)
		msumvrr	acc0,sx0,sy0,shsat,x0,gpra1,y0,gpra2
		addiu	s0,s0,2
		addiu	s2,s2,1
		slti	a1,s2,11
		bnez	a1,Syn_filt168
		addiu	s1,s1,-2
		mvp	gpra0,udi_mac0acc0lo
		mvp	gpra0,udi_mac0acc0lo
		mvp	gpra0,udi_mac0acc0lo
		mvp	gpra0,udi_mac0acc0lo		
		jal	L_shl_m
		li	a1,3
		jal	round16
		move	a0,v0		
		addiu	s5,s5,1
		slt	t2,s5,s6
		sh	v0,0(s3)
		addiu	s4,s4,2
		bnez	t2,Syn_filt140
		addiu	s3,s3,2
		
		//li	t1,1
		//move 	t6,zero
		lw	t6,Overflow
		
		mvp	gprt0,udi_mode
		sra	t0,t0,20
		and	t4,t0,1
		//movn	t6,t1,t4
		or	t6,t6,t4
		sw	t6,Overflow
		
		
		beqz	s8,Syn_filt312
		move	a2,zero
		lw	a0,272(sp)
		addiu	a1,sp,16
Syn_filt276:	addiu	t7,a2,1
		sll	t6,t7,0x10
		lhu	t5,20(a1)
		sra	a2,t6,0x10
		slt	t4,a2,s6
		sh	t5,0(a0)
		addiu	a1,a1,2
		bnez	t4,Syn_filt276
		addiu	a0,a0,2
		
Syn_filt312:	lw	s0,216(sp)
		beqz	s0,Syn_filt384
		lw	ra,260(sp)
		lw	s1,272(sp)
		sll	s3,s6,0x1
		lw	a0,280(sp)
		addu	a2,s3,s1
		move	a1,zero
Syn_filt344:	addiu	a1,a1,1
		lhu	s7,-20(a2)
		slti	s4,a1,10
		sh	s7,0(a0)
		addiu	a2,a2,2
		bnez	s4,Syn_filt344
		addiu	a0,a0,2
		
		lw	ra,260(sp)
Syn_filt384:	lw	s8,256(sp)
		lw	s7,252(sp)
		lw	s6,248(sp)
		lw	s5,244(sp)
		lw	s4,240(sp)
		lw	s3,236(sp)
		lw	s2,232(sp)
		lw	s1,228(sp)
		lw	s0,224(sp)
		jr	ra
		addiu	sp,sp,264
END( Syn_filt )

