#pragma once

#ifndef __WIN_UTILS_H__
#define __WIN_UTILS_H__

#include <Windows.h>
#include <tchar.h>
#include <vector>
#include "DllExport.h"

#ifndef _UNICODE
typedef std::string tstring;
#else
typedef std::wstring tstring;
#endif

#ifndef MAX_SIZE_OF_HOSTNMAE
#define MAX_SIZE_OF_HOSTNMAE 31
#endif

/** 
 * @class CWinUtils
 * @brief Windows常用操作函数
 */
class CWinUtils
{
public:
	/**
	* @brief 获取机器网卡数以及各个网卡MAC地址
	*
	* @param[out] numOfNic 网卡数
	* @param[out] mac      网卡mac地址，第一个网卡地址mac[0-5], 第二个网卡地址mac[6-11] ...
	*
	* @return 成功 TRUE  \n
	*         失败 FALSE
	*/
	static DLL_COMMONLIB_API BOOL WINAPI GetMacAddr(
		int *numOfNic, std::vector<BYTE> &mac);

	/**
	* @brief 获取主机名
	*
	* @param[out] name windows计算机名
	*
	* @return 成功 TRUE  \n
	*         失败 FALSE
	*/
	static DLL_COMMONLIB_API BOOL WINAPI GetHostname(
		char name[MAX_SIZE_OF_HOSTNMAE + 1]);

	/**
	* @brief 程序是否以管理员方式运行
	*
	* @return 成功 TRUE  \n
	*         失败 FALSE
	*/
	static DLL_COMMONLIB_API BOOL WINAPI IsAdminMode();

	/**
	* @brief 判断文件名是不是全路径
	*
	* @param[in] filename 文件名
	*
	* @return 成功 TRUE  \n
	*         失败 FALSE
	*/
	static DLL_COMMONLIB_API BOOL WINAPI IsFullpath(const TCHAR *filename);

	/**
	* @brief 获取当前进程所在会话ID
	*
	* @return 会话ID
	*/
	static DLL_COMMONLIB_API DWORD WINAPI GetSessionID();

	/**
	* @brief 获取会话登陆用户名
	*
	* @param[in] sessionID 会话ID
	* @param[out] username      登录用户名
	*
	* @return 成功 TRUE  \n
	*         失败 FALSE
	*/
	static DLL_COMMONLIB_API BOOL WINAPI GetSessionUserName(DWORD sessionID, tstring &username);

	/**
	* @brief 将宽字节转换为多字符
	*
	* @param[in] input 宽字节串
	*
	* @return 多字符串
	*/
	static DLL_COMMONLIB_API std::string WINAPI Wchar2Ansi(const WCHAR *input);

	/**
	* @brief 将多字符串转换为宽字符串
	*
	* @param[in] input 多字符串
	*
	* @return 宽字节串
	*/
	static DLL_COMMONLIB_API std::wstring WINAPI Ansi2WideString(const char *input);

	/**
	* @brief 获取进程名称
	*
	* @return 进程名称
	*/
	static DLL_COMMONLIB_API tstring WINAPI GetCurrentProcessName();

	/**
	* @brief 获取错误信息
	*
	* @param[in] errorCode Windows错误码
	*
	* @return Windows错误信息
	*/
	static DLL_COMMONLIB_API tstring WINAPI GetErrorMsg(DWORD errorCode);

#define WIN32_ERROR_STRING GetErrorMsg(GetLastError()).c_str()
#define WINNET_ERROR_STRING GetErrorMsg(WSAGetLastError()).c_str()


};

#endif