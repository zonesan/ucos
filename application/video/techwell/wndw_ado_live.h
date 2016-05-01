//==================================================================================
//						Audio Live wndwdow definition & variable description
//==================================================================================
#define	WNDW_ADO_LV_POS_X		13
#define	WNDW_ADO_LV_POS_Y		6
#define	WNDW_ADO_LV_L			19
#define	WNDW_ADO_LV_H			12
#define	WNDW_ADO_LV_CLOS_X		(WNDW_ADO_LV_POS_X+WNDW_ADO_LV_L-1)
#define	WNDW_ADO_LV_CLOS_Y		(WNDW_ADO_LV_POS_Y)

#define	WNDW_ADO_LV_SEL_RDO_0_X		(WNDW_ADO_LV_POS_X+2)
#define	WNDW_ADO_LV_SEL_RDO_1_X		(WNDW_ADO_LV_SEL_RDO_0_X+4)
#define	WNDW_ADO_LV_SEL_RDO_2_X		(WNDW_ADO_LV_SEL_RDO_1_X+4)
#define	WNDW_ADO_LV_SEL_RDO_3_X		(WNDW_ADO_LV_SEL_RDO_2_X+4)
#define	WNDW_ADO_LV_SEL_RDO_PB_X		(WNDW_ADO_LV_SEL_RDO_0_X)
#define	WNDW_ADO_LV_SEL_RDO_MIX_X	(WNDW_ADO_LV_SEL_RDO_2_X)
#define	WNDW_ADO_LV_SEL_RDO_03_Y		(WNDW_ADO_LV_POS_Y+3)
#define	WNDW_ADO_LV_SEL_RDO_47_Y		(WNDW_ADO_LV_SEL_RDO_03_Y+1)
#define	WNDW_ADO_LV_SEL_RDO_8B_Y		(WNDW_ADO_LV_SEL_RDO_47_Y+1)
#define	WNDW_ADO_LV_SEL_RDO_CF_Y		(WNDW_ADO_LV_SEL_RDO_8B_Y+1)
#define	WNDW_ADO_LV_SEL_RDO_PB_Y		(WNDW_ADO_LV_SEL_RDO_CF_Y+1)

//#define	WNDW_ADO_LV_MIX_CHK_X			(WNDW_ADO_LV_POS_X+2)
//#define	WNDW_ADO_LV_MIX_DEC_X			(WNDW_ADO_LV_MIX_CHK_X+10)
//#define	WNDW_ADO_LV_MIX_NUM_X			(WNDW_ADO_LV_MIX_DEC_X+3)
//#define	WNDW_ADO_LV_MIX_INC_X			(WNDW_ADO_LV_MIX_DEC_X+5)
//#define	WNDW_ADO_LV_MIX_NUM_Y			(WNDW_ADO_LV_SEL_RDO_PB_Y+3)
#define	WNDW_ADO_LV_BTN_X			(WNDW_ADO_LV_POS_X+5)
#define	WNDW_ADO_LV_BTN_Y			(WNDW_ADO_LV_SEL_RDO_PB_Y+3)


#if defined(__4CH__)
	#define	ADO_SEL_PB					16
#elif defined(__16CH__)
	#define	ADO_SEL_PB					19
#endif
#define	ADO_SEL_MIX						20//17//

//#define	MSE_ON_OSD_CH1			BIT0
//#define	MSE_ON_OSD_CH2			BIT1
//#define	MSE_ON_OSD_CH3			BIT2
//#define	MSE_ON_OSD_CH4			BIT3
//#define	MSE_ON_OSD_PB				BIT4

//==================================================================================
extern U8 wndw_ado_lv_sel;
//extern U8 wndw_ado_lv_mute;


//==================================================================================
//						Audio Live wndwdow function description
//==================================================================================
extern void	SetWndwAdoLvOSDSelRdo(U8 _pth);
extern void	SetWndwAdoLvOSDSel(U8 _pth);
extern void	SetWndwAdoLvOSDMix(U8 _pth);
extern void	CreateWndwAdoLv(U8 _pth);
extern void	RunWndwAdoLvSel(U8 _pth);
extern void	RunWndwAdoLv(U8 _pth);

