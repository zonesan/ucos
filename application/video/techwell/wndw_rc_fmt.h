//==================================================================================
//						Display wndwdow definition & variable description
//==================================================================================
#define	WNDW_RC_FM_POS_X			15
#define	WNDW_RC_FM_POS_Y			5 
#define	WNDW_RC_FM_L				15
#define	WNDW_RC_FM_H				14
#define	WNDW_RC_FM_CLOS_X			(WNDW_RC_FM_POS_X+WNDW_RC_FM_L-1)
#define	WNDW_RC_FM_CLOS_Y			(WNDW_RC_FM_POS_Y)

#define	WNDW_RC_FM_RDO_X				(WNDW_RC_FM_POS_X+1)
//#define	WNDW_RC_FM_RDO_X0				(WNDW_RC_FM_POS_X+1)
//#define	WNDW_RC_FM_RDO_X1				(WNDW_RC_FM_RDO_X0+1)
//
//#define	WNDW_RC_FM_RDO_CIF_Y			(WNDW_RC_FM_POS_Y+2)
#define	WNDW_RC_FM_RDO_CIF_FR_Y		(WNDW_RC_FM_POS_Y+2)
#define	WNDW_RC_FM_RDO_CIF_FL_Y		(WNDW_RC_FM_RDO_CIF_FR_Y+1)

//#define	WNDW_RC_FM_RDO_MUX_Y			(WNDW_RC_FM_RDO_CIF_FL_Y+1)
#define	WNDW_RC_FM_RDO_MUX_FR_Y		(WNDW_RC_FM_RDO_CIF_FL_Y+1)
#define	WNDW_RC_FM_RDO_MUX_FL_Y		(WNDW_RC_FM_RDO_MUX_FR_Y+1)
#define	WNDW_RC_FM_RDO_MUX_LV_Y		(WNDW_RC_FM_RDO_MUX_FL_Y+1)

#define	WNDW_RC_FM_PRD_X				(WNDW_RC_FM_RDO_X)
#define	WNDW_RC_FM_PRD_Y				(WNDW_RC_FM_RDO_MUX_LV_Y+2)

#define	WNDW_RC_FM_DEC_X				(WNDW_RC_FM_PRD_X+7)
#define	WNDW_RC_FM_NUM_X				(WNDW_RC_FM_DEC_X+1)
#define	WNDW_RC_FM_INC_X					(WNDW_RC_FM_DEC_X+5)

#define	WNDW_RC_FM_BTN_X				(WNDW_RC_FM_RDO_X+2)
#define	WNDW_RC_FM_BTN_QUE_Y			(WNDW_RC_FM_PRD_Y+2)
#define	WNDW_RC_FM_BTN_LOSS_Y		(WNDW_RC_FM_BTN_QUE_Y+2)

#define	FM_BOX_QUE					(BOX_BTN8)
#define	FM_BOX_LOSS				(BOX_BTN7)

#define	MSE_ON_OSD_PRD		BIT0



//==================================================================================
extern int	b_rc_fm_que;
extern int	b_rc_fm_loss;
extern int	b_rc_fm_ls_sk;
//#define	FM_TYPE_CIF		0
//#define	FM_TYPE_MUX	1
//extern int	b_rc_fm_type;
/*	//...	update:050705 Rev2.0
#if	defined(__16CH__)
sbit	b_rc_fm_que_cnt_rst = wndw_flg_rc_fm^4;
#endif
//	*/
#define	CIF_TYPE_FR				0//0
#define	CIF_TYPE_FL				1//1
#define	MUX_TYPE_FR				2//1
#define	MUX_TYPE_FL				3//2
#define	MUX_TYPE_LV				4//0

//extern U8 idata	wndw_rc_fm_cif_type;
//extern U8 idata	wndw_rc_fm_mux_type;
extern U8 wndw_rc_fm_type;


//==================================================================================
//==================================================================================
//void	CreateWndwRcFmLs();
//void	RunWndwRcFmLs(U8,U8);
//void	CreateWndwRcFmQu();
//void	RunWndwRcFmQu(U8,U8);

//==================================================================================
//						Display wndwdow function description
//==================================================================================
//extern void	SetWndwRcFmOSDType(U8 _pth);
extern void	SetWndwRcFmOSDCifRdo(U8 _pth);
extern void	SetWndwRcFmOSDCif(U8 _pth);
extern void	SetWndwRcFmOSDMuxRdo(U8 _pth);
extern void	SetWndwRcFmOSDMux(U8 _pth);
extern void	SetWndwRcFmOSDBtn(U8 _pth);
extern void	RunWndwRcFmOSDArw(U8 _pth, U8 _osd_pos_x_, U8 _osd_pos_y_);
extern void	SetWndwRcFmOSDNum(U8 _pth, U16 _data);
extern U16	GetWndwRcFmPrd(void);
extern void	SetWndwRcFmPrd(U16 _data);
extern void	SetWndwRcFmOSDPrd(U8 _pth);
extern void	SetWndwRcFmTypeCif(void);
extern void	SetWndwRcFmTypeMux(void);
extern void	SetWndwRcFmType(void);
extern void	CreateWndwRcFm(U8 _pth);
extern void	RunWndwRcFmType(U8 _pth);//U8 _osd_pos_x_, U8 _osd_pos_y_)
extern void	RunWndwRcFm(U8 _pth);//U8 _osd_pos_x_, U8 _osd_pos_y_)

