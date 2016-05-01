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
#define TWO_RISC_AUDIO_GLOBALS

#define AEC_AUDIO_FarNear_STEP 255

//
//echo ref test/////////////
#define	ECHO_REF_TEST_USING_AEC
//#undef	ECHO_REF_TEST_USING_AEC
#define ECHO_REF_TEST
#undef ECHO_REF_TEST
#ifdef ECHO_REF_TEST
#ifdef	ECHO_REF_TEST_USING_AEC
#include "ref.h"
#include "echo.h"
#else
#include "ref.h"
#include "echo.h"
#endif
#endif
////////////////


#include "risc.h"
#include "Sc.h"
#include "Mc.h"
#include "comm.h"
#include "except.h"

#include "../port/os_cpu.h"
#include "../uCOS Configure/os_cfg.h"
#include "../uCOS2.52/uCOS_II.h"

#include "two_risc_comm.h"
#include "two_risc_system.h"
#include "2risc_memory_map.h"
#include "2risc_comm_protocol.h"
#include "two_risc_audio.h"
#include "audio_api.h"

#include "../bsp/bsp.h"

#include "cctl.h"

#include "string.h"

#include "audio.h"
#include "jitterBuffer.h"
#include "../G726/G726.h"
#include "../G729/G729.h"
#include "../G711/G711.h"
#ifdef AUDIO_G723_ENABLE
#include "../G723/G723.h"
#endif

#include "../AEC/AEC.h"
#include "../LEC/lec.h"

//slic
#include "proslic.h"
//#include "two_risc_slic.h"
///////
#include "audio_cfg.h"

#include "timestamp.h"
#include "two_risc_statistics.h"
#ifdef AUDIO_TEST_ENABLE
#include "test.h"
#endif

#ifdef CODEC_TLV320AIC23B
#include "aic23.h"
#endif

#ifdef INCLUDE_WINBOND_SLIC
#include "w681388.h"
#else
#include "proslic.h"
#endif
#ifdef MIPS_CHECK
    LONG lwCountTask1 = 0;
    LONG lwCountTask0 = 0;
    LONG lwMaxCountTask = 0;

    LONG lwCountOneChannel1 = 0;
    LONG lwCountOneChannel0 = 0;
    LONG lwMaxCountOneChannel = 0;
   
    LONG lwCountEC1 = 0;
    LONG lwCountEC0 = 0;
    LONG lwMaxCountEC = 0;
    
    LONG lwCountGXXD1 = 0;
    LONG lwCountGXXD0 = 0;
    LONG lwMaxCountGXXD = 0;
    
    LONG lwCountGXXE1 = 0;
    LONG lwCountGXXE0 = 0;
    LONG lwMaxCountGXXE = 0;
#endif

static int no_rx = 0;
static int no_tx = 0;

unsigned int audioTs = 0;

tAudioCharacter channelAC[MAX_AUDIO_CHANNEL];

#ifdef AUDIO_AID_DATA_ENABLE
tAudioAidData audioAidData[MAX_AUDIO_CHANNEL][MAX_TALKER_PER_CHANNEL];
#endif

static LONG glwChannelIndex=0;
LONG lwFromTalkerDecoded16BitLinearData[MAX_TALKER_PER_CHANNEL][AUDIO_MAX_FRAME_LEN * 2];
BOOL bActiveTalker[MAX_TALKER_PER_CHANNEL];
LONG *lwNeedMixPointer[MAX_TALKER_PER_CHANNEL + 1]; //This array save 16 bit linear audio data address which need to be mixed, the last one must be 0

LONG glwInterruptTimeScale = 2; //for 10ms, 1; for 20ms, 2; for 30ms, 3
LONG glwCount_RiscVCNT_ISR_times = 0;

#ifdef MIPS_CHECK
LONG CountSpend(LONG lwCount1, LONG lwCount0)
{
    if (lwCount1 < lwCount0)
    { 
	lwCount1 += (CPU_MAIN_CLOCK / (2 * OS_TICKS_PER_SEC)) - lwCount0;
    }
    else
    {
	lwCount1 -= lwCount0;
    }
    return (lwCount1 * 2 / glwInterruptTimeScale);
}
#endif

#ifdef VER001
/*
 ***************************************************************************
 *                         TaskTwoRiscAudioLoopBackRisc0
 *
 * Description: 
 *
 * Arguments  : 
 *
 * Return     : 
 *
 * Note(s)    : 
 ***************************************************************************
 */
void TaskTwoRiscAudioLoopBackRisc0(void *data)
{
    int i = 0;		
    unsigned int rec[16];	
    unsigned int len, *pInt;

    BspStartTicker(OS_TICKS_PER_SEC); 	    

    //rRISC_GPIO_DIR &= ~0x4000000; /* Set GPIO26 output*/
    //rRISC_GPIO_DIR &= ~0x2000000; /* Set GPIO25 output*/  

    //OSTaskCreate(TaskLed, (void *)0, (void *)&StkTaskLed[TASK_LED_STK_SIZE - 1], 11);       

    RiscClrCNT(AUDIO_UCOS_TO_LINUX_CNT);                    

    debug("\r\n");
    debug("**********************************************************************\r\n");
    debug("*                 Risc%d Audio Loop Back Test                         *\r\n", RiscRdID());    
    debug("**********************************************************************\r\n");     
    debug("\r\n");

    RiscIntrInit();
    TwoRiscCommInit();     
    RiscFlagIntrInit();

    // please un-call this function in sigle-cpu mode
    RiscSemPend(SEM_UCOS_AUDIO_READY);    // RISC1 will post the semaphore when it is ready to receive audio data.

    TestAudioLoopBack();
    while(1);    

    while(1){    	    	   	

	if(TwoRiscCommRecPacket() != SUCCESSFUL) {
	    //debug("\r\nrisc0 rec NOT ok\r\n");	
	    //OSTimeDlyHMSM(0, 0, 0, 10);
	    continue;
	}

	//debug("\r\nrisc0 rec %d ok\r\n", no_rx++);

	len = *(rec + 4);    
	pInt = (unsigned int *)(*(rec + 3));        

	for(i = 0; i < len; i+=4) {
	    if((unsigned int)pInt == AUDIO_UCOS_TO_LINUX_E_ADDR)
		pInt = (unsigned int *)AUDIO_UCOS_TO_LINUX_B_ADDR; 
	    McWrQueue(AUD_OUT_CH0_M, *pInt++);    	
	}

	RiscDecCNT(AUDIO_LINUX_TO_UCOS_CNT, len / AUDIO_MAX_FRAME_LEN);

	//OSTimeDlyHMSM(0, 0, 0, 10); 
    }
}
#endif

/*
 ***************************************************************************
 *                              TwoRiscAudioTARisc1
 *
 * Description: 
 *
 * Arguments  : 
 *
 * Return     : 
 *
 * Note(s)    : 
 ***************************************************************************
 */
