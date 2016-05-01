#ifndef __VIN_SET_H__
#define  __VIN_SET_H__

#include "sun_type_defs.h"
#include "def_type.h"

extern unsigned int gImageWidth;
extern unsigned int gImageHeight;
extern int gFrameRate;
extern int gIntraFrameRate;
extern int gVideoQuality;

int SetQVGA();
int SetVGA();
int SetCIF();
int SetQCIF();
int SetCameraImgSize(uint16_t width, uint16_t height);
int SetCameraImgMode(ImageMode mode);
int IsVinEnabled();
ImageMode GetImageMode(uint16_t width, uint16_t height);
void SendAllCamRegValToLinux();
BOOL SetCamSingleRegister( BYTE reg, BYTE input);

#endif

