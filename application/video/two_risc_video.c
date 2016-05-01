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



#define   TWO_RISC_VIDEO_GLOBALS

#include "video_cfg.h"

//#include "fpga.h"
#include "cctl.h"

#include "comm.h"

#include "os_cpu.h"
#include "os_cfg.h"
#include "uCOS_II.h"

#include "two_risc_comm.h"
#include "2risc_memory_map.h"
#include "2risc_comm_protocol.h"
#include "two_risc_video.h"

#include "m4v.h"
#include "mpeg4_encoder.h"
#include "mpeg4_intr.h"
#include "Mc.h"



#include "../bsp/bsp.h"

#include "Sc.h"

#include "../video/VIN_API.h"

#include "../video_data/saved_vin_data.h"
#include "VIN_SET.h"

#include "Band_adjust.h"
#include "Motion_Detection.h"
#include "checksum.h"

#include "timestamp.h"
#include "cache.h"
#include "Camera.h"
#include "analog_camera.h"

//#include "Adv7181.h"
#ifdef DISPLAY_INCLUDE
#include "../../mpeg/mpeg4_decoder.h"
#include "two_risc_display.h"
#include "timestamp.h"
#include "video_buffer.h"
#endif

pCamera pcurrent_camera = NULL;
unsigned int gCamAutodetectEnable = 1;

unsigned int *gpMpegH263UcosToLinuxShareBuf;
unsigned int isFirstVinFrame;
unsigned int gTimeStamp = 0;
volatile unsigned int gFrameNum = 0;

#ifdef PRINT_MPEG_ENCODE_PERFORMANCE
extern unsigned int gTEncodeISum;
extern unsigned int gTEncodeICnt;
extern unsigned int gTEncodePSum;
extern unsigned int gTEncodePCnt;
#endif

#ifdef DUMP_M4V
unsigned int dump_m4v_buf[512000 / sizeof(int)];
#endif

/*
***************************************************************************
*                         TaskTwoRisc0IPCamera
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
void TaskTwoRisc0IPCameraFake(void *data)
{
    int i , tmp, isFirstFrame, isFrameStart, frame_no = 0, *pM4v;
    int frame_count /* the word count of a frame*/, frame_start_addr;		
    unsigned int packet[RPACKET_LEN/4];	
    unsigned int len, *pInt, *pTmp;
	
    BspStartTicker(OS_TICKS_PER_SEC); 	    
    
    //rRISC_GPIO_DIR &= ~0x4000000; /* Set GPIO26 output*/
    //rRISC_GPIO_DIR &= ~0x2000000; /* Set GPIO25 output*/  
    
    //OSTaskCreate(TaskLed, (void *)0, (void *)&StkTaskLed[TASK_LED_STK_SIZE - 1], 11);       
        
    RiscClrCNT(VIDEO_UCOS_TO_LINUX_CNT);                    

    //debug("\r\nlx160 risc0 ucos ok!\r\n");
    
    printf("\r\n");
    printf("**********************************************************************\r\n");
    printf("*                 Risc%d IP Camera Test                         *\r\n", RiscRdID());    
    printf("**********************************************************************\r\n");     
    printf("\r\n");
    
    //video_tx_share_addr_wr = (unsigned int *)AUDIO_UCOS_TO_LINUX_B_ADDR;     
    
    //TwoRiscCommInit();     
    
    // please un-call this function in sigle-cpu mode
    //RiscSemPend(SEM_UCOS_AUDIO_READY);    // RISC1 will post the semaphore when it is ready to receive audio data.                         

    pM4v = m4v;
    isFirstFrame = 1;
    pTmp = (unsigned int *)VIDEO_UCOS_TO_LINUX_B_ADDR;

    frame_no = 0;
    
    while(1){    	    	        
        frame_count = 0;
        frame_start_addr = (int)pTmp;
        isFrameStart = 0;
        
        for(;;) {
        	
            tmp = *pM4v;            
               	    	        
            if(isFrameStart == 0) {
                if(tmp == 0xb6010000)    // 0xb6010000 is the flag of a frame start 
                    isFrameStart = 1;	                                                       
            } else {
                if(tmp == 0xb6010000) 
                    break;
            }
            
            frame_count++;  
            
            //debug("0x%x, ", tmp);
            *pTmp++ = tmp; 
            if((int)pTmp == VIDEO_UCOS_TO_LINUX_E_ADDR)
                pTmp = (unsigned int *)VIDEO_UCOS_TO_LINUX_B_ADDR;           	                                	                
                
                pM4v++;    
            if(((int)pM4v) == ((int)(m4v + SIZE_M4V))) {
                pM4v = m4v;                                                           	                
                break;
            }                              
        }         
                
        pInt = packet;
        
        *pInt = RISC_PKT_START | (tVideo<<8) | (tVideoTransferUcosToLinux<<16) | (0x8<<24);
        
        *(pInt + 2) = (unsigned int)frame_start_addr;    // address item
        *(pInt + 3) = (unsigned int)frame_count * 4;     // length item    	
    	
        *(pInt + 7) = 0xffffffff;    // end flag          
        
        // Send packet    
        if(TwoRiscCommSendPacket(packet, RPACKET_LEN) == SUCCESSFUL) {                		   
    	     printf("send %d!!\r\n", frame_no++);
            RiscIncCNT(VIDEO_UCOS_TO_LINUX_CNT, 1);    // increase count by 1
            RiscFlagSet();    // Set risc1 flag to interrupt risc1        
        } else
            debug("risc0: ?");           
        
        OSTimeDlyHMSM(0, 0, 0, 33);
    }
}

