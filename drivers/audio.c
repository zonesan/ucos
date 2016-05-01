/*
***************************************************************************
*                  Copyright (c) 2005 vBridge Microsystem, Inc.  
*                        Unpublished & Not for Publication
*                              All Rights Reserved                   
*   
* File        : audio.c                     
*                                                                         
* Description : audio drivers
*
* Date        :
***************************************************************************
*/



#define AUDIO_GLOBALS

#include "../port/os_cpu.h"
#include "../uCOS Configure/os_cfg.h"
#include "../uCOS2.52/uCOS_II.h"

#include "audio.h"
#include "comm.h"
#include "cctl.h"
#include "Sc.h"
#include "2risc_memory_map.h"
#include "2risc_comm_protocol.h"
//#include "TestSc.h"
//#include "TestMc.h"
#include "Mc.h"
#include "../G726/G726.h"

#include "two_risc_audio.h"
#include "audio_cfg.h"
#include "timestamp.h"

static int audio_opened = FALSE;    // it indicate the audio's status

/*
***************************************************************************
*                              GLOBAL FUNCTIONS
***************************************************************************
*/

/*
***************************************************************************
*                             AppAudioTimingSet
*
* Description: Initialize the PCM timing, clock rising/falling, etc.
*
* Arguments  : none.
*
* Return     : SUCCESSFUL    if the audio interface is not opened, return
*                            SUCCESSFUL
*              FAILED        if the audio interface is opened, the timing
*                            set should not be changed.
*
* Note(s)    : 
***************************************************************************
*/
int AppAudioTimingSet(void)
{
    if(audio_opened)
        return FAILED;
        	
    // bit    31   30     29    28    27      
    // name   ENA  FS_IO  FS_W  FS_P  CLK_IO  
    // binary 1    0      0     0     0 
    //------------------------------------------------------------------
    // bit    26      25      24     23     22     21:16 15:8     7:0
    // name   DI_DLY  DO_DLY  FS_AL  DI_AL  DO_AL  N.A.  CLK_DIV  FS_DIV
    // binary 1       1       0      0      0      x     0        0
#ifdef CODEC_TLV320AIC23B
    rAUD_CTL = BIT_FS_IO | BIT_FS_W | BIT_FS_P | BIT_CLK_IO | /*BIT_DI_DLY | BIT_DO_DLY |*/ \
               BIT_FS_AL | BIT_DI_AL | BIT_DO_AL | BIT_CLK_DIV | BIT_FS_DIV;
#else
    rAUD_CTL = BIT_FS_IO | BIT_FS_W | BIT_FS_P | BIT_CLK_IO | BIT_DI_DLY | BIT_DO_DLY | \
               BIT_FS_AL | BIT_DI_AL | BIT_DO_AL | BIT_CLK_DIV | BIT_FS_DIV;
#endif
    if(CctlAudioClkSet() == FAILED)
        return FAILED;
    
    return SUCCESSFUL;
}

