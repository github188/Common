
#include "StdAfx.h"
#include <string>
#include <WtsApi32.h>
#include <UserEnv.h>
#include "Reg\RegOperation.h"

BOOL CRegOperation::RegReadDword(HKEY Key, const CHAR *Subkeyname, const CHAR *ValueName, 
    DWORD *Value, DWORD DefaultVal)
{
    HKEY  hk;

    if (Subkeyname == NULL || ValueName == NULL || Value == NULL)
    {
        return FALSE;
    }
    *Value = DefaultVal;

    if (RegOpenKeyExA(Key, Subkeyname, 0, KEY_READ, &hk) == ERROR_SUCCESS)
    {
        DWORD data, type;
        DWORD size = sizeof(DWORD);
        
        if (RegQueryValueExA(hk, ValueName, NULL, &type, (BYTE*)&data, &size) == ERROR_SUCCESS
            && type == REG_DWORD )
        {
            RegCloseKey(hk);
            *Value = data;
        }
        else
        {
            RegCloseKey(hk);
            return FALSE;
        }
    }

    return TRUE;
}

BOOL CRegOperation::RegReadDword(HKEY Key, const WCHAR *Subkeyname, const WCHAR *ValueName, 
    DWORD *Value, DWORD DefaultVal)
{
    HKEY  hk;
   
    if (Subkeyname == NULL || ValueName == NULL || Value == NULL )
    {
        return FALSE;
    }
    *Value = DefaultVal;

    if (RegOpenKeyExW(Key, Subkeyname, 0, KEY_READ, &hk) == ERROR_SUCCESS)
    {
        DWORD data, type;
        DWORD size = sizeof(DWORD);
        
        if (RegQueryValueExW(hk, ValueName, NULL, &type, (BYTE*)&data, &size) == ERROR_SUCCESS
            && type == REG_DWORD )
        {
            RegCloseKey(hk);
            *Value = data;
        }
        else
        {
            RegCloseKey(hk);
            return FALSE;
        }
    }

    return TRUE;
}

BOOL CRegOperation::RegReadDword(DWORD SessionId, const CHAR *Subkeyname, const CHAR *ValueName, 
    DWORD *Value, DWORD DefaultVal)
{
    HANDLE userToken;
    BOOL   ReturnValue;
 
    if (!WTSQueryUserToken(SessionId, &userToken))
    {
        return FALSE;
    }

    //仿冒
    if (!ImpersonateLoggedOnUser(userToken))
    {
        CloseHandle(userToken);
        return FALSE;
    }

    //调用DefineDosDevice
    ReturnValue = CRegOperation::RegReadDword(HKEY_CURRENT_USER, Subkeyname, ValueName, Value, DefaultVal);
    
    //返回SYSTEM权限
    RevertToSelf();

    CloseHandle(userToken);
    //返回
    return ReturnValue;
}

BOOL CRegOperation::RegReadDword(DWORD SessionId, const WCHAR *Subkeyname, const WCHAR *ValueName, 
    DWORD *Value, DWORD DefaultVal)
{
    HANDLE userToken;
    BOOL   ReturnValue;
 
    if (!WTSQueryUserToken(SessionId, &userToken))
    {
        return FALSE;
    }

    //仿冒
    if (!ImpersonateLoggedOnUser(userToken))
    {
        CloseHandle(userToken);
        return FALSE;
    }

    //调用DefineDosDevice
    ReturnValue = CRegOperation::RegReadDword(HKEY_CURRENT_USER, Subkeyname, ValueName, Value, DefaultVal);
    
    //返回SYSTEM权限
    RevertToSelf();

    CloseHandle(userToken);
    //返回
    return ReturnValue;
}

