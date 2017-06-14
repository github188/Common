// CommonTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "TestFunction.h"

#include "Service\ServiceUtils.h"

#define _TEST_LOG_
//#define _TEST_REG_
#define _TEST_WIN_UTILS_

BOOL WINAPI CommonTestServiceStart()
{
	//CGlobalController::GlobalInit();
	return TRUE;
}


BOOL WINAPI CommonTestServiceStop()
{
	//CGlobalController::GlobalDone();
	return FALSE;
}

int main()
{

	ServiceBaseInfo Info;

	Info.ConsoloMode = FALSE;
	Info.LogConfigPath = _T("CommonTest_logconf.ini");
	Info.LogModuleName = _T("CommonTestService");
	Info.ServiceName = _T("CommonTestService");
	Info.StartFun = CommonTestServiceStart;
	Info.StopFun = CommonTestServiceStop;

	ServiceMain(&Info);

//#ifdef _TEST_LOG_
//	TestLog();
//#endif // _TEST_LOG_
//
//#ifdef _TEST_LOG_
//	TestReg();
//#endif // _TEST_REG_
//
//#ifdef _TEST_WIN_UTILS_
//	TestWinUtils();
//#endif // _TEST_WIN_UTILS




	//system("pause");
	return 0;
}

