#include "TestFunction.h"
#include "stdafx.h"
#include "Reg\RegOperation.h"
#include "CTLogEx\CTLogEx.h"
#include "Log\LogExBase.h"
#include "Utils\WinUtils.h"

#define MODULE_NAME		L"TestFunction"

//日志操作
void TestLog()
{
	//直接调用 CTLogEx
	CTLogEx_init(_T("CommonTest_logconf.ini"));
	L_INFO(_T("test info, total time = %d \r\n"), GetTickCount());
	L_ERROR(_T("test log type： %s\n"), _T("error"));
	L_DEBUG(_T("test debug\n"));
	L_TRACE_ENTER();
	L_TRACE(_T("test trace\n"));
	L_TRACE_LEAVE();
	
	//基于二次封装的 CommonDll
	CLogEx::LogInitW(_T("CommonTest_logconf.ini"));
	LOG_INFO(_T("CommonDll Log info\n"));
	LOG_ERROR(_T("CommonDll Log error\n"));
	LOG_DEBUG(_T("CommonDll Log debug\n"));
	CLogEx::LogDone();
}

//注册表操作
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

	//读取字符串
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

//Windows常用操作
void TestWinUtils()
{
	BOOL bRet = FALSE;
	DWORD dwRet = 0;

	int numOfNic = 0;
	std::vector<BYTE> mac;
	bRet = CWinUtils::GetMacAddr(&numOfNic, mac);

	char hostName[32] = { 0 };
	bRet = CWinUtils::GetHostname(hostName);

	bRet = CWinUtils::IsAdminMode();

	bRet = CWinUtils::IsFullpath(_T("1.txt"));
	bRet = CWinUtils::IsFullpath(_T("C:\\1.txt"));

	std::wstring userName = _T("");
	dwRet = CWinUtils::GetSessionID();
	if (dwRet > 0)
	{
		bRet = CWinUtils::GetSessionUserName(dwRet, userName);
	}

	std::string ansi = CWinUtils::Wchar2Ansi(userName.c_str());
	std::wstring wchar = CWinUtils::Ansi2WideString(ansi.c_str());

	std::wstring processName = CWinUtils::GetCurrentProcessName();

	std::wstring errorMsg = CWinUtils::GetErrorMsg(GetLastError());
	std::wstring errorMsg2 = CWinUtils::WIN32_ERROR_STRING;

	int k = 0;
}
