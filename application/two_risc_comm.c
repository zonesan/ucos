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


#define   TWO_RISC_COMM_GLOBALS
#include "includes.h"
#include "risc.h"
#include "Sc.h"
#include "Mc.h"
#include "comm.h"
#include "os_cpu.h"
#include "os_cfg.h"
#include "uCOS_II.h"
#include "two_risc_system.h"
#include "two_risc_comm.h"
#include "ucos_task_priority.h"
//=================BY VINCENT 2007.08.31===========================
#ifdef GPIO_TEST
#include "GpioTest.h"
#endif
//=================BY VINCENT 2007.08.31===========================
#include "2risc_memory_map.h"
#include "2risc_comm_protocol.h"
#include "cctl.h"
#include "bsp.h"
#include "timestamp.h"

#include "video_cfg.h"

#ifdef VIDEO_MODULE_INCLUDE
#include "IP_CAM_API.h"
#include "two_risc_video.h"
#include "video_api.h"
#include "VIN_API.h"
#include "VIN_SET.h"
#include "mpeg4_decoder.h"
#include "two_risc_display.h"
#include "VSC_Task.h"
#include "VSC_API.h"
#include "Band_adjust.h"
#include "Motion_Detection.h"
#include "mpeg4_encoder.h"
#include "video_buffer.h"
#ifdef DVR
#include "./video/techwell/tw2835_api.h"
#endif
#include "gpio.h"
#ifdef DVR
#include "./video/techwell/osd.h"
#endif
#include "../application/video/keypad_remote_control/keypad.h"
#include "../application/video/alarm.h"
#include "video/keypad_remote_control/remote_control.h"
#ifdef OSD_ENABLE
#include "two_risc_rollstr.h"
#endif

#ifdef DVR
#include "../application/video/techwell/hw2835_md.h"
#endif

//#include "../application/video/techwell/11.h"

#include "timestamp.h"
#include "startup_picture.h"
//#include "../application/video/techwell/bmpMainMenu.h"




#include "Camera.h"
extern pCamera pcurrent_camera;
#ifdef DISPLAY_INCLUDE
//unsigned int L2UBufCnt;        //wy:not used now
unsigned int dropFrameFlag;
//tVideoLinuxToUcos QVideoL2UPackage[VIDEO_L2U_PKG_NUMBER];  //wy:not used now
extern OS_EVENT *QEeventVodeoL2U;
extern  unsigned int gVoutEnable;
#endif
unsigned int gVideoEnableDisable = 0;    // 1: enable, 0: disable
#ifdef TASK_STATISTIC_EN
#include "two_risc_statistics.h"
#endif

#endif

#ifdef AUDIO_MODULE_INCLUDE
#include "two_risc_audio.h"
#include "jitterBuffer.h"
#include "proslic.h"
#include "two_risc_slic.h"
#include "G726.h"
#include "audio_api.h"
#include "audio_cfg.h"
#include "audio.h"
#include "slic_test.h"
#include "Slic.h"
/* extern  unsigned int gAudioCodecMode[MAX_AUDIO_CHANNEL]; */
#ifdef RISC1_UCOS
#define DEBUG_MIX
#undef DEBUG_MIX

#ifdef DEBUG_MIX
#include "mix_ref.h"
#endif
#endif

#ifdef AUDIO_TEST_ENABLE
#include "test.h"
#endif

#ifdef CODEC_TLV320AIC23B
#include "aic23.h"
#endif

extern chipStruct chipData[CHANNEL_NUM];
extern int reg_msg;
unsigned int AudioVoice = 0;

#endif


unsigned char osdLUT = 0;

#ifdef DVR_BOARD
#define TW2835_MASTER 0x01
#else
#define TW2835_MASTER 0x7e
#endif

/*
***************************************************************************
*                              TaskTwoRiscComm
*
* Description: test 2 Riscs communication. Just read and write streams, 
*              the contents of the streams are blank.
*
* Arguments  : 
*
* Return     : 
*
* Note(s)    : ok on image 59, 6. Risc0 & Risc1 share the same function.
***************************************************************************
*/
void TaskTwoRiscComm(void *data)
{
    int i;    
    unsigned int No_send = 0, No_rec = 0;    
    unsigned int rec[16], send[16];    

    for(i = 0; i < 16; i++){
        send[i] = 0;    
    }
    *(unsigned char *)send = ':';    // the start flag
    
    BspStartTicker(OS_TICKS_PER_SEC);         
    
    rRISC_GPIO_DIR &= ~0x4000000; /* Set GPIO26 output*/
    rRISC_GPIO_DIR &= ~0x2000000; /* Set GPIO25 output*/  
    
    //OSTaskCreate(TaskLed, (void *)0, (void *)&StkTaskLed[TASK_LED_STK_SIZE - 1], 11);

       
    printf("\r\n");
    printf("**********************************************************************\r\n");
    printf("*                 Risc%d Send & Receive Packet Test                   *\r\n", RiscRdID());    
    printf("**********************************************************************\r\n");     
    printf("\r\n");
    
    
    if(RiscRdID() == RISC0)
        TwoRiscCommInit();         
    
    while(1){
        
        if(RiscRdID() == RISC0)     
            LED0_ON();               
        else
            LED1_ON();          
                     
        TwoRiscCommSendPacket(send, RPACKET_LEN);           
        printf("Risc%d: send %d packet!\r\n", RiscRdID(), No_send++);        
        
        if(TwoRiscCommRecPacket1()) {            
            printf("                           Risc%d: receive %d packet!\r\n", RiscRdID(), No_rec++);           
        }
        OSTimeDlyHMSM(0, 0, 1, 0);
        //delay(300, 300);
    }
}

/*
***************************************************************************
*                              TwoRiscCommInit
*
* Description: 
*
* Arguments  : 
*
* Return     : 
*
* Note(s)    : Only Risc0 calls the same function, Risc1 should not call it.
***************************************************************************
*/
int TwoRiscCommInit()
{
    unsigned int sID_rx, sID_tx;
    int sID_rx_addr, sID_tx_addr;    
    static int TwoRiscCommStreamInited = FALSE;    
    
    if(TwoRiscCommStreamInited == FALSE)
        TwoRiscCommStreamInited = TRUE;
    else
        return FAILED;
    
    if(RiscRdID() == RISC0) {
        debug("\r\nRisc0 streams init!\r\n");
        sID_rx = RISC0_RX;
        sID_tx = RISC0_TX;
        
        sID_rx_addr = SOFT_WR_0_S_ADDR;
        sID_tx_addr = SOFT_RD_0_S_ADDR;
        
    } else {
        debug("\r\nRisc1 streams init!\r\n");
        return FAILED;
        // sID_rx = RISC1_RX;
        // sID_tx = RISC1_TX;    
    }
    
    /* Initialize the streams */
    // init sID_rx
    debug("Initialize the stream 0x%x...", sID_rx);
    if(ScInit(sID_rx, sBufUse_Internal, sDBSize_4Byte /*db_size*/, \
              sBufSize_256Byte /*buf_size*/, sSBSize_4Byte /*sb_size*/, sMBSize_4Byte /*mb_size*/, \
              sSwap_no, sLast_Disable, sID_rx_addr)) {
        debug("OK\r\n");
        DebugScContext(sID_rx);debug(" S_CLOSE = %x.\r\n", S_CLOSE);
    } else {
        debug("ERROR\r\n");
        return FAILED;
    }
    
    /* Open sID_rx */
    debug("Open...");
    if(ScOpen(sID_rx)) {
        debug("OK\r\n");
        DebugScContext(sID_rx); debug(" S_EMPTY = %x.\r\n", S_EMPTY);    
    } else {
        debug("ERROR\r\n");    
        return FAILED;
    }    
    
    // init sID_tx
    debug("Initialize the stream 0x%x...", sID_tx);
    if(ScInit(sID_tx, sBufUse_Internal, sDBSize_4Byte /*db_size*/, \
              sBufSize_256Byte /*buf_size*/, sSBSize_4Byte /*sb_size*/, sMBSize_4Byte /*mb_size*/, \
              sSwap_no, sLast_Disable, sID_tx_addr)) {
        debug("OK\r\n");
        DebugScContext(sID_tx); debug(" S_CLOSE = %x.\r\n", S_CLOSE);
    } else {
        debug("ERROR\r\n");
        return FAILED;
    }
    
    /* Open sID_rx */
    debug("Open...");
    if(ScOpen(sID_tx)) {
        debug("OK\r\n");
        DebugScContext(sID_tx); debug(" S_EMPTY = %x.\r\n", S_EMPTY);    
    } else {
        debug("ERROR\r\n");    
        return FAILED;
    }        

#ifdef VIDEO_MODULE_INCLUDE
#ifdef DISPLAY_INCLUDE
//    L2UBufCnt = 0;
    dropFrameFlag = 0;
#endif
#endif
    return SUCCESSFUL;
}

/*
***************************************************************************
*                            TwoRiscCommSendPacket
*
* Description: 
*
* Arguments  : 
*
* Return     : 
*
* Note(s)    : (1). Risc0 & Risc1 share the same function.
*              (2). len is counted by bytes, NOT by words!
***************************************************************************
*/
int TwoRiscCommSendPacket(unsigned int *message, unsigned int len)
{
    OS_CPU_SR  cpu_sr;    
    unsigned int i, sID_tx;
    
    if(len != RPACKET_LEN)   
        return FAILED;
    
    if(RiscRdID() == RISC0) 
        sID_tx = RISC0_TX;
    else 
        sID_tx = RISC1_TX;
    
    OS_ENTER_CRITICAL();    // disable intr to prevent isr or other tasks call TwoRiscCommSendPacket
    if(ScRdBlankCount(sID_tx) < len) {
        OS_EXIT_CRITICAL();    
        return TWO_RISC_COMM_REC_NO_ENOUGH_SPACES; 
    }    
    
    for(i = 0; i < len; i += sizeof(int)) {        
        if(ScWr(sID_tx, *message++) == FAILED) {
            OS_EXIT_CRITICAL();            
            return FAILED;
        }
    }
    
    OS_EXIT_CRITICAL();        
    
    return SUCCESSFUL;
}

/*
***************************************************************************
*                            TwoRiscCommRecPacket
*
* Description: 
*
* Arguments  : 
*
* Return     :     
*
* Note(s)    : Risc0 & Risc1 share the same function.
***************************************************************************
*/
int TwoRiscCommRecPacket(void)
{
    int i;     
    unsigned int sID_rx;    
    unsigned int rec[RPACKET_LEN/sizeof(int)], *msg;    
    
    if(RiscRdID() == RISC0) 
        sID_rx = RISC0_RX;
    else 
        sID_rx = RISC1_RX;   
    
    msg = rec;
    
    while(1) {
        if(ScRdCount(sID_rx) < RPACKET_LEN)         
            return TWO_RISC_COMM_REC_NO_ENOUGH_BYTES;
        if(ScRd(sID_rx, msg) == SUCCESSFUL) {
            if(((*msg) & 0xff) != RISC_PKT_START) {
                debug("\r\nTwoRiscCommRecPacket: start type error!\r\n");
                continue;
            } else {
                break;
            }
        } else {
            return FAILED;    
        }    
    }

    msg++;
    
    /* if the start byte correct, a packet will be accepted */
    for(i = 0; i < (RPACKET_LEN - sizeof(int)); i += sizeof(int)) {        
        if(ScRd(sID_rx, msg++) == FAILED)
            return FAILED;   
    } 
    
    TwoRiscCommProcMsg(rec);
    
    return SUCCESSFUL;        
}

/*
***************************************************************************
*                            TwoRiscCommRecPacket2
*
* Description: 
*
* Arguments  : 
*
* Return     :     
*
* Note(s)    : Risc0 & Risc1 share the same function.
***************************************************************************
*/
int TwoRiscCommRecPacket2(unsigned int *rec)
{
    int i;     
    unsigned int sID_rx;    
    unsigned int *msg;    
    
    if(RiscRdID() == RISC0) 
        sID_rx = RISC0_RX;
    else 
        sID_rx = RISC1_RX;
 
    //if(ScRdCount(sID_rx) < RPACKET_LEN)         
    //    return FAILED;     
    
    msg = rec;
    
    //ScRd(sID_rx, msg);                
    
    /* Judge the start byte */
    //if(((*msg) & 0xff) != RISC_PKT_START) {
    //    debug("\r\nstart type error!\r\n");
    //    return NON_ZERO;
    //}
    while(1) {
        if(ScRdCount(sID_rx) < RPACKET_LEN)         
            return TWO_RISC_COMM_REC_NO_ENOUGH_BYTES;
        if(ScRd(sID_rx, msg) == SUCCESSFUL) {
            if(((*msg) & 0xff) != RISC_PKT_START) {
                debug("\r\nstart type error!\r\n");
                continue;
            } else {
                break;
            }
        } else {
            return FAILED;    
        }    
    }

    msg++;
    
    /* if the start byte correct, a packet will be accepted */
    for(i = 0; i < (RPACKET_LEN - sizeof(int)); i += sizeof(int)) {        
        if(ScRd(sID_rx, msg++) == FAILED)
            return FAILED;   
    } 
    
    TwoRiscCommProcMsg(rec);
    
    return SUCCESSFUL;        
}


/*
***************************************************************************
*                            TwoRiscCommRecPacket1
*
* Description: 
*
* Arguments  : 
*
* Return     :     ZERO:     when the stream does not have enough space
*               NON_ZERO: when a valid message is received; or
*                         the start byte is erroreorus
*
* Note(s)    : Risc0 & Risc1 share the same function. The difference from
*              TwoRiscCommRecPacket is this function just read stream, do 
*              not process the message in the stream.
***************************************************************************
*/
int TwoRiscCommRecPacket1(void)
{
    int i;     
    unsigned int sID_rx;    
    unsigned int rec[RPACKET_LEN/4], *msg;    
    
    if(RiscRdID() == RISC0) 
        sID_rx = RISC0_RX;
    else 
        sID_rx = RISC1_RX;
 
    if(ScRdCount(sID_rx) < RPACKET_LEN)         
        return ZERO;     
    
    msg = rec;
    
    ScRd(sID_rx, msg);                
    
    /* Judge the start byte */
    if(((*msg) & 0xff) != RISC_PKT_START) {
        debug("\r\nstart type error!\r\n");
        return NON_ZERO;
    }

    msg++;
    
    /* if the start byte i correct, a packet will be accepted */
    for(i = 0; i < (RPACKET_LEN -4); i+=4) {        
        ScRd(sID_rx, msg++);   
    } 
    
    //TwoRiscCommProcMsg(rec);
    
    return NON_ZERO;        
}

/*
***************************************************************************
*                            TwoRiscCommProcMsg
*
* Description: 
*
* Arguments  : 
*
* Return     : 
*
* Note(s)    : Risc0 & Risc1 share the same function.
***************************************************************************
*/
int TwoRiscCommProcMsg(unsigned int *msg)
{
    unsigned int main_type, sub_type, length;    

    main_type = ((*msg) & 0xff00) >> 8;
    
    sub_type = ((*msg) & 0xff0000) >> 16;
    
    length = ((*msg) & 0xff000000) >> 24;
    OS_CPU_SR  cpu_sr;    
    
    switch(main_type) {
#ifdef VIDEO_MODULE_INCLUDE            
        case tVideo:
            //OS_ENTER_CRITICAL();               
            TwoRiscCommProcVideo(msg, sub_type, length);
            //OS_EXIT_CRITICAL();            
            break;            
#endif

#ifdef AUDIO_MODULE_INCLUDE
        case tAudio:
            //OS_ENTER_CRITICAL();        
            TwoRiscCommProcAudio(msg, sub_type, length);
            //OS_EXIT_CRITICAL();
            break;
#endif

#ifdef DVR
        case tDVR:
            OS_ENTER_CRITICAL();                    
            TwoRiscCommProcDVR(msg, sub_type, length);
            OS_EXIT_CRITICAL();            
            break;  
#endif

        case tSys:
            TwoRiscCommProcSys(msg, sub_type, length);

            break;  

        default:            
            debug("\r\nTwoRiscCommProcMsg: main type error: 0x%x!\r\n", main_type);
            break;                                                    
    }      
}

