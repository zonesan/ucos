#ifndef W681388REG_H
#define W681388REG_H
// SPI Config
// RUN
#define SIF_MODE_RUN    (0x1 << 31)
//MODE
#define SIF_MODE_BOOT   (0x0 << 29)
#define SIF_MODE_CBUS   (0x1 << 29)
#define SIF_MODE_DBUS   (0x2 << 29)
//CS shift
#define SIF_MODE_CS_MASK    0x7
#define SIF_MODE_CS_SHIFT   26
//ctl
#define SIF_MODE_CTL_0BIT   (0x0 << 24)
#define SIF_MODE_CTL_8BIT   (0x1 << 24)
#define SIF_MODE_CTL_16BIT   (0x2 << 24)
//ADDRESS

//rw
#define SIF_MODE_READ      (0x0 << 21)
#define SIF_MODE_WRITE     (0x1 << 21)


//SPI timing
#define SIF_TIMING_PSI      (0x1 << 31)
#define SIF_TIMING_NSI      (0x0 << 31)
#define SIF_TIMING_PSO      (0x1 << 30)
#define SIF_TIMING_NSO      (0x0 << 30)

#define SIF_TIMING_CLK_PIDEL    (0x1 << 29)
#define SIF_TIMING_CLK_NIDEL    (0x0 << 29)

#define SIF_TIMING_CS_D0    (0x1 << 26)
#define SIF_TIMING_CS_D1    (0x1 << 26)
#define SIF_TIMING_CS_D2    (0x2 << 26)
#define SIF_TIMING_CS_D3    (0x3 << 26)

#define SIF_TIMING_CS_O0    (0x1 << 22)
#define SIF_TIMING_CS_O1    (0x1 << 22)
#define SIF_TIMING_CS_O2    (0x2 << 22)
#define SIF_TIMING_CS_O3    (0x3 << 22)

//w681388 Register Values
enum BLK_PCM		 //PCM
{
    W681388REG_PCMC = 0,
    W681388REG_TTL,		   	//TX
    W681388REG_RTL,			//RX
    W681388REG_TCH,
    W681388REG_PLLS,
    W681388REG_PCMFS,
    W681388REG_DVID,
    W681388REG_DSVID
};

enum BLK_FSK       //FSK
{
    W681388REG_FSKC = 0x10,
    W681388REG_FSKTD,
    W681388REG_FSKS,
    W681388REG_FSKLCR,
    W681388REG_FSKT
};

enum BLK_SYS         //SYS
{
    W681388REG_PHF = 0x20,
    W681388REG_LCX,
    W681388REG_PON,
    W681388REG_RESERVED1,
    W681388REG_RESERVED2,
    W681388REG_INTV,
    W681388REG_INT1,
    W681388REG_IE1,
    W681388REG_INT2,
    W681388REG_IE2,
    W681388REG_INT3,
    W681388REG_IE3
};



enum BLK_MONITOR         //MONITOR
{
    W681388REG_MONITOR1 = 0x30,
    W681388REG_MONITOR2,
    W681388REG_MONITOR5,
    W681388REG_MONITOR7,
    W681388REG_MONITOR9,
    W681388REG_MONITOR11,
    W681388REG_MONITOR13,
    W681388REG_MONITOR15
};

enum BLK_LINE         //LINE
{
    W681388REG_APG = 0x40,
    W681388REG_HB,
    W681388REG_VCMR,
    W681388REG_LAMC,
    W681388REG_LS,              //Shadow Linefeed Status | Programmed Linefeed Status
    W681388REG_LCL,
    W681388REG_RTLC,
    W681388REG_LCDB,
    W681388REG_RTDB,
    W681388REG_PWMT,
    W681388REG_DDCC,
    W681388REG_DDCPW,
    W681388REG_OHV,
    W681388REG_CMV,
    W681388REG_VBHV,
    W681388REG_VBLV,
    W681388REG_LCDCL,
    W681388REG_RTDCL,
    W681388REG_DCH,
    W681388REG_LCT,
    W681388REG_LCTHY,
    W681388REG_RTT,
    W681388REG_VOV
};


