/*
********************************************************************************************
*                  Copyright (c) 2007 vBridge Microsystem, Inc.  
*                        Unpublished & Not for Publication
*                               All Rights Reserved                   
*   
* File        :two_risc_resolution_ctrl_risc1.c                      
*                                                                         
* Description :    
*             
* Date        :                                                      
********************************************************************************************
*/
#define TWO_RISC_RESOLUTION_CONTROL_GLOBALS

#include "debug_risc1_cfg.h"

#include "os_cpu.h"
#include "os_cfg.h"
#include "uCOS_II.h"

#include "2risc_comm_protocol.h"
#include "video_api.h"
#include "comm.h"
#include "two_risc_comm.h"
#include "risc.h"

#include "two_risc_resolution_ctrl_risc1.h"

#ifdef TASK_SYSTEM_CONTROL_EN
unsigned int ResolutionSwitchCnt = 0;          //indicates the times of Resolution switched 
/*
*********************************************************************************************
*                                  TaskResolutionControl
*
* Description:change the RESOLUTION continuously between QQVGA,QVGA,VGA,
*                            QCIF,CIF,NTSC_D1,PAL_D1    
*
* Arguments  : none.
*
* Return     : none.
*
* Note(s)    : none.
*********************************************************************************************
*/
void TaskResolutionControl(void *data)
{
    unsigned char shift = 0x1;
    unsigned short width, height;

    unsigned int *pInt;     
    Risc_Packet_t risc_packet;         
    
    pInt = (unsigned int *)&risc_packet;
    risc_packet.start = RISC_PKT_START;
    risc_packet.main_type = tVideo;
    risc_packet.sub_type = tSetVideoSize;
    risc_packet.length = 0x8;
    *(pInt + 2) = 1;    //Mode VIN  
    risc_packet.status = 0;   
    risc_packet.end = RISC_PKT_END;   
            
    while(1)
    {
        if (0x80 == shift) {      //if shift is 0x80,it must start from QQVGA again.
            shift = 0x1;
        }

        switch (shift) 
        {
            case 0x1:
                width=160;  //QQVGA WIDTH;
                height=120;    //QQVGA HEIGHT;
                break;
                 
            case 0x2:
                width=320;//QVGA WIDTH;
                height=240;//QVGA HEIGHT;
                break;
                 
            case 0x4:
                width=640;//VGA WIDTH;
                height=480;//VGA HEIGHT;
                break;
                 
            case 0x8:
                width=176;  //QCIF WIDTH;
                height=144;    //QCIF HEIGHT;
                break;
                 
            case 0x10:
                width=352;//CIF WIDTH;
                height=288;//CIF HEIGHT;
                break;
                 
            case 0x20:
                width = 704;  //NTSC_D1 WIDTH;
                height = 480;    //NTSC_D1 HEIGHT;
                break;

            case 0x40:
                width = 704;//PAL_D1 WIDTH;
                height = 576;//PAL_D1 HEIGHT;
                break;                  
                 
            default:
                width=320;//QVGA WIDTH;
                height=240;//QVGA HEIGHT;
                break;
        }
        
        shift <<= 0x1;
        *(unsigned short *)(pInt  + 3) = width;
        *((unsigned short *)(pInt  + 3) + 1) = height;
        
        if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
            RiscFlagSet();    // Set risc1 flag to interrupt risc1      
            ResolutionSwitchCnt++;        
        } else 
            debug("\nTaskResolutionControlRisc1: send packet error\n");

        if (0 == ResolutionSwitchCnt & 0x7)   //print the value of ResolutionSwitchCnt every 8 times
            printc("\nresolution switched %u times\n", ResolutionSwitchCnt);

        OSTimeDlyHMSM (0, 0, 1, 0);
    }	
}

#endif 