/*
***************************************************************************
*                            TwoRiscCommProcSys
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

extern unsigned int gVoutClkSet;

int TwoRiscCommProcSys(unsigned int *msg, unsigned int sub_type, unsigned int len)
{

#ifdef GPIO_TEST
    unsigned char err;        //BY VINCENT
    int GpioPos;
    OS_TCB tcb_data;
 #endif
    unsigned char enable_alarm, gpio_no, gpio_level, alarm_attribute;
    unsigned char alarm_status;
    unsigned short alarm_pulse_counter;

    unsigned int camera_id;
 
    printf("\nS<0x%x|0x%x|0x%x|0x%x|\n", sub_type, *(msg + 2), *(msg + 3), *(msg + 4));

    switch(sub_type) {
        case 0x1:
            switch(*(msg + 2)) {
                case 0:
                    gtPrintMsgControl.en_print_all_msg = 0;
                    break;

                case 1:
                    gtPrintMsgControl.en_print_all_msg = 1;                 
                    gtPrintMsgControl.en_print_periodical_msg = 0;  
                    break;

                case 2:
                    gtPrintMsgControl.en_print_all_msg = 1;
                    gtPrintMsgControl.en_print_periodical_msg = 1;            
                    break;

                case 3:
                    gtPrintMsgControl.en_print_all_msg = 1;
                    gtPrintMsgControl.en_print_periodical_msg = 1;            
                    break;                    

                default:
                    printf("\nTwoRiscCommProcSys: mode error!\n");
                    break;

            }                    
            break;
  //==================BY VINCENT 2007.08.30========================= 
#ifdef GPIO_TEST
            case 0x4:
                 if(OSTaskQuery(PRIOR_TASK_GPIO, &tcb_data) != OS_NO_ERR) {
                    gptQMsgGpioTest= OSQCreate(&ArrayQMsgGpioTest[0], sizeof(ArrayQMsgGpioTest) / sizeof(ArrayQMsgGpioTest[0]));                    
                    if (
                        #ifdef CHIP_CFG_STACK_CHECK
                        OSTaskCreateExt(TaskGpioTest, (void *)0, (void *)&StkTaskGpioTest[sizeof(StkTaskGpioTest) / sizeof(StkTaskGpioTest[0]) - 1], PRIOR_TASK_GPIO,\
                            PRIOR_TASK_GPIO, (void *)&StkTaskGpioTest[0], sizeof(StkTaskGpioTest) / sizeof(StkTaskGpioTest[0]), (void *)0, OS_TASK_OPT_STK_CHK+OS_TASK_OPT_STK_CLR) != OS_NO_ERR	
                        #else
                        OSTaskCreate(TaskGpioTest, (void *)0, (void *)&StkTaskGpioTest[TASK_GPIO_TEST - 1], PRIOR_TASK_GPIO) != OS_NO_ERR
                        #endif    
                        )
                    {
                        debug ("\r\nTaskCreate ****TaskGpioTest***** error\r\n");       
                    }
                 }
                
                GpioPos = (*(msg + 2));    
                err = OSQPost(gptQMsgGpioTest, (void*)GpioPos);
                if(err != OS_NO_ERR)            
                    debug("\r\nRISC0: OSMboxPost error! err = %d!\r\n", err); 
                 break;
#endif
//==================BY VINCENT 2007.08.30=========================  
//==================BY VINCENT 2007.09.11=========================  

            case 0x5:
                GpioPos =  (*(msg + 2));
 #ifdef DVR               
                if((GpioPos&0xffff)>31)
                {
                    break;                    
                }
                else
#endif                    
                {

                    if(GpioPos >> 16)                 
                        set_gpio_in(GpioPos & 0x1f) ;                      
                    else 
                        set_gpio_out(GpioPos & 0x1f) ;     
                }
            break; 
                
            case 0x6:
                GpioPos =  (*(msg + 2));
 #ifdef DVR  
                if((GpioPos&0xffff)>31)
                {
                    gpio_no = GpioPos&0xf;
                    if(gpio_no>3)
                    {   
                        printf("Alarm out port is out of range!!\n");
                        break;
                    }
                    Dvr_AlarmOutWriteTo595(gpio_no,((GpioPos >> 16) &1)); 
                }
                else
#endif
                {
                    if(GpioPos >> 16) 
                        set_gpio(GpioPos & 0x1f);                       
                    else
                        clr_gpio(GpioPos & 0x1f);  
                }
            break;
            
#ifdef VIDEO_MODULE_INCLUDE  
        case tGetCameraID:
            if(RiscRdID() == RISC0) {
                printf("\nTwoRiscCommProcSys: subtype error!\n");
                break;
            }
            
            camera_id = *(msg+2);
            
            printf("\nTwoRiscCommProcSys: tGetCameraID\n");
            printf("\ncamera_id = %d!\n", camera_id );
            break;   

        case tAlarm:
            enable_alarm= *((unsigned char *)msg + 8);
            gpio_no = *((unsigned char *)msg + 9);
            gpio_level = *((unsigned char *)msg + 10);
            alarm_attribute = *((unsigned char *)msg + 11);

            //printf("\nTwoRiscCommProcSys: tAlarm\n");
            //printf("\nenable_alarm = %d!\n", enable_alarm);
            //printf("\ngpio_no = %d!\n", gpio_no);
            //printf("\ngpio_level = %d!\n", gpio_level);
            //printf("\nalarm_attribute = %d!\n", alarm_attribute);
            Alarm(enable_alarm, gpio_no, gpio_level, alarm_attribute);
            break;

        case tAlarmStatus:
            if(RiscRdID() == RISC0) {
                printf("\nTwoRiscCommProcSys: subtype error!\n");
                break;
            }
                
            gpio_no = *((unsigned char *)msg + 8);   
            alarm_status = *((unsigned char *)msg + 9);
            alarm_pulse_counter = *((unsigned short *)msg + 5);

            printf("\nTwoRiscCommProcSys: tAlarmStatus\n");
            printf("\ngpio_no = %d!\n", gpio_no );
            printf("\nalarm_status = %d!\n", alarm_status );
            printf("\nalarm_pulse_counter = %d!\n", alarm_pulse_counter);
            break;   

        case tSetClock:
            if (*(msg+2) == 1) {   //external
                if(gFreqFromCrystal == TRUE) 
                    break;
                gFreqFromCrystal = TRUE;
            } else if(*(msg+2) == 2) {   //internal
                if(gFreqFromCrystal == FALSE) 
                    break;
                gFreqFromCrystal = FALSE;
            } else {
                 printf(" TwoRiscCommProcSys: tSetClock error(%d)!",*(msg+2));
            }

            gVoutClkSet = FALSE;     //now we need to reset vout clock,so reset the flag
            if(OSTaskQuery(PRIOR_TASK_DISPLAY, &tcb_data) == OS_NO_ERR) {
                if(display_no > 0) {       //if sc interrupt has been occured, so we can set gVoutreEnable to 1 to reset vsc
                    gVoutReEnable = 1;
                    gVoutClkSet = TRUE;     //when ipcam task initialize cctl vout clock before sc interrupt occurs, we prevent it from initializing clock
                }else {                          //if sc interrupt has not occured, we reset vsc
                    gVoutEnable = 0;
                    err = OSQPost(gptQMsgTwoRiscSystemMonitor, (void *)TWO_RISC_SYSTEM_OPEN_VOUT);
                    if(err != OS_NO_ERR)
                        debug("\r\nRISC0: OSMboxPost error! err = %d!\r\n", err);
                }
            }
            
            if((err = OSQPost(gptQMsgTwoRiscSystemMonitor, (void *)TWO_RISC_SYSTEM_SET_CLOCK)) != OS_NO_ERR)
                debug("\r\nRISC0: OSMboxPost error! err = %d!\r\n", err);
            
            break;

#endif
//==================BY VINCENT 2007.09.11========================= 
         default:
            printf("\nTwoRiscCommProcSys: subtype error!\n");            
            break;
    }
}

#ifdef DVR
/*
***************************************************************************
*                            TwoRiscCommProcDVR
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

extern unsigned char cmn_dvc;
int TwoRiscCommProcDVR(unsigned int *msg, unsigned int sub_type, unsigned int len)
{
    unsigned short start_x, start_y, end_x, end_y;
    unsigned short path;
    unsigned int color;
    unsigned int width;
    unsigned int height;
    unsigned int attribute;
    unsigned char page;

    unsigned char input_mode;    
    unsigned char output_mode;
    unsigned short channel;
    unsigned short pos_control;

    unsigned int bitmap_addr, bmpdata_len;
    unsigned int bmpLUT_addr;
    unsigned short index_len;
    unsigned char bmp_attribute;
    unsigned short audio_attribute;
    
    unsigned int string_addr, string_length;

    unsigned char key;
    unsigned int statusled, buzzer;
    unsigned short alarm_status, alarm_pulse_counter;

    unsigned char enable_alarm, gpio_no, gpio_level, alarm_gpio_type;
    unsigned char contrast;
    unsigned int i;

    unsigned int *pCount = (unsigned int *)(COMM_COUNTBUF_B_ADDR + (tDVR -1 ) * 0x100 + ( sub_type -1 ));
    unsigned short clr_width, clr_heigth;
    unsigned char pb_outchannel, pb_inchannel;
    unsigned char md_lvsens, md_speedsens, md_tmpsens, md_spsens ;
    unsigned short MDChannel, MDAreaFlag;
    unsigned short display_page;
    unsigned char cam_attribute, cam_level;
    TW2835_2DBox  TwoDBoxDisplayTemp;
    TW2835Clr   ClrScreeParaTemp ;
    TW2835_MotionDetector  MotionDetectorParameterTemp;
    TW2835Clr * ClrScreePara ;
    TW2835_2DBox * TwoDBoxDisplay;
    TW2835_MotionDetector * MotionDetectorParameter ;
    TW2835_Line * tDrawLineParameter;
    TW2835_FillColor * tFillColorParameter;
    TW2835_WriteString * t2835WriteString;
    unsigned char enable,chan[4];
    unsigned short sensitivity;
    
    ClrScreePara = &ClrScreeParaTemp;
    TwoDBoxDisplay = &TwoDBoxDisplayTemp;
    MotionDetectorParameter = &MotionDetectorParameterTemp;

        printc("\nD<0x%x|0x%x\n", sub_type, *(msg + 2));
    
        printc("msg: ");
        for(i = 0; i < 4; i++) {
            printc("0x%x, ", *(msg + i));
        }
        printc("\n\n");

    switch(sub_type) {
        case tInit2835:
            input_mode = *(unsigned char *)(msg + 2);
            output_mode = *((unsigned char *)(msg + 2) + 1);
            channel = *((unsigned short *)(msg + 2) + 1);
            path = *(msg + 3);
            if(path == 0) path = 2;        
            printf("\nTwoRiscCommProcDVR: tInit2835!\n");
            printf("\nInput mode %d, output mode %d, channel %d!\n", input_mode, output_mode, channel);
            if(path == 1) printf("\npath: display!\n"); //PTH_X
            else if(path == 2) printf("\npath: record!\n");
            else printf("\npath is err!\n");            
            if(input_mode > 2) 
            {
                debug("\ntInit2835 Input mode err!%d\n",input_mode);
                break;
            }
            if(output_mode > 3) 
            {
                debug("\ntInit2835 Output mode err!%d\n",output_mode);
                break;
            }
            HwTW2835Init(path /*U8 _pth*/, input_mode /*U8 VinModeControl*/, output_mode /*U8 VoutModeControl*/, channel /*U8 VinCH*/);

            //RemoteControlInit();

            #ifdef CHIP_CFG_STACK_CHECK
            OSTaskCreateExt(TaskKeypad, (void *)0, (void *)&global_keypad_stack[sizeof(global_keypad_stack) / sizeof(global_keypad_stack[0]) - 1], PRIO_TASK_KEYPAD,\
                    PRIO_TASK_KEYPAD, (void *)&global_keypad_stack[0], sizeof(global_keypad_stack) / sizeof(global_keypad_stack[0]), (void *)0, OS_TASK_OPT_STK_CHK+OS_TASK_OPT_STK_CLR);	
             OSTaskCreateExt(TaskRemoteControl, (void *)0, (void *)&global_control_stack[sizeof(global_control_stack) / sizeof(global_control_stack[0]) - 1], PRIO_TASK_REMOTECTL,\
                    PRIO_TASK_REMOTECTL, (void *)&global_control_stack[0], sizeof(global_control_stack) / sizeof(global_control_stack[0]), (void *)0, OS_TASK_OPT_STK_CHK+OS_TASK_OPT_STK_CLR);	             
            #else            
            OSTaskCreate(TaskKeypad, (void *)0, (void *)&global_keypad_stack[STACK_SIZE_KEYPAD - 1], PRIO_TASK_KEYPAD);    
            OSTaskCreate(TaskRemoteControl, (void *)0, (void *)&global_control_stack[STACK_SIZE_REMOTE_CONTROL - 1], PRIO_TASK_REMOTECTL);	
 
            #endif
            
            break;

        case tDrawLine:
            tDrawLineParameter->start_pos_x = *(unsigned short *)(msg + 2);
            tDrawLineParameter->start_pos_y = *((unsigned short *)(msg + 2) + 1);
            tDrawLineParameter->end_pos_x = *((unsigned short *)(msg + 3) + 0);
            tDrawLineParameter->end_pos_y = *((unsigned short *)(msg + 3) + 1);
            tDrawLineParameter->attribute = *(msg + 4);
            path = ((tDrawLineParameter->attribute) & 0x08)>>3;
            if(path == 0) path = 2;              
            printf("\nTwoRiscCommProcDVR: tDrawLine\n");
            printf("\nstart_x = %d, start_y = %d, end_x = %d, end_y = %d!\n", tDrawLineParameter->start_pos_x, tDrawLineParameter->start_pos_y, tDrawLineParameter->end_pos_x, tDrawLineParameter->end_pos_y);
            printf("\nattribute = 0x%8x!\n", tDrawLineParameter->attribute);
            if(path == 1) printf("\npath: display!\n"); //PTH_X
            else if(path == 2) printf("\npath: record!\n");
            else printf("\npath is err!\n"); 
            HwTW2835CreateLn(TW2835_MASTER /*U8 _dvc*/, path /*U8 _pth*/, tDrawLineParameter /* TW2835_Line * tDrawLineParameter*/ );

            break;

        case tFillColor:
            tFillColorParameter->start_x = *(unsigned short *)(msg + 2);
            tFillColorParameter->start_y = *((unsigned short *)(msg + 2) + 1);
            tFillColorParameter->heigth = *((unsigned short *)(msg + 2) + 2);
            tFillColorParameter->width = *((unsigned short *)(msg + 2) + 3);
            tFillColorParameter->attribute = *(msg + 4);
            path = ((tFillColorParameter->attribute) & 0x08)>>3;
            if(path == 0) path = 2;                
            printf("\nTwoRiscCommProcDVR: tFillColor\n");
            printf("\nstart_x = %d, start_y = %d, heigth = %d, width = %d!\n", tFillColorParameter->start_x, tFillColorParameter->start_y, tFillColorParameter->heigth, tFillColorParameter->width);
            printf("\nattribute = 0x%8x!\n", tFillColorParameter->attribute);
            if(path == 1) printf("\npath: display!\n"); //PTH_X
            else if(path == 2) printf("\npath: record!\n");
            else printf("\npath is err!\n");            
            HwTW2835FillColor(TW2835_MASTER /*U8 _dvc*/, path /*U8 _pth*/, tFillColorParameter /*TW2835_FillColor * tFillColorParameter*/);

           break;

        case tFillBitmap:
            input_mode = *(unsigned char *)(msg + 2);
            output_mode = *((unsigned char *)(msg + 2) + 1);
            channel = *((unsigned short *)(msg + 2) + 1);
            path = *(msg + 3);
            if(path == 0) path = 2;               
            printf("\nTwoRiscCommProcDVR: tFillBitmap!\n");
            printf("\nInput mode %d, output mode %d, channel %d!\n", input_mode, output_mode, channel);
            if(path == 1) printf("\npath: display!\n"); //PTH_X
            else if(path == 2) printf("\npath: record!\n");
            else printf("\npath is err!\n");            
            //HwTW2835Init_CZH(path /*U8 _pth*/, input_mode /*U8 VinModeControl*/, output_mode /*U8 VoutModeControl*/, channel /*U8 VinCH*/);

	        
#if 0
ClrScreePara->page = 0;      
ClrScreePara->start_pos_x = 0;    
    
ClrScreePara->start_pos_y = 0;
	        
    
ClrScreePara->width = 704;	        
    
ClrScreePara->higth = 576;
	        
    
	        
    
HwTW2835Clr(TW2835_MASTER/*U8 _dvc*/, 1 /*U8 _pth*/, ClrScreePara/*TW2835Clr * ClrScreePara*/ );	        
    
	        
    
HwTW2835SetOSDColLUT(TW2835_MASTER , 1, ct_11);	        
    
	        
    
HwTW2835FillOSDBMP(TW2835_MASTER , 1 , 40 , 20,  gt_11_tst_24bit_64col_01 ,  ct_11 , 0);
#endif
            break;

        case tWriteString: 
            
            t2835WriteString->str_addr = (*(msg + 2) | 0xa0000000) ;
            t2835WriteString->str_len = *(msg + 3);
            t2835WriteString->start_x = *(unsigned short *)(msg + 4);
            t2835WriteString->start_y = *((unsigned short *)(msg + 4) + 1);
            t2835WriteString->attribute = *(msg + 5);
            path = ((t2835WriteString->attribute) & 0x08)>>3;
            if(path == 0) path = 2;  
          //  printf("\nTwoRiscCommProcDVR: tWriteString\n");
          //  printf("\nstring_addr = 0x%x, string_length = %d!\n", t2835WriteString->str_addr, t2835WriteString->str_len);
          //  printf("\nstart_x = %d, start_y = %d!\n", t2835WriteString->start_x, t2835WriteString->start_y);
           // printf("\nattribute = 0x%x!\n", t2835WriteString->attribute);
           // if(path == 1) printf("\npath: display!\n"); //PTH_X
           // else if(path == 2) printf("\npath: record!\n");
            //else printf("\npath is err!\n"); 
            HwTW2835WriteOSDStr(TW2835_MASTER /*U8 _dvc*/, path /*U8 _pth*/, t2835WriteString /*TW2835_WriteString * t2835WriteString*/);

            RiscChgCnt(SEM_DVR /*int SemID*/, tDVR /*int main_type*/, sub_type /*int sub_type*/, -1 /*int direction*/);            

            break;

       case tSetOutputMode:
            output_mode = *((unsigned char *)msg + 8);
            channel = *((unsigned char *)msg + 9);
            path =  *((unsigned short *)msg + 5);               
            printf("\nTwoRiscCommProcDVR: tSetOutputMode\n");
            printf("\noutput_mode = %d!\n", output_mode);
            printf("\nchannel = %d!\n", channel);
            if(output_mode > 3) 
            {
                debug("\tSetOutputMode Output mode err!%d\n",output_mode);
                break;
            }
            switch(path)
            {
                case 0:                      
                    printf("\n display path\n");
                    HwTw2835SetOutputMode_X(output_mode/*U8 Ch_ModeCon*/,channel /*U8 Ch_lv_Num*/);
                    break;
                case 1:                        
                    printf("\nrecord path\n");
                    HwTw2835SetOutputMode_Y(output_mode/*U8 Ch_ModeCon*/,channel /*U8 Ch_lv_Num*/);
                    break;
                case 2:                       
                    printf("\n playback path\n");  
                    HwTw2835SetOutputMode_PB(output_mode/*U8 Ch_ModeCon*/,channel /*U8 Ch_lv_Num*/);
                    break;
                case 3:
                    printf("\n enable display channel %d\n",channel);
                    HwEnableDisplayChannel(channel);
                    break;
                case 4:
                    printf("\n disable display channel %d\n",channel);
                    HwDisableDisplayChannel(channel);
                    break;
                case 5:
                    printf("\n enable Record channel %d\n",channel);
                    HwEnableRecordChannel(channel);
                    break;
                case 6:
                    printf("\n disable Record channel %d\n",channel);
                    HwDisableRecordChannel(channel);
                    break;
                default :                      
                    printf("\n set output mode  path error\n"); 
                    break;                
            }
            break;
            
        case tRemoteControl:
            if(RiscRdID() == RISC0) {
                printf("\nTwoRiscCommProcDVR: subtype error!\n");
                break;
            }
                
            key = *((unsigned char *)msg + 8);   
            printf("\nTwoRiscCommProcDVR: tRemoteControl\n");
            printf("\nkey = 0x%x!\n", key);           
            break;     

        case tFillBMP64:
            start_x = *(unsigned short *)(msg + 2);
            start_y = *((unsigned short *)(msg + 2) + 1);
            bitmap_addr = *(msg + 3);
            bmpdata_len = *(msg + 4);
            bmpLUT_addr = *(msg + 5);
            index_len = *(unsigned short *)(msg + 6);
            bmp_attribute = *((unsigned char *)(msg + 6) + 2);
            path = *((unsigned char *)(msg + 6) + 3);     
            if(path == 0) path = 2;
            //printf("\nTwoRiscCommProcDVR: tFillBMP64\n");
           // printf("\nbitmap_addr = 0x%x!\n", bitmap_addr);
            //printf("\bmpLUT_addr = 0x%x!\n", bmpLUT_addr);
           // printf("\nstart_x = %d, start_y = %d, bmp_attribute = %d!\n", start_x, start_y, bmp_attribute);
           // if(path == 1) printf("\npath: display!\n"); //PTH_X
            //else if(path == 2) printf("\npath: record!\n");
            //else printf("\npath is err! path = %d\n", path); 
            HwTW2835FillOSDBMP(TW2835_MASTER /*U8 _dvc*/, path /*U8 _pth*/, start_x /*U16 _osd_pos_x*/, start_y /*U16 _osd_pos_y*/,  (unsigned int *)(bitmap_addr  | 0xa0000000) /*U32 *_bmp_ptr*/, (unsigned int *)( bmpLUT_addr  | 0xa0000000 )/*U32 *BMPIndex*/, bmp_attribute/*U8 attribute*/);
            RiscChgCnt(SEM_DVR /*int SemID*/, tDVR /*int main_type*/, sub_type /*int sub_type*/, -1 /*int direction*/);            
            
            break;   

        case tStatusLed:
            statusled = *((unsigned short *)msg + 4);
            printf("\nTwoRiscCommProcDVR: StatusLed\n");
            printf("\nstatusled = %d!\n", statusled);           
            StatusLed(statusled);
            break;

        case tBuzzer:
            buzzer = *((unsigned char *)msg + 8); 
            printf("\nTwoRiscCommProcDVR: tBuzzer\n");
            printf("\nbuzzer = %d!\n", buzzer);            
            Buzzer(buzzer);
            break;
