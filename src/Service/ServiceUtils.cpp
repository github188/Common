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
	SC_HANDLE scm, service;
	SERVICE_STATUS serviceStatus;
	BOOL bSussecced = FALSE;

	L_TRACE_ENTER();
	L_INFO(L"Start service begin, name = %s\r\n", serviceName);

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

	L_INFO(L"Start service success, name = %s\r\n", serviceName);
	L_TRACE_LEAVE();

	return TRUE;
}

BOOL WINAPI ServiceStop(const WCHAR* serviceName)
{
	SC_HANDLE scm, service;
	SERVICE_STATUS serviceStatus;

	L_TRACE_ENTER();
	L_INFO(L"Stop service begin, name = %s\r\n", serviceName);

	scm = OpenSCManager(0, 0, SC_MANAGER_CONNECT);
	if (!scm)
	{
		L_FATAL(L"OpenSCManager Fail(%d)\r\n", GetLastError());
		return FALSE;
	}

	service = OpenService(scm, serviceName, SERVICE_QUERY_STATUS | SERVICE_STOP);
	if (!service)
	{
		L_FATAL(L"OpenService Fail(%d)\r\n", GetLastError());
		CloseServiceHandle(scm);
		return FALSE;
	}

	if (!QueryServiceStatus(service, &serviceStatus))
	{
		L_FATAL(L"QueryServiceStatus Fail(%d)\r\n", GetLastError());
		CloseServiceHandle(service);
		CloseServiceHandle(scm);
		return FALSE;
	}
	if (serviceStatus.dwCurrentState == SERVICE_STOPPED)
	{
		L_INFO(L"Service has been stopped already, name = %s\r\n", serviceName);
		CloseServiceHandle(service);
		CloseServiceHandle(scm);
		return TRUE;
	}

	if (!ControlService(service, SERVICE_CONTROL_STOP, &serviceStatus))
	{
		L_FATAL(L"ControlService Fail(%d)\r\n", GetLastError());
		CloseServiceHandle(service);
		CloseServiceHandle(scm);
		return FALSE;
	}

	do
	{
		Sleep(1000);

		if (!QueryServiceStatus(service, &serviceStatus))
		{
			L_FATAL(L"QueryServiceStatus Fail(%d)\r\n", GetLastError());
			CloseServiceHandle(service);
			CloseServiceHandle(scm);
			return FALSE;
		}

	} while (serviceStatus.dwCurrentState != SERVICE_STOPPED);

	CloseServiceHandle(service);
	CloseServiceHandle(scm);

	L_INFO(L"Stop service success, name = %s\r\n", serviceName);
	L_TRACE_LEAVE();

	return TRUE;
}