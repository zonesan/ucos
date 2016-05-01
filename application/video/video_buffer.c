#define VIDEO_BUFFER_GLOBALS

#include "video_cfg.h"
#ifdef DISPLAY_INCLUDE

#include <libc_s.h>
#include "os_cpu.h"
#include "os_cfg.h"
#include "uCOS_II.h"
#include "cache.h"

#include "comm.h"
#include "video_buffer.h"
#include "two_risc_video.h"
#include "two_risc_statistics.h"

#ifdef STATIC_BUFFER

void VBufInit(VIDEO_BUFFER *buf)
{
    buf->BufSize = MAX_BUFFER_SIZE;
    buf->CursorNumber = CUSRSOR_NUMBER;
    //buf->StartAddr = (unsigned int)(buf->buffer) | 0xa0000000;
#ifdef NEW_JITBUF_ALGORITHM
    buf->StartAddr = ((unsigned int)(buf->buffer) + 0x10) & 0x1ffffff0 | 0x80000000;    //use cache, and 16bit aligned
#else
    buf->StartAddr = (unsigned int)(buf->buffer) & 0x1fffffff | 0x80000000;    //use cache, and 16bit aligned
#endif
    buf->EndAddr = buf->StartAddr + buf->BufSize - 1;
    VideoBufferReset(buf);
}

#else
VIDEO_BUFFER *VBufInit(unsigned int BufferSize, unsigned int MiniVideoSize);
{
    VIDEO_BUFFER *buf;
    buf = (VIDEO_BUFFER *)malloc(sizeof(VIDEO_BUFFER));
    if(buf == NULL)
        return NULL;
    buf->buffer = (unsigned char *)malloc(BufferSize);
    if(buf->buffer == NULL)
    {
        free(buf);
        return NULL;
    }
    buf->cursor = (VIDEO_CURSOR *)malloc(BufferSize/MiniVideoSize);
    if(buf->cursor = NULL)
    {
        free(buf->buffer);
        free(buf);
        return NULL;
    }
    buf->BufSize = BufferSize;
    buf->CursorNumber = BufferSize/MiniVideoSize;
    buf->StartAddr = (unsigned int)(buf->buffer) | 0xa0000000;
    buf->EndAddr = buf->StartAddr + buf->BufSize - 1;
    
    VideoBufferReset(buf);
    
    return buf;
}
void VBufDestroy(VIDEO_BUFFER *buf);
{
    free(buf->buffer);
    free(buf->cursor);
    free(buf);
    buf = NULL;
}
#endif

void ClearCursor(VIDEO_CURSOR *cursor)
{
    cursor->StartAddr = 0;
    cursor->EndAddr = 0;
    cursor->PkgSize = 0;
    cursor->TimeStamp = 0;
    cursor->dropflag = 0;
    cursor->EndSpace = 0;
}

int ReleaseCursor(VIDEO_BUFFER *buf, unsigned int addr)
{
    if(buf->RdCursor->StartAddr != addr){
        printf("%x,%x\n", addr, buf->RdCursor->StartAddr);
        return 1;
    }

#ifdef USE_CRITICAL
    OS_CPU_SR  cpu_sr;
    OS_ENTER_CRITICAL();
#endif

    buf->BufCnt -= buf->RdCursor->PkgSize;
    if(buf->RdCursor->EndSpace)
        buf->BufCnt -= buf->RdCursor->EndSpace;
    buf->CurCnt--;
    ClearCursor(buf->RdCursor);
    buf->RdCursor = buf->RdCursor->nxt;
    
#ifdef USE_CRITICAL
    OS_EXIT_CRITICAL();
#endif

    return 0;
}

int SetMiniFrameNumInBuf(unsigned int cnt)
{
    if(cnt > VideoBuffer1.CursorNumber){
        printf("SetMiniFrameNumInBuf error!,%d\n", cnt);
        return 1;
    }
    else
        miniFrameInBuf = cnt;
    return 0;
}

