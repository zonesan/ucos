/*********************************************************************************** 
 * vb_g726.h --  Vbridge G.726 libraries 
 *
 * Created 2005 by Daniel Ding of Vbridge Microsystem, Inc.
 *
 **********************************************************************************/ 
#ifndef	G726_H
#define G726_H
#include "def_type.h"


#define	AUDIO_ENCODING_ULAW	(1)	/* ISDN u-law */
#define	AUDIO_ENCODING_ALAW	(2)	/* ISDN A-law */
#define	AUDIO_ENCODING_LINEAR	(3)	/* PCM 2's-complement (0-center) */

/*	wg add 8/30/2006	*/
#define G726FrameLenth	160

#define G726_DATA_TYPE_16K 4
#define G726_DATA_TYPE_24K 1
#define G726_DATA_TYPE_32K 2
#define G726_DATA_TYPE_40K 3

enum EG726TYPE  
{
    e_G726_16K,
    e_G726_24K=1,
    e_G726_32K=2,
    e_G726_40K=3,
    e_G726_16KT=4,
};


// CCITT G.726 prototypes 
// call by C files

VOID AppAudioG726EncodeInit(LONG lwChannelNum, LONG lwTalkNum, LONG lwAudioG726EnType, LONG lwAudioG729InputType);
VOID AppAudioG726DecodeInit(LONG lwChannelNum, LONG lwTalkNum, LONG lwAudioG726Type, LONG lwAudioG726OutputType);
LONG AppAudioG726Encode(
	LONG  	lwChannelNum,
	LONG 	lwTalkNum, 
	PBYTE   pcwG726EncodeInputData,
	BYTE    cwG726InputDataType,
	PBYTE   pcwG726EncodeOutData,
	LONG    lwAudioG726AudioFrameLen
);
LONG AppAudioG726Decode(
	LONG	lwChannelNum,
	LONG	lwTalkNum, 
	PBYTE   pcwG726DecodeInputData,
	BYTE    cwG726InputDataType,
	PBYTE   pcwG726DecodeOutData,
	LONG    lwAudioG726AudioFrameLen
);

#endif // !_VB_G726_H 
