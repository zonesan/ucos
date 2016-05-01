/*
**************************************************************************************
*                            Copyright (c) 2005 vBridge Microsystem, Inc.  
*                                  Unpublished & Not for Publication
*                                        All Rights Reserved                   
*   
* File        : 28F640J3.c                     
*                                                                         
* Description : 
*             
* Date        :                                                            
**************************************************************************************
*/



#include "pif.h"
#include "28F640J3.h"
#include "comm.h"



/*
**************************************************************************************
*                                       NorFlashPifInit
*
* Description: initialize the PCMCIA interface
*
* Arguments  : devID     controls which PCMCIA device to access. 
*                                  0 每 CS0
*                                  1 每 CS1
*                                  2 每 CS2
*                                  3 每 CS3
*                            For Virtex-4, devID = 0.
*
* Return     : none
*
* Note(s)    : Before any Nor flash operations, you should call it at first.
**************************************************************************************
*/
/*4> word 0xa80a0000
0x04000000
5> word 0xa80a0010
0x890F0FFF
6>*/ 
void NorFlashPifInit(void)
{
    unsigned int tmp;
    
    PifPCMCIAModeRegSet(PIF_CBUS_MODE, PCMCIA_MSTR_BS_8BYTE);    
    
    PifTimingRegSet(0xf /* setup */, 0xf /* hold */, 0xf /* interval */, \
                    0xf /* wait_cycle */, PIF_NO_WAIT /* ioack_wait */, \
                    PCMCIA_WIDTH_16BIT /* pcmcia_width */, PCMCIA_NO_CHECK /* wait */, \
                    PCMCIA_INSERT_IDLE /* swait */, PIF_CS0 /* devID */, \
                    PIF_OE /* xfr_type */);
    
    PIF_INTR_DIS();       // Disable the PIF interrupts
    PIF_DONE_CLEAR();     // Clear the bit done
    
    tmp = *(unsigned int *)0xa80a001c;
    tmp &= 0x7fffffff;
    *(unsigned int *)0xa80a001c = tmp;
}

/*
*********************************************************************************************
*                                       NorFlashErase
*
* Description: erase one specified block.
*
* Arguments  : block_no     is the block number. 
*
* Return     : SUCCESSFUL  shows the erasure is successful.
*              FAILED      shows the erasure is failed.
*
* Note(s)    : 
*********************************************************************************************
*/
int NorFlashErase(int block_no)
{
    short status;
    	
    /* the first cycle of block erase */	
    PIF_WRITE_SET();
    rPIF_PCI_LEN = 2;
    rPIF_PCI_DEV_A = block_no * NOR_FLASH_BYTES_PER_SECTOR;    // the byte address
    PIF_START();
    rPIF_PCMCIA_DATA = NOR_FLASH_ERASE_BLOCK1;
    
    PifWaitTillDone();         // Wait till the first cycle complete
    
    /* the second cycle of block erase */	
    PIF_START();
    rPIF_PCMCIA_DATA = NOR_FLASH_ERASE_BLOCK2;
    
    PifWaitTillDone();         // Wait till the second cycle complete
    
    status = 0x0;
    
    while(!(status & 0x80)) {     // Flash is busy?
        NorFlashRd(0, &status, 1);
    }
    if(status & 0x30)             // Whether the SR4 or SR5 is set
        return FAILED;
    return SUCCESSFUL;
}

/*
*********************************************************************************************
*                                       NorFlashProg
*
* Description: program a half-word into the specified address
*
* Arguments  : addr     is the byte address. 
*              data     is the half-word to be programmed.
*
* Return     : SUCCESSFUL  shows the erasure is successful.
*              FAILED      shows the erasure is failed.
*
* Note(s)    : 
*********************************************************************************************
*/
int NorFlashProg(int addr, short data)
{
    short status;
    	
    /* the first cycle */	
    PIF_WRITE_SET();
    rPIF_PCI_LEN = 2;
    PIF_START();
    rPIF_PCMCIA_DATA = NOR_FLASH_PROG_WORD;
    
    PifWaitTillDone();         // Wait till the first cycle complete
    
    /* the second cycle */	
    rPIF_PCI_DEV_A = addr;     // the byte address
    PIF_START();
    rPIF_PCMCIA_DATA = data;
    
    PifWaitTillDone();         // Wait till the second cycle complete
    
    status = 0x0;
    
    while(!(status & 0x80)) {     // Flash is busy?
        NorFlashRd(0, &status, 1);
    }
    if(status & 0x1a)             // Whether the SR1 SR3 or SR4 is set
        return FAILED;
    return SUCCESSFUL;
}

