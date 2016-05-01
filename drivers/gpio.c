#include "gpio.h"
//#include "../application/video/techwell/cpu.h"
//#include "../application/video/techwell/regs.h"
#include "chip_cfg.h"
#include "../port/os_cpu.h"
#include "../uCOS Configure/os_cfg.h"
#include "../uCOS2.52/uCOS_II.h"

/*
void tw_reset_on(void) {
	clr_gpio(GPIO_TW_RST_BIT);
}
	
void tw_reset_off(void) {
	set_gpio(GPIO_TW_RST_BIT);
}

*/

void set_mse_dat_out() {
	set_gpio_out(GPIO_MSE_DAT_BIT);
}

void set_mse_dat_in() {
	set_gpio_in(GPIO_MSE_DAT_BIT);
}

void set_mse_dat(void) {
	set_gpio(GPIO_MSE_DAT_BIT);
}

void clr_mse_dat(void) {
	clr_gpio(GPIO_MSE_DAT_BIT);
}

int get_mse_dat(void) {
	return (get_gpio(GPIO_MSE_DAT_BIT));
}

int get_key(int pos) {
	return (get_gpio(pos));
}

void set_mse_clk_out() {
	set_gpio_out(GPIO_MSE_CLK_BIT);
}

void set_mse_clk_in() {
	set_gpio_in(GPIO_MSE_CLK_BIT);
}

void clr_mse_clk(void) {
	clr_gpio(GPIO_MSE_CLK_BIT);
}

void set_mse_clk(void) {
	set_gpio(GPIO_MSE_CLK_BIT);
}

void set_gpio(int pos) {
       unsigned int cpu_sr;
       
	OS_ENTER_CRITICAL();
	RISC_GPIO_DAT_REG_ADDR |= (0x1 << pos);
	OS_EXIT_CRITICAL() ;
}

void clr_gpio(int pos) {
	unsigned int cpu_sr;
       
	OS_ENTER_CRITICAL();
	RISC_GPIO_DAT_REG_ADDR &= ~(0x1 << pos);
	OS_EXIT_CRITICAL() ;
}

int get_gpio(int pos) {
	unsigned int cpu_sr;
       
	OS_ENTER_CRITICAL();
	RISC_GPIO_DIR_REG_ADDR |= (0x1 << pos);
	OS_EXIT_CRITICAL();
	return ((RISC_GPIO_DAT_REG_ADDR>>pos)&0x1);
} 

void set_gpio_out(int pos) {
	unsigned int cpu_sr;
       
	OS_ENTER_CRITICAL();
	RISC_GPIO_DIR_REG_ADDR &= ~(0x1 << pos);
	OS_EXIT_CRITICAL();
}

void set_gpio_in(int pos) {
	unsigned int cpu_sr;
       
	OS_ENTER_CRITICAL();
	RISC_GPIO_DIR_REG_ADDR |= (0x1 << pos);
	OS_EXIT_CRITICAL();
}

void clr_gpio_int_ena_all() {
int risc_id;
	risc_id = RISC_COM_CTL_ADDR & 0x1;
	if (risc_id) RISC1_GPIO_INT_MASK_ADDR = 0;
	else RISC0_GPIO_INT_MASK_ADDR = 0;
}

void clr_gpio_int_ena(int pos) {
int risc_id;
	risc_id = RISC_COM_CTL_ADDR & 0x1;
	if (risc_id) RISC1_GPIO_INT_MASK_ADDR &= ~(1<<pos);
	else RISC0_GPIO_INT_MASK_ADDR &= ~(1<<pos);
}

void set_gpio_int_ena(int pos) {
int risc_id;
	risc_id = RISC_COM_CTL_ADDR & 0x1;
	if (risc_id) RISC1_GPIO_INT_MASK_ADDR |= 1<<pos;
	else RISC0_GPIO_INT_MASK_ADDR |= 1<<pos;
}

void set_gpio_int_pol(int pos, int pol) {
	unsigned int cpu_sr;
       
	OS_ENTER_CRITICAL();
	if (pol) {
		RISC_GPIO_INT_POL_ADDR &= ~(1<<pos); //0 -- active high
	} else {
		RISC_GPIO_INT_POL_ADDR |= 1<<pos; //1 -- active low
	}
	OS_EXIT_CRITICAL();
}

	
