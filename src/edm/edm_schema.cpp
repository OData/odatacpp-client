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