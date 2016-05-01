#include <risc.h>
#include "os_cpu.h"

#define IIC_WAIT_TIME   10000
#define IIC_PUSH_DELAY  2
#define IIC_PUSH_DELAY_TVP  83

#define rIIC_SDA_BIT_REG            (*(volatile unsigned *)(RISC_BASE_ADDR + 0x80))
#define rIIC_SDA_BIT_SHIFT          12
#define rIIC_SDA_DIR_BIT_REG        (*(volatile unsigned *)(RISC_BASE_ADDR + 0x84))
#define rIIC_SDA_DIR_BIT_SHIFT      12
#define rIIC_SCL_BIT_REG            (*(volatile unsigned *)(RISC_BASE_ADDR + 0x80))
#define rIIC_SCL_BIT_SHIFT          13
#define rIIC_SCL_DIR_BIT_REG        (*(volatile unsigned *)(RISC_BASE_ADDR + 0x84))
#define rIIC_SCL_DIR_BIT_SHIFT      13

/* data line direction control macro */
#define IIC_SDA_OUT()       \
        do {\
            OS_ENTER_CRITICAL();\
            (rIIC_SDA_DIR_BIT_REG &= ~(0x1 << rIIC_SDA_DIR_BIT_SHIFT));\
            OS_EXIT_CRITICAL();\
        } while (0)
        
#define IIC_SDA_IN()        \
        do {\
            OS_ENTER_CRITICAL();\
            (rIIC_SDA_DIR_BIT_REG |= (0x1 << rIIC_SDA_DIR_BIT_SHIFT));\
            OS_EXIT_CRITICAL();\
        } while (0)

/* data line level control marco */
#define IIC_SDA_SET()       \
        do {\
            OS_ENTER_CRITICAL();\
            (rIIC_SDA_BIT_REG |= (0x1 << rIIC_SDA_BIT_SHIFT));\
            OS_EXIT_CRITICAL();\
        } while (0)

#define IIC_SDA_CLR()       \
        do {\
            OS_ENTER_CRITICAL();\
            (rIIC_SDA_BIT_REG &= ~(0x1 << rIIC_SDA_BIT_SHIFT));\
            OS_EXIT_CRITICAL();\
        } while (0)

#define IIC_SDA_GET()       ((rIIC_SDA_BIT_REG >> rIIC_SDA_BIT_SHIFT) & 0x1)

/* clock line direction control macro */
#define IIC_SCL_OUT()       \
        do {\
            OS_ENTER_CRITICAL();\
            (rIIC_SCL_DIR_BIT_REG &= ~(0x1 << rIIC_SCL_DIR_BIT_SHIFT));\
            OS_EXIT_CRITICAL();\
        } while (0)

#define IIC_SCL_IN()        \
        do {\
            OS_ENTER_CRITICAL();\
            (rIIC_SCL_DIR_BIT_REG |= (0x1 << rIIC_SCL_DIR_BIT_SHIFT));\
            OS_EXIT_CRITICAL();\
        } while (0)

/* clock line level control marco */
#define IIC_SCL_SET()       \
        do {\
            OS_ENTER_CRITICAL();\
            (rIIC_SCL_BIT_REG |= (0x1 << rIIC_SCL_BIT_SHIFT));\
            OS_EXIT_CRITICAL();\
        } while (0)

#define IIC_SCL_CLR()       \
        do {\
            OS_ENTER_CRITICAL();\
            (rIIC_SCL_BIT_REG &= ~(0x1 << rIIC_SCL_BIT_SHIFT));\
            OS_EXIT_CRITICAL();\
        } while (0)

#define IIC_ACK_ERROR   -1
#define IIC_ACK_SUCCESS 1
#define IIC_ERROR   -1
#define IIC_OK      1

void IICDelayLoop();
void IICDelay(int cnt);
void IICinit(void);
void IICStart(void);         /* statr the iic bus */
void IICStop(void);          /* stop the iic bus */
void IICStop2(void);
int  IICRead(void);          /* read the iic bus */
void IICWrite(int buf);      /* write the iic bus */
int IICChkAck(void);         /* chack the acknowledge signel */
void IICChkNAck(void);
void IICSendAck(void);       /* send the acknowledge signel */
void IICSendNAck(void);

void IICStartTVP(void);         /* statr the iic bus */
void IICStopTVP(void);          /* stop the iic bus */
int  IICReadTVP(void);          /* read the iic bus */
void IICWriteTVP(int buf);      /* write the iic bus */
int IICChkAckTVP(void);         /* chack the acknowledge signel */
void IICSendNAckTVP(void);



int IICWriteByte(int SlaveAddr, int ByteAddr, int data, int mode);
int IICWriteByteS(int SlaveAddr, int ByteAddr, int data, int cnt, int mode);
int IICReadByte(int SlaveAddr, int ByteAddr, int *data, int mode);
int IICReadByteS(int SlaveAddr, int ByteAddr, int *data, int cnt, int mode);
int IICReadWord(int SlaveAddr, int ByteAddr, int *data, int mode);
int IICWriteWord(int SlaveAddr, int ByteAddr, int data, int mode);
int IICReadByteADV(int SlaveAddr, int ByteAddr, int *data, int mode);
int IICWriteByteTVP(int SlaveAddr, int ByteAddr, int data, int mode);
int IICReadByteTVP(int SlaveAddr, int ByteAddr, int *data, int mode);

