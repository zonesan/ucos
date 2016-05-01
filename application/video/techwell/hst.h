//==================================================================================
//						Host definition & variable description
//==================================================================================
//...	w78le516 port define
#if defined(__4CH__)
	#define	MDATA			P0		
	
	#define	HSPB			P1_0
	#define	HWRB			P1_1
	#define	HRDB			P1_2
	#define	HALE			P1_3
	
	#define	HCSB0			P3_6//P2_0//
	#define	HCSB1			P3_7//P2_1//
	
	#define	MSE_CLK		P3_2
	#define	MSE_DAT		P3_5
	
	#define	VGA_DET		P2_0
	#define	I2C_SCL		P2_6
	#define	I2C_SDA		P2_7
	
	#define	KEY0			P3_0
	#define	JP_VDO		P3_1
	#define	RSTn			P3_4
#elif defined(__16CH__)
	#define	MDATA			P0		
	
	#define	HSPB			P1_0
	#define	HWRB			P1_1
	#define	HRDB			P1_2
	#define	HALE			P1_3
	
	#define	M_HCSB0		P2_0
	#define	M_HCSB1		P2_1
	#define	S1_HCSB0		P2_2
	#define	S1_HCSB1		P2_3
	#define	S2_HCSB0		P2_4
	#define	S2_HCSB1		P2_5
	#define	S3_HCSB0		P2_6
	#define	S3_HCSB1		P2_7
	
	#define	MSE_CLK		P3_2
	#define	MSE_DAT		P3_5
	
	#define	KEY0			P3_0
	#define	JP_VDO		P3_1
	#define	RSTn			P3_4
	
	#define	RC_MD			P4
#endif



//==================================================================================
//						Host function prototype description
//==================================================================================
extern void InitPrt(void);
extern void InitRstN(void);
extern void	InitVdo(U8 VinModeControl);

