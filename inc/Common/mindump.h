/**-----------------------------------------------------------------------------
 * @file     mindump.h
 *
 * @author   yangrz@centerm.com.cn
 *
 * @date     2011/8/10
 *
 * @brief    在应用程序崩溃是自动生成DUMP文件，方便跟踪。
 *
 * @version  
 *
 *----------------------------------------------------------------------------*/
#include <windows.h>
#include <tchar.h>
#include <shlobj.h>
#include <dbghelp.h>

#define DEFAULT_MAX_NUM_OF_DUMP_FILE 5

#define FILETIME_TO_LARGE_INTEGER(filetime, lg) { \
    (lg).LowPart  = (filetime).dwLowDateTime; \
    (lg).HighPart = (filetime).dwHighDateTime; }

typedef BOOL (WINAPI *MiniDumpWriteDumpFun)(
    HANDLE hProcess,
    DWORD ProcessId,
    HANDLE hFile,
    MINIDUMP_TYPE DumpType,
    PMINIDUMP_EXCEPTION_INFORMATION ExceptionParam,
    PMINIDUMP_USER_STREAM_INFORMATION UserStreamParam,
    PMINIDUMP_CALLBACK_INFORMATION CallbackParam
);

/* dbghlep.dll */
static HMODULE g_hDbgHelp;
/* mindump函数*/
static MiniDumpWriteDumpFun g_fnMiniDumpWriteDump;
/* dump存放文件存放目录 */
static TCHAR g_baseDir[MAX_PATH + 1];
/* 模块名称 */
static TCHAR g_moduleName[MAX_PATH + 1];
/* 同一个目录下最大允许的DUMP文件数目 */
static int g_maxNumOfDumpFile;

/*
 * 枚举目录，获取文件个数以及最早创建的文件
 */
static int
FindOldestFile(const TCHAR *baseDir, const TCHAR *searchPattern, 
               LARGE_INTEGER *oldestFileTime, TCHAR oldestFileName[MAX_PATH + 1])
{
    HANDLE          hFind = INVALID_HANDLE_VALUE;
    TCHAR           pattern[MAX_PATH + 1];
    WIN32_FIND_DATA findFileData;
    int             numOfDumpFile = 0;

    _sntprintf(pattern, MAX_PATH, _T("%s\\%s"), baseDir, searchPattern);
    hFind = FindFirstFile(pattern, &findFileData);
    if ( hFind != INVALID_HANDLE_VALUE )
    {
        do
        {
            LARGE_INTEGER lg;

            FILETIME_TO_LARGE_INTEGER(findFileData.ftCreationTime, lg);
            if ( lg.QuadPart < oldestFileTime->QuadPart )
            {
                oldestFileTime->QuadPart = lg.QuadPart;
                _sntprintf(oldestFileName, MAX_PATH, _T("%s\\%s"), baseDir, findFileData.cFileName);
            }
            numOfDumpFile++;
        }
        while ( FindNextFile(hFind, &findFileData) != 0 );

        FindClose(hFind);
    }

    return numOfDumpFile;
}

static void
CTCompressDumpFile(const TCHAR *filename)
{
    STARTUPINFO si; 
    PROCESS_INFORMATION pi; 
    TCHAR cmd[MAX_PATH + 1];
    BOOL ret;

    _sntprintf(cmd, MAX_PATH, _T("gzip.exe \"%s\""), filename);
    memset(&si, 0, sizeof(STARTUPINFO));
    si.cb = sizeof(STARTUPINFO); 
    GetStartupInfo(&si); 
    // 隐藏命令行窗口
    si.wShowWindow = SW_HIDE; 
    si.dwFlags = STARTF_USESHOWWINDOW;

    // 创建命令行进程
    ret = CreateProcess (NULL, cmd, NULL, NULL, TRUE, 0, NULL, 
                         NULL, &si, &pi ); 
    if ( ret ) 
    {
        /*
         * 等待压缩程序退出
         */
        WaitForSingleObject(pi.hProcess, INFINITE);
        CloseHandle(pi.hThread); 
        CloseHandle(pi.hProcess); 
    }
}

