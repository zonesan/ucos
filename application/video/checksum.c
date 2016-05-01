#include "two_risc_statistics.h"
#include "checksum.h"
#include "2risc_memory_map.h"
#include "../drivers/risc.h"
#include "../drivers/comm.h"


#ifdef DISPLAY_INCLUDE
/*
***************************************************************************
*                          encode data check summary
*
* Description: 
*
* Arguments  : 
*
* Return     :     length -4 for the last 4bytes is checksum;
*
* Note(s)    :  length is counted by bytes, but should be multiple of sizeof(int)!
***************************************************************************
*/
extern unsigned int linuxToUcosFrameNum;
unsigned int encodeDataChksum(unsigned int * addr, unsigned int length)
{
    unsigned int sum = 0,i,len;
    len = length / sizeof(int) - 4;     //actual length

    addr = (unsigned int *)((int)addr & 0x1fffffff | 0x80000000);
    cache_writeback_invalidate_D(addr, length);
    
    if(length-16 != (*(addr+len)))//THE length is contain data length and chksum that is 8 bytes
    {
        printf("\n\nencodeDataChksum: frame length(%d,%d) error!\n\n", length, *(addr+len));
        gtStatistics.cnt_framelength_err++;
        return 1;  
    }
    
    for(i=0;i<len;i++)
    {
        sum += *addr++;      
    }
    
    if(sum != *(addr+1)) //the check sum
    {
        printf("\n\nencodeDataChksum: checksum(0x%x,0x%x) error!\n\n", sum, *(addr + 1));
        gtStatistics.cnt_checksum_err++;
        return 2;         
    }
    
    if((linuxToUcosFrameNum + 1) != *(addr +  2)) {
        printf("\n\nencodeDataChksum: frame num error(%d,%d)\n\n", linuxToUcosFrameNum, *(addr + 2));
        gtStatistics.cnt_frame_number_err++;
    }

    linuxToUcosFrameNum = *(addr + 2);
  
    return 0;   
}
#endif

