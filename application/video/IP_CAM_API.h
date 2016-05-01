/* file : IP_CAM_API.h
** author : hjm
** date : 2006-9-20
** discription : 	IP Camera application
*/

#ifndef IP_CAM_API_H
#define IP_CAM_API_H

#include "sun_type_defs.h"

//#define HIGH_VIDEO_QUALITY	2
//#define MEDIUM_VIDEO_QUALITY 1
//#define LOW_VIDEO_QUALITY	0

#define LOW_QUANT 5
#define MEDIUM_QUANT 15
#define HIGH_QUANT 31

#define Mode_Vin 0x1
#define Mode_Vout 0x2
#define Mode_Vsc 0x4
#define Mode_Pip 0x8
#define Mode_Rvo 0x10
#define Mode_Osd 0x20

// Declarations of API Functions
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
int SetNetworkBandwidth(int band_width);

/**********************************************************
* GetNetworkBandwidth
* Params:	none
* Return:		Value of the network bandwidth configured
* Notes:		Used to get the current value of network bandwidth
***********************************************************/
int GetNetworkBandwidth();

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
void SetVideoQuality(int quality);

/**********************************************************
* GetVideoQuality
* Params:	None
* Return:		Current value of video quality (High, Medium or Low).
* Notes:		Used to get the current video quality.
**********************************************************/
int GetVideoQuality();

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
int SetVideoFormat(int format);

/**********************************************************
* GetVideoFormat
* Params: 	None
* Return:		Current value of video format
* Notes:		- Used to get the current video format.
*			- The selection of CIF/QCIF/QVGA/VGA can be done only in the
*				beginning or if it is required to change the current 
*				video format during run time it is required to close 
*				all the clients.
**********************************************************/
int GetVideoFormat();

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
void EnableDHCP();

/*********************************************************
* DisableDHCP
* Params: 	None
* Return:		None
* Notes:		- Used to disable DHCP clients.
*			- Both the functions should be called before creating 
*				application tasks.
**********************************************************/
void DisableDHCP();

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
void SetVideoStandard(int standard);

/*********************************************************
* GetVideoStandard
* Params:	None
* Return:		Video standard of the current video stream.
*				Supported video standards:
*				NTSC	0
*				PAL		1
* Notes:		Used to get the current video standard.
**********************************************************/
int GetVideoStandard();

/*********************************************************
* SetMotionSensitivity
* Params:	sense - integer value of sensitivity. Its value can range
*					from 0 - 100.
* Return:		None
* Notes:		Used to set motion sensitivity.
**********************************************************/
void SetMotionSensitivity(int sense);

/*********************************************************
* GetMotionSensitivity
* Params:	sense - integer value of motion sensitivity(0 - 100).
* Return:		None
* Notes:		Used to retrieve motion sensitivity.
**********************************************************/
void GetMotionSensitivity(int* sense);

/**********************************************************
* SetMotObjSize
* Params:	size - integer value of motion object size. (0 - 100)
* Return:		None
* Notes:		Used to set motion object size.
**********************************************************/
void SetMotObjSize(int size);

/*********************************************************
* GetMotObjSize
* Params:	size - a pointer to the integer value of motion object size.
* Return: 	None
* Notes:		Used to retrieve motion object size.
*********************************************************/
void GetMotObjSize(int* size);

void EnableMotionDetect();
/*********************************************************
* DisableMotionDetect
* Params:	None
* Return:		None
* Notes:		Used to disable motion detection
*********************************************************/
void DisableMotionDetect();

/*********************************************************
* IsMotionDetectEnable
* Params:
* Return: 	1 - Motion Detection Enabled.
*			0 - Motion Detection Not enabled.
* Notes:		Used to check whether motion detection is enabled.
*********************************************************/
int IsMotionDetectEnable();

/*********************************************************
* GetFrameRate
* Params:	None
* Return:		Frame rate that has been configured currently.
* Notes:		Used to get frame rate.
*********************************************************/
int GetFrameRate();

/*********************************************************
* SetFrameRate
* Params:	frame_rate - new frame rate desired. (1 - 30)
* Return:		None
* Notes:		Used to set frame rate.
*********************************************************/
void SetFrameRate(int frame_rate);

/********************************************************
* SetIntraFrameRate
* Params:	iframe_rate - Intra frame refresh rate.
* Return:		1 - Setting intra frame rate successful.
*			-1 - Failed to set intra frame rate. Check if any RTSP 
*				clients are present. Changing intra frame rate not
*				permitted when RTSP clients are present.
* Notes:		Used to set intra refresh rate.
*********************************************************/
int SetIntraFrameRate(int iframe_rate);

/*********************************************************
* GetIntraFrameRate
* Params: 	None
* Return:		- Return the value of the current intra frame rate that 
*				has been configured.
* Notes:		Used to get intra frame rate.
*********************************************************/
int GetIntraFrameRate();

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
int EnableAudio(size_t enable);

/********************************************************
* IsAudioEnabled
* Params:	None
* Return:		1 - Audio Enabled.
*			0 - Audio Disabled.
* Notes:		Used to check if audio is enabled or disabled.
*********************************************************/
int IsAudioEnabled(void);

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
int EnableVideo(size_t enalbe);

/**********************************************************
* IsVideoEnabled
* Params:	None
* Return:		1 - video enabled.
*			0 - video disabled.
* Notes:		Used to check if video is enabled or disabled.
**********************************************************/
int IsVideoEnabled(void);

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
int SetAudioCodec(int a_mode);

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
int GetAudioCodec();

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
int SetVideoCodec(int v_mode);

/********************************************************
* GetVideoCodec
* Params:	None
* Return:		1 - 	MPEG4
*			2 - 	M-JPEG
* Notes:		- Used to check which video codec (MPEG4 or M-JPEG) 
*				is currently used.
*			- Currently only MPEG4 is supported.
*********************************************************/
int GetVideoCodec(void);

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
					unsigned int aPort, unsigned int ttl);

/********************************************************
* IsMulticastEnable
* Params:	None
* Return:		0 -	If multicast is disabled.
*			1 - 	If multicast is enalbed.
* Notes:		This function returns whether multicast is enabled or not.
*********************************************************/
int IsMulticastEnable();

typedef enum {
	B_FALSE,
	B_TURE
} boolean_t;

/*********************************************************
* EnableMulticast
* Params:	boolean_t 	enable
*			boolean_t is defined as follows : 
*			typedef enum { B_FALSE, B_TRUE } boolean_t.
* Return:		None
* Notes:		This function is used to set the multicast enable flag.
*********************************************************/
void EnableMulticast(boolean_t enable);

int AnyRemoteClientPresent();
void TestIPCamConfigAPI();
int SetVideoWH(uint16_t width, uint16_t height);
uint16_t GetVideoWidth();
uint16_t GetVideoHeight();
void SetFrameRate(int frame_rate);
#endif


