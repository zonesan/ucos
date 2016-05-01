/*
   ITU-T G.729A Speech Coder    ANSI-C Source Code
   Version 1.1    Last modified: September 1996

   Copyright (c) 1996,
   AT&T, France Telecom, NTT, Universite de Sherbrooke, Lucent Technologies
   All rights reserved.
*/

#include <stdio.h>
#include "typedef.h"
#include "basic_op.h"
#include "ld8a.h"


static Word16 wg_sature(Word32 L_var1)
  {
   Word16 var_out;

   if (L_var1 > 0X00007fffL)
     {
      Overflow = 1;
      var_out = MAX_16;
     }
   else if (L_var1 < (Word32)0xffff8000L)
     {
      Overflow = 1;
      var_out = MIN_16;
     }
   else
     {
      Overflow = 0;
      var_out = extract_l(L_var1);
     }

   return(var_out);
  }

void Lsp_get_quant(
  Word16 lspcb1[][M],      /* (i) Q13 : first stage LSP codebook      */
  Word16 lspcb2[][M],      /* (i) Q13 : Second stage LSP codebook     */
  Word16 code0,            /* (i)     : selected code of first stage  */
  Word16 code1,            /* (i)     : selected code of second stage */
  Word16 code2,            /* (i)     : selected code of second stage */
  Word16 fg[][M],          /* (i) Q15 : MA prediction coef.           */
  Word16 freq_prev[][M],   /* (i) Q13 : previous LSP vector           */
  Word16 lspq[],           /* (o) Q13 : quantized LSP parameters      */
  Word16 fg_sum[]          /* (i) Q15 : present MA prediction coef.   */
)
{
  Word16 j;
  Word16 buf[M];           /* Q13 */


  for ( j = 0 ; j < NC ; j++ )
    buf[j] = add( lspcb1[code0][j], lspcb2[code1][j] );

  for ( j = NC ; j < M ; j++ )
    buf[j] = add( lspcb1[code0][j], lspcb2[code2][j] );

  Lsp_expand_1_2(buf, GAP1);
  Lsp_expand_1_2(buf, GAP2);

  Lsp_prev_compose(buf, lspq, fg, freq_prev, fg_sum);

  Lsp_prev_update(buf, freq_prev);

  Lsp_stability( lspq );

  return;
}


void Lsp_expand_1(
  Word16 buf[],       
  Word16 gap          
)
{
  Word16 j, tmp, wg_temp;
  Word16 diff;        
  
  Word16 var_out;
  Word32 L_diff, L_somme, L_diff1, L_somme1;

  for ( j = 1 ; j < NC ; j++ ) {
  	
    
   	L_diff = (Word32) buf[j-1] - buf[j];
   	diff = wg_sature(L_diff);
   
   	L_somme = (Word32) diff + gap;
   	wg_temp = wg_sature(L_somme);

   	 tmp = wg_temp >> 1;

    	if ( tmp >  0 ) {
      		L_diff1 = (Word32) buf[j-1] - tmp;
      		buf[j-1] = wg_sature(L_diff1);
   
      		L_somme1 = (Word32) buf[j] + tmp;
      		buf[j]  = wg_sature(L_somme1);
	
    	}
  }
  return;
}


void Lsp_expand_2(
  Word16 buf[],       /* (i/o) Q13 : LSP vectors */
  Word16 gap          /* (i)   Q13 : gap         */
)
{
  Word16 j, tmp, wg_temp;
  Word16 diff;        /* Q13 */
  Word32 L_diff, L_somme, L_diff1, L_somme1;

  for ( j = NC ; j < M ; j++ ) {  	

   	L_diff = (Word32) buf[j-1] - buf[j];
   	diff = wg_sature(L_diff);   
   	L_somme = (Word32) diff + gap;
   	wg_temp = wg_sature(L_somme);	
    	tmp = wg_temp >> 1;
   	if ( tmp > 0 ) {

      		L_diff1 = (Word32) buf[j-1] - tmp;
      		buf[j-1] = wg_sature(L_diff1);
   
      		L_somme1 = (Word32) buf[j] + tmp;
      		buf[j]  = wg_sature(L_somme1);	  
    	}
  }
  return;
}


void Lsp_expand_1_2(
  Word16 buf[],       /* (i/o) Q13 : LSP vectors */
  Word16 gap          /* (i)   Q13 : gap         */
)
{
  Word16 j, tmp, wg_temp;
  Word16 diff;        /* Q13 */
  Word32 L_diff, L_somme, L_diff1, L_somme1;

  for ( j = 1 ; j < M ; j++ ) {
  	L_diff = (Word32) buf[j-1] - buf[j];
   	diff = wg_sature(L_diff);
   
   	L_somme = (Word32) diff + gap;
   	wg_temp = wg_sature(L_somme);
	
    	tmp = wg_temp >> 1;	

    	if ( tmp > 0 ) {

	      	L_diff1 = (Word32) buf[j-1] - tmp;
      		buf[j-1] = wg_sature(L_diff1);
   
      		L_somme1 = (Word32) buf[j] + tmp;
      		buf[j]  = wg_sature(L_somme1);
	
    	}
  }
  return;
}


