/*
   ITU-T G.729A Speech Coder    ANSI-C Source Code
   Version 1.1    Last modified: September 1996

   Copyright (c) 1996,
   AT&T, France Telecom, NTT, Universite de Sherbrooke, Lucent Technologies
   All rights reserved.
*/

#include "typedef.h"
#include "basic_op.h"
#include "oper_32b.h"

/*___________________________________________________________________________
 |                                                                           |
 |  This file contains operations in double precision.                       |
 |  These operations are not standard double precision operations.           |
 |  They are used where single precision is not enough but the full 32 bits  |
 |  precision is not necessary. For example, the function Div_32() has a     |
 |  24 bits precision which is enough for our purposes.                      |
 |                                                                           |
 |  The double precision numbers use a special representation:               |
 |                                                                           |
 |     L_32 = hi<<16 + lo<<1                                                 |
 |                                                                           |
 |  L_32 is a 32 bit integer.                                                |
 |  hi and lo are 16 bit signed integers.                                    |
 |  As the low part also contains the sign, this allows fast multiplication. |
 |                                                                           |
 |      0x8000 0000 <= L_32 <= 0x7fff fffe.                                  |
 |                                                                           |
 |  We will use DPF (Double Precision Format )in this file to specify        |
 |  this special format.                                                     |
 |___________________________________________________________________________|
*/

/*___________________________________________________________________________
 |                                                                           |
 |  Function L_Extract()                                                     |
 |                                                                           |
 |  Extract from a 32 bit integer two 16 bit DPF.                            |
 |                                                                           |
 |  Arguments:                                                               |
 |                                                                           |
 |   L_32      : 32 bit integer.                                             |
 |               0x8000 0000 <= L_32 <= 0x7fff ffff.                         |
 |   hi        : b16 to b31 of L_32                                          |
 |   lo        : (L_32 - hi<<16)>>1                                          |
 |___________________________________________________________________________|
*/

void L_Extract(Word32 L_32, Word16 *hi, Word16 *lo)
{
  Word32 temp,temp1,temp2;
  Word32 L_var_out;
  Word32 L_produit; 
  
  *hi  = (Word16)(L_32 >> 16);
  temp = L_32 >> 1;
  L_produit = (Word32)(*hi);	   
  L_produit = L_produit<<15;
  temp1 = temp - L_produit;
  *lo  = (Word16)(temp1 );
  
  return;
}
		/*
		sra	v1,a0,0x10
		sra	a3,a0,0x1
		sll	v0,v1,0xf
		subu	a0,a3,v0
		sh	v1,0(a1)
		sh	a0,0(a2)		
		*/
/*___________________________________________________________________________
 |                                                                           |
 |  Function L_Comp()                                                        |
 |                                                                           |
 |  Compose from two 16 bit DPF a 32 bit integer.                            |
 |                                                                           |
 |     L_32 = hi<<16 + lo<<1                                                 |
 |                                                                           |
 |  Arguments:                                                               |
 |                                                                           |
 |   hi        msb                                                           |
 |   lo        lsf (with sign)                                               |
 |                                                                           |
 |   Return Value :                                                          |
 |                                                                           |
 |             32 bit long signed integer (Word32) whose value falls in the  |
 |             range : 0x8000 0000 <= L_32 <= 0x7fff fff0.                   |
 |                                                                           |
 |___________________________________________________________________________|
*/

