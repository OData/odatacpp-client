//---------------------------------------------------------------------
// <copyright file="edm_schema.cpp" company="Microsoft">
//      Copyright (C) Microsoft Corporation. All rights reserved. See License.txt in the project root for license information.
// </copyright>
//---------------------------------------------------------------------

#include "odata/edm/edm_schema.h"

namespace odata { namespace edm
{

std::shared_ptr<edm_entity_type> edm_schema::find_entity_type(::odata::string_t name) const
{
	auto nsp_size = m_namespace.size();
	auto ali_size = m_alias.size();
	if (name.size() > nsp_size && name.compare(0, nsp_size, m_namespace) == 0)
	{
		// Strip qualification when searching.
		name.erase(0, nsp_size + 1);
	}
	else if (name.size() > ali_size && name.compare(0, ali_size, m_alias) == 0)
	{
		// Strip qualification when searching.
		name.erase(0, ali_size + 1);
	}

	auto find_iter = m_entity_types.find(name);
	if (find_iter != m_entity_types.end())
	{
		return find_iter->second;
	}

	return nullptr;
}

std::shared_ptr<edm_complex_type> edm_schema::find_complex_type(::odata::string_t name) const
{
	auto nsp_size = m_namespace.size();
	auto ali_size = m_alias.size();
	if (name.size() > nsp_size && name.compare(0, nsp_size, m_namespace) == 0)
	{
		// Strip qualification when searching.
		name.erase(0, nsp_size + 1);
	}
	else if (name.size() > ali_size && name.compare(0, ali_size, m_alias) == 0)
	{
		// Strip qualification when searching.
		name.erase(0, ali_size + 1);
	}

	auto find_iter = m_complex_types.find(name);
	if (find_iter != m_complex_types.end())
	{
		return find_iter->second;
	}

	return nullptr;
}

std::shared_ptr<edm_enum_type> edm_schema::find_enum_type(::odata::string_t name) const
{
	auto nsp_size = m_namespace.size();
	auto ali_size = m_alias.size();
	if (name.size() > nsp_size && name.compare(0, nsp_size, m_namespace) == 0)
	{
		// Strip qualification when searching.
		name.erase(0, nsp_size + 1);
	}
	else if (name.size() > ali_size && name.compare(0, ali_size, m_alias) == 0)
	{
		// Strip qualification when searching.
		name.erase(0, ali_size + 1);
	}

	auto find_iter = m_enum_types.find(name);
	if (find_iter != m_enum_types.end())
	{
		return find_iter->second;
	}

	return nullptr;
}

std::shared_ptr<edm_operation_type> edm_schema::find_operation_type(::odata::string_t name) const
{
	auto nsp_size = m_namespace.size();
	auto ali_size = m_alias.size();
	if (name.size() > nsp_size && name.compare(0, nsp_size, m_namespace) == 0)
	{
		// Strip qualification when searching.
		name.erase(0, nsp_size + 1);
	}
	else if (name.size() > ali_size && name.compare(0, ali_size, m_alias) == 0)
	{
		// Strip qualification when searching.
		name.erase(0, ali_size + 1);
	}

	auto find_iter = m_operation_types.find(name);
	if (find_iter != m_operation_types.end())
	{
		return find_iter->second;
	}

	return nullptr;
}

}}
