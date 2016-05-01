/*
***************************************************************************
*                  Copyright (c) 2006 vBridge Microsystem, Inc.  
*                       Unpublished & Not for Publication
*                               All Rights Reserved                   
*   
* File        : Coder_ld8a.S                     
*                                                                         
* Description:    
*             
* Date        : Jun 7, 2006                                                           
***************************************************************************
*/


/*
void Coder_ld8a(
     Word16 ana[],       // output  : Analysis parameters 
     Word16 frame,       // input   : frame counter       
     Word16 vad_enable   // input   : VAD enable flag     
)
{

  // LPC analysis 

  Word16 Aq_t[(MP1)*2];         // A(z)   quantized for the 2 subframes 
  Word16 Ap_t[(MP1)*2];         // A(z/gamma)       for the 2 subframes 
  Word16 *Aq, *Ap;              // Pointer on Aq_t and Ap_t             

  // Other vectors 

  Word16 h1[L_SUBFR];            // Impulse response h1[]              
  Word16 xn[L_SUBFR];            // Target vector for pitch search     
  Word16 xn2[L_SUBFR];           // Target vector for codebook search  
  Word16 code[L_SUBFR];          // Fixed codebook excitation          
  Word16 y1[L_SUBFR];            // Filtered adaptive excitation       
  Word16 y2[L_SUBFR];            // Filtered fixed codebook excitation 
  Word16 g_coeff[4];             // Correlations between xn & y1       

  Word16 g_coeff_cs[5];
  Word16 exp_g_coeff_cs[5];      
  // Correlations between xn, y1, & y2 <y1,y1>, -2<xn,y1>, <y2,y2>, -2<xn,y2>, 2<y1,y2> 

  // Scalars 

  Word16 i, j, k, i_subfr;
  Word16 T_op, T0, T0_min, T0_max, T0_frac;
  Word16 gain_pit, gain_code, index;
  Word16 temp, taming;
  Word32 L_temp;

//------------------------------------------------------------------------*
// *  - Perform LPC analysis:                                               *
// *       * autocorrelation + lag windowing                                *
// *       * Levinson-durbin algorithm to find a[]                          *
// *       * convert a[] to lsp[]                                           *
// *       * quantize and code the LSPs                                     *
// *       * find the interpolated LSPs and convert to a[] for the 2        *
// *         subframes (both quantized and unquantized)                     *
// *------------------------------------------------------------------------
  {
     // Temporary vectors 
    Word16 r_l[NP+1], r_h[NP+1];     // Autocorrelations low and hi          
    Word16 rc[M];                    // Reflection coefficients.             
    Word16 lsp_new[M], lsp_new_q[M]; // LSPs at 2th subframe                 

    // For G.729B 
    Word16 rh_nbe[MP1];
    Word16 lsf_new[M];
    Word16 lsfq_mem[MA_NP][M];
    Word16 exp_R0, Vad;

    // LP analysis 
    Autocorr(p_window, NP, r_h, r_l, &exp_R0);     // Autocorrelations 
    Copy(r_h, rh_nbe, MP1);
    Lag_window(NP, r_h, r_l);                      // Lag windowing    
    Levinson(r_h, r_l, Ap_t, rc, &temp);          // Levinson Durbin  
    Az_lsp(Ap_t, lsp_new, lsp_old);               // From A(z) to lsp 

    // For G.729B 
    // ------ VAD ------- 
    Lsp_lsf(lsp_new, lsf_new, M);
    vad(rc[1], lsf_new, r_h, r_l, exp_R0, p_window, frame,
        pastVad, ppastVad, &Vad);

    Update_cng(rh_nbe, exp_R0, Vad);

    // ---------------------- 
    // Case of Inactive frame 
    // ---------------------- 

    if ((Vad == 0) && (vad_enable == 1)){

      Get_freq_prev(lsfq_mem);
      Cod_cng(exc, pastVad, lsp_old_q, Aq_t, ana, lsfq_mem, &seed);
      Update_freq_prev(lsfq_mem);
      ppastVad = pastVad;
      pastVad = Vad;

      // Update wsp, mem_w and mem_w0 
      Aq = Aq_t;
      for(i_subfr=0; i_subfr < L_FRAME; i_subfr += L_SUBFR) {

        // Residual signal in xn 
        Residu(Aq, &speech[i_subfr], xn, L_SUBFR);

        Weight_Az(Aq, GAMMA1, M, Ap_t);

        // Compute wsp and mem_w 
        Ap = Ap_t + MP1;
        Ap[0] = 4096;
        for(i=1; i<=M; i++)    // Ap[i] = Ap_t[i] - 0.7 * Ap_t[i-1]; 
          Ap[i] = sub(Ap_t[i], mult(Ap_t[i-1], 22938));
        Syn_filt(Ap, xn, &wsp[i_subfr], L_SUBFR, mem_w, 1);

        // Compute mem_w0 
        for(i=0; i<L_SUBFR; i++) {
          xn[i] = sub(xn[i], exc[i_subfr+i]);  // residu[] - exc[] 
        }
        Syn_filt(Ap_t, xn, xn, L_SUBFR, mem_w0, 1);

        Aq += MP1;
      }


      sharp = SHARPMIN;

      // Update memories for next frames 
      Copy(&old_speech[L_FRAME], &old_speech[0], L_TOTAL-L_FRAME);
      Copy(&old_wsp[L_FRAME], &old_wsp[0], PIT_MAX);
      Copy(&old_exc[L_FRAME], &old_exc[0], PIT_MAX+L_INTERPOL);

      return;
    }  // End of inactive frame case 



    // -------------------- 
    // Case of Active frame 
    // -------------------- 

    // Case of active frame 
    *ana++ = 1;
    seed = INIT_SEED;
    ppastVad = pastVad;
    pastVad = Vad;

    // LSP quantization 
    Qua_lsp(lsp_new, lsp_new_q, ana);
    ana += 2;                         // Advance analysis parameters pointer 

    //--------------------------------------------------------------------*
     // Find interpolated LPC parameters in all subframes                  *
     // The interpolated parameters are in array Aq_t[].                   *
     //--------------------------------------------------------------------

    Int_qlpc(lsp_old_q, lsp_new_q, Aq_t);

    // Compute A(z/gamma) 

    Weight_Az(&Aq_t[0],   GAMMA1, M, &Ap_t[0]);
    Weight_Az(&Aq_t[MP1], GAMMA1, M, &Ap_t[MP1]);

    // update the LSPs for the next frame 

    Copy(lsp_new,   lsp_old,   M);
    Copy(lsp_new_q, lsp_old_q, M);
  }

 //----------------------------------------------------------------------*
  // - Find the weighted input speech w_sp[] for the whole speech frame   *
  // - Find the open-loop pitch delay                                     *
  //----------------------------------------------------------------------

  Residu(&Aq_t[0], &speech[0], &exc[0], L_SUBFR);
  Residu(&Aq_t[MP1], &speech[L_SUBFR], &exc[L_SUBFR], L_SUBFR);

  {
    Word16 Ap1[MP1];

    Ap = Ap_t;
    Ap1[0] = 4096;
    for(i=1; i<=M; i++)    // Ap1[i] = Ap[i] - 0.7 * Ap[i-1]; 
       Ap1[i] = sub(Ap[i], mult(Ap[i-1], 22938));
    Syn_filt(Ap1, &exc[0], &wsp[0], L_SUBFR, mem_w, 1);

    Ap += MP1;
    for(i=1; i<=M; i++)    // Ap1[i] = Ap[i] - 0.7 * Ap[i-1]; 
       Ap1[i] = sub(Ap[i], mult(Ap[i-1], 22938));
    Syn_filt(Ap1, &exc[L_SUBFR], &wsp[L_SUBFR], L_SUBFR, mem_w, 1);
  }

  // Find open loop pitch lag 

  T_op = Pitch_ol_fast(wsp, PIT_MAX, L_FRAME);

  // Range for closed loop pitch search in 1st subframe 

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


 //------------------------------------------------------------------------*
  //          Loop for every subframe in the analysis frame                 *
  //------------------------------------------------------------------------*
  //  To find the pitch and innovation parameters. The subframe size is     *
  //  L_SUBFR and the loop is repeated 2 times.                             *
  //     - find the weighted LPC coefficients                               *
  //     - find the LPC residual signal res[]                               *
  //     - compute the target signal for pitch search                       *
  //     - compute impulse response of weighted synthesis filter (h1[])     *
  //     - find the closed-loop pitch parameters                            *
  //     - encode the pitch delay                                           *
  //     - find target vector for codebook search                           *
  //     - codebook search                                                  *
  //     - VQ of pitch and codebook gains                                   *
  //     - update states of weighting filter                                *
  //------------------------------------------------------------------------

  Aq = Aq_t;    // pointer to interpolated quantized LPC parameters 
  Ap = Ap_t;    // pointer to weighted LPC coefficients             

  for (i_subfr = 0;  i_subfr < L_FRAME; i_subfr += L_SUBFR)
  {

    //---------------------------------------------------------------*
     // Compute impulse response, h1[], of weighted synthesis filter  *
     //---------------------------------------------------------------

    h1[0] = 4096;
    Set_zero(&h1[1], L_SUBFR-1);
    Syn_filt(Ap, h1, h1, L_SUBFR, &h1[1], 0);

   //----------------------------------------------------------------------*
    //  Find the target vector for pitch search:                            *
    //----------------------------------------------------------------------

    Syn_filt(Ap, &exc[i_subfr], xn, L_SUBFR, mem_w0, 0);

    //---------------------------------------------------------------------*
     //                 Closed-loop fractional pitch search                 *
     //---------------------------------------------------------------------

    T0 = Pitch_fr3_fast(&exc[i_subfr], xn, h1, L_SUBFR, T0_min, T0_max,
                    i_subfr, &T0_frac);

    index = Enc_lag3(T0, T0_frac, &T0_min, &T0_max,PIT_MIN,PIT_MAX,i_subfr);

    *ana++ = index;

    if (i_subfr == 0) {
      *ana++ = Parity_Pitch(index);
    }

   //-----------------------------------------------------------------*
    //   - find filtered pitch exc                                     *
    //   - compute pitch gain and limit between 0 and 1.2              *
    //   - update target vector for codebook search                    *
    //-----------------------------------------------------------------

    Syn_filt(Ap, &exc[i_subfr], y1, L_SUBFR, mem_zero, 0);

    gain_pit = G_pitch(xn, y1, g_coeff, L_SUBFR);

    // clip pitch gain if taming is necessary 

    taming = test_err(T0, T0_frac);

    if( taming == 1){
      if (sub(gain_pit, GPCLIP) > 0) {
        gain_pit = GPCLIP;
      }
    }

    // xn2[i]   = xn[i] - y1[i] * gain_pit  

    for (i = 0; i < L_SUBFR; i++)
    {
      L_temp = L_mult(y1[i], gain_pit);
      L_temp = L_shl(L_temp, 1);               // gain_pit in Q14 
      xn2[i] = sub(xn[i], extract_h(L_temp));
    }


   //-----------------------------------------------------*
    // - Innovative codebook search.                       *
    //-----------------------------------------------------

    index = ACELP_Code_A(xn2, h1, T0, sharp, code, y2, &i);

    *ana++ = index;        // Positions index 
    *ana++ = i;            // Signs index     


   //-----------------------------------------------------*
    // - Quantization of gains.                            *
    //-----------------------------------------------------

    g_coeff_cs[0]     = g_coeff[0];            // <y1,y1> 
    exp_g_coeff_cs[0] = negate(g_coeff[1]);    // Q-Format:XXX -> JPN 
    g_coeff_cs[1]     = negate(g_coeff[2]);    // (xn,y1) -> -2<xn,y1> 
    exp_g_coeff_cs[1] = negate(add(g_coeff[3], 1)); // Q-Format:XXX -> JPN 

    Corr_xy2( xn, y1, y2, g_coeff_cs, exp_g_coeff_cs );  // Q0 Q0 Q12 ^Qx ^Q0 
                         // g_coeff_cs[3]:exp_g_coeff_cs[3] = <y2,y2>   
                         // g_coeff_cs[4]:exp_g_coeff_cs[4] = -2<xn,y2> 
                         // g_coeff_cs[5]:exp_g_coeff_cs[5] = 2<y1,y2>  

    *ana++ = Qua_gain(code, g_coeff_cs, exp_g_coeff_cs,
                         L_SUBFR, &gain_pit, &gain_code, taming);


   //------------------------------------------------------------*
    // - Update pitch sharpening "sharp" with quantized gain_pit  *
    //------------------------------------------------------------

    sharp = gain_pit;
    if (sub(sharp, SHARPMAX) > 0) { sharp = SHARPMAX;         }
    if (sub(sharp, SHARPMIN) < 0) { sharp = SHARPMIN;         }

   //------------------------------------------------------*
    // - Find the total excitation                          *
    // - update filters memories for finding the target     *
    //   vector in the next subframe                        *
    //------------------------------------------------------

    for (i = 0; i < L_SUBFR;  i++)
    {
      // exc[i] = gain_pit*exc[i] + gain_code*code[i]; 
      // exc[i]  in Q0   gain_pit in Q14               
      // code[i] in Q13  gain_cod in Q1                

      L_temp = L_mult(exc[i+i_subfr], gain_pit);
      L_temp = L_mac(L_temp, code[i], gain_code);
      L_temp = L_shl(L_temp, 1);
      exc[i+i_subfr] = ROUND(L_temp);
    }

    update_exc_err(gain_pit, T0);

    for (i = L_SUBFR-M, j = 0; i < L_SUBFR; i++, j++)
    {
      temp       = extract_h(L_shl( L_mult(y1[i], gain_pit),  1) );
      k          = extract_h(L_shl( L_mult(y2[i], gain_code), 2) );
      mem_w0[j]  = sub(xn[i], add(temp, k));
    }

    Aq += MP1;           // interpolated LPC parameters for next subframe 
    Ap += MP1;

  }

 //--------------------------------------------------*
  // Update signal for next frame.                    *
  // -> shift to the left by L_FRAME:                 *
  //     speech[], wsp[] and  exc[]                   *
  //--------------------------------------------------

  Copy(&old_speech[L_FRAME], &old_speech[0], L_TOTAL-L_FRAME);
  Copy(&old_wsp[L_FRAME], &old_wsp[0], PIT_MAX);
  Copy(&old_exc[L_FRAME], &old_exc[0], PIT_MAX+L_INTERPOL);

  return;
}

*/