enum BLK_DTMF           //DTMF
{
    W681388REG_DTMFC = 0x60,
    W681388REG_DTMFTL,
    W681388REG_DTMFTH,
    W681388REG_DTMFPT,
    W681388REG_DTMFAT,
    W681388REG_DTMFDT,
    W681388REG_DTMFD,
    W681388REG_ETC,
    W681388REG_DTMFTEST,
    W681388REG_RESERVED3,
    W681388REG_TIPDAC,
    W681388REG_RINGDAC,
    W681388REG_METERDAC,
    W681388REG_DAPCMINL,
    W681388REG_DAPCMINH,
    W681388REG_ADPCMOUTL,
    W681388REG_ADPCMOUTH,
    W681388REG_DCTR
};

enum BLK_BAT
{
    W681388REG_BATV = 0x80,     //0x80 Battery Voltage Sense
    W681388REG_TIPV,            //0x81 TIP Voltage Sense
    W681388REG_RINGV,           //0x82 RING Voltage Sense
    W681388REG_QT3V,            //0x83 Transistor QT3 Emitter Voltage Sense
    W681388REG_QR3V,            //0x84 Transistor QR3 Emitter Voltage Sense
    W681388REG_QT3I,            //0x85 Transistor QT3 Current Sense
    W681388REG_QR3I,            //0x86 Transistor QR3 Current Sense
    W681388REG_QT1I,            //0x87 Transistor QT1 Current Sense
    W681388REG_QT2I,            //0x88 Transistor QT2 Current Sense
    W681388REG_QR1I,            //0x89 Transistor QR1 Current Sense
    W681388REG_QR2I,            //0x8A Transistor QR2 Current Sense
    W681388REG_TEMP,            //0x8B temperature inside the package
    W681388REG_VBGAP,           //0x8C
    W681388REG_LV,              //0x8D Loop Voltage Sense
    W681388REG_TIPI,            //0x8E TIP Current Sense
    W681388REG_RINGI,           //0x8F RING Current Sense
    W681388REG_LI,              //0x90 Loop Current Sense
    W681388REG_SPOL             //0x91
};

enum BLK_CAL
{
    W681388REG_PALPQ2 = 0xA0,
    W681388REG_PALPQ1,
    W681388REG_PALPQ3,
    W681388REG_PALPQH1,
    W681388REG_PALPQH2,
    W681388REG_PATHQ2,
    W681388REG_PATHQ1,
    W681388REG_PATHQ3,
    W681388REG_IM1,
    W681388REG_IM2,
    W681388REG_TATH,
    W681388REG_LCMCNT,
    W681388REG_CC,
    W681388REG_OS2RPD,
    W681388REG_RESERVED4,
    W681388REG_CAL1,
    W681388REG_CAL2,
    W681388REG_CAL3,
    W681388REG_TROFFS1,
    W681388REG_TROFFS2,
    W681388REG_TROFFS3,
    W681388REG_PWCT
};

enum BLK_OSC
{
    W681388REG_OSNC = 0xC0,
    W681388REG_RMPC,            //0xC1
    W681388REG_OS1ICL,            //0xC2 Initial conditions
    W681388REG_OS1ICH,          //0xC3
    W681388REG_OS2ICL,          //0xC4
    W681388REG_OS2ICH,          //0xC5
    W681388REG_OS1CL,           //0xC6 Coefficients
    W681388REG_OS1CH,           //0xC7
    W681388REG_OS2CL,           //0xC8
    W681388REG_OS2CH,           //0xC9
    W681388REG_OS1ATL,          //0xCA Active timers
    W681388REG_OS1ATH,          //0xCB
    W681388REG_OS2ATL,          //0xCC
    W681388REG_OS2ATH,          //0xCD
    W681388REG_OS1ITL,          //0xCE Inactive timers
    W681388REG_OS1ITH,          //0xCF
    W681388REG_OS2ITL,          //0xD0
    W681388REG_OS2ITH,          //0xD1
    W681388REG_MPATL,           //0xD2 Metering
    W681388REG_MPATH,           //0xD3
    W681388REG_MPITL,           //0xD4
    W681388REG_MPITH,           //0xD5
    W681388REG_MPICL,           //0xD6
    W681388REG_MPICH,           //0xD7
    W681388REG_MPCL,            //0xD8
    W681388REG_MPCH,            //0xD9
    W681388REG_MPADS,           //0xDA
    W681388REG_MPMX,            //0xDB
    W681388REG_ROFFS,           //0xDC
    W681388REG_ADCL,            //0xDD
    W681388REG_DACL,            //0xDE
    W681388REG_DGH,             //0xDF
    W681388REG_TM_OUT_L,        //0xE0
    W681388REG_TM_DIG,          //0xE1
    W681388REG_TM_DIGH          //0xE2
};

