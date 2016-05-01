/*
   ITU-T G.729A Speech Coder with Annex B    ANSI-C Source Code
   Version 1.3    Last modified: August 1997

   Copyright (c) 1996,
   AT&T, France Telecom, NTT, Universite de Sherbrooke, Lucent Technologies
   Rockwell International
   All rights reserved.
*/

/*-------------------------------------------------------------------*
 * Function  Qua_lsp:                                                *
 *           ~~~~~~~~                                                *
 *-------------------------------------------------------------------*/
#include "typedef.h"
#include "basic_op.h"

#include "ld8a.h"
#include "tab_ld8a.h"

void Qua_lsp(
  Word16 lsp[],       /* (i) Q15 : Unquantized LSP            */
  Word16 lsp_q[],     /* (o) Q15 : Quantized LSP              */
  Word16 ana[]        /* (o)     : indexes                    */
)
{
  Word16 lsf[M], lsf_q[M];  /* domain 0.0<= lsf <PI in Q13 */

  /* Convert LSPs to LSFs */
  Lsp_lsf2(lsp, lsf, M);

  Lsp_qua_cs(lsf, lsf_q, ana );

  /* Convert LSFs to LSPs */
  Lsf_lsp2(lsf_q, lsp_q, M);

  return;
}

/* static memory */

static Word16 freq_prev[MA_NP][M];    /* Q13:previous LSP vector       */

void Lsp_encw_reset(
  void
)
{
  Word16 i, j;

  for(i=0; i<MA_NP; i++)
  	//for(j=0; j<M; j++)
		//freq_prev[i][j] = freq_prev_reset[0][j];
    Copy( &freq_prev_reset[0], &freq_prev[i][0], M );
}


void Lsp_qua_cs(
  Word16 flsp_in[M],    /* (i) Q13 : Original LSP parameters    */
  Word16 lspq_out[M],   /* (o) Q13 : Quantized LSP parameters   */
  Word16 *code          /* (o)     : codes of the selected LSP  */
)
{
  Word16 wegt[M];       /* Q11->normalized : weighting coefficients */

  Get_wegt( flsp_in, wegt );

  Relspwed( flsp_in, wegt, lspq_out, lspcb1, lspcb2, fg,
    freq_prev, fg_sum, fg_sum_inv, code);
}

void Relspwed(
  Word16 lsp[],                 /* (i) Q13 : unquantized LSP parameters */
  Word16 wegt[],                /* (i) norm: weighting coefficients     */
  Word16 lspq[],                /* (o) Q13 : quantized LSP parameters   */
  Word16 lspcb1[][M],           /* (i) Q13 : first stage LSP codebook   */
  Word16 lspcb2[][M],           /* (i) Q13 : Second stage LSP codebook  */
  Word16 fg[MODE][MA_NP][M],    /* (i) Q15 : MA prediction coefficients */
  Word16 freq_prev[MA_NP][M],   /* (i) Q13 : previous LSP vector        */
  Word16 fg_sum[MODE][M],       /* (i) Q15 : present MA prediction coef.*/
  Word16 fg_sum_inv[MODE][M],   /* (i) Q12 : inverse coef.              */
  Word16 code_ana[]             /* (o)     : codes of the selected LSP  */
)
{
  Word16 mode, j;
  Word16 index, mode_index;
  Word16 cand[MODE], cand_cur;
  Word16 tindex1[MODE], tindex2[MODE];
  Word32 L_tdist[MODE];         /* Q26 */
  Word16 rbuf[M];               /* Q13 */
  Word16 buf[M];                /* Q13 */

  for(mode = 0; mode<MODE; mode++) {
    Lsp_prev_extract(lsp, rbuf, fg[mode], freq_prev, fg_sum_inv[mode]);

    Lsp_pre_select(rbuf, lspcb1, &cand_cur );
    cand[mode] = cand_cur;

    Lsp_select_1(rbuf, lspcb1[cand_cur], wegt, lspcb2, &index);

    tindex1[mode] = index;

    for( j = 0 ; j < NC ; j++ )
      buf[j] = add( lspcb1[cand_cur][j], lspcb2[index][j] );

    Lsp_expand_1(buf, GAP1);

    Lsp_select_2(rbuf, lspcb1[cand_cur], wegt, lspcb2, &index);

    tindex2[mode] = index;

    for( j = NC ; j < M ; j++ )
      buf[j] = add( lspcb1[cand_cur][j], lspcb2[index][j] );

    Lsp_expand_2(buf, GAP1);

    Lsp_expand_1_2(buf, GAP2);

    Lsp_get_tdist(wegt, buf, &L_tdist[mode], rbuf, fg_sum[mode]);
  }

  Lsp_last_select(L_tdist, &mode_index);

  code_ana[0] = shl( mode_index,NC0_B ) | cand[mode_index];
  code_ana[1] = shl( tindex1[mode_index],NC1_B ) | tindex2[mode_index];

  Lsp_get_quant(lspcb1, lspcb2, cand[mode_index],
      tindex1[mode_index], tindex2[mode_index],
      fg[mode_index], freq_prev, lspq, fg_sum[mode_index]) ;

  return;
}

