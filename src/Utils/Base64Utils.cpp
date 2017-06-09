#include "Common\base64.h"
#include "Utils\Base64Utils.h"
#include "Utils\StringUtils.h"

int CBase64Utils::getCipherLen(int len)
{

	int outlen = len + len / 3 + (int)(len % 3 != 0);

	if (outlen % 4)
		outlen += 4 - outlen % 4;

	return outlen;
}


int CBase64Utils::getPlainLen(int len)
{
	return len - len / 4 + 4;
}


bool CBase64Utils::Encode(const void *indata, int indatalen, std::wstring &outbase64)
{
	int len = getCipherLen(indatalen);
	char *buffer = NULL;
	bool	ret = false;

	do
	{
		buffer = new char[len + 1];
		if (NULL == buffer)
		{
			break;
		}
		memset(buffer, 0, len + 1);

		len = Base64Encode(buffer, (char *)indata, indatalen);
		CStringUtils::ASCII2Unicode(buffer, outbase64);
		free(buffer);
		buffer = NULL;
		ret = true;

	} while (0);

	return ret;
}

bool CBase64Utils::Encode(const void *indata, int indatalen, std::string &outbase64)
{
	int len = getCipherLen(indatalen);
	char *buffer = NULL;
	bool	ret = false;

	do
	{
		buffer = new char[len + 1];
		if (NULL == buffer)
		{
			break;
		}
		memset(buffer, 0, len + 1);

		len = Base64Encode(buffer, (char *)indata, indatalen);
		outbase64 = buffer;
		free(buffer);
		buffer = NULL;
		ret = true;

	} while (0);

	return ret;
}

bool CBase64Utils::Decode(const std::wstring &inbase64str, int &outlen, void *outdata)
{
	bool ret = false;
	int len = 0;
	do
	{
		std::string _tempstr;
		if (getPlainLen(inbase64str.size())>outlen)
		{
			outlen = getPlainLen(inbase64str.size());
			break;
		}

		CStringUtils::Unicode2ASCII(inbase64str, _tempstr);

		len = Base64Decode((char *)outdata, _tempstr.c_str(), _tempstr.size());
		outlen = len;
		ret = true;

	} while (0);

	return ret;
}

bool CBase64Utils::Decode(const std::string &inbase64str, int &outlen, void *outdata)
{
	bool ret = false;
	int len = 0;
	do
	{
		std::string _tempstr;
		if (getPlainLen(inbase64str.size())>outlen)
		{
			outlen = getPlainLen(inbase64str.size());
			break;
		}

		len = Base64Decode((char *)outdata, inbase64str.c_str(), inbase64str.size());
		outlen = len;
		ret = true;

	} while (0);

	return ret;
}