#ifdef AUDIO_MODULE_INCLUDE
/*
***************************************************************************
*                             AppAudioChannelOpen
*
* Description: 
*
* Arguments  : none.
*
* Return     : SUCCESSFUL    
*              FAILED        if the audio interface is opened/the channel
*                            number or parameter is wrong/queue, stream operation
*                            is wrong, return FAILED.
*
* Note(s)    : 
***************************************************************************
*/
int AppAudioChannelOpen(int channel_number, tAudioChannelPara *ptPara)
{
    unsigned int queue_out_id, queue_in_id, stream_out_id, stream_in_id, stream_autofetch_id;
    unsigned int queue_out_addr, queue_in_addr, stream_out_addr, stream_in_addr, stream_autofetch_addr;
    unsigned int audio_time_slot_mask, audio_time_slot_shift;
    int i, len;
#if 0
    extern LONG glwAudioOutputType;
    extern LONG glwAudioInputType;
#endif
    extern tAudioCharacter channelAC[MAX_AUDIO_CHANNEL];
    	
    //if(audio_opened)
    //    return FAILED;	
    
    /* get the queue addr & IDs, stream addr & IDs */
    switch(channel_number) {
        case 0:                               
            queue_out_id = AUD_OUT_CH0_M;	
            queue_in_id  = AUD_IN_CH0_M;	            
            
            stream_out_id = AUD_OUT_CH0_S;
            stream_in_id  = AUD_IN_CH0_S;
            stream_autofetch_id = AUD_AUTOFETCH_CH0_S;  // SOFT_RD_1_S
            
            queue_out_addr = AUDIO_OUT_CH0_M_ADDR;
            queue_in_addr  = AUDIO_IN_CH0_M_ADDR;
            
            stream_out_addr = AUD_OUT_CH0_S_ADDR;
            stream_in_addr  = AUD_IN_CH0_S_ADDR;
            stream_autofetch_addr = AUD_AUTOFETCH_CH0_S_ADDR;  // SOFT_RD_1_S_ADDR
            
            break;
            
        case 1:
            queue_out_id = AUD_OUT_CH1_M;	
            queue_in_id  = AUD_IN_CH1_M;	            
            
            stream_out_id = AUD_OUT_CH1_S;
            stream_in_id  = AUD_IN_CH1_S;
            stream_autofetch_id = AUD_AUTOFETCH_CH1_S;
            
            queue_out_addr = AUDIO_OUT_CH1_M_ADDR;
            queue_in_addr  = AUDIO_IN_CH1_M_ADDR;
            
            stream_out_addr = AUD_OUT_CH1_S_ADDR;
            stream_in_addr  = AUD_IN_CH1_S_ADDR;
            stream_autofetch_addr = AUD_AUTOFETCH_CH1_S_ADDR;
            
            break;
            
        case 2:
            queue_out_id = AUD_OUT_CH2_M;	
            queue_in_id  = AUD_IN_CH2_M;	            
            
            stream_out_id = AUD_OUT_CH2_S;
            stream_in_id  = AUD_IN_CH2_S;
            stream_autofetch_id = AUD_AUTOFETCH_CH2_S;            
            
            queue_out_addr = AUDIO_OUT_CH2_M_ADDR;
            queue_in_addr  = AUDIO_IN_CH2_M_ADDR;
            
            stream_out_addr = AUD_OUT_CH2_S_ADDR;
            stream_in_addr  = AUD_IN_CH2_S_ADDR;
            stream_autofetch_addr = AUD_AUTOFETCH_CH2_S_ADDR;
            
            break;
            
        case 3:
            queue_out_id = AUD_OUT_CH3_M;	
            queue_in_id  = AUD_IN_CH3_M;	            
            
            stream_out_id = AUD_OUT_CH3_S;
            stream_in_id  = AUD_IN_CH3_S;
            stream_autofetch_id = AUD_AUTOFETCH_CH3_S;    
                     
            queue_out_addr = AUDIO_OUT_CH3_M_ADDR;
            queue_in_addr  = AUDIO_IN_CH3_M_ADDR;           
            
            stream_out_addr = AUD_OUT_CH3_S_ADDR;
            stream_in_addr  = AUD_IN_CH3_S_ADDR;
            stream_autofetch_addr = AUD_AUTOFETCH_CH3_S_ADDR;            
            
            break;
            
        default:
            return FAILED;                                                
    }
    
    /* Set audio input time slot number */
    rAUD_INTS = (rAUD_INTS & (~(0xff<<((channel_number * 8))))) | \
   ((ptPara->time_input_slot_number)<< (channel_number * 8));
    
    /* Set audio output time slot number */
    rAUD_OUTTS = (rAUD_OUTTS & (~(0xff<<((channel_number * 8))))) | \
    ((ptPara->time_output_slot_number)<< (channel_number * 8));    
    
    /* check time slot count */
    if((ptPara->time_slot_count == 1) || (ptPara->time_slot_count == 2)) {        
        rAUD_SID = rAUD_SID & (~(0x3<<((channel_number * 2) + 24))) | \
          ((ptPara->time_slot_count)<<((channel_number * 2) + 24));    // set ICH_CNT
        
        rAUD_SID = rAUD_SID & (~(0x3<<((channel_number * 2) + 16))) | \
          ((ptPara->time_slot_count)<<((channel_number * 2) + 16));    // set OCH_CNT
    } else if(ptPara->time_slot_count == 4) {
        rAUD_SID = rAUD_SID & (~(0x3<<((channel_number * 2) + 24))) | \
                                (0x3<<((channel_number * 2) + 24));    // set ICH_CNT
        
        rAUD_SID = rAUD_SID & (~(0x3<<((channel_number * 2) + 16))) | \
                                (0x3<<((channel_number * 2) + 16));    // set OCH_CNT	
    } else 
        return FAILED;
        
    /* check ptPara->prefetch is 1 or 0 ?*/    
    if(ptPara->prefetch & 0xfffffffe)
        return FAILED;
    rAUD_SID = rAUD_SID & (~(1 << (channel_number + 12))) | \
           ((ptPara->prefetch) << (channel_number + 12));   // ser OCH_PF
        
    /* Initialize the output queue */
    for (i = 0; i < 0x2000; i += 4)
    {
	*(int *)(queue_out_addr | 0xa0000000 + i) = 0;
    }
    //debug("\r\nInitialize the audio output queue 0x%x...", queue_out_id);
    if(McQueueInit(queue_out_id, Q_EMPTY, Q_BufSize_8KByte /*buf_size*/, Q_SBufSize_4Byte /*subbuf_size*/, queue_out_addr /*base_addr*/)) {
        //debug("OK\r\n");
        //DebugMcQueueContext(queue_out_id); debug(" Q_EMPTY = %x.\r\n", Q_EMPTY);
    } else {
        //debug("ERROR\r\n");        
    }
    
    /* Initialize the input queue */
    //debug("\r\nInitialize the audio input queue 0x%x...", queue_in_id);
    if(McQueueInit(queue_in_id, Q_EMPTY, Q_BufSize_8KByte /*buf_size*/, Q_SBufSize_4Byte /*subbuf_size*/, queue_in_addr /*base_addr*/)) {
        //debug("OK\r\n");
        //DebugMcQueueContext(queue_in_id); debug(" Q_EMPTY = %x.\r\n", Q_EMPTY);
    } else {
        //debug("ERROR\r\n");        
    }    
    
    /* Initialize the output stream */
    //debug("\r\nInitialize the audio output stream 0x%x ...", stream_out_id);
   // if (glwAudioOutputType == AUDIO_ENCODING_LINEAR)
    if (channelAC[channel_number].lwAudioOutputType == AUDIO_ENCODING_LINEAR)
    {
	if(ScInit(stream_out_id, sBufUse_Sdram, sDBSize_Byte /*db_size*/, sBufSize_32Byte /*buf_size*/, sSBSize_32Byte /*sb_size*/, sMBSize_32Byte /*mb_size*/,\
		    sSwap_no, sLast_Disable, stream_out_addr /*base_addr*/)) 
		    //sSwap_2Byte, sLast_Disable, stream_out_addr /*base_addr*/)) 
	{
	    //debug("OK\r\n");
	    //DebugScContext(stream_out_id); debug(" S_CLOSE = %x.\r\n", S_CLOSE);
	} 
	else 
	{
	    //debug("ERROR\r\n");        
	}     
    }
    else
    {
	if(ScInit(stream_out_id, sBufUse_Sdram, sDBSize_Byte /*db_size*/, sBufSize_32Byte /*buf_size*/, sSBSize_32Byte /*sb_size*/, sMBSize_32Byte /*mb_size*/,\
		    sSwap_no, sLast_Disable, stream_out_addr /*base_addr*/)) 
	{
	    //debug("OK\r\n");
	    //DebugScContext(stream_out_id); debug(" S_CLOSE = %x.\r\n", S_CLOSE);
	} 
	else 
	{
	    //debug("ERROR\r\n");        
	}     
    }

    /* Initialize the input stream */
    //debug("\r\nInitialize the audio input stream 0x%x ...", stream_in_id);
    if (channelAC[channel_number].lwAudioOutputType == AUDIO_ENCODING_LINEAR)
    {
	if(ScInit(stream_in_id, sBufUse_Sdram, sDBSize_Byte /*db_size*/, sBufSize_32Byte /*buf_size*/, sSBSize_16Byte /*sb_size*/, sMBSize_16Byte /*mb_size*/,\
		    sSwap_2Byte, sLast_Disable, stream_in_addr /*base_addr*/)) 
		    //sSwap_no, sLast_Disable, stream_in_addr /*base_addr*/)) 
	{
	    //debug("OK\r\n");
	    //DebugScContext(stream_in_id); debug(" S_CLOSE = %x.\r\n", S_CLOSE);
	} 
	else 
	{
	    //debug("ERROR\r\n");        
	}
    }
    else
    {
	if(ScInit(stream_in_id, sBufUse_Sdram, sDBSize_Byte /*db_size*/, sBufSize_32Byte /*buf_size*/, sSBSize_16Byte /*sb_size*/, sMBSize_16Byte /*mb_size*/,\
		    sSwap_no, sLast_Disable, stream_in_addr /*base_addr*/)) 
	{
	    //debug("OK\r\n");
	    //DebugScContext(stream_in_id); debug(" S_CLOSE = %x.\r\n", S_CLOSE);
	} 
	else 
	{
	    //debug("ERROR\r\n");        
	}

    }

#if 0
    /* Initialize the auto-fetch stream */
    debug("\r\nInitialize the autofetch stream 0x%x ...", stream_autofetch_id);
    if(ScInit(stream_autofetch_id, sBufUse_Sdram, sDBSize_4Byte /*db_size*/, sBufSize_32Byte /*buf_size*/, sSBSize_16Byte /*sb_size*/, sMBSize_16Byte /*mb_size*/,\
              sSwap_no, sLast_Disable, stream_autofetch_addr /*base_addr*/)) {
        debug("OK\r\n");
        PrinfScContext(stream_autofetch_id); debug(" S_CLOSE = %x.\r\n", S_CLOSE);
    } else {
        debug("ERROR\r\n");        
    }
    
    /* Enable auto-fetch */
    debug("\r\nEnable auto-fetch...");
    if(McAutoFetchEn(stream_autofetch_id))
        debug("OK\r\n");
    else
        debug("failed\r\n");        
#endif
    
    /* Map the output queue and the output stream */
    //debug("\r\nMap the output queue and the output stream...");
    if(McScMap(queue_out_id, stream_out_id)) {
        //debug("OK\r\n");
        //DebugMcQueueContext(queue_out_id); debug("\r\n");
        //DebugScContext(stream_out_id); debug("\r\n");
    } else {
        //debug("ERROR\r\n");        
    } 

    /* Map the input queue and the input stream */
    //debug("\r\nMap the input queue and the input stream...");
    if(McScMap(queue_in_id, stream_in_id)) {
        //debug("OK\r\n");
        //DebugMcQueueContext(queue_in_id); debug("\r\n");
        //DebugScContext(stream_in_id); debug("\r\n");
    } else {
        //debug("ERROR\r\n");        
    } 

#if 0
    /* Map the input queue and the auto-fetch stream */
    debug("\r\nMap the input queue and the auto-fetch stream...");
    if(McScMap(queue_in_id, stream_autofetch_id)) {
        debug("OK\r\n");
        DebugMcQueueContext(queue_in_id); debug("\r\n");
        DebugScContext(stream_autofetch_id); debug("\r\n");
    } else {
        debug("ERROR\r\n");        
    }  
#endif	
        
    /* Write blank data into the queue to avoid underflow */   
    //debug("\r\nWrite blank data(32 bytes) into the queue to avoid underflow!\r\n");
    if (channelAC[channel_number].lwAudioOutputType == AUDIO_ENCODING_LINEAR)	
        len = 320 * 3;    // 60 ms' data
    else	
        len = 160 * 3;    // 60 ms' data		
    for(i = 0; i < len; i+=4) {    	    	
        if(McWrQueue(queue_out_id, 0) == FAILED) {                                    
            //debug("Write Error!\r\n");              
            break;           
        }
    }
    //DebugMcQueueContext(queue_out_id); debug("\r\n");
    //DebugScContext(stream_out_id); debug("\r\n");

    /* Open the audio output stream */
    //debug("\r\nOpen the audio output stream 0x%x...", stream_out_id);
    if(ScOpen(stream_out_id)) {
        //debug("OK\r\n");
    } else {
        //debug("ERROR\r\n"); 
    } 
    //DebugMcQueueContext(queue_out_id); debug("\r\n");
    //DebugScContext(stream_out_id); debug("\r\n"); 

#if 0    
    /* Open the auto-fetch stream */
    printf("\r\nOpen the auto-fetch stream 0x%x...", stream_autofetch_id);
    if(ScOpen(stream_autofetch_id)) {
        debug("OK\r\n");
    } else {
        printf("ERROR\r\n");      
    } 
    DebugMcQueueContext(queue_in_id); debug("\r\n");
    DebugScContext(stream_autofetch_id); debug("\r\n");             
#endif

//debug("gg");
	
    /* the audio input stream should be opened after audio is enabled! */    
    if(audio_opened == TRUE)  {
	if(ScOpen(stream_in_id)) {
	    ;//debug("OK\r\n");
	} else {
	    //debug("ERROR\r\n"); 
	    return FAILED;
	}
    }
         
    return SUCCESSFUL;
}

