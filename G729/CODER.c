/*
   ITU-T G.729A Speech Coder with Annex B    ANSI-C Source Code
   Version 1.3    Last modified: August 1997

   Copyright (c) 1996,
   AT&T, France Telecom, NTT, Universite de Sherbrooke, Lucent Technologies,
   Rockwell International
   All rights reserved.
*/

/*-------------------------------------------------------------------*
 * Main program of the ITU-T G.729A  8 kbit/s encoder.               *
 *                                                                   *
 *    Usage : coder speech_file  bitstream_file                      *
 *-------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>

#include "typedef.h"
#include "basic_op.h"
#include "ld8a.h"
#include "dtx.h"
#include "octet.h"


#include "vad.h"
#include "dtx.h"
#include "sid.h"
#include "oper_32b.h"
#include "tab_ld8a.h"
/*  wg add */
#include "tab_dtx.h"
#include "vad_test.h"
#include "G729.h"

short  y2_hi, y2_lo, y1_hi, y1_lo, x0_11, x1_12;

/* Initialization of static values */




/*---------------------------------------------------------------------------*
 * Function  Qua_gain                                                        *
 * ~~~~~~~~~~~~~~~~~~                                                        *
 * Inputs:                                                                   *
 *   code[]     :Innovative codebook.                                        *
 *   g_coeff[]  :Correlations compute for pitch.                             *
 *   L_subfr    :Subframe length.                                            *
 *                                                                           *
 * Outputs:                                                                  *
 *   gain_pit   :Quantized pitch gain.                                       *
 *   gain_cod   :Quantized code gain.                                        *
 *                                                                           *
 * Return:                                                                   *
 *   Index of quantization.                                                  *
 *                                                                           *
 *--------------------------------------------------------------------------*/
