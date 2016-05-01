/*
***************************************************************************
*                Copyright (c) 2007 vBridge Microsystem, Inc.  
*                       Unpublished & Not for Publication
*                             All Rights Reserved                   
*   
* File:         cache operation function for 4kec
*                                                                         
* Description :                     
*
* Date:         Dec 11, 2007                                                           
***************************************************************************
*/

#include <sysdefs.h>
#include  "mips_regdef.h"
#include  "cp0regdef.h"
#include "cache_s.h"

      .set noreorder

LEAF(cache_hit_writeback_D)

      cache	Hit_Writeback_D, 0(a0)
      nop

      sync    # avoid memory hazard
      jr	ra
      nop

END(cache_hit_writeback_D)

# Note: please be careful using it. Cache invalidate will clear the cache dirty flag!
# Comment it to avoid mis-using
/*
LEAF(cache_hit_invalidate_D)

      cache	Hit_Invalidate_D, 0(a0)
      nop

      jr	ra
      nop

END(cache_hit_invalidate_D)
*/

LEAF(cache_hit_writeback_invalidate_D)

      cache	Hit_Writeback_Invalidate_D, 0(a0)
      nop

      sync    # avoid memory hazard
      jr	ra
      nop

END(cache_hit_writeback_invalidate_D)

/*
Not recommended by "See Mips Run"!
*/
LEAF(cache_index_writeback_invalidate_D_all)

      lui t0, 0x8000
      li  t1, 1024    # D-cache sets of 4kec

1:    cache	Index_Writeback_Invalidate_D, 0(t0)
      add	t1, -1			# Decrement set counter

      bne	t1, zero, 1b
      add	t0, 16		# Get next line address

      sync
      jr	ra
      nop

END(cache_index_writeback_invalidate_D_all)

LEAF(cache_hit_invalidate_I)

      cache	Hit_Invalidate_I, 0(a0)
      nop

      jr	ra
      nop

END(cache_hit_invalidate_I)

# just save C0_TAGLO. C0_TAGHI is useless.
LEAF(cache_index_read_D)

      mtc0	zero, C0_TAGLO
      mtc0	zero, C0_TAGHI

      cache	Index_Load_Tag_D, 0(a0)
      nop

      mfc0	v0, C0_TAGLO

      jr	ra
      nop

END(cache_index_read_D)

      .set reorder
