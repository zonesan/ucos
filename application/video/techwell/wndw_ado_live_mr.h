//==================================================================================
//						Audio Live wndwdow definition & variable description
//==================================================================================
#define	WNDW_ADO_LV_MR_POS_X		(WNDW_ADO_LV_POS_X-4)//13
#define	WNDW_ADO_LV_MR_POS_Y		(WNDW_ADO_LV_POS_Y-2)//6
#define	WNDW_ADO_LV_MR_L			20
#define	WNDW_ADO_LV_MR_H			11
#define	WNDW_ADO_LV_MR_CLOS_X		(WNDW_ADO_LV_MR_POS_X+WNDW_ADO_LV_MR_L-1)
#define	WNDW_ADO_LV_MR_CLOS_Y		(WNDW_ADO_LV_MR_POS_Y)

//#define	WNDW_ADO_LV_MR_SEL_RDO_1_X		(WNDW_ADO_LV_MR_POS_X+2)
//#define	WNDW_ADO_LV_MR_SEL_RDO_2_X		(WNDW_ADO_LV_MR_SEL_RDO_1_X+4)
//#define	WNDW_ADO_LV_MR_SEL_RDO_3_X		(WNDW_ADO_LV_MR_SEL_RDO_2_X+4)
//#define	WNDW_ADO_LV_MR_SEL_RDO_4_X		(WNDW_ADO_LV_MR_SEL_RDO_3_X+4)
//#define	WNDW_ADO_LV_MR_SEL_RDO_PB_X		(WNDW_ADO_LV_MR_SEL_RDO_1_X)
//#define	WNDW_ADO_LV_MR_SEL_RDO_X	(WNDW_ADO_LV_MR_SEL_RDO_3_X)
//#define	WNDW_ADO_LV_MR_SEL_RDO_1_Y		(WNDW_ADO_LV_MR_POS_Y+3)
//#define	WNDW_ADO_LV_MR_SEL_RDO_PB_Y		(WNDW_ADO_LV_MR_SEL_RDO_1_Y+1)

#define	WNDW_ADO_LV_MR_CHK_X0			(WNDW_ADO_LV_MR_POS_X+1)
#define	WNDW_ADO_LV_MR_DEC_X0			(WNDW_ADO_LV_MR_CHK_X0+4)
#define	WNDW_ADO_LV_MR_NUM_X0			(WNDW_ADO_LV_MR_DEC_X0+2)
#define	WNDW_ADO_LV_MR_INC_X0			(WNDW_ADO_LV_MR_DEC_X0+3)
#define	WNDW_ADO_LV_MR_CHK_X1			(WNDW_ADO_LV_MR_CHK_X0+10)
#define	WNDW_ADO_LV_MR_DEC_X1			(WNDW_ADO_LV_MR_CHK_X1+4)
#define	WNDW_ADO_LV_MR_NUM_X1			(WNDW_ADO_LV_MR_DEC_X1+2)
#define	WNDW_ADO_LV_MR_INC_X1			(WNDW_ADO_LV_MR_DEC_X1+3)
#define	WNDW_ADO_LV_MR_CHK_Y			(WNDW_ADO_LV_MR_POS_Y+2)
#define	WNDW_ADO_LV_MR_PB_Y			(WNDW_ADO_LV_MR_CHK_Y+8)

//#define	ADO_SEL_PB						16
//#define	ADO_SEL_MIX						20//17//

//#define	MSE_ON_OSD_CH1			BIT0
//#define	MSE_ON_OSD_CH2			BIT1
//#define	MSE_ON_OSD_CH3			BIT2
//#define	MSE_ON_OSD_CH4			BIT3
//#define	MSE_ON_OSD_PB				BIT0

//==================================================================================
//extern U8 idata	wndw_ado_lv_mr_sel;
//extern U8 idata	wndw_ado_lv_mr_mute;


//==================================================================================
//						Audio Live wndwdow function description
//==================================================================================
extern void	RunWndwAdoLvMrOSDPbArw(U8 _pth_prp, U8 _osd_pos_x, U8 _osd_pos_y);
extern void	SetWndwAdoLvMrOSDArw(U8 _pth, U8 _ch);
extern void	SetWndwAdoLvMrOSDPbChk(U8 _pth);
extern void	SetWndwAdoLvMrOSDPbNum(U8 _pth);
extern void	SetWndwAdoLvMrOSDChk(U8 _pth, U8 _ch);
extern void	SetWndwAdoLvMrOSDNum(U8 _pth, U8 _ch);
extern void	SetWndwAdoLvMrOSD(U8 _pth);
extern U8	GetWndwAdoLvMr(U8 _dvc, U8 _ch);
extern void	SetWndwAdoLvMr(U8 _dvc, U8 _ch, U8 _dat);
extern void	CreateWndwAdoLvMr(U8 _pth);
extern void	RunWndwAdoLvMr(U8 _pth);
