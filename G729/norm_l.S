/*
***************************************************************************
*                  Copyright (c) 2006 vBridge Microsystem, Inc.  
*                       Unpublished & Not for Publication
*                               All Rights Reserved                   
*   
* File        : norm_l.S                     
*                                                                         
* Description:    
*             
* Date        : May 17, 2006                                                           
***************************************************************************
*/

#include <mips/asm.h>
#include <mips/udi.h>
#include <mips/regdef.h>

#include "mipsudi_inst.h"


      .set noat
      .set noreorder

LEAF( norm_l )
		bgtz	a0,1f
		clz	v0,a0
		bltz	a0,1f
		clo	v0,a0
		addi	v0,v0,1
1:		addi	v0,v0,-1
		jr	ra
		nop
END( norm_l )

