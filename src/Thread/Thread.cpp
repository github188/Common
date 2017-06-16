#include "Log\LogBase.h"
#include "Thread.h"

CThread::CThread(ThreadMainProc Func, LPVOID Param)
{
    this->Init(Func, Param, NULL, NULL);
}

CThread::CThread(ThreadMainProc MainFunc, LPVOID MainParam, ThreadEndProc EndFunc, LPVOID EndParam)
{
    this->Init(MainFunc, MainParam, EndFunc, EndParam);
}

void CThread::Init(ThreadMainProc MainFunc, LPVOID MainParam, ThreadEndProc EndFunc, LPVOID EndParam)
{
    this->mMainThread = NULL;
    this->mMainThreadId = 0;
	this->mContinueMainThread = FALSE;

    this->mStopMainThreadEvent = CreateEvent(NULL,TRUE,FALSE,NULL);
    this->mMainThreadStartedEvent = CreateEvent(NULL,TRUE,FALSE,NULL);
	this->mMainThreadStopedEvent = CreateEvent(NULL, TRUE, TRUE, NULL);		//默认为已触发

    this->mMainProc = MainFunc;
    this->mMainProcParam = MainParam;
    this->mEndProc = EndFunc;
    this->mEndProcParam = EndParam;

    InitializeCriticalSection(&this->mEndLock);
}

CThread::~CThread(void)
{
    this->StopMainThread();

    if (this->mStopMainThreadEvent)
    {
        CloseHandle(this->mStopMainThreadEvent);
        this->mStopMainThreadEvent = NULL;
    }
    if (this->mMainThreadStopedEvent)
    {
        CloseHandle(this->mMainThreadStopedEvent);
        this->mMainThreadStopedEvent = NULL;
    }
    if (this->mMainThreadStartedEvent)
    {
        CloseHandle(this->mMainThreadStartedEvent);
        this->mMainThreadStartedEvent = NULL;
    }
    if (this->mMainThread)
    {
        CloseHandle(this->mMainThread);
        this->mMainThread = NULL;
        this->mMainThreadId = 0;
    }

    DeleteCriticalSection(&this->mEndLock);
}

void CThread::StartMainThreadAsync()
{
    ResetEvent(this->mStopMainThreadEvent);
    ResetEvent(this->mMainThreadStartedEvent);

    //TODO: 这里最好判断线程是否已经启动
    //启动线程
	this->mMainThread = (HANDLE)_beginthreadex(
		NULL,
		0,
		(unsigned int(__stdcall *)(void *))CThread::MainThread,
		this,
		0,
		&this->mMainThreadId);
}

void CThread::StartMainThread()
{
    this->StartMainThreadAsync();
    //等待启动完成
    WaitForSingleObject(this->mMainThreadStartedEvent, INFINITE);
}

void CThread::StopMainThreadAsync()
{
    //如果线程不在运行，则回收句柄之后返回，以保证可以再次启动
    if (!this->IsMainThreadRunning())
    {
        if (this->mMainThread)
        {
            CloseHandle(this->mMainThread);
            this->mMainThread = NULL;
            this->mMainThreadId = 0;
        }
        return;
    }

    if (this->mMainThread == NULL)
    {
        return;
    }

    //先破除循环
    this->mContinueMainThread = FALSE;
    //再置事件
    SetEvent(this->mStopMainThreadEvent);

    //回收句柄，以保证可以再次启动
    CloseHandle(this->mMainThread);
    this->mMainThread = NULL;
    this->mMainThreadId = 0;
}

void CThread::StopMainThread()
{
    this->StopMainThreadAsync();

    //等待退出成功的信号
    WaitForSingleObject(this->mMainThreadStopedEvent, INFINITE);
}

DWORD WINAPI CThread::MainThread(LPVOID Lp)
{
	CThread* Thread = (CThread*)Lp;
    
	L_TRACE_ENTER();

#ifndef WIN32
    pthread_detach(pthread_self());
#endif

    //通知线程已经启动
    ResetEvent(Thread->mStopMainThreadEvent);
    ResetEvent(Thread->mMainThreadStopedEvent);
    Thread->mContinueMainThread = TRUE;
    SetEvent(Thread->mMainThreadStartedEvent);

    //主线程循环执行
    while (Thread->mContinueMainThread)
    {
        if (!Thread->mMainProc(Thread->mMainProcParam, Thread->mStopMainThreadEvent))
        {
            //返回false就退出
            break;
        }
    }
    //结束函数调用
    EnterCriticalSection(&Thread->mEndLock);
    if (Thread->mEndProc) {
        Thread->mEndProc(Thread->mEndProcParam);
    }
    //设置结束事件
    SetEvent(Thread->mMainThreadStopedEvent);
    LeaveCriticalSection(&Thread->mEndLock);

	L_TRACE_LEAVE();

    return 0;
}


BOOL CThread::IsMainThreadRunning()
{
    //检测stopevent是否有被置位，以此判断线程是否在运行
    EnterCriticalSection(&this->mEndLock);
    DWORD ret = WaitForSingleObject(mMainThreadStopedEvent, 0);
    LeaveCriticalSection(&this->mEndLock);
    if (ret == WAIT_TIMEOUT)
    {
        return TRUE;
    }
    return FALSE;
}



