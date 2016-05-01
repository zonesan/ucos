/*
***************************************************************************
*                   Copyright (c) 2007 vBridge Microsystem, Inc.  
*                          Unpublished & Not for Publication
*                                 All Rights Reserved                   
*   
* File        :     alarm.c 
*                                                                         
* Description : Receive the remote control pulses and decode                                          *                                                                          
*
* Date        : June 25, 2007                                                           
***************************************************************************
*/



#define ALARM_GLOBALS

#include "video_cfg.h"

#include "includes.h"
#include "alarm.h"
#include "gpio.h"
#include "../drivers/risc.h"

#include "../2risc platform/2risc_memory_map_IPC.h"
#include "../2risc platform/2risc_comm_protocol.h"
#include "../2risc platform/video_api.h"

#include "os_cpu.h"
#include "os_cfg.h"
#include "uCOS_II.h"


/*
***************************************************************************
*                          TaskAlarm
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
void TaskAlarm()
{

    BspStartTicker(OS_TICKS_PER_SEC);

    while(1) {     

                    printf("#");

                    Alarm(1, 0, 0, 0);

                    OSTimeDlyHMSM(0, 0, 2, 0);
                       
    }
	
}

unsigned int alarm_gpio_status = 0;
unsigned int alarm_gpio_attribute= 0;
unsigned int alarm_active_level[32]={0};
unsigned int alarm_type[32]={0};

void Alarm(char enable, char gpio_no, char gpio_level, char attribute)
{

    enable_alarm = enable;
    alarm_gpio_no = gpio_no; 
    alarm_active_level[alarm_gpio_no] = gpio_level;
    alarm_type[alarm_gpio_no] = attribute;
    
    alarm_gpio_isr_conut = 0;
    alarm_pulse_count = 0;
    
    if(enable) {
        AlarmIntrInit();
        debug("\nEnable Alarm!\n");
    } else{
        alarm_gpio_status &= ~(0x1<<alarm_gpio_no);   
        clr_gpio_int_ena(alarm_gpio_no);
        debug("\nDisable Alarm!\n"); 
    }
    
}

void AlarmIntrInit()
{

    RiscGPIOIntrInit();
    
     /* enable gpio intr */
    if(alarm_type[alarm_gpio_no] !=0x1){
        if(alarm_active_level[alarm_gpio_no]) set_gpio_int_pol(alarm_gpio_no, 1);         //set active high    
        else set_gpio_int_pol(alarm_gpio_no, 0);        //set active low       

        alarm_gpio_status |= 0x1<<alarm_gpio_no;     
        set_gpio_int_ena(alarm_gpio_no);      
    } else {
        if(alarm_active_level[alarm_gpio_no]){
            set_gpio_int_pol(alarm_gpio_no, 1);         //set active high   
            if(get_gpio(alarm_gpio_no)==0){
                alarm_gpio_status |= 0x1<<alarm_gpio_no;   
                set_gpio_int_ena(alarm_gpio_no);
            }         
        } else{
            set_gpio_int_pol(alarm_gpio_no, 0);        //set active low 
            if(get_gpio(alarm_gpio_no)){
                alarm_gpio_status |= 0x1<<alarm_gpio_no;  
                set_gpio_int_ena(alarm_gpio_no); 
            }
        }
    }

}


void  AlarmLevelTriggeredGpioISR()
{

    Risc_Packet_t risc_packet;

    risc_packet.start = RISC_PKT_START;
    risc_packet.main_type = tSys;
    risc_packet.sub_type = tAlarmStatus;
    risc_packet.length = 0x4;
    risc_packet.status = 0;

    risc_packet.message[0] = alarm_gpio_isr_no;
    risc_packet.message[1] = 0x1;
    *((short *)(&risc_packet.message[2])) = 0;

    risc_packet.end = RISC_PKT_END;

    if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
        RiscFlagSet();    // Set risc1 flag to interrupt risc1 
        debug("\r\n Alarm0 OK!\r\n");
    } else 
        debug("\r\n AlarmLevelTriggeredGpioISR: send packet error!\r\n");
   
    clr_gpio_int_ena(alarm_gpio_isr_no);
    
}


void  AlarmEdgeTriggeredGpioISR()
{

    Risc_Packet_t risc_packet;

    if((alarm_active_level[alarm_gpio_no]==0)&&((rRISC_GPIO_POL>>alarm_gpio_isr_no)&0x1) 
        || alarm_active_level[alarm_gpio_no]&&(((rRISC_GPIO_POL>>alarm_gpio_isr_no)&0x1)==0) ){
        risc_packet.start = RISC_PKT_START;
        risc_packet.main_type = tSys;
        risc_packet.sub_type = tAlarmStatus;
        risc_packet.length = 0x4;
        risc_packet.status = 0;

        risc_packet.message[0] = alarm_gpio_isr_no;
        risc_packet.message[1] = 0x2;
        *((short *)(&risc_packet.message[2])) = 0;

        risc_packet.end = RISC_PKT_END;

        if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
            RiscFlagSet();    // Set risc1 flag to interrupt risc1 
            debug("\r\n Alarm1 OK!\r\n");
        } else 
            debug("\r\n AlarmLevelTriggeredGpioISR: send packet error!\r\n");
    }

     if((rRISC_GPIO_POL>>alarm_gpio_isr_no)&0x1)
    {                                                                                                    //active low for input data
        set_gpio_int_pol(alarm_gpio_isr_no, 1);         //set active high     
    }
    else {                                                                                             //active high for output data
        set_gpio_int_pol(alarm_gpio_isr_no, 0);        //set active low  
    }
    
    
}



void  AlarmPulseCounterGpioISR()
{

    alarm_gpio_isr_conut++;
   
     if((rRISC_GPIO_POL>>alarm_gpio_isr_no)&0x1)
    {                                                                                                    //active low for input data
        set_gpio_int_pol(alarm_gpio_isr_no, 1);         //set active high     
    }
    else {                                                                                             //active high for output data
        set_gpio_int_pol(alarm_gpio_isr_no, 0);        //set active low  
    }
    
}

void AlarmPulseCounterSendMess()
{

    Risc_Packet_t risc_packet;
    

    risc_packet.start = RISC_PKT_START;
    risc_packet.main_type = tSys;
    risc_packet.sub_type = tAlarmStatus;
    risc_packet.length = 0x4;
    risc_packet.status = 0;
 
    risc_packet.message[0] = alarm_gpio_isr_no;
    risc_packet.message[1] = 0;
    *((short *)(&risc_packet.message[2])) = alarm_pulse_count;

    risc_packet.end = RISC_PKT_END;

    if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
        RiscFlagSet();    // Set risc1 flag to interrupt risc1            
        debug("\r\n Alarm2 OK!\r\n");
    } else 
        debug("\r\n AlarmPulseCounterSendMess: send packet error!\r\n");

}





