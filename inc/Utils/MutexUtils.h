#pragma once

#ifndef __MUTEX_UTILS_H__
#define __MUTEX_UTILS_H__

#include "DllExport.h"

/**
* @class CMutexUtils
* @brief ������������̬��
*
* �ṩ������ȫ�֡����أ�����ȡ��ȫ�֡����أ�����������ط�װ������
*/

class CMutexUtils
{
public:
	/** @brief ����ȫ�ֵĻ�����
	*	@param[in]	initialOwner	�Ƿ�ӵ����
	*	@param[in]	mutexName		����
	*	@return
	*	 - NULL	ʧ��
	*	 - ����	�ɹ�
	*/
	static DLL_COMMONLIB_API HANDLE	WINAPI CreateGlobalMutex(BOOL initialOwner, LPCSTR mutexName);

	/** @brief ����ȫ�ֵĻ�����
	*	@param[in]	initialOwner	�Ƿ�ӵ����
	*	@param[in]	mutexName		����
	*	@return
	*	 - NULL	ʧ��
	*	 - ����	�ɹ�
	*/
	static DLL_COMMONLIB_API HANDLE	WINAPI CreateGlobalMutex(BOOL initialOwner, LPCWSTR mutexName);

	/** @brief ����������
	*	@param[in]	initialOwner	�Ƿ�ӵ����
	*	@param[in]	mutexName		����
	*	@return
	*	 - NULL	ʧ��
	*	 - ����	�ɹ�
	*/
	static DLL_COMMONLIB_API HANDLE	WINAPI CreateLocalMutex(BOOL initialOwner, LPCSTR mutexName);

	/** @brief ����������
	*	@param[in]	initialOwner	�Ƿ�ӵ����
	*	@param[in]	mutexName		����
	*	@return
	*	 - NULL	ʧ��
	*	 - ����	�ɹ�
	*/
	static DLL_COMMONLIB_API HANDLE	WINAPI CreateLocalMutex(BOOL initialOwner, LPCWSTR mutexName);

	/** @brief ��ȡȫ�ֻ�����
	*	@param[in]	mutexName		����
	*	@param[in]	initialOwner	�Ƿ�ӵ����
	*	@return
	*	 - NULL	ʧ��
	*	 - ����	�ɹ�
	*	@note	������������������ʱ���򴴽��û�����������������������ʱ�����ȡ�û�������
	*/
	static DLL_COMMONLIB_API HANDLE	WINAPI GetGlobalMutex(LPCSTR mutexName, BOOL initialOwner = FALSE);

	/** @brief ��ȡȫ�ֻ�����
	*	@param[in]	mutexName		����
	*	@param[in]	initialOwner	�Ƿ�ӵ����
	*	@return
	*	 - NULL	ʧ��
	*	 - ����	�ɹ�
	*	@note	������������������ʱ���򴴽��û�����������������������ʱ�����ȡ�û�������
	*/
	static DLL_COMMONLIB_API HANDLE	WINAPI GetGlobalMutex(LPCWSTR mutexName, BOOL initialOwner = FALSE);

	/** @brief ��ȡ������
	*	@param[in]	mutexName		����
	*	@param[in]	initialOwner	�Ƿ�ӵ����
	*	@return
	*	 - NULL	ʧ��
	*	 - ����	�ɹ�
	*	@note	������������������ʱ���򴴽��û�����������������������ʱ�����ȡ�û�������
	*/
	static DLL_COMMONLIB_API HANDLE	WINAPI GetLocalMutex(LPCSTR mutexName, BOOL initialOwner = FALSE);

	/** @brief ��ȡ������
	*	@param[in]	mutexName		����
	*	@param[in]	initialOwner	�Ƿ�ӵ����
	*	@return
	*	 - NULL	ʧ��
	*	 - ����	�ɹ�
	*	@note	������������������ʱ���򴴽��û�����������������������ʱ�����ȡ�û�������
	*/
	static DLL_COMMONLIB_API HANDLE	WINAPI GetLocalMutex(LPCWSTR mutexName, BOOL initialOwner = FALSE);

	/** @brief �ȴ�����ȡ������
	*	@param[in]	mutex			������
	*	@param[in]	milliseconds	�ȴ�ʱ��
	*	@return
	*	 - WAIT_OBJECT_0	�ɹ�
	*	 - WAIT_TIMEOUT		��ʱ
	*	 - WAIT_ABANDONED	�����Ч�����绥��
	*	@note ÿ�ε��ö�Ҫ��ReleaseMutex��Ӧ
	*	@see ReleaseMutex
	*/
	static DLL_COMMONLIB_API DWORD	WINAPI WaitMutex(HANDLE mutex, DWORD milliseconds = INFINITE);

	/** @brief �ͷŻ�����
	*	@param[in]	mutex			������
	*	@return
	*	 - true		�ɹ�
	*	 - false	ʧ��
	*	@see WaitMutex
	*/
	static DLL_COMMONLIB_API BOOL	WINAPI ReleaseMutex(HANDLE mutex);
};


#endif