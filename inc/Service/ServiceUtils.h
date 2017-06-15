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

#ifdef __cplusplus
}
#endif

#endif
