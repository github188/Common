#ifndef __CTUTILS_H__
#define __CTUTILS_H__

#include <string.h>

/**
 * @brief ȡ�������С
 *
 * @param[in] a ָ��������
 *
 * @return �����С
 */
#ifndef ARRAYSIZE
#define ARRAYSIZE(a) sizeof(a)/sizeof((a)[0])
#endif

#ifndef MIN
#define MIN(a,b) ( (a) > (b) ? (b) : (a) )
#endif
#ifndef MAX
#define MAX(a,b) ( (a) > (b) ? (a) : (b) )
#endif

/**
 * @brief ���ֽ����г�ȡһ��WORD��ֵ
 *
 * @param[in]  p �ֽ���
 * @param[out] v ��ֵ
 */
#define C_VAL2(p, v) { (v) = (*((p))); (v) |= (*((p)+1)) << 8; }
/**
 * @brief ���ֽ����г�ȡһ��DWORD��ֵ
 *
 * @param[in]  p �ֽ���
 * @param[out] v ��ֵ
 */
#define C_VAL4(p, v) { unsigned int _v2; C_VAL2(p, _v2); C_VAL2(p+2, v); (v) = ((v) << 16) | _v2; }

/**
 * @brief ���Զ���
 */
#ifdef _DEBUG
#define CTASSERT(expr)                  \
    {                                   \
        if (!(expr))                    \
        {                               \
            L_FATAL(_T("assertion failed: (%S)\n"), #expr); \
            __asm { int 3 }            \
        }                              \
    }
#else
#define CTASSERT(expr)
#endif

/**
 * @brief �ڴ������
 */
#define CTMALLOC(ret, type, size)   \
    {                               \
        (ret) = (type)malloc(size); \
        if ( (ret) == NULL )        \
        {                           \
            L_FATAL(_T("malloc %d fail\n"), (size));    \
            exit(1);                                    \
        }                                               \
    }
#define CTMALLOC0(ret, type, size)      \
    {                                   \
        (ret) = (type)calloc(size, 1);  \
        if ( (ret) == NULL )            \
        {                               \
            L_FATAL(_T("malloc %d fail\n"), (size));    \
            exit(1);                                    \
        }                                               \
    }
#define CTREALLOC(ret, type, size)                      \
    {                                                   \
        type tmp;                                      \
        tmp = (type)realloc((ret), size);             \
        if ( tmp == NULL )                            \
        {                                               \
            L_FATAL(_T("realloc %d fail\n"), (size));   \
            exit(1);                                    \
        }                                               \
        (ret) = tmp;                                    \
    }
#define CTFREE_CHECK(p)         \
    {                           \
        if ( p )                \
        {                       \
            free(p);            \
            (p) = NULL;         \
        }                       \
    }
#define CTFREE(p) { free(p); }
#define CTSTRDUP(ret, str)                                  \
    {                                                       \
        size_t size = (_tcslen(str) + 1) * sizeof(TCHAR);   \
        (ret) = (TCHAR *)malloc(size);                  \
        if ( (ret) == NULL )                            \
        {                                               \
            L_FATAL(_T("strdup %d fail\n"), (size));    \
            exit(1);                                \
        }                                           \
        _tcscpy((ret), str);                        \
    }

typedef struct _VALUE_STRING_STRUCT
{
    ULONG value;
    const TCHAR *string;
}VALUE_STRING_STRUCT;
/**
 * @brief ��ȡָ��ֵ��Ӧ���ַ���
 *
 * @param vsarray   VALUE_STRING_STRUCT����
 * @param value     ֵ
 *
 * @return  �ɹ� ֵ��Ӧ���ַ���
 *          ʧ�� _T("unkown")
 */
static inline const TCHAR *
GetValueString(VALUE_STRING_STRUCT *vsarray, ULONG value)
{
    VALUE_STRING_STRUCT *tmp = vsarray;

    while ( tmp->string != NULL )
    {
        if ( tmp->value == value )
        {
            return tmp->string;
        }
        tmp++;
    }
    return _T("unkown");
}

#endif
