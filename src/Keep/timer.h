#ifndef TIMER_H
#define TIMER_H

#include <windows.h>
#include "kLog.h"

class KLog;
extern KLog klog;

typedef void (* TIMER_CALL_BACK_PROC)(void *);

void timeRun(TIMER_CALL_BACK_PROC proc, void *lparam, int intervalSecs)
{
	HANDLE keepTimer = CreateWaitableTimerEx(NULL,_T("keep_timer"),CREATE_WAITABLE_TIMER_MANUAL_RESET,EVENT_ALL_ACCESS);
	if(NULL == keepTimer)
	{
		LOG_ERROR("create timer error. ", GetLastError());
		return ;
	}
	if(ERROR_ALREADY_EXISTS == GetLastError())
	{
		LOG_INFO("timer already exists.");
	}
	LARGE_INTEGER liDueTime;//�Ӻ�ʱ��ʼ���ź�
	liDueTime.QuadPart = ((LONGLONG)-10000000) * intervalSecs;	//���ʱ�䣬��ʲôʱ��֮�󴥷�. 100 ����Ϊ��
	//�ص�����Ϊ�գ�����ʹ�� waitForSingleObject ȥ�ȴ�����ص������ᱻ����. ���һ������ָʾ���Ƿ�ϵͳ�ӵ�Դ����ģʽ�лָ�����

	while (true)
	{
		if(0 == SetWaitableTimer(keepTimer,&liDueTime,0,NULL,NULL,FALSE))
		{
			LOG_ERROR("set timer error. ", GetLastError());
			return ;
		}

		if(WAIT_OBJECT_0 != WaitForSingleObject(keepTimer,INFINITE))
		{
			LOG_ERROR("wait timer error. ", GetLastError());
			return ;
		}
		try
		{
			proc(lparam);
		}
		catch (... )
		{
			LOG_ERROR("timer proc exception.", 0);
		}
		
	}
}

#endif