//==================================================================================
//						Register window definition & variable description
//==================================================================================
//	on 8x10 font : 1 OSD's BOXHL=8, 1 OSD's BOXHW=4, 1 OSD's BOXVT=11, 1 OSD's BOXVW=5
#if	defined(__WNDW_RG_EV__)
	#define	WNDW_RG_NUM				6//				//... mux number -> 8
	
	#define	WNDW_RG_POS_X				8
	#define	WNDW_RG_POS_Y				7// 
	#define	WNDW_RG_L						29
	#define	WNDW_RG_H						(3+WNDW_RG_NUM)
	#define	WNDW_RG_CLOS_X				(WNDW_RG_POS_X+WNDW_RG_L-1)
	#define	WNDW_RG_CLOS_Y				(WNDW_RG_POS_Y)
	
	#define	WNDW_RG_RNG_Y				(WNDW_RG_POS_Y+2)
	#define	WNDW_RG_CTRL_Y				(WNDW_RG_RNG_Y+1)
	
	#define	WNDW_RG_DVC_X				(WNDW_RG_POS_X)
	#define	WNDW_RG_PAGE_X				(WNDW_RG_DVC_X+3)
	#define	WNDW_RG_ADDR_X			(WNDW_RG_PAGE_X+2)
	#define	WNDW_RG_DATA_B_X			(WNDW_RG_ADDR_X+3)
	#define	WNDW_RG_DATA_H_X			(WNDW_RG_DATA_B_X+9)
	#define	WNDW_RG_APLY_I_X			(WNDW_RG_DATA_H_X+3)
	#define	WNDW_RG_APLY_W_X			(WNDW_RG_APLY_I_X+2)
	#define	WNDW_RG_APLY_D_X			(WNDW_RG_APLY_W_X+2)
	#define	WNDW_RG_WRT_X				(WNDW_RG_APLY_D_X+2)
	#define	WNDW_RG_READ_X				(WNDW_RG_WRT_X+2)
#elif	defined(__WNDW_RG_PRO__)
	#define	WNDW_RG_NUM				4//
	
	#define	WNDW_RG_POS_X				11
	#define	WNDW_RG_POS_Y				8// 4// 
	#define	WNDW_RG_L						23
	#define	WNDW_RG_H						(3+WNDW_RG_NUM)
	#define	WNDW_RG_CLOS_X				(WNDW_RG_POS_X+WNDW_RG_L-1)
	#define	WNDW_RG_CLOS_Y				(WNDW_RG_POS_Y)
	
	#define	WNDW_RG_RNG_Y				(WNDW_RG_POS_Y+2)
	#define	WNDW_RG_CTRL_Y				(WNDW_RG_RNG_Y+1)
	
	#define	WNDW_RG_DVC_X				(WNDW_RG_POS_X)
	#define	WNDW_RG_PAGE_X				(WNDW_RG_DVC_X+3)
	#define	WNDW_RG_ADDR_X			(WNDW_RG_PAGE_X+2)
	#define	WNDW_RG_DATA_B_X			(WNDW_RG_ADDR_X+3)
	#define	WNDW_RG_DATA_H_X			(WNDW_RG_DATA_B_X+9)
	#define	WNDW_RG_APLY_W_X			(WNDW_RG_DATA_H_X+3)
	#define	WNDW_RG_READ_X				(WNDW_RG_APLY_W_X+2)
#endif

//==================================================================================
//==================================================================================
extern U8	wndw_rg_osd_pos_x;
extern U8	wndw_rg_osd_pos_y;

#if	defined(__WNDW_RG_EV__)
	extern U8	wndw_rg_wrt_en;
	extern U8	wndw_rg_rd_en;
	extern U8	wndw_rg_av_dvc[WNDW_RG_NUM];
	extern U8	wndw_rg_av_pg[WNDW_RG_NUM];
	extern U8	wndw_rg_av_addr[WNDW_RG_NUM];
	extern U8	wndw_rg_av_dat[WNDW_RG_NUM];
#elif	defined(__WNDW_RG_PRO__)
	extern U8	wndw_rg_read_en;
	extern U8	wndw_rg_av_dvc[WNDW_RG_NUM];
	extern U8	wndw_rg_av_pg[WNDW_RG_NUM];
	extern U8	wndw_rg_av_addr[WNDW_RG_NUM];
	extern U8	wndw_rg_av_dat[WNDW_RG_NUM];
#endif

extern U8	wndw_rg_bit_ptr_l;
extern U8	wndw_rg_bit_ptr_r;


//==================================================================================
//						Register window function description
//==================================================================================
extern void	SetWndwRgOSDDvc(U8 _pth, U8 _pos_x, U8 _pos_y, U8 *_av_dvc);
extern void	SetWndwRgOSDPg(U8 _pth, U8 _pos_x, U8 _pos_y, U8 *_av_pg);
extern void	SetWndwRgOSDAddr(U8 _pth, U8 _pos_x, U8 _pos_y, U8 *_av_addr);
extern void	SetWndwRgOSDDatBin(U8 _pth, U8 _pos_x, U8 _pos_y, U8 *_av_dat);
extern void	SetWndwRgOSDDatHex(U8 _pth, U8 _pos_x, U8 _pos_y, U8 *_av_dat);
extern void	SetWndwRgOSDDat(U8 _pth, U8 _pos_x, U8 _pos_y, U8 *_av_dat);
#if	defined(__WNDW_RG_EV__)
	extern void	SetWndwRgOSDWrt(U8 _pth, U8 _pos_y);
#endif
extern void	SetWndwRgOSDRd(U8 _pth, U8 _pos_y);
extern void	CreateWndwRg(U8 _pth);
extern void	RunWndwRgInc(U8 _pth, U8 _osd_pos_x, U8 _osd_pos_y);
extern void	RunWndwRgDec(U8 _pth, U8 _osd_pos_x, U8 _osd_pos_y);
#if	defined(__WNDW_RG_EV__)
	extern void	SetWndwRgRngData(U8 _osd_pos_x, U8 _osd_pos_y);
#endif
extern void	RunWndwRgAply(U8 _pth, U8 _osd_pos_x, U8 _osd_pos_y);
extern void	SetWndwRgOSD(U8 _pth, U8 _osd_pos_x, U8 _osd_pos_y);
extern void	RunWndwRg(U8 _pth);