BOOL CRegOperation::RegReadString(HKEY Key, const WCHAR *Subkeyname, const WCHAR *ValueName, 
    WCHAR *Buffer, DWORD *Length, const WCHAR *DefaultVal)
{
    HKEY  hk = NULL;
    BOOL  ret;
    DWORD type;
    DWORD size;
    TCHAR *data = NULL;
    BOOL ReturnValue;

    if (Subkeyname == NULL || ValueName == NULL || Length == NULL)
    {
        ReturnValue = FALSE;
        goto EXIT;
    }

    ret = RegOpenKeyExW(Key, Subkeyname, 0, KEY_READ, &hk);
    if (ret != ERROR_SUCCESS)
    {
        ReturnValue = FALSE;
        goto EXIT;
    }

    size = 0;
    ret = RegQueryValueExW(hk, ValueName, NULL, &type, NULL, &size);
    if (ret != ERROR_MORE_DATA && ret != ERROR_SUCCESS)
    {
        RegCloseKey(hk);
        ReturnValue = FALSE;
        goto EXIT;
    }

    if (type != REG_SZ)
    {
        RegCloseKey(hk);
        ReturnValue = FALSE;
        goto EXIT;
    }

    data = (TCHAR *)malloc(size);
    if ( data == NULL )
    {
        RegCloseKey(hk);
        ReturnValue = FALSE;
        goto EXIT;
    }

    ret = RegQueryValueExW(hk, ValueName, NULL, &type, (LPBYTE)data, &size);
    if (ret == ERROR_SUCCESS)
    {
        if ( type == REG_SZ )
        {
            RegCloseKey(hk);
            ReturnValue = TRUE;
        }
        else
        {
            RegCloseKey(hk);
            ReturnValue = FALSE;
            goto EXIT;
        }
    }

EXIT:
    if (ReturnValue)
    {
        if (*Length < size)
        {
            *Length = size;
            ReturnValue = FALSE;
            SetLastError(ERROR_MORE_DATA);
        }
        else
        {
            memcpy(Buffer, data, size);
        }
    }

    if (data)
    {
        free(data);
    }

    return ReturnValue;
}

BOOL CRegOperation::RegReadString(HKEY Key, const CHAR *Subkeyname, const CHAR *ValueName, 
    CHAR *Buffer, DWORD *Length, const CHAR *DefaultVal)
{
    HKEY  hk = NULL;
    BOOL  ret;
    DWORD type;
    DWORD size;
    TCHAR *data = NULL;
    BOOL ReturnValue = TRUE;

    if (Subkeyname == NULL || ValueName == NULL || Length == NULL)
    {
        ReturnValue = FALSE;
        goto EXIT;
    }

    ret = RegOpenKeyExA(Key, Subkeyname, 0, KEY_READ, &hk);
    if (ret != ERROR_SUCCESS)
    {
        ReturnValue = FALSE;
        goto EXIT;
    }

    size = 0;
    ret = RegQueryValueExA(hk, ValueName, NULL, &type, NULL, &size);
    if (ret != ERROR_MORE_DATA && ret != ERROR_SUCCESS)
    {
        RegCloseKey(hk);
        ReturnValue = FALSE;
        goto EXIT;
    }

    if (type != REG_SZ)
    {
        RegCloseKey(hk);
        ReturnValue = FALSE;
        goto EXIT;
    }

    data = (TCHAR *)malloc(size);
    if ( data == NULL )
    {
        RegCloseKey(hk);
        ReturnValue = FALSE;
        goto EXIT;
    }

    ret = RegQueryValueExA(hk, ValueName, NULL, &type, (LPBYTE)data, &size);
    if (ret == ERROR_SUCCESS)
    {
        if ( type == REG_SZ )
        {
            RegCloseKey(hk);
            ReturnValue = TRUE;
        }
        else
        {
            RegCloseKey(hk);
            ReturnValue = FALSE;
            goto EXIT;
        }
    }

EXIT:
    if (ReturnValue)
    {
        if (*Length < size)
        {
            *Length = size;
            ReturnValue = FALSE;
            SetLastError(ERROR_MORE_DATA);
        }
        else
        { 
            memcpy(Buffer, data, size);
        }
    }

    if (data)
    {
        free(data);
    }

    return ReturnValue;
}

BOOL CRegOperation::RegReadString(DWORD SessionId, const WCHAR *Subkeyname, const WCHAR *ValueName, 
    WCHAR *Buffer, DWORD *Length, const WCHAR *DefaultVal)
{
    HANDLE userToken;
    BOOL   ReturnValue;
 
    if (!WTSQueryUserToken(SessionId, &userToken))
    {
        return FALSE;
    }

    //仿冒
    if (!ImpersonateLoggedOnUser(userToken))
    {
        CloseHandle(userToken);
        return FALSE;
    }

    //调用DefineDosDevice
    ReturnValue = CRegOperation::RegReadString(HKEY_CURRENT_USER, Subkeyname, ValueName, Buffer, Length, DefaultVal);
    
    //返回SYSTEM权限
    RevertToSelf();

    CloseHandle(userToken);
    //返回
    return ReturnValue;
}

