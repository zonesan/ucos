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

static Word16  synth_buf[L_FRAME+M], *synth;	/* Synthesis              */
static Word16  parm[PRM_SIZE+2];             /* Synthesis parameters        */
static Word16  Az_dec[MP1*2];                /* Decoded Az for post-filter  */
static Word16  T2[2];                        /* Pitch lag for 2 subframes   */
// FILE   *f_syn, *f_serial;


VOID AppAudioG729DecodeInit()
{
	int i;

	/*-----------------------------------------------------------------*
	 *           Initialization of decoder                             *
	 *-----------------------------------------------------------------*/
	for (i=0; i<M; i++) synth_buf[i] = 0;
	synth = synth_buf + M;

	bad_lsf = 0;          /* Initialize bad LSF indicator */
	Init_Decod_ld8a();
	Init_Post_Filter();
	Init_Post_Process();

	/* for G.729b */
	Init_Dec_cng();
}

LONG AppAudioG729Decode(PBYTE  pcwDecodeSpeechInBuf, PSHORT  pswSpeechOutBuf, LONG DecodeFrameLen)
{
	Word16  Vad;

#if 0
	// NIUD need to add error checking
	read_frame(pcwDecodeSpeechInBuf, parm);
#else
	vb_bits2prm_ld8k(pcwDecodeSpeechInBuf, parm, DecodeFrameLen * 8);
#endif

    Decod_ld8a(parm, synth, Az_dec, T2, &Vad);
    Post_Filter(synth, Az_dec, T2, Vad);        /* Post-filter */
    Post_Process(synth, L_FRAME);

	memcpy(pswSpeechOutBuf, synth, sizeof(short)*L_FRAME);

	return(0);
}