#include "video_cfg.h"
#ifdef MOTION_DETECTION
#include "2risc_memory_map.h"
#include "chip_cfg.h"
#include "includes.h"

#include  "ppe.h"
#include "vb1000.h"
#include "comm.h"
#include "risc.h"
#include "VIN_SET.h"

#include "Motion_Detection.h"

int Data[4];                         //used to save the address of Y component of two frames ,and frame width, frame height

/*
***************************************************************************
*                        PpeISR()
***************************************************************************
*/
void PpeISR(void)
{
    unsigned int regs;

    OSSemPost(gpSemPpeCompareYFinished);

    regs = rdl(PPE_REGS_ADDR(PPE_CTL));    //clear intr
    wrl(regs, PPE_REGS_ADDR(PPE_CTL));
}
/*
***************************************************************************
*                   HwPpeIntrInit()
*
***************************************************************************
*/
void HwPpeIntrInit(void)
{
    unsigned int isr_base_addr;
    static unsigned int intropened = 0;

    if(intropened == 1)
        return;
    intropened = 1;

    if(RiscRdID() == RISC0)
        isr_base_addr = RISC0_FOURTH_VECTOR_B_ADDR | 0xa0000000;
    else
        isr_base_addr = RISC1_FOURTH_VECTOR_B_ADDR | 0xa0000000;

    *(unsigned int *)(isr_base_addr + 0x90) = (unsigned int)PpeISR;

    RiscIntrEn(PPE_INTR);
}
/*
***************************************************************************
*                                    HwPpeInitY()
* Argument:
* return:
* note:
***************************************************************************
*/
void HwPpeInitY(void)
{
    int i,len = 296;
    static int ppe_started = FALSE;
    volatile unsigned char *SpramPtr = (volatile unsigned char*)0xbfa02000;
    unsigned short buf[] = {
                                        0x1308,0x1209,0x1106,0x1008,0x0D18,0x44C3,0x0808,0x1820,
                                        0x1485,0x2600,0x1483,0x25D4,0x0481,0x2400,0x0600,0x131F,
                                        0x12A0,0x1130,0x1000,0x08A0,0x1638,0x0865,0x0966,0x0987,
                                        0x0650,0x4859,0x4FFF,0x784B,0x4859,0x4FFF,0x1800,0x1020,
                                        0x7840,0x1800,0x1020,0x082B,0x2400,0x2455,0x246A,0x247F,
                                        0x0960,0x19A0,0x1668,0x4857,0x4FFF,0x7020,0x080D,0x2400,
                                        0x1480,0x2454,0x2401,0x2454,0x2402,0x2454,0x2403,0x2454,
                                        0x09A0,0x3133,0x50F1,0x4859,0x4FFF,0x784C,0x4859,0x4FFF,
                                        0x1800,0x1020,0x7840,0x1800,0x1020,0x082C,0x2400,0x2455,
                                        0x246A,0x247F,0x0980,0x19C0,0x1668,0x4857,0x4FFF,0x7020,
                                        0x080E,0x2400,0x1480,0x2454,0x2401,0x2454,0x2402,0x2454,
                                        0x2403,0x2454,0x09C0,0x3133,0x50F1,0x080D,0x082E,0x2480,
                                        0x24D5,0x24EA,0x24FF,0x0D0B,0x2440,0x2455,0x246A,0x247F,
                                        0x0F60,0x3283,0x3132,0x50AE,0x3329,0x4C1B,0x0806,0x0828,
                                        0x2400,0x2455,0x246A,0x247F,0x08C0,0x0807,0x0828,0x2400,
                                        0x2455,0x246A,0x247F,0x08E0,0x3131,0x5098,0x0410,0x1820,
                                        0x1482,0x25C0,0x0480,0x0805,0x2400,0x2455,0x08A0,0x5B8D,
                                        0x6800,0x5801,0x1308,0x1209,0x1106,0x1044,0x08C0,0x1800,
                                        0x1380,0x0FC0,0x0000,0x0000
                                    };
        if(ppe_started == FALSE)
            ppe_started = TRUE;
        else
            return;

	printf("Init PPE for motion detection\n");
	memcpy(SpramPtr, buf, len);

	SpramPtr = (volatile unsigned char*)0xbfa03000;
	memset(SpramPtr, 0x0, 0x1000);

	wrb(0x1,PPE_REGS_ADDR(PPE_CACHE_INVLD));
	wrb(0x2,PPE_REGS_ADDR(PPE_CACHE_INVLD));
	for(i=0;i<PPE_ITAG_NUM;i++)
		wrl(0x10000,PPE_REGS_ADDR(PPE_ITAG + i*4));
	wrl(0x0,PPE_REGS_ADDR(PPE_STATUS));
	wrl(0x0,PPE_REGS_ADDR(PPE_QID));
	wrl(0x83c0aa01,PPE_REGS_ADDR(PPE_CTL));
	wrl(0x80000000,PPE_REGS_ADDR(PPE_DEBUG));
	
}