/*
***************************************************************************
*                         TaskTwoRisc1IPCamera
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
void TaskTwoRisc1IPCamera(void *data)
{
    unsigned int *pUcosToLinux, *pLinuxToUcos, *pTmp = (unsigned int *)((unsigned int)gMpeg4EncodeReconAndVideoShareBuffer | 0xa0000000);	
    unsigned int i, *msg;	
    Risc_Packet_t risc_packet;   
    unsigned char err;
    int frame_count = 0;	
    tVideoUcosToLinux *ptMsg;		
    unsigned int cpu_sr;	
    
    BspStartTicker(OS_TICKS_PER_SEC); 	      
    
    gptVideoUcosToLinuxMbox = OSMboxCreate((void*)0);
    
    if(gptVideoUcosToLinuxMbox != (OS_EVENT *)0)
        _PRINTF("OSMbox gptVideoUcosToLinuxMbox Create OK!\r\n");
    else
        _PRINTF("OSMbox gptVideoUcosToLinuxMbox Create error!\r\n");     

    RiscClrCNT(VIDEO_LINUX_TO_UCOS_CNT);

    printf("\n\nTaskTwoRisc1IPCamera: maximum cnt of video linux to ucos is %d!\n\n", \
        sizeof(gMpeg4EncodeReconAndVideoShareBuffer) / AssumedMaximumEncodeFrameSize);
    
    while(1){    	
    	
        ptMsg = OSMboxPend(gptVideoUcosToLinuxMbox, 0, &err); 

        if(err != OS_NO_ERR) {            
            debug("\n\nTaskTwoRisc1IPCamera: ucos pend msg error!\n\n");	
            continue;
        }	

        if(RiscRdCNT(VIDEO_LINUX_TO_UCOS_CNT) >= 8) {
            RiscDecCNT(VIDEO_UCOS_TO_LINUX_CNT, 1);
            printf("\n\nTaskTwoRisc1IPCamera: Cnt of linux to ucos video buffer is %d!\n\n", RiscRdCNT(VIDEO_LINUX_TO_UCOS_CNT));
            continue;
        }            

        if((unsigned int)pTmp + AssumedMaximumEncodeFrameSize > ((unsigned int)gMpeg4EncodeReconAndVideoShareBuffer | 0xa0000000) + \
                                                                                                    sizeof(gMpeg4EncodeReconAndVideoShareBuffer)) {         
            pTmp = (unsigned int *)((unsigned int)gMpeg4EncodeReconAndVideoShareBuffer | 0xa0000000);		
        }
        pLinuxToUcos = pTmp;
        pTmp = (unsigned int *)((unsigned int)pTmp + AssumedMaximumEncodeFrameSize);        

        msg = (unsigned int *)&risc_packet;        
        risc_packet.start = RISC_PKT_START;
        risc_packet.main_type = tVideo;
        risc_packet.sub_type = tVideoTransferLinuxToUcos;
        risc_packet.length = 0x8;
        risc_packet.status = 0;
        *(msg + 2) = (unsigned int)(pLinuxToUcos);    // address		
        *(msg + 3) = ptMsg->len;    // length
        *(msg + 4) = ptMsg->tstamp;
        risc_packet.end = RISC_PKT_END;  
		
        pUcosToLinux = (unsigned int *)ptMsg->addr;
        for(i = 0; i < ptMsg->len; i += 4) {        
            *pLinuxToUcos++ = *pUcosToLinux++;         
        }      

        if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {        	  
            RiscIncCNT(VIDEO_LINUX_TO_UCOS_CNT, 1);    // increase count by 1
            RiscFlagSet();

            if(RiscRdCNT(VIDEO_LINUX_TO_UCOS_CNT) >= 3) 
                printf("\n\nTaskTwoRisc1IPCamera: Cnt is %d!\n\n", RiscRdCNT(VIDEO_LINUX_TO_UCOS_CNT));            

        } else {
            printf("\n\nTaskTwoRisc1IPCamera: Linux To Ucos Video Full!\n\n");	
        }         
        RiscDecCNT(VIDEO_UCOS_TO_LINUX_CNT, 1);
    } 
}

/*
***************************************************************************
*                         TaskTwoRiscVinMpegEncode
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

unsigned int data_count_sec = 0;

unsigned int VMode = MPEG4MODE;
unsigned int gVinOpenTimes = 0;
void TaskTwoRiscVinMpegEncode(void *data)
{
    OS_CPU_SR cpu_sr;                    
    unsigned int isSeqFirstVinFrame;  
    unsigned int isSeqLastVinFrame;
    unsigned int isSeqCount;
    tYUVFrame *ptMsg1;
    unsigned int value;
    unsigned char err;
    unsigned int i;

extern unsigned gVideoStatus;
extern int gCameraSelectDigital;
   
    data = data;

#ifdef PRINT_MPEG_ENCODE_PERFORMANCE
    gTEncodeISum = 0;                //clear the sum and occurs of I frame and P frame 
    gTEncodePSum = 0;
    gTEncodeICnt = 0;
    gTEncodePCnt = 0;
#endif

    gFrameNum = 0;
    isFirstVinFrame = 1;

    BspStartTicker(OS_TICKS_PER_SEC);

    gpMpegH263UcosToLinuxShareBuf = (unsigned int *)VIDEO_UCOS_TO_LINUX_B_ADDR;

    gptSemMpegEncodeReady = OSSemCreate(1);
	
    gptMboxVideoInFrame = OSMboxCreate((void *)0);

#if 0
#ifdef PIP_ENABLE
    gptMqueueVideoInFrame = OSQCreate(arrayMsg, 1);    // if there are n buffers, the msg queu can just contain n-2 msgs to avoid read & write confict..
    //gptMqueueVideoInFrame = OSMboxCreate((void *)0);
#endif
#endif

    gptMboxMpegEncodeOutFrame = OSMboxCreate((void *)0);
	
    gptSemMp4EncodeOutBufReady	= OSSemCreate(1);

    RiscIntrInit();
    TwoRiscCommInit(); 

    RiscClrCNT(VIDEO_UCOS_TO_LINUX_CNT);
    RiscClrCNT(YUV_UCOS_TO_LINUX_CNT); //add by zjb 07 jan 8th

    switch (VMode) {
        
        case MPEG4MODE:
        case MPEG_JPEG_MODE:            
        case H263_MODE:
            //gMaxFrameSizeVideoUcosToLinux = MAX_MPEG_FRAME_SIZE;               
            gMaxFrameSizeVideoUcosToLinux = AssumedMaximumEncodeFrameSize;     
            gMaxCntIPCAMVideoShareBuf = VIDEO_MAX_DATABUF_SIZE / gMaxFrameSizeVideoUcosToLinux;
            gMaxCntVideoUcosToLinux = gMaxCntIPCAMVideoShareBuf \
                                                            - 1 /* TaskTwoRiscSendMpegH263Encode read will occupy 1 buffer! */ \
                                                            - 1 /* The size of gptMboxMpegEncodeOutFrame mail box is 1!. This can be larger if using mail queue! */ \
                                                            - 1 /* mpeg4 or H263 need 1 buffer to write! */;

            break;

        case MJPEGMODE:
            /* Seems that these are useless for MJPEGMODE. */
            gMaxFrameSizeVideoUcosToLinux = gImageWidth * gImageHeight * 3 /2;
            gMaxCntVideoUcosToLinux = VIDEO_MAX_DATABUF_SIZE / gMaxFrameSizeVideoUcosToLinux;

            break;

        default:
            break;
    }

    debug("\n\ngMaxCntIPCAMVideoShareBuf = %d, gMaxCntVideoUcosToLinux = %d, gMaxFrameSizeVideoUcosToLinux = %d!\n\n", gMaxCntIPCAMVideoShareBuf, gMaxCntVideoUcosToLinux, gMaxFrameSizeVideoUcosToLinux);
    if(gMaxCntVideoUcosToLinux <= 0) {
        OS_ENTER_CRITICAL();
        _PRINTF("\n\ngMaxCntVideoUcosToLinux should be at least 1! gMaxCntVideoUcosToLinux = %d!\n\n", gMaxCntVideoUcosToLinux);
        while(1);
    }
    
    /* initialize video in */

    rSC_INTR_S2M = 0x3800;    // clean vin streams interrupts
    rSC_IENA_S2M |= 0x3800;    // enable vin streams interrupts

    video_in_frame_count = 0;   // odd_even flag if vin interlaced
    gI_Frame_Occur_Period = gIntraFrameRate;  //added by wy
    /*AppVinInitialize();
    ScIntrEn();    // locate sc ISR, enable risc sc intr. 
    AppVinStart();
    OSTimeDlyHMSM(0, 0, 1, 0);*/

    if(gFreqFromCrystal == FALSE) {
        printf("\nTaskTwoRiscVinMpegEncode: camera clock is created from Vout!\n");
        CctlVoutClkSet();
    } else {
        printf("\nTaskTwoRiscVinMpegEncode: camera clock is created from external crystal!\n");
        CctlVoutClkSet();
    }

