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



#define TWO_RISC_SYSTEM_GLOBALS

#include "chip_cfg.h"

#include "risc.h"
#include "Sc.h"
#include "Mc.h"
#include "comm.h"
#include "includes.h"

#include "os_cpu.h"
#include "os_cfg.h"
#include "uCOS_II.h"

#include "bsp.h"
#include "two_risc_system.h"
#include "debug_risc1_cfg.h"

#include "ucos_task_priority.h"
#include "two_risc_comm.h"
#include "2risc_memory_map.h"
#include "2risc_comm_protocol.h"
//#include "video/two_risc_video.h"
#include "cctl.h"
#include "Band_adjust.h"
#include "VIN_SET.h"

#include "iic.h"
#include "printf.h"

#ifdef VIDEO_MODULE_INCLUDE
#include "video_cfg.h"
#include "two_risc_video.h"
#include "video_api.h"
#include "VIN_API.h"
#include "mpeg4_decoder.h"
#include "two_risc_display.h"
#include "VSC_Task.h"
#include "VSC_API.h"
#include "video_buffer.h"
#include "video/keypad_remote_control/remote_control.h"
#include "video/alarm.h"
#include "video/keypad_remote_control/keypad.h"
#include "gpio.h"
#ifdef DVR
#include "video/techwell/hw2835_md.h"
#endif
#include "timestamp.h"

unsigned int gVideoStatus;
OS_EVENT *QEeventVodeoL2U;

#ifdef DISPLAY_INCLUDE
#include "audio.h"
#endif

#ifdef DISPLAY_INCLUDE
void *pQVideoL2UPackage[VIDEO_L2U_PKG_NUMBER];
extern  unsigned int gVoutEnable;
#endif

#endif

#ifdef AUDIO_MODULE_INCLUDE
#include "audio_cfg.h"
#include "audio.h"
#include "two_risc_audio.h"
#include "audio_api.h"
#include "proslic.h"
#include "audio/w681388/slic_test.h"
#ifdef AUDIO_TEST_ENABLE
#include "test.h"
#endif
#endif

#ifdef TASK_SYSTEM_CONTROL_EN
#include "two_risc_pip_ctrl_risc1.h"
#include "two_risc_resolution_ctrl_risc1.h"
extern unsigned int PipSwitchCnt;
extern unsigned int ResolutionSwitchCnt;
#endif

#define RISC1_DEBUG_BASIC_VIDEO                /*I:open ipcam task D:disable ipcam V:enable vsc task
                                                *F:disable vout X:disable vout&vsc&del the task Y:enable rvo
                                                *U:disable rvo P:enable PIP O:idsable PIP
                                                */
#ifdef DVR//*******below is only use in DVR************
#define RISC1_DEBUG_NTSC_PAL                      //J R r j
#define RISC1_DEBUG_DVR                                 //@ # $ % ^ & * ( ) [ ] { } : ; ? < >,4,5,6,7
#define RISC1_DEBUG_RESOLUTION                  //C:CIF  Q:QVGA  W:VGA  >:SXGA  Z:QCIF  S:NTSC_D1  L:QQVGA  T:PAL_D1 [:NTSC_CIF ]:NTSC_QCIF B:SUB_QCIF
#else             

//#define RISC1_DEBUG_DECODE                          //M N K
#define RISC1_DEBUG_NTSC_PAL                      //J R r j

#define RISC1_DEBUG_OSD                                 //A E 1 2 3 4 5 6 7 8, a
//#define RSIC1_DEBUG_DECODE_DEBLK                     //m:enable deblocking n:disable deblocking
#define RSIC1_DEBUG_CAM_AUTODETECT_EN_DIS            //m:enable autodetect n:disable autodetect
//#define RISC1_DEBUG_RVO_DISPLAY_MODE         // 0 1 2
//#define RISC1_DEBUG_SCALE                              //0 9 5
//#define RISC1_DEBUG_RVO_SCALE                      // 123456 
//#define RISC1_DEBUG_AUTO_COMMD_SEND             //6,7:create and delete resolution switch task. 8,9:create and delete PIP switch task
#define RISC1_DEBUG_MOTION_DETECTION     //H G
//#define RISC1_DEBUG_OTHERS                           //T B
#define RISC1_DEBUG_RESOLUTION                  //C:CIF  Q:QVGA  W:VGA  >:SXGA  Z:QCIF  S:NTSC_D1  L:QQVGA  T:PAL_D1 [:NTSC_CIF ]:NTSC_QCIF B:SUB_QCIF
//#define RISC1_DEBUG_FRAMERATE                    //R
//#define RISC1_DEBUG_AUDIO                             //! | g i w p t o c s e a d m v f 
//#define RISC1_DEBUG_EVEN_ODD                          //J R M N H B
//#define RISC1_DEBUG_INTERLACED_PROGRESSIVE  // A E
//#define RISC1_DEBUG_ANALOG_DITITAL_CAMERA   // 1 2
//#define RISC1_DEBUG_IMAGE_SETTING                   //1:contrast  2:effect  3:exposure  4:saturation  5:brightness  6:hue  7sharpness 
//#define RISC1_DEBUG_IMAGE_MIRROR_FLIP          //1:normal,2:mirror,3:flip,4,mirror and flip  
//#define RISC1_DEBUG_ENCODE_MODE               // H:H263 mode  M:mpeg mode
//#define RISC1_DEBUG_PIP_EFFECT               // G:good PIP effect; K:bad PIP effect; 8:pip 1/4; 9:pip 1/16; 0:1/64

//#define RISC1_DEBUG_ALARMIN                     // 1 2 3 4 5 6 7 8
//#define RISC_DEBUG_SET_CLOCK                    //i e

#endif

/*
***************************************************************************
*                          TaskTwoRiscSystemMonitorRisc1
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
#ifdef RISC1_DEBUG_OSD      //debug osd scroll string
unsigned char ascii[65] = {33,34,35,36,37,38,39,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,
                                        82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100,101,102,103,104,105,
                                        106,107,108,109,110,111,112,113,114,115,116,117,118,119,120,121,122};
char *rollstring[3] = {  ascii,
                                    "it's scrollstring!",                                                                        //short one
                                    "it's wangyong's testing program, to test osd scrollstring, thanks!"    //long one
                                };
#endif

unsigned char AdjustOSDAttribute = 0;
unsigned char TW2835OUTPUT = 0;
unsigned char TW2835BitMapPage = 0;
unsigned char TW2835SetOutMode = 0;
unsigned char stringcolor = 0;
extern unsigned char color_init_index[];
unsigned char DvrIinitCh = 0;
unsigned char DvrSwitchCh = 0;
void TaskTwoRiscSystemMonitorRisc1(void *data)
{
    int i,j, cnt_for_TaskStatisticsPrintTaskInfo = 0;  
    unsigned char err;
    unsigned int msg; 
    char receive, receive1;
    unsigned int *pInt;     
    OS_CPU_SR cpu_sr;
    Risc_Packet_t risc_packet;         

    //add by hjm
    unsigned short width;
    unsigned short height;

    signed short int MagnificationHori = 0;
    signed short int MagnificationVerti = 0;
    int mode = 1;

    gtPrintMsgControl.en_print_all_msg = 1;
    gtPrintMsgControl.en_print_periodical_msg = 0;

#ifdef AUDIO_MODULE_INCLUDE
#define MSG_BODY_ADDR 0xa1080000

//for Audio loopback test/////////////////////

    int channelID = 0;
    int context;
  
    int OpenAudioMsgCMD[] = {channelID, 1/* 1: codec; 0: slic */};
#if 0
    static int SetAudioCodecMsgG711UNoEC[] =  { 0, 8, 8000, 20, 2, 3, 0};
    static int SetAudioCodecMsgG711ANoEC[] =  { 8, 8, 8000, 20, 1, 3, 0};
    static int SetAudioCodecMsgG726_16kNOEC[] ={98,16, 8000, 20, 3, 3, 0};
    static int SetAudioCodecMsgG726_24kNOEC[] ={97,16, 8000, 20, 3, 3, 0};
    static int SetAudioCodecMsgG726_32kNOEC[] ={ 2,16, 8000, 20, 3, 3, 0};
    static int SetAudioCodecMsgG726_40kNOEC[] ={96,16, 8000, 20, 3, 3, 0};
    static int SetAudioCodecMsgG729ANOEC[] =  {18,16, 8000, 20, 3, 3, 0};
#endif

    int talkerID = 1;
#ifdef CODEC_TLV320AIC23B
    int SetAudioCodecMsgG711UNoEC[] = { 0, 8, 8000, 20, 3, 3, 0, 20, talkerID};
    int SetAudioCodecMsgG711ANoEC[] = { 8, 8, 8000, 20, 3, 3, 0, 20, talkerID};
    int SetAudioCodecMsgG726_16kNoEC[] ={98,16, 8000, 20, 3, 3, 0, 20, talkerID};
    int SetAudioCodecMsgG726_24kNoEC[] ={97,16, 8000, 20, 3, 3, 0, 20, talkerID};
    int SetAudioCodecMsgG726_32kNoEC[] ={ 2,16, 8000, 20, 3, 3, 0, 20, talkerID};
    int SetAudioCodecMsgG726_40kNoEC[] ={96,16, 8000, 20, 3, 3, 0, 20, talkerID};
    int SetAudioCodecMsgG729ANoEC[] = {18,16, 8000, 20, 3, 3, 0, 20, talkerID};
    int SetAudioCodecMsgG723_1_24NoEC[] = {723124,16, 8000, 20, 1, 1, 0, 0xc80014, talkerID};
#else
    int SetAudioCodecMsgG711UNoEC[] = { 0, 8, 8000, 20, 2, 3, 0, 0x3200190, talkerID}; //0xc80014, JTBE=20ms, JTBF=200ms
    int SetAudioCodecMsgG711ANoEC[] = { 8, 8, 8000, 20, 1, 3, 0, 0x3200190, talkerID};
    int SetAudioCodecMsgG726_16kNoEC[] ={98,16, 8000, 20, 1, 1, 0, 0xc80014, talkerID};
    int SetAudioCodecMsgG726_24kNoEC[] ={97,16, 8000, 20, 1, 1, 0, 0xc80014, talkerID};
    int SetAudioCodecMsgG726_32kNoEC[] ={ 2,16, 8000, 20, 1, 1, 0, 0xc80014, talkerID};
    int SetAudioCodecMsgG726_40kNoEC[] ={96,16, 8000, 20, 1, 1, 0, 0xc80014, talkerID};
    int SetAudioCodecMsgG729ANoEC[] = {18,16, 8000, 20, 1, 1, 0, 0xc80014, talkerID};
    int SetAudioCodecMsgG723_1_24NoEC[] = {tG723_1_24,16, 8000, 20, 1, 1, 0, 0xc80014, talkerID};
#endif

    int SetAudioCodecMsg[] = { channelID, (unsigned int) MSG_BODY_ADDR };

    memcpy((int*)MSG_BODY_ADDR, SetAudioCodecMsgG711ANoEC, sizeof(SetAudioCodecMsgG711UNoEC));
    
//test1/////////
//extern tDumpContext DumpContext[MAX_DUMP_POINTS];
////////////////////
#endif  
    BspStartTicker(OS_TICKS_PER_SEC);             

#ifdef PRINT_IN_TASK
    print_init_buffer();

    #ifdef CHIP_CFG_STACK_CHECK
    OSTaskCreateExt(TaskTwoRiscPrint, (void *)0, (void *)&StkTaskTwoRiscPrint[sizeof(StkTaskTwoRiscPrint) / sizeof(StkTaskTwoRiscPrint[0]) - 1], PRIOR_TASK_PRINT,\
               PRIOR_TASK_PRINT, (void *)&StkTaskTwoRiscPrint[0], sizeof(StkTaskTwoRiscPrint) / sizeof(StkTaskTwoRiscPrint[0]), (void *)0, OS_TASK_OPT_STK_CHK+OS_TASK_OPT_STK_CLR);
    #else                    
    OSTaskCreate(TaskTwoRiscPrint, (void *)0, (void *)&StkTaskTwoRiscPrint[STK_SIZE_TaskTwoRiscPrint - 1], PRIOR_TASK_PRINT);
    #endif

