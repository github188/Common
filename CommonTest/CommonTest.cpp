// CommonTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "TestFunction.h"

#define _TEST_LOG_

//#define _TEST_SERVICE_
//#define _TEST_REG_
//#define _TEST_WIN_UTILS_
//#define _TEST_THREAD_


int main()
{

#ifdef _TEST_SERVICE_
	TestService();
#endif // _TEST_SERVICE_

#ifdef _TEST_LOG_
	TestLog();
#endif // _TEST_LOG_

#ifdef _TEST_LOG_
	TestReg();
#endif // _TEST_REG_

#ifdef _TEST_WIN_UTILS_
	TestWinUtils();
#endif // _TEST_WIN_UTILS

#ifdef _TEST_THREAD_
	TestThread();
#endif // _TEST_THREAD_


	//system("pause");
	return 0;
}