Word16 Qua_gain(
   Word16 code[],       /* (i) Q13 :Innovative vector.             */
   Word16 g_coeff[],    /* (i)     :Correlations <xn y1> -2<y1 y1> */
                        /*            <y2,y2>, -2<xn,y2>, 2<y1,y2> */
   Word16 exp_coeff[],  /* (i)     :Q-Format g_coeff[]             */
   Word16 L_subfr,      /* (i)     :Subframe length.               */
   Word16 *gain_pit,    /* (o) Q14 :Pitch gain.                    */
   Word16 *gain_cod,    /* (o) Q1  :Code gain.                     */
   Word16 tameflag      /* (i)     : set to 1 if taming is needed  */
)
{
   Word16  i, j, index1, index2;
   Word16  cand1, cand2;
   Word16  exp, gcode0, exp_gcode0, gcode0_org, e_min ;
   Word16  nume, denom, inv_denom;
   Word16  exp1,exp2,exp_nume,exp_denom,exp_inv_denom,sft,tmp;
   Word16  g_pitch, g2_pitch, g_code, g2_code, g_pit_cod;
   Word16  coeff[5], coeff_lsf[5];
   Word16  exp_min[5];
   Word32  L_gbk12;
   Word32  L_tmp, L_dist_min, L_temp, L_tmp1, L_tmp2, L_acc, L_accb;
   Word16  best_gain[2];

        /* Gain predictor, Past quantized energies = -14.0 in Q10 */

 

  /*---------------------------------------------------*
   *-  energy due to innovation                       -*
   *-  predicted energy                               -*
   *-  predicted codebook gain => gcode0[exp_gcode0]  -*
   *---------------------------------------------------*/

   Gain_predict( past_qua_en_wg, code, L_subfr, &gcode0, &exp_gcode0 );

   L_tmp1 = L_mult( g_coeff[0], g_coeff[2] );
   exp1   = add( add( exp_coeff[0], exp_coeff[2] ), 1-2 );
   L_tmp2 = L_mult( g_coeff[4], g_coeff[4] );
   exp2   = add( add( exp_coeff[4], exp_coeff[4] ), 1 );

   if( sub(exp1, exp2)>0 ){
      L_tmp = L_sub( L_shr( L_tmp1, sub(exp1,exp2) ), L_tmp2 );
      exp = exp2;
   }
   else{
      L_tmp = L_sub( L_tmp1, L_shr( L_tmp2, sub(exp2,exp1) ) );
      exp = exp1;
   }
   sft = norm_l( L_tmp );
   denom = extract_h( L_shl(L_tmp, sft) );
   exp_denom = sub( add( exp, sft ), 16 );

   inv_denom = div_s(16384,denom);
   inv_denom = negate( inv_denom );
   exp_inv_denom = sub( 14+15, exp_denom );

   L_tmp1 = L_mult( g_coeff[2], g_coeff[1] );
   exp1   = add( exp_coeff[2], exp_coeff[1] );
   L_tmp2 = L_mult( g_coeff[3], g_coeff[4] );
   exp2   = add( add( exp_coeff[3], exp_coeff[4] ), 1 );

   if( sub(exp1, exp2)>0 ){
      L_tmp = L_sub( L_shr( L_tmp1, add(sub(exp1,exp2),1 )), L_shr( L_tmp2,1 ) );
      exp = sub(exp2,1);
   }
   else{
      L_tmp = L_sub( L_shr( L_tmp1,1 ), L_shr( L_tmp2, add(sub(exp2,exp1),1 )) );
      exp = sub(exp1,1);
   }
   sft = norm_l( L_tmp );
   nume = extract_h( L_shl(L_tmp, sft) );
   exp_nume = sub( add( exp, sft ), 16 );

   sft = sub( add( exp_nume, exp_inv_denom ), (9+16-1) );
   L_acc = L_shr( L_mult( nume,inv_denom ), sft );
   best_gain[0] = extract_h( L_acc );            

   if (tameflag == 1){
     if(sub(best_gain[0], GPCLIP2) > 0) best_gain[0] = GPCLIP2;
   }

   L_tmp1 = L_mult( g_coeff[0], g_coeff[3] );
   exp1   = add( exp_coeff[0], exp_coeff[3] ) ;
   L_tmp2 = L_mult( g_coeff[1], g_coeff[4] );
   exp2   = add( add( exp_coeff[1], exp_coeff[4] ), 1 );

   if( sub(exp1, exp2)>0 ){
      L_tmp = L_sub( L_shr( L_tmp1, add(sub(exp1,exp2),1) ), L_shr( L_tmp2,1 ) );
      exp = sub(exp2,1);
   }
   else{
      L_tmp = L_sub( L_shr( L_tmp1,1 ), L_shr( L_tmp2, add(sub(exp2,exp1),1) ) );
      exp = sub(exp1,1);
   }
   sft = norm_l( L_tmp );
   nume = extract_h( L_shl(L_tmp, sft) );
   exp_nume = sub( add( exp, sft ), 16 );

   sft = sub( add( exp_nume, exp_inv_denom ), (2+16-1) );
   L_acc = L_shr( L_mult( nume,inv_denom ), sft );
   best_gain[1] = extract_h( L_acc );             

   if( sub(exp_gcode0,4) >= 0 ){
      gcode0_org = shr( gcode0, sub(exp_gcode0,4) );
   }
   else{
      L_acc = L_deposit_l( gcode0 );
      L_acc = L_shl( L_acc, sub( (4+16), exp_gcode0 ) );
      gcode0_org = extract_h( L_acc );             
   }

 

   Gbk_presel(best_gain, &cand1, &cand2, gcode0_org );


   exp_min[0] = add( exp_coeff[0], 13 );
   exp_min[1] = add( exp_coeff[1], 14 );
   exp_min[2] = add( exp_coeff[2], sub( shl( exp_gcode0, 1 ), 21 ) );
   exp_min[3] = add( exp_coeff[3], sub( exp_gcode0, 3 ) );
   exp_min[4] = add( exp_coeff[4], sub( exp_gcode0, 4 ) );

   e_min = exp_min[0];
   for(i=1; i<5; i++){
      if( sub(exp_min[i], e_min) < 0 ){
         e_min = exp_min[i];
      }
   }
  

   for(i=0; i<5; i++){
     j = sub( exp_min[i], e_min );
     L_tmp = L_deposit_h( g_coeff[i] );
     L_tmp = L_shr( L_tmp, j );          
     L_Extract( L_tmp, &coeff[i], &coeff_lsf[i] );         
   }


   L_dist_min = MAX_32;

   index1 = cand1;
   index2 = cand2;

if(tameflag == 1){
   for(i=0; i<NCAN1; i++){
      for(j=0; j<NCAN2; j++){
         g_pitch = add( gbk1[cand1+i][0], gbk2[cand2+j][0] );     
         if(g_pitch < GP0999) {
         L_acc = L_deposit_l( gbk1[cand1+i][1] );
         L_accb = L_deposit_l( gbk2[cand2+j][1] );                
         L_tmp = L_add( L_acc,L_accb );
         tmp = extract_l( L_shr( L_tmp,1 ) );                     

         g_code   = mult( gcode0, tmp );       
         g2_pitch = mult(g_pitch, g_pitch);    
         g2_code  = mult(g_code,  g_code);     
         g_pit_cod= mult(g_code,  g_pitch);     

         L_tmp = Mpy_32_16(coeff[0], coeff_lsf[0], g2_pitch);
         L_tmp = L_add(L_tmp, Mpy_32_16(coeff[1], coeff_lsf[1], g_pitch) );
         L_tmp = L_add(L_tmp, Mpy_32_16(coeff[2], coeff_lsf[2], g2_code) );
         L_tmp = L_add(L_tmp, Mpy_32_16(coeff[3], coeff_lsf[3], g_code) );
         L_tmp = L_add(L_tmp, Mpy_32_16(coeff[4], coeff_lsf[4], g_pit_cod) );

         L_temp = L_sub(L_tmp, L_dist_min);

         if( L_temp < 0L ){
            L_dist_min = L_tmp;
            index1 = add(cand1,i);
            index2 = add(cand2,j);
         }
        }
      }
   }

}
else{
   for(i=0; i<NCAN1; i++){
      for(j=0; j<NCAN2; j++){
         g_pitch = add( gbk1[cand1+i][0], gbk2[cand2+j][0] );    
         L_acc = L_deposit_l( gbk1[cand1+i][1] );
         L_accb = L_deposit_l( gbk2[cand2+j][1] );               
         L_tmp = L_add( L_acc,L_accb );
         tmp = extract_l( L_shr( L_tmp,1 ) );                   

         g_code   = mult( gcode0, tmp );         
         g2_pitch = mult(g_pitch, g_pitch);        
         g2_code  = mult(g_code,  g_code);    
         g_pit_cod= mult(g_code,  g_pitch);    

         L_tmp = Mpy_32_16(coeff[0], coeff_lsf[0], g2_pitch);
         L_tmp = L_add(L_tmp, Mpy_32_16(coeff[1], coeff_lsf[1], g_pitch) );
         L_tmp = L_add(L_tmp, Mpy_32_16(coeff[2], coeff_lsf[2], g2_code) );
         L_tmp = L_add(L_tmp, Mpy_32_16(coeff[3], coeff_lsf[3], g_code) );
         L_tmp = L_add(L_tmp, Mpy_32_16(coeff[4], coeff_lsf[4], g_pit_cod) );

         L_temp = L_sub(L_tmp, L_dist_min);

         if( L_temp < 0L ){
            L_dist_min = L_tmp;
            index1 = add(cand1,i);
            index2 = add(cand2,j);
         }

      }
   }
}
   
   *gain_pit = add( gbk1[index1][0], gbk2[index2][0] );    

   L_acc = L_deposit_l( gbk1[index1][1] );
   L_accb = L_deposit_l( gbk2[index2][1] );
   L_gbk12 = L_add( L_acc, L_accb );                          
   tmp = extract_l( L_shr( L_gbk12,1 ) );                   
   L_acc = L_mult(tmp, gcode0);              

   L_acc = L_shl(L_acc, add( negate(exp_gcode0),(-12-1+1+16) ));
   *gain_cod = extract_h( L_acc );                        

 
   Gain_update( past_qua_en_wg, L_gbk12 );

   return( add( map1[index1]*(Word16)NCODE2, map2[index2] ) );

}
/*---------------------------------------------------------------------------*
 * Function Gbk_presel                                                       *
 * ~~~~~~~~~~~~~~~~~~~                                                       *
 *   - presearch for gain codebook -                                         *
 *---------------------------------------------------------------------------*/
