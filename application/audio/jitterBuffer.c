#ifdef DBG
#include <stdio.h>
#endif

#include "jitterBuffer.h"
#include "os_cpu.h"
#include "os_cfg.h"
#include "uCOS_II.h"
#include "two_risc_audio.h"
#include "G726.h"
#include "audio.h"
#include "comm.h"
#include "libc_s.h"
#include "audio_cfg.h"

#ifdef DEBUG_AVSYNC
#ifdef TASK_STATISTIC_EN 
#include "two_risc_statistics.h"
#endif
#endif

#ifdef AUDIO_TEST_ENABLE
#include "test.h"
#endif

#include "timestamp.h"

BYTE cwJitterBuffer[JITTER_BUFFER_CHANNEL_MAX][JITTER_BUFFER_TALKER_MAX][JITTER_BUFFER_COUNT_MAX] = {0}; //Jitter Buffer
LONG lwJitterBufferReadIndex[JITTER_BUFFER_CHANNEL_MAX][JITTER_BUFFER_TALKER_MAX] = {0};	//Index point to the byte which will been read next time
LONG lwJitterBufferWriteIndex[JITTER_BUFFER_CHANNEL_MAX][JITTER_BUFFER_TALKER_MAX] = {0};	//Index point to the byte which will been written next time
LONG lwJitterBufferDepth[JITTER_BUFFER_CHANNEL_MAX][JITTER_BUFFER_TALKER_MAX] = {0};	//Valid data can be read, counted by LONG

BOOL bBuidUpFromEmpty[JITTER_BUFFER_CHANNEL_MAX][JITTER_BUFFER_TALKER_MAX];

//important parameter to adjust
LONG lwJitterBufferDepthMin;

LONG glwJitterBufferBuidUpFromEmptyDepthLimit[JITTER_BUFFER_CHANNEL_MAX][JITTER_BUFFER_TALKER_MAX] = {0};
LONG glwJTBBuildUpFromEmptyDelayNms[JITTER_BUFFER_CHANNEL_MAX][JITTER_BUFFER_TALKER_MAX] = {0};

LONG glwJitterBufferDropPacketFromDepth[JITTER_BUFFER_CHANNEL_MAX][JITTER_BUFFER_TALKER_MAX] = {0};
LONG glwJTBDropPacketWhenHoldDataNms[JITTER_BUFFER_CHANNEL_MAX][JITTER_BUFFER_TALKER_MAX] = {0};

//Audio data type
LONG lwAudioDataType = NOT_ITU_G729B;
////////////////////

#ifdef JTB_TIME_CHECK
//test1//////////////////
ULONG lwJTBWrToRdTime;
ULONG lwJTBRdToDspTime;
ULONG lwJTBTimeCheckState = 0;
ULONG lwJTBWrCheckError= 0;
ULONG lwJTBRdCheckError= 0;
#endif

//audio  buffer deffine
//extern LONG glwAudioOutputType; 
extern tAudioCharacter channelAC[MAX_AUDIO_CHANNEL];

#ifdef AUDIO_AID_DATA_ENABLE
extern tAudioAidData audioAidData[MAX_AUDIO_CHANNEL][MAX_TALKER_PER_CHANNEL];
#endif
/////////////////////////

VOID JitterBufferResetTalker(LONG lwChannelIndex, LONG lwTalkerIndex)
{
    lwJitterBufferReadIndex[lwChannelIndex][lwTalkerIndex] = 0;
    lwJitterBufferWriteIndex[lwChannelIndex][lwTalkerIndex] = 0;
    lwJitterBufferDepth[lwChannelIndex][lwTalkerIndex]= 0;
    bBuidUpFromEmpty[lwChannelIndex][lwTalkerIndex] = TRUE;
}