#endif
       
    printf("\r\n");
    printf("**********************************************************************\r\n");
    printf("*                  Risc%d: Two Risc System Monitor Task             *\r\n", RiscRdID());    
    printf("**********************************************************************\r\n");     
    printf("\r\n");
    
    RiscIntrInit();    
    RiscFlagIntrInit();    

    gptQMsgTwoRiscSystemMonitor = OSQCreate(&ArrayQMsgSysMonitor[0], ArrayQMsgSysMonitor_MAX);

    if(RiscRdID() == RISC1) {
#ifdef AUDIO_MODULE_INCLUDE
        audio_linux_to_ucos_addr = (unsigned int *)AUDIO_LINUX_TO_UCOS_B_ADDR;
        //if(OSTaskCreate(TwoRiscAudioTARisc1, (void *)0, (void *)&StkTwoRiscAudioTA[STK_SIZE_TwoRiscAudioTA - 1], 6) != OS_NO_ERR)
        //    debug("\r\n                           TaskTwoRiscSystemMonitorRisc1: TwoRiscAudioTARisc1 task create error!\r\n");      
#endif    

#ifdef VIDEO_MODULE_INCLUDE
        if(
            #ifdef CHIP_CFG_STACK_CHECK
            OSTaskCreateExt(TaskTwoRisc1IPCamera, (void *)0, (void *)&StkTaskTwoRisc1IPCamera[sizeof(StkTaskTwoRisc1IPCamera) / sizeof(StkTaskTwoRisc1IPCamera[0]) - 1], PRIOR_TASK_RISC1IPCAM,\
                    PRIOR_TASK_RISC1IPCAM, (void *)&StkTaskTwoRisc1IPCamera[0], sizeof(StkTaskTwoRisc1IPCamera) / sizeof(StkTaskTwoRisc1IPCamera[0]), (void *)0, OS_TASK_OPT_STK_CHK+OS_TASK_OPT_STK_CLR) != OS_NO_ERR	
            #else                    
             OSTaskCreate(TaskTwoRisc1IPCamera, (void *)0, (void *)&StkTaskTwoRisc1IPCamera[TaskTwoRiscMpegEncode_STK_SIZE - 1], PRIOR_TASK_RISC1IPCAM) != OS_NO_ERR
            #endif            
        )
            debug("\r\n                           TaskTwoRiscSystemMonitorRisc1: TaskTwoRisc1IPCamera task create error!\r\n");     
#endif          
    }
  
    while(1) {

        if(cnt_for_TaskStatisticsPrintTaskInfo++ > 400) {   // print every 20s
            TaskStatisticsPrintTaskInfo();
            cnt_for_TaskStatisticsPrintTaskInfo = 0;
        }
        
        msg = (unsigned int)OSQPend(gptQMsgTwoRiscSystemMonitor, 500 * OS_TICKS_PER_SEC /1000, &err);
        if(err == OS_NO_ERR)
        {
            switch(msg) {
                
                case TWO_RISC_SYSTEM_RECEIVE_LINUX_MSG:
                    while(1) {
                        if(TwoRiscCommRecPacket() != SUCCESSFUL) {
                            break;                
                        }
                    }                        
                    break;

                default:
                    printf("\nTaskTwoRiscSystemMonitorRisc1: type(%d) error!\n", msg);                    
                    break;
            }
        }

        
        if(CheckPressKey(&receive)) {
            //printf("%c", receive);    
            switch(receive) { 
                
                //Video Command
#ifdef VIDEO_MODULE_INCLUDE   

#ifdef DUMP_DECODE_OUTPUT
                case '~':    // open ipcam task 
                    pInt = (unsigned int *)&risc_packet;
          
                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tVideo;
                    risc_packet.sub_type = 0xfa;
                    risc_packet.length = 0x4;
                    risc_packet.status = 0;   
                    risc_packet.end = RISC_PKT_END;   
                    if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                        RiscFlagSet();    // Set risc1 flag to interrupt risc1            
                    } else 
                        debug("\r\n                           TwoRiscAudioTARisc1: send packet error!\r\n");            
                    break;

                case '!':    // open ipcam task 
                    pInt = (unsigned int *)&risc_packet;
          
                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tVideo;
                    risc_packet.sub_type = 0xef;
                    risc_packet.length = 0x4;
                    risc_packet.status = 0;   
                    risc_packet.end = RISC_PKT_END;   
                    if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                        RiscFlagSet();    // Set risc1 flag to interrupt risc1            
                    } else 
                        debug("\r\n                           TwoRiscAudioTARisc1: send packet error!\r\n");            
                    break;
                    
#endif
#ifdef RISC1_DEBUG_BASIC_VIDEO
                case 'I':    // open ipcam task 
                    pInt = (unsigned int *)&risc_packet;
          
                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tVideo;
                    risc_packet.sub_type = tVideoEnableDisable;
                    risc_packet.length = 0x4;
                    risc_packet.status = 0;   
                    *(pInt + 2) = 1;    // 1: ENABLE; 0: DISABLE    
                    *(pInt + 3) = 1;                       
                    risc_packet.end = RISC_PKT_END;   
                    if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                        RiscFlagSet();    // Set risc1 flag to interrupt risc1            
                    } else 
                        debug("\r\n                           TwoRiscAudioTARisc1: send packet error!\r\n");            
                    break;

                case 'D':    // disable ipcam 
                    pInt = (unsigned int *)&risc_packet;
          
                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tVideo;
                    risc_packet.sub_type = tVideoEnableDisable;
                    risc_packet.length = 0x4;
                    risc_packet.status = 0;   
                    *(pInt + 2) = 0;    // 1: ENABLE; 0: DISABLE                    
                    *(pInt + 3) = 1;    // 1: ipcam
                    risc_packet.end = RISC_PKT_END;   
                    if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                        RiscFlagSet();    // Set risc1 flag to interrupt risc1            
                    } else 
                        debug("\r\n                           TwoRiscAudioTARisc1: send packet error!\r\n");            
                    break;                    

                case 'V':    // enable vsc task 
                    pInt = (unsigned int *)&risc_packet;
          
                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tVideo;
                    risc_packet.sub_type = tVideoEnableDisable;
                    risc_packet.length = 0x4;
                    risc_packet.status = 0;   
                    *(pInt + 2) = 1;    // 1: ENABLE; 0: DISABLE    
                    *(pInt + 3) = 2;    // 2: vout                   
                    risc_packet.end = RISC_PKT_END;   
                    if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                        RiscFlagSet();    // Set risc1 flag to interrupt risc1            
                    } else 
                        debug("\r\n                           TwoRiscAudioTARisc1: send packet error!\r\n");            
                    break;    

                case 'F':    // disable vout, at the same the vsc is disabled!  
                    pInt = (unsigned int *)&risc_packet;
          
                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tVideo;
                    risc_packet.sub_type = tVideoEnableDisable;
                    risc_packet.length = 0x4;
                    risc_packet.status = 0;   
                    *(pInt + 2) = 0;    // 1: ENABLE; 0: DISABLE                    
                    *(pInt + 3) = 2;    // 2: vout
                    risc_packet.end = RISC_PKT_END;   
                    if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                        RiscFlagSet();    // Set risc1 flag to interrupt risc1            
                    } else 
                        debug("\r\n                           TwoRiscAudioTARisc1: send packet error!\r\n");            
                    break;

                case 'X':    // disable vout & vsc & del the task!  
                    pInt = (unsigned int *)&risc_packet;
          
                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tVideo;
                    risc_packet.sub_type = 0xfe;
                    risc_packet.length = 0x4;
                    risc_packet.status = 0;   
                    risc_packet.end = RISC_PKT_END;   
                    if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                        RiscFlagSet();    // Set risc1 flag to interrupt risc1            
                    } else 
                        debug("\r\n                           TwoRiscAudioTARisc1: send packet error!\r\n");            
                    break;                                        

                case 'Y':    // enable rvo  
                    pInt = (unsigned int *)&risc_packet;
          
                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tVideo;
                    risc_packet.sub_type = tVideoEnableDisable;
                    risc_packet.length = 0x4;
                    risc_packet.status = 0;   
                    *(pInt + 2) = 1;    // 1: ENABLE; 0: DISABLE    
                    *(pInt + 3) = 0x10 | 0x4;    // 0x10: rvo                   
                    risc_packet.end = RISC_PKT_END;   
                    if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                        RiscFlagSet();    // Set risc1 flag to interrupt risc1            
                    } else 
                        debug("\r\n                           TwoRiscAudioTARisc1: send packet error!\r\n");            
                    break;

                case 'U':    // disable rvo
                    pInt = (unsigned int *)&risc_packet;
          
                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tVideo;
                    risc_packet.sub_type = tVideoEnableDisable;
                    risc_packet.length = 0x4;
                    risc_packet.status = 0;   
                    *(pInt + 2) = 0;    // 1: ENABLE; 0: DISABLE                    
                    *(pInt + 3) = 0x10 | 0x4;    // 0x10: rvo
                    risc_packet.end = RISC_PKT_END;   
                    if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                        RiscFlagSet();    // Set risc1 flag to interrupt risc1            
                    } else 
                        debug("\r\n                           TwoRiscAudioTARisc1: send packet error!\r\n");            
                    break;                                 

                case 'P':    // enable pip  
                    pInt = (unsigned int *)&risc_packet;
          
                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tVideo;
                    risc_packet.sub_type = tVideoEnableDisable;
                    risc_packet.length = 0x4;
                    risc_packet.status = 0;   
                    *(pInt + 2) = 1;    // 1: ENABLE; 0: DISABLE    
                    *(pInt + 3) = 8|0x4;    // 8: pip                   
                    risc_packet.end = RISC_PKT_END;   
                    if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                        RiscFlagSet();    // Set risc1 flag to interrupt risc1            
                    } else 
                        debug("\r\n                           TwoRiscAudioTARisc1: send packet error!\r\n");            
                    break;

                case 'O':    // disable pip
                    pInt = (unsigned int *)&risc_packet;
          
                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tVideo;
                    risc_packet.sub_type = tVideoEnableDisable;
                    risc_packet.length = 0x4;
                    risc_packet.status = 0;   
                    *(pInt + 2) = 0;    // 1: ENABLE; 0: DISABLE                    
                    *(pInt + 3) = 8|0x4;    // 8: pip
                    risc_packet.end = RISC_PKT_END;   
                    if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                        RiscFlagSet();    // Set risc1 flag to interrupt risc1            
                    } else 
                        debug("\r\n                           TwoRiscAudioTARisc1: send packet error!\r\n");            
                    break;              
#endif 

#ifdef RSIC1_DEBUG_DECODE_DEBLK
                case 'm':    // enable de_blocking
                    pInt = (unsigned int *)&risc_packet;
          
                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tVideo;
                    risc_packet.sub_type = tSetDecodeDeblocking;
                    risc_packet.length = 0x4;
                    risc_packet.status = 0;   
                    
                    *(pInt + 2) = 0x1;    // 1: ENABLE; 0: DISABLE                    

                    risc_packet.end = RISC_PKT_END;   
                    if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                        RiscFlagSet();    // Set risc1 flag to interrupt risc1            
                    } else 
                        debug("\r\n                           TwoRiscAudioTARisc1: send packet error!\r\n");            
                    break;    

                case 'n':    // disable de_blocking
                    pInt = (unsigned int *)&risc_packet;
          
                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tVideo;
                    risc_packet.sub_type = tSetDecodeDeblocking;
                    risc_packet.length = 0x4;
                    risc_packet.status = 0;   
                    
                    *(pInt + 2) = 0x0;    // 1: ENABLE; 0: DISABLE                    

                    risc_packet.end = RISC_PKT_END;   
                    if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                        RiscFlagSet();    // Set risc1 flag to interrupt risc1            
                    } else 
                        debug("\r\n                           TwoRiscAudioTARisc1: send packet error!\r\n");            
                    break;              
#endif

#ifdef RSIC1_DEBUG_CAM_AUTODETECT_EN_DIS
                case 'm':    // enable de_blocking
                    pInt = (unsigned int *)&risc_packet;
          
                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tVideo;
                    risc_packet.sub_type = tSetCamAutodetect;
                    risc_packet.length = 0x4;
                    risc_packet.status = 0;   
                    
                    *(pInt + 2) = 0x1;    // 1: ENABLE; 0: DISABLE                    

                    risc_packet.end = RISC_PKT_END;   
                    if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                        RiscFlagSet();    // Set risc1 flag to interrupt risc1            
                    } else 
                        debug("\r\n                           TwoRiscAudioTARisc1: send packet error!\r\n");            
                    break;    

                case 'n':    // disable de_blocking
                    pInt = (unsigned int *)&risc_packet;
          
                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tVideo;
                    risc_packet.sub_type = tSetCamAutodetect;
                    risc_packet.length = 0x4;
                    risc_packet.status = 0;   
                    
                    *(pInt + 2) = 0x0;    // 1: ENABLE; 0: DISABLE                    

                    risc_packet.end = RISC_PKT_END;   
                    if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                        RiscFlagSet();    // Set risc1 flag to interrupt risc1            
                    } else 
                        debug("\r\n                           TwoRiscAudioTARisc1: send packet error!\r\n");            
                    break;              
#endif

#if defined(TASK_SYSTEM_CONTROL_EN) && defined(RISC1_DEBUG_AUTO_COMMD_SEND)
                case '6':    // enable Resolution auto switch
                    if (OSTaskCreateExt(TaskResolutionControl, (void *)0, (void *)&StkTaskResolutionControl[sizeof(StkTaskResolutionControl) / sizeof(StkTaskResolutionControl[0]) - 1], PRIOR_TASK_RESOLUTION_CONTROL,\
                             PRIOR_TASK_RESOLUTION_CONTROL, (void *)&StkTaskResolutionControl[0], sizeof(StkTaskResolutionControl) / sizeof(StkTaskResolutionControl[0]), (void *)0, OS_TASK_OPT_STK_CHK+OS_TASK_OPT_STK_CLR) != OS_NO_ERR)
                        debug("\nTaskTwoRiscSystemMonitorRisc1:TaskResolutionControl create error\n");
                    break;

                case '7':     //disable Resolution auto switch
                    ResolutionSwitchCnt = 0;
                    if (OSTaskDel(PRIOR_TASK_RESOLUTION_CONTROL) != OS_NO_ERR)
                        debug("\nTaskTwoRiscSystemMonitorRisc1:TaskResolutionControl delete error\n");
                    break;

                case '8':    // enable PIP auto switch
                    if (OSTaskCreateExt(TaskPIPControl, (void *)0, (void *)&StkTaskPIPControl[sizeof(StkTaskPIPControl) / sizeof(StkTaskPIPControl[0]) - 1], PRIOR_TASK_PIP_CONTROL,\
                             PRIOR_TASK_PIP_CONTROL, (void *)&StkTaskPIPControl[0], sizeof(StkTaskPIPControl) / sizeof(StkTaskPIPControl[0]), (void *)0, OS_TASK_OPT_STK_CHK+OS_TASK_OPT_STK_CLR) != OS_NO_ERR)
                        debug("\nTaskTwoRiscSystemMonitorRisc1:TaskPIPControl create error\n");
                    break;

                case '9':      //disable PIP auto switch
                    PipSwitchCnt = 0;
                    if (OSTaskDel(PRIOR_TASK_PIP_CONTROL) != OS_NO_ERR)
                        debug("\nTaskTwoRiscSystemMonitorRisc1:TaskPIPControl delete error\n");
                    break;
#endif

#ifdef RISC1_DEBUG_IMAGE_SETTING
                case '1':    // contrast
                    printf("Test image setting , Please enter 0 to 1\n");            
                    receive1 = WaitPressKey();
                    printf("%c", receive1);
                    pInt = (unsigned int *)&risc_packet;
                  
                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tVideo;
                    risc_packet.sub_type = tSetCamera;
                    risc_packet.length = 0x4;
                    risc_packet.status = 0; 
            
                    switch(receive1) 
                    {
                        case '0':
                            *(pInt + 2) = 0x1;
                            break;  

                        case '1':
                            *(pInt + 2) = 0x10001;
                            break;  

                        case '2':
                            *(pInt + 2) = 0x20001;
                            break;  

                        case '3':
                            *(pInt + 2) = 0x30001;
                            break;  
                                 
                        case '4':
                            *(pInt + 2) = 0x40001;
                            break;  
                        
                        case '5':
                            *(pInt + 2) = 0x50001;
                            break;  
                                 
                        case '6':
                            *(pInt + 2) = 0x60001;
                            break;  
                        
                        case '7':
                            *(pInt + 2) = 0x70001;
                            break;  
                                 
                        case '8':
                            *(pInt + 2) = 0x80001;
                            break;  
                        
                        case '9':
                            *(pInt + 2) = 0x90001;
                            break;  
                    }
                    
                    risc_packet.end = RISC_PKT_END;    
                    if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                        RiscFlagSet();    // Set risc1 flag to interrupt risc1            
                    } else 
                        debug("\r\n                           TwoRiscAudioTARisc1: send packet error!\r\n");            
                    break;

                case '2':    // effect
                    printf("Test image setting , Please enter 0 to 1\n");            
                    receive1 = WaitPressKey();
                    printf("%c", receive1);
                    pInt = (unsigned int *)&risc_packet;
          
                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tVideo;
                    risc_packet.sub_type = tSetCamera;
                    risc_packet.length = 0x4;
                    risc_packet.status = 0; 

                    switch(receive1) 
                    {
                        case '0':
                            *(pInt + 2) = 0x2;
                            break;  

                        case '1':
                            *(pInt + 2) = 0x10002;
                            break;  

                        case '2':
                            *(pInt + 2) = 0x20002;
                            break;  

                        case '3':
                            *(pInt + 2) = 0x30002;
                            break;  
                             
                        case '4':
                            *(pInt + 2) = 0x40002;
                            break;  

                        case '5':
                            *(pInt + 2) = 0x50002;
                            break;  
                             
                        case '6':
                            *(pInt + 2) = 0x60002;
                            break;  

                        case '7':
                            *(pInt + 2) = 0x70002;
                            break;  
                             
                        case '8':
                            *(pInt + 2) = 0x80002;
                            break;  

                        case '9':
                            *(pInt + 2) = 0x90002;
                            break;  
                   }
                    
                   risc_packet.end = RISC_PKT_END;   
                   if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                       RiscFlagSet();    // Set risc1 flag to interrupt risc1            
                   } else 
                       debug("\r\n                           TwoRiscAudioTARisc1: send packet error!\r\n");            
                   break;  

                case '3':    //exposure
                    printf("Test image setting exposure, Please enter 0 to 1\n");            
                    receive1 = WaitPressKey();
                    printf("%c", receive1);
                    pInt = (unsigned int *)&risc_packet;
          
                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tVideo;
                    risc_packet.sub_type = tSetCamera;
                    risc_packet.length = 0x4;
                    risc_packet.status = 0; 
          
                    switch(receive1) 
                    {
                        case '0':
                            *(pInt + 2) = 0x3;
                            break;  

                        case '1':
                            *(pInt + 2) = 0x10003;
                            break;  

                        case '2':
                            *(pInt + 2) = 0x20003;
                            break;  

                        case '3':
                            *(pInt + 2) = 0x30003;
                            break;  
                             
                        case '4':
                            *(pInt + 2) = 0x40003;
                            break;  

                        case '5':
                            *(pInt + 2) = 0x50003;
                            break;  
                             
                        case '6':
                            *(pInt + 2) = 0x60003;
                            break;  

                        case '7':
                            *(pInt + 2) = 0x70003;
                            break;  
                             
                        case '8':
                            *(pInt + 2) = 0x80003;
                            break;  

                        case '9':
                            *(pInt + 2) = 0x90003;
                            break;  
                    }
      
                    risc_packet.end = RISC_PKT_END;   
                    if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                        RiscFlagSet();    // Set risc1 flag to interrupt risc1            
                    } else 
                        debug("\r\n                           TwoRiscAudioTARisc1: send packet error!\r\n");            
                    break;  

                case '4':    // saturation
                    printf("Test image setting saturation, Please enter 0 to 1\n");            
                    receive1 = WaitPressKey();
                    printf("%c", receive1);
                    pInt = (unsigned int *)&risc_packet;
          
                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tVideo;
                    risc_packet.sub_type = tSetCamera;
                    risc_packet.length = 0x4;
                    risc_packet.status = 0; 
          
                    switch(receive1) 
                    {
                        case '0':
                            *(pInt + 2) = 0x4;
                            break;  

                        case '1':
                            *(pInt + 2) = 0x10004;
                            break;  

                        case '2':
                            *(pInt + 2) = 0x20004;
                            break;  

                        case '3':
                            *(pInt + 2) = 0x30004;
                            break;  
                             
                        case '4':
                            *(pInt + 2) = 0x40004;
                            break;  

                        case '5':
                            *(pInt + 2) = 0x50004;
                            break;  
                             
                        case '6':
                            *(pInt + 2) = 0x60004;
                            break;  

                        case '7':
                            *(pInt + 2) = 0x70004;
                            break;  
                             
                        case '8':
                            *(pInt + 2) = 0x80004;
                            break;  

                        case '9':
                            *(pInt + 2) = 0x90004;
                            break;  
                    }
      
                    risc_packet.end = RISC_PKT_END;   
                    if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                        RiscFlagSet();    // Set risc1 flag to interrupt risc1            
                    } else 
                        debug("\r\n                           TwoRiscAudioTARisc1: send packet error!\r\n");            
                    break;  

                case '5':    // brightness
                    printf("Test image setting brightness, Please enter 0 to 1\n");            
                    receive1 = WaitPressKey();
                    printf("%c", receive1);
                    pInt = (unsigned int *)&risc_packet;
          
                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tVideo;
                    risc_packet.sub_type = tSetCamera;
                    risc_packet.length = 0x4;
                    risc_packet.status = 0; 
          
                    switch(receive1) 
                    {
                        case '0':
                            *(pInt + 2) = 0x5;
                            break;  

                        case '1':
                            *(pInt + 2) = 0x10005;
                            break;  

                        case '2':
                            *(pInt + 2) = 0x20005;
                            break;  

                        case '3':
                            *(pInt + 2) = 0x30005;
                            break;  
                             
                        case '4':
                            *(pInt + 2) = 0x40005;
                            break;  

                        case '5':
                            *(pInt + 2) = 0x50005;
                            break;  
                             
                        case '6':
                            *(pInt + 2) = 0x60005;
                            break;  

                        case '7':
                            *(pInt + 2) = 0x70005;
                            break;  
                             
                        case '8':
                            *(pInt + 2) = 0x80005;
                            break;  

                        case '9':
                            *(pInt + 2) = 0x90005;
                            break;  
                   }
      
                    risc_packet.end = RISC_PKT_END;   
                    if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                        RiscFlagSet();    // Set risc1 flag to interrupt risc1            
                    } else 
                        debug("\r\n                           TwoRiscAudioTARisc1: send packet error!\r\n");            
                    break;  

                case '6':    // hue
                    printf("Test image setting hue, Please enter 0 to 1\n");            
                    receive1 = WaitPressKey();
                    printf("%c", receive1);
                    pInt = (unsigned int *)&risc_packet;
          
                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tVideo;
                    risc_packet.sub_type = tSetCamera;
                    risc_packet.length = 0x4;
                    risc_packet.status = 0; 
          
                    switch(receive1) 
                    {
                        case '0':
                            *(pInt + 2) = 0x6;
                            break;  

                        case '1':
                            *(pInt + 2) = 0x10006;
                            break;  

                        case '2':
                            *(pInt + 2) = 0x20006;
                            break;  

                        case '3':
                            *(pInt + 2) = 0x30006;
                            break;  
                             
                        case '4':
                            *(pInt + 2) = 0x40006;
                            break;  

                        case '5':
                            *(pInt + 2) = 0x50006;
                            break;  
                             
                        case '6':
                            *(pInt + 2) = 0x60006;
                            break;  

                        case '7':
                            *(pInt + 2) = 0x70006;
                            break;  
                             
                        case '8':
                            *(pInt + 2) = 0x80006;
                            break;  

                        case '9':
                            *(pInt + 2) = 0x90006;
                            break;  
                    }
      
                    risc_packet.end = RISC_PKT_END;   
                    if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                        RiscFlagSet();    // Set risc1 flag to interrupt risc1            
                    } else 
                        debug("\r\n                           TwoRiscAudioTARisc1: send packet error!\r\n");            
                    break;  

                case '7':    // sharpness
                    printf("Test image setting sharpness, Please enter 0 to 1\n");            
                    receive1 = WaitPressKey();
                    printf("%c", receive1);
                    pInt = (unsigned int *)&risc_packet;
          
                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tVideo;
                    risc_packet.sub_type = tSetCamera;
                    risc_packet.length = 0x4;
                    risc_packet.status = 0; 
          
                    switch(receive1) 
                    {
                        case '0':
                            *(pInt + 2) = 0x7;
                            break;  

                        case '1':
                            *(pInt + 2) = 0x10007;
                            break;  

                        case '2':
                            *(pInt + 2) = 0x20007;
                            break;  

                        case '3':
                            *(pInt + 2) = 0x30007;
                            break;  
                             
                        case '4':
                            *(pInt + 2) = 0x40007;
                            break;  

                        case '5':
                            *(pInt + 2) = 0x50007;
                            break;  
                             
                        case '6':
                            *(pInt + 2) = 0x60007;
                            break;  

                        case '7':
                            *(pInt + 2) = 0x70007;
                            break;  
                             
                        case '8':
                            *(pInt + 2) = 0x80007;
                            break;  

                        case '9':
                            *(pInt + 2) = 0x90007;
                            break;  
                    }
      
                    risc_packet.end = RISC_PKT_END;   
                    if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                        RiscFlagSet();    // Set risc1 flag to interrupt risc1            
                    } else 
                        debug("\r\n                           TwoRiscAudioTARisc1: send packet error!\r\n");            
                    break;  

 #endif

