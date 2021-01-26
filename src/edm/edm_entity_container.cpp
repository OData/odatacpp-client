//---------------------------------------------------------------------
// <copyright file="edm_entity_container.cpp" company="Microsoft">
//      Copyright (C) Microsoft Corporation. All rights reserved. See License.txt in the project root for license information.
// </copyright>
//---------------------------------------------------------------------

#include "odata/edm/odata_edm.h"

namespace odata { namespace edm
{

std::shared_ptr<edm_entity_set> edm_entity_container::find_entity_set(::odata::string_t name) const
{
	auto find_iter = m_entity_sets.find(name);
	if (find_iter != m_entity_sets.end())
	{
		return find_iter->second;
	}

	return nullptr;
}

std::shared_ptr<edm_singleton> edm_entity_container::find_singleton(::odata::string_t name) const
{
	auto find_iter = m_singletons.find(name);
	if (find_iter != m_singletons.end())
	{
		return find_iter->second;
	}

	return nullptr;
}

std::shared_ptr<edm_operation_import> edm_entity_container::find_operation_import(::odata::string_t name) const
{
	auto find_iter = m_operation_imports.find(name);
	if (find_iter != m_operation_imports.end())
	{
		return find_iter->second;
	}

	return nullptr;
}

}}
