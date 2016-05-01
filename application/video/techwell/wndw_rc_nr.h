//==================================================================================
//				Noise Reduction wndwdow definition & variable description
//==================================================================================
#define	WNDW_RC_NR_POS_X				13
#define	WNDW_RC_NR_POS_Y				4 
#define	WNDW_RC_NR_L					19
#define	WNDW_RC_NR_H					16
#define	WNDW_RC_NR_CLOS_X				(WNDW_RC_NR_POS_X+WNDW_RC_NR_L-1)
#define	WNDW_RC_NR_CLOS_Y				(WNDW_RC_NR_POS_Y)

#define	WNDW_RC_NR_RDO_X				(WNDW_RC_NR_POS_X+2)
#define	WNDW_RC_NR_RDO_Y				(WNDW_RC_NR_POS_Y+3)

#define	WNDW_RC_NR_BMP_X				((WNDW_RC_NR_RDO_X+10)<<1)//((WNDW_RC_NR_RDO_X+11)<<1)
#define	WNDW_RC_NR_BMP_Y				(WNDW_RC_NR_RDO_Y*10 + 3)//5)

#define	WNDW_RC_NR_CHK_ADPT_X		(WNDW_RC_NR_POS_X+1)
#define	WNDW_RC_NR_CHK_ADPT_Y		(WNDW_RC_NR_RDO_Y+5)

#define	WNDW_RC_NR_CHK_CH_X			(WNDW_RC_NR_POS_X+2)
#define	WNDW_RC_NR_CHK_CH07_Y		(WNDW_RC_NR_CHK_ADPT_Y+4)
#define	WNDW_RC_NR_CHK_CH8F_Y		(WNDW_RC_NR_CHK_CH07_Y+2)

#define	MSE_ON_OSD_NR_CH				BIT0
//#define	MSE_ON_OSD_NR_THR				BIT1
//#define	MSE_ON_OSD_NR_SLP				BIT2

//#define	RC_NR_BMP_OFST_H				2
//#define	RC_NR_BMP_OFST_V				8


//==================================================================================
//				Noise Reduction wndwdow function description
//==================================================================================
extern void	SetWndwRcNrOSDWnRdo(U8 _pth);
extern void	SetWndwRcNrOSDTyp(void);
extern void	SetWndwRcNrOSDWn(U8 _pth);
extern void	SetWndwRcNrOSDAdptChk(U8 _pth);
extern void	SetWndwRcNrOSDAdpt(U8 _pth);
extern void	SetWndwRcNrOSDChChk(U8 _pth, U8 _ch);
extern void	SetWndwRcNrOSDCh(U8 _pth);
extern void	CreateWndwRcNr(U8 _pth);
extern void	RunWndwRcNr(U8 _pth);