#ifdef RISC1_DEBUG_IMAGE_MIRROR_FLIP    //1:normal,2:mirror,3:flip,4,mirror and flip  
                case '1':    //normal
                    pInt = (unsigned int *)&risc_packet;
          
                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tVideo;
                    risc_packet.sub_type = tSetCameraImageMirror;
                    risc_packet.length = 0x4;
                    risc_packet.status = 0; 
          
                    *(unsigned short *)(pInt  + 2) = 0;
                    *((unsigned short *)(pInt  + 2) + 1) = 0;

                    risc_packet.end = RISC_PKT_END;   
                    if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                        RiscFlagSet();    // Set risc1 flag to interrupt risc1            
                    } else 
                        debug("\r\n                           TwoRiscAudioTARisc1: send packet error!\r\n");            
                    break;  

                case '2':    //mirror
                    pInt = (unsigned int *)&risc_packet;
          
                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tVideo;
                    risc_packet.sub_type = tSetCameraImageMirror;
                    risc_packet.length = 0x4;
                    risc_packet.status = 0; 
          
                    *(unsigned short *)(pInt  + 2) = 0;
                    *((unsigned short *)(pInt  + 2) + 1) = 1;

                    risc_packet.end = RISC_PKT_END;   
                    if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                        RiscFlagSet();    // Set risc1 flag to interrupt risc1            
                    } else 
                        debug("\r\n                           TwoRiscAudioTARisc1: send packet error!\r\n");            
                    break;  

                case '3':    //flip
                    pInt = (unsigned int *)&risc_packet;
          
                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tVideo;
                    risc_packet.sub_type = tSetCameraImageMirror;
                    risc_packet.length = 0x4;
                    risc_packet.status = 0; 
          
                    *(unsigned short *)(pInt  + 2) = 1;
                    *((unsigned short *)(pInt  + 2) + 1) = 0;

                    risc_packet.end = RISC_PKT_END;   
                    if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                        RiscFlagSet();    // Set risc1 flag to interrupt risc1            
                    } else 
                        debug("\r\n                           TwoRiscAudioTARisc1: send packet error!\r\n");            
                    break;  

                case '4':    //mirror and flip
                    pInt = (unsigned int *)&risc_packet;
          
                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tVideo;
                    risc_packet.sub_type = tSetCameraImageMirror;
                    risc_packet.length = 0x4;
                    risc_packet.status = 0; 
          
                    *(unsigned short *)(pInt  + 2) = 1;
                    *((unsigned short *)(pInt  + 2) + 1) = 1;

                    risc_packet.end = RISC_PKT_END;   
                    if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                        RiscFlagSet();    // Set risc1 flag to interrupt risc1            
                    } else 
                        debug("\r\n                           TwoRiscAudioTARisc1: send packet error!\r\n");            
                    break;  

#endif

#ifdef RISC1_DEBUG_ANALOG_DITITAL_CAMERA
                case '1':    // select digital camera

                    pInt = (unsigned int *)&risc_packet;
          
                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tVideo;
                    risc_packet.sub_type = tSetCameraSelect;
                    risc_packet.length = 0x4;
                    risc_packet.status = 0; 
          
                    *(pInt + 2) = 0x1;
      
                    risc_packet.end = RISC_PKT_END;   
                    if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                        RiscFlagSet();    // Set risc1 flag to interrupt risc1            
                    } else 
                        debug("\r\n                           TwoRiscAudioTARisc1: send packet error!\r\n");            
                    break;  

                case '2':    // select analog camera

                    pInt = (unsigned int *)&risc_packet;
          
                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tVideo;
                    risc_packet.sub_type = tSetCameraSelect;
                    risc_packet.length = 0x4;
                    risc_packet.status = 0; 
          
                    *(pInt + 2) = 0x2;
      
                    risc_packet.end = RISC_PKT_END;   
                    if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                        RiscFlagSet();    // Set risc1 flag to interrupt risc1            
                    } else 
                        debug("\r\n                           TwoRiscAudioTARisc1: send packet error!\r\n");            
                    break;  

#endif

#ifdef RISC1_DEBUG_INTERLACED_PROGRESSIVE
                case 'E':    // Rvo: interlaced

                    pInt = (unsigned int *)&risc_packet;
          
                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tVideo;
                    risc_packet.sub_type = tSetVoutRvoInterlaced;
                    risc_packet.length = 0x4;
                    risc_packet.status = 0; 
          
                    *(pInt + 2) = 0x0002;
      
                    risc_packet.end = RISC_PKT_END;   
                    if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                        RiscFlagSet();    // Set risc1 flag to interrupt risc1            
                    } else 
                        debug("\r\n                           TwoRiscAudioTARisc1: send packet error!\r\n");            
                    break;  

                case 'A':    // Rvo: progressive

                    pInt = (unsigned int *)&risc_packet;
          
                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tVideo;
                    risc_packet.sub_type = tSetVoutRvoInterlaced;
                    risc_packet.length = 0x4;
                    risc_packet.status = 0; 
          
                    *(pInt + 2) = 0x10002;
      
                    risc_packet.end = RISC_PKT_END;   
                    if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                        RiscFlagSet();    // Set risc1 flag to interrupt risc1            
                    } else 
                        debug("\r\n                           TwoRiscAudioTARisc1: send packet error!\r\n");            
                    break; 
#endif

#ifdef RISC1_DEBUG_EVEN_ODD
                case 'H':    // Vin Even/Odd mode(for Vin interlaced)

                    pInt = (unsigned int *)&risc_packet;
          
                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tVideo;
                    risc_packet.sub_type = tSetEOImgMode;
                    risc_packet.length = 0x4;
                    risc_packet.status = 0; 
          
                    *(pInt + 2) = 0x0;
      
                    risc_packet.end = RISC_PKT_END;   
                    if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                        RiscFlagSet();    // Set risc1 flag to interrupt risc1            
                    } else 
                        debug("\r\n                           TwoRiscAudioTARisc1: send packet error!\r\n");            
                    break;  

                case 'B':    // Vin merged mode(for Vin interlaced)

                    pInt = (unsigned int *)&risc_packet;
          
                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tVideo;
                    risc_packet.sub_type = tSetEOImgMode;
                    risc_packet.length = 0x4;
                    risc_packet.status = 0; 
          
                    *(pInt + 2) = 0x1;
      
                    risc_packet.end = RISC_PKT_END;   
                    if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                        RiscFlagSet();    // Set risc1 flag to interrupt risc1            
                    } else 
                        debug("\r\n                           TwoRiscAudioTARisc1: send packet error!\r\n");            
                    break; 

                case 'M':    // Vin: odd frame first(default)

                    pInt = (unsigned int *)&risc_packet;
          
                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tVideo;
                    risc_packet.sub_type = tOddEvenSelect;
                    risc_packet.length = 0x8;
                    risc_packet.status = 0; 
          
                    *(pInt + 2) = 0x1;                        
      
                    risc_packet.end = RISC_PKT_END;   
                    if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                        RiscFlagSet();    // Set risc1 flag to interrupt risc1            
                    } else 
                        debug("\r\n                           TwoRiscAudioTARisc1: send packet error!\r\n");            
                    break;  
                    
                case 'N':    // Vin: even frame first

                    pInt = (unsigned int *)&risc_packet;
          
                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tVideo;
                    risc_packet.sub_type = tOddEvenSelect;
                    risc_packet.length = 0x8;
                    risc_packet.status = 0; 
          
                    *(pInt + 2) = 0x10001;                        
      
                    risc_packet.end = RISC_PKT_END;   
                    if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                        RiscFlagSet();    // Set risc1 flag to interrupt risc1            
                    } else 
                        debug("\r\n                           TwoRiscAudioTARisc1: send packet error!\r\n");            
                    break;                       

                case 'R':    // Vsc: odd frame first

                    pInt = (unsigned int *)&risc_packet;
          
                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tVideo;
                    risc_packet.sub_type = tOddEvenSelect;
                    risc_packet.length = 0x8;
                    risc_packet.status = 0; 
          
                    *(pInt + 2) = 0x2;                        
      
                    risc_packet.end = RISC_PKT_END;   
                    if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                        RiscFlagSet();    // Set risc1 flag to interrupt risc1            
                    } else 
                        debug("\r\n                           TwoRiscAudioTARisc1: send packet error!\r\n");            
                    break;  
                    
                case 'J':    // Vsc: even frame first(default)

                    pInt = (unsigned int *)&risc_packet;
          
                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tVideo;
                    risc_packet.sub_type = tOddEvenSelect;
                    risc_packet.length = 0x8;
                    risc_packet.status = 0; 
          
                    *(pInt + 2) = 0x10002;                        
      
                    risc_packet.end = RISC_PKT_END;   
                    if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                        RiscFlagSet();    // Set risc1 flag to interrupt risc1            
                    } else 
                        debug("\r\n                           TwoRiscAudioTARisc1: send packet error!\r\n");            
                    break;                                 
#endif

#ifdef RISC1_DEBUG_PIP_EFFECT
                case 'G':    // good PIP effect
                    pInt = (unsigned int *)&risc_packet;
          
                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tVideo;
                    risc_packet.sub_type = 0xfb;
                    risc_packet.length = 0x4;
                    risc_packet.status = 0;   
                    risc_packet.end = RISC_PKT_END;   
                    if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                        RiscFlagSet();    // Set risc1 flag to interrupt risc1            
                    } else 
                        debug("\r\n                           TwoRiscAudioTARisc1: send packet error!\r\n");            
                    break;

                case 'K':    // bad PIP effect
                    pInt = (unsigned int *)&risc_packet;
          
                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tVideo;
                    risc_packet.sub_type = 0xfc;
                    risc_packet.length = 0x4;
                    risc_packet.status = 0;   
                    risc_packet.end = RISC_PKT_END;   
                    if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                        RiscFlagSet();    // Set risc1 flag to interrupt risc1            
                    } else 
                        debug("\r\n                           TwoRiscAudioTARisc1: send packet error!\r\n");            
                    break;                    

                case '8':  // pip 1/4

                    pInt = (unsigned int *)&risc_packet;

                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tVideo;
                    risc_packet.sub_type = tSetScale;
                    risc_packet.length = 0x8;
                    risc_packet.status = 0;

                    *(pInt + 2) = PIP;

                    *(pInt + 3) = ((unsigned short)Div_2 << 16) | (unsigned short)Div_2;

                    *(pInt + 4) = (240<<16) | (352);    // for NTSC & pip 1/4, all the vin size is OK!

                    risc_packet.end = RISC_PKT_END;

                    if (TwoRiscCommSendPacket ((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                        RiscFlagSet ();
                    } else {
                        debug ("\n\nTaskTwoRiscSystemMonitorRisc1: TwoRiscCommSendPacket error!\n\n");
                    }
                    break;

                case '9':  // pip 1/16

                    pInt = (unsigned int *)&risc_packet;

                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tVideo;
                    risc_packet.sub_type = tSetScale;
                    risc_packet.length = 0x8;
                    risc_packet.status = 0;

                    *(pInt + 2) = PIP;

                    *(pInt + 3) = ((unsigned short)Div_4 << 16) | (unsigned short)Div_4;

                    *(pInt + 4) = (360<<16) | (528);    // for NTSC & pip 1/16, all the vin size is OK!

                    risc_packet.end = RISC_PKT_END;

                    if (TwoRiscCommSendPacket ((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                        RiscFlagSet ();
                    } else {
                        debug ("\n\nTaskTwoRiscSystemMonitorRisc1: TwoRiscCommSendPacket error!\n\n");
                    }
                    break;        

                case '0':  // pip noscale

                    pInt = (unsigned int *)&risc_packet;

                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tVideo;
                    risc_packet.sub_type = tSetScale;
                    risc_packet.length = 0x8;
                    risc_packet.status = 0;

                    *(pInt + 2) = PIP;

                    *(pInt + 3) = ((unsigned short)NotScale<< 16) | (unsigned short)NotScale;

                    *(pInt + 4) = (0<<16) | (16);    // for NTSC & pip 1/16, all the vin size is OK!

                    risc_packet.end = RISC_PKT_END;

                    if (TwoRiscCommSendPacket ((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                        RiscFlagSet ();
                    } else {
                        debug ("\n\nTaskTwoRiscSystemMonitorRisc1: TwoRiscCommSendPacket error!\n\n");
                    }
                    break;                    

#endif

#ifdef RISC1_DEBUG_DECODE
                case 'M':    // do not add error word to decode input 
                    pInt = (unsigned int *)&risc_packet;
          
                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tVideo;
                    risc_packet.sub_type = 0xff;
                    risc_packet.length = 0x4;
                    risc_packet.status = 0;   
                    *(pInt + 2) = 0;    // 0: do not add error word                    
                    risc_packet.end = RISC_PKT_END;   
                    if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                        RiscFlagSet();    // Set risc1 flag to interrupt risc1            
                    } else 
                        debug("\r\n                           TwoRiscAudioTARisc1: send packet error!\r\n");            
                    break;

                case 'N':    // add error word to decode input  
                    pInt = (unsigned int *)&risc_packet;
          
                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tVideo;
                    risc_packet.sub_type = 0xff;
                    risc_packet.length = 0x4;
                    risc_packet.status = 0;   
                    *(pInt + 2) = 1;    // 1: add error word                     
                    risc_packet.end = RISC_PKT_END;   
                    if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                        RiscFlagSet();    // Set risc1 flag to interrupt risc1            
                    } else 
                        debug("\r\n                           TwoRiscAudioTARisc1: send packet error!\r\n");            
                    break;

                case 'K':    // manually  reset
                    pInt = (unsigned int *)&risc_packet;
          
                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tVideo;
                    risc_packet.sub_type = 0x1d;
                    risc_packet.length = 0x0;
                    risc_packet.status = 0;                      
                    risc_packet.end = RISC_PKT_END;   
                    if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                        RiscFlagSet();    // Set risc1 flag to interrupt risc1            
                    } else 
                        debug("\r\n                           TwoRiscAudioTARisc1: send packet error!\r\n");            
                    break;          
#endif

#ifdef RISC1_DEBUG_ENCODE_MODE
                case 'M':    // set the encode mode as MPEG 
                    pInt = (unsigned int *)&risc_packet;
          
                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tVideo;
                    risc_packet.sub_type = tSetVideoCodec;
                    risc_packet.length = 0x4;
                    risc_packet.status = 0;   
                    *(pInt + 2) = 1;                         
                    risc_packet.end = RISC_PKT_END;   
                    if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                        RiscFlagSet();    // Set risc1 flag to interrupt risc1            
                    } else 
                        debug("\r\n                           TwoRiscAudioTARisc1: send packet error!\r\n");            
                    break;

                case 'H':    // set the encode mode as H263   
                    pInt = (unsigned int *)&risc_packet;
          
                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tVideo;
                    risc_packet.sub_type = tSetVideoCodec;
                    risc_packet.length = 0x4;
                    risc_packet.status = 0;   
                    *(pInt + 2) = 4;    // 0: do not add error word                    
                    risc_packet.end = RISC_PKT_END;   
                    if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                        RiscFlagSet();    // Set risc1 flag to interrupt risc1            
                    } else 
                        debug("\r\n                           TwoRiscAudioTARisc1: send packet error!\r\n");            
                    break;

#endif

#ifdef RISC1_DEBUG_OTHERS
                case 'T':    // let cpu stop at some point  
                    pInt = (unsigned int *)&risc_packet;
          
                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tVideo;
                    risc_packet.sub_type = 0xfd;
                    risc_packet.length = 0x4;
                    risc_packet.status = 0;   
                    risc_packet.end = RISC_PKT_END;   
                    if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                        RiscFlagSet();    // Set risc1 flag to interrupt risc1            
                    } else 
                        debug("\r\n                           TwoRiscAudioTARisc1: send packet error!\r\n");            
                    break;           

                case 'B':    // enable vout, at the same the vsc is enabled!  
                    pInt = (unsigned int *)&risc_packet;
          
                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tVideo;
                    risc_packet.sub_type = tVideoEnableDisable;
                    risc_packet.length = 0x4;
                    risc_packet.status = 0;   
                    *(pInt + 2) = 1;    // 1: ENABLE; 0: DISABLE                    
                    *(pInt + 3) = 2;    // 2: vout
                    risc_packet.end = RISC_PKT_END;   
                    if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                        RiscFlagSet();    // Set risc1 flag to interrupt risc1            
                    } else 
                        debug("\r\n                           TwoRiscAudioTARisc1: send packet error!\r\n");            
                    break;