BOOL CRegOperation::RegReadString(DWORD SessionId,  const CHAR *Subkeyname, const CHAR *ValueName, 
    CHAR *Buffer, DWORD *Length, const CHAR *DefaultVal)
{
    HANDLE userToken;
    BOOL   ReturnValue;
 
    if (!WTSQueryUserToken(SessionId, &userToken))
    {
        return FALSE;
    }

    //仿冒
    if (!ImpersonateLoggedOnUser(userToken))
    {
        CloseHandle(userToken);
        return FALSE;
    }

    //调用DefineDosDevice
    ReturnValue = CRegOperation::RegReadString(HKEY_CURRENT_USER, Subkeyname, ValueName, Buffer, Length, DefaultVal);
    
    //返回SYSTEM权限
    RevertToSelf();

    CloseHandle(userToken);
    //返回
    return ReturnValue;
}

BOOL CRegOperation::RegReadMultiString(HKEY Key, const CHAR *Subkeyname, const CHAR *ValueName, 
    CHAR *Buffer, DWORD *Length)
{
    HKEY  hk = NULL;
    LONG  ret;
    DWORD type;
    DWORD size;

    if (Subkeyname == NULL || ValueName == NULL || Length == NULL)
    {
        return FALSE;
    }

    ret = RegOpenKeyExA(Key, Subkeyname, 0, KEY_READ, &hk);
    if (ret != ERROR_SUCCESS)
    {
        return FALSE;
    }

    size = 0;
    ret = RegQueryValueExA(hk, ValueName, NULL, &type, NULL, &size);
    if (ret != ERROR_MORE_DATA && ret != ERROR_SUCCESS)
    {
        RegCloseKey(hk);
        return FALSE;
    }

    if (type != REG_MULTI_SZ)
    {
        RegCloseKey(hk);
        return FALSE;
    }

    if (*Length < size)
    {
        RegCloseKey(hk);
        *Length = size;
        return FALSE;
    }

    *Length = size;

    ret = RegQueryValueExA(hk, ValueName, NULL, &type, (LPBYTE)Buffer, &size);
    if (ret == ERROR_SUCCESS)
    {
        RegCloseKey(hk);
        return TRUE;
    }

    RegCloseKey(hk);
    return FALSE;
}

BOOL CRegOperation::RegReadMultiString(HKEY Key, const WCHAR *Subkeyname, const WCHAR *ValueName, 
    WCHAR *Buffer, DWORD *Length)
{
    HKEY  hk = NULL;
    LONG  ret;
    DWORD type;
    DWORD size;

    if (Subkeyname == NULL || ValueName == NULL || Length == NULL)
    {
        return FALSE;
    }

    ret = RegOpenKeyExW(Key, Subkeyname, 0, KEY_READ, &hk);
    if (ret != ERROR_SUCCESS)
    {
        return FALSE;
    }

    size = 0;
    ret = RegQueryValueExW(hk, ValueName, NULL, &type, NULL, &size);
    if (ret != ERROR_MORE_DATA && ret != ERROR_SUCCESS)
    {
        RegCloseKey(hk);
        return FALSE;
    }

    if (type != REG_MULTI_SZ)
    {
        RegCloseKey(hk);
        return FALSE;
    }

    if (*Length < size)
    {
        RegCloseKey(hk);
        *Length = size;
        return FALSE;
    }

    *Length = size;

    ret = RegQueryValueExW(hk, ValueName, NULL, &type, (LPBYTE)Buffer, &size);
    if (ret == ERROR_SUCCESS)
    {
        RegCloseKey(hk);
        return TRUE;
    }

    RegCloseKey(hk);
    return FALSE;
}