#define PCMC_ALOW   (0x0 << 6)
#define PCMC_ULOW   (0x1 << 6)
#define PCMC_LINEAR (0x2 << 6)
#define PCMC_16BIT  (0x1 << 5)
#define PCMC_GCI    (0x1 << 4)
#define PCMC_TRI_N  (0x1 << 2)
#define PCMC_PCM_EN (0x1)

#define PCMC_SET(set, channel)       w681388Write(set, W681388REG_PCMC, channel)

#define PCMFS_BCLK256k      (0)
#define PCMFS_BCLK512k      (0x1 << 4)
#define PCMFS_BCLK768k      (0x2 << 4)
#define PCMFS_BCLK1000k     (0x3 << 4)
#define PCMFS_BCLK1024k     (0x4 << 4)
#define PCMFS_BCLK1536k     (0x5 << 4)
#define PCMFS_BCLK2000k     (0x6 << 4)
#define PCMFS_BCLK2048k     (0x7 << 4)
#define PCMFS_BCLK4000k     (0x8 << 4)
#define PCMFS_BCLK4096k     (0x9 << 4)
#define PCMFS_BCLK8000k     (0xa << 4)
#define PCMFS_BCLK8192k     (0xb << 4)

#define PCMFS_LFS           (0x1 << 3)
#define PCMFS_INTERNAL_FS   (0x1 << 2)
#define PCMFS_RESET         (0x1 << 1)


#define PHF_HPR     (0x1 << 1)
#define PHF_HPT     (0x1)
#define SET_PHF(phf, channel)       w681388Write(phf, W681388REG_PHF, channel)
#define GET_PHF(channel)            w681388Read(W681388REG_PHF, channel)

#define LC_ALP2     (0x1 << 4)
#define LC_ALP1     (0x1 << 3)
#define LC_DLP3     (0x1 << 2)
#define LC_DLP2     (0x1 << 1)
#define LC_DLP1     (0x1)
#define SET_LOOPBACK(lc, channel)   w681388Write(lc, W681388REG_LCX, channel)
#define GET_LOOPBACK(channle)       w681388Read(W681388REG_LCX, channel)

#define PON_DAC         (0x1 << 2)
#define PON_ADC         (0x1 << 1)
#define PON_DCDC_OFF    (0x1)
#define PON_FULL        (0x6)
#define SET_POWER_ON(power, channel)    w681388Write(power, W681388REG_PON, channel)
#define GET_POWER_ON(power, channel)    w681388Read(W681388REG_PON, channel)

#define INTV_IR3    (0x1 << 2)      //Interrupt pending in INT3
#define INTV_IR2    (0x1 << 1)      //Interrupt pending in INT2
#define INTV_IR1    (0x1)           //Interrupt pending in INT1

#define INTR1_RT     (0x1 << 0)         //Ring Trip Interrupt
#define INTR1_LC     (0x1 << 1)         //Loop Closure Interrupt
#define INTR1_PAT2   (0x1 << 2)         //Power Alarm QT2
#define INTR1_PAR2   (0x1 << 3)         //Power Alarm QR2
#define INTR1_PAR1   (0x1 << 4)         //Power Alarm QR1
#define INTR1_PAT1   (0x1 << 5)         //Power Alarm QT1
#define INTR1_PAR3   (0x1 << 6)         //Power Alarm QR3
#define INTR1_PAT3   (0x1 << 7)         //Power Alarm QT3

#define INTR2_O1A    ((0x1 << 0) << 8)  //Oscillator 1 Active Timer
#define INTR2_O1I    ((0x1 << 1) << 8)  //Oscillator 1 Inactive Timer
#define INTR2_O2A    ((0x1 << 2) << 8)  //Oscillator 2 Active Timer
#define INTR2_O2I    ((0x1 << 3) << 8)  //Oscillator 2 Inactive Timer
#define INTR2_RA     ((0x1 << 4) << 8)  //Ringing Active Timer
#define INTR2_RI     ((0x1 << 5) << 8)  //Ringing Inactive Timer
#define INTR2_DTMF   ((0x1 << 6) << 8)  //DTMF Interrupt
#define INTR2_FSK    ((0x1 << 7) << 8)  //FSK Interrupt occurs when the FSK FIFO is empty