#include <mips/asm.h>
#include <mips/udi.h>
#include <mips/regdef.h>

#include "mipsudi_inst.h"

      .set noat
      .set noreorder


LEAF( Coder_ld8a )
		addiu	sp,sp,-1008
		sw	s7,996(sp)
		sw	s2,976(sp)
		sw	s1,972(sp)
		addiu	s2,sp,672
		addiu	s1,sp,704
		move	s7,a0
		lw	a0,p_window
		sw	s6,992(sp)
		sw	s4,984(sp)
		sw	s3,980(sp)
		sll	s6,a2,0x10
		sll	s3,a1,0x10
		addiu	v0,sp,936
		li	a1,12
		move	a2,s1
		move	a3,s2
		addiu	s4,sp,40
		sw	ra,1004(sp)
		sw	s8,1000(sp)
		sw	s5,988(sp)
		sw	s0,968(sp)
		sw	v0,16(sp)
		jal	Autocorr
		sra	s3,s3,0x10
		sra	s6,s6,0x10
		move	a1,zero
		move	a0,s4
Coder_ld8a112:	addiu	a1,a1,1
		//sll	a1,a3,0x10
		lhu	v1,664(a0)
		//sra	a1,a1,0x10
		slti	a2,a1,11
		sh	v1,768(a0)
		bnez	a2,Coder_ld8a112
		addiu	a0,a0,2
		
		li	a0,12
		move	a1,s1
		move	a2,s2
		jal	Lag_window
		addiu	s8,sp,88
		addiu	s5,sp,668
		addiu	a3,sp,736
		move	a0,s1
		move	a1,s2
		move	a2,s8
		jal	Levinson
		sw	s5,16(sp)
		la	a2,en_lsp_old
		addiu	s5,sp,760
		move	a0,s8
		move	a1,s5
		jal	Az_lsp
		addiu	s0,sp,832
		move	a0,s5
		move	a1,s0
		jal	Lsp_lsf
		li	a2,10
		lh	t7,936(sp)
		lh	t5,en_pastVad
		lh	t4,ppastVad
		lw	t6,p_window
		lh	a0,738(sp)
		move	a3,s2
		addiu	t3,sp,938
		move	a1,s0
		move	a2,s1
		sw	t7,16(sp)
		sw	t6,20(sp)
		sw	t5,28(sp)
		sw	t4,32(sp)
		sw	t3,36(sp)
		jal	vad_wg
		sw	s3,24(sp)
		lh	a1,936(sp)
		lh	a2,938(sp)
		jal	Update_cng
		addiu	a0,sp,808
		lhu	a3,938(sp)
		xori	t2,s6,0x1
		sltiu	t1,t2,1
		sltiu	t0,a3,1
		and	a0,t0,t1
		beqz	a0,Coder_ld8a868
		li	s6,1		
		addiu	s6,sp,856
		jal	Get_freq_prev
		move	a0,s6
		la	t0,en_pastVad
		lh	a1,0(t0)
		lw	a0,udi_exc
		la	a2,en_lsp_old_q
		addiu	a3,sp,40
		sw	s7,16(sp)
		la	s7,en_seed
		sw	s7,24(sp)
		jal	Cod_cng
		sw	s6,20(sp)
		jal	Update_freq_prev
		move	a0,s6
		lhu	s2,en_pastVad
		lhu	s1,938(sp)
		sh	s2,ppastVad
		sh	s1,en_pastVad
		move	s5,s4
		move	s7,zero
		addiu	s6,sp,216
		move	s3,zero
