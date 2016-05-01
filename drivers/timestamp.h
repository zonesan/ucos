#ifndef TIMESTAMP_H
#define TIMESTAMP_H

#ifdef TIMESTAMP_GLOBALS
#define TIMESTAMP_EXT
#else
#define TIMESTAMP_EXT extern
#endif

#include "audio_cfg.h"
#define tSfUNCtEST

#ifdef AUDIO_20MS_INTERRUPT
#define AUDIO_FS            50
#else
#define AUDIO_FS            100
#endif

#define SYS_TS_HZ           AUDIO_FS
#define SYS_TS_PERIOD_MS    (1000 / SYS_TS_HZ)

#define VIDEO_NORMAL         0
#define VIDEO_EARLY_L1      1
#define VIDEO_EARLY_L2      2
#define VIDEO_LATE_L1        4
#define VIDEO_LATE_L2        8

#define AVSYNC                1
#define SELFSYNC            2
#define NOSYNC               3
#define SYNCMASK           4          //when bit2 is set, indicate linux has changed the sync mode, so ucos will not changed it.

typedef struct _avsync
{
    struct{
        char flag;             // 0: normal, 1:speed up, 2: slow down
        char val;              //
    }speed;

    int decodeinterval;
    int playth;
    int dropth;
    int play_ts_diff;
    int drop_ts_diff;
    int syncmode;

    unsigned int systemts;
    unsigned int refts;
    unsigned int videots;

    unsigned int dropflag;
}avsync;

/************************ globle variables ********************************************/
TIMESTAMP_EXT avsync gtavsync;
/************************* goble functions ********************************************/
int SetTsThreshold(unsigned int playTh,unsigned int dropTh );
void ResetSysTimeStamp(void);
void ResetVideoTimeStamp(void);
void ResetTimeStampRef(void);
void AdjustTsRef(unsigned int ts, int sign);
void AdjustVideoTs(unsigned int ts, int sign);
inline void IncSysTimeStamp(void);
inline int GetSysTimeStamp(void);
inline int ReflashTimeStampRef(unsigned int ts);
void ReflashTimeStampVideo(unsigned int ts);
int SetSyncMode(unsigned int mode);
int AvSyncHandler(int BufSpace);
int VideoSelfSync(unsigned int BufSpace, unsigned int ts);
void SetVideoJudegDrop(void);
int IsVideoJudgeDrop(void);
void avsync_init(int playth, int dropth);
void set_avsync_default(void);
#endif

