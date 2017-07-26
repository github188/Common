/**
* @file     Service.cpp
* @brief    服务接口类源文件
*/
#include "Service\ServiceUtils.h"
#include "Log\LogBase.h"
#include "mindump.h"

#define	MODULE_NAME		L"Service"

static ServiceBaseInfo*      gServiceInfo = NULL;
static HANDLE                gTerminateEvent = NULL;
static SERVICE_STATUS_HANDLE gServiceStatusHandle = NULL;
static DWORD                 gCurrentState;

static void PrintSeverBaseInfo(ServiceBaseInfo* service)
{
	L_INFO(L"%s service, build at %S %S\n",
		service->ServiceName,
		__DATE__,
		__TIME__);

	L_INFO(L"\tLog config file: %s Module Name\n", service->LogConfigPath, service->LogModuleName);
}

static BOOL
SendStatusToSCM(DWORD dwCurrentState,
	DWORD dwWin32ExitCode,
	DWORD dwServiceSpecificExitCode,
	DWORD dwCheckPoint,
	DWORD dwWaitHint)
{
	BOOL success;
	SERVICE_STATUS serviceStatus;

	gCurrentState = dwCurrentState;

	// Fill in all of the SERVICE_STATUS fields
	serviceStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
	serviceStatus.dwCurrentState = dwCurrentState;

	/*
	* If in the process of doing something, then accept
	* no control events, else accept anything
	*/
	if (dwCurrentState == SERVICE_START_PENDING)
	{
		serviceStatus.dwControlsAccepted = 0;
	}
	else
	{
		serviceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP |
			SERVICE_ACCEPT_SHUTDOWN;
	}

	/*
	* if a specific exit code is defined, set up
	* the win32 exit code properly
	*/
	if (dwServiceSpecificExitCode == 0)
	{
		serviceStatus.dwWin32ExitCode = dwWin32ExitCode;
	}
	else
	{
		serviceStatus.dwWin32ExitCode = ERROR_SERVICE_SPECIFIC_ERROR;
	}

	serviceStatus.dwServiceSpecificExitCode = dwServiceSpecificExitCode;
	serviceStatus.dwCheckPoint = dwCheckPoint;
	serviceStatus.dwWaitHint = dwWaitHint;

	// Pass the status record to the SCM
	success = SetServiceStatus(gServiceStatusHandle, &serviceStatus);
	return success;
}

/*
* 服务事件处理函数
*/
static DWORD WINAPI
HandlerEx(DWORD controlCode,
	DWORD dwEventType,
	LPVOID lpEventData,
	LPVOID lpContext)
{
	UNREFERENCED_PARAMETER(dwEventType);
	UNREFERENCED_PARAMETER(lpEventData);
	UNREFERENCED_PARAMETER(lpContext);

	switch (controlCode)
	{
		/*
		*  SERVICE_CONTROL_SHUTDOWN means turn off the machine
		*  Do nothing in a shutdown. Could do cleanup
		*  here but it must be very quick.
		*/
	case SERVICE_CONTROL_SHUTDOWN:
		L_DEBUG(L"***** SERVICE_CONTROL_SHUTDOWN *****\n");
		SendStatusToSCM(SERVICE_STOP_PENDING,
			NO_ERROR, 0, 1, 1000);
		SetEvent(gTerminateEvent);
		return NO_ERROR;

		// Stop the service
	case SERVICE_CONTROL_STOP:
		L_DEBUG(L"***** SERVICE_CONTROL_STOP *****\n");
		SendStatusToSCM(SERVICE_STOP_PENDING,
			NO_ERROR, 0, 1, 5000);
		SetEvent(gTerminateEvent);
		return NO_ERROR;

	case SERVICE_CONTROL_INTERROGATE:
		L_DEBUG(L"***** SERVICE_CONTROL_INTERROGATE *****\n");
		SendStatusToSCM(gCurrentState, NO_ERROR, 0, 0, 0);
		return NO_ERROR;

	default:
		L_DEBUG(L"Unhandle Service Control Code %d\n", controlCode);
		SendStatusToSCM(gCurrentState, NO_ERROR, 0, 0, 0);
		return ERROR_CALL_NOT_IMPLEMENTED;
	}
}