Coder_ld8a440:	
		lw	v1,speech
		move	a0,s5
		addu	a1,s3,v1
		move	a2,s6
		jal	Residu
		li	a3,40
		li	a2,10
		move	a0,s5
		li	a1,24576
		jal	Weight_Az
		move	a3,s8
		li	a2,4096
		addiu	v0,sp,110
		sh	a2,110(sp)
		li	s2,1
		addiu	s1,v0,2
		addiu	s0,s4,2
		
Coder_ld8a508:	lh	a0,46(s0)
		li	a1,22938
		mpymvrr	acc0,sx0,sy0,shnone,x0,gpra0,y0,gpra1
		satacc	mac0, acc0, round_disable, sat_disable, satp32, 12
		satacc	mac0, acc0, round_disable, sat_enable, satp16, 3
		mvp	gprt1,udi_mode		
		mvp	gprt1,udi_mode
		mvp	gprt1,udi_mode
		mvp	gpra1,udi_mac0acc0lo		
		jal	wg_sub
		lh	a0,48(s0)
		addiu	s2,s2,1
		slti	a1,s2,11
		sh	v0,0(s1)
		addiu	s0,s0,2
		bnez	a1,Coder_ld8a508
		addiu	s1,s1,2	
			
		lw	t3,wsp		
		la	t1,mem_w
		li	t0,1
		addiu	a0,sp,110
		addu	a2,s3,t3
		move	a1,s6
		li	a3,40
		sw	t1,16(sp)
		sw	t0,20(sp)
		jal	Syn_filt
		move	s2,zero
		move	s1,s3
		move	s0,s4
