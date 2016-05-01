#define TIMESTAMP_GLOBALS

#include "timestamp.h"
#include <comm.h>
#include "two_risc_display.h"
#include "video_buffer.h"

void ResetSysTimeStamp(void)
{
    gtavsync.systemts = 0;
}

void ResetTimeStampRef(void)
{
    gtavsync.refts = 0;
}

void ResetVideoTimeStamp(void)
{
    gtavsync.videots = 0;
}

inline void IncSysTimeStamp(void)
{
    gtavsync.systemts++;
}

inline int GetSysTimeStamp(void)
{
    return gtavsync.systemts;
}

inline int ReflashTimeStampRef(unsigned int ts)
{
    gtavsync.refts = ts;
}

void AdjustTsRef(unsigned int ts, int sign)
{
    if(sign == 1)
        gtavsync.refts += ts;
    else if(sign == -1)
        gtavsync.refts -= ts;
    
    if(gtavsync.refts < 0)
        gtavsync.refts = 0;
}

void AdjustVideoTs(unsigned int ts, int sign)
{
    if(sign == 1)
        gtavsync.videots += ts;
    else if(sign == -1)
        gtavsync.videots -=ts;

    if(gtavsync.videots < 0)
        gtavsync.videots = 0;
}

void ReflashTimeStampVideo(unsigned int ts)
{
    gtavsync.videots = ts;    //save video time stamp

    gtavsync.dropflag = 0;
}

int SetSyncMode(unsigned int mode)
{
    if((mode > 3) || (mode < 1)) {
        printf("Linux Set SyncMode error! error = %d\n", mode);
        return 1;
    }else {
        gtavsync.syncmode = mode | SYNCMASK;           //indicate linux has setup the sync mode, ucos will not change it
        printf("Set SyncMode = %d!\n", gtavsync.syncmode);
    }
    return 0;
}

#ifdef VIDEO_MODULE_INCLUDE
#ifdef DISPLAY_INCLUDE
int SetTsThreshold(unsigned int playth, unsigned int dropth )
{
    int tmp;

    tmp = 1000 / AUDIO_FS;
    if(playth < tmp){
        printf("time stemp different is to small! must more than %dms!\n", tmp);
        return -1;
    }
    if(dropth < playth){
        printf("dropThreshold must bigger than playThreshold %dms,%dms!\n", playth, dropth);
        return -1;
    }
    gtavsync.playth = playth;
    gtavsync.play_ts_diff = playth * SYS_TS_HZ / 1000;
    gtavsync.dropth = dropth;
    gtavsync.drop_ts_diff = dropth * SYS_TS_HZ / 1000;
    printf("Set timestampthreshold, playTh=%d, dropTh=%d\n", playth, dropth);
    
    return 0;
}
/*******************************************************************************
**                                              VideoSelfSync
**
**
**
**
******************************************************************************/
int VideoSelfSync(unsigned int CursorUsed, unsigned int ts)
{
    int tmp;
  
    ReflashTimeStampRef(gRiscTimerCnt);
        
    if(ts == gtavsync.videots)
        printf("VideoSelfSync Timestamp error!%d\n", ts);
    
    tmp = ts - gtavsync.videots;
    tmp = tmp * 10;                          //the time between two successive frames: ms
    if(gtavsync.speed.flag == 1)
        tmp = tmp / gtavsync.speed.val;
    else if(gtavsync.speed.flag == 2)
        tmp = tmp * gtavsync.speed.val;

    if(tmp > (gtavsync.refts + gtavsync.dropth)){   //too eraly,drop it after decode
        ReflashTimeStampVideo(ts);     //
        return VIDEO_EARLY_L2;
    }

    if(CursorUsed >= maxFrameInBuf) {     //buf has too much frames,should decode immediatly
        ReflashTimeStampVideo(ts);
        return VIDEO_NORMAL;
    }

    if(CursorUsed <= miniFrameInBuf) {  //buf has very few frames,then interval between two frames add 10ms
        if((tmp + 10) >= gtavsync.refts)     //can delay 10ms
            return VIDEO_EARLY_L1;
        else{
            ReflashTimeStampVideo(ts);
            return VIDEO_NORMAL;
        }
    }
 
    if(tmp > gtavsync.refts){   //early
        if(tmp >= (gtavsync.refts + 10))     //early, can delay 10ms
            return VIDEO_EARLY_L1;
        else{                                //early but acceptable
            ReflashTimeStampVideo(ts);
            return VIDEO_NORMAL;
        }
    }else{     //late
        if((tmp + gtavsync.dropth) < gtavsync.refts){   //too late, drop it after decode
            ReflashTimeStampVideo(ts);     //save video timestamp
            return VIDEO_LATE_L2;
        }
        else{                                         //late but acceptable, deocde now
            ReflashTimeStampVideo(ts);
            return VIDEO_LATE_L1;
        }
    }
}

