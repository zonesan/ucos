/*
***************************************************************************
*                Copyright (c) 2007 vBridge Microsystem, Inc.  
*                       Unpublished & Not for Publication
*                             All Rights Reserved                   
*   
* File:         cache operation function
*                                                                         
* Description :                     
*
* Date:         Dec 11, 2007                                                           
***************************************************************************
*/


#include "cache.h"

/*
writeback D-cache by address.
buf: is the start address.
nbytes: is the buf size.
*/
void cache_writeback_D(void *buf, unsigned int nbytes)
{
    unsigned int lnsize = 16;  // for 4Kec, D-cache line size is 16
    
    buf = (void *)((int)buf & 0x1fffffff | 0x80000000);  // convert to the kseg0 address
    
    char *s;
    for(s = (char *)buf; s < buf + nbytes + lnsize; s += lnsize)
        cache_hit_writeback_D(s);
}

/*
invalidate D-cache by address.
buf: is the start address.
nbytes: is the buf size.
Note: please be careful using it. Cache invalidate will clear the cache dirty flag!Comment it to aoid mis-using!
*/
/*
void cache_invalidate_D(void *buf, unsigned int nbytes)
{
    unsigned int lnsize = 16;  // for 4Kec, D-cache line size is 16
    
    buf = (void *)((int)buf & 0x1fffffff | 0x80000000);  // convert to the kseg0 address
    
    char *s;
    for(s = (char *)buf; s < buf + nbytes + lnsize; s += lnsize)
        cache_hit_invalidate_D(s);
}
*/

/*
writeback & invalidate D-cache by address.
buf: is the start address.
nbytes: is the buf size.
*/
void cache_writeback_invalidate_D(void *buf, unsigned int nbytes)
{
    unsigned int lnsize = 16;  // for 4Kec, D-cache line size is 16
    
    buf = (void *)((int)buf & 0x1fffffff | 0x80000000);  // convert to the kseg0 address
    
    char *s;
    for(s = (char *)buf; s < buf + nbytes + lnsize; s += lnsize)
        cache_hit_writeback_invalidate_D(s);
}

/*
invalidate I-cache by address.
buf: is the start address.
nbytes: is the buf size.
*/
void cache_invalidate_I(void *buf, unsigned int nbytes)
{
    unsigned int lnsize = 16;  // for 4Kec, D-cache line size is 16
    
    buf = (void *)((int)buf & 0x1fffffff | 0x80000000);  // convert to the kseg0 address
    
    char *s;
    for(s = (char *)buf; s < buf + nbytes + lnsize; s += lnsize)
        cache_hit_invalidate_I(s);
}
