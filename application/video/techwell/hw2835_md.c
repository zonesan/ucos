/*
***************************************************************************
*                   Copyright (c) 2007 vBridge Microsystem, Inc.  
*                          Unpublished & Not for Publication
*                                 All Rights Reserved                   
*   
* File        :     Remote_Control.c 
*                                                                         
* Description : Receive the remote control pulses and decode                                          *                                                                          
*
* Date        : June 9, 2007                                                           
***************************************************************************
*/



#include "includes.h"
#include "video_cfg.h"
#ifdef DVR
#include "hw2835_md.h"

#include "../2risc platform/2risc_memory_map_IPC.h"
#include "../2risc platform/2risc_comm_protocol.h"
#include "../2risc platform/video_api.h"

#include "os_cpu.h"
#include "os_cfg.h"
#include "uCOS_II.h"

#include "prtcl.h"
#include "gpio.h"
#include "../application/two_risc_system.h"

#include "tw2835_api.h"
#include "Motion_Detection.h"
extern int gMDPlayTime;
/*global variable*/

OS_EVENT *QEeventMotionDetector;

int ReadGpioState(void)
{

    unsigned int test;

    /* enable gpio intr */
    
    rRISC_GPIO_DIR |= (0x1<<GPIO_MOYION_DETECTOR_BIT); 
    test = rRISC_GPIO;
    //test = (test & 0x4000)>>14;
    //rRISC_GPIO |= 0x1<<GPIO_REMOTE_CONTROL_BIT; 
    //if(test != 0) 
    printf("test = %x!\n", test);
    return SUCCESSFUL;
}


/*
***************************************************************************
*                            HW2835MotionDetectorInit
*
* Description: HW2835 motion detector interrupt initiation
*
* Arguments  : 
*
* Return     : 
*
* Note(s)    : 
***************************************************************************
*/

void HW2835MotionDetectorInit()
{
  
    RiscGPIOIntrInit();
    HW2835MotionDetectorGpioIntrInit();


}


/*
***************************************************************************
*                            HW2835MotionDetectorGpioISR
*
* Description: motion detectorcontrol gpio interrupt service routine
*
* Arguments  : 
*
* Return     : 
*
* Note(s)    : 
***************************************************************************
*/

void  HW2835MotionDetectorGpioISR(void)
{ 
    unsigned char irqena_md, irqena_bd, irqena_novid, irqena_nd, temp4, temp5, temp6;
    Risc_Packet_t risc_packet;
    unsigned char md_channel;
    unsigned char err;

    clr_gpio_int_ena(GPIO_MOYION_DETECTOR_BIT);   
	
    err = OSQPost(gptQMsgTwoRiscSystemMonitor, (void *)TWO_RISC_SYSTEM_DVR_READ_MOTION_DETECTION);                            
    if(err != OS_NO_ERR)            
        debug("\r\nRISC0: OSMboxPost error! err = %d!\r\n", err); 

}

void HwSendVideoStatus(void)
{
     unsigned char i,irqena_bd,irqena_novid,irqena_nd,VideoStatusTmp[4];
     Risc_Packet_t risc_packet;
     
//--------------send video loss information to linux at the start---------     
     irqena_bd = ReadAsicByte(MASTER,DVC_PG1,0x7a);
     irqena_novid = ReadAsicByte(MASTER,DVC_PG1,0x78);
     irqena_nd = ReadAsicByte(MASTER,DVC_PG1,0x7b);
     
     for(i=0;i<4;i++)
                VideoStatusTmp[i] =((irqena_novid&(1<<i))>>i)|(((irqena_bd&(1<<i))<<1)>>i)
                                              |(((irqena_nd&(1<<i))<<2)>>i);
     
     risc_packet.start = RISC_PKT_START;
     risc_packet.main_type = tDVR;
     risc_packet.sub_type = tGetVideoLossBlindNightStatus;
     risc_packet.length = 0x10;
     risc_packet.status = 0;
        
     risc_packet.message[0] = VideoStatusTmp[0];//ch0 : bit0-- video loss, bit1--BD, bit2--ND
     risc_packet.message[1] = VideoStatusTmp[1];//ch1 : bit0-- video loss, bit1--BD, bit2--ND
     risc_packet.message[2] = VideoStatusTmp[2];//ch2 : bit0-- video loss, bit1--BD, bit2--ND
     risc_packet.message[3] = VideoStatusTmp[3];//ch3 : bit0-- video loss, bit1--BD, bit2--ND
     risc_packet.end = RISC_PKT_END;

     if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {         
         RiscFlagSet();    // Set risc1 flag to interrupt risc1   
         } 
     else 
        debug("\r\n DVR video status ERROR: send packet error!\r\n");

}


