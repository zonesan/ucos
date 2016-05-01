/*
   ITU-T G.729A Speech Coder with Annex B    ANSI-C Source Code
   Version 1.3    Last modified: March 1997

   Copyright (c) 1996,
   AT&T, France Telecom, NTT, Universite de Sherbrooke, Lucent Technologies,
   Rockwell International
   All rights reserved.
*/

/*-----------------------------------------------------------------*
 * Main program of the G.729A 8.0 kbit/s decoder.                  *
 *                                                                 *
 *    Usage : decoder  bitstream_file  synth_file                  *
 *                                                                 *
 *-----------------------------------------------------------------*/

#include <stdlib.h>
#include <stdio.h>

#include "typedef.h"
#include "basic_op.h"
#include "ld8a.h"
#include "dtx.h"
#include "octet.h"

Word16 bad_lsf;        /* bad LSF indicator   */




Word16 de_y2_hi, de_y2_lo, de_y1_hi, de_y1_lo, de_x0, de_x1;

/*
 Initialization of static values */

void Init_Post_Process(void)
{
  de_y2_hi = 0;
  de_y2_lo = 0;
  de_y1_hi = 0;
  de_y1_lo = 0;
  de_x0   = 0;
  de_x1   = 0;
}


/*
   This variable should be always set to zero unless transmission errors
   in LSP indices are detected.
   This variable is useful if the channel coding designer decides to
   perform error checking on these important parameters. If an error is
   detected on the  LSP indices, the corresponding flag is
   set to 1 signalling to the decoder to perform parameter substitution.
   (The flags should be set back to 0 for correct transmission).
*/

/*-----------------------------------------------------------------*
 *            Main decoder routine                                 *
 *-----------------------------------------------------------------*/

#if 0
void G729Decoder(char  *f_serial, int NNN, char *Dec_OutBit)
//int main(int argc, char *argv[] )
{
  Word16  synth_buf[L_FRAME+M], *synth; /* Synthesis                   */
  Word16  parm[PRM_SIZE+2];             /* Synthesis parameters        */
  Word16  Az_dec[MP1*2];                /* Decoded Az for post-filter  */
  Word16  T2[2];                        /* Pitch lag for 2 subframes   */


  Word16  i, Vad;
  int count1, count2, count4, count3;
  Word32  count_frame, j, decount, decount5;
  char Deref3[1600] ;
  //FILE   *f_syn, *f_serial;
  Word16  *f_syn;
/*  
  printf("\n");
  printf("************  ITU G.729A 8.0 KBIT/S SPEECH DECODER  ************\n");
  printf("                       (WITH ANNEX B)                           \n");
  printf("\n");
  printf("------------------ Fixed point C simulation --------------------\n");
  printf("\n");
  printf("------------------       Version 1.3        --------------------\n");
  printf("\n");
*/
   /* Passed arguments */

 
//#ifndef OCTET_TX_MODE
//  printf("OCTET TRANSMISSION MODE is disabled\n");
//#endif

/*-----------------------------------------------------------------*
 *           Initialization of decoder                             *
 *-----------------------------------------------------------------*/

  for (i=0; i<M; i++) synth_buf[i] = 0;
  synth = synth_buf + M;

  bad_lsf = 0;          /* Initialize bad LSF indicator */
  decount5 = 0;
  Init_Decod_ld8a();
  Init_Post_Filter();
  Init_Post_Process();

  /* for G.729b */
  Init_Dec_cng();

/*-----------------------------------------------------------------*
 *            Loop for each "L_FRAME" speech data                  *
 *-----------------------------------------------------------------*/
	count3 = 0;
  	count4 = 0;
  count_frame = 0L;
  //for(j=0;j<998;j++)
  while(read_frame(f_serial, parm, NNN) != 0)
  {
    //printf("Frame = %ld\r", count_frame++);
    //read_frame(f_serial, parm, NNN);
	count1 = 0;
	count2 = 0;    
	count1 = GetCount(); 
	    	
    Decod_ld8a(parm, synth, Az_dec, T2, &Vad);
    Post_Filter(synth, Az_dec, T2, Vad);        /* Post-filter */
    Post_Process(synth, L_FRAME);
 
 	count2 = GetCount();
	count4 = count2 - count1;
		
  	
	if(count4 >= count3) count3 = count4;
    
    for(i=0;i<L_FRAME;i++)
    {	
	Deref3[2*i] = (unsigned char)(synth[i] & 0x00ff);
	Deref3[2*i + 1] = (unsigned char)((synth[i] >> 8)&0x00ff);		
    }
    
    //CommWrite(Deref3, 160);
	    	decount = (L_FRAME * 2); 
	    	
	    	//CommWrite(ref3, count);
	    	
		for(i = 0; i<decount; i++)
	  	{
	  		if(Deref3[i] == Dec_OutBit[decount5 + i])
	  		{
	  			printf("W");
	  		}
	  		else
	  		{
	  			printf("\r\n");
	  			printf("It is error!");
	  			printf("\r\n");
	  		}	  		
	  	} 
	  	decount5 = decount5 + decount;
	  	
    //fwrite(synth, sizeof(short), L_FRAME, f_syn);

  }
  	printf("\r\n");
  	printf("Decoder Time = %d\r\n", count3 );
  	printf("\r\n");
  return(0);
}

#endif

