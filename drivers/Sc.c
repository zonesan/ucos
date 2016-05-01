/*
***************************************************************************
*                  Copyright (c) 2005 vBridge Microsystem, Inc.  
*                        Unpublished & Not for Publication
*                              All Rights Reserved                   
*   
* File        : Sc.c                     
*                                                                         
* Description : 
*             
* Date        : Dec 8, 2005                                                           
***************************************************************************
*/

#define PIP_IMPROVE
#define   SC_GLOBALS

#include "Sc.h"
#include "Mc.h"
#include "os_cpu.h"
#include "2risc_memory_map.h"
#include "2risc_comm_protocol.h"

#include "os_cpu.h"
#include "os_cfg.h"
#include "uCOS_II.h"

#include "two_risc_system.h"

#include "comm.h"
#include "chip_cfg.h"

#ifdef VIDEO_MODULE_INCLUDE
#include "VIN_API.h"
#include "video_intr.h"
#include "mpeg4_encoder.h"
#include "two_risc_video.h"
#include "mpeg4_decoder.h"
#include "two_risc_display.h"
#include "video_cfg.h"
#include "Motion_Detection.h"
#include "VOUT_API.h"
#include "Band_adjust.h"
#endif

#include	"../video/VSC_API.h"
#include "../application/video/VIN_SET.h"

#include "../application/two_risc_statistics.h"

int pip_improve_flag = TRUE;

/*
***************************************************************************
*                              ScInit
*
* Description: initialize the stream context.
*
* Arguments  : sID        is the stream ID
*              buf_use    specifies the stream is for internal use or 
*                         SDRAM transaction
*              db_size    specifies the word size on the DBUS, one byte or 
*                         4 byte.
*              size       specifies the buffer size.
*              sb_size    specifies the unit for stream EMPTY status.
*              mb_size    specifies the burst length to trigger request 
*                         to MC.
*              swap       controls the swap of data on the DBUS side.
*              last       1 - data in the stream FIFO is the last data
*                         0 - data in the stream FIFO is not the last data 
*              base_addr  is the start address of the stream in 
*                         the 4K byte buffer.
*
* Return     : SUCCESSFUL       shows the stream is not opened.
*              FAILED           shows the stream is opened before.
*
* Note(s)    : The read & write pointer are all zero.
***************************************************************************
*/
int ScInit(int sID, int buf_use, int db_size, \
           int size, int sb_size, int mb_size, \
           int swap, int last, int base_addr)
{
    int context;

#ifdef SC_NEW1_VERSION

#ifdef SC_NEW2_VERSION
    context = buf_use | db_size | size | sb_size | mb_size | \
              swap | ((base_addr & 0xe00) << SC_RPTR_OFFSET) | \
              ((base_addr & 0x1ff & (~((1 << ((size / 4) + 3)) - 1)))\
              << SC_WPTR_OFFSET);    // (1 << ((buf_size / 4) + 3)) 
                                     // is the actual buffer size
#else    	
    context = buf_use | db_size | size | sb_size | mb_size | \
              swap | (base_addr<<SC_RPTR_OFFSET);
#endif
              
#else
    context = buf_use | db_size | size | sb_size | mb_size | \
              swap | last | (base_addr<<SC_RPTR_OFFSET);    
#endif  

    //if(rSC_STATUS(sID) != S_CLOSE)
    //    return(FAILED);
    
    rSTREAM_ADDR(sID, W_CONTEXT_L) = context;
    
    /* maybe need wait */
        
    if(context != rSTREAM_ADDR(sID, R_CONTEXT_L))
        return(FAILED);
    
    return(SUCCESSFUL);
}           

/*
***************************************************************************
*                                ScOpen
*
* Description: Clear the stream to open it
*
* Arguments  : sID        is the stream ID
*
* Return     : SUCCESSFUL  shows the stream is opened successfully.
*              FAILED      shows the stream was opened before.
*
* Note(s)    : 
***************************************************************************
*/
int ScOpen(int sID)
{
    int status;
	
    status = rSC_STATUS(sID);	
    // if((status != S_CLOSE) && (status != S_DONE))  
    //    return FAILED; 

    rSTREAM_ADDR(sID, C_WnRP) = 0x0;
    
    /* maybe need wait */   
    status = rSC_STATUS(sID);
    if((status == S_CLOSE) && (status == S_DONE))  
        return FAILED; 
        	
    return SUCCESSFUL;
}

/*
***************************************************************************
*                              ScRd
*
* Description: read data from the stream.
*
* Arguments  : sID        is the stream ID.
*              data       is the data pointer. 
*                         What will be read is saved to the address.
*
* Return     : SUCCESSFUL   shows the stream is not empty and 
*                           data has been read.
*              FAILED       shows the stream is empty or closed or 
*                           underflow and no data is read.
*
* Note(s)    : 
***************************************************************************
*/
int ScRd(int sID, int *data)
{
    int status;
	
    status = rSC_STATUS(sID);

    if ((status == S_EMPTY) || (status == S_DONE) || \
        (status == S_CLOSE) || (status == S_UF)) 
        return FAILED;        // at this status, read will cause CPU hang!      
    	
    *data = rSTREAM_ADDR(sID, R_DATA);
    	
    return SUCCESSFUL;

}

/*
***************************************************************************
*                             ScWr
*
* Description: write data to the stream.
*
* Arguments  : sID        is the stream ID.
*              data       is the data to be writen into the stream.
*
* Return     : SUCCESSFUL  shows the data is writen into 
*                          the stream successfully.
*              FAILED      shows the data is NOT writen into the stream.
*
* Note(s)    : If the status is CLOSE, FULL, or OVERFLOW, 
*              write will cause CPU hang.
***************************************************************************
*/
int ScWr(int sID, int data)
{
    int *addr;
    int status;
	
    status = rSC_STATUS(sID);
    if ((status == S_FULL) || (status == S_CLOSE) || (status == S_OF) || (status == S_DONE)) 
        return FAILED;        // at this status, write may cause CPU hang!    	
        
    rSTREAM_ADDR(sID, W_DATA) = data;
	
    return SUCCESSFUL;

}

/*
***************************************************************************
*                                ScClose
*
* Description: Close the stream
*
* Arguments  : sID        is the stream ID
*
* Return     : SUCCESSFUL  shows the stream is closed successfully.
*              FAILED      shows the stream is not opened before.
*
* Note(s)    : 
***************************************************************************
*/
int ScClose(int sID)
{
    int status, tmp;

    // set bit 23 to 1 to close the stream
    tmp = V_BASE_ADDR + STREAM_B_ADDR + (W_DATA<<16) + (sID<<8);
    tmp |= 0x800000;
    *(unsigned int *)tmp = tmp;        
    
    status = rSC_STATUS(sID) & 0x7;
    	
#ifdef   SC_NEW1_VERSION    
    if((status == S_CLOSE) || (status == S_DONE) || (status == S_WR_DONE)) 
        return SUCCESSFUL;
#else
    if((status == S_CLOSE) || (status == S_DONE) || (status == S_XFR_DONE)) 
        return SUCCESSFUL;
#endif                             	
           
    return FAILED;       
}