int SetMaxFrameNumInBuf(unsigned int cnt)
{
    if(cnt > VideoBuffer1.CursorNumber){
        printf("SetMiniFrameNumInBuf error!,%d\n", cnt);
        return 1;
    }
    else
        maxFrameInBuf = cnt;
    return 0;
}

int VBufGetOneFrame(VIDEO_BUFFER *buf, VIDEO_FRAME *frame)
{
    
#ifdef USE_CRITICAL
    OS_CPU_SR  cpu_sr;
    OS_ENTER_CRITICAL();
#endif
    if((buf->BufCnt == 0) || (buf->CurCnt == 0))
    {
#ifdef USE_CRITICAL
        OS_EXIT_CRITICAL();
#endif
        return 1;
    }
    frame->addr = buf->RdCursor->StartAddr;
    frame->size = buf->RdCursor->PkgSize;
    frame->TimeStamp = buf->RdCursor->TimeStamp;
    frame->CurrentDepth = buf->CurCnt;
    frame->CurrentSpace = buf->CursorNumber - buf->CurCnt;
    frame->dropflag = buf->RdCursor->dropflag;
#ifdef USE_CRITICAL
    OS_EXIT_CRITICAL();
#endif
    return 0;
}

/*
* argument: length: in bytes
* note: this function is only used when checksum is enabled.
*/
char MyMemCpy_Checksum(unsigned int *dtaddr, unsigned int *sraddr, unsigned int length)
{
    unsigned int i;
    unsigned int len;
    unsigned int sum = 0;
    char err = 0;

    len = length/sizeof(int) - 4;
    if((length-16) != (*(sraddr+len)))   //the length contain framelength,checksum, framenum, timestamp, all 16bytes
    {
        printf("\n\nencodeDataChksum: frame length(%d,%d) error!\n\n", length, *(sraddr+len));
        gtStatistics.cnt_framelength_err++;
        err = 1;
    }

    for(i=0; i<len; i++)       //just copy frame data, not include the 4:framelen,checksum,framenum and timestamp
    {
        *dtaddr++ = *sraddr;
        sum += *sraddr++;
    }

    if(sum != *(sraddr+1))   //the checksum
    {
        printf("\n\nencodeDataChksum: checksum(0x%x,0x%x) error!\n\n", sum, *(sraddr + 1));
        gtStatistics.cnt_checksum_err++;
        if(err == 0) err = 2;         
    }

    for(i=0; i<4; i++)          //now, copy the 4 appendix data
        *dtaddr++ = *sraddr++;
    
    return err;
}

