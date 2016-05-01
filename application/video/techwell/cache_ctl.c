#include "cache_ctl.h"
#include "cpu.h"

void invalid_icache(unsigned long start, unsigned long length)
{
	unsigned long end = (start + length);

	while(start < end) {
		cache_unroll(start,Index_Inv_I);
		start += 16;
	}
}

void invalid_dcache(unsigned long start, unsigned long length)
{
	unsigned long end = (start + length);

	while(start < end) {
		cache_unroll(start,Index_Writeback_Inv_D);
		start += 16;
	}
}

void lock_icache(unsigned long start, unsigned long length)
{
	unsigned long end = (start + length);

	while(start < end) {
		cache_unroll(start,Index_lock_I);
		start += 16;
	}
}

void lock_dcache(unsigned long start, unsigned long length)
{
	unsigned long end = (start + length);

	while(start < end) {
		cache_unroll(start,Index_lock_D);
		start += 16;
	}
}

int itag_rd(int index) {
	cache_unroll(index, Index_rd_itag);
	return(mips_gettaglo());
}
 	
int dtag_rd(int index) {
	cache_unroll(index, Index_rd_dtag);
	return(mips_gettaglo());
}