/*
***************************************************************************
*                             AppAudioChannelClose
*
* Description: 
*
* Arguments  : 
*
* Return     : SUCCESSFUL    
*              FAILED        if the audio interface is opened/the channel
*                            number or parameter is wrong/queue, stream 
*                            operation is wrong, return FAILED.
*
* Note(s)    : 
***************************************************************************
*/
int AppAudioChannelClose(int channel_number)
{
    unsigned int stream_in_id, stream_out_id, stream_autofetch_id;
    int data;	
    //if(audio_opened)
    //    return FAILED;		
        
    switch(channel_number) {
        case 0:                
            stream_out_id = AUD_OUT_CH0_S;
            stream_in_id  = AUD_IN_CH0_S;
            stream_autofetch_id = AUD_AUTOFETCH_CH0_S;  // SOFT_RD_1_S                                               
            break;
                
        case 1:                                                       
            stream_out_id = AUD_OUT_CH1_S;
            stream_in_id  = AUD_IN_CH1_S;
            stream_autofetch_id = AUD_AUTOFETCH_CH1_S;  // SOFT_RD_1_S
            break;
            
        case 2:                                                       
            stream_out_id = AUD_OUT_CH2_S;
            stream_in_id  = AUD_IN_CH2_S;
            stream_autofetch_id = AUD_AUTOFETCH_CH2_S;  // SOFT_RD_1_S
            break;
            
        case 3:                                                       
            stream_out_id = AUD_OUT_CH3_S;
            stream_in_id  = AUD_IN_CH3_S;
            stream_autofetch_id = AUD_AUTOFETCH_CH3_S;  // SOFT_RD_1_S
            break;
            
        default:
            return FAILED;
    }
    
    rAUD_SID = rAUD_SID & (~(0x3<<(channel_number * 2 + 24)));    // clear ICH_CNT
    rAUD_SID = rAUD_SID & (~(0x3<<(channel_number * 2 + 16)));    // clear OCH_CNT                     
    rAUD_SID = rAUD_SID & (~(1 << (channel_number + 12)));        // clear prefetch

    /* Close the audio output stream */
    //printf("\r\nClose audio output stream 0x%x", stream_out_id);
    if(ScClose(stream_out_id)) {
    //    debug("OK\r\n");
    } else {
    //    printf("ERROR\r\n");      
    }
    /* read to make stream done! */
    //DebugScContext(stream_out_id); debug("\r\n"); 
    while(1) {
        if(ScRd(stream_out_id, &data) == FAILED)
            break;	
    } 
    //DebugScContext(stream_out_id); debug("\r\n");     
    
    /* Close the audio input stream */
    //printf("\r\nClose the audio input stream 0x%x...", stream_in_id);
    if(ScClose(stream_in_id)) {
    //    debug("OK\r\n");
    } else {
    //    printf("ERROR\r\n");      
    } 
    /* read to make stream done! */
#if 0
    DebugScContext(stream_in_id); debug("\r\n"); 
    while(1) {
        if(ScRd(stream_in_id, &data) == FAILED)
            break;	
    } 
    DebugScContext(stream_in_id); debug("\r\n");         
#endif

#if 0    
    /* Close the audio autofetch stream */
    printf("\r\nClose the autofetch stream 0x%x...", stream_autofetch_id);
    if(ScClose(stream_autofetch_id)) {
        debug("OK\r\n");
    } else {
        printf("ERROR\r\n");      
    }         
    /* read to make stream done! */
    DebugScContext(stream_autofetch_id); debug("\r\n"); 
    while(1) {
        if(ScRd(stream_autofetch_id, &data) == FAILED)
            break;	
    } 
    DebugScContext(stream_autofetch_id); debug("\r\n");         
#endif

    return SUCCESSFUL;       
}
#endif