/*
***************************************************************************
*                             ScRdStatus
*
* Description: read the status of the stream.
*
* Arguments  : sID        is the stream ID.
*
* Return     : the status of the stream.
*
* Note(s)    : Eric has read 0x13 as the status.
***************************************************************************
*/
int ScRdStatus(int sID)
{	
    return (rSC_STATUS(sID) & 0x7);

}

/*
***************************************************************************
*                             ScRdContext
*
* Description: read the context of the stream.
*
* Arguments  : sID        is the stream ID.
*
* Return     : the context of the stream.
*
* Note(s)    : 
***************************************************************************
*/
int ScRdContext(int sID)
{
    return(rSTREAM_ADDR(sID, R_CONTEXT_L));
}

/*
***************************************************************************
*                           ScRdBufSize
*
* Description: 
*
* Arguments  : sID        is the stream ID.
*
* Return     : the size of the stream.
*
* Note(s)    : 
***************************************************************************
*/
int ScRdBufSize(int sID)
{
    unsigned int context, size;
    
    context = rSTREAM_ADDR(sID, R_CONTEXT_L);
    size = (context & 0x1f) >> 2;    // Acquire the buffer size item
        
    return(1 << (size + 3));     
}

/*
***************************************************************************
*                           ScRdBaseAddr
*
* Description: read the base address of the stream.
*
* Arguments  : sID        is the stream ID.
*
* Return     : the context of the stream.
*
* Note(s)    : 
***************************************************************************
*/
int ScRdBaseAddr(int sID)
{
    unsigned int context, size;
    
    context = rSTREAM_ADDR(sID, R_CONTEXT_L);

#ifdef SC_NEW2_VERSION          
    return((context >> SC_RPTR_OFFSET) & 0xe00 | \
           (context >> SC_WPTR_OFFSET) & (~(ScRdBufSize(sID) - 1)));   
#else
    return((context >> SC_RPTR_OFFSET) & (SC_ONCHIP_RAM_SIZE - 1) & \
           (~(ScRdBufSize(sID) - 1))); 
#endif         
}

/*
***************************************************************************
*                            ScRdRPTR
*
* Description: read the read pointer of the stream.
*
* Arguments  : sID        is the stream ID.
*
* Return     : the context of the stream.
*
* Note(s)    : 
***************************************************************************
*/
int ScRdRPTR(int sID)
{
    unsigned int context, size;
    
    context = rSTREAM_ADDR(sID, R_CONTEXT_L);
                                
    return((context >> SC_RPTR_OFFSET) & (ScRdBufSize(sID) - 1 ));
}

/*
***************************************************************************
*                           ScRdReadAddr
*
* Description: read the read address of the stream.
*
* Arguments  : sID        is the stream ID.
*
* Return     : the context of the stream.
*
* Note(s)    : 
***************************************************************************
*/
int ScRdReadAddr(int sID)
{
    unsigned int context;
    
    context = rSTREAM_ADDR(sID, R_CONTEXT_L);        

    return((context >> SC_RPTR_OFFSET) & (ScRdBufSize(sID) - 1 ) | \
           (ScRdBaseAddr(sID)));
}

/*
***************************************************************************
*                              ScRdWPTR
*
* Description: read the write pointer of the stream.
*
* Arguments  : sID        is the stream ID.
*
* Return     : the context of the stream.
*
* Note(s)    : 
***************************************************************************
*/
int ScRdWPTR(int sID)
{
    unsigned int context;
    
    context = rSTREAM_ADDR(sID, R_CONTEXT_L);

    return((context >> SC_WPTR_OFFSET) & (ScRdBufSize(sID) - 1));   
}

/*
***************************************************************************
*                             ScRdWriteAddr
*
* Description: read the write address of the stream.
*
* Arguments  : sID        is the stream ID.
*
* Return     : the context of the stream.
*
* Note(s)    : 
***************************************************************************
*/
int ScRdWriteAddr(int sID)
{
    unsigned int context;
    
    context = rSTREAM_ADDR(sID, R_CONTEXT_L);
    
    return((context >> SC_WPTR_OFFSET) | (ScRdBaseAddr(sID)));     
}

/*
***************************************************************************
*                            ScRdCount
*
* Description: read the actual count in the stream.
*
* Arguments  : sID        is the stream ID.
*
* Return     : 
*
* Note(s)    : counted by bytes, NOT words! 
***************************************************************************
*/
int ScRdCount(int sID)
{
    unsigned int status, rd_ptr, wr_ptr;
    
    status = ScRdStatus(sID);
    
    switch(status) {
        case S_CLOSE:	
        case S_DONE:
        case S_EMPTY:	
            return 0;
        case S_FULL:
            return ScRdBufSize(sID);
        case S_OF:
        case S_UF:
            return -1;
        default:    // NEMPTY & 4
            rd_ptr = ScRdRPTR(sID);
            wr_ptr = ScRdWPTR(sID);
            if(rd_ptr < wr_ptr)
                return(wr_ptr - rd_ptr);
            return(ScRdBufSize(sID) + wr_ptr - rd_ptr);
    }
}

/*
***************************************************************************
*                            ScRdBlankCount
*
* Description: read the blank count in the stream.
*
* Arguments  : sID        is the stream ID.
*
* Return     : 
*
* Note(s)    : counted by bytes, NOT words! 
***************************************************************************
*/
int ScRdBlankCount(int sID)
{
    unsigned int status, rd_ptr, wr_ptr;
    
    status = ScRdStatus(sID);
    rd_ptr = ScRdRPTR(sID);
    wr_ptr = ScRdWPTR(sID);
    
    switch(status) {
        case S_CLOSE:	
        case S_DONE:
        case S_FULL:	
            return 0;
        case S_EMPTY:
            return ScRdBufSize(sID);
        case S_OF:
        case S_UF:
            return -1;
        default:    // NEMPTY & 4
            if(rd_ptr <= wr_ptr)
                return(ScRdBufSize(sID) + rd_ptr - wr_ptr);
            return(rd_ptr - wr_ptr);
    }
}