/*            
        case tAlarm:
            enable_alarm= *((unsigned char *)msg + 8);
            gpio_no = *((unsigned char *)msg + 9);
            gpio_level = *((unsigned char *)msg + 10);
            alarm_gpio_type = *((unsigned char *)msg + 11);

            printf("\nTwoRiscCommProcDVR: tAlarm\n");
            printf("\nenable_alarm = %d!\n", enable_alarm);
            printf("\ngpio_no = %d!\n", gpio_no);
            printf("\ngpio_level = %d!\n", gpio_level);
            printf("\nalarm_gpio_type = %d!\n", alarm_gpio_type);
            Alarm(enable_alarm, gpio_no, gpio_level, alarm_gpio_type);
            break;

        case tAlarmStatus:
            if(RiscRdID() == RISC0) {
                printf("\nTwoRiscCommProcDVR: subtype error!\n");
                break;
            }
                
            alarm_status = *((unsigned short *)msg + 4);
            alarm_pulse_counter = *((unsigned short *)msg + 5);

            printf("\nTwoRiscCommProcDVR: tAlarmStatus\n");
            printf("\nalarm_status = %d!\n", alarm_status );
            printf("\nalarm_pulse_counter = %d!\n", alarm_pulse_counter);
            break;   
*/
        case tClrScreen:
            start_x = *(unsigned short *)(msg + 2);
            start_y = *((unsigned short *)(msg + 2) + 1);
            clr_heigth = *(unsigned short *)(msg + 3);
            clr_width = *((unsigned short *)(msg + 3) + 1);
            path = *((unsigned short *)(msg + 4) + 1);
            if(path == 0) path = 2;            
            ClrScreePara->page = *((unsigned short *)(msg + 4) + 0); 
            //printf("\nTwoRiscCommProcDVR: tClrScreen\n");
            //printf("\nclr_width = 0x%x!", clr_width);
            //printf("\nbclr_heigth = 0x%x!", clr_heigth);
            //printf("\nstart_x = %d, start_y = %d!", start_x, start_y);
            //printf("\npage = %d!", ClrScreePara->page);
            //if(path == 1) printf("\npath: display!\n"); //PTH_X
            //else if(path == 2) printf("\npath: record!\n");
            //else printf("\npath is err!\n"); 
            ClrScreePara->start_pos_x = start_x;
            ClrScreePara->start_pos_y = start_y;
            ClrScreePara->width = clr_width;
            ClrScreePara->higth = clr_heigth;
            
            HwTW2835Clr(TW2835_MASTER/*U8 _dvc*/, path /*U8 _pth*/, ClrScreePara/*TW2835Clr * ClrScreePara*/ );

            break; 
        case tKeypad:
            if(RiscRdID() == RISC0) {
                printf("\nTwoRiscCommProcDVR: subtype error!\n");
                break;
            }
                
            key = *((unsigned char *)msg + 8);
            printf("\nTwoRiscCommProcDVR: tkeypad\n");
            printf("\nkey = %d!\n", key);          
            break;   

        case tMotionDetector:

            channel = *((unsigned short *)(msg + 2) + 0);            
            MotionDetectorParameter->md_lvsens = *((unsigned char *)(msg + 2) + 2);
            MotionDetectorParameter->md_speedsens = *((unsigned char *)(msg + 2) + 3);
            MotionDetectorParameter->md_tmpsens = *((unsigned char *)(msg + 3) + 0);
            MotionDetectorParameter->md_spsens = *((unsigned char *)(msg + 3) + 1);            
            path = *((unsigned short *)(msg + 3) + 1);
            if(path == 0) path = 2;
            MotionDetectorParameter->md_AreaVectorAddr = (*(msg + 4)) | 0xa0000000;      
            if((MotionDetectorParameter->md_lvsens==0) && (MotionDetectorParameter->md_speedsens==0) && (MotionDetectorParameter->md_tmpsens==0) && (MotionDetectorParameter->md_spsens==0))
            {
                printf("\nTwoRiscCommProcDVR: Shut Off tMotionDetector\n");
            }
            else
            {
	            printf("\nTwoRiscCommProcDVR: tMotionDetector\n");
	            printf("\nchannel = 0x%x!", channel);
                   if(MotionDetectorParameter->md_lvsens>31)//level sens (5 bits)no more than 32 
                       printf("\nmd level sensitivity ERR  0x%x",MotionDetectorParameter->md_lvsens);
                   else
                       printf("\nmd_lvsens = 0x%x!", MotionDetectorParameter->md_lvsens);
                   if(MotionDetectorParameter->md_speedsens>63)//speed sens (6 bits)no more than 64 
                       printf("\nmd speed sensitivity ERR  0x%x",MotionDetectorParameter->md_speedsens);
                   else
                       printf("\nmd_speedsens = 0x%x!", MotionDetectorParameter->md_speedsens);
                   if(MotionDetectorParameter->md_tmpsens > 15)//temporal sens (4 bits)no more than 16 
                       printf("\nmd temporal sensitivity ERR  0x%x",MotionDetectorParameter->md_tmpsens);
                   else
                       printf("\nmd_tmpsens = 0x%x!", MotionDetectorParameter->md_tmpsens);
                   if(MotionDetectorParameter->md_spsens > 15)//spatial sens (4 bits)no more than 16 
                       printf("\nmd spatial sensitivity ERR  0x%x",MotionDetectorParameter->md_spsens);
                   else
                       printf("\nmd_spsens = 0x%x!", MotionDetectorParameter->md_spsens);
                   
	            printf("\nmd_AreaVectorAddr = 0x%x!", MotionDetectorParameter->md_AreaVectorAddr);
            }
            if(path == 1) printf("\npath: display!\n"); //PTH_X
            else if(path == 2) printf("\npath: record!\n");
            else printf("\npath is err! path=%d\n",path); 
            HwTW2835Motiondetector(TW2835_MASTER/*U8 _dvc*/, path/*U8 _pth*/, channel/*U8 _ch*/, MotionDetectorParameter/*TW2835_MotionDetector * MotionDetectorParameter*/  );
            RiscChgCnt(SEM_DVR /*int SemID*/, tDVR /*int main_type*/, sub_type /*int sub_type*/, -1 /*int direction*/);  

            break; 
        
        case tTransmitMotionState:
   
            if(RiscRdID() == RISC0) {
                printf("\nTwoRiscCommProcDVR: subtype error!\n");
                break;
            }
                
            MDChannel = *((unsigned short *)(msg + 2));
            MDAreaFlag = *((unsigned short *)(msg + 2)+1);  
            printf("\nTwoRiscCommProcDVR: tTransmitMotionState\n");
            printf("\nMDChannel = %x!\n", MDChannel);
            printf("\nMDAreaFlag = %x!\n", MDAreaFlag);
            printf("\nAreaReg: %2x %2x %2x %2x \n", *((unsigned char *)(msg + 3)+0), *((unsigned char *)(msg + 3)+1), *((unsigned char *)(msg + 3)+2), *((unsigned char *)(msg + 3)+3));
            printf("\nAreaReg: %2x %2x %2x %2x \n", *((unsigned char *)(msg + 4)+0), *((unsigned char *)(msg + 4)+1), *((unsigned char *)(msg + 4)+2), *((unsigned char *)(msg + 4)+3));
            printf("\nAreaReg: %2x %2x %2x %2x \n", *((unsigned char *)(msg + 5)+0), *((unsigned char *)(msg + 5)+1), *((unsigned char *)(msg + 5)+2), *((unsigned char *)(msg + 5)+3));
            break; 
  
        case tPlayback:
            output_mode = *((unsigned char *)(msg + 2) + 0);
            pb_outchannel= *((unsigned char *)(msg + 2) + 1);
            pb_inchannel= *((unsigned char *)(msg + 2) + 2);
            path = *((unsigned char *)(msg + 2) + 3) ; 
            attribute = (path & 0x7f); 
            if(attribute == 0) attribute = 2;  
            printf("\nTwoRiscCommProcDVR: tPlayback\n");
            printf("\noutput_mode = 0x%x!\n", output_mode);
            printf("\npb_outchannel = %d, pb_inchannel = %d!\n", pb_outchannel, pb_inchannel);
            printf("\npath:%d\n",attribute); //PTH_X
            printf("\nplayback enable:%d\n",path); //PTH_X
            if(output_mode > 1) 
            {
                debug("\tSetOutputMode Output mode err!%d\n",output_mode);
                break;
            }
            HwTW2835Playback(TW2835_MASTER /*U8 _dvc*/, (unsigned char)path /*U8 _pth*/, output_mode/*U8 PB_Mode*/, pb_outchannel/*U8 PB_OutChannel*/, pb_inchannel/*U8 PB_InChannel*/);

            break;  
            
        case tSetColourLUT:
            bmpLUT_addr = *(msg + 2) ;
            path = *((unsigned short *)(msg + 3));     
            if(path == 0) path = 2;
           // printf("\nTwoRiscCommProcDVR: tSetColourLUT\n");
          //  printf("\nbmpLUT_addr = 0x%x!\n", bmpLUT_addr);
           // if(path == 1) printf("\npath: display!\n"); //PTH_X
           // else if(path == 2) printf("\npath: record!\n");
           // else printf("\npath is err!\n"); 
            HwTW2835SetOSDColLUT(TW2835_MASTER /*U8 _dvc*/, (unsigned char)path /*U8 _pth*/, (unsigned char *)(bmpLUT_addr | 0xa0000000)/*U8  *_tbl_osd_clr*/);
            RiscChgCnt(SEM_DVR /*int SemID*/, tDVR /*int main_type*/, sub_type /*int sub_type*/, -1 /*int direction*/);

            break;  

        case tChangeOSDShowPage:
            display_page = *((unsigned short *)(msg + 2) + 0);
            if(!(display_page&0xff00))
                break;
            else
                display_page &= 0xff; 
            path = *((unsigned short *)(msg + 2) + 1);     
            if(path == 0) path = 2;
            
            if(display_page > 5) 
            {
                debug("\tChangeOSDShowPage display page err!%d\n",display_page);
                break;
            }    
          //  printf("\nTwoRiscCommProcDVR: tChangeOSDShowPage\n");
           // printf("\ndisplay_page = 0x%x!\n", display_page);
           // if(path == 1) printf("\npath: display!\n"); //PTH_X
           // else if(path == 2) printf("\npath: record!\n");
           // else printf("\npath is err!\n"); 
            HwTW2835BitmapPageChange(TW2835_MASTER /*U8 _dvc*/, (unsigned char)path /*U8 _pth*/, display_page/*U8 page_attributr*/);

            break;  //tAdjustCamera

        case tAdjustCamera:
            channel = *((unsigned char *)(msg + 2) + 0);
            cam_attribute = *((unsigned char *)(msg + 2) + 1);
            cam_level = *((unsigned char *)(msg + 2) + 2);
            path = *((unsigned char *)(msg + 2) + 3);    
            if(path == 0) path = 2;              
            printf("\nTwoRiscCommProcDVR: tAdjustCamera\n");
            printf("\nchannel = 0x%x!\n", channel);
            printf("\ncam_attribute = 0x%x; cam_level = 0x%x!\n", cam_attribute, cam_level);
            if(path == 1) printf("\npath: display!\n"); //PTH_X
            else if(path == 2) printf("\npath: record!\n");
            else printf("\npath is err!\n"); 
            switch(cam_attribute) {
                case 0:/*	no	*/
                    printf("\nDVR@ message: Sorry,  the function is not done!\n"); 
                    break;

                case 1:/*	contrast	*/
                    HwTW2835AdjustContrast(TW2835_MASTER/*U8 _dvc*/, path/*U8 _pth*/, channel/*U8 _ch*/, cam_level/*U8 HueRange*/);           
                    break;

                case 2:/*	effect	*/
                    printf("\nDVR@ message: Sorry,  the function is not done!\n");           
                    break;

                case 3:/*	exposure	*/
                    printf("\nDVR@ message: Sorry,  the function is not done!\n");            
                    break; 
                    
                case 4:/*	saturation	*/
                    HwTW2835AdjustSaturation(TW2835_MASTER/*U8 _dvc*/, path/*U8 _pth*/, channel/*U8 _ch*/, cam_level/*U8 HueRange*/);
                    
                    break;

                case 5:/*	brightness	*/
                    HwTW2835AdjustBrightness(TW2835_MASTER/*U8 _dvc*/, path/*U8 _pth*/, channel/*U8 _ch*/, cam_level/*U8 HueRange*/);
                             
                    break;

                case 6:/*	hue	*/
                    HwTW2835AdjustHue(TW2835_MASTER/*U8 _dvc*/, path/*U8 _pth*/, channel/*U8 _ch*/, cam_level/*U8 HueRange*/);
                             
                    break;

                case 7:/*	sharpness	*/
                    printf("\nDVR@ message: Sorry,  the function is not done!\n");           
                    break;
                default:
                    printf("\n cam_attribute error!\n");
                    break;

            } 
            break;   

        case t2DBoxDisplay:

            channel = *((unsigned short *)(msg + 2) + 0); 
            TwoDBoxDisplay->TDBox_HL = *((unsigned short *)(msg + 2) + 1);
            TwoDBoxDisplay->TDBox_VT = *((unsigned short *)(msg + 3) + 0);
            TwoDBoxDisplay->TDBox_VW = *((unsigned short *)(msg + 3) + 1);
            TwoDBoxDisplay->TDBox_HW = *((unsigned short *)(msg + 4) + 0);
            TwoDBoxDisplay->TDBox_NUM = *((unsigned char *)(msg + 4) + 2);
            TwoDBoxDisplay->TDBox_PlaneCol = (*((unsigned char *)(msg + 4) + 3));
            TwoDBoxDisplay->TDBox_Attribute = *((unsigned short *)(msg + 5) + 0);            
            path = *((unsigned short *)(msg + 5) + 1);
            if(path == 0) path = 2;
            //printf("\nTwoRiscCommProcDVR: t2DBoxDisplay\n");
           //printf("\nchannel = 0x%x!\n", channel);
           // printf("\nTDBox_HL = 0x%x, TDBox_VT = 0x%x!\n", TwoDBoxDisplay->TDBox_HL, TwoDBoxDisplay->TDBox_VT);
           // printf("\nTDBox_VW = 0x%x, TDBox_HW = 0x%x!\n", TwoDBoxDisplay->TDBox_VW, TwoDBoxDisplay->TDBox_HW);
           //printf("\nTDBox_NUM = 0x%x, TDBox_PlaneCol = 0x%x!\n", TwoDBoxDisplay->TDBox_NUM, TwoDBoxDisplay->TDBox_PlaneCol);
           // printf("\nTDBox_Attribute = 0x%x!\n", TwoDBoxDisplay->TDBox_Attribute);
           // if(path == 1) printf("\npath: display!\n"); //PTH_X
           // else if(path == 2) printf("\npath: record!\n");
           // else printf("\npath is err!\n");    
           // if(TwoDBoxDisplay->TDBox_Attribute &0x80)
           //     HwTW2835_SingleBoxDisplay(TW2835_MASTER /*U8 _dvc*/, (unsigned char)path /*U8 _pth*/, channel/*U8 _ch*/, TwoDBoxDisplay/*TW2835_2DBox * TwoDBoxDisplay */ );
            //else
                HwTW2835_2DBoxDisplay(TW2835_MASTER /*U8 _dvc*/, (unsigned char)path /*U8 _pth*/, channel/*U8 _ch*/, TwoDBoxDisplay/*TW2835_2DBox * TwoDBoxDisplay */ );
           
            break;
            
        case tSetAudioGain:
            channel = *((unsigned short*)(msg + 2) +0); 
            audio_attribute =  *((unsigned short*)(msg + 2) +1) ;             
            path = (audio_attribute >> 15);
            if(path == 0) path = 2;
            printf("\nTwoRiscCommProcDVR: tSetAudioGain\n");
            printf("\nchannel = 0x%x!\n", channel);
            printf("\tSetAudioGain attribute = 0x%x,\n", audio_attribute);
            if(path == 1) printf("\npath: display!\n"); //PTH_X
            else if(path == 2) printf("\npath: record!\n");
            else printf("\npath is err!\n");          
            AppTW2835Audio_Gain(TW2835_MASTER, path, channel, audio_attribute);            
            break;    
            
        case tSetAudioLive:
            channel = *(unsigned short*)(msg + 2) ; 
            attribute =  *(msg + 3) ;             
            path = (*(msg + 3) >>31);
            if(path == 0) path = 2;  
            printf("\nTwoRiscCommProcDVR: tSetAudioLive\n");
            printf("\nchannel = 0x%x!\n", channel);
            printf("\ntSetAudioLive attribute = 0x%x,\n", attribute);
            if(path == 1) printf("\npath: display!\n"); //PTH_X
            else if(path == 2) printf("\npath: record!\n");
            else printf("\npath is err!\n");         
            AppTW2835Audio_Live(TW2835_MASTER, path, channel, attribute);            
            break;
            
         case tSetAudioRecord:
            channel = *((unsigned short *)(msg + 2) + 0); 
            audio_attribute = *((unsigned short *)(msg + 2) + 1);       
            path = (audio_attribute >>15);
            if(path == 0) path = 2;  
            printf("\nTwoRiscCommProcDVR: tSetAudioRecord\n");
            printf("\nchannel = 0x%x!\n", channel);
            printf("\ntSetAudioRecord attribute1 = 0x%x,\n", audio_attribute);
            printf("\ntSetAudioRecord attribute2 = 0x%x 0x%x,\n", *(msg + 3),*(msg + 4));
            if(path == 1) printf("\npath: display!\n"); //PTH_X
            else if(path == 2) printf("\npath: record!\n");
            else printf("\npath is err!\n");         
            AppTW2835_Record(TW2835_MASTER, path, channel, audio_attribute,(msg + 3) );          
            break;      
      
        case tEnableVideoLossDetect :
            channel = *((unsigned short *)(msg + 2) + 0);
            enable= *((unsigned short *)(msg + 2) + 1);
            debug("\nchannel %d enable %d\n",channel,enable);
            HwEnableVideoLossDetect(channel, enable);
            break;
            
        case tEnableVideoBlindDetect :
            channel = *((unsigned char *)(msg + 2) + 0);
            enable= *((unsigned char *)(msg + 2) + 1);
            sensitivity = *((unsigned short *)(msg + 2) + 1);
            printf("\nchannel %d enable %d sensitivity %x\n",channel,enable,sensitivity);
            HwEnableVideoBlindDetect(cmn_dvc,  channel, enable,sensitivity);
            break;
            
        case tEnableVideoNightDetect :
            channel = *((unsigned char *)(msg + 2) + 0);
            enable= *((unsigned char *)(msg + 2) + 1);
            sensitivity = *((unsigned short *)(msg + 2) + 1);
            debug("\nchannel %d enable %d sensitivity %x\n",channel,enable,sensitivity);
            HwEnableVideoNightDetect(cmn_dvc,  channel, enable,sensitivity);
            break;
            
        case   tGetVideoLossBlindNightStatus:           
           chan[0] = *((unsigned char *)(msg + 2) + 0);
           chan[1]= *((unsigned char *)(msg + 2) + 1);
           chan[2] = *((unsigned char *)(msg + 2) + 2);
           chan[3] = *((unsigned char *)(msg + 2) + 3);
            debug("\nch0:0x%x \nch1:0x%x\n ch2:0x%x \nch3:0x%x\n",chan[0],chan[1],chan[2],chan[3]);
            break;
            
        case   tSetBGContrast:           
            contrast =  *((unsigned char *)(msg + 2) + 0);     
            debug("\n set bg contrast %d \n",contrast);
            if(contrast<1||contrast>3)
            {
                printf("\n  tSetBGContrast err %d \n",contrast);
                break;
            }
            HwSetBGContrast(contrast);
            break;   
            
        default:    
            printf("\nTwoRiscCommProcDVR: subtype error!\n");
            break;
    }
}
#endif

/*
***************************************************************************
*                            TwoRiscCommProcVideo
*
* Description: 
*
* Arguments  : 
*
* Return     : 
*
* Note(s)    : Risc0 & Risc1 share the same function.
***************************************************************************
*/
#ifdef VIDEO_MODULE_INCLUDE

unsigned int linuxframenum = 0;
unsigned int JpegFlag = 1;
unsigned int JpegNumCnt = 0;
extern unsigned int gVinOddEvenFrameSelect  ;  //BY VINCENT 2007.09.21
extern unsigned int gVscOddEvenFrameSelect ;  //BY VINCENT 2007.09.21
//extern unsigned int gVoutClkSet;
tYUVFrame YUVLinuxToUcos;

