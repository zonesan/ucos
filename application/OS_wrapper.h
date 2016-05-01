/* file: OS_wrapper.h
** auther: sun
** date: 2007-7-25
*/

#ifndef OS_WRAPPER_H
#define OS_WRAPPER_H


#include "includes.h"

void OSSemPend_w(OS_EVENT* pevent, INT16U timeout, INT8U* err);
OS_EVENT* OSSemCreate_w(INT16U cnt);
void OSSemPost_w(OS_EVENT* pevent);
void OSSemDel_w(OS_EVENT* pevent, INT8U opt, INT8U* err);
OS_EVENT* OSMboxCreate_w(void * msg);
void OSMboxDel_w(OS_EVENT* pevent, INT8U opt, INT8U* err);
void OSMboxPost_w(OS_EVENT* pevent, void* msg);
void* OSMboxPend_w(OS_EVENT* pevent, INT16U timeout, INT8U* err);
void OSTaskCreate_w(void (*task)(void*), void* pdata, OS_STK* ptos, INT8U prio);

#endif

