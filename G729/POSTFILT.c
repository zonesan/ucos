/*
   ITU-T G.729A Speech Coder with Annex B    ANSI-C Source Code
   Version 1.3    Last modified: August 1997

   Copyright (c) 1996,
   AT&T, France Telecom, NTT, Universite de Sherbrooke, Lucent Technologies,
   Rockwell International
   All rights reserved.
*/

/*------------------------------------------------------------------------*
 *                         POSTFILTER.C                                   *
 *------------------------------------------------------------------------*
 * Performs adaptive postfiltering on the synthesis speech                *
 * This file contains all functions related to the post filter.           *
 *------------------------------------------------------------------------*/

#include "typedef.h"
#include "basic_op.h"
#include "ld8a.h"
#include "oper_32b.h"

/*---------------------------------------------------------------*
 *    Postfilter constant parameters (defined in "ld8a.h")       *
 *---------------------------------------------------------------*
 *   L_FRAME     : Frame size.                                   *
 *   L_SUBFR     : Sub-frame size.                               *
 *   M           : LPC order.                                    *
 *   MP1         : LPC order+1                                   *
 *   PIT_MAX     : Maximum pitch lag.                            *
 *   GAMMA2_PST  : Formant postfiltering factor (numerator)      *
 *   GAMMA1_PST  : Formant postfiltering factor (denominator)    *
 *   GAMMAP      : Harmonic postfiltering factor                 *
 *   MU          : Factor for tilt compensation filter           *
 *   AGC_FAC     : Factor for automatic gain control             *
 *---------------------------------------------------------------*/


/*------------------------------------------------------------*
 *   static vectors                                           *
 *------------------------------------------------------------*/

        /* inverse filtered synthesis (with A(z/GAMMA2_PST))   */

static Word16 res2_buf[PIT_MAX+L_SUBFR];
static Word16 *res2;
static Word16 scal_res2_buf[PIT_MAX+L_SUBFR];
static Word16 *scal_res2;

/*wg*/
Word16 past_gain; 
Word16 mem_pre;
        /* memory of filter 1/A(z/GAMMA1_PST) */

static Word16 mem_syn_pst[M];

/*---------------------------------------------------------------*
 * Procedure    Init_Post_Filter:                                *
 *              ~~~~~~~~~~~~~~~~                                 *
 *  Initializes the postfilter parameters:                       *
 *---------------------------------------------------------------*/

void Init_Post_Filter(void)
{
  res2  = res2_buf + PIT_MAX;
  scal_res2  = scal_res2_buf + PIT_MAX;

  Set_zero(mem_syn_pst, M);
  Set_zero(res2_buf, PIT_MAX+L_SUBFR);
  Set_zero(scal_res2_buf, PIT_MAX+L_SUBFR);

  past_gain=4096; 
  mem_pre = 0;

  return;
}


/*------------------------------------------------------------------------*
 *  Procedure     Post_Filter:                                            *
 *                ~~~~~~~~~~~                                             *
 *------------------------------------------------------------------------*
 *  The postfiltering process is described as follows:                    *
 *                                                                        *
 *  - inverse filtering of syn[] through A(z/GAMMA2_PST) to get res2[]    *
 *  - use res2[] to compute pitch parameters                              *
 *  - perform pitch postfiltering                                         *
 *  - tilt compensation filtering; 1 - MU*k*z^-1                          *
 *  - synthesis filtering through 1/A(z/GAMMA1_PST)                       *
 *  - adaptive gain control                                               *
 *------------------------------------------------------------------------*/

