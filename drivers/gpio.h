#include "risc.h"

#define GPIO_DATA_REG (*(volatile unsigned *)(RISC_BASE_ADDR + 0x80))
#define GPIO_DIR_REG (*(volatile unsigned *)(RISC_BASE_ADDR + 0x84))

//#define RISC_BASE_ADDR 0xa8000000
#define RISC_GPIO_DAT_REG_ADDR (*(volatile unsigned *)(RISC_BASE_ADDR + 0x80))
#define RISC_GPIO_DIR_REG_ADDR (*(volatile unsigned *)(RISC_BASE_ADDR + 0x84))
#define RISC0_GPIO_INT_MASK_ADDR (*(volatile unsigned *)(RISC_BASE_ADDR + 0x88))
#define RISC1_GPIO_INT_MASK_ADDR (*(volatile unsigned *)(RISC_BASE_ADDR + 0x8c))
#define RISC_GPIO_INT_POL_ADDR (*(volatile unsigned *)(RISC_BASE_ADDR + 0x200))
#define RISC_COM_CTL_ADDR (*(volatile unsigned *)(RISC_BASE_ADDR + 0x8))

//gpio definitions
#if 0
#define GPIO_I2C_SDA_BIT 0
#define GPIO_I2C_SCL_BIT 1
#define GPIO_TW_RST_BIT 2
#define GPIO_TW_HSPB_BIT 3
#define GPIO_MSE_DAT_BIT 4 
#define GPIO_MSE_CLK_BIT 5
#define TEST_BIT0 6
#define TEST_BIT1 7
#define GPIO_KEY_BIT 11
#else
#define GPIO_I2C_SDA_BIT 12
#define GPIO_I2C_SCL_BIT 13
//#define GPIO_TW_RST_BIT 26
//#define GPIO_TW_HSPB_BIT 0
#define GPIO_MSE_DAT_BIT 10 
#define GPIO_MSE_CLK_BIT 9
#define GPIO_KEY_BIT 26
#endif
extern void tw_reset_on(void);
extern void tw_reset_off(void);
extern void set_mse_dat_out(void);
extern void set_mse_dat_in(void);
extern void set_mse_dat(void);
extern void clr_mse_dat(void);
extern int get_mse_dat(void);
extern int get_key(int pos);
extern void set_mse_clk_out(void);
extern void set_mse_clk_in(void);
extern void clr_mse_clk(void);
extern void set_mse_clk(void);
extern void set_gpio(int pos);
extern void clr_gpio(int pos);
extern int get_gpio(int pos);
extern void set_gpio_out(int pos);
extern void set_gpio_in(int pos);
extern void clr_gpio_int_ena_all(void);
extern void clr_gpio_int_ena(int pos);
extern void set_gpio_int_ena(int pos);
extern void set_gpio_int_pol(int pos, int pol);
