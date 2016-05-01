//==================================================================================
//						Display wndwdow definition & variable description
//==================================================================================
#define	WNDW_DS_SC_POS_X			14
#define	WNDW_DS_SC_POS_Y			7//4// 
#define	WNDW_DS_SC_L				17//
#define	WNDW_DS_SC_H				9//16//
#define	WNDW_DS_SC_CLOS_X		(WNDW_DS_SC_POS_X+WNDW_DS_SC_L-1)
#define	WNDW_DS_SC_CLOS_Y		(WNDW_DS_SC_POS_Y)

#define	WNDW_DS_SC_RDO_FM_X			(WNDW_DS_SC_POS_X+1)
#define	WNDW_DS_SC_RDO_FM_Y			(WNDW_DS_SC_POS_Y+2)
#define	WNDW_DS_SC_CHK_X				(WNDW_DS_SC_RDO_FM_X+1)
#define	WNDW_DS_SC_CHK_4PB_Y			(WNDW_DS_SC_RDO_FM_Y+2)
#define	WNDW_DS_SC_CHK_16PB_Y		(WNDW_DS_SC_CHK_4PB_Y+2)
#define	WNDW_DS_SC_CHK_RC_Y			(WNDW_DS_SC_CHK_16PB_Y+2)//(WNDW_DS_SC_RDO_FM_Y+6)//
//#define	WNDW_DS_SC_RDO_LS_X			(WNDW_DS_SC_RDO_FM_X+1)
//#define	WNDW_DS_SC_RDO_LS_Y			(WNDW_DS_SC_CHK_RC_Y+3)
//
//#define	SC_LOSS_BYPS				0
//#define	SC_LOSS_FRZ				1
//#define	SC_LOSS_BLAK				2
//#define	SC_LOSS_BLIK				3

#define	WNDW_DS_SC_CNT_X				(WNDW_DS_SC_CHK_X+1)
#define	WNDW_DS_SC_UNIT_X			(WNDW_DS_SC_CHK_X+4)
#define	WNDW_DS_SC_CNT_Y				11//(WNDW_DS_SC_CHK_RC_Y+2)


#define	SC_FMT_1CH				0
#define	SC_FMT_4CH				1
#define	SC_FMT_4CH_PB			2
#define	SC_FMT_16CH				3
#define	SC_FMT_16CH_PB			4
#define	SC_FMT_8CH				5
#define	SC_FMT_8CH_RC			6


//==================================================================================
extern U8	wndw_ds_sc_fmt;
//extern U8	wndw_ds_sc_loss;


//==================================================================================
//						Display wndwdow function description
//==================================================================================
extern void	SetWndwDsScOSDFmtChk(U8 _pth);
extern void	SetWndwDsScOSDFmtRdo(U8 _pth);
extern void	SetWndwDsScOSDFmt(U8 _pth);
//extern void	InitWndwDsScVarLs(void);
extern void	ChngWndwDsScFmtLv(void);
extern void	InitWndwDsScFmtPsd8Ch(U8 _tgl);
extern void	SetWndwDsScFmtPsd8Ch(void);
extern void	SetWndwDsScFmtPsd8ChRc(void);
extern void	SetWndwDsScFmtLvPb(void);
extern void	SetWndwDsScFmtLv(void);
extern void	SetWndwDsScFmtFl(void);
extern void	CreateWndwDsSc(U8 _pth);
extern void	RunWndwDsSc(U8 _pth);//U8 _index_x, U8 _index_y)