void TwoRiscAudioTARisc1(void *data)
{
    char receive;
    OS_CPU_SR cpu_sr;	

    unsigned int rec[16];	
    unsigned int i  = 0, count, len;
    unsigned int *pInt; 
    unsigned int *pTmp; 
    Risc_Packet_t risc_packet;      
    
    audio_linux_to_ucos_addr = (unsigned int *)AUDIO_UCOS_TO_LINUX_B_ADDR;

    BspStartTicker(OS_TICKS_PER_SEC); 	    

    debug("\r\n");
    debug("**********************************************************************\r\n");
    debug("*                 Risc%d TwoRiscAudioTARisc1                        *\r\n", RiscRdID());    
    debug("**********************************************************************\r\n");     
    debug("\r\n");            



    while(1) {
	if(CheckPressKey(&receive)) {
           printf("%c", receive);		
	    switch(receive) {			
		case '!':
		    debug("You have pressed a key to stop!");
		    OS_ENTER_CRITICAL();
		    while(1);
		    break;
		    
		case 't':
		    debug("You have pressed a key: %c\n",receive);
		    break;

		case 's':    // start voice	
		    risc_packet.start = RISC_PKT_START;
		    risc_packet.main_type = tAudio;
		    risc_packet.sub_type = tAudioStartVoice;
		    risc_packet.length = 0x4;
		    risc_packet.status = 0;		
		    risc_packet.end = RISC_PKT_END;		

		    if(TwoRiscCommSendPacket((unsigned int*)(&risc_packet), RPACKET_LEN) == SUCCESSFUL) {
			RiscFlagSet();    // Set risc1 flag to interrupt risc1            
		    } else 
			debug("\r\nTwoRiscAudioTARisc1: send packet error!\r\n");            
		    break;

		case 'e':    // stop voice	
		    risc_packet.start = RISC_PKT_START;
		    risc_packet.main_type = tAudio;
		    risc_packet.sub_type = tAudioStopVoice;
		    risc_packet.length = 0x4;
		    risc_packet.status = 0;		
		    risc_packet.end = RISC_PKT_END;		

		    if(TwoRiscCommSendPacket((unsigned int*)(&risc_packet), RPACKET_LEN) == SUCCESSFUL) {
			RiscFlagSet();    // Set risc1 flag to interrupt risc1            
		    } else 
			debug("\r\nTwoRiscAudioTARisc1: send packet error!\r\n");            
		    break;

		default:
		    break; 		
	    }					

	}

	if(i++ == 20) {   // 10 secondss
	    debug("\r\n                           TwoRiscAudioTARisc1!");
	    i = 0;
	} 

	OSTimeDlyHMSM(0, 0, 0, 500);    
    }
}


//#define VER001

#ifdef VER001

/*
 ***************************************************************************
 *                         TwoRiscAudioTA
 *
 * Description: 
 *
 * Arguments  : 
 *
 * Return     : 
 *
 * Note(s)    : 
 ***************************************************************************
 */
void TwoRiscAudioTA(void *data)
{
    int i = 0;		
    unsigned char err;	
    LONG plwBuffer[AUDIO_MAX_FRAME_LEN / sizeof(LONG)];
    unsigned int *pTmp = AUDIO_UCOS_TO_LINUX_B_ADDR;	
    Risc_Packet_t risc_packet;	
    unsigned int audio_in, *pPacket;


    tAudioChannelPara tPara;    

    BspStartTicker(OS_TICKS_PER_SEC); 	                 

    debug("\r\n");
    debug("**********************************************************************\r\n");
    debug("*                 Risc%d Audio TA Task                              *\r\n", RiscRdID());    
    debug("**********************************************************************\r\n");     
    debug("\r\n");

    audio_ucos_to_linux_addr = (unsigned int *)AUDIO_UCOS_TO_LINUX_B_ADDR;

    gptAudioTaskReady = OSSemCreate(0);

    RiscClrCNT(AUDIO_UCOS_TO_LINUX_CNT);                    

    /* init jitter buffer */
    JitterBufferResetAllChannel();

    /* config and start audio */
    tPara.time_input_slot_number = 0;
    tPara.time_output_slot_number = 0;	
    tPara.time_slot_count = 1;
    tPara.prefetch = 1;

    AppAudioClose();    // add on Sept. 19

    if(AppAudioGetStatus() == AUDIO_CLOSED)
	debug("\r\nTaskTwoRiscSystemMonitor: audio was closed!");    
    else
	debug("\r\nTaskTwoRiscSystemMonitor: audio was opened!"); 		

    AppAudioTimingSet();
    AppAudioChannelOpen(0 /* channel_number */, &tPara);
    AppAudioOpen();

    if(AppAudioGetStatus() == AUDIO_CLOSED)
	debug("\r\nTaskTwoRiscSystemMonitor: audio was closed!");    
    else
	debug("\r\nTaskTwoRiscSystemMonitor: audio was opened!"); 		

    while(1) {    	    	
	OSSemPend(gptAudioTaskReady, 0, &err);

	//debug("\r\nTwoRiscAudioTA: OSSemPend(gptAudioTaskReady err = %d\r\n", err);

	//continue;	


	i = ReadJitterBuffer(plwBuffer, AUDIO_MAX_FRAME_LEN, 0 /* channel_number */);

	AppAudioWriteData(0 /* channel_number */, plwBuffer, i);	

	pPacket = &risc_packet;

	// read from audio input, and write into the share memory        
	risc_packet.start = RISC_PKT_START;
	risc_packet.main_type = tAudio;
	risc_packet.sub_type = tAudioTransferUcosToLinux;
	risc_packet.length = 0x10;
	risc_packet.status = 0;
	*(pPacket + 2) = 0;
	*(pPacket + 3) = (int)pTmp;
	*(pPacket + 4) = AUDIO_MAX_FRAME_LEN;		
	*(pPacket + 5) = 73;		
	//risc_packet.message[0] = 0;
	//risc_packet.message[1] = (int)pTmp;
	//risc_packet.message[2] = AUDIO_MAX_FRAME_LEN;		
	risc_packet.end = RISC_PKT_END;

	for(i = 0; i < AUDIO_MAX_FRAME_LEN; i+=4) {    	
	    ScRd(SOFT_RD_1_S, &audio_in);            
	    *pTmp++ = audio_in; 
	    if((int)pTmp == AUDIO_UCOS_TO_LINUX_E_ADDR)
		pTmp = (unsigned int *)AUDIO_UCOS_TO_LINUX_B_ADDR;           	                
	}

	if(TwoRiscCommSendPacket((unsigned int *)(&risc_packet), RPACKET_LEN) == SUCCESSFUL) {
	    RiscIncCNT(AUDIO_UCOS_TO_LINUX_CNT, 1);    // increase count by 1
	    RiscFlagSet();    // Set risc1 flag to interrupt risc1        
	    //debug("\r\nTwoRiscAudioTA: send packet OK!\r\n");
	} else 
	    debug("\r\nTwoRiscAudioTA: send packet error!\r\n");		

    }
}

#else
static LONG CalTsDif(tAudioCharacter *tAC)
{
    LONG i;
    LONG ts;
    LONG chID;

   switch(tAC->lwChannel) {
        case 0:                               
            chID = AUD_OUT_CH0_M;        
            break;
            
        case 1:                               
            chID = AUD_OUT_CH1_M;        
            break;            

        case 2:                               
            chID = AUD_OUT_CH2_M;        
            break;

        case 3:                               
            chID = AUD_OUT_CH3_M;        
            break;                        
            
        default:
            return FAILED;
            break;
    }
    
    i = McRdQueueCount(chID);
    if(tAC->lwG726DecodeOutputType == AUDIO_ENCODING_LINEAR)
        ts = i / (AUDIO_MAX_FRAME_LEN * 2);
    else
        ts = i / AUDIO_MAX_FRAME_LEN;

    return ts;
}


VOID AppReadTalkerJTBFail16BitLinearProtect(LONG * plwOutPutBuffer, LONG lwPoints)
{
    memset(plwOutPutBuffer, 0, lwPoints * 2);
}

LONG App16BitLinearMixer(LONG **ppData, LONG *plwMixResult, LONG lwPoints)
{
    LONG i;
    LONG j = 0;
    BOOL bFirstFlag = FALSE;

    SHORT *pswSource;
    SHORT *pswResult;

    LONG lwTempResult;

    pswResult = (SHORT *) plwMixResult;

    while (1)
    {
	if (*ppData == 0)
	{
	    break;
	}

	if (!bFirstFlag)
	{
	    memcpy(pswResult, *ppData, lwPoints * sizeof(SHORT));
	    bFirstFlag = TRUE;
	    ppData++;
	    j++;
	    continue;
	}

	pswSource = (SHORT *) *ppData;
	for (i = 0; i < lwPoints; i++)
	{
	    lwTempResult = (LONG) pswResult[i] + (LONG) (*pswSource);
	    
	    if (lwTempResult > 0x00007fffL)
	    {
		pswResult[i] = (SHORT)0x7fff;
	    }
	    else if (lwTempResult < (LONG)0xffff8000L)
	    {
		pswResult[i] = (SHORT)0x8000;
	    }
	    else
	    {
		pswResult[i] = (SHORT)(lwTempResult);
	    }
		     
	    pswSource++;
	}
	ppData++;
	j++;
    }
    return j;
}

