//#include <stdio.h>
#include <stdlib.h>
//#include <math.h>
//#include <string.h>
//#include <memory.h>

typedef  long  int   Word32   ;
typedef  short int   Word16   ;
typedef  short int   Flag  ;

#define MAX_32 (Word32)0x7fffffffL
#define MIN_32 (Word32)0x80000000L

#define MAX_16 (Word16)0x7fff
#define MIN_16 (Word16)0x8000

#define TRUE	1
#define FALSE	0

#define kPrototypeFilterLength 128

#define _NEW_SCALING	1
//#define _ENABLE_TEST8_FIX	1

#define NLP_ON							0

#define Printf	TRACE
#define TRACE  // printf  
  
#define _BLOCK_LMS_ALG 0

// Constants used in the BLMS code
#define	FRAME_SIZE			80
#define	ECHO_FILTER_LENGTH	1024
#define	BLOCK_SIZE			10

// #define	SUB_BAND_EC_LENGTH 	128
#define DEC_FACTOR			8

#define NUM_OF_REFLECTORS		3
#define TIEC_ECHO_SPAN				192

#define TC_512					9
#define TC_32					5

#define	_EC_NO_DT			0
#define	_EC_DT_DETECTED		1


/* Constants used in LMS algorithm */
#define		INV_CNST		0x40000000
#define		INV_SH_CNST		(32-6)
//#define		kPreEmpConst	(4 * 32768/8.0)		// Whitening filter coefficient
#define		kPreEmpConst	(4 * 32768/8)		// Whitening filter coefficient



unsigned char malloc_wg[13704];
unsigned char malloc_wg1[4];

struct tEC {
	/* temporary home to DC removal states */
	Word32 farEndDC;
	Word32 nearEndDC;

	Word16 st_preemp;				// Preemphasis filter state
	Word16 st_preemp_upd;			// Preemphasis filter (used for coeff updation) state
	Word16 st_deemp;				// Deemphasis filter state

	// SNCHANGE
	Word16 filt_states_fixed[ECHO_FILTER_LENGTH+FRAME_SIZE+1];	// Filter states
	Word16 EchoDelaylineBuf[ECHO_FILTER_LENGTH+FRAME_SIZE];	// Used only to sync C and Assembly
	Word16 *states_new_ptr;			// Ptr to states
	Word16 *states_old_ptr;			// Ptr to states
	Word16 DoubleTalkHangCount;
	Word16 ConvergenceHangCount;
	Word16 bDoubleTalk;
	Word16 EchoPathChanged;
	
	Word32 Rin_L1_Tc32;	
	Word32 PrevFrameEnergy;
	Word32 Pn;
	Word32 RinPower_Tc512;
	/* This variable not needed */
//	Word32 RinPower_TcN;
	Word32 SinPower_Tc512;
	Word32 Sout_L1_Tc32;
	Word32 SoutPower_Tc512;
	Word32 EoutPower_Tc512;
	

	Word32 MaxRin;
	Word32 MaxM06;
	Word32 MaxBlocks[28];
	Word16 NumBlocks;
	Word32 *gpMaxBlocks;
//	Word16 bAdaptOff; 
	Word16 AdaptationCount;
	Word16 nl_supress;
	Word16 SampleCount;
	Word16 CoeffWrFlag;
	Word16 frame_mu_mant;
	Word16 frame_mu_exp;
	Word16 modified_mu_exp;

/* BG noise enhancements */
	Word32 Sin_L1_Tc32;
	Word32 Sin_L1_Tc512;
	Word32 Sout_L1_Tc512;
	Word32 nearEndBackgroundNoiseLevel;
	Word16 bFrameAdaptOff;
	Word16 ScalingFactor;
	Word16 ShiftConst;
	Word16 DivergenceHangOverCount;
	Word16 BestCoeff100;
	Word16 BestCoeff50;
	Word16 CurCoeffBuf;
	Word16 sdram_data_state;
	Word16 bestSoutPower100;
	Word16 bestSinPower100;
	Word16 bestSoutPower50;
	Word16 bestSinPower50;
	Word16 consecutiveImprovementCount;
	Word16 timeToUpdate;
	Word16 last_sin_power;
	Word16 last_sout_power;
	Word16 ShiftConst2;

/* Control Parameter  */
	Word16 bAdaptOn;

