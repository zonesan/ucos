
#ifndef _MOTION_DETECTION_H
#define _MOTION_DETECTION_H

#ifdef MOTION_DETECTION_GLOBALS
#define MOTION_DETECTION_EXT
#else
#define MOTION_DETECTION_EXT extern
#endif

#define SEND_MD_TO_LINUX      //if run ipcam,define it. if ucos loopback, undefine it
//#define DEBUG_MD                   //if want to debug motion detect, define it
//#define MD_UCOS_LOOPBACK     //if ucos loopback, define it to see the motion range

enum Motion_Detection_Type  {mdframesize, mdmotionvector, mdtemporalDif};
enum Motion_Detection_Type gMdType;           //now, there are 3 types: motion vectors, framesize, temporal difference

MOTION_DETECTION_EXT int gMotionSensi;	      //sensitivity of motion detect, range from 1 to 100.
MOTION_DETECTION_EXT int gIsMDEnabled;	     //TRUE:motion detect enabled, FALSE:motion detect disabled

typedef struct _MDFrameRateCtrl
{
    unsigned char framerate_motion;
    unsigned char framerate_nomotion;
    unsigned short motionduration;
}tMDFrameRateCtrl;

/*************************************************************************************************
*                                                               MOTION DETECTION TYPE = motion vectors
**************************************************************************************************/


/*************************************************************************************************
*                                                               MOTION DETECTION TYPE = frame size
**************************************************************************************************/
#ifdef SUPPORT_MD_FRAMESIZE

#define MODE_CBR 1
#define MODE_VBR 2

#define MAX_Q_VALUE                      32            //Q has 32 different values
#define MAX_SENSITIVITY_LEVEL    10            //
#define INTERVAL                             5             //we will get the min P frame size in this time interval
#define TOTALVMODE                       13           //munber of video format

#define MAXQDIF                             5            //in CBR mode, when the Q difference exceed, motion detected
#define MINQDIF                             2            //in CBR mode, when the Q difference littler than this value, no motion

typedef struct _MD_Info{
    unsigned int IFrameSensi;                  //I frame sensivity level, there are 10 levels, calculated from gMotionSensi
    unsigned int PFrameSensi;                 //P frame sensivity level, there are 10 levels, calculated from gMotionSensi
    unsigned int QSensi;                           //in CBR mode, when Q difference exceed, a motion occured, calculated from gMotionSensi
    unsigned int pframe_outrange_num;

    unsigned int avePsize[2];                       //in VBR mode, average p frame size of last second
    unsigned int lastIsize;                       //in VBR mode
    unsigned int encodeQ;                       //in VBR mode, current Q value

    unsigned int maxQ;                           //in CBR mode, max Q value
    unsigned int minQ;                            //in CBR mode, min Q value
    unsigned int maxQindex;                  //in CBR mode, max Q value index
    unsigned int minQindex;

    int initflag;                                       //in VBR mode, is parameters initialized
}tMDInfo;

#endif
/*************************************************************************************************
*                                                               MOTION DETECTION TYPE = temporal difference
**************************************************************************************************/
#define STARTTIME 3

#define BLOCK_WIDTH  32
#define BLOCK_HEIGHT 24
#define MAX_IMAGE_WIDTH 704
#define MAX_IMAGE_HEIGHT 576
#define MAX_HORI_BLOCK (MAX_IMAGE_WIDTH / BLOCK_WIDTH)
#define MAX_VERT_BLOCK (MAX_IMAGE_HEIGHT / BLOCK_HEIGHT)
#define MAX_VERT_CELL ((MAX_VERT_BLOCK + 1) / 2)           //if odd, then last block will be seen as a cell
#define MAX_HORI_CELL ((MAX_HORI_BLOCK + 1) / 2)           //if odd, then last block will be seen as a cell

#define LVMIN 4000         //Y level difference threshold

#define MD_TASK_STACK_SIZE 1000
MOTION_DETECTION_EXT OS_STK StkMDStart[MD_TASK_STACK_SIZE];
MOTION_DETECTION_EXT OS_STK StkMDProcess[MD_TASK_STACK_SIZE];
MOTION_DETECTION_EXT OS_EVENT *gpSemPpeCompareYFinished;
MOTION_DETECTION_EXT OS_EVENT *gpMboxPpeNewVinFrame;
MOTION_DETECTION_EXT unsigned int gpVinFrameAddr;

MOTION_DETECTION_EXT unsigned int gpAreaVectorAddr;       //the first addr that linux to ucos, setting which region is masked from motion detection

typedef struct _MaskRange{
    int startx;
    int starty;
    int endx;
    int endy;
}tMaskRange;

MOTION_DETECTION_EXT char gMdBlockMask[MAX_VERT_BLOCK][MAX_HORI_BLOCK];       //if mask a block, modtion detection will not use it
MOTION_DETECTION_EXT char gMdBlockStatus[MAX_VERT_BLOCK][MAX_HORI_BLOCK];     //the status of each block
MOTION_DETECTION_EXT char gMdCellMask[MAX_VERT_CELL][MAX_HORI_CELL];                //if mask a cell, motion detection will not use it
MOTION_DETECTION_EXT char gMdCellStatus[MAX_VERT_CELL][MAX_HORI_CELL];              //the status of each cell
MOTION_DETECTION_EXT unsigned int gMdLvsens;    //lum level sensitivity. there are 10 levels
MOTION_DETECTION_EXT unsigned int gMdSpsens;    //spacial sensitivity.1~64
MOTION_DETECTION_EXT unsigned int gMdCelsens;   //cell  sensitivity.1~4
MOTION_DETECTION_EXT unsigned int gMdTmpsens; //temporal sensitivity. 1: 1 frame; 2: two frames; 3: three frames
MOTION_DETECTION_EXT unsigned int gMdSpeed;      //velocity control. 1: each frame; 2: every two frames; 3: every three frames

MOTION_DETECTION_EXT unsigned int gMDVinFirstFrame; //

extern unsigned int gVinSoftwareFrameRate;
extern int gMDPlayTime;
extern tMDFrameRateCtrl gtMDFrameRateCtrl;
extern int gMDStartDelay;
/***************************************************************************************************
*                                                                     Globe functions
***************************************************************************************************/
void MDInit(int sensi);
void MDTemporalDif(int *data);
void SetMDMask(int startx, int starty, int endx, int endy);//if want to prevent a range from MD, set the startpoint and end point.
void TaskMotionDetection(void *pdata);
void TaskMDProcess(void *pdata);
void MDTemporalDifInit(int sensi);
void	Motion_Detection(int framelen);

#endif
