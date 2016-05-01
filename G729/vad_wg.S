/*
***************************************************************************
*                  Copyright (c) 2006 vBridge Microsystem, Inc.  
*                       Unpublished & Not for Publication
*                               All Rights Reserved                   
*   
* File        : vad_wg.S                     
*                                                                         
* Description:    
*             
* Date        : Jun 7, 2006                                                           
***************************************************************************
*/

/*
void vad(
         Word16 rc,
         Word16 *lsf, 
         Word16 *r_h,
         Word16 *r_l, 
         Word16 exp_R0,
         Word16 *sigpp,
         Word16 frm_count,
         Word16 prev_marker,
         Word16 pprev_marker,
         Word16 *marker)
{
 // scalar 
  Word32 acc0;
  Word16 i, j, exp, frac;
  Word16 ENERGY, ENERGY_low, SD, ZC, dSE, dSLE, dSZC;
  Word16 COEF, C_COEF, COEFZC, C_COEFZC, COEFSD, C_COEFSD;

  // compute the frame energy 
  acc0 = L_Comp(r_h[0], r_l[0]);
  Log2(acc0, &exp, &frac);
  acc0 = Mpy_32_16(exp, frac, 9864);
  i = sub(exp_R0, 1);  
  i = sub(i, 1);
  acc0 = L_mac(acc0, 9864, i);
  acc0 = L_shl(acc0, 11);
  ENERGY = extract_h(acc0);
  ENERGY = sub(ENERGY, 4875);

  // compute the low band energy 
  acc0 = 0;
  for (i=1; i<=NP; i++)
    acc0 = L_mac(acc0, r_h[i], lbf_corr[i]);
  acc0 = L_shl(acc0, 1);
  acc0 = L_mac(acc0, r_h[0], lbf_corr[0]);
  Log2(acc0, &exp, &frac);
  acc0 = Mpy_32_16(exp, frac, 9864);
  i = sub(exp_R0, 1);  
  i = sub(i, 1);
  acc0 = L_mac(acc0, 9864, i);
  acc0 = L_shl(acc0, 11);
  ENERGY_low = extract_h(acc0);
  ENERGY_low = sub(ENERGY_low, 4875);
  
  // compute SD 
  acc0 = 0;
  for (i=0; i<M; i++){//vad444
    j = sub(lsf[i], MeanLSF[i]);
    acc0 = L_mac(acc0, j, j);
  }
  SD = extract_h(acc0);      // Q15 
  
  // compute # zero crossing 
  ZC = 0;//vad560
  for (i=ZC_START+1; i<=ZC_END; i++)
    if (mult(sigpp[i-1], sigpp[i]) < 0)
      ZC = add(ZC, 410);     // Q15 

  // Initialize and update Mins 
  if(sub(frm_count, 129) < 0){//vad620
  	//vad2884
    if (sub(ENERGY, Min) < 0){
	//vad3540
      Min = ENERGY;
      Prev_Min = ENERGY;
    }
    
    if((frm_count & 0x0007) == 0){
	//vad2904
      i = sub(shr(frm_count,3),1);
      Min_buffer[i] = Min;
      Min = MAX_16;
    }
  }

  if((frm_count & 0x0007) == 0){
  	//vad644
    Prev_Min = Min_buffer[0];
	//vad680
    for (i=1; i<16; i++){
      if (sub(Min_buffer[i], Prev_Min) < 0)
        Prev_Min = Min_buffer[i];
    }
  }
  
  if(sub(frm_count, 129) >= 0){//vad732
    if(((frm_count & 0x0007) ^ (0x0001)) == 0){
		//vad732
      Min = Prev_Min;
      Next_Min = MAX_16;
    }
    if (sub(ENERGY, Min) < 0)//vad780
      Min = ENERGY;
    if (sub(ENERGY, Next_Min) < 0)//vad796
      Next_Min = ENERGY;
    
    if((frm_count & 0x0007) == 0){
		//vad816
      for (i=0; i<15; i++)//vad840
        Min_buffer[i] = Min_buffer[i+1]; 
      Min_buffer[15] = Next_Min; 
      Prev_Min = Min_buffer[0];
      for (i=1; i<16; i++) //vad908
        if (sub(Min_buffer[i], Prev_Min) < 0)
          Prev_Min = Min_buffer[i];
    }
    
  }

  if (sub(frm_count, INIT_FRAME) <= 0)//vad960
  //vad2696
    if(sub(ENERGY, 3072) < 0){
		//vad3504
      *marker = NOISE;
      less_count++;
    }
    else{
		//vad2696
      *marker = VOICE;
      acc0 = L_deposit_h(MeanE);
      acc0 = L_mac(acc0, ENERGY, 1024);
      MeanE = extract_h(acc0);
      acc0 = L_deposit_h(MeanSZC);
      acc0 = L_mac(acc0, ZC, 1024);
      MeanSZC = extract_h(acc0);
      for (i=0; i<M; i++){
	  	//vad2812
        acc0 = L_deposit_h(MeanLSF[i]);
        acc0 = L_mac(acc0, lsf[i], 1024);
        MeanLSF[i] = extract_h(acc0);
      }
    }
  
  if (sub(frm_count, INIT_FRAME) >= 0){//vad980
    if (sub(frm_count, INIT_FRAME) == 0){//vad996wg
    //vad2428
      acc0 = L_mult(MeanE, factor_fx[less_count]);
      acc0 = L_shl(acc0, shift_fx[less_count]);
      MeanE = extract_h(acc0);

      acc0 = L_mult(MeanSZC, factor_fx[less_count]);
      acc0 = L_shl(acc0, shift_fx[less_count]);
      MeanSZC = extract_h(acc0);

      for (i=0; i<M; i++){
        acc0 = L_mult(MeanLSF[i], factor_fx[less_count]);
        acc0 = L_shl(acc0, shift_fx[less_count]);
        MeanLSF[i] = extract_h(acc0);
      }

      MeanSE = sub(MeanE, 2048);   // Q11 
      MeanSLE = sub(MeanE, 2458);  // Q11 
    }
	//vad1012
    dSE = sub(MeanSE, ENERGY);
    dSLE = sub(MeanSLE, ENERGY_low);
    dSZC = sub(MeanSZC, ZC);

    if(sub(ENERGY, 3072) < 0)//vad1064wg
      *marker = NOISE;
	//vad2332
    else 
		//vad1064wg
      *marker = MakeDec(dSLE, dSE, SD, dSZC);

    v_flag = 0;
    if((prev_marker==VOICE) && (*marker==NOISE) && (add(dSE,410) < 0) 
       && (sub(ENERGY, 3072)>0)){//vad1172
      *marker = VOICE;
      v_flag = 1;
    }

    if(flag == 1){
		//vad1436
      if((pprev_marker == VOICE) && 
         (prev_marker == VOICE) && 
         (*marker == NOISE) && 
         (sub(abs_s(sub(prev_energy,ENERGY)), 614) <= 0)){
        count_ext++;
        *marker = VOICE;
        v_flag = 1;
        if(sub(count_ext, 4) <= 0)
          flag=1;
        else{
          count_ext=0;
          flag=0;
        }
      }
    }
    else
		//vad1176
      flag=1;
    //vad1200
    if(*marker == NOISE)
      count_sil++;

    if((*marker == VOICE) && (sub(count_sil, 10) > 0) && 
       (sub(sub(ENERGY,prev_energy), 614) <= 0)){
       //vad2972
      *marker = NOISE;
      count_sil=0;
    }

    if(*marker == VOICE)
      count_sil=0;
	//vad1248
    if ((sub(sub(ENERGY, 614), MeanSE) < 0) && (sub(frm_count, 128) > 0)
        && (!v_flag) && (sub(rc, 19661) < 0))
      *marker = NOISE;
	//vad1280
    if ((sub(sub(ENERGY,614),MeanSE) < 0) && (sub(rc, 24576) < 0)
        && (sub(SD, 83) < 0)){ //vad3060
      count_update++;
      if (sub(count_update, INIT_COUNT) < 0){
        COEF = 24576;
        C_COEF = 8192;
        COEFZC = 26214;
        C_COEFZC = 6554;
        COEFSD = 19661;
        C_COEFSD = 13017;
      } 
      else
        if (sub(count_update, INIT_COUNT+10) < 0){
          COEF = 31130;
          C_COEF = 1638;
          COEFZC = 30147;
          C_COEFZC = 2621;
          COEFSD = 21299;
          C_COEFSD = 11469;
        }
        else
          if (sub(count_update, INIT_COUNT+20) < 0){
            COEF = 31785;
            C_COEF = 983;
            COEFZC = 30802;
            C_COEFZC = 1966;
            COEFSD = 22938;
            C_COEFSD = 9830;
          }
          else
            if (sub(count_update, INIT_COUNT+30) < 0){
              COEF = 32440;
              C_COEF = 328;
              COEFZC = 31457;
              C_COEFZC = 1311;
              COEFSD = 24576;
              C_COEFSD = 8192;
            }
            else
              if (sub(count_update, INIT_COUNT+40) < 0){
                COEF = 32604;
                C_COEF = 164;
                COEFZC = 32440;
                C_COEFZC = 328;
                COEFSD = 24576;
                C_COEFSD = 8192;
              }
              else{
                COEF = 32604;
                C_COEF = 164;
                COEFZC = 32702;
                C_COEFZC = 66;
                COEFSD = 24576;
                C_COEFSD = 8192;
              }
      

      // compute MeanSE 
      acc0 = L_mult(COEF, MeanSE);
      acc0 = L_mac(acc0, C_COEF, ENERGY);
      MeanSE = extract_h(acc0);

      // compute MeanSLE 
      acc0 = L_mult(COEF, MeanSLE);
      acc0 = L_mac(acc0, C_COEF, ENERGY_low);
      MeanSLE = extract_h(acc0);

      // compute MeanSZC 
      acc0 = L_mult(COEFZC, MeanSZC);
      acc0 = L_mac(acc0, C_COEFZC, ZC);
      MeanSZC = extract_h(acc0);
      
      // compute MeanLSF 
      for (i=0; i<M; i++){
	  	//vad3372
        acc0 = L_mult(COEFSD, MeanLSF[i]);
        acc0 = L_mac(acc0, C_COEFSD, lsf[i]);
        MeanLSF[i] = extract_h(acc0);
      }
    }

    if((sub(frm_count, 128) > 0) && (((sub(MeanSE,Min) < 0) &&
                        (sub(SD, 83) < 0)) || (sub(MeanSE,Min) > 2048))){
      MeanSE = Min;
      count_update = 0;
    }
  }
	//vad1384
  prev_energy = ENERGY;

}

// local function   
static Word16 MakeDec(
               Word16 dSLE,    // (i)  : differential low band energy 
               Word16 dSE,     // (i)  : differential full band energy 
               Word16 SD,      // (i)  : differential spectral distortion 
               Word16 dSZC     // (i)  : differential zero crossing rate 
               )
{
  Word32 acc0;
  
  // SD vs dSZC 
  acc0 = L_mult(dSZC, -14680);          // Q15*Q23*2 = Q39   
  acc0 = L_mac(acc0, 8192, -28521);     // Q15*Q23*2 = Q39 
  acc0 = L_shr(acc0, 8);                // Q39 -> Q31 
  acc0 = L_add(acc0, L_deposit_h(SD));
  if (acc0 > 0) return(VOICE);

  acc0 = L_mult(dSZC, 19065);           // Q15*Q22*2 = Q38 
  acc0 = L_mac(acc0, 8192, -19446);     // Q15*Q22*2 = Q38 
  acc0 = L_shr(acc0, 7);                // Q38 -> Q31 
  acc0 = L_add(acc0, L_deposit_h(SD));
  if (acc0 > 0) return(VOICE);

  // dSE vs dSZC 
  acc0 = L_mult(dSZC, 20480);           // Q15*Q13*2 = Q29 
  acc0 = L_mac(acc0, 8192, 16384);      // Q13*Q15*2 = Q29 
  acc0 = L_shr(acc0, 2);                // Q29 -> Q27 
  acc0 = L_add(acc0, L_deposit_h(dSE));
  if (acc0 < 0) return(VOICE);

  acc0 = L_mult(dSZC, -16384);          // Q15*Q13*2 = Q29 
  acc0 = L_mac(acc0, 8192, 19660);      // Q13*Q15*2 = Q29 
  acc0 = L_shr(acc0, 2);                // Q29 -> Q27 
  acc0 = L_add(acc0, L_deposit_h(dSE));
  if (acc0 < 0) return(VOICE);
 
  acc0 = L_mult(dSE, 32767);            // Q11*Q15*2 = Q27 
  acc0 = L_mac(acc0, 1024, 30802);      // Q10*Q16*2 = Q27 
  if (acc0 < 0) return(VOICE);
  
  // dSE vs SD 
  acc0 = L_mult(SD, -28160);            // Q15*Q5*2 = Q22 
  acc0 = L_mac(acc0, 64, 19988);        // Q6*Q14*2 = Q22 
  acc0 = L_mac(acc0, dSE, 512);         // Q11*Q9*2 = Q22 
  if (acc0 < 0) return(VOICE);

  acc0 = L_mult(SD, 32767);             // Q15*Q15*2 = Q31 
  acc0 = L_mac(acc0, 32, -30199);       // Q5*Q25*2 = Q31 
  if (acc0 > 0) return(VOICE);

  // dSLE vs dSZC 
  acc0 = L_mult(dSZC, -20480);          // Q15*Q13*2 = Q29 
  acc0 = L_mac(acc0, 8192, 22938);      // Q13*Q15*2 = Q29 
  acc0 = L_shr(acc0, 2);                // Q29 -> Q27 
  acc0 = L_add(acc0, L_deposit_h(dSE));
  if (acc0 < 0) return(VOICE);

  acc0 = L_mult(dSZC, 23831);           // Q15*Q13*2 = Q29 
  acc0 = L_mac(acc0, 4096, 31576);      // Q12*Q16*2 = Q29 
  acc0 = L_shr(acc0, 2);                // Q29 -> Q27 
  acc0 = L_add(acc0, L_deposit_h(dSE));
  if (acc0 < 0) return(VOICE);
  
  acc0 = L_mult(dSE, 32767);            // Q11*Q15*2 = Q27 
  acc0 = L_mac(acc0, 2048, 17367);      // Q11*Q15*2 = Q27 
  if (acc0 < 0) return(VOICE);
  
  // dSLE vs SD 
  acc0 = L_mult(SD, -22400);            // Q15*Q4*2 = Q20 
  acc0 = L_mac(acc0, 32, 25395);        // Q5*Q14*2 = Q20 
  acc0 = L_mac(acc0, dSLE, 256);        // Q11*Q8*2 = Q20 
  if (acc0 < 0) return(VOICE);
  
  // dSLE vs dSE 
  acc0 = L_mult(dSE, -30427);           // Q11*Q15*2 = Q27 
  acc0 = L_mac(acc0, 256, -29959);      // Q8*Q18*2 = Q27 
  acc0 = L_add(acc0, L_deposit_h(dSLE));
  if (acc0 > 0) return(VOICE);

  acc0 = L_mult(dSE, -23406);           // Q11*Q15*2 = Q27 
  acc0 = L_mac(acc0, 512, 28087);       // Q19*Q17*2 = Q27 
  acc0 = L_add(acc0, L_deposit_h(dSLE));
  if (acc0 < 0) return(VOICE);

  acc0 = L_mult(dSE, 24576);            // Q11*Q14*2 = Q26 
  acc0 = L_mac(acc0, 1024, 29491);      // Q10*Q15*2 = Q26 
  acc0 = L_mac(acc0, dSLE, 16384);      // Q11*Q14*2 = Q26 
  if (acc0 < 0) return(VOICE);

  return (NOISE);
}


*/