/*
void Lsp_pre_select(
  Word16 rbuf[],              
  Word16 lspcb1[][M],       
  Word16 *cand               
)
{
  Word16 i, j;
  Word16 tmp;               
  Word32 L_dmin;            
  Word32 L_tmp;              
  Word32 L_temp;


  *cand = 0;
  L_dmin = MAX_32;
  for ( i = 0 ; i < NC0 ; i++ ) {
    L_tmp = 0;
    for ( j = 0 ; j < M ; j++ ) {
      tmp = sub(rbuf[j], lspcb1[i][j]);
      L_tmp = L_mac( L_tmp, tmp, tmp );
    }

    L_temp = L_sub(L_tmp,L_dmin);
    if (  L_temp< 0L) {
      L_dmin = L_tmp;
      *cand = i;
    }
  }
  return;
}

*/
/*
void Lsp_select_1(
  Word16 rbuf[],              
  Word16 lspcb1[],        
  Word16 wegt[],             
  Word16 lspcb2[][M],      
  Word16 *index             
)
{
  Word16 j, k1;
  Word16 buf[M];  
  Word32 L_dist;   
  Word32 L_dmin;  
  Word16 tmp,tmp2;   
  Word32 L_temp;

  for ( j = 0 ; j < NC ; j++ )
    buf[j] = sub(rbuf[j], lspcb1[j]);

                  
  *index = 0;
  L_dmin = MAX_32;
  for ( k1 = 0 ; k1 < NC1 ; k1++ ) {
    L_dist = 0;
    for ( j = 0 ; j < NC ; j++ ) {
      tmp = sub(buf[j], lspcb2[k1][j]);
      tmp2 = mult( wegt[j], tmp );
      L_dist = L_mac( L_dist, tmp2, tmp );
    }

    L_temp =L_sub(L_dist,L_dmin);
    if ( L_temp <0L ) {
      L_dmin = L_dist;
      *index = k1;
    }
  }
  return;
}

*/
/*
void Lsp_select_2(
  Word16 rbuf[],              
  Word16 lspcb1[],         
  Word16 wegt[],            
  Word16 lspcb2[][M],       
  Word16 *index              
)
{
  Word16 j, k1;
  Word16 buf[M];            
  Word32 L_dist;              
  Word32 L_dmin;            
  Word16 tmp,tmp2;          
  Word32 L_temp;

  for ( j = NC ; j < M ; j++ )
    buf[j] = sub(rbuf[j], lspcb1[j]);

                           
  *index = 0;
  L_dmin = MAX_32;
  for ( k1 = 0 ; k1 < NC1 ; k1++ ) {
    L_dist = 0;
    for ( j = NC ; j < M ; j++ ) {
      tmp = sub(buf[j], lspcb2[k1][j]);
      tmp2 = mult( wegt[j], tmp );
      L_dist = L_mac( L_dist, tmp2, tmp );
    }

    L_temp = L_sub(L_dist, L_dmin);
    if ( L_temp <0L ) {
      L_dmin = L_dist;
      *index = k1;
    }
  }
  return;
}
*/
/*

void Lsp_get_tdist(
  Word16 wegt[],       
  Word16 buf[],         
  Word32 *L_tdist,     
  Word16 rbuf[],        
  Word16 fg_sum[]       
)
{
  Word16 j;
  Word16 tmp, tmp2;     
  Word32 L_acc;        

  *L_tdist = 0;
  for ( j = 0 ; j < M ; j++ ) {
  	
    tmp = sub( buf[j], rbuf[j] );
    tmp = mult( tmp, fg_sum[j] );

    L_acc = L_mult( wegt[j], tmp );
    tmp2 = extract_h( L_shl( L_acc, 4 ) );
    *L_tdist = L_mac( *L_tdist, tmp2, tmp );
  }

  return;
}

*/

