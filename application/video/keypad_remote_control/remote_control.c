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



#define REMOTE_CONTROL_GLOBALS

#include "video_cfg.h"
#ifdef DVR

#include "includes.h"
#include "remote_control.h"
#include "gpio.h"

#include "../2risc platform/2risc_memory_map_IPC.h"
#include "../2risc platform/2risc_comm_protocol.h"
#include "../2risc platform/video_api.h"

#include "os_cpu.h"
#include "os_cfg.h"
#include "uCOS_II.h"


/*
***************************************************************************
*                          TaskRemoteControl
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

/*remote control data*/

int Remote_Control_DATA[40]= {
    0xf50afb00, 0xf30cfb00, 0xf00ffb00, 0xfe01fb00, 0xfd02fb00, 0xfc03fb00, 0xfb04fb00, 0xfa05fb00,
    0xf906fb00, 0xf807fb00, 0xf708fb00, 0xf609fb00, 0xba45fb00, 0xff00fb00, 0xe21dfb00, 0xbf40fb00,
    0xe51afb00, 0xbd42fb00, 0xbe41fb00, 0xf40bfb00, 0xef10fb00, 0xf20dfb00, 0xee11fb00, 0xf10efb00, 
    0xea15fb00, 0xe31cfb00, 0xe11efb00, 0xe01ffb00, 0xe916fb00, 0xe817fb00, 0xe718fb00, 0xe619fb00, 
    0xb847fb00, 0xbc43fb00, 0xb946fb00, 0xed12fb00, 0xec13fb00, 0xe41bfb00, 0xeb14fb00, 0xbb44fb00
};

/*global variable*/
volatile int gpio_enter;
volatile int gpiointr_num;
volatile int code_num_enter;
volatile int flag_key_enter;
volatile int lpress_key;
volatile int flag_lpress1;
volatile int flag_lpress2;

void TaskRemoteControl()
{

    Risc_Packet_t risc_packet;
    OS_CPU_SR    cpu_sr;

    BspStartTicker(OS_TICKS_PER_SEC);

    RemoteControlInit();
    flag_lpress1 = 0;
    flag_lpress2 = 0;
    
    while(1) {     

        OSTimeDlyHMSM(0, 0, 0, 150);

        //printf("#");                    
        if(flag_lpress1){
            flag_lpress1 = 0;
        } else if(flag_lpress2){
        
            OS_ENTER_CRITICAL();
            gpiointr_num = 0;
            flag_key_enter = 0;
            flag_lpress1 = 0;
            flag_lpress2 = 0;

            if(lpress_key<REMOTE_CONTROL_NO_KEY_BIT){

                printf("IRCode=0x%x\n", lpress_key & 0xffff |0x20000);

                risc_packet.start = RISC_PKT_START;
                risc_packet.main_type = tDVR;
                risc_packet.sub_type = tRemoteControl;
                risc_packet.length = 0x4;
                risc_packet.status = 0;

                *((int *)(&risc_packet.message[0])) = lpress_key & 0xffff |0x20000;
                OS_EXIT_CRITICAL();

                risc_packet.end = RISC_PKT_END;

                if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                    RiscFlagSet();    // Set risc1 flag to interrupt risc1            
                } else 
                    debug("\r\n RemoteControlGpioISR: send packet error!\r\n");
            }else{
                OS_EXIT_CRITICAL();
            }
            

            //lpress_key = REMOTE_CONTROL_NO_KEY_BIT;
            //OS_EXIT_CRITICAL();
            

        }                                                                       

    }
	
}


/*
***************************************************************************
*                            RemoteControlInit
*
* Description: Global variable initiation and interrupt initiation
*
* Arguments  : 
*
* Return     : 
*
* Note(s)    : 
***************************************************************************
*/

void RemoteControlInit()
{
    
    gpio_enter = 0;
    gpiointr_num = 0;
    code_num_enter = 0;
    flag_key_enter = 0;

    RiscGPIOIntrInit();
    RemoteControlGpioIntrInit();


}


