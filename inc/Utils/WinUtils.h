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

	/**
	* @brief ��ȡ������
	*
	* @param[out] name windows�������
	*
	* @return �ɹ� TRUE  \n
	*         ʧ�� FALSE
	*/
	static DLL_COMMONLIB_API BOOL WINAPI GetHostname(
		char name[MAX_SIZE_OF_HOSTNMAE + 1]);

	/**
	* @brief �����Ƿ��Թ���Ա��ʽ����
	*
	* @return �ɹ� TRUE  \n
	*         ʧ�� FALSE
	*/
	static DLL_COMMONLIB_API BOOL WINAPI IsAdminMode();

	/**
	* @brief �ж��ļ����ǲ���ȫ·��
	*
	* @param[in] filename �ļ���
	*
	* @return �ɹ� TRUE  \n
	*         ʧ�� FALSE
	*/
	static DLL_COMMONLIB_API BOOL WINAPI IsFullpath(const TCHAR *filename);

	/**
	* @brief ��ȡ��ǰ�������ڻỰID
	*
	* @return �ỰID
	*/
	static DLL_COMMONLIB_API DWORD WINAPI GetSessionID();

	/**
	* @brief ��ȡ�Ự��½�û���
	*
	* @param[in] sessionID �ỰID
	* @param[out] username      ��¼�û���
	*
	* @return �ɹ� TRUE  \n
	*         ʧ�� FALSE
	*/
	static DLL_COMMONLIB_API BOOL WINAPI GetSessionUserName(DWORD sessionID, tstring &username);

	/**
	* @brief �����ֽ�ת��Ϊ���ַ�
	*
	* @param[in] input ���ֽڴ�
	*
	* @return ���ַ���
	*/
	static DLL_COMMONLIB_API std::string WINAPI Wchar2Ansi(const WCHAR *input);

	/**
	* @brief �����ַ���ת��Ϊ���ַ���
	*
	* @param[in] input ���ַ���
	*
	* @return ���ֽڴ�
	*/
	static DLL_COMMONLIB_API std::wstring WINAPI Ansi2WideString(const char *input);

	/**
	* @brief ��ȡ��������
	*
	* @return ��������
	*/
	static DLL_COMMONLIB_API tstring WINAPI GetCurrentProcessName();

	/**
	* @brief ��ȡ������Ϣ
	*
	* @param[in] errorCode Windows������
	*
	* @return Windows������Ϣ
	*/
	static DLL_COMMONLIB_API tstring WINAPI GetErrorMsg(DWORD errorCode);

#define WIN32_ERROR_STRING GetErrorMsg(GetLastError()).c_str()
#define WINNET_ERROR_STRING GetErrorMsg(WSAGetLastError()).c_str()


};

#endif