void Lsp_last_select(
  Word32 L_tdist[],    
  Word16 *mode_index   
)
{
    Word32 L_temp;
  *mode_index = 0;
  L_temp =L_sub(L_tdist[1] ,L_tdist[0]);
  if (  L_temp<0L){
    *mode_index = 1;
  }
  return;
}
/*
void Get_wegt(
  Word16 flsp[],    
  Word16 wegt[]    
)
{
  Word16 i, var2;
  Word16 tmp;
  Word32 L_acc;
  Word16 sft;
  Word16 buf[M]; 


  buf[0] = sub( flsp[1], (PI04+8192) );      

  for ( i = 1 ; i < M-1 ; i++ ) {
    	tmp = sub( flsp[i+1], flsp[i-1] );
    	buf[i] = sub( tmp, 8192 );
  }

  buf[M-1] = sub( (PI92-8192), flsp[M-2] );

  
  for ( i = 0 ; i < M ; i++ ) {
    	if ( buf[i] > 0 ){
		wegt[i] = 2048;                  
	}	
    	else {
      		L_acc = L_mult( buf[i], buf[i] );        

     		 for(var2 = 2; var2>0;var2--)
        	{
        		 if (L_acc > (Word32) 0X3fffffffL)
           		{
           			 //Overflow = 1;
            			L_acc = MAX_32;
           		}
         		else if(L_acc < (Word32) 0xc0000000L)
           		{
              		//Overflow = 1;
               		L_acc = MIN_32;
               	}
		  	else
		   	{
	    			L_acc <<= 1;
	    		}
        	}	  
	  
      		tmp = extract_h(  L_acc );     

      		L_acc = L_mult( tmp, CONST10 );           

      		for(var2 = 2;var2>0;var2--)
        	{
         		if (L_acc > (Word32) 0X3fffffffL)
           		{
            			//Overflow = 1;
            			L_acc = MAX_32;
           		}
         		else if(L_acc < (Word32) 0xc0000000L)
           		{
               		//Overflow = 1;
               		L_acc = MIN_32;
       
             
           		}
	  		else
	   		{
	    			L_acc <<= 1;
	    		}
        	}	  
      		tmp = extract_h(  L_acc );      
	  

      		wegt[i] = add( tmp, 2048 );              
    	}
  }

  L_acc = L_mult( wegt[4], CONST12 );  
//  L_acc <<= 1;

  if (L_acc > (Word32) 0X3fffffffL)
           {
            //Overflow = 1;
            L_acc = MAX_32;
           }
         else if(L_acc < (Word32) 0xc0000000L)
           {
               //Overflow = 1;
               L_acc = MIN_32;             
           }
	  else
	   {
	    	L_acc <<= 1;
	    }

  wegt[4] = extract_h( L_acc ); 
//  L_acc <<= 1;


  L_acc = L_mult( wegt[5], CONST12 );           


         if (L_acc > (Word32) 0X3fffffffL)
           {
            //Overflow = 1;
            L_acc = MAX_32;
           }
         else if(L_acc < (Word32) 0xc0000000L)
           {
               //Overflow = 1;
               L_acc = MIN_32;             
           }
	  else
	   {
	    	L_acc <<= 1;
	    }

  wegt[5] = extract_h(  L_acc ); 
  
  tmp = 0;
  for ( i = 0; i < M; i++ ) {
   // if ( sub(wegt[i], tmp) > 0 ) {
   if ( wegt[i] > tmp ) {
      tmp = wegt[i];
    }
  }

  sft = norm_s(tmp);
  for ( i = 0; i < M; i++ ) {
    wegt[i] = shl(wegt[i], sft);              
  }

  return;
}
*/

void Get_freq_prev(Word16 x[MA_NP][M])
{
  Word16 i, j;

  for (i=0; i<MA_NP; i++)
    //Copy(&freq_prev[i][0], &x[i][0], M);
       for (j = 0; j < M; j++)
     		x[i][j] = freq_prev[i][j];
}
  
void Update_freq_prev(Word16 x[MA_NP][M])
{
  Word16 i, j;

  for (i=0; i<MA_NP; i++)
    //Copy(&x[i][0], &freq_prev[i][0], M);
    for (j = 0; j < M; j++)
     		freq_prev[i][j] = x[i][j];
}
  