/*
* 服务主处理函数
*/
static VOID WINAPI ServiceMainFunction(DWORD argc, LPTSTR * argv)
{
	BOOL ret;

	L_TRACE_ENTER();

	UNREFERENCED_PARAMETER(argc);
	UNREFERENCED_PARAMETER(argv);

	//注册服务事件处理函数
	gServiceStatusHandle = RegisterServiceCtrlHandlerEx(gServiceInfo->ServiceName, HandlerEx, NULL);
	if (!gServiceStatusHandle)
	{
		L_FATAL(L"RegisterServiceCtrlHandlerEx Fail(%d)\n", GetLastError());
		goto exit;
	}

	L_TRACE(_T("ServiceMainFunction RegisterServiceCtrlHandlerEx\r\n"));

	/*
	* 报告启动进度
	*/
	ret = SendStatusToSCM(SERVICE_START_PENDING, NO_ERROR, 0, 1, 5000);
	if (!ret)
	{
		L_FATAL(L"SendStatusToSCM Fail(%d)\n", GetLastError());
		goto exit;
	}

	L_TRACE(_T("ServiceMainFunction SendStartStatusToSCM\r\n"));

	gTerminateEvent = CreateEvent(0, TRUE, FALSE, NULL);
	if (!gTerminateEvent)
	{
		L_FATAL(L"CreateEvent Fail(%d)\n", GetLastError());
		goto exit;
	}

	L_TRACE(_T("ServiceMainFunction CreateTerminateEvent\r\n"));

	if (gServiceInfo->StartFun)
	{
		(gServiceInfo->StartFun)();
	}

	L_TRACE(_T("ServiceMainFunction StartFun\r\n"));

	/*
	* 报告启动进度
	*/
	ret = SendStatusToSCM(SERVICE_RUNNING, NO_ERROR, 0, 0, 0);
	if (!ret)
	{
		L_FATAL(L"SendStatusToSCM Fail(%d)\n", GetLastError());
		goto end;
	}

	L_INFO(L"%s service running...\n", gServiceInfo->ServiceName);

	// 等待服务停止
	WaitForSingleObject(gTerminateEvent, INFINITE);

	L_TRACE_LEAVE();
end:
	if (gServiceInfo->StopFun)
	{
		(gServiceInfo->StopFun)();
	}

exit:
	if (gTerminateEvent)
	{
		CloseHandle(gTerminateEvent);
	}

	if (gServiceStatusHandle)
	{
		SendStatusToSCM(SERVICE_STOPPED, 0, 0, 0, 0);
	}
}

static BOOL WINAPI HandlerConsoleCtrl(DWORD dwCtrlType)
{
	if (dwCtrlType == CTRL_C_EVENT || dwCtrlType == CTRL_CLOSE_EVENT)
	{
		SetEvent(gTerminateEvent);
		return TRUE;
	}

	return FALSE;
}

static int ServiceMainProcess(ServiceBaseInfo *service)
{
	WORD            wVersionRequested;
	WSADATA         wsaData;
	BOOL            bConsole = FALSE;

	gServiceInfo = service;

	L_INFO(L"Starting %s service...\n", service->ServiceName);
	/*
	* 输出版本信息
	*/
	PrintSeverBaseInfo(service);

	/*
	* 初始化Win Socket2
	*/
	wVersionRequested = MAKEWORD(2, 2);
	if (WSAStartup(wVersionRequested, &wsaData) != 0)
	{
		L_FATAL(L"WSAStartup Fail(%d)\n", GetLastError());
		goto EXIT;
	}

	if (!service->ConsoleMode)
	{
		WCHAR ServiceName[MAX_PATH];

		wcscpy(ServiceName, service->ServiceName);

		SERVICE_TABLE_ENTRY serviceTable[] = {
			{ ServiceName, (LPSERVICE_MAIN_FUNCTION)ServiceMainFunction },
			{ NULL, NULL }
		};

		L_INFO(_T("StartServiceCtrlDispatcher\r\n"));
		/*
		* 启动服务
		*/
		if (StartServiceCtrlDispatcher(serviceTable) == FALSE)
		{
			L_FATAL(L"StartServiceCtrlDispatcher Fail(%d)\n", GetLastError());
		}
	}
	else
	{
		//控制台模式
		gTerminateEvent = CreateEvent(0, TRUE, FALSE, NULL);
		if (gTerminateEvent == NULL)
		{
			goto EXIT;
		}

		if (service->StartFun)
		{
			(service->StartFun)();
		}

		SetConsoleCtrlHandler(HandlerConsoleCtrl, TRUE);
		WaitForSingleObject(gTerminateEvent, INFINITE);

		if (service->StopFun)
		{
			(service->StopFun)();
		}
		CloseHandle(gTerminateEvent);
	}

EXIT:
	L_INFO(L"%s service exit\n", service->ServiceName);

	WSACleanup();

	return 0;
}

int WINAPI ServiceMainLib(ServiceBaseInfo *ServiceInfo)
{
	CTMinDumpInit(NULL, NULL, 0);

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	LogBase_init(ServiceInfo->LogConfigPath);

	ServiceMainProcess(ServiceInfo);

	LogBase_done();

	return 0;
}