void Gbk_presel(
   Word16 best_gain[],     
   Word16 *cand1,          
   Word16 *cand2,         
   Word16 gcode0          
)
{
   	Word16    acc_h;
   	Word16    sft_x,sft_y;
   	Word32    L_acc,L_preg,L_cfbg,L_tmp,L_tmp_x,L_tmp_y;
   	Word32 	  L_temp;


	L_cfbg = L_mult( coef[0][0], best_gain[0] );        
   	//L_acc = L_shr( L_coef[1][1], 15 );                  
   	L_acc = ( L_coef[1][1] >> 15 );
   	L_acc = L_add( L_cfbg , L_acc );
   	acc_h = extract_h( L_acc );                         
   	L_preg = L_mult( acc_h, gcode0 );                  
   	L_acc = L_shl( L_deposit_l( best_gain[1] ), 7 );    
   	L_acc = L_sub( L_acc, L_preg );
   	acc_h = extract_h( L_shl( L_acc,2 ) );             
   	L_tmp_x = L_mult( acc_h, INV_COEF );               


   	//L_acc = L_shr( L_coef[0][1], 10 );                 
   	L_acc = ( L_coef[0][1] >> 10 );
   	L_acc = L_sub( L_cfbg, L_acc );                     
   	acc_h = extract_h( L_acc );                        
   	acc_h = mult( acc_h, gcode0 );                      
   	L_tmp = L_mult( acc_h, coef[1][0] );               

   	L_preg = L_mult( coef[0][0], best_gain[1] );        
   	L_acc = L_sub( L_tmp, L_shr(L_preg,3) );            

   	acc_h = extract_h( L_shl( L_acc,2 ) );              
   	L_tmp_y = L_mult( acc_h, INV_COEF );               

   	sft_y = 3;         
   	sft_x = 5;         

   	if(gcode0>0){
      	
	      	*cand1 = 0 ;
	      	do{
	         	L_temp = L_sub( L_tmp_y, L_shr(L_mult(thr1[*cand1],gcode0),sft_y));
	         	if(L_temp >0L  ){
	        		(*cand1) =add(*cand1,1);
	     		}
	         	else               break ;
	      	} while(sub((*cand1),(NCODE1-NCAN1))<0) ;
	      	
	      	*cand2 = 0 ;
	      	do{
	        	L_temp = L_sub( L_tmp_x , L_shr(L_mult(thr2[*cand2],gcode0),sft_x));
	         	if( L_temp >0L) {
	        		(*cand2) =add(*cand2,1);
	     		}
	         	else               break ;
	      	} while(sub((*cand2),(NCODE2-NCAN2))<0) ;
   	}
   	else{
     
      		*cand1 = 0 ;
      		do{
        		L_temp = L_sub(L_tmp_y ,L_shr(L_mult(thr1[*cand1],gcode0),sft_y));
         		if( L_temp <0L){
        			(*cand1) =add(*cand1,1);
     			}
         		else               break ;
      		} while(sub((*cand1),(NCODE1-NCAN1))) ;
      		
      		*cand2 = 0 ;
      		do{
         		L_temp =L_sub(L_tmp_x ,L_shr(L_mult(thr2[*cand2],gcode0),sft_x));
         		if( L_temp <0L){
        			(*cand2) =add(*cand2,1);
     			}
         		else               break ;
      		} while(sub( (*cand2),(NCODE2-NCAN2))) ;
   	}

   	return ;
}


