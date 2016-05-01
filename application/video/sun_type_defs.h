/* file : sun_type_defs.h
** create by sun ,  modify by hjm 
** date : 2006 - 9 - 20
*/

#ifndef SUN_TYPE_DEFS_H
#define SUN_TYPE_DEFS_H

#ifndef NULL
#define NULL  ((void *)0)
#endif

typedef unsigned char uint8_t;
typedef signed char int8_t;
typedef unsigned short uint16_t;
typedef short int16_t;
typedef unsigned int uint32_t;
typedef int int32_t;
typedef uint32_t size_t;
typedef int32_t ssize_t;
typedef unsigned long long int uint64_t;
typedef long long int int64_t;

typedef enum {
    EQCIF,
    SVGA,
    QCIF,
    QVGA,
    QQVGA,
    CIF,
    VGA,
    SXGA,
    NTSC_D1,
    PAL_D1,
    NTSC_CIF,
    NTSC_QCIF,
    SUB_QCIF
} ImageMode;
#define ImageModeN 6

typedef struct {
unsigned int addr;
unsigned int len;
} memblock_t;

#define ASSERT(statement) if (!(statement)) printf("ASSERT %s failed on %s %d.\n", \
										#statement, __FILE__, __LINE__);

#define VGAWIDTH 640
#define VGAHEIGHT 480
#define QVGAWIDTH 320
#define QVGAHEIGHT 240
#define CIFWIDTH 352
#define CIFHEIGHT 288
#define QCIFWIDTH 176
#define QCIFHEIGHT 144

#define SXGAWIDTH 1280
#define SXGAHEIGHT 1024
#endif

