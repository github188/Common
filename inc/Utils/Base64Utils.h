#pragma once

#ifndef __BASE64_UTILS_H__
#define __BASE64_UTILS_H__

#include "DllExport.h"

/**
* @class CBase64Utils
* @brief Base64����뾲̬��
*
* ����Base64����룬���������string,wtring��base64�����ı���ת��Ϊ��Ӧ�Ķ��������ݡ�������Ķ����Ʊ��룬	\
* ת��Ϊstring,wstring��base64�����ı� 
*/

class CBase64Utils
{
public:
	/** @brief ��������������Ϊbase64
	*	@param[in]	indata		ָ����������
	*	@param[in]	indatalen	ָ��������������
	*	@param[out] outbase64	���base64����
	*	@return  ���ز������
	*	 - false ת��ʧ��
	*	 - true	 ת���ɹ�
	*/
	static DLL_COMMONLIB_API bool WINAPI Encode(const void *indata, int indatalen, std::wstring &outbase64);

	/** @brief ��������������Ϊbase64
	*	@param[in]	indata		ָ����������
	*	@param[in]	indatalen	ָ��������������
	*	@param[out] outbase64	���base64����
	*	@return  ���ز������
	*	 - false ת��ʧ��
	*	 - true	 ת���ɹ�
	*/
	static DLL_COMMONLIB_API bool WINAPI Encode(const void *indata, int indatalen, std::string &outbase64);

	/** @brief ��������������Ϊbase64
	*	@param[in]	inbase64str		ָ����������
	*	@param[in,out]	outlen		ָ��������������
	*	@param[out] outdata			���base64����
	*	@return  ���ز������
	*	 - false ת��ʧ�ܣ�������������Ŀռ��С̫С
	*	 - true	 ��ʾ�ɹ�
	*	@note ��outlen �Ĵ�СС�� getCipherLen(instr.size())ʱ���������޸�outlen�ĳ��ȡ���ʾ��Ҫ���������Ŀռ��С��
	*	@par ʾ��
	*	@code
	*		int len = 0;
	*		Decode(instr,len,NULL);//len = getCipherLen(instr.size());
	*		char *outdata = new char[len];
	*		Decode(instr,len,outdata);
	*	 @endcode
	*/
	static DLL_COMMONLIB_API bool WINAPI Decode(const std::wstring &inbase64str, int &outlen, void *outdata);


	/** @brief ��������base64
	*	@param[in]	inbase64str		ָ����������
	*	@param[in,out]	outlen		ָ��������������
	*	@param[out] outdata			���base64����
	*	@return  ���ز������
	*	 - false ת��ʧ�ܣ�������������Ŀռ��С̫С
	*	 - true	 ��ʾ�ɹ�
	*	@note ��outlen �Ĵ�СС�� getCipherLen(instr.size())ʱ���������޸�outlen�ĳ��ȡ���ʾ��Ҫ���������Ŀռ��С��
	*	@par ʾ��
	*	@code
	*		int len = 0;
	*		Decode(instr,len,NULL);//len = getCipherLen(instr.size());
	*		char *outdata = new char[len];
	*		Decode(instr,len,outdata);
	*	 @endcode
	*/
	static DLL_COMMONLIB_API bool WINAPI Decode(const std::string &inbase64str, int &outlen, void *outdata);



	/** @brief Ԥ�⾭��base64���������ݳ���
	*	@param[in]	len	����������ݵĳ���
	*	@return  ����Ԥ��ı��������ݳ���
	*/
	static DLL_COMMONLIB_API int WINAPI getCipherLen(int len);


	/** @brief Ԥ�⾭��base64���������ݳ���
	*	@param[in]	len	����������ݵĳ���
	*	@return  ����Ԥ��Ľ��������ݳ���
	*/
	static DLL_COMMONLIB_API int WINAPI getPlainLen(int len);
};


#endif