/*
*********************************************************************************************
*                                       NorFlashClrSR
*
* Description: clr the flash status.
*
* Arguments  : none
*
* Return     : none
*
* Note(s)    : If FAILED returns after program or erase operation, 
*              you should run NorFlashClrSR before the next program or erase operation. 
*              The function does not verify.
*********************************************************************************************
*/
void NorFlashClrSR(void)
{           	
    PIF_WRITE_SET();
    rPIF_PCI_LEN = 2;
    PIF_START();
    rPIF_PCMCIA_DATA = NOR_FLASH_CL_SR;
    
    PifWaitTillDone();         // Wait till the first cycle complete	            
}

/*
*********************************************************************************************
*                                       NorFlashRdSR
*
* Description: read the flash status.
*
* Arguments  : none
*
* Return     : the flash status
*
* Note(s)    : 
*********************************************************************************************
*/
short NorFlashRdSR(void)
{   
    short status;
    	
    PIF_WRITE_SET();
    rPIF_PCI_LEN = 2;
    PIF_START();
    rPIF_PCMCIA_DATA = NOR_FLASH_RD_SR;
    
    PifWaitTillDone();         // Wait till the first cycle complete	
    
    NorFlashRd(0, &status, 1);
    return(status);
}

/*
*********************************************************************************************
*                                       NorFlashRdID
*
* Description: read the manufacture ID, device ID, and block lock configuration.
*
* Arguments  : mID              is the manufacture ID pointer.
*              deviceID         is the device ID pointer.
*              block_lock_cfg   is the block lock configuration.
*
* Return     : none.
*
* Note(s)    : 
*********************************************************************************************
*/
void NorFlashRdID(short *mID, short *deviceID, short *block_lock_cfg)
{
    short status;
    	
    PIF_WRITE_SET();
    rPIF_PCI_LEN = 2;
    PIF_START();
    rPIF_PCMCIA_DATA = NOR_FLASH_RD_ID;
    
    PifWaitTillDone();         // Wait till the first cycle complete	
    
    NorFlashRd(0, mID, 1);
    NorFlashRd(2, deviceID, 1);
    NorFlashRd(4, block_lock_cfg, 1);
}

/*
*********************************************************************************************
*                                       NorFlashRdData
*
* Description: read the data from the specified address.
*
* Arguments  : start_addr       is the start byte address.
*              data             is the data pointer.
*              number           is the data of number to be read. It is counted by half-words.
*
* Return     : none.
*
* Note(s)    : 
*********************************************************************************************
*/
void NorFlashRdData(int start_addr, short *data, int number)
{
    int i;	
	
    /* The read array command */	
    PIF_WRITE_SET();
    rPIF_PCI_LEN = 2;
    PIF_START();
    rPIF_PCMCIA_DATA = NOR_FLASH_RD_ARRAY;
    
    PifWaitTillDone();           // Wait till the first cycle complete
    
    NorFlashRd(start_addr, data, number);    	
}

/*
*********************************************************************************************
*                                       NorFlashRdDataBurst
*
* Description: read the data from the specified address.
*
* Arguments  : start_addr       is the start byte address.
*              data             is the data pointer.
*              number           is the data of number to be read. It is counted by half-words.
*                               Maximum is 2047. 
*
* Return     : none.
*
* Note(s)    : 
*********************************************************************************************
*/
void NorFlashRdDataBurst(int start_addr, unsigned short *data, int number)
{
    int i;	
	
    /* The read array command */	
    PIF_WRITE_SET();
    rPIF_PCI_LEN = 2;
    PIF_START();
    rPIF_PCMCIA_DATA = NOR_FLASH_RD_ARRAY;
    
    PifWaitTillDone();           // Wait till the first cycle complete
    
    NorFlashRd1(start_addr, data, number);    	
}

/*
*********************************************************************************************
*                                       NorFlashRd1
*
* Description: read the data from the specified address.
*
* Arguments  : start_addr       is the start byte address.
*              data             is the data pointer.
*              number           is the data of number to be read. Counted by short integers.
*
* Return     : none.
*
* Note(s)    : number should be less than 2047.
*********************************************************************************************
*/
void NorFlashRd1(int start_addr, short *data, int number)
{
    unsigned int i, tmp, iN, word_number;	
    

    //number &= 0x7ff;         // Maxium of byte length of the transfer is 4096 bytes

    PIF_READ_SET();
    rPIF_PCI_LEN = number * 2;     // rPIF_PCI_LEN is counted by bytes.
    rPIF_PCI_DEV_A = start_addr;   // the byte address
    PIF_START();
    
    if(number & 0x1) {
        word_number = number/2 + 1;
    } else
        word_number = number/2;     	
    
    iN = 0;
    for(i = 0; i < word_number; i++) {
        tmp = rPIF_PCMCIA_DATA;	
        *data++ = tmp & 0xffff;
        iN++;
        if (iN == number)
            break;
        *data++ = (tmp >> 16);
        iN++;
    }
}

