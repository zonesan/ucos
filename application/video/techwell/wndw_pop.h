//==================================================================================
//						Popup definition & variable description
//==================================================================================
//#define	POP_X_MAIN_NUM_H		9
//#define	POP_X_MAIN_NUM_V		7
//#define	POP_Y_MAIN_NUM_H		9
//#define	POP_Y_MAIN_NUM_V		7
#define	POP_MN_NUM_H		9
#if defined(__WTH_ADO__)
	#define	POP_MN_NUM_V		5
#elif defined(__WTH_VGA__)
	#define	POP_MN_NUM_V		4//5			//... because of VGA board detection
#endif

extern U8	wndw_pop_sub_num_h;
extern U8	wndw_pop_sub_num_v;


//==================================================================================
#define	POP_POS_DS					0
#define		POP_POS_DS_SC			0x00
#define		POP_POS_DS_SR			0x10
//#define		POP_POS_DS_PB			0x10
#define		POP_POS_ZM				0x20
#define		POP_POS_DS_IN			0x30
//#define	POP_POS_CH						1
//#define	POP_POS_ZM						1
#define	POP_POS_RC						1
#define		POP_POS_RC_FM				0x01
#define		POP_POS_RC_IN				0x11
#define		POP_POS_RC_NR				0x21
#define	POP_POS_MTN						2
#define		POP_POS_MTN_MD				0x02
#define		POP_POS_MTN_BD				0x12
#define		POP_POS_MTN_ND				0x22

#if defined(__WTH_ADO__)
	#define	POP_POS_ADO						3
	#define		POP_POS_ADO_GN				0x03
	#define		POP_POS_ADO_LV				0x13
	#define		POP_POS_ADO_RC				0x23
	//#define		POP_POS_ADO_DT				0x35
	#define	POP_POS_RG						4
#elif defined(__WTH_VGA__)
	#define	POP_POS_VGA						3
	#define		POP_POS_VGA_OR				0x03
	#define		POP_POS_VGA_DI				0x13
	#define		POP_POS_VGA_IE				0x23
	#define		POP_POS_VGA_NR				0x33
	#define	POP_POS_RG						3//5			//... because of VGA board detection
#endif
//#define	POP_POS_ABT						6
//#define	POP_POS_RG							0
//#define	POP_POS_SCRN_X						1
//#define		POP_POS_SCRN_X_FL			0x01
//#define		POP_POS_SCRN_X_QD			0x11
//#define		POP_POS_SCRN_X_NRT		0x21
//#define	POP_POS_SCRN_Y						1
//#define		POP_POS_SCRN_Y_QD			0x01
//#define		POP_POS_SCRN_Y_CIF		0x11
//#define		POP_POS_SCRN_Y_MUX		0x21

extern U8	wndw_pop_crnt_ch;
extern U8	wndw_crnt_dvc;
extern U8	wndw_crnt_ch;
//... wndw_mn_pos_crnt : Use bit[7 - 4] for popup_sub_pos when enter the popup sub menu
extern U8	wndw_mn_pos_crnt;



//==================================================================================
//						Popup function description
//==================================================================================
extern void SetPopMainOSD(U8 _pth, U8 _pos_x, U8 _pos_y);
extern void SetPopMain(U8 _pth, U8 _pos_x, U8 _pos_y, U8 _lngth, U8 _hght);
extern void SetPopSubOSD(U8 _pth, U8 _pos_x, U8 _pos_y, U8 _main_mn_pos_crnt);
extern void SetPopSub(U8 _pth, U8 _pos_x, U8 _pos_y, U8 _lngth, U8 _hght, U8 _main_mn_pos_crnt);
extern void RemovePopMain(void);
extern void CreatePopMain(void);
extern void RemovePopSub(void);
extern void CreatePopSub(U8 _main_mn_pos_crnt);
extern void RunPopProc(void);
