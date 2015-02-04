//---------------------------------------------------------------------
// <copyright file="os_utilities.cpp" company="Microsoft">
//      Copyright (C) Microsoft Corporation. All rights reserved. See License.txt in the project root for license information.
// </copyright>
//---------------------------------------------------------------------

#include "stdafx.h"

#include "os_utilities.h"

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#define NOMINMAX
#include <Windows.h>
#else
#include <unistd.h>
#endif

namespace tests { namespace common { namespace utilities {

void os_utilities::sleep(unsigned long ms)
{
#ifdef WIN32
    Sleep(ms);
#else
    usleep(ms*1000);
#endif
}

unsigned long os_utilities::interlocked_increment(volatile unsigned long *addend)
{
#ifdef WIN32
    return InterlockedIncrement(addend);
#elif defined(__GNUC__)
    return __sync_add_and_fetch(addend, 1);
#else
#error Need to implement interlocked_increment
#endif
}

long os_utilities::interlocked_exchange(volatile long *target, long value)
{
#ifdef WIN32
    return InterlockedExchange(target, value);
#elif defined(__GNUC__)
    return __sync_lock_test_and_set(target, value);
#else
#error Need to implement interlocked_exchange
#endif
}

}}}