BOOL CRegOperation::RegReadMultiString(DWORD SessionId, const CHAR *Subkeyname, const CHAR *ValueName, 
    CHAR *Buffer, DWORD *Length)
{
    HANDLE userToken;
    BOOL   ReturnValue;
 
    if (!WTSQueryUserToken(SessionId, &userToken))
    {
        return FALSE;
    }

    //仿冒
    if (!ImpersonateLoggedOnUser(userToken))
    {
        CloseHandle(userToken);
        return FALSE;
    }

    //调用DefineDosDevice
    ReturnValue = CRegOperation::RegReadMultiString(HKEY_CURRENT_USER, Subkeyname, ValueName, Buffer, Length);
    
    //返回SYSTEM权限
    RevertToSelf();

    CloseHandle(userToken);
    //返回
    return ReturnValue;
}

BOOL CRegOperation::RegReadMultiString(DWORD SessionId, const WCHAR *Subkeyname, const WCHAR *ValueName, 
    WCHAR *Buffer, DWORD *Length)
{
    HANDLE userToken;
    BOOL   ReturnValue;
 
    if (!WTSQueryUserToken(SessionId, &userToken))
    {
        return FALSE;
    }

    //仿冒
    if (!ImpersonateLoggedOnUser(userToken))
    {
        CloseHandle(userToken);
        return FALSE;
    }

    //调用DefineDosDevice
    ReturnValue = CRegOperation::RegReadMultiString(HKEY_CURRENT_USER, Subkeyname, ValueName, Buffer, Length);
    
    //返回SYSTEM权限
    RevertToSelf();

    CloseHandle(userToken);
    //返回
    return ReturnValue;
}

BOOL CRegOperation::RegWriteDword(HKEY Key, const CHAR *Subkeyname, 
    const CHAR *ValueName, DWORD Value)
{
    HKEY hk;
    LONG ret;
    DWORD disposition;

    if (Subkeyname == NULL || ValueName == NULL)
    {
        return FALSE;
    }

    ret = RegCreateKeyExA(Key, Subkeyname, 0, NULL, REG_OPTION_NON_VOLATILE, 
                         KEY_CREATE_SUB_KEY | KEY_SET_VALUE, NULL, &hk, &disposition);
    if (ret == ERROR_SUCCESS)
    {
        if (RegSetValueExA(hk, ValueName, 0, REG_DWORD, (LPBYTE)&Value, sizeof(DWORD)) == ERROR_SUCCESS)
        {
            RegCloseKey(hk);
            return TRUE;
        }
        else
        {
            RegCloseKey(hk);
            return FALSE;
        }
    }
    return FALSE;
}

BOOL CRegOperation::RegWriteDword(HKEY Key, const WCHAR *Subkeyname, 
    const WCHAR *ValueName, DWORD Value)
{
    HKEY hk;
    LONG ret;
    DWORD disposition;

    if (Subkeyname == NULL || ValueName == NULL)
    {
        return FALSE;
    }

    ret = RegCreateKeyExW(Key, Subkeyname, 0, NULL, REG_OPTION_NON_VOLATILE, 
                         KEY_CREATE_SUB_KEY | KEY_SET_VALUE, NULL, &hk, &disposition);
    if (ret == ERROR_SUCCESS)
    {
        if (RegSetValueExW(hk, ValueName, 0, REG_DWORD, (LPBYTE)&Value, sizeof(DWORD)) == ERROR_SUCCESS)
        {
            RegCloseKey(hk);
            return TRUE;
        }
        else
        {
            RegCloseKey(hk);
            return FALSE;
        }
    }
    return FALSE;
}

BOOL CRegOperation::RegWriteString(HKEY Key, const CHAR *Subkeyname, 
    const CHAR *ValueName, const CHAR *Value)
{
    HKEY hk;
    LONG ret;
    DWORD disposition;

    if (Subkeyname == NULL || ValueName == NULL || Value == NULL)
    {
        return FALSE;
    }

    ret = RegCreateKeyExA(Key, Subkeyname, 0, NULL, REG_OPTION_NON_VOLATILE, 
                         KEY_CREATE_SUB_KEY | KEY_SET_VALUE, NULL, &hk, &disposition);
    if (ret == ERROR_SUCCESS)
    {
        if (RegSetValueExA(hk, ValueName, 0, REG_SZ, (LPBYTE)Value, strlen(Value) + 1) == ERROR_SUCCESS)
        {
            RegCloseKey(hk);
            return TRUE;
        }
        else
        {
            RegCloseKey(hk);
            return FALSE;
        }
    }

    return FALSE;
}

