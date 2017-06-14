#pragma once

/**
* @file ServiceUtils.h
* @brief 服务接口类头文件
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
 * @brief 服务基本信息
 */
typedef struct
{
    BOOL         ConsoloMode;    /** < 控制台模式      */
    WCHAR*       ServiceName;    /** < 服务名称        */
    WCHAR*       LogConfigPath;  /** < Log配置文件路径 */
    WCHAR*       LogModuleName;  /** < Log的模块名称   */
    IServiceFunc StartFun;       /** < 服务启动函数    */
    IServiceFunc StopFun;        /** < 服务启动函数    */
} ServiceBaseInfo;

#pragma pack()

/** @brief Windows 服务框架函数
*	@param[in]	ServiceBaseInfo		服务基本信息
*	@par 示例
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

#ifdef __cplusplus
}
#endif

#endif
