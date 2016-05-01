#include "cmn.h"

#include "video_cfg.h"

//==================================================================================
//						Protocol definition & variable description
//==================================================================================
//#define	I2C_ID_2844		0x28			//... I2C device ID of tw2844 (7b0101000)
//#define	I2C_ID_2814		0x42			//... I2C device ID of TW2814 (7b1000010)
//#define	I2C_ID_7121_A	0x46			//... I2C device ID of SAA7121_A
//#define	I2C_ID_7121_D	0x44			//... I2C device ID of SAA7121_D
//
//#define	DVC_ID_2844_A	0
//#define	DVC_ID_2844_D	3
//
//#define	I2C_ASIC_SCL(x)		IO_DAT_SPADDR7(x)
//#define	I2C_ASIC_SDA(x)		IO_DAT_SPDAT7(x)
////#define	I2C_STND_SCL(x)		IO_DAT_I2C_SCL(x)
////#define	I2C_STND_SDA(x)		IO_DAT_I2C_SDA(x)


#define	I2C_ID_7121_C	(0x46)			//... I2C device ID of SAA7121_C
#define	I2C_ID_7121_D	(0x44)			//... I2C device ID of SAA7121_D
#define	I2C_ID_2835		(0x80|0x35)	//... [7]:initial value of MDATA7(I2C_ASIC_SDA)|[6:0] : Slave Address of TW2824 = 0x28
#define	I2C_ID_MDIN180	(0x6e)//... : 7bit address, (0xDC): 8bit address(original address)


//#define	PRL_MSTR			(0x03)
//#define	MASTER			PRL_MSTR	//	I2C_MSTR	//		//... temporal
//DWDW

#ifdef  DVR_BOARD

#define	DVC_ALL		(0x0f)
#define	MASTER			(0x01)
#define	SLAVE1			(0x03)
#define	SLAVE2			(0x07)
#define	SLAVE3			(0x0f)
#else
#define	DVC_ALL		(0x7e)
#define	MASTER			(0x7e)
#define	SLAVE1			(0x7e)
#define	SLAVE2			(0x7e)
#define	SLAVE3			(0x7e)
#endif


//#define	DVC_ID_2835_A	SLAVE3//MASTER
//#define	DVC_ID_2835_B	SLAVE2//SLAVE1
//#define	DVC_ID_2835_C	SLAVE1//SLAVE2
//#define	DVC_ID_2835_D	MASTER//SLAVE3

//#define	MASTER			SLAVE3		//... temporal

#define	DVC_PG0		0
#define	DVC_PG1		1
#define	DVC_PG2		2

#define	PTH_X			BIT0//BIT1//
#define	PTH_Y			BIT1//BIT5//
#define	PTH_PB		BIT2//
#define	PTH_ALL		(PTH_X|PTH_Y)


//#define	TW2835		(0x80|0x35)	// [7]:I2C_ASIC_SDA = 1, [6:1]:Slave Address of TW2824 = 0x35
//#define	TW2815		(0x80|0x15)	// [7]:I2C_ASIC_SDA = 1, [6:1]:Slave Address of TW2824 = 0x15
//#define	TW9901		0x44    // Slave Address of TW9901 (SIAD1/SIAD0 : GND)
//#define	KS0123		0x0c    // Slave Address of KS0123


//==================================================================================
//						Protocol function prototype description
//==================================================================================

#if defined(__WTH_VGA__)
	extern void SetI2CStart(void);
	extern void SetI2CStop(void);
	extern void SetI2CAck(U8 _tgl);
	extern void SetI2CP2S(U8 _wdat);
	//extern void WriteI2CByte(U8 _dvc, U8 _addr, U8 _wdat);
	//extern void WriteI2CTable(U8 _dvc, U8 _addr, U8 *_tbl_ptr, U8 _tbl_cnt);
	extern U8 GetI2CS2P(void);
	//extern U8 ReadI2CByte(U8 _dvc, U8 _addr);
	extern void WriteI2CWord(U8 _dvc, U16 _addr, U16 _wdat);
	extern U16 ReadI2CWord(U8 _dvc, U16 _addr);
#endif

//extern void SetAsicI2CStart(void);
//extern void SetAsicI2CStop(void);
//extern void SetAsicI2CAck(void);
//extern void SetAsicI2CP2S(U8 _wdat);
//extern void WriteAsicI2CByte(U8 _dvc, U8 _pg, U8 _addr, U8 _wdat);
//extern U8 GetAsicI2CS2P(void);
//extern U8 ReadAsicI2CByte(U8 _dvc, U8 _pg, U8 _addr);
//extern void WriteAsicI2CTable(U8 _dvc, U8 _pg, U8 _addr, U8 *_tbl_ptr, U8 _tbl_cnt);
//
//extern void ChipSelAllOff(void);
//extern void ChipSelPage0(void);
//extern void ChipSelPage1(void);
//extern void ChipSelPage2(void);
extern void WriteAsicByteOSD(U8 _dvc, U8 _addr, U8 _wdat);
extern void WriteAsicByte(U8 _dvc, U8 _pg, U8 _addr, U8 _wdat);
extern void WriteAsicTable(U8 _dvc, U8 _pg, U8 _addr, U8 *_tbl_ptr, U8 _tbl_cnt);
//extern void WriteAsicTablePair(U8 _dvc, U8 _pg, U8 *_tbl_ptr, U8 _tbl_cnt_pair);
extern U8 ReadAsicByte(U8 _dvc, U8 _pg, U8 _addr);
//extern U8	ReadAsicFld(void);
extern U8	ReadAsicOSDMemWr(U8 _dvc);//void);

