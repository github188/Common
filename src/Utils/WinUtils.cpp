#include "Utils\WinUtils.h"
#include "Utils\StringUtils.h"
#include "Log\LogBase.h"
#include <Iphlpapi.h>
#include <WtsApi32.h>
#include "CTUtils.h"
#include "Shlwapi.h"
#include <wbemidl.h>
#include "VersionHelpers.h"

#define  MODULE_NAME  L"WinUtils"

BOOL CWinUtils::GetMacList(int *numOfNic, std::vector<BYTE> &mac)
{
	PIP_ADAPTER_INFO pAdapterInfo = NULL;
	ULONG ulSizeAdapterInfo = 0;
	DWORD dwReturnvalueGetAdapterInfo = NO_ERROR;
	PIP_ADAPTER_INFO pOriginalPointer = NULL;
	DWORD r = FALSE;
	int index;

	*numOfNic = 0;
	mac.clear();
	dwReturnvalueGetAdapterInfo = GetAdaptersInfo(pAdapterInfo, &ulSizeAdapterInfo);
	if (dwReturnvalueGetAdapterInfo == ERROR_BUFFER_OVERFLOW)
	{
		pAdapterInfo = (PIP_ADAPTER_INFO)malloc(ulSizeAdapterInfo);
		if (!pAdapterInfo)
		{
			L_FATAL(_T("malloc fail\n"));
			return FALSE;
		}
		pOriginalPointer = pAdapterInfo;
		dwReturnvalueGetAdapterInfo = GetAdaptersInfo(pAdapterInfo, &ulSizeAdapterInfo);
	}

	if (dwReturnvalueGetAdapterInfo != NO_ERROR)
	{
		L_WARN(_T("GetAdaptersInfo FAIL:0x%x\n"), GetLastError());
		goto EXIT;
	}

	if (ulSizeAdapterInfo == 0)
	{
		L_WARN(_T("No found netcard in system\n"));
		goto EXIT;
	}
	r = TRUE;

	index = 0;
	while (pAdapterInfo != NULL)
	{
		/* MAC地址长度不是6字节??? */

		if (pAdapterInfo->Type != MIB_IF_TYPE_ETHERNET || pAdapterInfo->AddressLength != 6)
		{
			pAdapterInfo = pAdapterInfo->Next;
		}
		else
		{
			BYTE mac00[6] = { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };
			BYTE macff[6] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };

			/*
			* 忽略全0及全ff的mac地址
			*/
			if (!(memcmp(pAdapterInfo->Address, mac00, 6) == 0)
				&& !(memcmp(pAdapterInfo->Address, macff, 6) == 0))
			{
				mac.insert(mac.end(), pAdapterInfo->Address, pAdapterInfo->Address + 6);
				(*numOfNic)++;
			}
			pAdapterInfo = pAdapterInfo->Next;
		}
	}

EXIT:
	if (pOriginalPointer)
	{
		free(pOriginalPointer);
	}

	return r;
}

BOOL CWinUtils::GetMacAddress(WCHAR* mac)
{
	int NicNum = 0;
	std::vector<BYTE> macList;

	if (!GetMacList(&NicNum, macList))
	{
		L_ERROR(_T("GetMacList fail %d\r\n"), GetLastError());
		return FALSE;
	}

	if (NicNum == 0 && macList.size() >= 6)
	{
		L_ERROR(_T("GetMacList NicNum = %d macList.size = %d\r\n"), NicNum, macList.size());
		return FALSE;
	}

	swprintf_s(mac, 256, _T("%02x:%02x:%02x:%02x:%02x:%02x"), macList[0], macList[1], macList[2],
		macList[3], macList[4], macList[5]);

	return TRUE;
}

void CWinUtils::GetIPList(std::vector<DWORD> &ipList)
{
	//初始化Win Socket2
	WORD            wVersionRequested;
	WSADATA         wsaData;
	wVersionRequested = MAKEWORD(2, 2);
	WSAStartup(wVersionRequested, &wsaData);
	
	char hostName[256];
	ipList.clear();
	if (gethostname(hostName, 256) == 0)
	{
		struct hostent *host;
		int i;

		host = gethostbyname(hostName);
		for (i = 0; host != NULL && host->h_addr_list[i] != NULL; i++)
		{
			struct in_addr *in = (struct in_addr *)host->h_addr_list[i];

			ipList.push_back(htonl(in->s_addr));
		}
	}
}

