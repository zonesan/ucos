/*
***************************************************************************
*                  Copyright (c) 2005 vBridge Microsystem, Inc.  
*                        Unpublished & Not for Publication
*                              All Rights Reserved                   
*   
* File        : Mc.c                     
*                                                                         
* Description : 
*             
* Date        : Dec 8, 2005                                                           
***************************************************************************
*/


#include "chip_cfg.h"

#include "Mc.h"
#include "Sc.h"
#include "2risc_memory_map.h"
#include "comm.h"
#ifdef AUDIO_MODULE_INCLUDE
#include "audio.h"
#include "audio_cfg.h"

#include "../port/os_cpu.h"
#include "../uCOS Configure/os_cfg.h"
#include "../uCOS2.52/uCOS_II.h"

#include "def_type.h"
#include "../G726/G726.h"
#include "two_risc_audio.h"

//LONG glwAudioOutputType; 
extern tAudioCharacter channelAC[MAX_AUDIO_CHANNEL];
#endif
#include "two_risc_statistics.h"

/*
***************************************************************************
*                            McQueueInit
*
* Description: initialize the memory context as a queue context.
*
* Arguments  : mID            is the memory context ID.
*              status         specifies the status of the buffer. 
*                             Generally, status = Q_EMPTY.
*              buf_size       specifies the buffer size. 
*                             The minimum is 256 bytes, 
*                             the maximum is 16K bytes.
*              subbuf_size    specifies the unit for EMPTY status.
*              base_addr      is the base address. 
*
* Return     : none
*
* Note(s)    : 1. The read & write pointer are zero. This function has problems!
*              2. base_addr can be the physical or virtual address.
***************************************************************************
*/
int McQueueInit(int mID, int status, int buf_size, int subbuf_size, \
                 int base_addr)
{   
#if 0	
    int context_hi, context_lo;
    
    /* write memory context MSB(bit[63:32]) */
    context_hi = (QUEUE<<(MC_TYPE_OFFSET - 32)) | (status<<14) | (buf_size<<11) |\
                 (subbuf_size<<8);
    rMC_CTXT(mID, W_CONTEXT_HI) = context_hi;
    
    /* write memory context LSB(bit[31:0]). */
    context_lo = base_addr & 0x3ffc000;   // Base address is from bit 25 to 14
    rMC_CTXT(mID, W_CONTEXT_LO) = context_lo;
    
    if(rMC_CTXT(mID, R_CONTEXT_HI) != context_hi)
        return(FAILED);

    if(rMC_CTXT(mID, R_CONTEXT_LO) != context_lo)
        return(FAILED);    
        
    return(SUCCESSFUL);
#endif

    int context_hi, context_lo, wr_pointer;
    
    wr_pointer = base_addr & 0x3ffc;
    
    /* write memory context MSB(bit[63:32]) */
    context_hi = (QUEUE<<(MC_TYPE_OFFSET - 32)) | (status<<14) | (buf_size<<11) |\
                 (subbuf_size<<8) | (wr_pointer >> 6);
    rMC_CTXT(mID, W_CONTEXT_HI) = context_hi;
    
    /* write memory context LSB(bit[31:0]). */
    context_lo = (wr_pointer << 26) | (base_addr & 0x3fffffc);   
    rMC_CTXT(mID, W_CONTEXT_LO) = context_lo;
    
    if(rMC_CTXT(mID, R_CONTEXT_HI) != context_hi)
        return(FAILED);

    if(rMC_CTXT(mID, R_CONTEXT_LO) != context_lo)
        return(FAILED);    
        
    return(SUCCESSFUL);    
} 

/*
***************************************************************************
*                            McQueueReInit
*
* Description: re-initialize a queue context.
*
* Arguments  : mID            is the memory context ID.
*              status         specifies the status of the buffer. 
*                             Generally, status = Q_EMPTY.
*              buf_size       specifies the buffer size. 
*                             The minimum is 256 bytes, 
*                             the maximum is 16K bytes.
*              subbuf_size    specifies the unit for EMPTY status.
*              base_addr      is the base address. 
*
* Return     : none
*
* Note(s)    : 1. The read & write pointer are zero.
*              2. base_addr can be the physical or virtual address.
***************************************************************************
*/
int McQueueReInit(int mID)
{   
    McQueueInit(mID, Q_EMPTY /* status */, McRdQueueBufsize(mID) /* buf_size */ ,\
                  McRdQueueSubBufsize(mID)/* subbuf_size */, McRdQueueBaseAddr(mID) /* base_addr */);
} 

