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
#include "vout_interface.h"
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
#include "two_risc_rollstr.h"
#include "Motion_Detection.h"
#include "Camera.h"
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

extern int gCameraSelectDigital;
extern pCamera pcurrent_camera;

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


/*
***************************************************************************
*                          TaskTwoRiscSystemMonitorRisc0
*
* Description: the task running when multi tasks begin
*
* Arguments  : 
*
* Return     : 
*
* Note(s)    : 
***************************************************************************
*/

unsigned int SetVMode = 1;//
extern unsigned int gVoutClkSet;
void TaskTwoRiscSystemMonitorRisc0(void *data)
{
    unsigned char err;
    unsigned int msg;   
    unsigned short ret = 0;
    OS_TCB tcb_data;    

    unsigned int temp;

    unsigned int vstd = 0;

    
    BspStartTicker(OS_TICKS_PER_SEC);          

    OSStatInit();

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

#ifdef AUDIO_MODULE_INCLUDE
    gptAudioTaskReady = 0;      
#endif

#ifdef VIDEO_MODULE_INCLUDE
    tVideoLinuxToUcos *pVideoPackage;
    unsigned int VideoStatusBak;
    unsigned int *pInt;
    Risc_Packet_t risc_packet;
    VideoStatusBak = 0;
    gVideoStatus = 0;
    
/*******************************************************************
the code below is to fix the audio input channel 0 queue underflow bug, 
details see bug number 5 in "avsyncbuglist.xls" in buglist.
*******************************************************************/
    McScMap(VSC_RD_Y0_M, VSC_RD_Y0_S);
    McScMap(VSC_RD_U0_M, VSC_RD_U0_S);
    McScMap(VSC_RD_V0_M, VSC_RD_V0_S);
    
    McScMap(VSC_RD_Y1_M, VSC_RD_Y1_S);
    McScMap(VSC_RD_U1_M, VSC_RD_U1_S);
    McScMap(VSC_RD_V1_M, VSC_RD_V1_S);
/*******************************************************************/

    gtVinFormat.yuv_format = YUV420;
    gtVinFormat.camera_format = NTSC; //0: NTSC mode, 1: PAL mode
    if(gtVinFormat.camera_format == PAL) {
        gtVinFormat.width = 704;
        gtVinFormat.height = 576;
    } else {
        gtVinFormat.width = 704;
        gtVinFormat.height = 480;
    }

    decode_flag = MPEG_ENCODE;  //20071129;indicate the encode mode of the bit stream default is MPEG mode

#ifdef DVR    
    gChecksumEnabled = TRUE;   // enable checksum
#else
    gChecksumEnabled = FALSE;   // please do not change the default value
#endif

#ifdef DVR    
    gImage_Mode = EVENODD_IMAGE;//MERGED_IMAGE;
#else
    gImage_Mode = EVENODD_IMAGE;
#endif

    gImgEncodeMode = ENCODEBOTH;
#endif

#ifdef DVR
    MM74HC595Init();
#endif

    gtPrintMsgControl.en_print_all_msg = 1;
    gtPrintMsgControl.en_print_periodical_msg = 1;

    memset((void *)&gtStatistics, 0, sizeof(gtStatistics));
    
    /* David's email on Dec 12 to improve performance for video & mpeg */
    *(volatile int *)0xa8040000 = 0x1680596;
    *(volatile int *)0xa8020000 = 0x11115;      
    *(volatile int *)0xa8020004 = 0x0a77;      
    
    RiscIntrInit();    
    RiscFlagIntrInit();  
    IICinit();
    TwoRiscCommInit();      
     
#ifdef DUAL_CPU_MODE
    RiscSemUnLock(7);    // tell risc1 ISR table has been created!
#endif

    gptQMsgTwoRiscSystemMonitor = OSQCreate(&ArrayQMsgSysMonitor[0], ArrayQMsgSysMonitor_MAX);   

#ifdef VIDEO_MODULE_INCLUDE

    gRvoDisplayMode = RVO_DISPLAY_BOTH;

    gFreqFromCrystal = TRUE;   //TRUE: external, FALSE:internal
    gVoutClkSet = FALSE;
    gCameraSelectDigital = TRUE;//TRUE:digital,FALSE:analog

extern unsigned int gVinOddEvenFrameSelect;
extern unsigned int gVscOddEvenFrameSelect; 

   gVinOddEvenFrameSelect = ODD_FRAME_FIRST;
#ifdef DVR
    gVscOddEvenFrameSelect = ODD_FRAME_FIRST;    
#else
    gVscOddEvenFrameSelect = EVEN_FRAME_FIRST;
#endif
#ifdef MOTION_DETECTION     //wy:motion detect init
    gMotionSensi = 0;          //default disable motion detection.
    gIsMDEnabled = FALSE;    //default disable motion detection.
    gMdType = mdtemporalDif;  //default motion detection method.
#endif

#ifdef BAND_WIDTH_CONTROL
    set_cbr_default();
#endif

#ifdef AV_SYNC
    set_avsync_default();
#endif

#ifdef DISPLAY_INCLUDE

#ifdef VIDEO_JITTER_BUFFER
    QEeventVodeoL2U = OSQCreate(pQVideoL2UPackage, VIDEO_L2U_PKG_NUMBER);
    if(QEeventVodeoL2U == NULL)
        _PRINTF("\r\nxxxxxxxx: Risc0: OSQCreate QEeventVodeoL2U error!\r\n");
#endif

#ifdef STARTUP_PICTURE
    err = OSQPost(gptQMsgTwoRiscSystemMonitor, (void *)TWO_RISC_SYSTEM_OPEN_VOUT);
    if(err != OS_NO_ERR)    		
        printf("\r\nRISC0: OSMboxPost error! err = %d!\r\n", err);      		
#endif

#endif
    
#endif  	

#ifdef TASK_STATISTIC_EN 
    if(
       #ifdef CHIP_CFG_STACK_CHECK
       OSTaskCreateExt(TaskStatistics, (void *)0, (void *)&StkTaskStatistics[sizeof(StkTaskStatistics) / sizeof(StkTaskStatistics[0]) - 1], PRIOR_TASK_STATISTICS,\
                PRIOR_TASK_STATISTICS, (void *)&StkTaskStatistics[0], sizeof(StkTaskStatistics) / sizeof(StkTaskStatistics[0]), (void *)0, OS_TASK_OPT_STK_CHK+OS_TASK_OPT_STK_CLR) != OS_NO_ERR	
       #else                    
       OSTaskCreate(TaskStatistics, (void *)0, (void *)&StkTaskStatistics[STK_SIZE_TaskStatistics - 1], PRIOR_TASK_STATISTICS) != OS_NO_ERR
       #endif    
    )
    {
        debug ("\r\nTaskCreate TaskCount1s error\r\n");
    }
#endif    

  //RiscTimerInit(1000000);
    while(1) {
#if 0        
//#ifdef VIDEO_JITTER_BUFFER
        pVideoPackage = (tVideoLinuxToUcos *)OSQPend(QEeventVodeoL2U, 200 * OS_TICKS_PER_SEC /1000, &err);
        if(err == OS_NO_ERR) {
            VideoFrameReceive.addr = pVideoPackage->addr;
            VideoFrameReceive.size = pVideoPackage->len;
            VideoFrameReceive.TimeStamp = pVideoPackage->tstamp;
            err = VBufSaveOneFrame(&VideoBuffer1, &VideoFrameReceive);
            if(err)
                printf("xxx save viedo frame to buffer error! err = %d\n", err);
            else
            {
                //printf("depth = %d, space = %d\n", VideoFrameReceive.CurrentDepth, VideoFrameReceive.CurrentSpace);
                //printf("w = 0x%8.8x, %d, %d\n", VideoFrameReceive.addr, VideoFrameReceive.size, VideoFrameReceive.TimeStamp);
            }
            RiscDecCNT(VIDEO_LINUX_TO_UCOS_CNT, 1);
            if(OSSemPost(gptVideoLinuxToUcosMbox) != OS_NO_ERR)
                printf("Risc0:OSMboxPost error!\r\n");  
        }
        else if(err == OS_TIMEOUT)
            ;//printf("ucos receive video package time out!\n");

        msg = (unsigned int)OSQAccept(gptQMsgTwoRiscSystemMonitor);
        if(msg)
#else
        msg = (unsigned int)OSQPend(gptQMsgTwoRiscSystemMonitor, 200 * OS_TICKS_PER_SEC /1000, &err);
        if(err == OS_NO_ERR)
#endif
        {
            switch(msg) {
                
                case TWO_RISC_SYSTEM_RECEIVE_LINUX_MSG:
                    while(1) {
                        if(TwoRiscCommRecPacket() != SUCCESSFUL) {
                            break;                
                        }
                    }                        
                    break;
                
#ifdef VIDEO_MODULE_INCLUDE
#ifdef DISPLAY_INCLUDE
                case TWO_RISC_SYSTEM_CLOSE_VOUT:       
                    gVoutEnable = 0;
                    break;  

                case TWO_RISC_SYSTEM_CLOSE_DISPLAY_TASK:     	
                    gVoutEnable = 0;

                    printf("Close Display Task!\n");

                    //del task
                    if(OSTaskDel(PRIOR_TASK_DISPLAY) != OS_NO_ERR )
                        printf("OSTaskDel failed!\n");
                    else
                        printf("OSTaskDel OK!\n");

                    OSSemDel_w(gptMpegDecodeReadyToStart, OS_DEL_ALWAYS, &err);
                    OSSemDel_w(gptVideoLinuxToUcosMbox, OS_DEL_ALWAYS, &err);

#ifdef AV_SYNC
                    OSSemDel_w(gptRiscTimerIntrOccur, OS_DEL_ALWAYS, &err);
#endif
                    OSQDel(gptMpeg4DecodeOutputReady, OS_DEL_ALWAYS, &err);
                    if(err != OS_NO_ERR)
                        printf("OSQDel failed!\n"); 
#ifdef PIP_ENABLE
                    OSQDel (gptMqueueVideoInFrame, OS_DEL_ALWAYS, &err);
                    if(err != OS_NO_ERR)
                        printf("OSQDel failed, err = %d!", err);
                    gVideoStatus &= ~tPipEn;
#endif
                    break;                    

                case TWO_RISC_SYSTEM_OPEN_VOUT:  
                    if(OSTaskQuery(PRIOR_TASK_DISPLAY, &tcb_data) == OS_NO_ERR) {   
                    //if(OSTaskCreate(TaskTwoRiscDisplay, (void *)0, (void *)&StkTaskTwoRiscDisplay[STK_SIZE_TaskTwoRiscDisplay - 1], PRIOR_TASK_DISPLAY) != OS_NO_ERR) {
                        if(gVoutEnable == 0) {
                            gVoutEnable = 1;    

                            GetVoutInterface();
                            VoutInterfaceIntialize(gtDisplayFormat.vout_format);
                            /*
                            printf("Init 7171 ");   
                            if(HwADV7171Initialize(gtDisplayFormat.vout_format) == SUCCESSFUL)    // 0: NTSC mode,1: pal mode           
                                printf("ok\n");
                            else 
                                printf("failed\n");
                            */
                            if(gtDisplayFormat.vout_format == LCD) {
                                gtDisplayFormat.vout_interlaced = FALSE;            
                            } else {  // NTSC or PAL
                                gtDisplayFormat.vout_interlaced = TRUE;    
                            }

#ifdef OSD_ENABLE
                            unsigned int task_rollstr_flag = 0;
                            if(gtOsdCurrent.osd_format!=gtDisplayFormat.vout_format){
                                if(OSTaskQuery(PRIOR_TASK_ROLLSTR, &tcb_data) == OS_NO_ERR){
                                    task_rollstr_flag = 1;
                                    if(OSTaskDel(PRIOR_TASK_ROLLSTR) != OS_NO_ERR )
                                        debug("OSTaskDel(PRIOR_TASK_ROLLSTR) failed!\n");
                                }
                                    

                                if(gtDisplayFormat.vout_format == NTSC){
                                    gtOsdCurrent.osd_format = NTSC;
                                    gtOsdCurrent.width = 704;
                                    gtOsdCurrent.height = 480;
                                } else if(gtDisplayFormat.vout_format == PAL){
                                    gtOsdCurrent.osd_format = PAL;
                                    gtOsdCurrent.width = 704;
                                    gtOsdCurrent.height = 576;
                                } else if(gtDisplayFormat.vout_format == LCD){
                                    gtOsdCurrent.osd_format = LCD;
                                    gtOsdCurrent.width = 480;
                                    gtOsdCurrent.height = 240;
                                }
                                //printf("\ngtOsdCurrent.width=%d,gtOsdCurrent.height=%d\n", gtOsdCurrent.width, gtOsdCurrent.height);
                                gtOsdCurrent.start_pixel = 0;
                                gtOsdCurrent.start_line = 0;
                                gtOsdCurrent.backgroundColor = 15;
                                gtOsdCurrent.OSD_T_Mask = 15;
                                gtOsdCurrent.fill_addr = (((unsigned int)OSD_memory_0)&0x1fffffff);
                                gtOsdCurrent.display_addr = (((unsigned int)OSD_memory_0)&0x1fffffff);
                                InitOsdMemory(gtOsdCurrent.start_pixel ,gtOsdCurrent.start_line, gtOsdCurrent.width, gtOsdCurrent.height , gtOsdCurrent.backgroundColor, gtOsdCurrent.fill_addr, gtOsdCurrent.OSD_T_Mask);


                                if(task_rollstr_flag == 1){
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
                                        debug("\r\nTaskCreate TaskRollString OK!\r\n");
                                    }
                                }
                            }                        
#endif

                            VscInit();                               
                            AppVscStart();              						   
                            gVideoStatus |= tVoutEn;
                        }
                    } else {    
                        TwoRiscDisplayRvoPipOsdInit();
                        if(
                           #ifdef CHIP_CFG_STACK_CHECK
                           OSTaskCreateExt(TaskTwoRiscDisplay, (void *)0, (void *)&StkTaskTwoRiscDisplay[sizeof(StkTaskTwoRiscDisplay) / sizeof(StkTaskTwoRiscDisplay[0]) - 1], PRIOR_TASK_DISPLAY,\
                                    PRIOR_TASK_DISPLAY, (void *)&StkTaskTwoRiscDisplay[0], sizeof(StkTaskTwoRiscDisplay) / sizeof(StkTaskTwoRiscDisplay[0]), (void *)0, OS_TASK_OPT_STK_CHK+OS_TASK_OPT_STK_CLR) != OS_NO_ERR	
                           #else                    
                            OSTaskCreate(TaskTwoRiscDisplay, (void *)0, (void *)&StkTaskTwoRiscDisplay[STK_SIZE_TaskTwoRiscDisplay - 1], PRIOR_TASK_DISPLAY) != OS_NO_ERR
                           #endif                        
                        )
                            printf("\r\nTaskTwoRiscDisplay create error!\r\n");    
                    }
                    break;

                case TWO_RISC_SYSTEM_CLOSE_PIP:     
                    gPipEnable = 0;
                    break;

                case TWO_RISC_SYSTEM_CLOSE_RVO:     
                    gRvoEnable = 0;
                    break;                    
                    
                case TWO_RISC_SYSTEM_CLOSE_OSD:     
                    gOsdEnable = 0;
                    break;

                case TWO_RISC_SYSTEM_OPEN_RVO:
                    if(gRvoEnable == 0) {
                        if(gVoutEnable == 1) {
                            gVoutReEnable = 1;
                        }
                        gRvoEnable = 1;
                    }
                    break;
                    
                case TWO_RISC_SYSTEM_OPEN_PIP:
                    if(gPipEnable == 0) {
                        if(gVoutEnable == 1) {
                            gVoutReEnable = 1;
                        }
                        gPipEnable = 1;
                    }
                    break;

#ifdef OSD_ENABLE                    
                case TWO_RISC_SYSTEM_OPEN_OSD:  	
                    if(gOsdEnable==0) {
                        if(gVoutEnable == 1)
                            gVoutReEnable = 1;
                            gOsdEnable =1;
                    }

                    /*if (OSTaskCreate(TaskRollString, (void *)0, (void *)&StkTaskRollStr[STK_SIZE_ROLLSTR - 1], PRIOR_TASK_ROLLSTR) != OS_NO_ERR)
                    {
                        debug ("\r\nTaskCreate TaskRollString error!\r\n");
                    } else
                        printf("\r\nTaskCreate TaskRollString succed!\r\n");*/
                    break;
#endif
                    
#endif                    
                case TWO_RISC_SYSTEM_OPEN_VIN:

                    #ifdef CHIP_CFG_STACK_CHECK
                    OSTaskCreateExt(TaskTwoRiscVinMpegEncode, (void *)0, (void *)&StkTaskTwoRisc0IPCamera[sizeof(StkTaskTwoRisc0IPCamera) / sizeof(StkTaskTwoRisc0IPCamera[0]) - 1], PRIOR_TASK_IPCAM,\
                            PRIOR_TASK_IPCAM, (void *)&StkTaskTwoRisc0IPCamera[0], sizeof(StkTaskTwoRisc0IPCamera) / sizeof(StkTaskTwoRisc0IPCamera[0]), (void *)0, OS_TASK_OPT_STK_CHK+OS_TASK_OPT_STK_CLR);	
                    #else
                    OSTaskCreate(TaskTwoRiscVinMpegEncode, (void *)0, (void *)&StkTaskTwoRisc0IPCamera[TaskTwoRiscMpegEncode_STK_SIZE - 1], PRIOR_TASK_IPCAM);
                    #endif
        
                    #ifdef CHIP_CFG_STACK_CHECK
                    OSTaskCreateExt(TaskTwoRiscSendMpegH263Encode, (void *)0, (void *)&StkTaskTwoRiscSendMpegH263Encode[sizeof(StkTaskTwoRiscSendMpegH263Encode) / sizeof(StkTaskTwoRiscSendMpegH263Encode[0]) - 1], PRIOR_TASK_TWO_RISC_SEND_MPEG_H263_ENCODE,\
                            PRIOR_TASK_TWO_RISC_SEND_MPEG_H263_ENCODE, (void *)&StkTaskTwoRiscSendMpegH263Encode[0], sizeof(StkTaskTwoRiscSendMpegH263Encode) / sizeof(StkTaskTwoRiscSendMpegH263Encode[0]), (void *)0, OS_TASK_OPT_STK_CHK+OS_TASK_OPT_STK_CLR);	
                    #else
                    OSTaskCreate(TaskTwoRiscSendMpegH263Encode, (void *)0, (void *)&StkTaskTwoRiscSendMpegH263Encode[TaskTwoRiscSendMpegh263Encode_STK_SIZE - 1], PRIOR_TASK_TWO_RISC_SEND_MPEG_H263_ENCODE);
                    #endif
                    
                    break;
                    
                case TWO_RISC_SYSTEM_REOPEN_VIN_TASK:
                {
                    INT8U dret = 0;
                    DIS_VIN_VIDEO();
                    debug("before OSTaskDel(PRIOR_TASK_IPCAM).\n");
                    OSTaskDel(PRIOR_TASK_TWO_RISC_SEND_MPEG_H263_ENCODE);
                    dret = OSTaskDel(PRIOR_TASK_IPCAM);
                    debug("after OSTaskDel(PRIOR_TASK_IPCAM).\n");
                    if (dret != OS_NO_ERR )
                        debug("OSTaskDel(PRIOR_TASK_IPCAM) failed.\n");
                    else
                        debug("OSTaskDel(PRIOR_TASK_IPCAM) success.\n");
                    OSSemDel_w(gptSemMpegEncodeReady, OS_DEL_ALWAYS, &err);
                    OSSemDel_w(gptSemMp4EncodeOutBufReady, OS_DEL_ALWAYS, &err);
                    OSMboxDel_w(gptMboxVideoInFrame, OS_DEL_ALWAYS, &err);
                    OSMboxDel_w(gptMboxMpegEncodeOutFrame, OS_DEL_ALWAYS, &err);
                    gVideoStatus &= ~tVinEn;
#if 0
#ifdef PIP_ENABLE        
                    OSQDel (gptMqueueVideoInFrame, OS_DEL_ALWAYS, &err);
                    if(err != OS_NO_ERR)
                    printf("OSQDel failed, err = %d!", err);
#endif
#endif

                    #ifdef CHIP_CFG_STACK_CHECK
                    OSTaskCreateExt(TaskTwoRiscVinMpegEncode, (void *)0, (void *)&StkTaskTwoRisc0IPCamera[sizeof(StkTaskTwoRisc0IPCamera) / sizeof(StkTaskTwoRisc0IPCamera[0]) - 1], PRIOR_TASK_IPCAM,\
                            PRIOR_TASK_IPCAM, (void *)&StkTaskTwoRisc0IPCamera[0], sizeof(StkTaskTwoRisc0IPCamera) / sizeof(StkTaskTwoRisc0IPCamera[0]), (void *)0, OS_TASK_OPT_STK_CHK+OS_TASK_OPT_STK_CLR);	
                    #else
                    OSTaskCreate(TaskTwoRiscVinMpegEncode, (void *)0, (void *)&StkTaskTwoRisc0IPCamera[TaskTwoRiscMpegEncode_STK_SIZE - 1], PRIOR_TASK_IPCAM);
                    #endif
        
                    #ifdef CHIP_CFG_STACK_CHECK
                    OSTaskCreateExt(TaskTwoRiscSendMpegH263Encode, (void *)0, (void *)&StkTaskTwoRiscSendMpegH263Encode[sizeof(StkTaskTwoRiscSendMpegH263Encode) / sizeof(StkTaskTwoRiscSendMpegH263Encode[0]) - 1], PRIOR_TASK_TWO_RISC_SEND_MPEG_H263_ENCODE,\
                            PRIOR_TASK_TWO_RISC_SEND_MPEG_H263_ENCODE, (void *)&StkTaskTwoRiscSendMpegH263Encode[0], sizeof(StkTaskTwoRiscSendMpegH263Encode) / sizeof(StkTaskTwoRiscSendMpegH263Encode[0]), (void *)0, OS_TASK_OPT_STK_CHK+OS_TASK_OPT_STK_CLR);	
                    #else
                    OSTaskCreate(TaskTwoRiscSendMpegH263Encode, (void *)0, (void *)&StkTaskTwoRiscSendMpegH263Encode[TaskTwoRiscSendMpegh263Encode_STK_SIZE - 1], PRIOR_TASK_TWO_RISC_SEND_MPEG_H263_ENCODE);
                    #endif
                }
                    break;
                    
                case TWO_RISC_SYSTEM_DEL_VIN_TASK:
                    DIS_VIN_VIDEO();
                    INT8U dret = 0;
                    debug("before OSTaskDel(PRIOR_TASK_IPCAM).\n");
                    OSTaskDel(PRIOR_TASK_TWO_RISC_SEND_MPEG_H263_ENCODE);                    
                    dret = OSTaskDel(PRIOR_TASK_IPCAM);
                    debug("after OSTaskDel(PRIOR_TASK_IPCAM).\n");
                    if (dret != OS_NO_ERR )
                        debug("OSTaskDel(PRIOR_TASK_IPCAM) failed.\n");
                    else
                        debug("OSTaskDel(PRIOR_TASK_IPCAM) success.\n"); 
                    OSSemDel_w(gptSemMpegEncodeReady, OS_DEL_ALWAYS, &err);
                    OSSemDel_w(gptSemMp4EncodeOutBufReady, OS_DEL_ALWAYS, &err);
                    OSMboxDel_w(gptMboxVideoInFrame, OS_DEL_ALWAYS, &err);
                    OSMboxDel_w(gptMboxMpegEncodeOutFrame, OS_DEL_ALWAYS, &err);
                    gVideoStatus &= ~tVinEn;
                    break;

#endif /*VIDEO_MODULE_INCLUDE*/

#ifdef DVR
/*  wg add for DVR 2007.08.16   */
                case TWO_RISC_SYSTEM_DVR_READ_MOTION_DETECTION:

                    Hw2835ReadMDRegister();
#endif                   
                    break;

#ifdef VIDEO_MODULE_INCLUDE
            case TWO_RISC_SYSTEM_SET_CLOCK:                
                if(gFreqFromCrystal == FALSE) {
                    printf("\nTaskTwoRiscVinMpegEncode: camera clock is created from Vout!\n");
                    CctlVoutClkSet();
                } else {
                    printf("\nTaskTwoRiscVinMpegEncode: camera clock is created from external crystal!\n");
                    CctlVoutClkSet();
                }

                gCameraSelectDigital = -1;   //initialize it to avoid default FALSE
                pcurrent_camera = GetDigitCamera();
                if(pcurrent_camera) {
                    gCameraSelectDigital = TRUE;
                }else {
                    pcurrent_camera = GetAnalogCamera();
                    if(pcurrent_camera) {
                        gCameraSelectDigital = FALSE;
                        
                        if(pcurrent_camera != NULL)                 //has found digital camera or 7181
                            pcurrent_camera->camera_init();
                        else {                                                      //
                            printf("\nTaskTwoRiscVinMpegEncode: no analog cameras found!\n");
                        }
                        
                        if(pcurrent_camera->GetCamVideoStd) {   //is the chip support video standard detection?
                            if(pcurrent_camera->GetCamVideoStd(&vstd) == SUCCESSFUL) {       //get analog camera and video standard
                                if((gtVinFormat.camera_format != vstd)) {
                                    gtVinFormat.camera_format = vstd;
                                }
                            }    
                        }
                    }
                }
                break;
#endif
                
                default:
                    printf("\nTaskTwoRiscSystemMonitorRisc0: type(%d) error!\n", msg);
                    break;             	

            }   	
        }
        
#ifdef VIDEO_MODULE_INCLUDE
#ifdef DISPLAY_INCLUDE      
        if(gVoutEnable == 0)
            gVideoStatus &= ~tVoutEn;
#endif
        
        if(VideoStatusBak != gVideoStatus)
        {
            debug("\n>>>>>>>>>>>>>>>>>>\n");
            VideoStatusBak = gVideoStatus;
            //don't send gVideoStatus, it will be change in other task while you send!
            pInt = (unsigned int *)&risc_packet;
            risc_packet.start = RISC_PKT_START;
            risc_packet.main_type = tVideo;
            risc_packet.sub_type = tCheckVideoStatus;
            risc_packet.length = 0x8;
            risc_packet.status = 0;     
            *(pInt + 2) = VideoStatusBak;                    
            risc_packet.end = RISC_PKT_END;     
            if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL)
                RiscFlagSet();    // Set risc1 flag to interrupt risc1
            else 
                debug("\r\nSend Video Status error!\r\n");
        }
#endif /* VIDEO_MODULE_INCLUDE */
    }
}


