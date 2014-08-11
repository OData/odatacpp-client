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
 
 #include "odata/edm/edm_schema.h"

namespace odata { namespace edm
{

std::shared_ptr<edm_entity_type> edm_schema::find_entity_type(::utility::string_t name) const
{
    auto nsp_size = m_namespace.size();
    if (name.size() > nsp_size && name.substr(0, nsp_size) == m_namespace)
    {
        // Strip qualification when searching.
        name = name.substr(nsp_size + 1);
    }

	auto find_iter = m_entity_types.find(name);
	if (find_iter != m_entity_types.end())
	{
		return find_iter->second;
	}

    return nullptr;
}

std::shared_ptr<edm_complex_type> edm_schema::find_complex_type(::utility::string_t name) const
{
    auto nsp_size = m_namespace.size();
    if (name.size() > nsp_size && name.substr(0, nsp_size) == m_namespace)
    {
        // Strip qualification when searching.
        name = name.substr(nsp_size + 1);
    }

    auto find_iter = m_complex_types.find(name);
	if (find_iter != m_complex_types.end())
	{
		return find_iter->second;
	}

    return nullptr;
}

std::shared_ptr<edm_enum_type> edm_schema::find_enum_type(::utility::string_t name) const
{
    auto nsp_size = m_namespace.size();
    if ( name.size() > nsp_size && name.substr(0, nsp_size) == m_namespace)
    {
        // Strip qualification when searching.
        name = name.substr(nsp_size + 1);
    }

    auto find_iter = m_enum_types.find(name);
	if (find_iter != m_enum_types.end())
	{
		return find_iter->second;
	}

    return nullptr;
}

std::shared_ptr<edm_operation_type> edm_schema::find_operation_type(::utility::string_t name) const
{
    auto nsp_size = m_namespace.size();
    if ( name.size() > nsp_size && name.substr(0, nsp_size) == m_namespace)
    {
        // Strip qualification when searching.
        name = name.substr(nsp_size+1);
    }

    auto find_iter = m_operation_types.find(name);
	if (find_iter != m_operation_types.end())
	{
		return find_iter->second;
	}

	return nullptr;
}

}}