/*
***************************************************************************
*                             McRandomInit
*
* Description: initialize the memory context as a random context.
*
* Arguments  : mID            is the memory context ID.
*              count          is the byte counter for the transfer. 
*              base_addr      is the Sdram pointer. 
*
* Return     : none
*
* Note(s)    : 
***************************************************************************
*/
int McRandomInit(int mID, int count, int base_addr)
{   
    int context_hi, context_lo;
    
    /* write memory context MSB(bit[63:32]) */
    context_hi = count >> 6;
    rMC_CTXT(mID, W_CONTEXT_HI) = context_hi;
    
    /* write memory context LSB(bit[31:0]). */
    context_lo = (count<<26) | (base_addr & (~0xa0000000));
    rMC_CTXT(mID, W_CONTEXT_LO) = context_lo;
    
    if(rMC_CTXT(mID, R_CONTEXT_HI) != context_hi)
        return(FAILED);

    if(rMC_CTXT(mID, R_CONTEXT_LO) != context_lo)
        return(FAILED);    
        
    return(SUCCESSFUL);    
} 

/*
***************************************************************************
*                             McVideoInit
*
* Description: initialize the memory context as a Video context.
*
* Arguments  : mID            is the memory context ID.
*              line_count     is the Line count per frame.
*              pixel_count    is the Pixel count per line.
*              pixel_pointer  increments for every pixel and jumps to the
*                             other line start address when the pixel count
*                             reaches the end of the line.
*
* Return     : 
*
* Note(s)    : 
***************************************************************************
*/
int McVideoInit(int mID, int line_count, int pixel_count, int pixel_pointer)
{   
    int context_hi, context_lo;
    
    /* write memory context MSB(bit[63:32]) */
    context_hi = (line_count >> 6) | ( pixel_count<< 6) | (VIDEO << 18);
    rMC_CTXT(mID, W_CONTEXT_HI) = context_hi;
    
    /* write memory context LSB(bit[31:0]). */
    context_lo = (line_count<<26) | (pixel_pointer & 0x3ffffff);    // pixel_pointer is bit 25 ~ 0
    rMC_CTXT(mID, W_CONTEXT_LO) = context_lo;
    
    if(rMC_CTXT(mID, R_CONTEXT_HI) != context_hi)
        return(FAILED);

    if(rMC_CTXT(mID, R_CONTEXT_LO) != context_lo)
        return(FAILED);    
        
    return(SUCCESSFUL);    
} 

/*
***************************************************************************
*                             McVideoReInit
*
* Description: re-initialize the Video context.
*
* Arguments  : mID            is the memory context ID.
*
* Return     : 
*
* Note(s)    : 
***************************************************************************
*/
int McVideoReInit(int mID)
{   
    int context_hi, context_lo;
    unsigned int exID;    // the extended video ID
    unsigned int pixel_pointer;
    
    switch(mID) {
        case VIN_Y_M: 	
        case VIN_U_M:
        case VIN_V_M:
            exID = 0;
            break;
            
        case VOUT_Y_M:            
        case VOUT_U_M:  
        case VOUT_V_M:          
            exID = 1;
            break;        
            
        case VSC_RD_Y0_M:            
        case VSC_RD_U0_M:  
        case VSC_RD_V0_M:          
            exID = 2;
            break;        
            
        case VSC_RD_Y1_M:            
        case VSC_RD_U1_M:  
        case VSC_RD_V1_M:          
            exID = 3;
            break;        
            
        case VSC_RD_OSD_M:
            exID = 4;
            break; 
            
        case VSC_WR_Y_M:            
        case VSC_WR_U_M:  
        case VSC_WR_V_M:          
            exID = 5;
            break;   
            
        default:
            return(FAILED);                                                           
    }
    
    /* write memory context MSB(bit[63:32]) */
    context_hi = (0 >> 6) | ( 0 << 6) | (VIDEO << 18);
    rMC_CTXT(mID, W_CONTEXT_HI) = context_hi;
    
    /* decide the pixel_pointer */
    printf("?%x,%d,%d.", McRdVideoPointer(mID), McRdVideoLineCount(mID), McRdVideoPixelCount(mID));	
    pixel_pointer = McRdVideoPointer(mID) - McRdVideoFramWidth(exID) * McRdVideoFramDepth(exID);
    
    /* write memory context LSB(bit[31:0]). */
    context_lo = (0<<26) | (pixel_pointer & 0x3ffffff);    // pixel_pointer is bit 25 ~ 0
    rMC_CTXT(mID, W_CONTEXT_LO) = context_lo;
    
    if(rMC_CTXT(mID, R_CONTEXT_HI) != context_hi)
        return(FAILED);

    if(rMC_CTXT(mID, R_CONTEXT_LO) != context_lo)
        return(FAILED);    
        
    return(SUCCESSFUL);    
} 