#endif                    

#ifdef RISC1_DEBUG_MOTION_DETECTION                    
                case 'H':        //set motion detection sensity =50
                    pInt = (unsigned int *)&risc_packet;
          
                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tVideo;
                    risc_packet.sub_type = tSetMotionSensity;
                    risc_packet.length = 0x4;
                    risc_packet.status = 0;   
                    *(pInt + 2) = 50;        
                    *(pInt + 3) = 0;
                      risc_packet.end = RISC_PKT_END;   
                    if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                        RiscFlagSet();    // Set risc1 flag to interrupt risc1            
                    } else 
                        debug("\r\n                           TwoRiscAudioTARisc1: send packet error!\r\n");            
                    break;

                case 'G':               //set motion detection sensity = 0
                          pInt = (unsigned int *)&risc_packet;
          
                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tVideo;
                    risc_packet.sub_type = tSetMotionSensity;
                    risc_packet.length = 0x4;
                    risc_packet.status = 0;   
                    *(pInt + 2) = 0;
                    *(pInt + 3) = 0;
                      risc_packet.end = RISC_PKT_END;   
                    if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                        RiscFlagSet();    // Set risc1 flag to interrupt risc1            
                    } else 
                        debug("\r\n                           TwoRiscAudioTARisc1: send packet error!\r\n");            
                    break;
#endif                    

#ifdef RISC1_DEBUG_NTSC_PAL
                case 'J':    //set display format to pal mode

                    pInt = (unsigned int *)&risc_packet;
          
                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tVideo;
                    risc_packet.sub_type = tSetNTSCPal;
                    risc_packet.length = 0x4;
                    risc_packet.status = 0; 
          
                    *(unsigned short *)(pInt + 2) = 0x2;  //1: vin, 2: vout;    
                    *((unsigned short *)(pInt  + 2) + 1) = 1;  //0: NTSC, 1: PAL
      
                    risc_packet.end = RISC_PKT_END;   
                    if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                        RiscFlagSet();    // Set risc1 flag to interrupt risc1            
                    } else 
                        debug("\r\n                           TwoRiscAudioTARisc1: send packet error!\r\n");            
                    break;

                case 'R':    //set display format to NTSC mode

                    pInt = (unsigned int *)&risc_packet;
          
                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tVideo;
                    risc_packet.sub_type = tSetNTSCPal;
                    risc_packet.length = 0x4;
                    risc_packet.status = 0; 
          
                    *(unsigned short *)(pInt + 2) = 0x2;  //1: vin, 2: vout;    
                    *((unsigned short *)(pInt  + 2) + 1) = 0;  //0: NTSC, 1: PAL
      
                    risc_packet.end = RISC_PKT_END;   
                    if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                        RiscFlagSet();    // Set risc1 flag to interrupt risc1            
                    } else 
                        debug("\r\n                           TwoRiscAudioTARisc1: send packet error!\r\n");            
                    break;

                case 'K':    //set display format to LCD

                    pInt = (unsigned int *)&risc_packet;
          
                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tVideo;
                    risc_packet.sub_type = tSetNTSCPal;
                    risc_packet.length = 0x4;
                    risc_packet.status = 0; 
          
                    *(unsigned short *)(pInt + 2) = 0x2;  //1: vin, 2: vout;    
                    *((unsigned short *)(pInt  + 2) + 1) = 2;  //0: NTSC, 1: PAL; 2: LCD
      
                    risc_packet.end = RISC_PKT_END;   
                    if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                        RiscFlagSet();    // Set risc1 flag to interrupt risc1            
                    } else 
                        debug("\r\n                           TwoRiscAudioTARisc1: send packet error!\r\n");            
                    break;

                case 'r':    //set vin format to NTSC mode

                    pInt = (unsigned int *)&risc_packet;
          
                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tVideo;
                    risc_packet.sub_type = tSetNTSCPal;
                    risc_packet.length = 0x4;
                    risc_packet.status = 0; 
          
                    *(unsigned short *)(pInt + 2) = 0x1;  //1: vin, 2: vout;    
                    *((unsigned short *)(pInt  + 2) + 1) = 0;  //0: NTSC, 1: PAL
      
                    risc_packet.end = RISC_PKT_END;   
                    if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                        RiscFlagSet();    // Set risc1 flag to interrupt risc1            
                    } else 
                        debug("\r\n                           TwoRiscAudioTARisc1: send packet error!\r\n");            
                    break;

                case 'j':    //set vin format to PAL mode

                    pInt = (unsigned int *)&risc_packet;
          
                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tVideo;
                    risc_packet.sub_type = tSetNTSCPal;
                    risc_packet.length = 0x4;
                    risc_packet.status = 0; 
          
                    *(unsigned short *)(pInt + 2) = 0x1;  //1: vin, 2: vout;    
                    *((unsigned short *)(pInt  + 2) + 1) = 1;  //0: NTSC, 1: PAL
      
                    risc_packet.end = RISC_PKT_END;   
                    if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                        RiscFlagSet();    // Set risc1 flag to interrupt risc1            
                    } else 
                        debug("\r\n                           TwoRiscAudioTARisc1: send packet error!\r\n");            
                    break;
#endif

#ifdef RISC1_DEBUG_RESOLUTION

                case 'T':    // set 704*576

                    pInt = (unsigned int *)&risc_packet;
          
                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tVideo;
                    risc_packet.sub_type = tSetVideoSize;
                    risc_packet.length = 0x8;
                    risc_packet.status = 0; 
          
                    *(pInt + 2) = 0x1;//Mode_Vin;   
                    width = 704;//CIFWIDTH;
                    height = 576;//CIFHEIGHT;
                    *(unsigned short *)(pInt  + 3) = width;
                    *((unsigned short *)(pInt  + 3) + 1) = height;
      
                    risc_packet.end = RISC_PKT_END;   
                    if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                        RiscFlagSet();    // Set risc1 flag to interrupt risc1            
                    } else 
                        debug("\r\n                           TwoRiscAudioTARisc1: send packet error!\r\n");            
                    break;

                case 'C':    // set CIF

                    pInt = (unsigned int *)&risc_packet;
          
                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tVideo;
                    risc_packet.sub_type = tSetVideoSize;
                    risc_packet.length = 0x8;
                    risc_packet.status = 0; 
          
                    *(pInt + 2) = 0x1;//Mode_Vin;   
                    width=352;//CIFWIDTH;
                    height=288;//CIFHEIGHT;
                    *(unsigned short *)(pInt  + 3) = width;
                    *((unsigned short *)(pInt  + 3) + 1) = height;
      
                    risc_packet.end = RISC_PKT_END;   
                    if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                        RiscFlagSet();    // Set risc1 flag to interrupt risc1            
                    } else 
                        debug("\r\n                           TwoRiscAudioTARisc1: send packet error!\r\n");            
                    break;

                case '|':    // set SUB_QCIF

                    pInt = (unsigned int *)&risc_packet;
          
                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tVideo;
                    risc_packet.sub_type = tSetVideoSize;
                    risc_packet.length = 0x8;
                    risc_packet.status = 0; 
          
                    *(pInt + 2) = 0x1;//Mode_Vin;   
                    width=128;//SUB_QCIF WIDTH;
                    height=96;//SUB_QCIF HEIGHT;
                    *(unsigned short *)(pInt  + 3) = width;
                    *((unsigned short *)(pInt  + 3) + 1) = height;
      
                    risc_packet.end = RISC_PKT_END;   
                    if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                        RiscFlagSet();    // Set risc1 flag to interrupt risc1            
                    } else 
                        debug("\r\n                           TwoRiscAudioTARisc1: send packet error!\r\n");            
                    break;

                case 'Q':    // set QVGA

                    pInt = (unsigned int *)&risc_packet;
          
                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tVideo;
                    risc_packet.sub_type = tSetVideoSize;
                    risc_packet.length = 0x8;
                    risc_packet.status = 0; 
          
                    *(pInt + 2) = 0x1;//Mode_Vin;   
                    width=320;//QVGAWIDTH;
                    height=240;//QVGAHEIGHT;
                    *(unsigned short *)(pInt  + 3) = width;
                    *((unsigned short *)(pInt  + 3) + 1) = height;
      
                    risc_packet.end = RISC_PKT_END;   
                    if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                        RiscFlagSet();    // Set risc1 flag to interrupt risc1            
                    } else 
                        debug("\r\n                           TwoRiscAudioTARisc1: send packet error!\r\n");            
                    break;

                case 'W':    // set VGA

                    pInt = (unsigned int *)&risc_packet;
          
                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tVideo;
                    risc_packet.sub_type = tSetVideoSize;
                    risc_packet.length = 0x8;
                    risc_packet.status = 0; 
          
                    *(pInt + 2) = 0x1;//Mode_Vin;   
                    width=640;//QVGAWIDTH;
                    height=480;//QVGAHEIGHT;
                    *(unsigned short *)(pInt  + 3) = width;
                    *((unsigned short *)(pInt  + 3) + 1) = height;
      
                    risc_packet.end = RISC_PKT_END;   
                    if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                        RiscFlagSet();    // Set risc1 flag to interrupt risc1            
                    } else 
                        debug("\r\n                           TwoRiscAudioTARisc1: send packet error!\r\n");            
                    break;
/*
                case '>':    // set SXGA

                    pInt = (unsigned int *)&risc_packet;
          
                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tVideo;
                    risc_packet.sub_type = tSetVideoSize;
                    risc_packet.length = 0x8;
                    risc_packet.status = 0; 
          
                    *(pInt + 2) = 0x1;//Mode_Vin;   
                    width=1280;//SXGAWIDTH;
                    height=1024;//SXGAHEIGHT;
                    *(unsigned short *)(pInt  + 3) = width;
                    *((unsigned short *)(pInt  + 3) + 1) = height;
      
                    risc_packet.end = RISC_PKT_END;   
                    if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                        RiscFlagSet();    // Set risc1 flag to interrupt risc1            
                    } else 
                        debug("\r\n                           TwoRiscAudioTARisc1: send packet error!\r\n");            
                    break;
*/                    
 
                case 'Z':    // set Qcif

                    pInt = (unsigned int *)&risc_packet;
          
                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tVideo;
                    risc_packet.sub_type = tSetVideoSize;
                    risc_packet.length = 0x8;
                    risc_packet.status = 0; 
          
                    *(pInt + 2) = 0x1;//Mode_Vin;   
                    width=176;  //Qcif WIDTH;
                    height=144;    //Qcif HEIGHT;
                    *(unsigned short *)(pInt  + 3) = width;
                    *((unsigned short *)(pInt  + 3) + 1) = height;
      
                    risc_packet.end = RISC_PKT_END;   
                    if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                        RiscFlagSet();    // Set risc1 flag to interrupt risc1            
                    } else 
                        debug("\r\n                           TwoRiscAudioTARisc1: send packet error!\r\n");            
                    break;                    

                case 'S':    // set D1: 704 * 480

                    pInt = (unsigned int *)&risc_packet;
          
                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tVideo;
                    risc_packet.sub_type = tSetVideoSize;
                    risc_packet.length = 0x8;
                    risc_packet.status = 0; 
          
                    *(pInt + 2) = 0x1;//Mode_Vin;   
                    width = 704;  //D1 WIDTH;
                    height = 480;    //D1 HEIGHT;
                    *(unsigned short *)(pInt  + 3) = width;
                    *((unsigned short *)(pInt  + 3) + 1) = height;
      
                    risc_packet.end = RISC_PKT_END;   
                    if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                        RiscFlagSet();    // Set risc1 flag to interrupt risc1            
                    } else 
                        debug("\r\n                           TwoRiscAudioTARisc1: send packet error!\r\n");            
                    break;           
#ifndef DVR
                case '[':    // set NTSC CIF: 352 * 240

                    pInt = (unsigned int *)&risc_packet;
          
                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tVideo;
                    risc_packet.sub_type = tSetVideoSize;
                    risc_packet.length = 0x8;
                    risc_packet.status = 0; 
          
                    *(pInt + 2) = 0x1;//Mode_Vin;   
                    width = 352;  //D1 WIDTH;
                    height = 240;    //D1 HEIGHT;
                    *(unsigned short *)(pInt  + 3) = width;
                    *((unsigned short *)(pInt  + 3) + 1) = height;
      
                    risc_packet.end = RISC_PKT_END;   
                    if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                        RiscFlagSet();    // Set risc1 flag to interrupt risc1            
                    } else 
                        debug("\r\n                           TwoRiscAudioTARisc1: send packet error!\r\n");            
                    break;                 

                case ']':    // set NTSC QCIF: 176 * 120

                    pInt = (unsigned int *)&risc_packet;
          
                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tVideo;
                    risc_packet.sub_type = tSetVideoSize;
                    risc_packet.length = 0x8;
                    risc_packet.status = 0; 
          
                    *(pInt + 2) = 0x1;//Mode_Vin;   
                    width = 176;  //D1 WIDTH;
                    height = 120;    //D1 HEIGHT;
                    *(unsigned short *)(pInt  + 3) = width;
                    *((unsigned short *)(pInt  + 3) + 1) = height;
      
                    risc_packet.end = RISC_PKT_END;   
                    if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                        RiscFlagSet();    // Set risc1 flag to interrupt risc1            
                    } else 
                        debug("\r\n                           TwoRiscAudioTARisc1: send packet error!\r\n");            
                    break;
