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
 
 #include "odata/edm/odata_edm.h"

namespace odata { namespace edm
{

std::shared_ptr<edm_entity_set> edm_entity_container::find_entity_set(::utility::string_t name) const
{
    auto find_iter = m_entity_sets.find(name);
	if (find_iter != m_entity_sets.end())
	{
		return find_iter->second;
	}

    return nullptr;
}

std::shared_ptr<edm_singleton> edm_entity_container::find_singleton(::utility::string_t name) const
{
    auto find_iter = m_singletons.find(name);
	if (find_iter != m_singletons.end())
	{
		return find_iter->second;
	}

    return nullptr;
}

std::shared_ptr<edm_operation_import> edm_entity_container::find_operation_import(::utility::string_t name) const
{
    auto find_iter = m_operation_imports.find(name);
	if (find_iter != m_operation_imports.end())
	{
		return find_iter->second;
	}

	return nullptr;
}

}}