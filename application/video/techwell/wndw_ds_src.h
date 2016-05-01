//==================================================================================
//						Display wndwdow definition & variable description
//==================================================================================
#define	WNDW_DS_SR_POS_X				14
#define	WNDW_DS_SR_POS_Y				6 
#define	WNDW_DS_SR_L						18
#define	WNDW_DS_SR_H						11
#define	WNDW_DS_SR_CLOS_X				(WNDW_DS_SR_POS_X+WNDW_DS_SR_L-1)
#define	WNDW_DS_SR_CLOS_Y				(WNDW_DS_SR_POS_Y)

#define	WNDW_DS_SR_DEC_WN_X			(WNDW_DS_SR_POS_X+10)
#define	WNDW_DS_SR_INC_WN_X			(WNDW_DS_SR_DEC_WN_X+5)
#define	WNDW_DS_SR_NUM_WN_X			(WNDW_DS_SR_DEC_WN_X+3)
#define	WNDW_DS_SR_NUM_WN_Y			(WNDW_DS_SR_POS_Y+2)

#define	WNDW_DS_SR_RDO_X				(WNDW_DS_SR_POS_X+2)
#define	WNDW_DS_SR_RDO_LV_Y			(WNDW_DS_SR_NUM_WN_Y+3)
#define	WNDW_DS_SR_DEC_LV_X			(WNDW_DS_SR_RDO_X+3)
#define	WNDW_DS_SR_INC_LV_X			(WNDW_DS_SR_DEC_LV_X+7)
#define	WNDW_DS_SR_NUM_LV_X			(WNDW_DS_SR_DEC_LV_X+4)
#define	WNDW_DS_SR_NUM_LV_Y			(WNDW_DS_SR_RDO_LV_Y+1)
#define	WNDW_DS_SR_RDO_PB_Y			(WNDW_DS_SR_NUM_LV_Y+1)
#define	WNDW_DS_SR_DEC_PB_X			(WNDW_DS_SR_RDO_X+3)
#define	WNDW_DS_SR_INC_PB_X			(WNDW_DS_SR_DEC_PB_X+7)
#define	WNDW_DS_SR_NUM_PB_X			(WNDW_DS_SR_DEC_PB_X+4)
#define	WNDW_DS_SR_NUM_PB_Y			(WNDW_DS_SR_RDO_PB_Y+1)

#define	WNDW_DS_SR_CHK_X				(WNDW_DS_SR_POS_X+1)
#define	WNDW_DS_SR_CHK_Y				(WNDW_DS_SR_NUM_PB_Y+2)

#define	MSE_ON_OSD_DS_SR_WN		BIT0
#define	MSE_ON_OSD_DS_SR_LV		BIT1
#define	MSE_ON_OSD_DS_SR_PB			BIT2


//==================================================================================
//						Display wndwdow function description
//==================================================================================
#if	defined(__16CH__)
extern U8	GetWndwDsSrPbOfst();
#endif
//==================================================================================
//==================================================================================
extern void	RunWndwDsSrOSDArwWn(U8 _pth, U8 _osd_pos_x, U8 _osd_pos_y);
extern void	RunWndwDsSrOSDArwLv(U8 _pth, U8 _osd_pos_x, U8 _osd_pos_y);
extern void	RunWndwDsSrOSDArwPb(U8 _pth, U8 _osd_pos_x, U8 _osd_pos_y);
extern void	SetWndwDsSrOSDWn(U8 _pth);
extern void	SetWndwDsSrOSDLvNum(U8 _pth, U8 _data);
extern void	SetWndwDsSrOSDLv(U8 _pth);
extern void	SetWndwDsSrOSDPbNum(U8 _pth, U8 _data);
extern void	SetWndwDsSrOSDPb(U8 _pth);
extern void	SetWndwDsSrOSDEnhChk(U8 _pth);
extern void	SetWndwDsSrOSDEnh(U8 _pth);
//extern void	SelWndwDsSrSrc(void);
extern void	CreateWndwDsSr(U8 _pth);
extern void	RunWndwDsSrDec(U8 _pth, U8 _osd_pos_y);
extern void	RunWndwDsSrInc(U8 _pth, U8 _osd_pos_y);
extern void	RunWndwDsSr(U8 _pth);