BOOL CWinUtils::GetIPAddresss(WCHAR* IP)
{
	std::vector<DWORD> ipList;
	std::vector<DWORD>::iterator Itor;
	DWORD ip;
	WCHAR tmp[256];
	BOOL found = FALSE;

	GetIPList(ipList);
	if (ipList.empty())
	{
		L_ERROR(_T("GetIPList fail %d\r\n"), GetLastError());
		return FALSE;
	}

	for (Itor = ipList.begin(); Itor != ipList.end(); Itor++)
	{
		ip = *Itor;
		swprintf_s(tmp, 256, _T("%d.%d.%d.%d"),
			(ip >> 24) & 0xFF,
			(ip >> 16) & 0xFF,
			(ip >> 8) & 0xFF,
			(ip >> 0) & 0xFF);

		//去掉回环地址
		if (((ip >> 24) & 0xFF) == 127)
		{
			L_ERROR(_T("GetIPList Invalid IP %S\r\n"), tmp);
			continue;
		}
		else
		{
			found = TRUE;
			break;
		}
	}

	if (found)
	{
		//strncpy(IP, tmp, 256);
		StrCpyW(IP, tmp);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL CWinUtils::GetHostname(char name[MAX_SIZE_OF_HOSTNMAE + 1])
{
	char computerName[MAX_COMPUTERNAME_LENGTH + 1];
	DWORD size = sizeof(computerName);

	memset(name, 0, (MAX_SIZE_OF_HOSTNMAE + 1));
	if (GetComputerNameA(computerName, &size) == FALSE)
	{
		//L_ERROR(_T("GetComputerName fail: %d\n"), GetLastError());
		return FALSE;
	}
	strncpy(name, computerName, MAX_SIZE_OF_HOSTNMAE);
	return TRUE;
}

BOOL CWinUtils::IsAdminMode()
{
	BOOL fIsRunAsAdmin = FALSE;
	PSID pAdministratorsGroup = NULL;

	// Allocate and initialize a SID of the administrators group.
	SID_IDENTIFIER_AUTHORITY NtAuthority = SECURITY_NT_AUTHORITY;
	if (!AllocateAndInitializeSid(
		&NtAuthority,
		2,
		SECURITY_BUILTIN_DOMAIN_RID,
		DOMAIN_ALIAS_RID_ADMINS,
		0, 0, 0, 0, 0, 0,
		&pAdministratorsGroup))
	{
		goto Cleanup;
	}

	// Determine whether the SID of administrators group is enabled in 
	// the primary access token of the process.
	if (!CheckTokenMembership(NULL, pAdministratorsGroup, &fIsRunAsAdmin))
	{
		goto Cleanup;
	}

Cleanup:
	// Centralized cleanup for all allocated resources.
	if (pAdministratorsGroup)
	{
		FreeSid(pAdministratorsGroup);
		pAdministratorsGroup = NULL;
	}

	return fIsRunAsAdmin;
}

BOOL CWinUtils::IsFullpath(const TCHAR *filename)
{
	TCHAR prefix[3];

	if (filename == NULL)
	{
		return FALSE;
	}
	/*
	* XXX: 以"x:"开始的文件名认为是全路径
	*      暂不处理网络磁盘文件名，如\\test\test.ini
	*/
	_tcscpy(prefix, _T("a:"));
	for (int i = 0; i < 26; i++)
	{
		if (_tcsnicmp(prefix, filename, 2) == 0)
		{
			return TRUE;
		}
		prefix[0]++;
	}
	return FALSE;
}

DWORD CWinUtils::GetSessionID()
{
	DWORD  sessionID = (DWORD)-1;
	LPTSTR pSessionInfo = NULL;
	DWORD  len = 0;

	if (!WTSQuerySessionInformation(WTS_CURRENT_SERVER_HANDLE, WTS_CURRENT_SESSION,
		WTSSessionId, (LPTSTR*)&pSessionInfo, &len))
	{
		//L_WARN(_T("WTSQuerySessionInformation WTSSessionId fail: %d\n"), GetLastError());
		return (DWORD)-1;
	}
	sessionID = *((DWORD *)pSessionInfo);
	WTSFreeMemory(pSessionInfo);
	return sessionID;
}

BOOL CWinUtils::GetSessionUserName(DWORD sessionID, tstring &username)
{
	TCHAR *usernameStr = NULL;
	DWORD len = 0;
	BOOL  ret = FALSE;

	if (WTSQuerySessionInformation(WTS_CURRENT_SERVER_HANDLE,
		sessionID,
		WTSUserName,
		(LPTSTR *)&usernameStr,
		&len))
	{
		if (_tcslen(usernameStr))
		{
			username = usernameStr;
			ret = TRUE;
		}
		WTSFreeMemory(usernameStr);
	}

	return ret;
}

std::string CWinUtils::Wchar2Ansi(const WCHAR *input)
{
	int length;
	char *out;
	std::string result = "";

	length = WideCharToMultiByte(CP_ACP, 0, input, -1, NULL, 0, NULL, NULL);
	if (length <= 0)
	{
		return result;
	}

	out = (char *)calloc(length, 1);
	if (out == NULL)
	{
		return result;
	}
	WideCharToMultiByte(CP_ACP, 0, input, -1, out, length, NULL, NULL);
	out[length - 1] = 0;

	result = out;
	free(out);

	return result;
}

std::wstring CWinUtils::Ansi2WideString(const char *input)
{
	int          length;
	PWCHAR       out;
	std::wstring result = L"";

	length = MultiByteToWideChar(CP_ACP, 0, input, -1, NULL, NULL);
	if (length <= 0)
	{
		return result;
	}

	out = (WCHAR *)calloc(length, sizeof(WCHAR));
	if (out == NULL)
	{
		return result;
	}

	MultiByteToWideChar(CP_ACP, 0, input, -1, out, length);

	result.assign(out);
	free(out);

	return result;
}

tstring CWinUtils::GetCurrentProcessName()
{
	TCHAR   filePath[MAX_PATH + 1];
	TCHAR   processName[MAX_PATH + 1];
	int     length, i;

	GetModuleFileName(NULL, filePath, sizeof(filePath) / sizeof(TCHAR));
	memset(processName, 0, sizeof(processName));
	length = _tcslen(filePath);
	if (length > 0)
	{
		for (i = length - 1; i >= 0; i--)
		{
			if (filePath[i] == '\\' || filePath[i] == '/')
			{
				_tcscpy(processName, filePath + i + 1);
				break;
			}
		}
		if (i < 0)
		{
			_tcscpy(processName, filePath);
		}
	}

	return processName;
}

void CWinUtils::GetCurrentProcessPath(std::wstring& path)
{
	wchar_t wsPath[MAX_PATH] = { 0 };
	if (GetModuleFileName(NULL, wsPath, MAX_PATH))
	{
		path = wsPath;
		path = path.substr(0, path.find_last_of(L"\\") + 1);
	}
}

tstring CWinUtils::GetErrorMsg(DWORD errorCode)
{
	HLOCAL hlocal = NULL;
	TCHAR buf[32];
	tstring errorMsg;

	_sntprintf(buf, sizeof(buf) / sizeof(TCHAR), _T("(%d) - "), errorCode);
	errorMsg = buf;

	BOOL fOk = FormatMessage(
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,
		NULL, errorCode, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US),
		(PTSTR)&hlocal, 0, NULL);

	if (!fOk)
	{
		//Try net ...
		HMODULE hDll = LoadLibraryEx(TEXT("netmsg.dll"), NULL,
			DONT_RESOLVE_DLL_REFERENCES);

		if (hDll != NULL)
		{
			FormatMessage(
				FORMAT_MESSAGE_FROM_HMODULE | FORMAT_MESSAGE_FROM_SYSTEM,
				hDll, errorCode, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US),
				(PTSTR)&hlocal, 0, NULL);
			FreeLibrary(hDll);
		}
	}

	if (hlocal != NULL)
	{
		errorMsg += (PTSTR)hlocal;
		LocalFree(hlocal);
	}
	else
	{
		TCHAR *unkownMsg = _T("Unkown error");
		errorMsg += unkownMsg;
	}

	return errorMsg;
}