/*
***************************************************************************
*                             AppAudioOpen
*
* Description: 
*
* Arguments  : none.
*
* Return     : SUCCESSFUL    
*              FAILED        
*
* Note(s)    : 
***************************************************************************
*/

int AppAudioOpen(void)
{
    if(audio_opened)
        return FAILED;          
    
    /* Don't forget to open the audio input stream */    
    if(rAUD_SID & 0xc0000000) {    // channel 3 is active?        
        //debug("\r\nOpen the input stream 0x%x...", AUD_IN_CH3_S);
        if(ScOpen(AUD_IN_CH3_S)) {
            ;//debug("OK\r\n");
        } else {
            debug("ERROR\r\n"); 
            return FAILED;
        } 
        //DebugMcQueueContext(AUD_IN_CH3_M); debug("\r\n");
        //DebugScContext(AUD_IN_CH3_S); debug("\r\n"); 
    }
    
    if(rAUD_SID & 0x30000000) {    // channel 2 is active?        
        //debug("\r\nOpen the input stream 0x%x...", AUD_IN_CH2_S);
        if(ScOpen(AUD_IN_CH2_S)) {
            ;//debug("OK\r\n");
        } else {
            debug("ERROR\r\n"); 
            return FAILED;
        }     
        //DebugMcQueueContext(AUD_IN_CH2_M); debug("\r\n");
        //DebugScContext(AUD_IN_CH2_S); debug("\r\n"); 
    }
    
    if(rAUD_SID & 0x0c000000) {    // channel 1 is active?        
        //debug("\r\nOpen the input stream 0x%x...", AUD_IN_CH1_S);
        if(ScOpen(AUD_IN_CH1_S)) {
            ;//debug("OK\r\n");
        } else {
            debug("ERROR\r\n"); 
            return FAILED;
        }     
        //DebugMcQueueContext(AUD_IN_CH1_M); debug("\r\n");
        //DebugScContext(AUD_IN_CH1_S); debug("\r\n");             
    }

    if(rAUD_SID & 0x03000000) {    // channel 0 is active?        
        //debug("\r\nOpen the input stream 0x%x...", AUD_IN_CH0_S);
        if(ScOpen(AUD_IN_CH0_S)) {
            ;//debug("OK\r\n");
        } else {
            debug("ERROR\r\n"); 
            return FAILED;
        }
        //DebugMcQueueContext(AUD_IN_CH0_M); debug("\r\n");
        //DebugScContext(AUD_IN_CH0_S); debug("\r\n"); 
    }
    
    /* enable the queue interrupt */
    McIntrEn();
    
    /* Clear the VCNT and set VCNT threshold(160 bytes) */
    RiscClrVCNT();    
    RiscVCNTIntrEn(AUDIO_MAX_FRAME_LEN/* vcnt_val */);    // 160 bytes    
   // RiscVCNTIntrEn(AUDIO_MAX_FRAME_LEN/* vcnt_val */);    // 160 bytes    
 
    audio_opened = TRUE;
    //gtavsync.syncmode = AVSYNC;     //wy: not need yet
    
    return SUCCESSFUL;         
}

