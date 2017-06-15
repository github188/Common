#pragma once

/**
* @file ServiceUtils.h
* @brief ����ӿ���ͷ�ļ�
*/

#ifndef __SERVICE_UTILS_H__
#define __SERVICE_UTILS_H__

#include "DllExport.h"

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(1)

typedef BOOL (WINAPI *IServiceFunc)();

/**
 * @brief ���������Ϣ
 */
typedef struct
{
    BOOL         ConsoloMode;    /** < ����̨ģʽ      */
    WCHAR*       ServiceName;    /** < ��������        */
    WCHAR*       LogConfigPath;  /** < Log�����ļ�·�� */
    WCHAR*       LogModuleName;  /** < Log��ģ������   */
    IServiceFunc StartFun;       /** < ������������    */
    IServiceFunc StopFun;        /** < ������������    */
} ServiceBaseInfo;

#pragma pack()

/** @brief Windows �����ܺ���
*	@param[in]	ServiceBaseInfo		���������Ϣ
*	@par ʾ��
*	@code
*		ServiceBaseInfo info;
*		info.ConsoloMode   = FALSE;
*		info.LogConfigPath = _T("CommonTest_logconf.ini");
*		info.LogModuleName = _T("CommonTestService");
*		info.ServiceName   = _T("CommonTestService");
*		info.StartFun = CommonTestServiceStart;
*		info.StopFun       = CommonTestServiceStop;
*		ServiceMain(&info);
*	 @endcode
*/
DLL_COMMONLIB_API int WINAPI ServiceMain(ServiceBaseInfo *ServiceInfo);

/** @brief ��ȡ����״̬
*	@param[in]	serviceName	��������
*	@param[out] dwState ����״̬
*	@dwState ����ֵ�б�
*		SERVICE_STOPPED
*		SERVICE_START_PENDING
*		SERVICE_STOP_PENDING
*		SERVICE_RUNNING
*		SERVICE_CONTINUE_PENDING
*		SERVICE_PAUSE_PENDING
*		SERVICE_PAUSED
*	@return  ���ز������
*	 - false ʧ��
*	 - true �ɹ�
*/
DLL_COMMONLIB_API BOOL WINAPI GetServiceState(const WCHAR* serviceName, DWORD* dwState);

/** @brief ��������
*	@param[in]	serviceName	��������
*	@return  ���ز������
*	 - false ʧ��
*	 - true �ɹ�
*/
DLL_COMMONLIB_API BOOL WINAPI ServiceStart(const WCHAR* serviceName);

/** @brief ֹͣ����
*	@param[in]	serviceName	��������
*	@return  ���ز������
*	 - false ʧ��
*	 - true �ɹ�
*/
DLL_COMMONLIB_API BOOL WINAPI ServiceStop(const WCHAR* serviceName);

/** @brief ��װ���񣬷�������Ϊ�Զ�����
*	@param[in]	binPath ��������������·��
*	@param[in]	serviceName ��������
*	@param[in]	description ��������
*	@return  ���ز������
*	 - false ʧ��
*	 - true �ɹ�
*/
DLL_COMMONLIB_API BOOL WINAPI ServiceInstall(const WCHAR* binPath, const WCHAR* serviceName, WCHAR* description);

/** @brief ж�ط���
*	@param[in]	serviceName ��������
*	@return  ���ز������
*	 - false ʧ��
*	 - true �ɹ�
*/
DLL_COMMONLIB_API BOOL WINAPI ServiceUninstall(const WCHAR* serviceName);

#ifdef __cplusplus
}
#endif

#endif
