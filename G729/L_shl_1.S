/*
***************************************************************************
*                  Copyright (c) 2006 vBridge Microsystem, Inc.  
*                       Unpublished & Not for Publication
*                               All Rights Reserved                   
*   
* File        : L_shl_1.S                     
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
LEAF( L_shl_1 )		
		lui	a2,0x4000		
		//slt	v0,a2,a0	
		lui	t1,0x7fff
		//bnez	v0,1f
		bge	a0,a2,1f
		ori	t1,t1,0xffff
		lui	v1,0xc000
		slt	t3,a0,v1
		sll	t0,a0,0x1
		lui	t1,0x8000
		movz	t1,t0,t3
1:		jr	ra
		move	v0,t1
END( L_shl_1 )

