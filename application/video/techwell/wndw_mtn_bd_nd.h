//==================================================================================
//			Blind and Night Detect window definition & variable description
//==================================================================================
#define	WNDW_MTN_BDND_POS_X				12
#define	WNDW_MTN_BDND_POS_Y				5//8
#define	WNDW_MTN_BDND_L					21
#define	WNDW_MTN_BDND_H					13//7
#define	WNDW_MTN_BDND_CLOS_X			(WNDW_MTN_BDND_POS_X+WNDW_MTN_BDND_L-1)
#define	WNDW_MTN_BDND_CLOS_Y			(WNDW_MTN_BDND_POS_Y)

#define	WNDW_MTN_BDND_CH_DEC_X			(WNDW_MTN_BDND_POS_X+11)
#define	WNDW_MTN_BDND_CH_INC_X			(WNDW_MTN_BDND_CH_DEC_X+5)
#define	WNDW_MTN_BDND_CH_NUM_X			(WNDW_MTN_BDND_CH_DEC_X+3)
#define	WNDW_MTN_BDND_CH_NUM_Y			(WNDW_MTN_BDND_POS_Y+2)

#define	WNDW_MTN_BDND_SENS_TTL_X		(WNDW_MTN_BDND_POS_X+2)
#define	WNDW_MTN_BDND_SENS_DEC_X		(WNDW_MTN_BDND_POS_X+13)
#define	WNDW_MTN_BDND_SENS_INC_X		(WNDW_MTN_BDND_SENS_DEC_X+5)
#define	WNDW_MTN_BDND_SENS_NUM_X		(WNDW_MTN_BDND_SENS_DEC_X+3)

#define	WNDW_MTN_BDND_SENS_CL_Y		(WNDW_MTN_BDND_CH_NUM_Y+2)
#define	WNDW_MTN_BDND_SENS_LV_Y		(WNDW_MTN_BDND_SENS_CL_Y+1)

#define	WNDW_MTN_BDND_DT_X				(WNDW_MTN_BDND_POS_X+3)
#define	WNDW_MTN_BDND_DT_Y				(WNDW_MTN_BDND_SENS_LV_Y+3)

#define	MSE_ON_OSD_BDND_CH		BIT0
#define	MSE_ON_OSD_BDND_CL		BIT1
#define	MSE_ON_OSD_BDND_LV		BIT2



//==================================================================================
//					Blind and Night Detect window function description
//==================================================================================
extern void	RunWndwMtnBdNdOSDChArw(U8 _pth, U8 _osd_pos_x, U8 _osd_pos_y);
extern void	RunWndwMtnBdNdOSDArw(U8 _pth, U8 _osd_pos_x, U8 _osd_pos_y);

extern void	SetWndwMtnBdNdOSDChNum(U8 _pth);
extern void	SetWndwMtnBdNdOSDCh(U8 _pth);
extern U8	GetWndwMtnBdNdNum(U8 _osd_pos_y);
extern void	SetWndwMtnBdNdNum(U8 _osd_pos_y, U8 _data);
extern void	SetWndwMtnBdNdOSDNum(U8 _pth, U8 _osd_pos_y, U8 _data);
extern void	SetWndwMtnBdNdOSDNumAll(U8 _pth);
extern void	SetWndwMtnBdNdOSDDtBx(U8 _pth, U8 _ch);
extern void	SetWndwMtnBdNdOSDDt(U8 _pth);
extern void	SetWndwMtnBdNdOSD(U8 _pth);
extern void	CreateWndwMtnBdNd(U8 _pth);

extern void	RunWndwMtnBdNdCh(U8 _pth, U8 _osd_pos_x);
extern void	RunWndwMtnBdNdDec(U8 _pth, U8 _osd_pos_y);
extern void	RunWndwMtnBdNdInc(U8 _pth, U8 _osd_pos_y);
extern void	RunWndwMtnBdNd(U8 _pth);

