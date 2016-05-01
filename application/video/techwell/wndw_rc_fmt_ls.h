//==================================================================================
//						Display wndwdow definition & variable description
//==================================================================================
#define	WNDW_RC_FM_LS_POS_X			(WNDW_RC_FM_BTN_X+3)
#define	WNDW_RC_FM_LS_POS_Y			(WNDW_RC_FM_POS_Y-2)
#define	WNDW_RC_FM_LS_L					19
#define	WNDW_RC_FM_LS_H					14
#define	WNDW_RC_FM_LS_CLOS_X			(WNDW_RC_FM_LS_POS_X+WNDW_RC_FM_LS_L-1)
#define	WNDW_RC_FM_LS_CLOS_Y			(WNDW_RC_FM_LS_POS_Y)

#define	WNDW_RC_FM_LS_RDO_MD_X			(WNDW_RC_FM_LS_POS_X+1)
#define	WNDW_RC_FM_LS_RDO_MD_Y			(WNDW_RC_FM_LS_POS_Y+2)
#define	WNDW_RC_FM_LS_RDO_SK_X			(WNDW_RC_FM_LS_RDO_MD_X+1)
#define	WNDW_RC_FM_LS_RDO_SK_Y			(WNDW_RC_FM_LS_RDO_MD_Y+4)

#define	WNDW_RC_FM_LS_SK_CH_X			(WNDW_RC_FM_LS_RDO_MD_X+1)
#define	WNDW_RC_FM_LS_SK_CH07_Y			(WNDW_RC_FM_LS_RDO_SK_Y+3)
#define	WNDW_RC_FM_LS_SK_CHK07_Y		(WNDW_RC_FM_LS_SK_CH07_Y+1)
#define	WNDW_RC_FM_LS_SK_CH8F_Y			(WNDW_RC_FM_LS_SK_CHK07_Y+1)
#define	WNDW_RC_FM_LS_SK_CHK8F_Y		(WNDW_RC_FM_LS_SK_CH8F_Y+1)

//#define	LOSS_BOX_FRM			(BOX_BTN6)
//#define	LOSS_BOX_TTL				(BOX_BTN5)
//#define	LOSS_BOX_QUIT			(BOX_BTN4)

#define	LOSS_MD_BYPS		0
#define	LOSS_MD_HOLD		1
#define	LOSS_MD_BLUE		2
#define	LOSS_MD_SKIP			3

#define	LOSS_SKIP_AUTO			0
#define	LOSS_SKIP_MANL			1


//==================================================================================
//==================================================================================
extern U8	wndw_rc_fm_ls_md;
extern U16 	wndw_rc_fm_ls_sk_mn;


//==================================================================================
//						Display wndwdow function description
//==================================================================================
extern void	SetWndwRcFmLsOSDMd(void);
extern void	SetWndwRcFmLsOSDSk(void);
extern void	SetWndwRcFmLsOSDSkMn(U16 _data);
//extern U16	GetWndwRcFmLsSkMn(void);
extern void	SetWndwRcFmLsSkMn(U16 _data);
extern void	SetWndwRcFmLsMd(void);
extern void	InitWndwRcFmVarLsMd(void);
extern void	CreateWndwRcFmLs(void);
extern void	RunWndwRcFmLs(void);//U8 _osd_pos_x_, U8 _osd_pos_y_)

