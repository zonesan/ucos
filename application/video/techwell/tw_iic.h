#include "cmn.h"
#include "os_cpu.h"

#define TW_IIC_WAIT_TIME   10000
#define TW_IIC_PUSH_DELAY  1
//#define TW_IIC_STATUS_PROTECT
#if 1
#define rIIC_SDA_BIT_SHIFT          12
#define rIIC_SDA_DIR_BIT_SHIFT      12
#define rIIC_SCL_BIT_SHIFT          13
#define rIIC_SCL_DIR_BIT_SHIFT      13
#else
#define rIIC_SDA_BIT_SHIFT          0
#define rIIC_SDA_DIR_BIT_SHIFT      0
#define rIIC_SCL_BIT_SHIFT          1
#define rIIC_SCL_DIR_BIT_SHIFT      1
#endif
#define rIIC_SDA_BIT_REG            (*(volatile unsigned *)(RISC_BASE_ADDR + 0x80))
#define rIIC_SDA_DIR_BIT_REG        (*(volatile unsigned *)(RISC_BASE_ADDR + 0x84))
#define rIIC_SCL_BIT_REG            (*(volatile unsigned *)(RISC_BASE_ADDR + 0x80))
#define rIIC_SCL_DIR_BIT_REG        (*(volatile unsigned *)(RISC_BASE_ADDR + 0x84))
/* data line direction control macro */
#ifdef    TW_IIC_STATUS_PROTECT
#define TW_IIC_SDA_OUT()       \
        do  {\
            OS_ENTER_CRITICAL();\
            (rIIC_SDA_DIR_BIT_REG &= ~(0x1 << rIIC_SDA_DIR_BIT_SHIFT));\
            OS_EXIT_CRITICAL();\
        }while (0) 

#define TW_IIC_SDA_IN()        \
        do {\
            OS_ENTER_CRITICAL();\
            (rIIC_SDA_DIR_BIT_REG |= (0x1 << rIIC_SDA_DIR_BIT_SHIFT));\
            OS_EXIT_CRITICAL();\
        }while (0) 

/* data line level control marco */
#define TW_IIC_SDA_SET()       \
       do  {\
            OS_ENTER_CRITICAL();\
            (rIIC_SDA_BIT_REG |= (0x1 << rIIC_SDA_BIT_SHIFT));\
            OS_EXIT_CRITICAL();\
        }while (0) 

#define TW_IIC_SDA_CLR()       \
         do {\
            OS_ENTER_CRITICAL();\
            (rIIC_SDA_BIT_REG &= ~(0x1 << rIIC_SDA_BIT_SHIFT));\
            OS_EXIT_CRITICAL();\
        }while (0) 

#define TW_IIC_SDA_GET()       ((rIIC_SDA_BIT_REG >> rIIC_SDA_BIT_SHIFT) & 0x1)
/* clock line direction control macro */
#define TW_IIC_SCL_OUT()       \
        do {\
            OS_ENTER_CRITICAL();\
            (rIIC_SCL_DIR_BIT_REG &= ~(0x1 << rIIC_SCL_DIR_BIT_SHIFT));\
            OS_EXIT_CRITICAL();\
        }while (0)

#define TW_IIC_SCL_IN()        \
         do {\
            OS_ENTER_CRITICAL();\
            (rIIC_SCL_DIR_BIT_REG |= (0x1 << rIIC_SCL_DIR_BIT_SHIFT));\
            OS_EXIT_CRITICAL();\
        } 

/* clock line level control marco */
#define TW_IIC_SCL_SET()       \
      do  {\
            OS_ENTER_CRITICAL();\
            (rIIC_SCL_BIT_REG |= (0x1 << rIIC_SCL_BIT_SHIFT));\
            OS_EXIT_CRITICAL();\
        }while (0) 

#define TW_IIC_SCL_CLR()       \
        do {\
            OS_ENTER_CRITICAL();\
            (rIIC_SCL_BIT_REG &= ~(0x1 << rIIC_SCL_BIT_SHIFT));\
            OS_EXIT_CRITICAL();\
        }while (0) 
