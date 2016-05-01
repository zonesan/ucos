#ifndef __ppe_h__
#define __ppe_h__

#include "risc.h"

//#define DEBUG_PPE_PERFORMANCE

//RISC local register specifications 
#define PPE_ITAG		0x200

#define PPE_CTL 	0x600
#define PPE_CACHE_BASE 	0x604
#define PPE_MAC_SA_LO	0x608
#define PPE_MAC_SA_HI	0x60C
#define PPE_QID 	0x610
#define PPE_CACHE_INVLD 0x640
#define PPE_DEBUG		0x644
#define PPE_PC		0x648
#define PPE_STATUS		0x688	

#define PPE_ITAG_NUM	128

#define PPE_REGS_ADDR(REGS)	 (BLOCK_BASE_ADDR(PPE_BLOCK)+REGS)

void HwPpeIntrInit(void);
void HwPpeInitY(void);
void HwPpeCompareY(int y1_addr, int y2_addr, int width, int height);

#endif
