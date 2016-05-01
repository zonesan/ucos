#include	"hdr.h"
#include	"cmn.h"
#include	"key.h"
#include	"gpio.h"
//#include	"key0.h"

//==================================================================================
//						Key definition & variable description
//==================================================================================
int	b_key0_past;
int	b_key0_rlse;
int	b_key0_pshd;
int	b_key0_pshng;
int	b_key0_dwn;
int	b_key0_clk;
int	b_key0_pshng_clk;

int	b_key0_tmp;

U8      key0_cnt = 0;

//==================================================================================
//						Key function description
//==================================================================================
void	SetKey0(void)
{
	if(get_key(GPIO_KEY_BIT) != b_key0_past){
		b_key0_past = get_key(GPIO_KEY_BIT);
		
		if(b_key0_past == 1){						// detecting low to high(button up) transition.
			b_key0_rlse = 1;
			if(b_key0_pshd == 1)	b_key0_clk = 1;
			b_key0_pshd = 0;
			b_key0_pshng = 0;
		}
		else{															// detecting high to low(button down) transition.
			if((b_key0_pshd == 0)&&(b_key0_pshng == 0)){
				b_key0_pshd = 1;
				b_key0_rlse = 0;
				b_key0_dwn = 1;

				key0_cnt = KEY_CNT_CHTR;		//... default
			}
		}
	}
	else{
		if(b_key0_pshd == 1){
			if(key0_cnt > 0)	key0_cnt--;
			else{
				if(b_key0_pshng == 0){
					b_key0_pshng = 1;			// pushing button checking
				}
				else	b_key0_pshng_clk = 1;			// continuous button checking
				
				key0_cnt = KEY_CNT_CHTR>>2;			//... default
			}
		}
	}
}

