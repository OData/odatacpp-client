//---------------------------------------------------------------------
// <copyright file="odata_complex_value.h" company="Microsoft">
//      Copyright (C) Microsoft Corporation. All rights reserved. See License.txt in the project root for license information.
// </copyright>
//---------------------------------------------------------------------

#pragma once

#include "odata/edm/odata_edm.h"
#include "odata/core/odata_value.h"
#include "odata/core/odata_structured_value.h"

namespace odata { namespace core
{

class odata_complex_value : public odata_structured_value
{
public:
	odata_complex_value(std::shared_ptr<::odata::edm::edm_named_type> type) : odata_structured_value(type)
    {
    }
};

}}