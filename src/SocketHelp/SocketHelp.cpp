#include "stdafx.h"
#include "SocketHelp.h"
#include <stdio.h>

#ifndef WIN32
#include <errno.h>
#include <sys/param.h>

#endif
SOCKET CreateListenSocket(int port)
{
	DWORD dwError;
	sockaddr_in sin;
	DWORD  portvalue = port;
	WSADATA WSAData ;
    int on = 1;
	dwError = 0;

#ifdef WIN32
	// initiates use of WS2_32.DLL by a process
	WSAStartup (MAKEWORD(2,0), &WSAData);
#endif

	SOCKET socket_listen = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if( socket_listen == INVALID_SOCKET )
	{
		return socket_listen;
	}

	portvalue = port;
	sin.sin_family = AF_INET;
	sin.sin_port = htons((unsigned short )portvalue);
	sin.sin_addr.s_addr = INADDR_ANY;
#ifndef WIN32

    setsockopt( socket_listen, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on) );
#endif
	if ( bind(socket_listen, (sockaddr*)&sin, sizeof(sin)) == SOCKET_ERROR )
	{
        printf("bind error \r\n");
		closesocket(socket_listen);
		socket_listen = INVALID_SOCKET;
	}

	return socket_listen;
}

#ifdef WIN32

SOCKET CreateTimeOutConnectSocket(char* ip,int port,int sec)
{
#ifdef _WIN32
	WSADATA WSAData ;
	// initiates use of WS2_32.DLL by a process
	WSAStartup (MAKEWORD(2,0), &WSAData);
#endif
	SOCKET s;
	sockaddr_in  si;
	int len;
	int ret;

	s = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(s == INVALID_SOCKET)
	{
		return s;
	}

	//设置非阻塞方式连接
	unsigned long ul = 1;

	ret = ioctlsocket(s, FIONBIO, (unsigned long*)&ul);
	if(ret==SOCKET_ERROR)
	{
		closesocket(s);
		s = INVALID_SOCKET;
		return s;
	}

	si.sin_family = AF_INET;
	si.sin_port = htons(port);
	si.sin_addr.s_addr = inet_addr(ip);
	len = sizeof(sockaddr_in);

	connect(s,(sockaddr*)&si,len);

	//select模型，即设置超时
	struct timeval timeout ;
	fd_set r; 


	FD_ZERO(&r);
	FD_SET(s,&r);
	timeout.tv_sec = sec; //连接超时15秒
	timeout.tv_usec =0;
	ret = select(0, 0,&r, 0,&timeout);

	if ( ret<= 0 )
	{
		closesocket(s);
		s = INVALID_SOCKET;
		return s;
	}
	else
	{
		unsigned long ul1= 0 ;

		ret = ioctlsocket(s, FIONBIO, (unsigned long*)&ul1);
		if(ret==SOCKET_ERROR)
		{
			closesocket(s);
			s = INVALID_SOCKET;
			return s;
		}


	}
	return s;
}



BOOL SocketRead(SOCKET s,BYTE* pBuffer,DWORD dwBufferSize,DWORD* pdwReaded,HANDLE hStopEvent)
{
	BOOL bRet = TRUE;
	int rc;
	int err = 0;
	DWORD RecvBytes = 0, dwRet,Flags;
	WSABUF DataBuf;
	WSAOVERLAPPED RecvOverlapped;
	SecureZeroMemory((PVOID) & RecvOverlapped, sizeof (WSAOVERLAPPED));
	RecvOverlapped.hEvent = WSACreateEvent();

	DataBuf.len = dwBufferSize;
	DataBuf.buf = (CHAR*)pBuffer;
	while (1)
	{
		HANDLE hEvents[2] = {RecvOverlapped.hEvent,hStopEvent};
		Flags = 0;
		rc = WSARecv(s, &DataBuf, 1, &RecvBytes, &Flags, &RecvOverlapped, NULL);
    	if ((rc == SOCKET_ERROR) && (WSA_IO_PENDING != (err = WSAGetLastError()))) 
		{
			printf("WSARecv failed with error: %d\n", err);
			bRet = FALSE;
			break;
		}

		dwRet = WaitForMultipleObjects(2,hEvents,FALSE,INFINITE);
		if (dwRet == WAIT_OBJECT_0)
		{
			rc = WSAGetOverlappedResult(s, &RecvOverlapped, &RecvBytes, TRUE, &Flags);
        	if (rc == FALSE) 
			{
				printf("WSARecv operation failed with error: %d\n", WSAGetLastError());
				bRet = FALSE;
				break;
			}
            if(RecvBytes == 0)
            {
                // connection is closed
                bRet = FALSE;
                break;
            }
			break;
		}
		else
		{
			if (dwRet == WAIT_OBJECT_0 + 1)
			{
				printf("get exit event \r\n");
			}

			WSASetEvent(RecvOverlapped.hEvent);
			rc = WSAGetOverlappedResult(s,&RecvOverlapped,&RecvBytes,TRUE,&Flags);
			bRet = FALSE;
			break;
		}
	}
	WSACloseEvent(RecvOverlapped.hEvent);

	*pdwReaded = RecvBytes;

	return bRet;
}
BOOL SocketWrite(SOCKET s,BYTE* pBuffer,DWORD dwBufferSize,DWORD* pdwWritten,HANDLE hStopEvent)
{
	WSAOVERLAPPED SendOverlapped;
	WSABUF DataBuf;
	DWORD SendBytes = 0;
	int err = 0;
	int rc;
	HANDLE hEvents[2] = {0};

	DWORD dwRet;
	DWORD Flags = 0;
	BOOL bRet = FALSE;

	SecureZeroMemory((PVOID) & SendOverlapped, sizeof (WSAOVERLAPPED));
	SendOverlapped.hEvent = WSACreateEvent();

	hEvents[0] = SendOverlapped.hEvent;
	hEvents[1] = hStopEvent;

	if (SendOverlapped.hEvent == NULL)
	{
		printf("create wsaevent fail \r\n");
		return FALSE;
	}



	DataBuf.len = dwBufferSize;
	DataBuf.buf = (char*)pBuffer;

	do 
	{
		rc = WSASend(s, &DataBuf, 1,
			&SendBytes, 0, &SendOverlapped, NULL);
		if (rc == 0)
		{
			bRet = TRUE;
		}
		
		if ((rc == SOCKET_ERROR) &&
			(WSA_IO_PENDING != (err = WSAGetLastError()))) {
				printf("WSASend failed with error: %d\n", err);
				break;
		}

		dwRet = WaitForMultipleObjects(2,hEvents,FALSE,INFINITE);

		if (dwRet == WAIT_OBJECT_0)
		{
			if (WSAGetOverlappedResult(s, &SendOverlapped, &SendBytes,
				TRUE, &Flags))
			{
				bRet = TRUE;
			}
		}
		else
		{
			if (dwRet == WAIT_OBJECT_0 + 1)
			{
				printf("get exit event \t\n");

			}
			WSASetEvent(SendOverlapped.hEvent);

			WSAGetOverlappedResult(s, &SendOverlapped, &SendBytes,
				TRUE, &Flags);
		}

	} while (FALSE);

	WSACloseEvent(SendOverlapped.hEvent);
	*pdwWritten = SendBytes;

	return bRet;
}

