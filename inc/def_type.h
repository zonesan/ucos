#ifndef __DEF_TYPE_H
#define __DEF_TYPE_H
/*
* The function name should reflect the level and module. 
* eg. HwSecInitialization () means Hardware layer, security module code. 
* The variable name need to have a name reflects the type as well. If it is global variable, it needs to start with g and have similar naming convention as functiond. 
* eg. LONG glwSecErrorCount  means global data, 32 bits unsign long Security module variable
*/

typedef unsigned int BOOL;        /* Boolean, Best type to hold 0 or 1 */
typedef int LONG;
typedef unsigned int ULONG;
typedef short SHORT;
typedef unsigned short WORD;
typedef char CHAR;
typedef unsigned char BYTE;
typedef void VOID;

typedef unsigned char *	PBYTE;
typedef char * PCHAR;
typedef unsigned short * PWORD;
typedef short * PSHORT;
typedef int *	PLONG;
typedef unsigned int *PULONG;
typedef void *	PVOID;

#endif