LONG AppAudioDecode(tAudioCharacter *tAC, LONG *lwBuff)
{
    LONG lwJitterBufferDataReady;
    BYTE cwAudioBeforeDecodeData[AUDIO_MAX_FRAME_LEN * 4 * 2];
    BYTE cwAudioAfterDecodeData[AUDIO_MAX_FRAME_LEN * 4 * 2];
    BYTE *pcwTemp;
    SHORT *pswTemp;
    LONG lwAudioOutputLength;

    LONG lwTalkerIndex;
    LONG i;
    LONG j;
    
    i = 0;

    for (lwTalkerIndex = 0; lwTalkerIndex < MAX_TALKER_PER_CHANNEL; lwTalkerIndex++)
    {
	if (!bActiveTalker[lwTalkerIndex])
	{
	    continue;
	}
	
	lwJitterBufferDataReady  = ReadJitterBuffer(cwAudioBeforeDecodeData, tAC->lwJitterBufferReadBytes[lwTalkerIndex], tAC->lwChannel /* channel_number */, lwTalkerIndex /* talker id */);
       j = CalTsDif(tAC);
       AdjustTsRef(j, 1);

	if (lwJitterBufferDataReady > 0)
	{

	    //test1///////////
#ifdef DUMP_ENABLE
	    AppSetDumpContext(lwTalkerIndex, lwJitterBufferDataReady, 50*10, 50*DUMP_WAIT_S);
	    AppDumpThem(lwTalkerIndex, cwAudioBeforeDecodeData);
#endif
	    //////////////////

#ifdef MIPS_CHECK
	    lwCountGXXD0 = GetCount();
#endif
	    switch (tAC->lwGxxType[lwTalkerIndex]) 
	    {
		case GXX_TYPE_G726: 
		    AppAudioG726Decode(tAC->lwChannel, lwTalkerIndex, cwAudioBeforeDecodeData, 0, (PBYTE)lwFromTalkerDecoded16BitLinearData[lwTalkerIndex], lwJitterBufferDataReady); 
		    break;

		case GXX_TYPE_G729A: 
		    pcwTemp = (PBYTE)cwAudioBeforeDecodeData; 
		    pswTemp = (PSHORT)lwFromTalkerDecoded16BitLinearData[lwTalkerIndex]; 
		    AppAudioG729Decode(tAC->lwChannel, lwTalkerIndex, pcwTemp, pswTemp, 10); 

		    pcwTemp += 10; 
		    pswTemp += 160/2; 
		    AppAudioG729Decode(tAC->lwChannel, lwTalkerIndex, pcwTemp, pswTemp, 10); 
		    break; 

		case GXX_TYPE_G729B: 
		    break; 

		case GXX_TYPE_G711A: 
		    AppMassALaw2Linear((LONG*)cwAudioBeforeDecodeData, (LONG*)lwFromTalkerDecoded16BitLinearData[lwTalkerIndex], AUDIO_MAX_FRAME_LEN * glwInterruptTimeScale);
		    break;

		case GXX_TYPE_G711U: 
		    AppMassULaw2Linear((LONG*)cwAudioBeforeDecodeData, (LONG*)lwFromTalkerDecoded16BitLinearData[lwTalkerIndex] , AUDIO_MAX_FRAME_LEN * glwInterruptTimeScale);
		   // memcpy(cwAudioAfterDecodeData, cwAudioBeforeDecodeData, lwJitterBufferDataReady);
		    //test1///////////
		    //AppSetDumpContext(0, channelAC[glwChannelIndex].lwAudioInputBytes, 50*10);
		    //AppDumpThem(0, cwAudioAfterDecodeData);
		    //////////////////
		    break;

#ifdef AUDIO_G723_ENABLE
		case GXX_TYPE_G723_1_24_B:
		    //AppAudioG726Decode(tAC->lwChannel, lwTalkerIndex, cwAudioBeforeDecodeData, 0, (PBYTE)lwFromTalkerDecoded16BitLinearData[lwTalkerIndex], lwJitterBufferDataReady); 
		    AppAudioG723Decode((PSHORT)lwFromTalkerDecoded16BitLinearData[lwTalkerIndex], (CHAR*)cwAudioBeforeDecodeData, 0, 0);
		    break;

		case GXX_TYPE_G723_1_20_B:
		    AppAudioG723Decode((PSHORT)lwFromTalkerDecoded16BitLinearData[lwTalkerIndex], (CHAR*)cwAudioBeforeDecodeData, 0, 1);
		    break;
#endif
		default: 
		    break; 
	    }
	    //test1///////////
#ifdef DUMP_ENABLE
	    AppSetDumpContext(lwTalkerIndex + 2, AUDIO_MAX_FRAME_LEN*2, 50*10, 50*DUMP_WAIT_S);
	    AppDumpThem(lwTalkerIndex + 2, lwFromTalkerDecoded16BitLinearData[lwTalkerIndex]);
#endif
	    //////////////////

#ifdef MIPS_CHECK 
	    lwCountGXXD1 = GetCount();
#endif 
	}
	else
	{
extern int LinuxToUcosATs[];	
#ifndef DVR
            printf("\n*%d,", LinuxToUcosATs[0]);
#endif
	    AppReadTalkerJTBFail16BitLinearProtect(lwFromTalkerDecoded16BitLinearData[lwTalkerIndex], AUDIO_MAX_FRAME_LEN * glwInterruptTimeScale);

#ifdef AUDIO_TEST_ENABLE
	    talker_info[tAC->lwChannel][lwTalkerIndex].linear_uf_protect++;
#endif 
	    //test1///
	    //debug("=%d-%d",tAC->lwChannel, lwTalkerIndex);
	    /////////
	}

	lwNeedMixPointer[i] = lwFromTalkerDecoded16BitLinearData[lwTalkerIndex];
	i++;
    }

    lwNeedMixPointer[i] = 0;

    App16BitLinearMixer(lwNeedMixPointer, (LONG*)cwAudioAfterDecodeData, AUDIO_MAX_FRAME_LEN * glwInterruptTimeScale);

    //test1///////////
#ifdef DUMP_ENABLE
    AppSetDumpContext(4, AUDIO_MAX_FRAME_LEN*2, 50*10, 50*DUMP_WAIT_S);
    AppDumpThem(4, cwAudioAfterDecodeData);
#endif
    //////////////////

    //from linear to alaw or ulaw
    switch (tAC->lwAudioOutputType)
    { 
	case AUDIO_ENCODING_LINEAR: 
	    memcpy(lwBuff, cwAudioAfterDecodeData, AUDIO_MAX_FRAME_LEN * 2 * glwInterruptTimeScale); 
	    lwAudioOutputLength = AUDIO_MAX_FRAME_LEN * 2 * glwInterruptTimeScale; 
	    break; 
	case AUDIO_ENCODING_ALAW: 
	    AppMassLinear2ALaw((LONG*) cwAudioAfterDecodeData, (LONG*) lwBuff, AUDIO_MAX_FRAME_LEN * glwInterruptTimeScale); 
	    lwAudioOutputLength = AUDIO_MAX_FRAME_LEN * glwInterruptTimeScale; 
	    break; 
	default:	//AUDIO_ENCODING_ULAW 
	    AppMassLinear2ULaw((LONG*) cwAudioAfterDecodeData, (LONG*) lwBuff, AUDIO_MAX_FRAME_LEN * glwInterruptTimeScale); 
	    lwAudioOutputLength = AUDIO_MAX_FRAME_LEN * glwInterruptTimeScale; 
	    break; 
    } 

    return lwAudioOutputLength;
}