Coder_ld8a620:	
		lw	t8,udi_exc
		lh	a0,176(s0)
		addu	t7,s1,t8
		lh	a1,0(t7)
		jal	wg_sub
		addiu	s1,s1,2
		addiu	s2,s2,1
		slti	t4,s2,40
		sh	v0,176(s0)
		bnez	t4,Coder_ld8a620
		addiu	s0,s0,2
		
		la	ra,mem_w0
		addiu	s1,s7,40
		move	a0,s8
		move	a1,s6
		move	a2,s6
		li	a3,40
		li	s2,1
		sw	ra,16(sp)
		jal	Syn_filt
		sw	s2,20(sp)
		sll	t9,s1,0x10
		sra	s7,t9,0x10
		slti	s0,s7,80
		addiu	s5,s5,22
		bnez	s0,Coder_ld8a440
		addiu	s3,s3,80		
		la	a1,old_speech
		addiu	a0,a1,160		
		li	s4,3277		
		sh	s4,en_sharp
		li	a2,160
		jal	Copy
		nop
		la	a1,old_wsp
		addiu	a0,a1,160
		jal	Copy
		li	a2,143
		la	a1,en_old_exc
		addiu	a0,a1,160	
		jal	Copy
		li	a2,154
		
		lw	ra,1004(sp)
		lw	s8,1000(sp)
		lw	s7,996(sp)
		lw	s6,992(sp)
		lw	s5,988(sp)
		lw	s4,984(sp)
		lw	s3,980(sp)
		lw	s2,976(sp)
		lw	s1,972(sp)
		lw	s0,968(sp)
		jr	ra
		addiu	sp,sp,1008
		
