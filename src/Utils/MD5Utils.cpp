#include "Common\md5.h"
#include "Utils\StringUtils.h"
#include "Utils\MD5Utils.h"

bool CMD5Utils::GetFileMD5(const std::wstring& localFilePath, std::wstring &md5String)
{
	// 打开文件流
	std::ifstream localFileStream;
	localFileStream.open(localFilePath.c_str(), std::ios_base::in | std::ios_base::binary);
	if (!localFileStream)
	{
		return false;
	}
	// 计算MD5
	CMD5 md5;
	md5.update(localFileStream);
	md5String = CStringUtils::Convert(md5.toString());
	localFileStream.close();
	return true;
}


void CMD5Utils::GetStringMD5(const std::string &inString, std::wstring &md5String)
{
	CMD5	md5;
	md5.update(inString);
	md5String = CStringUtils::Convert(md5.toString());
}

void CMD5Utils::GetStringMD5(const std::wstring &inString, std::wstring &md5String)
{

	CMD5	md5;
	string _tempstr;
	md5.update(inString.c_str(), inString.size() * sizeof(wchar_t));
	md5String = CStringUtils::Convert(md5.toString());
}


void CMD5Utils::GetDataMD5(const void *data, int len, std::wstring &md5String)
{
	CMD5	md5;
	md5.update(data, len);
	md5String = CStringUtils::Convert(md5.toString());
}