//==================================================================================
//						Display wndwdow definition & variable description
//==================================================================================
#define	WNDW_RC_FM_QU_POS_X			5
#define	WNDW_RC_FM_QU_POS_Y			(WNDW_RC_FM_POS_Y-2)
#define	WNDW_RC_FM_QU_L					21
#define	WNDW_RC_FM_QU_H					14
#define	WNDW_RC_FM_QU_CLOS_X			(WNDW_RC_FM_QU_POS_X+WNDW_RC_FM_QU_L-1)
#define	WNDW_RC_FM_QU_CLOS_Y			(WNDW_RC_FM_QU_POS_Y)

#if	defined(__4CH__)
#define	WNDW_RC_FM_QU_SZ_DEC_X			(WNDW_RC_FM_QU_POS_X+4)
#define	WNDW_RC_FM_QU_SZ_INC_X			(WNDW_RC_FM_QU_SZ_DEC_X+11)
#define	WNDW_RC_FM_QU_SZ_NUM_X			(WNDW_RC_FM_QU_SZ_DEC_X+6)
#define	WNDW_RC_FM_QU_SZ_NUM_Y			(WNDW_RC_FM_QU_POS_Y+3)
#elif	defined(__16CH__)
#define	WNDW_RC_FM_QU_STG_DEC_X		(WNDW_RC_FM_QU_POS_X+2)
#define	WNDW_RC_FM_QU_STG_INC_X			(WNDW_RC_FM_QU_STG_DEC_X+4)
#define	WNDW_RC_FM_QU_STG_NUM_X		(WNDW_RC_FM_QU_STG_DEC_X+2)
#define	WNDW_RC_FM_QU_STG_NUM_Y		(WNDW_RC_FM_QU_POS_Y+3)

#define	WNDW_RC_FM_QU_SZ_DEC_X			(WNDW_RC_FM_QU_STG_INC_X+5)
#define	WNDW_RC_FM_QU_SZ_INC_X			(WNDW_RC_FM_QU_SZ_DEC_X+6)
#define	WNDW_RC_FM_QU_SZ_NUM_X			(WNDW_RC_FM_QU_SZ_DEC_X+3)
#define	WNDW_RC_FM_QU_SZ_NUM_Y			(WNDW_RC_FM_QU_POS_Y+3)
#endif

#define	WNDW_RC_FM_QU_TBL_ROW_X	(WNDW_RC_FM_QU_POS_X+1)
#define	WNDW_RC_FM_QU_TBL_COL_X	(WNDW_RC_FM_QU_TBL_ROW_X+3)
#define	WNDW_RC_FM_QU_TBL_Y			(WNDW_RC_FM_QU_SZ_NUM_Y+2)
#define	WNDW_RC_FM_QU_TBL_NUM_X	(WNDW_RC_FM_QU_TBL_COL_X)
#define	WNDW_RC_FM_QU_TBL_NUM_Y	(WNDW_RC_FM_QU_TBL_Y+1)

#define	WNDW_RC_FM_QU_SCRL_X			(WNDW_RC_FM_QU_POS_X+WNDW_RC_FM_QU_L-1)//(WNDW_RC_FM_QU_CLOS_X)
#define	WNDW_RC_FM_QU_SCRL_UP_Y		(WNDW_RC_FM_QU_TBL_Y+1)
#define	WNDW_RC_FM_QU_SCRL_DN_Y	(WNDW_RC_FM_QU_SCRL_UP_Y+7)


//#define	QUE_BOX_FRM				(BOX_BTN6)
//#define	QUE_BOX_TTL				(BOX_BTN5)
//#define	QUE_BOX_QUIT			(BOX_BTN4)

#define	QUE_2DBOX_ROW			0
#define	QUE_2DBOX_COL			1
#define	QUE_2DBOX_NUM			2
#define	QUE_2DBOX_SCRL			3

#define	QUE_TBL_NUM_DEC		0
#define	QUE_TBL_NUM_INC		1

#define	QUE_RD_MD_CRNT			0x00
#define	QUE_RD_MD_WRT				0x50
#define	QUE_RD_MD_LD				0x70

#define	MSE_SCRL_QUE_HL			0xc8			//... 9 bit resolution
#define	MSE_SCRL_QUE_HW			0x06
#define	MSE_SCRL_QUE_HR			(MSE_SCRL_QUE_HL+MSE_SCRL_QUE_HW)
#define	MSE_SCRL_QUE_VT			0x61//0x59			//... scrl position of step 0 (9 bit resolution)
#define	MSE_SCRL_QUE_VW			0x21
#define	MSE_SCRL_QUE_VB			(MSE_SCRL_QUE_VT+MSE_SCRL_QUE_VW)

#define	MSE_ON_OSD_STG		BIT0
#define	MSE_ON_OSD_SIZE		BIT1
#define	MSE_ON_OSD_SCRL		BIT2

//==================================================================================
//==================================================================================
extern U8	wndw_rc_fm_qu_scrl_step;
extern U8	wndw_rc_fm_qu_size;

extern U8	wndw_rc_fm_qu_scrl_grip_vt;

//extern U8	wndw_rc_fm_qu_bak[8];

//==================================================================================
//						Display wndwdow function description
//==================================================================================
#if	defined(__16CH__)
extern U8	GetWndwRcFmQuOfst(void);//U8 _osd_pos_x, U8 _osd_pos_y)
extern void	SetWndwRcFmQuOSDStgNum(U8 _dvc);
extern void	SetWndwRcFmQuOSDStg(void);
extern void	SetWndwRcFmQuPopRdMd(U8 _que_rd_mode);
extern void	GetWndwRcFmQuPopData(U8 _addr, U8 *_a4);
extern void	SetWndwRcFmQuPopData(U8 _addr, U8 *_a4);
extern void	LoadWndwRcFmQuOSDTblPop(void);
extern void	RunWndwRcFmQuStg(U8 _osd_pos_x);
extern void	RunWndwRcFmQuTblPop(S8 _osd_pos_x, S8 _osd_pos_y, U8 _cal);		//... S8 : signed char
#endif
extern U8	GetWndwRcFmQuSz(void);
extern void	SetWndwRcFmQuSz(U8 _data);
extern void	SetWndwRcFmQuOSDSzNum(U8 _data);
extern void	SetWndwRcFmQuOSDSz(void);
extern U8	GetWndwRcFmQuScrlStep(void);
extern void	SetWndwRcFmQuScrlBar(U8 _step);
extern void	SetWndwRcFmQuOSDScrl(void);
extern void	SetWndwRcFmQuMuxRdMd(U8 _que_rd_mode);
extern U8	GetWndwRcFmQuMuxData(U8 _addr);
extern void	SetWndwRcFmQuMuxData(U8 _addr, U8 _data);
extern void	LoadWndwRcFmQuOSDTblMux(void);
extern void	SetWndwRcFmQuOSDTblCol(void);
extern void	SetWndwRcFmQuOSDTblRow(void);
extern void	SetWndwRcFmQuOSDTblNum(void);
extern void	CreateWndwRcFmQu(void);
extern void	RunWndwRcFmQuSz(U8 _osd_pos_x);
extern void	RunWndwRcFmQuScrl(U8 _osd_pos_y);
extern void	RunWndwRcFmQuTblMux(S8 _osd_pos_x, S8 _osd_pos_y, U8 _cal);		//... S8 : signed char
extern void	RunWndwRcFmQuTbl(S8 _osd_pos_x, S8 _osd_pos_y, U8 _cal);
extern void	RunWndwRcFmQu(void);//U8 _osd_pos_x, U8 _osd_pos_y)