/*
***************************************************************************
*                            ScIntrEn
*
* Description: locate sc ISR, enable risc sc intr. 
*
* Arguments: 
*
* Return     : 
*
* Note(s)    : 
***************************************************************************
*/
void  ScIntrEn(void)
{
    static unsigned int sc_intr_initialized = 0;
    unsigned int isr_base_addr;

    if(sc_intr_initialized)
        return;

    sc_intr_initialized = 1;	
#ifdef VIDEO_MODULE_INCLUDE
    flag_vin_y = 0;	
    flag_vin_u = 0;
    flag_vin_v = 0;
    video_in_frame_count = 0;	

    flag_vsc_w_y = 0;
    flag_vsc_w_u = 0;
    flag_vsc_w_v = 0;

    // the fourth-interrupt table address	
    if(RiscRdID()  == RISC0)
        isr_base_addr = RISC0_FOURTH_VECTOR_B_ADDR | 0xa0000000;    // phy-addr is used to avoid flush cache
    else
        isr_base_addr = RISC1_FOURTH_VECTOR_B_ADDR | 0xa0000000;    // phy-addr is used to avoid flush cache      
        
    *(unsigned int *)(isr_base_addr + 0x40) = (unsigned int)ScISR; 
    //*(unsigned int *)(isr_base_addr + 0x40) = (unsigned int)ScISRVga; 
	
    RiscIntrEn(SC_INTR);
#endif
}

/*
***************************************************************************
*                            ScISR
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

unsigned int giwCount;//add by hjm,compute ScISR---->MpegISR 's time
extern volatile unsigned int VscFrameNum;
unsigned int gVinOddEvenFrameSelect;  //BY VINCENT 2007.09.21
unsigned int gVscOddEvenFrameSelect;  //BY VINCENT 2007.09.21
void  ScISR(void)
{
#ifdef VIDEO_MODULE_INCLUDE
    tYUVFrame *msg0, *msg1;
    tYUVFrame *pTmp;
#ifdef OSD_ENABLE
    unsigned int addr;
#endif
#ifdef MOTION_DETECTION
    static unsigned int k = 0, frameindex = 0;
    unsigned int tmp;
#endif
    static tYUVFrame static_msg_rvo, static_msg_pip;

    unsigned int t1, vin_skip = 0;
    unsigned char err;
    static unsigned int count;	
    int gene =   CPU_MAIN_CLOCK/2000;

    unsigned int *pInt;
    Risc_Packet_t VideoPacket;    

    unsigned int state;

    /* clear Vin streams interrupts */
    if(rSC_INTR_S2M & 0x2000) {
        flag_vin_v = 1;
        rSC_INTR_S2M = 0x2000;    // Clear v done bit  
    }
    
    if(rSC_INTR_S2M & 0x1000) {
        flag_vin_u = 1;
        rSC_INTR_S2M = 0x1000;    // Clear u done bit  
    }    
    
    if(rSC_INTR_S2M & 0x800) {
        flag_vin_y = 1;
        rSC_INTR_S2M = 0x800;    // Clear y done bit  
    }

    /* clear Vsc write streams interrupts */
    if(rSC_INTR_WS & 0x4000) {
        flag_vsc_w_y = 1;
        rSC_INTR_WS = 0x4000;    // Clear Vsc write y stream done bit  
    }    

    if(rSC_INTR_WS & 0x8000) {
        flag_vsc_w_u = 1;
        rSC_INTR_WS = 0x8000;    // Clear Vsc write u stream done bit  
    }    
    
    if(rSC_INTR_WS & 0x10000) {
        flag_vsc_w_v = 1;
        rSC_INTR_WS = 0x10000;    // Clear Vsc write v stream done bit  
    }
    
