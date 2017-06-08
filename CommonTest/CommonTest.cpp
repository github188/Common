// CommonTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "TestFunction.h"

#define _TEST_REG_


int main()
{
#ifdef _TEST_REG_
	TestReg();
#endif // _TEST_REG_

	system("pause");
	return 0;
}

