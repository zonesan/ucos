/* file : IP_CAM_API.c
** author : hjm
** date : 2006-9-20
*/


#include "video_cfg.h"
#include "VIN_API.h"
#include "includes.h"
#include "IP_CAM_API.h"
#include "mpeg4_encoder.h"
#include "two_risc_video.h"
#include "VIN_SET.h"
#include "../../mpeg/mpeg4_decoder.h"
#include "two_risc_display.h"
#include "../two_risc_system.h"
#include "iic.h"
extern int  gFrameRate;
extern unsigned char camera_sensor_init[];

/*********************************************************
* SetNetworkBandwidth
* Params:	band_width - Value of Network bandwidth in kilo bits per
*						second(kpbs).
* Return:		1 	- 	Bandwidth modified.
*			-1	-	Bandwidth could not be modified. Bandwidth 
*					needs to be at least 128 kbps if streaming of
*					both Audio and Video is enabled.
* Notes:		Used to set the network bandwidth. The bit stream speed
*			of video is dependent on this value.
**********************************************************/
int SetNetworkBandwidth(int band_width)
{

}

/**********************************************************
* GetNetworkBandwidth
* Params:	none
* Return:		Value of the network bandwidth configured
* Notes:		Used to get the current value of network bandwidth
***********************************************************/
int GetNetworkBandwidth()
{

}

/**********************************************************
* EnableDHCP
* Params:	None
* Return:		None
* Notes:		- Used to enable DHCP client to acquire IP address 
*				during IP CAM startup.
*			- If during the startup if DHCP client on IP CAM board 
*				fails to get IP address from a DHCP server even 
*				after a few iterations, then the system starts up 
*				with pre-configured IP address configured during
*				application build time.
**********************************************************/
void EnableDHCP()
{

}

/*********************************************************
* DisableDHCP
* Params: 	None
* Return:		None
* Notes:		- Used to disable DHCP clients.
*			- Both the functions should be called before creating 
*				application tasks.
**********************************************************/
void DisableDHCP()
{

}

/**********************************************************
* SetVideoQuality
* Params:	quality - Video quality level. The valid values are:
*					HIGH_VIDEO_QUALITY	2
*					MEDIUM_VIDEO_QUALITY 1
*					LOW_VIDEO_QUALITY	0
* Return:		None
* Notes:		Used to set the Video quality of the video stream. Three
*			different quality levels are supported; High, Medium and
*			Low.
**********************************************************/
void SetVideoQuality(int quality)
{
    if(quality == 24){
        printc("Quality can't be 24!");
        return;
    }
    if(quality < 5)
        quality = 5;
    gVideoQuality = quality;
    mp4EncodeParams.quant = quality;
    debug("UCOS set quant == %d\n", mp4EncodeParams.quant);
}

/**********************************************************
* GetVideoQuality
* Params:	None
* Return:		Current value of video quality (High, Medium or Low).
* Notes:		Used to get the current video quality.
**********************************************************/
int GetVideoQuality()
{

	return gVideoQuality;
	
}

/*********************************************************
* SetVideoStandard
* Params:	standard - Video standard to be set for IP CAM video 
*						stream supported image standards:
*						NTSC 	0
*						PAL		1
* Return:		None
* Notes:		Used to set the video standard. The supported video 
*				standards are NTSC and PAL
**********************************************************/
void SetVideoStandard(int standard)
{
	if (standard != 0 && standard != 1) {
		printf("Video Standard: %d not supported. Please specify 0 for NTSC, "
				"and 1 for PAL.\n", standard);
		return;
	}
	else {
		// TODO:	Unfinished
		printf("Set Video Standard successfully done. You may need to "
				"reinitialize hardware.\n");
	}
}

/*********************************************************
* GetVideoStandard
* Params:	None
* Return:		Video standard of the current video stream.
*				Supported video standards:
*				NTSC	0
*				PAL		1
* Notes:		Used to get the current video standard.
**********************************************************/
int GetVideoStandard()
{
	// TODO: 	Unfinished
	return 0;
}

int SetVideoWH(uint16_t width, uint16_t height)
{
    if(gImageWidth == width && gImageHeight == height)  // the new size is the same as the old one?
        return 1;

	if (SetCameraImgSize(width, height))    // the new size is not valid?
		return -1;

       /* this is done in GetImageMode(SetCameraImgSize) */ 
	//gImageWidth = width;
	//gImageHeight = height;
	
	/* Do not need change the mpeg4 encoder size because the ipcam task will restart! */
	//mp4EncodeParams.image_width = width;
	//mp4EncodeParams.image_height = height;

#ifdef PIP_ENABLE
        if(gPipEnable && gVoutEnable) {
            debug("\r\nPip size changed. Vout re-open!\r\n");
            gVoutReEnable = 1;
        }
#endif

	return 0;
}

