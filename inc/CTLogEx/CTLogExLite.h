/**-----------------------------------------------------------------------------
 * @file     CTLogExLite.h
 *
 * @author   yangrz@centerm.com.cn
 *
 * @date     2011/8/30
 *
 * @brief    CTLogEx�ļ򻯰汾��
 *           ���������ƣ�
 *           1. inline����ʵ�֣������Ӷ����ƵĴ�С��
 *           2. ��֧�ִ�ini�ļ���̬��ȡ���ã�
 *           3. ���֧��2048���ַ�;
 *           4. ��֧�����������ʽ;
 *           5. ��֧��������ļ�
 *
 *           �������ŵ�:
 *           1. ����Ҫ��ʼ��, ����ͷ�ļ�����ʹ�ã��������Ӷ���Ŀ�, �ʺ���ʱ�����Լ�Сģ�顣
 *           2. ����ͨ��
 *              #define CTLOGEX_LEVEL 0 
 *              �ر�log
 *              ������Ϣ���ᱻ���뵽�������С�
 *
 *           ʹ�÷�����
 *           // ����ģ�����ƣ�Ĭ��"default"
 *           #define MODULE_NAME "CTLOGEX"
 *           // ���õ��Եȼ�, Ĭ��LOG_LEVEL_TRACE
 *           #define CTLOGEX_LEVEL  0 // ����log
 *           // ���������ʽ, windows Ĭ�ϵ��Կڣ�Linux Ĭ�ϱ�׼����
 *           #define CTLOGEX_OUTPUT  LOG_OUTPUT_STDERR
 *           // ����DUMP��Ĭ�Ͽ���
 *           #define CTLOGEX_DUMP    0 
 *           // ʹ�ø߾���ʱ��, Ĭ�Ϲر�
 *           #define USE_HIGH_PERFORMANCE_COUNTER 1
 *
 *           #include "CTLogExLite.h"
 *
 * @version  
 *          v1.00 ��ʼ��
 *
 *----------------------------------------------------------------------------*/
#ifndef __CTLOGEX_H__
#define __CTLOGEX_H__

#ifndef CTLOGEX_LEVEL 
# define CTLOGEX_LEVEL LOG_LEVEL_TRACE
#endif

#ifndef MODULE_NAME
# define MODULE_NAME _T("default")
#endif

#ifndef CTLOGEX_OUTPUT 
# ifdef _WIN32
#  define CTLOGEX_OUTPUT LOG_OUTPUT_DBGPORT
# else
#  define CTLOGEX_OUTPUT LOG_OUTPUT_STDERR
# endif
#endif

#ifndef CTLOGEX_DUMP
# define CTLOGEX_DUMP 1
#endif

#ifdef _WIN32 /* windows */
#include <windows.h>
#include <tchar.h>

# define CTLOGAPI __cdecl

# ifndef __cplusplus
#  define inline 
# endif

#else        /* Linux */
# define CTLOGAPI

# ifndef _T
#  define _T(x) x
# endif

# ifndef TCHAR
#  define TCHAR char
# endif

# ifndef _ftprintf
#  define _ftprintf   fprintf
# endif

# ifndef _sntprintf
#  define _sntprintf  snprintf
# endif

# ifndef _vsntprintf     
#  define _vsntprintf vsnprintf
# endif 

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <unistd.h>
#include <time.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/time.h>

#endif

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

#ifndef __GNUC__
# define __attribute__(x) /*NOTHING*/
#endif

#if defined(_MSC_VER) && defined(_PREFAST_)
# include <sal.h>
#else
# ifndef _Printf_format_string_
#  define _Printf_format_string_
# endif
#endif

/**
 * @brief ���Եȼ�
 */
#define LOG_LEVEL_NONE      0   /**< �ر�log */
#define LOG_LEVEL_FATAL     1   /**< �������� */
#define LOG_LEVEL_ERROR     2   /**< ���� */
#define LOG_LEVEL_WARN      3   /**< ���� */
#define LOG_LEVEL_INFO      4   /**< ��Ϣ */
#define LOG_LEVEL_DEBUG     5   /**< ���� */
#define LOG_LEVEL_TRACE     6   /**< ���� */
#define LOG_TYPE_DUMP       100 /**< DUMP��Ϣ */
#define LOG_TYPE_TMP        101 /**< ��ʱ���� */