extern unsigned int gVinOpenTimes;
extern unsigned int gDecodeDeblocking;
extern unsigned int gCamAutodetectEnable;
int TwoRiscCommProcVideo(unsigned int *msg, unsigned int sub_type, unsigned int len)
{
    int mode;
#ifdef DISPLAY_INCLUDE
    VIDEO_FRAME frameFromeLinux;
#endif
    static Risc_Packet_t pkt = {0};
    pkt.start = RISC_PKT_START;
    pkt.end = RISC_PKT_END;
    pkt.main_type = tVideo;
    int value;
    char err = 0;
    
    BYTE camReg;
    BYTE regVal;

    int qVal;
    int maxQVal;
    int minQVal;

    static unsigned int count;    
    unsigned int t1;  

    OS_TCB tcb_data;   
    
    OS_CPU_SR cpu_sr;
    unsigned int VideoModeTmp; 
    unsigned int i;    
    unsigned int *tmpjpeg;    

    short int tmp;

    int flag_scale_changed; 
    int flag_position_changed = 0;
    short frame_rate;
    short frame_rate_mode;    

    extern int gCameraSelectDigital;
//    tVideoLinuxToUcos *pVideoPackage;    //wy:not used now

    gtClearScreenSetting tClearScreenParameters;
    gtDrawLineSetting tDrawLineParameters;
    gtFillColorSetting tFillColorParameters;
    gtWriteStringSetting tWriteStringParameters;

    unsigned short start_x, start_y, end_x, end_y;
    unsigned short display_page;
    unsigned int index_addr;
    unsigned char rollstr_page;

    if((sub_type != tVideoTransferLinuxToUcos) && (sub_type != tVideoTransferUcosToLinux)) {
        if(RiscRdID() == RISC0) 
            printf("\nV<0x%x|0x%x|0x%x|0x%x|\n", sub_type, *(msg + 2), *(msg + 3), *(msg + 4));
    }

    switch(sub_type) {
        case tVideoEnableDisable:
            if(*(msg+2) == 1) {    // enable video            

                //StayFrame = WaitPressKey() - '0';
                StayFrame = 8;
                
                switch(*(msg+3)) {
                    case 0x1://VIN
                        debug("UCOS OPEN VIN\n");                                                                     
                        err = OSQPost(gptQMsgTwoRiscSystemMonitor, (void *)TWO_RISC_SYSTEM_OPEN_VIN);
                        if(err != OS_NO_ERR)            
                            debug("\r\nRISC0: OSMboxPost error! err = %d!\r\n", err);                
                         //OSTaskCreate(TaskTwoRiscVinMpegEncode, (void *)0, (void *)&StkTaskTwoRisc0IPCamera[TaskTwoRiscMpegEncode_STK_SIZE - 1], PRIOR_TASK_IPCAM);  
                        break;
                     
                    case 0x2://VOUT
                        gVideoEnableDisable = 1;
                        debug("UCOS OPEN VOUT %d\n", *(msg+3));
                        err = OSQPost(gptQMsgTwoRiscSystemMonitor, (void *)TWO_RISC_SYSTEM_OPEN_VOUT);
                        if(err != OS_NO_ERR)            
                            debug("\r\nRISC0: OSMboxPost error! err = %d!\r\n", err);                
                        break;

#ifdef DISPLAY_INCLUDE                    
                    default:    
                        if(*(msg+3)& 0x4) {    //OPEN VSC                    
                            if(*(msg+3)& 0x8) {    //open PIP                        
                                printf("UCOS OPEN PIP\n");
                                //gTwoRiscSystemMonitorMsg = TWO_RISC_SYSTEM_OPEN_PIP;
                                //err = OSQPost(gptQMsgTwoRiscSystemMonitor, &gTwoRiscSystemMonitorMsg);
                                err = OSQPost(gptQMsgTwoRiscSystemMonitor, (void *)TWO_RISC_SYSTEM_OPEN_PIP);
                                if(err != OS_NO_ERR)            
                                    debug("\r\nRISC0: OSMboxPost error! err = %d!\r\n", err);    
                            }
                    
                            if(*(msg+3)& 0x10) {    //open RVO    
                                printf("UCOS OPEN RVO\n");       
                                err = OSQPost(gptQMsgTwoRiscSystemMonitor, (void *)TWO_RISC_SYSTEM_OPEN_RVO);
                                if(err != OS_NO_ERR)            
                                    debug("\r\nRISC0: OSMboxPost error! err = %d!\r\n", err);                                 
                            }
                    
                            if(*(msg+3)& 0x20) {    //open OSD
                                printf("UCOS OPEN OSD\n");                                            
                                err = OSQPost(gptQMsgTwoRiscSystemMonitor, (void *)TWO_RISC_SYSTEM_OPEN_OSD);
                                if(err != OS_NO_ERR)            
                                    debug("\r\nRISC0: OSMboxPost error! err = %d!\r\n", err);    
                            }                        
                        }                
                        break;
#else
                    default:
                        break;
#endif
                }   // end of switch(*(msg+3)) 

            }     else if(*(msg+2) == 0) {    // disable video   
               
                switch(*(msg+3)) {
                    case 0x1://VIN
                        debug("UCOS CLOSE VIN\n");
                        err = OSQPost(gptQMsgTwoRiscSystemMonitor, (void *)TWO_RISC_SYSTEM_DEL_VIN_TASK);            
                        if(err != OS_NO_ERR)            
                            debug("\r\nRISC0: OSMboxPost error! err = %d!\r\n", err);
                        break;
                        
                    case 0x2://VOUT
                        gVideoEnableDisable = 0;
                        debug("\nUCOS CLOSE VOUT! Start Picture Will Occur!\n");
                    
                        //err = OSQPost(gptQMsgTwoRiscSystemMonitor, (void *)TWO_RISC_SYSTEM_CLOSE_VOUT);            
                        err = OSQPost(gptQMsgTwoRiscSystemMonitor, (void *)TWO_RISC_SYSTEM_CLOSE_DISPLAY_TASK);            
                        if(err != OS_NO_ERR)            
                            debug("\r\nRISC0: OSMboxPost error! err = %d!\r\n", err);

                        /* backup the gtRvoCurrent */
                        //memcpy((void *)&gtRvoCurrent1, (void *)&gtRvoCurrent, sizeof(gtRvoCurrent));
                        gtRvoCurrent1.interlaced = gtRvoCurrent.interlaced;
#ifdef STARTUP_PICTURE
                        TwoRiscDisplayStartupPicInit();
#endif

                        err = OSQPost(gptQMsgTwoRiscSystemMonitor, (void *)TWO_RISC_SYSTEM_OPEN_VOUT);            
                        if(err != OS_NO_ERR)            
                            debug("\r\nRISC0: OSMboxPost error! err = %d!\r\n", err);                                   

                        break;                    

#ifdef DISPLAY_INCLUDE                                  
                    default:                                                                
                        if(*(msg+3)&0x4) {    //close VSC
                            //break;
                            if(*(msg+3)&0x8) {     //close PIP
                                
                                printf("UCOS CLOSE PIP\n");
                                //gTwoRiscSystemMonitorMsg = TWO_RISC_SYSTEM_CLOSE_PIP;
                                //err = OSQPost(gptQMsgTwoRiscSystemMonitor, &gTwoRiscSystemMonitorMsg);
                                err = OSQPost(gptQMsgTwoRiscSystemMonitor, (void *)TWO_RISC_SYSTEM_CLOSE_PIP);
                                if(err != OS_NO_ERR)            
                                    debug("\r\nRISC0: OSMboxPost error! err = %d!\r\n", err);
                            }
                            
                            if(*(msg+3)&0x10) {    //close RVO
                                //break;
                                printf("UCOS CLOSE RVO\n");       
                                err = OSQPost(gptQMsgTwoRiscSystemMonitor, (void *)TWO_RISC_SYSTEM_CLOSE_RVO);
                                if(err != OS_NO_ERR)            
                                    debug("\r\nRISC0: OSMboxPost error! err = %d!\r\n", err);                                
                            }
                            
                            if(*(msg+3)&0x20) {    //close OSD
                                printf("UCOS CLOSE OSD\n");                                            
                                err = OSQPost(gptQMsgTwoRiscSystemMonitor, (void *)TWO_RISC_SYSTEM_CLOSE_OSD);
                                if(err != OS_NO_ERR)            
                                    debug("\r\nRISC0: OSMboxPost error! err = %d!\r\n", err);
                            }                        
                        }
                        break;
#else
                    default:
                        break;
#endif
                }  // end of switch(*(msg+3))          
            }     // end of if(*(msg+2) == 0) {    // disable video    
                    
            break;

        case tSetCamAutodetect:
            if (gCamAutodetectEnable != *(msg + 2)) {
                gCamAutodetectEnable = *(msg + 2);

                /*clear the pcurrent_camera if disable automatic detection*/
                if (0 == gCamAutodetectEnable) {
                    pcurrent_camera = NULL;
                }
                
                if(OSTaskQuery(PRIOR_TASK_IPCAM, &tcb_data) == OS_NO_ERR)   // IPCAM task is created?
                {
                    err = OSQPost(gptQMsgTwoRiscSystemMonitor, (void *)TWO_RISC_SYSTEM_REOPEN_VIN_TASK);                            
                    if(err != OS_NO_ERR)            
                        debug("\nTwoRiscCommProcVideo: OSQPost error! err = %d!\n", err);                
                }                                  
            } else
                printf("TwoRiscCommProcVideo:tSetCamAutodetect state is the same as the old one!\n");
            break;
        
        case tSetVideoSize:
            mode = *(msg + 2);
            if(mode == Mode_Vin) {
                uint16_t width = *(uint16_t *)(msg + 3);
                uint16_t height = *((uint16_t *)(msg + 3) + 1);
                printf("TwoRiscCommProcVideo: tSetVideoSize: width = %d, height = %d!\n", width, height);
                err = SetVideoWH(width, height);
                if(err == -1) {    // new size is not valid?
                    printf("\nNew Vin Frame Size Invalid!\n");
                    break;
                } else if(err == 1) {    // new size is not changed?
                    printf("\nNew Vin Frame Size is the same as the old ones!\n");
                    break;
                }

                /* new size is valid */
                printf("\nNew Vin Frame Size is Valid!\n");
                
                /* video size changed, the ipcam task should be restarted! */
                if(OSTaskQuery(PRIOR_TASK_IPCAM, &tcb_data) == OS_NO_ERR)   // IPCAM task is created?
                {
                    err = OSQPost(gptQMsgTwoRiscSystemMonitor, (void *)TWO_RISC_SYSTEM_REOPEN_VIN_TASK);                            
                    if(err != OS_NO_ERR)            
                        debug("\nTwoRiscCommProcVideo: OSQPost error! err = %d!\n", err);                
                }                                  
            } else if (mode == 4) { //if video output is LCD
                uint16_t width = *(uint16_t *)(msg + 3);
                uint16_t height = *((uint16_t *)(msg + 3) + 1);
                printf("TwoRiscCommProcVideo:LCD output tSetVideoSize: width = %d, height = %d!\n", width, height);

                if (width <= MAX_VIN_WIDTH && height <= MAX_VIN_HEIGHT)
                    printf("\nNew output size of LCD Valid!\n");
                else {
                    printf("\nNew output size of LCD Invalid\n");
                    break;
                }
                
                if ((gtDisplayFormat.width != width) || (gtDisplayFormat.height != height)) {
                    gtDisplayFormat.width = width;
                    gtDisplayFormat.height = height;
                } else {
                    printf("\nNew output size of LCD is the same as the old one!\n");
                    break;
                }

                if(gVoutEnable == 1) {
                    gVoutReEnable = 1;
                }                    
            }else
                printf("\nTwoRiscCommProcVideo:tSetVideoSize error\n");
            break;
        case tSetDecodeDeblocking:
            if(*(msg + 2) == gDecodeDeblocking) { 
                printf("TwoRiscCommProcVideo:decode deblocking is the same as the old one\n");
            } else {
                gDecodeDeblocking = *(msg + 2);
            }
            break;
        
        case tGetVideoSize:
            pkt.sub_type = tGetVideoSize;
            pkt.length = 0xc;
            uint16_t width = GetVideoWidth();
            uint16_t height = GetVideoHeight();
            *(size_t *)(pkt.message) = Mode_Vin;
            uint16_t *data = (uint16_t *)((size_t *)pkt.message + 1);
            *data = width;
            *(data + 1) = height;
            if(TwoRiscCommSendPacket((size_t *)&pkt, RPACKET_LEN)==SUCCESSFUL)
                ;
            else
                debug("Dispose tGetVideoSize error int risc0");
            break;

/*        case tSetClockSource:
            if (*(msg+2) == 1) {   //external
                if(gFreqFromCrystal == TRUE) 
                    break;
                gFreqFromCrystal = TRUE;
            } else if(*(msg+2) == 2) {   //internal
                if(gFreqFromCrystal == FALSE) 
                    break;
                gFreqFromCrystal = FALSE;
            } else {
                 printf(" TwoRiscCommProcVideo: tSetClockSource error(%d)!",*(msg+2));
            }

            gVoutClkSet = FALSE;     //now we need to reset vout clock,so reset the flag
            if(OSTaskQuery(PRIOR_TASK_DISPLAY, &tcb_data) == OS_NO_ERR) {
                if(display_no > 0) {       //if sc interrupt has been occured, so we can set gVoutreEnable to 1 to reset vsc
                    gVoutReEnable = 1;
                    gVoutClkSet = TRUE;     //when ipcam task initialize cctl vout clock before sc interrupt occurs, we prevent it from initializing clock
                }else {                          //if sc interrupt has not occured, we reset vsc
                    gVoutEnable = 0;
                    err = OSQPost(gptQMsgTwoRiscSystemMonitor, (void *)TWO_RISC_SYSTEM_OPEN_VOUT);
                    if(err != OS_NO_ERR)
                        debug("\r\nRISC0: OSMboxPost error! err = %d!\r\n", err);
                }
            }
            if(OSTaskQuery(PRIOR_TASK_IPCAM, &tcb_data) == OS_NO_ERR) {
                err = OSQPost(gptQMsgTwoRiscSystemMonitor, (void *)TWO_RISC_SYSTEM_REOPEN_VIN_TASK);            
                if(err != OS_NO_ERR)
                    debug("\r\nRISC0: OSMboxPost error! err = %d!\r\n", err);
            }
            
            break;
*/
        case tSetChecksumEnableDisable:    //this api should be called before vin is opened on the first time.
/*
            if((OSTaskQuery(PRIOR_TASK_IPCAM, &tcb_data) == OS_NO_ERR) || (gVideoEnableDisable == TRUE))
            {
                printf("vin or vout has been opened, now cann't enable/disable checksum!\n");
                break;
            }
*/
            if(gVinOpenTimes > 0) {
                printf("now can't set checksum enable/disable!\n");
                break;
            }
 
            if(*(msg + 2) == TRUE) {
                printf("linux set checksum enabled!\n");
            }else if(*(msg + 2) == FALSE){
                printf("linux set checksum disabled!\n");
            }else {
                printf("linux set checksum error=%d!\n", *(msg + 2));
                break;
            }
            gChecksumEnabled = *(msg + 2);
            
            break;

        /* not supported after autodetection added
        case tSetCameraSelect:
            if(*(msg+2) == 1) {
                 gCameraSelectDigital = TRUE;
            } else if (*(msg+2) == 2){
                 gCameraSelectDigital = FALSE;
            } else {
                 printf(" TwoRiscCommProcVideo: tSetCameraSelect error(%d)!",*(msg+2));
            }
            break;
        */
           
        case tSetFrameRate:
//            short frame_rate;
//            short frame_rate_mode;

            frame_rate = *((short *)(msg + 2) + 1);
            frame_rate_mode = *(short *)(msg + 2);
            
            if(frame_rate_mode == 0x1) {  // camera framerate
                if(frame_rate == gFrameRate) {
                    printf("\nTwoRiscCommProcVideo tSetFrameRate: camera framerate is the same as the old one!\n");
                    break;
                } else {
                    if((frame_rate >= 1) && (frame_rate <= 30)) {   // valid frame rate?
                        printf("\nTwoRiscCommProcVideo tSetFrameRate: new camera framerate(%d) is valid!\n", frame_rate);
                        gFrameRate = frame_rate;
                        if(OSTaskQuery(PRIOR_TASK_IPCAM, &tcb_data) == OS_NO_ERR) {  // IPCAM task is created?            
                            err = OSQPost(gptQMsgTwoRiscSystemMonitor, (void *)TWO_RISC_SYSTEM_REOPEN_VIN_TASK);                            
                                if(err != OS_NO_ERR)            
                                    debug("\r\nRISC0: OSMboxPost error! err = %d!\r\n", err);   
/*                                                            
                            if(pcurrent_camera != NULL)
                                if(pcurrent_camera->SetFrameRate)                               
                                    pcurrent_camera->SetFrameRate(gFrameRate);   
*/                                    
                        }              
                    } else {
                        printf("\nTwoRiscCommProcVideo tSetFrameRate: new camera framerate(%d) is NOT valid!\n", frame_rate);                    
                    }
                }
            } else if(frame_rate_mode == 0x2) {    // software skip frame
                gSkipFrame = frame_rate;
                printf("\nTwoRiscCommProcVideo tSetFrameRate:  gSkipFrame = %d!\n", frame_rate);                
            } else {
                printf("\nTwoRiscCommProcVideo tSetFrameRate:  mode is NOT valid!\n");                
            }

            break;
/*            
            value = *(msg + 2);
            if((value & 0xffff) == 0x1)    // set Vin frame rate            
            {
                if (((( value & 0xffff0000 ) >> 16 ) <= 30 ) && ((( value & 0xffff0000 ) >> 16) >= 1 ))                        
                {
                    gFrameRate = (( value & 0xffff0000 ) >> 16 );                
                    printf("TwoRiscCommProcVideo tSetFrameRate framerate = %d\n", (( value & 0xfff0000 ) >> 16) );                        
                } else {
                    debug ("framerate isn't between [1,30]\r\n");
                    break;
                }
            } else if((value & 0xffff) == 0x2) {   // set skip frame            
                gSkipFrame = (value & 0xffff0000) >> 16;
                printf("TwoRiscCommProcVideo tSetFrameRate gSkipFrame = %d\n", gSkipFrame);                        
            }
            
            //xxxxxxx to do : reconut the timestemp;
            ReCountTimeStampMaxDiff();
            
            if(OSTaskQuery(PRIOR_TASK_IPCAM, &tcb_data) == OS_NO_ERR)   // IPCAM task is created?
            {
                err = OSQPost(gptQMsgTwoRiscSystemMonitor, (void *)TWO_RISC_SYSTEM_REOPEN_VIN_TASK);                            
                if(err != OS_NO_ERR)            
                    debug("\r\nRISC0: OSMboxPost error! err = %d!\r\n", err);                
            }            
            break;
*/            
    
        case tGetFrameRate:
            pkt.sub_type = tGetFrameRate;
            pkt.length = 4;
            value = GetFrameRate();
            *(int *)(pkt.message) = value;
            TwoRiscCommSendPacket((size_t *)&pkt, RPACKET_LEN);
            break;
        
        case tSetIntraFrameRate:
            value = *(msg + 2);

            if(value)
                debug("\nTwoRiscCommProcVideo: tSetIntraFrameRate ok(%d)!\n", value);
            else {
                debug("\nTwoRiscCommProcVideo: tSetIntraFrameRate error! Should not be zero!\n");
                break;
            }
            
            gIntraFrameRate = value;
            
            //SetIntraFrameRate(value); 
            break;
        
        case tGetIntraFrameRate:
            pkt.sub_type = tGetIntraFrameRate;
            pkt.length = 4;
            value = GetIntraFrameRate();
            debug("in UCOS tGetIntraFreshRate,GetIntraFrameRate()==%d\n",value);
            *(int *)(pkt.message) = value;
            TwoRiscCommSendPacket((size_t *)&pkt, RPACKET_LEN);
            break;

        case tSetVideoCodec:
            VideoModeTmp = *(msg + 2);
            if( VMode != VideoModeTmp){
                switch (VideoModeTmp) {
                    case MPEG4MODE:
                        printf ("\nTwoRiscCommProcVideo tSetVideoCodec: MPEG4MODE!\n");
                        VMode = MPEG4MODE;
                        gEncodeMode = MPEG_ENCODE;
                        printf("set MPEG4MODE mode ~~~~~~\n");
                        
                        if(OSTaskQuery(PRIOR_TASK_IPCAM, &tcb_data) == OS_NO_ERR)
                        {
                            err = OSQPost(gptQMsgTwoRiscSystemMonitor, (void *)TWO_RISC_SYSTEM_REOPEN_VIN_TASK);                            
                            if(err != OS_NO_ERR)            
                                debug("\r\nRISC0: OSMboxPost error! err = %d!\r\n", err);                
                        }                    

                        break;

                    case MJPEGMODE:
                        printf ("\nTwoRiscCommProcVideo tSetVideoCodec: MJPEGMODE!\n");
                        VMode = MJPEGMODE;
                        printf("set MJPEGMODE mode ~~~~~~\n");
                        
                        if(OSTaskQuery(PRIOR_TASK_IPCAM, &tcb_data) == OS_NO_ERR)
                        {
                            err = OSQPost(gptQMsgTwoRiscSystemMonitor, (void *)TWO_RISC_SYSTEM_REOPEN_VIN_TASK);                            
                            if(err != OS_NO_ERR)            
                                debug("\r\nRISC0: OSMboxPost error! err = %d!\r\n", err);                
                        }                    

                        break;

                    case MPEG_JPEG_MODE:
                        printf ("\nTwoRiscCommProcVideo tSetVideoCodec: MPEG_JPEG_MODE!\n");
                        VMode = MPEG_JPEG_MODE;
                        gEncodeMode = MPEG_ENCODE;
                        printf("set MPEG_JPEG_MODE mode ~~~~~~\n");
                        
                        if(OSTaskQuery(PRIOR_TASK_IPCAM, &tcb_data) == OS_NO_ERR)
                        {
                            err = OSQPost(gptQMsgTwoRiscSystemMonitor, (void *)TWO_RISC_SYSTEM_REOPEN_VIN_TASK);                            
                            if(err != OS_NO_ERR)            
                                debug("\r\nRISC0: OSMboxPost error! err = %d!\r\n", err);                
                        }                    

                        break;

                    case H263_MODE:
                        printf ("\nTwoRiscCommProcVideo tSetVideoCodec: H263_MODE!\n");
                        VMode = H263_MODE;
                        gEncodeMode = H263_ENCODE;
                        printf("set H263 mode ``````\n");

                        if(OSTaskQuery(PRIOR_TASK_IPCAM, &tcb_data) == OS_NO_ERR)
                        {
                            err = OSQPost(gptQMsgTwoRiscSystemMonitor, (void *)TWO_RISC_SYSTEM_REOPEN_VIN_TASK);                            
                            if(err != OS_NO_ERR)            
                                debug("\r\nRISC0: OSMboxPost error! err = %d!\r\n", err);                
                        }                    

                        break;

                    default:
                        printf ("wrong video mode");
                        break;
                }
            }else
                printf("\r\nRISC0: The video mode is the same as the old one!\r\n");

            break;
        
        case tSetVideoQuality:  // gb: set the Q and the range(just for CBR). Maybe should have some protection!

            qVal = (int)( *((short *)(msg + 2)));
            maxQVal = (int)(*(((char *)(msg + 2))+2));
            minQVal = (int)(*(((char *)(msg + 2))+3));
            printf("\nTwoRiscCommProcVideo tSetVideoQuality Q: %d, max: %d, min: %d!\n", qVal, maxQVal, minQVal);

            SetVideoQuality(qVal); 
#ifdef BAND_WIDTH_CONTROL
            set_qp_range(maxQVal, minQVal );
#endif
              
            break;
        
        case tGetVideoQuality:
            value = GetVideoQuality();
            pkt.sub_type = tGetVideoQuality;
            pkt.length = 4;
            *(int *)(pkt.message) = value;
            TwoRiscCommSendPacket((size_t *)&pkt, RPACKET_LEN);
            break;
            
        case tSetNTSCPal:
            t1= *((unsigned short *)(msg + 2));
            tmp=*((unsigned short *)(msg + 2) + 1);

            /* not supported after autodetection added!
            if(t1 ==1) {    // video input    
                if(gCameraSelectDigital == FALSE) {
                    if(gtVinFormat.camera_format == tmp)
                        break;
                    else {
                        gtVinFormat.camera_format = tmp;
                        if(tmp == NTSC) {
                            gtVinFormat.width = 704;
                            gtVinFormat.height = 480;
                        }
                        else {
                            gtVinFormat.width = 704;
                            gtVinFormat.height = 576;
                        }
                          
                        SetVideoWH(gtVinFormat.width, gtVinFormat.height);
                        if(OSTaskQuery(PRIOR_TASK_IPCAM, &tcb_data) == OS_NO_ERR)   // IPCAM task is created?
                        {
                            err = OSQPost(gptQMsgTwoRiscSystemMonitor, (void *)TWO_RISC_SYSTEM_REOPEN_VIN_TASK);                            
                            if(err != OS_NO_ERR)            
                                debug("\r\nRISC0: OSMboxPost error! err = %d!\r\n", err);                
                        }
                    }
                } else {
                    printf("\nTwoRiscCommProcVideo: tSetNTSCPal not fit for digital camera!\n");
                }
                break;
            }
            */

#ifdef DISPLAY_INCLUDE
            if(t1 == 2) {
                if(gtDisplayFormat.vout_format == tmp)
                    break;
                else {  
                    if(tmp > LCD)
                        break;
                    gtDisplayFormat.vout_format = tmp;

                    /*The default LCD output size is 480X240*/
                    if (gtDisplayFormat.vout_format == LCD) {
                        gtDisplayFormat.width = 480;
                        gtDisplayFormat.height =240;
                    }

                    if(gVoutEnable == 1) {
                        gVoutReEnable = 1;
                    }                    
                } 
                break;
            }
#endif

#ifdef DVR 
            if(t1 ==1) 
            {    
                if(tmp == NTSC) {
                    gtVinFormat.width = 704;
                    gtVinFormat.height = 480;
                }
                else {
                    gtVinFormat.width = 704;
                    gtVinFormat.height = 576;
                }
                  
                SetVideoWH(gtVinFormat.width, gtVinFormat.height);
                if(OSTaskQuery(PRIOR_TASK_IPCAM, &tcb_data) == OS_NO_ERR)   // IPCAM task is created?
                {
                    err = OSQPost(gptQMsgTwoRiscSystemMonitor, (void *)TWO_RISC_SYSTEM_REOPEN_VIN_TASK);                            
                    if(err != OS_NO_ERR)            
                        debug("\r\nRISC0: OSMboxPost error! err = %d!\r\n", err);                
                }    
            }

#else
            printf("\nTwoRiscCommProcVideo: tSetNTSCPal error(%d)!\n", t1);
#endif
            break;  
            
        case tSetMotionDetectionEnabled:                 //receive api:0--enable,1--disable.
#ifdef MV_ENABLE
            value=*(msg+2);                            
            if((value&0xF)==1)      
            {
                flag_mv_enable = 1;
                printf("\nTwoRiscCommProcVideo tSetMotionDetectionEnabled: Enable Mv\n");//enable mv
            }
                    //EnableMotionDetect();
            else if((value&0xF)==0)//disable mv
            {
                flag_mv_enable = 0;
                printf("\nTwoRiscCommProcVideo tSetMotionDetectionEnabled:  Disable Mv\n");
            }           //DisableMotionDetect()
            if(OSTaskQuery(PRIOR_TASK_IPCAM, &tcb_data) == OS_NO_ERR)   // IPCAM task is created?
            {
                err = OSQPost(gptQMsgTwoRiscSystemMonitor, (void *)TWO_RISC_SYSTEM_REOPEN_VIN_TASK);                            
                if(err != OS_NO_ERR)            
                    debug("\r\nRISC0: OSMboxPost error! err = %d!\r\n", err);                
            }  
#endif
            break;
            
        case tSetCamReg:
            printf("\nTwoRiscCommProcVideo: tSetCamReg\n");
		if(pcurrent_camera != NULL)
                if(pcurrent_camera->SetCamRegister != NULL)
                    pcurrent_camera->SetCamRegister(*(msg + 2) /*camReg*/, *(msg + 3) /*regVal*/);
                else
                    printf("\nTwoRiscCommProcVideo: no SetCamRegister() found!\n");
                
		else
            		printf("\nTwoRiscCommProcVideo: no digit camera found!\n");
            break;

        case tSetCamera:
            printf("\nTwoRiscCommProcVideo: tSetCamera\n");
            short level;
            level = *((short *)(msg + 2) + 1);

            if(gtCameraSetting.inited == 0) {    // if not inited yet
                printf("TwoRiscCommProcVideo tSetCamera: set camera value by linux!");
                gtCameraSetting.inited = 1;
            }

            switch(*(short *)(msg + 2)) {
                case 1:     // contrast
                    if(gtCameraSetting.contrast != level) {
                        gtCameraSetting.contrast = level;

                        if(OSTaskQuery(PRIOR_TASK_IPCAM, &tcb_data) == OS_NO_ERR) {  // IPCAM task is created?
                            if(pcurrent_camera != NULL)
                                if(pcurrent_camera->camera_set_contrast)
                                    pcurrent_camera->camera_set_contrast();              
                        }                        
                    }
                    break;

                case 2:     // effect
                    if(gtCameraSetting.effect != level) {
                        gtCameraSetting.effect = level;

                        if(OSTaskQuery(PRIOR_TASK_IPCAM, &tcb_data) == OS_NO_ERR) {  // IPCAM task is created?
                            if(pcurrent_camera != NULL)
                                if(pcurrent_camera->camera_set_effect)
                                    pcurrent_camera->camera_set_effect();              
                        }                        
                    }
                    break;

                case 3:     // exposure
                    if(gtCameraSetting.exposure != level) {
                        gtCameraSetting.exposure = level;

                        if(OSTaskQuery(PRIOR_TASK_IPCAM, &tcb_data) == OS_NO_ERR) {  // IPCAM task is created?
                            if(pcurrent_camera != NULL)
                                if(pcurrent_camera->camera_set_exposure)                                
                                    pcurrent_camera->camera_set_exposure();              
                        }                        
                    }
                    break;

                case 4:     // saturation
                    if(gtCameraSetting.saturation != level) {
                        gtCameraSetting.saturation = level;

                        if(OSTaskQuery(PRIOR_TASK_IPCAM, &tcb_data) == OS_NO_ERR) {  // IPCAM task is created?
                            if(pcurrent_camera != NULL)
                                if(pcurrent_camera->camera_set_saturation)
                                    pcurrent_camera->camera_set_saturation();              
                        }                        
                    }
                    break;

                case 5:     // brightness
                    if(gtCameraSetting.brightness != level) {
                        gtCameraSetting.brightness = level;

                        if(OSTaskQuery(PRIOR_TASK_IPCAM, &tcb_data) == OS_NO_ERR) {  // IPCAM task is created?
                            if(pcurrent_camera != NULL)
                                if(pcurrent_camera->camera_set_brightness)                                
                                    pcurrent_camera->camera_set_brightness();              
                        }                        
                    }
                    break;

                case 6:     // hue
                    if(gtCameraSetting.hue != level) {
                        gtCameraSetting.hue = level;

                        if(OSTaskQuery(PRIOR_TASK_IPCAM, &tcb_data) == OS_NO_ERR) {  // IPCAM task is created?
                            if(pcurrent_camera != NULL)
                                if(pcurrent_camera->camera_set_hue)
                                    pcurrent_camera->camera_set_hue();              
                        }                        
                    }
                    break;

                case 7:     // sharpness
                    if(gtCameraSetting.sharpness != level) {
                        gtCameraSetting.sharpness = level;

                        if(OSTaskQuery(PRIOR_TASK_IPCAM, &tcb_data) == OS_NO_ERR) {  // IPCAM task is created?
                            if(pcurrent_camera != NULL)
                                if(pcurrent_camera->camera_set_sharpness)                                
                                    pcurrent_camera->camera_set_sharpness();              
                        }                        
                    }
                    break;

                case 8:     // lightmode
                    if(gtCameraSetting.lightmode != level) {
                        gtCameraSetting.lightmode = level;

                        if(OSTaskQuery(PRIOR_TASK_IPCAM, &tcb_data) == OS_NO_ERR) {  // IPCAM task is created?
                            if(pcurrent_camera != NULL)
                                if(pcurrent_camera->camera_set_lightmode)                                
                                    pcurrent_camera->camera_set_lightmode();              
                        }                        
                    }
                    break;

                case 9:     // frequency
                    if(gtCameraSetting.frequency != level) {
                        gtCameraSetting.frequency = level;

                        if(OSTaskQuery(PRIOR_TASK_IPCAM, &tcb_data) == OS_NO_ERR) {  // IPCAM task is created?
                            if(pcurrent_camera != NULL)
                                if(pcurrent_camera->camera_set_frequency)                                
                                    pcurrent_camera->camera_set_frequency();              
                        }                        
                    }
                    break;

                case 10:     // home mode
                    if(gtCameraSetting.homemode != level) {
                        gtCameraSetting.homemode = level;

                        if(OSTaskQuery(PRIOR_TASK_IPCAM, &tcb_data) == OS_NO_ERR) {  // IPCAM task is created?
                            if(pcurrent_camera != NULL)
                                if(pcurrent_camera->camera_set_homemode)                                
                                    pcurrent_camera->camera_set_homemode();              
                        }                        
                    }
                    break;


                default:    
                    printf("attribute error!\n");
                    break;
            }
            break;

        case tSetCameraImageMirror:
            printf("\nTwoRiscCommProcVideo: tSetCameraImageMirror\n");

            if(gtCameraSetting.inited == 0) {    // if not inited yet
                printf("TwoRiscCommProcVideo tSetCameraImageMirror: set camera image by linux!");
                gtCameraSetting.inited = 1;
            }

            if(gtCameraSetting.image_mirror != *((short *)(msg + 2) + 1)) {
                gtCameraSetting.image_mirror = *((short *)(msg + 2) + 1);

                if(OSTaskQuery(PRIOR_TASK_IPCAM, &tcb_data) == OS_NO_ERR) {  // IPCAM task is created?
                    if(pcurrent_camera != NULL)
                        if(pcurrent_camera->camera_set_image_mirror)                                
                            pcurrent_camera->camera_set_image_mirror();              
                }                        
            }

            if(gtCameraSetting.image_flip != *(short *)(msg + 2)) {
                gtCameraSetting.image_flip = *(short *)(msg + 2);

                if(OSTaskQuery(PRIOR_TASK_IPCAM, &tcb_data) == OS_NO_ERR) {  // IPCAM task is created?
                    if(pcurrent_camera != NULL)
                        if(pcurrent_camera->camera_set_image_flip)                                
                            pcurrent_camera->camera_set_image_flip();              
                }                        
            }

            break;
            
        
        case tGetCamReg:
            break;

        case tSetCBR:    
#ifdef BAND_WIDTH_CONTROL
            gtcbrinfo.bw = *(msg + 2);    
            cbr_init(gtcbrinfo.bw);
            printf("\nTwoRiscCommProcVideo tSetCBR: bandwidth = %d!\n", gtcbrinfo.bw);
#endif
            break;
            
        case tGetCBR:
            break;

#ifdef OSD_ENABLE
        case tOSDClearScreen:
            tClearScreenParameters.pixel_start = *(unsigned short *)(msg + 2);
            tClearScreenParameters.line_start = *((unsigned short *)(msg + 2) + 1);
            tClearScreenParameters.height = *(unsigned short *)(msg + 3);
            tClearScreenParameters.width = *((unsigned short *)(msg + 3) + 1); 
            tClearScreenParameters.attribute = *(msg + 4);
            
            printf("\nTwoRiscCommProcVideo: tClearScreen\n");            
            printf("\nstart_x = %d, start_y = %d\n", tClearScreenParameters.pixel_start, tClearScreenParameters.line_start);
            printf("\nclr_width = %d, clr_height = %d\n", tClearScreenParameters.width, tClearScreenParameters.height);
            printf("\nattribute= 0x%x\n", tClearScreenParameters.attribute);

            Vb2100ClearScreen(VB2100_MASTER, VB2100_PATH, tClearScreenParameters/*gtClearScreenSetting tClearScreenParameters*/);
       
            break;  

        case tOSDFillBMP:
            gtBmpParameters.start_pixel =  *(unsigned short *)(msg + 2);
            gtBmpParameters.start_line = *((unsigned short *)(msg + 2)+1);
            gtBmpParameters.bmp_addr = *(msg + 3); 
            gtBmpParameters.index_addr = *(msg + 5);
            gtBmpParameters.attribute=  *((unsigned short *)(msg + 6)+1);    
            
            printf("\nTwoRiscCommProcVideo: tFillBMP\n");
            printf("\nstart_pixel = %d,start_line = %d\n", gtBmpParameters.start_pixel, gtBmpParameters.start_line);
            printf("\nindex_addr = 0x%x\n", gtBmpParameters.index_addr );
            printf("\nbmp_addr = 0x%x\n", gtBmpParameters.bmp_addr );
            printf("\nattribute= 0x%x\n", gtBmpParameters.attribute);
            
            Vb2100FillBmp(VB2100_MASTER, VB2100_PATH, gtBmpParameters.start_pixel, gtBmpParameters.start_line, (unsigned int *)(gtBmpParameters.index_addr|0xa0000000), (unsigned int *)(gtBmpParameters.bmp_addr|0xa0000000), gtBmpParameters.attribute);

            RiscChgCnt(SEM_VIDEOPHONE /*int SemID*/, tVideo/*int main_type*/, sub_type /*int sub_type*/, -1 /*int direction*/); 
   
            break;  

        case tOSDDrawLine:
            tDrawLineParameters.start_x_pos = *(unsigned short *)(msg + 2);
            tDrawLineParameters.start_y_pos = *((unsigned short *)(msg + 2) + 1);
            tDrawLineParameters.end_x_pos = *(unsigned short *)(msg + 3) ;
            tDrawLineParameters.end_y_pos = *((unsigned short *)(msg + 3) + 1);
            tDrawLineParameters.attribute = *(msg + 4);
            
            printf("\nTwoRiscCommProcVideo: tOSDDrawLine\n");
            printf("\nstart_x = %d, start_y = %d, end_x = %d, end_y = %d!\n", tDrawLineParameters.start_x_pos, tDrawLineParameters.start_y_pos, tDrawLineParameters.end_x_pos, tDrawLineParameters.end_y_pos);
            printf("\nattribute = 0x%x!\n", tDrawLineParameters.attribute);

            Vb2100DrawLine(VB2100_MASTER, VB2100_PATH, tDrawLineParameters /* gtDrawLineSetting tDrawLineParameter*/);
            break;  

        case tOSDFillColor:
            tFillColorParameters.pixel_start = *(unsigned short *)(msg + 2);
            tFillColorParameters.line_start = *((unsigned short *)(msg + 2) + 1);
            tFillColorParameters.height = *(unsigned short *)(msg + 3);
            tFillColorParameters.width = *((unsigned short *)(msg + 3) + 1); 
            tFillColorParameters.attribute = *(msg + 4);

                        
            printf("\nTwoRiscCommProcVideo: tFillColor\n");            
            printf("\nstart_x = %d, start_y = %d\n", tFillColorParameters.pixel_start, tFillColorParameters.line_start);
            printf("\nheight = %d, width = %d\n", tFillColorParameters.height, tFillColorParameters.width);
            printf("\nattribute = 0x%x\n", tFillColorParameters.attribute);         

            Vb2100FillColor(VB2100_MASTER, VB2100_PATH, tFillColorParameters/*gtFillColorSetting tFillColorParameters*/);
            
            break;  

        case tSetOSDColorPallete:
            //index_addr = *(msg+2);
            gtOsdCurrent.color_pallate = (unsigned int *)(*(msg+2)|0xa0000000);

            printf("\nTwoRiscCommProcVideo: tSetColorPallete\n");            
            printf("\ngtOsdCurrent.color_pallate = %x\n", gtOsdCurrent.color_pallate);

            Vb2100FillOsdPT(VB2100_MASTER, VB2100_PATH, gtOsdCurrent.color_pallate);

            RiscChgCnt(SEM_VIDEOPHONE /*int SemID*/, tVideo/*int main_type*/, sub_type /*int sub_type*/, -1 /*int direction*/); 
   
            break;

        case tChangeOSDPage:
            display_page = *(unsigned short *)(msg + 2);

            printf("\nTwoRiscCommProcVideo: tChangeOSDPage\n");            
            printf("\ndisplay_page = %x\n", display_page);

            Vb2100ChangeOSDPage(VB2100_MASTER, VB2100_PATH, display_page);

            break;

        case tOSDWriteString:
            tWriteStringParameters.string_addr =( *(msg+2)|0xa0000000);
            tWriteStringParameters.string_len = *(msg+3);
            tWriteStringParameters.start_x = *(unsigned short *)(msg + 4);
            tWriteStringParameters.start_y = *((unsigned short *)(msg + 4) + 1);
            tWriteStringParameters.attribute = *(msg+5);
              
            printf("\nTwoRiscCommProcVideo: tOSDWriteString\n");
            printf("\nstring_addr = 0x%x\n", tWriteStringParameters.string_addr);
            printf("\nstring_len = 0x%x\n", tWriteStringParameters.string_len );          
            printf("\nstart_x = %d, start_y = %d\n", tWriteStringParameters.start_x, tWriteStringParameters.start_y);
            printf("\nattribute = 0x%x\n", tWriteStringParameters.attribute);         

            Vb2100WriteString(VB2100_MASTER, VB2100_PATH, tWriteStringParameters/*gtWriteStringSetting tWriteStringParameters*/);
            
            RiscChgCnt(SEM_VIDEOPHONE /*int SemID*/, tVideo/*int main_type*/, sub_type /*int sub_type*/, -1 /*int direction*/); 
   
            break;

        case tOSDRollString:
            if(*(msg + 3) > 1000) {   // linux to ucos max string length is 1000
                printf("linux to ucos OSD string length exceeds 64!\n");
                break;
            }
            gtRollStr.str_len = *(msg + 3);
            gtRollStr.attribute = *(msg+4);
            memcpy((void *)OSDStr, (void *)(*(msg + 2)|0xa0000000), gtRollStr.str_len);
            gtRollStr.str_addr = OSDStr;
              
            printf("\nTwoRiscCommProcVideo: tOSDRollString\n");
            printf("string = %s\n", OSDStr);
            printf("string length = %d\n", gtRollStr.str_len ); 
            printf("attribute = 0x%x\n", gtRollStr.attribute ); 

            if(gtRollStr.attribute&0x1){
                if(
                    #ifdef CHIP_CFG_STACK_CHECK
                    OSTaskCreateExt(TaskRollString, (void *)0, (void *)&StkTaskRollStr[sizeof(StkTaskRollStr) / sizeof(StkTaskRollStr[0]) - 1], PRIOR_TASK_ROLLSTR,\
                            PRIOR_TASK_ROLLSTR, (void *)&StkTaskRollStr[0], sizeof(StkTaskRollStr) / sizeof(StkTaskRollStr[0]), (void *)0, OS_TASK_OPT_STK_CHK+OS_TASK_OPT_STK_CLR) != OS_NO_ERR	
                    #else                    
                    OSTaskCreate(TaskRollString, (void *)0, (void *)&StkTaskRollStr[STK_SIZE_ROLLSTR - 1], PRIOR_TASK_ROLLSTR) != OS_NO_ERR
                    #endif
                  )
                    debug ("\r\nTaskCreate TaskRollString error!\r\n");
                else{
                    rollstr_page = (unsigned char)((gtRollStr.attribute>>0x1)&0x7);
                    printf("\r\nTaskCreate TaskRollString OK!\r\n");
                }
            } else {
                if(OSTaskDel(PRIOR_TASK_ROLLSTR) != OS_NO_ERR )
                    printf("OSTaskDel(PRIOR_TASK_ROLLSTR) failed!\n");
                else{ 
                    tClearScreenParameters.pixel_start = gtRollStr.start_pixel;
                    tClearScreenParameters.line_start = gtRollStr.start_line;
                    tClearScreenParameters.width = gtOsdCurrent.width;
                    tClearScreenParameters.height = gtRollStr.str_height;
                    tClearScreenParameters.attribute = (unsigned int) rollstr_page;
                    Vb2100ClearScreen(VB2100_MASTER, VB2100_PATH, tClearScreenParameters);
                    printf("OSTaskDel(PRIOR_TASK_ROLLSTR) OK!\n");
                }
            }

            RiscChgCnt(SEM_VIDEOPHONE /*int SemID*/, tVideo/*int main_type*/, sub_type /*int sub_type*/, -1 /*int direction*/); 
   
            break;

#endif

#ifdef MOTION_DETECTION         
        case tSetMotionSensity:

            gMotionSensi = *(msg + 2);
            gpAreaVectorAddr = *(msg + 3);
            if(gMotionSensi>0)
                gIsMDEnabled = TRUE;
            else
                gIsMDEnabled = FALSE;

#ifndef DVR
            if(OSTaskQuery(PRIOR_TASK_MD_START, &tcb_data) == OS_PRIO_ERR)
            {
                if(gMotionSensi > 0) {
#ifdef CHIP_CFG_STACK_CHECK
                    err = OSTaskCreateExt(TaskMotionDetection, (void *)0, (void *)&StkMDStart[MD_TASK_STACK_SIZE - 1], PRIOR_TASK_MD_START,
                                                          PRIOR_TASK_MD_START, (void *)&StkMDStart[0], MD_TASK_STACK_SIZE, (void *)0, OS_TASK_OPT_STK_CHK|OS_TASK_OPT_STK_CLR);
#else
                    err = OSTaskCreate(TaskMotionDetection, (void *)0, (void *)&StkMDStart[MD_TASK_STACK_SIZE - 1], PRIOR_TASK_MD_START);
#endif
                    if(err != OS_NO_ERR)
                        printf("OSTaskCreate TaskMotionDetection error =%d\n", err);
                    else
                        printf("OSTaskCreate TaskMotionDetection sucessful.\n");
#ifdef CHIP_CFG_STACK_CHECK
                    err = OSTaskCreateExt(TaskMDProcess, (void *)0, (void *)&StkMDProcess[MD_TASK_STACK_SIZE - 1], PRIOR_TASK_MD_PROCESS,
                                                        PRIOR_TASK_MD_PROCESS, (void *)&StkMDProcess[0], MD_TASK_STACK_SIZE, (void *)0, OS_TASK_OPT_STK_CHK|OS_TASK_OPT_STK_CLR);
#else
                    err = OSTaskCreate(TaskMDProcess, (void *)0, (void *)&StkMDProcess[MD_TASK_STACK_SIZE - 1], PRIOR_TASK_MD_PROCESS);
#endif
                    if(err != OS_NO_ERR)
                        printf("OSTaskCreate TaskMDProcess error =%d\n", err);
                    else
                        printf("OSTaskCreate TaskMDProcess sucessful.\n");
                }
            }else {
                MDTemporalDifInit(gMotionSensi);
            }
#endif
            break;

        case tSetMDFrameRate:
            gtMDFrameRateCtrl.framerate_nomotion= *((unsigned char *)(msg + 2)+3);
            gtMDFrameRateCtrl.framerate_motion = *((unsigned char *)(msg + 2) + 2);
            gtMDFrameRateCtrl.motionduration = *(unsigned short *)(msg + 2);
            printf("tSetMDFrameRate:nomotion=%d,motion=%d,duration=%d\n",gtMDFrameRateCtrl.framerate_nomotion,
                gtMDFrameRateCtrl.framerate_motion, gtMDFrameRateCtrl.motionduration);
            break;
 #endif
 
        case tCheckVideoStatus:
            //debug("\nxxxxxxxxxx video status is 0x%x", *(msg + 2));
            break;
            
        case tVideoTransferUcosToLinux:
            switch(*(unsigned short*)(msg+4)) {
                case 1:
                    break;
            
                case 0:
                    gtVideoUcosToLinux.addr = *(msg + 2);
                    gtVideoUcosToLinux.len = *(msg + 3);
                    gtVideoUcosToLinux.tstamp = *(msg + 5);            
                    //gtVideoUcosToLinux.frame_count = *(msg + 6);     
                        
                    if(OSMboxPost(gptVideoUcosToLinuxMbox, &gtVideoUcosToLinux) != OS_NO_ERR) {
                        printf("Risc1:OSMboxPost error!\r\n");
                        RiscDecCNT(VIDEO_UCOS_TO_LINUX_CNT, 1);
                    }
                    break;
                    
                default:
                    break;
            }
            break;

        case tOddEvenSelect:   
            if((*(msg + 2)&0xffff) == 0x1) {    // Vin
                if((*(msg + 2)&0xffff0000) & 0x10000) 
                    gVinOddEvenFrameSelect = EVEN_FRAME_FIRST;     // 1 - Even frame first
                else                                                
                    gVinOddEvenFrameSelect = ODD_FRAME_FIRST;     //0 - Odd frame first        

                break;
            }

#ifdef DISPLAY_INCLUDE            
            if((*(msg + 2)&0xffff) == 0x2) {    // Vsc
                if((*(msg + 2)&0xffff0000) & 0x10000) 
                    gVscOddEvenFrameSelect = EVEN_FRAME_FIRST;     // 1 - Even frame first
                else                                                
                    gVscOddEvenFrameSelect = ODD_FRAME_FIRST;     //0 - Odd frame first  

                break;
            }
#endif            

            debug("\nTwoRiscCommProcVideo: tOddEvenSelect error!\n");        
            break;

        case tSetEOImgMode:     //vin:divided image or merged image when analog camera
            if(gCameraSelectDigital == TRUE)
                break;
            if((*(msg + 2) == 0) || (*(msg + 2) == 1)) {
                if(gImage_Mode != *(msg + 2)) {
                    gImage_Mode = *(msg + 2);
                    if(OSTaskQuery(PRIOR_TASK_IPCAM, &tcb_data) == OS_NO_ERR)
                    {
                            err = OSQPost(gptQMsgTwoRiscSystemMonitor, (void *)TWO_RISC_SYSTEM_REOPEN_VIN_TASK);                            
                            if(err != OS_NO_ERR)            
                                debug("\r\nRISC0: OSMboxPost error! err = %d!\r\n", err);                
                    }                    
                }
                printf("tSetEvenOddImgMode = %d\n", gImage_Mode);
            } else {
                printf("tSetEvenOddImgMode error = %d\n", *(msg + 2));
            }
            break;

         case tSetEOImgEncodeMode:    //encoder: encode the both field or one of them when analog camera and divided image
            if(gCameraSelectDigital == TRUE)
                break;
            if((*(msg + 2) >= 0) && (*(msg + 2) < 3) && (*(msg + 2) != gImgEncodeMode)) {
                gImgEncodeMode = *(msg +2);
                if(OSTaskQuery(PRIOR_TASK_IPCAM, &tcb_data) == OS_NO_ERR) {  // IPCAM task is created?
                    err = OSQPost(gptQMsgTwoRiscSystemMonitor, (void *)TWO_RISC_SYSTEM_REOPEN_VIN_TASK);                            
                    if(err != OS_NO_ERR)            
                        debug("\r\nRISC0: OSMboxPost error! err = %d!\r\n", err);                
                } 
                printf("tSetEOImgEncodeMode =%d\n", gImgEncodeMode);
            } else {
                printf("tSetEOImgEncodeMode error = %d\n", gImgEncodeMode);
            }
            break;

#ifdef  DISPLAY_INCLUDE

        case tSetVoutRvoInterlaced:
            if((*(msg + 2)&0xffff) == 0x2) {    // Rvo
                if((*(msg + 2)&0xffff0000) & 0x10000) {    // progressive 
                    if(gtRvoCurrent.interlaced != FALSE) {
                        gtRvoCurrent.interlaced = FALSE;
                        gtRvoCurrent1.interlaced = FALSE;

                        if(gRvoEnable) {
                            if(gVoutEnable) {
                                gVoutReEnable = 1;
                            }                     
                        }   
                    }                    
                } else {     // interlaced                                               
                    if(gtRvoCurrent.interlaced != TRUE) {
                        gtRvoCurrent.interlaced = TRUE;
                        gtRvoCurrent1.interlaced = TRUE;

                        if(gRvoEnable) {
                            if(gVoutEnable) {
                                gVoutReEnable = 1;
                            }                     
                        }   
                    } 
                }

                printf("\nTwoRiscCommProcVideo: tSetVoutRvoInterlaced gtRvoCurrent.interlaced = %d!\n", gtRvoCurrent.interlaced);

                break;
            }

            if((*(msg + 2)&0xffff) == 0x1) {    // Vout
                if((*(msg + 2)&0xffff0000) & 0x10000) 
                    printf("\nTwoRiscCommProcVideo: tSetVoutRvoInterlaced Vout progressive not support till now!\n");     // progressive
                break;
            }

            printf("\nTwoRiscCommProcVideo: tSetVoutRvoInterlaced error!\n");
            break;

        case tVideoTransferLinuxToUcos:

#if 0          
            /* compute the time of a frame */
            t1= count;
            count = GetCount();
            if(count > t1)
                t1 = count - t1;
            else
                t1 = CPU_MAIN_CLOCK / (2 * OS_TICKS_PER_SEC) + count - t1;    
            printf("<f%d>", t1 / (CPU_MAIN_CLOCK/(2*1000)));    // counted by mili-seconds
#endif

#if 0                        
            gtVideoLinuxToUcos.addr = *(msg + 2);
            gtVideoLinuxToUcos.len = *(msg + 3);
            //gtVideoLinuxToUcos.stuff_byte_count = *(msg + 5);            
            gtVideoLinuxToUcos.frame_count = *(msg + 6);            
            
            if(OSMboxPost(gptVideoLinuxToUcosMbox, &gtVideoLinuxToUcos) != OS_NO_ERR) {
                debug("Risc0:OSMboxPost error!\r\n");
            } else 
                ;//printf("#%d", gtVideoLinuxToUcos.frame_count);           
#endif

#if 0
            if(ScWr(SC_VIDEO_LINUX_TO_UCOS, *(msg + 2)) == FAILED)
                printf("ScWr failed!");                

            if(ScWr(SC_VIDEO_LINUX_TO_UCOS, *(msg + 3)) == FAILED)
                printf("ScWr failed!");                
#endif
            gtStatistics.cnt_video_linux_to_ucos++;
            gtStatistics.frame_linux_to_ucos_ps++;
            if(gVideoEnableDisable == 0) {
                RiscDecCNT(VIDEO_LINUX_TO_UCOS_CNT, 1);
                printf("(D)");
                break;
            }
#ifndef VIDEO_JITTER_BUFFER
            if(TwoRiscDisplaySendPacket(SC_VIDEO_LINUX_TO_UCOS, msg+2, /* 3 * sizeof(int) */sizeof(tVideoLinuxToUcos)) == FAILED) {
                RiscDecCNT(VIDEO_LINUX_TO_UCOS_CNT, 1);    
                printf("D");
                break;
            } else {
                logprintf(DEBUG_MESSAGE, MP4_DECODE, "[%d]", RiscRdCNT(VIDEO_LINUX_TO_UCOS_CNT));
            }

            if(OSSemPost(gptVideoLinuxToUcosMbox) != OS_NO_ERR)
                debug("Risc0:OSMboxPost error!\r\n");                       
#else
            frameFromeLinux.addr = *(msg + 2);
            frameFromeLinux.size = *(msg + 3);
            frameFromeLinux.TimeStamp = *(msg + 4);
#ifdef DEBUG_AVSYNC
            gtStatistics.video_in_ts = frameFromeLinux.TimeStamp;
#endif
            /* gb add July 31 */
            
            if(1/*err == OS_NO_ERR*/) {

                if(gChecksumEnabled == TRUE) {
                    i = frameFromeLinux.size / 4 - 2;
                    i = *((unsigned int *)frameFromeLinux.addr + i);
                    if((linuxframenum+1) != i)
                        printf("linux to ucos frame num error(%d,%d)\n", linuxframenum, i);
                    linuxframenum = i;
                }
                err = VBufSaveOneFrame(&VideoBuffer1, &frameFromeLinux);
                if(err != 0){               
                    printf("VBufSaveOneFrame err!%d, %d\n", err, VideoBuffer1.CurCnt);   
                    RiscDecCNT(VIDEO_LINUX_TO_UCOS_CNT, 1);
                    break;
                }
            }   
            if(RiscRdCNT(VIDEO_LINUX_TO_UCOS_CNT) != VideoBuffer1.CurCnt)
                printf("error,cnt=%d,jitbuf depth=%d\n", RiscRdCNT(VIDEO_LINUX_TO_UCOS_CNT), VideoBuffer1.CurCnt);
//            RiscDecCNT(VIDEO_LINUX_TO_UCOS_CNT, 1);
            if(OSSemPost(gptVideoLinuxToUcosMbox) != OS_NO_ERR)
                printf("Risc0:OSMboxPost error!\r\n");  
#endif
            break;                

        case tYUVTransferLinuxToUcos:
            if((gVoutEnable == 0) || (gRvoEnable == 0)) {
                RiscDecCNT(YUV_LINUX_TO_UCOS_CNT, 1);
                break;
            }
            if((*(short *)(msg + 2) > MAX_MPEG_DECODE_HEIGHT) || (*((short *)(msg + 2) + 1) > MAX_MPEG_DECODE_WIDTH))
            {
                printf("YUV frame too big(width=%d,height=%d)\n", *((short *)(msg + 2) + 1), *(short *)(msg + 2));
                RiscDecCNT(YUV_LINUX_TO_UCOS_CNT, 1);
                break;
            }
            YUVLinuxToUcos.height = *(short *)(msg + 2);
            YUVLinuxToUcos.width = *((short *)(msg + 2) + 1);
            YUVLinuxToUcos.y_addr = *(msg + 3);
            YUVLinuxToUcos.u_addr = *(msg + 4);
            YUVLinuxToUcos.v_addr = *(msg + 5);

            //YUVLinuxToUcos.format = YUV420;
            //YUVLinuxToUcos.line_ratio = vertical_no_scale;
            //YUVLinuxToUcos.pixel_ratio = horizontal_no_scale;
            //YUVLinuxToUcos.interlaced = FALSE;

            if(gMpeg4DecodeOutputBufNo == 0) {
                memcpy((void *)(gMpeg4DecodeOptIaddr0|0xa0000000), (void *)YUVLinuxToUcos.y_addr, YUVLinuxToUcos.height*YUVLinuxToUcos.width*3/2);

                YUVLinuxToUcos.y_addr = gMpeg4DecodeOptIaddr0;
                YUVLinuxToUcos.u_addr = gMpeg4DecodeOptIaddr0 + YUVLinuxToUcos.height*YUVLinuxToUcos.width;
                YUVLinuxToUcos.v_addr = YUVLinuxToUcos.u_addr + YUVLinuxToUcos.height*YUVLinuxToUcos.width / 4;
                gMpeg4DecodeOutputBufNo = 1;
            }else {
                memcpy((void *)(gMpeg4DecodeOptIaddr1|0xa0000000), (void *)YUVLinuxToUcos.y_addr, YUVLinuxToUcos.height*YUVLinuxToUcos.width*3/2);

                YUVLinuxToUcos.y_addr = gMpeg4DecodeOptIaddr1;
                YUVLinuxToUcos.u_addr = gMpeg4DecodeOptIaddr1 + YUVLinuxToUcos.height*YUVLinuxToUcos.width;
                YUVLinuxToUcos.v_addr = YUVLinuxToUcos.u_addr + YUVLinuxToUcos.height*YUVLinuxToUcos.width / 4;
                gMpeg4DecodeOutputBufNo = 0;
            }
 
            if((YUVLinuxToUcos.width != gtRvoCurrent.width) || \
                (YUVLinuxToUcos.height != gtRvoCurrent.height))
            {
                gtRvoCurrent.height = YUVLinuxToUcos.height;
                gtRvoCurrent.width = YUVLinuxToUcos.width;
                if((gVoutEnable == 1) && (gRvoEnable == 1))
                    gVoutReEnable = 1;
            }
            
            if(OSQPost(gptMpeg4DecodeOutputReady, &YUVLinuxToUcos) != OS_NO_ERR) {
                printf("\nOSQPost in tYUVTransferLinuxToUcos error!\n");
            }
            
            RiscDecCNT(YUV_LINUX_TO_UCOS_CNT, 1);
            break;
            
        case 0x1d:    // manually reset
            manual_reset_flag = 1; 
            break;            
            
        case tSetScale:
            flag_scale_changed = 0;
            if(*(msg + 2) == RVO) {   // RVO or PIP?                
                switch (*(short *)(msg + 3)) {      // pixel ratio   
                    case NotScale:
                        if(gtRvoCurrent1.pixel_ratio != horizontal_no_scale) {
                            flag_scale_changed = 1;
                            gtRvoCurrent1.pixel_ratio = horizontal_no_scale;
                        }
                        break;
                                
                    case Times_2:
                        if(gtRvoCurrent1.pixel_ratio != horizontal_times_2) {
                            flag_scale_changed = 1;
                            gtRvoCurrent1.pixel_ratio = horizontal_times_2;
                        }
                        break;
                                
                    case Times_4:
                        if(gtRvoCurrent1.pixel_ratio != horizontal_times_4) {
                            flag_scale_changed = 1;
                            gtRvoCurrent1.pixel_ratio = horizontal_times_4;
                        }
                        break;
                                
                    case Div_2:
                        if(gtRvoCurrent1.pixel_ratio != horizontal_div_2) {
                            flag_scale_changed = 1;
                            gtRvoCurrent1.pixel_ratio = horizontal_div_2;
                        }
                        break;
                                
                    case Div_4:
                        if(gtRvoCurrent1.pixel_ratio != horizontal_div_4) {
                            flag_scale_changed = 1;
                            gtRvoCurrent1.pixel_ratio = horizontal_div_4;
                        }
                        break;
                                
                    case Div_8:
                        if(gtRvoCurrent1.pixel_ratio != horizontal_div_8) {
                            flag_scale_changed = 1;
                            gtRvoCurrent1.pixel_ratio = horizontal_div_8;
                        }
                        break;
    
                     default:
                        printf("\nTwoRiscCommProcVideo: tSetScale: pixel ratio error!\n");
                        break;
                }   
    
                switch (*((short *)(msg + 3) + 1)) {    // line ratio
                    case NotScale:
                        if(gtRvoCurrent1.line_ratio != vertical_no_scale) {
                            flag_scale_changed = 1;
                            gtRvoCurrent1.line_ratio = vertical_no_scale;
                        }
                        break;
                                
                    case Times_2:
                        if(gtRvoCurrent1.line_ratio != vertical_times_2) {
                            flag_scale_changed = 1;
                            gtRvoCurrent1.line_ratio = vertical_times_2;
                        }
                        break;
                               
                    case Times_4:
                        if(gtRvoCurrent1.line_ratio != vertical_times_4) {
                            flag_scale_changed = 1;
                            gtRvoCurrent1.line_ratio = vertical_times_4;
                        }
                        break;
                                
                    case Times_8:
                        if(gtRvoCurrent1.line_ratio != vertical_times_8) {
                            flag_scale_changed = 1;
                            gtRvoCurrent1.line_ratio = vertical_times_8;
                        }
                        break;
                                
                    case Div_2:
                        if(gtRvoCurrent1.line_ratio != vertical_div_2) {
                            flag_scale_changed = 1;
                            gtRvoCurrent1.line_ratio = vertical_div_2;
                        }
                        break;
                                
                    case Div_4:
                        if(gtRvoCurrent1.line_ratio != vertical_div_4) {
                            flag_scale_changed = 1;
                            gtRvoCurrent1.line_ratio = vertical_div_4;
                        }
                        break;
                                
                    case Div_8:
                        if(gtRvoCurrent1.line_ratio != vertical_div_8) {
                            flag_scale_changed = 1;
                            gtRvoCurrent1.line_ratio = vertical_div_8;
                        }
                        break;                                
    
                    default:
                        printf("\nTwoRiscCommProcVideo: tSetScale: line ratio error!\n");
                        break;
                }

                printf("linux set rvo scale:(%d,%d)\n", gtRvoCurrent1.pixel_ratio, gtRvoCurrent1.line_ratio);
                if(flag_scale_changed) {
                    if(gVoutEnable)
                        gVoutReEnable = 1;
                }
                        
            } else if(*(msg + 2) == PIP) {
                flag_scale_changed = 0;
                switch (*(short *)(msg + 3)) {      // pixel ratio   
                    case NotScale:
                        if(gtPipCurrent.pixel_ratio != horizontal_no_scale) {
                            flag_scale_changed = 1;
                            gtPipCurrent.pixel_ratio = horizontal_no_scale;
                        }
                        break;
                                
                    case Times_2:
                        if(gtPipCurrent.pixel_ratio != horizontal_times_2) {
                            flag_scale_changed = 1;
                            gtPipCurrent.pixel_ratio = horizontal_times_2;
                        }
                        break;
                                
                    case Times_4:
                        if(gtPipCurrent.pixel_ratio != horizontal_times_4) {
                            flag_scale_changed = 1;
                            gtPipCurrent.pixel_ratio = horizontal_times_4;
                        }
                        break;
                                
                    case Div_2:
                        if(gtPipCurrent.pixel_ratio != horizontal_div_2) {
                            flag_scale_changed = 1;
                            gtPipCurrent.pixel_ratio = horizontal_div_2;
                        }
                        break;
                                
                    case Div_4:
                        if(gtPipCurrent.pixel_ratio != horizontal_div_4) {
                            flag_scale_changed = 1;
                            gtPipCurrent.pixel_ratio = horizontal_div_4;
                        }
                        break;
                                
                    case Div_8:
                        if(gtPipCurrent.pixel_ratio != horizontal_div_8) {
                            flag_scale_changed = 1;
                            gtPipCurrent.pixel_ratio = horizontal_div_8;
                        }
                        break;
    
                     default:
                        printf("\nTwoRiscCommProcVideo: tSetScale: pixel ratio error!\n");
                        break;
                }   
    
                switch (*((short *)(msg + 3) + 1)) {    // line ratio
                    case NotScale:
                        if(gtPipCurrent.line_ratio != vertical_no_scale) {
                            flag_scale_changed = 1;
                            gtPipCurrent.line_ratio = vertical_no_scale;
                        }
                        break;
                                
                    case Times_2:
                        if(gtPipCurrent.line_ratio != vertical_times_2) {
                            flag_scale_changed = 1;
                            gtPipCurrent.line_ratio = vertical_times_2;
                        }
                        break;
                               
                    case Times_4:
                        if(gtPipCurrent.line_ratio != vertical_times_4) {
                            flag_scale_changed = 1;
                            gtPipCurrent.line_ratio = vertical_times_4;
                        }
                        break;
                                
                    case Times_8:
                        if(gtPipCurrent.line_ratio != vertical_times_8) {
                            flag_scale_changed = 1;
                            gtPipCurrent.line_ratio = vertical_times_8;
                        }
                        break;
                                
                    case Div_2:
                        if(gtPipCurrent.line_ratio != vertical_div_2) {
                            flag_scale_changed = 1;
                            gtPipCurrent.line_ratio = vertical_div_2;
                        }
                        break;
                                
                    case Div_4:
                        if(gtPipCurrent.line_ratio != vertical_div_4) {
                            flag_scale_changed = 1;
                            gtPipCurrent.line_ratio = vertical_div_4;
                        }
                        break;
                                
                    case Div_8:
                        if(gtPipCurrent.line_ratio != vertical_div_8) {
                            flag_scale_changed = 1;
                            gtPipCurrent.line_ratio = vertical_div_8;
                        }
                        break;                                
    
                    default:
                        printf("\nTwoRiscCommProcVideo: tSetScale: line ratio error!\n");
                        break;
                }

                start_x = *(short *)(msg + 4);
                start_y = *((short *)(msg + 4) + 1);
                if(gtPipSetting.start_pixel != start_x)
                {
                    if(start_x == 0)         //if 0, change to 8
                        gtPipSetting.start_pixel = 8;
                    else if((start_x & 0x7) != 0)   //if not 8 alined, changed to 8 alined
                        gtPipSetting.start_pixel = (start_x + 8) & 0xfffffff8;
                    else 
                        gtPipSetting.start_pixel = start_x;
                    flag_position_changed = 1;
                }
                if(gtPipSetting.start_line != start_y)
                {
                    if(start_y == 0)         //if 0, change to 8
                        gtPipSetting.start_line = 8;
                    else if((start_y & 0x7) != 0)   //if not 8 alined, changed to 8 alined
                        gtPipSetting.start_line = (start_y + 8) & 0xfffffff8;
                    else
                        gtPipSetting.start_line = start_y;
                    flag_position_changed = 1;
                }
                printf("set pip position start_line=%d,start_pixel=%d.\n", gtPipSetting.start_line, gtPipSetting.start_pixel);
                
                if((flag_scale_changed == 1) || (flag_position_changed == 1)) {
                    if(gVoutEnable && gPipEnable)
                        gVoutReEnable = 1;
                }

            } else {
                printf("\nTwoRiscCommProcVideo: tSetScale: mode error!\n");
            }
 
            break;

        case tSetSyncMode:    //avsync/selfsync/nosync
            SetSyncMode(*(msg + 2));
            break;

         case tSetThreshold:  //avsync
            SetTsThreshold(*(short *)(msg + 2), *((short *)(msg + 2) + 1));
            break;

        case tStillImageInfo:  //added by wy        
#ifdef STARTUP_PICTURE
            err = AppGetStiImgData(*(msg + 4), *(msg +5));
            if(err) {
                printf("ucos get startup picture data error!\n");
                break;
            }
            RiscChgCnt(SEM_VIDEOPHONE, tVideo, tStillImageInfo, -1);
            gtStartupPic.width = *(short *)(msg + 3);
            gtStartupPic.height =  *((short *)(msg + 3) + 1);
            if(*(short *)(msg + 2) == 2)
                gtStartupPic.yuv_input_format = YUV420;
            else if(*(short *)(msg + 2) == 3)
                gtStartupPic.yuv_input_format = YUV422;
            else
                printf("startup picture format error,format=%d\n", *(short *)(msg + 2));
            
            printf("startup picture width=%d, height=%d\n", gtStartupPic.width, gtStartupPic.height);

            if((gVoutEnable == 1) && (gRvoEnable == 1) && (gVideoEnableDisable == 0)) {
                TwoRiscDisplayStartupPicInit();
                gVoutReEnable = 1;
            }
#endif
            break;

        case tDisplayStiImg:   //added by wy
            //AppShowStiImg(i);
            break;

        case tGetCameraType:
            if(RiscRdID() == RISC0) {
                printf("\n\nTwoRiscCommProcVideo: tGetCameraType error!\n\n");
            }
            break;

#ifdef DUMP_DECODE_OUTPUT
        case 0xef:
            g_dump_decode_output_en = 0;
            printf("\ng_dump_decode_output_en = 0\n");
            break;

        case 0xfa:
            g_dump_decode_output_en = 1;
            printf("\ng_dump_decode_output_en = 1\n");
            break;
#endif

        case tRVODisplayMode:
            gRvoDisplayMode = *(msg + 2);
            break;

#if 1

        extern int pip_improve_flag;

        // the following subtype is just used for debug
        case 0xfb:    // good PIP effect
            pip_improve_flag = TRUE;
            break;

        case 0xfc:    // bad PIP effect
            pip_improve_flag = FALSE;
            break;            

        case 0xfd:    // manually reset
            manual_stop_flag = 1;
            break;
        
        case 0xfe:    // disable vout & vsc & del the task!    
            //gTwoRiscSystemMonitorMsg = TWO_RISC_SYSTEM_CLOSE_DISPLAY_TASK;
            //err = OSQPost(gptQMsgTwoRiscSystemMonitor, &gTwoRiscSystemMonitorMsg);
            err = OSQPost(gptQMsgTwoRiscSystemMonitor, (void *)TWO_RISC_SYSTEM_CLOSE_DISPLAY_TASK);
            if(err != OS_NO_ERR)            
                debug("\r\nRISC0: OSMboxPost error! err = %d!\r\n", err);                
            break;

        case 0xff:    // this subtype is just used for debug
            if(*(msg + 2) == 1)    // add error word to decode input
                manual_err_flag = 1;

            if(*(msg + 2) == 0)    // do not add error word to decode input
                manual_err_flag = 0;        
            break;
#endif

#endif      // end of #ifdef  DISPLAY_INCLUDE
            
        default:
            debug("\nTwoRiscCommProcVideo: sub_type error: 0x%x!\n", sub_type);        
            break;
    }
}
#endif