/*
  Functions which use previous LSP parameter (freq_prev).
*/


/*
  compose LSP parameter from elementary LSP with previous LSP.
*/
/*
void Lsp_prev_compose(
  Word16 lsp_ele[],           
  Word16 lsp[],                
  Word16 fg[][M],              
  Word16 freq_prev[][M],       
  Word16 fg_sum[]              
)
{
  Word16 j, k;
  Word32 L_acc;               

  for ( j = 0 ; j < M ; j++ ) {
    L_acc = L_mult( lsp_ele[j], fg_sum[j] );
    for ( k = 0 ; k < MA_NP ; k++ )
      L_acc = L_mac( L_acc, freq_prev[k][j], fg[k][j] );

    lsp[j] = extract_h(L_acc);
  }
  return;
}
*/

/*
  extract elementary LSP from composed LSP with previous LSP
*/
/*
void Lsp_prev_extract(
  Word16 lsp[M],               
  Word16 lsp_ele[M],           
  Word16 fg[MA_NP][M],          
  Word16 freq_prev[MA_NP][M],   
  Word16 fg_sum_inv[M]          
)
{
  Word16 j, k, var2;
  Word32 L_temp;              
  Word16 temp;                  


  for ( j = 0 ; j < M ; j++ ) {
    //L_temp = L_deposit_h(lsp[j]);
    L_temp = (Word32) lsp[j] << 16;
    for ( k = 0 ; k < MA_NP ; k++ )
      L_temp = L_msu( L_temp, freq_prev[k][j], fg[k][j] );

    temp = extract_h(L_temp);
    L_temp = L_mult( temp, fg_sum_inv[j] );

	
      
      for(var2 = 3;var2>0;var2--)
        {
         if (L_temp > (Word32) 0X3fffffffL)
           {
            //Overflow = 1;
            L_temp = MAX_32;
           }
         else if(L_temp < (Word32) 0xc0000000L)
           {
               //Overflow = 1;
               L_temp = MIN_32;
           }
	  else
	   {
	    	L_temp <<= 1;
	    }
        }

    lsp_ele[j] = extract_h(  L_temp );	
    //lsp_ele[j] = extract_h( L_shl( L_temp, 3 ) );

  }
  return;
}
*/

/*
  update previous LSP parameter
*/
void Lsp_prev_update(
  Word16 lsp_ele[M],             /* (i)   Q13 : LSP vectors           */
  Word16 freq_prev[MA_NP][M]     /* (i/o) Q13 : previous LSP vectors  */
)
{
  Word16 k;

  for ( k = MA_NP-1 ; k > 0 ; k-- )
    Copy(freq_prev[k-1], freq_prev[k], M);

  Copy(lsp_ele, freq_prev[0], M);
  return;
}

void Lsp_stability(
  Word16 buf[]       /* (i/o) Q13 : quantized LSP parameters      */
)
{
  Word16 j;
  Word16 tmp;
  Word32 L_diff;
  Word32 L_acc, L_accb;

  for(j=0; j<M-1; j++) {
    //L_acc = L_deposit_l( buf[j+1] );
    L_acc = (Word32) buf[j+1];
    //L_accb = L_deposit_l( buf[j] );
    L_accb = (Word32) buf[j];
    L_diff = L_sub( L_acc, L_accb );

    if( L_diff < 0L ) {
      /* exchange buf[j]<->buf[j+1] */
      tmp      = buf[j+1];
      buf[j+1] = buf[j];
      buf[j]   = tmp;
    }
  }


  if( sub(buf[0], L_LIMIT) <0 ) {
    buf[0] = L_LIMIT;
    printf("lsp_stability warning Low \n");
  }
  for(j=0; j<M-1; j++) {
    //L_acc = L_deposit_l( buf[j+1] );
    //L_accb = L_deposit_l( buf[j] );
    L_acc = (Word32) buf[j+1];
    L_accb = (Word32) buf[j];
    L_diff = L_sub( L_acc, L_accb );

    if( L_sub(L_diff, GAP3)<0L ) {
      buf[j+1] = add( buf[j], GAP3 );
    }
  }

  if( sub(buf[M-1],M_LIMIT)>0 ) {
    buf[M-1] = M_LIMIT;
    printf("lsp_stability warning High \n");
  }
  return;
}
