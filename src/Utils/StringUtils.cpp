#include "Utils\StringUtils.h"

//宽字节字符串转窄字节字符串
void CStringUtils::Unicode2UTF8(const std::wstring& wstr, std::string& ustr)
{
	ustr = Convert(wstr, CP_UTF8);
}

//宽字节字符串转窄字节字符串
void CStringUtils::Unicode2ASCII(const std::wstring& wstr, std::string& str)
{
	str = Convert(wstr, CP_ACP);
}

//UTF8字符串转宽字节字符串
void CStringUtils::UTF82Unicode(const std::string& ustr, std::wstring& wstr)
{
	wstr = Convert(ustr, CP_UTF8);
}

//UTF8字符串转窄字节字符串
void UTF82ASCII(const std::string& ustr, std::string& str)
{
	std::wstring wstr;
	CStringUtils::UTF82Unicode(ustr, wstr);
	CStringUtils::Unicode2ASCII(wstr, str);
}

//窄字符串转为宽字符串
void CStringUtils::ASCII2Unicode(const std::string& str, std::wstring& wstr)
{
	wstr = Convert(str, CP_ACP);
}

//窄字符串转为UTF8字符串
void CStringUtils::ASCII2UTF8(const std::string& str, std::string& ustr)
{
	std::wstring wstr;
	ASCII2Unicode(str, wstr);
	Unicode2UTF8(wstr, ustr);
}

void CStringUtils::ReplaceAllString(std::string& str, const std::string& old_Value, const std::string& new_Value)
{
	std::string newstring;
	std::string::size_type pos(0);
	while (1)
	{
		std::string::size_type tempos(0);
		tempos = str.find(old_Value, pos);
		if (tempos == std::string::npos)
		{
			newstring += str.substr(pos);
			break;
		}
		newstring += str.substr(pos, tempos - pos);
		newstring += new_Value;
		pos = tempos + old_Value.length();
	}
	str = newstring;
}

void CStringUtils::ReplaceAllString(std::wstring& str, const std::wstring& old_Value, const std::wstring& new_Value)
{
	std::wstring newstring;
	std::wstring::size_type pos(0);
	while (1)
	{
		std::wstring::size_type tempos(0);
		tempos = str.find(old_Value, pos);
		if (tempos == std::wstring::npos)
		{
			newstring += str.substr(pos);
			break;
		}
		newstring += str.substr(pos, tempos - pos);
		newstring += new_Value;
		pos = tempos + old_Value.length();
	}
	str = newstring;
}

std::string CStringUtils::Format(const char *fmt, ...)
{
	std::string strResult = "";
	do
	{
		if (NULL == fmt)
		{
			break;
		}

		va_list marker = NULL;
		va_start(marker, fmt);                            //初始化变量参数 
		size_t nLength = _vscprintf(fmt, marker) + 1;    //获取格式化字符串长度

		if (1 >= nLength)
		{
			break;
		}

		std::vector<char> vBuffer(nLength, '\0');        //创建用于存储格式化字符串的字符数组
		int nWritten = _vsnprintf_s(&vBuffer[0], vBuffer.size(), nLength, fmt, marker);
		if (nWritten>0)
		{
			strResult = &vBuffer[0];
		}
		va_end(marker);                                    //重置变量参数

	} while (0);

	return strResult;
}

//格式化输出宽字符串
std::wstring CStringUtils::Format(const wchar_t *fmt, ...)
{
	std::wstring strResult = L"";
	do
	{
		if (NULL == fmt)
		{
			break;
		}
		va_list marker = NULL;
		va_start(marker, fmt);                            //初始化变量参数
		size_t nLength = _vscwprintf(fmt, marker) + 1;    //获取格式化字符串长度

		if (1 >= nLength)
		{
			break;
		}

		std::vector<wchar_t> vBuffer(nLength, L'\0');    //创建用于存储格式化字符串的字符数组
		int nWritten = _vsnwprintf_s(&vBuffer[0], vBuffer.size(), nLength, fmt, marker);
		if (nWritten > 0)
		{
			strResult = &vBuffer[0];
		}
		va_end(marker);                                    //重置变量参数
	} while (0);

	return strResult;
}

