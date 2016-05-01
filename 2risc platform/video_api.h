#ifndef __VIDEO_API_H__
#define __VIDEO_API_H__

#include  <def_type.h>


/*
*********************************************************************************************************
*              			Definition of Vin & Vout Mode(NTSC/ PAL)
*********************************************************************************************************
*/
#define NTSC	0
#define PAL 	1

#define YUV422			0
#define YUV420			1

#define EVEN_FRAME_FIRST    1
#define ODD_FRAME_FIRST    0

#define EVENODD_IMAGE 0
#define MERGED_IMAGE 1
/*
***************************************************************************
*                           Type Definitions
***************************************************************************
*/
typedef  struct {
    unsigned int y_addr;
    unsigned int u_addr;
    unsigned int v_addr;
    
    unsigned int width;
    unsigned int height;   

    unsigned int format;

    unsigned int pixel_ratio;
    unsigned int line_ratio;  

    unsigned int interlaced;

    unsigned int timestamp;   //add by wy
    unsigned int framenum;    //add by wy
} tYUVFrame;

typedef struct VIDEO_START_PARA
{
	LONG lwOperateMode;
	WORD swMethod;
	WORD swFormat;
	WORD swVinWidth;
	WORD swVinHeight;
	WORD swVinSkip;
	WORD swVinScaling;
	WORD swVoutWidth;
	WORD swVoutHeight;
	WORD swVoutSkip;
	WORD swVoutScaling;
	WORD swVscWidth;
	WORD swVscHeight;
	WORD swVscSkip;
	WORD swVscScaling;
	WORD swRvoWidth;
	WORD swRvoHeight;
	WORD swRvoSkip;
	WORD swRvoScaling;
	WORD swRvoXStart;
	WORD swRvoYStart;
	WORD swRvoXStop;
	WORD swRvoYStop;
	WORD swPipWidth;
	WORD swPipHeight;
	WORD swPipSkip;
	WORD swPipScaling;
	WORD swPipXStart;
	WORD swPipYStart;
	WORD swPipXStop;
	WORD swPipYStop;
}tVIDEO_START_PARA, *ptVIDEO_START_PARA;

typedef struct VIDEO_PARAMETERS
{
	LONG lwLuminance;

}tVIDEO_PARAMETERS, *ptVIDEO_PARAMETERS;



/*
func: update parameter of Video
In para: pConfig(VIDEO_PARA_B_ADDR)
return TRUE/FALSE
*/
BOOL AppVideoUpdatePara(VOID);
/*
func: get parameter of Video
In para: pConfig(VIDEO_PARA_B_ADDR)
return TRUE/FALSE
*/
BOOL AppVideoGetPara(VOID);

/*
func: init camera
return TRUE/FALSE
*/
BOOL AppVideoInitCamera(VOID);


/*
func:Video start
In para: pConfig(VIDEO_START_PARA_B_ADDR)
return: TRUE/FALSE
*/
BOOL AppVideoStart(VOID);

/*
func:Video Stop
In para: lwOperateMode (1-VIN,2-VOUT,4-VSC)
return: TRUE/FALSE
*/
BOOL AppVideoStop(LONG lwOperateMode);



/*
func:Sip receive data from Video 
In para: data's len  required 
return: >=0 : data's len received; -1: error
*/
LONG AppVideoReceiveData(PCHAR pcwData,LONG lwLen);

/*
func:Sip send data to Video
return:len
*/
LONG AppVideoSendData(PCHAR pcwData,LONG lwLen);

#ifdef BEFORE_Ver002
typedef enum tVideoSubType
{
	tVideoInitCamera	= 0x1,	
	tVideoStart		= 0x2, 
	tVideoStop			= 0x3,	
	tVideoTransferLinuxToUcos = 0x4,
	tVideoTransferUcosToLinux = 0x5,
	tVideoUpdatePara	= 0x6,
	tVideoGetPara		= 0x7,
	tVideoRelease		= 0x8,
}tVideoSubType;
#endif