#endif
                case 'L':    // set QQVGA

                    pInt = (unsigned int *)&risc_packet;
          
                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tVideo;
                    risc_packet.sub_type = tSetVideoSize;
                    risc_packet.length = 0x8;
                    risc_packet.status = 0; 
          
                    *(pInt + 2) = 0x1;//Mode_Vin;   
                    width=160;  //Qqvga WIDTH;
                    height=120;    //Qqvga HEIGHT;
                    *(unsigned short *)(pInt  + 3) = width;
                    *((unsigned short *)(pInt  + 3) + 1) = height;
      
                    risc_packet.end = RISC_PKT_END;   
                    if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                        RiscFlagSet();    // Set risc1 flag to interrupt risc1            
                    } else 
                        debug("\r\n                           TwoRiscAudioTARisc1: send packet error!\r\n");            
                    break;        
#endif                    

#ifdef RISC1_DEBUG_SCALE
                case '0':  //scale test

                    pInt = (unsigned int *)&risc_packet;

                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tVideo;
                    risc_packet.sub_type = tSetScale;
                    risc_packet.length = 0x8;
                    risc_packet.status = 0;

                    *(pInt + 2) = mode;

                    *(pInt + 3) = (MagnificationHori << 16) | MagnificationVerti;

                    risc_packet.end = RISC_PKT_END;

                    if (TwoRiscCommSendPacket ((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                        RiscFlagSet ();
                    } else {
                        debug ("send packet error\r\n");
                    }

                    MagnificationHori++;
                    MagnificationVerti++;

                    if ((MagnificationHori > Times_2) && (MagnificationVerti > Times_2)) {
                        MagnificationHori = Div_4;
                        MagnificationVerti = Div_4;
                        }

                    break;                  
                    
                case '9':  //scale test

                    pInt = (unsigned int *)&risc_packet;

                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tVideo;
                    risc_packet.sub_type = tSetScale;
                    risc_packet.length = 0x8;
                    risc_packet.status = 0;

                    *(pInt + 2) = mode;

                    MagnificationHori = 0;
                    MagnificationVerti = 0;

                    *(pInt + 3) = (MagnificationHori << 16) + MagnificationVerti;

                    risc_packet.end = RISC_PKT_END;

                    if (TwoRiscCommSendPacket ((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                        RiscFlagSet ();
                        }
                    else {
                        debug ("send packet error\r\n");
                        }

                    break;        

                case '5':

                    pInt = (unsigned int *)&risc_packet;

                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tVideo;
                    risc_packet.sub_type = tSetVideoCodec;
                    risc_packet.length = 0x8;
                    risc_packet.status = 0;

                    if (SetVMode == 1) {
                        SetVMode = 2;
                        }
                    else {
                        SetVMode = 1;
                        }

                    *(pInt + 2) = SetVMode;

                    risc_packet.end = RISC_PKT_END;

                    if (TwoRiscCommSendPacket ((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                        RiscFlagSet ();
                        }
                    else {
                        debug ("send packet error");
                        }

                    break;   
#endif

#ifdef RISC1_DEBUG_RVO_SCALE
                unsigned short pixel_ratio, line_ratio;

                case '4':  //scale test
                    mode = RVO;

                    switch(WaitPressKey()) {
                        case '1':     // pixel no scale
                            pixel_ratio = 0;
                            break;

                        case '2':     // pixel scale 2 times
                            pixel_ratio = 1;
                            break;

                        case '3':     // pixel scale 4 times
                            pixel_ratio = 2;
                            break;

                        case '5':     // pixel scale 1/2
                            pixel_ratio = -1;
                            break;

                        case '6':     // pixel scale 1/4
                            pixel_ratio = -2;
                            break;

                        case '7':     // pixel scale 1/8
                            pixel_ratio = -3;
                            break;

                        default:
                            pixel_ratio = 0;
                            printf("\n\nTaskTwoRiscSystemMonitorRisc1: ratio error!\n\n");
                            break;
                    }

                    switch(WaitPressKey()) {
                        case '1':     // pixel no scale
                            line_ratio = 0;
                            break;

                        case '2':     // pixel scale 2 times
                            line_ratio = 1;
                            break;

                        case '3':     // pixel scale 4 times
                            line_ratio = 2;
                            break;

                        case '5':     // pixel scale 1/2
                            line_ratio = -1;
                            break;

                        case '6':     // pixel scale 1/4
                            line_ratio = -2;
                            break;

                        case '7':     // pixel scale 1/8
                            line_ratio = -3;
                            break;

                        default:
                            line_ratio = 0;
                            printf("\n\nTaskTwoRiscSystemMonitorRisc1: ratio error!\n\n");
                            break;
                    }


                    pInt = (unsigned int *)&risc_packet;

                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tVideo;
                    risc_packet.sub_type = tSetScale;
                    risc_packet.length = 0x8;
                    risc_packet.status = 0;

                    *(pInt + 2) = mode;

                    *(pInt + 3) = (line_ratio << 16) | pixel_ratio;

                    risc_packet.end = RISC_PKT_END;

                    if (TwoRiscCommSendPacket ((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                        RiscFlagSet ();
                    } else {
                        debug ("\n\nTaskTwoRiscSystemMonitorRisc1: send packet error\n\n");
                    }
                    break;            
#endif                    

#ifdef RISC1_DEBUG_FRAMERATE                    
/*
                case 'R':    // set frame rate

                    pInt = (unsigned int *)&risc_packet;
          
                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tVideo;
                    risc_packet.sub_type = tSetFrameRate;
                    risc_packet.length = 0x4;
                    risc_packet.status = 0; 

                    *(unsigned short *)(pInt  + 2) = 1;  //1--VIN, 2--VOUT
                    *((unsigned short *)(pInt  + 2) + 1) = 30;
      
                    risc_packet.end = RISC_PKT_END;   
                    if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                        RiscFlagSet();    // Set risc1 flag to interrupt risc1            
                    } else 
                        debug("\r\n TaskTwoRiscSystemMonitorRisc1: send packet error!\r\n");            
                    break;          
*/
#endif

#ifdef RISC1_DEBUG_RVO_DISPLAY_MODE
                case '0':    // RVO_DISPLAY_MODE = both
                    pInt = (unsigned int *)&risc_packet;
          
                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tVideo;
                    risc_packet.sub_type = tRVODisplayMode;
                    risc_packet.length = 0x4;
                    risc_packet.status = 0;   
                    *(pInt + 2) = 0;
                    risc_packet.end = RISC_PKT_END;   
                    if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                        RiscFlagSet();    // Set risc1 flag to interrupt risc1            
                    } else 
                        debug("\r\n                           TwoRiscAudioTARisc1: send packet error!\r\n");            
                    break;   

                case '1':    // RVO_DISPLAY_MODE = upper part
                    pInt = (unsigned int *)&risc_packet;
          
                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tVideo;
                    risc_packet.sub_type = tRVODisplayMode;
                    risc_packet.length = 0x4;
                    risc_packet.status = 0;   
                    *(pInt + 2) = 1;
                    risc_packet.end = RISC_PKT_END;   
                    if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                        RiscFlagSet();    // Set risc1 flag to interrupt risc1            
                    } else 
                        debug("\r\n                           TwoRiscAudioTARisc1: send packet error!\r\n");            
                    break;   

                case '2':    // RVO_DISPLAY_MODE = down part
                    pInt = (unsigned int *)&risc_packet;
          
                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tVideo;
                    risc_packet.sub_type = tRVODisplayMode;
                    risc_packet.length = 0x4;
                    risc_packet.status = 0;   
                    *(pInt + 2) = 2;
                    risc_packet.end = RISC_PKT_END;   
                    if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                        RiscFlagSet();    // Set risc1 flag to interrupt risc1            
                    } else 
                        debug("\r\n                           TwoRiscAudioTARisc1: send packet error!\r\n");            
                    break;   
#endif

#ifdef RISC1_DEBUG_OSD
                case 'A':    // enable OSD  
                    pInt = (unsigned int *)&risc_packet;
          
                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tVideo;
                    risc_packet.sub_type = tVideoEnableDisable;
                    risc_packet.length = 0x4;
                    risc_packet.status = 0;   
                    *(pInt + 2) = 1;    // 1: ENABLE; 0: DISABLE    
                    *(pInt + 3) = 0x20|0x4;    // 8: pip                   
                    risc_packet.end = RISC_PKT_END;   
                    if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                        RiscFlagSet();    // Set risc1 flag to interrupt risc1            
                    } else 
                        debug("\r\n                           TwoRiscAudioTARisc1: send packet error!\r\n");            
                    break;   

                case 'E':    // disable OSD
                    pInt = (unsigned int *)&risc_packet;
          
                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tVideo;
                    risc_packet.sub_type = tVideoEnableDisable;
                    risc_packet.length = 0x4;
                    risc_packet.status = 0;   
                    *(pInt + 2) = 0;    // 1: ENABLE; 0: DISABLE                    
                    *(pInt + 3) = 0x20|0x4;    // 0X20: osd
                    risc_packet.end = RISC_PKT_END;   
                    if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                        RiscFlagSet();    // Set risc1 flag to interrupt risc1            
                    } else 
                        debug("\r\n                           TwoRiscAudioTARisc1: send packet error!\r\n");            
                    break; 

                case '1':    // clear screen
                    pInt = (unsigned int *)&risc_packet;
          
                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tVideo;
                    risc_packet.sub_type =tOSDClearScreen;
                    risc_packet.length = 0xc;
                    risc_packet.status = 0; 
          
                    *((unsigned short *)pInt + 4) = 120;    // start X position
                    *((unsigned short *)pInt + 5) = 120;    // start Y position
                    *((unsigned short *)pInt + 6) = 80;    // Height
                    *((unsigned short *)pInt + 7) = 80;    //width
                    *(pInt + 4) = 0x0 ;                          //page  
      
                    risc_packet.end = RISC_PKT_END;   
                    if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                        RiscFlagSet();    // Set risc1 flag to interrupt risc1            
                    } else 
                        debug("\r\nTaskTwoRiscSystemMonitorRisc1: send packet error!\r\n");  
                    break;

                case '2':    // draw line
                    pInt = (unsigned int *)&risc_packet;
          
                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tVideo;
                    risc_packet.sub_type =tOSDDrawLine;
                    risc_packet.length = 0xc;
                    risc_packet.status = 0; 
          
                    *((unsigned short *)pInt + 4) = 80;    // start X position
                    *((unsigned short *)pInt + 5) = 80;    // start Y position
                    *((unsigned short *)pInt + 6) = 360;    // End X position
                    *((unsigned short *)pInt + 7)=80;      //End Y position
                    *(pInt + 4) = 0x4<<4|0x0;                  //Color <<4|page  
      
                    risc_packet.end = RISC_PKT_END;   
                    if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                        RiscFlagSet();    // Set risc1 flag to interrupt risc1            
                    } else 
                        debug("\r\nTaskTwoRiscSystemMonitorRisc1: send packet error!\r\n");  
                    break;
                case '3':    // draw line
                    pInt = (unsigned int *)&risc_packet;
          
                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tVideo;
                    risc_packet.sub_type =tOSDDrawLine;
                    risc_packet.length = 0xc;
                    risc_packet.status = 0; 
          
                    *((unsigned short *)pInt + 4) = 80;    // start X position
                    *((unsigned short *)pInt + 5) = 80;    // start Y position
                    *((unsigned short *)pInt + 6) = 80;    // End X position
                    *((unsigned short *)pInt + 7)=320;      //End Y position
                    *(pInt + 4) = 0x0<<4|0x1;                  //Color <<4|page  
      
                    risc_packet.end = RISC_PKT_END;   
                    if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                        RiscFlagSet();    // Set risc1 flag to interrupt risc1            
                    } else 
                        debug("\r\nTaskTwoRiscSystemMonitorRisc1: send packet error!\r\n");  
                    break;

                case '4':    // fill color
                    pInt = (unsigned int *)&risc_packet;
          
                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tVideo;
                    risc_packet.sub_type =tOSDFillColor;
                    risc_packet.length = 0xc;
                    risc_packet.status = 0; 
          
                    *((unsigned short *)pInt + 4) = 120;    // start X position
                    *((unsigned short *)pInt + 5) = 120;    // start Y position
                    *((unsigned short *)pInt + 6) = 80;    // Height
                    *((unsigned short *)pInt + 7) = 80;    //width
                    *(pInt + 4) = 0x0<<4|0x0 ;                  //Color <<4|page  
      
                    risc_packet.end = RISC_PKT_END;   
                    if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                        RiscFlagSet();    // Set risc1 flag to interrupt risc1            
                    } else 
                        debug("\r\nTaskTwoRiscSystemMonitorRisc1: send packet error!\r\n");  
                    break;

                case '6':    // fill color
                    pInt = (unsigned int *)&risc_packet;
          
                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tVideo;
                    risc_packet.sub_type =tOSDFillColor;
                    risc_packet.length = 0xc;
                    risc_packet.status = 0; 
          
                    *((unsigned short *)pInt + 4) = 240;    // start X position
                    *((unsigned short *)pInt + 5) = 100;    // start Y position
                    *((unsigned short *)pInt + 6) = 80;    // Height
                    *((unsigned short *)pInt + 7) = 80;    //width
                    *(pInt + 4) = 0x1<<4|0x1 ;                  //Color <<4|page  
      
                    risc_packet.end = RISC_PKT_END;   
                    if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                        RiscFlagSet();    // Set risc1 flag to interrupt risc1            
                    } else 
                        debug("\r\nTaskTwoRiscSystemMonitorRisc1: send packet error!\r\n");  
                    break;
              

                case '7':    // change osd page
                    pInt = (unsigned int *)&risc_packet;
          
                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tVideo;
                    risc_packet.sub_type =tChangeOSDPage;
                    risc_packet.length = 0x4;
                    risc_packet.status = 0; 

                    *(pInt + 2) = 0x1;                  //page:0~5  
      
                    risc_packet.end = RISC_PKT_END;   
                    if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                        RiscFlagSet();    // Set risc1 flag to interrupt risc1            
                    } else 
                        debug("\r\nTaskTwoRiscSystemMonitorRisc1: send packet error!\r\n");  
                    break;

                case '8':    // change osd page
                    pInt = (unsigned int *)&risc_packet;
          
                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tVideo;
                    risc_packet.sub_type =tChangeOSDPage;
                    risc_packet.length = 0x4;
                    risc_packet.status = 0; 

                    *(pInt + 2) = 0x0;                  //page:0~5  
      
                    risc_packet.end = RISC_PKT_END;   
                    if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                        RiscFlagSet();    // Set risc1 flag to interrupt risc1            
                    } else 
                        debug("\r\nTaskTwoRiscSystemMonitorRisc1: send packet error!\r\n");  
                    break;

                case 'a':                             //send scroll string, but first you should open OSD
                    pInt = (unsigned int *)&risc_packet;
          
                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tVideo;
                    risc_packet.sub_type = tOSDRollString;
                    risc_packet.length = 0x4;
                    risc_packet.status = 0; 

                    *(pInt + 2) = rollstring[0];
                    *(pInt + 3) = strlen(rollstring[0]);
                    *(pInt + 4) = 1;

                    risc_packet.end = RISC_PKT_END;
                     if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                        RiscFlagSet();    // Set risc1 flag to interrupt risc1      
                    } else 
                        debug("\r\n                           TwoRiscAudioTARisc1: send packet error!\r\n");  
                    break;
#endif