#else
#define TW_IIC_SDA_OUT()       \
         { \
             clr_ie; \
             (rIIC_SDA_DIR_BIT_REG &= ~(0x1 << rIIC_SDA_DIR_BIT_SHIFT));\
             set_ie;\
        } 

#define TW_IIC_SDA_IN()        \
        {\
            clr_ie;\
            (rIIC_SDA_DIR_BIT_REG |= (0x1 << rIIC_SDA_DIR_BIT_SHIFT));\
            set_ie;\
        } 

/* data line level control marco */
#define TW_IIC_SDA_SET()       \
        {\
            clr_ie;\
            (rIIC_SDA_BIT_REG |= (0x1 << rIIC_SDA_BIT_SHIFT));\
            set_ie;\
        } 

#define TW_IIC_SDA_CLR()       \
         {\
            clr_ie;\
            (rIIC_SDA_BIT_REG &= ~(0x1 << rIIC_SDA_BIT_SHIFT));\
            set_ie;\
        } 

#define TW_IIC_SDA_GET()       ((rIIC_SDA_BIT_REG >> rIIC_SDA_BIT_SHIFT) & 0x1)
/* clock line direction control macro */
#define TW_IIC_SCL_OUT()       \
        {\
            clr_ie;\
            (rIIC_SCL_DIR_BIT_REG &= ~(0x1 << rIIC_SCL_DIR_BIT_SHIFT));\
            set_ie;\
        }

#define TW_IIC_SCL_IN()        \
         {\
            clr_ie;\
            (rIIC_SCL_DIR_BIT_REG |= (0x1 << rIIC_SCL_DIR_BIT_SHIFT));\
            set_ie;\
        } 

/* clock line level control marco */
#define TW_IIC_SCL_SET()       \
       {\
            clr_ie;\
            (rIIC_SCL_BIT_REG |= (0x1 << rIIC_SCL_BIT_SHIFT));\
            set_ie;\
        } 

#define TW_IIC_SCL_CLR()       \
         {\
            clr_ie;\
            (rIIC_SCL_BIT_REG &= ~(0x1 << rIIC_SCL_BIT_SHIFT));\
            set_ie;\
        } 
#endif
#define TW_IIC_ACK_ERROR   -1
#define TW_IIC_ACK_SUCCESS 1
#define TW_IIC_ERROR   -1
#define TW_IIC_OK      1

void TW_IICDelayLoop();
void TW_IICDelay(int cnt);
void TW_IICinit(void);
void TW_IICStart(void);         /* statr the iic bus */
void TW_IICStop(void);          /* stop the iic bus */
int  TW_IICRead(void);          /* read the iic bus */
void TW_IICWrite(int buf);      /* write the iic bus */
int TW_IICChkAck(void);         /* chack the acknowledge signel */
void TW_IICChkNAck(void);
void TW_IICSendAck(void);       /* send the acknowledge signel */
void TW_IICSendNAck(void);

int TW_IICWriteByte(int SlaveAddr, int ByteAddr, int data, int mode);
int TW_IICWriteByteS(int SlaveAddr, int ByteAddr, int data, int cnt, int mode);
int TW_IICWriteByteB(int SlaveAddr, int PageAddr, int ByteAddr, U8 *data, int len);
int TW_IICWriteByteB256(int SlaveAddr, int PageAddr, int ByteAddr, U8 *data, int len);
int TW_IICReadByte(int SlaveAddr, int ByteAddr, int *data, int mode);
int TW_IICReadByteS(int SlaveAddr, int ByteAddr, int *data, int cnt, int mode);
int TW_IICReadByteB(int SlaveAddr, int PageAddr, int ByteAddr, U8 *data, int len);
int TW_IICReadByteB256(int SlaveAddr, int PageAddr, int ByteAddr, U8 *data, int len);