LONG AppReadAudioInput(tAudioCharacter *tAC, LONG *lwBuff)
{
    LONG lwReadAudioBuff[AUDIO_MAX_FRAME_LEN * 2];
    LONG *plwTemp;
    LONG i, j;
    LONG lwReadNByte;
    
    lwReadNByte = tAC->lwAudioInputBytes;


    if (tAC->lwAudioInputType == AUDIO_ENCODING_LINEAR)
    {
	plwTemp = (LONG*)lwBuff; 
    }
    else 
    { 
	plwTemp = lwReadAudioBuff; 
    }

    // read audio input raw data 
    if(AppAudioReadData(tAC->lwChannel /* channel_number */, plwTemp, lwReadNByte /* length */) == FAILED)
    {
	return 0;
    }
    else  //added by wy
    {
        i = McRdQueueCount(tAC->lwChannel) * glwInterruptTimeScale / lwReadNByte ;  // get how many frames in the queue
        j = GetSysTimeStamp() - i;   //
        if(j < i)
            printf("audio timestamp error!(%d,%d)\n", j, i);
        if(audioTs >= j)
            printf("ucos to linux timestamp error(%d,%d)~~~~~~~~!\n", audioTs, j);
        else if(j >= (audioTs + glwInterruptTimeScale * 2))
            printf("ucos to linux timestamp lost frame(%d,%d)~~~~~~~!\n", audioTs, j);
        audioTs = j;    
    }

    
    //test1///////////
#ifdef DUMP_ENABLE
    AppSetDumpContext(5, channelAC[glwChannelIndex].lwAudioInputBytes, 50*10, 50*DUMP_WAIT_S);
    AppDumpThem(5, plwTemp);
#endif
    //////////////////

    //alaw and ulaw to linear 
    if (tAC->lwAudioInputType == AUDIO_ENCODING_ALAW)
    { 
	AppMassALaw2Linear(plwTemp, (LONG*) lwBuff, lwReadNByte);
	return lwReadNByte * 2;
    }
    else if (tAC->lwAudioInputType == AUDIO_ENCODING_ULAW)
    { 
	AppMassULaw2Linear(plwTemp, (LONG*) lwBuff, lwReadNByte);
	return lwReadNByte * 2;
    }

    return lwReadNByte;

    //echo ref test/////////////
#ifdef ECHO_REF_TEST
    static char* pEcho = echo;
    static char* pRef = ref;

    memcpy(lwAudioInput16BitLinearData, pEcho, 320);
    memcpy(lwAudioInputFar16BitLinearData, pRef, 320);
    pEcho += 320;
    pRef += 320;
    if (glwECType == EC_TYPE_AEC)
    { 
	if (pEcho == echo + 320000)
	    pEcho = echo;
	if (pRef == ref + 320000)
	    pRef = ref;
    }
    else
    {
	if (pEcho == echo + 320000)
	    pEcho = echo;
	if (pRef == ref + 320000)
	    pRef = ref;
    }

    goto HERE2;
#endif

}


BOOL AppRunEC(tAudioCharacter *tAC, LONG *lwAudioInput16BitLinearData, LONG *lwAudioBeforeEncodeData)
{
    // lwNearDataNByte  must be right!
    int i;
    LONG lwAudioInputFar16BitLinearData[AUDIO_MAX_FRAME_LEN * 2]={0};
    LONG lwAudioInputFarData[AUDIO_MAX_FRAME_LEN * 2];
    LONG lwLinearBytes;
    BYTE *pcwTemp;
    SHORT *pswTemp;
    SHORT *pswTemp1;
    LONG lwNearDataNByte;
    lwNearDataNByte = tAC->lwAudioInputBytes;

    if (tAC->lwECType == EC_TYPE_NO)
    {
	memcpy(lwAudioBeforeEncodeData, lwAudioInput16BitLinearData, AUDIO_MAX_FRAME_LEN * 2 * glwInterruptTimeScale);
	return TRUE;
    }

    if (tAC->lwAudioInputType == AUDIO_ENCODING_LINEAR)
    { 
	lwLinearBytes = lwNearDataNByte;
	i = AppAudioReadHistoryData(tAC->lwChannel, lwAudioInputFar16BitLinearData, lwNearDataNByte); 
    } 
    else 
    { 
	lwLinearBytes = lwNearDataNByte * 2;
	i = AppAudioReadHistoryData(tAC->lwChannel, lwAudioInputFarData, lwNearDataNByte); 
    } 

    if (i != lwNearDataNByte)
    {
	debug("..Read Audio History Error! i= %d\n",i);
	memcpy(lwAudioBeforeEncodeData, lwAudioInput16BitLinearData, lwLinearBytes);
	return FALSE;
    }

    ///test1////////////////
   //lwLinearBytes = lwNearDataNByte;
       /////////////////////////
    if (tAC->lwAudioInputType == AUDIO_ENCODING_ALAW) 
    { 
	AppMassALaw2Linear(lwAudioInputFarData, lwAudioInputFar16BitLinearData, lwNearDataNByte);
    }
    else
    {
	if (tAC->lwAudioInputType == AUDIO_ENCODING_ULAW)
	    AppMassULaw2Linear(lwAudioInputFarData, lwAudioInputFar16BitLinearData, lwNearDataNByte);
    }                                                                                              		
HERE2:
#ifdef MIPS_CHECK
    lwCountEC0 = GetCount();
#endif

    if (tAC->lwECType == EC_TYPE_AEC)
    {
	///test//////
	//debug("AEC");
	///////////////
 
    //AppAudioAECRun((PSHORT)lwAudioInput16BitLinearData, (PSHORT)lwAudioInputFar16BitLinearData, (PSHORT)lwAudioBeforeEncodeData);
    AppAudioAECRun(tAC->lwChannel, 0, (PSHORT)lwAudioInput16BitLinearData, (PSHORT)lwAudioInputFar16BitLinearData, (PSHORT)lwAudioBeforeEncodeData);    

    }
    else if (tAC->lwECType == EC_TYPE_LEC)
    {
	///test//////
	//debug("LEC");
	///////////////

	    pswTemp1 = (PSHORT)lwAudioInput16BitLinearData;
	    //pswTemp1 = pswTempWg;
	    pswTemp = (PSHORT)lwAudioInputFar16BitLinearData;
	    pcwTemp = (PBYTE)lwAudioBeforeEncodeData;
	    AppAudioBLMSEc(pswTemp1, pswTemp, pcwTemp);
	    pswTemp1 += AUDIO_MAX_FRAME_LEN/2;
	    pswTemp += AUDIO_MAX_FRAME_LEN/2;
	    pcwTemp += AUDIO_MAX_FRAME_LEN;
	    AppAudioBLMSEc(pswTemp1, pswTemp, pcwTemp);

    }


#ifdef MIPS_CHECK
    lwCountEC1 = GetCount();
#endif

    return TRUE;

}

LONG AppEncodePathMix(LONG lwExceptTalkerIndex, LONG *plwAudio16BitLinearData, LONG *plwAfterMixData)
{
    LONG i;
    LONG j = 0;

    for (i = 0; i < MAX_TALKER_PER_CHANNEL; i++)
    {
	if (i == lwExceptTalkerIndex)
	{
	    lwNeedMixPointer[j] = plwAudio16BitLinearData;
	    j++;
	    continue;
	}

	if (bActiveTalker[i])
	{
	    lwNeedMixPointer[j] = lwFromTalkerDecoded16BitLinearData[i];
	    j++;
	}
    }

    lwNeedMixPointer[j] = 0;
    App16BitLinearMixer(lwNeedMixPointer, plwAfterMixData, AUDIO_MAX_FRAME_LEN * glwInterruptTimeScale);

    return j;
}