/*
********************************************************************
*                           HwPpeCompareY()
*
*Argument: y1_addr: the addr of Y component of one frame(field)
*                 y2_addr: the addr of Y component of other frame(field)
*                width: the width of the frame(field)
*                height: the height of the frame(field)
*
*return: null
*
*note: compare the Y component of two frames(fields), then partition the frame(field)
*         into blocks, get the Y difference of each block
*
********************************************************************
*/
void HwPpeCompareY(int y1_addr, int y2_addr, int width, int height)
{
    unsigned int regs;
    int pc = 0;
    int *plwData = (unsigned int *)((unsigned int)Data | 0xa0000000);

    if(((width == 704) && ((height == 576) ||(height == 288)))||
        ((width == 704) && ((height == 480) ||(height == 240)))||
        ((width == 640) && ((height == 480) ||(height == 240)))||
        ((width == 352) && ((height == 288) ||(height == 144)))||
        ((width == 320) && ((height == 240) ||(height == 120)))||
        ((width == 176) && ((height == 144) ||(height == 72)))||
        ((width == 160) && ((height == 120) ||(height == 60))))
    {
        plwData[0] = y1_addr;
        plwData[1] = y2_addr;
        plwData[2] = width;
        plwData[3] = height;

        wrl(plwData,PPE_REGS_ADDR(PPE_MAC_SA_LO));

        regs = rdl(PPE_REGS_ADDR(PPE_CTL));    //reset PPE CPU
        regs &= ~0x2;
        wrl(regs,PPE_REGS_ADDR(PPE_CTL));
	
        wrl(pc,PPE_REGS_ADDR(PPE_PC));       //set the initial address
        while( rdl(PPE_REGS_ADDR(PPE_DEBUG)) & 0x80000000)   //see if program is stopped
            wrl(0x0,PPE_REGS_ADDR(PPE_DEBUG));            //set CPU continue to run
	
        regs = rdl(PPE_REGS_ADDR(PPE_CTL));
        regs |= 0x2;
        wrl(regs,PPE_REGS_ADDR(PPE_CTL));	//enable PPE CPU	

    }
    else 
    {
        printf("YUV len error,width=%d,height=%d\n",width,height);
    }

}
/*
******************************************************************
*                                      HwPpeWaitEnd()
* Argument:
* return:
* note: 
*
******************************************************************
*/
#if 0
void HwPpeWaitEnd(void)
{
    unsigned int regs;
    volatile int *SpramPtr = (volatile int*)0xbfa03000;
    unsigned int blocknum = 0;
    unsigned int i;

#ifdef DEBUG_MD
    unsigned int total = 0; 
    int maxcellvalue = 0;
    int avecellvalue = 0;
    int value;
#endif

    blocknum = gImageWidth / 32 * gImageHeight / 24;

    regs = rRISC_INT_STATUS;
    while(1) {
        if(regs & (0x1<<9))  
            break;
        if( rdl(PPE_REGS_ADDR(PPE_DEBUG)) & 0x80000000 )   //see if program is stopped
            break;
    }

    regs = rdl(PPE_REGS_ADDR(PPE_CTL));    //
    wrl(regs, PPE_REGS_ADDR(PPE_CTL));

//    printf("PPE compare Y finished.\n");

#ifdef DEBUG_MD
    for(i=0; i<blocknum; i++)
    {
#if 0
        printf("%d,", SpramPtr[i]);
        if((i+1) % 20 == 0)
            printf("\n");
#endif

        if(SpramPtr[i] < 0) 
            total -= SpramPtr[i];
        else 
            total += SpramPtr[i];
        value = SpramPtr[i];
        if(value < 0) value = -value;
        if(value > maxcellvalue)
            maxcellvalue = value;
    }
#endif

#ifdef DEBUG_MD
    avecellvalue = total / blocknum;
    printf("totalblock=%d, average=%d, maxvalue=%d\n", blocknum, avecellvalue, maxcellvalue);
#endif

    MDTemporalDif(SpramPtr);             //process motion detection

    memset(SpramPtr, 0x0, 0x1000);    //clear Y difference value

#ifdef DEBUG_PPE_PERFORMANCE
    end = GetCount();
    if(end > start)
         end = end - start;
     else
         end = CPU_MAIN_CLOCK / (2 * OS_TICKS_PER_SEC) + end - start; 
    printf("<ppe=%d>\n", end / (CPU_MAIN_CLOCK/(2*1000000)));    // counted by microsecond
#endif
}
#endif

#endif


