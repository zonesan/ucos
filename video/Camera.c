#include "video_cfg.h"

#include "Camera.h"
#include "def_type.h"
#include "iic.h"

#include "comm.h"
#include "2risc_comm_protocol.h"
#include "video_api.h"
#include "sun_type_defs.h"
 int gCameraSelectDigital;

#if INCLUDE_OV7720
extern int GetOV7720ID(void);
extern void HwOV7720Init(void);
extern void SetOV7720FrameRate(int frame_rate);
extern BOOL SetOV7720SingleRegister( BYTE reg, BYTE input);
void SetOV7720Brightness(void);
void SetOV7720Contrast(void);
void SetOV7720Hue(void);
void SetOV7720Saturation(void);
void SetOV7720Sharpness(void);
void SetOV7720Lightmode(void);
void SetOV7720Frequency(void);
void SetOV7720Homemode(void);
void SetOV7720ImageMirror(void);
void SetOV7720ImageFlip(void);
#endif

#if INCLUDE_OV7670
extern int GetOV7670ID(void);
extern void HwOV7670Init(void);
extern void SetOV7670FrameRate(int frame_rate);
extern BOOL SetOV7670SingleRegister( BYTE reg, BYTE input);
void SetOV7670Brightness(void);
void SetOV7670Saturation(void);
void SetOV7670Contrast(void);
void SetOV7670Hue(void);
void SetOV7670Sharpness(void);
void SetOV7670Lightmode(void);
void SetOV7670Frequency(void);
void SetOV7670Homemode(void);
void SetOV7670ImageMirror(void);
void SetOV7670ImageFlip(void);
#endif

#if INCLUDE_OV7650
extern int GetOV7650ID(void);
extern void HwOV7650Init(void);
extern void SetOV7650FrameRate(int frame_rate);
extern BOOL SetOV7650SingleRegister( BYTE reg, BYTE input);
#endif

#if INCLUDE_MT9M111
extern int GetMt9m111ID(void);
extern void HwMt9m111Init(void);
extern void SetMt9m111FrameRate(int frame_rate);
extern BOOL SetMt9m111SingleRegister( BYTE reg, BYTE input);
void SetMt9m111Brightness(void);
void SetMt9m111Sharpness(void);
void SetMt9m111Saturation(void);
void SetMt9m111Contrast(void);
void SetMt9m111Hue(void);
#endif

#if INCLUDE_MT9M112
extern int GetMt9m112ID(void);
extern void HwMt9m112Init(void);
extern void SetMt9m112FrameRate(int frame_rate);
extern BOOL SetMt9m112SingleRegister( BYTE reg, BYTE input);
#endif