#ifdef DISPLAY_INCLUDE
    
    /* VSC write streams are all finished */
    if((flag_vsc_w_y == 1) && (flag_vsc_w_u == 1) && (flag_vsc_w_v == 1)) {

        /* clear Vsc write y,u,v stream interrupt flag */    	
        flag_vsc_w_y = 0;
        flag_vsc_w_u = 0;
        flag_vsc_w_v = 0;      
        
#ifdef TASK_STATISTIC_EN
        gtStatistics.vout_frame_ps++;
#endif

        if((display_no++ & 1) || (gtDisplayFormat.vout_interlaced == FALSE)) {    // the first field is to be output

            /* The vout should be disabled only when the same picture is displayed twice! */
            if(gVoutEnable == 0) {
                *(int *)(0xa80b0000) = (*(int *)(0xa80b0000)) & 0x0fffffff;    // disable vsc  
                VOUT_CTL = VOUT_CTL & 0x7fffffff;    // disable vout
                return;
            }

            if(gVoutReEnable) {
                gVoutReEnable = 0;
                gVoutEnable = 0;
                *(int *)(0xa80b0000) = (*(int *)(0xa80b0000)) & 0x0fffffff;    // disable vsc  
                VOUT_CTL = VOUT_CTL & 0x7fffffff;    // disable vout

                if(OSQPost(gptQMsgTwoRiscSystemMonitor, (void *)TWO_RISC_SYSTEM_OPEN_VOUT) != OS_NO_ERR)			
                    debug("\r\nRISC0: OSMboxPost error! err = %d!\r\n", err);
                
                return;
            }
            
            /* rvo */			            
            if(gRvoEnable) {
                if(msg0 = OSQAccept(gptMpeg4DecodeOutputReady)) {    // new frame is ready to display
                    /* update the static_msg */
                    static_msg_rvo.y_addr = msg0->y_addr;
                    static_msg_rvo.u_addr = msg0->u_addr;
                    static_msg_rvo.v_addr = msg0->v_addr;
                    
                    gtRvoCurrent.y_addr = msg0->y_addr;
                    gtRvoCurrent.u_addr = msg0->u_addr;
                    gtRvoCurrent.v_addr = msg0->v_addr;

#if 0
                    if(gChecksumEnabled == TRUE) {
                        if(msg0->framenum != VscFrameNum + 1) {
                            printc("ucos frame num error!%d,%d\n", VscFrameNum, msg0->framenum);
                        }
                        VscFrameNum = msg0->framenum;
                    }
#endif
                } else {
                    gtRvoCurrent.y_addr = static_msg_rvo.y_addr;
                    gtRvoCurrent.u_addr = static_msg_rvo.u_addr;
                    gtRvoCurrent.v_addr = static_msg_rvo.v_addr;
                
                    //printf("P");
                }
                
                if(gtRvoCurrent.interlaced == TRUE) {
                    if(gVscOddEvenFrameSelect == EVEN_FRAME_FIRST) {            
                        gtRvoCurrent.y_addr += gtRvoCurrent.width * (gtRvoCurrent.height/2) ;	
                        gtRvoCurrent.u_addr += gtRvoCurrent.width * (gtRvoCurrent.height/2) / 4;
                        gtRvoCurrent.v_addr += gtRvoCurrent.width * (gtRvoCurrent.height/2) / 4;
                    }
                } else {
                    if(gVscOddEvenFrameSelect == EVEN_FRAME_FIRST)                
                        gtRvoCurrent.y_addr += gtRvoCurrent.width;	
                }                                    
            } else {
                DIS_VDE_RVO();
            }
				
#ifdef PIP_ENABLE
            /* pip */
            if(gPipEnable) {				
                if(msg1 = OSQAccept(gptMqueueVideoInFrame)) {    // new frame is ready to display   
                    /* update the static_msg */
                    static_msg_pip.y_addr = msg1->y_addr;
                    static_msg_pip.u_addr = msg1->u_addr;
                    static_msg_pip.v_addr = msg1->v_addr;
                
                    gtPipCurrent.y_addr = msg1->y_addr;
                    gtPipCurrent.u_addr = msg1->u_addr;
                    gtPipCurrent.v_addr = msg1->v_addr;
                } else {
                    gtPipCurrent.y_addr = static_msg_pip.y_addr;
                    gtPipCurrent.u_addr = static_msg_pip.u_addr;
                    gtPipCurrent.v_addr = static_msg_pip.v_addr;
                }

                if(gtPipCurrent.interlaced == TRUE) {
                    if(gVscOddEvenFrameSelect == EVEN_FRAME_FIRST) {                     
                        gtPipCurrent.y_addr += gtPipCurrent.width * (gtPipCurrent.height/2) ;	
                        gtPipCurrent.u_addr += gtPipCurrent.width * (gtPipCurrent.height/2) / 4;
                        gtPipCurrent.v_addr += gtPipCurrent.width * (gtPipCurrent.height/2) / 4;
                    }
                } else {
                    if(gVscOddEvenFrameSelect == EVEN_FRAME_FIRST) {
                        if(pip_improve_flag == FALSE) {
                            gtPipCurrent.y_addr += gtPipCurrent.width;
                        } else {
                            switch(gtPipCurrent.line_ratio) {
                                case vertical_div_2:
                                    gtPipCurrent.y_addr += 2 * gtPipCurrent.width;
                                    break;
                                
                                case vertical_div_4:
                                    gtPipCurrent.y_addr += 4 * gtPipCurrent.width;
                                    break;

                                case vertical_div_8:
                                    gtPipCurrent.y_addr += 8 * gtPipCurrent.width;
                                    break;                                
                                
                                case vertical_no_scale:
                                default:
                                    gtPipCurrent.y_addr += gtPipCurrent.width;
                                    break;
                            }
                        }
                    }
                }  
                
            }	 else {
                DIS_VDE_PIP();
            }			
#endif

#ifdef OSD_ENABLE
            if(gOsdEnable) {
                if(addr = OSQAccept(gptMqueueOSDFrame)) {    // new OSD frame is ready to display 
                    gtOsdCurrent.display_addr = addr;
                }
                if(gVscOddEvenFrameSelect == EVEN_FRAME_FIRST)
                    McVideoInit(VSC_RD_OSD_M, 0, 0, gtOsdCurrent.display_addr + gtOsdCurrent.width/2);
                else 
                    McVideoInit(VSC_RD_OSD_M, 0, 0, gtOsdCurrent.display_addr);                    
                ScOpen(VSC_RD_OSD_S);
            } else {
                DIS_VDE_OSD();
            }
#endif
        } else {    // the second field is to be output
            if(gRvoEnable) {
                if(gtRvoCurrent.interlaced == TRUE) {
                    if(gVscOddEvenFrameSelect == EVEN_FRAME_FIRST) {                    
                        gtRvoCurrent.y_addr -= gtRvoCurrent.width * (gtRvoCurrent.height/2) ;	
                        gtRvoCurrent.u_addr -= gtRvoCurrent.width * (gtRvoCurrent.height/2) / 4;
                        gtRvoCurrent.v_addr -= gtRvoCurrent.width * (gtRvoCurrent.height/2) / 4;
                    } else {
                        gtRvoCurrent.y_addr += gtRvoCurrent.width * (gtRvoCurrent.height/2) ;	
                        gtRvoCurrent.u_addr += gtRvoCurrent.width * (gtRvoCurrent.height/2) / 4;
                        gtRvoCurrent.v_addr += gtRvoCurrent.width * (gtRvoCurrent.height/2) / 4;
                    }
                } else {
                    if(gVscOddEvenFrameSelect == EVEN_FRAME_FIRST)
                        gtRvoCurrent.y_addr -= gtRvoCurrent.width;	
                    else
                        gtRvoCurrent.y_addr += gtRvoCurrent.width;	                        
                }                
            }
#ifdef PIP_ENABLE
            if(gPipEnable) {
                if(gtPipCurrent.interlaced == TRUE) {
                    if(gVscOddEvenFrameSelect == EVEN_FRAME_FIRST) {                    
                        gtPipCurrent.y_addr -= gtPipCurrent.width * (gtPipCurrent.height/2) ;	
                        gtPipCurrent.u_addr -= gtPipCurrent.width * (gtPipCurrent.height/2) / 4;
                        gtPipCurrent.v_addr -= gtPipCurrent.width * (gtPipCurrent.height/2) / 4;
                    } else {
                        gtPipCurrent.y_addr += gtPipCurrent.width * (gtPipCurrent.height/2) ;	
                        gtPipCurrent.u_addr += gtPipCurrent.width * (gtPipCurrent.height/2) / 4;
                        gtPipCurrent.v_addr += gtPipCurrent.width * (gtPipCurrent.height/2) / 4;
                    }
                } else {
                    if(gVscOddEvenFrameSelect == EVEN_FRAME_FIRST) {
                        if(pip_improve_flag == FALSE) {
                            gtPipCurrent.y_addr -= gtPipCurrent.width;	
                        } else {
                            switch(gtPipCurrent.line_ratio) {
                                case vertical_div_2:
                                    gtPipCurrent.y_addr -= 2 * gtPipCurrent.width;
                                    break;
                                
                                case vertical_div_4:
                                    gtPipCurrent.y_addr -= 4 * gtPipCurrent.width;
                                    break;

                                case vertical_div_8:
                                    gtPipCurrent.y_addr -= 8 * gtPipCurrent.width;
                                    break;                                
                                
                                case vertical_no_scale:
                                default:
                                    gtPipCurrent.y_addr -= gtPipCurrent.width;
                                    break;
                            }
                        }
                    } else {
                        if(pip_improve_flag == FALSE) {
                            gtPipCurrent.y_addr += gtPipCurrent.width;	                        
                        } else {
                            switch(gtPipCurrent.line_ratio) {
                                case vertical_div_2:
                                    gtPipCurrent.y_addr += 2 * gtPipCurrent.width;
                                    break;
                                
                                case vertical_div_4:
                                    gtPipCurrent.y_addr += 4 * gtPipCurrent.width;
                                    break;

                                case vertical_div_8:
                                    gtPipCurrent.y_addr += 8 * gtPipCurrent.width;
                                    break;                                
                                
                                case vertical_no_scale:
                                    default:
                                    gtPipCurrent.y_addr += gtPipCurrent.width;
                                    break;
                            }
                        }
                    }
                }                
            }
#endif

#ifdef OSD_ENABLE
            if(gOsdEnable) {

                if(gVscOddEvenFrameSelect == EVEN_FRAME_FIRST)
                    McVideoInit(VSC_RD_OSD_M, 0, 0, gtOsdCurrent.display_addr);
                else
                    McVideoInit(VSC_RD_OSD_M, 0, 0, gtOsdCurrent.display_addr + gtOsdCurrent.width/2);                    
                    
                ScOpen(VSC_RD_OSD_S);
            }
#endif

        }       

        if(gRvoEnable) {
            if(gtRvoCurrent.interlaced == TRUE) {      

                McVideoInit(VSC_RD_Y0_M, 0, 0, gtRvoCurrent.y_addr + 0x0);
                McVideoInit(VSC_RD_U0_M, 0, 0, gtRvoCurrent.u_addr + 0x10);
                McVideoInit(VSC_RD_V0_M, 0, 0, gtRvoCurrent.v_addr + 0x10);	                   
                
            } else {
                McVideoInit(VSC_RD_Y0_M, 0, 0, gtRvoCurrent.y_addr);
                McVideoInit(VSC_RD_U0_M, 0, 0, gtRvoCurrent.u_addr + 0x10);
                McVideoInit(VSC_RD_V0_M, 0, 0, gtRvoCurrent.v_addr + 0x10);	   
            }
        }

#ifdef PIP_ENABLE
       if(gPipEnable) {
            McVideoInit(VSC_RD_Y1_M, 0, 0, gtPipCurrent.y_addr);
            McVideoInit(VSC_RD_U1_M, 0, 0, gtPipCurrent.u_addr + 0x10);
            McVideoInit(VSC_RD_V1_M, 0, 0, gtPipCurrent.v_addr + 0x10);           
       }
#endif		
			
        /* re-open the Vsc read streams(RVO) */
        if(gRvoEnable) {
	    ScOpen(VSC_RD_V0_S);
           ScOpen(VSC_RD_U0_S);
	    ScOpen(VSC_RD_Y0_S);
        }

#ifdef PIP_ENABLE
	 /* re-open the Vsc read streams(pip) */
        if(gPipEnable) {
            ScOpen(VSC_RD_V1_S);
            ScOpen(VSC_RD_U1_S);
            ScOpen(VSC_RD_Y1_S);            
        }
#endif

        if(!(display_no & 1)) {     

            if(gRvoEnable) {
                EN_VDE_RVO();				
            }
            
#ifdef PIP_ENABLE            
            if(gPipEnable) {
                EN_VDE_PIP();				
            }
#endif            

        }

        /* re-open the Vsc write atreams */
        ScOpen(VSC_WR_V_S); 
        ScOpen(VSC_WR_U_S);
        ScOpen(VSC_WR_Y_S);				

#if 0
            /* compute the time of a frame */
            t1= count;
            count = GetCount();
            if(count > t1)
                t1 = count - t1;
            else
                t1 = CPU_MAIN_CLOCK / (2 * OS_TICKS_PER_SEC) + count - t1;	
            printf("<%d>", t1 / (CPU_MAIN_CLOCK/(2*1000)));    // counted by mili-seconds
#endif			

    }
