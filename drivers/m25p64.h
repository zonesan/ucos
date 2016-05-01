/*
*********************************************************************************************************
*                                Copyright (c) 2005 vBridge Microsystem, Inc.  
*                                      Unpublished & Not for Publication
*                                            All Rights Reserved                   
*   
* File        : m25p64.h                     
*                                                                         
* Description : the serial flash m25p64 page size, command definitions.   
*               m25p64 erase one sector or all the block at a time.
*               The page program instruction allows up to 256 bytes to be programmed.
*               Before the Erase, Program & Write Status Register instruction, Write Enable instruction 
*               should be applied.
*               The read instruction can read as many as possible bytes at a time.
* Date        : Dec 1, 2005                                                           
**********************************************************************************************************
*/


// FPGA image version
#define  FPGA_IMAGE33

#define  SIF_SECTOR_NUMBER           128    // Erase a sector at a time or all the flash.
#define  SIF_PAGES_PER_SECTOR        256
#define  SIF_PAGE_SIZE               256
#define  SIF_BYTES_PER_SECTOR        (SIF_PAGES_PER_SECTOR * SIF_PAGE_SIZE)

#define  M_ID        0x20    // Manufacture ID for STMicroelectronics
#define  MEM_TYPE    0x20    // indicates the memory type
#define  MEM_CAP     0x17    // indicates the memory capacity


// m25p64 instructions set
#define  SIF_INSTR_WREN          0x06    // address bytes: 0; dummy bytes: 0; data bytes: 0     
#define  SIF_INSTR_WRDI          0x04    // address bytes: 0; dummy bytes: 0; data bytes: 0
#define  SIF_INSTR_RDID          0x9f    // address bytes: 0; dummy bytes: 0; data bytes: 1 to 3
#define  SIF_INSTR_RDSR          0x05    // address bytes: 0; dummy bytes: 0; data bytes: 1 to infinity
#define  SIF_INSTR_WRSR          0x01    // address bytes: 0; dummy bytes: 0; data bytes: 1
#define  SIF_INSTR_READ          0x03    // address bytes: 3; dummy bytes: 0; data bytes: 1 to infinity
#define  SIF_INSTR_FAST_READ     0x0b    // address bytes: 3; dummy bytes: 1; data bytes: 1 to infinity
#define  SIF_INSTR_PP            0x02    // address bytes: 3; dummy bytes: 0; data bytes: 1 to 256
#define  SIF_INSTR_SE            0xd8    // address bytes: 3; dummy bytes: 0; data bytes: 0
#define  SIF_INSTR_BE            0xc7    // address bytes: 0; dummy bytes: 0; data bytes: 0
#define  SIF_INSTR_RES           0xab    // address bytes: 0; dummy bytes: 3; data bytes: 1 to infinity



/*
*********************************************************************************************************
*                              FUCTION PROTYPES
*********************************************************************************************************
*/
void SflashSifInit(void);
void SflashReadByte(int start_addr, char *data, int number);
void SflashReadWord(int start_addr, int *data, int number);
char SflashReadStatus(void);
void SflashReadID(char *pM_id, char *pMem_type, char *pMem_cap);
void SflashProgByte(int start_addr, char *data, int number);
void SflashProgWord(int start_addr, int  *data, int number);
void SflashSecErase(int sector_no);
void SflashBulkErase(void);
void SflashWriteEn(void);
void SflashWaitTillReady(void);

void SflashProgData(int prog_data);
int  SflashSectorVerify(int no_sector, int verify_data);
void SflashProgSectorWord(int sector_no, int prog_data);