#if 0
int AppAudioOpen(void)
{
//debug("bb");
    if(audio_opened)
        return FAILED;	
    
    // enable audio block
    CctlAudioBlockEn();
    //rCCTL_BLOCK_ENABLE |= CCTL_AUDIO_BLOCK;            
        
    INTR_DIS();              // Disable the audio interrupts

    debug("\r\nopen audio, then the input stream!\r\n");            
    EN_AUDIO();              // Enable audio interface.          
    
    /* Don't forget to open the audio input stream */    
    if(rAUD_SID & 0xc0000000) {    // channel 3 is active?        
        //debug("\r\nOpen the input stream 0x%x...", AUD_IN_CH3_S);
        if(ScOpen(AUD_IN_CH3_S)) {
            ;//debug("OK\r\n");
        } else {
            debug("ERROR\r\n"); 
            return FAILED;
        } 
        //DebugMcQueueContext(AUD_IN_CH3_M); debug("\r\n");
        //DebugScContext(AUD_IN_CH3_S); debug("\r\n"); 
    }
    
    if(rAUD_SID & 0x30000000) {    // channel 2 is active?        
        //debug("\r\nOpen the input stream 0x%x...", AUD_IN_CH2_S);
        if(ScOpen(AUD_IN_CH2_S)) {
            ;//debug("OK\r\n");
        } else {
            debug("ERROR\r\n"); 
            return FAILED;
        }     
        //DebugMcQueueContext(AUD_IN_CH2_M); debug("\r\n");
        //DebugScContext(AUD_IN_CH2_S); debug("\r\n"); 
    }
    
    if(rAUD_SID & 0x0c000000) {    // channel 1 is active?        
        //debug("\r\nOpen the input stream 0x%x...", AUD_IN_CH1_S);
        if(ScOpen(AUD_IN_CH1_S)) {
            ;//debug("OK\r\n");
        } else {
            debug("ERROR\r\n"); 
            return FAILED;
        }     
        //DebugMcQueueContext(AUD_IN_CH1_M); debug("\r\n");
        //DebugScContext(AUD_IN_CH1_S); debug("\r\n");             
    }

    if(rAUD_SID & 0x03000000) {    // channel 0 is active?        
        //debug("\r\nOpen the input stream 0x%x...", AUD_IN_CH0_S);
        if(ScOpen(AUD_IN_CH0_S)) {
            ;//debug("OK\r\n");
        } else {
            debug("ERROR\r\n"); 
            return FAILED;
        }
        //DebugMcQueueContext(AUD_IN_CH0_M); debug("\r\n");
        //DebugScContext(AUD_IN_CH0_S); debug("\r\n"); 
    }
    
    /* enable the queue interrupt */
    McIntrEn();
    
    /* Clear the VCNT and set VCNT threshold(160 bytes) */
    RiscClrVCNT();    
    RiscVCNTIntrEn(AUDIO_MAX_FRAME_LEN/* vcnt_val */);    // 160 bytes    
   // RiscVCNTIntrEn(AUDIO_MAX_FRAME_LEN/* vcnt_val */);    // 160 bytes    
	
    audio_opened = TRUE;
    
    return SUCCESSFUL;        	
}
#endif

/*
***************************************************************************
*                             AppAudioClose
*
* Description: 
*
* Arguments  : none.
*
* Return     : SUCCESSFUL    
*              FAILED        
*
* Note(s)    : 
***************************************************************************
*/
int AppAudioClose(void)
{
    if(audio_opened == FALSE)
        return FAILED;	                

#if 0
    /* close the input stream at first */
    if(rAUD_SID & 0xc0000000) {    // channel 3 is active?        
        debug("\r\nClose the input stream 0x%x...", AUD_IN_CH3_S);
        if(ScClose(AUD_IN_CH3_S)) {
            debug("OK\r\n");
        } else {
            debug("ERROR\r\n"); 
            return FAILED;
        }
     
        DebugMcQueueContext(AUD_IN_CH3_M); debug("\r\n");
        DebugScContext(AUD_IN_CH3_S); debug("\r\n");     
    }

    if(rAUD_SID & 0x30000000) {    // channel 2 is active?        
        debug("\r\nClose the input stream 0x%x...", AUD_IN_CH3_S);
        if(ScClose(AUD_IN_CH2_S)) {
            debug("OK\r\n");
        } else {
            debug("ERROR\r\n"); 
            return FAILED;
        }
     
        DebugMcQueueContext(AUD_IN_CH2_M); debug("\r\n");
        DebugScContext(AUD_IN_CH2_S); debug("\r\n");     
    }
    
    if(rAUD_SID & 0x0c000000) {    // channel 1 is active?        
        debug("\r\nClose the input stream 0x%x...", AUD_IN_CH3_S);
        if(ScClose(AUD_IN_CH1_S)) {
            debug("OK\r\n");
        } else {
            debug("ERROR\r\n"); 
            return FAILED;
        }
     
        DebugMcQueueContext(AUD_IN_CH1_M); debug("\r\n");
        DebugScContext(AUD_IN_CH1_S); debug("\r\n");     
    }
    
    if(rAUD_SID & 0x03000000) {    // channel 0 is active?        
        debug("\r\nClose the input stream 0x%x...", AUD_IN_CH3_S);
        if(ScClose(AUD_IN_CH0_S)) {
            debug("OK\r\n");
        } else {
            debug("ERROR\r\n"); 
            return FAILED;
        }
     
        DebugMcQueueContext(AUD_IN_CH0_M); debug("\r\n");
        DebugScContext(AUD_IN_CH0_S); debug("\r\n");     
    }        
#endif

    /* clsoe the audio */
    DIS_AUDIO();
    
    audio_opened = FALSE;       

    return SUCCESSFUL;
}

/*
***************************************************************************
*                             AppAudioReset
*
* Description: 
*
* Arguments  : none.
*
* Return     : SUCCESSFUL    
*              FAILED        
*
* Note(s)    : 
***************************************************************************
*/
void AppAudioReset(void)
{
    rCCTL_BLOCK_RESET |= CCTL_AUDIO_BLOCK;
    /* maybe need some delay to wait reset complete */
    rCCTL_BLOCK_RESET &= ~(CCTL_AUDIO_BLOCK);
}

/*
***************************************************************************
*                             AppAudioResetReEnable
*
* Description: 
*
* Arguments  : none.
*
* Return     : SUCCESSFUL    
*              FAILED        
*
* Note(s)    : 
***************************************************************************
*/
void AppAudioResetReEnable(void)
{
    rCCTL_BLOCK_RESET |= CCTL_AUDIO_BLOCK;
    /* maybe need some delay to wait reset complete */
    rCCTL_BLOCK_RESET &= ~(CCTL_AUDIO_BLOCK);
    audio_opened = FALSE;
    AppAudioClkOpen();
    AppAudioOpen();
}