/**
 * @brief log���������
 */
enum log_output_type_t
{
    LOG_OUTPUT_DISABLE  = 0x0, /* ����log */
    LOG_OUTPUT_STDERR   = 0x1, /* ���������̨ */
    LOG_OUTPUT_DBGPORT  = 0x2, /* �����Windows���Կ� */
    LOG_OUTPUT_FILE     = 0x3, /* ������ļ� */
};

// ����log��Ĭ�������ʽ
#define TRACE_DEFAULT_FMT _T("%d %T %p(%M)(%f) %m")
// ����log��Ĭ�������ʽ
#define DEBUG_DEFAULT_FMT _T("%d %T %p(%M) %m")
// ��Ϣlog��Ĭ�������ʽ
#define INFO_DEFAULT_FMT  _T("%d %T %p(%M) %m")
// ����log��Ĭ�������ʽ
#define WARN_DEFAULT_FMT  _T("%d %T %p(%M)(%f) %m")
// ����log��Ĭ�������ʽ
#define ERROR_DEFAULT_FMT _T("%d %T %p(%M)(%f:%L) %m")
// ��������log��Ĭ�������ʽ
#define FATAL_DEFAULT_FMT _T("%d %T %p(%M)(%f:%L) %m")
// DUMP���������ʽ
#define DUMP_DEFAULT_FMT  _T("%d %T %p(%M)(%f) %m")
// ��ʱ������Ϣ�������ʽ
#define TMP_DEFAULT_FMT _T("%d %T %p(%M)(%f:%L) %m")

#ifdef _UNICODE
#define CHARSTRING_FMT L"%S"
#else
#define CHARSTRING_FMT "%s"
#endif
#define MAX_CONF_BUF_SIZE 2048

typedef struct _MYSYSTEMTIME {
    unsigned short wYear;
    unsigned short wMonth;
    unsigned short wDayOfWeek;
    unsigned short wDay;
    unsigned short wHour;
    unsigned short wMinute;
    unsigned int   dwSecond;
    unsigned int   dwUs;
} MYSYSTEMTIME, *PMYSYSTEMTIME;

#ifdef _WIN32
static inline unsigned long 
Log_CurrentThreadID()
{
    return GetCurrentThreadId();
}

static inline void 
Log_OsGetTime(MYSYSTEMTIME *time)
{
#if USE_HIGH_PERFORMANCE_COUNTER
    LARGE_INTEGER freq;
    LARGE_INTEGER cur;
    ULONGLONG us, s, min, hour;

    memset(time, 0, sizeof(SYSTEMTIME));

    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&cur);

    us = cur.QuadPart * 1000000LL / freq.QuadPart;

    time->dwUs = us % 1000000LL;
    s = us / 1000000LL;
    time->dwSecond = s % 60LL;
    min = s / 60LL;
    time->wMinute = min % 60LL;
    hour = min / 60LL;
    time->wHour = hour % 24LL;
#else
    SYSTEMTIME t;

    GetLocalTime(&t);
    time->wYear      = t.wYear;
    time->wMonth     = t.wMonth;
    time->wDayOfWeek = t.wDayOfWeek;
    time->wDay       = t.wDay;
    time->wHour      = t.wHour;
    time->wMinute    = t.wMinute;
    time->dwSecond   = t.wSecond;
    time->dwUs       = t.wMilliseconds * 1000;
#endif
}

/*
 * VC�� sprintf�ռ䲻��ʱ����-1
 */
#define ADD_WICH_CHECK(pos, ret, count)                  \
    {                                               \
        if ( (ret) >=0 )                            \
            (pos) += (ret);                         \
        else                                        \
            (pos) += (count);                       \
    }

#else
static inline unsigned long 
Log_CurrentThreadID()
{
    return syscall(SYS_gettid);
}

