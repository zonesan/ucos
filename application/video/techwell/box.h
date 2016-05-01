#include "cmn.h"

//==================================================================================
//						Box definition & variable description
//==================================================================================
//	on 8x10 font : 1 OSD's BOXHL=8, 1 OSD's BOXHW=4, 1 OSD's BOXVT=11, 1 OSD's BOXVW=5
#define	BOX_POP_MN			3//15
#define	BOX_POP_MN_HL		2//14
#define	BOX_POP_SB			1//13
#define	BOX_POP_SB_HL		0//12
//#define	BOX_WIN_FRM				11
//#define	BOX_WIN_TTL				10
//#define	BOX_WIN_QUIT			9
//#define	BOX_BTN8					8
//#define	BOX_BTN7					7
//#define	BOX_BTN6					6
//#define	BOX_BTN5					5
//#define	BOX_BTN4					4
//#define	BOX_BTN3					3
//#define	BOX_BTN2					2
//#define	BOX_BTN1					1
//#define	BOX_BTN0					0


//==================================================================================
//						Box function description
//==================================================================================
//extern void EnableBox(U8 _pth, U8 _box_num);
extern void DisableBox(U8 _pth, U8 _box_num);
extern void SetBoxPln(U8 _pth, U8 _box_num, U8 _tgl);
extern void SetBoxBnd(U8 _pth, U8 _box_num, U8 _tgl);
extern void SetBoxMix(U8 _pth, U8 _box_num, U8 _tgl);
extern void SetBoxPos(U8 _box_num, U16 _boxhl, U16 _boxvt);
extern void SetBoxSize(U8 _box_num, U16 _boxhw, U16 _boxvw);
extern void SetBox(U8 _box_num, U16 _boxhl, U16 _boxhw, U16 _boxvt, U16 _boxvw);
//extern void DisableBox(U8 _pth, U8 _box_num);

