#include <mips/asm.h>
#include <mips/udi.h>
#include <mips/regdef.h>

#include "mipsudi_inst.h"

      .set noat
      .set noreorder

LEAF( Get_wegt )
							  addiu	sp,sp,-72
								sw	s4,56(sp)
								sw	s3,52(sp)
								sw	s2,48(sp)
								sw	s1,44(sp)
								sw	s0,40(sp)
								sw	ra,64(sp)
								sw	s5,60(sp)
								move	s3,a0
								lh	a0,2(a0)
								move	s4,a1
								subu    v0,a0,9221
								sh	v0,16(sp)
								li	s2,1
								addiu	s1,sp,18
								addiu	s0,s3,2
Get_wegt68:	    lh	a0,2(s0)
								lh	a1,-2(s0)
								subu    a0,a0,a1
								addiu	s0,s0,2
								subu    v0,a0,8192
								addiu	v1,s2,1
								sll	s2,v1,0x10
								sra	s2,s2,0x10
								slti	a0,s2,9
								sh	v0,0(s1)
								bnez	a0,Get_wegt68
								addiu	s1,s1,2
								lh	a1,16(s3)
								li	a0,15485
								subu    v0,a0,a1
								lui	a1,0x3fff
								ori	s3,a1,0xffff
								addiu	s1,sp,16
								move	s2,zero
								move	s0,s4
								move	s5,s3
								b	Get_wegt200
								sh	v0,34(sp)
Get_wegt168:	  addiu	t1,s2,1
								sll	t0,t1,0x10
								sra	s2,t0,0x10
								slti	a3,s2,10
								sh	v0,0(s0)
								addiu	s1,s1,2
								beqz	a3,Get_wegt428
								addiu	s0,s0,2
Get_wegt200:	  lh	a0,0(s1)
								bgtz	a0,Get_wegt168
								li	v0,2048
								move	v0,a0
								mpymvrr acc0,sx0,sy0,shsat,x0,gpra0,y0,gprv0
								udi_nop
								udi_nop
								udi_nop
								mvp     gprv0,udi_mac0acc0lo
								move	a1,v0
								li	a2,2
								move	t0,s5
								lui	t1,0x7fff
								lui	a3,0xc000
								sll	v0,a1,0x1
Get_wegt244:	  slt	t3,a1,a3
								slt	t2,t0,a1
								lui	a1,0x8000
								beqz	t2,Get_wegt268
								movz	a1,v0,t3
								ori	a1,t1,0xffff
Get_wegt268:	  addiu	t4,a2,-1
								sll	a2,t4,0x10
								sra	a2,a2,0x10
								bgtz	a2,Get_wegt244
								sll	v0,a1,0x1
								sra     t4,a1,0x10
								sll     a0,t4,0x10
								sra     a0,a0,0x10
								li	v0,20480
								mpymvrr acc0,sx0,sy0,shsat,x0,gpra0,y0,gprv0
								udi_nop
								udi_nop
								udi_nop
								mvp     gprv0,udi_mac0acc0lo
								move	a1,v0
								li	a2,2
								move	t0,s3
								lui	t1,0x7fff
								lui	a3,0xc000
								sll	t6,a1,0x1
Get_wegt332:	  slt	t7,a1,a3
								slt	t5,t0,a1
								lui	a1,0x8000
								beqz	t5,Get_wegt356
								movz	a1,t6,t7
								ori	a1,t1,0xffff
Get_wegt356:	  addiu	t9,a2,-1
								sll	t8,t9,0x10
								sra	a2,t8,0x10
								bgtz	a2,Get_wegt332
								sll	t6,a1,0x1
								sra     t4,a1,0x10
								sll     a0,t4,0x10
								sra     a0,a0,0x10
								addu  v0,a0,2048
								addiu	t1,s2,1
								sll	t0,t1,0x10
								sra	s2,t0,0x10
								slti	a3,s2,10
								sh	v0,0(s0)
								addiu	s1,s1,2
								bnez	a3,Get_wegt200
								addiu	s0,s0,2
Get_wegt428:	  lh	a0,8(s4)
								li	v0,19661
								mpymvrr acc0,sx0,sy0,shsat,x0,gpra0,y0,gprv0
								udi_nop
								udi_nop
								udi_nop
								mvp     gprv0,udi_mac0acc0lo
								lui	s5,0x3fff
								ori	s3,s5,0xffff
								lui	s1,0x7fff
								slt	s0,s3,v0
								bnez	s0,Get_wegt484
								ori	a0,s1,0xffff
								lui	a0,0xc000
								slt	s2,v0,a0
								sll	ra,v0,0x1
								lui	a0,0x8000
								movz	a0,ra,s2
Get_wegt484:	  sra     t4,a0,0x10
								sll     a0,t4,0x10
								sra     v0,a0,0x10
								sh	v0,8(s4)
								lh	a0,10(s4)
								li	v0,19661
								mpymvrr acc0,sx0,sy0,shsat,x0,gpra0,y0,gprv0
								udi_nop
								udi_nop
								udi_nop
								mvp     gprv0,udi_mac0acc0lo
								lui	t3,0x3fff
								move	a1,v0
								ori	v0,t3,0xffff
								lui	v1,0x7fff
								slt	t2,v0,a1
								bnez	t2,Get_wegt556
								ori	a0,v1,0xffff
								lui	t5,0xc000
								slt	t4,a1,t5
								sll	a2,a1,0x1
								lui	a0,0x8000
								movz	a0,a2,t4
Get_wegt556:	  sra     t4,a0,0x10
								sll     a0,t4,0x10
								sra     v0,a0,0x10
								sh	v0,10(s4)
								move	a3,zero
								move	a2,zero
								move	a1,s4
Get_wegt580:	  addiu	t0,a2,1
								lh	t7,0(a1)
								sll	t9,t0,0x10
								sra	a2,t9,0x10
								slt	t8,a3,t7
								slti	t6,a2,10
								movn	a3,t7,t8
								bnez	t6,Get_wegt580
								addiu	a1,a1,2
								jal	norm_s
								move	a0,a3
								move	s2,v0
								move	s0,s4
								move	s1,zero
Get_wegt636:	  lh	a0,0(s0)
								jal	wg_shl
								move	a1,s2
								addiu	a3,s1,1
								sll	a1,a3,0x10
								sra	s1,a1,0x10
								slti	s4,s1,10
								sh	v0,0(s0)
								bnez	s4,Get_wegt636
								addiu	s0,s0,2
								lw	ra,64(sp)
								lw	s5,60(sp)
								lw	s4,56(sp)
								lw	s3,52(sp)
								lw	s2,48(sp)
								lw	s1,44(sp)
								lw	s0,40(sp)
								jr	ra
								addiu	sp,sp,72
END( Get_wegt )