typedef enum tVideoSubType
{
    tVideoEnableDisable	= 0x1,	
    tSetMotionDetectionEnabled		= 0x2, 
    tGetMotionDetectionEnabled			= 0x3,	
    tVideoTransferLinuxToUcos = 0x4,
    tVideoTransferUcosToLinux = 0x5,
    tSetVideoQuality	= 0x6,
    tGetVideoQuality		= 0x7,
    tSetVideoSize		= 0x8,
    tGetVideoSize	= 0x9,
    tSetMotionSensity = 0xa,
    tGetMotionSensity = 0xb,
    tSetMotionObjectSize = 0xc,
    tGetMotionObjectSize = 0xd,
    tSetFrameRate = 0xe,
    tGetFrameRate = 0xf,
    tSetIntraFrameRate = 0x10,
    tGetIntraFrameRate = 0x11,
    tSetVideoCodec = 0x12,
    tGetVideoCodec = 0x13,
    tSetScale = 0x14,
    tSetCamReg = 0x18,
    tGetCamReg = 0x19,
    tSetCBR = 0x1a,
    tGetCBR = 0x1b,
    tGetMotionAlarm = 0x1e,
    tCheckVideoStatus = 0x1f, 
    tSetNTSCPal = 0x21,
    tSetCamera = 0x22,	
    tSetCameraImageMirror = 0x23,
    tOSDClearScreen=0x24,
    tOSDFillBMP=0x25,
    tOSDDrawLine=0x29,
    tOSDFillColor=0x2a,
    tSetOSDColorPallete=0x2b,
    tChangeOSDPage=0x2c,
    tYUVTransferUcosToLinux = 0x2e,
    tSetChecksumEnableDisable = 0x2f,
    tSetClockSource = 0x2d,
    tSetCameraSelect = 0x28,
    tSetVoutRvoInterlaced = 0x26,    
    tOddEvenSelect = 0x27,
    tStillImageInfo = 0x30,
    tDisplayStiImg = 0x31,
    tSetSyncMode = 0x32,
    tSetEOImgMode = 0x33,
    tSetEOImgEncodeMode = 0x34,
    tOSDWriteString = 0x35,
    tSetThreshold = 0x36,
    tGetCameraType = 0x38,
    tOSDRollString = 0x3a,
    tYUVTransferLinuxToUcos = 0x3b,
    tRVODisplayMode = 0x3c,    
    tSetMDFrameRate              = 0x3d,
    tGetMotionStatus = 0x3e,
    tSetDecodeDeblocking = 0x3f,
    tSetCamAutodetect = 0x40
}tVideoSubType;

typedef enum tDVRSubtype
{
    tInit2835 = 0x1,
    tDrawLine = 0x2,
    tFillColor = 0x3,
    tFillBitmap = 0x4,
    tWriteString = 0x5,
    tSetOutputMode=0x06,
    tRemoteControl=0x7,
    tFillBMP64 = 0x8,
    tStatusLed=0x9,
    tBuzzer=0xa,
/*    tAlarm=0xb,
    tAlarmStatus=0xc,*/
    tClrScreen = 0x0d,
    tKeypad=0xe,
    tMotionDetector = 0x0f,
    tTransmitMotionState = 0x10,
    tPlayback = 0x11,
    tSetColourLUT = 0x12,
    tChangeOSDShowPage = 0x13,
    tAdjustCamera = 0x14,
    t2DBoxDisplay = 0x15,
    tSetAudioGain= 0x16,
    tSetAudioLive= 0x17,
    tSetAudioRecord= 0x18,
    tEnableVideoLossDetect = 0x19,
    tEnableVideoBlindDetect = 0x1a,
    tEnableVideoNightDetect = 0x1b,
    tGetVideoLossBlindNightStatus= 0x1c,
    tSetBGContrast= 0x1d,
}tDVRSubtype;

typedef enum tVideoStatus
{
    tVinEn = 0x1,
    tVoutEn = 0x2,
    tVscEn = 0x4,
    tPipEn = 0x8,
    tRvoEn = 0x10,
    tOsdEn = 0x20,
}tVideoStatus;

typedef enum tSysSubType
{
    tGetCameraID=0x7,
    tAlarm=0x8,
    tAlarmStatus=0x9,
    tSetClock = 0xa,

}tSysSubType;




#endif