void Post_Filter(
  Word16 *syn,       /* in/out: synthesis speech (postfiltered is output)    */
  Word16 *Az_4,      /* input : interpolated LPC parameters in all subframes */
  Word16 *T,          /* input : decoded pitch lags in all subframes          */
  Word16 Vad
)
{
 /*-------------------------------------------------------------------*
  *           Declaration of parameters                               *
  *-------------------------------------------------------------------*/

 Word16 res2_pst[L_SUBFR];  /* res2[] after pitch postfiltering */
 Word16 syn_pst[L_FRAME];   /* post filtered synthesis speech   */

 Word16 Ap3[MP1], Ap4[MP1];  /* bandwidth expanded LP parameters */

 Word16 *Az;                 /* pointer to Az_4:                 */
                             /*  LPC parameters in each subframe */
 Word16   t0_max, t0_min;    /* closed-loop pitch search range   */
 Word16   i_subfr;           /* index for beginning of subframe  */

 Word16 h[L_H];

 Word16  i, j;
 Word16  temp1, temp2;
 Word32  L_tmp;

   /*-----------------------------------------------------*
    * Post filtering                                      *
    *-----------------------------------------------------*/

    Az = Az_4;

    for (i_subfr = 0; i_subfr < L_FRAME; i_subfr += L_SUBFR)
    {
      /* Find pitch range t0_min - t0_max */

      t0_min = sub(*T++, 3);
      t0_max = add(t0_min, 6);
      if (sub(t0_max, PIT_MAX) > 0) {
        t0_max = PIT_MAX;
        t0_min = sub(t0_max, 6);
      }

      /* Find weighted filter coefficients Ap3[] and ap[4] */

      Weight_Az(Az, GAMMA2_PST, M, Ap3);
      Weight_Az(Az, GAMMA1_PST, M, Ap4);

      /* filtering of synthesis speech by A(z/GAMMA2_PST) to find res2[] */

      Residu(Ap3, &syn[i_subfr], res2, L_SUBFR);

      /* scaling of "res2[]" to avoid energy overflow */

      for (j=0; j<L_SUBFR; j++)
      {
        scal_res2[j] = shr(res2[j], 2);
      }

      /* pitch postfiltering */
      if (sub(Vad, 1) == 0)
        pit_pst_filt(res2, scal_res2, t0_min, t0_max, L_SUBFR, res2_pst);
      else
        for (j=0; j<L_SUBFR; j++)
          res2_pst[j] = res2[j];

      /* tilt compensation filter */

      /* impulse response of A(z/GAMMA2_PST)/A(z/GAMMA1_PST) */

      Copy(Ap3, h, M+1);
      Set_zero(&h[M+1], L_H-M-1);
      Syn_filt(Ap4, h, h, L_H, &h[M+1], 0);

      /* 1st correlation of h[] */

      L_tmp = L_mult(h[0], h[0]);
      for (i=1; i<L_H; i++) L_tmp = L_mac(L_tmp, h[i], h[i]);
      temp1 = extract_h(L_tmp);

      L_tmp = L_mult(h[0], h[1]);
      for (i=1; i<L_H-1; i++) L_tmp = L_mac(L_tmp, h[i], h[i+1]);
      temp2 = extract_h(L_tmp);

      if(temp2 <= 0) {
        temp2 = 0;
      }
      else {
        temp2 = mult(temp2, MU);
        temp2 = div_s(temp2, temp1);
      }

      preemphasis(res2_pst, temp2, L_SUBFR);

      /* filtering through  1/A(z/GAMMA1_PST) */

      Syn_filt(Ap4, res2_pst, &syn_pst[i_subfr], L_SUBFR, mem_syn_pst, 1);

      /* scale output to input */

      agc(&syn[i_subfr], &syn_pst[i_subfr], L_SUBFR);

      /* update res2[] buffer;  shift by L_SUBFR */

      Copy(&res2[L_SUBFR-PIT_MAX], &res2[-PIT_MAX], PIT_MAX);
      Copy(&scal_res2[L_SUBFR-PIT_MAX], &scal_res2[-PIT_MAX], PIT_MAX);

      Az += MP1;
    }

    /* update syn[] buffer */

    Copy(&syn[L_FRAME-M], &syn[-M], M);

    /* overwrite synthesis speech by postfiltered synthesis speech */

    Copy(syn_pst, syn, L_FRAME);

    return;
}


/*---------------------------------------------------------------------------*
 * procedure pitch_pst_filt                                                  *
 * ~~~~~~~~~~~~~~~~~~~~~~~~                                                  *
 * Find the pitch period  around the transmitted pitch and perform           *
 * harmonic postfiltering.                                                   *
 * Filtering through   (1 + g z^-T) / (1+g) ;   g = min(pit_gain*gammap, 1)  *
 *--------------------------------------------------------------------------*/
