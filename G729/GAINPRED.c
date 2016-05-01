/*
   ITU-T G.729A Speech Coder    ANSI-C Source Code
   Version 1.1    Last modified: September 1996
   Copyright (c) 1996,
   AT&T, France Telecom, NTT, Universite de Sherbrooke, Lucent Technologies
   All rights reserved.
*/

/*---------------------------------------------------------------------------*
 *  Gain_predict()        : make gcode0(exp_gcode0)                          *
 *  Gain_update()         : update table of past quantized energies.         *
 *  Gain_update_erasure() : update table of past quantized energies.         *
 *                                                        (frame erasure)    *
 *    This function is used both Coder and Decoder.                          *
 *---------------------------------------------------------------------------*/

#include "typedef.h"
#include "basic_op.h"
#include "ld8a.h"
#include "tab_ld8a.h"
#include "oper_32b.h"

/*---------------------------------------------------------------------------*
 * Function  Gain_predict                                                    *
 * ~~~~~~~~~~~~~~~~~~~~~~                                                    *
 * MA prediction is performed on the innovation energy (in dB with mean      *
 * removed).                                                                 *
 *---------------------------------------------------------------------------*/

/*
void Gain_predict(
   Word16 past_qua_en[], 
   Word16 code[],        
   Word16 L_subfr,      
   Word16 *gcode0,       
   Word16 *exp_gcode0    
)
{
   Word16  i, exp, frac;
   Word32  L_tmp;

   L_tmp = 0;
   for(i=0; i<L_subfr; i++)
     	L_tmp = L_mac(L_tmp, code[i], code[i]);


   Log2(L_tmp, &exp, &frac);               
   L_tmp = Mpy_32_16(exp, frac, -24660); 
   L_tmp = L_mac(L_tmp, 32588, 32); 

   L_tmp = L_shl(L_tmp, 10);                    
   for(i=0; i<4; i++)
     	L_tmp = L_mac(L_tmp, pred[i], past_qua_en[i]); 

   *gcode0 = extract_h(L_tmp);

   L_tmp = L_mult(*gcode0, 5439);      
   L_tmp = L_shr(L_tmp, 8);            
   L_Extract(L_tmp, &exp, &frac);       

   *gcode0 = extract_l(Pow2(14, frac)); 
                                       
   *exp_gcode0 = sub(14,exp);
}
*/

/*---------------------------------------------------------------------------*
 * Function  Gain_update                                                     *
 * ~~~~~~~~~~~~~~~~~~~~~~                                                    *
 * update table of past quantized energies                                   *
 *---------------------------------------------------------------------------*/
void Gain_update(
   Word16 past_qua_en[],   /* (io) Q10 :Past quantized energies           */
   Word32  L_gbk12         /* (i) Q13 : gbk1[indice1][1]+gbk2[indice2][1] */
)
{
   Word16  i, tmp;
   Word16  exp, frac;
   Word32  L_acc;

   for(i=3; i>0; i--){
      past_qua_en[i] = past_qua_en[i-1];         /* Q10 */
   }
  /*----------------------------------------------------------------------*
   * -- past_qua_en[0] = 20*log10(gbk1[index1][1]+gbk2[index2][1]); --    *
   *    2 * 10 log10( gbk1[index1][1]+gbk2[index2][1] )                   *
   *  = 2 * 3.0103 log2( gbk1[index1][1]+gbk2[index2][1] )                *
   *  = 2 * 3.0103 log2( gbk1[index1][1]+gbk2[index2][1] )                *
   *                                                 24660:Q12(6.0205)    *
   *----------------------------------------------------------------------*/

   Log2( L_gbk12, &exp, &frac );               /* L_gbk12:Q13       */
   L_acc = L_Comp( sub(exp,13), frac);         /* L_acc:Q16           */
   tmp = extract_h( L_shl( L_acc,13 ) );       /* tmp:Q13           */
   past_qua_en[0] = mult( tmp, 24660 );        /* past_qua_en[]:Q10 */
}


/*---------------------------------------------------------------------------*
 * Function  Gain_update_erasure                                             *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~                                             *
 * update table of past quantized energies (frame erasure)                   *
 *---------------------------------------------------------------------------*
 *     av_pred_en = 0.0;                                                     *
 *     for (i = 0; i < 4; i++)                                               *
 *        av_pred_en += past_qua_en[i];                                      *
 *     av_pred_en = av_pred_en*0.25 - 4.0;                                   *
 *     if (av_pred_en < -14.0) av_pred_en = -14.0;                           *
 *---------------------------------------------------------------------------*/
void Gain_update_erasure(
   Word16 past_qua_en[]     /* (i) Q10 :Past quantized energies        */
)
{
   Word16  i, av_pred_en;
   Word32  L_tmp;

   L_tmp = 0;                                                     /* Q10 */
   for(i=0; i<4; i++)
      L_tmp = L_add( L_tmp, L_deposit_l( past_qua_en[i] ) );
   av_pred_en = extract_l( L_shr( L_tmp, 2 ) );
   av_pred_en = sub( av_pred_en, 4096 );                          /* Q10 */

   if( sub(av_pred_en, -14336) < 0 ){
      av_pred_en = -14336;                              /* 14336:14[Q10] */
   }

   for(i=3; i>0; i--){
      past_qua_en[i] = past_qua_en[i-1];
   }
   past_qua_en[0] = av_pred_en;
}