#ifdef DVR

    printf("\nTaskTwoRiscVinMpegEncode: DVR!\n");
    gtVinFormat.interlaced = TRUE;
    //TechwellAppMain();

#else
/*********************************************************/
    if (1 == gCamAutodetectEnable) {       
        while(1) {
            gCameraSelectDigital = -1;   //initialize it to avoid default FALSE
            pcurrent_camera = GetDigitCamera();
            if(pcurrent_camera) {
                gCameraSelectDigital = TRUE;
            } else {
                pcurrent_camera = GetAnalogCamera();
                if(pcurrent_camera) {
                    gCameraSelectDigital = FALSE;
                }
            }

            if(pcurrent_camera == 0) {
                OSTimeDlyHMSM(0,0,1,0);
                continue;
            }
            OS_ENTER_CRITICAL();
            if(gtCameraSetting.inited == 0) {    // if not inited yet
                printf("TaskTwoRiscVinMpegEncode: set camera default value by ucos!");
                gtCameraSetting.contrast = pcurrent_camera->contrast;
                //gtCameraSetting.effect = pcurrent_camera->effect;
                //gtCameraSetting.exposure = pcurrent_camera->exposure;
                gtCameraSetting.saturation = pcurrent_camera->saturation;
                gtCameraSetting.brightness = pcurrent_camera->brightness;
                gtCameraSetting.hue = pcurrent_camera->hue;
                gtCameraSetting.sharpness = pcurrent_camera->sharpness;        
                gtCameraSetting.lightmode = pcurrent_camera->lightmode;        
                gtCameraSetting.frequency = pcurrent_camera->frequency;        
                gtCameraSetting.inited = 1;
            }
            OS_EXIT_CRITICAL();
            
            printf("Init camera(digital or analog)!\n");
            if(pcurrent_camera != NULL)                 //has found digital camera or 7181
                pcurrent_camera->camera_init();
            else {                                                      //
                printf("\nTaskTwoRiscVinMpegEncode: no cameras found!\n");
            }
            
            if(gCameraSelectDigital == FALSE) {   //adv7181 or tvp5150a, so see if an analog camera has been connected to it
                if(pcurrent_camera->GetCamVideoStd) {    //an analog camera connected to adv7181 or tvp5150a
                    if(pcurrent_camera->GetCamVideoStd(&value) == SUCCESSFUL) {       
                        gtVinFormat.camera_format = value;
                        break;
                    }
                } else {
                    gtVinFormat.camera_format = NTSC;
                    break;
                }
            }else if(gCameraSelectDigital == TRUE) {      //has found a digital camera
                break;
            }
            
            gtCameraSetting.inited = 0;    //because has not find the right camera, so reset the flag.
            OSTimeDlyHMSM(0,0,1,0);
        }
    } else {
        printf("TaskTwoRiscVinMpegEncode:Disable camera autodetection\n");
    }
    
    if (pcurrent_camera != NULL) {
        gtVinFormat.interlaced = !(pcurrent_camera->frame_pic);
        if (gCameraSelectDigital == FALSE) {
            if(gtVinFormat.camera_format == PAL) {
                gtVinFormat.width = 704;
                gtVinFormat.height = 576;
            } else if(gtVinFormat.camera_format == NTSC) {
                gtVinFormat.width = 704;
                gtVinFormat.height = 480;
            }
            AnalogCameraRatioSet();
        }
    } else
        gtVinFormat.interlaced = TRUE;
    
