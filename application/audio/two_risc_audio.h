/*
***************************************************************************
*                  Copyright (c) 2005 vBridge Microsystem, Inc.  
*                        Unpublished & Not for Publication
*                               All Rights Reserved                   
*   
* File        :                      
*                                                                         
* Description :    
*             
* Date        :                                                      
***************************************************************************
*/

#ifndef TWO_RISC_AUDIO_H
#define TWO_RISC_AUDIO_H

#ifdef   TWO_RISC_AUDIO_GLOBALS
#define  TWO_RISC_AUDIO_EXT
#else
#define  TWO_RISC_AUDIO_EXT  extern
#endif


#include "def_type.h"
#include "audio_cfg.h"
#include "os_cpu.h"
#include "os_cfg.h"
#include "uCOS_II.h"

/*
***************************************************************************
*                           MACRO Definitions
***************************************************************************
*/
#define MAX_CHANNEL_NUMBER    4    
#define CHANNEL_BUFFER_SIZE    0x2000

#define PRIOR_TwoRiscAudioTA    3

#define GXX_TYPE_G711A 1
#define GXX_TYPE_G711U 2
#define GXX_TYPE_G726 3
#define GXX_TYPE_G729A 4
#define GXX_TYPE_G729B 5
#ifdef AUDIO_G723_ENABLE
#define GXX_TYPE_G723_1_24_B 6
#define GXX_TYPE_G723_1_20_B 7
#endif

#define EC_TYPE_AEC 1
#define EC_TYPE_LEC 2
#define EC_TYPE_NO 3


#define AUDIO_DEVICE_SLIC 0
#define AUDIO_DEVICE_CODEC 1
#define AUDIO_DEVICE_TW2835 2

#define MAX_STORE_AUDIO_PACKET (50*5) //1s~50

/*
***************************************************************************
*                           Type Definitions
***************************************************************************
*/
typedef  struct {
    unsigned int channelID;
    unsigned int addr;
    unsigned int len;    
    unsigned int talkerID;
    unsigned int timeStamp;
} tAudioTwoRiscShare;

typedef struct {
    //channel Character
    BOOL bChannelEnable; //TRUE, FALSE
    LONG lwAudioDevice;  //AUDIO_DEVICE_SLIC AUDIO_DEVICE_CODEC
    LONG lwChannel; //0, 1, 2, 3
    LONG lwAudioInputPoints;     // gb: 711,729,726: 160; 723: 240
    LONG lwAudioInputBytes;
    LONG lwAudioInputType; //AUDIO_ENCODING_ALAW, AUDIO_ENCODING_ULAW, AUDIO_ENCODING_LINEAR
    LONG lwAudioOutputType; //AUDIO_ENCODING_ALAW, AUDIO_ENCODING_ULAW, AUDIO_ENCODING_LINEAR
    LONG lwHistoryTimeStampMs; 
    LONG lwECType;  //EC_TYPE_AEC, EC_TYPE_LEC, EC_TYPE_NO

    //talker Character
    LONG lwTalkerEnableBitFlag; //When the bit is 1, The talker is enabled. bit 0 for talker 0, ...bit 31 for talker 31
    LONG lwTalkerHaveSetCodecBitFlag; //Set Audio Codec should set these bits. bit 0 for talker 0, ...bit 31 for talker 31
    LONG lwTalkerMaskBitFlag; //When the bit is 1, The talker is sound masked. bit 0 for talker 0, ...bit 31 for talker 31
    LONG lwCurrentActiveTalkerCount; //Receive add one talker command + 1, Receive del one talker command -1
    LONG lwGxxType[MAX_TALKER_PER_CHANNEL]; //GXX_TYPE_G711A, GXX_TYPE_G711U, GXX_TYPE_G726, GXX_TYPE_G729A, GXX_TYPE_G729B
    //For G726
    LONG lwG726EncodeInputType[MAX_TALKER_PER_CHANNEL], lwG726EncodeOutputType[MAX_TALKER_PER_CHANNEL];
    LONG lwG726DecodeInputType[MAX_TALKER_PER_CHANNEL], lwG726DecodeOutputType[MAX_TALKER_PER_CHANNEL];
    //For Encode Decode
    LONG lwJitterBufferReadBytes[MAX_TALKER_PER_CHANNEL];
} tAudioCharacter;