void Init_Pre_Process(void)
{
  y2_hi = 0;
  y2_lo = 0;
  y1_hi = 0;
  y1_lo = 0;
  x0_11   = 0;
  x1_12   = 0;
}



/*---------------------------------------------------------------------------*
 * Function  vad_init                                                        *
 * ~~~~~~~~~~~~~~~~~~                                                        *
 *                                                                           *
 * -> Initialization of variables for voice activity detection               *
 *                                                                           *
 *---------------------------------------------------------------------------*/
void vad_init(void)
{
  /* Static vectors to zero */
  Set_zero(MeanLSF, M);

  /* Initialize VAD parameters */
  MeanSE = 0;
  MeanSLE = 0;
  MeanE = 0;
  MeanSZC = 0;
  count_sil = 0;
  count_update = 0;
  count_ext = 0;
  less_count = 0;
  wg_flag = 1;
  Min = MAX_16;
}




void Init_Coder_ld8a(void)
{

  /*----------------------------------------------------------------------*
  *      Initialize pointers to speech vector.                            *
  *                                                                       *
  *                                                                       *
  *   |--------------------|-------------|-------------|------------|     *
  *     previous speech           sf1           sf2         L_NEXT        *
  *                                                                       *
  *   <----------------  Total speech vector (L_TOTAL)   ----------->     *
  *   <----------------  LPC analysis window (L_WINDOW)  ----------->     *
  *   |                   <-- present frame (L_FRAME) -->                 *
  * old_speech            |              <-- new speech (L_FRAME) -->     *
  * p_window              |              |                                *
  *                     speech           |                                *
  *                             new_speech                                *
  *-----------------------------------------------------------------------*/

  new_speech = old_speech + L_TOTAL - L_FRAME;         /* New speech     */
  speech     = new_speech - L_NEXT;                    /* Present frame  */
  p_window   = old_speech + L_TOTAL - L_WINDOW;        /* For LPC window */

  /* Initialize static pointers */

  wsp    = old_wsp + PIT_MAX;
  udi_exc    = en_old_exc + PIT_MAX + L_INTERPOL;

  /* Static vectors to zero */

  Set_zero(old_speech, L_TOTAL);
  Set_zero(en_old_exc, PIT_MAX+L_INTERPOL);
  Set_zero(old_wsp, PIT_MAX);
  Set_zero(mem_w,   M);
  Set_zero(mem_w0,  M);
  Set_zero(mem_zero, M);
  en_sharp = SHARPMIN;

  /* Initialize en_lsp_old_q[] */

  Copy(en_lsp_old, en_lsp_old_q, M);
  Lsp_encw_reset();
  Init_exc_err();

  /* For G.729B */
  /* Initialize VAD/DTX parameters */
  en_pastVad = 1;
  ppastVad = 1;
  en_seed = INIT_SEED;
  vad_init();
  Init_lsfq_noise();

  return;
}