/*
***************************************************************************
*                             McVideoExtendedInit
*
* Description: initialize the extended video context.
*
* Arguments  : extended_videoID    is the memory context ID.
*              frame_width         is the video pixels per line.
*              frame_depth         is the number of lines per frame.
*              y_skip              is the line skip value for the Y frame.
*                                  NO_SKIP         每 no skip/dup. Display every line
*                                  HALF_SCALING    每 1/2 or 2 scaling. Display every 2nd line
*                                  QUARTER_SCALING 每 1/4 or 4 scaling. Display every 4th line
*                                  EIGHTH_SCALING  每 1/8 or 8 scaling. Display every 8th line
*              uv_skip             is the line skip value for the UV frame.
*                                  NO_SKIP         每 no skip/dup. Display every line
*                                  HALF_SCALING    每 1/2 or 2 scaling. Display every 2nd line
*                                  QUARTER_SCALING 每 1/4 or 4 scaling. Display every 4th line
*                                  EIGHTH_SCALING  每 1/8 or 8 scaling. Display every 8th line
*              y_dup               Y_DUP_DOWN_SCALING - Y frame down scaling    
*                                  Y_DUP_UP_SCALING   - Y frame up scaling 
*              uv_dup              UV_DUP_DOWN_SCALING - UV frame down scaling    
*                                  UV_DUP_UP_SCALING   - UV frame up scaling   
*
* Return     : none
*
* Note(s)    : 
***************************************************************************
*/
int McVideoExtendedInit(int extended_videoID, int frame_width, int frame_depth,\
                        int y_skip, int uv_skip, int y_dup, int uv_dup)
{   
    unsigned int context;
        
    context = frame_width | (frame_depth << 12) | (y_skip << 24) | (uv_skip << 26) |\
              (y_dup << 28) | (uv_dup << 29);
    
    rMC_VCTXT(extended_videoID) = context;

    if(rMC_VCTXT(extended_videoID) != context)
        return(FAILED);    
        
    return(SUCCESSFUL);    
} 

/*
***************************************************************************
*                             McRandomSet
*
* Description: Set address & length of the random context.
*
* Arguments  : sID            is the stream ID. 
*              addr           is the random address of the memory context. 
*              len            is the length of the random context. 
*
* Return     : none
*
* Note(s)    : 
***************************************************************************
*/
int McRandomSet(int sID, unsigned int addr, unsigned int len)
{   
    rMC_M_ASET(sID) = addr;
    rMC_M_LSET(sID) = len;
    
    //if(rMC_M_ASET(sID) != addr)
    //    return(FAILED);

    //if(rMC_M_LSET(sID) != len)
    //    return(FAILED);    
        
    return(SUCCESSFUL);    
} 

/*
***************************************************************************
*                             McRandomSet1
*
* Description: Set address & length of the random context.
*
* Arguments  : sID            is the stream ID. 
*              addr           is the random address of the memory context. 
*              len            is the length of the random context. 
*
* Return     : none
*
* Note(s)    : 
***************************************************************************
*/
int McRandomSet1(int mID, unsigned int addr, unsigned int len)
{           
    rMC_CTXT(mID, S_POINT_VALUE) = addr;
    rMC_CTXT(mID, S_RCONTEXT_LEN) = len; 
        
    return(SUCCESSFUL);    
} 

/*
***************************************************************************
*                             McScMap
*
* Description: Map the stream ID to the memory ID. 
*
* Arguments  : mID           is memory context ID.
*              sID           is the stream context ID.          
*
* Return     : FAILED        shows the stream is not opened.
*              SUCCESSFUL    shows the MAP is established.
* Note(s)    : 
***************************************************************************
*/
int McScMap(int mID, int sID)
{
    unsigned int mb_size, map;
    	
    //if(rSC_STATUS(sID) == S_CLOSE)
    //    return(FAILED);	
        
    mb_size = (rSTREAM_ADDR(sID, R_CONTEXT_L) & 0x180) >> 7;
    
    map = mID | (mb_size<<6);
    
    rMC_SID_MAP(sID) = map;
    
    if(rMC_SID_MAP(sID) == map)    
        return(SUCCESSFUL);
    else
        return(FAILED);
}

/*
***************************************************************************
*                             McRdQueue
*
* Description: read the data from the queue.
*
* Arguments  : mID        is the queue context ID.
*              data       is the data pointer. 
*                         What will be read is saved to the address.
*
* Return     : SUCCESSFUL   shows data has been read.
*              FAILED       shows the queue is empty or closed or 
*                           underflow and no data is read.
*
* Note(s)    : 
***************************************************************************
*/
int McRdQueue(int mID, int *data)
{               		
    int status;
	
    status = McRdQueueStatus(mID);

#ifdef MC_NEW1_VERSION  
    if ((status == Q_EMPTY) || (status == Q_UF)) 
        return FAILED;        // at this status, read may cause CPU hang!
#else
    if ((status == Q_EMPTY) || (status == Q_DONE) || \
        (status == Q_CLOSE) || (status == Q_UF)) 
        return FAILED;        // at this status, read may cause CPU hang!      
#endif
    	
    *data = rQUEUE_ADDR(mID, 0);
    	
    return SUCCESSFUL;
}