#else

#include <execinfo.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <exception>
BOOL SocketRead(SOCKET s,BYTE* pBuffer,DWORD dwBufferSize,DWORD* pdwReaded,HANDLE hStopEvent)
{
	fd_set fdw = {0};

	int stopfd = 0;
	int n;
	int ret = 0;
	int dwReaded = 0;
	*pdwReaded = 0;

	if (s == -1)
	{
        return FALSE;
	}


	FD_ZERO(&fdw);

	FD_SET(s,&fdw);
	n = s + 1;
	if (hStopEvent)
	{
		stopfd  = GetEventFileDescriptor(hStopEvent);
		FD_SET(stopfd,&fdw);

		n = MAX(s,stopfd) + 1;
	}

	ret = select(n,&fdw,NULL,NULL,NULL);

	if (ret == 0)
	{
		printf("should not be here !!! \r\n");
		return FALSE;
	}
	else
	{
		if (ret == -1)
		{
			if (errno == EWOULDBLOCK)
			{
				*pdwReaded = 0;
				ret = 0;
				return TRUE;
			}
			else
			{
				printf("recv select error \r\n");
				return FALSE;
			}
		}
		else
		{
			if (FD_ISSET(stopfd,&fdw))
			{
				printf("recv get stop event \r\n");
				return FALSE;
			}
			else
			if (FD_ISSET(s,&fdw))
			{
				dwReaded = recv(s,pBuffer,dwBufferSize,0);
                *pdwReaded = dwReaded;

				if (dwReaded == 0)
				{
					*pdwReaded = 0;
					return FALSE;
				}
				else
				if (dwReaded == -1)
				{
					if (errno == EWOULDBLOCK)
					{
						*pdwReaded = 0;
						return TRUE;
					}

					*pdwReaded = 0;
					return FALSE;
				}
				else
				{
					return TRUE;
				}
			}
		}
	}
}

BOOL SocketWrite(SOCKET s,BYTE* pBuffer,DWORD dwBufferSize,DWORD* pdwWritten,HANDLE hStopEvent)
{
	fd_set fdw = {0};

	int stopfd = 0;
	int n;
	int ret = 0;
	int dwWrite = 0;
	*pdwWritten = 0;

	if (s == -1)
	{
        return FALSE;
	}

	FD_ZERO(&fdw);

	FD_SET(s,&fdw);
	n = s + 1;
	if (hStopEvent)
	{
		stopfd  = GetEventFileDescriptor(hStopEvent);
		FD_SET(stopfd,&fdw);

		n = MAX(s,stopfd) + 1;
	}

	ret = select(n,NULL,&fdw,NULL,NULL);

	if (ret == 0)
	{
		printf("should not be here !!! \r\n");
		return FALSE;
	}
	else
	{
		if (ret == -1)
		{
			if (errno == EWOULDBLOCK)
			{
				ret = 0;
				return TRUE;
			}
			else
			{
				printf("recv select error \r\n");
				return FALSE;
			}
		}
		else
		{
			if (FD_ISSET(stopfd,&fdw))
			{
				printf("recv get stop event \r\n");
				return FALSE;
			}
			else
				if (FD_ISSET(s,&fdw))
				{
                    signal(SIGPIPE,SIG_IGN);
                    try
                    {
					dwWrite = send(s,pBuffer,dwBufferSize,0);
                    *pdwWritten = dwWrite;

					if (dwWrite == 0)
					{
						return FALSE;
					}
					if (dwWrite == -1)
					{
						if (errno == EWOULDBLOCK)
						{
							*pdwWritten = 0;
							return TRUE;
						}
                        return FALSE;
					}
                    return TRUE;
                    }
                    catch (std::exception& e)
                    {
                        printf("send data error \r\n");
                        return FALSE;
                    }
				}
		}
	}
}
#endif
