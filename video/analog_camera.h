
#ifndef __ANALOG_CAMERA
#define __ANALOG_CAMERA

#include "Camera.h"

#ifdef ANALOG_CAMERA_GLOBLS
#define analog_camera_ext
#else
#define analog_camera_ext extern
#endif

#define ADV7181_ID     0x7181		//chip version
#define TVP5150A_ID    0x5150             //chip version

analog_camera_ext unsigned int AnalogCameraRatio;

Camera *GetAnalogCamera(void);
void AnalogCameraRatioSet();
#endif

