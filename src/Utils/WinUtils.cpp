#include "stdafx.h"
#include <vector>
#include <string>
#include <Iphlpapi.h>
#include <WtsApi32.h>
#include "CTUtils.h"
#include "Utils\WinUtils.h"

BOOL CWinUtils::GetMacAddr(int *numOfNic, std::vector<BYTE> &mac)
{
	PIP_ADAPTER_INFO pAdapterInfo = NULL;
	ULONG ulSizeAdapterInfo = 0;
	DWORD dwReturnvalueGetAdapterInfo = NO_ERROR;
	PIP_ADAPTER_INFO pOriginalPointer = NULL;
	DWORD r = FALSE;
	int index;

	*numOfNic = 0;
	mac.clear();
	dwReturnvalueGetAdapterInfo = GetAdaptersInfo(pAdapterInfo, &ulSizeAdapterInfo);
	if (dwReturnvalueGetAdapterInfo == ERROR_BUFFER_OVERFLOW)
	{
		pAdapterInfo = (PIP_ADAPTER_INFO)malloc(ulSizeAdapterInfo);
		if (!pAdapterInfo)
		{
			//L_FATAL(_T("malloc fail\n"));
			return FALSE;
		}
		pOriginalPointer = pAdapterInfo;
		dwReturnvalueGetAdapterInfo = GetAdaptersInfo(pAdapterInfo, &ulSizeAdapterInfo);
	}

	if (dwReturnvalueGetAdapterInfo != NO_ERROR)
	{
		//L_WARN(_T("GetAdaptersInfo FAIL:0x%x\n"), GetLastError());
		goto EXIT;
	}

	if (ulSizeAdapterInfo == 0)
	{
		//L_WARN(_T("No found netcard in system\n"));
		goto EXIT;
	}
	r = TRUE;

	index = 0;
	while (pAdapterInfo != NULL)
	{
		/* MAC地址长度不是6字节??? */

		if (pAdapterInfo->Type != MIB_IF_TYPE_ETHERNET || pAdapterInfo->AddressLength != 6)
		{
			pAdapterInfo = pAdapterInfo->Next;
		}
		else
		{
			BYTE mac00[6] = { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };
			BYTE macff[6] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };

			/*
			* 忽略全0及全ff的mac地址
			*/
			if (!(memcmp(pAdapterInfo->Address, mac00, 6) == 0)
				&& !(memcmp(pAdapterInfo->Address, macff, 6) == 0))
			{
				mac.insert(mac.end(), pAdapterInfo->Address, pAdapterInfo->Address + 6);
				(*numOfNic)++;
			}
			pAdapterInfo = pAdapterInfo->Next;
		}
	}

EXIT:
	if (pOriginalPointer)
	{
		free(pOriginalPointer);
	}

	return r;
}