#ifdef RISC1_DEBUG_ALARMIN


                case '1':    //enable alarm
                    pInt = (unsigned int *)&risc_packet;
          
                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tSys;
                    risc_packet.sub_type = tAlarm;
                    risc_packet.length = 0x4;
                    risc_packet.status = 0; 
          
                    *((unsigned char *)pInt + 8) = 1;   // 1-- enable, 0--disable
                    *((unsigned char *)pInt + 9) = 0xe;  //0xe~0x11--gpio num
                    *((unsigned char *)pInt + 10) = 0;   // gpio level,0--low,1--high
                    *((unsigned char *)pInt + 11) = 0;  //type,0--level triggered ,1--edge triggered ,2--pulse counter
      
                    risc_packet.end = RISC_PKT_END;   
                    if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                        RiscFlagSet();    // Set risc1 flag to interrupt risc1      
                    } else 
                        debug("\r\n                           TwoRiscAudioTARisc1: send packet error!\r\n");  
                    break;


                case '2':    //enable alarm
                    pInt = (unsigned int *)&risc_packet;
          
                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tSys;
                    risc_packet.sub_type = tAlarm;
                    risc_packet.length = 0x4;
                    risc_packet.status = 0; 
          
                    *((unsigned char *)pInt + 8) = 1;   // 1-- enable, 0--disable
                    *((unsigned char *)pInt + 9) = 0xe;  //0xe~0x11--gpio num
                    *((unsigned char *)pInt + 10) = 0;   // gpio level,0--low,1--high
                    *((unsigned char *)pInt + 11) = 0x1;  //type,0--level triggered ,1--edge triggered ,2--pulse counter
      
                    risc_packet.end = RISC_PKT_END;   
                    if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                        RiscFlagSet();    // Set risc1 flag to interrupt risc1      
                    } else 
                        debug("\r\n                           TwoRiscAudioTARisc1: send packet error!\r\n");  
                    break;

                case '3':    //enable alarm
                    pInt = (unsigned int *)&risc_packet;
          
                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tSys;
                    risc_packet.sub_type = tAlarm;
                    risc_packet.length = 0x4;
                    risc_packet.status = 0; 
          
                    *((unsigned char *)pInt + 8) = 0;   // 1-- enable, 0--disable
                    *((unsigned char *)pInt + 9) = 0xe;  //0xe~0x11--gpio num
                    *((unsigned char *)pInt + 10) = 0;   // gpio level,0--low,1--high
                    *((unsigned char *)pInt + 11) = 0x1;  //type,0--level triggered ,1--edge triggered ,2--pulse counter
      
                    risc_packet.end = RISC_PKT_END;   
                    if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                        RiscFlagSet();    // Set risc1 flag to interrupt risc1      
                    } else 
                        debug("\r\n                           TwoRiscAudioTARisc1: send packet error!\r\n");  
                    break;


                case '4':    //enable alarm
                    pInt = (unsigned int *)&risc_packet;
          
                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tSys;
                    risc_packet.sub_type = tAlarm;
                    risc_packet.length = 0x4;
                    risc_packet.status = 0; 
          
                    *((unsigned char *)pInt + 8) = 1;   // 1-- enable, 0--disable
                    *((unsigned char *)pInt + 9) = 0xe;  //0xe~0x11--gpio num
                    *((unsigned char *)pInt + 10) = 0;   // gpio level,0--low,1--high
                    *((unsigned char *)pInt + 11) = 0x2;  //type,0--level triggered ,1--edge triggered ,2--pulse counter
      
                    risc_packet.end = RISC_PKT_END;   
                    if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                        RiscFlagSet();    // Set risc1 flag to interrupt risc1      
                    } else 
                        debug("\r\n                           TwoRiscAudioTARisc1: send packet error!\r\n");  
                    break;

                case '5':    //disable alarm
                    pInt = (unsigned int *)&risc_packet;
          
                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tSys;
                    risc_packet.sub_type = tAlarm;
                    risc_packet.length = 0x4;
                    risc_packet.status = 0; 
          
                    *((unsigned char *)pInt + 8) = 0;   // 1-- enable, 0--disable
                    *((unsigned char *)pInt + 9) = 0xe;  //0xe~0x11--gpio num
                    *((unsigned char *)pInt + 10) = 0;   // gpio level,0--low,1--high
                    *((unsigned char *)pInt + 11) = 0x2;  //type,0--level triggered ,1--edge triggered ,2--pulse counter
      
                    risc_packet.end = RISC_PKT_END;   
                    if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                        RiscFlagSet();    // Set risc1 flag to interrupt risc1      
                    } else 
                        debug("\r\n                           TwoRiscAudioTARisc1: send packet error!\r\n");  
                    break;

                case '6':    //enable alarm
                    pInt = (unsigned int *)&risc_packet;
          
                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tSys;
                    risc_packet.sub_type = tAlarm;
                    risc_packet.length = 0x4;
                    risc_packet.status = 0; 
          
                    *((unsigned char *)pInt + 8) = 1;   // 1-- enable, 0--disable
                    *((unsigned char *)pInt + 9) = 0xf;  //0xe~0x11--gpio num
                    *((unsigned char *)pInt + 10) = 0;   // gpio level,0--low,1--high
                    *((unsigned char *)pInt + 11) = 0;  //type,0--level triggered ,1--edge triggered ,2--pulse counter
      
                    risc_packet.end = RISC_PKT_END;   
                    if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                        RiscFlagSet();    // Set risc1 flag to interrupt risc1      
                    } else 
                        debug("\r\n                           TwoRiscAudioTARisc1: send packet error!\r\n");  
                    break;

                case '7':    //enable alarm
                    pInt = (unsigned int *)&risc_packet;
          
                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tSys;
                    risc_packet.sub_type = tAlarm;
                    risc_packet.length = 0x4;
                    risc_packet.status = 0; 
          
                    *((unsigned char *)pInt + 8) = 1;   // 1-- enable, 0--disable
                    *((unsigned char *)pInt + 9) = 0xf;  //0xe~0x11--gpio num
                    *((unsigned char *)pInt + 10) = 0;   // gpio level,0--low,1--high
                    *((unsigned char *)pInt + 11) = 0x1;  //type,0--level triggered ,1--edge triggered ,2--pulse counter
      
                    risc_packet.end = RISC_PKT_END;   
                    if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                        RiscFlagSet();    // Set risc1 flag to interrupt risc1      
                    } else 
                        debug("\r\n                           TwoRiscAudioTARisc1: send packet error!\r\n");  
                    break;

                case '8':    //enable alarm
                    pInt = (unsigned int *)&risc_packet;
          
                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tSys;
                    risc_packet.sub_type = tAlarm;
                    risc_packet.length = 0x4;
                    risc_packet.status = 0; 
          
                    *((unsigned char *)pInt + 8) = 0;   // 1-- enable, 0--disable
                    *((unsigned char *)pInt + 9) = 0xf;  //0xe~0x11--gpio num
                    *((unsigned char *)pInt + 10) = 0;   // gpio level,0--low,1--high
                    *((unsigned char *)pInt + 11) = 0x1;  //type,0--level triggered ,1--edge triggered ,2--pulse counter
      
                    risc_packet.end = RISC_PKT_END;   
                    if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                        RiscFlagSet();    // Set risc1 flag to interrupt risc1      
                    } else 
                        debug("\r\n                           TwoRiscAudioTARisc1: send packet error!\r\n");  
                    break;


#endif

#ifdef RISC_DEBUG_SET_CLOCK 
                case 'i':    //Set internal clock from crystal
                    pInt = (unsigned int *)&risc_packet;
          
                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tSys;
                    risc_packet.sub_type = tSetClock;
                    risc_packet.length = 0x4;
                    risc_packet.status = 0; 
          
                    *(pInt + 2) = 0x2;  //0x1----external clock,0x2----internal clock
      
                    risc_packet.end = RISC_PKT_END;   
                    if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                        RiscFlagSet();    // Set risc1 flag to interrupt risc1      
                    } else 
                        debug("\r\nTwoRiscVideoRisc1: send packet error!\r\n");  
                    break;

                case 'e':    //Set external clock from systerm
                    pInt = (unsigned int *)&risc_packet;
          
                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tSys;
                    risc_packet.sub_type = tSetClock;
                    risc_packet.length = 0x4;
                    risc_packet.status = 0; 
          
                    *(pInt + 2) = 0x1;  //0x1----external clock,0x2----internal clock
      
                    risc_packet.end = RISC_PKT_END;   
                    if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                        RiscFlagSet();    // Set risc1 flag to interrupt risc1      
                    } else 
                        debug("\r\nTwoRiscVideoRisc1: send packet error!\r\n");  
                    break;

#endif

#ifdef RISC1_DEBUG_DVR
                /* DVR */
                case '@':    // shift 2: 2835 init
                    pInt = (unsigned int *)&risc_packet;
          
                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tDVR;
                    risc_packet.sub_type = tInit2835;
                    risc_packet.length = 0x8;
                    risc_packet.status = 0; 
          
                    *((unsigned char *)pInt + 8) = 1;    // input PAL or NTSC? 1: PAL; 0: NTSC 
                    *((unsigned char *)pInt + 9) = 1;    // output 4 way or 1 way? 1: 4 way; 0: 1 way 
                    *((unsigned short *)pInt + 5) = DvrIinitCh++;    // channel number: 0 1 2 3
                    *(pInt + 3) =1;    // path: 1: display; 0: record     
                    if(DvrIinitCh >3) DvrIinitCh = 0;
            
                    risc_packet.end = RISC_PKT_END;   
                    if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                        RiscFlagSet();    // Set risc1 flag to interrupt risc1            
                    } else 
                        debug("\r\n                           TwoRiscAudioTARisc1: send packet error!\r\n");  
                    break;
                    
                case '#':    // shift 3: draw line
                    pInt = (unsigned int *)&risc_packet;
          
                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tDVR;
                    risc_packet.sub_type =tDrawLine;
                    risc_packet.length = 0x10;
                    risc_packet.status = 0; 
          
                    *((unsigned short *)pInt + 4) = 100;    // start X position
                    *((unsigned short *)pInt + 5) = 50;    // start Y position
                    *((unsigned short *)pInt + 6) = 300;    // End X position
                    *((unsigned short *)pInt + 7)=50;      //End Y position
                    *(pInt + 4) = 0x00000|(0x00<<4) |0x8 | 0x00  ;                  //attribute | Color | path | page
                     
      
                    risc_packet.end = RISC_PKT_END;   
                    if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                        RiscFlagSet();    // Set risc1 flag to interrupt risc1            
                    } else 
                        debug("\r\n                           TwoRiscAudioTARisc1: send packet error!\r\n");  
                    break;

                case '$':    // shift 4:fill color
                    pInt = (unsigned int *)&risc_packet;
          
                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tDVR;
                    risc_packet.sub_type = tFillColor;
                    risc_packet.length = 0x10;
                    risc_packet.status = 0; 
          
                    *((unsigned short *)pInt + 4) = 360;    // X position
                    *((unsigned short *)pInt + 5) = 288;    // Y position 
                    *((unsigned short *)pInt + 6) = 100;    // Height
                    *((unsigned short *)pInt + 7) = 100;    //width
                    *(pInt + 4) = 0x00000|(0x90<<4) | 0x08| 0x02  ;                  //attribute | Color | path | page   
      
                    risc_packet.end = RISC_PKT_END;   
                    if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                         RiscFlagSet();    // Set risc1 flag to interrupt risc1            
                    } else 
                        debug("\r\n                           TwoRiscAudioTARisc1: send packet error!\r\n");  
                    break;

                case '%':    //tSetOutputMode
                   pInt = (unsigned int *)&risc_packet;
          
                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tDVR;
                    risc_packet.sub_type = tSetOutputMode;
                    risc_packet.length = 0x04;
                    risc_packet.status = 0; 
          
                    *((unsigned short *)(pInt + 2) + 0) = 0x000+1;  // 1-4 ways, 0-1 way
                    //*((unsigned short *)(pInt + 2) + 1) = (TW2835SetOutMode & 0x03);  //0,1,2,3 (valid only when output mode is 1 way)  
                    *((unsigned short *)(pInt + 2) + 1) = 0;      
           
                    risc_packet.end = RISC_PKT_END;   
                    if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                        RiscFlagSet();    // Set risc1 flag to interrupt risc1            
                    } else 
                        debug("\r\n                           TwoRiscAudioTARisc1: send packet error!\r\n");  
                    TW2835SetOutMode++;
                    break;

                case '^':    // shift 6:write string
                    pInt = (unsigned int *)&risc_packet;
          
                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tDVR;
                    risc_packet.sub_type = tWriteString;
                    risc_packet.length = 0x10;
                    risc_packet.status = 0; 

                    unsigned char  *test_str = "0123456789ABCDEF ";
                    unsigned char  test_strlen;
                    
                    test_strlen = strlen(test_str);
          
                    *(pInt + 2) = test_str;    // string address
                    *(pInt + 3) = test_strlen;                   // string length
                    *((unsigned short *)(pInt + 4) + 0) = 60;    // X position 
                    *((unsigned short *)(pInt + 4) + 1) = 50+25*stringcolor;    // Y position
                    *(pInt + 5) = (stringcolor<<8)|0x08 | 0x00;    // path: 1: display; 0: record   //path | page  
                    stringcolor++;
                    if(stringcolor > 29) stringcolor = 0;//BY VINCENT 2007.10.18
                    risc_packet.end = RISC_PKT_END;   
                    if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                        RiscFlagSet();    // Set risc1 flag to interrupt risc1            
                    } else 
                        debug("\r\n                           TwoRiscAudioTARisc1: send packet error!\r\n");  
                    break;

                case '&':    //shift 7:set output mode
                    pInt = (unsigned int *)&risc_packet;
          
                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tDVR;
                    risc_packet.sub_type = tSetOutputMode;
                    risc_packet.length = 0x04;
                    risc_packet.status = 0; 
          
                    *((unsigned char *)(pInt + 2) + 0) =0;  // 0 --1 way ;1-- 4 ways; 3--16 way
                    *((unsigned char *)(pInt + 2) + 1) = 0;  // channel num: 0 ,1, 2, 3
                    //*((unsigned short *)(pInt + 2) + 1) = (TW2835SetOutMode & 0x03);  //0,1,2,3 (valid only when output mode is 1 way)  
                    *((unsigned short *)(pInt + 2) + 1) = 1;// path : 0--display ; 1--record; 2--playback
                    if(DvrSwitchCh >3) DvrSwitchCh = 0;
           
                    risc_packet.end = RISC_PKT_END;   
                    if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                        RiscFlagSet();    // Set risc1 flag to interrupt risc1            
                    } else 
                        debug("\r\n                           TwoRiscAudioTARisc1: send packet error!\r\n");  
                    TW2835SetOutMode++;
                    break;