#ifdef AUDIO_AID_DATA_ENABLE
typedef struct
{
    LONG lwGxxType[MAX_STORE_AUDIO_PACKET];
    LONG lwTimeStamp[MAX_STORE_AUDIO_PACKET];
    LONG lwLength[MAX_STORE_AUDIO_PACKET];

    LONG ReadIndex;
    LONG WriteIndex;
    
} tAudioAidData; //One tAudioAidData per talker
#endif


#ifdef DUMP_ENABLE
#define MAX_DUMP_POINTS 10
#define DUMP_TO_ADDR 0xa1100000
#define DUMP_STRUCT_ADDR_POINTER 0xa10ff000
#define DUMP_PERMIT_RAM_FLAG_ADDR 0xa10fe000
#define DUMP_PERMIT_VALUE 0xa5a5a5a5
#define DUMP_NOT_PERMIT_VALUE 0x0
#define DUMP_WAIT_S 5 

typedef struct{
    BYTE *pcwStart;
    BOOL bDumpFinish;
    BOOL bDumpSetted;
    LONG lwDumpOneTimeSize;
    LONG lwDumpTimes;
    LONG lwDumpedTimes;
    LONG lwDumpDelayTimes;
}tDumpContext;
#endif
/*
***************************************************************************
*                           GLOBAL VARIABLES
***************************************************************************
*/
TWO_RISC_AUDIO_EXT unsigned int *audio_ucos_to_linux_addr;

TWO_RISC_AUDIO_EXT unsigned int *audio_linux_to_ucos_addr;

TWO_RISC_AUDIO_EXT unsigned int *video_tx_share_addr_wr;

#define STK_SIZE_TwoRiscAudioTA 0x6000

TWO_RISC_AUDIO_EXT unsigned int StkTwoRiscAudioTA[STK_SIZE_TwoRiscAudioTA];

TWO_RISC_AUDIO_EXT  OS_EVENT *gptAudioTaskReady;

TWO_RISC_AUDIO_EXT  OS_SEM_DATA gtAudioTaskReady;

TWO_RISC_AUDIO_EXT tAudioTwoRiscShare gtAudioLinuxToUcos;

TWO_RISC_AUDIO_EXT tAudioTwoRiscShare gtAudioUcosToLinux;

//extern tAudioCharacter channelAC[MAX_AUDIO_CHANNEL];

/*
***************************************************************************
*                              FUCTION PROTYPES
***************************************************************************
*/
void TaskTwoRiscAudioLoopBackRisc0(void *data);
void TwoRiscAudioTARisc1(void *data);
void TwoRiscAudioTA(void *data);

VOID AppMassALaw2Linear(LONG *lwALawBuffer, LONG *lwLinearBuffer, LONG lwALawPoints);
VOID AppMassULaw2Linear(LONG *lwULawBuffer, LONG *lwLinearBuffer, LONG lwULawPoints);
VOID AppMassLinear2ULaw(LONG *lwLinearBuffer, LONG *lwULawBuffer, LONG lwLinearPoints);
VOID AppMassLinear2ALaw(LONG *lwLinearBuffer, LONG *lwALawBuffer, LONG lwLinearPoints);

VOID AppInitChannelEx(LONG i);
VOID AppInitTalker(LONG lwChannel, LONG lwTalker);

#ifdef DUMP_ENABLE
VOID AppResetDumpContext(LONG i);
VOID AppSetDumpContext(LONG i, LONG lwDumpOneTimeSize, LONG lwDumpTimes, LONG lwDumpDelayTimes);
VOID AppDumpThem(LONG i, VOID *from);
VOID AppReadDumpedData(LONG i);
LONG AppCopyDumpedContext(LONG i);
#endif

#endif
