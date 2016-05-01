/*
***************************************************************************
*                  Copyright (c) 2005 vBridge Microsystem, Inc.  
*                       Unpublished & Not for Publication
*                               All Rights Reserved                   
*   
* File        : risc.h                     
*                                                                         
* Description :    
*             
* Date        : Feb 13, 2006                                                           
***************************************************************************
*/



#ifndef __RISC_H__
#define __RISC_H__

#define TRUE 	1
#define FALSE	0

#define SUCCESSFUL 1
#define FAILURE	0
#define FAILED	0

#define ZERO 0
#define NON_ZERO 1

#define V_BASE_ADDR	0xa0000000

/* Global space: SDRAM/reg/queue/stream */
#define GLOBAL_B_ADDR 0x00000000
#define GLOBAL_E_ADDR 0x0fffffff

/* boot space */
#define BOOT_B_ADDR 0x1fc00000
#define BOOT_E_ADDR 0x1fffffff

/* Internal SPRAM: RISC internal SPRAM */
#define I_SPRAM_B_ADDR 0x1fff8000
#define I_SPRAM_E_ADDR 0x1fffffff

/* External SPRAM: RISC external SPRAM */
#define E_SPRAM_B_ADDR 0x1fa00000
#define E_SPRAM_E_ADDR 0x1fbfffff


/* Global space */
/* SDRAM: 64Mbytes */
#define SDRAM_B_ADDR 0x00000000
#define SDRAM_E_ADDR 0x03ffffff

/* Queue
* bit[15:8] : queue ID 
* bit[19:16]: extended video context ID, bit16=1 for UV, bit16=0 for Y
*/
#define QUEUE_B_ADDR 0x04000000
#define QUEUE_E_ADDR 0x07ffffff

/* On chip registers 
* bit[20:16]: block ID
* bit[15:0] : local register address of each block
*/
#define REGS_B_ADDR 0x08000000
#define REGS_E_ADDR 0x0bffffff

/* Stream
* bit[15: 8]: stream ID
* bit[19:16]: steam operation code 
  bit[23]: stream write last */
#define STREAM_B_ADDR 0x0c000000
#define STREAM_E_ADDR 0x0fffffff



//Block ID
#define RISC_BLOCK	0x0
#define CCTL_BLOCK 	0x1
#define MC_BLOCK 	0x2
#define SIF_BLOCK 	0x3
#define SC_BLOCK	0x4
#define VIN_BLOCK	0x5
#define VOUT_BLOCK	0x6
#define AUD_BLOCK	0x7
#define DMAC_BLOCK	0x8
#define PPE_BLOCK	0x9
#define PIF_BLOCK	0xA
#define VSC_BLOCK	0xB
#define EMAC0_BLOCK	0xC
#define EMAC1_BLOCK	0xD
#define SEC_BLOCK	0xE
#define UART_BLOCK	0xF
#define SD0_BLOCK	0x10
#define IDE_BLOCK	0x11
#define VCODEC_BLOCK	0x12
#define SD1_BLOCK	0x13

#define BLOCK_BASE_ADDR(BLOCK_ID) (V_BASE_ADDR + REGS_B_ADDR + (BLOCK_ID<<16))



// RISC local register specifications 
#define  RISC_BASE_ADDR    BLOCK_BASE_ADDR(RISC_BLOCK)