#include <mips/asm.h>
#include <mips/udi.h>
#include <mips/regdef.h>
#include "mipsudi_inst.h"

      .set noat
      .set noreorder

LEAF( vad_wg )
		addiu	sp,sp,-80
		sw	a1,84(sp)
		sll	a1,a0,0x10
		sw	ra,76(sp)
		sw	s8,72(sp)
		sw	s7,68(sp)
		sw	s6,64(sp)
		sw	s5,60(sp)
		sw	s4,56(sp)
		sw	s3,52(sp)
		sw	s2,48(sp)
		sw	s1,44(sp)
		sw	s0,40(sp)
		sra	a0,a1,0x10
		lh	v1,112(sp)
		lh	a1,0(a3)
		move	s3,a2
		sw	a0,20(sp)
		lh	a0,0(a2)
		lh	a2,108(sp)
		sw	v1,28(sp)
		lh	s5,96(sp)
		sw	a2,24(sp)
		lh	s7,104(sp)
		jal	L_Comp
		addiu	s8,sp,18
		move	a0,v0
		addiu	a1,sp,16
		jal	Log2
		move	a2,s8
		lh	a0,16(sp)
		lh	a1,18(sp)
		jal	Mpy_32_16
		li	a2,9864
		move	a0,s5
		li	a1,1
		jal	wg_sub
		move	s0,v0
		move	a0,v0
		jal	wg_sub
		li	a1,1
		move	a2,v0
		move	a0,s0
		jal	L_mac
		li	a1,9864
		move	a0,v0
		jal	L_shl
		li	a1,11
		jal	extract_h
		move	a0,v0
		move	a0,v0
		jal	wg_sub
		li	a1,4875
		
		move	s6,v0
		la	v0,lbf_corr
		move	s4,v0
				
		addiu	s2,v0,2
		move	a0,zero
		mvp	udi_mac0acc0lo,gpra0
		mvp	udi_mac0acc0hi,gpra0
		li	s1,1
		addiu	s0,s3,2
