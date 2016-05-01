#ifndef __vb1000_h__
#define __vb1000_h__

#include "def_type.h"
#include "risc.h"

#define EMAC_RMII_TYPE	1
#define DMAC_EMAC_TYPE	1
#define EMAC_DEBUG 1
#define ROMFS_IMAGE_LEN 5612544

#define TRUE 		1
#define FALSE		0
#define SUCCESSFUL 	1
#define FAILURE		0
#define FAILED	 	0

#ifndef NULL 
#define NULL 0
#endif

#define rdb(addr) (*(volatile unsigned char*)(addr))
#define rdw(addr) (*(volatile unsigned short*)(addr))
#define rdl(addr) (*(volatile unsigned int*)(addr))
#define wrb(data,addr) ((*(volatile unsigned char*)(addr))=(data))
#define wrw(data,addr) ((*(volatile unsigned short*)(addr))=(data))
#define wrl(data,addr) ((*(volatile unsigned int*)(addr))=(data))



#endif
