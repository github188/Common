#pragma once

#ifndef __MD5_UTILS_H__
#define __MD5_UTILS_H__

#include "DllExport.h"

/**
* @class CMD5Utils
* @brief MD5���뾲̬��
*
* ���ڼ����ļ����ַ�����һ�ζ��������ݵ�MD5ֵ��
*/

class CMD5Utils
{
public:
	/** @brief �����ļ���MD5
	*	@param[in]	localFilePath ָ���ļ���·��
	*	@param[out] md5String	����ļ���MD5ֵ
	*	@return  ���ز������
	*	 - false ��ʾʧ�ܣ��޷������ļ���MD5ֵ���������ļ��޷��򿪻��޷���ȡ
	*	 - true	 ��ʾ�ɹ�
	*	@par ʾ��
	*	@code
	*		wstring md5str;
	*		GetFileMD5(L"c:\\1.txt",md5str);
	*	 @endcode
	*/
	static DLL_COMMONLIB_API bool	WINAPI GetFileMD5(const std::wstring &localFilePath, std::wstring &md5String);


	/** @brief �����ַ�����խ�ַ�����MD5
	*	@param[in]	inString	ָ���ַ���
	*	@param[out] md5String	����ļ���MD5ֵ
	*	@warning	�������Ƽ��㣬��ͬ���ַ�����խ�ֽںͿ��ֽڼ������MD5��һ��
	*/
	static DLL_COMMONLIB_API void	WINAPI GetStringMD5(const std::string &inString, std::wstring	&md5String);

	/** @brief �����ַ��������ַ�����MD5
	*	@param[in]	inString	ָ���ַ���
	*	@param[out] md5String	����ļ���MD5ֵ
	*	@warning	�������Ƽ��㣬��ͬ���ַ�����խ�ֽںͿ��ֽڼ������MD5��һ��
	*/
	static DLL_COMMONLIB_API void	WINAPI GetStringMD5(const std::wstring &inString, std::wstring &md5String);


	/** @brief �����ַ��������ַ�����MD5
	*	@param[in]	data	ָ����������
	*	@param[in]	dataLen	ָ��������������
	*	@param[out] md5String	����ļ���MD5ֵ
	*/
	static DLL_COMMONLIB_API void	WINAPI GetDataMD5(const void *data, int dataLen, std::wstring	&md5String);
};


#endif