vad240:		lh	a1,0(s0)
		lh	a2,0(s2)
		macmvrr	acc0,sx0,sy0,shsat,x0,gpra1,y0,gpra2		
		addiu	s0,s0,2
		addiu	s1,s1,1
		slti	a3,s1,13
		bnez	a3,vad240
		addiu	s2,s2,2		
		mvp	gprv0,udi_mode
		mvp	gprv0,udi_mode
		mvp	gprv0,udi_mode
		mvp	gpra0,udi_mac0acc0lo		
		jal	L_shl
		li	a1,1
		lh	a1,0(s3)
		lh	a2,0(s4)		
		jal	L_mac
		move	a0,v0
		move	a0,v0
		move	a2,s8
		jal	Log2
		addiu	a1,sp,16
		lh	a0,16(sp)
		lh	a1,18(sp)
		jal	Mpy_32_16
		li	a2,9864
		move	a0,s5
		li	a1,1
		jal	wg_sub
		move	s2,v0
		move	a0,v0
		jal	wg_sub
		li	a1,1
		move	a2,v0
		move	a0,s2
		jal	L_mac
		li	a1,9864
		move	a0,v0
		jal	L_shl
		li	a1,11
		jal	extract_h
		move	a0,v0
		move	a0,v0
		jal	wg_sub
		li	a1,4875
		
		la	s1,MeanLSF		
		lw	s0,84(sp)
		move	s3,zero
		sw	v0,36(sp)
		move	s2,zero
		mvp	udi_mac0acc0lo,gprs2
		mvp	udi_mac0acc0hi,gprs2
