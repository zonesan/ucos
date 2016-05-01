
#define ANALOG_CAMERA_GLOBLS

#include "video_cfg.h"

#include "Camera.h"
#include "analog_camera.h"
#include "def_type.h"
#include "iic.h"

#include "comm.h"
#include "2risc_comm_protocol.h"
#include "video_api.h"
#include "sun_type_defs.h"

#include "../application/video/two_risc_video.h"

#if INCLUDE_ADV7181
extern int GetAdv7181ID(void);
extern void HwAdv7181Init(void);
extern unsigned char ADV7181AutodetectVMode(unsigned int *vmode);
void SetAdv7181Brightness(void);
void SetAdv7181Contrast(void);
void SetAdv7181Hue(void);
void SetAdv7181Saturation(void);
void SetAdv7181Sharpness(void);
#endif

#if INCLUDE_TVP5150A
extern int GetTvp5150aID(void);
extern void HwTvp5150aInit(void);
extern unsigned char GetTvp5150aVStd(unsigned int *vstd);
void SetTvp5150aBrightness(void);
void SetTvp5150aContrast(void);
void SetTvp5150aHue(void);
void SetTvp5150aSaturation(void);
void SetTvp5150aSharpness(void);
#endif

Camera analog_camera_list[] = {

#if INCLUDE_ADV7181

    {
        ADV7181_ID,
        "ADV7181",
        1,
        0x20,
        1,
        0,
        
        GetAdv7181ID,
        HwAdv7181Init,
        0 /* Set SingleRegister */,
        ADV7181AutodetectVMode,
        0 /* Set FrameRate */,
        SetAdv7181Contrast, 0 /* effect */, 0 /* exposure */, SetAdv7181Saturation , SetAdv7181Brightness, SetAdv7181Hue , SetAdv7181Sharpness ,0/*lightmode*/,0/*frequency*/,0/*homemode*/,0/*ImageMirror*/,0/*ImageFlip*/,
        
        4 /* contrast */, 0 /* effect */, 0 /* exposure */, 4 /* saturation */, 4 /* brightness */, 4 /* hue */, 0 /* sharpness */,0/*lightmode*/,0/*frequency*/,0/*homemode*/,0/*ImageMirror*/,0/*ImageFlip*/
    },
    
#endif

#if INCLUDE_TVP5150A

    {
        TVP5150A_ID,
        "TVP5150A",
        1,
        0x5D,
        1,
        0,
        
        GetTvp5150aID,
        HwTvp5150aInit,
        0/* Set SingleRegister */,
        GetTvp5150aVStd,
        0/* Set FrameRate */,
        SetTvp5150aContrast, 0 /* effect */, 0 /* exposure */, SetTvp5150aSaturation , SetTvp5150aBrightness, SetTvp5150aHue , SetTvp5150aSharpness ,0/*lightmode*/,0/*frequency*/,0/*homemode*/,0/*ImageMirror*/,0/*ImageFlip*/,
        4 /* contrast */, 0 /* effect */, 0 /* exposure */, 4 /* saturation */, 4 /* brightness */, 4 /* hue */, 0 /* sharpness */,0/*lightmode*/,0/*frequency*/,0/*homemode*/,0/*ImageMirror*/,0/*ImageFlip*/
    },
    
#endif

    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
};

