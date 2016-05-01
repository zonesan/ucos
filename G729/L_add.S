/*
***************************************************************************
*                  Copyright (c) 2006 vBridge Microsystem, Inc.  
*                       Unpublished & Not for Publication
*                               All Rights Reserved                   
*   
* File        : L_add.S                     
*                                                                         
* Description:    
*             
* Date        : Jul 14, 2006                                                           
***************************************************************************
*/

#include <mips/asm.h>
#include <mips/udi.h>
#include <mips/regdef.h>



      .set noat
      .set noreorder
LEAF( L_add )
		xor	v0,a0,a1
		addu	v1,a0,a1
		bltz	v0,L_add24
		xor	a1,v1,a0
		bltzl	a1,L_add32
		lui	v1,0x8000
L_add24:	jr	ra
		move	v0,v1
L_add32:	bgezl	a0,L_add48
		addiu	v1,v1,-1
L_add48:	li	a1,1
		sw	a1,Overflow
		jr	ra
		move	v0,v1
		
END( L_add )