vad444:		lh	a0,0(s0)
		lh	a1,0(s1)
		jal	wg_sub
		addiu	s0,s0,2
		macmvrr	acc0,sx0,sy0,shsat,x0,gprv0,y0,gprv0
		addiu	s2,s2,1
		slti	t3,s2,10
		bnez	t3,vad444
		addiu	s1,s1,2	
		
		mvp	gprv0,udi_mode
		mvp	gprv0,udi_mode
		mvp	gprv0,udi_mode
		mvp	gpra0,udi_mac0acc0lo	
		lw	t6,100(sp)
		move	s2,zero
		sra	v0,a0,0x10
		addiu	s0,t6,242
		li	s1,121
		b	vad560
		sw	v0,32(sp)
		
vad536:		addiu	s1,s1,1
		slti	t7,s1,201
		beqz	t7,vad620
		move	a0,s7
vad560:		lh	a0,-2(s0)
		lh	a1,0(s0)
		mpymvrr	acc0,sx0,sy0,shnone,x0,gpra0,y0,gpra1
		satacc	mac0, acc0, round_disable, sat_disable, satp32, 12
		satacc	mac0, acc0, round_disable, sat_enable, satp16, 3
		mvp	gprv0,udi_mode
		mvp	gprv0,udi_mode
		mvp	gprv0,udi_mode
		mvp	gprv0,udi_mac0acc0lo
		addiu	s0,s0,2
		move	a0,s2
		bgez	v0,vad536
		li	a1,410
		
		//sll	s3,s4,0x10
		jal	wg_add
		addiu	s1,s1,1
		//sra	s1,s3,0x10
		slti	t7,s1,201	//ZC_END
		bnez	t7,vad560
		move	s2,v0
				
		move	a0,s7
vad620:		//jal	wg_sub
		//li	a1,129
		//bltz	v0,vad2884
		slti	v0,a0,129
		bgtz	v0,vad2884
		andi	s3,s7,0x7
vad630:		bnez	s3,vad732
		move	a0,s7
vad644:		
		la	a2,Min_buffer
		lhu	v1,0(a2)
		move	v0,a2
		sh	v1,Prev_Min
		li	s0,1
		b	vad680
		addiu	s1,v0,2	
		
vad672:		beqz	v1,vad728
		addiu	s1,s1,2
vad680:		lh	a1,Prev_Min
		jal	wg_sub
		lh	a0,0(s1)		
		addiu	a1,s0,1
		sll	a0,a1,0x10
		sra	s0,a0,0x10
		bgez	v0,vad672
		slti	v1,s0,16
		lhu	a3,0(s1)		
		sh	a3,Prev_Min
		bnez	v1,vad680
		addiu	s1,s1,2
				
vad728:		move	a0,s7
vad732:		//jal	wg_sub
		//li	a1,129
		//bltz	v0,vad960
		slti	v0,a0,129
		bgtz	v0,vad960
		
		move	a0,s7
		xori	s1,s3,0x1
		lh	a1,Min
		nop
		bnez	s1,vad780
		nop
		lhu	t1,Prev_Min
		li	t0,32767
		sh	t1,Min
		sh	t0,Next_Min
		lh	a1,Min
		nop
vad780:		jal	wg_sub
		move	a0,s6
		la	t0,Min
		bltzl	v0,vad796		
		sh	s6,0(t0)		
vad796:		
		lh	a1,Next_Min
		jal	wg_sub
		move	a0,s6
		la	t0,Next_Min
		bltzl	v0,vad816
		sh	s6,0(t0)
		
vad816:		bnezl	s3,vad960
		move	a0,s7		
		la	a2,Min_buffer
		move	a1,zero
		move	a0,a2
vad840:		addiu	t5,a1,1
		sll	t4,t5,0x10
		lhu	t3,2(a0)
		sra	a1,t4,0x10
		slti	t2,a1,15
		sh	t3,0(a0)
		bnez	t2,vad840
		addiu	a0,a0,2	
		
		lhu	t7,0(a2)
		lhu	t6,Next_Min
		addiu	s1,a2,2
		li	s0,1
		sh	t7,Prev_Min
		b	vad908
		sh	t6,30(a2)
				
