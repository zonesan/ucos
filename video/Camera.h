
#ifndef __CAMERA_H
#define __CAMERA_H

//#define ALL_DIGIT_CAMERA
#define OV7720_ID        0x7721
#define OV7670_ID	0x7673		//product ID & VER
#define OV7650_ID	0x7648
#define MT9M111_ID	0x143A		//chip version
#define MT9M112_ID	0x148c
#define PO5130N_ID      0x5130

typedef struct {
    unsigned int deviceID;   //
    char *desc;  
    char reg_width;
    char iic_addr;
    unsigned char clk_polority;   //set the clock for VIN:0 posedge 1:negedge
    unsigned char frame_pic;    //set the frame_pic for VIN:0: interlaced pictue 1: progressive picture
    
    unsigned int (*GetDeviceID)(void);
    void (*camera_init)(void);       	//function call for init camera
    unsigned int (*SetCamRegister)(unsigned int addr,unsigned int input);
    unsigned char (*GetCamVideoStd)(unsigned int *vstd);
    void (*SetFrameRate)(int frame_rate);
    void (*camera_set_contrast)(void);    
    void (*camera_set_effect)(void);    
    void (*camera_set_exposure)(void);    
    void (*camera_set_saturation)(void);    
    void (*camera_set_brightness)(void);    
    void (*camera_set_hue)(void);    
    void (*camera_set_sharpness)(void);    
    void (*camera_set_lightmode)(void);
    void (*camera_set_frequency)(void);
    void (*camera_set_homemode)(void);
    void (*camera_set_image_mirror)(void);
    void (*camera_set_image_flip)(void);
    
    int contrast;
    int effect;
    int exposure;
    int saturation; 
    int brightness; 
    int hue; 
    int sharpness;
    int lightmode;
    int frequency;
    int homemode;
    int image_mirror;
    int image_flip;
}Camera,*pCamera;

typedef struct {
    short inited;   // 0: not inited yet!; 1: inited already
    short contrast;
    short effect;
    short exposure;
    short saturation; 
    short brightness; 
    short hue; 
    short sharpness;
    short lightmode;
    short frequency;
    short homemode;
    short image_mirror;    //0 mormal image, 1 mirror image
    short image_flip;    //0 mormal image, 1 vertical flip image
}CameraSetting;


extern CameraSetting gtCameraSetting;

Camera *GetDigitCamera(void);

#endif

