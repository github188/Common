#include "TestFunction.h"
#include "stdafx.h"
#include "Reg\RegOperation.h"
#include "CTLogEx\CTLogEx.h"

#define MODULE_NAME		L"TestFunction"

//��־����
void TestLog()
{
	CTLogEx_init(_T("CommonTest_logconf.ini"));

	L_INFO(_T("test info, total time = %d \r\n"), GetTickCount());
	L_ERROR(_T("test log type�� %s\n"), _T("error"));
	L_DEBUG(_T("test debug\n"));
	L_TRACE_ENTER();
	L_TRACE(_T("test trace\n"));
	L_TRACE_LEAVE();
}

//ע������
void TestReg()
{
	BOOL bRet = FALSE;
	std::wstring keyPath = _T("SOFTWARE\\HQW");
	std::wstring keyDword = _T("TestD	word");
	std::wstring keyString = _T("TestString");
	DWORD dwRet = 0;
	DWORD dwLen = 0;
	TCHAR* szString;


	bRet = CRegOperation::RegWriteDword(HKEY_LOCAL_MACHINE, keyPath.c_str(), keyDword.c_str(), 10);
	bRet = CRegOperation::RegWriteString(HKEY_LOCAL_MACHINE, keyPath.c_str(), keyString.c_str(), _T("huangqiwei"));
	bRet = CRegOperation::RegReadDword(HKEY_LOCAL_MACHINE, keyPath.c_str(), keyDword.c_str(), &dwRet, 99);

	//��ȡ�ַ���
	bRet = CRegOperation::RegReadString(HKEY_LOCAL_MACHINE, keyPath.c_str(), keyString.c_str(), NULL, &dwLen, 
		_T("default value"));
	if (dwLen == 0)
	{
		return;
	}
	szString = (TCHAR*)malloc(dwLen);
	bRet = CRegOperation::RegReadString(HKEY_LOCAL_MACHINE, keyPath.c_str(), keyString.c_str(), szString, &dwLen,
		_T("default value"));
	if (bRet == FALSE)
	{
		free(szString);
		return;
	}
	free(szString);
}
