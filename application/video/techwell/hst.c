#include	"hdr.h"
#include	"cmn.h"
#include	"hst.h"
#include 	"gpio.h"



//U32 rGPGtmp;
//==================================================================================
//						Host init function description
//==================================================================================
//... initialize port
//void InitPrt(void)
//{
//	P0 = 0xff;
//	P1 = 0xff;
//	P2 = 0xff;
//	P3 = 0xff;
//}
//==================================================================================
void InitRstN(void)
{
U16 _dly_;
	
	DLY_FOR(5000);
	//tw_reset_on();
	DLY_FOR(10000);
	//tw_reset_off();
	DLY_FOR(10000);

}

//==================================================================================
void	InitVdo(U8 VinModeControl)
{

	if(VinModeControl == VDO_NTSC){ 
		b_cmn_jp_vdo = VDO_NTSC;
		cmn_bot = 120;
		cmn_cnt_vs_sub = 54;
	}
	else{
		b_cmn_jp_vdo = VDO_PAL;
		cmn_bot = 144;
		cmn_cnt_vs_sub = 45;
	}
#if defined(__4CH__)
	#if defined(__WTH_VGA__)
		if(VGA_DET)	cmn_vga_det = 1;
		else	cmn_vga_det = 0;
	#endif
#endif
}
//==================================================================================
