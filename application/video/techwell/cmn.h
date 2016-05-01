#ifndef CMN_H
#define CMN_H 
//==================================================================================
//						Common definition & variable description
//==================================================================================
#define	BIT0		(0x0001)
#define	BIT1		(0x0002)
#define	BIT2		(0x0004)
#define	BIT3		(0x0008)
#define	BIT4		(0x0010)
#define	BIT5		(0x0020)
#define	BIT6		(0x0040)
#define	BIT7		(0x0080)
#define	BIT8		(0x0100)
#define	BIT9		(0x0200)
#define	BIT10		(0x0400)
#define	BIT11		(0x0800)
#define	BIT12		(0x1000)
#define	BIT13		(0x2000)
#define	BIT14		(0x4000)
#define	BIT15		(0x8000)

//==================================================================================
#define U8  unsigned char
#define U16 unsigned int
#define U32 unsigned int
#define S8  signed char
#define S16 signed short int

//==================================================================================
#define	NULL		0
#define	ON			1
#define	OFF		0

//==================================================================================
#define	SetBit(x,y)			((x) |= (y))         
#define	ClearBit(x,y)		((x) &= ~(y))        
#define	BitSet(x,y)			(((x)&(y))== (y))    
#define	BitClear(x,y)		(((x)&(y))== 0)      
#define	IfBitSet(x,y)		if(((x)&(y)) == (y)) 
#define	IfBitClear(x,y)	if(((x)&(y)) == 0)

//==================================================================================
#define	DLY_FOR(x)				for(_dly_=0;_dly_<(x<<3);_dly_++)
#define	DLY_I2C					60		//... SCL : 80 kHz at 4.7 kΩ
#define	DLY_PRLL_TBL			50
#define	DLY_IO					5
#define	DLY_SW_STRB				20000//10

//==================================================================================
#define	LOSS_MODE_BYPS		0
#define	LOSS_MODE_FRZ			1
#define	LOSS_MODE_BLAK		2
#define	LOSS_MODE_BLK_FRZ	3



//==================================================================================
#define	VDO_NTSC				0
#define	VDO_PAL				1
extern int	b_cmn_dtct_vs;
extern int	b_cmn_jp_vdo;
extern int	b_cmn_wndw_pop;
extern int	b_cmn_wndw_mn;
extern int	b_cmn_irq;
extern int	b_cmn_fld_od;
//extern int	b_cmn_fld_evn;

extern int	b_cmn_tmp;


extern int b_cmn_cnt_vs;
extern int b_cmn_cnt_fld;
extern int b_cmn_cnt_tgl;
extern int b_cmn_cnt_01s;
extern int b_cmn_cnt_02s;
extern int b_cmn_cnt_05s;
extern int b_cmn_cnt_1s;



//------------------------------------------------------------------------------------------
//extern int	b_rc_in_bnd;
//extern int	b_rc_in_dgtl;
//extern int	b_rc_in_anlg;
//extern int	b_rc_in_dtct;
//extern int	b_rc_in_run;


//#define	CIF_TYPE_FR				0
//#define	CIF_TYPE_FL				1
//#define	MUX_TYPE_LV			0
//#define	MUX_TYPE_FL			1
//#define	MUX_TYPE_FR			2
//
//extern U8 idata	wndw_rc_fm_cif_type;
//extern U8 idata	wndw_rc_fm_mux_type;



extern int	b_ds_pb_cif;
extern int	b_ds_wth_4pb;
extern int	b_ds_wth_16pb;
extern int	b_ds_wth_rc;
//extern int	b_ds_in_eror;
extern int	b_ds_in_run;
extern int	b_ds_in_auto;

//#define	SC_FMT_1CH				0
//#define	SC_FMT_4CH				1
//#define	SC_FMT_4CH_PB			2
//#define	SC_FMT_16CH				3
//#define	SC_FMT_16CH_PB				4
//#define	SC_FMT_8CH				5
//#define	SC_FMT_8CH_RC			6
//
////#if defined(__4CH__)
////extern U8 	wndw_ds_sc_fmt;
////#elif	defined(__16CH__)
////extern U8 	wndw_ds_sc_fmt;
////#endif
//extern U8 	wndw_ds_sc_fmt;
//



#define	CH_SRC_LV						0
#define	CH_SRC_PB						1
extern int	b_ch_src;
extern int	b_ado_lv_mr;
extern int	b_ado_lv_mr_arw_pb;

//------------------------------------------------------------------------------------------

U8      cmn_bot;
//extern U8	run_8ch_cnt_ana_sw;
extern U8	cmn_cnt_vs;
extern U8	cmn_cnt_vs_sub;
extern U8 	cmn_vga_det;				//... __WTH_VGA__일때 idata 없애면 compile시 error 발생.

#if	defined(__16CH__)
	extern U8	cmn_rc_md;
	#define	RC_MD_120_FPS				0
	#define	RC_MD_240_FPS				1
	#define	RC_MD_480_FPS				2
#endif
extern U8      cmn_dvc;
//DWDW extern U8	MASTER;
extern U8	cmn_lst_slvr_x;
extern U8	cmn_lst_slvr_y;
extern U8	cmn_max_ch;


//==================================================================================
//						Common function prototype description
//==================================================================================

extern void	WaitCmnFldOd(void);
extern U8	GetCodeLength(unsigned char *str);
extern U8	ConvDec2Bcd(U8 _dat);	// ex : 38 -> 0x38, 99 -> 0x99 (BCD값이므로 a~f는 포함이 안 된다는 가정에서, decimal값 10마다 6씩 더해준다.)
//extern U8	ConvBcd2Dec(U8 _dat);	// ex : 0x38 -> 38, 0xaf -> X (BCD값이므로 a~f는 포함이 안 된다는 가정에서, hex값 0x10마다 6씩 빼준다.)
//
//extern U8	GetBitFlgType(U8 _var, U8 _flag);
//extern void	SetBitFlgType(U8 *_p_var, U8 _flag, U8 _data);
//extern U8	GetBitVarType(U8 _var, U8 _bit_ofst, U8 _bit_wdth);
//extern void	SetBitVarType(U8 *_p_var, U8 _bit_ofst, U8 _bit_wdth, U8 _data);

extern void	SetCrntDvcCh(U8 _ch, U8 *_crnt_dvc, U8 *_crnt_ch);
extern U8	GetAsicFlgType(U8 _dvc, U8 _pg, U8 _addr, U8 _flg);
extern void	SetAsicFlgType(U8 _dvc, U8 _pg, U8 _addr, U8 _flg, U8 _data);
extern U8	GetAsicVar8Type(U8 _dvc, U8 _pg, U8 _addr, U8 _flg);
extern void	SetAsicVar8Type(U8 _dvc, U8 _pg, U8 _addr, U8 _flg, U8 _data);

extern U16	ChkNoVdo(void);

#endif