/*
*********************************************************************************************
*                                       NorFlashRd
*
* Description: read the data from the specified address.
*
* Arguments  : start_addr       is the start byte address.
*              data             is the data pointer.
*              number           is the data of number to be read. It is counted by half-words.
*
* Return     : none.
*
* Note(s)    : 
*********************************************************************************************
*/
void NorFlashRd(int start_addr, short *data, int number)
{
    unsigned int i, tmp;	        

    PIF_READ_SET();
    rPIF_PCI_LEN = 2;          // rPIF_PCI_LEN is counted by bytes.  	
        
    for(i = 0; i < number; i++) {
    	rPIF_PCI_DEV_A = start_addr;    // the byte address
    	start_addr += 2;	    	
    	PIF_START();    	
        *data++ = rPIF_PCMCIA_DATA;	        
    }
}

/*
*********************************************************************************************
*                               Following functions will be used in NOR-FLASH testing!
*********************************************************************************************
*/

/*
*********************************************************************************************
*                                       NorflashSectorVerify
*
* Description: Read the short integers from the sector one by one, compare with verify_data.
*
* Arguments  : verify_data    is the short int compared.
*
* Return     : SUCCESSFUL:    all the short integers are equal to verify_data.
*              FAILED:        at least one short integer is not qeual to verify_data.
*
* Note(s)    : 
*********************************************************************************************
*/
int NorFlashSectorVerify(int no_sector, short int verify_data)
{   
    int i, j;
    int tmp1, tmp2;
    
    tmp2 = 0x400;
    tmp1 = NOR_FLASH_BYTES_PER_SECTOR / (2 * tmp2); 
    
    short int rd_data[tmp2];
    
    for(i = 0; i < tmp1; i++) {    	
        NorFlashRdData(no_sector * NOR_FLASH_BYTES_PER_SECTOR + i * tmp2 * 2, rd_data, tmp2);	
        for(j = 0; j < tmp2; j++) {
            if(rd_data[j] != verify_data) {         
                return FAILED;	
            }
        }
    }                
    return SUCCESSFUL;          
}

/*
*********************************************************************************************
*                                       NorflashSectorVerify1
*
* Description: Read the short integers from the sector one by one, compare with verify_data.
*
* Arguments  : verify_data    is the short int compared.
*
* Return     : SUCCESSFUL:    all the short integers are equal to verify_data.
*              FAILED:        at least one short integer is not qeual to verify_data.
*
* Note(s)    : 
*********************************************************************************************
*/
int NorFlashSectorVerify1(int no_sector, short int verify_data)
{ 
    char receive;	  
    int i, j;
    int tmp1, tmp2;
    int addr;
    
    tmp2 = 0x400;
    tmp1 = NOR_FLASH_BYTES_PER_SECTOR / (2 * tmp2); 
    
    short int rd_data[tmp2];
    
    for(i = 0; i < tmp1; i++) {    	
        NorFlashRdDataBurst(no_sector * NOR_FLASH_BYTES_PER_SECTOR + i * tmp2 * 2, rd_data, tmp2);
        for(j = 0; j < tmp2; j++) {
            if(rd_data[j] != verify_data) {
            	addr = no_sector * NOR_FLASH_BYTES_PER_SECTOR + i * tmp2 * 2 + j * 2;            	            	
            	printf("\r\nverify_data = 0x%x, flash data = 0x%x, byte addr = 0x%x, half-word addr = 0x%x", verify_data, rd_data[j], addr, addr/2);                               
            	receive = WaitPressKey();            	
            	verify_data++;
                if(receive == 'e')
                    return FAILED;
                if(receive == 'b')
                    break;                    	
            } else
                verify_data++;            
        }
    }                
    return SUCCESSFUL;          
}

/*
*********************************************************************************************
*                                       PifNorFlashProgSector
*
* Description: program a half-word into the specified sector
*
* Arguments  : sec_no     is the sector number. 
*              data       is the half-word to be programmed.
*
* Return     : SUCCESSFUL  shows the erasure is successful.
*              FAILED      shows the erasure is failed.
*
* Note(s)    : addr should be 2-byte aligned.
*********************************************************************************************
*/
int NorFlashProgSector(int sec_no, short data)
{
    int i;	
    
    for(i = 0; i < NOR_FLASH_BYTES_PER_SECTOR; i+=2) {
        if(NorFlashProg(sec_no * NOR_FLASH_BYTES_PER_SECTOR + i, data) == FAILED)
            return FAILED;    
    }
    return SUCCESSFUL;
}       
