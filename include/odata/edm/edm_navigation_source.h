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

#include "odata/common/utility.h"
#include "odata/edm/edm_type.h"

namespace odata { namespace edm
{

enum container_resource_type
{
	E_RESOURCE_ENTITY_SET = 0,
	E_RESOURCE_SINGLETON,
};

class edm_navigation_source
{
public:
	edm_navigation_source(::utility::string_t name, container_resource_type resource_type) : m_name(std::move(name)), m_resource_type(resource_type)
	{}

	virtual ~edm_navigation_source()
	{}

	void add_navigation_source(const ::utility::string_t& property_path_name, const ::utility::string_t& target_name)
	{
		navigation_property_mapping[property_path_name] = target_name;
	}

	::utility::string_t get_navigation_source_name(const ::utility::string_t& property_path_name)
	{
		return navigation_property_mapping[property_path_name];
	}

	const std::unordered_map<::utility::string_t, ::utility::string_t>& get_navigation_sources() const
	{
		return navigation_property_mapping;
	}


    const ::utility::string_t& get_name() const 
    {
        return m_name;
    }

	container_resource_type get_resource_type() const
	{
		return m_resource_type;
	}

protected:
	std::unordered_map<::utility::string_t, ::utility::string_t> navigation_property_mapping;
    ::utility::string_t m_name;
	container_resource_type m_resource_type;
};

}}