static inline void 
Log_OsGetTime(MYSYSTEMTIME *time)
{
    struct timeval tv;
    struct tm tmp;

    gettimeofday(&tv, NULL);
    localtime_r(&(tv.tv_sec), &tmp);

    time->wYear      = tmp.tm_yday;
    time->wMonth     = tmp.tm_mon;
    time->wDayOfWeek = tmp.tm_wday;
    time->wDay       = tmp.tm_mday;
    time->wHour      = tmp.tm_hour;
    time->wMinute    = tmp.tm_min;
    time->dwSecond   = tmp.tm_sec;
    time->dwUs = tv.tv_usec;
}

/*
 * gcc sprintf�ռ䲻��ʱ������Ҫ���ֽ���
 */
#define ADD_WICH_CHECK(pos, ret, count)             \
    {                                               \
        if ( (ret) <= count )                       \
            (pos) += (ret);                         \
        else                                        \
            (pos) += (count);                       \
    }

#endif

#ifdef _WIN32
# pragma warning(push)
/*  C4996: '_CRT_SECURE_NO_WARNINGS. */
# pragma warning(disable: 4996)
#endif

static inline void
CTLogEx_puts(TCHAR *out_str, enum log_output_type_t out_type)
{
#ifdef _WIN32
    if ( out_type == LOG_OUTPUT_DBGPORT )
    {
        OutputDebugString(out_str);
    }
    else
#endif
    {
        _ftprintf(stderr, _T("%s"), out_str);
    }
}

static inline void CTLOGAPI 
CTLogEx_printf(const TCHAR *module_name, 
               int loglevel, 
               const char *function_name,
               const char *file_name,
               int line,
               _Printf_format_string_ const TCHAR *pszFormat, ...)
{
    TCHAR           buf[MAX_CONF_BUF_SIZE + 128];
    TCHAR           *out = buf;
    int             max_out_chars = MAX_CONF_BUF_SIZE;
    TCHAR          *out_p;

    /* 
     * ���������ʽ 
     */
    {
        const TCHAR *p;
        const TCHAR *prefix;

        switch ( loglevel )
        {
            case LOG_LEVEL_TRACE:
                prefix = _T("TRACE");
                p = TRACE_DEFAULT_FMT;
                break;
            case LOG_LEVEL_DEBUG:
                prefix = _T("DEBUG");
                p = DEBUG_DEFAULT_FMT;
                break;
            case LOG_LEVEL_INFO:
                prefix = _T("_INFO");
                p = INFO_DEFAULT_FMT;
                break;
            case LOG_LEVEL_WARN:
                prefix = _T("_WARN");
                p = WARN_DEFAULT_FMT;
                break;
            case LOG_LEVEL_ERROR:
                prefix = _T("ERROR");
                p = ERROR_DEFAULT_FMT;
                break;
            case LOG_LEVEL_FATAL:
                prefix= _T("FATAL");
                p = FATAL_DEFAULT_FMT;
                break;
            case LOG_TYPE_DUMP:
                prefix = _T("_DUMP");
                p = DUMP_DEFAULT_FMT;
                break;
            case LOG_TYPE_TMP:
                prefix = _T("_TEMP");
                p = TMP_DEFAULT_FMT;
                break;
            default:
                return;
        }

        out_p = out;
        while ( *p )
        {
            int b_match = 0;
            /*
             * %F ��ʾ����logger��Դ�ļ���
             * %L ��ʾ����logger�Ĵ�����
             * %m ��ʾ�����Ϣ
             * %M ģ������
             * %f ��ʾ����logger�ĺ�����
             * %p ��ʾ������־�����ȼ�
             * %d ��ʾ��־��¼ʱ��
             * %P logǰ׺����log_prefix����
             * %T �߳�ID
             */
            if ( *p == _T('%') )
            {
                int ret;
                int count;

                b_match = 1;
                switch ( *(p + 1) )
                {
                    case _T('F'):
                        count = max_out_chars - (int)(out_p - out);
                        ret = _sntprintf(out_p, count, CHARSTRING_FMT, file_name);
                        ADD_WICH_CHECK(out_p, ret, count);
                        break;

                    case _T('L'):
                        count = max_out_chars - (int)(out_p - out);
                        ret = _sntprintf(out_p, count, _T("%d"), line);
                        ADD_WICH_CHECK(out_p, ret, count);
                        break;

                    case _T('T'):
                        count = max_out_chars - (int)(out_p - out);
                        ret = _sntprintf(out_p, count, _T("%08ld"), Log_CurrentThreadID());
                        ADD_WICH_CHECK(out_p, ret, count);
                        break;

                    case _T('m'):
                    {
                        va_list args;

                        va_start(args, pszFormat);
                        count = max_out_chars - (int)(out_p - out);
                        ret = _vsntprintf(out_p, count, pszFormat, args);
                        ADD_WICH_CHECK(out_p, ret, count);
                        va_end(args);
                        break;
                    }

                    case _T('M'):
                        count = max_out_chars - (int)(out_p - out);
                        ret = _sntprintf(out_p, count, _T("%s"), module_name);
                        ADD_WICH_CHECK(out_p, ret, count);
                        break;

                    case _T('f'):
                        count = max_out_chars - (int)(out_p - out);
                        ret = _sntprintf(out_p, count, CHARSTRING_FMT, function_name);
                        ADD_WICH_CHECK(out_p, ret, count);
                        break;

                    case _T('p'):
                    {
                        count = max_out_chars - (int)(out_p - out);
                        ret = _sntprintf(out_p, count, _T("%s"), prefix);
                        ADD_WICH_CHECK(out_p, ret, count);
                        break;
                    }

                    case _T('d'):
                    {
                        MYSYSTEMTIME time;
                        TCHAR time_buf[128];

                        Log_OsGetTime(&time);
                        _sntprintf(time_buf, sizeof(time_buf)/sizeof(TCHAR), 
                                   _T("%02d:%02d:%02d.%06d"), time.wHour, time.wMinute, 
                                   time.dwSecond, time.dwUs);
                        count = max_out_chars - (int)(out_p - out);
                        ret = _sntprintf(out_p, count, _T("%s"), time_buf);
                        ADD_WICH_CHECK(out_p, ret, count);
                        break;
                    }

                    case _T('P'):
                        count = max_out_chars - (int)(out_p - out);
                        ret = _sntprintf(out_p, count, _T("%s"), prefix);
                        ADD_WICH_CHECK(out_p, ret, count);
                        break;

                    default:
                        b_match = 0;
                        break;
                }
                if ( b_match )
                {
                    p += 2;
                }
            }
            if ( !b_match )
            {
                *out_p = *p;
                out_p++;
                p++;
            }
        }
        *out_p = 0;
    }

    /*
     * д��log��Ϣ
     */
    CTLogEx_puts(out, CTLOGEX_OUTPUT);
}

