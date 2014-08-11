/*
 * Copyright (c) Microsoft Corporation. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
 
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