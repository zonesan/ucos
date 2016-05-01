#include <mips/asm.h>
#include <mips/udi.h>
#include <mips/regdef.h>

#include "mipsudi_inst.h"


      .set noat
      .set noreorder

LEAF( Copy )
		sll	a2,a2,0x10
		sra	a2,a2,0x10
		blez	a2,Copy52
		move	a3,zero
Copy16:		addiu	a3,a3,2
		//lhu	v1,0(a0)
		lw	v1,0(a0)
		slt	v0,a3,a2
		sw	v1,0(a1)
		addiu	a0,a0,4
		bnez	v0,Copy16
		addiu	a1,a1,4
Copy52:		jr	ra
		nop
END( Copy )
