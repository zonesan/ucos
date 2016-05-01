#ifndef W681388INIT_H
#define W681388INIT_H

unsigned char w681388RegInit[/*247*/] = {
    //reg0x00 ~ reg0x0f
    0x1,    //reg0 :    A-low, 8bit, 1 bclk pre data bit, positive edge
    0x0,    //          time slot0
    0x0,
    0x0,
    0x0,    //reg4
    0x70,   //          2048k bclk, short frame sync, external frame sync, disable reset
    0x0,
    0x0,    
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    //reg0x10 ~ reg0x1f
    0x0,    //reg10:    Bell 202, disable
    0x0,
    0x0,
    0x1,    //          fsk encoder output level: -23.512dB
    0x0,    //reg14
    0x0,
    0x0,
    0x0,
    0x0,    //reg18
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    //reg0x20 ~ reg0x2f
    0x0,    //reg20:    enable RMC Tx/Rx High pass filter
    0x0,    //          disable all loopback
    0x6,    //          enable DAC/ADC and DC-DC path power on,
    0x0,    //          
    0x0,    //reg24
    0x0,
    0xff,   //          clear all int
    0x0,
    0xff,   //reg28     clear all int
    0x0,
    0x7,    //          clear all int
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    //reg0x30 ~ reg0x3f
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    //reg0x40 ~ reg0x4f
    0x0,    //reg40     AC Path gain 0dB
    0x1b,   //          Metering Pulse Hybrid Balance : +4.08dB, audio Hybrid : +4.08dB 
    0x0,    //          Vcnr 0V
    0x7,    //          Power Alarm Automatic React, Ring Automatic, Loop Closure Detect Automatic
    0x99,   //reg44     Linefeed status: Forward Idel
    0x8,    //          on hook tx bias current: 1mA; Current limit 20ms
    0x0,    
    120,    //          loop closure detect debounce interval 150ms = 120 x 1.25ms
    8,      //reg48     ring trip detect debounce interval 10ms = 8 x 1.25ms
    194,    //          PWM period: 14.10591us = (194 + 1) x 72.338ns
    0x12,   //for 33uH  //0x9 for 68uH  //20,     //          DC-to-DC converter Minimum Off Time 1.4467us = 20 x 72.338ns
    0x0,    //
    32,     //reg4c     on-hook voltage: -48 = 32 x -1.5V
    3,      //          Vcm -4.5V = 3 x -1.5V
    50,     //          high battery voltage: -75V = 50 x -1.5V
    16,     //          low battery voltage: -24V = 16 x -1.5V
    //reg0x50 ~ reg0x5f
    242,    //reg50     LCDC = (1 - 28PI*fLP/800)*2^12
    242,    //          RTDC = (1 - 28PI*fLP/800)*2^12
    0x0,  
    9,      //          ?
    7,      //reg54     ?
    25,
    0x3,   //3,      //          Offset Voltage betwween TIP and RING    4.5V = 3 x 1.5V
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    //reg0x60 ~ reg0x6f     DTMF REGISTERS
    0xf5,   //reg60     deviation: 3.5%; Enable; 0101b
    0x13,   //          sensitivity of the DTMF detector: -24dB
    0x8,
    20,      //          DTMF Tone Present Detect Time: 4ms = 8 x 0.5ms		stone  changed from 8 to 20
    100,    //reg64     DTMF Tone Absent Detect Time: 50ms = 100 x 0.5ms
    100,    //          DTMF Duration Time  50ms = 100 x 0.5ms
    0x0,
    0x40,   //          
    0x0,    //reg68
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    //reg0x70 ~ reg0x7f
    0x0,    //reg70
    0x0,
    0x0,
    0x0,
    0x0,    //reg74
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    //reg0x80 ~ reg0x8f     TIP Voltage Sence
    0x0,    //reg80
    0x0,
    0x0,
    0x0,
    0x0,    //reg84
    0x0,
    0x0,
    0x0,
    0x0,    //reg88
    0x0,
    0x0,
    122,
    163,    //reg8c
    0x0,
    0x0,
    0x0,
    //reg0x90 ~ reg0x9f
    0x0,    //reg90
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    //reg0xa0 ~ reg0xaf
    192,     //rega0
    192,
    192,
    255,
    127,    //rega4
    94,
    13,
    42,
    0x0,    //rega8
    48,
    205,    //          temperature alarm threshold: 125 = 205 - 80
    0x0,
    0x0,    //regac
    0x0,
    0x0,
    0x77,
    //reg0xb0 ~ reg0xbf
    0x97,   //regb0
    0x9,
    0,
    119,
    0,      //regb4
    0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    //reg0xc0 ~ reg0xcf //tone generation register
    0x0,    //regc0
    0x0,
    0x0,
    0x0,
    0,    //regc4
    13,
    0x0,
    0x0,
    108,    //regc8
    126,
    0x0,
    0x0,
    64,
    6,
    0x0,
    0x0,
    //reg0xd0 ~ reg0xdf
    128,
    12,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    68
};
#endif

