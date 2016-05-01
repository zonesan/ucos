#include  "includes.h"
#include  "typedef.h"
#include  "test_I2C.h"
#include  "tw_iic.h"
#include  "prtcl.h"

extern Flag fail;

void test_I2C(int seed)
{
char *c;
char *c_w;
	*c_w = 'A';
	TW_IICinit();
	TW_IICWriteByteB(MASTER, 2, 0x05, c_w, 1);
	TW_IICReadByteB(MASTER, 2, 0x05, c, 1);
	TW_IICReadByteB(MASTER, 2, 0x05, c, 1);
	TW_IICReadByteB(MASTER, 2, 0x05, c, 1);
	TW_IICReadByteB(MASTER, 2, 0x05, c, 1);
	TW_IICReadByteB(MASTER, 2, 0x05, c, 1);
	TW_IICReadByteB(MASTER, 2, 0x05, c, 1);
	TW_IICReadByteB(MASTER, 2, 0x05, c, 1);
	if (*c != *c_w) {
		printf("I2c access failure, expect %x actual %x \n", *c_w, *c);
	}
	return;
}

