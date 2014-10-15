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
#include "odata/core/odata_property_map.h"

namespace odata { namespace core
{

class odata_entity_value;

// A combination of the property type (see enum above) and a string representation of the property value.
class odata_value
{
public:
    /// <summary>Default constructor</summary>
    odata_value() : m_property_type(std::make_shared<::odata::edm::edm_named_type>()){}

    odata_value(std::shared_ptr<::odata::edm::edm_named_type>type, bool is_null_value = false) 
		: m_property_type(type), m_is_null_value(is_null_value)
	{
	}

    virtual ~odata_value(){};

    std::shared_ptr<::odata::edm::edm_named_type> get_value_type() const { return m_property_type; }

	void set_value_type(std::shared_ptr<::odata::edm::edm_named_type> property_type)
	{
		m_property_type = property_type;
	}

private:
    friend class entity;
    friend class odata_property_map;

    std::shared_ptr<::odata::edm::edm_named_type> m_property_type;
	bool m_is_null_value;
};

}}