vad900:		beqz	v1,vad956
		addiu	s1,s1,2
vad908:		addiu	s4,s0,1
		lh	a0,0(s1)
		lh	a1,Prev_Min
		jal	wg_sub
		sll	s3,s4,0x10
		sra	s0,s3,0x10
		bgez	v0,vad900
		slti	v1,s0,16		
		lhu	s5,0(s1)
		sh	s5,Prev_Min
		bnez	v1,vad908	
		addiu	s1,s1,2
				
vad956:		move	a0,s7
vad960:		jal	wg_sub
		li	a1,32	//INIT_FRAME
		blez	v0,vad2696
		move	a0,s6
		
vad976:		move	a0,s7
vad980:		jal	wg_sub
		li	a1,32
		bltz	v0,vad1384
		move	a0,s7
vad996wg:	jal	wg_sub
		li	a1,32
		lh	s8,less_count
		beqz	v0,vad2428
		nop
vad1012:	
		lh	a0,MeanSE
		jal	wg_sub
		move	a1,s6		
		lh	a0,MeanSLE
		lw	a1,36(sp)
		jal	wg_sub
		move	s1,v0		
		lh	a0,MeanSZC
		move	a1,s2
		jal	wg_sub
		move	s4,v0		
		move	a0,s6
		li	a1,3072
vad1064wg:	jal	wg_sub
		move	s3,v0
		bltz	v0,vad2332
		move	t0,s3	
		li	t1,-14680
		li	a2,-28521
		li	a1,8192
		mpymvrr	acc0,sx0,sy0,shsat,x0,gprt0,y0,gprt1
		macmvrr	acc0,sx0,sy0,shsat,x0,gpra1,y0,gpra2
		satacc	mac0,acc0,round_disable,sat_disable,satp32,8
		mvp	gprv0,udi_mode
		mvp	gprv0,udi_mode
		mvp	gprv0,udi_mode
		lw	t0,32(sp)
		mvp	gpra0,udi_mac0acc0lo		
		jal	L_add
		sll	a1,t0,0x10
		blez	v0,vad1588
		li	v1,1
vad1148:	lw	t3,116(sp)
		lw	t5,24(sp)
		li	t4,1
		sh	zero,v_flag
		beq	t5,t4,vad2356
		sh	v1,0(t3)		
vad1172:	
		lh	s1,wg_flag
vad1176:	li	s0,1
		beq	s1,s0,vad1436
		lw	a3,28(sp)
		sh	s0,wg_flag
vad1192:	lw	s3,116(sp)
vad1196:	lh	v1,0(s3)
vad1200:	bnez	v1,vad1228
		li	t4,1
		
		lhu	t3,count_sil
		lw	s8,116(sp)
		addiu	s0,t3,1
		sh	s0,count_sil
		lh	v1,0(s8)
vad1228:	
		lh	t0,count_sil
		beql	v1,t4,vad2972
		move	a0,t0
		
vad1236:	li	t7,1
		la	t0,count_sil
		beql	v1,t7,vad1248
		sh	zero,0(t0)
		
vad1248:	move	a0,s6
		jal	wg_sub
		li	a1,614
		lh	a1,MeanSE
		jal	wg_sub
		move	a0,v0
		bltzl	v0,vad3448
		move	a0,s7
vad1280:	move	a0,s6
vad1284:	jal	wg_sub
		li	a1,614
		lh	a1,MeanSE
		jal	wg_sub
		move	a0,v0
		bltz	v0,vad3060
		lw	a0,20(sp)
		move	a0,s7
vad1316:	jal	wg_sub
		li	a1,128
		lh	a0,MeanSE
		blez	v0,vad1384
		nop
		lh	a1,Min
		jal	wg_sub
		nop
		bltz	v0,vad3028
		lw	a0,32(sp)
		lh	a0,MeanSE
vad1352:	
		lh	a1,Min
		jal	wg_sub
		nop
		slti	s7,v0,2049
		la	t0,prev_energy
		bnezl	s7,vad1388
		sh	s6,0(t0)
		lhu	v0,Min
		sh	zero,count_update
		sh	v0,MeanSE
		nop
		
vad1384:	
		sh	s6,prev_energy
vad1388:	lw	ra,76(sp)
		lw	s8,72(sp)
		lw	s7,68(sp)
		lw	s6,64(sp)
		lw	s5,60(sp)
		lw	s4,56(sp)
		lw	s3,52(sp)
		lw	s2,48(sp)
		lw	s1,44(sp)
		lw	s0,40(sp)
		jr	ra
		addiu	sp,sp,80
		
vad1436:	lw	a0,24(sp)
		xori	a1,a3,0x1
		xori	v0,a0,0x1
		sltiu	v1,a1,1
		sltiu	a2,v0,1
		and	ra,v1,a2
		beqz	ra,vad1580
		lw	s5,116(sp)
		lw	t0,116(sp)
		lh	v1,0(t0)
		bnez	v1,vad1228
		li	t4,1
		lh	a0,prev_energy
		jal	wg_sub
		move	a1,s6
		jal	abs_s
		move	a0,v0
		move	a0,v0
		jal	wg_sub
		li	a1,614
		bgtz	v0,vad1196
		lw	s3,116(sp)
		lhu	s4,count_ext
		lw	t1,116(sp)
		addiu	t2,s4,1
		sh	t2,count_ext
		sh	s0,0(t1)
		lh	a0,count_ext
		sh	s0,v_flag
		jal	wg_sub
		li	a1,4
		la	t0,wg_flag		
		blezl	v0,vad1192
		sh	s0,0(t0)		
		sh	zero,count_ext
		sh	zero,wg_flag
		lw	s5,116(sp)
vad1580:	b	vad1200
		lh	v1,0(s5)
		
		// MakeDec
