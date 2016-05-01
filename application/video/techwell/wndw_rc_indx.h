//==================================================================================
//						Display wndwdow definition & variable description
//==================================================================================
#define	WNDW_RC_IN_POS_X			12
#define	WNDW_RC_IN_POS_Y			5 
#define	WNDW_RC_IN_L				21
#define	WNDW_RC_IN_H				13
#define	WNDW_RC_IN_CLOS_X			(WNDW_RC_IN_POS_X+WNDW_RC_IN_L-1)
#define	WNDW_RC_IN_CLOS_Y			(WNDW_RC_IN_POS_Y)

#define	WNDW_RC_IN_CHK_TYPE_X		(WNDW_RC_IN_POS_X+1)
#define	WNDW_RC_IN_CHK_TYPE_Y		(WNDW_RC_IN_POS_Y+2)
#define	WNDW_RC_IN_CHK_X0				(WNDW_RC_IN_CHK_TYPE_X+1)
#define	WNDW_RC_IN_CHK_RN_Y			(WNDW_RC_IN_CHK_TYPE_Y+3)
#define	WNDW_RC_IN_CHK_DT_Y			(WNDW_RC_IN_CHK_RN_Y+1)
#define	WNDW_RC_IN_CHK_UR_Y			(WNDW_RC_IN_CHK_DT_Y+1)

#define	WNDW_RC_IN_DEC_X				(WNDW_RC_IN_CHK_X0+10)
#define	WNDW_RC_IN_NUM_X				(WNDW_RC_IN_DEC_X+3)
#define	WNDW_RC_IN_INC_X				(WNDW_RC_IN_DEC_X+6)

#define	WNDW_RC_IN_HS_Y				(WNDW_RC_IN_CHK_UR_Y+2)
#define	WNDW_RC_IN_PX_Y					(WNDW_RC_IN_HS_Y+1)
#define	WNDW_RC_IN_VS_Y					(WNDW_RC_IN_PX_Y+1)
//#define	WNDW_RC_IN_LN_Y				(WNDW_RC_IN_VS_Y+1)

#define	IN_TYPE_RC				0
#define	IN_TYPE_DGTL				1
#define	IN_TYPE_ANLG				2

#define	IN_BOX_DECO				(BOX_BTN8)

#define	MSE_ON_OSD_HS			BIT0
#define	MSE_ON_OSD_PX			BIT1
#define	MSE_ON_OSD_VS			BIT2
#define	MSE_ON_OSD_LN			BIT3

//==================================================================================
extern int	b_rc_in_bnd;
extern int	b_rc_in_dgtl;
extern int	b_rc_in_anlg;
extern int	b_rc_in_dtct;
extern int	b_rc_in_usr;
extern int	b_rc_in_run;



//==================================================================================
//						Display wndwdow function description
//==================================================================================
extern void	SetWndwRcInOSDType(U8 _pth);
extern void	SetWndwRcInOSDAnlgChk(U8 _pth);
extern void	SetWndwRcInOSDAnlg(U8 _pth);
extern void	SetWndwRcInOSDSpec(U8 _pth);
extern void	RunWndwRcInOSDArwH(U8 _pth, U8 _osd_pos_x, U8 _osd_pos_y);
extern void	RunWndwRcInOSDArwV(U8 _pth, U8 _osd_pos_x, U8 _osd_pos_y);
extern void	SetWndwRcInOSDNumHs(U8 _pth, U8 _data);
extern void	SetWndwRcInOSDNumPx(U8 _pth, U8 _data);
extern void	SetWndwRcInOSDNumVs(U8 _pth, U8 _data);
//extern void	SetWndwRcInOSDNumLn(U8 _pth, U8 _data);
extern void	SetWndwRcInOSDNum(U8 _pth);
extern void	InitWndwRcInFlg(void);
extern void	CreateWndwRcIn(U8 _pth);
extern void	RunWndwRcInDec(U8 _pth, U8 _osd_pos_y);
extern void	RunWndwRcInInc(U8 _pth, U8 _osd_pos_y);
extern void	RunWndwRcIn(U8 _pth);//U8 _osd_pos_x, U8 _osd_pos_y)

