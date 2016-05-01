#ifndef VIDEO_BUFFER_H
#define VIDEO_BUFFER_H

#include "video_cfg.h"
#ifdef DISPLAY_INCLUDE
#ifdef VIDEO_BUFFER_GLOBALS
#define VIDEO_BUFFER_EXT
#else
#define VIDEO_BUFFER_EXT extern
#endif


#define MAX_BUFFER_SIZE     (512 * 1024)     //as big as the share buffer, but memory is not enough,and 512 is enough
#define MINI_PACKAGE_SIZE   (16 * 1024)
#define CUSRSOR_NUMBER      MAX_BUFFER_SIZE / MINI_PACKAGE_SIZE

#define STATIC_BUFFER

#define USE_CRITICAL

typedef struct __VideoCursor
{
    struct __VideoCursor *nxt;
    struct __VideoCursor *pre;
    unsigned int StartAddr;            //the frame's start addr, 16Byte aligned
    unsigned int EndAddr;             //frame's end addr, = StartAddr + PkgSize - 1
    unsigned int PkgSize;
    unsigned int TimeStamp;
    unsigned int EndSpace;          //end space in the end of frame that not used
    unsigned int dropflag;           // 1: drop this frame before decode
}VIDEO_CURSOR;

typedef struct __VideoBuffer
{
#ifdef STATIC_BUFFER
#ifdef NEW_JITBUF_ALGORITHM
    unsigned char buffer[MAX_BUFFER_SIZE + 16];
#else
    unsigned char buffer[MAX_BUFFER_SIZE];
#endif
    VIDEO_CURSOR cursor[CUSRSOR_NUMBER];
#else
    unsigned char *buffer;
    VIDEO_CURSOR *cursor;
#endif
    unsigned int StartAddr;     //jitbuf start addr, should 16Byte aligned
    unsigned int EndAddr;       //jitbuf end addr, = StartAddr + bufsize - 1
    unsigned int BufSize;         //jitbuf size
    unsigned int CursorNumber;   //jitbuf total node number in list
    unsigned int BufCnt;                //the space that has been used yet
    unsigned int CurCnt;               //the total node number that has been used yet
    VIDEO_CURSOR *RdCursor;  //the read node struct point
    VIDEO_CURSOR *WrCursor; //the write node struct point
}VIDEO_BUFFER;

typedef struct __VideoFrame
{
    unsigned int addr;
    unsigned int size;
    unsigned int TimeStamp;
    unsigned int CurrentDepth;          //cursor size, av sync will use
    unsigned int CurrentSpace;          //cursor size, av sync will use
    unsigned int dropflag;
}VIDEO_FRAME;

typedef struct __BufferState
{
    unsigned int BufSize;
    unsigned int CurSize;
}BUFFER_STATE;

/*************************************************************************/
VIDEO_BUFFER_EXT VIDEO_BUFFER VideoBuffer1;
VIDEO_BUFFER_EXT VIDEO_FRAME VideoFrameReceive;

VIDEO_BUFFER_EXT unsigned int miniFrameInBuf;       //mini num of frames in jitbuffer
VIDEO_BUFFER_EXT unsigned int maxFrameInBuf;        //max num of frames in jitbuffer
/*************************************************************************/
#ifdef STATIC_BUFFER
void VBufInit(VIDEO_BUFFER *buf);
#else
VIDEO_BUFFER *VBufInit(unsigned int BufferSize, unsigned int MiniVideoSize);
void VBufDestroy(VIDEO_BUFFER *buf);
#endif

int SetMiniFrameNumInBuf(unsigned int cnt);
int SetMaxFrameNumInBuf(unsigned int cnt);
void ClearCursor(VIDEO_CURSOR *cursor);
int ReleaseCursor(VIDEO_BUFFER *buf, unsigned int addr);
int VBufGetOneFrame(VIDEO_BUFFER *buf, VIDEO_FRAME *frame);
int VBufSaveOneFrame(VIDEO_BUFFER *buf, VIDEO_FRAME *frame);
unsigned int VBufGetCurFrameSize(VIDEO_BUFFER *buf);
unsigned int VBufGetCurTimeStamp(VIDEO_BUFFER *buf);
void VBufGetDepth(VIDEO_BUFFER *buf, BUFFER_STATE *state);
void VBufGetSpace(VIDEO_BUFFER *buf, BUFFER_STATE *state);
void VideoBufferReset(VIDEO_BUFFER *buf);

#endif
#endif