vad1588:					
		li	t1,19065
		li	a2,-19446
		li	a1,8192
		mpymvrr	acc0,sx0,sy0,shsat,x0,gprs3,y0,gprt1		
		macmvrr	acc0,sx0,sy0,shsat,x0,gpra1,y0,gpra2
		satacc	mac0,acc0,round_disable,sat_disable,satp32,7
		mvp	gprv0,udi_mode
		mvp	gprv0,udi_mode
		mvp	gprv0,udi_mode
		lw	t0,32(sp)
		mvp	gpra0,udi_mac0acc0lo
		jal	L_add
		sll	a1,t0,0x10
		
		bgtz	v0,vad1148
		li	v1,1			
		li	t1,20480
		li	a2,16384
		li	a1,8192
		mpymvrr	acc0,sx0,sy0,shsat,x0,gprs3,y0,gprt1
		macmvrr	acc0,sx0,sy0,shsat,x0,gpra1,y0,gpra2
		satacc	mac0,acc0,round_disable,sat_disable,satp32,2
		mvp	gprv0,udi_mode
		mvp	gprv0,udi_mode
		mvp	gprv0,udi_mode
		mvp	gpra0,udi_mac0acc0lo		
		jal	L_add
		sll	a1,s1,0x10
		
		bltz	v0,vad1148
		li	v1,1
			
		li	t1,-16384
		li	a2,19660
		li	a1,8192
		mpymvrr	acc0,sx0,sy0,shsat,x0,gprs3,y0,gprt1
		macmvrr	acc0,sx0,sy0,shsat,x0,gpra1,y0,gpra2
		satacc	mac0,acc0,round_disable,sat_disable,satp32,2
		mvp	gprv0,udi_mode
		mvp	gprv0,udi_mode
		mvp	gprv0,udi_mode
		mvp	gpra0,udi_mac0acc0lo		
		jal	L_add
		sll	a1,s1,0x10
		
		bltz	v0,vad1148
		li	v1,1		
			
		li	t1,32767
		li	a2,30802
		li	a1,1024
		mpymvrr	acc0,sx0,sy0,shsat,x0,gprs1,y0,gprt1
		macmvrr	acc0,sx0,sy0,shsat,x0,gpra1,y0,gpra2
		mvp	gprv0,udi_mode
		mvp	gprv0,udi_mode
		mvp	gprv0,udi_mode
		mvp	gprv0,udi_mac0acc0lo
		
		bltz	v0,vad1148
		li	v1,1
		
		lw	a0,32(sp)
		li	t1,-28160
		li	a2,19988
		li	a1,64
		li	t2,512
		mpymvrr	acc0,sx0,sy0,shsat,x0,gpra0,y0,gprt1
		macmvrr	acc0,sx0,sy0,shsat,x0,gpra1,y0,gpra2
		macmvrr	acc0,sx0,sy0,shsat,x0,gprs1,y0,gprt2
		mvp	gprv0,udi_mode
		mvp	gprv0,udi_mode
		mvp	gprv0,udi_mode
		mvp	gprv0,udi_mac0acc0lo
		
		bltz	v0,vad1148
		li	v1,1
		
		lw	a0,32(sp)
		li	t1,32767
		li	a2,-30199
		li	a1,32
		mpymvrr	acc0,sx0,sy0,shsat,x0,gpra0,y0,gprt1
		macmvrr	acc0,sx0,sy0,shsat,x0,gpra1,y0,gpra2
		mvp	gprv0,udi_mode
		mvp	gprv0,udi_mode
		mvp	gprv0,udi_mode
		mvp	gprv0,udi_mac0acc0lo
		
		bgtz	v0,vad1148
		li	v1,1
			
		li	t1,-20480
		li	a2,22938
		li	a1,8192
		mpymvrr	acc0,sx0,sy0,shsat,x0,gprs3,y0,gprt1
		macmvrr	acc0,sx0,sy0,shsat,x0,gpra1,y0,gpra2
		satacc	mac0,acc0,round_disable,sat_disable,satp32,2
		mvp	gprv0,udi_mode
		mvp	gprv0,udi_mode
		mvp	gpra0,udi_mode
		mvp	gpra0,udi_mac0acc0lo		
		jal	L_add
		sll	a1,s1,0x10
		
		bltz	v0,vad1148
		li	v1,1
			
		li	t1,23831
		li	a2,31576
		li	a1,4096
		mpymvrr	acc0,sx0,sy0,shsat,x0,gprs3,y0,gprt1
		macmvrr	acc0,sx0,sy0,shsat,x0,gpra1,y0,gpra2
		satacc	mac0,acc0,round_disable,sat_disable,satp32,2
		mvp	gprv0,udi_mode
		mvp	gprv0,udi_mode
		mvp	gpra0,udi_mode
		mvp	gpra0,udi_mac0acc0lo		
		jal	L_add
		sll	a1,s1,0x10
		
		bltz	v0,vad1148
		li	v1,1
		
		li	t1,32767
		li	a2,17367
		li	a1,2048
		mpymvrr	acc0,sx0,sy0,shsat,x0,gprs1,y0,gprt1
		macmvrr	acc0,sx0,sy0,shsat,x0,gpra1,y0,gpra2
		mvp	gprv0,udi_mode
		mvp	gprv0,udi_mode
		mvp	gprv0,udi_mode
		mvp	gprv0,udi_mac0acc0lo
		
		bltz	v0,vad1148
		li	v1,1
		
		lw	a0,32(sp)
		li	t1,-22400
		li	a2,25395
		li	a1,32
		li	t2,256
		mpymvrr	acc0,sx0,sy0,shsat,x0,gpra0,y0,gprt1
		macmvrr	acc0,sx0,sy0,shsat,x0,gpra1,y0,gpra2
		macmvrr	acc0,sx0,sy0,shsat,x0,gprs4,y0,gprt2
		mvp	gprv0,udi_mode
		mvp	gprv0,udi_mode
		mvp	gprv0,udi_mode
		mvp	gprv0,udi_mac0acc0lo
		
		bltz	v0,vad1148
		li	v1,1
			
		li	t1,-30427
		li	a2,-29959
		li	a1,256
		mpymvrr	acc0,sx0,sy0,shsat,x0,gprs1,y0,gprt1
		macmvrr	acc0,sx0,sy0,shsat,x0,gpra1,y0,gpra2
		mvp	gprv0,udi_mode
		mvp	gprv0,udi_mode
		mvp	gpra0,udi_mode
		//lw	t0,32(sp)
		mvp	gpra0,udi_mac0acc0lo		
		jal	L_add
		sll	a1,s4,0x10
		
		bgtz	v0,vad1148
		li	v1,1
			
		li	t1,-23406
		li	a2,28087
		li	a1,512
		mpymvrr	acc0,sx0,sy0,shsat,x0,gprs1,y0,gprt1
		macmvrr	acc0,sx0,sy0,shsat,x0,gpra1,y0,gpra2
		mvp	gprv0,udi_mode
		mvp	gprv0,udi_mode
		mvp	gpra0,udi_mode
		mvp	gpra0,udi_mac0acc0lo		
		jal	L_add
		sll	a1,s4,0x10
		
		bltz	v0,vad1148
		li	v1,1
		
		
		li	t1,24576
		li	a2,29491
		li	a1,1024
		li	t2,16384
		mpymvrr	acc0,sx0,sy0,shsat,x0,gprs1,y0,gprt1
		macmvrr	acc0,sx0,sy0,shsat,x0,gpra1,y0,gpra2
		macmvrr	acc0,sx0,sy0,shsat,x0,gprs4,y0,gprt2
		mvp	gprv0,udi_mode
		mvp	gprv0,udi_mode
		mvp	gprv0,udi_mode
		mvp	gprv0,udi_mac0acc0lo
		
		b	vad1148
		slti	v1,v0,0
		