	Word16 bBGNFrameAdaptOff;			
	Word16 DataLenEstCounter;
	Word16 reflectorSearchCheckHangCount;
};

// NIUD ************************************************************************************
typedef struct 
{
	Flag Overflow;
	Flag Carry;
}SIGMAFLAG;


/******************************* SigmaLib ***********************************/
Word16 sigmaLib_sature(Word32 L_var1, SIGMAFLAG *pFlag);             /* Limit to 16 bits,    1 */
Word16 sigmaLib_sub(Word16 var1, Word16 var2, SIGMAFLAG *pFlag);     /* Short sub,           1 */
Word16 sigmaLib_abs_s(Word16 var1);                /* Short abs,           1 */
Word16 sigmaLib_mult(Word16 var1, Word16 var2, SIGMAFLAG *pFlag);    /* Short mult,          1 */
Word32 sigmaLib_L_mult(Word16 var1, Word16 var2, SIGMAFLAG *pFlag);  /* Long mult,           1 */
Word16 sigmaLib_extract_h(Word32 L_var1);          /* Extract high,        1 */
Word16 sigmaLib_extract_l(Word32 L_var1);          /* Extract low,         1 */
Word16 sigmaLib_round_vcu(Word32 L_var1);          /* Round,               1 */
Word32 sigmaLib_L_add(Word32 L_var1, Word32 L_var2, SIGMAFLAG *pFlag);   /* Long add,        2 */
Word32 sigmaLib_L_sub(Word32 L_var1, Word32 L_var2, SIGMAFLAG *pFlag);   /* Long sub,        2 */
Word32 sigmaLib_L_shl(Word32 L_var1, Word16 var2, SIGMAFLAG *pFlag); /* Long shift left,     2 */
Word32 sigmaLib_L_shr(Word32 L_var1, Word16 var2, SIGMAFLAG *pFlag); /* Long shift right,    2 */
Word32 sigmaLib_L_deposit_h(Word16 var1);       /* 16 bit var1 -> MSB,     2 */
Word32 sigmaLib_L_shr_r(Word32 L_var1, Word16 var2, SIGMAFLAG *pFlag);/* Long shift right with round,  3*/
Word32 sigmaLib_L_abs(Word32 L_var1);            /* Long abs,              3 */

long int sigmaLib_ADD_LbL(long int TT, long int SS);
long int sigmaLib_ASL_LbS(short SS, long int TT);
long int sigmaLib_ASR_LbS(short SS, long int TT);
long int sigmaLib_MPY_SbS(short int TT, short int SS);
long int sigmaLib_SUB_LbL(long int TT, long int SS);

// NIUD added
SIGMAFLAG *initSigmaLibBlock(void);
void destroySigmaLibBlock(SIGMAFLAG *pFlag);
// NIUD  end ************************************************************************************


struct iEC{
	Word16 effectiveTailLength;
	Word16 segCount;
	Word16 DelayIndex[NUM_OF_REFLECTORS];
	Word16 N[NUM_OF_REFLECTORS];
	Word16 FilterCoef[ECHO_FILTER_LENGTH];	
//	Word16 SubBandFilterCoef[kPrototypeFilterLength];	
};

