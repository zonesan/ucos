//==================================================================================
//						Mouse definition & variable description
//==================================================================================
#define	LEFT_BTN					BIT0
#define	RIGHT_BTN				BIT1

extern int b_mse_btn_l;				//... don't change bit position because it should be same as LEFT_BTN
extern int b_mse_btn_r;				//... don't change bit position because it should be same as RIGHT_BTN
extern int b_mse_rlse_lb;
extern int b_mse_rlse_rb;
extern int b_mse_pshd_lb;
extern int b_mse_pshd_rb;
extern int b_mse_pshng_lb;
extern int b_mse_pshng_rb;
#define mse_flg_bits ( (b_mse_btn_l<<0) | (b_mse_btn_r<<1) | (b_mse_rlse_lb<<2) | (b_mse_rlse_rb<<3) | (b_mse_pshd_lb<<4) | (b_mse_pshd_rb<<5) | (b_mse_pshng_lb<<6) | (b_mse_pshng_rb<<7) ) 

extern int b_mse_btn_dwn_lb;
extern int b_mse_btn_dwn_rb;
extern int b_mse_clk_lb;
extern int b_mse_clk_rb;
extern int b_mse_pshng_clk_lb;
extern int b_mse_pshng_clk_rb;
extern int b_mse_no_btn;
extern int b_mse_no_mvng;

extern int b_mse_rd_done;
extern int b_mse_scl_bnd_only;
extern int b_mse_init_scrl;
extern int b_mse_init_zm;
extern int b_mse_init_scl;
extern int b_mse_init_drg;

#define	MSE_PTH_X		0
#define	MSE_PTH_Y		1
#define	CRSR_SML			0
#define	CRSR_BIG			1
extern int b_mse_stts_pth;
extern int b_mse_stts_pth_pop;
extern int b_mse_stts_pth_mn;
extern int b_mse_stts_free;
extern int b_mse_stts_crsr;
extern int b_mse_tmp_l;
extern int b_mse_tmp_r;

//==================================================================================
//==================================================================================
#define	MSE_BTN_CNT_CHTR			12// 16// 
#define	MSE_BTN_DLY_CNTNS			1//2	//(MSE_BTN_CNT_CHTR>>2)		//... MOUSE BUTTON DELAY CONTINUOUS
#define	MSE_BTN_DLY_CLK			2//3	//(MSE_BTN_CNT_CHTR>>1)		//... must be bigger than MOUSE_BTN_DELAY_CONT
//... depend on mouse_read_done timing (-> use internal timer interrupt, but not use time constant(TH,TL) and set TF before enable ET)

extern U16	mse_flg_on_osd;

extern U8	mse_rdata[4];
extern U8	mse_cnt_clk;
       
extern U8	mse_ch;
       
extern U8	mse_osd_x;
extern U8	mse_osd_y;
extern U8	mse_osd_x_sub;
extern U8	mse_osd_y_sub;
       
extern U8	mse_btn_pshng_cnt;


//==================================================================================
//						Mouse function description
//==================================================================================
extern void WriteMse(U8 wdata);
//U8 ReadMse(U8 packet_num);
//void ReadMseMove();
extern void InitMse(void);
extern void SetMseCrsr(U8 _tgl_crsr);
extern void SetMseLyr(U8 _tgl_lyr);
extern void SetMsePtrX(U16 _pos_x);
extern void SetMsePtrY(U16 _pos_y);
extern void SetMsePtr(U16 _pos_x, U16 _pos_y);			//... set mouse pointer
//extern void DsplyMsePtr(U8 _pth);							//... display mouse pointer
extern U8	GetMseCh(U8 _pos_x, U8 _pos_y);
extern U8	GetMseMaskX(U8 _dvc, U8 _ch, U16 _crsr_pos_x);
extern U8	GetMseMaskY(U8 _dvc, U8 _ch, U16 _crsr_pos_y);
extern U8 GetMseOSDX(U16 _pos_x);
extern U8 GetMseOSDY(U16 _pos_y);
extern void	SetMseMvmnt(void);								//... set mouse movement
extern void	SetMseBtn(void);								//... set mouse button
extern void	RunMseOnOSDH(U8 _pth_prp, U8 _osd_pos_x, U8 _osd_pos_y, U8 _pos_x_l, U8 _pos_x_r, U8 _pos_y, U8 _pos_flg);
extern void	RunMseOnOSDV(U8 _pth_prp, U8 _osd_pos_x, U8 _osd_pos_y, U8 _pos_x, U8 _pos_y_t, U8 _pos_y_b, U8 _pos_flg);
