#pragma once

#ifndef __EVENT_UTILS_H__
#define __EVENT_UTILS_H__

#include "DllExport.h"

/**
* @class CEventUtils
* @brief �¼�������̬��
*
* �ṩ������ȫ�֡����أ��¼�����ȡ��ȫ�֡����أ��¼�����ط�װ������
*/

class CEventUtils
{
public:
	/** @brief ����ȫ���¼�
	*	@param[in]	manualReset	�Ƿ��ֶ�����ź�
	*	@param[in]	initialState	�����¼��ĳ�ʼ��״̬
	*	@param[in]	eventName		���������¼���ȫ���¼�������������Ϊ��
	*	@return
	*	 - NULL	ʧ��
	*	 - ����
	*/
	static DLL_COMMONLIB_API 	HANDLE	WINAPI CreateGlobalEvent(BOOL manualReset, BOOL initialState, LPCSTR eventName);

	/** @brief ����ȫ���¼�
	*	@param[in]	manualReset		�Ƿ��ֶ�����ź�
	*	@param[in]	initialState	�����¼��ĳ�ʼ��״̬
	*	@param[in]	eventName		���������¼���ȫ���¼�������������Ϊ��
	*	@return
	*	 - NULL	ʧ��
	*	 - ����
	*/
	static DLL_COMMONLIB_API 	HANDLE	WINAPI CreateGlobalEvent(BOOL manualReset, BOOL initialState, LPCWSTR eventName);


	/** @brief ��ȡȫ���¼�
	*	@param[in]	eventName		���������¼���ȫ���¼�������������Ϊ��
	*	@param[in]	manualReset		�Ƿ��ֶ�����ź�
	*	@param[in]	initialState	�����¼��ĳ�ʼ��״̬
	*	@return
	*	 - NULL	ʧ��
	*	 - ����
	*	@note	�������ڸ������¼�ʱ���򴴽������������
	*/
	static DLL_COMMONLIB_API 	HANDLE	WINAPI GetGlobalEvent(LPCSTR eventName, BOOL manualReset = TRUE, BOOL initialState = FALSE);

	/** @brief ��ȡȫ���¼�
	*	@param[in]	eventName		���������¼���ȫ���¼�������������Ϊ��
	*	@param[in]	manualReset		�Ƿ��ֶ�����ź�
	*	@param[in]	initialState	�����¼��ĳ�ʼ��״̬
	*	@return
	*	 - NULL	ʧ��
	*	 - ����
	*	@note	�������ڸ������¼�ʱ���򴴽������������
	*/
	static DLL_COMMONLIB_API 	HANDLE	WINAPI GetGlobalEvent(LPCWSTR eventName, BOOL manualReset = TRUE, BOOL initialState = FALSE);


	/** @brief �����¼�
	*	@param[in]	manualReset	�Ƿ��ֶ�����ź�
	*	@param[in]	initialState	�����¼��ĳ�ʼ��״̬
	*	@param[in]	eventName		���������¼���ȫ���¼�������������Ϊ��
	*	@return
	*	 - NULL	ʧ��
	*	 - ����
	*/
	static DLL_COMMONLIB_API 	HANDLE	WINAPI CreateLocalEvent(BOOL manualReset, BOOL initialState, LPCSTR eventName = NULL);

	/** @brief �����¼�
	*	@param[in]	manualReset		�Ƿ��ֶ�����ź�
	*	@param[in]	initialState	�����¼��ĳ�ʼ��״̬
	*	@param[in]	eventName		���������¼���ȫ���¼�������������Ϊ��
	*	@return
	*	 - NULL	ʧ��
	*	 - ����
	*/
	static DLL_COMMONLIB_API 	HANDLE	WINAPI CreateLocalEvent(BOOL manualReset, BOOL initialState, LPCWSTR eventName = NULL);

	/** @brief ��ȡ�¼�
	*	@param[in]	eventName		���������¼���ȫ���¼�������������Ϊ��
	*	@param[in]	manualReset		�Ƿ��ֶ�����ź�
	*	@param[in]	initialState	�����¼��ĳ�ʼ��״̬
	*	@return
	*	 - NULL	ʧ��
	*	 - ����
	*	@note 		�������ڸ������¼�ʱ���򴴽������������
	*/
	static DLL_COMMONLIB_API 	HANDLE	WINAPI GetLocalEvent(LPCSTR eventName, BOOL manualReset = TRUE, BOOL initialState = FALSE);

	/** @brief ��ȡ�¼�
	*	@param[in]	eventName		���������¼���ȫ���¼�������������Ϊ��
	*	@param[in]	manualReset		�Ƿ��ֶ�����ź�
	*	@param[in]	initialState	�����¼��ĳ�ʼ��״̬
	*	@return
	*	 - NULL	ʧ��
	*	 - ����
	*	@note 		�������ڸ������¼�ʱ���򴴽������������
	*/
	static DLL_COMMONLIB_API 	HANDLE	WINAPI GetLocalEvent(LPCWSTR eventName, BOOL manualReset = TRUE, BOOL initialState = FALSE);
};


#endif