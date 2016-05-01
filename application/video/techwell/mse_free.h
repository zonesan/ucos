//==================================================================================
//						Mse & channel co-operation definition & variable description
//==================================================================================
extern U8	mse_ch_drg;
extern U8	mse_ch_scl;
extern U8	mse_ch_ptr;

extern U8	mse_ch_l;
extern U8	mse_ch_r;
extern U8	mse_ch_t;
extern U8	mse_ch_b;


//==================================================================================
//						Mse & channel co-operation function description
//==================================================================================
//extern void	SetMseFreeOSDCh(U8 _index_x, U8 _index_y, U8 _ch);
extern void	SetMseFreeDrg(void);
extern U16	GetMseFreeSclVSub(U16 _scl, U8 _dvc, U8 _ch, U8 _picvt, U8 _picvb);
extern U16	GetMseFreeSclV(U16 _scl, U8 _dvc, U8 _ch, U8 _picvt, U8 _picvb);
extern void	SetMseFreeSclV(U8 _dvc, U8 _ch, U8 _picxx, U8 _picvt, U8 _picvb);
extern void	SetMseFreeSclH(U8 _dvc, U8 _ch, U8 _picxx, U8 _pichl, U8 _pichr, U8 _shft);
extern void	TglMseChMrr(U8 _dvc, U8 _ch, U8 tgl_mrr);
extern void	SetMseFreeScl(void);