/*
***************************************************************************
*                             AppAudioWriteData
*
* Description: 
*
* Arguments  : 
*
* Return     : 
*
* Note(s)    : length is counted by bytes!
***************************************************************************
*/
int AppAudioWriteData(int channel_number, int *data, int length)
{
    unsigned int queue_out_id;
    	
    if(audio_opened == FALSE)
        return FAILED;	 	
        	        
    switch(channel_number) {
        case 0:                               
            queue_out_id = AUD_OUT_CH0_M;        
            break;
            
        case 1:                               
            queue_out_id = AUD_OUT_CH1_M;        
            break;            

        case 2:                               
            queue_out_id = AUD_OUT_CH2_M;        
            break;

        case 3:                               
            queue_out_id = AUD_OUT_CH3_M;        
            break;                        
            
        default:
            return FAILED;
            break;
    }
    
    if(!(rAUD_SID & (0x3<<(channel_number * 2 + 24))))    // the channel is NOT active?                   
        return FAILED;    // This channel is not active!    
        
#ifdef VER001    
    while(length > 0) {
        if(McWrQueue(queue_out_id, *data++) == FAILED)
            return FAILED;
        length -= 4;
    }        
#endif

    if(McRdQueueCount(queue_out_id) > McRdQueueActualBufsize(queue_out_id) / 2)
        return FAILED;	

    while(length > 0) {		
        if(McWrQueue(queue_out_id, *data++) == FAILED)
            return FAILED;
        length -= 4;
    }   

    return SUCCESSFUL;
}

/*
***************************************************************************
*                             AppAudioReadData
*
* Description: 
*
* Arguments  : 
*
* Return     : 
*
* Note(s)    : length is counted by bytes!
***************************************************************************
*/
int AppAudioReadData(int channel_number, int *data, int length)
{
    unsigned int queue_in_id;
    int i;
    	
    if(audio_opened == FALSE)
    {
	debug("a0\n");
        return FAILED;	 	
    }
        	        
    switch(channel_number) {
        case 0:                               
            queue_in_id = AUD_IN_CH0_M;        
            break;
            
        case 1:                               
            queue_in_id = AUD_IN_CH1_M;        
            break;            

        case 2:                               
            queue_in_id = AUD_IN_CH2_M;        
            break;

        case 3:                               
            queue_in_id = AUD_IN_CH3_M;        
            break;                        
            
        default:
            return FAILED;
            break;
    }
    
    if(!(rAUD_SID & (0x3<<(channel_number * 2 + 24))))    // the channel is NOT active? 
    {
        return FAILED;    // This channel is not active!            
    }

    if(McRdQueueCount(queue_in_id) < length)
    {
	//debug("a2 McRdQ=%d len=%d\n",McRdQueueCount(queue_in_id),length);
        return FAILED;	
    }

    while(length > 0) {		
        if(McRdQueue(queue_in_id, data++) == FAILED)
	{
	    return FAILED;
	}
        length -= 4;
    }   

    return SUCCESSFUL;
}

/*
***************************************************************************
*                             AppAudioReadOutputCount
*
* Description: Read the count in the audio output queue!
*
* Arguments  : 
*
* Return     : 
*
* Note(s)    : counted by bytes!
***************************************************************************
*/
int AppAudioReadOutputCount(unsigned int channel_number)
{
    if(channel_number > 4)
        return -1;		
    return(McRdQueueCount(channel_number + 4));
}	
	
#ifdef VER001  
/*
***************************************************************************
*                             AppAudioReadHistoryData
*
* Description: 
*
* Arguments  : channel_number    is the channel number, can be 0, 1, 2, 3
*              data              is the data address to be read
*              length            is the length of data to be read, 
*                                  counted by bytes! Should be multi of 4.
*              time_stamp        is the time you want to read. For example, 
*                                  if you want to read the data at the last 10ms,
*                                  and the sample bits is 8- bit, sample rate is
*                                  8k, then you should set time_stamp as 80. 
*
* Return     : The actual length of the read data, counted by bytes!
*
* Note(s)    : 
***************************************************************************
*/
int AppAudioReadHistoryData(int channel_number, int *data, int length, int time_stamp)
{
    unsigned int i, queue_out_id, rd_addr, queue_start_addr, queue_end_addr;
    unsigned int *pTmp;
    	
    if(audio_opened == FALSE)
        return 0;	 	
        	        
    switch(channel_number) {
        case 0:                               
            queue_out_id = AUD_OUT_CH0_M;
            queue_start_addr = AUDIO_OUT_CH0_M_ADDR | 0xa0000000;        
            break;
            
        case 1:                               
            queue_out_id = AUD_OUT_CH1_M;        
            queue_start_addr = AUDIO_OUT_CH1_M_ADDR | 0xa0000000;
            break;            

        case 2:                               
            queue_out_id = AUD_OUT_CH2_M;        
            queue_start_addr = AUDIO_OUT_CH2_M_ADDR | 0xa0000000;
            break;

        case 3:                               
            queue_out_id = AUD_OUT_CH3_M;        
            queue_start_addr = AUDIO_OUT_CH3_M_ADDR | 0xa0000000;
            break;                        
            
        default:
            return 0;
            break;
    }
    
    if(!(rAUD_SID & (0x3<<(channel_number * 2 + 24))))    // the channel is NOT active?                   
        return 0;    // This channel is not active!    

    if((length + McRdQueueCount(queue_out_id) + time_stamp[channel_number]) >= McRdQueueActualBufsize(queue_out_id))
        return 0;
    
    rd_addr = McRdQueueReadAddr(queue_out_id) | 0xa0000000;
    queue_end_addr = queue_start_addr + McRdQueueActualBufsize(queue_out_id) - 4;
    for(i = 0; i < time_stamp[channel_number]; i +=4) {
    	if(rd_addr < queue_start_addr)
            rd_addr = queue_end_addr;
        rd_addr -= 4;
    }

    pTmp = (unsigned int *)((((unsigned int)data) + length - 4) | 0xa0000000);    
    for(i = 0; i < length; i += 4) {
    	if(rd_addr < queue_start_addr)
            rd_addr = queue_end_addr;
        *pTmp-- = *(unsigned int *)rd_addr;
        rd_addr -= 4;
    }
    
    return length;    	
}
#endif

