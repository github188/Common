#pragma once

#ifndef __MUTEX_UTILS_H__
#define __MUTEX_UTILS_H__

#include "DllExport.h"

/**
* @class CMutexUtils
* @brief 互斥量操作静态库
*
* 提供创建（全局、本地），获取（全局、本地）互斥量的相关封装动作。
*/

class CMutexUtils
{
public:
	/** @brief 创建全局的互斥量
	*	@param[in]	initialOwner	是否拥有锁
	*	@param[in]	mutexName		命名
	*	@return
	*	 - NULL	失败
	*	 - 其他	成功
	*/
	static DLL_COMMONLIB_API HANDLE	WINAPI CreateGlobalMutex(BOOL initialOwner, LPCSTR mutexName);

	/** @brief 创建全局的互斥量
	*	@param[in]	initialOwner	是否拥有锁
	*	@param[in]	mutexName		命名
	*	@return
	*	 - NULL	失败
	*	 - 其他	成功
	*/
	static DLL_COMMONLIB_API HANDLE	WINAPI CreateGlobalMutex(BOOL initialOwner, LPCWSTR mutexName);

	/** @brief 创建互斥量
	*	@param[in]	initialOwner	是否拥有锁
	*	@param[in]	mutexName		命名
	*	@return
	*	 - NULL	失败
	*	 - 其他	成功
	*/
	static DLL_COMMONLIB_API HANDLE	WINAPI CreateLocalMutex(BOOL initialOwner, LPCSTR mutexName);

	/** @brief 创建互斥量
	*	@param[in]	initialOwner	是否拥有锁
	*	@param[in]	mutexName		命名
	*	@return
	*	 - NULL	失败
	*	 - 其他	成功
	*/
	static DLL_COMMONLIB_API HANDLE	WINAPI CreateLocalMutex(BOOL initialOwner, LPCWSTR mutexName);

	/** @brief 获取全局互斥量
	*	@param[in]	mutexName		命名
	*	@param[in]	initialOwner	是否拥有锁
	*	@return
	*	 - NULL	失败
	*	 - 其他	成功
	*	@note	当互斥量命名不存在时，则创建该互斥量，当互斥量命名存在时，则获取该互斥量。
	*/
	static DLL_COMMONLIB_API HANDLE	WINAPI GetGlobalMutex(LPCSTR mutexName, BOOL initialOwner = FALSE);

	/** @brief 获取全局互斥量
	*	@param[in]	mutexName		命名
	*	@param[in]	initialOwner	是否拥有锁
	*	@return
	*	 - NULL	失败
	*	 - 其他	成功
	*	@note	当互斥量命名不存在时，则创建该互斥量，当互斥量命名存在时，则获取该互斥量。
	*/
	static DLL_COMMONLIB_API HANDLE	WINAPI GetGlobalMutex(LPCWSTR mutexName, BOOL initialOwner = FALSE);

	/** @brief 获取互斥量
	*	@param[in]	mutexName		命名
	*	@param[in]	initialOwner	是否拥有锁
	*	@return
	*	 - NULL	失败
	*	 - 其他	成功
	*	@note	当互斥量命名不存在时，则创建该互斥量，当互斥量命名存在时，则获取该互斥量。
	*/
	static DLL_COMMONLIB_API HANDLE	WINAPI GetLocalMutex(LPCSTR mutexName, BOOL initialOwner = FALSE);

	/** @brief 获取互斥量
	*	@param[in]	mutexName		命名
	*	@param[in]	initialOwner	是否拥有锁
	*	@return
	*	 - NULL	失败
	*	 - 其他	成功
	*	@note	当互斥量命名不存在时，则创建该互斥量，当互斥量命名存在时，则获取该互斥量。
	*/
	static DLL_COMMONLIB_API HANDLE	WINAPI GetLocalMutex(LPCWSTR mutexName, BOOL initialOwner = FALSE);

	/** @brief 等待并获取互斥量
	*	@param[in]	mutex			互斥句柄
	*	@param[in]	milliseconds	等待时间
	*	@return
	*	 - WAIT_OBJECT_0	成功
	*	 - WAIT_TIMEOUT		超时
	*	 - WAIT_ABANDONED	句柄无效，比如互斥
	*	@note 每次调用都要与ReleaseMutex对应
	*	@see ReleaseMutex
	*/
	static DLL_COMMONLIB_API DWORD	WINAPI WaitMutex(HANDLE mutex, DWORD milliseconds = INFINITE);

	/** @brief 释放互斥量
	*	@param[in]	mutex			互斥句柄
	*	@return
	*	 - true		成功
	*	 - false	失败
	*	@see WaitMutex
	*/
	static DLL_COMMONLIB_API BOOL	WINAPI ReleaseMutex(HANDLE mutex);
};


#endif