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


void cache_hit_writeback_D(char *s);
void cache_hit_invalidate_D(char *s);
void cache_hit_invalidate_I(char *s);
void cache_hit_writeback_invalidate_D(char *s);
void cache_index_writeback_invalidate_D_all(void);

void cache_writeback_D(void *buf, unsigned int nbytes);
void cache_invalidate_D(void *buf, unsigned int nbytes);
void cache_writeback_invalidate_D(void *buf, unsigned int nbytes);
void cache_invalidate_I(void *buf, unsigned int nbytes);
