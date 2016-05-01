#ifndef G711_H
#define G711_H

#include "def_type.h"

//16 bit pcm to alaw, ulaw
VOID G711AlawEncode(SHORT *lBuf, BYTE *oBuf, LONG nSmp);  
VOID G711UlawEncode(SHORT *lBuf, BYTE *oBuf, LONG nSmp);  

//alaw, ulaw to 16 bit pcm
VOID G711AlawDecode(BYTE *iBuf, SHORT *lBuf, LONG nSmp);
VOID G711UlawDecode(BYTE *iBuf, SHORT *lBuf, LONG nSmp);

#endif
