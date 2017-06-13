#include "Utils\SocketUtils.h"
#include "Log\LogBase.h"

#define MODULE_NAME		_T("SocketUtils")

SOCKET CSocketUtils::Socket_CreateListenSock(const TCHAR * /* addr */, USHORT port, int backlog)
{
	SOCKET listenSock = INVALID_SOCKET;
	SOCKADDR_IN sockAddr;
	int ret = 0;
	BOOL bReuseaddr = FALSE;

	// 创建监听socket
	listenSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == listenSock)
	{
		L_ERROR(_T("create listen socket failed %d\n"), WSAGetLastError());
		goto EXIT;
	}

	// 允许重复绑定端口
	bReuseaddr = TRUE;
	ret = setsockopt(listenSock,
		SOL_SOCKET,
		SO_REUSEADDR,
		(const char*)&bReuseaddr,
		sizeof(BOOL));
	if (ret == SOCKET_ERROR)
	{
		L_WARN(_T("set SO_REUSEADDR failed %d\n"), WSAGetLastError());
	}

	// FIXME 可以设定绑定的监听地址
	sockAddr.sin_addr.s_addr = INADDR_ANY;
	sockAddr.sin_port = htons(port);
	sockAddr.sin_family = AF_INET;
	if (SOCKET_ERROR == bind(listenSock, (PSOCKADDR)&sockAddr, sizeof(sockAddr)))
	{
		L_ERROR(_T("bind port %d failed: %d \n"), port, WSAGetLastError());
		listenSock = INVALID_SOCKET;
		goto EXIT;
	}

	// 监听socket连接,设置同时的最大连接数
	if (SOCKET_ERROR == listen(listenSock, backlog))
	{
		L_ERROR(_T("listen failed: %d\n"), WSAGetLastError());
		listenSock = INVALID_SOCKET;
		goto EXIT;
	}

EXIT:

	return listenSock;
}

SOCKET CSocketUtils::Socket_Connect(const TCHAR *addr, USHORT port)
{
	SOCKET sock;
	struct sockaddr_in sock_addr;
	char   serverAddr[MAX_PATH + 1];
	int ret = 0;
	int nodelay = 0;

#ifdef _UNICODE
	memset(serverAddr, 0, sizeof(serverAddr));
	wcstombs(serverAddr, addr, MAX_PATH);
#else
	serverAddr[MAX_PATH] = 0;
	strncpy(serverAddr, addr, MAX_PATH);
#endif

	// 创建套接字
	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock == INVALID_SOCKET)
	{
		L_ERROR(_T("Create socket fail: %d"), WSAGetLastError());
		return INVALID_SOCKET;
	}

	// 服务器地址
	if (inet_addr(serverAddr) == INADDR_NONE)
	{
		struct hostent *hp = NULL;

		if ((hp = gethostbyname(serverAddr)) != NULL)
		{
			// TODO: 连接所有可用域名
			sock_addr.sin_family = AF_INET;
			sock_addr.sin_addr.s_addr = *(u_long *)hp->h_addr_list[0];
		}
		else
		{
			L_WARN(_T("Get host name fail\n"));
			return INVALID_SOCKET;
		}
	}
	else
	{
		sock_addr.sin_addr.s_addr = inet_addr(serverAddr);
		sock_addr.sin_family = AF_INET;
	}
	sock_addr.sin_port = htons(port);

	// 连接服务器
	L_TRACE(_T("conneting to %s:%d\n"), addr, port);
	ret = connect(sock, (struct sockaddr *) &sock_addr, sizeof(sock_addr));
	if (ret < 0)
	{
		closesocket(sock);
		L_WARN(_T("connect to server %s:%d fail: %d\n"),
			addr, port, WSAGetLastError());
		return INVALID_SOCKET;
	}

	// 设置nodelay参数
	nodelay = 1;
	setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (const char *)&nodelay, sizeof(int));

	return sock;
}

BOOL CSocketUtils::Socket_ReadNBytes(SOCKET so, BYTE *buf, DWORD size, DWORD timeout, HANDLE stopEvent)
{
	BOOL ret = FALSE;
	DWORD cbBytesRead = 0;
	OVERLAPPED ol;
	BYTE *pos;
	HANDLE hReadEvent;

	memset(&ol, 0, sizeof(ol));
	ol.hEvent = hReadEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	if (ol.hEvent == NULL)
	{
		return FALSE;
	}

	pos = buf;
	while (size > 0)
	{
		BOOL    readRet;
		DWORD   flag = 0;
		WSABUF  wsabuf;

		wsabuf.buf = (char*)pos,
			wsabuf.len = size;
		readRet = WSARecv(so, &wsabuf, 1, &cbBytesRead, &flag, &ol, NULL);
		if (readRet == 0)
		{
			if (cbBytesRead == 0)
			{
				goto EXIT;
			}
			pos += cbBytesRead;
			size -= cbBytesRead;
			ResetEvent(hReadEvent);
		}
		else
		{
			if (WSAGetLastError() == ERROR_IO_PENDING)
			{
				DWORD waitRet;
				HANDLE handles[2];
				int numOfHandles = 1;

				handles[0] = hReadEvent;
				if (stopEvent)
				{
					handles[1] = stopEvent;
					numOfHandles++;
				}

				waitRet = WSAWaitForMultipleEvents(numOfHandles, handles, FALSE, timeout, FALSE);
				if (waitRet == WSA_WAIT_EVENT_0)
				{
					readRet = WSAGetOverlappedResult(so, &ol, &cbBytesRead, TRUE, &flag);
					if (readRet == FALSE)
					{
						goto EXIT;
					}
					pos += cbBytesRead;
					size -= cbBytesRead;
					ResetEvent(hReadEvent);
				}
				else /* timeout, notify event or other */
				{
					/*
					* Cancel request
					*/
					if (CancelIo((HANDLE)so) == FALSE)
					{
						L_ERROR(_T("IoCancel fail 0x%x, force close socket\n"), GetLastError());
						closesocket(so);
					}
					else
					{
						/*
						* Wait cannel finish
						*/
						WSAGetOverlappedResult(so, &ol, &cbBytesRead, TRUE, &flag);
					}
					goto EXIT;
				}
			}
			else
			{
				goto EXIT;
			}
		}
	}
	ret = TRUE;

EXIT:
	if (hReadEvent)
	{
		CloseHandle(hReadEvent);
	}
	return ret;
}