//static unsigned char md_test = 0;
void Hw2835ReadMDRegister(void)
{
    unsigned char irqena_md, irqena_bd, irqena_novid, irqena_nd, tempVL, tempBD, tempND;
    Risc_Packet_t risc_packet;
    unsigned char md_channel;
    unsigned char err, i;
    unsigned char cAreaHigh, cAreaLow;
  
    

    //clr_gpio_int_ena(GPIO_MOYION_DETECTOR_BIT);


    irqena_md = ReadAsicByte(MASTER,DVC_PG1,0x79);
    irqena_bd = ReadAsicByte(MASTER,DVC_PG1,0x7a);
    irqena_novid = ReadAsicByte(MASTER,DVC_PG1,0x78);
    irqena_nd = ReadAsicByte(MASTER,DVC_PG1,0x7b);
  
//---------add video loss ,blind detect,night detect --vincent 2008.01.31------------------------------
    unsigned char VideoStatusTmp[4]={0};
    static unsigned char videostatus[3]={0};//store the last video status
//only when video status change ,send message to linux  
    if((videostatus[0]!=irqena_novid)||(videostatus[1]!=irqena_bd)||(videostatus[2]!=irqena_nd))
    {  //only when one of the 3 detect is enable,then send message to linux
        if((gVideoLossEn!=0)||(gVideoBlindEn!=0)||(gVideoNightEn!=0))
        {
            tempVL = gVideoLossEn & irqena_novid;
            tempBD = gVideoBlindEn & irqena_bd;
            tempND = gVideoNightEn & irqena_nd;
                 	    
            for(i=0;i<4;i++)
                VideoStatusTmp[i] =((tempVL&(1<<i))>>i)|(((tempBD&(1<<i))<<1)>>i)
                                              |(((tempND&(1<<i))<<2)>>i);

            
             risc_packet.start = RISC_PKT_START;
             risc_packet.main_type = tDVR;
             risc_packet.sub_type = tGetVideoLossBlindNightStatus;
             risc_packet.length = 0x10;
             risc_packet.status = 0;
                
             risc_packet.message[0] = VideoStatusTmp[0];//ch0 : bit0-- video loss, bit1--BD, bit2--ND
             risc_packet.message[1] = VideoStatusTmp[1];//ch1 : bit0-- video loss, bit1--BD, bit2--ND
             risc_packet.message[2] = VideoStatusTmp[2];//ch2 : bit0-- video loss, bit1--BD, bit2--ND
             risc_packet.message[3] = VideoStatusTmp[3];//ch3 : bit0-- video loss, bit1--BD, bit2--ND
             risc_packet.end = RISC_PKT_END;
        
             if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {         
                 RiscFlagSet();    // Set risc1 flag to interrupt risc1   
                 } 
             else 
                debug("\r\n DVR video status ERROR: send packet error!\r\n");
        }
        videostatus[0]=irqena_novid;//updata status
        videostatus[1]=irqena_bd;
        videostatus[2]=irqena_nd;
    }

//---------vincent 2008.01.31------------------------------------
    if(irqena_md &0x0f) gMDPlayTime = gtMDFrameRateCtrl.motionduration;
    if(((irqena_md & 0x01)== 1)&&(gVideoMotionEn&(0x01)))//only when MD is enable,then send msg to linux
    {        
        risc_packet.start = RISC_PKT_START;
        risc_packet.main_type = tDVR;
        risc_packet.sub_type = tTransmitMotionState;
        risc_packet.length = 0x10;
        risc_packet.status = 0;
        
        risc_packet.message[0] = 0;
        risc_packet.message[1] = 0;
        risc_packet.message[2] = 0;
        risc_packet.message[3] = 0;  
        //debug("\n");
        for(i = 0; i < 12; i+=2){
            cAreaHigh = ReadAsicByte(MASTER,DVC_PG2,0x87+0x20*0+i);
            cAreaLow = ReadAsicByte(MASTER,DVC_PG2,0x86+0x20*0+i);
            risc_packet.message[4+i] = ((cAreaHigh>>7)&0x01)|((cAreaHigh>>5)&0x02)|((cAreaHigh>>3)&0x04)|((cAreaHigh>>1)&0x08)|((cAreaHigh<<1)&0x10)|((cAreaHigh<<3)&0x20)|((cAreaHigh<<5)&0x40)|((cAreaHigh<<7)&0x80);
            risc_packet.message[4+i+1] = ((cAreaLow>>7)&0x01)|((cAreaLow>>5)&0x02)|((cAreaLow>>3)&0x04)|((cAreaLow>>1)&0x08)|((cAreaLow<<1)&0x10)|((cAreaLow<<3)&0x20)|((cAreaLow<<5)&0x40)|((cAreaLow<<7)&0x80);  
            }

        risc_packet.end = RISC_PKT_END;        

        if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
            RiscFlagSet();    // Set risc1 flag to interrupt risc1   
            } 
        else 
            debug("\r\n DVR@ ERROR: send packet error!\r\n");


        risc_packet.start = RISC_PKT_START;
        risc_packet.main_type = tDVR;
        risc_packet.sub_type = tTransmitMotionState;
        risc_packet.length = 0x10;
        risc_packet.status = 0;
        
        risc_packet.message[0] = 0;
        risc_packet.message[1] = 0;
        risc_packet.message[2] = 1;
        risc_packet.message[3] = 0;        

        for(i = 0; i < 12; i+=2){
            //risc_packet.message[4+i] = ReadAsicByte(MASTER,DVC_PG2,0x93+0x20*md_set_ch+i);
            //risc_packet.message[4+i+1] = ReadAsicByte(MASTER,DVC_PG2,0x92+0x20*md_set_ch+i);
            cAreaHigh = ReadAsicByte(MASTER,DVC_PG2,0x93+0x20*0+i);
            cAreaLow = ReadAsicByte(MASTER,DVC_PG2,0x92+0x20*0+i);
            risc_packet.message[4+i] = ((cAreaHigh>>7)&0x01)|((cAreaHigh>>5)&0x02)|((cAreaHigh>>3)&0x04)|((cAreaHigh>>1)&0x08)|((cAreaHigh<<1)&0x10)|((cAreaHigh<<3)&0x20)|((cAreaHigh<<5)&0x40)|((cAreaHigh<<7)&0x80);
            risc_packet.message[4+i+1] = ((cAreaLow>>7)&0x01)|((cAreaLow>>5)&0x02)|((cAreaLow>>3)&0x04)|((cAreaLow>>1)&0x08)|((cAreaLow<<1)&0x10)|((cAreaLow<<3)&0x20)|((cAreaLow<<5)&0x40)|((cAreaLow<<7)&0x80);  

            }


        risc_packet.end = RISC_PKT_END;

        if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
            //printf("\n1 Transmit Successful !\n");
            RiscFlagSet();    // Set risc1 flag to interrupt risc1   
            } 
        else 
            debug("\r\n DVR@ ERROR: send packet error!\r\n");

    }

    if(((irqena_md & 0x02)== 2)&&(gVideoMotionEn&(0x02)))
        {        
        risc_packet.start = RISC_PKT_START;
        risc_packet.main_type = tDVR;
        risc_packet.sub_type = tTransmitMotionState;
        risc_packet.length = 0x10;
        risc_packet.status = 0;
        
        risc_packet.message[0] = 1;
        risc_packet.message[1] = 0;
        risc_packet.message[2] = 0;
        risc_packet.message[3] = 0;   
        for(i = 0; i < 12; i+=2){
            cAreaHigh = ReadAsicByte(MASTER,DVC_PG2,0x87+0x20*1+i);
            cAreaLow = ReadAsicByte(MASTER,DVC_PG2,0x86+0x20*1+i);
            risc_packet.message[4+i] = ((cAreaHigh>>7)&0x01)|((cAreaHigh>>5)&0x02)|((cAreaHigh>>3)&0x04)|((cAreaHigh>>1)&0x08)|((cAreaHigh<<1)&0x10)|((cAreaHigh<<3)&0x20)|((cAreaHigh<<5)&0x40)|((cAreaHigh<<7)&0x80);
            risc_packet.message[4+i+1] = ((cAreaLow>>7)&0x01)|((cAreaLow>>5)&0x02)|((cAreaLow>>3)&0x04)|((cAreaLow>>1)&0x08)|((cAreaLow<<1)&0x10)|((cAreaLow<<3)&0x20)|((cAreaLow<<5)&0x40)|((cAreaLow<<7)&0x80);  

            }

        risc_packet.end = RISC_PKT_END;        

        if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
            RiscFlagSet();    // Set risc1 flag to interrupt risc1   
            } 
        else 
            debug("\r\n DVR@ ERROR: send packet error!\r\n");


        risc_packet.start = RISC_PKT_START;
        risc_packet.main_type = tDVR;
        risc_packet.sub_type = tTransmitMotionState;
        risc_packet.length = 0x10;
        risc_packet.status = 0;
        
        risc_packet.message[0] = 1;
        risc_packet.message[1] = 0;
        risc_packet.message[2] = 1;
        risc_packet.message[3] = 0;        

        for(i = 0; i < 12; i+=2){
            cAreaHigh = ReadAsicByte(MASTER,DVC_PG2,0x93+0x20*1+i);
            cAreaLow = ReadAsicByte(MASTER,DVC_PG2,0x92+0x20*1+i);
            risc_packet.message[4+i] = ((cAreaHigh>>7)&0x01)|((cAreaHigh>>5)&0x02)|((cAreaHigh>>3)&0x04)|((cAreaHigh>>1)&0x08)|((cAreaHigh<<1)&0x10)|((cAreaHigh<<3)&0x20)|((cAreaHigh<<5)&0x40)|((cAreaHigh<<7)&0x80);
            risc_packet.message[4+i+1] = ((cAreaLow>>7)&0x01)|((cAreaLow>>5)&0x02)|((cAreaLow>>3)&0x04)|((cAreaLow>>1)&0x08)|((cAreaLow<<1)&0x10)|((cAreaLow<<3)&0x20)|((cAreaLow<<5)&0x40)|((cAreaLow<<7)&0x80);  

            }

        risc_packet.end = RISC_PKT_END;

        if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
            //printf("\n1 Transmit Successful !\n");
            RiscFlagSet();    // Set risc1 flag to interrupt risc1   
            } 
        else 
            debug("\r\n DVR@ ERROR: send packet error!\r\n");

    }

    if(((irqena_md & 0x04)== 4)&&(gVideoMotionEn&(0x04)))
        {        
        risc_packet.start = RISC_PKT_START;
        risc_packet.main_type = tDVR;
        risc_packet.sub_type = tTransmitMotionState;
        risc_packet.length = 0x10;
        risc_packet.status = 0;
        
        risc_packet.message[0] = 2;
        risc_packet.message[1] = 0;
        risc_packet.message[2] = 0;
        risc_packet.message[3] = 0;   
        for(i = 0; i < 12; i+=2){
            cAreaHigh = ReadAsicByte(MASTER,DVC_PG2,0x87+0x20*2+i);
            cAreaLow = ReadAsicByte(MASTER,DVC_PG2,0x86+0x20*2+i);
            risc_packet.message[4+i] = ((cAreaHigh>>7)&0x01)|((cAreaHigh>>5)&0x02)|((cAreaHigh>>3)&0x04)|((cAreaHigh>>1)&0x08)|((cAreaHigh<<1)&0x10)|((cAreaHigh<<3)&0x20)|((cAreaHigh<<5)&0x40)|((cAreaHigh<<7)&0x80);
            risc_packet.message[4+i+1] = ((cAreaLow>>7)&0x01)|((cAreaLow>>5)&0x02)|((cAreaLow>>3)&0x04)|((cAreaLow>>1)&0x08)|((cAreaLow<<1)&0x10)|((cAreaLow<<3)&0x20)|((cAreaLow<<5)&0x40)|((cAreaLow<<7)&0x80);  

            }

        risc_packet.end = RISC_PKT_END;        

        if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
            RiscFlagSet();    // Set risc1 flag to interrupt risc1   
            } 
        else 
            debug("\r\n DVR@ ERROR: send packet error!\r\n");


        risc_packet.start = RISC_PKT_START;
        risc_packet.main_type = tDVR;
        risc_packet.sub_type = tTransmitMotionState;
        risc_packet.length = 0x10;
        risc_packet.status = 0;
        
        risc_packet.message[0] = 2;
        risc_packet.message[1] = 0;
        risc_packet.message[2] = 1;
        risc_packet.message[3] = 0;        

        for(i = 0; i < 12; i+=2){
            cAreaHigh = ReadAsicByte(MASTER,DVC_PG2,0x93+0x20*2+i);
            cAreaLow = ReadAsicByte(MASTER,DVC_PG2,0x92+0x20*2+i);
            risc_packet.message[4+i] = ((cAreaHigh>>7)&0x01)|((cAreaHigh>>5)&0x02)|((cAreaHigh>>3)&0x04)|((cAreaHigh>>1)&0x08)|((cAreaHigh<<1)&0x10)|((cAreaHigh<<3)&0x20)|((cAreaHigh<<5)&0x40)|((cAreaHigh<<7)&0x80);
            risc_packet.message[4+i+1] = ((cAreaLow>>7)&0x01)|((cAreaLow>>5)&0x02)|((cAreaLow>>3)&0x04)|((cAreaLow>>1)&0x08)|((cAreaLow<<1)&0x10)|((cAreaLow<<3)&0x20)|((cAreaLow<<5)&0x40)|((cAreaLow<<7)&0x80);  

            }

        risc_packet.end = RISC_PKT_END;

        if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
            //printf("\n1 Transmit Successful !\n");
            RiscFlagSet();    // Set risc1 flag to interrupt risc1   
            } 
        else 
            debug("\r\n DVR@ ERROR: send packet error!\r\n");

    }

    if(((irqena_md & 0x08)== 8)&&(gVideoMotionEn&(0x08)))
        {        
        risc_packet.start = RISC_PKT_START;
        risc_packet.main_type = tDVR;
        risc_packet.sub_type = tTransmitMotionState;
        risc_packet.length = 0x10;
        risc_packet.status = 0;
        
        risc_packet.message[0] = 3;
        risc_packet.message[1] = 0;
        risc_packet.message[2] = 0;
        risc_packet.message[3] = 0;   
        for(i = 0; i < 12; i+=2){
            cAreaHigh = ReadAsicByte(MASTER,DVC_PG2,0x87+0x20*3+i);
            cAreaLow = ReadAsicByte(MASTER,DVC_PG2,0x86+0x20*3+i);
            risc_packet.message[4+i] = ((cAreaHigh>>7)&0x01)|((cAreaHigh>>5)&0x02)|((cAreaHigh>>3)&0x04)|((cAreaHigh>>1)&0x08)|((cAreaHigh<<1)&0x10)|((cAreaHigh<<3)&0x20)|((cAreaHigh<<5)&0x40)|((cAreaHigh<<7)&0x80);
            risc_packet.message[4+i+1] = ((cAreaLow>>7)&0x01)|((cAreaLow>>5)&0x02)|((cAreaLow>>3)&0x04)|((cAreaLow>>1)&0x08)|((cAreaLow<<1)&0x10)|((cAreaLow<<3)&0x20)|((cAreaLow<<5)&0x40)|((cAreaLow<<7)&0x80);  

            }

        risc_packet.end = RISC_PKT_END;        

        if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
            RiscFlagSet();    // Set risc1 flag to interrupt risc1   
            } 
        else 
            debug("\r\n DVR@ ERROR: send packet error!\r\n");


        risc_packet.start = RISC_PKT_START;
        risc_packet.main_type = tDVR;
        risc_packet.sub_type = tTransmitMotionState;
        risc_packet.length = 0x10;
        risc_packet.status = 0;
        
        risc_packet.message[0] = 3;
        risc_packet.message[1] = 0;
        risc_packet.message[2] = 1;
        risc_packet.message[3] = 0;        

        for(i = 0; i < 12; i+=2){
            cAreaHigh = ReadAsicByte(MASTER,DVC_PG2,0x93+0x20*3+i);
            cAreaLow = ReadAsicByte(MASTER,DVC_PG2,0x92+0x20*3+i);
            risc_packet.message[4+i] = ((cAreaHigh>>7)&0x01)|((cAreaHigh>>5)&0x02)|((cAreaHigh>>3)&0x04)|((cAreaHigh>>1)&0x08)|((cAreaHigh<<1)&0x10)|((cAreaHigh<<3)&0x20)|((cAreaHigh<<5)&0x40)|((cAreaHigh<<7)&0x80);
            risc_packet.message[4+i+1] = ((cAreaLow>>7)&0x01)|((cAreaLow>>5)&0x02)|((cAreaLow>>3)&0x04)|((cAreaLow>>1)&0x08)|((cAreaLow<<1)&0x10)|((cAreaLow<<3)&0x20)|((cAreaLow<<5)&0x40)|((cAreaLow<<7)&0x80);  

            }

        risc_packet.end = RISC_PKT_END;

        if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
            //printf("\n1 Transmit Successful !\n");
            RiscFlagSet();    // Set risc1 flag to interrupt risc1   
            } 
        else 
            debug("\r\n DVR@ ERROR: send packet error!\r\n");

    }
    
    //set_gpio_int_pol(GPIO_MOYION_DETECTOR_BIT, 1);    //set active High
    set_gpio_int_ena(GPIO_MOYION_DETECTOR_BIT);   



}





/*
***************************************************************************
*                                HW2835MotionDetectorGpioIntrInit
*
* Description: Enable HW2835 Motion detector gpio interrupt
*
* Arguments  : 
*
* Return     : 
*
* Note(s)    : 
***************************************************************************
*/
int HW2835MotionDetectorGpioIntrInit(void)
{

    /* enable gpio intr */
    set_gpio_in(GPIO_MOYION_DETECTOR_BIT);
    clr_gpio(GPIO_MOYION_DETECTOR_BIT);

    set_gpio_int_pol(GPIO_MOYION_DETECTOR_BIT, 1);    //set active High
    set_gpio_int_ena(GPIO_MOYION_DETECTOR_BIT);     
    return SUCCESSFUL;
    
}


#endif


