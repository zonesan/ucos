/*
***************************************************************************
*                  Copyright (c) 2006 vBridge Microsystem, Inc.  
*                       Unpublished & Not for Publication
*                               All Rights Reserved                   
*   
* File        : L_shl_n.S                     
*                                                                         
* Description:    
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
LEAF( L_shl_n )
		sll	a1,a1,0x10
		sra	a1,a1,0x10
		blez	a1,1f
		addiu	t1,a1,-1
		lui	t0,0x4000
		lui	a3,0xc000
L_shl_n28:	
		bge	a0,t0,2f
		slt	a2,a0,a3
		sll	a0,a0,0x1		
		bnez	a2,3f
		move	v0,a0
		bgtz	t1,L_shl_n28
		addiu	t1,t1,-1		
1:		jr	ra
		nop
2:		lui	a0,0x7fff
		jr	ra
		ori	v0,a0,0xffff
3:		jr	ra
		lui	v0,0x8000
END( L_shl_n )

