#ifndef JITTERBUFFER_H 
#define JITTERBUFFER_H

//#include "typedef.h"
#include "def_type.h"
#include "risc.h"
#include "audio_cfg.h"
#include "two_risc_audio.h"

#define JITTER_BUFFER_CHANNEL_MAX MAX_AUDIO_CHANNEL
#define JITTER_BUFFER_TALKER_MAX MAX_TALKER_PER_CHANNEL

#define JITTER_BUFFER_BYTES_MAX (8*1000)
#define JITTER_BUFFER_COUNT_MAX JITTER_BUFFER_BYTES_MAX


#define MIN_UNDERFLOW_PROTECTION    160    // counted by bytes!

#ifdef UCOS_USE_AUDIO_JITBUF
#define JTB_BUILD_UP_DELAY_DEFAULT_MS 100   //Jitter Buffer build up from empty, delay to play voice n ms.
#define JTB_MAX_HOLD_DATA_DEFAULT_MS 300 //When jitter buffer holds audio data reach to JTB_MAX_HOLD_DATA_DEFAULT_MS, it will begin to drop the packet.
#else
#define JTB_BUILD_UP_DELAY_DEFAULT_MS 60
#define JTB_MAX_HOLD_DATA_DEFAULT_MS 60
#endif
//Audio data type
#define NOT_ITU_G729B 0x711
#define ITU_G729B 0x729b
////////////////////

//ERROR DFINE
#define ERROR_JTBUF_AUDIO_CHANNEL_INDEX 1
#define ERROR_JTBUF_DEPTH_WILL_OVERFLOW 2
#define ERROR_JTBUF_DEPTH_REACHED_MAX 3
#define ERROR_JTBUF_LENGTH_TO_COUNT 4
#define ERROR_JTBUF_FETCH_PACK_DATA_SIZE 5
#define ERROR_JTBUF_FETCH_PACK_ADDR_SIZE 6
#define ERROR_JTBUF_FETCH_PACK_ADDR_NOT_BEGIN_WITH_0XA 7
#define ERROR_JTBUF_AUDIO_TALKER_INDEX 8

#define GET_BUFFER_VALID_SIZE(READ_INDEX, WRITE_INDEX, BUFFER_SIZE) \
		(WRITE_INDEX + BUFFER_SIZE - READ_INDEX)%BUFFER_SIZE 


#define GET_JITTER_BUFFER_DEPTH(CHANNEL, TALKER) \
	    GET_BUFFER_VALID_SIZE(lwJitterBufferReadIndex[CHANNEL][TALKER], lwJitterBufferWriteIndex[CHANNEL][TALKER], JITTER_BUFFER_COUNT_MAX)


LONG PacketIntoJitterBuffer(tAudioTwoRiscShare *pAudioLinuxToUcos);
LONG FetchPackData(tAudioTwoRiscShare *pAudioLinuxToUcos);
LONG ReadJitterBuffer(BYTE *pcwBuffer, LONG lwLength/*important! this should be byte size*/, LONG lwChannelIndex, LONG lwTalkerIndex);

void AudioDataUnderFlowProtection(LONG lwChannelIndex, LONG lwTalkerIndex);

VOID JitterBufferResetTalker(LONG lwChannelIndex, LONG lwTalkerIndex);

#endif
