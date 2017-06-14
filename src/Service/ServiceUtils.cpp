#include "Service\ServiceUtils.h"
#include "Log\LogBase.h"
#include "Service.h"

#ifndef  MODULE_NAME
#define	MODULE_NAME		L"ServiceUtils"
#endif // !MODULE_NAME


int WINAPI ServiceMain(ServiceBaseInfo *ServiceInfo)
{
	return ServiceMainLib(ServiceInfo);
}

BOOL WINAPI ServiceStart(const WCHAR* serviceName)
{
	DWORD dwError = NOERROR;
	SC_HANDLE scm, service;
	SERVICE_STATUS serviceStatus;
	BOOL bSussecced = FALSE;

	L_TRACE_ENTER();

	scm = OpenSCManager(0, 0, SC_MANAGER_CONNECT);
	if (!scm)
	{
		L_FATAL(L"OpenSCManager Fail(%d)\r\n", GetLastError());
		return FALSE;
	}

	service = OpenService(scm, serviceName, SERVICE_QUERY_STATUS | SERVICE_START);
	if (!service)
	{
		L_FATAL(L"OpenService Fail(%d)\r\n", GetLastError());
		CloseServiceHandle(scm);
		return FALSE;
	}

	if (!StartService(service, 0, NULL))
	{
		L_FATAL(L"StartService Fail(%d)\r\n", GetLastError());
		CloseServiceHandle(service);
		CloseServiceHandle(scm);
		return FALSE;
	}

	do
	{
		Sleep(1000);
		bSussecced = QueryServiceStatus(service, &serviceStatus);
		if (!bSussecced)
		{
			L_FATAL(L"QueryServiceStatus Fail(%d)\r\n", GetLastError());
			CloseServiceHandle(service);
			CloseServiceHandle(scm);
			return FALSE;
		}

	} while (serviceStatus.dwCurrentState == SERVICE_START_PENDING);

	if (serviceStatus.dwCurrentState != SERVICE_RUNNING)
	{
		L_FATAL(L"Service state isn't SERVICE_RUNNING failed Error = %d\r\n", GetLastError());
		CloseServiceHandle(service);
		CloseServiceHandle(scm);
		return FALSE;
	}

	CloseServiceHandle(service);
	CloseServiceHandle(scm);

	L_INFO(L"Start Servie : %s Success.\r\n", serviceName);

	L_TRACE_LEAVE();

	return TRUE;
}