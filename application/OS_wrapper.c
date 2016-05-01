/* file : OS_wrapper.c
** auther: sun
** date: 2006-7-25
*/

#include "OS_wrapper.h"
#include "includes.h"

void OSSemPend_w(OS_EVENT* pevent, INT16U timeout, INT8U* err)
{
	OSSemPend(pevent, timeout, err);
	if (*err != OS_NO_ERR)
		printf("ucos pend for Semaphore failed.\n");
}

OS_EVENT* OSSemCreate_w(INT16U cnt)
{
	OS_EVENT* pevent = 0;
	if ( (pevent = OSSemCreate(cnt)) == 0) {
		printf("OSSemCreate() failed.\n");
		return 0;
	}
	else 
		return pevent;
}

void OSSemPost_w(OS_EVENT* pevent)
{
	if (OSSemPost(pevent) != OS_NO_ERR)
		printf("OSSemPost() failed.\n");
}

void OSSemDel_w(OS_EVENT* pevent, INT8U opt, INT8U* err)
{
	OS_EVENT* ret = 0;
	ret = OSSemDel(pevent, opt, err);
	if (*err != OS_NO_ERR || ret )
		printf("OSSemDel() failed.\n");
}

OS_EVENT* OSMboxCreate_w(void * msg)
{
	OS_EVENT* ret = OSMboxCreate (msg);
	if (!ret)
		printf("OSMboxCreate() failed.\n");
	return ret;
}

#if OS_MBOX_DEL_EN > 0
void OSMboxDel_w(OS_EVENT* pevent, INT8U opt, INT8U* err)
{
	OS_EVENT* ret = 0;
	ret = OSMboxDel(pevent, opt, err);
	if (*err != OS_NO_ERR || ret )
		printf("OSMboxDel() failed.\n");
}
#endif

void* OSMboxPend_w(OS_EVENT* pevent, INT16U timeout, INT8U* err)
{
	void* ret = 0;
	ret = OSMboxPend(pevent, timeout, err);
	if (!ret || *err != OS_NO_ERR) {
		printf("OSMboxPend() failed.\n");
		return 0;
	}
	else 
		return ret;
}

void OSMboxPost_w(OS_EVENT* pevent, void* msg)
{
	INT8U ret = 0;
	
	if ( (ret = OSMboxPost(pevent, msg)) != OS_NO_ERR)
		printf("OSMboxPost() failed. errno: %d\n", ret);
}

#if 0
void OSTaskCreate_w(void (*task)(void*), void* pdata, OS_STK* ptos, INT8U prio)
{
	int ret = 0;
	ret = OSTaskCreate(task, pdata, ptos, prio);
	if (ret != OS_NO_ERR)
		printf("OSTaskCreate() failed. errno: %d\n", ret);
}
#endif