/* lwPacketOutPutDataCount: counted by LONG */
//LONG PacketIntoJitterBuffer(BYTE *pcwPacketOutPutData, LONG lwPacketOutPutDataCount, LONG lwChannelIndex, LONG lwTalkerIndex)
LONG PacketIntoJitterBuffer(tAudioTwoRiscShare *pAudioLinuxToUcos)
{
    LONG i;

#ifdef AUDIO_AID_DATA_ENABLE
    LONG lwAudioAidDataReadIndex;
    LONG lwAudioAidDataWriteIndex;
#endif
    
    BYTE *pcwPacketOutPutData;
    LONG lwPacketOutPutDataCount;
    LONG lwChannelIndex;
    LONG lwTalkerIndex;
    LONG lwTimeStamp;

    //Channel and Talker Number must be checked before.
    ///////////////////////////////////
    lwChannelIndex = pAudioLinuxToUcos->channelID;
    lwTalkerIndex = pAudioLinuxToUcos->talkerID;
    lwPacketOutPutDataCount = pAudioLinuxToUcos->len;
    pcwPacketOutPutData  = (BYTE *)pAudioLinuxToUcos->addr;

    lwJitterBufferDepth[lwChannelIndex][lwTalkerIndex] = GET_JITTER_BUFFER_DEPTH(lwChannelIndex, lwTalkerIndex);

    if (lwJitterBufferDepth[lwChannelIndex][lwTalkerIndex] < glwJitterBufferDropPacketFromDepth[lwChannelIndex][lwTalkerIndex])
    {
	if ((lwJitterBufferDepth[lwChannelIndex][lwTalkerIndex] + lwPacketOutPutDataCount) > glwJitterBufferDropPacketFromDepth[lwChannelIndex][lwTalkerIndex])
	{
#ifdef AUDIO_TEST_ENABLE
	    talker_info[lwChannelIndex][lwTalkerIndex].jtbuf_drop_pkg++;
#endif
	    // drop the packet
	    return -ERROR_JTBUF_DEPTH_WILL_OVERFLOW;
	}


#ifdef AUDIO_AID_DATA_ENABLE
	// write audioAidData
	if (audioAidData[lwChannelIndex][lwTalkerIndex].WriteIndex >= MAX_STORE_AUDIO_PACKET)
	{
	    audioAidData[lwChannelIndex][lwTalkerIndex].WriteIndex = 0;
	}

	lwAudioAidDataWriteIndex = audioAidData[lwChannelIndex][lwTalkerIndex].WriteIndex;
	
	audioAidData[lwChannelIndex][lwTalkerIndex].lwLength[lwAudioAidDataWriteIndex] = lwPacketOutPutDataCount;
	audioAidData[lwChannelIndex][lwTalkerIndex].lwTimeStamp[lwAudioAidDataWriteIndex] = pAudioLinuxToUcos->timeStamp;
	
	audioAidData[lwChannelIndex][lwTalkerIndex].WriteIndex++;
#endif
	

	//Add data to Jitter buffer tail

	for (i = 0; i < lwPacketOutPutDataCount; i++)
	{
		
	    if(lwJitterBufferWriteIndex[lwChannelIndex][lwTalkerIndex] >= JITTER_BUFFER_COUNT_MAX)
	    {
		lwJitterBufferWriteIndex[lwChannelIndex][lwTalkerIndex] = 0;

#ifdef JTB_TIME_CHECK
		//test1/////////
		RestRisc0Timer();
		if (lwJTBTimeCheckState == 0)
		{
		    lwJTBTimeCheckState = 1;
		}
		else
		{
		    lwJTBWrCheckError++;
		}
		///////////////////
#endif


	    }
	    cwJitterBuffer[lwChannelIndex][lwTalkerIndex][lwJitterBufferWriteIndex[lwChannelIndex][lwTalkerIndex]] = pcwPacketOutPutData[i];
	    lwJitterBufferWriteIndex[lwChannelIndex][lwTalkerIndex]++;	
	}
    }
    else
    {
#ifdef AUDIO_TEST_ENABLE
	talker_info[lwChannelIndex][lwTalkerIndex].jtbuf_drop_pkg++;
#endif
	// drop the packet
	return -ERROR_JTBUF_DEPTH_REACHED_MAX;
    }
    return i; //return the real write count.

}

