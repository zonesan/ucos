#include	"hdr.h"
#include	"cmn.h"
#include	"mse.h"
#include	"hst.h"
#include	"isr.h"
#include 	"gpio.h"
#include 	"cpu.h"
#include 	"regs.h"

 
//==================================================================================
//						Interrupt service routine function description
//==================================================================================
void ExeIsrEx0(void) //DWDWinterrupt 0		//... mse clock interrupt
{
	U8 _t1_;
	
	if(mse_cnt_clk >= 0 && mse_cnt_clk < 8)
	{
		set_mse_dat_out();
		if((mse_rdata[0] & 0x01) != 0)
			set_mse_dat(); //DWDW use one GPIO (P3_5 in 8051)
		else
			clr_mse_dat();

		mse_rdata[0] >>= 1;
		if (mse_cnt_clk==7) mse_cnt_clk++;
	}
	else
	{
		if(get_mse_dat() == 0)
			_t1_ = 0x00;
		else
			_t1_ = 0x80;
	
		if((mse_cnt_clk >= 13) && (mse_cnt_clk <= 20))
		{
			mse_rdata[0] >>= 1;
			mse_rdata[0] |= _t1_;  
		}
		else if((mse_cnt_clk >= 24) &&( mse_cnt_clk <= 31))
		{
			mse_rdata[1] >>= 1;
			mse_rdata[1] |= _t1_;  
		}
		else if((mse_cnt_clk >= 35) &&( mse_cnt_clk <= 42))
		{
			mse_rdata[2] >>= 1;
			mse_rdata[2] |= _t1_;  
		}
		else if((mse_cnt_clk >= 46) && (mse_cnt_clk <= 53))
		{
			mse_rdata[3] >>= 1;
			mse_rdata[3] |= _t1_;  
		}
	}

	if(mse_cnt_clk < 55){
		mse_cnt_clk++;
		b_mse_rd_done = 0;
	}
	else{		//... mse_cnt_clk == 54
		b_mse_rd_done = 1;
	}
}

void IsrGpio() {
int gpio_dat;
int gpio_pol;
int gpio_int;

	gpio_dat = RISC_GPIO_DAT_REG_ADDR;
	gpio_pol = RISC_GPIO_INT_POL_ADDR;
	gpio_int = gpio_pol ^ gpio_dat;

	if ( (gpio_int >> GPIO_KEY_BIT)&1) {
		if ((gpio_pol >> GPIO_KEY_BIT)&1) {
			printf ("Key push detected by interrupt\n");
			set_gpio_int_pol(GPIO_KEY_BIT, 1);
		}
		else {
			set_gpio_int_pol(GPIO_KEY_BIT, 0);
		}
	} else if ((gpio_int >> GPIO_MSE_CLK_BIT)&1) {
		if ((gpio_pol >> GPIO_MSE_CLK_BIT)&1) { //active low for input data
			//printf ("Mouse clk detected by interrupt\n");
			ExeIsrEx0();
			set_gpio_int_pol(GPIO_MSE_CLK_BIT, 1); //set active high
		}
		else { //active high for output data
			if (mse_cnt_clk>=9) {
				//printf ("mse_cnt = %d\n", mse_cnt_clk);
				set_mse_dat_in();
			}
			set_gpio_int_pol(GPIO_MSE_CLK_BIT, 0); //set active low
		}
	}
}

//==================================================================================
//... initialize Interrupt
void InitIsr(void)
{
	clr_gpio_int_ena_all();	

	set_gpio_int_pol(GPIO_MSE_CLK_BIT, 0); //active low
	set_gpio_int_ena(GPIO_MSE_CLK_BIT);

	set_gpio_int_pol(GPIO_KEY_BIT, 0); //active low
	set_gpio_int_ena(GPIO_KEY_BIT);

	BspStartTicker(33); //30ms per second

 	mips_setcr (mips_getcr() & ~(1<<23)); //clr IV
	mips_setsr (mips_getsr() | (1<<13) | (1<<15) | 1); //bit13 to enable gpio interrupt, bit15 to enable timer, bit 1 to enable interrupt

}