/*
***************************************************************************
*                             AppAudioReadHistoryData
*
* Description: 
*
* Arguments  : channel_number    is the channel number, can be 0, 1, 2, 3
*              data              is the data address to be read
*              length            is the length of data to be read, 
*                                  counted by bytes! Should be multi of 4.
*              time_stamp        is the time you want to read. For example, 
*                                  if you want to read the data at the last 10ms,
*                                  and the sample bits is 8- bit, sample rate is
*                                  8k, then you should set time_stamp as 80. 
*
* Return     : The actual length of the read data, counted by bytes!
*
* Note(s)    : 
***************************************************************************
*/
int AppAudioReadHistoryData(int channel_number, int *data, int length)
{
    unsigned int i, queue_out_id, rd_addr, queue_start_addr, queue_end_addr;
    unsigned int *pTmp, history_pointer;
    //static unsigned int channel_history_pointer[4];	
    unsigned int tmp;	
    	
    if(audio_opened == FALSE)
        return 0;	 	

    if(!(rAUD_SID & (0x3<<(channel_number * 2 + 24))))    // the channel is NOT active?                   
        return 0;    // This channel is not active!  
        	        
    switch(channel_number) {
        case 0:                               
            queue_out_id = AUD_OUT_CH0_M;
            queue_start_addr = AUDIO_OUT_CH0_M_ADDR | 0xa0000000;
            queue_end_addr = queue_start_addr + McRdQueueActualBufsize(queue_out_id) - 4;
            /*			
            if(channel_history_pointer_initialized[0] == 0) {
                debug("|");				
                tmp = McRdQueueReadAddr(queue_out_id) | 0xa0000000;				
                for(i = 0; i < (time_stamp + length); i +=4) {
                    tmp -= 4;
                    if(tmp < queue_start_addr)
                        tmp = queue_end_addr;
                }
                channel_history_pointer_initialized[0] = 1;				
                channel_history_pointer[0]	= tmp;
            }
            */
            history_pointer = channel_history_pointer[0];            
			
            for(i = 0; i < length; i += 4) {
               *data++ = *(unsigned int *)history_pointer;	
               history_pointer += 4;   
    	        if(history_pointer > queue_end_addr)
                   history_pointer = queue_start_addr;
            }	            

            channel_history_pointer[0] = history_pointer;
			
            break;
            
        case 1:                               
            queue_out_id = AUD_OUT_CH1_M;        
            queue_start_addr = AUDIO_OUT_CH1_M_ADDR | 0xa0000000;
            queue_end_addr = queue_start_addr + McRdQueueActualBufsize(queue_out_id) - 4;	
            /*			
            if(channel_history_pointer_initialized[1] == 0) {
                tmp = McRdQueueReadAddr(queue_out_id) | 0xa0000000;				
                for(i = 0; i < (time_stamp + length); i +=4) {
                    tmp -= 4;
                    if(tmp < queue_start_addr)
                        tmp = queue_end_addr;
                }
                channel_history_pointer_initialized[1] = 1;				
                channel_history_pointer[1]	= tmp;
            }
            */
            history_pointer = channel_history_pointer[1];            
			
            for(i = 0; i < length; i += 4) {
               *data++ = *(unsigned int *)history_pointer;	
               history_pointer += 4;		
    	        if(history_pointer > queue_end_addr)
                   history_pointer = queue_start_addr;
            }	            

            channel_history_pointer[1] = history_pointer;
						
            break;            

        case 2:                               
            queue_out_id = AUD_OUT_CH2_M;        
            queue_start_addr = AUDIO_OUT_CH2_M_ADDR | 0xa0000000;
            queue_end_addr = queue_start_addr + McRdQueueActualBufsize(queue_out_id) - 4;	
            /*			
            if(channel_history_pointer_initialized[2] == 0) {
                tmp = McRdQueueReadAddr(queue_out_id) | 0xa0000000;				
                for(i = 0; i < (time_stamp + length); i +=4) {
                    tmp -= 4;
                    if(tmp < queue_start_addr)
                        tmp = queue_end_addr;
                }
                channel_history_pointer_initialized[2] = 1;				
                channel_history_pointer[2]	= tmp;
            }
            */
            history_pointer = channel_history_pointer[2];            
			
            for(i = 0; i < length; i += 4) {
               *data++ = *(unsigned int *)history_pointer;	
               history_pointer += 4;		
    	        if(history_pointer > queue_end_addr)
                   history_pointer = queue_start_addr;
            }	            

            channel_history_pointer[2] = history_pointer;
						
            break;

        case 3:                               
            queue_out_id = AUD_OUT_CH3_M;        
            queue_start_addr = AUDIO_OUT_CH3_M_ADDR | 0xa0000000;
            queue_end_addr = queue_start_addr + McRdQueueActualBufsize(queue_out_id) - 4;	
            /*			
            if(channel_history_pointer_initialized[3] == 0) {
                tmp = McRdQueueReadAddr(queue_out_id) | 0xa0000000;				
                for(i = 0; i < (time_stamp + length); i +=4) {
                    tmp -= 4;
                    if(tmp < queue_start_addr)
                        tmp = queue_end_addr;
                }
                channel_history_pointer_initialized[3] = 1;				
                channel_history_pointer[3]	= tmp;
            }
            */
            history_pointer = channel_history_pointer[3]; 
			
            for(i = 0; i < length; i += 4) {
               *data++ = *(unsigned int *)history_pointer;	
               history_pointer += 4;	
    	        if(history_pointer > queue_end_addr)
                   history_pointer = queue_start_addr;
            }	            

            channel_history_pointer[3] = history_pointer;
						
            break;                        
            
        default:
            return 0;
            break;
    }
    
    return length;    	
}

/*
***************************************************************************
*                             AppAudioGetStatus
*
* Description: Get the status of the audio interface.
*
* Arguments  : none.
*
* Return     : the status of the audio.
*
* Note(s)    : 
***************************************************************************
*/
int AppAudioGetStatus(void)
{        
    if(audio_opened)
        return AUDIO_OPENED;
    else	
        return AUDIO_CLOSED;		
}

