#define RISC_BASE_ADDR 0xa8000000
#define RISC_GPIO_DAT_REG_ADDR (*(volatile unsigned *)(RISC_BASE_ADDR + 0x80))
#define RISC_GPIO_DIR_REG_ADDR (*(volatile unsigned *)(RISC_BASE_ADDR + 0x84))
#define RISC0_GPIO_INT_MASK_ADDR (*(volatile unsigned *)(RISC_BASE_ADDR + 0x88))
#define RISC1_GPIO_INT_MASK_ADDR (*(volatile unsigned *)(RISC_BASE_ADDR + 0x8c))
#define RISC_GPIO_INT_POL_ADDR (*(volatile unsigned *)(RISC_BASE_ADDR + 0x200))
#define RISC_COM_CTL_ADDR (*(volatile unsigned *)(RISC_BASE_ADDR + 0x8))

#define set_ie                  mips_setsr(mips_getsr() | 0x1)
#define clr_ie                  mips_setsr(mips_getsr() & ~0x1)