int VBufSaveOneFrame(VIDEO_BUFFER *buf, VIDEO_FRAME *frame)
{
    VIDEO_CURSOR *cursor;
    int EndSpace;
    char ret;

#ifdef USE_CRITICAL
    OS_CPU_SR  cpu_sr;
    OS_ENTER_CRITICAL();
#endif

    if(buf->CurCnt >= buf->CursorNumber - 1)
    {
#ifdef USE_CRITICAL
        OS_EXIT_CRITICAL();
#endif
        return 1;
    }
    
    if(frame->size == 0)
    {
#ifdef USE_CRITICAL
        OS_EXIT_CRITICAL();
#endif
        return 2;
    }

    if((buf->BufCnt + frame->size - 1) >= buf->BufSize)  //have enough space to save this frame?
    {
#ifdef USE_CRITICAL
        OS_EXIT_CRITICAL();
#endif
        return 4;
    }

#ifdef NEW_JITBUF_ALGORITHM
    cursor = buf->WrCursor;
    if(cursor->StartAddr < buf->RdCursor->StartAddr)
    {
        if((buf->RdCursor->StartAddr - cursor->StartAddr) > (frame->size + 32))
        {
            cursor->EndAddr = cursor->StartAddr + frame->size - 1;
            buf->WrCursor = cursor->nxt;
            buf->WrCursor->StartAddr = (cursor->EndAddr + 32) & 0x1ffffff0|0x80000000;
            buf->WrCursor->EndAddr = buf->WrCursor->StartAddr;
            cursor->EndSpace = buf->WrCursor->StartAddr - cursor->EndAddr - 1;
            buf->BufCnt += (frame->size + cursor->EndSpace);
        }else {
#ifdef USE_CRITICAL
            OS_EXIT_CRITICAL();
#endif
            return 4;
        }
    }else {
        if((cursor->StartAddr + frame->size - 1) <= buf->EndAddr)   //can save in the end
        {
            cursor->EndAddr = cursor->StartAddr + frame->size - 1;
            buf->WrCursor = cursor->nxt;
            if((buf->EndAddr - cursor->EndAddr) > 32) {
                buf->WrCursor->StartAddr = (cursor->EndAddr + 32) & 0x1ffffff0|0x80000000;
                buf->WrCursor->EndAddr = buf->WrCursor->StartAddr;
                cursor->EndSpace = buf->WrCursor->StartAddr - cursor->EndAddr - 1;
                buf->BufCnt += (frame->size + cursor->EndSpace);
            }else {
                buf->WrCursor->StartAddr = buf->StartAddr;
                buf->WrCursor->EndAddr = buf->StartAddr;
                cursor->EndSpace = buf->EndAddr - cursor->EndAddr;
                buf->BufCnt += (frame->size + cursor->EndSpace);
            }
        }else {    //can't save in the end
            if((buf->StartAddr + frame->size - 1) >= buf->RdCursor->StartAddr)  //can't save at the beginning
            {
#ifdef USE_CRITICAL
                OS_EXIT_CRITICAL();
#endif
                return 3;
            }
            //can saved at the beginning
            EndSpace = buf->EndAddr - cursor->StartAddr + 1;   //must +1
            cursor->StartAddr = buf->StartAddr;
            cursor->EndAddr = buf->StartAddr + frame->size - 1;
            
            buf->WrCursor = cursor->nxt;
            buf->WrCursor->StartAddr = (cursor->EndAddr + 32) & 0x1ffffff0 |0x80000000;  //16Byte aligned
            buf->WrCursor->EndAddr = buf->WrCursor->StartAddr;

            cursor->EndSpace = buf->WrCursor->StartAddr - cursor->EndAddr - 1;  //16~31Bytes
            if(buf->CurCnt != 0) {
                buf->BufCnt += (frame->size + EndSpace + cursor->EndSpace);
                cursor->pre->EndSpace += EndSpace;
            }else {
                buf->BufCnt += (frame->size + cursor->EndSpace);
            }
        }
    }
    cursor->PkgSize = frame->size;
    cursor->TimeStamp = frame->TimeStamp;
    buf->CurCnt++;
#ifdef USE_CRITICAL
    OS_EXIT_CRITICAL();
#endif
    if(buf->CurCnt > buf->CursorNumber)
        printf("cursor exceeds(%d)\n", buf->CurCnt);
    if(buf->BufCnt > buf->BufSize)
        printf("buf size exceeds(%d)\n", buf->BufSize);

#else    //end of NEW_JITBUF_ALGORITHM
    cursor = buf->WrCursor;
    
    if((cursor->StartAddr + frame->size - 1) > buf->EndAddr)
    {
        EndSpace = buf->EndAddr - cursor->StartAddr;
        if((buf->StartAddr + frame->size - 1) >= buf->RdCursor->StartAddr)
        {
#ifdef USE_CRITICAL
            OS_EXIT_CRITICAL();
#endif
            return 3;
        }
        
        cursor->StartAddr = buf->StartAddr;
        cursor->EndAddr = buf->StartAddr + frame->size - 1;        // ? should be word align?
        if(buf->CurCnt != 0) {
            buf->BufCnt += (frame->size + EndSpace);
            cursor->pre->EndSpace = EndSpace;
        }else
            buf->BufCnt += frame->size;
    }else {
#if 0
        if((buf->BufCnt + frame->size - 1) >= buf->BufSize)
        {
#ifdef USE_CRITICAL
            OS_EXIT_CRITICAL();
#endif
            return 4;
        }
#endif
        buf->BufCnt +=frame->size;
        cursor->EndAddr = cursor->StartAddr + frame->size - 1;  // ? should be word align?
    }

    cursor->PkgSize = frame->size;
    cursor->TimeStamp = frame->TimeStamp;
    buf->CurCnt++;
    buf->WrCursor = cursor->nxt;
    buf->WrCursor->StartAddr = cursor->EndAddr + 1;
    buf->WrCursor->EndAddr = cursor->EndAddr + 1;
//    frame->CurrentDepth = buf->CurCnt;
//    frame->CurrentSpace = buf->CursorNumber - buf->CurCnt;
#ifdef USE_CRITICAL
    OS_EXIT_CRITICAL();
#endif

#endif

    //start to copy!;
    cursor->StartAddr = cursor->StartAddr & 0x1fffffff | 0x80000000;
    frame->addr = frame->addr & 0x1fffffff | 0x80000000;
    cache_writeback_invalidate_D((void *)frame->addr, frame->size);
    if(gChecksumEnabled == TRUE) {
        ret = MyMemCpy_Checksum(cursor->StartAddr, frame->addr, cursor->PkgSize);
        if(ret != 0)
            cursor->dropflag = TRUE;
    }else {
        memcpy((void *)cursor->StartAddr, (void *)frame->addr, cursor->PkgSize);
    }
    cache_writeback_D((void *)cursor->StartAddr, frame->size);

    return 0;
}

