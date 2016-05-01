/*
***************************************************************************
*                Copyright (c) 2007 vBridge Microsystem, Inc.  
*                       Unpublished & Not for Publication
*                             All Rights Reserved                   
*   
* File:         cache operation code header file
*                                                                         
* Description :                     
*
* Date:         Dec 11, 2007                                                           
***************************************************************************
*/



/* all the cache operations code for cache instruction for mips 4KEC */

/* 
Set the state of the cache block at the specified
index to invalid.

This required encoding may be used by
software to invalidate the entire instruction
cache by stepping through all valid indices.
*/
#define Index_Invalidate_I    0x0

/* 
For a write-back cache: If the state of the cache
block at the specified index is valid and dirty,
write the block back to the memory address
specified by the cache tag. After that operation
is completed, set the state of the cache block to
invalid. If the block is valid but not dirty, set the
state of the block to invalid.

For a write-through cache: Set the state of the
cache block at the specified index to invalid.

This required encoding may be used by
software to invalidate the entire data cache by
stepping through all valid indices. Note that
Index Store Tag should be used to initialize the
cache at powerup.
*/
#define Index_Writeback_Invalidate_D    0x1

/*
Read the tag for the cache block at the specified
index into the TagLo and TagHi Coprocessor 0
registers.
*/
#define Index_Load_Tag_I    0x4
#define Index_Load_Tag_D    0x5

/*
Write the tag for the cache block at the
specified index from the TagLo and TagHi
Coprocessor 0 registers.

This required encoding may be used by
software to initialize the entire instruction or
data caches by stepping through all valid
indices. Doing so requires that the TagLo and
TagHi registers associated with the cache be
initialized first.
*/
#define Index_Store_Tag_I    0x8
#define Index_Store_Tag_D    0x9

/*
If the cache block contains the specified address, 
set the state of the cache block to invalid.

This required encoding may be used by
software to invalidate a range of addresses
from the instruction cache by stepping through
the address range by the line size of the cache.
*/
#define Hit_Invalidate_I    0x10
#define Hit_Invalidate_D    0x11

/*
Fill the cache from the specified address.
*/
#define Fill_I    0x14

/*
For a write-back cache: If the cache block
contains the specified address and it is valid
and dirty, write the contents back to memory.
After that operation is completed, set the state
of the cache block to invalid. If the block is
valid but not dirty, set the state of the block to
invalid.

For a write-through cache: If the cache block
contains the specified address, set the state of
the cache block to invalid.

This required encoding may be used by
software to invalidate a range of addresses
from the data cache by stepping through the
address range by the line size of the cache.
*/
#define Hit_Writeback_Invalidate_D    0x15

/*
If the cache block contains the specified
address and it is valid and dirty, write the
contents back to memory. After the operation is
completed, leave the state of the line valid, but
clear the dirty state. 

For a write-through cache,
this operation may be treated as a nop.
*/
#define Hit_Writeback_D    0x19

/*
If the cache does not contain the specified
address, fill it from memory, performing a
writeback if required, and set the state to valid
and locked. If the cache already contains the
specified address, set the state to locked. In
set-associative or fully-associative caches, the
way selected on a fill from memory is
implementation dependent.
*/
#define Fetch_Lock_I    0x1c
#define Fetch_Lock_D    0x1d