/*
***************************************************************************
*                             McWrQueue
*
* Description: write the data to the queue.
*
* Arguments  : mID        is the queue context ID.
*              data       is the data to be writen into the queue.
*
* Return     : SUCCESSFUL  shows the data is writen into 
*                          the queue successfully.
*              FAILED      shows the data is NOT writen into the queue.
*
* Note(s)    : 
***************************************************************************
*/
int McWrQueue(int mID, int data)
{               		
    int status;
	
    status = McRdQueueStatus(mID);

#ifdef MC_NEW1_VERSION  
    if ((status == Q_FULL) || (status == Q_OF)) 
        return FAILED;        // at this status, write may cause CPU hang!
#else
    if ((status == Q_FULL) || (status == Q_DONE) || \
        (status == Q_CLOSE) || (status == Q_OF)) 
        return FAILED;        // at this status, write may cause CPU hang!      
#endif
    	
    rQUEUE_ADDR(mID, 0) = data;
    	
    return SUCCESSFUL;
}

/*
***************************************************************************
*                             McWrRandom
*
* Description: write the data to the random.
*
* Arguments  : mID        is the queue context ID.
*              data       is the data to be writen into the queue.
*
* Return     : SUCCESSFUL  shows the data is writen into 
*                          the queue successfully.
*              FAILED      shows the data is NOT writen into the queue.
*
* Note(s)    : 
***************************************************************************
*/
int McWrRandom(int mID, int data)
{               		
    int status;
	
    status = McRdQueueStatus(mID);

#ifdef MC_NEW1_VERSION  
    if ((status == Q_FULL) || (status == Q_OF)) 
        return FAILED;        // at this status, write may cause CPU hang!
#else
    if ((status == Q_FULL) || (status == Q_DONE) || \
        (status == Q_CLOSE) || (status == Q_OF)) 
        return FAILED;        // at this status, write may cause CPU hang!      
#endif
    	
    rQUEUE_ADDR(mID, 0) = data;
    	
    return SUCCESSFUL;
}

/*
***************************************************************************
*                             McRdContextHi
*
* Description: read the context[63:32] of the memory context.
*
* Arguments  : mID        is the memory context ID.
*
* Return     : the context[63:32] of the memory context.
*
* Note(s)    : 
***************************************************************************
*/
int McRdContextHi(int mID)
{
    return(rMC_CTXT(mID, R_CONTEXT_HI));
}

/*
***************************************************************************
*                             McRdContextLo
*
* Description: read the context[31:0] of the memory context.
*
* Arguments  : mID        is the memory context ID.
*
* Return     : the context[31:0] of the memory context.
*
* Note(s)    : 
***************************************************************************
*/
int McRdContextLo(int mID)
{
    return(rMC_CTXT(mID, R_CONTEXT_LO));
}

/*
***************************************************************************
*                             McRdQueueCount
*
* Description: read the queue count of the queue context.
*
* Arguments  : mID        is the queue context.
*
* Return     : the queue count of the queue context. Counted by bytes.
*
* Note(s)    : 
***************************************************************************
*/
int McRdQueueCount(int mID)
{           		
    return(rMC_CTXT(mID, R_QUEUE_COUNT));
}

/*
***************************************************************************
*                             McRdRandomPointer
*
* Description: read the pointer value of the ramdom context.
*
* Arguments  : mID        is the random context.
*
* Return     : the pointer value of the random context.
*
* Note(s)    : 
***************************************************************************
*/
int McRdRandomPointer(int mID)
{           		
    return(rMC_CTXT(mID, S_POINT_VALUE));
}

/*
***************************************************************************
*                             McRdRandomLen
*
* Description: read the random context length of the ramdom context.
*
* Arguments  : mID        is the random context.
*
* Return     : the random context length of the random context.
*
* Note(s)    : 
***************************************************************************
*/
int McRdRandomLen(int mID)
{           		
    return(rMC_CTXT(mID, S_RCONTEXT_LEN));
}

/*
***************************************************************************
*                             McRdQueueStatus
*
* Description: read the status of the queue context.
*
* Arguments  : mID        is the queue context.
*
* Return     : the status of the queue context.
*
* Note(s)    : 
***************************************************************************
*/
int McRdQueueStatus(int mID)
{
    int context_hi;

    context_hi = rMC_CTXT(mID, R_CONTEXT_HI);
            		
    return((context_hi & 0x1c000) >> 14);
}

/*
***************************************************************************
*                             McRdQueueBufsize
*
* Description: read the buf-size item of the queue context.
*
* Arguments  : mID        is the queue context.
*
* Return     : the "buf-size" item of the queue context.
*
* Note(s)    : 
***************************************************************************
*/
int McRdQueueBufsize(int mID)
{
    int context_hi;

    context_hi = rMC_CTXT(mID, R_CONTEXT_HI);
            		
    return((context_hi & 0x3800) >> 11);
}