/********************************************************/
#endif    

#ifdef DISPLAY_INCLUDE
    if(gtVinFormat.yuv_format != gtPipCurrent.format || \
        gtPipCurrent.width != gImageWidth || gtPipCurrent.height != gImageHeight)  {
        gtPipCurrent.format = gtVinFormat.yuv_format;
        gtPipCurrent.width = gImageWidth;
        gtPipCurrent.height = gImageHeight;
        if(gPipEnable && gVoutEnable) {
            gVoutReEnable = 1;
        }
    }

    if((gtVinFormat.interlaced == TRUE) && (gImage_Mode == MERGED_IMAGE)) {
        if(gtPipCurrent.interlaced != FALSE) {       
            gtPipCurrent.interlaced = FALSE;
            if(gPipEnable && gVoutEnable) {
                gVoutReEnable = 1;
            }            
        }
    } else {
        if(gtPipCurrent.interlaced != gtVinFormat.interlaced) {       
            gtPipCurrent.interlaced = gtVinFormat.interlaced;
            if(gPipEnable && gVoutEnable) {
                gVoutReEnable = 1;
            }            
        }
    }    
#endif

    mp4EncodeParams.quant = gVideoQuality;
#ifdef BAND_WIDTH_CONTROL
    cbr_init(gtcbrinfo.bw);