/****************************************************************************
**                                      AvSyncHandler
**
**
**
***************************************************************************/
int AvSyncHandler(int BufSpace)
{
    int var1, var2;
    int fr, tmp, BufLevel;
    int i;
    
/*
    i = RiscRdCNT(VIDEO_LINUX_TO_UCOS_CNT);
    if(BufSpace != i)
        printf("error,cnt=%d,jitbuf depth=%d\n", i, BufSpace);

    if((BufSpace > 20) || (BufSpace < 0))
        printf("AvSyncHandler BufSpace error!%d\n", BufSpace);
*/
    var1 = (gtavsync.refts + gtavsync.play_ts_diff);    //the play timestamp allowed
    var2 = (gtavsync.videots + gtavsync.play_ts_diff);
    
    if((gtavsync.videots <= var1) && (var2 >= gtavsync.refts)){
        return VIDEO_NORMAL;
    } 
#if 0
    if(VideoTimeStamp > var1){
        tmp = VideoTimeStamp - var1;
        tmp = tmp * (1000 / SYS_TS_HZ);   // gb: counted by ms

    //if Vide time stamp more early than Audio Time Stamp and 3/5 buffer, must drop package! ms
    //buflevel = 3/5 * space * period *1000 ms
    //buflevel = 600 * space / framerate
        fr = GetFrameRate();          //Hz
        BufLevel =  (600 * BufSpace) / fr;   
        printf("BLevel = %dms(%d), Dts = %dms, %d, %d\n", BufLevel, BufSpace, tmp, VideoTimeStamp, TimeStampRef);

        if(tmp > BufLevel)
            return VIDEO_EARLY_L2;     // video buffer is not big enough
                                                    // gb: Maybe some video frames should be gave up.
        else
            return VIDEO_EARLY_L1;    // gb: video buffer is big enough. video buffer just wait. 
    }
#else
    if(gtavsync.videots > var1) {
        tmp = gtavsync.videots - var1;
        if(tmp > 100)                                         // 1s
            return VIDEO_EARLY_L2;
        else
            return VIDEO_EARLY_L1;
    }
#endif

    if((var2 < gtavsync.refts) && ((gtavsync.videots + gtavsync.drop_ts_diff) > gtavsync.refts)){
        return VIDEO_LATE_L1;        //late but acceptable
    }else if((gtavsync.videots + gtavsync.drop_ts_diff) < gtavsync.refts)
        return VIDEO_LATE_L2;    
}


void SetVideoJudegDrop(void)
{
    gtavsync.dropflag = 1;
}

int IsVideoJudgeDrop(void)
{
    return gtavsync.dropflag;
}

void avsync_init(int playth, int dropth)
{
    SetTsThreshold(playth, dropth);
}

void set_avsync_default(void)
{
    gtavsync.speed.flag = 0;
    gtavsync.speed.val = 1;
    gtavsync.syncmode = SELFSYNC;
    gtavsync.decodeinterval = 5;
}

#endif
#endif