/*
***************************************************************************
*                             McRdQueueActualBufsize
*
* Description: read the buf-size of the queue context.
*
* Arguments  : mID        is the queue context.
*
* Return     : the actual buf size of the queue. Counted by bytes!
*
* Note(s)    : 
***************************************************************************
*/
int McRdQueueActualBufsize(int mID)
{
    return(256 * (1<<(McRdQueueBufsize(mID))));
}

/*
***************************************************************************
*                             McRdQueueSubBufsize
*
* Description: read the buf-size item of the queue context.
*
* Arguments  : mID        is the queue context.
*
* Return     : the "subbuf-size" item of the queue context.
*
* Note(s)    : 
***************************************************************************
*/
int McRdQueueSubBufsize(int mID)
{
    int context_hi;

    context_hi = rMC_CTXT(mID, R_CONTEXT_HI);
            		
    return((context_hi & 0x700) >> 8);
}

/*
***************************************************************************
*                             McRdQueueRPTR
*
* Description: read the read pointer of the queue context.
*
* Arguments  : mID        is the queue context.
*
* Return     : the read pointer of the queue context.
*
* Note(s)    : 
***************************************************************************
*/
int McRdQueueRPTR(int mID)
{
    unsigned int context_hi, context_lo;

    context_hi = rMC_CTXT(mID, R_CONTEXT_HI);
    context_lo = rMC_CTXT(mID, R_CONTEXT_LO);
                		
    return(((context_hi & 0xff) << 6) | ((context_lo & 0xfc000000) >> 26));
}

/*
***************************************************************************
*                             McRdQueueReadAddr
*
* Description: read the read address of the queue context.
*
* Arguments  : mID        is the queue context.
*
* Return     : the read address of the queue context.
*
* Note(s)    : 
***************************************************************************
*/
int McRdQueueReadAddr(int mID)
{               		
    return(McRdQueueBaseAddr(mID) | McRdQueueRPTR(mID));
}

/*
***************************************************************************
*                             McRdQueueWPTR
*
* Description: read the write pointer of the queue context.
*
* Arguments  : mID        is the queue context.
*
* Return     : the write pointer of the queue context.
*
* Note(s)    : 
***************************************************************************
*/
int McRdQueueWPTR(int mID)
{
    unsigned int context_lo;
    
    context_lo = rMC_CTXT(mID, R_CONTEXT_LO);
                		
    return(context_lo & 0x3fff);
}

/*
***************************************************************************
*                             McRdQueueWriteAddr
*
* Description: read the write address of the queue context.
*
* Arguments  : mID        is the queue context.
*
* Return     : the write address of the queue context.
*
* Note(s)    : 
***************************************************************************
*/
int McRdQueueWriteAddr(int mID)
{
    unsigned int context_lo;
    
    context_lo = rMC_CTXT(mID, R_CONTEXT_LO);
                		
    return(context_lo & 0x3ffffff);
}

/*
***************************************************************************
*                             McRdQueueBaseAddr
*
* Description: read the base address of the queue context.
*
* Arguments  : mID        is the queue context.
*
* Return     : the base address of the queue context.
*
* Note(s)    : 
***************************************************************************
*/
int McRdQueueBaseAddr(int mID)
{               		
    return(McRdQueueWriteAddr(mID) & (~0x3fff));
}

/*
***************************************************************************
*                             McRdRandomBaseAddr
*
* Description: read the base address of the random context.
*
* Arguments  : mID        is the random context.
*
* Return     : the base address of the random context.
*
* Note(s)    : 
***************************************************************************
*/
int McRdRandomBaseAddr(int mID)
{               		
    return(McRdContextLo(mID) & 0x3ffffff);   // base addr is bit[25:0]
}

/*
***************************************************************************
*                             McRdRandomCount
*
* Description: read the count of the random context.
*
* Arguments  : mID        is the random context.
*
* Return     : the count of the random context.
*
* Note(s)    : 
***************************************************************************
*/
int McRdRandomCount(int mID)
{ 
    int count;
    
    count = ((McRdContextLo(mID) >> 26) & 0x3f) | \
            ((McRdContextHi(mID) & 0xff) << 6);
          	              		
    return(count);   
}

/*
***************************************************************************
*                             McRdVideoPointer
*
* Description: read the pixel pointer of the video context.
*
* Arguments  : mID        is the video context ID.
*
* Return     : the pixel pointer of the video context.
*
* Note(s)    : 
***************************************************************************
*/
int McRdVideoPointer(int mID)
{         	              		
    return(McRdContextLo(mID) & 0x3ffffff);   
}

/*
***************************************************************************
*                             McRdVideoPixelCount
*
* Description: read the Pixel count per frame of the video context.
*
* Arguments  : mID        is the video context ID.
*
* Return     : the Pixel count per frame of the video context.
*
* Note(s)    : 
***************************************************************************
*/
int McRdVideoPixelCount(int mID)
{         	              		
    return((McRdContextHi(mID) >> 6) & 0x3ff);   
}

