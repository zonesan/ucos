//==================================================================================
//						Display wndwdow definition & variable description
//==================================================================================
#define	WNDW_DS_IN_POS_X			12
#define	WNDW_DS_IN_POS_Y			5 
#define	WNDW_DS_IN_L				20// 
#define	WNDW_DS_IN_H				13//
#define	WNDW_DS_IN_CLOS_X			(WNDW_DS_IN_POS_X+WNDW_DS_IN_L-1)
#define	WNDW_DS_IN_CLOS_Y			(WNDW_DS_IN_POS_Y)


#define	WNDW_DS_IN_RDO_TYPE_X			(WNDW_DS_IN_POS_X+1)
#define	WNDW_DS_IN_RDO_TYPE_Y			(WNDW_DS_IN_POS_Y+2)
#define	WNDW_DS_IN_RDO_DT_X			(WNDW_DS_IN_RDO_TYPE_X+1)
#define	WNDW_DS_IN_RDO_DT_Y			(WNDW_DS_IN_RDO_TYPE_Y+3)
//#define	WNDW_DS_IN_CHK_X0				(WNDW_DS_IN_RDO_TYPE_X+1)
#define	WNDW_DS_IN_CHK_RN_X				(WNDW_DS_IN_RDO_DT_X+1)
//#define	WNDW_DS_IN_CHK_DT_Y			(WNDW_DS_IN_RDO_TYPE_Y+3)
//#define	WNDW_DS_IN_CHK_RN_Y			(WNDW_DS_IN_CHK_DT_Y+1)
#define	WNDW_DS_IN_CHK_RN_Y			(WNDW_DS_IN_RDO_DT_Y+2)
//#define	WNDW_DS_IN_CHK_AT_Y			(WNDW_DS_IN_CHK_RN_Y+1)

#define	WNDW_DS_IN_TTL_X					(WNDW_DS_IN_RDO_DT_X)
#define	WNDW_DS_IN_DEC_X					(WNDW_DS_IN_TTL_X+9)
#define	WNDW_DS_IN_NUM_X				(WNDW_DS_IN_DEC_X+3)
#define	WNDW_DS_IN_INC_X					(WNDW_DS_IN_DEC_X+6)

#define	WNDW_DS_IN_HS_Y					(WNDW_DS_IN_CHK_RN_Y+2)
#define	WNDW_DS_IN_PX_Y					(WNDW_DS_IN_HS_Y+1)
#define	WNDW_DS_IN_VS_Y					(WNDW_DS_IN_PX_Y+1)
//#define	WNDW_DS_IN_LN_Y					(WNDW_DS_IN_VS_Y+1)


#define	IN_TYPE_REC					0
#define	IN_TYPE_DGTL				1
#define	IN_TYPE_ANLG				2

#define	IN_DT_AT						0
#define	IN_DT_MNL					1

#define	MSE_ON_OSD_HS			BIT0
#define	MSE_ON_OSD_PX			BIT1
#define	MSE_ON_OSD_VS			BIT2
#define	MSE_ON_OSD_LN			BIT3

//==================================================================================
//==================================================================================
extern U8 wndw_ds_in_type;

//extern U8 wndw_flg_ds;
//extern bit	b_ds_pb_cif;
//extern bit	b_ds_wth_rc;
////extern bit	b_ds_in_eror;
//extern bit	b_ds_in_run;
//extern bit	b_ds_in_auto;


//==================================================================================
//						Display wndwdow function description
//==================================================================================
extern void	SetWndwDsInOSDTypeRdo(U8 _pth);
extern void	SetWndwDsInOSDType(U8 _pth);
extern void	SetWndwDsInOSDAnlgRdo(U8 _pth);
extern void	SetWndwDsInOSDAnlgChk(U8 _pth);
extern void	SetWndwDsInOSDAnlg(U8 _pth);
extern void	SetWndwDsInOSDSpec(U8 _pth);
extern void	RunWndwDsInOSDArw(U8 _pth, U8 _osd_pos_x, U8 _osd_pos_y);
extern void	SetWndwDsInOSDNumHs(U8 _pth, U8 _data);
extern void	SetWndwDsInOSDNumPx(U8 _pth, U8 _data);
extern void	SetWndwDsInOSDNumVs(U8 _pth, U8 _data);
//extern void	SetWndwDsInOSDNumLn(U8 _pth, U8 _data);
extern void	SetWndwDsInOSDNum(U8 _pth);
extern void	InitWndwDsInFlg(void);
extern void	CreateWndwDsIn(U8 _pth);
extern void	RunWndwDsInDec(U8 _pth, U8 _osd_pos_y);
extern void	RunWndwDsInInc(U8 _pth, U8 _osd_pos_y);
extern void	RunWndwDsIn(U8 _pth);