/*
void pit_pst_filt(
  Word16 *signal,      
  Word16 *scal_sig,   
  Word16 t0_min,      
  Word16 t0_max,       
  Word16 L_subfr,      
  Word16 *signal_pst   
)
{
  Word16 i, j, t0;
  Word16 g0, gain, cmax, en, en0;
  Word16 *p, *p1, *p2, *deb_sig;
  Word32 corr, cor_max, ener, ener0, temp;
  Word32 L_temp;

  deb_sig = &scal_sig[-t0_min];
  cor_max = MIN_32;
  t0 = t0_min;            
  for (i=t0_min; i<=t0_max; i++)
  {
    corr = 0;
    p    = scal_sig;
    p1   = deb_sig;
    for (j=0; j<L_subfr; j++)
       corr = L_mac(corr, *p++, *p1++);

    L_temp = L_sub(corr, cor_max);
    if (L_temp > (Word32)0)
    {
      cor_max = corr;
      t0 = i;
    }
    deb_sig--;
  }
  
  ener = 1;
  ener0 = 1;
  p2 = scal_sig;  
  
  p = scal_sig - t0;
  for ( i=0; i<L_subfr ;i++, p++,p2++)
  	{
    	ener = L_mac(ener, *p, *p);
	ener0 = L_mac(ener0, *p2, *p2);
  	}

  //for ( i=0; i<L_subfr; i++, p++)
    

  if (cor_max < 0)
  {
    cor_max = 0;
  }

  temp = cor_max;
  if (ener > temp)
  {
    temp = ener;
  }
  if (ener0 > temp)
  {
    temp = ener0;
  }
  j = norm_l(temp);
  cmax = ROUND(L_shl(cor_max, j));
  en = ROUND(L_shl(ener, j));
  en0 = ROUND(L_shl(ener0, j));

  temp = L_mult(cmax, cmax);
  temp = L_sub(temp, L_shr(L_mult(en, en0), 1));

  if (temp < (Word32)0)           
  {      //pit_pst_filt772                         
    for (i = 0; i < L_subfr; i++)
      signal_pst[i] = signal[i];
    return;
  }

  if (sub(cmax, en) > 0)      
  {
    g0 = INV_GAMMAP;
    gain = GAMMAP_2;
  }
  else {//pit_pst_filt876
    cmax = shr(mult(cmax, GAMMAP), 1);  
    en = shr(en, 1);          
    i = add(cmax, en);
    if(i > 0)
    {
      gain = div_s(cmax, i);    
      g0 = sub(32767, gain);    
    }
    else
    {
      g0 =  32767;
      gain = 0;
    }
  }


  for (i = 0; i < L_subfr; i++)
  {

    signal_pst[i] = add(mult(g0, signal[i]), mult(gain, signal[i-t0]));

  }

  return;
}
*/

/*---------------------------------------------------------------------*
 * routine preemphasis()                                               *
 * ~~~~~~~~~~~~~~~~~~~~~                                               *
 * Preemphasis: filtering through 1 - g z^-1                           *
 *---------------------------------------------------------------------*/
/*
void preemphasis(
  Word16 *signal,  
  Word16 g,        
  Word16 L         
)
{
  
  Word16 *p1, *p2, temp, i;

  p1 = signal + L - 1;
  p2 = p1 - 1;
  temp = *p1;

  for (i = 0; i <= L-2; i++)
  {
    *p1-- = sub(*p1, mult(g, *p2--));
  }

  *p1 = sub(*p1, mult(g, mem_pre));

  mem_pre = temp;

  return;
}
*/


/*----------------------------------------------------------------------*
 *   routine agc()                                                      *
 *   ~~~~~~~~~~~~~                                                      *
 * Scale the postfilter output on a subframe basis by automatic control *
 * of the subframe gain.                                                *
 *  gain[n] = AGC_FAC * gain[n-1] + (1 - AGC_FAC) g_in/g_out            *
 *----------------------------------------------------------------------*/
  /*
void agc(
  Word16 *sig_in,   
  Word16 *sig_out,  
  Word16 l_trm      
)
{
    
  Word16 i, exp;
  Word16 gain_in, gain_out, g0, gain;                     
  Word32 s;

  Word16 signal[L_SUBFR];


s = 0;
  for(i=0; i<l_trm; i++)//agc80
    //signal[i] = shr(sig_out[i], 2);
  	{
    signal[i] = (sig_out[i] >> 2);
	s = L_mac(s, signal[i], signal[i]);
  	}

  
 // for(i=0; i<l_trm; i++)//agc136
    //s = L_mac(s, signal[i], signal[i]);

  if (s == 0) {
    past_gain = 0;
    return;
  }
  exp = sub(norm_l(s), 1);
  gain_out = ROUND(L_shl(s, exp)); 
 s = 0;
  for(i=0; i<l_trm; i++)//agc252
  {
    //signal[i] = shr(sig_in[i], 2);
    signal[i] = (sig_in[i] >> 2);
   s = L_mac(s, signal[i], signal[i]);
}

 
  //for(i=0; i<l_trm; i++)//agc308
   //s = L_mac(s, signal[i], signal[i]);

  if (s == 0) {
    g0 = 0;
  }
  else {
    i = norm_l(s);
    gain_in = ROUND(L_shl(s, i));
    exp = sub(exp, i);   

    s = L_deposit_l(div_s(gain_out,gain_in));   
    s = L_shl(s, 7);           
    s = L_shr(s, exp);        

    
    s = Inv_sqrt(s);           
    i = ROUND(L_shl(s,9));     

    
    g0 = mult(i, AGC_FAC1);       
  }

  

  gain = past_gain;
  for(i=0; i<l_trm; i++) {//agc380
    gain = mult(gain, AGC_FAC);
    gain = add(gain, g0);
    sig_out[i] = extract_h(L_shl(L_mult(sig_out[i], gain), 3));
  }
  past_gain = gain;

  return;
}
*/