/*
***************************************************************************
*                             McRdVideoLineCount
*
* Description: read the line count per line of the video context.
*
* Arguments  : mID        is the video context ID.
*
* Return     : the line count per line of the video context.
*
* Note(s)    : 
***************************************************************************
*/
int McRdVideoLineCount(int mID)
{         	              		
    int count;
    
    count = ((McRdContextLo(mID) >> 26) & 0x3f) | \
            ((McRdContextHi(mID) & 0x3f) << 6);
          	              		
    return(count); 
}

/*
***************************************************************************
*                             McRdVideoFramWidth
*
* Description: read the video pixel per line from extended video context.
*
* Arguments  : extended_videoID        is the extended video context ID.
*
* Return     : the video pixel per line.
*
* Note(s)    : 
***************************************************************************
*/
int McRdVideoFramWidth(int extended_videoID)
{         	              		
    unsigned int context;
    
    context = rMC_VCTXT(extended_videoID);
    
    return(context & 0xfff);   
}

/*
***************************************************************************
*                             McRdVideoFramDepth
*
* Description: read the number of line per frame from extended video context.
*
* Arguments  : extended_videoID        is the extended video context ID.
*
* Return     : the number of linw per frame.
*
* Note(s)    : 
***************************************************************************
*/
int McRdVideoFramDepth(int extended_videoID)
{         	              		
    unsigned int context;
    
    context = rMC_VCTXT(extended_videoID);
    
    return((context >> 12) & 0xfff);   
}

/*
***************************************************************************
*                             McAutoFetchEn
*
* Description: enable the stream auto-fetch.
*
* Arguments  : sID        is the stream ID.
*
* Return     : SUCCESSFUL    the oepration succeed.
*              FAILED        the oepration failed.
*
* Note(s)    : sID should be from the soft read stream IDs, from 0x35 to 0x3f
***************************************************************************
*/
int McAutoFetchEn(int sID)
{         	              		
    unsigned int tmp;
    
    if((sID < 0x35) || (sID > 0x3f))
        return FAILED;
        
    tmp = rMC_AS_MASK;
    
    tmp |= (1 << (sID - 0x35)); 
    
    rMC_AS_MASK = tmp;
    
    if(rMC_AS_MASK == tmp)
        return SUCCESSFUL;            
    
    return FAILED;
}

/*
***************************************************************************
*                            McIntrEn
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
void  McIntrEn(void)
{
    unsigned int isr_base_addr;
    static int mc_intr_inited = 0;

    if(mc_intr_inited)
        return;

    mc_intr_inited = 1;

    // the fourth-interrupt table address	
    if(RiscRdID()  == RISC0)
        isr_base_addr = RISC0_FOURTH_VECTOR_B_ADDR | 0xa0000000;    // phy-addr is used to avoid flush cache
    else
        isr_base_addr = RISC1_FOURTH_VECTOR_B_ADDR | 0xa0000000;    // phy-addr is used to avoid flush cache      
        
    *(unsigned int *)(isr_base_addr + 0x20) = (unsigned int)McISR; 
    
    RiscIntrEn(MC_INTR);
}

#ifdef AUDIO_MODULE_INCLUDE
/*
num is the queue ID!
*/
void McHandleAudIn(int num, unsigned int mc_intr_status)
{
    if((((mc_intr_status >> 7) & 0x7) == 5) && (mc_intr_status & 0x1)) {   // overflow
        switch (num)
        {
            case 0: 
                McQueueReInit(AUD_IN_CH0_M);
                break;
                
            case 1: 
                McQueueReInit(AUD_IN_CH1_M);
                break;
                
            case 2: 
                McQueueReInit(AUD_IN_CH2_M);
                break;
    
            case 3:
                McQueueReInit(AUD_IN_CH3_M);
                break;
    
            default:    // should not happen!
                debug("McHandleAudIn: num = %d Error!\n", num);
                break;
        }
    
        debug("^");	//Overflow        
        channel_history_pointer_initialized[num] = 0;
        channel_history_pointer_recount[num] = 1;
    } else {    // audio in queue should not underflow!
        debug("\n\nMcHandleAudIn: mc audio input channel %d isr error! rMC_INTR_CTL = 0x%x!\n\n", num, mc_intr_status);
        McQueueReInit(num);
    }

    /* this is done in McISR */    
    //rMC_INTR_CTL = rMC_INTR_CTL | 0x1;   // clear interrupt source
}

