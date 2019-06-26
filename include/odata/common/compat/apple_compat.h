//---------------------------------------------------------------------
// <copyright file="apple_compat.h" company="Microsoft">
//      Copyright (C) Microsoft Corporation. All rights reserved. See License.txt in the project root for license information.
// </copyright>
//---------------------------------------------------------------------

#pragma once
#include <cstdint>
#include <string>
#include <sstream>
#include <iostream>
#define __cdecl

#include "nosal.h"

// MSVC doesn't support this yet
#define _noexcept noexcept

#define novtable /* no novtable equivalent */
#define __declspec(x) __attribute__ ((x))
#define __export __attribute__ ((dllexport))
#define __stdcall __attribute__ ((stdcall))
#define STDMETHODCALLTYPE __export __stdcall

// ignore these:
#define dllimport 

// OpenProt defines
#define _SH_DENYRW 0x20

#include <stdint.h>

#define __assume(x) do { if (!(x)) __builtin_unreachable(); } while (false)

typedef uint32_t HRESULT;

#define SOCKET int
#define SOCKET_ERROR -1

#define S_OK 0
#define S_FALSE 1
#define STG_E_CANTSAVE 0x80030103
#define STG_E_INVALIDPOINTER 0x80030009
#define E_NOTIMPL 0x80004001
#define E_NOINTERFACE 0x80004002

typedef unsigned long ULONG;
typedef unsigned short WORD;
typedef unsigned long DWORD;

typedef struct _SYSTEMTIME {
  WORD wYear;
  WORD wMonth;
  WORD wDayOfWeek;
  WORD wDay;
  WORD wHour;
  WORD wMinute;
  WORD wSecond;
  WORD wMilliseconds;
} SYSTEMTIME, *PSYSTEMTIME;

#define ULARGE_INTEGER uint64_t
#define LARGE_INTEGER int64_t

#define WINAPI __stdcall

#define YieldProcessor() __asm__ __volatile__ ("pause")

#define UNREFERENCED_PARAMETER(x) (void)x
#define _ASSERTE(x) assert(x)

#include <string>

typedef char16_t utf16char;
typedef std::u16string utf16string;
typedef std::basic_stringstream<char16_t> utf16stringstream;
typedef std::basic_ostringstream<char16_t> utf16ostringstream;
typedef std::basic_ostream<char16_t> utf16ostream;
typedef std::basic_istream<char16_t> utf16istream;
typedef std::basic_istringstream<char16_t> utf16istringstream;

#include "odata/common/compat/SafeInt3.hpp"
typedef SafeInt<size_t> SafeSize;
