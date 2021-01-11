//---------------------------------------------------------------------
// <copyright file="edm_model.h" company="Microsoft">
//      Copyright (C) Microsoft Corporation. All rights reserved. See License.txt in the project root for license information.
// </copyright>
//---------------------------------------------------------------------

#pragma once

#include "odata/common/utility.h"
#include "odata/edm/edm_schema.h"

namespace odata { namespace edm
{

class edm_model
{
public:
	std::shared_ptr<edm_schema> operator[](const ::odata::string_t& name)
	{
		for (size_t i = 0; i < m_schemata.size(); ++i)
		{
			if (m_schemata[i]->get_name() == name)
			{
				return m_schemata[i];
			}
		}
		return nullptr;
	}

	std::shared_ptr<edm_schema> add_schema(const ::odata::string_t& name, const ::odata::string_t& alias)
	{
		m_schemata.emplace_back(::odata::make_shared<edm_schema>(name, alias));
		return m_schemata.back();
	}

	const std::vector<std::shared_ptr<edm_schema>>& get_schema()
	{
		return m_schemata;
	}

	const ::odata::string_t& get_version() const
	{
		return m_version;
	}

	void set_version(::odata::string_t version)
	{
		m_version = std::move(version);
	}

	/// <summary>
	/// Looks up an entity type of the schema by name.
	/// </summary>
	/// <param name="name">The qualified or unqualified name of the entity type.</param>
	/// <returns>A pointer to the type if found, an empty pointer otherwise.</returns>
	ODATACPP_CLIENT_API std::shared_ptr<edm_entity_type> find_entity_type(::odata::string_t name) const;

	/// <summary>
	/// Looks up a complex type of the schema by name.
	/// </summary>
	/// <param name="name">The qualified or unqualified name of the entity type.</param>
	/// <returns>A pointer to the type if found, an empty pointer otherwise.</returns>
	ODATACPP_CLIENT_API std::shared_ptr<edm_complex_type> find_complex_type(::odata::string_t name) const;

	/// <summary>
	/// Looks up an enum type of the schema by name.
	/// </summary>
	/// <param name="name">The qualified or unqualified name of the enum type.</param>
	/// <returns>A pointer to the type if found, an empty pointer otherwise.</returns>
	ODATACPP_CLIENT_API std::shared_ptr<edm_enum_type> find_enum_type(::odata::string_t name) const;

	/// <summary>
	/// Looks up an operation type of the schema by name.
	/// </summary>
	/// <param name="name">The qualified or unqualified name of the operation type.</param>
	/// <returns>A pointer to the type if found, an empty pointer otherwise.</returns>
	ODATACPP_CLIENT_API std::shared_ptr<edm_operation_type> find_operation_type(::odata::string_t name) const;

	/// <summary>
	/// Looks up an entity container of the schema by name.
	/// </summary>
	/// <param name="name">The qualified or unqualified name of the entity container; an empty string refers to the default container</param>
	/// <returns>A pointer to the container if found, an empty pointer otherwise.</returns>
	ODATACPP_CLIENT_API std::shared_ptr<edm_entity_container> find_container(::odata::string_t name = _XPLATSTR("")) const;

private:
	std::vector<std::shared_ptr<edm_schema>> m_schemata;
	::odata::string_t  m_version;
};

}}