typedef struct 
{
	// Constants
	int kBlkCountFor100msec;		
	int kBlkCountFor75msec;			
	int	kSampleCountFor20msec;
	int	kUpdateIntervalInSamples;

	int k_42dBm;
	int	k_42dBm_scaled;
	int	k_30dBm_scaled;

	int	m_FrameSize;			
	int	m_N;					
	int	m_M;	
	Word16	m_Log2_N;			
	Word16	m_MuMant;			
	Word16	m_MuExp;			
	Word16	m_Log2_M;

	// Flags
	int m_bEnableNLP;
	int m_bEnableDoubleTalk;
	int m_bEnablePreEmphasis;

	// Debug variables
	Word16 m_NumBlocks;
	Word32 m_SampleNum, m_PrevSampleNum, m_Transition;

	// EC states
	struct tEC   EC_states;

	int m_bBandChangeCheck;
	int m_bReflectorSearchCheck;

	// i/o interface
	Word16 m_SoutBuf[FRAME_SIZE];
	Word16 m_SoutBufNlp[FRAME_SIZE];
	Word16 m_FarEndBuf[FRAME_SIZE];
	Word16 m_NearEndBuf[FRAME_SIZE];		

	// SDRAM interface
	Word16 m_SdramIOPtr;
	struct iEC m_EC_Ref;
	struct iEC m_EC_Ref_SDRAM[3];		// 3 Sets used for Double Talk logic
	
	// Power table
	Word16 m_InvTab[16]; 

	// NIUD added
	SIGMAFLAG *pFlag;
}BLMS_EC;

// Member functions to simulate SDRAM I/O
void SDRAM_RdSegInfo(BLMS_EC *pBlock);
void SDRAM_WrSegInfo(BLMS_EC *pBlock); 

// Initialization Modules
int BLMSEC_EcInit(BLMS_EC *pBlock, int bECType, int tail, int framesize, int blksize, int nlp, 
			int doubletalk, int emphasis);
void BLMSEC_EcInitTables(BLMS_EC *pBlock);

// Member functions
void BLMSEC_EchoCanceller(BLMS_EC *pBlock);

// private functions
void ComputePowers(BLMS_EC *pBlock, struct tEC *EC_st, Word16 *pRin, Word16 *pSin, Word16 *pSout);
void ComputeDoubleTalkFlag (BLMS_EC *pBlock, struct tEC *EC_st, Word16 *pSout);
void AdaptFilterCoeffs(BLMS_EC *pBlock, struct tEC *EC_st, Word16 *pSout);
static Word32 CompPower(Word32 ener, Word32 Power, Word16 shiftCount);
void BLMSEC_InvPower(BLMS_EC *pBlock, Word32 Power, Word16 *mant, Word16 *exp);
void EstimateEcho(BLMS_EC *pBlock, struct tEC *EC_st, Word16 *sin, Word16 *sout);
void EstimateEcho2(BLMS_EC *pBlock, struct tEC *EC_st, Word16 *sin, Word16 *sout, Word16 NumSamples);
static void UpdateFilterCoeffs(BLMS_EC *pBlock, Word16 *state, Word16 *st_preemp_upd, Word16 *sout,
			Word16 mu, Word16 exp);
void VP3_PreEmphasis(BLMS_EC *pBlock, Word16 *pIn, Word16 preempState, Word16 *pOut, struct iEC EC_Ref, 
			Word32 *pSum1, Word32 *pSum2);
static void PreEmphasis(BLMS_EC *pBlock, Word16 *pInput, Word16 len, Word16 *pState, Word16 *pOutput);
static void DeEmphasis(BLMS_EC *pBlock, Word16 *pInput, Word16 len, Word16 *pState, Word16 *pOutput);
static Word32 ComputeMaxPower(struct tEC *EC_st);
void EcFrameProcessing(BLMS_EC *pBlock, struct tEC *EC_st,Word16 *pSin, Word16 *pSout, Word16 *pSoutNLP);
static Word32 GetL1Norm(BLMS_EC *pBlock, Word16 *data, Word32 Power, Word16 shiftCount);
static Word32 GetL2Norm(Word16	*inp, Word16 len);
static void ClearECPath(BLMS_EC *pBlock, struct tEC *EC_st, Word16 *pSin, Word16 *pSout, Word16 *pSoutNLP);
BLMS_EC *initBLMSECBlock(void);
void destroyBLMSECBlock(BLMS_EC *pblock);

