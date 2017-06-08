/**-----------------------------------------------------------------------------
 * @file     CTLogEx_OsUnix.h
 *
 * @author   yangrz@centerm.com.cn
 *
 * @date     2011-02-08
 *
 * @brief    
 *
 * @version  
 *
 *----------------------------------------------------------------------------*/
//#include "windef.h"
#include <winpr/wtypes.h>
#include <winpr/tchar.h>
#include <winpr/file.h>
//#include <tchar.h>
#include <string.h>
#include <tchar.h>
#include <string>
#include <errno.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>

typedef std::string tstring;

typedef struct _MYSYSTEMTIME {
    WORD wYear;
    WORD wMonth;
    WORD wDayOfWeek;
    WORD wDay;
    WORD wHour;
    WORD wMinute;
    DWORD dwSecond;
    DWORD dwUs;
} MYSYSTEMTIME, *PMYSYSTEMTIME;

/*
 * ����ϵͳ��ز��ֵĳ�ʼ��
 */
BOOL Log_OsInit(const TCHAR *confname, int flags);
void Log_OsDone();
/*
 * ��ȡ�ļ�·������ڿ�ִ�г��������·��
 */
tstring Log_OsGetFullPathNameRelativeExec(const TCHAR *filename);
/*
 * �ƶ��ļ�
 */
BOOL Log_OsMoveFile(const TCHAR *orgfilename, const TCHAR *newfilename);
/*
 * ��ȡϵͳʱ��
 */
void Log_OsGetTime(MYSYSTEMTIME *time, BOOL b_performance_counter);
/*
 * ��ʾlogģ��Ĵ�����Ϣ
 */
void Log_OsShowError(const TCHAR *fmt, ...);
/*
 * ��
 */
void Log_OsLock();
void Log_OsUnlock();

/*
 * ��ȡ�߳�ID
 */
unsigned long Log_CurrentThreadID();

UINT
GetPrivateProfileInt(
    const char *lpAppName,
    const char *lpKeyName,
    INT         nDefault,
    const char *lpFileName);

DWORD
GetPrivateProfileString(
    const char *lpAppName,
    const char *lpKeyName,
    const char *lpDefault,
    char       *lpReturnedString,
    DWORD      nSize,
    const char *lpFileName);
    