unsigned int VBufGetCurFrameSize(VIDEO_BUFFER *buf)
{
    return buf->RdCursor->PkgSize;
}

unsigned int VBufGetCurTimeStamp(VIDEO_BUFFER *buf)
{
    return buf->RdCursor->TimeStamp;
}

void VBufGetDepth(VIDEO_BUFFER *buf, BUFFER_STATE *state)
{
#ifdef USE_CRITICAL
    OS_CPU_SR  cpu_sr;
    OS_ENTER_CRITICAL();
#endif
    state->BufSize = buf->BufCnt;
    state->CurSize = buf->CurCnt;
#ifdef USE_CRITICAL
    OS_EXIT_CRITICAL();
#endif
}
void VBufGetSpace(VIDEO_BUFFER *buf, BUFFER_STATE *state)
{
#ifdef USE_CRITICAL
    OS_CPU_SR  cpu_sr;
    OS_ENTER_CRITICAL();
#endif
    state->BufSize = buf->BufSize - buf->BufCnt;
    state->CurSize = buf->CursorNumber - buf->CurCnt;
#ifdef USE_CRITICAL
    OS_EXIT_CRITICAL();
#endif
}

void VideoBufferReset(VIDEO_BUFFER *buf)
{
    VIDEO_CURSOR *cursor;
    int i;
    buf->BufCnt = 0;
    buf->CurCnt = 0;
    buf->RdCursor = buf->cursor;
    buf->WrCursor = buf->cursor;

    cursor = buf->cursor;
    
    cursor[0].nxt = &cursor[1];       //make cursor link.
    cursor[0].pre = &cursor[buf->CursorNumber - 1];
    ClearCursor(&cursor[0]);
    
    for(i = 1; i < buf->CursorNumber - 1; i++)
    {
        cursor[i].nxt = &cursor[i + 1];       //make cursor link.
        cursor[i].pre = &cursor[i - 1];
        ClearCursor(&cursor[i]);
    }
    
    cursor[i].nxt = &cursor[0];
    cursor[i].pre = &cursor[i - 1];
    ClearCursor(&cursor[i]);
    
    cursor[0].StartAddr = buf->StartAddr;
    cursor[0].EndAddr = buf->StartAddr;
}

