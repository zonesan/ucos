/*
***************************************************************************
*                  Copyright (c) 2006 vBridge Microsystem, Inc.  
*                       Unpublished & Not for Publication
*                               All Rights Reserved                   
*   
* File        : abs_s.S                     
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
LEAF( abs_s )
		sll	a3,a0,0x10
		sra	a1,a3,0x10		
		bgez	a1,abs_s_1
		li	v1,32767		
		beq	a0,v1,abs_s_1
		not	a0,a1
		addi	a0,a0,1
abs_s_1:		
		jr	ra
		move	v0,a0
END( abs_s )

