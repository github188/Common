#pragma once

#ifndef __DLLEXPORT_H__
#define __DLLEXPORT_H__

#ifdef WIN32
#define DLL_COMMONLIB_API __declspec(dllexport)
#else
#define DLL_COMMONLIB_API
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

#include <stdio.h>
#include <tchar.h>
#include <vector>
#include <map>
#include <list>
#include <algorithm>
#include <Windows.h>
#include <SetupAPI.h>
#include <cfgmgr32.h>

#endif