vad2332:	lw	s4,116(sp)
		lw	t5,24(sp)
		li	t4,1
		sh	zero,v_flag
		bne	t5,t4,vad1172
		sh	zero,0(s4)
		
vad2356:	lw	t7,116(sp)
		lh	t6,0(t7)
		lh	t0,wg_flag
		bnezl	t6,vad1176
		move	s1,t0
		move	a0,s1
		jal	wg_add
		li	a1,410
		lh	s1,wg_flag
		bgez	v0,vad1176
		nop
		//lh	s1,-32510(gp)
		move	a0,s6
		jal	wg_sub
		li	a1,3072
		blez	v0,vad1172
		lw	t8,24(sp)
		lw	t9,116(sp)
		sh	t8,v_flag
		b	vad1172
		sh	t8,0(t9)
		
vad2428:	
		la	s4,factor_fx
		sll	s3,s8,0x1
		addu	s1,s3,s4
		lh	a1,0(s1)		
		lh	a0,MeanE
		jal	L_mult
		move	s1,zero
		lh	t8,less_count
		la	s3,shift_fx
		sll	s5,t8,0x1
		addu	t7,s5,s3
		lh	a1,0(t7)
		jal	L_shl
		move	a0,v0
		jal	extract_h
		move	a0,v0
		lh	t6,less_count
		lh	a0,MeanSZC
		sll	t5,t6,0x1
		addu	t4,t5,s4
		sh	v0,MeanE
		jal	L_mult
		lh	a1,0(t4)
		
		lh	t3,less_count
		sll	t2,t3,0x1
		addu	t1,t2,s3
		lh	a1,0(t1)
		
		jal	L_shl
		move	a0,v0
		la	s0,MeanLSF
		jal	extract_h
		move	a0,v0
		sh	v0,MeanSZC
vad2576:	
		lh	t2,less_count
		lh	a0,0(s0)
		sll	t1,t2,0x1
		addu	t0,t1,s4
		jal	L_mult
		lh	a1,0(t0)
		lh	a3,less_count
		move	a0,v0
		sll	a1,a3,0x1
		addu	v0,a1,s3
		jal	L_shl
		lh	a1,0(v0)
		jal	extract_h
		move	a0,v0
		addiu	a0,s1,1
		sll	a2,a0,0x10
		sra	s1,a2,0x10
		slti	v1,s1,10
		sh	v0,0(s0)
		bnez	v1,vad2576
		addiu	s0,s0,2	
		lh	a0,MeanE
		jal	wg_sub
		li	a1,2048
		lh	a0,MeanE
		sh	v0,MeanSE
		jal	wg_sub
		li	a1,2458
		sh	v0,MeanSLE
		b	vad1012
		nop
				
vad2696:	jal	wg_sub
		li	a1,3072
		bltz	v0,vad3504
		li	v1,1
		lw	s0,116(sp)
		sh	v1,0(s0)
		lh	a0,MeanE
		jal	L_deposit_h_wg
		move	s3,zero
		move	a0,v0
		move	a1,s6
		jal	L_mac
		li	a2,1024
		jal	extract_h
		move	a0,v0
		lh	a0,MeanSZC
		sh	v0,MeanE
		jal	L_deposit_h_wg
		nop
		move	a0,v0
		move	a1,s2
		jal	L_mac
		li	a2,1024
		jal	extract_h
		move	a0,v0
		sh	v0,MeanSZC
		lw	s0,84(sp)
		la	s1,MeanLSF
vad2812:	jal	L_deposit_h_wg
		//sll	v0,a0,0x10
		lh	a0,0(s1)
		lh	a1,0(s0)
		li	a2,1024
		jal	L_mac
		//macmvrr	acc0,sx0,sy0,shsat,x0,gpra1,y0,gpra2
		//mvp	gprv0,udi_mode
		//mvp	gprv0,udi_mode
		//mvp	gprv0,udi_mode
		//mvp	gprv0,udi_mac0acc0lo
		move	a0,v0
		jal	extract_h
		//sra	v0,a0,0x10
		move	a0,v0
		addiu	a1,s3,1
		sll	a0,a1,0x10
		sra	s3,a0,0x10
		slti	a2,s3,10
		sh	v0,0(s1)
		addiu	s0,s0,2
		bnez	a2,vad2812
		addiu	s1,s1,2		
		b	vad980
		move	a0,s7		
