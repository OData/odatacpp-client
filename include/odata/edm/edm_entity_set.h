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
#include "odata/edm/edm_navigation_source.h"
#include "odata/edm/edm_type.h"

namespace odata { namespace edm
{

class edm_entity_container;

/// <summary>
/// Represents a set of entities in a container
/// </summary>
class edm_entity_set : public edm_navigation_source
{
public:
    /// <summary>
    /// Constructor
    /// </summary>
    /// <param name="name">The name of the entity set.</param>
    edm_entity_set(::utility::string_t name, ::utility::string_t type) 
		: m_type_name(type), edm_navigation_source(name, container_resource_type::E_RESOURCE_ENTITY_SET)
    {}

	edm_entity_set(::utility::string_t name, std::shared_ptr<edm_entity_type> type) 
		: m_entity_type(type), edm_navigation_source(name, container_resource_type::E_RESOURCE_ENTITY_SET)
    {}

	~edm_entity_set()
	{
#ifdef _MS_WINDOWS_DEBUG
		std::wcout << U("destroy edm_entity_set") << std::endl;
#endif
	}

    /// <summary>
    /// Gets the name of the type of the entity set
    /// </summary>
    /// <returns>The name of the type of the entity set.</returns>
    const ::utility::string_t& get_entity_type_name() const 
    {
        return m_type_name;
    }

	std::shared_ptr<edm_entity_type> get_entity_type() const
	{
		return m_entity_type.lock();
	}

	void set_entity_type(const std::shared_ptr<edm_entity_type>& entity_type)
	{
		m_entity_type = entity_type;
	}

private:
    friend class edm_entity_container;
    ::utility::string_t m_type_name;
	std::weak_ptr<edm_entity_type> m_entity_type;
};

}}