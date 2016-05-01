/*
***************************************************************************
*                  Copyright (c) 2007 vBridge Microsystem, Inc.  
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


#define   TWO_RISC_STATISTICS_GLOBALS

#include "../version.h"
#include "video_cfg.h"

#include "two_risc_statistics.h"
#include "chip_cfg.h"
#include "audio_cfg.h"
#include "./audio/test.h"

#include "Band_adjust.h"
#include "VIN_SET.h"

#include "os_cpu.h"
#include "os_cfg.h"
#include "uCOS_II.h"

#include "2risc_comm_protocol.h"
#include "two_risc_system.h"
#include "2risc_comm_protocol.h"
#include "timestamp.h"

#include "../application/video/alarm.h"

#ifdef VIDEO_MODULE_INCLUDE
#include "two_risc_video.h" 
#include "mpeg4_encoder.h"
#include "two_risc_display.h"
#include "risc.h"
#include "Camera.h"
//#include "ADV7181.h"
#include "ucos_task_priority.h"
extern unsigned int gVideoStatus;
extern pCamera pcurrent_camera;
#endif

#ifdef MOTION_DETECTION
#include "Motion_Detection.h"
#endif

#ifdef PRINT_MPEG_DECODE_PERFORMANCE
extern unsigned int gTDecodeSumMd2;
extern unsigned int gTDecodeCntMd2;
extern unsigned int gTDecodeSumMd4;
extern unsigned int gTDecodeCntMd4;
#endif

#ifdef PRINT_MPEG_ENCODE_PERFORMANCE
extern unsigned int gTEncodeISum;
extern unsigned int gTEncodeICnt;
extern unsigned int gTEncodePSum;
extern unsigned int gTEncodePCnt;
#endif

void TaskStatisticsPrintTaskStackInfo(int task_prior)
{
    unsigned char err;
    OS_STK_DATA task_stack_data;
    OS_TCB task_tcb_data;     

    if(OSTaskQuery(task_prior, &task_tcb_data) == OS_NO_ERR) {    
        printf("Task %d: \n", OSPrioCur);

        printf("\tOS_TCB: StkPtr=0x%x, StkBottom=0x%x, StkSize=%d, Stat=%d, Prior=%d!\n", \
                task_tcb_data.OSTCBStkPtr, task_tcb_data.OSTCBStkBottom, task_tcb_data.OSTCBStkSize, task_tcb_data.OSTCBStat, task_tcb_data.OSTCBPrio);
                        
        err = OSTaskStkChk(task_prior, &task_stack_data);
        if(err == OS_NO_ERR)
            printf("\tStack: %d free, %d(%d%%) used!\n", task_stack_data.OSFree, task_stack_data.OSUsed, task_stack_data.OSUsed * 100/(task_stack_data.OSFree + task_stack_data.OSUsed));
        else if(err == OS_TASK_OPT_ERR)
            printf("\tStack: cannot be checked!\n");
     } else {
        printf("\nTaskStatisticsPrintTaskStackInfo: task %d not created!\n", task_prior);
        return;            
     }

     task_tcb_data.OSTCBStkPtr = (unsigned int *)((int)task_tcb_data.OSTCBStkPtr & 0x1fffffff | 0x80000000);

     printf("StkPtr(0x%x) content:\n", task_tcb_data.OSTCBStkPtr);
      
     printf("\tra = 0x%x\n", *task_tcb_data.OSTCBStkPtr++);
     printf("\tfp = 0x%x\n", *task_tcb_data.OSTCBStkPtr++);
     printf("\tgp = 0x%x\n", *task_tcb_data.OSTCBStkPtr++);
     printf("\tt9 = 0x%x\n", *task_tcb_data.OSTCBStkPtr++);
     printf("\tt8 = 0x%x\n", *task_tcb_data.OSTCBStkPtr++);
     printf("\ts7 = 0x%x\n", *task_tcb_data.OSTCBStkPtr++);
     printf("\ts6 = 0x%x\n", *task_tcb_data.OSTCBStkPtr++);
     printf("\ts5 = 0x%x\n", *task_tcb_data.OSTCBStkPtr++);
     printf("\ts4 = 0x%x\n", *task_tcb_data.OSTCBStkPtr++);
     printf("\ts3 = 0x%x\n", *task_tcb_data.OSTCBStkPtr++);
     printf("\ts2 = 0x%x\n", *task_tcb_data.OSTCBStkPtr++);
     printf("\ts1 = 0x%x\n", *task_tcb_data.OSTCBStkPtr++);
     printf("\ts0 = 0x%x\n", *task_tcb_data.OSTCBStkPtr++);
     printf("\tt7 = 0x%x\n", *task_tcb_data.OSTCBStkPtr++);
     printf("\tt6 = 0x%x\n", *task_tcb_data.OSTCBStkPtr++);
     printf("\tt5 = 0x%x\n", *task_tcb_data.OSTCBStkPtr++);
     printf("\tt4 = 0x%x\n", *task_tcb_data.OSTCBStkPtr++);
     printf("\tt3 = 0x%x\n", *task_tcb_data.OSTCBStkPtr++);
     printf("\tt2 = 0x%x\n", *task_tcb_data.OSTCBStkPtr++);
     printf("\tt1 = 0x%x\n", *task_tcb_data.OSTCBStkPtr++);
     printf("\tt0 = 0x%x\n", *task_tcb_data.OSTCBStkPtr++);
     printf("\ta3 = 0x%x\n", *task_tcb_data.OSTCBStkPtr++);
     printf("\ta2 = 0x%x\n", *task_tcb_data.OSTCBStkPtr++);
     printf("\ta1 = 0x%x\n", *task_tcb_data.OSTCBStkPtr++);
     printf("\ta0 = 0x%x\n", *task_tcb_data.OSTCBStkPtr++);
     printf("\tv1 = 0x%x\n", *task_tcb_data.OSTCBStkPtr++);
     printf("\tv0 = 0x%x\n", *task_tcb_data.OSTCBStkPtr++);
     printf("\tat = 0x%x\n", *task_tcb_data.OSTCBStkPtr++);
     printf("\thi = 0x%x\n", *task_tcb_data.OSTCBStkPtr++);
     printf("\tlo = 0x%x\n", *task_tcb_data.OSTCBStkPtr++);
     printf("\tSR = 0x%x\n", *task_tcb_data.OSTCBStkPtr++);
     printf("\tPC = 0x%x\n", *task_tcb_data.OSTCBStkPtr++);
}

void TaskStatisticsPrintTaskInfo()
{
    int i;
    unsigned char err;
    OS_STK_DATA task_stack_data;
    OS_TCB task_tcb_data;        
    
    printc("Risc%d tasks information: \n", RiscRdID());

    for(i = 0; i < 64; i++) {
        if(OSTaskQuery(i, &task_tcb_data) == OS_NO_ERR) {    
            printc("\tTask %d: \n", i);

            printc("\t\tOS_TCB: StkPtr=0x%x, StkBottom=0x%x, StkSize=%d, Stat=%d, Prior=%d!\n", \
                    task_tcb_data.OSTCBStkPtr, task_tcb_data.OSTCBStkBottom, task_tcb_data.OSTCBStkSize, task_tcb_data.OSTCBStat, task_tcb_data.OSTCBPrio);
                        
            err = OSTaskStkChk(i, &task_stack_data);
            if(err == OS_NO_ERR)
                printc("\t\tStack: %d free, %d(%d%%) used!\n", task_stack_data.OSFree, task_stack_data.OSUsed, task_stack_data.OSUsed * 100/(task_stack_data.OSFree + task_stack_data.OSUsed));
            else if(err == OS_TASK_OPT_ERR)
                printc("\t\tStack: cannot be checked!\n");
         }
    }
}

/*
***************************************************************************
*                         TaskStatistics
*
* Description: This task will statistilize the frame rate, etc. 
*
* Arguments  : 
*
* Return     : 
*
* Note(s)    : 
***************************************************************************
*/
#ifdef TASK_STATISTIC_EN

