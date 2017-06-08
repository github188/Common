/**-----------------------------------------------------------------------------
 * @file     CTLogEx.h
 *
 * @author   yangrz@centerm.com.cn
 *
 * @date     2011/1/31
 *
 * @brief    logϵͳ�����ӿ�\n
 *           ����ģ��ʹ����һЩ�ӿ����log��Ϣ 
 *
 * @version  
 *
 *----------------------------------------------------------------------------*/
#ifndef __CTLOGEX_H__
#define __CTLOGEX_H__

#ifdef _WIN32
# define CTLOGAPI __cdecl
#else
# define CTLOGAPI
# ifndef _T
#  define _T(x) x
# endif
#endif

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
enum log_level_t
{
    LOG_LEVEL_FATAL = 1, /**< �������� */
    LOG_LEVEL_ERROR = 2, /**< ���� */
    LOG_LEVEL_WARN  = 3, /**< ���� */
    LOG_LEVEL_INFO  = 4, /**< ��Ϣ */
    LOG_LEVEL_DEBUG = 5, /**< ���� */
    LOG_LEVEL_TRACE = 6, /**< ���� */

    LOG_TYPE_DUMP   = 100, /**< DUMP��Ϣ */
    LOG_TYPE_TMP    = 101, /**< ��ʱ���� */
};

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

/**
 * @brief ��������ļ��仯
 */
#define CTLOG_INIT_FLAGS_MONITOR 0x1

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief ��ʼ��logϵͳ
 *
 * @param[in] config_file               �����ļ�·��\n
 *                                      �����һ��������NULL���������ļ������ڣ���ʹ��Ĭ��ֵ
 * @param[in] flags                     ��CTLOG_INIT_FLAGS_XXXXX ����
 *
 * @return �ɹ� ��0\n
 *         ʧ�� 0
 */
#ifdef _WIN32
int CTLOGAPI CTLogEx_initW(const wchar_t *config_file);
int CTLOGAPI CTLogEx_initExW(const wchar_t *config_file, int flags);
#endif
int CTLOGAPI CTLogEx_initA(const char *config_file);
int CTLOGAPI CTLogEx_initExA(const char *config_file, int flags);
#ifdef _UNICODE 
#define CTLogEx_init CTLogEx_initW
#define CTLogEx_initEx CTLogEx_initExW
#else
#define CTLogEx_init CTLogEx_initA
#define CTLogEx_initEx CTLogEx_initExA
#endif

/**
 * @brief ����logϵͳ�ַ������Ͳ��� 
 *
 * @param[in] modulename ģ������"GLOBAL"��ʾȫ��(Ĭ��)����
 * @param[in] filedname  ����
 * @param[in] value      ֵ
 */
#ifdef _WIN32
void CTLOGAPI CTLogEx_SetOptStrW(const wchar_t *modulename, 
                                 const wchar_t *filedname, 
                                 const wchar_t *value);
#endif
void CTLOGAPI CTLogEx_SetOptStrA(const char *modulename, 
                                 const char *filedname, 
                                 const char *value);
#ifdef _UNICODE 
#define CTLogEx_SetOptStr CTLogEx_SetOptStrW
#else
#define CTLogEx_SetOptStr CTLogEx_SetOptStrA
#endif

/**
 * @brief ����logϵͳint���Ͳ��� 
 *
 * @param[in] modulename ģ������"GLOBAL"��ʾȫ��(Ĭ��)����
 * @param[in] filedname  ����
 * @param[in] value      ֵ
 */
#ifdef _WIN32
void CTLOGAPI CTLogEx_SetOptIntW(const wchar_t *modulename, 
                                 const wchar_t *filedname, 
                                 int value);
#endif
void CTLOGAPI CTLogEx_SetOptIntA(const char *modulename, 
                                 const char *filedname, 
                                 int value);
#ifdef _UNICODE 
#define CTLogEx_SetOptInt CTLogEx_SetOptIntW
#else
#define CTLogEx_SetOptInt CTLogEx_SetOptIntA
#endif

/**
 * @brief ����log�������
 *
 * @param[in] type �������
 * @see   log_output_type_t
 */
#define CTLogEx_setOutput(type) \
    CTLogEx_SetOptInt(_T("GLOBAL"), _T("log_output_type"), (log_output_type_t)(type));
/**
 * @brief ����log����ļ���
 *
 * @param[in] filename
 */
#define CTLogEx_setLogFile(filename) \
    { CTLogEx_SetOptStr(_T("GLOBAL"), _T("log_output_filepath"), (filename));  \
      CTLogEx_SetOptInt(_T("GLOBAL"), _T("log_output_type"),     LOG_OUTPUT_FILE); }

/**
 * @brief ����/����DUMP
 *
 * @param filename
 */
#define CTLogEx_enableDump(bEnable) \
    CTLogEx_SetOptInt(_T("GLOBAL"), _T("log_dump"), bEnable);

/**
 * @brief ����logĬ�ϵȼ�
 *
 * @param[in] level log�ȼ�
 * @see log_level_t
 */