#define INTR3_TMP    ((0x1 << 0) << 16) //Metering Pulse Inactive Timer
#define INTR3_MPA    ((0x1 << 1) << 16) //Metering Pulse Active Timer
#define INTR3_MPI    ((0x1 << 2) << 16) //Metering Pulse Inactive Timer

#define GET_INTV(channel)           w681388Read(W681388REG_INTV, channel)
#define GET_INT1(channel)           w681388Read(W681388REG_INT1, channel)
#define CLR_INT1(isr, channel)      w681388Write(isr, W681388REG_INT1, channel)
#define GET_INT2(channel)           w681388Read(W681388REG_INT2, channel)
#define CLR_INT2(isr, channel)      w681388Write(isr, W681388REG_INT2, channel)
#define GET_INT3(channel)           w681388Read(W681388REG_INT3, channel)
#define CLR_INT3(isr, channel)      w681388Write(isr, W681388REG_INT3, channel)

#define APG_TX_0dB      0x0
#define APG_TX_N3P5dB   0x1
#define APG_TX_P3P5dB   0x2
#define APG_TX_MULE     0x3

#define APG_RX_0dB      (0x0 << 2)
#define APG_RX_N3P5dB   (0x1 << 2)
#define APG_RX_P3P5dB   (0x2 << 2)
#define APG_RX_MULE     (0x3 << 2)
#define SET_AC_GAIN(arx, atx, channel)         w681388Write(((arx) | (atx)), W681388REG_APG, channel)
#define GET_ARX_GAIN(channel)       ((w681388Read(W681388REG_APG, channel) >> 2) & 0x3)
#define GET_ATX_GAIN(channel)       (w681388Read(W681388REG_APG, channel) & 0x3)

#define HB_MPB_DIS      (0x4 << 3)
#define HB_MPB_N2P77dB  (0x5 << 3)
#define HB_MPB_N1P94dB  (0x6 << 3)
#define HB_MPB_N1P02dB  (0x7 << 3)
#define HB_MPB_0dB      (0x0 << 3)
#define HB_MPB_P1P16dB  (0x1 << 3)
#define HB_MPB_P2P5dB   (0x2 << 3)
#define HB_MPB_P4P08dB  (0x3 << 3)

#define HB_APB_DIS      0x4
#define HB_APB_N2P77dB  0x5
#define HB_APB_N1P94dB  0x6
#define HB_APB_N1P02dB  0x7
#define HB_APB_0dB      0x0
#define HB_APB_P1P16dB  0x1
#define HB_APB_P2P5dB   0x2
#define HB_APB_P4P08dB  0x3

#define SET_HYBRID_BALANCE(mpb, apb, channel)   w681388Write(((mpb) | (apb)), W681388REG_HB, channel)
#define GET_MPB_BALANCE(channel)       ((w681388Read(W681388REG_HB, channel) >> 3) & 0x7)
#define GET_APB_BALANCE(channel)       (w681388Read(W681388REG_HB, channel) & 0x7)

#define LAMC_CMM    (0x1 << 6)      //Manual Mode:TIP (forward) or RING (reverse) forces voltage to follow VCM value
#define LAMC_DIFM   (0x1 << 5)      //Manual Mode:Forces differential voltage to follow VOH value
#define LAMC_SPUPM  (0x1 << 3)      //Disable Automatic Speed Up
#define LAMC_PAA    (0x1 << 2)      //Automatic Control Enter Open state automatically upon power alarm regardless of current state.
#define LAMC_RGA    (0x1 << 1)      //Automatic Ring Control When entering Ringing state the Ring Oscillator is
                                    //automatically enabled. Both OSNC:O2EN and RMPC:R1EN are set automatically.
                                    //Enter Active state from Ringing state automatically
                                    //upon Ring Trip Detect. The Ring Oscillators are
                                    //automatically disabled.
                                    //Forward or Reverse states determined primarily by OHV:SB