//LONG FetchPackData(BYTE *pcwPacketOutPutData, LONG lwLength, LONG lwChannelIndex, LONG lwTalkerIndex, LONG lwReceiveTimeStamp)
LONG FetchPackData(tAudioTwoRiscShare *pAudioLinuxToUcos)
{
    BYTE *pcwPacketOutPutData;
    LONG lwLength;
    LONG lwChannelIndex;
    LONG lwTalkerIndex;
    
    if (pAudioLinuxToUcos->channelID > JITTER_BUFFER_CHANNEL_MAX - 1)
    {
	return -ERROR_JTBUF_AUDIO_CHANNEL_INDEX;
    }

    
    if (pAudioLinuxToUcos->talkerID > JITTER_BUFFER_TALKER_MAX - 1)
    {
	return -ERROR_JTBUF_AUDIO_TALKER_INDEX;
    }

    
#if 0
    if ((pAudioLinuxToUcos->len % sizeof(LONG)) != 0)
    {
	debug("%%!%d\n", pAudioLinuxToUcos->len);
	return -ERROR_JTBUF_FETCH_PACK_DATA_SIZE;
    }

    if ((gtAudioLinuxToUcos.addr % sizeof(LONG)) != 0)
    {
	debug("&!%x\n", gtAudioLinuxToUcos.addr);
	//		    return -ERROR_JTBUF_FETCH_PACK_ADDR_SIZE;
    }
#endif

    
    if ((((ULONG)pAudioLinuxToUcos->addr) >> 28) != 0xa)
    {
	debug("&A!%x,%x\n", pAudioLinuxToUcos->addr, (((ULONG)pAudioLinuxToUcos->addr) >> 24));
	return -ERROR_JTBUF_FETCH_PACK_ADDR_NOT_BEGIN_WITH_0XA;
    }

    return PacketIntoJitterBuffer(pAudioLinuxToUcos);
}

