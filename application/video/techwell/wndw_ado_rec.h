//==================================================================================
//						Audio Record wndwdow definition & variable description
//==================================================================================
#define	WNDW_ADO_RC_POS_X		14
#define	WNDW_ADO_RC_POS_Y		4
#define	WNDW_ADO_RC_L			18
#define	WNDW_ADO_RC_H			14
#define	WNDW_ADO_RC_CLOS_X		(WNDW_ADO_RC_POS_X+WNDW_ADO_RC_L-1)
#define	WNDW_ADO_RC_CLOS_Y		(WNDW_ADO_RC_POS_Y)

#define	WNDW_ADO_RC_SR_RDO_8_X	(WNDW_ADO_RC_POS_X+2)
#define	WNDW_ADO_RC_SR_RDO_16_X	(WNDW_ADO_RC_SR_RDO_8_X+7)
#define	WNDW_ADO_RC_SR_RDO_Y		(WNDW_ADO_RC_POS_Y+3)

#define	WNDW_ADO_RC_BW_RDO_8_X	(WNDW_ADO_RC_POS_X+2)
#define	WNDW_ADO_RC_BW_RDO_16_X	(WNDW_ADO_RC_BW_RDO_8_X+7)
#define	WNDW_ADO_RC_BW_RDO_Y		(WNDW_ADO_RC_SR_RDO_Y+3)

#define	WNDW_ADO_RC_MC_RDO_X		(WNDW_ADO_RC_POS_X+2)
#define	WNDW_ADO_RC_MC_RDO_Y		(WNDW_ADO_RC_BW_RDO_Y+3)


//==================================================================================
/*
U8 	wndw_flag_ado = 0x00;
sbit	b_ado_smpl_rto = wndw_flag_ado^2;
sbit	b_ado_bit_wdth = wndw_flag_ado^1;
sbit	b_ado_mlt_ch = wndw_flag_ado^0;
//	*/


//==================================================================================
//						Audio Record wndwdow function description
//==================================================================================
extern void	SetWndwAdoRcOSDSrChk(U8 _pth, U8 _tgl);
extern void	SetWndwAdoRcOSDSr(U8 _pth);
extern void	SetWndwAdoRcOSDBwChk(U8 _pth, U8 _tgl);
extern void	SetWndwAdoRcOSDBw(U8 _pth);
extern void	SetWndwAdoRcOSDMcChk(U8 _pth, U8 _data);
extern void	SetWndwAdoRcOSDMc(U8 _pth);
extern void	CreateWndwAdoRc(U8 _pth);
extern void	RunWndwAdoRc(U8 _pth);