/*
***************************************************************************
*                            RemoteControlGpioISR
*
* Description: Remote control gpio interrupt service routine
*
* Arguments  : 
*
* Return     : 
*
* Note(s)    : 
***************************************************************************
*/
void  RemoteControlGpioISR(void)
{   
    /* Clear the intr source */
    static int count;
    Risc_Packet_t risc_packet;
    
    int i, t1;
    int pulse_width; 
    short code_data_enter, re_code_data_enter;
    short code_data;
    
    t1 = count;
    count = GetCount();
    if(count > t1)
         t1 = count - t1;
    else
         t1 = CPU_MAIN_CLOCK / (2 * OS_TICKS_PER_SEC) + count - t1;

     pulse_width = t1 / (CPU_MAIN_CLOCK/(2*1000000));
   //  printf("<%d>", t1 / (CPU_MAIN_CLOCK/(2*1000000)));    // counted by u-seconds
    
    gpiointr_num++;
   // printf("gpiointr_num=%d\n",gpiointr_num);

    if(flag_key_enter==0){
        if(gpiointr_num==1){  
            if(((rRISC_GPIO_POL>>GPIO_REMOTE_CONTROL_BIT)&0x1)==0)
                gpiointr_num = 0;
        } else if(gpiointr_num==2){
            if((rRISC_GPIO_POL>>GPIO_REMOTE_CONTROL_BIT)&0x1 || (pulse_width<LEADER_LOW_PULSE_WIDTH_MIN)||(pulse_width>LEADER_LOW_PULSE_WIDTH_MAX)){ 
                gpiointr_num = 0;
                code_num_enter = 0;
            }
        } else if(gpiointr_num==3){
            if((pulse_width<LEADER_HIGH_PULSE_WIDTH_MIN)||(pulse_width>LEADER_HIGH_PULSE_WIDTH_MAX)){ 
                gpiointr_num = 0;
                code_num_enter = 0;
            }
        } else if(gpiointr_num>3){
            if((rRISC_GPIO_POL>>GPIO_REMOTE_CONTROL_BIT)&0x1){
                if((pulse_width>ONE_HIGH_PULSE_WIDTH_MIN)&&(pulse_width<ONE_HIGH_PULSE_WIDTH_MAX)){              
                    gpio_enter|= 1<<code_num_enter;
                    code_num_enter++;               
                    if(code_num_enter==32)  flag_key_enter=1;
                } else if((pulse_width>ZERO_HIGH_PULSE_WIDTH_MIN)&&(pulse_width<ZERO_HIGH_PULSE_WIDTH_MAX)){
                    gpio_enter|= 0<<code_num_enter;
                    code_num_enter++;               
                    if(code_num_enter==32)  flag_key_enter=1;
                } else {
                    gpiointr_num = 0; 
                    code_num_enter = 0;
                }           
            
            } else if((pulse_width<LOW_PULSE_WIDTH_MIN)||(pulse_width>LOW_PULSE_WIDTH_MAX)) {
                gpiointr_num = 0;
                code_num_enter = 0;
            }
            
        }

        
        if(flag_key_enter) {
                    
                gpiointr_num = 0;
                code_num_enter = 0;

                code_data = REMOTE_CONTROL_NO_KEY_BIT;

                code_data_enter = (gpio_enter>>16)&0xff;
                re_code_data_enter = (gpio_enter>>24)&0xff;
                re_code_data_enter = (~re_code_data_enter)&0xff; 
                
                if(code_data_enter==re_code_data_enter){
                    for(i=0; i<40; i++){
                        if(gpio_enter==Remote_Control_DATA[i]){
                            code_data = (Remote_Control_DATA[i]>>16)&0xff; 
                            lpress_key = code_data;
                        } else printf("%");
                    }
                } else printf("!");

                if(code_data<REMOTE_CONTROL_NO_KEY_BIT){
                    
                    printf("IRCode=0x%x\n", code_data & 0xffff);
                    
                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tDVR;
                    risc_packet.sub_type = tRemoteControl;
                    risc_packet.length = 0x4;
                    risc_packet.status = 0;

                    *((int *)(&risc_packet.message[0])) = code_data & 0xffff;

                    risc_packet.end = RISC_PKT_END;

                    if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                        RiscFlagSet();    // Set risc1 flag to interrupt risc1            
                    } else 
                        debug("\r\n RemoteControlGpioISR: send packet error!\r\n");
                }
                
                gpio_enter = 0;
                //flag_key_enter = 0;
                
            }
    } else if(flag_key_enter==1){
        if(gpiointr_num==1){  
            if((rRISC_GPIO_POL>>GPIO_REMOTE_CONTROL_BIT)&0x1 ||(pulse_width<LPRESS_1LOW_PULSE_WIDTH_MIN)||(pulse_width>LPRESS_1LOW_PULSE_WIDTH_MAX)){
                    gpiointr_num = 0;
                    flag_key_enter=0;
            }
        } else if(gpiointr_num==2){
            if((pulse_width<LPRESS_11HIGH_PULSE_WIDTH_MIN)||(pulse_width>LPRESS_11HIGH_PULSE_WIDTH_MAX)){ 
                gpiointr_num = 1;
                flag_key_enter=0;
            }
        } else if(gpiointr_num==3){
            if((pulse_width<LPRESS_2LOW_PULSE_WIDTH_MIN)||(pulse_width>LPRESS_2LOW_PULSE_WIDTH_MAX)){ 
                gpiointr_num = 0;
                flag_key_enter=0;
            }
        } else if(gpiointr_num==4){
            if((pulse_width<LPRESS_2HIGH_PULSE_WIDTH_MIN)||(pulse_width>LPRESS_2HIGH_PULSE_WIDTH_MAX)){ 
                gpiointr_num = 0;
                flag_key_enter=0;
            } else{
                gpiointr_num = 0;
                flag_key_enter++;
            }
        }  
    } else if(flag_key_enter>1){
        if(gpiointr_num==1){  
            if((rRISC_GPIO_POL>>GPIO_REMOTE_CONTROL_BIT)&0x1 ||(pulse_width<LPRESS_1LOW_PULSE_WIDTH_MIN)||(pulse_width>LPRESS_1LOW_PULSE_WIDTH_MAX)){
                    gpiointr_num = 0;
                    flag_key_enter=0;
            } else{
                    flag_lpress1 = 1;
            }
        } else if(gpiointr_num==2){
            if((pulse_width<LPRESS_12HIGH_PULSE_WIDTH_MIN)||(pulse_width>LPRESS_12HIGH_PULSE_WIDTH_MAX)){ 
                gpiointr_num = 1;
                flag_key_enter=0;
            }
        } else if(gpiointr_num==3){
            if((pulse_width<LPRESS_2LOW_PULSE_WIDTH_MIN)||(pulse_width>LPRESS_2LOW_PULSE_WIDTH_MAX)){ 
                gpiointr_num = 0;
                flag_key_enter=0;
            }
        } else if(gpiointr_num==4){
            if((pulse_width<LPRESS_2HIGH_PULSE_WIDTH_MIN)||(pulse_width>LPRESS_2HIGH_PULSE_WIDTH_MAX)){ 
                gpiointr_num = 0;
                flag_key_enter=0;
            } else{
                gpiointr_num = 0;
                flag_key_enter++;
                
                if(flag_key_enter==LPRESS_FST_SND_INTERVAL){
                    
                        if(lpress_key<REMOTE_CONTROL_NO_KEY_BIT){

                            flag_lpress2 = 1;
                            printf("IRCode=0x%x\n", lpress_key & 0xffff |0x10000);

                            risc_packet.start = RISC_PKT_START;
                            risc_packet.main_type = tDVR;
                            risc_packet.sub_type = tRemoteControl;
                            risc_packet.length = 0x4;
                            risc_packet.status = 0;

                            *((int *)(&risc_packet.message[0])) = lpress_key & 0xffff |0x10000;

                            risc_packet.end = RISC_PKT_END;

                            if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                                RiscFlagSet();    // Set risc1 flag to interrupt risc1            
                            } else 
                                debug("\r\n RemoteControlGpioISR: send packet error!\r\n");
                        }
                } 
            }
        }  
    }
    if((rRISC_GPIO_POL>>GPIO_REMOTE_CONTROL_BIT)&0x1)
    {                                                                                                    //active low for input data
        set_gpio_int_pol(GPIO_REMOTE_CONTROL_BIT, 1);                           //set active high
    }
    else {                                                                                             //active high for output data           
        set_gpio_int_pol(GPIO_REMOTE_CONTROL_BIT, 0);                       //set active low
    }

    
    
}

/*
***************************************************************************
*                                RemoteControlGpioIntrInit
*
* Description: Enable remote control gpio interrupt
*
* Arguments  : 
*
* Return     : 
*
* Note(s)    : 
***************************************************************************
*/
int RemoteControlGpioIntrInit(void)
{
   
    /* enable gpio intr */

    set_gpio_in(GPIO_REMOTE_CONTROL_BIT);
    set_gpio(GPIO_REMOTE_CONTROL_BIT);

    set_gpio_int_pol(GPIO_REMOTE_CONTROL_BIT, 0);    //set active low
    set_gpio_int_ena(GPIO_REMOTE_CONTROL_BIT);


     
    return SUCCESSFUL;
}

#endif