Coder_ld8a868:	sh	s6,0(s7)
		lhu	t0,en_pastVad
		addiu	s1,sp,784
		li	t1,11111	//INIT_SEED		
		addiu	a2,s7,2
		sh	t1,en_seed
		sh	t0,ppastVad
		sh	a3,en_pastVad		
		move	a0,s5
		move	a1,s1		
		jal	Qua_lsp
		nop		
		la	s0,en_lsp_old_q
		move	a0,s0
		move	a1,s1
		jal	Int_qlpc
		addiu	a2,sp,40
		addiu	s2,sp,62
		addiu	a0,sp,40
		li	a1,24576	//GAMMA1
		li	a2,10
		jal	Weight_Az
		move	a3,s8
		addiu	a3,sp,110
		move	a0,s2
		li	a1,24576	//GAMMA1
		jal	Weight_Az
		li	a2,10		
		la	a1,en_lsp_old
		move	a0,s5
		jal	Copy
		li	a2,10
		move	a0,s1
		move	a1,s0
		jal	Copy
		li	a2,10		
		lw	a1,speech		
		lw	a2,udi_exc
		addiu	a0,sp,40
		jal	Residu
		li	a3,40		
		lw	v1,speech		
		lw	a2,udi_exc
		move	a0,s2
		addiu	a1,v1,80
		addiu	a2,a2,80
		li	a3,40
		jal	Residu
		li	s5,4096
		sh	s6,656(sp)
		sh	s5,944(sp)
		addiu	s6,s7,6
		lh	s7,656(sp)
		