LONG AppAudioEncode(tAudioCharacter *tAC, LONG lwTalkerIndex, LONG *lwAudioBeforeEncodeData, LONG *lwAudioAfterEncodeData)
{
    int i;
    int j;
    BYTE *pcwTemp;
    SHORT *pswTemp;
    LONG lwAudioInputPoints;
    lwAudioInputPoints = tAC->lwAudioInputPoints;

HERE1:

#ifdef MIPS_CHECK
    lwCountGXXE0 = GetCount();
#endif

    switch(tAC->lwGxxType[lwTalkerIndex])
    {
	case GXX_TYPE_G726: 
	    //test// 
	    //debug("E6");
	    ///////
	    j = AppAudioG726Encode(tAC->lwChannel, lwTalkerIndex, (PBYTE)lwAudioBeforeEncodeData, 0, (PBYTE)lwAudioAfterEncodeData , lwAudioInputPoints);
	    break; 
	case GXX_TYPE_G729A:
	    //test//
	    //debug("Ea");
	    ///////
	    pswTemp = (PSHORT)lwAudioBeforeEncodeData;
	    pcwTemp = (PBYTE)lwAudioAfterEncodeData;
	    j = AppAudioG729Encode(tAC->lwChannel, lwTalkerIndex, pswTemp, pcwTemp, G729A); 
	    pswTemp += AUDIO_MAX_FRAME_LEN * glwInterruptTimeScale/2;
	    pcwTemp += j;
	    i = AppAudioG729Encode(tAC->lwChannel, lwTalkerIndex, pswTemp, pcwTemp, G729A); 
	    j += i;

	    break; 
	case GXX_TYPE_G729B: 
	    j = 0;
	    break;

	case GXX_TYPE_G711A:
	    AppMassLinear2ALaw(lwAudioBeforeEncodeData, lwAudioAfterEncodeData, AUDIO_MAX_FRAME_LEN * glwInterruptTimeScale);
	    j = AUDIO_MAX_FRAME_LEN * glwInterruptTimeScale;
	    break;
	    
	case GXX_TYPE_G711U:
	    AppMassLinear2ULaw(lwAudioBeforeEncodeData, lwAudioAfterEncodeData, AUDIO_MAX_FRAME_LEN * glwInterruptTimeScale);
	    j = AUDIO_MAX_FRAME_LEN * glwInterruptTimeScale;
	    break;
	    
#ifdef AUDIO_G723_ENABLE
	case GXX_TYPE_G723_1_24_B:
	    //j = AppAudioG726Encode(tAC->lwChannel, lwTalkerIndex, (PBYTE)lwAudioBeforeEncodeData, 0, (PBYTE)lwAudioAfterEncodeData , lwAudioInputPoints);
	    AppAudioG723Encode((PSHORT)lwAudioBeforeEncodeData, (CHAR*) lwAudioAfterEncodeData, 0); 
	    j = 24;
	    break;

	case GXX_TYPE_G723_1_20_B:
	    AppAudioG723Encode((PSHORT)lwAudioBeforeEncodeData, (CHAR*) lwAudioAfterEncodeData, 1); 
	    j = 20;
	    break;
#endif

	default: 
	    j = 0;
	    break; 
    }
#ifdef MIPS_CHECK
    lwCountGXXE1 = GetCount();
#endif
    return j;
}

void AppSendEncodedAudioDataToLinux(tAudioCharacter *tAC, LONG lwTalkerIndex, LONG lwDataNBytes, LONG *lwFrom, LONG **lwTo)
{
    Risc_Packet_t risc_packet;	
    ULONG *pPacket;
    int i;

    if (lwDataNBytes > 0)
    {  
        pPacket = (ULONG*) &risc_packet;
        risc_packet.start = RISC_PKT_START;
        risc_packet.main_type = tAudio; 
        risc_packet.sub_type = tAudioTransferUcosToLinux;
        risc_packet.length = 0x14;
        risc_packet.status = 0;
        *(pPacket + 2) = tAC->lwChannel;
        *(pPacket + 3) = (ULONG) *lwTo;
        *(pPacket + 4) = lwDataNBytes;
        *(pPacket + 5) = lwTalkerIndex;
        *(pPacket + 6) = audioTs;
        risc_packet.end = RISC_PKT_END;

        memcpy(*lwTo, lwFrom, lwDataNBytes);
        *lwTo += lwDataNBytes/4;
        if((ULONG)(*lwTo) >= (ULONG)(AUDIO_UCOS_TO_LINUX_E_ADDR - 160)) //prevent overflow 
        {
            *lwTo = (LONG *)AUDIO_UCOS_TO_LINUX_B_ADDR;
        }

	if(TwoRiscCommSendPacket((unsigned int *)(&risc_packet), RPACKET_LEN) == SUCCESSFUL) 
	{ 
#ifdef DEBUG_AVSYNC
            gtStatistics.audio_out_ts = audioTs;
            gtStatistics.audio_out_cnt = RiscRdCNT(AUDIO_UCOS_TO_LINUX_CNT);
#endif
	    RiscIncCNT(AUDIO_UCOS_TO_LINUX_CNT, 1);    // increase count by 1 
	    RiscFlagSet();    // Set risc1 flag to interrupt risc1 
	} 
	else 
	    debug("\r\nTwoRiscAudioTA: send packet error!\r\n"); 
    }


}

VOID AppInitChannel(tAudioCharacter *ptAC)
{
    tAudioChannelPara tPara;    
    
#ifdef AUDIO_TEST_ENABLE
    test_init(INT_S, INT_MS, CTL);
    test_enable(CTL);
#endif

    /* config and start audio */
    if (ptAC->lwAudioDevice == AUDIO_DEVICE_CODEC)
    {
#ifdef CODEC_TLV320AIC23B
        tPara.time_input_slot_number = 0;
	    tPara.time_output_slot_number = 0x1f;
	    tPara.time_slot_count = 2;
	    tPara.prefetch = 1;
#else
	    tPara.time_input_slot_number = 0;
	    tPara.time_output_slot_number = 0;
	    tPara.time_slot_count = 1;
	    tPara.prefetch = 1;
#endif
    }
    else //AUDIO_DEVICE_SLIC
    {

#ifdef DVR
       tPara.time_input_slot_number = (ptAC->lwChannel) << 1;//only channel 1 has output data to 2835    
        tPara.time_output_slot_number = tPara.time_input_slot_number - 1;
        if (ptAC->lwAudioInputType == AUDIO_ENCODING_LINEAR)
    	    tPara.time_slot_count = 2;
    	 else
    	    tPara.time_slot_count = 1;
    
    	 if(tPara.time_input_slot_number != 0)
    	    tPara.prefetch = 0;
    	 else	
        {
    	    tPara.prefetch = 1;
           tPara.time_output_slot_number = 30;
        }
#if 0         
         if(ptAC->lwChannel ==1)
         {
           tPara.time_output_slot_number = 1;
           tPara.prefetch = 0;
         }
#endif
         debug("\n DVR timeslot %d-%d-%d-%d \n",tPara.time_input_slot_number,
            tPara.time_output_slot_number,tPara.time_slot_count,tPara.prefetch );
#else    
	tPara.time_input_slot_number = (ptAC->lwChannel+1) << 2; 
	tPara.time_output_slot_number = tPara.time_input_slot_number - 1;

	////////////
	//HwSlicSetTimeSlot(0, tPara.time_input_slot_number, tPara.time_output_slot_number);
	printf("input slottime = 0x%x\n",tPara.time_input_slot_number);
	SlicSetTimeSlot(tPara.time_input_slot_number*8+1, tPara.time_input_slot_number*8+1, ptAC->lwChannel);
	if (ptAC->lwAudioInputType == AUDIO_ENCODING_LINEAR)
	    tPara.time_slot_count = 2;
	else
	    tPara.time_slot_count = 1;

	if(tPara.time_input_slot_number != 0)
	    tPara.prefetch = 0;
	else	
	    tPara.prefetch = 1;
#endif        
    }

#if 0
    if(AppAudioGetStatus() == AUDIO_CLOSED)
	debug("\r\nAppInitChannel: audio was closed!");    
    else
	debug("\r\nAppInitChannel: audio was opened!"); 
#endif


    if (ptAC->lwECType == EC_TYPE_AEC)
    { 
	//AppAudioAECInit();
	switch(glwInterruptTimeScale)
	{
	    case 1:
		AppAudioAECInit(ptAC->lwChannel, 0, FrameLength_10ms);
		break;

	    case 2:
		AppAudioAECInit(ptAC->lwChannel, 0, FrameLength_20ms);
		break;

	    default:
		AppAudioAECInit(ptAC->lwChannel, 0, FrameLength_30ms);
		break;
	}
	    
    }
    else 
    {
	AppAudioBLMSEcInit();
    }

    ptAC->lwAudioInputPoints = AUDIO_MAX_FRAME_LEN * glwInterruptTimeScale;

    if (ptAC->lwAudioInputType == AUDIO_ENCODING_LINEAR)
    {
	ptAC->lwAudioInputBytes = ptAC->lwAudioInputPoints *2;
    }
    else
    {
	ptAC->lwAudioInputBytes = ptAC->lwAudioInputPoints;
    }

    AppAudioChannelOpen(ptAC->lwChannel /* channel_number */, &tPara);
}