//小于4G
int CWinUtils::ReadFileToBuf(const std::wstring &filepath, void *&buf, size_t &size)
{
	int ret = -1;
	DWORD readsize = 0;
	HANDLE hfile = INVALID_HANDLE_VALUE;

	do
	{
		hfile = CreateFile(filepath.c_str(), GENERIC_ALL, FILE_SHARE_READ | FILE_SHARE_DELETE | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

		if (INVALID_HANDLE_VALUE == hfile)
		{
			break;
		}

		size = GetFileSize(hfile, NULL);

		if (size>30 * 1024 * 1024)
		{
			break;
		}


		buf = new BYTE[size];

		if (NULL == buf)
		{
			break;
		}

		if (!ReadFile(hfile, buf, size, &readsize, NULL))
		{
			break;
		}
		size = readsize;

		ret = 0;

	} while (0);

	if (hfile != INVALID_HANDLE_VALUE)
	{
		CloseHandle(hfile);
	}
	return GetLastError();

}

int CWinUtils::WriteBufToFile(const std::wstring &filepath, const void *buf, size_t size)
{
	int ret = -1;
	DWORD writesize = 0;
	HANDLE hfile = INVALID_HANDLE_VALUE;
	do
	{
		hfile = CreateFile(filepath.c_str(), GENERIC_ALL, FILE_SHARE_READ | FILE_SHARE_DELETE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

		if (INVALID_HANDLE_VALUE == hfile)
		{
			break;
		}

		if (!WriteFile(hfile, buf, size, &writesize, NULL))
		{
			break;
		}

		ret = 0;

	} while (0);

	if (hfile != INVALID_HANDLE_VALUE)
	{
		CloseHandle(hfile);
	}

	return ret;
}

int CWinUtils::GetDiskFreeSpaceByPath(const std::wstring& path, ULONGLONG &filesize)
{
	std::wstring tmpPath;
	ULARGE_INTEGER totalAvalie = { 0 };
	ULARGE_INTEGER totalSize = { 0 };
	ULARGE_INTEGER totalFree = { 0 };

	if (path.empty())
	{
		GetCurrentProcessPath(tmpPath);
	}
	else
	{
		tmpPath = path;
	}

	GetDiskFreeSpaceEx(tmpPath.c_str(), &totalAvalie, &totalSize, &totalFree);
	filesize = totalAvalie.QuadPart;
	return 0;

}

int CWinUtils::GetDiskUsedSpaceByPath(const std::wstring& path, ULONGLONG &filesize)
{
	std::wstring tmpPath;
	ULARGE_INTEGER totalAvalie = { 0 };
	ULARGE_INTEGER totalSize = { 0 };
	ULARGE_INTEGER totalFree = { 0 };
	if (path.empty())
	{
		GetCurrentProcessPath(tmpPath);
	}
	else
	{
		tmpPath = path;
	}

	GetDiskFreeSpaceEx(tmpPath.c_str(), &totalAvalie, &totalSize, &totalFree);
	filesize = totalSize.QuadPart - totalAvalie.QuadPart;

	return 0;
}

int CWinUtils::GetDirectorysSize(const std::wstring &path, ULONGLONG &size)
{
	WIN32_FIND_DATA findfiledata = { 0 };
	HANDLE hFind;
	std::wstring findfilepath = path;

	int pos = findfilepath.find_last_of(L"\\");
	if (pos == std::wstring::npos || pos != findfilepath.length() - 1)
	{
		findfilepath += L"\\";
	}

	hFind = FindFirstFile((findfilepath + L"*").c_str(), &findfiledata);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		return -1;
	}

	do
	{
		if (wcscmp(findfiledata.cFileName, L".") == 0 || wcscmp(findfiledata.cFileName, L"..") == 0)
		{
			continue;
		}

		if (findfiledata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{

			GetDirectorysSize(findfilepath + findfiledata.cFileName, size);
			continue;
		}
		ULONGLONG filesize = (((ULONGLONG)findfiledata.nFileSizeHigh) << 32) + findfiledata.nFileSizeLow;
		size += filesize;
		memset(&findfiledata, 0, sizeof(findfiledata));

	} while (FindNextFile(hFind, &findfiledata));

	FindClose(hFind);

	return 0;

}

int CWinUtils::GetLocalFileSize(const std::wstring &path, ULONGLONG &size)
{
	WIN32_FIND_DATA findfiledata = { 0 };
	HANDLE hFind;

	hFind = FindFirstFile(path.c_str(), &findfiledata);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		return -1;
	}

	size = (((ULONGLONG)findfiledata.nFileSizeHigh) << 32) + findfiledata.nFileSizeLow;

	FindClose(hFind);
	return 0;
}

int CWinUtils::FormatFileSize(ULONGLONG &size, ULONGLONG &tailSize, std::wstring &unit)
{
	int unitType = 0;//0:B  1:KB  2:MB  3:GB  4:TB
	ULONGLONG tmpSize = 0;

	if (size >= 1024)
	{
		// 0:B
		tmpSize = size / 1024;
		unitType = 1;
		tailSize = (size - tmpSize * 1024);
		size = tmpSize;
		if (size >= 1024)
		{
			// 1:KB
			tmpSize = size / 1024;
			unitType = 2;
			tailSize = (size - tmpSize * 1024);
			size = tmpSize;
			if (size >= 1024)
			{
				// 3:GB
				tmpSize = size / 1024;
				unitType = 3;
				tailSize = (size - tmpSize * 1024);
				size = tmpSize;
				if (size >= 1024)
				{
					// 4:TB
					tmpSize = size / 1024;
					unitType = 4;
					tailSize = (size - tmpSize * 1024);
					size = tmpSize;
				}
			}
		}
	}

	tailSize = tailSize * 1000 / 1024 / 10;

	switch (unitType)
	{
	case 0:
		unit = L"B";
		break;
	case 1:
		unit = L"KB";
		break;
	case 2:
		unit = L"MB";
		break;
	case 3:
		unit = L"GB";
		break;
	case 4:
		unit = L"TB";
		break;
	default:
		unit = L"B";
		break;
	}

	return 0;
}

BOOL CWinUtils::AdjustProcessPrivileges(const WCHAR* privilegesname)
{
	HANDLE   hToken = NULL;
	TOKEN_PRIVILEGES   tkp;
	BOOL ret = FALSE;

	do
	{
		if (!OpenProcessToken(GetCurrentProcess(),
			TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
		{
			L_ERROR(_T("OpenProcessToken failed %d\n"), GetLastError());
			break;
		}

		if (!LookupPrivilegeValueW(NULL, privilegesname, &tkp.Privileges[0].Luid))
		{
			L_ERROR(_T("LookupPrivilegeValueW failed %d\n"), GetLastError());
			break;
		}

		tkp.PrivilegeCount = 1;
		tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
		if (!AdjustTokenPrivileges(hToken, FALSE, &tkp, sizeof(tkp), NULL, NULL))
		{
			L_ERROR(_T("AdjustTokenPrivileges failed %d\n"), GetLastError());
			break;
		}

		DWORD dwError = GetLastError();
		if (ERROR_SUCCESS != dwError)
		{
			L_ERROR(_T("AdjustTokenPrivileges failed %d %s\n"), dwError, GetErrorMsg(dwError));
			break;
		}

		ret = TRUE;
	} while (0);

	if (hToken)
	{
		CloseHandle(hToken);
	}

	return ret;
}

bool	CWinUtils::CreateDirectorys(const std::wstring &path)
{
	std::wstring createDirString;
	std::vector<std::wstring> subDirs;

	if (FileExists(path.c_str()))
	{
		return false;
	}
	createDirString = path;
	CStringUtils::ReplaceAllString(createDirString, L"/", L"\\");
	CStringUtils::SplitString(createDirString, L"\\", subDirs);
	createDirString = subDirs[0];
	for (std::vector<std::wstring> ::iterator it = subDirs.begin() + 1; it != subDirs.end(); it++)
	{
		createDirString.append(1, L'\\').append(it->c_str());
		if (PathFileExists(createDirString.c_str()) && PathIsDirectory(createDirString.c_str()))
		{
			continue;
		}

		if (!CreateDirectory(createDirString.c_str(), NULL))
		{
			return false;
		}
	}

	return true;
}

bool	CWinUtils::RemoveDirectorys(const std::wstring &dir)
{
	WIN32_FIND_DATA findfiledata = { 0 };
	HANDLE hFind;
	std::wstring findfilepath = dir;

	int pos = findfilepath.find_last_of(L"\\");
	if (pos == std::wstring::npos || pos != findfilepath.length() - 1)
	{
		findfilepath += L"\\";
	}

	hFind = FindFirstFile((findfilepath + L"*").c_str(), &findfiledata);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	do
	{
		if (wcscmp(findfiledata.cFileName, L".") == 0 || wcscmp(findfiledata.cFileName, L"..") == 0)
		{
			continue;
		}

		if (findfiledata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{

			RemoveDirectorys(findfilepath + findfiledata.cFileName);
			continue;
		}

		DeleteFile((findfilepath + findfiledata.cFileName).c_str());
		memset(&findfiledata, 0, sizeof(findfiledata));

	} while (FindNextFile(hFind, &findfiledata));

	FindClose(hFind);
	return (bool)RemoveDirectory(dir.c_str());
}

int CWinUtils::EnumDirectoryFile(const std::wstring &dir, bool enumSubDir, std::list<std::wstring> &fileList)
{
	WIN32_FIND_DATA findfiledata = { 0 };
	HANDLE hFind;
	std::wstring findfilepath = dir;

	int pos = findfilepath.find_last_of(L"\\");
	if (pos == std::wstring::npos || pos != findfilepath.length() - 1)
	{
		findfilepath += L"\\";
	}

	hFind = FindFirstFile((findfilepath + L"*").c_str(), &findfiledata);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		return -1;
	}

	do
	{
		if (wcscmp(findfiledata.cFileName, L".") == 0 || wcscmp(findfiledata.cFileName, L"..") == 0)
		{
			continue;
		}

		if (findfiledata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if (enumSubDir)
			{
				EnumDirectoryFile(findfilepath + findfiledata.cFileName, enumSubDir, fileList);
			}
			continue;
		}

		fileList.push_back(findfilepath + findfiledata.cFileName);

		memset(&findfiledata, 0, sizeof(findfiledata));

	} while (FindNextFile(hFind, &findfiledata));

	FindClose(hFind);

	return 0;
}

bool CWinUtils::FileExists(const std::wstring &filepath)
{

	return (bool)PathFileExists(filepath.c_str());
}

BOOL CWinUtils::GetSystemUUID(CHAR* uuid)
{
	HRESULT Result;
	IWbemLocator  *WbemLocator = NULL;
	IWbemServices *WbemServices = NULL;
	IEnumWbemClassObject * EnumObject = NULL;
	BOOL bRet = FALSE;
	ULONG Count = 1;
	ULONG Returned = 1;

	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	Result = CoInitializeSecurity(
		NULL,                        // Security descriptor    
		-1,                          // COM negotiates authentication service
		NULL,                        // Authentication services
		NULL,                        // Reserved
		RPC_C_AUTHN_LEVEL_DEFAULT,   // Default authentication level for proxies
		RPC_C_IMP_LEVEL_IMPERSONATE, // Default Impersonation level for proxies
		NULL,                        // Authentication info
		EOAC_NONE,                   // Additional capabilities of the client or server
		NULL);                       // Reserved
	if (FAILED(Result) && Result != RPC_E_TOO_LATE)
	{
		L_ERROR(_T("Failed to initialize security. Error code = 0x%x\r\n"), Result);
		CoUninitialize();
		return FALSE;                  // Program has failed.
	}

	Result = CoCreateInstance(CLSID_WbemLocator, 0,
		CLSCTX_INPROC_SERVER, IID_IWbemLocator, (LPVOID *)&WbemLocator);
	if (FAILED(Result))
	{
		L_ERROR(_T("Failed to create IWbemLocator object. Err code = 0x%x\r\n"), Result);
		CoUninitialize();
		return FALSE;     // Program has failed.
	}

	// Connect to the root\default namespace with the current user.
	Result = WbemLocator->ConnectServer(
		BSTR(L"ROOT\\CIMV2"),  //namespace
		NULL,                  // User name 
		NULL,                  // User password
		0,                     // Locale 
		NULL,                  // Security flags
		0,                     // Authority 
		0,                     // Context object 
		&WbemServices);        // IWbemServices proxy
	if (FAILED(Result))
	{
		L_ERROR(_T("Could not connect. Error code = 0x%x\r\n"), Result);
		WbemLocator->Release();
		CoUninitialize();
		return FALSE;      // Program has failed.
	}

	WCHAR strQuery[1024] = { '\0' };
	wcscpy(strQuery, L"SELECT UUID FROM Win32_ComputerSystemProduct");
	BSTR strQL = (L"WQL");
	Result = WbemServices->ExecQuery(strQL, strQuery, WBEM_FLAG_RETURN_IMMEDIATELY, NULL, &EnumObject);
	if (FAILED(Result))
	{
		L_ERROR(_T("ExecQuery Fail %x\r\n"), Result);
		bRet = FALSE;
		goto end;
	}

	IWbemClassObject * ClassObject = NULL;
	Result = EnumObject->Reset();
	if (FAILED(Result))
	{
		L_ERROR(_T("Could not Enumerate %x\r\n"));
		bRet = FALSE;
		goto end;
	}

	Result = EnumObject->Next(WBEM_INFINITE, Count, &ClassObject, &Returned);
	if (FAILED(Result))
	{
		L_ERROR(_T("Could not Enumerate Next %x\r\n"));
		bRet = FALSE;
		goto end;
	}

	VARIANT v;
	BSTR ClassPropString = SysAllocString(L"UUID");
	Result = ClassObject->Get(ClassPropString, 0, &v, 0, 0);
	SysFreeString(ClassPropString);
	if (FAILED(Result))
	{
		L_ERROR(_T("Could not Get UUID %x\r\n"), Result);
		bRet = FALSE;
		goto end;
	}

	sprintf(uuid, "%S", v.bstrVal);
	bRet = TRUE;

	VariantClear(&v);

end:
	WbemServices->Release();
	WbemLocator->Release();
	CoUninitialize();

	return bRet;
}

BOOL CWinUtils::RunApplicationInSession(DWORD sessionId, TCHAR* cmdline, BOOL wait)
{
	HANDLE UserToken = NULL;
	PROCESS_INFORMATION processInfo;
	SECURITY_ATTRIBUTES sa;
	STARTUPINFO startUpInfo;

	ZeroMemory(&processInfo, sizeof(PROCESS_INFORMATION));
	ZeroMemory(&startUpInfo, sizeof(STARTUPINFO));
	ZeroMemory(&sa, sizeof(SECURITY_ATTRIBUTES));
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	startUpInfo.cb = sizeof(STARTUPINFO);

	if (!WTSQueryUserToken(sessionId, &UserToken))
	{
		L_ERROR(_T("WTSQueryUserToken Fail %d\n"), GetLastError());
		return FALSE;
	}

	BOOL result = CreateProcessAsUser(UserToken,
		NULL,
		cmdline,
		&sa,
		&sa,
		FALSE,
		0,
		NULL,
		NULL,
		&startUpInfo,
		&processInfo);
	if (!result)
	{
		L_ERROR(_T("CreateProcessAsUser Fail %d\n"), GetLastError());
		CloseHandle(UserToken);
		return FALSE;
	}

	if (wait)
	{
		WaitForSingleObject(processInfo.hProcess, INFINITE);
	}

	CloseHandle(startUpInfo.hStdInput);
	CloseHandle(startUpInfo.hStdError);
	CloseHandle(startUpInfo.hStdOutput);
	CloseHandle(processInfo.hProcess);
	CloseHandle(processInfo.hThread);
	CloseHandle(UserToken);

	return TRUE;
}

void CWinUtils::LogoffSessionByUsername(CHAR* logoffUsername)
{
	WTS_SESSION_INFO *sessionInfo = NULL;
	DWORD sessionInfoCount;
	char username[256] = { 0 };
	LPSTR pBuffer = NULL;
	DWORD dwBufferLen;
	BOOL bRet = FALSE;

	BOOL result = WTSEnumerateSessions(WTS_CURRENT_SERVER_HANDLE, 0, 1, &sessionInfo, &sessionInfoCount);
	unsigned int userCount(0);
	int num = 0;
	for (unsigned int i = 0; i < sessionInfoCount; ++i)
	{
		if ((sessionInfo[i].State == WTSActive) || (sessionInfo[i].State == WTSDisconnected))
		{
			bRet = WTSQuerySessionInformationA(WTS_CURRENT_SERVER_HANDLE, sessionInfo[i].SessionId, WTSUserName, &pBuffer, &dwBufferLen);
			if (bRet)
			{
				strcpy(username, pBuffer);
				if (strcmp(username, logoffUsername) == 0)
				{
					L_INFO(_T("Logoff the %S session %d\n"), username, sessionInfo[i].SessionId);

					//注销会话，阻塞等待完成才返回
					WTSLogoffSession(WTS_CURRENT_SERVER, sessionInfo[i].SessionId, TRUE);
				}
			}

			WTSFreeMemory(pBuffer);
			userCount++;
		}
	}
	WTSFreeMemory(sessionInfo); //释放  
}

BOOL CWinUtils::Is64BitOS()
{
	SYSTEM_INFO si = { 0 };
	OSVERSIONINFO osver;

	GetNativeSystemInfo(&si);
	if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64
		|| si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_IA64)
	{
		return TRUE;
	}
	
	return FALSE;
}

DWORD CWinUtils::GetOSVersion()
{
	SYSTEM_INFO si = { 0 };
	OSVERSIONINFO osver;
	DWORD dwRet = -1;

	GetNativeSystemInfo(&si);

	if (IsWindows10OrGreater())
	{
		if (IsWindowsServer())
		{
			dwRet = OS_WIN2012R2;
		}
		else
		{
			dwRet = OS_WIN10;
		}
	}
	else if (IsWindows8Point1OrGreater())
	{
		if (IsWindowsServer())
		{
			dwRet = OS_WIN2012R2;
		}
		else
		{
			dwRet = OS_WIN10;
		}
	}
	else
	{
		osver.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
		GetVersionEx(&osver);
		if (osver.dwPlatformId == VER_PLATFORM_WIN32_NT) {
			if (osver.dwMajorVersion == 5 && osver.dwMinorVersion == 1) {
				L_TRACE(_T("windows xp\n"));
				dwRet = OS_WINXP;
			}
			if (osver.dwMajorVersion == 5 && osver.dwMinorVersion == 2) {
				L_TRACE(_T("windows 2003\n"));
				dwRet = OS_WIN2003;
			}
			if (osver.dwMajorVersion == 6 && osver.dwMinorVersion == 0) {
				L_TRACE(_T("windows vista or windows 2008\n"));
				dwRet = OS_WINVISTA;
			}
			if (osver.dwMajorVersion == 6 && osver.dwMinorVersion == 1) {
				L_TRACE(_T("windows 7 or windows 2008 r2\n"));
				dwRet = OS_WIN7;
			}
			if (osver.dwMajorVersion == 6 && osver.dwMinorVersion == 2) {
				L_TRACE(_T("windows 8 or windows 2012\n"));
				dwRet = OS_WIN8;
			}
			if (osver.dwMajorVersion == 6 && osver.dwMinorVersion == 3) {
				L_TRACE(_T("windows 8.1 or windows 2012r2\n"));
				dwRet = OS_WIN81;
			}
		}
	}

	return dwRet;
}

BOOL CWinUtils::GetSessionToken(DWORD sessionid, HANDLE* userToken)
{
	BOOL result = FALSE;

	if (NULL == userToken)
	{
		L_ERROR(_T("GetSessionToken Param Error\n"));
		return FALSE;
	}

	result = WTSQueryUserToken(sessionid, userToken);
	if (!result)
	{
		L_ERROR(_T("WTSQueryUserToken Fail %d\n"), GetLastError());
		return FALSE;
	}

	return result;
}

BOOL CWinUtils::GetUacTokenForUserToken(HANDLE userToken, HANDLE *uacToken)
{
	BOOL result = FALSE;
	DWORD len = 0;

	if (NULL == userToken)
	{
		L_ERROR(_T("GetSessionToken Param Error\n"));
		return FALSE;
	}

	result = GetTokenInformation(userToken, TokenLinkedToken, uacToken, sizeof(HANDLE), &len);
	if (!result)
	{
		//某些用户没有uac账户
		L_WARN(_T("GetTokenInformation Fail %d\n"), GetLastError());
		*uacToken = NULL;
		return TRUE;
	}

	L_TRACE(_T("GetTokenInformation return ret %d handle2 %x Len %d\n"), result, uacToken, len);

	return result;
}


