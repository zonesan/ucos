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



extern Word16 *new_speech;     /* Pointer to new speech data            */
static Word16 prm[PRM_SIZE+1];        /* Analysis parameters + frame type      */
static Word16 serial[SERIAL_SIZE];    /* Output bitstream buffer               */
// static Word32 count_frame;
/* For G.729B */
static Word16 vad_enable = 0;

VOID AppAudioG729EncodeInit()
{
	/*--------------------------------------------------------------------------*
	 * Initialization of the coder.                                             *
	*--------------------------------------------------------------------------*/

	Init_Pre_Process();
	Init_Coder_ld8a();
	Set_zero(prm, PRM_SIZE+1);
	Init_Cod_cng();

}

LONG  AppAudioG729Encode(PSHORT  pswSpeechInBuf, PBYTE pcwEncoderSpeechOutBuf)
{
	static Word16 frame = 0;  // frame counter 
	Word16 nb_words;
	int len = 0;

	memcpy(new_speech, pswSpeechInBuf, sizeof(Word16)*L_FRAME);

    if (frame == 32767) frame = 256;
    else frame++;

    Pre_Process(new_speech, L_FRAME);
    Coder_ld8a(prm, frame, vad_enable);

#if 0
    prm2bits_ld8k( prm, serial);
    nb_words = serial[1] +  (Word16)2;
	memcpy(pcwEncoderSpeechOutBuf, serial, sizeof(Word16)*nb_words);
#else
	len = vb_prm2bits_ld8k(prm, pcwEncoderSpeechOutBuf);
#endif

	return len;
}