vad2884:	
		lh	a1,Min
		jal	wg_sub
		move	a0,s6
		la	t0,Min
		bltzl	v0,vad3540
		sh	s6,0(t0)
vad2904:	andi	s3,s7,0x7
		bnez	s3,vad732
		move	a0,s7
		jal	wg_shr
		li	a1,3
		move	a0,v0
		jal	wg_sub
		li	a1,1
		lhu	t9,Min
		sll	s8,v0,0x1		
		la	ra,Min_buffer
		addu	t8,s8,ra
		li	s5,32767
		sh	t9,0(t8)
		sh	s5,Min
		b	vad644
		nop
				
vad2972:	jal	wg_sub
		li	a1,10
		lh	a1,prev_energy
		blez	v0,vad3528
		nop
		jal	wg_sub
		move	a0,s6
		move	a0,v0
		jal	wg_sub
		li	a1,614
		blez	v0,vad3548
		lw	t5,116(sp)
		lw	a0,116(sp)
		b	vad1236
		lh	v1,0(a0)
		
vad3028:	jal	wg_sub
		li	a1,83
		lh	a0,MeanSE
		bgez	v0,vad1352
		nop
		lhu	v0,Min
		sh	zero,count_update
		sh	v0,MeanSE
		b	vad1384
		nop
		
vad3060:	jal	wg_sub
		li	a1,24576
		bgez	v0,vad1316
		move	a0,s7
		lw	a0,32(sp)
		jal	wg_sub
		li	a1,83
		bgez	v0,vad1316
		move	a0,s7
		lhu	v1,count_update
		li	a1,20
		addiu	s1,v1,1
		sll	ra,s1,0x10
		sra	a0,ra,0x10
		sh	s1,count_update
		jal	wg_sub
		nop
vad3124:	bltz	v0,vad3648
		li	s0,24576
		lh	a0,count_update
		jal	wg_sub
		li	a1,30
		bltz	v0,vad3624
		li	s0,31130
		lh	a0,count_update
		jal	wg_sub
		li	a1,40
		bltz	v0,vad3600
		li	s0,31785
		lh	a0,count_update
		jal	wg_sub
		li	a1,50
		bltz	v0,vad3584
		li	s0,32440
		lh	a0,count_update
		jal	wg_sub
		li	a1,60
		bltz	v0,vad3568
		li	s0,32604
		li	s1,164
		li	s3,32702
		li	s4,66
vad3224:	li	s5,24576
		li	s8,8192
vad3232:	
		lh	a1,MeanSE
		jal	L_mult
		move	a0,s0
		move	a0,v0
		move	a1,s1
		jal	L_mac
		move	a2,s6
		jal	extract_h
		move	a0,v0
		lh	a1,MeanSLE
		sh	v0,MeanSE
		jal	L_mult
		move	a0,s0
		lw	a2,36(sp)
		move	a1,s1
		jal	L_mac
		move	a0,v0
		jal	extract_h
		move	a0,v0
		lh	a1,MeanSZC
		sh	v0,MeanSLE
		jal	L_mult
		move	a0,s3
		move	a0,v0
		move	a2,s2
		jal	L_mac
		move	a1,s4
		jal	extract_h
		move	a0,v0	
		lw	s0,84(sp)
		la	s1,MeanLSF
		sh	v0,MeanSZC
		move	s2,zero
vad3372:	lh	a1,0(s1)
		jal	L_mult
		move	a0,s5
		lh	a2,0(s0)
		move	a1,s8
		jal	L_mac
		move	a0,v0
		jal	extract_h
		move	a0,v0
		addiu	a3,s2,1
		sll	a0,a3,0x10
		sra	s2,a0,0x10
		slti	a1,s2,10
		sh	v0,0(s1)
		addiu	s0,s0,2
		bnez	a1,vad3372
		addiu	s1,s1,2
		b	vad1316		
		move	a0,s7
		
vad3448:	jal	wg_sub
		li	a1,128
		lh	t8,v_flag
		blez	v0,vad1280
		nop
		bnez	t8,vad1284
		move	a0,s6
		lw	a0,20(sp)
		jal	wg_sub
		li	a1,19661
		bgezl	v0,vad1284
		move	a0,s6
		lw	t9,116(sp)
		b	vad1280
		sh	zero,0(t9)
		
vad3504:	lw	s8,116(sp)
		sh	zero,0(s8)
		lhu	t9,less_count
		addiu	t8,t9,1
		sh	t8,less_count
		b	vad976
		nop
		
vad3528:	lw	t6,116(sp)
		b	vad1236
		lh	v1,0(t6)
vad3540:	
		//sh	s6,Min
		sh	s6,Prev_Min
		b	vad2904
		nop
		
vad3548:	sh	zero,0(t5)
		move	a0,t5
		sh	zero,count_sil
		b	vad1236
		lh	v1,0(a0)
		
vad3568:	li	s1,164
		li	s3,32440
		b	vad3224
		li	s4,328
vad3584:	li	s1,328
		li	s3,31457
		b	vad3224
		li	s4,1311
vad3600:	li	s1,983
		li	s3,30802
		li	s4,1966
		li	s5,22938
		b	vad3232
		li	s8,9830
vad3624:	li	s1,1638
		li	s3,30147
		li	s4,2621
		li	s5,21299
		b	vad3232
		li	s8,11469
vad3648:	li	s1,8192
		li	s3,26214
		li	s4,6554
		li	s5,19661
		b	vad3232
		li	s8,13017
END( vad_wg )

