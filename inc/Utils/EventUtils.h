#pragma once

#ifndef __EVENT_UTILS_H__
#define __EVENT_UTILS_H__

#include "DllExport.h"

/**
* @class CEventUtils
* @brief 事件操作静态库
*
* 提供创建（全局、本地）事件，获取（全局、本地）事件的相关封装动作。
*/

class CEventUtils
{
public:
	/** @brief 创建全局事件
	*	@param[in]	manualReset	是否手动清除信号
	*	@param[in]	initialState	设置事件的初始化状态
	*	@param[in]	eventName		设置命名事件，全局事件，该命名不能为空
	*	@return
	*	 - NULL	失败
	*	 - 其他
	*/
	static DLL_COMMONLIB_API 	HANDLE	WINAPI CreateGlobalEvent(BOOL manualReset, BOOL initialState, LPCSTR eventName);

	/** @brief 创建全局事件
	*	@param[in]	manualReset		是否手动清除信号
	*	@param[in]	initialState	设置事件的初始化状态
	*	@param[in]	eventName		设置命名事件，全局事件，该命名不能为空
	*	@return
	*	 - NULL	失败
	*	 - 其他
	*/
	static DLL_COMMONLIB_API 	HANDLE	WINAPI CreateGlobalEvent(BOOL manualReset, BOOL initialState, LPCWSTR eventName);


	/** @brief 获取全局事件
	*	@param[in]	eventName		设置命名事件，全局事件，该命名不能为空
	*	@param[in]	manualReset		是否手动清除信号
	*	@param[in]	initialState	设置事件的初始化状态
	*	@return
	*	 - NULL	失败
	*	 - 其他
	*	@note	当不存在该命名事件时，则创建。若存在则打开
	*/
	static DLL_COMMONLIB_API 	HANDLE	WINAPI GetGlobalEvent(LPCSTR eventName, BOOL manualReset = TRUE, BOOL initialState = FALSE);

	/** @brief 获取全局事件
	*	@param[in]	eventName		设置命名事件，全局事件，该命名不能为空
	*	@param[in]	manualReset		是否手动清除信号
	*	@param[in]	initialState	设置事件的初始化状态
	*	@return
	*	 - NULL	失败
	*	 - 其他
	*	@note	当不存在该命名事件时，则创建。若存在则打开
	*/
	static DLL_COMMONLIB_API 	HANDLE	WINAPI GetGlobalEvent(LPCWSTR eventName, BOOL manualReset = TRUE, BOOL initialState = FALSE);


	/** @brief 创建事件
	*	@param[in]	manualReset	是否手动清除信号
	*	@param[in]	initialState	设置事件的初始化状态
	*	@param[in]	eventName		设置命名事件，全局事件，该命名不能为空
	*	@return
	*	 - NULL	失败
	*	 - 其他
	*/
	static DLL_COMMONLIB_API 	HANDLE	WINAPI CreateLocalEvent(BOOL manualReset, BOOL initialState, LPCSTR eventName = NULL);

	/** @brief 创建事件
	*	@param[in]	manualReset		是否手动清除信号
	*	@param[in]	initialState	设置事件的初始化状态
	*	@param[in]	eventName		设置命名事件，全局事件，该命名不能为空
	*	@return
	*	 - NULL	失败
	*	 - 其他
	*/
	static DLL_COMMONLIB_API 	HANDLE	WINAPI CreateLocalEvent(BOOL manualReset, BOOL initialState, LPCWSTR eventName = NULL);

	/** @brief 获取事件
	*	@param[in]	eventName		设置命名事件，全局事件，该命名不能为空
	*	@param[in]	manualReset		是否手动清除信号
	*	@param[in]	initialState	设置事件的初始化状态
	*	@return
	*	 - NULL	失败
	*	 - 其他
	*	@note 		当不存在该命名事件时，则创建。若存在则打开
	*/
	static DLL_COMMONLIB_API 	HANDLE	WINAPI GetLocalEvent(LPCSTR eventName, BOOL manualReset = TRUE, BOOL initialState = FALSE);

	/** @brief 获取事件
	*	@param[in]	eventName		设置命名事件，全局事件，该命名不能为空
	*	@param[in]	manualReset		是否手动清除信号
	*	@param[in]	initialState	设置事件的初始化状态
	*	@return
	*	 - NULL	失败
	*	 - 其他
	*	@note 		当不存在该命名事件时，则创建。若存在则打开
	*/
	static DLL_COMMONLIB_API 	HANDLE	WINAPI GetLocalEvent(LPCWSTR eventName, BOOL manualReset = TRUE, BOOL initialState = FALSE);
};


#endif