void G729Encoder(char *Inspeech, int N, char *outbit, char *decodeoutput  )
{

  	Word16 frame, framecount;                  /* frame counter */
  	Word16 HighByteSi,LowByteSi;
  	char ref3[20] ;
  	Word32 count_frame;
  
  	int i, j, z, count1, count2, count4, count3;
	unsigned char pCodeBuf[10]; 
	Word16 new_speech_wg[L_FRAME];
	unsigned char g729decodespeech[10];
	unsigned short g729decodeoutputdata[80], g729testdata[80];
  	ULONG count5;	


	count5 = 0;
	
	for(z=0;z<20;z++)
	{
		ref3[z] = 0;
	}

	AppAudioG729EncodeInit();
	AppAudioG729DecodeInit();



  	frame = 0;
  	count_frame = 0L;
  	count3 = 0;
  	count4 = 0;
  
	  for(framecount=0; framecount<N; framecount++)
	  {
	  	
		if (frame == 32767) frame = 256;
    		else frame++;
	    	for(i=0;i<80;i++)
	    	{
			LowByteSi = (short)Inspeech[160*framecount + 2*i];			
	 		HighByteSi = (short)Inspeech[160*framecount + 2*i + 1];
			HighByteSi = HighByteSi << 8;
			HighByteSi = (HighByteSi | LowByteSi) & 0xffff;
			new_speech_wg[i] = HighByteSi;
	    	}

		for(i=0;i<10;i++)
	    	{
			g729decodespeech[i] = outbit[count5 + i];
	    	}
		
	    	count1 = 0;
	    	count2 = 0;    
	    	count1 = GetCount();    

		AppAudioG729Encode(new_speech_wg, pCodeBuf )	;


		AppAudioG729Decode(g729decodespeech, g729decodeoutputdata, 10);
		
	    	count2 = GetCount();
	    	count4 = count2 - count1;
	    	  	
  	
	    	if(count4 >= count3) count3 = count4; 		
	    	
	    	for(i=0;i<10;i++)
	    	{	
			ref3[i] = pCodeBuf[i] ;	
	    	} 

		for(i=0;i<80;i++)
	    	{
			LowByteSi = (short)decodeoutput[160*framecount + 2*i];			
	 		HighByteSi = (short)decodeoutput[160*framecount + 2*i + 1];
			HighByteSi = HighByteSi << 8;
			HighByteSi = (HighByteSi | LowByteSi) & 0xffff;
			g729testdata[i] = HighByteSi;
	    	}
		
	    	
		for(i = 0; i<10; i++)
	  	{
	  		if(ref3[i] == outbit[count5 + i])
	  		{
	  			printf("W");
	  		}
	  		else
	  		{
	  			printf("\r\n");
	  			printf("Encode It is error!");
	  			printf("\r\n");
	  		}	  		
	  	} 

		
		for(i = 0; i<80; i++)
	  	{
	  		if(g729decodeoutputdata[i] == g729testdata[ i])
	  		{
	  			printf("W");
	  		}
	  		else
	  		{
	  			printf("\r\n");
	  			printf("Decode It is error!");
	  			printf("\r\n");
	  		}	  		
	  	}

	
	  	count5 = count5 + 10;
	    	
  	}

  	printf("\r\n");
  	printf("Encode Time = %d\r\n", count3 );
  	printf("\r\n");
}

