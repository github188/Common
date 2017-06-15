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

BOOL WINAPI GetServiceState(const WCHAR* serviceName, DWORD* dwState)
{
	SC_HANDLE scm, service;
	SERVICE_STATUS serviceStatus;
	*dwState = 0;

	L_TRACE_ENTER();
	L_INFO(L"GetServiceState begin, name = %s\r\n", serviceName);

	scm = OpenSCManager(0, 0, SC_MANAGER_CONNECT);
	if (!scm)
	{
		L_FATAL(L"OpenSCManager Fail(%d)\r\n", GetLastError());
		return FALSE;
	}

	service = OpenService(scm, serviceName, SERVICE_QUERY_STATUS);
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

	CloseServiceHandle(service);
	CloseServiceHandle(scm);

	L_INFO(L"GetServiceState success, name = %s\r\n", serviceName);
	L_TRACE_LEAVE();

	*dwState = serviceStatus.dwCurrentState;
	return TRUE;
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

	if (!QueryServiceStatus(service, &serviceStatus))
	{
		L_FATAL(L"QueryServiceStatus Fail(%d)\r\n", GetLastError());
		CloseServiceHandle(service);
		CloseServiceHandle(scm);
		return FALSE;
	}
	if (serviceStatus.dwCurrentState == SERVICE_RUNNING)
	{
		L_INFO(L"Service has been running already, name = %s\r\n", serviceName);
		CloseServiceHandle(service);
		CloseServiceHandle(scm);
		return TRUE;
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

BOOL WINAPI ServiceInstall(const WCHAR* binPath, const WCHAR* serviceName, WCHAR* description)
{
	SC_HANDLE scm, service;
	SERVICE_DESCRIPTION sd;

	L_TRACE_ENTER();
	L_INFO(L"Install service begin, name = %s\r\n", serviceName);

	scm = OpenSCManager(0, 0, SC_MANAGER_CREATE_SERVICE);
	if (!scm)
	{
		L_FATAL(L"OpenSCManager Fail(%d)\r\n", GetLastError());
		return FALSE;
	}

	service = CreateService(scm,
		serviceName,
		serviceName,
		SERVICE_ALL_ACCESS,
		SERVICE_WIN32_OWN_PROCESS | SERVICE_INTERACTIVE_PROCESS,
		SERVICE_AUTO_START,
		SERVICE_ERROR_NORMAL,
		binPath,
		NULL, NULL, NULL, NULL, NULL);
	if (!service)
	{
		if (ERROR_SERVICE_EXISTS == GetLastError())	//已经存在该服务名称了
		{
			L_INFO(L"Service has been exist already, name = %s", serviceName);
			service = OpenService(scm,
				serviceName,
				SERVICE_QUERY_STATUS |
				SERVICE_START |
				SERVICE_CHANGE_CONFIG |
				SERVICE_QUERY_CONFIG);
		}
	}
	else
	{
		//设置服务的描述信息
		if (NULL != description)
		{
			sd.lpDescription = description;
			ChangeServiceConfig2(service, SERVICE_CONFIG_DESCRIPTION, &sd);
		}
	}

	if (!service)
	{
		L_FATAL(L"CreateService Fail(%d)\r\n", GetLastError());
		CloseServiceHandle(scm);
		return FALSE;
	}

	CloseServiceHandle(service);
	CloseServiceHandle(scm);
	
	L_INFO(L"Install service success, name = %s\r\n", serviceName);
	L_TRACE_LEAVE();

	return TRUE;
}

BOOL WINAPI ServiceUninstall(const WCHAR* serviceName)
{
	SC_HANDLE scm, service;
	SERVICE_STATUS serviceStatus;

	L_TRACE_ENTER();
	L_INFO(L"Uninstall service begin, name = %s\r\n", serviceName);

	scm = OpenSCManager(0, 0, SC_MANAGER_CONNECT);
	if (!scm)
	{
		L_FATAL(L"OpenSCManager Fail(%d)\r\n", GetLastError());
		return FALSE;
	}

	service = OpenService(scm, serviceName, SERVICE_QUERY_STATUS | SERVICE_STOP | DELETE);
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

	if (serviceStatus.dwCurrentState != SERVICE_STOPPED)
	{
		BOOL bStop = ServiceStop(serviceName);
		if (!bStop)
		{
			L_FATAL(L"ServiceStop Fail(%d)\r\n", GetLastError());
			CloseServiceHandle(service);
			CloseServiceHandle(scm);
			return FALSE;
		}
	}

	if (!DeleteService(service))
	{
		L_FATAL(L"DeleteService Fail(%d)\r\n", GetLastError());
		CloseServiceHandle(service);
		CloseServiceHandle(scm);
		return FALSE;
	}

	CloseServiceHandle(service);
	CloseServiceHandle(scm);

	L_INFO(L"Uninstall service success, name = %s\r\n", serviceName);
	L_TRACE_LEAVE();

	return TRUE;
}