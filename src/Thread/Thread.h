/**
 * @file     Thread.h
 * @brief    线程类头文件
 */
#pragma once

#ifndef __THREAD_H__
#define __THREAD_H__

#include "Thread\ThreadUtils.h"

#ifdef WIN32
#include <Windows.h>
#include <list>
#include <process.h>
#else
#include <winpr/wtypes.h>
#include <winpr/synch.h>
#include <pthread.h>
#endif

#define  MODULE_NAME  L"ThreadUtils"

/** 
 * @class CThread
 * @brief 线程控制类
 *
 * 用于创建一个可控的线程对象，可以同步启动与停止线程
 * 在线程处理函数被动退出时也会调用注册的函数进行通知
 * 欢迎重载
 */
class CThread : public IThread
{
public:
    /** 构造函数与析构函数 */

    /**
     * @brief 基础构造函数
     *
     * 以指定函数为主函数创建线程，线程结束不执行任何函数
     *
     * @param[in] Func 主线程执行函数
     * @param[in] Param 主线程执行函数的入参
     */
	CThread(ThreadMainProc Func, LPVOID Param);
    /**
     * @brief 完整的构造函数
     *
     * 以指定函数为主函数创建线程，线程结束会调用结束函数
     *
     * @param[in] MainFunc 主线程执行函数
     * @param[in] MainParam 主线程入参
     * @param[in] Endfunc 线程结束时的执行函数
     * @param[in] Endparam 线程结束时的入参
     */
	CThread(ThreadMainProc MainFunc, LPVOID MainParam, ThreadEndProc Endfunc, LPVOID Endparam);

    /**
     * @brief 析构函数
     */
    virtual ~CThread(void);
public:
    /**
     * @brief 启动主线程
     * 
     * 返回后保证主线程开始运行
     */
    virtual void WINAPI StartMainThread();

    /**
     * @brief 结束主线程
     * 
     * 返回后保证主线程已经停止运行
     */
    virtual void WINAPI StopMainThread();

    /**
     * @brief 异步启动主线程
     * 
     * 返回后不保证主线程开始运行，主线程会在随后自主运行
     */
    virtual void WINAPI StartMainThreadAsync();

    /**
     * @brief 异步结束主线程
     *
     * 返回后不保证主线程停止，主线程会自主停止运行
     */
    virtual void WINAPI StopMainThreadAsync();

    /**
     * @brief 主线程是否正在运行
     * @return 返回主线程是否正在运行
     */
    virtual BOOL WINAPI IsMainThreadRunning();
private:
    /** 
     * @brief 初始化对象函数
     *
     * 只由构造函数调用的内部函数
     *
     * @param[in] MainFunc 主线程执行函数
     * @param[in] MainParam 主线程入参
     * @param[in] Endfunc 线程结束时的执行函数
     * @param[in] Endparam 线程结束时的入参
     */
    void Init(ThreadMainProc MainFunc, LPVOID MainParam, ThreadEndProc Endfunc, LPVOID Endparam);

    /**
     * @brief 主线程函数
     *
     * 复合windows的线程处理函数定义的主线程处理函数，在内部会调用注册的处理函数
     */
    static DWORD WINAPI MainThread(LPVOID Lp);

    
    HANDLE mMainThread;             /**< 主线程句柄         */
    UINT  mMainThreadId;           /**< 主线程ID           */
    HANDLE mMainThreadStartedEvent; /**< 主线程启动句柄     */
    HANDLE mStopMainThreadEvent;    /**< 停止主线程所用事件 */
    HANDLE mMainThreadStopedEvent;  /**< 主线程已经停止通告 */
    BOOL   mContinueMainThread;     /**< 线程是否要继续执行 */
    
    ThreadMainProc mMainProc;       /**< 主线程函数         */
    LPVOID         mMainProcParam;  /**< 主线程参数         */
    ThreadEndProc  mEndProc;        /**< 主线程结束通告函数 */
    LPVOID         mEndProcParam;   /**< 主线程结束通告参数 */
    CRITICAL_SECTION mEndLock;      /**< 结束通告锁         */
};

#endif
