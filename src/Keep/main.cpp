#include <stdio.h>
#include <tchar.h>
#include <cassert>
#include <windows.h>
#include "kLog.h"
#include "timer.h"
#include "control.h"
#include "config.h"
#include "util.h"

int WINAPI WinMain(
				   HINSTANCE hInstance,       //程序当前实例的句柄，以后随时可以用GetModuleHandle(0)来获得  
				   HINSTANCE hPrevInstance,   //这个参数在Win32环境下总是0，已经废弃不用了  
				   char * lpCmdLine,          //指向以/0结尾的命令行，不包括EXE本身的文件名，可以用GetCommandLine()来获取完整的命令行  
				   int nCmdShow               //指明应该以什么方式显示主窗口  
				   )
{
	
	int hasRunned = tryFirstRunModule();
	if(hasRunned)
	{
		LOG_INFO(_T("app start failed, it has been running."));
		return -1;
	}

	LOG_INFO(_T("app start successfuly."));
	const TCHAR *modulePath = getCurrentModulePathWithSlash();
	TCHAR configpath[256] = {0};
	sprintf_s(configpath,255,"%s%s",modulePath,"conf.ini");
	if(!fileExists(configpath))
	{
		TCHAR msg [320] = {0};
		sprintf_s(msg,320,"conf '%s' dones't exists.will use default conf! [targetScreenTimeoutSecs=1800, currentScreenTimeoutSecs=1200]",configpath);
		LOG_INFO(msg);
		LOG_ERROR("file not exists ",GetLastError());
	}

	Config config(configpath);
	UINT target = config.getAsUint(_T("keep"),_T("targetScreenTimeoutSecs"),30 * 60);
	UINT current = config.getAsUint(_T("keep"),_T("currentScreenTimeoutSecs"),10 * 60);
	
	TCHAR msg [1024] = {0};
	sprintf_s(msg,1023,"targetScreenTimeoutSecs: %d, currentScreenTimeoutSecs: %d", target, current);
	LOG_INFO(msg);

	Control contrl(target,current);
	timeRun(timerProc,&contrl, current >> 2);
	//timeRun(testScreenLocked,&contrl, 2);
	return 0;
}