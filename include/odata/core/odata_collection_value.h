//---------------------------------------------------------------------
// <copyright file="odata_collection_value.h" company="Microsoft">
//      Copyright (C) Microsoft Corporation. All rights reserved. See License.txt in the project root for license information.
// </copyright>
//---------------------------------------------------------------------

#pragma once

#include "odata/edm/odata_edm.h"
#include "odata/core/odata_value.h"

namespace odata { namespace core
{

class odata_collection_value : public odata_value
{
public:
    odata_collection_value(std::shared_ptr<::odata::edm::edm_named_type> type) : odata_value(type)
    {
    }

    void add_collection_value(std::shared_ptr<odata_value> value)
    {
        m_values.push_back(value);
    }

	const std::vector<std::shared_ptr<odata_value>>& get_collection_values() const 
	{
		return m_values;
	}

private:
    std::vector<std::shared_ptr<odata_value>>  m_values;
};

}}