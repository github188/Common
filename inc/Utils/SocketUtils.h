#pragma once

#ifndef __SOCKET_UTILS_H__
#define __SOCKET_UTILS_H__

#include <WinSock2.h>
#include "DllExport.h"

/**
* @class CSocketUtils
* @brief Socket������̬��
*/

/**
* @brief ���ṹ
*/
typedef struct
{
	/**< ����С��������ͷ */
	DWORD len;
	/**< �����ͣ������� RP_XXX_XXX�Ķ��� */
	DWORD  type;
	/**< ���� */
	BYTE   data[1];
}PACKET_T, *PPACKET_T;
#define GET_PACKET_LEN(pkg)         ((pkg)->len)
#define PACKET_HEADER_LEN           (sizeof(PACKET_T) - 1)
#define GET_PACKET_DATA(pkg)        ((pkg)->data)
#define GET_PACKET_DATA_LEN(pkg)    ((pkg)->len - PACKET_HEADER_LEN)
#define GET_PACKET_TYPE(pkg)        ((pkg)->type)

class CSocketUtils
{
public:
	/**
	* @brief ��������socket
	*
	* @param [in] addr    - ������ַ��ΪNULL
	* @param [in] port    - �����˿�
	* @param [in] backlog - ͬʱ���������
	*
	* @return ���ش����ļ���socket����ʧ�ܷ���INVALID_SOCKET
	*/
	static DLL_COMMONLIB_API SOCKET WINAPI Socket_CreateListenSock(const TCHAR * /* addr */, USHORT port, int backlog);

	/**
	* @brief ��������socket
	*
	* @param [in] addr    - ���ӵ�ַ
	* @param [in] port    - ���Ӷ˿�
	*
	* @return ���ش�����socket����ʧ�ܷ���INVALID_SOCKET
	*/
	static DLL_COMMONLIB_API SOCKET WINAPI Socket_Connect(const TCHAR *addr, USHORT port);

	/**
	* @brief ��ȡ����ֽ�
	*/
	static DLL_COMMONLIB_API BOOL WINAPI Socket_ReadNBytes(
		SOCKET so, BYTE *buf, DWORD size, DWORD timeout, HANDLE stopEvent);

	/**
	* @brief д����ֽ�
	*/
	static DLL_COMMONLIB_API BOOL WINAPI Socket_WriteNBytes(
		SOCKET so, BYTE *buf, DWORD size, DWORD timeout, HANDLE stopEvent);

	/**
	* @brief ��ȡһ�����ݰ�
	*/
	static DLL_COMMONLIB_API PACKET_T * WINAPI Socket_RecvAPacket(SOCKET so, DWORD timeout, HANDLE stopEvent);

	/**
	* @brief дһ�����ݰ�
	*/
	static DLL_COMMONLIB_API BOOL WINAPI Socket_SendAPacket(SOCKET so, void *data, int datalen,
		DWORD type, DWORD timeout, HANDLE stopEvent);
};


#endif