static LONG WINAPI 
CTUnhandledExceptionFilter(struct _EXCEPTION_POINTERS *exp)
{
    BOOL            ret;
    TCHAR           fileName[MAX_PATH + 1];
    HANDLE          hDumpFile;
    SYSTEMTIME      localTime;
    MINIDUMP_TYPE   minidumpType;
    LARGE_INTEGER   oldestFileTime; 
    TCHAR           oldestFileName[MAX_PATH + 1];
    int             numOfDumpFile = 0;
    SECURITY_ATTRIBUTES sa;
    SECURITY_DESCRIPTOR sd;
    MINIDUMP_EXCEPTION_INFORMATION expParam;
    
    /*
     * 允许所有用户修改文件以及目录
     */
    InitializeSecurityDescriptor(&sd,SECURITY_DESCRIPTOR_REVISION);
    SetSecurityDescriptorDacl(&sd, TRUE, NULL, FALSE);
    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.bInheritHandle = FALSE;
    sa.lpSecurityDescriptor = &sd;

    /*
     * 创建目录 
     */
    SHCreateDirectoryEx(NULL, g_baseDir, &sa);

    /*
     * 获取已经创建的dump文件数
     */
    memset(oldestFileName, 0, sizeof(oldestFileName));
    oldestFileTime.QuadPart = _I64_MAX;
    numOfDumpFile = FindOldestFile(g_baseDir, _T("*.dmp"), &oldestFileTime, oldestFileName);
    numOfDumpFile += FindOldestFile(g_baseDir, _T("*.gz"), &oldestFileTime, oldestFileName);

    /*
     * 删除过期的文件
     */
    if ( numOfDumpFile >= g_maxNumOfDumpFile ) 
    {
        DeleteFile(oldestFileName);
    }

    /*
     * 获取文件名
     */
    GetLocalTime(&localTime);
    _sntprintf(fileName, MAX_PATH, _T("%s\\%s-%04d%02d%02d-%02d-%02d-%02d.dmp"), 
               g_baseDir, g_moduleName, 
               localTime.wYear, localTime.wMonth, localTime.wDay,
               localTime.wHour, localTime.wMinute, localTime.wSecond);

    /*
     * 创建DUMP文件
     */
    hDumpFile = CreateFile(fileName, GENERIC_READ|GENERIC_WRITE,
                FILE_SHARE_WRITE|FILE_SHARE_READ, NULL, CREATE_ALWAYS, 0, 0);
    if ( hDumpFile == INVALID_HANDLE_VALUE )
    {
        return EXCEPTION_CONTINUE_SEARCH;
    }

#if 0
    /*
     * 禁止弹出应用程序错误的对话框
     */
    SetErrorMode(SEM_NOGPFAULTERRORBOX);
#endif
  
    expParam.ThreadId = GetCurrentThreadId();
    expParam.ExceptionPointers = exp;
    expParam.ClientPointers = TRUE;
    /*
     * 生成DUMP文件
     */
    minidumpType = MINIDUMP_TYPE(MiniDumpNormal
                                 | MiniDumpWithHandleData
                                 | MiniDumpWithPrivateReadWriteMemory
                                 | MiniDumpWithDataSegs
                                 | MiniDumpWithUnloadedModules
                                 | MiniDumpWithIndirectlyReferencedMemory
                                 | MiniDumpScanMemory
                                 | MiniDumpWithProcessThreadData
                                 | MiniDumpWithThreadInfo);
    ret = g_fnMiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(),
                                hDumpFile, minidumpType, &expParam, NULL, NULL);
    CloseHandle(hDumpFile);

    if ( ret != FALSE )
    {
        /*
         * 压缩文件
         */
        CTCompressDumpFile(fileName);
    }

    return EXCEPTION_CONTINUE_SEARCH;
} 

#if defined _M_X64 || defined _M_IX86
static LPTOP_LEVEL_EXCEPTION_FILTER WINAPI
MyDummySetUnhandledExceptionFilter(
    LPTOP_LEVEL_EXCEPTION_FILTER lpTopLevelExceptionFilter)
{
    UNREFERENCED_PARAMETER(lpTopLevelExceptionFilter);
    return NULL;
}
#else
# error "This code works only for x86 and x64!"
#endif