uint16_t GetVideoWidth()
{
	return gImageWidth;
}

uint16_t GetVideoHeight()
{
	return gImageHeight;
}

/**********************************************************
* SetVideoFormat
* Params:	format - Format of the video to be set. Valid video 
*					formats: 
*						QCIF	0
*						CIF		1
*						QVGA	2
*						VGA		3
* Return:		1 - Setting video format successful.
*			-1 - Failed to set video format. Video format cannot
*				be changed if active RTSP clients are present.
* Notes:		Used to set Video format to be used for video streaming
**********************************************************/
/*
int SetVideoFormat(int format)
{
	ImageMode imgMode;
	
	switch (format) {
		case 0:
			imgMode = QCIF;
			break;
		case 1:
			imgMode = CIF;
			break;
		case 2:
			imgMode = QVGA;
			break;
		case 3:
			imgMode = VGA;
			break;
		default:
			printf("SetVideoFormat() format %d unsupported.\n", format);
			return -1;
	}

	// Statically change configure
	const size_t* width = GetWidthArray();
	const size_t* height = GetHeightArray();
	if (SetCameraImgMode(imgMode))
		return -1;
	SetVinImgWH(width[imgMode], height[imgMode]);
	mpeg4EncoderSetImageWH(width[imgMode], height[imgMode]);

	// Dynamically change configure
	
	return 1;		
}
*/

/*****************************************************************************
* GetVideoFormat
* Params: 	None
* Return:		Current value of video format
* Notes:		- Used to get the current video format.
*			- The selection of CIF/QCIF/QVGA/VGA can be done only in the
*				beginning or if it is required to change the current 
*				video format during run time it is required to close 
*				all the clients.
*****************************************************************************/
/*
int GetVideoFormat()
{
	size_t m4eImgWidth = mpeg4EncoderGetImageWidth();
	size_t vinImgWidth = GetVinWidth();
	const size_t* width = GetWidthArray();
	
	ASSERT(m4eImgWidth == vinImgWidth);
	

	if (vinImgWidth == width[QCIF]) 
		return 0;
	else if (vinImgWidth == width[CIF])
		return 1;
	else if (vinImgWidth == width[QVGA])
		return 2;
	else if (vinImgWidth == width[VGA])
		return 3;
	else {
		printf("In GetVideoFormat() image width : %d"
					" is unexpected. Program may run into error.\n", 
					vinImgWidth);
		return -1;
	}		

}
*/

/*********************************************************
* GetFrameRate
* Params:	None
* Return:		Frame rate that has been configured currently.
* Notes:		Used to get frame rate.
*********************************************************/
int GetFrameRate()
{
/*
	int value = readMpeg4EncoderReg(eMPEG4_ENCODER_INCREMENT_RESOLUTION);
	ASSERT(value == mp4EncodeParams.frame_rate);
	return mp4EncodeParams.frame_rate;
*/
	return gFrameRate;
}

/********************************************************
* SetIntraFrameRate
* Params:	iframe_rate - Intra frame refresh rate.
* Return:		1 - Setting intra frame rate successful.
*			-1 - Failed to set intra frame rate. Check if any RTSP 
*				clients are present. Changing intra frame rate not
*				permitted when RTSP clients are present.
* Notes:		Used to set intra refresh rate.
*********************************************************/
int SetIntraFrameRate(int iframe_rate)
{

	gIntraFrameRate = iframe_rate;

//	gI_Frame_Occur_Period = iframe_rate;
	
	return 1;
}

/*********************************************************
* GetIntraFrameRate
* Params: 	None
* Return:		- Return the value of the current intra frame rate that 
*				has been configured.
* Notes:		Used to get intra frame rate.
*********************************************************/
int GetIntraFrameRate()
{
	return gIntraFrameRate;
//return gI_Frame_Occur_Period;
}

/*********************************************************
* SetMotionSensitivity
* Params:	sense - integer value of sensitivity. Its value can range
*					from 0 - 100.
* Return:		None
* Notes:		Used to set motion sensitivity.
**********************************************************/
void SetMotionSensitivity(int sense)
{

}

/*********************************************************
* GetMotionSensitivity
* Params:	sense - integer value of motion sensitivity(0 - 100).
* Return:		None
* Notes:		Used to retrieve motion sensitivity.
**********************************************************/
void GetMotionSensitivity(int* sense)
{

}

/**********************************************************
* SetMotObjSize
* Params:	size - integer value of motion object size. (0 - 100)
* Return:		None
* Notes:		Used to set motion object size.
**********************************************************/
void SetMotObjSize(int size)
{

}

