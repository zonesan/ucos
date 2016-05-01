#include "audio_cfg.h"

#ifdef INCLUDE_SI3210

#include "../inc/audio_cfg.h"
#include "../drivers/sif.h"

unsigned int *slic_tx_share_addr_wr;

//extern int slic_flag = 0;
void Slic_Task(void *);
void TwoRiscCommProcSlic(unsigned int *, unsigned int, unsigned int);
void TwoRiscSetPara(int *point);
void HwSlicPhoneSystemLoop();
void dumpregs(ULONG channelid);
void TwoRiscSendMsg(char key,int channelid);
VOID HwSlicStartTone(ULONG type,ULONG channelid);

#endif