Word32 L_Comp(Word16 hi, Word16 lo)
{
  Word32 L_32;
  L_32 = (Word32) hi << 16; 
	//L_32 = L_deposit_h(hi);
  return( L_mac(L_32, lo, 1));          /* = hi<<16 + lo<<1 */
}
/*
		lh	a1,40(a2)
		lh	a0,16(a2)		
		sll	a0,a0,0x10
		srl	v0,a0,31
		move	t5,zero
		li	t2,0xff
		movn	t5,t2,v0
		mvp	udi_mac1acc1hi,gprt5		
		sll	a1,a1,0x10
		mvp		udi_mac1acc1lo,gpra0
		macmvrr		acc1,sx0,sy0,shsat,x0,gpra1,y0,gprt1
		mvp		gprt0,udi_mode
		mvp		gprt0,udi_mode
		mvp		gprt0,udi_mode
*/
/*___________________________________________________________________________
 | Function Mpy_32()                                                         |
 |                                                                           |
 |   Multiply two 32 bit integers (DPF). The result is divided by 2**31      |
 |                                                                           |
 |   L_32 = (hi1*hi2)<<1 + ( (hi1*lo2)>>15 + (lo1*hi2)>>15 )<<1              |
 |                                                                           |
 |   This operation can also be viewed as the multiplication of two Q31      |
 |   number and the result is also in Q31.                                   |
 |                                                                           |
 | Arguments:                                                                |
 |                                                                           |
 |  hi1         hi part of first number                                      |
 |  lo1         lo part of first number                                      |
 |  hi2         hi part of second number                                     |
 |  lo2         lo part of second number                                     |
 |                                                                           |
 |___________________________________________________________________________|
*/

Word32 Mpy_32(Word16 hi1, Word16 lo1, Word16 hi2, Word16 lo2)
{
  Word32 L_32;

  L_32 = L_mult(hi1, hi2);
  L_32 = L_mac(L_32, mult(hi1, lo2) , 1);
  L_32 = L_mac(L_32, mult(lo1, hi2) , 1);

  return( L_32 );
}
/*
li			t1,1
mpymvrr		acc0,sx0,sy0,shnone,x0,hi1,y0,lo2
mpymvrr		acc1,sx0,sy0,shnone,x0,lo1,y0,hi2
mvp			gprt0,udi_mode
mvp			gprt0,udi_mode
satacc		mac0,acc0,round_disable, sat_disable, satp32, 12
satacc		mac0,acc1,round_disable, sat_disable, satp32, 12
mvp			gprt0,udi_mode
mvp			gprt0,udi_mode
//mpymvrr		acc0,sx0,sy0,shsat,x0,hi1,y0,hi2
satacc		mac0,acc0,round_disable, sat_enable, satp16, 3
satacc		mac0,acc1,round_disable, sat_enable, satp16, 3
mvp			gprt0,udi_mode
mvp			gprt0,udi_mode
mvp			gprt2,udi_mac0acc0lo
mvp			gprt3,udi_mac0acc1lo
mpymvrr		acc0,sx0,sy0,shsat,x0,hi1,y0,hi2
macmvrr		acc0,sx0,sy0,shsat,x0,t2,y0,t1
macmvrr		acc0,sx0,sy0,shsat,x0,t3,y0,t1
mvp			gprt0,udi_mode
mvp			gprt0,udi_mode
mvp			gprt0,udi_mode

*/
/*___________________________________________________________________________
 | Function Mpy_32_16()                                                      |
 |                                                                           |
 |   Multiply a 16 bit integer by a 32 bit (DPF). The result is divided      |
 |   by 2**15                                                                |
 |                                                                           |
 |   This operation can also be viewed as the multiplication of a Q31        |
 |   number by a Q15 number, the result is in Q31.                           |
 |                                                                           |
 |   L_32 = (hi1*lo2)<<1 + ((lo1*lo2)>>15)<<1                                |
 |                                                                           |
 | Arguments:                                                                |
 |                                                                           |
 |  hi          hi part of 32 bit number.                                    |
 |  lo          lo part of 32 bit number.                                    |
 |  n           16 bit number.                                               |
 |                                                                           |
 |___________________________________________________________________________|
*/

