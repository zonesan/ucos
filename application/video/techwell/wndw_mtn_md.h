//==================================================================================
//						Motion and 2Dbox window definition & variable description
//==================================================================================
#define	WNDW_MTN_MD_POS_X				13
#define	WNDW_MTN_MD_POS_Y				6//4// 
#define	WNDW_MTN_MD_L					20
#define	WNDW_MTN_MD_H					12//16//
#define	WNDW_MTN_MD_CLOS_X			(WNDW_MTN_MD_POS_X+WNDW_MTN_MD_L-1)
#define	WNDW_MTN_MD_CLOS_Y			(WNDW_MTN_MD_POS_Y)

#define	WNDW_MTN_MD_CH_DEC_X			(WNDW_MTN_MD_POS_X+11)
#define	WNDW_MTN_MD_CH_INC_X			(WNDW_MTN_MD_CH_DEC_X+5)
#define	WNDW_MTN_MD_CH_NUM_X			(WNDW_MTN_MD_CH_DEC_X+3)
#define	WNDW_MTN_MD_CH_NUM_Y			(WNDW_MTN_MD_POS_Y+2)

#define	WNDW_MTN_MD_SENS_TTL_X		(WNDW_MTN_MD_POS_X+2)
#define	WNDW_MTN_MD_SENS_DEC_X		(WNDW_MTN_MD_POS_X+12)
#define	WNDW_MTN_MD_SENS_INC_X		(WNDW_MTN_MD_SENS_DEC_X+5)
#define	WNDW_MTN_MD_SENS_NUM_X		(WNDW_MTN_MD_SENS_DEC_X+3)
#define	WNDW_MTN_MD_SENS_CL_Y		(WNDW_MTN_MD_CH_NUM_Y+2)
#define	WNDW_MTN_MD_SENS_LV_Y		(WNDW_MTN_MD_SENS_CL_Y+1)
#define	WNDW_MTN_MD_SENS_SP_Y		(WNDW_MTN_MD_SENS_LV_Y+1)
#define	WNDW_MTN_MD_SENS_TM_Y		(WNDW_MTN_MD_SENS_SP_Y+1)
#define	WNDW_MTN_MD_SENS_VL_Y		(WNDW_MTN_MD_SENS_TM_Y+1)

#define	WNDW_MTN_MD_CHK_X				(WNDW_MTN_MD_POS_X+1)
#define	WNDW_MTN_MD_CHK_MTN_Y		(WNDW_MTN_MD_SENS_VL_Y+2)
#define	WNDW_MTN_MD_CHK_MSK_Y		(WNDW_MTN_MD_CHK_MTN_Y+1)


#define	MSE_ON_OSD_MD_CH				BIT0
#define	MSE_ON_OSD_MD_CL				BIT1
#define	MSE_ON_OSD_MD_LV				BIT2
#define	MSE_ON_OSD_MD_SP				BIT3
#define	MSE_ON_OSD_MD_TM				BIT4
#define	MSE_ON_OSD_MD_VL				BIT5


//==================================================================================
//==================================================================================
extern U8 wndw_mtn_md_blck_strt;

//extern U8 	wndw_flag_mtn;
//extern int	b_mtn_dspl;// = wndw_flag_mtn^7;


//==================================================================================
//						Motion and 2Dbox window function description
//==================================================================================
extern void Enable2DBox(U8 _pth, U8 _dvc, U8 _ch);
extern void Disable2DBoxDsply(U8 _dvc, U8 _ch);
//extern void	Set2DBoxCtrl(U8 _dvc, U8 _ch, U8 _property, U8 _tgl);
extern void Aply2DBoxCh(U8 _dvc, U8 _ch);
extern void Set2DBoxPos(U8 _dvc, U8 _ch, U16 _boxhl, U16 _boxvt);
extern void Set2DBoxSize(U8 _dvc, U8 _ch, U8 _boxhw, U8 _boxvw);
extern void Set2DBox(U8 _dvc, U8 _ch, U16 _boxhl, U8 _boxhw, U16 _boxvt, U8 _boxvw);
extern void Set2DBoxCell(U8 _dvc, U8 _ch, U8 _lngth, U8 _hght);
//extern void Set2DBoxCur(U8 _dvc, U8 _ch, U8 _pos_x, U8 _pos_y);
extern void	Set2DBoxTable(U8 _pth, U8 _dvc, U8 _ch, U8 _osd_pos_x, U8 _osd_pos_y, U8 _lngth, U8 _hght, U8 _osd_per_cell);
extern void	SetMskBlck(U8 _dvc, U8 _ch, U8 _blk_hl, U8 _blk_vt, U8 _blk_wdth, U8 _blk_hght);

extern void	RunWndwMtnMdOSDChArw(U8 _pth, U8 _osd_pos_x, U8 _osd_pos_y);
extern void	RunWndwMtnMdOSDArw(U8 _pth, U8 _osd_pos_x, U8 _osd_pos_y);

extern void	SetWndwMtnMdOSDChNum(U8 _pth);
extern void	SetWndwMtnMdOSDCh(U8 _pth);
extern U8	GetWndwMtnMdNum(U8 _osd_pos_y);
extern void	SetWndwMtnMdNum(U8 _osd_pos_y, U8 _data);
extern void	SetWndwMtnMdOSDNum(U8 _pth, U8 _osd_pos_y, U8 _data);
extern void	SetWndwMtnMdOSDChk(U8 _pth, U8 _osd_pos_y);
//extern void	SetWndwMtnMdOSDDsplChk(U8 _pth);
//extern void	SetWndwMtnMdOSDDspl(U8 _pth);
extern void	SetWndwMtnMdOSDNumAll(U8 _pth);
extern void	SetWndwMtnMdOSD(U8 _pth);
extern void	CreateWndwMtnMd(U8 _pth);

extern void	RunWndwMtnMdCh(U8 _pth, U8 _osd_pos_x);
extern void	RunWndwMtnMdDec(U8 _pth, U8 _osd_pos_y);
extern void	RunWndwMtnMdInc(U8 _pth, U8 _osd_pos_y);
extern void	RunWndwMtnMd(U8 _pth);