/*
num is the channel number!
*/
void McHandleAudOut(int num, unsigned int mc_intr_status)
{
    unsigned int length;    	            
    unsigned int i;

    if((((mc_intr_status >> 7) & 0x7) == 6) && (mc_intr_status & 0x1)) 
    {   // underflow              
        switch (num)
        {
            case 0:
                McQueueReInit(AUD_OUT_CH0_M);
                break;
    
            case 1:
                McQueueReInit(AUD_OUT_CH1_M);
                break;
    
            case 2:
                McQueueReInit(AUD_OUT_CH2_M);
                break;
    
            case 3:
                McQueueReInit(AUD_OUT_CH3_M);
                break;
    
            default:    // should not happen
                debug("\n\nMcHandleAudOut: num = %d Error!\n\n", num);
    	}

        debug("&");	//Underflow
    
        if (channelAC[num].lwAudioOutputType == AUDIO_ENCODING_LINEAR) 
        {
            length = 320 * 2;    // 40 ms's data 
        } else {
            length = 160 * 2;    // 40 ms's data
        }

    	switch (num)
    	{
            case 0:
            	for(i = 0; i < length; i+=4) 
                {    	   	
                    if(McWrQueue(AUD_OUT_CH0_M, 0x0) == FAILED) 
    			    break;           
                }
                break;
    
            case 1:
                for(i = 0; i < length; i+=4) 
                {    	   	
                    if(McWrQueue(AUD_OUT_CH1_M, 0x0) == FAILED) 
                    break;           
                }
                break;
    
            case 2:
                for(i = 0; i < length; i+=4) 
                {    	   	
                   if(McWrQueue(AUD_OUT_CH2_M, 0x0) == FAILED) 
                   break;           
                }
                break;
    
            case 3:
                for(i = 0; i < length; i+=4) 
                {    	   	
                    if(McWrQueue(AUD_OUT_CH3_M, 0x0) == FAILED) 
                    break;           
                }
                break;
    
            default:    // should not happen
                debug("\n\nMcHandleAudOut: num = %d Error!\n\n", num);
        }		
	
        channel_history_pointer_initialized[num] = 0;
        channel_history_pointer_recount[num] = 1;
    } else {    // audio output should not overflow!
        debug("\n\nMcHandleAudOut: mc audio output channel %d isr error! rMC_INTR_CTL = 0x%x!\r\n", mc_intr_status);
        McQueueReInit(num + 4);
    }

    /* this is done in McISR */
    //rMC_INTR_CTL = rMC_INTR_CTL | 0x1;   // clear interrupt source
}
#endif

/*
***************************************************************************
*                            McISR
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
void  McISR(void)
{
    unsigned int mc_intr_status, mc_id;
    static int intr_no = 0;     // record how many times CPU enter into McISR, but do not clear!
    
    mc_intr_status = rMC_INTR_CTL;

    gtStatistics.cnt_mc_intr++;
    
#ifdef AUDIO_MODULE_INCLUDE
    if(!(mc_intr_status & 0x1)) {     // not queue underflow or overflow intr!
        printf("\n\nMcISR: mc isr error! not queue intr! rMC_INTR_CTL = 0x%x!\n\n", mc_intr_status);  
        if(++intr_no == 3) {    // if nobody clear the intr, we do it. To prevent ucos dies in intr!
            rMC_INTR_CTL = mc_intr_status;
            intr_no = 0;
        }
        return;
    }

    gtStatistics.cnt_mc_intr_by_audio++;

    mc_id = (mc_intr_status >> 14) & 0x3f;    

    switch(mc_id) {
	//Audio_in
        case AUD_IN_CH0_M:	
	    McHandleAudIn(0, mc_intr_status);
            break;
            
        case AUD_IN_CH1_M:	
	    McHandleAudIn(1, mc_intr_status);
            break;
	    
        case AUD_IN_CH2_M:	
	    McHandleAudIn(2, mc_intr_status);
            break;
	    
        case AUD_IN_CH3_M:	
	    McHandleAudIn(3, mc_intr_status);
            break;

	//Audio_out
        case AUD_OUT_CH0_M:
	    McHandleAudOut(0, mc_intr_status);
	    break;
			
        case AUD_OUT_CH1_M:
	    McHandleAudOut(1, mc_intr_status);
	    break;
			
        case AUD_OUT_CH2_M:
	    McHandleAudOut(2, mc_intr_status);
	    break;
			
        case AUD_OUT_CH3_M:
	    McHandleAudOut(3, mc_intr_status);
	    break;
                        
        default:
            printf("\n\nMcISR: mc isr error! rMC_INTR_CTL = 0x%x!\n\n", mc_intr_status);                
            break;                    
    }
    rMC_INTR_CTL = (rMC_INTR_CTL & ~0x3) | 0x1;   // clear interrupt source. Be careful that do not clear zero length intr!
#else
    /* if no audio module included, should not have mc intr. Just print and return! Do not clear it! */
    printf("\n\nMcISR: mc isr occur! but should not happen! rMC_INTR_CTL = 0x%x!\n\n", mc_intr_status); 
    if(++intr_no == 3) {    // if nobody clear the intr, we do it. To prevent ucos dies in intr!
        rMC_INTR_CTL = mc_intr_status;
        intr_no = 0;
    }