static BOOL 
PreventSetUnhandledExceptionFilter()
{
    HMODULE hKernel32 = LoadLibrary(_T("kernel32.dll"));

    if ( hKernel32 == NULL )
    {
        return FALSE;
    }
    void *pOrgEntry = GetProcAddress(hKernel32,
                                     "SetUnhandledExceptionFilter");
    if ( pOrgEntry == NULL )
    {
        return FALSE;
    }

    DWORD dwOldProtect = 0;
    SIZE_T jmpSize = 5;
#ifdef _M_X64
    jmpSize = 13;
#endif
    BOOL bProt = VirtualProtect(pOrgEntry, jmpSize,
                                PAGE_EXECUTE_READWRITE, &dwOldProtect);
    BYTE newJump[20];
    void *pNewFunc = &MyDummySetUnhandledExceptionFilter;
#ifdef _M_IX86
    DWORD dwOrgEntryAddr = (DWORD)pOrgEntry;
    dwOrgEntryAddr += jmpSize; // add 5 for 5 op-codes for jmp rel32
    DWORD dwNewEntryAddr = (DWORD)pNewFunc;
    DWORD dwRelativeAddr = dwNewEntryAddr - dwOrgEntryAddr;
    // JMP rel32: Jump near, relative, displacement relative to next instruction.
    newJump[0] = 0xE9; // JMP rel32
    memcpy(&newJump[1], &dwRelativeAddr, sizeof(pNewFunc));
#elif _M_X64
    // We must use R10 or R11, because these are "scratch" registers
    // which need not to be preserved accross function calls
    // For more info see: Register Usage for x64 64-Bit
    // http://msdn.microsoft.com/en-us/library/ms794547.aspx
    // Thanks to Matthew Smith!!!
    newJump[0] = 0x49; // MOV R11, ...
    newJump[1] = 0xBB; // ...
    memcpy(&newJump[2], &pNewFunc, sizeof(pNewFunc));
    //pCur += sizeof (ULONG_PTR);
    newJump[10] = 0x41; // JMP R11, ...
    newJump[11] = 0xFF; // ...
    newJump[12] = 0xE3; // ...
#endif
    SIZE_T bytesWritten;
    BOOL bRet = WriteProcessMemory(GetCurrentProcess(),
                                   pOrgEntry, newJump, jmpSize, &bytesWritten);

    if ( bProt != FALSE )
    {
        DWORD dwBuf;
        VirtualProtect(pOrgEntry, jmpSize, dwOldProtect, &dwBuf);
    }
    return bRet;
}

/**
 * @brief 初始化MINDUMP
 *
 * @param moduleName            模块名称，NULL表示使用可执行文件名
 * @param baseDir               MINDUMP文件的存放目录, NULL表示使用 可执行文件所在目录\dump\moduleName
 * @param maxNumOfDumpFile      同一个目录下最大允许的DUMP文件数目, 0表示使用默认值
 *
 * @note: 如果在动态库中使用此函数，则动态库不能被动态卸载，否则可能导致程序异常。
 *     
 */
static void
CTMinDumpInit(const TCHAR *moduleName, const TCHAR *baseDir, int maxNumOfDumpFile)
{
    TCHAR imagePath[MAX_PATH + 1];
    TCHAR *p;

    /*
     * 载入dbghelp.dll
     */
    g_hDbgHelp = LoadLibrary(_T("dbghelp.dll"));
    if ( g_hDbgHelp == NULL )
    {
        return;
    }
    g_fnMiniDumpWriteDump = (MiniDumpWriteDumpFun)GetProcAddress(g_hDbgHelp, "MiniDumpWriteDump");
    if ( g_fnMiniDumpWriteDump == NULL )
    {
        return;
    }

    /*
     * 获取模块名称
     */
    memset(g_moduleName, 0, sizeof(g_moduleName));
    if ( moduleName == NULL || *moduleName == 0 )
    {
        memset(imagePath, 0, sizeof(imagePath));
        GetModuleFileName(NULL, imagePath, MAX_PATH);

        p = _tcsrchr(imagePath, _T('\\'));
        if ( p != NULL )
        {
            p++;
            _tcsncpy(g_moduleName, p, MAX_PATH);
        }
        else
        {
            return;
        }
    }
    else
    {
        _tcsncpy(g_moduleName, moduleName, MAX_PATH);
    }

    /*
     * 获取存放目录
     */
    memset(g_baseDir, 0, sizeof(g_baseDir));
    if ( baseDir == NULL || *baseDir == 0 )
    {
        memset(imagePath, 0, sizeof(imagePath));
        GetModuleFileName(NULL, imagePath, MAX_PATH);

        p = _tcsrchr(imagePath, _T('\\'));
        if ( p != NULL )
        {
            *p = NULL;
            _sntprintf(g_baseDir, MAX_PATH, _T("%s\\dump\\%s"), imagePath, g_moduleName);
        }
        else
        {
            return;
        }
    }
    else
    {
        _tcsncpy(g_baseDir, baseDir, MAX_PATH);
    }

    if ( maxNumOfDumpFile == 0 )
    {
        g_maxNumOfDumpFile = DEFAULT_MAX_NUM_OF_DUMP_FILE;
    }
    else
    {
        g_maxNumOfDumpFile = maxNumOfDumpFile;
    }

    /*
     * 设置默认异常处理函数
     */
    SetUnhandledExceptionFilter(CTUnhandledExceptionFilter);

    /*
     * 禁止MS CRT调用SetUnhandledExceptionFilter
     * See:
     * http://blog.kalmbach-software.de/2008/04/02/unhandled-exceptions-in-vc8-and-above-for-x86-and-x64/
     */
    PreventSetUnhandledExceptionFilter();
}