Coder_ld8a1084:	
		li	a1,22938
		sll	t9,s7,0x1
		addu	ra,t9,s8
		lh	a0,-2(ra)				
		mpymvrr	acc0,sx0,sy0,shnone,x0,gpra0,y0,gpra1
		satacc	mac0, acc0, round_disable, sat_disable, satp32, 12
		satacc	mac0, acc0, round_disable, sat_enable, satp16, 3
		mvp	gprt1,udi_mode		
		mvp	gprt1,udi_mode
		mvp	gprt1,udi_mode
		mvp	gpra1,udi_mac0acc0lo		
		
		addu	s3,t9,s4
		addu	t6,t9,s8
		jal	wg_sub
		lh	a0,0(t6)
		addiu	s7,s7,1
		slti	t3,s7,11
		bnez	t3,Coder_ld8a1084
		sh	v0,904(s3)
		
		la	s3,mem_w
		addiu	s2,sp,944
		lw	a1,udi_exc
		lw	a2,wsp
		li	s5,1
		move	a0,s2
		li	a3,40
		sw	s3,16(sp)
		sw	s5,20(sp)
		jal	Syn_filt
		addiu	s1,s8,22
		sh	s5,656(sp)
Coder_ld8a1220:	
		li	a1,22938
		sll	t3,s5,0x1
		addu	t4,t3,s1
		lh	a0,-2(t4)				
		mpymvrr	acc0,sx0,sy0,shnone,x0,gpra0,y0,gpra1
		satacc	mac0, acc0, round_disable, sat_disable, satp32, 12
		satacc	mac0, acc0, round_disable, sat_enable, satp16, 3
		mvp	gprt1,udi_mode		
		mvp	gprt1,udi_mode
		mvp	gprt1,udi_mode
		mvp	gpra1,udi_mac0acc0lo		
		addu	s7,t3,s4
		addu	t0,t3,s1
		jal	wg_sub
		lh	a0,0(t0)		
		addiu	s5,s5,1
		slti	s0,s5,11
		bnez	s0,Coder_ld8a1220
		sh	v0,904(s7)
		
		lw	t6,udi_exc		
		lw	s1,wsp
		addiu	a1,t6,80
		move	a0,s2
		li	a3,40
		addiu	a2,s1,80
		li	s2,1
		sw	s3,16(sp)
		jal	Syn_filt
		sw	s2,20(sp)
		
		lw	a0,wsp
		li	a1,143
		jal	Pitch_ol_fast
		li	a2,80
		move	a0,v0
		jal	wg_sub
		li	a1,3
		move	a0,v0
		li	a1,20
		jal	wg_sub
		sh	v0,658(sp)
		bltz	v0,Coder_ld8a2508
		li	s3,20
Coder_ld8a1396:	lh	a0,658(sp)
		jal	wg_add
		li	a1,6
		move	a0,v0
		li	a1,143
		jal	wg_sub
		sh	v0,660(sp)
		blez	v0,Coder_ld8a1452
		li	t7,143
		li	a0,143
		li	a1,6
		jal	wg_sub
		sh	t7,660(sp)
		sh	v0,658(sp)
