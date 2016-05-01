/*
***************************************************************************
*                  Copyright (c) 2006 vBridge Microsystem, Inc.  
*                       Unpublished & Not for Publication
*                               All Rights Reserved                   
*   
* File        : norm_s.S                     
*                                                                         
* Description:    
*             
* Date        : Jul 7, 2006                                                           
***************************************************************************
*/

#include <mips/asm.h>
#include <mips/udi.h>
#include <mips/regdef.h>

#include "mipsudi_inst.h"


      .set noat
      .set noreorder

LEAF( norm_s )
		sll	a0,a0,16
		bgtz	a0,1f
		clz	v0,a0
		bltz	a0,1f
		clo	v0,a0
		addi	v0,v0,1
		
1:		
		jr	ra
		addi	v0,v0,-1
		
END( norm_s )

