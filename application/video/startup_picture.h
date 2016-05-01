#include "video_cfg.h"

#ifdef STARTUP_PICTURE

#define STARTUP_PICTURE_COPY       //if define it, startup picture will be copied to local buffer

#include "os_cpu.h"
#include "os_cfg.h"
#include "uCOS_II.h"

#include "../../mpeg/mpeg4_decoder.h"
#include "two_risc_display.h"
//#include "../../video/VSC_API.h"

extern  tDisplayFormat gtStartupPic;
extern unsigned int startupPic_addr;

char AppGetStiImgData(unsigned int src, unsigned int len);
#endif