Coder_ld8a1452:	move	s7,s8
		move	s1,zero
		addiu	s8,sp,216
Coder_ld8a1464:	li	t1,4096
		addiu	a0,sp,138
		li	a1,39
		jal	Set_zero
		sh	t1,136(sp)
		addiu	a1,sp,136
		move	a2,a1
		move	a0,s7
		li	a3,40
		addiu	s0,sp,138
		sw	s0,16(sp)
		jal	Syn_filt
		sw	zero,20(sp)
		
		lw	a3,udi_exc		
		sll	s0,s1,0x1
		la	v1,mem_w0
		addu	a1,s0,a3
		move	a0,s7
		move	a2,s8
		li	a3,40
		sw	v1,16(sp)
		jal	Syn_filt
		sw	zero,20(sp)
		lw	a2,udi_exc
		lh	v0,658(sp)
		lh	s5,660(sp)
		addu	a0,s0,a2
		addiu	ra,sp,662
		move	a1,s8
		addiu	a2,sp,136
		li	a3,40
		sw	ra,28(sp)
		sw	v0,16(sp)
		sw	s5,20(sp)
		jal	Pitch_fr3_fast
		sw	s1,24(sp)
		lh	a1,662(sp)
		li	t9,20
		li	t8,143
		move	a0,v0
		addiu	a2,sp,658
		addiu	a3,sp,660
		move	s3,v0
		sw	t9,16(sp)
		sw	t8,20(sp)
		jal	Enc_lag3
		sw	s1,24(sp)
		sh	v0,0(s6)
		beqz	s1,Coder_ld8a2488
		addiu	s2,s6,2
Coder_ld8a1668:	
		lw	a1,udi_exc
		la	t2,mem_zero
		move	a0,s7
		addu	a1,s0,a1
		addiu	a2,sp,456
		li	a3,40
		sw	t2,16(sp)
		jal	Syn_filt
		sw	zero,20(sp)
		move	a0,s8
		addiu	a1,sp,456
		addiu	a2,sp,616
		jal	G_pitch
		li	a3,40
		lh	a1,662(sp)
		move	a0,s3
		sh	v0,664(sp)
		jal	test_err
		li	s6,1
		beq	v0,s6,Coder_ld8a2456
		move	s5,v0
Coder_ld8a1756:	move	s6,zero
		
Coder_ld8a1760:
		lh	a1,664(sp)
		sll	a3,s6,0x1
		addu	t0,a3,s4
		lh	a0,416(t0)
		mpymvrr	acc0,sx0,sy0,shsat,x0,gpra0,y0,gpra1
		mvp	gprt1,udi_mode		
		mvp	gprt1,udi_mode
		mvp	gprt1,udi_mode
		mvp	gpra0,udi_mac0acc0lo
		//jal	L_shl_wg
		//li	a1,1
		
		sll	v0,a0,0x1
		
		sra	a1,v0,0x10
		sll	v0,s6,0x1
		addu	s0,v0,s4		
		sll	ra,s6,0x1
		addu	t9,ra,s4
		jal	wg_sub
		lh	a0,176(t9)
		addiu	s6,s6,1
		slti	t4,s6,40
		bnez	t4,Coder_ld8a1760
		sh	v0,256(s0)
		
		lh	a3,en_sharp
		addiu	t6,sp,376
		addiu	t5,sp,536
		move	a2,s3
		addiu	a1,sp,136
		addiu	a0,sp,296
		addiu	s6,sp,656
		sw	t6,16(sp)
		sw	t5,20(sp)
		jal	ACELP_Code_A
		sw	s6,24(sp)
		sh	v0,0(s2)
		lhu	a0,656(sp)
		addiu	s6,s2,6
		sh	a0,2(s2)
		lhu	a1,616(sp)
		lh	a0,618(sp)
		jal	negate
		sh	a1,624(sp)
		lh	a0,620(sp)
		jal	negate
		sh	v0,640(sp)
		lh	a0,622(sp)
		li	a1,1
		jal	wg_add
		sh	v0,626(sp)
		jal	negate
		move	a0,v0
		addiu	t4,sp,640
		move	a0,s8
		addiu	a1,sp,456
		addiu	a2,sp,536
		addiu	a3,sp,624
		sh	v0,642(sp)
		jal	Corr_xy2
		sw	t4,16(sp)
		addiu	a0,sp,376
		addiu	a1,sp,624
		addiu	t3,sp,664
		addiu	t2,sp,666
		addiu	a2,sp,640
		li	a3,40
		sw	s5,24(sp)
		sw	t3,16(sp)
		jal	Qua_gain
		sw	t2,20(sp)
		sh	v0,4(s2)
		lhu	s2,664(sp)
		
		sh	s2,en_sharp
		sll	s5,s2,0x10
		sra	a0,s5,0x10
		jal	wg_sub
		li	a1,13017	//SHARPMAX
		lhu	a0,en_sharp
		nop
		blez	v0,Coder_ld8a2100
		nop
		li	a0,13017
		sh	a0,en_sharp
		nop
		nop
		