BOOL CSocketUtils::Socket_WriteNBytes(SOCKET so, BYTE *buf, DWORD size, DWORD timeout, HANDLE stopEvent)
{
	BOOL            ret = FALSE;
	BOOL            writeRet;
	OVERLAPPED      ol;
	BYTE            *pos;
	DWORD           cbBytesWrite;
	HANDLE          hWriteEvent;

	memset(&ol, 0, sizeof(ol));
	ol.hEvent = hWriteEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	if (ol.hEvent == NULL)
	{
		return FALSE;
	}

	pos = buf;
	while (size > 0)
	{
		WSABUF  wsabuf;
		DWORD   flag = 0;

		wsabuf.buf = (char*)pos,
			wsabuf.len = size;
		writeRet = WSASend(so, &wsabuf, 1, &cbBytesWrite, 0, &ol, NULL);
		if (writeRet == 0)
		{
			if (cbBytesWrite == 0)
			{
				goto EXIT;
			}
			pos += cbBytesWrite;
			size -= cbBytesWrite;
			ResetEvent(hWriteEvent);
		}
		else
		{
			if (WSAGetLastError() == ERROR_IO_PENDING)
			{
				DWORD waitRet;
				HANDLE handles[2];
				int numOfHandles = 1;

				handles[0] = hWriteEvent;
				if (stopEvent)
				{
					handles[1] = stopEvent;
					numOfHandles++;
				}

				waitRet = WSAWaitForMultipleEvents(numOfHandles, handles, FALSE, timeout, FALSE);
				if (waitRet == WSA_WAIT_EVENT_0)
				{
					writeRet = WSAGetOverlappedResult(so, &ol, &cbBytesWrite, TRUE, &flag);
					if (writeRet == FALSE)
					{
						goto EXIT;
					}
					pos += cbBytesWrite;
					size -= cbBytesWrite;
					ResetEvent(hWriteEvent);
				}
				else /* timeout, notify event or other */
				{
					/*
					* Cancel request
					*/
					if (CancelIo((HANDLE)so) == FALSE)
					{
						L_ERROR(_T("IoCancel fail 0x%x, force close socket\n"), GetLastError());
						closesocket(so);
					}
					else
					{
						/*
						* Wait cannel finish
						*/
						WSAGetOverlappedResult(so, &ol, &cbBytesWrite, TRUE, &flag);
					}
					goto EXIT;
				}
			}
			else
			{
				goto EXIT;
			}
		}
	}
	ret = TRUE;

EXIT:
	if (hWriteEvent)
	{
		CloseHandle(hWriteEvent);
	}
	return ret;
}

PACKET_T * CSocketUtils::Socket_RecvAPacket(SOCKET so, DWORD timeout, HANDLE stopEvent)
{
	PACKET_T    *pkg;
	PACKET_T    pkgHeader;

	if (Socket_ReadNBytes(so, (BYTE *)&pkgHeader, PACKET_HEADER_LEN, timeout, stopEvent) == FALSE)
	{
		return FALSE;
	}

	pkg = (PACKET_T *)malloc(pkgHeader.len);
	if (pkg == NULL)
	{
		L_FATAL(_T("malloc fail\n"));
		return FALSE;
	}
	if (pkgHeader.len > PACKET_HEADER_LEN)
	{
		if (Socket_ReadNBytes(so, (BYTE *)pkg + PACKET_HEADER_LEN, pkgHeader.len - PACKET_HEADER_LEN, timeout, stopEvent) == FALSE)
		{
			free(pkg);
			return FALSE;
		}
	}
	memcpy(pkg, &pkgHeader, PACKET_HEADER_LEN);

	return pkg;
}

BOOL CSocketUtils::Socket_SendAPacket(SOCKET so, void *data, int datalen,
	DWORD type, DWORD timeout, HANDLE stopEvent)
{
	PACKET_T pkgHeader;

	memset(&pkgHeader, 0, sizeof(PACKET_T));
	pkgHeader.len = PACKET_HEADER_LEN + datalen;
	pkgHeader.type = type;
	if (Socket_WriteNBytes(so, (BYTE *)&pkgHeader,
		PACKET_HEADER_LEN, timeout, stopEvent) == FALSE)
	{
		return FALSE;
	}
	if (data && datalen)
	{
		return Socket_WriteNBytes(so, (BYTE *)data, datalen, timeout, stopEvent);
	}
	else
	{
		return TRUE;
	}
}
