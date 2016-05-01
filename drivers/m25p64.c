/*
***************************************************************************
*                  Copyright (c) 2005 vBridge Microsystem, Inc.  
*                        Unpublished & Not for Publication
*                               All Rights Reserved                   
*   
* File        :                      
*                                                                         
* Description :    
*             
* Date        : Aug 11, 2005                                                           
***************************************************************************
*/



#include "sif.h"
#include "m25p64.h"

unsigned char printf (char *pcFmt,...);
char WaitPressKey(void);



/*
***************************************************************************
*                              SifInit
*
* Description: Initialize the serial flash interface.
*
* Arguments  : 
*
* Return     : 
*
* Note(s)    : 
***************************************************************************
*/
void SflashSifInit(void)
{   
    while(SifBusyRead());      // If a transaction doesn't end, 
                                            // the initialization cannot be started.

    SifTimingRegSet(SIF_SI_TIMING_RISING, SIF_SO_TIMING_RISING, SIF_CLK_POL_IDLE1,\
                    SIF_CS_WIDTH_OFF_NONE, SIF_CS_DLY_2CLK, SIF_CS_OFF_2CLK, \
                    SIF_GAP_NONE, SIF_BGAP_1BIT, 1 /* clk_div */);    // right one           
    SifModeRegSet(SIF_CBUS_MODE, SIF_CS_n0, SIF_CTL_SEL_8BIT, SIF_ADD_SEL_3BYTE,\
                  SIF_READ, 0 /* sID */, 4 /* blen */);

    SifIntrDis();                             // Disable the SIF interrupt.
}

/*
***************************************************************************
*                             SflashReadByte
*
* Description: Read byte from the serial flash.
*
* Arguments  : start_addr    is the start address.
*              data          is the data pointer.
*              number        is the number to be read. Counted by bytes.
*
* Return     : none.
*
* Note(s)    : 
***************************************************************************
*/
void SflashReadByte(int start_addr, char *data, int number)
{   
    int i;
    
    SflashWaitTillReady();        // Wait till the internal Program, 
                                  // Erasure, & Status Register Write complete
    
    SifModeRegSet(SIF_CBUS_MODE, SIF_CS_n0, SIF_CTL_SEL_8BIT, SIF_ADD_SEL_3BYTE, \
                  SIF_READ, 0 /* sID */, 1/* blen */);
    SifCommandSet(SIF_INSTR_READ);    // Write the read instruction into the register.
    SifAddrSet(start_addr);   // Write the start address into the register.    
                              // Do not need set the address every time,
                              // because the adress register will add by blen automatically.
                              
    for(i = 0; i < number; i++) {
        SifStart();           // Start SPI operation. 
        while(SifBusyRead());
        *data++ = SifDataRead();	        
    }
}

/*
*********************************************************************************************
*                                       SflashReadWord
*
* Description: Read 4-byte from the serial flash.
*
* Arguments  : start_addr    is the start address.
*              data          is the data pointer.
*              number        is the number to be read. Counted by words.
*
* Return     : none.
*
* Note(s)    : Add on Feb 21
*********************************************************************************************
*/
void SflashReadWord(int start_addr, int *data, int number)
{   
    int i;
    
    SflashWaitTillReady();        // Wait till the internal Program, 
                                  // Erasure, & Status Register Write complete
    
    SifModeRegSet(SIF_CBUS_MODE, SIF_CS_n0, SIF_CTL_SEL_8BIT, SIF_ADD_SEL_3BYTE, \
                  SIF_READ, 0 /* sID */, 4/* blen */);
    SifCommandSet(SIF_INSTR_READ);    // Write the read instruction into the register.
    SifAddrSet(start_addr);   // Write the start address into the register.    
                              // Do not need set the address every time,
                              // because the adress register will add by blen automatically.
                              
    for(i = 0; i < number; i++) {
        SifStart();           // Start SPI operation. 
        while(SifBusyRead());
        *data++ = SifDataRead();	        
    }
}

/*
*********************************************************************************************
*                                       SflashReadStatus
*
* Description: Read the status registers.
*
* Arguments  : 
*
* Return     : none.
*
* Note(s)    : 
*********************************************************************************************
*/
char SflashReadStatus(void)
{   
    int i;
    
    while(SifBusyRead());       // If a transaction doesn't end, new one cannot be started.
                                   
    SifModeRegSet(SIF_CBUS_MODE, SIF_CS_n0, SIF_CTL_SEL_8BIT, SIF_ADD_SEL_NONE, \
                  SIF_READ, 0 /* sID */, 1 /* blen */);	
    SifCommandSet(SIF_INSTR_RDSR);            // Write the read instruction into the register.
    SifStart();                               // Start SPI operation.

    while(SifBusyRead());
    
    return(SifDataRead());	
}

