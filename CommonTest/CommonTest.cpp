// CommonTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Reg\RegOperation.h"


int main()
{
	BOOL bRet = CRegOperation::RegWriteDword(HKEY_LOCAL_MACHINE,
		_T("SOFTWARE\\HQW"), _T("Test"), 3);
	
	return 0;
}