Word32 Mpy_32_16(Word16 hi, Word16 lo, Word16 n)
{
  Word32 L_32;

  L_32 = L_mult(hi, n);
  L_32 = L_mac(L_32, mult(lo, n) , 1);

  return( L_32 );
}
/*
li			t1,1
mpymvrr		acc0,sx0,sy0,shsat,x0,hi,y0,n
mpymvrr		acc1,sx0,sy0,shnone,x0,lo,y0,n
mvp			gprt0,udi_mode
mvp			gprt0,udi_mode
satacc		mac0,acc1,round_disable, sat_disable, satp32, 12
mvp			gprt0,udi_mode
mvp			gprt0,udi_mode
satacc		mac0,acc1,round_disable, sat_enable, satp16, 3
mvp			gprt0,udi_mode
mvp			gprt0,udi_mode
mvp			gprt2,udi_mac0acc0lo
mvp			gprt3,udi_mac0acc1lo
macmvrr		acc0,sx0,sy0,shsat,x0,t3,y0,t1
mvp			gprt0,udi_mode
mvp			gprt0,udi_mode
mvp			gprt0,udi_mode

*/
/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : Div_32                                                  |
 |                                                                           |
 |   Purpose :                                                               |
 |             Fractional integer division of two 32 bit numbers.            |
 |             L_num / L_denom.                                              |
 |             L_num and L_denom must be positive and L_num < L_denom.       |
 |             L_denom = denom_hi<<16 + denom_lo<<1                          |
 |             denom_hi is a normalize number.                               |
 |             The result is in Q30.                                         |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |    L_num                                                                  |
 |             32 bit long signed integer (Word32) whose value falls in the  |
 |             range : 0x0000 0000 < L_num < L_denom                         |
 |                                                                           |
 |    L_denom = denom_hi<<16 + denom_lo<<1      (DPF)                        |
 |                                                                           |
 |       denom_hi                                                            |
 |             16 bit positive normalized integer whose value falls in the   |
 |             range : 0x4000 < hi < 0x7fff                                  |
 |       denom_lo                                                            |
 |             16 bit positive integer whose value falls in the              |
 |             range : 0 < lo < 0x7fff                                       |
 |                                                                           |
 |   Return Value :                                                          |
 |                                                                           |
 |    L_div                                                                  |
 |             32 bit long signed integer (Word32) whose value falls in the  |
 |             range : 0x0000 0000 <= L_div <= 0x7fff ffff.                  |
 |             It's a Q31 value                                              |
 |                                                                           |
 |  Algorithm:                                                               |
 |                                                                           |
 |  - find = 1/L_denom.                                                      |
 |      First approximation: approx = 1 / denom_hi                           |
 |      1/L_denom = approx * (2.0 - L_denom * approx )                       |
 |                                                                           |
 |  -  result = L_num * (1/L_denom)                                          |
 |___________________________________________________________________________|
*/
/*
Word32 Div_32(Word32 L_num, Word16 denom_hi, Word16 denom_lo)
{
  Word16 approx, hi, lo, n_hi, n_lo, i;
  Word32 L_32;

  approx = div_s( (Word16)0x3fff, denom_hi);                             

  L_32 = Mpy_32_16(denom_hi, denom_lo, approx); 
  L_32 = L_sub( (Word32)0x7fffffffL, L_32);     
  L_Extract(L_32, &hi, &lo);

  L_32 = Mpy_32_16(hi, lo, approx);            
  L_Extract(L_32, &hi, &lo);
  L_Extract(L_num, &n_hi, &n_lo);
  L_32 = Mpy_32(n_hi, n_lo, hi, lo);             
   
  for(i=2;i>0;i--)
        {
         if (L_32 > (Word32) 0X3fffffffL)
           {
            Overflow = 1;
            L_32 = MAX_32;
            break;
           }
         else
           {
            if (L_32 < (Word32) 0xc0000000L)
              {
               Overflow = 1;
               L_32 = MIN_32;
               break;
              }
           }
         L_32 *= 2;
        }

  return( L_32 );
}
*/