VOID AppPrintChannelPara(LONG i)
{
    debug("[%d].bChannelEnable=%d\n", i, channelAC[i].bChannelEnable);
    debug("[%d].lwAudioDevice=%d\n", i, channelAC[i].lwAudioDevice);
    debug("[%d].lwChannel=%d\n", i, channelAC[i].lwChannel);
    debug("[%d].lwGxxType=%d\n", i, channelAC[i].lwGxxType);
    debug("[%d].lwECType=%d\n", i, channelAC[i].lwECType);
    debug("[%d].lwHistoryTimeStampMs=%d\n", i, channelAC[i].lwHistoryTimeStampMs);
    debug("[%d].lwAudioInputType=%d\n", i, channelAC[i].lwAudioInputType);
    debug("[%d].lwAudioOutputType=%d\n", i, channelAC[i].lwAudioOutputType);
    debug("[%d].lwG726EncodeInputType=%d\n", i, channelAC[i].lwG726EncodeInputType);
    debug("[%d].lwG726EncodeOutputType=%d\n", i, channelAC[i].lwG726EncodeOutputType);
    debug("[%d].lwG726DecodeInputType=%d\n", i, channelAC[i].lwG726DecodeInputType);
    debug("[%d].lwG726DecodeOutputType=%d\n", i, channelAC[i].lwG726DecodeOutputType);
    debug("[%d].lwJitterBufferReadBytes=%d\n", i, channelAC[i].lwJitterBufferReadBytes);
    debug("[%d].lwAudioInputPoints=%d\n", i, channelAC[i].lwAudioInputPoints);
    debug("[%d].lwAudioInputBytes=%d\n", i, channelAC[i].lwAudioInputBytes);
}

VOID AppInitTalker(LONG lwChannel, LONG lwTalker)
{
    extern LONG glwJitterBufferBuidUpFromEmptyDepthLimit[JITTER_BUFFER_CHANNEL_MAX][JITTER_BUFFER_TALKER_MAX];
    extern LONG glwJTBBuildUpFromEmptyDelayNms[JITTER_BUFFER_CHANNEL_MAX][JITTER_BUFFER_TALKER_MAX];

    extern LONG glwJitterBufferDropPacketFromDepth[JITTER_BUFFER_CHANNEL_MAX][JITTER_BUFFER_TALKER_MAX];
    extern LONG glwJTBDropPacketWhenHoldDataNms[JITTER_BUFFER_CHANNEL_MAX][JITTER_BUFFER_TALKER_MAX];

    glwInterruptTimeScale = 2; //for 10ms, 1; for 20ms, 2; for 30ms, 3
    glwCount_RiscVCNT_ISR_times = 0;

    switch (channelAC[lwChannel].lwGxxType[lwTalker]) 
    {
	case GXX_TYPE_G726:
	    switch (channelAC[lwChannel].lwG726DecodeInputType[lwTalker])
	    {
		case G726_DATA_TYPE_24K:
		    channelAC[lwChannel].lwJitterBufferReadBytes[lwTalker] = 60; 
		    break;

		case G726_DATA_TYPE_40K:
		    channelAC[lwChannel].lwJitterBufferReadBytes[lwTalker] = 100; 
		    break;

		default: //G726_DATA_TYPE_32K
		    channelAC[lwChannel].lwJitterBufferReadBytes[lwTalker] = 80; 
	    }

	    AppAudioG726EncodeInit(lwChannel, lwTalker, channelAC[lwChannel].lwG726EncodeOutputType[lwTalker], channelAC[lwChannel].lwG726EncodeInputType[lwTalker]);
	    AppAudioG726DecodeInit(lwChannel, lwTalker, channelAC[lwChannel].lwG726DecodeInputType[lwTalker], channelAC[lwChannel].lwG726DecodeOutputType[lwTalker]);
	    break; 

	case GXX_TYPE_G729A: 
	    channelAC[lwChannel].lwJitterBufferReadBytes[lwTalker] = 20; 

	    AppAudioG729EncodeInit(lwChannel, lwTalker); 
	    AppAudioG729DecodeInit(lwChannel, lwTalker); 
	    break; 

	case GXX_TYPE_G729B: 
	    break; 

#ifdef AUDIO_G723_ENABLE
	case GXX_TYPE_G723_1_24_B:
	    //AppAudioG726EncodeInit(lwChannel, lwTalker, channelAC[lwChannel].lwG726EncodeOutputType[lwTalker], channelAC[lwChannel].lwG726EncodeInputType[lwTalker]);
	    //AppAudioG726DecodeInit(lwChannel, lwTalker, channelAC[lwChannel].lwG726DecodeInputType[lwTalker], channelAC[lwChannel].lwG726DecodeOutputType[lwTalker]);
	    //channelAC[lwChannel].lwJitterBufferReadBytes[lwTalker] = 80; 
	    //channelAC[lwChannel].lwJitterBufferReadBytes[lwTalker] = 120; 

	    channelAC[lwChannel].lwJitterBufferReadBytes[lwTalker] = 24; 
	    glwInterruptTimeScale = 3;  //30ms, =3;
	    AppAudioG723Init(0);

	    break;

	case GXX_TYPE_G723_1_20_B:

	    channelAC[lwChannel].lwJitterBufferReadBytes[lwTalker] = 20; 
	    glwInterruptTimeScale = 3;  //30ms, =3;
	    AppAudioG723Init(1);

	    break;
#endif


	default://G711 
	    channelAC[lwChannel].lwJitterBufferReadBytes[lwTalker] = 160; 
	    break;
    }

    
    /*init jitter buffer */
    glwJitterBufferBuidUpFromEmptyDepthLimit[lwChannel][lwTalker] = (glwJTBBuildUpFromEmptyDelayNms[lwChannel][lwTalker]/(glwInterruptTimeScale * 10)) * channelAC[lwChannel].lwJitterBufferReadBytes[lwTalker];
    
    glwJitterBufferDropPacketFromDepth[lwChannel][lwTalker] = (glwJTBDropPacketWhenHoldDataNms[lwChannel][lwTalker]/(glwInterruptTimeScale * 10)) * channelAC[lwChannel].lwJitterBufferReadBytes[lwTalker];

    JitterBufferResetTalker(lwChannel, lwTalker);
    //test1//
    debug("JBE%d-%d %d ", lwChannel, lwTalker, glwJitterBufferBuidUpFromEmptyDepthLimit[lwChannel][lwTalker]);
    debug("JBM%d-%d %d\n", lwChannel, lwTalker, glwJitterBufferDropPacketFromDepth[lwChannel][lwTalker]);
    //////////

    channelAC[lwChannel].lwTalkerHaveSetCodecBitFlag |= (1<<lwTalker);
}

VOID AppInitChannelEx(LONG i)
    //i must less then MAX_CHANNEL_NUMBER
{
    long k;
    channel_history_pointer_initialized[i] = 0;
    channel_history_pointer_recount[i] = 0; 	

    channelAC[i].lwHistoryTimeStampMs = 2;

    if (channelAC[i].lwAudioInputType == AUDIO_ENCODING_LINEAR)
    {
	time_stamp[i] = 16 * channelAC[i].lwHistoryTimeStampMs;
	his_len[i] = AUDIO_MAX_FRAME_LEN * glwInterruptTimeScale * 2;
    }
    else
    {
	time_stamp[i] = 8 * channelAC[i].lwHistoryTimeStampMs;
	his_len[i] = AUDIO_MAX_FRAME_LEN * glwInterruptTimeScale;
    }

#ifdef AUDIO_AID_DATA_ENABLE
    for (k = 0; k < MAX_TALKER_PER_CHANNEL; k++)
    {   
        audioAidData[i][k].ReadIndex = 0;
        audioAidData[i][k].WriteIndex = 0;
    }
#endif

    AppInitChannel(&channelAC[i]);

#ifdef MIPS_CHECK
    lwMaxCountTask = 0;
    lwMaxCountOneChannel = 0;
    lwMaxCountEC = 0;
    lwMaxCountGXXD = 0;
    lwMaxCountGXXE = 0;
#endif

    //test1///////
    //AppPrintChannelPara(i);
    ////////////////////////
}


