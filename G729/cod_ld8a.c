/*
   ITU-T G.729A Speech Coder with Annex B    ANSI-C Source Code
   Version 1.3    Last modified: August 1997

   Copyright (c) 1996,
   AT&T, France Telecom, NTT, Universite de Sherbrooke, Lucent Technologies,
   Rockwell International
   All rights reserved.
*/

/*-----------------------------------------------------------------*
 *   Functions Coder_ld8a and Init_Coder_ld8a                      *
 *             ~~~~~~~~~~     ~~~~~~~~~~~~~~~                      *
 *                                                                 *
 *  Init_Coder_ld8a(void);                                         *
 *                                                                 *
 *   ->Initialization of variables for the coder section.          *
 *                                                                 *
 *                                                                 *
 *  Coder_ld8a(Word16 ana[]);                                      *
 *                                                                 *
 *   ->Main coder function.                                        *
 *                                                                 *
 *                                                                 *
 *  Input:                                                         *
 *                                                                 *
 *    80 speech data should have beee copy to vector new_speech[]. *
 *    This vector is global and is declared in this function.      *
 *                                                                 *
 *  Ouputs:                                                        *
 *                                                                 *
 *    ana[]      ->analysis parameters.                            *
 *                                                                 *
 *-----------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>

#include "typedef.h"
#include "basic_op.h"
#include "ld8a.h"
#include "vad.h"
#include "dtx.h"
#include "sid.h"
//#include "vad_test.h"


/*-----------------------------------------------------------*
 *    Coder constant parameters (defined in "ld8a.h")        *
 *-----------------------------------------------------------*
 *   L_WINDOW    : LPC analysis window size.                 *
 *   L_NEXT      : Samples of next frame needed for autocor. *
 *   L_FRAME     : Frame size.                               *
 *   L_SUBFR     : Sub-frame size.                           *
 *   M           : LPC order.                                *
 *   MP1         : LPC order+1                               *
 *   L_TOTAL     : Total size of speech buffer.              *
 *   PIT_MIN     : Minimum pitch lag.                        *
 *   PIT_MAX     : Maximum pitch lag.                        *
 *   L_INTERPOL  : Length of filter for interpolation        *
 *-----------------------------------------------------------*/


/*-----------------------------------------------------------------*
 *   Function  Init_Coder_ld8a                                     *
 *            ~~~~~~~~~~~~~~~                                      *
 *                                                                 *
 *  Init_Coder_ld8a(void);                                         *
 *                                                                 *
 *   ->Initialization of variables for the coder section.          *
 *       - initialize pointers to speech buffer                    *
 *       - initialize static  pointers                             *
 *       - set static vectors to zero                              *
 *                                                                 *
 *-----------------------------------------------------------------*/




/*
void  wg_udi_2(Word16 i_subfr,Word16 gain_pit,Word16 * y1,Word16 * y2,Word16 * xn, Word16 gain_code )
{
    Word16 i, j;
	Word16 temp, k;
	
    for (i = L_SUBFR-M, j = 0; i < L_SUBFR; i++, j++)
    {
      temp       = extract_h(L_shl( L_mult(y1[i], gain_pit),  1) );
      k          = extract_h(L_shl( L_mult(y2[i], gain_code), 2) );
      mem_w0[j]  = sub(xn[i], add(temp, k));
    }
 
}
*/
/*-----------------------------------------------------------------*
 *   Functions Coder_ld8a                                          *
 *            ~~~~~~~~~~                                           *
 *  Coder_ld8a(Word16 ana[]);                                      *
 *                                                                 *
 *   ->Main coder function.                                        *
 *                                                                 *
 *                                                                 *
 *  Input:                                                         *
 *                                                                 *
 *    80 speech data should have beee copy to vector new_speech[]. *
 *    This vector is global and is declared in this function.      *
 *                                                                 *
 *  Ouputs:                                                        *
 *                                                                 *
 *    ana[]      ->analysis parameters.                            *
 *                                                                 *
 *-----------------------------------------------------------------*/

