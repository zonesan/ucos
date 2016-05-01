/*
   ITU-T G.729A Speech Coder    ANSI-C Source Code
   Version 1.1    Last modified: September 1996

   Copyright (c) 1996,
   AT&T, France Telecom, NTT, Universite de Sherbrooke, Lucent Technologies
   All rights reserved.
*/

/*------------------------------------------------------------------------*
 * Function Post_Process()                                                *
 *                                                                        *
 * Post-processing of output speech.                                      *
 *   - 2nd order high pass filter with cut off frequency at 100 Hz.       *
 *   - Multiplication by two of output speech with saturation.            *
 *-----------------------------------------------------------------------*/

#include "typedef.h"
#include "basic_op.h"
#include "oper_32b.h"

#include "ld8a.h"
#include "tab_ld8a.h"

/*------------------------------------------------------------------------*
 * 2nd order high pass filter with cut off frequency at 100 Hz.           *
 * Designed with SPPACK efi command -40 dB att, 0.25 ri.                  *
 *                                                                        *
 * Algorithm:                                                             *
 *                                                                        *
 *  y[i] = b[0]*x[i]   + b[1]*x[i-1]   + b[2]*x[i-2]                      *
 *                     + a[1]*y[i-1]   + a[2]*y[i-2];                     *
 *                                                                        *
 *     b[3] = {0.93980581E+00, -0.18795834E+01, 0.93980581E+00};          *
 *     a[3] = {0.10000000E+01, 0.19330735E+01, -0.93589199E+00};          *
 *-----------------------------------------------------------------------*/

/* Static values to be preserved between calls */
/* y[] values is keep in double precision      */

//static Word16 y2_hi, y2_lo, y1_hi, y1_lo, x0, x1;
//extern Word16 de_y2_hi, de_y2_lo, de_y1_hi, de_y1_lo, de_x0, de_x1;

/* Initialization of static values 

void Init_Post_Process(void)
{
  y2_hi = 0;
  y2_lo = 0;
  y1_hi = 0;
  y1_lo = 0;
  x0   = 0;
  x1   = 0;
}
*/
/*


void Post_Process(
  Word16 signal[],    
  Word16 lg)          
{
  Word16 i, x2, var2;
  Word32 L_tmp, L_tmp_test;

  for(i=0; i<lg; i++)
  {
     x2 = de_x1;
     de_x1 = de_x0;
     de_x0 = signal[i];

     
     

     L_tmp     = Mpy_32_16(de_y1_hi, de_y1_lo, a100[1]);
     L_tmp     = L_add(L_tmp, Mpy_32_16(de_y2_hi, de_y2_lo, a100[2]));
     L_tmp     = L_mac(L_tmp, de_x0, b100[0]);
     L_tmp     = L_mac(L_tmp, de_x1, b100[1]);
     L_tmp     = L_mac(L_tmp, x2, b100[2]);
     L_tmp     = L_shl(L_tmp, 2); 

	for(var2=2;var2>0;var2--)
   	{
         if (L_tmp > (Word32) 0X3fffffffL)
         {
            L_tmp = MAX_32;
           
         }
         else  if (L_tmp < (Word32) 0xc0000000L)
         {
               //Overflow = 1;
               L_tmp = MIN_32;
               //break;
          }
          else
          {
	  	L_tmp <<= 1;
         	//L_var_out = L_var1;
          }
   	}

         if (L_tmp > (Word32) 0X3fffffffL)
         {
            L_tmp_test = MAX_32;
           
         }
         else  if (L_tmp < (Word32) 0xc0000000L)
         {
               //Overflow = 1;
               L_tmp_test = MIN_32;
               //break;
          }
          else
          {
	  	L_tmp <<= 1;
         	L_tmp_test = L_tmp;
          }
		
     signal[i] = ROUND(L_tmp_test);

     de_y2_hi = de_y1_hi;
     de_y2_lo = de_y1_lo;
     L_Extract(L_tmp, &de_y1_hi, &de_y1_lo);
  }
  return;
}

*/