#define rRISC0_CTL          *(volatile unsigned *)(RISC_BASE_ADDR + 0x0)
#define rRISC1_CTL          *(volatile unsigned *)(RISC_BASE_ADDR + 0x4)
#define rRISC_COM_CTL       *(volatile unsigned *)(RISC_BASE_ADDR + 0x8)
#define rRISC_PCNT          *(volatile unsigned *)(RISC_BASE_ADDR + 0xC)
#define rRISC_INT_STATUS    *(volatile unsigned *)(RISC_BASE_ADDR + 0x10)
#define rRISC_VCNT          *(volatile unsigned *)(RISC_BASE_ADDR + 0x14)
#define rRISC_VCNT_VAL      *(volatile unsigned *)(RISC_BASE_ADDR + 0x18)
#define rRISC_SEMAPHORE0    *(volatile unsigned *)(RISC_BASE_ADDR + 0x20)
#define rRISC_SEMAPHORE1    *(volatile unsigned *)(RISC_BASE_ADDR + 0x24)
#define rRISC_SEMAPHORE2    *(volatile unsigned *)(RISC_BASE_ADDR + 0x28)
#define rRISC_SEMAPHORE3    *(volatile unsigned *)(RISC_BASE_ADDR + 0x2C)
#define rRISC_SEMAPHORE4    *(volatile unsigned *)(RISC_BASE_ADDR + 0x30)
#define rRISC_SEMAPHORE5    *(volatile unsigned *)(RISC_BASE_ADDR + 0x34)
#define rRISC_SEMAPHORE6    *(volatile unsigned *)(RISC_BASE_ADDR + 0x38)
#define rRISC_SEMAPHORE7    *(volatile unsigned *)(RISC_BASE_ADDR + 0x3C)
#define rRISC_CNT0          *(volatile unsigned *)(RISC_BASE_ADDR + 0x40)
#define rRISC_CNT1          *(volatile unsigned *)(RISC_BASE_ADDR + 0x44)
#define rRISC_CNT2          *(volatile unsigned *)(RISC_BASE_ADDR + 0x48)
#define rRISC_CNT3          *(volatile unsigned *)(RISC_BASE_ADDR + 0x4C)
#define rRISC_CNT4          *(volatile unsigned *)(RISC_BASE_ADDR + 0x50)
#define rRISC_CNT5          *(volatile unsigned *)(RISC_BASE_ADDR + 0x54)
#define rRISC_CNT6          *(volatile unsigned *)(RISC_BASE_ADDR + 0x58)
#define rRISC_CNT7          *(volatile unsigned *)(RISC_BASE_ADDR + 0x5C)
#define rRISC0_FLAG         *(volatile unsigned *)(RISC_BASE_ADDR + 0x60)
#define rRISC1_FLAG         *(volatile unsigned *)(RISC_BASE_ADDR + 0x64)
#define rRISC0_TIMER        *(volatile unsigned *)(RISC_BASE_ADDR + 0x68)
#define rRISC1_TIMER        *(volatile unsigned *)(RISC_BASE_ADDR + 0x6C)
#define rRISC_GPIO	    *(volatile unsigned *)(RISC_BASE_ADDR + 0x80)
#define rRISC_GPIO_DIR	    *(volatile unsigned *)(RISC_BASE_ADDR + 0x84)
#define rRISC0_GPIO_INTR_MASK	    *(volatile unsigned *)(RISC_BASE_ADDR + 0x88)
#define rRISC1_GPIO_INTR_MASK	    *(volatile unsigned *)(RISC_BASE_ADDR + 0x8C)
#define rRISC_GPIO_POL	    *(volatile unsigned *)(RISC_BASE_ADDR + 0x200)
#define rRISC_QUEUE_STATUS	    *(volatile unsigned *)(RISC_BASE_ADDR + 0x100)


#define RISC_SPRAM_READ_STREAM_B_ADDR     (V_BASE_ADDR + E_SPRAM_B_ADDR + 0x8000)
#define RISC_SPRAM_WRITE_STREAM_B_ADDR    (V_BASE_ADDR + E_SPRAM_B_ADDR + 0x9000)



// Risc IDs
#define RISC0    0    
#define RISC1    1



// Risc interrupt sources. According to rRISC_INT_STATUS.
// The offset is the same as the block IDs.
#define VCNT_INTR	(1<<0x0)
#define CCTL_INTR 	(1<<0x1)
#define MC_INTR 	(1<<0x2)
#define SIF_INTR 	(1<<0x3)
#define SC_INTR	        (1<<0x4)
#define VIN_INTR	(1<<0x5)
#define VOUT_INTR	(1<<0x6)
#define AUD_INTR	(1<<0x7)
#define DMAC_INTR	(1<<0x8)
#define PPE_INTR	(1<<0x9)
#define PIF_INTR	(1<<0xA)
#define VSC_INTR	(1<<0xB)
#define ETH0_INTR	(1<<0xC)
#define ETH1_INTR	(1<<0xD)
#define SEC_INTR	(1<<0xE)
#define UART_INTR	(1<<0xF)
#define SD0_INTR	(1<<0x10)
#define IDE_INTR	(1<<0x11)
#define VCODEC_INTR	(1<<0x12)
#define SD1_INTR	(1<<0x13)

/*
***************************************************************************
*                              FUCTION PROTYPES
***************************************************************************
*/
unsigned int RiscRdCNT(int id);
void RiscIncCNT(int id, unsigned int value);
void RiscDecCNT(int id, unsigned int value);
void RiscClrCNT(int id);
void RiscSetCNT(int id, unsigned int  value);

int RiscRdVCNT(void);
int RiscClrVCNT(void);
int RiscIncVCNT(int value);
int RiscDecVCNT(int value);
void RiscSetVCNT_VAL(int vint_val);

int RiscRdID(void);
int RiscRdIntrStatus(void);
int RiscIntrEn(unsigned int intr_source);
int RiscIntrDis(unsigned int intr_source);
void RiscSemLock(int SemID);
void RiscSemPend(int SemID);
int RiscSemPend1(int SemID);
void RiscChgCnt(int SemID, int main_type, int sub_type, signed char direction);
void RiscSemUnLock(int SemID);
void RiscBootRisc1(void);
void Risc1Copy180Entry(void);
void Risc2CpuStart(void);
void Risc2CpuStartLx160(void);

void RiscFlagSet(void);
void RiscFlagClr(void);
int RiscFlagIntrInit(void);
void RiscFlagIntrDis(void);

void  RiscIntrInit(void);
void  RiscIntrHandler(void);
void  RiscInitIntrHandlers (void);
void  RiscPrintIntrMsg(void);

void  RiscVCNTIntrEn(int vcnt_val);
void  RiscVCNTIntrDis();
void  RiscVCNT_ISR(void);

void  RiscFlagISR(void);

void  RiscGPIOISR(void);
int RiscGPIOIntrInit(void);

void  RiscTimerISR(void);
int  RiscTimerInit(unsigned int time_interval);


#endif // __RISC_H__