/*
***************************************************************************
*                              TaskTwoRiscTestRiscFlag
*
* Description: Just test the risc flag interrupts.
*
* Arguments  : 
*
* Return     : 
*
* Note(s)    : Risc0 & Risc1 share the same function. You should re-write 
*              the Risc Flag ISR.
***************************************************************************
*/
void TaskTwoRiscTestRiscFlag(void *data)
{
    int i = 0;        
    unsigned int rec[16];    
    unsigned int len, *pInt;
    
    BspStartTicker(OS_TICKS_PER_SEC);
    
    rRISC_GPIO_DIR &= ~0x4000000; /* Set GPIO26 output*/
    rRISC_GPIO_DIR &= ~0x2000000; /* Set GPIO25 output*/  
    
    //OSTaskCreate(TaskLed, (void *)0, (void *)&StkTaskLed[TASK_LED_STK_SIZE - 1], 11);                                       
    
    RiscFlagIntrInit();
    
    printf("\r\n");
    printf("**********************************************************************\r\n");
    printf("*                 Risc%d Test Risc Flag                         *\r\n", RiscRdID());    
    printf("**********************************************************************\r\n");     
    printf("\r\n");                        
           
    while(1){        
        
        printf("Risc%d Task!\r\n", RiscRdID());    
        
        if(RiscRdID() == RISC0)     
            LED0_ON();               
        else
            LED1_ON();          
        
        RiscFlagSet();
            
        OSTimeDlyHMSM(0, 0, 1, 0); 
    }
}