#if INCLUDE_PO5130N
extern int GetPO5130NID(void);
extern void HwPO5130NInit(void);
#endif
Camera camera_list[] = {

#if INCLUDE_OV7720
    {    
        OV7720_ID,
        "OV7720",
        1,
        0x21,
        1,
        1,
        
        GetOV7720ID,
        HwOV7720Init,
        SetOV7720SingleRegister,
        0,
        SetOV7720FrameRate,
        SetOV7720Contrast, 0/*effect */,0/*exposure*/,SetOV7720Saturation, SetOV7720Brightness,SetOV7720Hue,SetOV7720Sharpness,SetOV7720Lightmode,SetOV7720Frequency,SetOV7720Homemode,SetOV7720ImageMirror,SetOV7720ImageFlip,
        
        4 /* contrast */, 0 /* effect */, 0 /* exposure */, 4/* saturation */, 4 /* brightness */, 4 /* hue */, 4 /* sharpness */,1/*lightmode*/,0/*frequency*/,0/*homemode*/,0/*imageMirror*/,0/*imageFlip*/        
    },
#endif

#if INCLUDE_OV7670
    {    
        OV7670_ID,
        "OV7670",
        1,
        0x21,
        1,
        1,
        
        GetOV7670ID,
        HwOV7670Init,
        SetOV7670SingleRegister,
        0,
        SetOV7670FrameRate,
        SetOV7670Contrast, 0/*Effect*/ , 0/*Exposure*/ , SetOV7670Saturation , SetOV7670Brightness ,SetOV7670Hue , SetOV7670Sharpness ,SetOV7670Lightmode,SetOV7670Frequency,0/*homemode*/,SetOV7670ImageMirror,SetOV7670ImageFlip,
        
        4 /* contrast */, 0 /* effect */, 0 /* exposure */, 4 /* saturation */, 4 /* brightness */, 4 /* hue */, 0 /* sharpness */,1/*lightmode*/,0/*frequency*/,0/*homemode*/,0/*imageMirror*/,0/*imageFlip*/        
    },
#endif

#if INCLUDE_OV7650

    {
        OV7650_ID,
        "OV7650",
        1,
        0x21,
        1,
        1,
        
        GetOV7650ID,
        HwOV7650Init,
        SetOV7650SingleRegister,
        0,
        SetOV7650FrameRate,
        0/*contrast*/, 0 /* effect */, 0 /* exposure */, 0/*saturation*/, 0/*brightness*/, 0/*hue*/, 0/*sharpness*/,0/*lightmide*/,0/*frequency*/,0/*homemode*/,0/*ImageMirror*/,0/*ImageFlip*/,
        
        0 /* contrast */, 0 /* effect */, 0 /* exposure */, 0 /* saturation */, 0 /* brightness */, 0 /* hue */, 0 /* sharpness */,0/*lightmode*/,0/*frequency*/,0/*homemode*/,0/*ImageMirror*/,0/*ImageFlip*/
    },
    
#endif

#if INCLUDE_MT9M111

    {
        MT9M111_ID,
        "Mt9M111",
        2,
        0x5d,
        1,
        1,
        
        GetMt9m111ID,
        HwMt9m111Init,
        SetMt9m111SingleRegister,
        0,
        SetMt9m111FrameRate,
        SetMt9m111Contrast, 0 /* effect */, 0 /* exposure */, SetMt9m111Saturation, SetMt9m111Brightness, SetMt9m111Hue, SetMt9m111Sharpness,0/*lightmide*/,0/*frequency*/,0/*homemode*/,0/*ImageMirror*/,0/*ImageFlip*/,
        
        7 /* contrast */, 0 /* effect */, 0 /* exposure */, 4 /* saturation */, 5 /* brightness */, 6 /* hue */, 2 /* sharpness */,0/*lightmode*/,0/*frequency*/,0/*homemode*/,0/*ImageMirror*/,0/*ImageFlip*/
    },
    
#endif

#if INCLUDE_MT9M112

    {
        MT9M112_ID,
        "Mt9M112",
        2,
        0x5d,
        1,
        1,
        
        GetMt9m112ID,
        HwMt9m112Init,
        SetMt9m112SingleRegister,
        0,
        SetMt9m112FrameRate,
        0/*contrast*/, 0 /* effect */, 0 /* exposure */, 0/*saturation*/, 0/*brightness*/, 0/*hue*/, 0/*sharpness*/,0/*lightmide*/,0/*frequency*/,0/*homemode*/,0/*ImageMirror*/,0/*ImageFlip*/,
        
        0 /* contrast */, 0 /* effect */, 0 /* exposure */, 0 /* saturation */, 0 /* brightness */, 0 /* hue */, 0 /* sharpness */,0/*lightmode*/,0/*frequency*/,0/*homemode*/,0/*ImageMirror*/,0/*ImageFlip*/
        
    },
    
#endif

#if INCLUDE_PO5130N

{
         PO5130N_ID,
         "PO5130",
         1,
         0x76,
         1,
         1,

         GetPO5130NID,
         HwPO5130NInit,
         0,
         0,         
         0,         
         0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0
    },

 #endif        
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
};

CameraSetting gtCameraSetting = {0 /* inited */, 0 /* contrast */, 0 /* effect */, 0 /* exposure */, 0 /* saturation */, 0 /* brightness */, 0 /* hue */, 0 /* sharpness */,0/*lightmode*/,0/*frequency*/,0/*homemode*/,0/*imageMirror*/,0/*imageFlip*/};//optimal image quality setting

Camera *GetDigitCamera(void)
{
    Camera *pcam;
    int device_id;
    debug("\nGetDigitCamera: get digital camera...\n");
    pcam = camera_list;
    static Risc_Packet_t pkt;
    unsigned int *pInt;
    pInt = (unsigned int *)&pkt;
    pkt.start = RISC_PKT_START;
    pkt.main_type = tSys;
    pkt.sub_type = tGetCameraID;
    pkt.length = 4;
    pkt.status = 0;	
    pkt.end = RISC_PKT_END;		

    while(pcam->deviceID)        
    {
        debug("detect %s...", pcam->desc);
        device_id = pcam->GetDeviceID();
        debug("device_id = 0x%x\n",device_id);
        if((pcam->deviceID == device_id) || (device_id == 0x773f))    //find the camera
        {    
            debug("\nGetDigitCamera: find %s camera!\n",pcam->desc);
            switch(device_id)
            {
                case OV7670_ID:
                    *(pInt + 2) = 0x0;                                //find ov7670 camera
                    break;
 
                case OV7720_ID:
                    *(pInt + 2) = 0x1;                                //find ov7720 camera
                    break;
 
                case MT9M111_ID:
                    *(pInt + 2) = 0x2;                                //find mt9m111 camera
                    break;

                case OV7650_ID:                                    //find ov7650 camera
                    *(pInt + 2) = 0x5;
                    break;

                default:
                    break;
            }
             
            if(TwoRiscCommSendPacket((size_t *)&pkt, RPACKET_LEN)==SUCCESSFUL) {    //send camera type command to linux
                    RiscFlagSet();    // Set risc1 flag to interrupt risc1     
            } else {
        	    printf("TwoRiscCommSendPacket error \n");
            }
 
            return pcam;
       }
        pcam++;
    }
    debug("\nGetDigitCamera: can't find digital cameras!\n");
    return 0;
}