#define NB_BYTE 16
static inline void 
CTLOGAPI CTLogEx_dump(const TCHAR *module_name, const char *function_name, const char *file_name, int line, 
                      const void *buf1, int len)
{
    unsigned char  *buf = (unsigned char *)buf1;
    char            str[NB_BYTE*10 + 1];
    int             i,j;
    char            c;
    TCHAR           msg[256];

    CTLogEx_printf(module_name, LOG_TYPE_DUMP, function_name, file_name, line, 
                   _T("---------- Begin Dump %d bytes -------------\n"), len);
    for ( i = 0; i < len; i += NB_BYTE )
    {
        char * p = str;

        memset( str, 0x20, NB_BYTE*10 +1 );

        // Show Hex 
        for ( j = i; j < len && j < i + NB_BYTE; j++ )
        {

            *p++ = ' ';
            c = (buf[j] >> 4) & 0xf;
            *p++ = (c<10) ? c+'0' : c-10+'a';
            c = buf[j] & 0xf;
            *p++ = (c<10) ? c+'0' : c-10+'a';
        }

        // PADs for not 16 bytes align
        p +=  (NB_BYTE - (j - i)) * 3 + 2;

        // Show ASCII
        for ( j = i; j < len && j < i + NB_BYTE; j++ )
        {
            c = buf[j];
            if ( ( c < 0x7F ) && ( c > 0x1F) )
            {
                *p++ = c;
            } else
                *p++ = '.';             
        }

        *p = 0;

        _sntprintf(msg, sizeof(msg)/sizeof(msg[0]), _T("\t%04x:") CHARSTRING_FMT _T("\n"), i, str);
        CTLogEx_puts(msg, CTLOGEX_OUTPUT);
    }
    CTLogEx_printf(module_name, LOG_TYPE_DUMP, function_name, file_name, line, 
                   _T("-------------- End Dump --------------\n"));
}

