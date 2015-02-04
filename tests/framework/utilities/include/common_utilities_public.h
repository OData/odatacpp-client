//---------------------------------------------------------------------
// <copyright file="common_utilities_public.h" company="Microsoft">
//      Copyright (C) Microsoft Corporation. All rights reserved. See License.txt in the project root for license information.
// </copyright>
//---------------------------------------------------------------------

#pragma once

#if !defined(_WIN32) && !defined(__cplusplus_winrt)
#define TEST_UTILITY_API
#endif // !_WIN32 && !__cplusplus_winrt

#ifndef TEST_UTILITY_API
#ifdef COMMONUTILITIES_EXPORTS
#define TEST_UTILITY_API __declspec(dllexport)
#else // COMMONUTILITIES_EXPORTS
#define TEST_UTILITY_API __declspec(dllimport)
#endif // COMMONUTILITIES_EXPORTS
#endif // TEST_UTILITY_API
