/**-----------------------------------------------------------------------------
 * @file     main.cpp
 *
 * @author   yangrz@centerm.com.cn
 *
 * @date     2011/2/2
 *
 * @brief    
 *
 * @version  
 *
 *----------------------------------------------------------------------------*/
#if 0
#define UNICODE
#define _UNICODE
#endif

#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <tchar.h>
#include "CTLogEx.h"

#define MODULE_NAME _T("CTLogEx_test")

TCHAR buf1[8192];
TCHAR buf2[8192];

int main(int argc, char *argv[])
{
    int i;
    int counts = 0;

#if 1
    CTLogEx_init(_T("ctlogex.ini"));
#else
    CTLogEx_init(NULL);
#endif
    CTLogEx_setLogFile(_T("c:\\1.log"));

    memset(buf1, 0, sizeof(buf1));
    memset(buf2, 0, sizeof(buf2));
    for ( i = 0; i < 5678; i++ )
    {
        buf1[i] = _T('a');
    }
    for ( i = 0; i < 2048; i++ )
    {
        buf2[i] = _T('b');
    }

    while ( counts < 100 )
    {
#if 1
        L_INFO(_T("%s:%s\n"), buf1, buf2);
#endif

        L_DEBUG(_T("--- debug1 ----\n"));
        L_INFO(_T("--- info ----\n"));
        L_WARN(_T("--- warn ----\n"));
        L_ERROR(_T("--- error ----\n"));
        L_FATAL(_T("--- fatal ----\n"));
        L_DUMP((BYTE *)buf1, 65);

        counts++;
    }

    system("pause");

    CTLogEx_done();

    return 0;
}