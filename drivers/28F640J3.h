/*
*********************************************************************************************************
*                                Copyright (c) 2005 vBridge Microsystem, Inc.  
*                                      Unpublished & Not for Publication
*                                            All Rights Reserved                   
*   
* File        : 28F640J3.h                     
*                                                                         
* Description : the intel nor flash 28F640J3 device command definitions.  
*               28F640J3 erase one sector at a time. It has no all-block erasure command.
*               Program only one byte or half-word at a time, from 1 to 0, NOT 0 to 1.
*               The number of read at a time is not confined.  
*             
* Date        : Dec 1, 2005                                                           
**********************************************************************************************************
*/


#define  NOR_FLASH_SECTOR_NUMBER           64        // Erase a sector at a time.
#define  NOR_FLASH_BYTES_PER_SECTOR        0x20000   /* Program just byte or halfword at a time.
                                                        From 1 to 0, cannot from 0 to 1. */
#define  NOR_FLASH_HALFWORDS_PER_SECTOR    0x10000                                                        

#define  NOR_FLASH_M_ID         0x89
#define  NOR_FLASH_DEVICE_ID    0x17

// 28F640J3 device commands
#define  NOR_FLASH_RD_ARRAY          0xff    // one write cycle
#define  NOR_FLASH_RD_ID             0x90    // one write cycle, at leat one read cycle 
#define  NOR_FLASH_RD_SR             0x70    // one write cycle, one read cycle 
#define  NOR_FLASH_CL_SR             0x50    // one write cycle
#define  NOR_FLASH_PROG_WORD         0x40    // two write cycles
#define  NOR_FLASH_PROG_BYTE         0x10    // two write cycles

#define  NOR_FLASH_ERASE_BLOCK1      0x20    // the first write cycle for block erase operation
#define  NOR_FLASH_ERASE_BLOCK2      0xd0    // the second write cycle for block erase operation

#define  NOR_FLASH_SET_LOCKBIT1      0x60    // the first write cycle for setting block lock-bit
#define  NOR_FLASH_SET_LOCKBIT2      0x1     // the second write cycle for setting block lock-bit

#define  NOR_FLASH_CLR_LOCKBIT1      0x60    // the first write cycle for clearing block lock-bit
#define  NOR_FLASH_CLR_LOCKBIT2      0xd0    // the second write cycle for clearing block lock-bit


/*
*********************************************************************************************************
*                              FUCTION PROTYPES
*********************************************************************************************************
*/
void  NorFlashPifInit(void);
int   NorFlashErase(int block_no);
int   NorFlashProg(int addr, short data);
void  NorFlashClrSR(void);
short NorFlashRdSR(void);
void  NorFlashRdID(short *mID, short *deviceID, short *block_lock_cfg);
void  NorFlashRdData(int start_addr, short *data, int number);
void  NorFlashRdDataBurst(int start_addr, unsigned short *data, int number);
void  NorFlashRd1(int start_addr, short *data, int number);
void  NorFlashRd(int start_addr, short *data, int number);
int   NorFlashSectorVerify (int no_sector, short int verify_data);
int   NorFlashSectorVerify1(int no_sector, short int verify_data);
int   NorFlashProgSector(int sec_no, short data);
