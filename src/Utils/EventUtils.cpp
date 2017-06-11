#include "Utils\StringUtils.h"
#include "Utils\EventUtils.h"

HANDLE	CEventUtils::CreateGlobalEvent(BOOL manualReset, BOOL initialState, LPCSTR eventName)
{
	std::wstring _eventname;
	const wchar_t *_wceventname = NULL;

	if (eventName)
	{
		CStringUtils::ASCII2Unicode(eventName, _eventname);
		_wceventname = _eventname.c_str();
	}

	return CreateGlobalEvent(manualReset, initialState, _wceventname);
}


HANDLE	CEventUtils::CreateGlobalEvent(BOOL manualReset, BOOL initialState, LPCWSTR eventName)
{
	HANDLE event = NULL;
	if (NULL == eventName || wcslen(eventName) == 0)
	{
		return NULL;
	}
	PSECURITY_DESCRIPTOR pSec = (PSECURITY_DESCRIPTOR)LocalAlloc(LMEM_FIXED, SECURITY_DESCRIPTOR_MIN_LENGTH);
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
	event = CreateEventW(&attr, manualReset, initialState, eventName);
	LocalFree(pSec);
	return event;
}

HANDLE CEventUtils::GetGlobalEvent(LPCSTR eventName, BOOL manualReset, BOOL initialState)
{
	std::wstring _eventname;
	const wchar_t *_wceventname = NULL;

	if (eventName)
	{
		CStringUtils::ASCII2Unicode(eventName, _eventname);
		_wceventname = _eventname.c_str();
	}


	return GetGlobalEvent(_wceventname, manualReset, initialState);

}

HANDLE CEventUtils::GetGlobalEvent(LPCWSTR eventName, BOOL manualReset, BOOL initialState)
{
	HANDLE hEvent = OpenEventW(EVENT_ALL_ACCESS, FALSE, eventName);
	if (NULL == hEvent)
	{
		hEvent = CreateGlobalEvent(manualReset, initialState, eventName);
	}
	return hEvent;
}


HANDLE	CEventUtils::CreateLocalEvent(BOOL manualReset, BOOL initialState, LPCSTR eventName)
{
	std::wstring _eventname;
	const wchar_t *_wceventname = NULL;

	if (eventName)
	{
		CStringUtils::ASCII2Unicode(eventName, _eventname);
		_wceventname = _eventname.c_str();
	}

	return CreateLocalEvent(manualReset, initialState, _wceventname);
}

HANDLE	CEventUtils::CreateLocalEvent(BOOL manualReset, BOOL initialState, LPCWSTR eventName)
{
	return ::CreateEvent(NULL, manualReset, initialState, eventName);
}

HANDLE	CEventUtils::GetLocalEvent(LPCSTR eventName, BOOL manualReset, BOOL initialState)
{
	std::wstring _eventname;
	const wchar_t *_wceventname = NULL;

	if (eventName)
	{
		CStringUtils::ASCII2Unicode(eventName, _eventname);
		_wceventname = _eventname.c_str();
	}

	return GetLocalEvent(_wceventname, manualReset, initialState);
}

HANDLE	CEventUtils::GetLocalEvent(LPCWSTR eventName, BOOL manualReset, BOOL initialState)
{
	HANDLE hEvent = OpenEventW(EVENT_ALL_ACCESS, FALSE, eventName);
	if (NULL == hEvent)
	{
		hEvent = CreateLocalEvent(manualReset, initialState, eventName);
	}
	return hEvent;
}