/*
*********************************************************************************************
*                                       SflashReadID
*
* Description: Read byte from the serial flash.
*
* Arguments  : 
*
* Return     : none.
*
* Note(s)    : 
*********************************************************************************************
*/
void SflashReadID(char *pM_id, char *pMem_type, char *pMem_cap)
{   
    int tmp;
    
    SflashWaitTillReady();        // Wait till the internal Program, 
                                  // Erasure, & Status Register Write complete
    
    SifModeRegSet(SIF_CBUS_MODE, SIF_CS_n0, SIF_CTL_SEL_8BIT, SIF_ADD_SEL_NONE, \
                  SIF_READ, 0 /* sID */, 3/* blen */);
    SifCommandSet(SIF_INSTR_RDID);    // Write the read instruction into the register.
                              
    SifStart();           // Start SPI operation. 
    while(SifBusyRead());
    tmp = SifDataRead();	    
    
    *pM_id = tmp & 0xff;    
    *pMem_type = (tmp & 0xff00) >> 8; 
    *pMem_cap = (tmp & 0xff0000) >> 16;
}

/*
*********************************************************************************************
*                                       SflashProgByte
*
* Description: Program the serial flash.
*
* Arguments  : start_addr    is the start address.
*              data          is the data pointer
*              number        is the number to be programmed. Counted by bytes.                            
*
* Return     : none.
*
* Note(s)    : number does not have limited.
*********************************************************************************************
*/
void SflashProgByte(int start_addr, char *data, int number)
{
    int i;	

    while(SifBusyRead());       // If a transaction doesn't end, new one cannot be started.
    
    for(i = 0; i < number; i++) {
    	SflashWriteEn();
    
        SifModeRegSet(SIF_CBUS_MODE, SIF_CS_n0, SIF_CTL_SEL_8BIT, SIF_ADD_SEL_3BYTE, \
                      SIF_WRITE, 0 /* sID */, 1 /* blen */);		
        SifCommandSet(SIF_INSTR_PP);            
      
        SifAddrSet(start_addr++);       
        
        SifDataWrite(*data++);	
        SifStart();           // Start SPI operation. 
        while(SifBusyRead());
    }    
}

/*
*********************************************************************************************
*                                       SflashProgWord
*
* Description: Program the serial flash by word.
*
* Arguments  : start_addr    is the start address.
*              data          is the data pointer
*              number        is the number to be programmed. Counted by words.                            
*
* Return     : none.
*
* Note(s)    : number does not have limited.
*********************************************************************************************
*/
void SflashProgWord(int start_addr, int *data, int number)
{
    int i;	

    while(SifBusyRead());       // If a transaction doesn't end, new one cannot be started.
    
    for(i = 0; i < number; i++) {
    	SflashWriteEn();
    
        SifModeRegSet(SIF_CBUS_MODE, SIF_CS_n0, SIF_CTL_SEL_8BIT, SIF_ADD_SEL_3BYTE, \
                      SIF_WRITE, 0 /* sID */, 4 /* blen */);		
        SifCommandSet(SIF_INSTR_PP);            
      
        SifAddrSet(start_addr);       
        start_addr += 4;
        
        SifDataWrite(*data++);	
        SifStart();           // Start SPI operation. 
        while(SifBusyRead());
    }    
}

/*
*********************************************************************************************
*                                       SflashSecErase
*
* Description: Erase a specific sector.
*
* Arguments  : sector_no   is the sector number to be erased.
*
* Return     : none.
*
* Note(s)    : 
*********************************************************************************************
*/
void SflashSecErase(int sector_no)
{
    SflashWriteEn();	
    
    SifModeRegSet(SIF_CBUS_MODE, SIF_CS_n0, SIF_CTL_SEL_8BIT, SIF_ADD_SEL_3BYTE, \
                  SIF_WRITE, 0 /* sID */, 0 /* blen */);		
    SifCommandSet(SIF_INSTR_SE);            
    SifAddrSet(sector_no * SIF_PAGES_PER_SECTOR * SIF_PAGE_SIZE);                   // Write the start address into the register.
    SifStart();                                // Start SPI operation.
}

/*
*********************************************************************************************
*                                       SflashBulkErase
*
* Description: Erase the whole chip.
*
* Arguments  : none.
*
* Return     : none.
*
* Note(s)    : 
*********************************************************************************************
*/
void SflashBulkErase(void)
{
    SflashWriteEn();	
    
    SifModeRegSet(SIF_CBUS_MODE, SIF_CS_n0, SIF_CTL_SEL_8BIT, SIF_ADD_SEL_NONE, \
                  SIF_WRITE, 0 /* sID */, 0 /* blen */);		
    SifCommandSet(SIF_INSTR_BE);            
    SifStart();                               // Start SPI operation.
}

/*
*********************************************************************************************
*                                       SflashWriteEn
*
* Description: set the Write Enable Latch bit.
*
* Arguments  : none.
*
* Return     : none.
*
* Note(s)    : The Write Enable Latch (WEL) bit must be set prior to 
*              every Page Program (PP), Sector Erase(SE), Bulk Erase (BE) and 
*              Write Status Register(WRSR) instruction.
*********************************************************************************************
*/
void SflashWriteEn(void)
{
    SflashWaitTillReady();
    
    SifModeRegSet(SIF_CBUS_MODE, SIF_CS_n0, SIF_CTL_SEL_8BIT, SIF_ADD_SEL_NONE, \
                  SIF_WRITE, 0 /* sID */, 0 /* blen */);	
    SifCommandSet(SIF_INSTR_WREN);            
    SifStart();                               // Start SPI operation.
    
    while(!(SflashReadStatus() & 0x2));  // Wait till the WEL bit is set.    
}