/*********************************************************
* GetMotObjSize
* Params:	size - a pointer to the integer value of motion object size.
* Return: 	None
* Notes:		Used to retrieve motion object size.
*********************************************************/
void GetMotObjSize(int* size)
{

}

void EnableMotionDetect()
{     int value;
	//value = (rM4EN_MV_INTR_MASK<<8) | (rM4EN_FIX_REC_BUF<<7) | (rM4EN_BIG_ENDIAN_VLC<<6) |
          //  (rM4EN_EXT_STALL_ENABLE<<5) | (rM4EN_FIELD_SRC<<4) | (rM4EN_ENC_FINISH_MASK<<3) |
           // (rM4EN_VOP_FINISH_MASK<<2) | (rM4EN_BIG_ENDIAN_SRC<<1) | rM4EN_CLK_INV_IF;
        value=readMpeg4EncoderReg(eMPEG4_ENCODER_SYSTEM_CONTROL);
        value=value&~(1<<8 );
	writeMpeg4EncoderReg(eMPEG4_ENCODER_SYSTEM_CONTROL, value);//Dec 27
	
}

/*********************************************************
* DisableMotionDetect
* Params:	None
* Return:		None
* Notes:		Used to disable motion detection
*********************************************************/
void DisableMotionDetect()
{	int value;
	//value = (rM4EN_MV_INTR_MASK<<8) | (rM4EN_FIX_REC_BUF<<7) | (rM4EN_BIG_ENDIAN_VLC<<6) |
           // (rM4EN_EXT_STALL_ENABLE<<5) | (rM4EN_FIELD_SRC<<4) | (rM4EN_ENC_FINISH_MASK<<3) |
           // (rM4EN_VOP_FINISH_MASK<<2) | (rM4EN_BIG_ENDIAN_SRC<<1) | rM4EN_CLK_INV_IF;
    value = readMpeg4EncoderReg(eMPEG4_ENCODER_SYSTEM_CONTROL);       
    value= value | 1<<8;
    writeMpeg4EncoderReg(eMPEG4_ENCODER_SYSTEM_CONTROL, value);

}

/*********************************************************
* IsMotionDetectEnable
* Params:
* Return: 	1 - Motion Detection Enabled.
*			0 - Motion Detection Not enabled.
* Notes:		Used to check whether motion detection is enabled.
*********************************************************/
int IsMotionDetectEnable()
{

}

/********************************************************
* EnableAudio
* Params:	enable :
*					1 - Enable Audio
*					0 - Disable Audio
* Return:		1 - Enabling / Disabling Audio was successful
*			-1 - Failed to enable / disable audio. Check if any RTSP
*				clients are present. Disabling / Enabling audio is
*				not permitted when RTSP clients are present.
* Notes:		Used to enable or disable audio.
*********************************************************/
int EnableAudio(BOOL enable)
{

/*
	if (enable) 
		EN_AUDIO();
	else 
		DIS_AUDIO();
*/
	return -1;
}

/********************************************************
* IsAudioEnabled
* Params:	None
* Return:		1 - Audio Enabled.
*			0 - Audio Disabled.
* Notes:		Used to check if audio is enabled or disabled.
*********************************************************/
int IsAudioEnabled(void)
{
	//return (rAUD_CTL & BIT_ENA) ? 1 : 0;
	return -1;
}


/*********************************************************
* EnableVideo
* Params:	Enable : 
*				1 - Enable video
*				0 - Disable video
* Return:		1 - 	Enabling / Disabling Video was successful
*			-1 -	Failed to enable / disable video. Check if any RTSP
*				clients are present. Disabling / Enabling video is not
*				permitted when RTSP clients are present.
* Notes:		Used to enable or disable video.
**********************************************************/

int EnableVideo(size_t enable)
{
/*

	if (enable) {
		RiscIntrEn(SC_INTR);
		// Reset global variaries
		//VinMpegEncodeTaskInit();
		
		HwCameraInitialize();
		AppVinInitialize();
		AppVinStart();
	}
	else {
		RiscIntrDis(SC_INTR);
		DIS_VIN_VIDEO();
	}

	*/
	return 1;
}


/**********************************************************
* IsVideoEnabled
* Params:	None
* Return:		1 - video enabled.
*			0 - video disabled.
* Notes:		Used to check if video is enabled or disabled.
**********************************************************/
int IsVideoEnabled(void)
{
	return IsVinEnabled();
}

/********************************************************
* SetMuticastInfo
* Params:	char* mCastAddr 	-	Muticast address.
*			unsigned int vPort	- 	Video Port number.
*			unsigned int aPort	-	Audio port number.
*			unsigend int ttl	-	Time to live.
* Return:		None
* Notes:		This function is used to configure the details for the
*				multicast streaming.
********************************************************/
void SetMulticastInfo(char* mCastAddr, unsigned int vPort,
					unsigned int aPort, unsigned int ttl)
{

}

