#ifdef WIN32
#include <WinSock2.h>
#endif

SOCKET CreateListenSocket(int port);
SOCKET CreateTimeOutConnectSocket(char* ip,int port,int sec);

BOOL SocketRead(SOCKET s,BYTE* pBuffer,DWORD dwBufferSize,DWORD* pdwReaded,HANDLE hStopEvent);
BOOL SocketWrite(SOCKET s,BYTE* pBuffer,DWORD dwBufferSize,DWORD* pdwWritten,HANDLE hStopEvent);