//==================================================================================
//						Channel definition & variable description
//==================================================================================


//==================================================================================
//						Channel property(position & size, scale, ... ) function description
//==================================================================================
extern void	SetSclH(U8 _pth, U8 _dvc, U8 _ch, U16 _scl_h);
extern void	SetSclV(U8 _pth, U8 _dvc, U8 _ch, U16 _scl_v);
extern void	SetSclRtoH(U8 _pth, U8 _dvc, U8 _ch, U8 _rto_x);
extern void	SetSclRtoV(U8 _pth, U8 _dvc, U8 _ch, U8 _rto_y);
extern void	SetSclRto(U8 _pth, U8 _dvc, U8 _ch, U8 _rto_x, U8 _rto_y);
extern void	SetPicPos(U8 _pth, U8 _dvc, U8 _ch, U8 _pichl, U8 _picvt);
extern void	SetPicSzRto(U8 _pth, U8 _dvc, U8 _ch, U8 _rto_x, U8 _rto_y);
extern void	SetPicRto(U8 _pth, U8 _dvc, U8 _ch, U8 _pichl, U8 _picvt, U8 _rto_x, U8 _rto_y);
extern void	SetChFl(U8 _pth, U8 _dvc, U8 _ch);
extern void	SetChQdPos(U8 _pth, U8 _dvc, U8 _ch, U8 _cnt_pos);
extern void	SetChQd(U8 _pth, U8 _dvc, U8 _ch, U8 _cnt_pos);
extern void	SetChNinePos(U8 _pth, U8 _dvc, U8 _ch, U8 _cnt_pos);
extern void	SetChNine(U8 _pth, U8 _dvc, U8 _ch, U8 _cnt_pos);
extern void	SetChPIP(U8 _pth, U8 _dvc, U8 _ch, U8 _cnt_pos);
extern void	SetCh36Pos(U8 _pth, U8 _dvc, U8 _ch, U8 _cnt_pos);
extern void	SetCh36(U8 _pth, U8 _dvc, U8 _ch, U8 _cnt_pos);
//extern void	SetChCIF(U8 _pth, U8 _dvc, U8 _ch, U8 _cnt_pos);
extern void	WaitChStrbDoneX(U16 _flg_ch);
extern void	WaitChStrbDoneY(U16 _flg_ch);
extern void InitChScaleAdapt(U8 _dvc, U8 _ch);

