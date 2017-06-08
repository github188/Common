/**-----------------------------------------------------------------------------
 * @file     CTLogEx_OsWin.h
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
#ifndef __CTLOGEX_OS_WIN_H__
#define __CTLOGEX_OS_WIN_H__

#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <string.h>
#include <string>

typedef std::wstring tstring;

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
void Log_OsShowError(TCHAR *fmt, ...);
/*
 * ��
 */
void Log_OsLock();
void Log_OsUnlock();

/*
 * ��ȡ�߳�ID
 */
unsigned long Log_CurrentThreadID();


#endif
