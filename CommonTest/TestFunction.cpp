#include "TestFunction.h"
#include "stdafx.h"
#include "Log\LogBase.h"
#include "Reg\RegOperation.h"
#include "Utils\WinUtils.h"
#include "Utils\StringUtils.h"
#include "Utils\Base64Utils.h"
#include "Utils\MD5Utils.h"
#include "Utils\MutexUtils.h"
#include "Service\ServiceUtils.h"

#define MODULE_NAME		L"TestFunction"

//日志操作
void TestLog()
{
	LogBase_init(_T("CommonTest_logconf.ini"));
	L_INFO(_T("test info, total time = %d \r\n"), GetTickCount());
	L_ERROR(_T("test log type： %s\n"), _T("error"));
	L_DEBUG(_T("test debug\n"));
	L_TRACE_ENTER();
	L_TRACE(_T("test trace\n"));
	L_TRACE_LEAVE();
	//LogBase_done();
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
	bRet = CRegOperation::RegWriteString(HKEY_LOCAL_MACHINE, keyPath.c_str(), keyString.c_str(), _T("hello world"));
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

	//WinUtils
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

	bRet =CWinUtils::CreateDirectorys(_T("C:\\2\\3"));

	//StringUtils
	std::string newS = CStringUtils::ToUpper("helWor");

	//Base64Utils
	std::string base64Str = "";
	char inData[32] = "hello world";
	CBase64Utils::Encode(inData, 12, base64Str);
	int len = 0;
	CBase64Utils::Decode(base64Str, len, NULL);
	char *outdata = new char[len];
	CBase64Utils::Decode(base64Str, len, outdata);
	std::string outStr = outdata;
	if (NULL != outdata)
	{
		delete []outdata;
	}

	//MD5Utils
	std::wstring wstrInput = L"helloworod";
	std::wstring wstrMd5;
	bool bReturn = CMD5Utils::GetFileMD5(_T("C:\\1.txt"), wstrMd5);
	CMD5Utils::GetStringMD5(wstrInput, wstrMd5);

	//MutexUtils
	HANDLE hMutex = CMutexUtils::CreateGlobalMutex(TRUE, L"MutexTest");
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		std::wstring msg = CWinUtils::GetErrorMsg(ERROR_ALREADY_EXISTS);
		L_ERROR(_T("CreateGlobalMutex error : %s\n"), msg.c_str());
		return;
	}
	if (NULL != hMutex)
	{
		bRet = CMutexUtils::ReleaseMutex(hMutex);
	}
}

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

//注册成服务s
void TestService()
{
	ServiceBaseInfo Info;

	Info.ConsoloMode = FALSE;
	Info.LogConfigPath = _T("CommonTest_logconf.ini");
	Info.LogModuleName = _T("CommonTestService");
	Info.ServiceName = _T("CommonTestService");
	Info.StartFun = CommonTestServiceStart;
	Info.StopFun = CommonTestServiceStop;

	ServiceMain(&Info);
}