/*
*********************************************************************************************
*                                       SflashWaitTillReady
*
* Description: wait till the serial flash is ready.
*
* Arguments  : none.
*
* Return     : none.
*
* Note(s)    : 
*********************************************************************************************
*/
void SflashWaitTillReady(void)
{    
    while(SflashReadStatus() & 0x1);             // Wait till the internal erase, 
                                                 // program or Status Write ends.
}

/*
*********************************************************************************************
*                               Following functions will be used in serial testing!
*********************************************************************************************
*/

/*
*********************************************************************************************
*                                       SflashProgData
*
* Description: Program the whole serial flash with the same word.
*
* Arguments  : prog_data     is the program data, 4-byte.
*
* Return     : none.
*
* Note(s)    : The routine is OK under revised 20, but NOT supported in image 33
*********************************************************************************************
*/
void SflashProgData(int prog_data)
{
    int i, addr;	

    addr = 0;
    
    while(SifBusyRead());       // If a transaction doesn't end, new one cannot be started.
    
    printf("Sector ");
    for(i = 0; i < SIF_SECTOR_NUMBER * SIF_PAGES_PER_SECTOR; i++) {    	
    	if(!(i & (SIF_PAGES_PER_SECTOR - 1)))
    	    printf("%d, ", i/SIF_PAGES_PER_SECTOR);
    	SflashWriteEn();
    
        SifModeRegSet(SIF_CBUS_MODE, SIF_CS_n0, SIF_CTL_SEL_8BIT, SIF_ADD_SEL_3BYTE, \
                      SIF_WRITE, 0 /* sID */, SIF_PAGE_SIZE /* blen */);		
        SifCommandSet(SIF_INSTR_PP);            
      
        SifAddrSet(addr);
        addr += SIF_PAGE_SIZE;
                       
        SifDataWrite(prog_data);	
        SifStart();           // Start SPI operation. 
        while(SifBusyRead());
    }   
    printf("complete!\r\n"); 
}

/*
*********************************************************************************************
*                                       SflashSectorVerify
*
* Description: Read the word from the sector one by one, compare with verify_data.
*
* Arguments  : no_sector      is the sector number.
*              verify_data    is the 4-byte compared.
*
* Return     : SUCCESSFUL:    all the byte are equal to verify_data.
*              FAILED:        at least one byte is not qeual to verify_data.
*
* Note(s)    : 
*********************************************************************************************
*/
int SflashSectorVerify(int no_sector, int verify_data)
{    
    int i, tmp;
    
    SflashWaitTillReady();        // Wait till the internal Program, 
                                  // Erasure, & Status Register Write complete
    
    SifModeRegSet(SIF_CBUS_MODE, SIF_CS_n0, SIF_CTL_SEL_8BIT, SIF_ADD_SEL_3BYTE, \
                  SIF_READ, 0 /* sID */, 4/* blen */);
    SifCommandSet(SIF_INSTR_READ);    // Write the read instruction into the register.
    SifAddrSet(no_sector * SIF_PAGES_PER_SECTOR * SIF_PAGE_SIZE);  
                              
    for(i = 0; i < SIF_PAGES_PER_SECTOR * SIF_PAGE_SIZE; i = i + 4) {
        SifStart();                   // Start SPI operation. 
        while(SifBusyRead());
        tmp = SifDataRead();
        if (tmp != verify_data) {
             printf("%x\r\n", tmp);
             printf("%x\r\n", verify_data);
             WaitPressKey();        
            //return FAILED;	
        }        
    }
    
    return SUCCESSFUL;          
}

/*
*********************************************************************************************
*                                       SflashProgSectorWord
*
* Description: Program the specified sector with the same word.
*
* Arguments  : sector_no     is the sector number.
*              prog_data     is the program data, 4-byte.
*
* Return     : none.
*
* Note(s)    : add on Feb 21
*********************************************************************************************
*/
void SflashProgSectorWord(int sector_no, int prog_data)
{
    int i, addr;	

    addr = sector_no * SIF_BYTES_PER_SECTOR;
    
    while(SifBusyRead());       // If a transaction doesn't end, new one cannot be started.
    
    //printf("Sector ");
    for(i = 0; i < SIF_PAGES_PER_SECTOR; i++) {    	
    	SflashWriteEn();
    
        SifModeRegSet(SIF_CBUS_MODE, SIF_CS_n0, SIF_CTL_SEL_8BIT, SIF_ADD_SEL_3BYTE, \
                      SIF_WRITE, 0 /* sID */, SIF_PAGE_SIZE /* blen */);		
        SifCommandSet(SIF_INSTR_PP);            
      
        SifAddrSet(addr);
        addr += SIF_PAGE_SIZE;
                       
        SifDataWrite(prog_data);	
        SifStart();           // Start SPI operation. 
        while(SifBusyRead());
    }   
    //printf("complete!\r\n"); 
}