#endif
/*
#ifdef VIDEO_MODULE_INCLUDE
    if(rMC_INTR_CTL & 0x2) {
        printf("\nRisc%d(vout), rMC_INTR_CTL = 0x%x!\n", RiscRdID(), mc_intr_status); 
        rMC_INTR_CTL = rMC_INTR_CTL | 0x2;
    }        
#endif*/	
}

// The following functions come from TestMc.c

/*
***************************************************************************
*                             PrinfMcQueueContext
*
* Description: 
*
* Arguments  : mID        is the queue context ID.
*
* Return     : 
*
* Note(s)    : 
***************************************************************************
*/
void PrinfMcQueueContext(int mID)
{
    printf("Queue %d: Context Hi = 0x%x, Context Lo = 0x%x, rMC_STATUS_HI = 0x%x, rMC_STATUS_LO = 0x%x, rMC_WSTATUS_HI = 0x%x, rMC_WSTATUS_LO = 0x%x, rMC_INTR_CTL = 0x%x.\r\n", \
            mID, McRdContextHi(mID), McRdContextLo(mID), rMC_STATUS_HI, rMC_STATUS_LO, rMC_WSTATUS_HI, rMC_WSTATUS_LO, rMC_INTR_CTL);	
    printf("Queue %d: Queue Count = 0x%x, BaseAddr = 0x%x, Read Pointer = 0x%x, ReadAddr = 0x%x; Write Pointer = 0x%x, WriteAddr = 0x%x. Status = %d.", \
            mID, McRdQueueCount(mID), McRdQueueBaseAddr(mID), McRdQueueRPTR(mID), McRdQueueReadAddr(mID), McRdQueueWPTR(mID), McRdQueueWriteAddr(mID), McRdQueueStatus(mID));	              
}

/*
***************************************************************************
*                             DebugMcQueueContext
*
* Description: 
*
* Arguments  : mID        is the queue context ID.
*
* Return     : 
*
* Note(s)    : 
***************************************************************************
*/
void DebugMcQueueContext(int mID)
{
    debug("Queue %d: Context Hi = 0x%x, Context Lo = 0x%x, rMC_STATUS_HI = 0x%x, rMC_STATUS_LO = 0x%x, rMC_WSTATUS_HI = 0x%x, rMC_WSTATUS_LO = 0x%x, rMC_INTR_CTL = 0x%x.\r\n", \
            mID, McRdContextHi(mID), McRdContextLo(mID), rMC_STATUS_HI, rMC_STATUS_LO, rMC_WSTATUS_HI, rMC_WSTATUS_LO, rMC_INTR_CTL);	
    debug("Queue %d: Queue Count = 0x%x, BaseAddr = 0x%x, Read Pointer = 0x%x, ReadAddr = 0x%x; Write Pointer = 0x%x, WriteAddr = 0x%x. Status = %d.", \
            mID, McRdQueueCount(mID), McRdQueueBaseAddr(mID), McRdQueueRPTR(mID), McRdQueueReadAddr(mID), McRdQueueWPTR(mID), McRdQueueWriteAddr(mID), McRdQueueStatus(mID));	              
}

/*
***************************************************************************
*                             PrinfMcRandomContext
*
* Description: 
*
* Arguments  : mID        is the random context ID.
*
* Return     : 
*
* Note(s)    : 
***************************************************************************
*/
void PrinfMcRandomContext(int mID, int sID)
{
    printf("Random: Context Hi = 0x%x, Context Lo = 0x%x, BaseAddr = 0x%x, Count = 0x%x, Pointer Value = 0x%x, Random Context Length = 0x%x.\r\n", \
            McRdContextHi(mID), McRdContextLo(mID), McRdRandomBaseAddr(mID), McRdRandomCount(mID), McRdRandomPointer(mID), McRdRandomLen(mID));	           
    printf("rMC_INTR_CTL = 0x%x. ", \
            rMC_INTR_CTL);	
    if(sID != 0xff) {
        printf("rMC_M_ASET(0x%x) = 0x%x, rMC_M_LSET(0x%x) = 0x%x.", \
                sID, rMC_M_ASET(sID), sID, rMC_M_LSET(sID));    	
    }               
}

/*
***************************************************************************
*                             PrinfMcVideoContext
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
void PrinfMcVideoContext(int videoID, int extended_videoID)
{
    printf("Video: Context Hi = 0x%x, Context Lo = 0x%x, Line Count Per Frame = %d, Pixel Count Per Line = %d, Video Pointer = 0x%x.", \
            McRdContextHi(videoID), McRdContextLo(videoID), McRdVideoLineCount(videoID), McRdVideoPixelCount(videoID), McRdVideoPointer(videoID));	           
    if(extended_videoID != 0xff) {
        printf("\r\nExtended Video: Context = 0x%x, video pixel per line = %d, number of line per frame = %d.", \
                rMC_VCTXT(extended_videoID), McRdVideoFramWidth(extended_videoID), McRdVideoFramDepth(extended_videoID));    	
    }               
}