#ifdef AUDIO_MODULE_INCLUDE
/*
***************************************************************************
*                            TwoRiscCommProcAudio
*
* Description: 
*
* Arguments  : 
*
* Return     : 
*
* Note(s)    : Risc0 & Risc1 share the same function.
***************************************************************************
*/
static int slic_initialized = 0;
unsigned int LinuxToUcosATs[MAX_AUDIO_CHANNEL] = {0,};  //used to debug every audio channel
extern LONG glwInterruptTimeScale;
void TwoRiscCommProcAudio(unsigned int *msg, unsigned int sub_type, unsigned int len)
{
    unsigned char err;
    Risc_Packet_t risc_packet;
    unsigned int *pTmp, audio_in, i, *pPacket;    
    unsigned int tonetype, length;
    unsigned int channelId,talkerId;
    unsigned int channel;

    extern tAudioCharacter channelAC[MAX_AUDIO_CHANNEL];
    extern LONG glwJTBBuildUpFromEmptyDelayNms[JITTER_BUFFER_CHANNEL_MAX][JITTER_BUFFER_TALKER_MAX];
    extern LONG glwJTBDropPacketWhenHoldDataNms[JITTER_BUFFER_CHANNEL_MAX][JITTER_BUFFER_TALKER_MAX];

    //test///////////
    int j;
    //////////////
    
    channelId = *(msg + 2);    
	//printf("Audio test");
#ifdef AUDIO_TEST_ENABLE    
    if(sub_type != tAudioTransferLinuxToUcos)
    test_save_cmd(sub_type);
#endif

    if((sub_type != tAudioTransferLinuxToUcos) && (sub_type != tAudioTransferUcosToLinux))
        printf("\nA<0x%x|0x%x|0x%x|0x%x|\n", sub_type, *(msg + 2), *(msg + 3), *(msg + 4));

    switch(sub_type) {
        
        case tAudioOpen:     // open audio //*msg = 0: slic, *msg = 1: codec
            pTmp = msg + 3;

            //AppAudioReset();
            gtStatistics.audioInfo[channelId][0] = *pTmp;
            if ((*pTmp) == 0){   // slic
                //test1//////////
                debug("Open audio channel %d\n",channelId);

                channelAC[channelId].lwAudioDevice = AUDIO_DEVICE_SLIC;

#ifdef RISC1_UCOS
                LONG lwSlicCsReset[]={2, 0x000a0006, 0x000a0007};
                *(msg+4) = (int)lwSlicCsReset;
#endif
                TwoRiscSetPara((int *)(*(msg+4)));  //add for getting slic parametre

                if(!slic_initialized){
                    AppAudioClkOpen();
#ifndef DVR                      
                    if(
                       #ifdef CHIP_CFG_STACK_CHECK
                       OSTaskCreateExt(Slic_Task, (void *)0, (void *)&StkTwoRiscSlicTA[sizeof(StkTwoRiscSlicTA) / sizeof(StkTwoRiscSlicTA[0]) - 1], PRIOR_TASK_SLIC,\
                                PRIOR_TASK_SLIC, (void *)&StkTwoRiscSlicTA[0], sizeof(StkTwoRiscSlicTA) / sizeof(StkTwoRiscSlicTA[0]), (void *)0, OS_TASK_OPT_STK_CHK+OS_TASK_OPT_STK_CLR) != OS_NO_ERR	
                       #else                    
                       OSTaskCreate(Slic_Task, (void *)0, (void *)&StkTwoRiscSlicTA[STK_SIZE_TwoRiscSlicTA - 1], PRIOR_TASK_SLIC) != OS_NO_ERR
                       #endif
                       )   //gb: this can be moved out of the ISR
                        debug("\r\nTwoRiscCommProcAudio: ucos task create error!\r\n");    
#endif
                    slic_initialized = 1;
                }
                debug("..Audio Using Slic.\n");
            }
	     else{    // codec
                AudioVoice = 0;
#ifdef CODEC_TLV320AIC23B
                ConfigureAIC(12000000, 8000);
                AICInit();
#endif
                channelAC[channelId].lwAudioDevice = AUDIO_DEVICE_CODEC;
                AppAudioClkOpen();
                debug("channelAC[%d].lwAudioDevice = %d\n", channelId, channelAC[channelId].lwAudioDevice);
                debug("..Audio Using codec.\n");
            }

#if 0            
            else if((*pTmp) == 1){    // codec
                AudioVoice = 0;
#ifdef CODEC_TLV320AIC23B
                ConfigureAIC(12000000, 8000);
                AICInit();
#endif
                channelAC[channelId].lwAudioDevice = AUDIO_DEVICE_CODEC;
                AppAudioClkOpen();
                debug("channelAC[%d].lwAudioDevice = %d\n", channelId, channelAC[channelId].lwAudioDevice);
                debug("..Audio Using codec.\n");
            }
            else{    // DVR Audio input
                AudioVoice = 0;
                /* Init tw2835 audio */
                AppTW2835AudioInit();

                channelAC[channelId].lwAudioDevice = AUDIO_DEVICE_TW2835;
                AppAudioClkOpen();
                debug("channelAC[%d].lwAudioDevice = %d\n", channelId, channelAC[channelId].lwAudioDevice);
                debug("..Audio Using tw2835 audio.\n");
            }
#endif            
            channelAC[channelId].lwChannel = channelId;
            channelAC[channelId].lwTalkerEnableBitFlag = 0;
            channelAC[channelId].lwTalkerHaveSetCodecBitFlag = 0;
            channelAC[channelId].lwTalkerMaskBitFlag = 0;
            channelAC[channelId].lwCurrentActiveTalkerCount = 0;

            break;

        case tAudioClose:    // close audio                    
            break;

        case tAudioSetAudioCodec:

            //channelAC[channelId].lwChannel = channelId;
            //printf ("**Come is %d**", *pTmp);

            pTmp = (unsigned int *)(*(msg + 3));

            talkerId = *(pTmp + 8); //get Talker Id
            gtStatistics.audioInfo[channelId][1] = talkerId;
            //test///////////
            debug("Set=%d-%d ",channelId, talkerId);
#if 0
            debug("len=%d..CodecPointer=0x%x msg = ",len, pTmp);
            for (j=0; j < 10; j++)
            {
                debug("0x%x ",*(pTmp+j));
            }
        /////////////
#endif

            //test////
            debug ("**Come is %d**", *pTmp);
            gtStatistics.audioInfo[channelId][2] = *pTmp;
            /////////
            switch (*pTmp) //1  encoding codec type
            {
                case tG711U:
                    //test/////
                    debug("11U");
                    ///////////
                    channelAC[channelId].lwGxxType[talkerId] = GXX_TYPE_G711U;
                    break;

                case tG711A:
                    debug("11A");
                    ///////////
                    channelAC[channelId].lwGxxType[talkerId] = GXX_TYPE_G711A;
                    break;

                case tG726_16K:
                    //test/////
                    debug("2616");
                    ///////////
                    channelAC[channelId].lwGxxType[talkerId] = GXX_TYPE_G726;
                    channelAC[channelId].lwG726EncodeOutputType[talkerId] = G726_DATA_TYPE_16K;
                    channelAC[channelId].lwG726DecodeInputType[talkerId] = G726_DATA_TYPE_16K;
                    channelAC[channelId].lwG726EncodeInputType[talkerId] = AUDIO_ENCODING_LINEAR;
                    break;

                case tG726_24K:
                    //test/////
                    debug("2624");
                    ///////////
                    channelAC[channelId].lwGxxType[talkerId] = GXX_TYPE_G726;
                    channelAC[channelId].lwG726EncodeOutputType[talkerId] = G726_DATA_TYPE_24K;
                    channelAC[channelId].lwG726DecodeInputType[talkerId] = G726_DATA_TYPE_24K;
                    channelAC[channelId].lwG726EncodeInputType[talkerId] = AUDIO_ENCODING_LINEAR;
                    break;

                case tG726_32K:
                    //test/////
                    debug("2632");
                    ///////////
                    channelAC[channelId].lwGxxType[talkerId] = GXX_TYPE_G726;
                    channelAC[channelId].lwG726EncodeOutputType[talkerId] = G726_DATA_TYPE_32K;
                    channelAC[channelId].lwG726DecodeInputType[talkerId] = G726_DATA_TYPE_32K;
                    channelAC[channelId].lwG726EncodeInputType[talkerId] = AUDIO_ENCODING_LINEAR;
                    break;

                case tG726_40K:
                    //test/////
                    debug("2640");
                    ///////////
                    channelAC[channelId].lwGxxType[talkerId] = GXX_TYPE_G726;
                    channelAC[channelId].lwG726EncodeOutputType[talkerId] = G726_DATA_TYPE_40K;
                    channelAC[channelId].lwG726DecodeInputType[talkerId] = G726_DATA_TYPE_40K;
                    channelAC[channelId].lwG726EncodeInputType[talkerId] = AUDIO_ENCODING_LINEAR;
#if 0
#ifdef AUDIO_G723_ENABLE
            /////////////just for test!!! del it!!!!
            channelAC[channelId].lwG726EncodeOutputType[talkerId] = G726_DATA_TYPE_32K;
            channelAC[channelId].lwG726DecodeInputType[talkerId] = G726_DATA_TYPE_32K;
            channelAC[channelId].lwG726EncodeInputType[talkerId] = AUDIO_ENCODING_LINEAR;
            _PRINTF("G23SetCodec");
            channelAC[channelId].lwGxxType[talkerId] = GXX_TYPE_G723_1_24_B;
#endif
#endif
                    break;

                case tG729A:
                    //test/////
                    debug("29A");
                    ///////////
                    channelAC[channelId].lwGxxType[talkerId] = GXX_TYPE_G729A;
                    break;
                case tG729B:
                    //test/////
                    debug("29B");
                    ///////////
                    channelAC[channelId].lwGxxType[talkerId] = GXX_TYPE_G729B;
                    break;

#ifdef AUDIO_G723_ENABLE
                case tG723_1_24:
                    //channelAC[channelId].lwG726EncodeOutputType[talkerId] = G726_DATA_TYPE_32K;
                    //channelAC[channelId].lwG726DecodeInputType[talkerId] = G726_DATA_TYPE_32K;
                    //channelAC[channelId].lwG726EncodeInputType[talkerId] = AUDIO_ENCODING_LINEAR;
                    channelAC[channelId].lwGxxType[talkerId] = GXX_TYPE_G723_1_24_B;
                   break; 
                   
                case tG723_1_20:
                    channelAC[channelId].lwGxxType[talkerId] = GXX_TYPE_G723_1_20_B;
                   break; 
#endif

                default: //tG711A
                    //test/////
                    debug("11ADefault");
                    ///////////
                    channelAC[channelId].lwGxxType[talkerId] = GXX_TYPE_G711A;
                    break;
            }

            pTmp++;
            gtStatistics.audioInfo[channelId][3] = *pTmp;
            switch (*pTmp) //2 sample bit
            {
                case 8:
                    break;
                case 16:
                    break;
                case 24:
                    break;
                case 32:
                    break;
                case 40:
                    break;
                default:
                    break;
            }

            pTmp++; //3 sample rate, 8000
            gtStatistics.audioInfo[channelId][4] = *pTmp;
            if (*pTmp != 8000)
                debug("Sample rate setting error! It's not 8k. It's %d!\n", *pTmp);

            pTmp++; //4 Fram Time(ms), 20
            gtStatistics.audioInfo[channelId][5] = *pTmp;
            if (*pTmp != 20)
                debug("Fram Time(ms), setting error! It's not 20ms. It's %dms!\n", *pTmp);

            pTmp++; //5 UnderLock    //1: alaw 2: ulaw 3: linear 
            gtStatistics.audioInfo[channelId][6] = *pTmp;
            switch (*pTmp) 
            {
                case 1:
                    channelAC[channelId].lwAudioInputType = AUDIO_ENCODING_ALAW;
                    channelAC[channelId].lwAudioOutputType = AUDIO_ENCODING_ALAW;
                    //channelAC[channelId].lwG726DecodeOutputType = AUDIO_ENCODING_ALAW;
                    channelAC[channelId].lwG726DecodeOutputType[talkerId] = AUDIO_ENCODING_LINEAR;
                    //set slic
                    //writeDirectReg(0x1, 0x20,channelId);      //enable PCM,a Law(0x20),u law(0x28) linear(0x38)
                    chipData[channelId].code_type = AUDIO_ENCODING_ALAW;
                    // HwSlicSetPara(channelId, PCM_ALAW);
                    break;

                case 2:
                    channelAC[channelId].lwAudioInputType = AUDIO_ENCODING_ULAW;
                    channelAC[channelId].lwAudioOutputType = AUDIO_ENCODING_ULAW;
                    //channelAC[channelId].lwG726DecodeOutputType = AUDIO_ENCODING_ULAW;
                    channelAC[channelId].lwG726DecodeOutputType[talkerId] = AUDIO_ENCODING_LINEAR;
                    //set slic
                    //writeDirectReg(0x1, 0x28,channelId);      //enable PCM,a Law(0x20),u law(0x28) linear(0x38)
                    chipData[channelId].code_type = AUDIO_ENCODING_ULAW;
                    // HwSlicSetPara(channelId, PCM_ULAW);
                    break;

                default: //linear 
                    channelAC[channelId].lwAudioInputType = AUDIO_ENCODING_LINEAR;
                    channelAC[channelId].lwAudioOutputType = AUDIO_ENCODING_LINEAR;
                    channelAC[channelId].lwG726DecodeOutputType[talkerId] = AUDIO_ENCODING_LINEAR;
                    //set slic
                    //writeDirectReg(0x1, 0x3c,channelId);      //enable PCM,a Law(0x20),u law(0x28) linear(0x38)
                    chipData[channelId].code_type = AUDIO_ENCODING_LINEAR;
                    // HwSlicSetPara(channelId, PCM_LINEAR);
                    //printf("reg[1] = %x,reg[2] = %x,reg[4] = %x\n",readDirectReg(1,channelId),readDirectReg(2,channelId),readDirectReg(4,channelId));
                    break;
            }

            pTmp++; //6 EC, 1:AEC, 2:LEC, 3:NO EC
            gtStatistics.audioInfo[channelId][7] = *pTmp;
            switch (*pTmp)
            {
                case 1:
                    channelAC[channelId].lwECType = EC_TYPE_AEC;
                    break;
                case 2:
                    channelAC[channelId].lwECType = EC_TYPE_LEC;
                    break;
                default:
                    channelAC[channelId].lwECType = EC_TYPE_NO;
                    break;
            }

            pTmp++; //7 Silence Support
            gtStatistics.audioInfo[channelId][8] = *pTmp;
            pTmp++; //8 jitterBufer Build up from empty (by ms)
            i = *pTmp;
            i &= 0xffff;
#ifdef UCOS_USE_AUDIO_JITBUF
            gtStatistics.audioInfo[channelId][9] = *pTmp;
            if (i <= 5000){
                glwJTBBuildUpFromEmptyDelayNms[channelId][talkerId] = (LONG) i;
            }
            else{
                glwJTBBuildUpFromEmptyDelayNms[channelId][talkerId] = JTB_BUILD_UP_DELAY_DEFAULT_MS;
            }

            i = (*pTmp) >> 16;
            i &= 0xffff; 
            
            if (i <= 5000 && i >= 200){   
                glwJTBDropPacketWhenHoldDataNms[channelId][talkerId] = (LONG) i;
            }
            else{   
                glwJTBDropPacketWhenHoldDataNms[channelId][talkerId] = JTB_MAX_HOLD_DATA_DEFAULT_MS;
            }   
#else
            gtStatistics.audioInfo[channelId][9] = (JTB_MAX_HOLD_DATA_DEFAULT_MS<<16)|JTB_BUILD_UP_DELAY_DEFAULT_MS;
            glwJTBBuildUpFromEmptyDelayNms[channelId][talkerId] = JTB_BUILD_UP_DELAY_DEFAULT_MS;
            glwJTBDropPacketWhenHoldDataNms[channelId][talkerId] = JTB_MAX_HOLD_DATA_DEFAULT_MS;   //40ms
#endif  
            pTmp++; //Talker ID

            AppInitTalker(channelId, talkerId);

            break;

        case tAudioStartVoice:     
            // do channel init
            channelId = *(msg + 2);
            gtStatistics.audioChannel[channelId] = 1;
            channelAC[channelId].lwTalkerEnableBitFlag |= 1;
            channelAC[channelId].lwCurrentActiveTalkerCount++;
            AppInitChannelEx(channelId);
            channelAC[channelId].bChannelEnable = TRUE;

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


            if((gtavsync.syncmode & SYNCMASK) == 0)       //linux not setup the syncmode
                gtavsync.syncmode = AVSYNC;

		debug("\nTwoRiscAudioTA task create success!\n");      

            if(
               #ifdef CHIP_CFG_STACK_CHECK
               OSTaskCreateExt(TwoRiscAudioTA, (void *)0, (void *)&StkTwoRiscAudioTA[sizeof(StkTwoRiscAudioTA) / sizeof(StkTwoRiscAudioTA[0]) - 1], PRIOR_TASK_TA,\
                        PRIOR_TASK_TA, (void *)&StkTwoRiscAudioTA[0], sizeof(StkTwoRiscAudioTA) / sizeof(StkTwoRiscAudioTA[0]), (void *)0, OS_TASK_OPT_STK_CHK+OS_TASK_OPT_STK_CLR) != OS_NO_ERR	
               #else                    
                OSTaskCreate(TwoRiscAudioTA, (void *)0, (void *)&StkTwoRiscAudioTA[STK_SIZE_TwoRiscAudioTA - 1], PRIOR_TASK_TA) != OS_NO_ERR
               #endif
            ) {
                debug("\nTwoRiscAudioTA task create error!\n");            
            }
            
            break;

    case tAudioStopVoice:

        channelId = *(msg + 2);
        gtStatistics.audioChannel[channelId] = 0;
        channelAC[channelId].lwTalkerEnableBitFlag = 0;
        channelAC[channelId].lwCurrentActiveTalkerCount = 0;
        channelAC[channelId].bChannelEnable = FALSE;

        for(i = 0; i < MAX_AUDIO_CHANNEL; i++){
            if (channelAC[i].bChannelEnable){
                break;
            }
        }

        if (i >= MAX_AUDIO_CHANNEL){
            if((gtavsync.syncmode&SYNCMASK) == 0)  //linux not setup the sync mode
                gtavsync.syncmode = SELFSYNC;                
            AppAudioResetReEnable();
#ifdef AUDIO_TEST_ENABLE
            test_disable();
#endif
        }

        debug("tAudioStopVoice\n");

        if(RiscRdID() == RISC1) {
            debug("\r\nRISC1: audio sub_type error!\n");                    
            break;
        }    
#if 0
        //gTwoRiscSystemMonitorMsg = TWO_RISC_SYSTEM_CLOSE_VOICE;
        //err = OSQPost(gptQMsgTwoRiscSystemMonitor, &gTwoRiscSystemMonitorMsg);
        err = OSQPost(gptQMsgTwoRiscSystemMonitor, TWO_RISC_SYSTEM_CLOSE_VOICE);
        if(err != OS_NO_ERR)            
        debug("\r\nRISC0: OSMboxPost error! err = %d!\r\n", err);
        else
        debug("\r\nRISC0: OSMboxPost ok!\r\n");                
#endif

        if(AppAudioChannelClose(channelId) == SUCCESSFUL) {
            debug("\nCloseAudioChannel(%d)ok\n", channelId);                 
        } else
            debug("\nCloseAudioChannel(%d)failed\n", channelId);  

#ifdef DUMP_ENABLE
        AppReadDumpedData(0);
        AppReadDumpedData(1);
        AppReadDumpedData(2);
        AppReadDumpedData(3);
        AppReadDumpedData(4);
        AppReadDumpedData(5);
        AppReadDumpedData(6);
        AppReadDumpedData(7);
        AppReadDumpedData(8);
        AppReadDumpedData(9);
#endif
        break;

    case tAudioTransferLinuxToUcos:    // put to jitter buffer            
        if(RiscRdID() == RISC1) {
            debug("\r\nRISC1: audio sub_type error!\r\n");                    
            break;
        }    
        gtStatistics.audio_linux_to_ucos_ps++;
        gtAudioLinuxToUcos.channelID = *(msg + 2);
        gtAudioLinuxToUcos.addr = *(msg + 3);
        gtAudioLinuxToUcos.len = *(msg + 4);
        talkerId = (*(msg + 5)) & 0xffff; 
        channelId = *(msg + 2);
        gtAudioLinuxToUcos.talkerID = talkerId;
        gtAudioLinuxToUcos.timeStamp = *(msg + 6);

#ifdef DEBUG_AVSYNC
        gtStatistics.audio_in_ts = gtAudioLinuxToUcos.timeStamp;
#endif
#ifdef AV_SYNC        
        channel = gtAudioLinuxToUcos.channelID;
        if(LinuxToUcosATs[channel] >= gtAudioLinuxToUcos.timeStamp) {
            printf("linux to ucos audio ts error,ts=%d,channelID=%d!\n", LinuxToUcosATs[channel], channel);
            RiscDecCNT(AUDIO_LINUX_TO_UCOS_CNT, 1);
            LinuxToUcosATs[channel] = gtAudioLinuxToUcos.timeStamp;
            break;
        }
        if((gtAudioLinuxToUcos.timeStamp - LinuxToUcosATs[channel]) > glwInterruptTimeScale)
            printf("linux to ucos audio lost frame ts=(%d,%d),channelID=%d!\n", LinuxToUcosATs[channel], gtAudioLinuxToUcos.timeStamp, channel);
        
        LinuxToUcosATs[channel] = gtAudioLinuxToUcos.timeStamp;
#endif      
//        if(RiscRdCNT(AUDIO_LINUX_TO_UCOS_CNT) > 0) {
            if (channelAC[gtAudioLinuxToUcos.channelID].bChannelEnable){
                i = channelAC[gtAudioLinuxToUcos.channelID].lwTalkerEnableBitFlag >> talkerId;
                i &= 0x1;
                if (i != 0){
                    //FetchPackData((BYTE*) gtAudioLinuxToUcos.addr, \
                    gtAudioLinuxToUcos.len, \
                    gtAudioLinuxToUcos.channel_number, \
                    talkerId);
                    FetchPackData(&gtAudioLinuxToUcos);
                }
                else{
                    debug(")%d", talkerId);
                }
            }
            else{
                debug("(%d", gtAudioLinuxToUcos.channelID);
            }
#ifndef UCOS_USE_AUDIO_JITBUF
extern LONG lwJitterBufferDepth[JITTER_BUFFER_CHANNEL_MAX][JITTER_BUFFER_TALKER_MAX];
extern LONG glwJitterBufferDropPacketFromDepth[JITTER_BUFFER_CHANNEL_MAX][JITTER_BUFFER_TALKER_MAX];
            if((lwJitterBufferDepth[channelId][talkerId] < (glwJitterBufferDropPacketFromDepth[channelId][talkerId] - channelAC[channelId].lwJitterBufferReadBytes[talkerId]))
                && ((RiscRdCNT(AUDIO_LINUX_TO_UCOS_CNT) > 0)))
            {
                RiscDecCNT(AUDIO_LINUX_TO_UCOS_CNT, 1);
            }
#endif
            //FetchPackData(gtAudioLinuxToUcos.channel_number);
            //if(AppAudioWriteData(0 /* channel_number */, gtAudioLinuxToUcos.addr, AUDIO_MAX_FRAME_LEN))
            //        debug("AppAudioWriteData ok");;    

            //RiscDecCNT(AUDIO_LINUX_TO_UCOS_CNT, gtAudioLinuxToUcos.len / AUDIO_MAX_FRAME_LEN);            
//        }
#ifdef UCOS_USE_AUDIO_JITBUF
        RiscDecCNT(AUDIO_LINUX_TO_UCOS_CNT, 1); 
#endif
#ifdef AUDIO_TEST_ENABLE
        talker_info[channelId][talkerId].ucos_rx++;           
#endif
        break;

    case tAudioTransferUcosToLinux:
        if(RiscRdID() == RISC0) {
            debug("\r\nRISC0: audio sub_type error!\r\n");
            break;
        }     else            
            ;//debug("\r\nRISC1: tAudioTransferUcosToLinux!\r\n");

#ifdef DEBUG_MIX
        static LONG *plwRef1 = ref1;
        static LONG *plwRefa = refa;

        pPacket = (unsigned int *)&risc_packet;
        risc_packet.start = RISC_PKT_START;
        risc_packet.main_type = tAudio;
        risc_packet.sub_type = tAudioTransferLinuxToUcos;
        risc_packet.length = 0x14;
        risc_packet.status = 0;

        *(pPacket + 2) = channelId;
        risc_packet.end = RISC_PKT_END;

        if((unsigned int)audio_linux_to_ucos_addr >= (unsigned int)(AUDIO_LINUX_TO_UCOS_E_ADDR - 320)){
            audio_linux_to_ucos_addr = (unsigned int *)AUDIO_LINUX_TO_UCOS_B_ADDR;                               
        }

        *(pPacket + 3) = (int)audio_linux_to_ucos_addr;

        length = *(msg + 4);
        *(pPacket + 4) = length;

        talkerId = *(msg + 5) & 0xffff;
        *(pPacket + 5) = talkerId;            

        if (talkerId == 0){
            if ((ULONG)plwRef1 + 320 >= (ULONG)ref1 + sizeof(ref1)){
                plwRef1 = ref1;
            }
            pTmp  = plwRef1;
            ((ULONG) plwRef1) += (ULONG)length;
        }
        else{
            if ((ULONG)plwRefa + 320 >= (ULONG)refa + sizeof(refa)){
                plwRefa = refa;
            }
            pTmp  = plwRefa;
            ((ULONG) plwRefa) += (ULONG)length;
        }

        for(i = 0; i < length; i+=4){
            *audio_linux_to_ucos_addr++ = *pTmp++;
        }

#else        
        length = *(msg + 4);

        pPacket = (unsigned int *)&risc_packet;            
        //for(i = 0; i < *(msg + 2); i++) {
        // read from audio input, and write into the share memory
        pTmp = (int *)*(msg + 3);
        risc_packet.start = RISC_PKT_START;
        risc_packet.main_type = tAudio;
        risc_packet.sub_type = tAudioTransferLinuxToUcos;
        risc_packet.length = 0x10;
        risc_packet.status = 0;
        *(pPacket + 2) = channelId;

        if((unsigned int)audio_linux_to_ucos_addr >= (unsigned int)(AUDIO_LINUX_TO_UCOS_E_ADDR - 320))
            audio_linux_to_ucos_addr = (unsigned int *)AUDIO_LINUX_TO_UCOS_B_ADDR;                               

        *(pPacket + 3) = (int)audio_linux_to_ucos_addr;
        //*(pPacket + 4) = AUDIO_MAX_FRAME_LEN;                
        *(pPacket + 4) = length;   
        talkerId = *(msg + 5) & 0xffff;
        *(pPacket + 5) = talkerId;
        *(pPacket + 6) = *(msg + 6);
        //risc_packet.message[0] = 0;
        //risc_packet.message[1] = (int)audio_linux_to_ucos_addr;
        //risc_packet.message[2] = AUDIO_MAX_FRAME_LEN;        
        risc_packet.end = RISC_PKT_END;

        //debug("@, len=%d ", length);

        for(i = 0; i < length; i+=4) {
            //CommInqSendChar(UART_CHANNEL, '8');
            *audio_linux_to_ucos_addr++ = *pTmp++;
            //CommInqSendChar(UART_CHANNEL, '9');                    
            //CommInqSendChar(UART_CHANNEL, '0');                    
        }

        //debug("(");
#endif

        RiscDecCNT(AUDIO_UCOS_TO_LINUX_CNT, 1);

        //debug(")");

        if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
            RiscIncCNT(AUDIO_LINUX_TO_UCOS_CNT, 1);    // increase count by 1
            RiscFlagSet();    // Set risc1 flag to interrupt risc1            
            //debug("\r\nRISC1: TwoRiscCommSendPacket!\r\n");
        } else 
            debug("\r\nTwoRiscCommProcAudio: send packet error!\r\n");
        //}            
        break;

    case tAudioStartTone:
        tonetype = *(msg +3);
        debug("tone type:%d  channel id:%d  \n",tonetype,channelId);
        HwSlicStartTone(tonetype,channelId);
        break;

    case tAudioStopTone:
        debug("Slic-Stop-Tone:\r\n");
        chipData[channelId].stoptone = 1;
        break;

    case tAudioPlayPCMData:
        break;

    case tAudioTransmitKeyFromUcosToLinux:
        break;

    case tSendCallerID:
        ProcessCallid((char *)(*(msg+3)), (int)(*(msg+4)), (int)channelId);
        break;

    case tAddOneTalker:
        channelId = *(msg + 2);
        talkerId = *(msg + 3);

        if (channelId >= MAX_AUDIO_CHANNEL)
        {
            debug("tAddOneTalker err! channelId=%d\n", channelId);
            break;
        }

        if (talkerId >= MAX_TALKER_PER_CHANNEL)
        {
            debug("tAddOneTalker err!, talkerId=%d\n", talkerId);
            break;
        }

        if (channelAC[channelId].lwCurrentActiveTalkerCount >= MAX_TALKER_PER_CHANNEL-1)
        {
            debug("tAddOneTalker err!, Add too much talker\n");
            break;
        }

        i = channelAC[channelId].lwTalkerHaveSetCodecBitFlag >> talkerId;
        i &= 0x1;

        if (i == 0)
        {
            debug("tAddOneTalker err!, talker codec is not set\n");
            break;
        }

        i = 0x1;
        i = i << talkerId;
        channelAC[channelId].lwTalkerEnableBitFlag |= i;
        i = ~i;
        channelAC[channelId].lwTalkerMaskBitFlag &= i;
        channelAC[channelId].lwCurrentActiveTalkerCount++;

        break;

    case tDelOneTalker:
        channelId = *(msg + 2);
        talkerId = *(msg + 3);

        if (channelId >= MAX_AUDIO_CHANNEL)
        {
            debug("tDelOneTalker err! channelId=%d\n", channelId);
            break;
        }

        if (talkerId >= MAX_TALKER_PER_CHANNEL)
        {
            debug("tDelOneTalker err!, talkerId=%d\n", talkerId);
            break;
        }

        if (channelAC[channelId].lwCurrentActiveTalkerCount == 1)
        {
            debug("tDelOneTalker err!, Del too much talker, only 1 is left\n");
            break;
        }

        i = 0x1;
        i <<= talkerId;
        i = ~i;
        channelAC[channelId].lwTalkerEnableBitFlag &= i;
        channelAC[channelId].lwTalkerHaveSetCodecBitFlag &= i;
        channelAC[channelId].lwTalkerMaskBitFlag &= i;
        channelAC[channelId].lwCurrentActiveTalkerCount--;

        break;

    case tSetSlicRegister:
       // chipData[channelId].setreg = 1;
        reg_msg = *(msg +3);
	  Ins_Queue(&chipData[channelId].workqueue, reg_msg);
        //TwoRiscSetReg(*(msg +3),channelId);
        break;
    case tSetEnAudioMode:
        /*
        if((*(msg + 3)) & (0x1 << 16))
        {
            gAudioCodecMode[channelId] |= (*(msg + 3)) & 0x3;
            debug("channel %d code mode on %x  \n", channelId, gAudioCodecMode[channelId]);
        }
        else
        {
            gAudioCodecMode[channelId] &= ~((*(msg + 3)) & 0x3);
            debug("channel %d code mode off %x  \n", channelId, gAudioCodecMode[channelId]);
        }
        */ 
        break;
    case tSetVoiceUP:
        AudioVoice++;
#ifdef CODEC_TLV320AIC23B
        aic23_set_volume(AudioVoice, AudioVoice);
#endif

#ifdef CODEC_TW2835
	if( AudioVoice > 15 ) AudioVoice = 0x0f;
	AppTW2835AudioSetVolume(TW2835_MASTER, channel, AudioVoice, AudioVoice);
#endif

        break;
    case tSetVoiceDown:
#ifdef CODEC_TLV320AIC23B
        AudioVoice--;
        aic23_set_volume(AudioVoice, AudioVoice);
#endif

#ifdef CODEC_TW2835
	if( AudioVoice != 0 ) AudioVoice--;
	AppTW2835AudioSetVolume(TW2835_MASTER, channel, AudioVoice, AudioVoice);
#endif


        break;
    default:
           debug("\r\naudio sub_type error: 0x%x!\r\n", sub_type);        
            break;
    }      
}
#endif

