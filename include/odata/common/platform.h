//---------------------------------------------------------------------
// <copyright file="platform.h" company="Microsoft">
//      Copyright (C) Microsoft Corporation. All rights reserved. See License.txt in the project root for license information.
// </copyright>
//---------------------------------------------------------------------

#pragma once

#if defined(_MSC_VER) && (_MSC_VER >= 1800)
#include <ppltasks.h>
namespace pplx = Concurrency;
#else 
#include "pplx/pplxtasks.h"
#endif

#ifdef _MSC_VER
#pragma warning(disable:4146 4267 4521 4522 4566 4996)
#endif

#define U(x) _XPLATSTR(x)

#ifndef ODATACPP_CLIENT_API
#ifdef ODATACLIENT_EXPORTS
#define ODATACPP_CLIENT_API __declspec(dllexport)
#else
#define ODATACPP_CLIENT_API __declspec(dllimport)
#endif
#endif