/*


void Coder_ld8a(
     Word16 ana[],       
     Word16 frame,       
     Word16 vad_enable   
)
{

  Word16 Aq_t[(MP1)*2];        
  Word16 Ap_t[(MP1)*2];         
  Word16 *Aq, *Ap;              

  Word16 h1[L_SUBFR];            
  Word16 xn[L_SUBFR];            
  Word16 xn2[L_SUBFR];         
  Word16 code[L_SUBFR];     
  Word16 y1[L_SUBFR];         
  Word16 y2[L_SUBFR];    
  Word16 g_coeff[4];      

  Word16 g_coeff_cs[5];
  Word16 exp_g_coeff_cs[5];      

  Word16 i, j, k, i_subfr;
  Word16 T_op, T0, T0_min, T0_max, T0_frac;
  Word16 gain_pit, gain_code, index;
  Word16 temp, taming;
  Word32 L_temp;

  {
   
    Word16 r_l[NP+1], r_h[NP+1];    
    Word16 rc[M];                   
    Word16 lsp_new[M], lsp_new_q[M]; 

    Word16 rh_nbe[MP1];
    Word16 lsf_new[M];
    Word16 lsfq_mem[MA_NP][M];
    Word16 exp_R0, Vad;
    Word16	i;

    Autocorr(p_window, NP, r_h, r_l, &exp_R0);     

    //Copy(r_h, rh_nbe, MP1);
     for (i = 0; i < MP1; i++)		//Coder_ld8a112
     	rh_nbe[i] = r_h[i];

    Lag_window(NP, r_h, r_l);                      
    Levinson(r_h, r_l, Ap_t, rc, &temp);         
    Az_lsp(Ap_t, lsp_new, lsp_old);              


    Lsp_lsf(lsp_new, lsf_new, M);
    vad_wg(rc[1], lsf_new, r_h, r_l, exp_R0, p_window, frame,
        pastVad, ppastVad, &Vad);

    Update_cng(rh_nbe, exp_R0, Vad);


    if ((Vad == 0) && (vad_enable == 1)){

      Get_freq_prev(lsfq_mem);
      Cod_cng(udi_exc, pastVad, lsp_old_q, Aq_t, ana, lsfq_mem, &seed);
      Update_freq_prev(lsfq_mem);
      ppastVad = pastVad;
      pastVad = Vad;
 
      Aq = Aq_t;
      for(i_subfr=0; i_subfr < L_FRAME; i_subfr += L_SUBFR) {

        Residu(Aq, &speech[i_subfr], xn, L_SUBFR);

        Weight_Az(Aq, GAMMA1, M, Ap_t);

        Ap = Ap_t + MP1;
        Ap[0] = 4096;
        for(i=1; i<=M; i++)   
          Ap[i] = sub(Ap_t[i], mult(Ap_t[i-1], 22938));
        Syn_filt(Ap, xn, &wsp[i_subfr], L_SUBFR, mem_w, 1);

        for(i=0; i<L_SUBFR; i++) {
          xn[i] = sub(xn[i], udi_exc[i_subfr+i]);  
        }
        Syn_filt(Ap_t, xn, xn, L_SUBFR, mem_w0, 1);

        Aq += MP1;
      }

      sharp = SHARPMIN;

      Copy(&old_speech[L_FRAME], &old_speech[0], L_TOTAL-L_FRAME);
      Copy(&old_wsp[L_FRAME], &old_wsp[0], PIT_MAX);
      Copy(&old_exc[L_FRAME], &old_exc[0], PIT_MAX+L_INTERPOL);

      return;
    } 


    // Case of active frame 
    *ana++ = 1;
    seed = INIT_SEED;
    ppastVad = pastVad;
    pastVad = Vad;

    Qua_lsp(lsp_new, lsp_new_q, ana);
    ana += 2;                        

    Int_qlpc(lsp_old_q, lsp_new_q, Aq_t);



    Weight_Az(&Aq_t[0],   GAMMA1, M, &Ap_t[0]);
    Weight_Az(&Aq_t[MP1], GAMMA1, M, &Ap_t[MP1]);



    Copy(lsp_new,   lsp_old,   M);
    Copy(lsp_new_q, lsp_old_q, M);
  }



  Residu(&Aq_t[0], &speech[0], &udi_exc[0], L_SUBFR);
  Residu(&Aq_t[MP1], &speech[L_SUBFR], &udi_exc[L_SUBFR], L_SUBFR);

  {
    Word16 Ap1[MP1];

    Ap = Ap_t;
    Ap1[0] = 4096;
    for(i=1; i<=M; i++)    //Coder_ld8a1084
       Ap1[i] = sub(Ap[i], mult(Ap[i-1], 22938));
    Syn_filt(Ap1, &udi_exc[0], &wsp[0], L_SUBFR, mem_w, 1);

    Ap += MP1;
    for(i=1; i<=M; i++)    
       Ap1[i] = sub(Ap[i], mult(Ap[i-1], 22938));
    Syn_filt(Ap1, &udi_exc[L_SUBFR], &wsp[L_SUBFR], L_SUBFR, mem_w, 1);
  }



  T_op = Pitch_ol_fast(wsp, PIT_MAX, L_FRAME);

 

  T0_min = sub(T_op, 3);
  if (sub(T0_min,PIT_MIN)<0) {
    T0_min = PIT_MIN;
  }

  T0_max = add(T0_min, 6);
  if (sub(T0_max ,PIT_MAX)>0)
  {
     T0_max = PIT_MAX;
     T0_min = sub(T0_max, 6);
  }


 

  Aq = Aq_t;    
  Ap = Ap_t;   

  for (i_subfr = 0;  i_subfr < L_FRAME; i_subfr += L_SUBFR)
  {

    h1[0] = 4096;
    Set_zero(&h1[1], L_SUBFR-1);
    Syn_filt(Ap, h1, h1, L_SUBFR, &h1[1], 0);

    Syn_filt(Ap, &udi_exc[i_subfr], xn, L_SUBFR, mem_w0, 0);

    T0 = Pitch_fr3_fast(&udi_exc[i_subfr], xn, h1, L_SUBFR, T0_min, T0_max,
                    i_subfr, &T0_frac);

    index = Enc_lag3(T0, T0_frac, &T0_min, &T0_max,PIT_MIN,PIT_MAX,i_subfr);

    *ana++ = index;

    if (i_subfr == 0) {
      *ana++ = Parity_Pitch(index);
    }



    Syn_filt(Ap, &udi_exc[i_subfr], y1, L_SUBFR, mem_zero, 0);

    gain_pit = G_pitch(xn, y1, g_coeff, L_SUBFR);   

    taming = test_err(T0, T0_frac);

    if( taming == 1){
		//Coder_ld8a2456
      if (sub(gain_pit, GPCLIP) > 0) {
        gain_pit = GPCLIP;
      }
    }


    for (i = 0; i < L_SUBFR; i++)//Coder_ld8a1760
    {
      L_temp = L_mult(y1[i], gain_pit);
      L_temp = L_shl(L_temp, 1);               
      xn2[i] = sub(xn[i], extract_h(L_temp));
    }



    index = ACELP_Code_A(xn2, h1, T0, sharp, code, y2, &i);

    *ana++ = index;        
    *ana++ = i;            


    g_coeff_cs[0]     = g_coeff[0];           
    exp_g_coeff_cs[0] = negate(g_coeff[1]);    
    g_coeff_cs[1]     = negate(g_coeff[2]);    
    exp_g_coeff_cs[1] = negate(add(g_coeff[3], 1)); 

    Corr_xy2( xn, y1, y2, g_coeff_cs, exp_g_coeff_cs );  

    *ana++ = Qua_gain(code, g_coeff_cs, exp_g_coeff_cs,
                         L_SUBFR, &gain_pit, &gain_code, taming);


    sharp = gain_pit;
    if (sub(sharp, SHARPMAX) > 0) { sharp = SHARPMAX;         }
    if (sub(sharp, SHARPMIN) < 0) { sharp = SHARPMIN;         }

    for (i = 0; i < L_SUBFR;  i++)	//Coder_ld8a2132
    {
      L_temp = L_mult(udi_exc[i+i_subfr], gain_pit);
      L_temp = L_mac(L_temp, code[i], gain_code);
      L_temp = L_shl(L_temp, 1);
      udi_exc[i+i_subfr] = ROUND(L_temp);
    }

   
    	update_exc_err(gain_pit, T0);
	
	wg_udi_2(i_subfr, gain_pit, y1, y2, xn, gain_code );


    Aq += MP1;           
    Ap += MP1;

  }



  Copy(&old_speech[L_FRAME], &old_speech[0], L_TOTAL-L_FRAME);
  Copy(&old_wsp[L_FRAME], &old_wsp[0], PIT_MAX);
  Copy(&old_exc[L_FRAME], &old_exc[0], PIT_MAX+L_INTERPOL);

  return;
}

*/