Coder_ld8a2100:	sll	t7,a0,0x10
		sra	a0,t7,0x10
		jal	wg_sub
		li	a1,3277		//SHARPMIN
		bltz	v0,Coder_ld8a2448
		li	t8,3277
Coder_ld8a2124:	
		lw	s2,udi_exc
		sh	zero,656(sp)
		move	s5,zero
Coder_ld8a2132:	
		lh	a1,664(sp)
		addu	t5,s5,s1
		sll	t3,t5,0x1
		addu	t4,t3,s2
		lh	a0,0(t4)		
		mpymvrr	acc0,sx0,sy0,shsat,x0,gpra0,y0,gpra1		
		lh	a2,666(sp)
		sll	t2,s5,0x1
		addu	t1,t2,s4
		lh	a1,336(t1)
		macmvrr	acc0,sx0,sy0,shsat,x0,gpra1,y0,gpra2		
		mvp	gprt1,udi_mode		
		mvp	gprt1,udi_mode
		mvp	gprt1,udi_mode
		mvp	gpra0,udi_mac0acc0lo
		sll	v0,a0,0x1
		
		jal	round16
		move	a0,v0
		//lw	a2,udi_exc
		addu	t1,s5,s1
		sll	a3,t1,0x1
		addu	t0,a3,s2		
		addiu	s5,s5,1
		slti	s0,s5,40
		bnez	s0,Coder_ld8a2132
		sh	v0,0(t0)
		
		lh	a0,664(sp)
		jal	update_exc_err
		move	a1,s3
		lh	t7,666(sp)
		lh	a1,664(sp)
		move	a0,s1
		addiu	a2,sp,456
		addiu	a3,sp,536
		sw	t7,20(sp)
		jal	wg_udi_2
		sw	s8,16(sp)
		addiu	a2,s1,40
		sll	s1,a2,0x10
		sra	s1,s1,0x10
		slti	s3,s1,80
		bnez	s3,Coder_ld8a1464
		addiu	s7,s7,22
		la	a1,old_speech
		addiu	a0,a1,160
		
		jal	Copy
		li	a2,160
		la	a1,old_wsp
		addiu	a0,a1,160
		jal	Copy
		li	a2,143
		la	a1,en_old_exc
		addiu	a0,a1,160
		li	a2,154
		jal	Copy
		nop
		lw	ra,1004(sp)
		lw	s8,1000(sp)
		lw	s7,996(sp)
		lw	s6,992(sp)
		lw	s5,988(sp)
		lw	s4,984(sp)
		lw	s3,980(sp)
		lw	s2,976(sp)
		lw	s1,972(sp)
		lw	s0,968(sp)
		jr	ra
		addiu	sp,sp,1008
Coder_ld8a2448:	sh	t8,en_sharp
		nop
		b	Coder_ld8a2124
		nop
Coder_ld8a2456:	lh	a0,664(sp)
		jal	wg_sub
		li	a1,15564	//GPCLIP
		blezl	v0,Coder_ld8a1760
		sh	zero,656(sp)
		li	a0,15564
		b	Coder_ld8a1756
		sh	a0,664(sp)
		
Coder_ld8a2488:	jal	Parity_Pitch
		move	a0,v0
		addiu	s2,s6,4
		b	Coder_ld8a1668
		sh	v0,2(s6)
		
Coder_ld8a2508:	b	Coder_ld8a1396
		sh	s3,658(sp)
END( Coder_ld8a )