void DumpVideoBufferStruct(VIDEO_BUFFER *buf)
{
    VIDEO_CURSOR *cursor;
    printf("============buffer context=========\n");
    printf("buf.buffer          0x%8.8x\n", (unsigned int)buf->buffer);
    printf("buf.cursor          0x%8.8x\n", (unsigned int)buf->cursor);
    printf("buf.EndAddr       0x%8.8x\n", (unsigned int)buf->EndAddr);
    printf("buf.StartAddr       0x%8.8x\n", (unsigned int)buf->StartAddr);
    printf("buf.BufSize         %d\n", buf->BufSize);
    printf("buf.CursorNumber    %d\n", buf->CursorNumber);
    printf("buf.BufCnt          %d\n", buf->BufCnt);
    printf("buf.CurCnt          %d\n", buf->CurCnt);
    printf("buf.RdCursor        0x%8.8x\n", (unsigned int)buf->RdCursor);
    printf("buf.WrCursor        0x%8.8x\n", (unsigned int)buf->WrCursor);
    
    cursor = buf->WrCursor;
    printf("========write cursor context========\n");
    printf("cursor.nxt          0x%8.8x\n", (unsigned int)cursor->nxt);
    printf("cursor.pre          0x%8.8x\n", (unsigned int)cursor->pre);
    printf("cursor.StartAddr    0x%8.8x\n", (unsigned int)cursor->StartAddr);
    printf("cursor.EndAddr      0x%8.8x\n", (unsigned int)cursor->EndAddr);
    printf("cursor.PkgSize      %d\n", cursor->PkgSize);
    printf("cursor.TimeStamp    %d\n", cursor->TimeStamp);
    printf("cursor.EndSpace     %d\n", cursor->EndSpace);
    
    cursor = buf->RdCursor;
    printf("========read cursor context========\n");
    printf("cursor.nxt          0x%8.8x\n", (unsigned int)cursor->nxt);
    printf("cursor.pre          0x%8.8x\n", (unsigned int)cursor->pre);
    printf("cursor.StartAddr    0x%8.8x\n", (unsigned int)cursor->StartAddr);
    printf("cursor.EndAddr      0x%8.8x\n", (unsigned int)cursor->EndAddr);
    printf("cursor.PkgSize      %d\n", cursor->PkgSize);
    printf("cursor.TimeStamp    %d\n", cursor->TimeStamp);
    printf("cursor.EndSpace     %d\n", cursor->EndSpace);
}

#if 0
#define TEST_BUFFER_SIZE 1024*64
int main(void)
{
    VIDEO_BUFFER buffer1;
    BUFFER_STATE state1;
    VIDEO_FRAME frame;
    unsigned char buf_w[TEST_BUFFER_SIZE];
    unsigned char buf_r[TEST_BUFFER_SIZE];
    int i;
    int error;
    
    for(i = 0; i < TEST_BUFFER_SIZE; i++)
        buf_w[i] = (unsigned char)(i & 0xff);
    for(i = 0; i < TEST_BUFFER_SIZE; i++)
        buf_r[i] = 0;
    
    VBufInit(&buffer1);
    VBufGetSpace(&buffer1, &state1);
    printf("space = %d, %d\n", state1.BufSize, state1.CurSize);
    VBufGetDepth(&buffer1, &state1);
    printf("depth = %d, %d\n", state1.BufSize, state1.CurSize);
    for(i = 0; i < 70; i++)
    {
        frame.addr = &buf_w[i];
        frame.size = TEST_BUFFER_SIZE;
        frame.TimeStamp = i;
        error = VBufSaveOneFrame(&buffer1, &frame);
        if(error)
            printf("save error! error = %d\n", error);
        //DumpVideoBufferStruct(&buffer1);
    }
    VBufGetSpace(&buffer1, &state1);
    printf("space = %d, %d\n", state1.BufSize, state1.CurSize);
    VBufGetDepth(&buffer1, &state1);
    printf("depth = %d, %d\n", state1.BufSize, state1.CurSize);
    for(i = 0; i < 70; i++)
    {
        error = VBufGetOneFrame(&buffer1, &frame);
        if(error)
            printf("get error! error = %d\n", error);
        else
        {
            printf("size:%d, ts:%d, d0:%d, d1:%d\n", frame.size, frame.TimeStamp, frame.addr[0], frame.addr[1]);
            ReleaseCursor(&buffer1, frame.addr);
        }
            
        //DumpVideoBufferStruct(&buffer1);
    }
    return 1;
}
#endif
#endif