/*
                case '*':    //shift 8:enable alarm
                    pInt = (unsigned int *)&risc_packet;
          
                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tDVR;
                    risc_packet.sub_type = tAlarm;
                    risc_packet.length = 0x4;
                    risc_packet.status = 0; 
          
                    *((unsigned char *)pInt + 8) = 1;   // 1-- enable, 0--disable
                    *((unsigned char *)pInt + 9) = 0;  //0~3--gpio num
                    *((unsigned char *)pInt + 10) = 0;   // gpio level,0--low,1--high
                    *((unsigned char *)pInt + 11) = 1;  //type,0--level triggered , 1--pulse counter
      
                    risc_packet.end = RISC_PKT_END;   
                    if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                        RiscFlagSet();    // Set risc1 flag to interrupt risc1      
                    } else 
                        debug("\r\n                           TwoRiscAudioTARisc1: send packet error!\r\n");  
                    break;*/

                case '(':    //shift 9:turn on Motion detector 
                    pInt = (unsigned int *)&risc_packet;
          
                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tDVR;
                    risc_packet.sub_type = tMotionDetector;   
                    *(unsigned short *)(pInt + 2) = 0;  // chanael
                    
                    *((unsigned char *)(pInt + 2) + 2) = 0x4a;  // md_lvsens
                    *((unsigned char *)(pInt + 2) + 3) = 0x07;  // md_speedsens
                    *((unsigned char *)(pInt + 3) + 0) = 0x02;  // md_tmpsens
                    *((unsigned char *)(pInt + 3) + 1) = 0x04;  // md_spsens
                    *((unsigned short *)(pInt + 3) + 1) = 1;  // path: 1: display; 1: record 
                    *(pInt + 3) = 0xa1234567;  // md_AreaVectorAddr 
                
                    risc_packet.end = RISC_PKT_END;   
                    if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                        RiscFlagSet();    // Set risc1 flag to interrupt risc1            
                    } else 
                        debug("\r\n                           TwoRiscAudioTARisc1: send packet error!\r\n");  
                    break;
                case ')':    //shift 0:Shut off Motion detector 
                    pInt = (unsigned int *)&risc_packet;
          
                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tDVR;
                    risc_packet.sub_type = tMotionDetector;
                    *(unsigned short *)(pInt + 2) = 0;  // chanael
                    *((unsigned char *)(pInt + 2) + 2) = 0;  // md_lvsens
                    *((unsigned char *)(pInt + 2) + 3) = 0;  // md_speedsens
                    *((unsigned char *)(pInt + 3) + 0) = 0;  // md_tmpsens
                    *((unsigned char *)(pInt + 3) + 1) = 0;  // md_spsens
                    *((unsigned short *)(pInt + 3) + 1) = 1;  // path: 1: display; 2: record 
      
                    risc_packet.end = RISC_PKT_END;   
                    if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                        RiscFlagSet();    // Set risc1 flag to interrupt risc1            
                    } else 
                        debug("\r\n                           TwoRiscAudioTARisc1: send packet error!\r\n");  
                    break; 

                case '[':    //tSetColourLUT
                    pInt = (unsigned int *)&risc_packet;
          
                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tDVR;
                    risc_packet.sub_type = tSetColourLUT;
                    
                    *(pInt + 2) = color_init_index;  // Color Index Addr
                    *((unsigned short *)(pInt + 3) + 0) = 0;  // path: 1: display; 0: record 

                    risc_packet.end = RISC_PKT_END;   
                    if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                        RiscFlagSet();    // Set risc1 flag to interrupt risc1            
                    } else 
                        debug("\r\n                           TwoRiscAudioTARisc1: send packet error!\r\n");  
                    break; 

                case ']':    //tChangeOSDShowPage 
                    pInt = (unsigned int *)&risc_packet;
          
                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tDVR;
                    risc_packet.sub_type = tChangeOSDShowPage;
                    *(unsigned short *)(pInt + 2) = TW2835BitMapPage;  // Current Page
                    *((unsigned char *)(pInt + 2) + 2) = 0;  // path: 1: display; 0: record       
        
                    risc_packet.end = RISC_PKT_END;   
                    if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                        RiscFlagSet();    // Set risc1 flag to interrupt risc1            
                    } else 
                        debug("\r\n                           TwoRiscAudioTARisc1: send packet error!\r\n");  

                    TW2835BitMapPage++;
                    break; 

                case '{':    //tAdjustCamera 
                    for(i=0; i<4; i++){
                         pInt = (unsigned int *)&risc_packet;
            
                         risc_packet.start = RISC_PKT_START;
                         risc_packet.main_type = tDVR;
                         risc_packet.sub_type = tAdjustCamera;
                         *(unsigned char *)(pInt + 2) = 0;  // chanael
                         *((unsigned char *)(pInt + 2) + 1) = 4+i;  // attribute
                         if(i==0)   *((unsigned char *)(pInt + 2) + 1) = 1;  // attribute
        
                         *((unsigned char *)(pInt + 2) + 2) = 0xff & ( 8 * AdjustOSDAttribute);  // level: default 128
                         *((unsigned char *)(pInt + 2) + 3) = 1;  // path: display = 1; record = 0;
          
                         risc_packet.end = RISC_PKT_END;    
                         if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                             RiscFlagSet();    // Set risc1 flag to interrupt risc1            
                         } else 
                              debug("\r\n                           TwoRiscAudioTARisc1: send packet error!\r\n");  
                        }
                    AdjustOSDAttribute++;
                    break; 

                case '}':    //turn on t2DBoxDisplay 
                    pInt = (unsigned int *)&risc_packet;
          
                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tDVR;
                    risc_packet.sub_type = t2DBoxDisplay;
                    *((unsigned short *)(pInt + 2) + 0) = 0;  // chanael
                    *((unsigned short *)(pInt + 2) + 1) = 100;  // start_x
                    *((unsigned short *)(pInt + 3) + 0) = 100;  // start_y
                    *((unsigned short *)(pInt + 3) + 1) = 100;  // heigth
                    *((unsigned short *)(pInt + 4) + 0) = 100;  // width
                    *((unsigned char *)(pInt + 4) + 2) = 0x30 | 0x03;  // Boxnum = Hnum | Vnum 
                    *((unsigned char *)(pInt + 4) + 3) = 0x30 | 0x03;  // Boxcol = M_col | D_col 
                    *((unsigned short *)(pInt + 5) + 0) = 0x10;  // bit 1~0-->border_2DBox ; bit4 --->2DBOXEN=1 
                    *((unsigned short *)(pInt + 5) + 1) = 1;  // path
        
                    risc_packet.end = RISC_PKT_END;   
                    if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                        RiscFlagSet();    // Set risc1 flag to interrupt risc1            
                    } else 
                        debug("\r\n                           TwoRiscAudioTARisc1: send packet error!\r\n");  
                    break; 

                    case ':':    //shut off t2DBoxDisplay 
                    pInt = (unsigned int *)&risc_packet;
          
                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tDVR;
                    risc_packet.sub_type = t2DBoxDisplay;
                    *((unsigned short *)(pInt + 2) + 0) = 0;  // chanael
                    *((unsigned short *)(pInt + 2) + 1) = 100;  // start_x
                    *((unsigned short *)(pInt + 3) + 0) = 100;  // start_y
                    *((unsigned short *)(pInt + 3) + 1) = 100;  // heigth
                    *((unsigned short *)(pInt + 4) + 0) = 100;  // width
                    *((unsigned char *)(pInt + 4) + 2) = 0x30 | 0x03;  // Boxnum = Hnum | Vnum 
                    *((unsigned char *)(pInt + 4) + 3) = 0x30 | 0x03;  // Boxcol = M_col | D_col 
                    *((unsigned short *)(pInt + 5) + 0) = 0x00;  // bit 1~0-->border_2DBox ; bit4 --->2DBOXEN=1 
                    *((unsigned short *)(pInt + 5) + 1) = 1;  // path: 1: display; 0: record 
        
                    risc_packet.end = RISC_PKT_END;   
                    if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                        RiscFlagSet();    // Set risc1 flag to interrupt risc1            
                    } else 
                        debug("\r\n                           TwoRiscAudioTARisc1: send packet error!\r\n");  
                    break; 

                case ';':    //tPlayback
                    for(i=0;i<4;i++)
                    {
                        pInt = (unsigned int *)&risc_packet;
              
                        risc_packet.start = RISC_PKT_START;
                        risc_packet.main_type = tDVR;
                        risc_packet.sub_type = tPlayback;
                        risc_packet.length = 0x10;
                        risc_packet.status = 0; 
              
                        *((unsigned char *)(pInt + 2) + 0) = 1;   // output 4 way or 1 way? 1: 4 way; 0: 1 way 
                        *((unsigned char *)(pInt + 2) + 1) = i;  //pb_outchannel
                        *((unsigned char *)(pInt + 2) + 2) = i;   // pb_inchannel
                        *((unsigned char *)(pInt + 2) + 3) = 0;  //path: 1: display; 0: record 
          
                        risc_packet.end = RISC_PKT_END;   
                        if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                            RiscFlagSet();    // Set risc1 flag to interrupt risc1      
                        } else 
                            debug("\r\n                           TwoRiscAudioTARisc1: send packet error!\r\n");  
                    }
                    break;

                case '?':    //shift 8:enable alarm
                    pInt = (unsigned int *)&risc_packet;
          
                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tDVR;
                    risc_packet.sub_type = tPlayback;
                    risc_packet.length = 0x4;
                    risc_packet.status = 0; 
          
                    *((unsigned char *)pInt + 8) = 1;   // output_mode
                    *((unsigned char *)pInt + 9) = 0;  //pb_outchannel
                    *((unsigned char *)pInt + 10) = 3;   // pb_inchannel
                    *((unsigned char *)pInt + 11) = 1;  //path
      
                    risc_packet.end = RISC_PKT_END;   
                    if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                        RiscFlagSet();    // Set risc1 flag to interrupt risc1      
                    } else 
                        debug("\r\n                           TwoRiscAudioTARisc1: send packet error!\r\n");  
                    break;

                case '<':    //shift 8:enable alarm
                    pInt = (unsigned int *)&risc_packet;
          
                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tDVR;
                    risc_packet.sub_type = tPlayback;
                    risc_packet.length = 0x4;
                    risc_packet.status = 0; 
          
                    *((unsigned char *)pInt + 8) = 1;   // output_mode
                    *((unsigned char *)pInt + 9) = 1;  //pb_outchannel
                    *((unsigned char *)pInt + 10) = 3;   // pb_inchannel
                    *((unsigned char *)pInt + 11) = 1;  //path
      
                    risc_packet.end = RISC_PKT_END;   
                    if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                        RiscFlagSet();    // Set risc1 flag to interrupt risc1      
                    } else 
                        debug("\r\n                           TwoRiscAudioTARisc1: send packet error!\r\n");  
                    break;

                case '>':    //shift 8:enable alarm
                    pInt = (unsigned int *)&risc_packet;
          
                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tDVR;
                    risc_packet.sub_type = tPlayback;
                    risc_packet.length = 0x4;
                    risc_packet.status = 0; 
          
                    *((unsigned char *)pInt + 8) = 1;   // output_mode
                    *((unsigned char *)pInt + 9) = 2;  //pb_outchannel
                    *((unsigned char *)pInt + 10) = 3;   // pb_inchannel
                    *((unsigned char *)pInt + 11) = 1;  //path
      
                    risc_packet.end = RISC_PKT_END;   
                    if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                        RiscFlagSet();    // Set risc1 flag to interrupt risc1      
                    } else 
                        debug("\r\n                           TwoRiscAudioTARisc1: send packet error!\r\n");  
                    break;
                 case '3':    //tSetAudioGain
                    pInt = (unsigned int *)&risc_packet;                    
                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tDVR;
                    risc_packet.sub_type = 0xff;
                    risc_packet.length = 0x8;
                    risc_packet.status = 0;    
                  
                    risc_packet.end = RISC_PKT_END;        
                    if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                        RiscFlagSet();    // Set risc1 flag to interrupt risc1      
                    } else 
                        debug("\r\n                           TwoRiscAudioTARisc1: send packet error!\r\n");  
                    break; 
                case '4':    //tSetAudioGain
                    pInt = (unsigned int *)&risc_packet;                    
                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tDVR;
                    risc_packet.sub_type = tSetAudioGain;
                    risc_packet.length = 0x8;
                    risc_packet.status = 0;    

                   *((unsigned short *)(pInt + 2) + 0) = 3;//CHANNEL  
                   *((unsigned short *)(pInt + 2) + 1)   = 0x0;   //
                   *((unsigned short *)(pInt + 2) + 1) |= 0x1;   //up/down :0 - Inc  ; 1 -- Dec
                   *((unsigned short *)(pInt + 2) + 1) |= (1<<1 );   //direction : 0 - Input ; 1--output
                   *((unsigned short *)(pInt + 2) + 1) |= (1<<2 );   //Gain value
                   *((unsigned short *)(pInt + 2) + 1) |= (1<<15 );   //path
                    risc_packet.end = RISC_PKT_END;        
                    if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                        RiscFlagSet();    // Set risc1 flag to interrupt risc1      
                    } else 
                        debug("\r\n                           TwoRiscAudioTARisc1: send packet error!\r\n");  
                    break; 
                    
                case '7':   
                    pInt = (unsigned int *)&risc_packet;                    
                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tDVR;
                    risc_packet.sub_type = tSetAudioGain;
                    risc_packet.length = 0x8;
                    risc_packet.status = 0;    

                   *((unsigned short *)(pInt + 2) + 0) = 3;//CHANNEL  
                   *((unsigned short *)(pInt + 2) + 1)   = 0x0;   //
                   *((unsigned short *)(pInt + 2) + 1) |= 0x0;   //up/down :0 - Inc  ; 1 -- Dec
                   *((unsigned short *)(pInt + 2) + 1) |= (1<<1 );   //direction : 0 - Input ; 1--output
                   *((unsigned short *)(pInt + 2) + 1) |= (1<<2 );   //Gain value
                   *((unsigned short *)(pInt + 2) + 1) |= (1<<15 );   //path
                    risc_packet.end = RISC_PKT_END;        
                    if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                        RiscFlagSet();    // Set risc1 flag to interrupt risc1      
                    } else 
                        debug("\r\n                           TwoRiscAudioTARisc1: send packet error!\r\n");  
                    break;  
                    
                 case '8':    //tSetAudioGain
                    pInt = (unsigned int *)&risc_packet;                    
                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tDVR;
                    risc_packet.sub_type = tSetAudioGain;
                    risc_packet.length = 0x8;
                    risc_packet.status = 0;    

                   *((unsigned short *)(pInt + 2) + 0) = 0;//CHANNEL  
                   *((unsigned short *)(pInt + 2) + 1)   = 0x0;   //
                   *((unsigned short *)(pInt + 2) + 1) |= 0x1;   //up/down :0 - Inc  ; 1 -- Dec
                   *((unsigned short *)(pInt + 2) + 1) |= (0<<1 );   //direction : 0 - Input ; 1--output
                   *((unsigned short *)(pInt + 2) + 1) |= (1<<2 );   //Gain value
                   *((unsigned short *)(pInt + 2) + 1) |= (1<<15 );   //path
                    risc_packet.end = RISC_PKT_END;        
                    if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                        RiscFlagSet();    // Set risc1 flag to interrupt risc1      
                    } else 
                        debug("\r\n                           TwoRiscAudioTARisc1: send packet error!\r\n");  
                    break; 
                    
                case '9':   
                    pInt = (unsigned int *)&risc_packet;                    
                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tDVR;
                    risc_packet.sub_type = tSetAudioGain;
                    risc_packet.length = 0x8;
                    risc_packet.status = 0;    

                   *((unsigned short *)(pInt + 2) + 0) = 0;//CHANNEL  
                   *((unsigned short *)(pInt + 2) + 1)   = 0x0;   //
                   *((unsigned short *)(pInt + 2) + 1) |= 0x0;   //up/down :0 - Inc  ; 1 -- Dec
                   *((unsigned short *)(pInt + 2) + 1) |= (0<<1 );   //direction : 0 - Input ; 1--output
                   *((unsigned short *)(pInt + 2) + 1) |= (1<<2 );   //Gain value
                   *((unsigned short *)(pInt + 2) + 1) |= (1<<15 );   //path
                    risc_packet.end = RISC_PKT_END;        
                    if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                        RiscFlagSet();    // Set risc1 flag to interrupt risc1      
                    } else 
                        debug("\r\n                           TwoRiscAudioTARisc1: send packet error!\r\n");  
                    break;      
                    
                 case '5':    //tSetAudioLive
                    pInt = (unsigned int *)&risc_packet;
                    
                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tDVR;
                    risc_packet.sub_type = tSetAudioLive;
                    risc_packet.length = 0x8;
                    risc_packet.status = 0;    

                    *(pInt + 2) = 0;//CHANNEL  
                    *(pInt + 3) = 0x88888;   // MIX_RATIO0,MIX_RATIO1,MIX_RATIO02,MIX_RATIO3,MIX_RATIOP
                    *(pInt + 3) |= (0x0 << 20); //MIX_OUTSEL
                    //*(pInt + 3) |= (1 << 24); //Sel / Mix  ----> 0 -- Lv_sel ;1 -- Lv_mix
                    *(pInt + 3) |= (1 << 25); //IMIX_MUTE AIN0 
                    *(pInt + 3) |= (0 << 26); //IMIX_MUTE AIN1  ---->0 --normal;1 -- muted
                    *(pInt + 3) |= (0 << 27); //IMIX_MUTE AIN2
                    *(pInt + 3) |= (0 << 28); //IMIX_MUTE AIN3
                    *(pInt + 3) |= (0 << 29);//IMIX_MUTE PB ---->0 --normal;1 -- muted
                    *(pInt + 3) |= (0 << 30); //MIX_MODE  ---->0-- Average mode;1-- Just summation mode
                    *(pInt + 3) |= (1 << 31);//path             
                    risc_packet.end = RISC_PKT_END;        
                    if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                        RiscFlagSet();    // Set risc1 flag to interrupt risc1      
                    } else 
                        debug("\r\n                           TwoRiscAudioTARisc1: send packet error!\r\n");  
                    break;
                    
                 case '6':    //tSetAudioRecord
                    pInt = (unsigned int *)&risc_packet;                    
                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tDVR;
                    risc_packet.sub_type = tSetAudioRecord;
                    risc_packet.length = 0x8;
                    risc_packet.status = 0;    

                   *((unsigned short *)(pInt + 2) + 0) = 0;//CHANNEL  
                   *((unsigned short *)(pInt + 2) + 1)   = 0x0;   //
                   *((unsigned short *)(pInt + 2) + 1) |= 0x3;   //R_MULTCH
                   *((unsigned short *)(pInt + 2) + 1) |= (0<<2 );   //R_ADATM
                   *((unsigned short *)(pInt + 2) + 1) |= (0<<3 );   //RM_BITRATE
                   *((unsigned short *)(pInt + 2) + 1) |= (0<<4 );   //RM_SAMRATE
                   *((unsigned short *)(pInt + 2) + 1) |= (0<<5 );   //RM_BITWID
                   *((unsigned short *)(pInt + 2) + 1) |= (1<<6 );   //RM_SYNC
                   *((unsigned short *)(pInt + 2) + 1) |= (0<<7 );   //PB_BITRATE
                   *((unsigned short *)(pInt + 2) + 1) |= (0<<8 );   //PB_SAMRATE
                   *((unsigned short *)(pInt + 2) + 1) |= (0<<9 );   //PB_BITWID
                   *((unsigned short *)(pInt + 2) + 1) |= (1<<10 );   //PB_SYNC
                   *((unsigned short *)(pInt + 2) + 1) |= (1<<15 );   //path
                   *(pInt + 3)  = 0x76543210;
                   *(pInt + 4)  = 0xfedcba98;
                    risc_packet.end = RISC_PKT_END;        
                    if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                        RiscFlagSet();    // Set risc1 flag to interrupt risc1      
                    } else 
                        debug("\r\n                           TwoRiscAudioTARisc1: send packet error!\r\n");  
                    break;
                    
    
#endif                    

        //Video Command End
#endif /* VIDEO_MODULE_INCLUDE */
        //Audio Command
        
#ifdef AUDIO_MODULE_INCLUDE