#endif

#ifdef MOTION_DETECTION
#ifndef DVR
    /* motion detection init */
    if((gIsMDEnabled == TRUE) && (gMdType == mdtemporalDif))
    {
        gMDVinFirstFrame = 0;
        MDTemporalDifInit(gMotionSensi);
    }
    gMDStartDelay = STARTTIME;
#endif
#endif    

    AppVinInitialize();
    ScIntrEn();    // locate sc ISR, enable risc sc intr. 
    skip_no = 0;
    AppVinStart();

#ifdef AV_SYNC
#ifdef VIDEO_MODULE_INCLUDE
#ifdef DISPLAY_INCLUDE
    if((gtavsync.syncmode&(~SYNCMASK)) == SELFSYNC) {
        AppAudioClkOpen();       //
        RiscClrVCNT();    
        RiscVCNTIntrEn(AUDIO_MAX_FRAME_LEN/* vcnt_val */);    // 80 bytes
    }
#endif
#endif
#endif

    gtMpegEncodeThrowFrame.frame_rate = gFrameRate;
    gtMpegEncodeThrowFrame.throw_frame_enable = 0;
    gtMpegEncodeThrowFrame.invalid_frame = 0;
    gVinFrameCount = 0;	

    for(i=0; i<MAX_VIN_BUF_NUM; i++)
        printf("VIN_Y_memory_%d=0x%x\n", i, gtVideoInFrame[i].y_addr);
			
    gVideoStatus |= tVinEn;
	
    while(1) {
        if(gI_Frame_Occur_Period != gIntraFrameRate) {
            gI_Frame_Occur_Period = gIntraFrameRate;
            isFirstVinFrame = 1;
        } 
  
        /* wait for video-in frame */
        ptMsg1 = OSMboxPend(gptMboxVideoInFrame, 0, &err);
        gTimeStamp = ptMsg1->timestamp;

        if(err == OS_NO_ERR) {
            
            if(isFirstVinFrame) {    // If it the first frame, the encode has not started yet.
                isSeqFirstVinFrame = 0;
                isSeqLastVinFrame = 0;
                isSeqCount = 1;

                // give the source yuv address!
                mp4EncodeParams.src_Y  = ptMsg1->y_addr;    // use physical address
                mp4EncodeParams.src_Cb = ptMsg1->u_addr;
                mp4EncodeParams.src_Cr = ptMsg1->v_addr;

                /* intialize mpeg4 encode */
                mpeg4IntrInit();   //  init mpeg4(encode, decode, & mv) intr, enable risc vcodec intr			
                mpeg4EncoderInit();    // reset encode & start encode with VOL
                        
#ifdef EACH_I_VOL                        
                if(gI_Frame_Occur_Period != 1) {
                    isSeqCount++;				
                    isFirstVinFrame = 0;
                } else {
                    isFirstVinFrame = 1;
                }
#else
                isSeqCount++;				
                isFirstVinFrame = 0;
#endif
                        
            } else {		 
                if (++gIPFrameCount == gI_Frame_Occur_Period) {
                    gIPFrameCount = 0;					
                    mp4EncodeParams.isPFrame =  0;    // I frame 
                } else {
     	             mp4EncodeParams.isPFrame = 1;    // P frame	     	
                }
        		      
                // re-give the source yuv address!
                mp4EncodeParams.src_Y  = ptMsg1->y_addr;               
                mp4EncodeParams.src_Cb = ptMsg1->u_addr;
                mp4EncodeParams.src_Cr = ptMsg1->v_addr;                

                mp4EncodeParams.out_bits = ((int)gpMpegH263UcosToLinuxShareBuf) & 0x1fffffff;    // use physical address

#ifdef EACH_I_VOL
                /* Every I frame will have a vol. */
                if(gI_Frame_Occur_Period == 1) {
                    isFirstVinFrame = 1;       
                    isSeqLastVinFrame = 1;
                } else {
                    if(isSeqCount == gI_Frame_Occur_Period) {
                        isSeqLastVinFrame = 1;                              
                    }    	
                }
#endif				
        				
                // update mpeg encode register and start next frame encode 
                mpeg4EncoderUpdate(&mp4EncodeParams);

                if(isSeqLastVinFrame) {
                    isSeqLastVinFrame = 0;
                    isSeqFirstVinFrame = 1;
                    isSeqCount = 0;
                    mpeg4EncoderLastVopStart();
                } else {
                    if(isSeqFirstVinFrame) {
                        isSeqFirstVinFrame = 0;						
                        mpeg4EncoderInit1();    // reset encode & start encode with VOL
                    } else {
                        mpeg4EncoderVopStart();
                    }
                }

                isSeqCount++;
#if 0
#ifdef MOTION_DETECTION
                if((gIsMDEnabled == TRUE) && (gMdType == mdtemporalDif))
                {
                    if(flag == 1) {
                        if((gImgEncodeMode == ENCODEDOWN) && (gImage_Mode == EVENODD_IMAGE) && (gtVinFormat.interlaced == TRUE)) 
                            HwPpeCompareY(md_y1_addr, ptMsg1->y_addr, gImageWidth, gImageHeight / 2);
                        else
                            HwPpeCompareY(md_y1_addr, ptMsg1->y_addr, gImageWidth, gImageHeight);
                        md_y1_addr = ptMsg1->y_addr;
                    }else {
                        md_y1_addr = ptMsg1->y_addr;
                        flag = 1;
                    }
                }
#endif
#endif

#ifdef PRINT_MPEG_ENCODE_PERFORMANCE
                mpeg_count = GetCount();
#endif			

            }       	
        } else {
            debug("\nTaskTwoRiscVinMpegEncode: OSMboxPend error!\n"); 
        }
    }
}	

