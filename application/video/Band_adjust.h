/*
*********************************************************************************************************
*                             Params for Q setting
*********************************************************************************************************
*/
#ifndef BAND_ADJUST_H
#define BAND_ADJUST_H

#ifdef    Band_adjust_GLOBALS
#define  BANDWIDTH_EXT
#else
#define  BANDWIDTH_EXT  extern
#endif

#define NEW_CBR_ALGORITHM
//#define DEBUG_CBR             //define it to debug cbr, all information will be printed

#define IP_HEAD 4000         //there are about 4000 bytes of IP head data every second ,85 x 50 bytes.

#define MAX_Q 25
#define MIN_Q 5
#define DefaultProportion 5
#define DEFAULT_BANDWIDTH 384
#define MPEG_ENCODE_MAX_THROW_FRAME 10

#define Percent_10   10
#define Percent_50   50
#define Percent_150 150

typedef struct _CBR
{
    /* variabes set each time cbr initialized */
    unsigned int bw;                            // kbits
    unsigned int videobw;                   //kbits
    unsigned int audiobw;                   //kbits
    unsigned int ip_head;                   //kbits
    unsigned int cbr_enabled;
    unsigned int max_qp;
    unsigned int min_qp;
    /* to save the actual framerate, updated every second */
    unsigned int framerate;
    unsigned int estimatePsize;
    unsigned int estimateIsize;
    unsigned int ipproportion;
    unsigned int maxskipframe;
    unsigned int frametoskip;
    /* to count the actual frame rate, not fields rate */
    unsigned int framenum;             // VIN frame num
    /* qp controls */
    unsigned int qpstep;
    unsigned int currentqp;
    /* variables for calculation */
    unsigned int pframenum;             //P frame num counter
    unsigned int iframenum;              //I frame num counter
    unsigned int pframesize;             //P frame size counter
    unsigned int iframesize;              //I frame size counter
    unsigned int allocatedbw;           //
    unsigned int usedbw;
    /* variables for debug purperce */
#ifdef DEBUG_CBR
    unsigned int staticInt[5];
    unsigned int total_Q_Ps;
    unsigned int cbrframenum;
    unsigned int cbrallframesize;
    unsigned int cbraveframesize;
    unsigned int invalideFm_Ps;
    unsigned int calmaxskip;
    unsigned int actualmaxskip;
    unsigned int duration;                     //second, MUST >0
#endif
}cbr_info;

BANDWIDTH_EXT cbr_info gtcbrinfo;

int set_qp_range(int maxq, int minq );
void Updatecbr(void);
void QAutoAdjust(unsigned int framesize, unsigned int pframe);
void cbr_init(unsigned int bw);
void set_cbr_default(void);

#endif