#endif
    
    /* Vin y, u ,v are all finished */
    if((flag_vin_y == 1) && (flag_vin_u == 1) && (flag_vin_v == 1)) {

#ifdef TASK_STATISTIC_EN
        gtStatistics.video_in_fame_rate++;
#endif

    	 video_in_frame_count++;

        state = (*(volatile unsigned int *)0xa805000c & 0x07000000) >> 24;

        if(state >= 3) {    // the second field is to be input
            if(!(video_in_frame_count & 0x1) && gtVinFormat.interlaced) {  // but the software flag shows "the first field is to be input".
                video_in_frame_count++;
                gtStatistics.cnt_video_in_even_odd_reverse++;
                printf("\nVin even/odd field reverse! state = %d!^_^\n", state);
            }
        } else {    // the first field is to be input
            if((video_in_frame_count & 0x1) && gtVinFormat.interlaced) {  // but the software flag shows "the second field is to be input".
                video_in_frame_count++;
                gtStatistics.cnt_video_in_even_odd_reverse++;
                printf("\nVin even/odd field reverse! state = %d!^_^\n", state);
            }
        }
        
        //if(video_in_frame_count <= 112) {
        if(video_in_frame_count <= 12) {    // should be EVEN number, for interlaced Vin
            //video_in_frame_count++; 
            McVideoInit(VIN_Y_M, 0, 0, gtVideoInFrame[VinBufferNumber].y_addr);    // Re-initialize VIN memory to buffer 0
            McVideoInit(VIN_U_M, 0, 0, gtVideoInFrame[VinBufferNumber].u_addr);    // Re-initialize VIN memory to buffer 0       	
            McVideoInit(VIN_V_M, 0, 0, gtVideoInFrame[VinBufferNumber].v_addr);    // Re-initialize VIN memory to buffer 0 

        } else {    //
             if(!(video_in_frame_count & 0x1) || !gtVinFormat.interlaced) { // the first field is to be input(just for interlaced)            

                //printf("f%d,", (*(unsigned int *)0xa805000c & 0x07000000) >> 24);

                gtStatistics.cnt_video_in++;
                gtcbrinfo.framenum++;

/******************* control the encode frame rate skip or not*******************************************/
                if(++skip_no <= gSkipFrame) {     //software framerate
                    vin_skip = 1;
                }else {
                    if(gtMpegEncodeThrowFrame.throw_frame_enable) {    //CBR framerate
                        if(gtMpegEncodeThrowFrame.invalid_frame) { 
                            gtMpegEncodeThrowFrame.invalid_frame--;					
                            vin_skip = 1;					 
                        }
                    }
#ifdef MOTION_DETECTION
                    if(gtcbrinfo.cbr_enabled == FALSE) {  //VBR
                        if(gVinSoftwareFrameRate == 0) {
                            gVinSoftwareFrameRate = frameindex;
                            frameindex = 0;
                            k = 0;
                        }
                        frameindex++;
                        if((gMDPlayTime > 0) || (gIsMDEnabled == FALSE)) {   //motion framerate
                            if(gtMDFrameRateCtrl.framerate_motion < gVinSoftwareFrameRate)
                            {
                                tmp = frameindex * gtMDFrameRateCtrl.framerate_motion / gVinSoftwareFrameRate;  //gVinSoftwareFrameRate can't be 0
                                if(k == tmp)
                                    vin_skip = 1;
                                k = tmp;
                            }
                        }else {                           //no motion framerate
                            if(gtMDFrameRateCtrl.framerate_nomotion < gVinSoftwareFrameRate)
                            {
                                tmp = frameindex * gtMDFrameRateCtrl.framerate_nomotion / gVinSoftwareFrameRate;
                                if(k == tmp)
                                    vin_skip = 1;
                                k = tmp;
                            }
                        }
                    }
#endif
                    
                    skip_no = 0;
                }
/************************************************************************************/

                if(vin_skip == 0) {      //this frame not skipped.
                    
                    pTmp = &gtVideoInFrame[VinBufferNumber];
                    VinBufferNumber = (VinBufferNumber + 1) % MAX_VIN_BUF_NUM;
#ifdef AV_SYNC
                    pTmp->timestamp = GetSysTimeStamp();
#endif

                    if((gImgEncodeMode == ENCODEDOWN) &&  \
                      (gImage_Mode == EVENODD_IMAGE) && \
                      (gtVinFormat.interlaced == TRUE)) {
                        pTmp->y_addr = pTmp->y_addr + gImageWidth * gImageHeight /2;
                        pTmp->u_addr = pTmp->u_addr + gImageWidth * gImageHeight /8;
                        pTmp->v_addr = pTmp->v_addr + gImageWidth * gImageHeight /8;                        
                    }

#ifdef DUMP_YUV
                    unsigned int i;
                    _PRINTF("\r\n\r\nstart dump YUV: gImageWidth = %d, gImageHeight = %d!\r\n\r\n", \
                                                                gImageWidth, gImageHeight);

                    /* dump Y */
                    _PRINTF("\r\n\r\nstart dump Y:\r\n\r\n");
                    for(i = 0; i < gImageWidth * gImageHeight; i += 4) {
                        _PRINTF("0x%x,",  *(unsigned int *)(pTmp->y_addr + i | 0xa0000000));
                    }
                    _PRINTF("\r\n\r\nthe Y ends!\r\n\r\n");          

                    /* dump U */
                    _PRINTF("\r\n\r\nstart dump U:\r\n\r\n");
                    for(i = 0; i < gImageWidth * gImageHeight / 4; i += 4) {
                        _PRINTF("0x%x,",  *(unsigned int *)(pTmp->u_addr + i | 0xa0000000));
                    }
                    _PRINTF("\r\n\r\nthe U ends!\r\n\r\n");  

                    /* dump V */
                    _PRINTF("\r\n\r\nstart dump V:\r\n\r\n");
                    for(i = 0; i < gImageWidth * gImageHeight / 4; i += 4) {
                        _PRINTF("0x%x,",  *(unsigned int *)(pTmp->v_addr + i | 0xa0000000));
                    }
                    _PRINTF("\r\n\r\nthe V ends!\r\n\r\n");  
                    while(1);                        
#endif

                    switch(VMode) {
                    
                        case MPEG4MODE:   
                        case H263_MODE:
                            if(OSSemAccept(gptSemMpegEncodeReady)) {    // mpeg is idle?

#ifdef MOTION_DETECTION
#ifndef DVR
                                if((gIsMDEnabled == TRUE) && (gMdType == mdtemporalDif))
                                {
                                    gpVinFrameAddr = pTmp->y_addr;
                                    if(OSMboxPost(gpMboxPpeNewVinFrame, &gpVinFrameAddr) != OS_NO_ERR)
                                        printf("\nScISR: OSMboxPost gpMboxPpeNewVinFrame error\n");
                                    gtStatistics.md_frame_rate++;
                                }
#endif
#endif

                                /* Send YUV to mpeg */
                                if(RiscRdCNT(VIDEO_UCOS_TO_LINUX_CNT) < gMaxCntVideoUcosToLinux) {     // VideoUcosToLinuxFull?
                                    if(OSMboxPost(gptMboxVideoInFrame, pTmp) != OS_NO_ERR) {
                                        debug("\nScISR: OSMboxPost error!\n");	
                                    }
                                    gCntFrameFromUcosToLinuxOver = 0;
                                } else {
                                    gCntFrameFromUcosToLinuxOver++;
                                    gCntVideoUcosToLinuxFull++;
                                    printf("$");                                    
                                    if(OSSemPost(gptSemMpegEncodeReady) != OS_NO_ERR)
                                        debug("\nScISR: OSSemPost error!\n");
                                }
                                    
                                /* Send YUV to PIP */
#ifdef PIP_ENABLE
                                if(gPipEnable && gVoutEnable) {
                                    if((err = OSQPost(gptMqueueVideoInFrame, pTmp)) != OS_NO_ERR) {
                                        debug("\nPIP input queue is full, err=%d!\n", err);
                                    }
                                } 
#endif   
                                gCntMpegBusy1 = 0;                                    
                            } 
                            else {    // mpeg is still busy
                                VinBufferNumber = (VinBufferNumber + MAX_VIN_BUF_NUM -1)%MAX_VIN_BUF_NUM;
                                gCntMpegBusy++;
                                if(gCntMpegBusy1++ > 10) {
                                    gCntMpegBusy1 = 0;
                                    gCntMpegEncodeH263TimeOut++;
                                    if(OSQPost(gptQMsgTwoRiscSystemMonitor, (void *)TWO_RISC_SYSTEM_REOPEN_VIN_TASK) != OS_NO_ERR)            
                                        printf("\nScISR: OSQPost error!\n");                                         
                                }                                
                                debug("+");
                            }

                            break;

                        case MJPEGMODE:
                            if(RiscRdCNT(YUV_UCOS_TO_LINUX_CNT) != 0) {     // full?
                                VinBufferNumber = (VinBufferNumber + MAX_VIN_BUF_NUM -1)%MAX_VIN_BUF_NUM;
                                printf("!");
                            } else {
                                /* post a messege to linux */
                                pInt = (unsigned int *)&VideoPacket;
                                    
                                VideoPacket.start = RISC_PKT_START;
                                VideoPacket.main_type = tVideo;
                                VideoPacket.sub_type = tYUVTransferUcosToLinux;
                                VideoPacket.length = 0x10;

                                *(unsigned short *)(pInt + 2) = gImageHeight;
                                *((unsigned short *)(pInt + 2) + 1) = gImageWidth;

                                *(pInt + 3) = pTmp->y_addr | 0xa0000000;
                                *(pInt + 4) = pTmp->u_addr | 0xa0000000;
                                *(pInt + 5) = pTmp->v_addr | 0xa0000000;
                                *(pInt + 6) = GetSysTimeStamp();                //added 2008.3.21, according to protocal, zx asked
                                UCOS_TO_LINUX_YUVBufferNumber = (VinBufferNumber + MAX_VIN_BUF_NUM -1)%MAX_VIN_BUF_NUM;

                                VideoPacket.end = RISC_PKT_END;                                    
                                    
                                if(TwoRiscCommSendPacket((unsigned int *)&VideoPacket, RPACKET_LEN) == SUCCESSFUL) {                				
                                    RiscIncCNT(YUV_UCOS_TO_LINUX_CNT, 1);                            // increase count by 1
                                    RiscFlagSet();
                                } else {
                                    logprintf(FATAL_ERROR, MP4_ENCODE, "~");     
                                }                                    
                            }
                            break;

                        case MPEG_JPEG_MODE:

                            if(OSSemAccept(gptSemMpegEncodeReady)) {    // mpeg is idle?

#ifdef MOTION_DETECTION
#ifndef DVR
                                if((gIsMDEnabled == TRUE) && (gMdType == mdtemporalDif))
                                {
                                    gpVinFrameAddr = pTmp->y_addr;
                                    if(OSMboxPost(gpMboxPpeNewVinFrame, &gpVinFrameAddr) != OS_NO_ERR)
                                        printf("\nScISR: OSMboxPost gpMboxPpeNewVinFrame error\n");
                                    gtStatistics.md_frame_rate++;
                                }
#endif
#endif

                                /* Send YUV to mpeg */
                                if(RiscRdCNT(VIDEO_UCOS_TO_LINUX_CNT) < gMaxCntVideoUcosToLinux) {                                     
                                    if(OSMboxPost(gptMboxVideoInFrame, pTmp) != OS_NO_ERR) {
                                        debug("\nScISR: OSMboxPost error!\n");	
                                    }   
                                    gCntFrameFromUcosToLinuxOver = 0;
                                } else {
                                    gCntFrameFromUcosToLinuxOver++;
                                    gCntVideoUcosToLinuxFull++;
                                    printf("$");                                    
                                    if(OSSemPost(gptSemMpegEncodeReady) != OS_NO_ERR)
                                        debug("\nScISR: OSSemPost error!\n");
                                }

                                /* Send YUV to PIP. Maybe needed later! */

                                /* Send YUV to linux */
                                if(RiscRdCNT(YUV_UCOS_TO_LINUX_CNT) != 0) {     // full?
                                    if(UCOS_TO_LINUX_YUVBufferNumber == VinBufferNumber) {  //skip this buffer, use next buffer
                                        VinBufferNumber = (UCOS_TO_LINUX_YUVBufferNumber + 1) % MAX_VIN_BUF_NUM;
                                    }     
                                    printf("!");
                                } else {    // empty

                                    /* post a messege to linux */
                                    pInt = (unsigned int *)&VideoPacket;
                                        
                                    VideoPacket.start = RISC_PKT_START;
                                    VideoPacket.main_type = tVideo;
                                    VideoPacket.sub_type = tYUVTransferUcosToLinux;
                                    VideoPacket.length = 0x10;

                                    *(unsigned short *)(pInt + 2) = gImageHeight;
                                    *((unsigned short *)(pInt + 2) + 1) = gImageWidth;

                                    *(pInt + 3) = pTmp->y_addr | 0xa0000000;
                                    *(pInt + 4) = pTmp->u_addr | 0xa0000000;
                                    *(pInt + 5) = pTmp->v_addr | 0xa0000000;
                                    *(pInt + 6) = GetSysTimeStamp();                //added 2008.3.21, according to protocal, zx asked
                                    UCOS_TO_LINUX_YUVBufferNumber = (VinBufferNumber + MAX_VIN_BUF_NUM -1)%MAX_VIN_BUF_NUM;

                                    VideoPacket.end = RISC_PKT_END;                                    
                                    
                                    if(TwoRiscCommSendPacket((unsigned int *)&VideoPacket, RPACKET_LEN) == SUCCESSFUL) {                				
                                        RiscIncCNT(YUV_UCOS_TO_LINUX_CNT, 1);                            // increase count by 1
                                        RiscFlagSet();
                                    } else {
                                        logprintf(FATAL_ERROR, MP4_ENCODE, "~");     
                                    }                                    
                                }                                                                        
                                gCntMpegBusy1 = 0;                                
                            } 
                            else {    // mpeg is still busy?
                                VinBufferNumber = (VinBufferNumber + MAX_VIN_BUF_NUM -1)%MAX_VIN_BUF_NUM;
                                gCntMpegBusy++;
                                if(gCntMpegBusy1++ > 10) {
                                    gCntMpegBusy1 = 0;
                                    gCntMpegEncodeH263TimeOut++;
                                    if(OSQPost(gptQMsgTwoRiscSystemMonitor, (void *)TWO_RISC_SYSTEM_REOPEN_VIN_TASK) != OS_NO_ERR)            
                                        printf("\nScISR: OSQPost error!\n");                                         
                                }
                                debug("+");
                            }

                            break;

                        default:                                        
                            printf("\nScISR: VMode error!\n");                                        
                            break;
                                        
                    }
                }    // end of if(vin_skip == 0)
#ifdef MOTION_DETECTION
#ifndef DVR
                else {            //vin_skip == 1
                    pTmp = &gtVideoInFrame[VinBufferNumber];
                    VinBufferNumber = (VinBufferNumber + 1) % MAX_VIN_BUF_NUM;

                    if((gImgEncodeMode == ENCODEDOWN) &&  \
                      (gImage_Mode == EVENODD_IMAGE) && \
                      (gtVinFormat.interlaced == TRUE)) {
                        pTmp->y_addr = pTmp->y_addr + gImageWidth * gImageHeight /2;
                        pTmp->u_addr = pTmp->u_addr + gImageWidth * gImageHeight /8;
                        pTmp->v_addr = pTmp->v_addr + gImageWidth * gImageHeight /8;                        
                    }

                    switch(VMode)
                    {
                        case MPEG4MODE:
                        case H263_MODE:
                            if(OSSemAccept(gptSemMpegEncodeReady)) {    // mpeg is idle?

                                if((gIsMDEnabled == TRUE) && (gMdType == mdtemporalDif))
                                {
                                    gpVinFrameAddr = pTmp->y_addr;
                                    if(OSMboxPost(gpMboxPpeNewVinFrame, &gpVinFrameAddr) != OS_NO_ERR)
                                        printf("\nScISR: OSMboxPost gpMboxPpeNewVinFrame error\n");
                                    gtStatistics.md_frame_rate++;
                                }
                                OSSemPost(gptSemMpegEncodeReady);
                            }else {         //mpeg is busy
                                VinBufferNumber = (VinBufferNumber + MAX_VIN_BUF_NUM -1) % MAX_VIN_BUF_NUM;
                            }
                            break;
                        case MJPEGMODE:
                            break;
                        case MPEG_JPEG_MODE:
                            if(OSSemAccept(gptSemMpegEncodeReady)) {    // mpeg is idle?
                                if((gIsMDEnabled == TRUE) && (gMdType == mdtemporalDif))
                                {
                                    gpVinFrameAddr = pTmp->y_addr;
                                    if(OSMboxPost(gpMboxPpeNewVinFrame, &gpVinFrameAddr) != OS_NO_ERR)
                                        printf("\nScISR: OSMboxPost gpMboxPpeNewVinFrame error\n");
                                    gtStatistics.md_frame_rate++;
                                }
                                OSSemPost(gptSemMpegEncodeReady);
                                if(RiscRdCNT(YUV_UCOS_TO_LINUX_CNT) != 0) {     // full?
                                    if(UCOS_TO_LINUX_YUVBufferNumber == VinBufferNumber) {  //skip this buffer, use next buffer
                                        VinBufferNumber = (UCOS_TO_LINUX_YUVBufferNumber + 1) % MAX_VIN_BUF_NUM;
                                    }     
                                    printf("!");
                                } 
                            }else {    //mpeg is busy
                                VinBufferNumber = (VinBufferNumber + MAX_VIN_BUF_NUM -1)%MAX_VIN_BUF_NUM;
                            }
                            break;
                        default:
                            printf("\nScISR: VMode error!\n");
                            break;
                    }
                }
#endif
#endif
                if(!gtVinFormat.interlaced) {    //vin yuv format progressive
                    McVideoInit(VIN_Y_M, 0, 0, gtVideoInFrame[VinBufferNumber].y_addr);
                    McVideoInit(VIN_U_M, 0, 0, gtVideoInFrame[VinBufferNumber].u_addr);
                    McVideoInit(VIN_V_M, 0, 0, gtVideoInFrame[VinBufferNumber].v_addr);
                }else {        //vin yuv format interlaced
                    if(gVinOddEvenFrameSelect == ODD_FRAME_FIRST) {
                        McVideoInit(VIN_Y_M, 0, 0, gtVideoInFrame[VinBufferNumber].y_addr);
                        McVideoInit(VIN_U_M, 0, 0, gtVideoInFrame[VinBufferNumber].u_addr);
                        McVideoInit(VIN_V_M, 0, 0, gtVideoInFrame[VinBufferNumber].v_addr);
                    }else {
                        if(gImage_Mode == EVENODD_IMAGE) {
                            McVideoInit(VIN_Y_M, 0, 0, gtVideoInFrame[VinBufferNumber].y_addr + gImageWidth * gImageHeight / 2);    // Re-initialize VIN memory to buffer 0
                            McVideoInit(VIN_U_M, 0, 0, gtVideoInFrame[VinBufferNumber].u_addr + gImageWidth * gImageHeight / 8);    // Re-initialize VIN memory to buffer 0
                            McVideoInit(VIN_V_M, 0, 0, gtVideoInFrame[VinBufferNumber].v_addr + gImageWidth * gImageHeight / 8);    // Re-initialize VIN memory to buffer 0
                        }else {
                            McVideoInit(VIN_Y_M, 0, 0, gtVideoInFrame[VinBufferNumber].y_addr + gImageWidth);    // Re-initialize VIN memory to buffer 0
                            McVideoInit(VIN_U_M, 0, 0, gtVideoInFrame[VinBufferNumber].u_addr);    // Re-initialize VIN memory to buffer 0       	
                            McVideoInit(VIN_V_M, 0, 0, gtVideoInFrame[VinBufferNumber].v_addr);    // Re-initialize VIN memory to buffer 0
                        }
                    }
                }
            } 
            else {    // the second field is to be input(just for interlaced)    
                if(gVinOddEvenFrameSelect == ODD_FRAME_FIRST) {
                    if(gImage_Mode == EVENODD_IMAGE) {
                        McVideoInit(VIN_Y_M, 0, 0, gtVideoInFrame[VinBufferNumber].y_addr + gImageWidth * gImageHeight / 2);    // Re-initialize VIN memory to buffer 0
                        McVideoInit(VIN_U_M, 0, 0, gtVideoInFrame[VinBufferNumber].u_addr + gImageWidth * gImageHeight / 8);    // Re-initialize VIN memory to buffer 0       	
                        McVideoInit(VIN_V_M, 0, 0, gtVideoInFrame[VinBufferNumber].v_addr + gImageWidth * gImageHeight / 8);    // Re-initialize VIN memory to buffer 0   
                    } else {
                        McVideoInit(VIN_Y_M, 0, 0, gtVideoInFrame[VinBufferNumber].y_addr + gImageWidth);    // Re-initialize VIN memory to buffer 0
                        McVideoInit(VIN_U_M, 0, 0, gtVideoInFrame[VinBufferNumber].u_addr);    // Re-initialize VIN memory to buffer 0       	
                        McVideoInit(VIN_V_M, 0, 0, gtVideoInFrame[VinBufferNumber].v_addr);    // Re-initialize VIN memory to buffer 0
                    }
                } else {
                    McVideoInit(VIN_Y_M, 0, 0, gtVideoInFrame[VinBufferNumber].y_addr);    // Re-initialize VIN memory to buffer 0
                    McVideoInit(VIN_U_M, 0, 0, gtVideoInFrame[VinBufferNumber].u_addr);    // Re-initialize VIN memory to buffer 0       	
                    McVideoInit(VIN_V_M, 0, 0, gtVideoInFrame[VinBufferNumber].v_addr);    // Re-initialize VIN memory to buffer 0       
                }              
            }
		
#if 0
            /* compute the time of a frame */
            t1= count;
            count = GetCount();
            if(count > t1)
                t1 = count - t1;
            else
                t1 = CPU_MAIN_CLOCK / (2 * OS_TICKS_PER_SEC) + count - t1;	
            printf("<v%d>", t1 / (CPU_MAIN_CLOCK/(2*1000)));    // counted by mili-seconds
#endif			

        }    // if(video_in_frame_count < 100)

        ScOpen(VIN_Y_S);    //Re-open Y stream
        ScOpen(VIN_U_S);    //Re-open U stream
        ScOpen(VIN_V_S);    //Re-open V stream        
        
        /* clear Vin y,u,v interrupt flag */
        flag_vin_y = 0;
        flag_vin_u = 0;
        flag_vin_v = 0;
        
    }    // if((flag_vin_y == 1) && (flag_vin_u == 1) && (flag_vin_v == 1))
#endif  
}

// The following functions come from TestSc.c

/*
***************************************************************************
*                             PrinfScContext
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
void PrinfScContext(int sID)
{
    printf("Stream 0x%x: Context = 0x%x, BaseAddr = 0x%x, Read Pointer = 0x%x, \
ReadAddr = 0x%x; Write Pointer = 0x%x, WriteAddr = 0x%x. Status = %d.", \
            sID, ScRdContext(sID), ScRdBaseAddr(sID), ScRdRPTR(sID), ScRdReadAddr(sID), \
            ScRdWPTR(sID), ScRdWriteAddr(sID), ScRdStatus(sID));	
}

/*
***************************************************************************
*                             DebugScContext
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
void DebugScContext(int sID)
{
    debug("Stream 0x%x: Context = 0x%x, BaseAddr = 0x%x, Read Pointer = 0x%x, \
ReadAddr = 0x%x; Write Pointer = 0x%x, WriteAddr = 0x%x. Status = %d.", \
            sID, ScRdContext(sID), ScRdBaseAddr(sID), ScRdRPTR(sID), ScRdReadAddr(sID), \
            ScRdWPTR(sID), ScRdWriteAddr(sID), ScRdStatus(sID));	
}

