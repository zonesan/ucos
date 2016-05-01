//==================================================================================
//						Audio Gain window definition & variable description
//==================================================================================
#define	WNDW_ADO_GN_POS_X		13
#define	WNDW_ADO_GN_POS_Y		8
#define	WNDW_ADO_GN_L			18
#define	WNDW_ADO_GN_H			9
#define	WNDW_ADO_GN_CLOS_X		(WNDW_ADO_GN_POS_X+WNDW_ADO_GN_L-1)
#define	WNDW_ADO_GN_CLOS_Y		(WNDW_ADO_GN_POS_Y)

#define	WNDW_ADO_GN_TTL_X				(WNDW_ADO_GN_POS_X+2)
//#define	WNDW_ADO_GN_IG_CH_X			(WNDW_ADO_GN_TTL_X)
#define	WNDW_ADO_GN_IG_CH_Y			(WNDW_ADO_GN_POS_Y+3)
#define	WNDW_ADO_GN_IG_CH_DEC_X		(WNDW_ADO_GN_TTL_X+8)
#define	WNDW_ADO_GN_IG_CH_NUM_X		(WNDW_ADO_GN_IG_CH_DEC_X+3)
#define	WNDW_ADO_GN_IG_CH_INC_X		(WNDW_ADO_GN_IG_CH_DEC_X+5)
#define	WNDW_ADO_GN_IG_CH_NUM_Y		(WNDW_ADO_GN_IG_CH_Y)

#define	WNDW_ADO_GN_IG_DEC_X			(WNDW_ADO_GN_IG_CH_DEC_X)
#define	WNDW_ADO_GN_IG_NUM_X			(WNDW_ADO_GN_IG_DEC_X+3)
#define	WNDW_ADO_GN_IG_INC_X			(WNDW_ADO_GN_IG_DEC_X+5)
#define	WNDW_ADO_GN_IG_NUM_Y			(WNDW_ADO_GN_IG_CH_Y+1)

#define	WNDW_ADO_GN_OG_DEC_X			(WNDW_ADO_GN_IG_CH_DEC_X)
#define	WNDW_ADO_GN_OG_NUM_X			(WNDW_ADO_GN_OG_DEC_X+3)
#define	WNDW_ADO_GN_OG_INC_X			(WNDW_ADO_GN_OG_DEC_X+5)
#define	WNDW_ADO_GN_OG_NUM_Y			(WNDW_ADO_GN_IG_NUM_Y+3)


#define	MSE_ON_OSD_IG_CH		BIT0
//#define	MSE_ON_OSD_IG_CH2		BIT1
//#define	MSE_ON_OSD_IG_CH3		BIT2
//#define	MSE_ON_OSD_IG_CH4		BIT3
#define	MSE_ON_OSD_IG			BIT1
#define	MSE_ON_OSD_OG			BIT2


//==================================================================================
//						Audio Gain window function description
//==================================================================================
extern void	SetWndwAdoGnOSDIgChNum(U8 _pth);
extern void	SetWndwAdoGnOSDIgNum(U8 _pth, U8 _dat);
extern void	SetWndwAdoGnOSDIg(U8 _pth);
extern void	SetWndwAdoGnOSDOgNum(U8 _pth, U8 _dat);
extern void	SetWndwAdoGnOSDOg(U8 _pth);
extern U8	GetWndwAdoGnIg(void);
extern void	SetWndwAdoGnIg(U8 _dat);
extern void	CreateWndwAdoGn(U8 _pth);
extern void	RunWndwAdoGnDec(U8 _pth, U8 _osd_pos_y);
extern void	RunWndwAdoGnInc(U8 _pth, U8 _osd_pos_y);
extern void	RunWndwAdoGn(U8 _pth);