BOOL CRegOperation::RegWriteString(HKEY Key, const WCHAR *Subkeyname, 
    const WCHAR *ValueName, const WCHAR *Value)
{
    HKEY hk;
    LONG ret;
    DWORD disposition;

    if (Subkeyname == NULL || ValueName == NULL || Value == NULL)
    {
        return FALSE;
    }

    ret = RegCreateKeyExW(Key, Subkeyname, 0, NULL, REG_OPTION_NON_VOLATILE, 
                         KEY_CREATE_SUB_KEY | KEY_SET_VALUE, NULL, &hk, &disposition);
    if (ret == ERROR_SUCCESS)
    {
        if (RegSetValueExW(hk, ValueName, 0, REG_SZ, (LPBYTE)Value, (wcslen(Value) + 1) * sizeof(WCHAR)) == ERROR_SUCCESS)
        {
            RegCloseKey(hk);
            return TRUE;
        }
        else
        {
            RegCloseKey(hk);
            return FALSE;
        }
    }

    return FALSE;
}

BOOL CRegOperation::ReadBinary(HKEY hKey, const CHAR *subkeyname, const CHAR *valueName, BYTE **val, int *vallen)
{
    HKEY  hk = NULL;
    LONG  ret;
    DWORD type;
    DWORD size;
    BYTE *data = NULL;

    if ( subkeyname == NULL || valueName == NULL )
    {
        return ERROR_INVALID_PARAMETER;
    }

    *val = NULL;
    *vallen = 0;
    ret = RegOpenKeyExA(hKey, subkeyname, 0, KEY_READ, &hk);
    if ( ret != ERROR_SUCCESS )
    {
        return ret;
    }

    size = 0;
    ret = RegQueryValueExA(hk, valueName, NULL, &type, NULL, &size);
    if ( ret != ERROR_MORE_DATA && ret != ERROR_SUCCESS )
    {
        goto EXIT;
    }
    if ( type != REG_BINARY )
    {
        ret  = ERROR_INVALID_DATA;
    }

    data = (BYTE *)malloc(size);
    if ( data == NULL )
    {
        ret = ERROR_INSUFFICIENT_BUFFER;
        goto EXIT;
    }
    ret = RegQueryValueExA(hk, valueName, NULL, &type, (LPBYTE)data, &size);
    if ( ret == ERROR_SUCCESS )
    {
        if ( type == REG_BINARY )
        {
            *val = data;
            *vallen = size;
        }
        else
        {
            ret = ERROR_INVALID_DATA;
        }
    }

EXIT:
    RegCloseKey(hk);
    if ( ret != ERROR_SUCCESS )
    {
        if ( data )
        {
            free(data);
        }
    }
    return ret;
}

BOOL CRegOperation::ReadBinary(HKEY hKey, const WCHAR *subkeyname, const WCHAR *valueName, BYTE **val, int *vallen)
{
    HKEY  hk = NULL;
    LONG  ret;
    DWORD type;
    DWORD size;
    BYTE *data = NULL;

    if ( subkeyname == NULL || valueName == NULL )
    {
        return ERROR_INVALID_PARAMETER;
    }

    *val = NULL;
    *vallen = 0;
    ret = RegOpenKeyExW(hKey, subkeyname, 0, KEY_READ, &hk);
    if ( ret != ERROR_SUCCESS )
    {
        return ret;
    }

    size = 0;
    ret = RegQueryValueExW(hk, valueName, NULL, &type, NULL, &size);
    if ( ret != ERROR_MORE_DATA && ret != ERROR_SUCCESS )
    {
        goto EXIT;
    }
    if ( type != REG_BINARY )
    {
        ret  = ERROR_INVALID_DATA;
    }

    data = (BYTE *)malloc(size);
    if ( data == NULL )
    {
        ret = ERROR_INSUFFICIENT_BUFFER;
        goto EXIT;
    }
    ret = RegQueryValueExW(hk, valueName, NULL, &type, (LPBYTE)data, &size);
    if ( ret == ERROR_SUCCESS )
    {
        if ( type == REG_BINARY )
        {
            *val = data;
            *vallen = size;
        }
        else
        {
            ret = ERROR_INVALID_DATA;
        }
    }

EXIT:
    RegCloseKey(hk);
    if ( ret != ERROR_SUCCESS )
    {
        if ( data )
        {
            free(data);
        }
    }
    return ret;
}