Camera *GetAnalogCamera(void)
{
    Camera *pcam;
    int device_id;
    debug("\nGetAnalogCamera: get analog camera...\n");
    pcam = analog_camera_list;
#if 0               //now we move it to the ADV7181AutodetectVMode() function
    static Risc_Packet_t pkt;
    unsigned int *pInt;
    pInt = (unsigned int *)&pkt;
    pkt.start = RISC_PKT_START;
    pkt.main_type = tVideo;
    pkt.sub_type = tGetCameraType;
    pkt.length = 4;
    pkt.status = 0;	
    pkt.end = RISC_PKT_END;		
#endif

    while(pcam->deviceID)        
    {
        debug("detecting %s...", pcam->desc);
        device_id = pcam->GetDeviceID();
        debug("device_id = 0x%x\n",device_id);
        if(pcam->deviceID == device_id)    //find the camera
        {    
            debug("\nGetAnalogCamera: find %s OK!\n",pcam->desc);
#if 0
            *(pInt + 2) = 0x3;             //find adv7181
            if(TwoRiscCommSendPacket((size_t *)&pkt, RPACKET_LEN)==SUCCESSFUL) {    //send adv7181 command to linux
        	    RiscFlagSet();    // Set risc1 flag to interrupt risc1     
            } else {
        	    printf("TwoRiscCommSendPacket error \n");
            }
#endif
            return pcam;
        }
        pcam++;
    }
    debug("\nGetAnalogCamera: can't find analog cameras!\n");
    return 0;
}

extern unsigned int gImageWidth, gImageHeight;
void AnalogCameraRatioSet()
{
        unsigned int ratio, width_ratio, height_ratio;

        /* judge the width & height less than the MINIMUM? */
        if(gImageWidth < MIN_WIDTH) {
            gImageWidth = MIN_WIDTH;
            printf("\nAnalogCameraRatioSet: gImageWidth less than MIN_WIDTH(%d)! gImageWidth = MIN_WIDTH!\n", MIN_WIDTH);
        }

        if(gImageHeight < MIN_HEIGHT) {
            gImageHeight = MIN_HEIGHT;
            printf("\nAnalogCameraRatioSet: gImageHeight less than MIN_HEIGHT(%d)! gImageHeight = MIN_HEIGHT!\n", MIN_HEIGHT);
        }

        /* judge the width & height exceeds the MAXIUM? */
        if(gImageWidth > MAX_VIN_WIDTH) {
            gImageWidth = MAX_VIN_WIDTH;
            printf("\nAnalogCameraRatioSet: gImageWidth exceeds MAX_VIN_WIDTH(%d)! gImageWidth = MAX_VIN_WIDTH!\n", MAX_VIN_WIDTH);
        }

        if(gImageHeight > MAX_VIN_HEIGHT) {
            gImageHeight = MAX_VIN_HEIGHT;
            printf("\nAnalogCameraRatioSet: gImageHeight exceeds MAX_VIN_HEIGHT(%d)! gImageHeight = MAX_VIN_HEIGHT!\n", MAX_VIN_HEIGHT);
        }    

         /* judge the width & height exceeds the d1? */
        if(gImageWidth > gtVinFormat.width) {
            gImageWidth = gtVinFormat.width;
            printf("\nAnalogCameraRatioSet: gImageWidth exceeds gtVinFormat.width(%d)! gImageWidth = gtVinFormat.width!\n", gtVinFormat.width);
        }

        if(gImageHeight > gtVinFormat.height) {
            gImageHeight = gtVinFormat.height;
            printf("\nAnalogCameraRatioSet: gImageHeight exceeds gtVinFormat.height(%d)! gImageHeight = gtVinFormat.height!\n", gtVinFormat.height);
        }    

        width_ratio = gtVinFormat.width / gImageWidth;
        height_ratio = gtVinFormat.height / gImageHeight;
        ratio = (width_ratio < height_ratio) ? width_ratio : height_ratio;

        if(ratio > 4)
            ratio = 4;

        switch(ratio) {
            case 1:     
                AnalogCameraRatio = 0;    // no scale
                break;
                
            case 2:   
            case 3:    
                AnalogCameraRatio = 1;    // scale down 2
                break;                

            case 4:             
                AnalogCameraRatio = 2;    // scale down 4
                break;

            default:    // should not happen
                AnalogCameraRatio = 0;
                break;

        }

        printf("\nAnalogCameraRatioSet: AnalogCameraRatio = %d!\n", AnalogCameraRatio);
        
}

