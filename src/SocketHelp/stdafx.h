// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             //  �� Windows ͷ�ļ����ų�����ʹ�õ���Ϣ


#ifndef WIN32

#include <windef.h>
//#include <winpr/winpr.h>
#include <winpr/wtypes.h>
#include <winsync.h>
#include <winpr/winsock.h>

#define closesocket close

#endif
// TODO: �ڴ˴����ó�����Ҫ������ͷ�ļ�
