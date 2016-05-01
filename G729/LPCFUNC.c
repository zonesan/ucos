/*
   ITU-T G.729A Speech Coder    ANSI-C Source Code
   Version 1.1    Last modified: September 1996

   Copyright (c) 1996,
   AT&T, France Telecom, NTT, Universite de Sherbrooke, Lucent Technologies
   All rights reserved.
*/

/*-------------------------------------------------------------*
 *  Procedure Lsp_Az:                                          *
 *            ~~~~~~                                           *
 *   Compute the LPC coefficients from lsp (order=10)          *
 *-------------------------------------------------------------*/

#include "typedef.h"
#include "basic_op.h"
#include "oper_32b.h"

#include "ld8a.h"
#include "tab_ld8a.h"

/* local function */

//static void Get_lsp_pol(Word16 *lsp, Word32 *f);

void Lsp_Az(
  Word16 lsp[],    /* (i) Q15 : line spectral frequencies            */
  Word16 a[]       /* (o) Q12 : predictor coefficients (order = 10)  */
)
{
  Word16 i, j;
  Word32 f1[6], f2[6];
  Word32 t0;

  Get_lsp_pol(&lsp[0],f1);
  Get_lsp_pol(&lsp[1],f2);

  for (i = 5; i > 0; i--)
  {
    f1[i] = L_add(f1[i], f1[i-1]);        /* f1[i] += f1[i-1]; */
    f2[i] = L_sub(f2[i], f2[i-1]);        /* f2[i] -= f2[i-1]; */
  }

  a[0] = 4096;
  for (i = 1, j = 10; i <= 5; i++, j--)
  {
    t0   = L_add(f1[i], f2[i]);                 /* f1[i] + f2[i]             */
    a[i] = extract_l( L_shr_r(t0, 13) );        /* from Q24 to Q12 and * 0.5 */

    t0   = L_sub(f1[i], f2[i]);                 /* f1[i] - f2[i]             */
    a[j] = extract_l( L_shr_r(t0, 13) );        /* from Q24 to Q12 and * 0.5 */

  }

  return;
}

/*-----------------------------------------------------------*
 * procedure Get_lsp_pol:                                    *
 *           ~~~~~~~~~~~                                     *
 *   Find the polynomial F1(z) or F2(z) from the LSPs        *
 *-----------------------------------------------------------*
 *                                                           *
 * Parameters:                                               *
 *  lsp[]   : line spectral freq. (cosine domain)    in Q15  *
 *  f[]     : the coefficients of F1 or F2           in Q24  *
 *-----------------------------------------------------------*/
/*
static void Get_lsp_pol(Word16 *lsp, Word32 *f)
{
  Word16 i,j, hi, lo;
  Word32 t0;
  
	     	Word32 temp,temp1,temp2;
		Word32 L_var_out;
		Word32 L_produit; 
		

   *f = L_mult(4096, 2048);             
   f++;
   *f = L_msu((Word32)0, *lsp, 512);   

   f++;
   lsp += 2;                           

   for(i=2; i<=5; i++)
   {
     *f = f[-2];

     for(j=1; j<i; j++, f--)
     {
      // L_Extract(f[-1] ,&hi, &lo);
		  
		hi  = (Word16)(f[-1] >> 16);
		temp = f[-1] >> 1;
		L_produit = (Word32)(hi);	   
		L_produit = L_produit<<15;
		temp1 = temp - L_produit;
		lo  = (Word16)(temp1 );

  
       t0 = Mpy_32_16(hi, lo, *lsp);        
       //t0 = L_shl(t0, 1);
       if (t0 > (Word32) 0X3fffffffL)
       {
            	t0 = MAX_32;
       }
       else if (t0 < (Word32) 0xc0000000L)
       {            
          	t0 = MIN_32;
        }
	 else{
	  	t0 <<= 1;
	 }
		 
       *f = L_add(*f, f[-2]);               
       *f = L_sub(*f, t0);                  
       
     }
     *f   = L_msu(*f, *lsp, 512);            
     f   += i;                               
     lsp += 2;                              
   }

   return;
}
*/
/*___________________________________________________________________________
 |                                                                           |
 |   Functions : Lsp_lsf and Lsf_lsp                                         |
 |                                                                           |
 |      Lsp_lsf   Transformation lsp to lsf                                  |
 |      Lsf_lsp   Transformation lsf to lsp                                  |
 |---------------------------------------------------------------------------|
 |  Algorithm:                                                               |
 |                                                                           |
 |   The transformation from lsp[i] to lsf[i] and lsf[i] to lsp[i] are       |
 |   approximated by a look-up table and interpolation.                      |
 |___________________________________________________________________________|
*/


void Lsf_lsp(
  Word16 lsf[],    /* (i) Q15 : lsf[m] normalized (range: 0.0<=val<=0.5) */
  Word16 lsp[],    /* (o) Q15 : lsp[m] (range: -1<=val<1)                */
  Word16 m         /* (i)     : LPC order                                */
)
{
  Word16 i, ind, offset;
  Word32 L_tmp;

  for(i=0; i<m; i++)
  {
    ind    = shr(lsf[i], 8);               /* ind    = b8-b15 of lsf[i] */
    offset = lsf[i] & (Word16)0x00ff;      /* offset = b0-b7  of lsf[i] */

    /* lsp[i] = table[ind]+ ((table[ind+1]-table[ind])*offset) / 256 */

    L_tmp   = L_mult(sub(table[ind+1], table[ind]), offset);
    lsp[i] = add(table[ind], extract_l(L_shr(L_tmp, 9)));
  }
  return;
}

/*
void Lsp_lsf(
  Word16 lsp[],   
  Word16 lsf[],   
  Word16 m                                    
)
{
  Word16 i, ind, tmp,wg_tmp;
  Word32 L_tmp;

  ind = 63;  

  for(i= m-(Word16)1; i >= 0; i--)
  {
   
    while( sub(table[ind], lsp[i]) < 0 )
    {
      ind = sub(ind,1);
    }

 

    L_tmp  = L_mult( sub(lsp[i], table[ind]) , slope[ind] );
    tmp = ROUND(L_shl(L_tmp, 3));     
    wg_tmp = 	(ind << 8);
    lsf[i] = add(tmp, wg_tmp );
  }
  return;
}
*/
/*___________________________________________________________________________
 |                                                                           |
 |   Functions : Lsp_lsf and Lsf_lsp                                         |
 |                                                                           |
 |      Lsp_lsf   Transformation lsp to lsf                                  |
 |      Lsf_lsp   Transformation lsf to lsp                                  |
 |---------------------------------------------------------------------------|
 |  Algorithm:                                                               |
 |                                                                           |
 |   The transformation from lsp[i] to lsf[i] and lsf[i] to lsp[i] are       |
 |   approximated by a look-up table and interpolation.                      |
 |___________________________________________________________________________|
*/
/*
void Lsf_lsp2(
  Word16 lsf[],    
  Word16 lsp[],    
  Word16 m  
)
{
  Word16 i, ind;
  Word16 offset;
  Word16 freq; 
  Word32 L_tmp, wg_temp;

  for(i=0; i<m; i++)
  {
    freq = mult(lsf[i], 20861);  
    ind = freq >> 8;
    offset = freq & (Word16)0x00ff;      

    //if ( sub(ind, 63)>0 ){
    if ( ind > 63 ){
      ind = 63;                
    }

    L_tmp   = L_mult(slope_cos[ind], offset);   
    wg_temp = L_tmp >> 13;
    lsp[i] = add(table2[ind], extract_l(wg_temp));

  }
  return;
}



void Lsp_lsf2(
  Word16 lsp[],    
  Word16 lsf[],   
  Word16 m       
)
{
  Word16 i, ind, temp;
  Word16 offset;   
  Word16 freq;    
  Word32 L_tmp;

  ind = 63;          
  for(i= m-(Word16)1; i >= 0; i--)
  {
    while( table2[ind] < lsp[i] )
    {
      ind = sub(ind,1);
      if ( ind <= 0 )
        break;
    }

    offset = sub(lsp[i], table2[ind]);

    L_tmp  = L_mult( slope_acos[ind], offset );  

    L_tmp = L_tmp >> 12;
    temp = extract_l(L_tmp);
	
    //freq = add(shl(ind, 9), extract_l(L_shr(L_tmp, 12)));
    freq = add(shl_test(ind, 9), temp);
	
    lsf[i] = mult(freq, 25736);          

  }
  return;
}
*/
/*-------------------------------------------------------------*
 *  procedure Weight_Az                                        *
 *            ~~~~~~~~~                                        *
 * Weighting of LPC coefficients.                              *
 *   ap[i]  =  a[i] * (gamma ** i)                             *
 *                                                             *
 *-------------------------------------------------------------*/

/*
void Weight_Az(
  Word16 a[],      
  Word16 gamma,    
  Word16 m,       
  Word16 ap[]     
)
{
  Word16 i, fac;
  Word32 L_var_ap1, L_var_ap2, L_var_ap3;

  ap[0] = a[0];
  fac   = gamma;
  for(i=1; i<m; i++)
  {
    L_var_ap1 = L_mult(a[i], fac);
    ap[i] = ROUND( L_var_ap1 );
    L_var_ap2 = L_mult(fac, gamma);	
    fac   = ROUND( L_var_ap2 );
  }
  L_var_ap3 = L_mult(a[m], fac); 
  ap[m] = ROUND( L_var_ap3 );

  return;
}
*/
/*----------------------------------------------------------------------*
 * Function Int_qlpc()                                                  *
 * ~~~~~~~~~~~~~~~~~~~                                                  *
 * Interpolation of the LPC parameters.                                 *
 *----------------------------------------------------------------------*/

/* Interpolation of the quantized LSP's */

void Int_qlpc(
 Word16 lsp_old[], /* input : LSP vector of past frame              */
 Word16 lsp_new[], /* input : LSP vector of present frame           */
 Word16 Az[]       /* output: interpolated Az() for the 2 subframes */
)
{
  Word16 i, wg_shr_temp1, wg_shr_temp2;
  Word16 lsp[M];

  /*  lsp[i] = lsp_new[i] * 0.5 + lsp_old[i] * 0.5 */

  for (i = 0; i < M; i++) {
    wg_shr_temp1 = lsp_new[i] >> 1;
    wg_shr_temp2 = lsp_old[i] >> 1;	
    lsp[i] = add((wg_shr_temp1), (wg_shr_temp2));
    //lsp[i] = add(shr(lsp_new[i], 1), shr(lsp_old[i], 1));
  }

  Lsp_Az(lsp, Az);              /* Subframe 1 */

  Lsp_Az(lsp_new, &Az[MP1]);    /* Subframe 2 */

  return;
}