LONG AppGetActiveTalker(LONG lwTalkerEnableBitFlag)
{
    LONG i;
    LONG j = 0;
    for (i = 0; i < MAX_TALKER_PER_CHANNEL; i++)
    {
	if (((lwTalkerEnableBitFlag >> i) & 0x1) == 0)
	{
	    bActiveTalker[i] = FALSE;
	}
	else
	{
	    bActiveTalker[i] = TRUE;
	    j++;
	}
    }

    return j;
}

/*
 ***************************************************************************
 *                         TwoRiscAudioTA
 *
 * Description: 
 *
 * Arguments  : 
 *
 * Return     : 
 *
 * Note(s)    : 
 ***************************************************************************
 */
void TwoRiscAudioTA(void *data)
{
    int i,j;		
    unsigned char err;	

    LONG lwAudioInput16BitLinearData[AUDIO_MAX_FRAME_LEN * 2];    // gb *2 to fit 723


    LONG lwAudioAfterDecodeData[AUDIO_MAX_FRAME_LEN * 2];
    LONG lwAudioBeforeEncodeData[AUDIO_MAX_FRAME_LEN * 2];
    LONG lwAudioEncodePathBeforeMixData[AUDIO_MAX_FRAME_LEN * 2];
    LONG lwAudioEncodePathAfterMixData[AUDIO_MAX_FRAME_LEN * 2];
    LONG lwAudioAfterEncodeData[AUDIO_MAX_FRAME_LEN * 2]; 

    LONG *plwTemp1 = (LONG*)AUDIO_UCOS_TO_LINUX_B_ADDR; 

    tAudioCharacter tACCurrent;
    tAudioCharacter *ptACCurrent;

    BspStartTicker(OS_TICKS_PER_SEC); 	                 

    debug("\n*Risc%d Audio TA Task*\n", RiscRdID());    

    audio_ucos_to_linux_addr = (unsigned int *)AUDIO_UCOS_TO_LINUX_B_ADDR;

    gptAudioTaskReady = OSSemCreate(0);

    RiscClrCNT(AUDIO_UCOS_TO_LINUX_CNT);                    


    //test1/////////////////
#ifdef DUMP_ENABLE
    AppResetDumpContext(0);
    AppResetDumpContext(1);
    AppResetDumpContext(2);
    AppResetDumpContext(3); 
    AppResetDumpContext(4);
    AppResetDumpContext(5);
    AppResetDumpContext(6);
    AppResetDumpContext(7);
    AppResetDumpContext(8);
    AppResetDumpContext(9);
    *((LONG*)(DUMP_PERMIT_RAM_FLAG_ADDR)) = DUMP_NOT_PERMIT_VALUE;
#endif

    lwNeedMixPointer[MAX_TALKER_PER_CHANNEL] = 0;

    AudioIntrEn();
    AppAudioOpen();

#if 0
    if(AppAudioGetStatus() == AUDIO_CLOSED)
	debug("\r\nTwoRiscAudioTA: audio was closed!");
    else
	debug("\r\nTwoRiscAudioTA: audio was opened!");
#endif

    ptACCurrent = &tACCurrent;
    
    while(1) 
    {

	OSSemPend(gptAudioTaskReady, 0, &err);


#ifdef MIPS_CHECK
	    lwCountTask0 = GetCount();
#endif

	for (glwChannelIndex = 0; glwChannelIndex < MAX_AUDIO_CHANNEL; glwChannelIndex++)
	{
	    if (!channelAC[glwChannelIndex].bChannelEnable)
	    {
		continue;
	    }
	    
#ifdef MIPS_CHECK
	    lwCountOneChannel0 = GetCount();
#endif

	    memcpy(ptACCurrent, &channelAC[glwChannelIndex], sizeof(tAudioCharacter));

	    i = AppGetActiveTalker(ptACCurrent->lwTalkerEnableBitFlag);
	    if (i != ptACCurrent->lwCurrentActiveTalkerCount)
	    {
		debug("error! channel[%d] lwTalkerEnableBitFlag 0x%x not match with lwCurrentActiveTalkerCount 0x%d\n", glwChannelIndex, ptACCurrent->lwTalkerEnableBitFlag, ptACCurrent->lwCurrentActiveTalkerCount);
	    }
	    
	    //***********************************************
	    //Decode part	
	    //***********************************************

	   i = AppAudioDecode(ptACCurrent, lwAudioAfterDecodeData);

#ifdef JTB_TIME_CHECK
	    //test1///////////
	    extern ULONG lwJTBWrToRdTime;
	    extern ULONG lwJTBRdToDspTime;
	    extern ULONG lwJTBTimeCheckState ;
	    extern ULONG lwJTBWrCheckError;
	    extern ULONG lwJTBRdCheckError;
	    
	    if (lwJTBTimeCheckState == 2)
	    {
		lwJTBRdToDspTime = (ULONG) GetRisc0Timer();
		lwJTBTimeCheckState = 3;
	    }
#endif


	    //test1///////////
#ifdef DUMP_ENABLE
	    //AppSetDumpContext(1, i, 50*10, 50*10);
	    //AppDumpThem(1, lwAudioAfterDecodeData);
#endif
	    //////////////////
#ifdef TEST_PCM_IF
	    {
	        int cnt_t;
	        for(cnt_t = 0; cnt_t < 80; cnt_t++)
	        {
	            lwAudioAfterDecodeData[cnt_t] = 0xa135a135;
	        }
	    }
#endif
	    if (i > 0)
	    {
		AppAudioWriteData(ptACCurrent->lwChannel /* channel_number */, (int*)lwAudioAfterDecodeData, i); //send docoded data to audio output 
#ifndef UCOS_USE_AUDIO_JITBUF
                if(RiscRdCNT(AUDIO_LINUX_TO_UCOS_CNT) > 0)
                    RiscDecCNT(AUDIO_LINUX_TO_UCOS_CNT, 1);
#endif
	    } 
	    else
	    {
		AudioDataUnderFlowProtection(ptACCurrent->lwChannel /* channel_number */, 0);
	    }

	    //***********************************************
	    //Encode part	
	    //***********************************************


	    i = AppReadAudioInput(ptACCurrent, lwAudioInput16BitLinearData);

	    if (i == 0)
	    {
		debug("AI0\n");
		continue;
	    }

	    //start EC
	    AppRunEC(ptACCurrent, lwAudioInput16BitLinearData, lwAudioEncodePathBeforeMixData);

	    for (i = 0; i < MAX_TALKER_PER_CHANNEL; i++)
	    {
		if (bActiveTalker[i])
		{
		    AppEncodePathMix(i, lwAudioEncodePathBeforeMixData, lwAudioEncodePathAfterMixData);
		    //test1///////////
#ifdef DUMP_ENABLE
		    AppSetDumpContext(i+6, AUDIO_MAX_FRAME_LEN * 2, 50*10, 50*DUMP_WAIT_S);
		    AppDumpThem(i+6, lwAudioEncodePathAfterMixData);
#endif
		    //////////////////
		    j = AppAudioEncode(ptACCurrent, i, lwAudioEncodePathAfterMixData, lwAudioAfterEncodeData);

		    //test1///////////
#ifdef DUMP_ENABLE
		    AppSetDumpContext(i+8, j, 50*10, 50*DUMP_WAIT_S);
		    AppDumpThem(i+8, lwAudioAfterEncodeData);
#endif
		    //////////////////

#ifdef DUMP_ENABLE
#if 0
		    //test1///////////
		    if (i==0)
		    {
    			AppSetDumpContext(3, j, 50*10, 50*10);
			AppDumpThem(3, lwAudioAfterEncodeData);
			//////////////////
		    }
		    if (i==1)
		    {
    			AppSetDumpContext(4, j, 50*10, 50*10);
			AppDumpThem(4, lwAudioAfterEncodeData);
			//////////////////
		    }
#endif
#endif
		    if (j>0)
		    {
			AppSendEncodedAudioDataToLinux(ptACCurrent, i, j, lwAudioAfterEncodeData, &plwTemp1);
			
#ifdef AUDIO_TEST_ENABLE
			talker_info[glwChannelIndex][i].ucos_tx++;
#endif
				    

		    }
		}
	    }
#ifdef JTB_TIME_CHECK	    
	    if (lwJTBTimeCheckState == 3)
	    {
		lwJTBTimeCheckState = 0;
		debug("~%d %d", (~lwJTBWrToRdTime)/200000, (~lwJTBRdToDspTime)/200000);
	    }


	    if (lwJTBWrCheckError > 0 || lwJTBRdCheckError > 0)
	    {
    		debug("E%d %d", lwJTBWrCheckError, lwJTBRdCheckError);
		lwJTBWrCheckError = 0;
		lwJTBRdCheckError = 0;
	    }
	    /////////////////////////
#endif

#ifdef MIPS_CHECK
	    
	    lwCountOneChannel1 = GetCount();

	    lwCountOneChannel1 = CountSpend(lwCountOneChannel1, lwCountOneChannel0);
	    if (lwMaxCountOneChannel < lwCountOneChannel1)
	    {
	        lwMaxCountOneChannel = lwCountOneChannel1;
		debug("L=%d\n", lwMaxCountOneChannel);
	    }

	    lwCountEC1 = CountSpend(lwCountEC1, lwCountEC0);
	    if (lwMaxCountEC < lwCountEC1)
	    {
	        lwMaxCountEC = lwCountEC1;
		debug("C=%d\n", lwMaxCountEC);
	    }

	    lwCountGXXD1 = CountSpend(lwCountGXXD1, lwCountGXXD0);
	    if (lwMaxCountGXXD < lwCountGXXD1)
	    {
	        lwMaxCountGXXD = lwCountGXXD1;
		debug("D=%d\n", lwMaxCountGXXD);
	    }

	    lwCountGXXE1 = CountSpend(lwCountGXXE1, lwCountGXXE0);
	    if (lwMaxCountGXXE < lwCountGXXE1)
	    {
		lwMaxCountGXXE = lwCountGXXE1;
		debug("E=%d\n", lwMaxCountGXXE);
	    }

	    lwCountOneChannel1 = 0;
	    lwCountOneChannel0 = 0;
	    lwCountEC1 = 0;
	    lwCountEC0 = 0;
	    lwCountGXXD1 = 0;
	    lwCountGXXD0 = 0;
	    lwCountGXXE1 = 0;
	    lwCountGXXE0 = 0;

#endif 
	}

#ifdef MIPS_CHECK
	lwCountTask1 = GetCount();

	lwCountTask1 = CountSpend(lwCountTask1,lwCountTask0);
	if (lwMaxCountTask < lwCountTask1)
	{
	    lwMaxCountTask = lwCountTask1;
	    debug("T=%d\n",lwMaxCountTask);
	}

	if ( lwCountTask1 >= (18 * (CPU_MAIN_CLOCK/2000)) )
	{
	    debug("t=%d\n",lwCountTask1);
	}

	lwCountTask1 = 0;
	lwCountTask0 = 0;
#endif

    }
}

