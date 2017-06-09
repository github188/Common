#include "Common\md5.h"
#include "Utils\StringUtils.h"
#include "Utils\MutexUtils.h"

HANDLE	CMutexUtils::CreateGlobalMutex(BOOL initialOwner, LPCSTR mutexName)
{
	std::wstring _mutexName;
	const wchar_t *_wcmutexName = NULL;

	if (mutexName)
	{
		CStringUtils::ASCII2Unicode(mutexName, _mutexName);
		_wcmutexName = _mutexName.c_str();
	}

	return CreateGlobalMutex(initialOwner, _wcmutexName);
}


HANDLE	CMutexUtils::CreateGlobalMutex(BOOL initialOwner, LPCWSTR mutexName)
{
	HANDLE hMutex = NULL;
	PSECURITY_DESCRIPTOR pSec = (PSECURITY_DESCRIPTOR)LocalAlloc(LMEM_FIXED, SECURITY_DESCRIPTOR_MIN_LENGTH);

	if (NULL == mutexName || wcslen(mutexName) == 0)
	{
		return NULL;
	}

	if (!pSec)
	{
		return NULL;
	}

	if (!InitializeSecurityDescriptor(pSec, SECURITY_DESCRIPTOR_REVISION))
	{
		LocalFree(pSec);
		return NULL;
	}

	if (!SetSecurityDescriptorDacl(pSec, TRUE, NULL, TRUE))
	{
		LocalFree(pSec);
		return NULL;
	}

	SECURITY_ATTRIBUTES attr;
	attr.bInheritHandle = FALSE;
	attr.lpSecurityDescriptor = pSec;
	attr.nLength = sizeof(SECURITY_ATTRIBUTES);

	hMutex = ::CreateMutexW(&attr, initialOwner, mutexName);

	LocalFree(pSec);
	return hMutex;
}


HANDLE	CMutexUtils::GetGlobalMutex(LPCSTR mutexName, BOOL initialOwner)
{
	std::wstring _mutexName;
	const wchar_t *_wcmutexname = NULL;

	if (mutexName)
	{
		CStringUtils::ASCII2Unicode(mutexName, _mutexName);
		_wcmutexname = _mutexName.c_str();
	}

	return	GetGlobalMutex(_wcmutexname, initialOwner);
}


HANDLE	CMutexUtils::GetGlobalMutex(LPCWSTR mutexName, BOOL initialOwner)
{
	HANDLE hMutex = ::OpenMutex(MUTEX_ALL_ACCESS, FALSE, mutexName);
	if (hMutex == NULL)
	{
		hMutex = CreateGlobalMutex(initialOwner, mutexName);
	}
	return hMutex;

}


HANDLE	CMutexUtils::GetLocalMutex(LPCSTR mutexName, BOOL initialOwner)
{
	std::wstring _mutexName;
	const wchar_t *_wcmutexName = NULL;

	if (mutexName)
	{
		CStringUtils::ASCII2Unicode(mutexName, _mutexName);
		_wcmutexName = _mutexName.c_str();
	}

	return	GetLocalMutex(_wcmutexName, initialOwner);

}


HANDLE	CMutexUtils::GetLocalMutex(LPCWSTR mutexName, BOOL initialOwner)
{
	HANDLE hMutex = ::OpenMutex(MUTEX_ALL_ACCESS, FALSE, mutexName);
	if (hMutex == NULL)
	{
		hMutex = CreateLocalMutex(initialOwner, mutexName);
	}
	return hMutex;
}


HANDLE	CMutexUtils::CreateLocalMutex(BOOL initialOwner, LPCSTR mutexName)
{
	std::wstring _mutexName;
	const wchar_t *_wcmutexName = NULL;

	if (mutexName)
	{
		CStringUtils::ASCII2Unicode(mutexName, _mutexName);
		_wcmutexName = _mutexName.c_str();
	}

	return CreateLocalMutex(initialOwner, _wcmutexName);
}


HANDLE	CMutexUtils::CreateLocalMutex(BOOL initialOwner, LPCWSTR mutexName)
{
	return	::CreateMutexW(NULL, initialOwner, mutexName);
}


DWORD	CMutexUtils::WaitMutex(HANDLE handle, DWORD milliseconds)
{
	return ::WaitForSingleObject(handle, milliseconds);
}


BOOL	CMutexUtils::ReleaseMutex(HANDLE mutex)
{
	return	::ReleaseMutex(mutex);
}