#define CTLogEx_setLogLevel(level) \
    CTLogEx_SetOptInt(_T("GLOBAL"), _T("log_level"), (log_level_t)(level));

/**
 * @brief ������������
 */
void CTLOGAPI CTLogEx_reload();

#ifdef _WIN32
void CTLOGAPI CTLogEx_printfW(const wchar_t *module_name, 
                              log_level_t loglevel, 
                              const char *function_name,
                              const char *file_name,
                              int line,
                              _Printf_format_string_ const wchar_t *pszFormat, ...);
#endif
void CTLOGAPI CTLogEx_printfA(const char *module_name, 
                              enum  log_level_t loglevel, 
                              const char *function_name,
                              const char *file_name,
                              int line,
                              _Printf_format_string_ const char *pszFormat, ...)
                              __attribute__((__format__(__printf__,6,7)));
#ifdef _UNICODE 
#define CTLogEx_printf CTLogEx_printfW
#else
#define CTLogEx_printf CTLogEx_printfA
#endif

/**
 * @brief ���ٵȼ���־���
 *
 * @param[in] fmt ��ʽ���ַ���
 */
#define L_TRACE(...) CTLogEx_printf(MODULE_NAME, LOG_LEVEL_TRACE, __FUNCTION__, __FILE__, __LINE__, __VA_ARGS__);
#define L_TRACE_ENTER() L_TRACE(_T("Enter\n"))
#define L_TRACE_LEAVE() L_TRACE(_T("Leave\n"))
#define L_TRACE_EXIT() L_TRACE(_T("Exit\n"))

/**
 * @brief ���Եȼ���־���
 *
 * @param[in] fmt ��ʽ���ַ���
 */
#define L_DEBUG(...) CTLogEx_printf(MODULE_NAME, LOG_LEVEL_DEBUG, __FUNCTION__, __FILE__, __LINE__, __VA_ARGS__);
/**
 * @brief ��Ϣ�ȼ���־���
 *
 * @param[in] fmt ��ʽ���ַ���
 */
#define L_INFO(...)  CTLogEx_printf(MODULE_NAME, LOG_LEVEL_INFO,  __FUNCTION__, __FILE__, __LINE__, __VA_ARGS__);
/**
 * @brief ����ȼ���־���
 *
 * @param[in] fmt ��ʽ���ַ���
 */
#define L_WARN(...)  CTLogEx_printf(MODULE_NAME, LOG_LEVEL_WARN,  __FUNCTION__, __FILE__, __LINE__, __VA_ARGS__);
/**
 * @brief ����ȼ���־���
 *
 * @param[in] fmt ��ʽ���ַ���
 */
#define L_ERROR(...) CTLogEx_printf(MODULE_NAME, LOG_LEVEL_ERROR, __FUNCTION__, __FILE__, __LINE__, __VA_ARGS__);
/**
 * @brief ��������ȼ���־���
 *
 * @param[in] fmt ��ʽ���ַ���
 */
#define L_FATAL(...) CTLogEx_printf(MODULE_NAME, LOG_LEVEL_FATAL, __FUNCTION__, __FILE__, __LINE__, __VA_ARGS__);


/**
 * @brief ��ʱ���Ժ� \n
 * ��һ������Ϣǿ�������ֻ������ʱ����, �ύ��svn�İ汾����������һ����
 *
 * @param[in] fmt ��ʽ���ַ���
 */
#define L_TEMP(...) CTLogEx_printf(MODULE_NAME, LOG_TYPE_TMP, __FUNCTION__, __FILE__, __LINE__, __VA_ARGS__);

#ifdef _WIN32
void CTLOGAPI CTLogEx_dumpW(const wchar_t *module_name, int bForce, const char *function_name, const char *file_name, int line, 
                            const void *buf, int len);
#endif
void CTLOGAPI CTLogEx_dumpA(const char *module_name, int bForce, const char *function_name, const char *file_name, int line, 
                            const void *buf, int len);
#ifdef _UNICODE 
#define CTLogEx_dump CTLogEx_dumpW
#else
#define CTLogEx_dump CTLogEx_dumpA
#endif

/**
 * @brief dump���ݿ�
 *
 * @param[in] buf         ���ݵ�ַ
 * @param[in] len         ���ݿ��С
 */
#define L_DUMP(buf, len) CTLogEx_dump(MODULE_NAME, 0, __FUNCTION__, __FILE__, __LINE__, buf, len);

/**
 * @brief dump����, ֻ������ʱ����, �ύ��svn�İ汾������ʹ��
 *
 * @param[in] buf         ���ݵ�ַ
 * @param[in] len         ���ݿ��С
 */
#define L_TMPDUMP(buf, len) CTLogEx_dump(MODULE_NAME, 1, __FUNCTION__, __FILE__, __LINE__, buf, len);

/**
 * @brief ����logϵͳ
 */
void CTLOGAPI CTLogEx_done(void);

#ifdef __cplusplus
}
#endif

#endif
