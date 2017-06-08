#pragma once

#ifndef __WIN_UTILS_H__
#define __WIN_UTILS_H__

#include <Windows.h>
#include <tchar.h>
#include <vector>
#include "DllExport.h"

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

};

#endif