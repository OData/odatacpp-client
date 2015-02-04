//---------------------------------------------------------------------
// <copyright file="os_utilities.h" company="Microsoft">
//      Copyright (C) Microsoft Corporation. All rights reserved. See License.txt in the project root for license information.
// </copyright>
//---------------------------------------------------------------------

#pragma once

#include "common_utilities_public.h"

namespace tests { namespace common { namespace utilities {

class os_utilities
{
public:

    static TEST_UTILITY_API void sleep(unsigned long ms);

    // Could use std::atomics but VS 2010 doesn't support it yet.
    static TEST_UTILITY_API unsigned long interlocked_increment(volatile unsigned long *addend);
    static TEST_UTILITY_API long interlocked_exchange(volatile long *target, long value);

private:
    os_utilities();
    os_utilities(const os_utilities &);
    os_utilities & operator=(const os_utilities &);
};

}}}

