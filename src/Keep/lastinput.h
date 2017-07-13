#ifndef LASTINPUT_H
#define LASTINPUT_H


#include <windows.h>
#include "kLog.h"

class LastInputTickcountMs
{
public:
	LastInputTickcountMs()
	{}
	
	DWORD getValatile()
	{
		LASTINPUTINFO lii;
		TCHAR buffer[1024] = {0};
		lii.cbSize = sizeof(LASTINPUTINFO);
		//ȡ������һ�η��� input �¼�ʱ��ϵͳ�����е�ʱ���. lii.dwTime=GetTickCount

		GetLastInputInfo(&lii);
		DWORD ret = lii.dwTime;
	//	sprintf_s(buffer,1023,"last input event time : %d",ret);
	//	LOG_INFO(buffer);
		return ret;
	}
};

LastInputTickcountMs lastInputService;

#endif