#pragma once

#ifndef __MD5_UTILS_H__
#define __MD5_UTILS_H__

#include "DllExport.h"

/**
* @class CMD5Utils
* @brief MD5编码静态库
*
* 用于计算文件、字符串、一段二进制数据的MD5值。
*/

class CMD5Utils
{
public:
	/** @brief 计算文件的MD5
	*	@param[in]	localFilePath 指定文件的路径
	*	@param[out] md5String	输出文件的MD5值
	*	@return  返回操作结果
	*	 - false 表示失败，无法计算文件的MD5值，可能是文件无法打开或无法读取
	*	 - true	 表示成功
	*	@par 示例
	*	@code
	*		wstring md5str;
	*		GetFileMD5(L"c:\\1.txt",md5str);
	*	 @endcode
	*/
	static DLL_COMMONLIB_API bool	WINAPI GetFileMD5(const std::wstring &localFilePath, std::wstring &md5String);


	/** @brief 计算字符串（窄字符）的MD5
	*	@param[in]	inString	指定字符串
	*	@param[out] md5String	输出文件的MD5值
	*	@warning	按二进制计算，相同的字符串的窄字节和宽字节计算出的MD5不一样
	*/
	static DLL_COMMONLIB_API void	WINAPI GetStringMD5(const std::string &inString, std::wstring	&md5String);

	/** @brief 计算字符串（宽字符）的MD5
	*	@param[in]	inString	指定字符串
	*	@param[out] md5String	输出文件的MD5值
	*	@warning	按二进制计算，相同的字符串的窄字节和宽字节计算出的MD5不一样
	*/
	static DLL_COMMONLIB_API void	WINAPI GetStringMD5(const std::wstring &inString, std::wstring &md5String);


	/** @brief 计算字符串（宽字符）的MD5
	*	@param[in]	data	指定二进制流
	*	@param[in]	dataLen	指定二进制流长度
	*	@param[out] md5String	输出文件的MD5值
	*/
	static DLL_COMMONLIB_API void	WINAPI GetDataMD5(const void *data, int dataLen, std::wstring	&md5String);
};


#endif