unsigned int TimeInterval = 1;

void TaskStatistics(void *data)
{
    unsigned int vstd = 0;
    unsigned int i, j, n = 0;
    char avsyncmode[3] = {'A','S','N'};
    char flag_ch[2] = {'N', 'Y'};
    char *charray1[4] = {"MPEG4", "MJPEG", "MPEG_JPEG", "H263"};
    char *charray2[2] = {"MPEG", "H263"};
    char *chaudio[10] = {"slic/codec", "talkID", "codetype", "samplebits", "samplerate", "frametime", "a/ulaw/linear", "EC", "Silence", "jitbufEF"};
    char *mdtype[3] = {"framesize", "motionvectors", "temporaldif"};
    unsigned int vinStartFlag = 0;
    unsigned char err;
    OS_TCB tcb_data;
    OS_CPU_SR cpu_sr;
    volatile unsigned int rtc_time, rtc_day, rtc_hour, rtc_minute, rtc_second; 
    unsigned short int reg_val;

    Risc_Packet_t pkt;
    unsigned int *pInt;

#ifdef AUDIO_MODULE_INCLUDE    
#ifdef AUDIO_TEST_ENABLE
    test_init(INT_S, INT_MS, CTL);
    test_cmd_clear();
#endif
#endif

    while (1) {
        
#ifdef VIDEO_MODULE_INCLUDE

#ifdef BAND_WIDTH_CONTROL
        if((gVideoStatus & tVinEn) == 1){
            if(vinStartFlag == 1){
                Updatecbr();
            }else if(vinStartFlag == 0)
                vinStartFlag = 1;
        } else {
            vinStartFlag = 0;
        }
#endif

        /* compute the date & time */
        rtc_time = OSTimeGet();
        rtc_time /= OS_TICKS_PER_SEC;   // how many second?
        rtc_day = rtc_time / 86400;     // one day has 86400 seconds!
        rtc_hour = (rtc_time - rtc_day * 86400) / 3600;     // one hour has 3600 seconds!
        rtc_minute = (rtc_time - rtc_day * 86400 - rtc_hour * 3600) / 60;   // one minute has 60 seconds!
        rtc_second =  rtc_time - rtc_day * 86400 - rtc_hour * 3600 - rtc_minute * 60;

        if(gCntFrameFromUcosToLinuxOver > 300) {
            gCntFrameFromUcosToLinuxOver = 0;
            gCntVideoUcosToLinuxFullClearByUcos++;
            printf("\n\nTaskStatistics: VIDEO_UCOS_TO_LINUX_CNT is %d!\n\n", RiscRdCNT(VIDEO_UCOS_TO_LINUX_CNT));
            RiscClrCNT(VIDEO_UCOS_TO_LINUX_CNT);
        } else if(gCntFrameFromUcosToLinuxOver > 250) {
            gCntVideoUcosToLinuxFullBeforeClearByUcos++;
            printf("\n\nTaskStatistic$: VIDEO_UCOS_TO_LINUX_CNT is %d, RISC1_RX is %d!\n\n", RiscRdCNT(VIDEO_UCOS_TO_LINUX_CNT), ScRdCount(RISC1_RX));        
            RiscFlagSet();
        }


        if(!(rtc_second & 0x1)) {
            printf("\n\n%d:%d:%d:%d\n", rtc_day, rtc_hour, rtc_minute, rtc_second);            
            printc("<$%d(%d,%d),%d(%d,%d),%d,+(%d,%d),%d>\n", \
                gCntVideoUcosToLinuxFull, gCntVideoUcosToLinuxFullBeforeClearByUcos, gCntVideoUcosToLinuxFullClearByUcos, \
                gCntMpegH263TooBig, gCntMpegH263TooTooBig, gCntMpegH263TooBigRuinRecon, \
                gCntTaskTwoRiscSendMpegH263EncodeBusy, \
                gCntMpegBusy, gCntMpegEncodeH263TimeOut, \
                gCntScVideoUcosToLinuxFull);
            printc("{%d,%d}\n", gtStatistics.cnt_video_in, gtStatistics.cnt_video_in_even_odd_reverse);

            printc("[%d,%d", gtStatistics.cnt_video_linux_to_ucos,gtStatistics.cnt_decodetimeout);
            if(gChecksumEnabled) {
                printc(",%d,%d,%d]\n", gtStatistics.cnt_checksum_err, gtStatistics.cnt_framelength_err, gtStatistics.cnt_frame_number_err);
            } else {
                printc("]\n");
            }

            printc("(<V%d,%d><M%d,%d><%d,%d>%d,%d,%d,%d,%d,%d)\n", \
                            gtStatistics.cnt_vout_intr, gtStatistics.cnt_vin_intr, \
                            gtStatistics.cnt_mc_intr, gtStatistics.cnt_mc_intr_by_audio, \
                            gtStatistics.cnt_all_excpts, gtStatistics.cnt_all_excpts_not_caused_by_icache, \
                            gtStatistics.cnt_unknow_mips_intr, gtStatistics.cnt_excpt_tlb, gtStatistics.cnt_excpt_ade, gtStatistics.cnt_excpt_ri, gtStatistics.cnt_excpt_cpu, gtStatistics.cnt_excpt_others);

            if(gtStatistics.frameEncoded_ps > 0)
                gtStatistics.averFrameLen = gtStatistics.averFrameLen / gtStatistics.frameEncoded_ps;
            printc("general infomation: \n");
#ifdef DVR
            printf("\t Vinfr=%d, Voutfr=%d, VLtUfr=%d, ALtUfr=%d, flagISRrate=%d\n", gtStatistics.video_in_fame_rate, gtStatistics.vout_frame_ps, gtStatistics.frame_linux_to_ucos_ps, gtStatistics.audio_linux_to_ucos_ps, gtStatistics.flagISR_rate);
            printf("\t Vencoderate=%d, aveFrameLen=%d(B), Vdecoderate=%d\n",  gtStatistics.frameEncoded_ps, gtStatistics.averFrameLen, gtStatistics.frameDecoded_ps);
            HwSendVideoStatus();
#else
            printc("\t Vinfr=%d, Voutfr=%d, VLtUfr=%d, ALtUfr=%d, flagISRrate=%d\n", gtStatistics.video_in_fame_rate, gtStatistics.vout_frame_ps, gtStatistics.frame_linux_to_ucos_ps, gtStatistics.audio_linux_to_ucos_ps, gtStatistics.flagISR_rate);
            printc("\t Vencoderate=%d, aveFrameLen=%d(B), Vdecoderate=%d\n",  gtStatistics.frameEncoded_ps, gtStatistics.averFrameLen, gtStatistics.frameDecoded_ps);
#endif
        }
        
        gtStatistics.video_in_fame_rate = 0;
        gtStatistics.averFrameLen = 0;
        gtStatistics.frame_linux_to_ucos_ps = 0;
        gtStatistics.audio_linux_to_ucos_ps = 0;
        gtStatistics.flagISR_rate = 0;
        gtStatistics.vout_frame_ps = 0;
        gtStatistics.frameEncoded_ps = 0;
        gtStatistics.frameDecoded_ps = 0;
        gtStatistics.VideoFrameUcosToLinux_ps = 0;

        if(n == 10) {
            printc("version information: \n");
            printc("\t VbridgeRealtimeProject %s\n", version);

            TaskStatisticsPrintTaskInfo();
            
            printc("avsync information: \n");
#ifdef AV_SYNC
            i = gtavsync.syncmode&(~SYNCMASK);
            printf("\t AVMode ='%c', localTS=%d, AoutTS=%d,VoutTS=%d,AinTS=%d,VinTS=%d\n", avsyncmode[i-1], gtavsync.systemts, gtStatistics.audio_out_ts, gtStatistics.video_out_ts,gtStatistics.audio_in_ts,gtStatistics.video_in_ts);
            printc("\t AoutCnt=%d,VoutCnt=%d,AbufDepth=%d,VbufDepth=%d\n", gtStatistics.audio_out_cnt, gtStatistics.video_out_cnt, gtStatistics.audio_jitbuf_depth, gtStatistics.video_jitbuf_depth);
#else
            printc("\t AV_SYNC macro not opened\n");
#endif

            printc("MD information: \n");
#ifdef MOTION_DETECTION
            printc("\t mv_enable='%c', md_enable='%c', mdtype='%s',sensitivity=%d\n", flag_ch[flag_mv_enable], flag_ch[gIsMDEnabled], mdtype[gMdType], gMotionSensi);
            printc("\t motion detection framerate = %d\n", gtStatistics.md_frame_rate);
#else
            printc("\t MOTION DETECTION macro not opened\n");
#endif

            printc("CODEC information: \n");
            printc("\t encodemode=%s, decodemode=%s, vmode=%s\n", charray2[gEncodeMode], charray2[decode_flag], charray1[ VMode - 1]);

            printc("encoder Q information: \n");
            printc("\t currentQ=%d\n", mp4EncodeParams.quant);

            printc("CBR information: \n");
#ifdef BAND_WIDTH_CONTROL	
            printc("\t CBR dis/enable ='%c'\n", flag_ch[gtcbrinfo.cbr_enabled]);
            printc("\t bw =%d, videobw =%d, minQ=%d, maxQ=%d\n", gtcbrinfo.bw, gtcbrinfo.videobw, gtcbrinfo.min_qp, gtcbrinfo.max_qp);
#else
            printc("BAND_WIDTH_CONTROL macro not opened\n");
#endif

            printc("checksum infomation: \n");
            printc("\t checksum dis/enable='%c'\n", flag_ch[gChecksumEnabled]);
            if(gChecksumEnabled) {
                printc("\t checksumerr=%d, framelenerr=%d, framenumerr=%d\n", gtStatistics.cnt_checksum_err, gtStatistics.cnt_framelength_err, gtStatistics.cnt_frame_number_err);
            }

            printc("intr time information: \n");
            printc("\t time_intr=%d, ISRID=0x%x, time_critical=%d, time_critical_ps=%d\n", gtStatistics.time_in_intr, gtStatistics.ISR_ID, gtStatistics.time_in_critical, gtStatistics.time_in_critical_ps);
            printc("\t time_critical_pc =0x%x\n", gtStatistics.pc_critical);
            printc("audio information: \n");
            i = 0;
            while(1) {
                if(i > 3)
                    break;
                if(gtStatistics.audioChannel[i] == 0) {
                    i++;
                    continue;
                }
                printc("\t channelID ");
                for(j=0; j<10;j++)
                    printc("%s ", chaudio[j]);
                printc("\n");

                printc("\t channelID=%d: {", i);
                for(j=0; j<9; j++)
                    printc("%d, ", gtStatistics.audioInfo[i][j]);
                printc("0x%x}\n", gtStatistics.audioInfo[i][j]);
                i++;
            }
            
#ifdef PRINT_MPEG_DECODE_PERFORMANCE            
            OS_ENTER_CRITICAL();
            printc("The sum of md2 is %d!\n", gTDecodeSumMd2);
            printc("The occurs of md2 are %d!\n", gTDecodeCntMd2);
            if(gTDecodeCntMd2 != 0)
                printc("The average md2 time is %d!\n", gTDecodeSumMd2/gTDecodeCntMd2);  //calculate the average time
        
            printc("The sum of md4 is %d!\n", gTDecodeSumMd4);
            printc("The occurs of md4 are %d!\n", gTDecodeCntMd4);
            if(gTDecodeCntMd4 != 0)
                printc("The average md4 time is %d!\n", gTDecodeSumMd4/gTDecodeCntMd4);  //calculate the average time
            OS_EXIT_CRITICAL();
#endif           

#ifdef PRINT_MPEG_ENCODE_PERFORMANCE
            OS_ENTER_CRITICAL();
            printc("The sum of I frame me is %d!\n", gTEncodeISum);
            printc("The occurs of I frame me are %d!\n", gTEncodeICnt);
            if(gTEncodeICnt != 0)
                printc("The average I frame me time is %d!\n", gTEncodeISum/gTEncodeICnt);  //calculate the average time

            printc("The sum of P frame me is %d!\n", gTEncodePSum);
            printc("The occurs of P frame me are %d!\n", gTEncodePCnt);
            if(gTEncodePCnt != 0)
                printc("The average P frame me time is %d!\n", gTEncodePSum/gTEncodePCnt);  //calculate the average time
            OS_EXIT_CRITICAL();
#endif
        }
        n++;
        if(n > 10) n = 0;

        gtStatistics.time_in_critical_ps = 0;

        alarm_pulse_count = alarm_gpio_isr_conut>>1;
        if(alarm_pulse_count){ 
            printc("alarm_gpio_no=%d\n", alarm_gpio_isr_no);
            printc("alarm_pulse_count=%d\n",alarm_pulse_count);
            AlarmPulseCounterSendMess();
            alarm_gpio_isr_conut = 0;
            alarm_pulse_count = 0;
        }

 extern int gCameraSelectDigital;
        if(gCameraSelectDigital == FALSE) {     //analog camera
            if(pcurrent_camera->GetCamVideoStd) {   //is the chip support video standard detection?
                if(pcurrent_camera->GetCamVideoStd(&vstd) == SUCCESSFUL) {       //get analog camera and video standard
                    if((gtVinFormat.camera_format != vstd)) {
                        gtVinFormat.camera_format = vstd;
                        if(OSTaskQuery(PRIOR_TASK_IPCAM, &tcb_data) == OS_NO_ERR) {
                            err = OSQPost(gptQMsgTwoRiscSystemMonitor, (void *)TWO_RISC_SYSTEM_REOPEN_VIN_TASK);            
                            if(err != OS_NO_ERR)
                                debug("\r\nRISC0: OSMboxPost error! err = %d!\r\n", err);
                        }
                    }
                }    
            }
        }

#ifdef LIGHT_ALARM_EN
        if ((gCameraSelectDigital == TRUE) && (pcurrent_camera != NULL)) {
            if (pcurrent_camera->deviceID && (OV7720_ID == pcurrent_camera->deviceID)) {              //light alarm is just for OV7720 camera
                pInt = (unsigned int *)&pkt;
                pkt.start = RISC_PKT_START;
                pkt.main_type = tVideo;
                pkt.sub_type = tGetMotionAlarm;
                pkt.length = 4;
                *(pInt + 2) = 2;           //light alarm
                pkt.status = 0;	
                pkt.end = RISC_PKT_END;	
                
                IICReadByte(0x21, 0x2F, &reg_val, 0);         //0x21 is the IIC slave address of the camera OV7720
                *(pInt + 3) = reg_val;
                if (TwoRiscCommSendPacket((size_t *)&pkt, RPACKET_LEN)==SUCCESSFUL) {    //send analog camera type to linux
                    RiscFlagSet();    // Set risc1 flag to interrupt risc1     
                } else {
                    printf("TaskStatistics:TwoRiscCommSendPacket error \n");
                }
                if (reg_val < 0x20)
                    printf("TaskStatistics:LIGHT ALARM\n");
            }
        }
#endif

#ifdef MOTION_DETECTION            //used in changing framerate when motion/no-motion
        if(gMDPlayTime > 0)
            gMDPlayTime--;
        gVinSoftwareFrameRate = 0;
        if(gMDStartDelay > 0)
            gMDStartDelay--;
        gtStatistics.md_frame_rate = 0;
#endif

#endif

#ifdef AUDIO_MODULE_INCLUDE 
#ifdef AUDIO_TEST_ENABLE
        test_timer();
        test_message_output();
        test_print_cmd();
#endif
#endif

        /* Check the messege queue in case of no flag intr! */
        if(ScRdCount(RISC0_RX) >= RPACKET_LEN) {    // There is at least one messege to be read!
            err = OSQPost(gptQMsgTwoRiscSystemMonitor, (void *)TWO_RISC_SYSTEM_RECEIVE_LINUX_MSG);                            
            if(err != OS_NO_ERR) {           
                debug("\nTaskStatistics: OSQPost error! err = %d!\n", err);                                 
            } else {
                debug("\nTaskStatistics: OSQPost OK!\n");                                 
            }           
        }

        OSTimeDlyHMSM (0, 0, TimeInterval, 0);
    }
    
    
}

#endif