/**
 * @brief ���ٵȼ���־���
 *
 * @param[in] fmt ��ʽ���ַ���
 */
#if (CTLOGEX_LEVEL >= LOG_LEVEL_TRACE)
#define L_TRACE(...) CTLogEx_printf(MODULE_NAME, LOG_LEVEL_TRACE, __FUNCTION__, __FILE__, __LINE__, __VA_ARGS__);
#else
#define L_TRACE(...)
#endif
#define L_TRACE_ENTER() L_TRACE(_T("Enter\n"))
#define L_TRACE_LEAVE() L_TRACE(_T("Leave\n"))

/**
 * @brief ���Եȼ���־���
 *
 * @param[in] fmt ��ʽ���ַ���
 */
#if (CTLOGEX_LEVEL >= LOG_LEVEL_DEBUG)
#define L_DEBUG(...) CTLogEx_printf(MODULE_NAME, LOG_LEVEL_DEBUG, __FUNCTION__, __FILE__, __LINE__, __VA_ARGS__);
#else
#define L_DEBUG(...)
#endif

/**
 * @brief ��Ϣ�ȼ���־���
 *
 * @param[in] fmt ��ʽ���ַ���
 */
#if (CTLOGEX_LEVEL >= LOG_LEVEL_INFO)
#define L_INFO(...)  CTLogEx_printf(MODULE_NAME, LOG_LEVEL_INFO,  __FUNCTION__, __FILE__, __LINE__, __VA_ARGS__);
#else
#define L_INFO(...)
#endif

/**
 * @brief ����ȼ���־���
 *
 * @param[in] fmt ��ʽ���ַ���
 */
#if (CTLOGEX_LEVEL >= LOG_LEVEL_WARN)
#define L_WARN(...)  CTLogEx_printf(MODULE_NAME, LOG_LEVEL_WARN,  __FUNCTION__, __FILE__, __LINE__, __VA_ARGS__);
#else
#define L_WARN(...)
#endif
/**
 * @brief ����ȼ���־���
 *
 * @param[in] fmt ��ʽ���ַ���
 */
#if (CTLOGEX_LEVEL >= LOG_LEVEL_ERROR)
#define L_ERROR(...) CTLogEx_printf(MODULE_NAME, LOG_LEVEL_ERROR, __FUNCTION__, __FILE__, __LINE__, __VA_ARGS__);
#else
#define L_ERROR(...)
#endif

/**
 * @brief ��������ȼ���־���
 *
 * @param[in] fmt ��ʽ���ַ���
 */
#if (CTLOGEX_LEVEL >= LOG_LEVEL_FATAL)
#define L_FATAL(...) CTLogEx_printf(MODULE_NAME, LOG_LEVEL_FATAL, __FUNCTION__, __FILE__, __LINE__, __VA_ARGS__);
#else
#define L_FATAL(...)
#endif

/**
 * @brief ��ʱ���Ժ� \n
 * ��һ������Ϣǿ�������ֻ������ʱ����, �ύ��svn�İ汾����������һ����
 *
 * @param[in] fmt ��ʽ���ַ���
 */
#define L_TEMP(...) CTLogEx_printf(MODULE_NAME, LOG_TYPE_TMP, __FUNCTION__, __FILE__, __LINE__, __VA_ARGS__);

/**
 * @brief dump���ݿ�
 *
 * @param[in] buf         ���ݵ�ַ
 * @param[in] len         ���ݿ��С
 */
#if CTLOGEX_DUMP
#define L_DUMP(buf, len) CTLogEx_dump(MODULE_NAME, __FUNCTION__, __FILE__, __LINE__, buf, len);
#else
#define L_DUMP(buf, len)
#endif

/**
 * @brief dump����, ֻ������ʱ����, �ύ��svn�İ汾������ʹ��
 *
 * @param[in] buf         ���ݵ�ַ
 * @param[in] len         ���ݿ��С
 */
#define L_TMPDUMP(buf, len) CTLogEx_dump(MODULE_NAME, __FUNCTION__, __FILE__, __LINE__, buf, len);

#ifdef _WIN32
# pragma warning(pop)
#endif

#endif
