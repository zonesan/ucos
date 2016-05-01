#define Index_Inv_I   0x00
#define Index_Writeback_Inv_D   0x01
#define Index_lock_I   0x1c
#define Index_lock_D   0x1d
#define Index_rd_itag  0x4
#define Index_rd_dtag  0x5

#define cache_unroll(base,op)                   \
        __asm__ __volatile__("                  \
                .set noreorder;                 \
                .set mips3;                     \
                cache %1, (%0);                 \
                .set mips0;                     \
                .set reorder"                   \
                :                               \
                : "r" (base),                   \
                  "i" (op));

void invalid_icache(
	unsigned long start, 
	unsigned long length);

void invalid_dcache(
	unsigned long start, 
	unsigned long length);

void lock_icache(
	unsigned long start, 
	unsigned long length);

void lock_dcache(
	unsigned long start, 
	unsigned long length);

int itag_rd(int index);

int dtag_rd(int index);

