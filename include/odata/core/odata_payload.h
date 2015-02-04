//---------------------------------------------------------------------
// <copyright file="odata_payload.h" company="Microsoft">
//      Copyright (C) Microsoft Corporation. All rights reserved. See License.txt in the project root for license information.
// </copyright>
//---------------------------------------------------------------------

#pragma once

#include "odata/core/odata_entity_value.h"

namespace odata { namespace core
{

enum odata_payload_kind
{
	PAYLOAD_KIND_SERVICE_DOCUMENT = 0,
	PAYLOAD_KIND_ENTITY,
	PAYLOAD_KIND_COLLECTION_ENTITY,
	PAYLOAD_KIND_SINGLETON,
	PAYLOAD_KIND_DERIVED_ENTITY,
	PAYLOAD_KIND_COLLECTION_DERIVED_ENTITY,
	PAYLOAD_KIND_PROJECT_ENTITY,
	PAYLOAD_KIND_COLLECTION_PROJECT_ENTITY,
	PAYLOAD_KIND_EXPAND_ENTITY,
	PAYLOAD_KIND_COLLECTION_EXPAND_ENTITY,
	PAYLOAD_KIND_ENTITY_REFERENCE,
	PAYLOAD_KIND_COLLECTION_ENTITY_REFERENCE,
	PAYLOAD_KIND_ENTITY_PROPERTY,
	PAYLOAD_KIND_PRIMITIVE_COMPLEX,
	PAYLOAD_KIND_COLLECTION_PRIMITIVE_COMPLEX,
	PAYLOAD_KIND_UNKNOWN,
};

class odata_payload
{
public:
	odata_payload() {}

	void add_value(std::shared_ptr<odata_value> value)
	{
		m_odata_values.push_back(value);
	}

	void insert_values(const std::vector<std::shared_ptr<odata_value>>& values)
	{
		m_odata_values.insert(m_odata_values.end(), values.begin(), values.end()); 
	}

	const int value_count() const
	{
		return m_odata_values.size();
	}

	const std::vector<std::shared_ptr<odata_value>>& get_values() const 
	{
		return m_odata_values;
	}

	const ::utility::string_t& get_next_link() const
	{
		return m_next_link;
	}

	void set_next_link(::utility::string_t next_link)
	{
		m_next_link = std::move(next_link);
	}

	const ::utility::string_t& get_context_url() const
	{
		return m_context_url;
	}

	void set_context_url(::utility::string_t context_ur)
	{
		m_context_url = std::move(context_ur);
	}

	bool has_next_page_data()
	{
		return !m_next_link.empty();
	}

private:
	std::vector<std::shared_ptr<odata_value>>   m_odata_values;
	::utility::string_t               m_next_link;
	::utility::string_t               m_context_url;
};

}}