#endif


VOID AppMassALaw2Linear(LONG *lwALawBuffer, LONG *lwLinearBuffer, LONG lwALawPoints)
{
    SHORT *pswTemp;
    BYTE *pcwTemp;

    pswTemp = (SHORT *)lwLinearBuffer;
    pcwTemp = (BYTE *)lwALawBuffer;
    G711AlawDecode(pcwTemp, pswTemp, lwALawPoints);
}

VOID AppMassULaw2Linear(LONG *lwULawBuffer, LONG *lwLinearBuffer, LONG lwULawPoints)
{
    SHORT *pswTemp; 
    BYTE *pcwTemp;

    pswTemp = (SHORT *)lwLinearBuffer; 
    pcwTemp = (BYTE *)lwULawBuffer; 
    G711UlawDecode(pcwTemp, pswTemp, lwULawPoints);
}

VOID AppMassLinear2ULaw(LONG *lwLinearBuffer, LONG *lwULawBuffer, LONG lwLinearPoints)
{
    SHORT *pswTemp;
    BYTE *pcwTemp;

    pswTemp = (SHORT *)lwLinearBuffer;
    pcwTemp = (BYTE *)lwULawBuffer;
    G711UlawEncode(pswTemp, pcwTemp, lwLinearPoints);
}

VOID AppMassLinear2ALaw(LONG *lwLinearBuffer, LONG *lwALawBuffer, LONG lwLinearPoints)
{
    SHORT *pswTemp;
    BYTE *pcwTemp;

    pswTemp = (SHORT *)lwLinearBuffer;
    pcwTemp = (BYTE *)lwALawBuffer;
    G711AlawEncode(pswTemp, pcwTemp, lwLinearPoints);
}

#ifdef DUMP_ENABLE
tDumpContext DumpContext[MAX_DUMP_POINTS];

VOID AppResetDumpContext(LONG i)
{
    DumpContext[i].bDumpSetted = FALSE;
    AppCopyDumpedContext(i);
}

VOID AppSetDumpContext(LONG i, LONG lwDumpOneTimeSize, LONG lwDumpTimes, LONG lwDumpDelayTimes)
{
    if (!DumpContext[i].bDumpSetted)
    {
	DumpContext[i].bDumpFinish = FALSE;
	DumpContext[i].pcwStart = (BYTE *)(DUMP_TO_ADDR + i * 0x100000);
	DumpContext[i].lwDumpOneTimeSize = lwDumpOneTimeSize;
	DumpContext[i].lwDumpTimes = lwDumpTimes;
	DumpContext[i].lwDumpedTimes = 0;
	DumpContext[i].lwDumpDelayTimes = lwDumpDelayTimes;
	DumpContext[i].bDumpSetted = TRUE;
	AppCopyDumpedContext(i);
    }
}

VOID AppDumpThem(LONG i, VOID *from)
{
    BYTE *pcwToAddr;

    if ( *((LONG*)(DUMP_PERMIT_RAM_FLAG_ADDR)) != DUMP_PERMIT_VALUE )
    {
	return;
    }

    if (DumpContext[i].lwDumpDelayTimes > 0)
    {
	DumpContext[i].lwDumpDelayTimes--;
	return;
    }

    if (!DumpContext[i].bDumpFinish)
    {
	pcwToAddr = DumpContext[i].pcwStart + DumpContext[i].lwDumpedTimes * DumpContext[i].lwDumpOneTimeSize;
	memcpy(pcwToAddr, from, DumpContext[i].lwDumpOneTimeSize);
	DumpContext[i].lwDumpedTimes++;

	if (DumpContext[i].lwDumpedTimes == DumpContext[i].lwDumpTimes)
	{
	    DumpContext[i].bDumpFinish = TRUE;
	    AppCopyDumpedContext(i);
	}
	debug(".");
    }
}

LONG AppCopyDumpedContext(LONG i)
{
    LONG *plwTemp;

    plwTemp = (LONG *) ((ULONG)DUMP_STRUCT_ADDR_POINTER + sizeof(tDumpContext) * i);
    memcpy(plwTemp, &DumpContext[i], sizeof(tDumpContext));
}

VOID AppReadDumpedData(LONG i)
{
    int j;
    LONG *plwTemp;
    tDumpContext tempDumpContext;

    plwTemp = (LONG *) ((ULONG)DUMP_STRUCT_ADDR_POINTER + sizeof(tDumpContext) * i);
    memcpy(&tempDumpContext, plwTemp, sizeof(tDumpContext));

    if (!tempDumpContext.bDumpSetted)
    {
	return;
    }

    if (!tempDumpContext.bDumpFinish)
    {
	_PRINTF("Sorry, dumping is not finish in context %d\n", i);
	//debug("Sorry, dumping is not finish in context %d\n", i);
	return;
    }

    plwTemp = (LONG*) tempDumpContext.pcwStart;
    _PRINTF("\r\n@@@@_%d_@@@@\r\n", i);
    //debug("\r\n@@@@_%d_@@@@\r\n", i);
    for (j = 0; j < (tempDumpContext.lwDumpTimes * tempDumpContext.lwDumpOneTimeSize); j += 4)
    {
	_PRINTF("0x%x ", *plwTemp);
	//debug("0x%x ", *plwTemp);
	plwTemp++;
    }
    _PRINTF("\r\n@@@@_%d_END_@@@@\r\n", i);
    //debug("\r\n@@@@_%d_END_@@@@\r\n", i);
}

#endif