/*********************************************************
* EnableMulticast
* Params:	boolean_t 	enable
*			boolean_t is defined as follows : 
*			typedef enum { B_FALSE, B_TRUE } boolean_t.
* Return:		None
* Notes:		This function is used to set the multicast enable flag.
*********************************************************/
void EnableMulticast(boolean_t enable)
{

}

/********************************************************
* IsMulticastEnable
* Params:	None
* Return:		0 -	If multicast is disabled.
*			1 - 	If multicast is enalbed.
* Notes:		This function returns whether multicast is enabled or not.
*********************************************************/
int IsMulticastEnable()
{

}

/*********************************************************
* GetAudioCodec
* Params:	None
* Return:		1 - 	G711A codec.
*			2 - 	G711A codec.
*			3 - 	G726 codec.
* Notes:		- Used to check which Audio Codec (G711 or G726) is 
*				currently used.
*			- Currently only G711 is supported.
**********************************************************/
int GetAudioCodec()
{

}

/**********************************************************
* SetAudioCodec
* Params:	a_mode : 
*				1 - G711A
*				2 - G711U
*				3 - G726
* Return:		1 - 	Audio codec successfully updated.
*			-1 - Audio codec selection not supported. Currently
*				only G711 is supported.
* Notes:		Used to select Audio Codec (G711 or G726).
**********************************************************/
int SetAudioCodec(int a_mode)
{

}

/*********************************************************
* SetVideoCodec
* Params:	v_mode :
*				1 - 	MPEG4
*				2 -	M-JPEG
* Return:		1 - 	Video codec succesfully updated.
*			-1 -	Video codec selection not supported. Currently
*				only MPEG4 is supported.
* Notes:		Used to select video codec (MPEG4 or M-JPEG)
*********************************************************/
int SetVideoCodec(int v_mode)
{
	if (v_mode != 1) {
		printf("M-JPEG is not supported now.\n");
		return -1;
	}
	else
		return 1;
}

/********************************************************
* GetVideoCodec
* Params:	None
* Return:		1 - 	MPEG4
*			2 - 	M-JPEG
* Notes:		- Used to check which video codec (MPEG4 or M-JPEG) 
*				is currently used.
*			- Currently only MPEG4 is supported.
*********************************************************/
int GetVideoCodec(void)
{
	return 1;
}



// ---------------------------------Unit Test---------------------------------

void TestIPCamConfigAPI()
{
//	mpeg4EncoderSetup();
	
	// Test change statically
	SetVideoQuality(2);
	SetVideoWH(640, 480); // VGA
	//SetFrameRate(30);
	SetIntraFrameRate(15);
	EnableAudio(0);
	EnableVideo(1);  		// Start Vin and M4E
	SetVideoCodec(1);


	int quality = GetVideoQuality();
	ASSERT(quality == 2);
	uint16_t width = GetVideoWidth();
	ASSERT(width == 640);
	uint16_t height = GetVideoHeight();
	ASSERT(height == 480);
	int frameRate = GetFrameRate();
	ASSERT(frameRate == 30);
	int intraFrameRate = GetIntraFrameRate();
	ASSERT(intraFrameRate == 15);
	ASSERT(!IsAudioEnabled());
	ASSERT(IsVideoEnabled());
	ASSERT(GetVideoCodec() == 1);
	
	// Test change dynamically
	SetVideoQuality(1);
	SetVideoWH(320, 240);
	//SetFrameRate(15);
	SetIntraFrameRate(5);
	
	quality = GetVideoQuality();
	ASSERT(quality == 1);
	width = GetVideoWidth();
	ASSERT(width = 320);
	height = GetVideoHeight();
	ASSERT(height == 240);
	frameRate = GetFrameRate();
	ASSERT(frameRate == 15);
	intraFrameRate = GetIntraFrameRate();
	ASSERT(intraFrameRate == 5);
	
	EnableVideo(0);
	SetVideoQuality(2);
	SetVideoWH(640, 480); // VGA
	//SetFrameRate(30);
	SetIntraFrameRate(15);
	EnableAudio(0);
	EnableVideo(1);
	
	quality = GetVideoQuality();
	ASSERT(quality == 2);
	width = GetVideoWidth();
	ASSERT(width == 640);
	height = GetVideoHeight();
	ASSERT(height == 480);
	frameRate = GetFrameRate();
	ASSERT(frameRate == 30);
	intraFrameRate = GetIntraFrameRate();
	ASSERT(intraFrameRate == 15);
	ASSERT(!IsAudioEnabled());
	ASSERT(IsVideoEnabled());

}


