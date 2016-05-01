/*
***************************************************************************
*                  Copyright (c) 2006 vBridge Microsystem, Inc.  
*                       Unpublished & Not for Publication
*                               All Rights Reserved                   
*   
* File        : Random.S                     
*                                                                         
* Description : Random generator    
*             
* Date        : Jul 14, 2006                                                           
***************************************************************************
*/

#include <mips/asm.h>
#include <mips/udi.h>
#include <mips/regdef.h>
#include "mipsudi_inst.h"


      .set noat
      .set noreorder
      
/*------------------------------------------------------------------------*
Word16 Random(Word16 *seed)
{

  *seed = extract_l(L_add(L_shr(L_mult(*seed, 31821), 1), 13849L));

  return(*seed);
}
*------------------------------------------------------------------------*/
LEAF( Random )

		li	t0,13849
		move	t1,zero
		li	a1,31821
		lh	t2,0(a0)
		mvp	udi_mac1acc0lo,gprt0
		mvp	udi_mac1acc0hi,gprt1
		mpymvrr	acc1,sx0,sy0,shsat,x0,gpra1,y0,gprt2
		satacc	mac0,acc1,round_disable,sat_disable,satp32,1
		addacc	acc1,acc0
		satacc	mac0,acc1,round_disable,sat_enable,satp32,0
		mvp	gprv0,udi_mac0acc1lo
		mvp	gprv0,udi_mac0acc1lo
		mvp	gprv0,udi_mac0acc1lo
		mvp	gprt0,udi_mac0acc1lo
		sll	v0,t0,0x10
		sra	v0,v0,0x10
		jr	ra
		sh	v0,0(a0)
END( Random )

