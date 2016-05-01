/*
********************************************************************************************
*                  Copyright (c) 2007 vBridge Microsystem, Inc.  
*                        Unpublished & Not for Publication
*                               All Rights Reserved                   
*   
* File        :two_risc_pip_ctrl_risc1.c                      
*                                                                         
* Description :    
*             
* Date        :                                                      
********************************************************************************************
*/
#define TWO_RISC_PIP_CONTROL_GLOBALS

#include "debug_risc1_cfg.h"

#include "os_cpu.h"
#include "os_cfg.h"
#include "uCOS_II.h"

#include "2risc_comm_protocol.h"
#include "video_api.h"
#include "comm.h"
#include "two_risc_comm.h"
#include "risc.h"

#include "two_risc_pip_ctrl_risc1.h"

#ifdef TASK_SYSTEM_CONTROL_EN
unsigned int PipSwitchCnt = 0;              //indicates the times of PIP switched
/*
*********************************************************************************************
*                                  TaskPIPControl
*
* Description:turn on and off the PIP continues     
*
* Arguments  : none.
*
* Return     : none.
*
* Note(s)    : none.
*********************************************************************************************
*/
void TaskPIPControl(void *data)
{
    unsigned int *pInt;     
    Risc_Packet_t risc_packet;         
    
    pInt = (unsigned int *)&risc_packet;
    risc_packet.start = RISC_PKT_START;
    risc_packet.main_type = tVideo;
    risc_packet.sub_type = tVideoEnableDisable;
    risc_packet.length = 0x4;
    *(pInt + 2) = 1;    // 1: ENABLE; 0: DISABLE    
    *(pInt + 3) = 8|0x4;    // 8: pip                   
    risc_packet.status = 0;   
    risc_packet.end = RISC_PKT_END;   

    while(1)
    {
        *(pInt + 2) ^= 0x1;
        if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
            RiscFlagSet();    // Set risc1 flag to interrupt risc1        
            PipSwitchCnt++;
        } else 
            debug("\nTaskPIPControlRisc1: send packet error\n");            
        
        if (0 == PipSwitchCnt & 0xf)  //print the value of PipSwitchCnt every 16 times
            printc("\nTaskPIPControlRisc1:PIP switched %u times\n", PipSwitchCnt);

        OSTimeDlyHMSM (0, 0, 0, 500);
    }	
}

#endif 