void TaskTwoRiscSendMpegH263Encode(void)
{
    Risc_Packet_t VideoPacket;
    tMpegEncodeFrame *ptMsg2;
    unsigned int *pInt, *pShareBuf;
    unsigned int checksum = 0;
    unsigned int frame_no, frame_lastno;
    unsigned int timestamp;
    unsigned int i;
    unsigned char err;    
    OS_CPU_SR cpu_sr;

#ifdef DUMP_M4V
    unsigned int *pTmpDump = dump_m4v_buf;
    unsigned int i_dump_m4v = 0;
#endif

    while(1) {
        /* wait for mpeg4 encode frame*/
        ptMsg2 = OSMboxPend(gptMboxMpegEncodeOutFrame, 0, &err);
        
        frame_no = ptMsg2->framenum;
        timestamp = ptMsg2->timestamp;
        /* send mpeg encode results to linux */
        if(err == OS_NO_ERR) {      
            gtStatistics.frameEncoded_ps ++;
            gtStatistics.averFrameLen += ptMsg2->len * 4;

#ifdef BAND_WIDTH_CONTROL		
            if(gtcbrinfo.cbr_enabled)
                QAutoAdjust( ptMsg2->len*4, ptMsg2->pframe);
#endif		                                             	

#ifdef DUMP_M4V
            if(++i_dump_m4v > DUMP_M4V_FRAME_CNT || \
                (unsigned int)pTmpDump + ptMsg2->len * sizeof(int) > (unsigned int)dump_m4v_buf + sizeof(dump_m4v_buf)) {
                /* start dump */
                OS_ENTER_CRITICAL();
                _PRINTF("\n\nstart dump: %d frames, len = %d bytes\n\n", i_dump_m4v-1, (unsigned int)(pTmpDump - dump_m4v_buf) * sizeof(int)):
                
                for(i = 0; (unsigned int)(dump_m4v_buf + i) < (unsigned int)pTmpDump; i++)
                    _PRINTF("0x%x,", dump_m4v_buf[i]);

                _PRINTF("\n\nThe dump end!");
                while(1);
            } else {
                cache_writeback_invalidate_D((void *)(ptMsg2->addr & 0x1fffffff | 0x80000000), ptMsg2->len * sizeof(int));            
                memcpy((void *)pTmpDump, (void *)(ptMsg2->addr & 0x1fffffff | 0x80000000), ptMsg2->len * sizeof(int));
                pTmpDump += ptMsg2->len;
            }
#endif

            /* send mpeg encode results to share buffer */
            pInt = (unsigned int *)&VideoPacket;

            VideoPacket.start = RISC_PKT_START;
            VideoPacket.main_type = tVideo;
            VideoPacket.sub_type = tVideoTransferUcosToLinux;
            VideoPacket.length = 0x8;

            *(pInt + 2) = ptMsg2->addr;                         //address item

            if(gChecksumEnabled == TRUE) {
                *(pInt + 3) = (ptMsg2->len + 4) * sizeof (int);       //ptMsg2->len is counted by word, add Length, check sum, frame_No & timestamp
            }else{
                *(pInt + 3) = (ptMsg2->len) * sizeof(int);     // ptMsg2->len is counted by words
            }
            
            *((unsigned short *)(pInt + 4)) = 0;                      //0: Vin frame data, 1:Motion Vector
            *(pInt + 5) = timestamp;

            VideoPacket.end = RISC_PKT_END;

            if(gChecksumEnabled == TRUE) {         
                checksum = 0;

                /* use cached data */
                pInt = (unsigned int *)(ptMsg2->addr & 0x1fffffff | 0x80000000);
                cache_writeback_invalidate_D(pInt, ptMsg2->len * sizeof(int));
                //cache_writeback_invalidate_D(pInt, ptMsg2->len * sizeof(int));
                //cache_index_writeback_invalidate_D_all();

                /* calaulate the checksum! */
                for(i = 0; i < ptMsg2->len; i++) {
                    checksum += *pInt++;          
                }            

                //pInt = (unsigned int *)((int)pInt | 0xa0000000);
                *pInt = ptMsg2->len * sizeof(int);              
                *(pInt + 1) = checksum;
                if(frame_no != frame_lastno + 1) {
                    printc("ucos to linux frame num error!frame_lastno=%d, frame_no=%d\n", frame_lastno, frame_no);
                }
                *(pInt + 2)  = frame_no;
                frame_lastno = frame_no;
                *(pInt + 3) = timestamp;

                /* write back from cache to sdram */
                cache_writeback_D(ptMsg2->addr + ptMsg2->len * sizeof(int), 16);
            }            
        } else {
            debug("\n\nTaskTwoRiscSendMpegH263Encode: OSMboxPend error!\n\n");
            continue;
        }

        if(TwoRiscCommSendPacket((unsigned int *)&VideoPacket, RPACKET_LEN) == SUCCESSFUL) {  
            RiscIncCNT(VIDEO_UCOS_TO_LINUX_CNT, 1);                            // increase count by 1            
            RiscFlagSet();

#ifdef DEBUG_AVSYNC
            gtStatistics.video_out_ts = timestamp;
            gtStatistics.video_out_cnt = RiscRdCNT(VIDEO_UCOS_TO_LINUX_CNT);
#endif
            gtStatistics.VideoFrameUcosToLinux_ps++;            
                
        } else {
            gCntScVideoUcosToLinuxFull++;
            isFirstVinFrame = 1;                                                                // the mpeg encode should re-start if one frame is skipped                
            logprintf(FATAL_ERROR, MP4_ENCODE, "~");     
        }
    }
}