LONG ReadJitterBuffer(BYTE *pcwBuffer, LONG lwLength/*important! this should be byte size*/, LONG lwChannelIndex, LONG lwTalkerIndex)
{
    LONG lwTransferCount;
    LONG i;
    LONG lwCurrentTalkerJitterBufferDepth;
    LONG lwReadIndex;
    // OS_CPU_SR  cpu_sr;

    if (lwChannelIndex > JITTER_BUFFER_CHANNEL_MAX - 1)
    {
	return -ERROR_JTBUF_AUDIO_CHANNEL_INDEX;
    }

    if (lwTalkerIndex > JITTER_BUFFER_TALKER_MAX - 1)
    {
	return -ERROR_JTBUF_AUDIO_TALKER_INDEX;
    }

    //	if (lwLength%sizeof(LONG))
    //	{
    //		return -ERROR_JTBUF_LENGTH_TO_COUNT;
    //	}

    //OS_ENTER_CRITICAL();

    lwCurrentTalkerJitterBufferDepth = GET_JITTER_BUFFER_DEPTH(lwChannelIndex, lwTalkerIndex);	
#ifdef DEBUG_AVSYNC
    gtStatistics.audio_jitbuf_depth = lwCurrentTalkerJitterBufferDepth;
#endif
    if (lwCurrentTalkerJitterBufferDepth == 0)
    {
	bBuidUpFromEmpty[lwChannelIndex][lwTalkerIndex] = TRUE;
#ifndef DVR    
       printf("~0");
#endif
	return 0;
    }

    if (bBuidUpFromEmpty[lwChannelIndex][lwTalkerIndex])

    {
	if (lwCurrentTalkerJitterBufferDepth < glwJitterBufferBuidUpFromEmptyDepthLimit[lwChannelIndex][lwTalkerIndex])
	{
           printf("&%d,", lwCurrentTalkerJitterBufferDepth);	
	    return 0;
	}
	else
	{
	    bBuidUpFromEmpty[lwChannelIndex][lwTalkerIndex] = FALSE;
	}
    }

    lwTransferCount = lwLength;

    if (lwTransferCount > lwCurrentTalkerJitterBufferDepth)
    {
       printf("%%^");
	return 0;
    }

#ifdef AUDIO_AID_DATA_ENABLE
    //OK, Read audioAidData
    i = GET_BUFFER_VALID_SIZE(audioAidData[lwChannelIndex][lwTalkerIndex].ReadIndex, audioAidData[lwChannelIndex][lwTalkerIndex].WriteIndex, MAX_STORE_AUDIO_PACKET);
    if (i == 0)
    {
	debug("Error at Read Write Index of audioAidData, rIdx=%d wIdx=%d JTBDepth=%d\n", audioAidData[lwChannelIndex][lwTalkerIndex].ReadIndex, audioAidData[lwChannelIndex][lwTalkerIndex].WriteIndex, lwCurrentTalkerJitterBufferDepth);
    }

    if (audioAidData[lwChannelIndex][lwTalkerIndex].ReadIndex >= MAX_STORE_AUDIO_PACKET)
    {
	audioAidData[lwChannelIndex][lwTalkerIndex].ReadIndex = 0;
    }

    i = audioAidData[lwChannelIndex][lwTalkerIndex].ReadIndex;

    if(lwChannelIndex == 0)  //sometimes, there may exit several audio channels, and each has a different timestamp, so we choose channel0
        ReflashTimeStampRef(audioAidData[lwChannelIndex][lwTalkerIndex].lwTimeStamp[i]);
	
    audioAidData[lwChannelIndex][lwTalkerIndex].ReadIndex++;
#endif


    //OK, now let's transfer data from jitter buffer to user buffer.

#if 0
    for (i = 0; i < lwTransferCount; i++)
    {
	if(lwJitterBufferReadIndex[lwChannelIndex][lwTalkerIndex] >= JITTER_BUFFER_COUNT_MAX)
	{
	    lwJitterBufferReadIndex[lwChannelIndex][lwTalkerIndex] = 0;

#ifdef JTB_TIME_CHECK
		//test1/////////
		lwJTBWrToRdTime = (ULONG) GetRisc0Timer();
		if (lwJTBTimeCheckState == 1)
		{
		    lwJTBTimeCheckState = 2;
		}
		else
		{
		    lwJTBRdCheckError++;
		}
		RestRisc0Timer();
#endif
	}
	pcwBuffer[i] = cwJitterBuffer[lwChannelIndex][lwTalkerIndex][lwJitterBufferReadIndex[lwChannelIndex][lwTalkerIndex]];
	lwJitterBufferReadIndex[lwChannelIndex][lwTalkerIndex]++;	
    }

    //OS_EXIT_CRITICAL();
    return i;	//Return in byte size, because request input is in byte size
#endif

    lwReadIndex = lwJitterBufferReadIndex[lwChannelIndex][lwTalkerIndex];

    i = JITTER_BUFFER_COUNT_MAX - lwReadIndex;
    if (i > lwTransferCount)
    {
	memcpy(pcwBuffer, &cwJitterBuffer[lwChannelIndex][lwTalkerIndex][lwReadIndex], lwTransferCount);
	lwJitterBufferReadIndex[lwChannelIndex][lwTalkerIndex] += lwTransferCount;
    }
    else if (i == lwTransferCount)
    {
	memcpy(pcwBuffer, &cwJitterBuffer[lwChannelIndex][lwTalkerIndex][lwReadIndex], lwTransferCount);
	lwJitterBufferReadIndex[lwChannelIndex][lwTalkerIndex] = 0;
    }
    else
    {
	memcpy(pcwBuffer, &cwJitterBuffer[lwChannelIndex][lwTalkerIndex][lwReadIndex], i);
	lwReadIndex = 0;
	memcpy(pcwBuffer + i, &cwJitterBuffer[lwChannelIndex][lwTalkerIndex][lwReadIndex], (lwTransferCount - i));
	lwJitterBufferReadIndex[lwChannelIndex][lwTalkerIndex] = lwTransferCount - i;
    }
    
    //OS_EXIT_CRITICAL();

    return lwTransferCount;	//Return in byte size, because request input is in byte size
}

#if 1
void AudioDataUnderFlowProtection(LONG lwChannelIndex, LONG lwTalkerIndex)
{
    int i;
    unsigned int length, data[MIN_UNDERFLOW_PROTECTION * 2/4];	

    memset(data, 0xd5, MIN_UNDERFLOW_PROTECTION * 2);


    if (channelAC[lwChannelIndex].lwAudioOutputType == AUDIO_ENCODING_LINEAR) {
	length = MIN_UNDERFLOW_PROTECTION * 2;
    }	else
	length = MIN_UNDERFLOW_PROTECTION;

    i = AppAudioReadOutputCount(lwChannelIndex);

    if(i == -1) {
	debug("\nAppAudioReadOutputCount error!");		
	return;
    }

    if(i < length) {
	if(AppAudioWriteData(lwChannelIndex, data, length * 2) == SUCCESSFUL)
	    debug("?");
	return;		
    } 

    if(i < length * 2) {
	if(AppAudioWriteData(lwChannelIndex, data, length) == SUCCESSFUL)
	    debug("[");
    }

}
#endif
