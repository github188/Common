#pragma once

#ifndef __WIN_UTILS_H__
#define __WIN_UTILS_H__

#include <Windows.h>
#include <tchar.h>
#include <vector>
#include "DllExport.h"

/** 
 * @class CWinUtils
 * @brief Windows���ò�������
 */
class CWinUtils
{
public:
	/**
	* @brief ��ȡ�����������Լ���������MAC��ַ
	*
	* @param[out] numOfNic ������
	* @param[out] mac      ����mac��ַ����һ��������ַmac[0-5], �ڶ���������ַmac[6-11] ...
	*
	* @return �ɹ� TRUE  \n
	*         ʧ�� FALSE
	*/
	static DLL_COMMONLIB_API BOOL WINAPI GetMacAddr(
		int *numOfNic, std::vector<BYTE> &mac);

};

#endif