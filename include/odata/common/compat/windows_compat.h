//---------------------------------------------------------------------
// <copyright file="windows_compat.h" company="Microsoft">
//      Copyright (C) Microsoft Corporation. All rights reserved. See License.txt in the project root for license information.
// </copyright>
//---------------------------------------------------------------------

#pragma once

#include "odata\common\compat\safeint3.hpp"
#include <string>

#if _MSC_VER >= 1700
// Support VS2012 SAL syntax only
#include <sal.h>
#else
#include "odata/common/compat/nosal.h"
#endif

#define _noexcept 

typedef wchar_t utf16char;
typedef std::wstring utf16string;
typedef std::wstringstream utf16stringstream;
typedef std::wostringstream utf16ostringstream;
typedef std::wostream utf16ostream;
typedef std::wistream utf16istream;
typedef std::wistringstream utf16istringstream;

#define _XPLATSTR(x) L ## x

#ifndef _TURN_OFF_PLATFORM_STRING
#define U(x) _XPLATSTR(x)
#endif // !_TURN_OFF_PLATFORM_STRING

typedef SafeInt<size_t> SafeSize;