#define LAMC_LCDA   (0x1 << 0)      //Automatic Control Upon Loop Closure Detect the device will enter the
                                    //Active state from Idle, TIP Open, RING Open and
                                    //On-Hook Transmission states automatically upon Loop Closure Detect.
                                    //Forward or Reverse states determined primarily by OHV:SB
#define SET_LAMC(manual, channel)    w681388Write(manul, W681388REG_LAMC, channel)


#define LS_OPEN         0x0
#define LS_FW_ACTIVE    0x1
#define LS_FW_OH_TX     0x2
#define LS_TIP_OPEN     0x3
#define LS_RINGING      0x4
#define LS_RV_ACITVE    0x5
#define LS_RV_OH_TX     0x6
#define LS_RING_OPEN    0x7
#define LS_FW_IDEL      0x8
#define LS_RV_IDEL      0x9

#define GET_LS_STATUS(channel)              (w681388Read(W681388REG_LS, channel) & 0xf)
//Get the programmed linefeed state, not necessarily the actual linefeed state.
#define GET_LS_REALTIME_STATUS(channel)     ((w681388Read(W681388REG_LS, channel) >> 4) & 0xf)
//Get the actual real-time linefeed state.
//Automatic operations may cause actual linefeed state to deviate from the state defined in LS:LS[3:0].
#define SET_LS_STATUS(ls, channel)          w681388Write(ls, W681388REG_LS, channel)


#define OVH_SIGN_MASK       (0x1 << 6)

#define ETC_CBY     (0x1 <<5)   //Capacitors CT and CR bypassed

#define CAL1_TDT_MASK        (0xf << 4)
#define CAL1_VBAT1T_MASK     (0xf)
#define CAL2_TVTE1_MASK      (0xf << 4)
#define CAL2_RDT_MASK        (0xf)
#define CAL3_RVTE_MASK       (0xf)

#define W681388DCDC_DDCC (23)
#define W681388DCDC_PWMT (125)

#define W681388LINE_48V  (0x20)
#define W681388LINE_4_5V (0x03)
#define W681388LINE_9V   (0x06)

#define SET_OS1_FREQUENCY(f, channel)   w681388Write((unsigned char)((f) & 0xff), W681388REG_OS1CL, channel); \
                                        w681388Write((unsigned char)(((f) >> 8) & 0xff), W681388REG_OS1CH, channel)
#define SET_OS2_FREQUENCY(f, channel)   w681388Write((unsigned char)((f) & 0xff), W681388REG_OS2CL, channel); \
                                        w681388Write((unsigned char)(((f) >> 8) & 0xff), W681388REG_OS2CH, channel)
#define SET_OS1_AMPLITUDE(a, channel)   w681388Write((unsigned char)((a) & 0xff), W681388REG_OS1ICL, channel); \
                                        w681388Write((unsigned char)(((a) >> 8) & 0xff), W681388REG_OS1ICH, channel)
#define SET_OS2_AMPLITUDE(a, channel)   w681388Write((unsigned char)((a) & 0xff), W681388REG_OS2ICL, channel); \
                                        w681388Write((unsigned char)(((a) >> 8) & 0xff), W681388REG_OS2ICH, channel)
#define SET_OS1_ON_TIME(t, channle)     w681388Write((unsigned char)((t) & 0xff), W681388REG_OS1ATL, channel); \
                                        w681388Write((unsigned char)(((t) >> 8) & 0xff), W681388REG_OS1ATH, channel)
#define SET_OS2_ON_TIME(t, channle)     w681388Write((unsigned char)((t) & 0xff), W681388REG_OS2ATL, channel); \
                                        w681388Write((unsigned char)(((t) >> 8) & 0xff), W681388REG_OS2ATH, channel)
#define SET_OS1_OFF_TIME(t, channle)    w681388Write((unsigned char)((t) & 0xff), W681388REG_OS1ITL, channel); \
                                        w681388Write((unsigned char)(((t) >> 8) & 0xff), W681388REG_OS1ITH, channel)
#define SET_OS2_OFF_TIME(t, channle)    w681388Write((unsigned char)((t) & 0xff), W681388REG_OS2ITL, channel); \
                                        w681388Write((unsigned char)(((t) >> 8) & 0xff), W681388REG_OS2ITH, channel)

#endif