#ifdef RISC1_DEBUG_AUDIO
                case '!':
                    printf("You have pressed a key to stop!");
                    OS_ENTER_CRITICAL();
                    while(1);
                    break;

                case 'l':
                    debug("\nSet Channel, please enter '0', '1', '2' or '3'\n");
                    receive1 = WaitPressKey();
                    if (receive1 >= '0' && receive1 <= '3')
                    {
                        channelID = (int) receive1 - '0';
                    }
                    else
                    {
                      channelID = 0;
                    }
                    debug("Set channel: %d\n", channelID);
                    SetAudioCodecMsg[0] = channelID;
                    OpenAudioMsgCMD[0] = channelID;
            
                    debug("\nSet Talker of Channel %d , please enter '0', '1', '2' or '3'\n", channelID);
                    receive1 = WaitPressKey();
                    if (receive1 >= '0' && receive1 <= '3')
                    {
                        talkerID = (int) receive1 - '0';
                    }
                    else
                    {
                        talkerID = 0;
                    }
                    SetAudioCodecMsgG711UNoEC[8] = talkerID;
                    SetAudioCodecMsgG711ANoEC[8] = talkerID;
                    SetAudioCodecMsgG726_16kNoEC[8] = talkerID;
                    SetAudioCodecMsgG726_24kNoEC[8] = talkerID;
                    SetAudioCodecMsgG726_32kNoEC[8] = talkerID;
                    SetAudioCodecMsgG726_40kNoEC[8] = talkerID;
                    SetAudioCodecMsgG729ANoEC[8] = talkerID;
                    SetAudioCodecMsgG723_1_24NoEC[8] = talkerID;
            
                    debug("Set taker: %d\n", talkerID);
            
                    break;
            
                case 'g':
                    debug("\nSet G7xx, please enter:\n");
                    debug("'0' for G711a\n");
                    debug("'1' for G711u\n");
                    debug("'2' for G726_24k\n");
                    debug("'3' for G726_32k\n");
                    debug("'4' for G726_40k\n");
                    debug("'5' for G729A\n");
                    debug("'6' for G723.1 24 Bytes\n");
                    receive1 = WaitPressKey();
                    switch (receive1)
                    {
                        case '0':
                            memcpy((int*)MSG_BODY_ADDR, SetAudioCodecMsgG711ANoEC, sizeof(SetAudioCodecMsgG711UNoEC));
                            debug("You Set G7xx to G711A\n");
                            break;
                      
                        case '1':
                            memcpy((int*)MSG_BODY_ADDR, SetAudioCodecMsgG711UNoEC, sizeof(SetAudioCodecMsgG711UNoEC));
                            debug("You Set G7xx to G711U\n");
                            break;
            
                        case '2':
                            memcpy((int*)MSG_BODY_ADDR, SetAudioCodecMsgG726_24kNoEC, sizeof(SetAudioCodecMsgG711UNoEC));
                            debug("You Set G7xx to G726_24k\n");
                            break;
                      
                        case '3':
                            memcpy((int*)MSG_BODY_ADDR, SetAudioCodecMsgG726_32kNoEC, sizeof(SetAudioCodecMsgG711UNoEC));
                            debug("You Set G7xx to G726_32k\n");
                            break;
                      
                        case '4':
                            memcpy((int*)MSG_BODY_ADDR, SetAudioCodecMsgG726_40kNoEC, sizeof(SetAudioCodecMsgG711UNoEC));
                            debug("You Set G7xx to G726_40k\n");
                            break;
                      
                        case '5':
                            memcpy((int*)MSG_BODY_ADDR, SetAudioCodecMsgG729ANoEC, sizeof(SetAudioCodecMsgG711UNoEC));
                            debug("You Set G7xx to G729A\n");
                            break;
                      
                        case '6':
                            memcpy((int*)MSG_BODY_ADDR, SetAudioCodecMsgG723_1_24NoEC, sizeof(SetAudioCodecMsgG711UNoEC));
                            debug("You Set G7xx to G723_1_24\n");
                            break;
            
                    }
                    debug("channel %d, talker %d\n", channelID,talkerID);
                    break;
                      
                case 'i':    // tAudioOpen  
                    //channel 0
                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tAudio;
                    risc_packet.sub_type = tAudioStartTone;
                    risc_packet.length = 0x8;
                    risc_packet.status = 0; 
                    *((int *)(&risc_packet.message[0])) = 0;
                    *((int *)(&risc_packet.message[4])) = 1;
                    //(int)risc_packet.message[0] = 0;
                    //(int)risc_packet.message[4] = 1L;
                    risc_packet.end = RISC_PKT_END;   
            
                    if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                        RiscFlagSet();    // Set risc1 flag to interrupt risc1            
                    } else 
                        debug("\r\nTwoRiscAudioTARisc1: send packet error!\r\n");            
                    // break;
                    //sleep(10);
                    //channel 1
                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tAudio;
                    risc_packet.sub_type = tAudioStartTone;
                    risc_packet.length = 0x8;
                    risc_packet.status = 0; 
                    *((int *)(&risc_packet.message[0])) = 1;
                    *((int *)(&risc_packet.message[4])) = 1;
                    //(int)risc_packet.message[0] = 1;
                    //(int)risc_packet.message[4] = 1;
                    risc_packet.end = RISC_PKT_END;   
            
                    if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                        RiscFlagSet();    // Set risc1 flag to interrupt risc1            
                    } else 
                        debug("\r\nTwoRiscAudioTARisc1: send packet error!\r\n");            
                    break;
                    
                case 'w':
                    debug("sWap Stream!\n");
                    debug(" Press 0 for channel 0 input  no_sWap\n");
                    debug(" Press 1 for channel 0 input  2Byte_sWap\n");
                    debug(" Press 2 for channel 0 output no_sWap\n");
                    debug(" Press 3 for channel 0 output 2Byte_sWap\n");
                    debug(" Press 4 for channel 1 input  no_sWap\n");
                    debug(" Press 5 for channel 1 input  2Byte_sWap\n");
                    debug(" Press 6 for channel 1 output no_sWap\n");
                    debug(" Press 7 for channel 1 output 2Byte_sWap\n");
                    receive1 = WaitPressKey();
                    printf("%c", receive1);
                    switch(receive1) 
                    {
                        case '0':
                            context = ScRdContext(AUD_IN_CH0_S);
                            context &= 0xfffff9ff;
                            rSTREAM_ADDR(AUD_IN_CH0_S, W_CONTEXT_L) = context;
                            break;
                  
                        case '1':
                            context = ScRdContext(AUD_IN_CH0_S);
                            context &= 0xfffffbff;
                            context |= 0x00000200;
                            rSTREAM_ADDR(AUD_IN_CH0_S, W_CONTEXT_L) = context;
                            break;
                          
                        case '2':
                            context = ScRdContext(AUD_OUT_CH0_S);
                            context &= 0xfffff9ff;
                            rSTREAM_ADDR(AUD_IN_CH0_S, W_CONTEXT_L) = context;
                            break;
                  
                        case '3':
                            context = ScRdContext(AUD_OUT_CH0_S);
                            context &= 0xfffffbff;
                            context |= 0x00000200;
                            rSTREAM_ADDR(AUD_IN_CH0_S, W_CONTEXT_L) = context;
                            break;
                  
                        case '4':
                            context = ScRdContext(AUD_IN_CH1_S);
                            context &= 0xfffff9ff;
                            rSTREAM_ADDR(AUD_IN_CH1_S, W_CONTEXT_L) = context;
                            break;
                  
                        case '5':
                            context = ScRdContext(AUD_IN_CH1_S);
                            context &= 0xfffffbff;
                            context |= 0x00000200;
                            rSTREAM_ADDR(AUD_IN_CH1_S, W_CONTEXT_L) = context;
                            break;
                          
                        case '6':
                            context = ScRdContext(AUD_OUT_CH1_S);
                            context &= 0xfffff9ff;
                            rSTREAM_ADDR(AUD_IN_CH1_S, W_CONTEXT_L) = context;
                            break;
                  
                        case '7':
                            context = ScRdContext(AUD_OUT_CH1_S);
                            context &= 0xfffffbff;
                            context |= 0x00000200;
                            rSTREAM_ADDR(AUD_IN_CH1_S, W_CONTEXT_L) = context;
                            break;
            
                    }
                    debug("Contex=%x\n",context);
            
                    break;
                    
#ifdef DUMP_ENABLE
                case 'p':
                    debug("\nPrint dumped data. Please enter '0','1','2'...9\n");            
                    receive1 = WaitPressKey();
                    printf("%c", receive1);
                    switch(receive1) 
                    {
                        case '0':  
                        case '1':  
                        case '2':  
                        case '3':  
                        case '4':  
                        case '5':  
                        case '6':  
                        case '7':  
                        case '8':  
                        case '9':  
                            AppReadDumpedData(receive1-'0');
                            break;
                    }
                    break;
#endif        
                case 't':
                    debug("Test Slic Tone, Please enter 0 or 1\n");            
                    receive1 = WaitPressKey();
                    printf("%c", receive1);
                    switch(receive1) 
                    {
                        case '0':    // tAudioOpen  
                            //channel 0
                            risc_packet.start = RISC_PKT_START;
                            risc_packet.main_type = tAudio;
                            risc_packet.sub_type = tAudioStartTone;
                            risc_packet.length = 0x8;
                            risc_packet.status = 0; 
                            *((int *)(&risc_packet.message[0])) = 0;
                            *((int *)(&risc_packet.message[4])) = 4;
                            //(int)risc_packet.message[0] = 0;
                            //(int)risc_packet.message[4] = 4;
                            risc_packet.end = RISC_PKT_END;   
                  
                            if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                                RiscFlagSet();    // Set risc1 flag to interrupt risc1            
                            } else 
                                debug("\r\nTwoRiscAudioTARisc1: send packet error!\r\n");            
                            break;
                        case '1':    // tAudioOpen  
                            //channel 0
                            risc_packet.start = RISC_PKT_START;
                            risc_packet.main_type = tAudio;
                            risc_packet.sub_type = tAudioStartTone;
                            risc_packet.length = 0x8;
                            risc_packet.status = 0; 
                            *((int *)(&risc_packet.message[0])) = 1;
                            *((int *)(&risc_packet.message[4])) = 4;
                            //(int)risc_packet.message[0] = 1;
                            //(int)risc_packet.message[4] = 4;
                            risc_packet.end = RISC_PKT_END;   
                  
                            if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                                RiscFlagSet();    // Set risc1 flag to interrupt risc1            
                            } else 
                                debug("\r\nTwoRiscAudioTARisc1: send packet error!\r\n");            
                            break;
                  
                    }
                    break;
            
                case 'o':    // tAudioOpen  
                    debug("Risc1: Sent OpenAudio Command!\n");
                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tAudio;
                    risc_packet.sub_type = tAudioOpen;
                    risc_packet.length = 0xC;
                    risc_packet.status = 0;
                    memcpy(risc_packet.message, OpenAudioMsgCMD, sizeof(OpenAudioMsgCMD));
                    risc_packet.end = RISC_PKT_END;
            
            
                    if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                        RiscFlagSet();    // Set risc1 flag to interrupt risc1            
                    } else 
                        debug("\r\nTwoRiscAudioTARisc1: send packet error!\r\n");            
                    break;
            
            
                case 'c':    //Set Audio Codec  
                    debug("Risc1: Set Audio Codec!\n");
                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tAudio;
                    risc_packet.sub_type = tAudioSetAudioCodec;
                    risc_packet.length = 0x8;
                    risc_packet.status = 0;
                    memcpy(risc_packet.message, SetAudioCodecMsg, risc_packet.length);
                    risc_packet.end = RISC_PKT_END;
            
                    if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                        RiscFlagSet();    // Set risc1 flag to interrupt risc1            
                    } else 
                        debug("\r\nTwoRiscAudioTARisc1: send packet error!\r\n");            
                    break;
            
            
                case 's':    // start voice 
                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tAudio;
                    risc_packet.sub_type = tAudioStartVoice;
                    risc_packet.length = 0x4;
                    risc_packet.status = 0;   
                    memcpy(risc_packet.message, &channelID, risc_packet.length);
                    risc_packet.end = RISC_PKT_END;   
            
                    if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                        RiscFlagSet();    // Set risc1 flag to interrupt risc1            
                    } else 
                        debug("\r\nTwoRiscAudioTARisc1: send packet error!\r\n");            
                    break;
            
                case 'e':    // stop voice  
                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tAudio;
                    risc_packet.sub_type = tAudioStopVoice;
                    risc_packet.length = 0x4;
                    risc_packet.status = 0;   
                    memcpy(risc_packet.message, &channelID, risc_packet.length);
                    risc_packet.end = RISC_PKT_END;   
            
                    if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                        RiscFlagSet();    // Set risc1 flag to interrupt risc1            
                    } else 
                        debug("\r\nTwoRiscAudioTARisc1: send packet error!\r\n");            
                    break;
            
                case 'a': //add one talker
                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tAudio;
                    risc_packet.sub_type = tAddOneTalker;
                    risc_packet.length = 0x8;
                    risc_packet.status = 0;
                    risc_packet.end = RISC_PKT_END;   
            
                    pInt = (int*) risc_packet.message;
                    *pInt = channelID;
                    pInt++;
                    *pInt = talkerID;
            
                    debug("A %d,%d ", channelID, talkerID);
            
                    if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                        RiscFlagSet();    // Set risc1 flag to interrupt risc1            
                    } else 
                        debug("\r\nTwoRiscAudioTARisc1: send packet error!\r\n");            
                    break;
            
                case 'd': //del one talker
                    debug("\nDel Channel X?, please enter 0~3\n");
                    receive1 = WaitPressKey();
                    if (receive1 >= '0' && receive1 <= '3')
                    {
                        i = (int) receive1 - '0';
                    }
                    else
                    {
                        i = 0;
                    }
                    debug("\nDel Channel %d Talker X?, 0~3\n", i);
                    receive1 = WaitPressKey();
                    if (receive1 >= '0' && receive1 <= '3')
                    {
                        j = (int) receive1 - '0';
                    }
                    else
                    {
                        j = 0;
                    }
                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tAudio;
                    risc_packet.sub_type = tDelOneTalker;
                    risc_packet.length = 0x8;
                    risc_packet.status = 0;
                    risc_packet.end = RISC_PKT_END;   
            
                    pInt = (int*) risc_packet.message;
                    *pInt = i;
                    pInt++;
                    *pInt = j;
            
                    debug("D %d,%d ", i, j);
            
                    if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                        RiscFlagSet();    // Set risc1 flag to interrupt risc1            
                    } else 
                        debug("\r\nTwoRiscAudioTARisc1: send packet error!\r\n");            
                    break;
                    
                case 'm':    // set code mode:
                {
                    int key_val;
                    unsigned int *pkg_msg;
                    pkg_msg = (int *)(risc_packet.message);
                    *pkg_msg = channelID;
                    
                    debug("\ncode mode:\n");
                    debug("'0' disble all\n");
                    debug("'1' enable encode\n");
                    debug("'2' enable decode\n");
                    debug("'3' enable encode and decode\n");
                    debug("'4' disable encode\n");
                    debug("'5' disable decode\n");
                    key_val = WaitPressKey();
                    switch(key_val)
                    {
                        /*
                        case '0':
                            *(pkg_msg+1) = AUDIO_DO_ENCODE | AUDIO_DO_DECODE;
                            break;
                        case '1':
                            *(pkg_msg+1) = AUDIO_DO_ENCODE | 0x1<<16;
                            break;
                        case '2':
                            *(pkg_msg+1) = AUDIO_DO_DECODE | 0x1<<16;
                            break;
                        case '3':
                                  *(pkg_msg+1) = AUDIO_DO_ENCODE | AUDIO_DO_DECODE | 0x1<<16;
                            break;
                        case '4':
                            *(pkg_msg+1) = AUDIO_DO_ENCODE;
                            break;
                        case '5':
                            *(pkg_msg+1) = AUDIO_DO_DECODE;
                            break;
                        
                        default:
                            *(pkg_msg+1) = AUDIO_DO_ENCODE | AUDIO_DO_DECODE | 0x1<<16;
                            break;
                              */
                    }
                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tAudio;
                    risc_packet.sub_type = tSetEnAudioMode;
                    risc_packet.length = 0x8;
                    risc_packet.status = 0;
                    risc_packet.end = RISC_PKT_END;   
            
                    if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                      RiscFlagSet();    // Set risc1 flag to interrupt risc1            
                    } else 
                      debug("\r\nTwoRiscAudioTARisc1: send packet error!\r\n");            
                    break;
                }
                
                case 'v':    //
                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tAudio;
                    risc_packet.sub_type = tSetVoiceUP;
                    risc_packet.length = 0x8;
                    risc_packet.status = 0;
                    risc_packet.end = RISC_PKT_END;   
                    if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL)
                        RiscFlagSet();    // Set risc1 flag to interrupt risc1            
                    else 
                        debug("\r\nTwoRiscAudioTARisc1: send packet error!\r\n");            
                    break;
                    
                case 'f':
                    risc_packet.start = RISC_PKT_START;
                    risc_packet.main_type = tAudio;
                    risc_packet.sub_type = tSetVoiceDown;
                    risc_packet.length = 0x8;
                    risc_packet.status = 0;
                    risc_packet.end = RISC_PKT_END;   
                    if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL)
                        RiscFlagSet();    // Set risc1 flag to interrupt risc1            
                    else 
                        debug("\r\nTwoRiscAudioTARisc1: send packet error!\r\n");            
                    break;
            
#endif            
                    
#endif
                default:
                    printf("\n\nTaskTwoRiscSystemMonitorRisc1: command error!\n\n");
                    break;    
                }
                    
        }
                  
        //OSTimeDlyHMSM(0, 0, 0, 500);            
    }
}