std::wstring CStringUtils::Convert(const std::string& s, UINT codepage)
{
	if (0 == s.length())
	{
		return L"";
	}

	wchar_t* buf = new wchar_t[s.length() * 2 + 1];
	if (NULL == buf)
	{
		return L"";
	}

	memset(buf, 0, (s.length() * 2 + 1) * sizeof(wchar_t));
	std::wstring ws;
	MultiByteToWideChar(codepage, 0, s.c_str(), -1, buf, s.length() + 1);
	ws = buf;
	delete[] buf;
	return ws;
}

std::string CStringUtils::Convert(const std::wstring& ws, UINT codepage)
{
	if (0 == ws.length())
	{
		return "";
	}

	char* buf = new char[3 * (ws.length() + 1)];

	if (NULL == buf)
	{
		return "";
	}

	memset(buf, 0, 3 * (ws.length() + 1));
	std::string s;
	WideCharToMultiByte(codepage, 0, ws.c_str(), -1,
		buf, (ws.length() + 1) * 3, NULL, NULL);
	s = buf;
	delete[] buf;
	return s;
}

void CStringUtils::TrimString(std::string& str)
{
	str.erase(0, str.find_first_not_of(" "));
	unsigned int nonSpacePos = str.find_last_not_of(" ");
	if (std::string::npos != nonSpacePos)
	{
		str.erase(nonSpacePos + 1);
	}
}

void CStringUtils::TrimString(std::wstring& str)
{
	str.erase(0, str.find_first_not_of(L" "));
	unsigned int nonSpacePos = str.find_last_not_of(L" ");
	if (std::wstring::npos != nonSpacePos)
	{
		str.erase(nonSpacePos + 1);
	}
}

void CStringUtils::SplitString(const std::string &srcString, const std::string &separator, std::vector<std::string> &strList)
{
	size_t pos = 0;
	for (;;)
	{
		size_t  tpos = 0;
		std::string substr;
		tpos = srcString.find(separator.c_str(), pos);
		if (tpos == std::string::npos)
		{
			substr = srcString.substr(pos);
			if (substr.size())
			{
				strList.push_back(substr);
			}
			break;
		}
		//if (tpos != pos)		//分隔符隔出来的空字符串也取出来
		{
			substr = srcString.substr(pos, tpos - pos);
			strList.push_back(substr);
		}
		pos = tpos + separator.length();

	}
}

void CStringUtils::SplitString(const std::wstring &srcString, const std::wstring &separator, std::vector<std::wstring> &strList)
{
	size_t pos = 0;
	for (;;)
	{
		size_t  tpos = 0;
		std::wstring substr;
		tpos = srcString.find(separator.c_str(), pos);
		//没有找到，则说明最后
		if (tpos == std::wstring::npos)
		{
			substr = srcString.substr(pos);
			if (substr.size())
			{
				strList.push_back(substr);
			}				break;
		}

		//if (tpos != pos)		//分隔符隔出来的空字符串也取出来
		{
			substr = srcString.substr(pos, tpos - pos);
			strList.push_back(substr);
		}
		pos = tpos + separator.length();
	}
}



template<class T>
T _ToLower(const T value)
{
	T temp = value;
	if ('A' <= value  && value <= 'Z')
	{
		temp = value - 'A' + 'a';
	}
	return temp;
}

template<class T>
T _ToUpper(const T value)
{
	T temp = value;
	if ('a' <= value  && value <= 'z')
	{
		temp = value - 'a' + 'A';
	}
	return temp;
}

std::wstring CStringUtils::ToLower(const std::wstring &srcString)
{
	std::wstring outstr = srcString;
	transform(srcString.begin(), srcString.end(), outstr.begin(), _ToLower<wchar_t>);
	return outstr;
}

std::string	 CStringUtils::ToLower(const std::string  &srcString)
{
	std::string outstr = srcString;
	transform(srcString.begin(), srcString.end(), outstr.begin(), _ToLower<char>);
	return outstr;
}

std::wstring CStringUtils::ToUpper(const std::wstring &srcString)
{
	std::wstring outstr = srcString;
	transform(srcString.begin(), srcString.end(), outstr.begin(), _ToUpper<wchar_t>);
	return outstr;
}

std::string  CStringUtils::ToUpper(const std::string  &srcString)
{
	std::string outstr = srcString;
	transform(srcString.begin(), srcString.end(), outstr.begin(), _ToUpper<char>);
	return outstr;
}