/*
***************************************************************************
*                             AudioRegInit
*
* Description: This routine inits the audio registers.
*
* Arguments  : none.
*
* Return     : none.
*
* Note(s)    : 
***************************************************************************
*/
void AudioRegInit(void)
{
    // bit    31   30     29    28    27      
    // name   ENA  FS_IO  FS_W  FS_P  CLK_IO  
    // binary 1    0      0     0     0 
    //------------------------------------------------------------------
    // bit    26      25      24     23     22     21:16 15:8     7:0
    // name   DI_DLY  DO_DLY  FS_AL  DI_AL  DO_AL  N.A.  CLK_DIV  FS_DIV
    // binary 1       1       0      0      0      x     0        0
    rAUD_CTL = BIT_FS_IO | BIT_FS_W | BIT_FS_P | BIT_CLK_IO | BIT_DI_DLY | BIT_DO_DLY | \
               BIT_FS_AL | BIT_DI_AL | BIT_DO_AL | BIT_CLK_DIV | BIT_FS_DIV;

    // bit    31:24 23:16 15:8  7:0
    // name   ITS3  ITS2  ITS1  ITS0
    // binary 1    0      0     0     
    rAUD_INTS = BIT_ITS3 | BIT_ITS2 | BIT_ITS1 | BIT_ITS0;

    // bit    31:24 23:16 15:8  7:0
    // name   OTS3  OTS2  OTS1  OTS0
    // binary 1    0      0     0     
    rAUD_OUTTS = BIT_OTS3 | BIT_OTS2 | BIT_OTS1 | BIT_OTS0;

    // bit    31:30     29:28     27:26     25:24
    // name   ICH3_CNT  ICH2_CNT  ICH1_CNT  ICH0_CNT
    // binary 3         3         3         3        
    //-------------------------------------------------
    // bit    22:23     21:20     19:18     17:16
    // name   OCH3_CNT  OCH2_CNT  OCH1_CNT  OCH0_CNT
    // binary 3         3         3         3    
    //-------------------------------------------------
    // bit    15       14       13       12
    // name   OCH3_PF  OCH2_PF  OCH1_PF  OCH0_PF  
    // binary 1        1        1        1    
    //-------------------------------------------------
    // bit    11       10
    // name   OCH_VAL  ICH_VAL
    // binary 1        0      
    rAUD_SID = BIT_ICH3_CNT | BIT_ICH2_CNT | BIT_ICH1_CNT | BIT_ICH0_CNT |
               BIT_OCH3_CNT | BIT_OCH2_CNT | BIT_OCH1_CNT | BIT_OCH0_CNT |
               BIT_OCH3_PF  | BIT_OCH2_PF  | BIT_OCH1_PF  | BIT_OCH0_PF  | 
               OCH_VAL      | ICH_VAL;

    // Set Audio clock divider value
    rCCTL_AUDIO_CLOCK_DIVIDE = CCTL_AUDIO_CLOCK_ENABLE | CCTL_AUDIO_PLL_SOURCE_RISC | CCTL_AUDIO_DIVIDER_VALUE;    
    

    // bit    31       30       29       
    // name   INTR_EN  INTR_WR  INTR_RD  
    // binary 1        1        1
    //rAUD_STAT = 0xe<<29;
}

/*
***************************************************************************
*                            AudioStart
*
* Description: The routine does the two things:
*                  1. Enable auido interface;
*                  2. Disable the interrupt of audio interface.  
*
* Arguments  : none.
*
* Return     : none.
*
* Note(s)    : 
***************************************************************************
*/
void AudioStart(void)
{
    INTR_DIS();              // Disable the audio interrupts
    EN_AUDIO();              // Enable audio interface.      
}

/*
***************************************************************************
*                             AudioIntrEn
*
* Description: The routine does the two things:
*                  1. clear the interrupt source;
*                  2. Enable the interrupt of audio interface.  
*
* Arguments  : none.
*
* Return     : none.
*
* Note(s)    : 
***************************************************************************
*/
void AudioIntrEn(void)
{
    unsigned int isr_base_addr;

    // the fourth-interrupt table address	
    if(RiscRdID()  == RISC0)
        isr_base_addr = RISC0_FOURTH_VECTOR_B_ADDR | 0xa0000000;    // phy-addr is used to avoid flush cache
    else
        isr_base_addr = RISC1_FOURTH_VECTOR_B_ADDR | 0xa0000000;    // phy-addr is used to avoid flush cache     
    
    *(unsigned int *)(isr_base_addr + 0x70) = (unsigned int)AudioISR;   
    
    RiscIntrEn(AUD_INTR);  
    	
    CLEAR_INTR_RD();       // Before enabling the interrupt, clear the interrupt source
    CLEAR_INTR_WR();         
    INTR_EN();             // Enable the interrupt of audio interface. 
}

/*
***************************************************************************
*                           AudioIntrDis
*
* Description: The routine does the two things:
*                  1. clear the interrupt source;
*                  2. Disable the interrupt of audio interface.  
*
* Arguments  : none.
*
* Return     : none.
*
* Note(s)    : 
***************************************************************************
*/
void AudioIntrDis(void)
{
    //CLEAR_INTR_RD();        // Before enabling the interrupt, clear the interrupt source
    //CLEAR_INTR_WR();         
    INTR_DIS();               // Disable the interrupt of audio interface. 
}

/*
***************************************************************************
*                              AudioISR
*
* Description: 
*
* Arguments: 
*
* Return     : 
*
* Note(s)    : 
***************************************************************************
*/
void AudioISR(void)
{
    unsigned int aud_status;
    
    aud_status = rAUD_STAT;
    
    if(aud_status & 0x40000000) {    // Audio to DBUS dropped
        debug("\r\nAudio input interrupt! rAUD_STAT = 0x%x\r\n", rAUD_STAT);
        
        ScOpen(AUD_IN_CH0_S);
        PrinfScContext(AUD_IN_CH0_S); debug("\r\n");
        
        rAUD_STAT |= 0x40000000;     // clear the interrupt source
        //return;
    }
    
    if(aud_status & 0x20000000) {    // Audio from DBUS dropped
        debug("\r\nAudio output interrupt! rAUD_STAT = 0x%x\r\n", rAUD_STAT);
        
        ScOpen(AUD_OUT_CH0_S);
        PrinfScContext(AUD_OUT_CH0_S); debug("\r\n");        
        
        rAUD_STAT |= 0x20000000;     // clear the interrupt source
        return;
    }    
    
}



/*
***************************************************************************
*                             AppAudioClkOpen
*
* Description: 
*
* Arguments : none.
*
* Return     : SUCCESSFUL    
*              FAILED        
*
* Note(s)    : This function open the audio clock for slic, should be called before slic task, otherwise
*                  the slic cannot work!
***************************************************************************
*/
void AppAudioClkOpen(void)
{
    AppAudioTimingSet();

    // enable audio block
    CctlAudioBlockEn();
        
    INTR_DIS();              // Disable the audio interrupts

    EN_AUDIO();              // Enable audio interface.             
}


