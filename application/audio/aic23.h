/****************************************************************************/
/*  The following macros define constants used throughout the functions.    */
/****************************************************************************/
/* macros used in aic23 */

#define  LLICVC  0x0
#define  RLICVC  0x1
#define  LCHVC   0x2
#define  RCHVC   0x3
#define  AAPC    0x4
#define  DAPC    0x5
#define  PDC     0x6
#define  DAIF    0x7
#define  SRC     0x8
#define  DIA     0x9
#define  RESET   0xf

/* For DM270 CS_ is being taken care by h/w */
#define  CS_	0x0010                              /* lj_q does these pin map for vb2100?*/

#define  SCLK	0x0020 //GIO21
#define  SDI	0x0040 //GIO22

// LLICVC Left Line Input Channel Volume Control (Address:0000000)
#define 	LRS 	0x0100
#define 	LIM		0x0080
#define		LIV4	0x0010
#define		LIV3	0x0008
#define		LIV2	0x0004
#define		LIV1	0x0002
#define		LIV0	0x0001

// RLICVC Right Line Input Channel Volume Control (Address:0000001)
#define 	RLS 	0x0100
#define 	RIM		0x0080
#define		RIV4	0x0010
#define		RIV3	0x0008
#define		RIV2	0x0004
#define		RIV1	0x0002
#define		RIV0	0x0001

// LCHVC Left Channel Headphone Volume Control    (Address:0000010)
#define     LRSH    0x0100
#define 	LZC		0x0080
#define		LHV6	0x0040
#define		LHV5	0x0020
#define		LHV4	0x0010
#define		LHV3	0x0008
#define		LHV2	0x0004
#define		LHV1	0x0002
#define		LHV0	0x0001

// RCHVC Left Channel Headphone Volume Control    (Address:0000011)
#define     RLSH    0x0100
#define 	RZC		0x0080
#define		RHV6	0x0040
#define		RHV5	0x0020
#define		RHV4	0x0010
#define		RHV3	0x0008
#define		RHV2	0x0004
#define		RHV1	0x0002
#define		RHV0	0x0001

// AAPC Analog Audio Path Control                 (Address:0000100)
#define 	STA1	0x0080
#define		STA0	0x0040
#define		STE 	0x0020
#define		DAC 	0x0010
#define		BYPAIC 	0x0008
#define		INSEL	0x0004
#define		MICM	0x0002
#define		MICB	0x0001

// DAPC Digital Audio Path Control                (Address:0000101)
#define		DACM	0x0008
#define		DEEMP1	0x0004
#define		DEEMP0	0x0002
#define		ADCHP	0x0001

// PDC Power Down Control                         (Address:0000110)
//#define		OFF		0x0080
#define		CLK		0x0040
#define		OSC		0x0020
#define		OUT		0x0010
#define		DACPD	0x0008
#define		ADCPD	0x0004
#define		MIC		0x0002
#define		LINE	0x0001

// DAIF Digital Audio Path Control                (Address:0000101)
#define		MS 		0x0040
#define		LRSWAP	0x0020
#define		LRP 	0x0010
#define		IWL1	0x0008
#define		IWL0	0x0004
#define		FOR1	0x0002
#define		FOR0	0x0001

// SRC Sample Rate Control                        (Address:0001000)
#define		CLKOUT	0x0080
#define		CLKIN	0x0040
#define		SR3SR	0x0020
#define	    SR2SR	0x0010
#define     SR1SR	0x0008
#define	    SR0SR	0x0004
#define		BOSR	0x0002
#define		MSSR   	0x0001

// DIA Digital Interface Activation               (Address:0001001)
#define		ACT		0x0001

// RESET   RESet                                   (Address:0001111)
#define     RES     0x01FF

#define I2C_ACK 0
#define I2C_NCK 1

#define AIC23_i2cDevAddr 0x1B

#define DM_SDA  12
#define DM_SCL  13
#define AIC_CS

void byp_onH_offL(unsigned int sel);
void aic_codec_delay(unsigned char delay_cnt);
static void aic_codec_data_high_output(void);
static void aic_codec_data_low_output(void);
static void aic_codec_clk_high_output(void);
static void aic_codec_clk_low_output(void);
void AICInit(void);
int Send_aic23_cmd(int cmd );
int Send_aic23_cmd_test(unsigned char addr, unsigned int cmd);
void ConfigureAIC(unsigned int Oscillator_freq,unsigned int Sampling_freq);
void aic23_set_volume(int volL, int volR);
inline void GIOOutput(unsigned int bit);
inline void GIOInput(unsigned int bit);
inline void GIOBitSet(unsigned int bit);
inline void GIOBitClear(unsigned int bit);
inline unsigned int GIOGetBit(unsigned int bit);
