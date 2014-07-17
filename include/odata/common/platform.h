#pragma once

#if defined(_MSC_VER) && (_MSC_VER >= 1800)
#include <ppltasks.h>
namespace pplx = Concurrency;
#else 
#include "pplx/pplxtasks.h"
#endif

#ifdef _MSC_VER
#pragma warning(disable:4146 4521 4522 4996)
#endif

#define U(x) _XPLATSTR(x)

#ifndef ODATACPP_API
#ifdef ODATALIB_EXPORTS
#define ODATACPP_API __declspec(dllexport)
#else
#define ODATACPP_API __declspec(dllimport)
#endif
#endif

#ifndef ODATACPP_CLIENT_API
#ifdef ODATACLIENT_EXPORTS
#define ODATACPP_CLIENT_API __declspec(dllexport)
#else
#define ODATACPP_CLIENT_API __declspec(dllimport)
#endif
#endif