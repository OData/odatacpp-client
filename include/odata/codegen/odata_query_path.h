//---------------------------------------------------------------------
// <copyright file="odata_query_path.h" company="Microsoft">
//      Copyright (C) Microsoft Corporation. All rights reserved. See License.txt in the project root for license information.
// </copyright>
//---------------------------------------------------------------------

#pragma once

#include "odata/common/utility.h"
#include "odata/codegen/odata_query_option.h"
#include "odata/core/odata_core.h"

namespace odata { namespace codegen {

class odata_query_path
{
private:
	odata_query_path(::odata::string_t path, bool is_root = false)
	:	l_child_item(nullptr), l_sibling_item(nullptr), m_query_options(), m_resource_path(std::move(path)), m_is_root(is_root)
	{
	}

	odata_query_path(const odata_query_path& /*path*/)
	:	l_child_item(nullptr), l_sibling_item(nullptr), m_query_options(), m_resource_path(_XPLATSTR("")), m_is_root(false)
	{
	}

	odata_query_path& operator = (const odata_query_path& /*path*/)
	{
		return *this;
	}

public:
	static odata_query_path* creat_query_path(const ::odata::string_t& path, bool is_root = false)
	{
		return new odata_query_path(path, is_root);
	}

	~odata_query_path()
	{
		if (l_sibling_item)
		{
			delete l_sibling_item;
			l_sibling_item = nullptr;
		}

		if (l_child_item)
		{
			delete l_child_item;
			l_child_item = nullptr;
		}
	}

	const ::odata::string_t& get_resource_path() const
	{
		return m_resource_path;
	}

	odata_query_path* top(int count)
	{
		::odata::string_t query_clause = _XPLATSTR("$top=");
		::odata::string_t string_count;
		::odata::common::to_string(count, string_count);
		query_clause += string_count;

		m_query_options.emplace_back(ODATA_QUERY_OPTION_TYPE::TOP_QUERY, query_clause);

		return this;
	}

	odata_query_path* skip(int count)
	{
		::odata::string_t query_clause = _XPLATSTR("$skip=");
		::odata::string_t string_count;
		::odata::common::to_string(count, string_count);
		query_clause += string_count;

		m_query_options.emplace_back(ODATA_QUERY_OPTION_TYPE::SKIP_QUERY, query_clause);

		return this;
	}

	odata_query_path* orderby(const ::odata::string_t& orderby_clause)
	{
		m_query_options.emplace_back(ODATA_QUERY_OPTION_TYPE::ORDER_BY, _XPLATSTR("$orderby=") + orderby_clause);

		return this;
	}

	odata_query_path* filter(const ::odata::string_t& filter_clause)
	{
		m_query_options.emplace_back(ODATA_QUERY_OPTION_TYPE::FILTER_QUERY, _XPLATSTR("$filter=") + filter_clause);

		return this;
	}

	odata_query_path* select(const ::odata::string_t& select_clause)
	{
		m_query_options.emplace_back(ODATA_QUERY_OPTION_TYPE::SELECT_QUERY, _XPLATSTR("$select=") + select_clause);

		return this;
	}

	odata_query_path* key(const ::odata::string_t& key)
	{
		m_resource_path += _XPLATSTR("(") + key + _XPLATSTR(")");

		return this;
	}

	odata_query_path* expand(const ::odata::string_t& expand_path)
	{
		expand(new odata_query_path(expand_path));

		return this;
	}

	odata_query_path* expand(odata_query_path* expand_item)
	{
		if (expand_item)
		{
			expand_item->l_sibling_item = this->l_child_item;
			l_child_item = expand_item;
		}

		return this;
	}

	odata_query_path* search(const ::odata::string_t& search_clause)
	{
		m_query_options.emplace_back(ODATA_QUERY_OPTION_TYPE::SEARCH_QUERY, _XPLATSTR("$search=") + search_clause);

		return this;
	}

	ODATACPP_CLIENT_API ::odata::string_t evaluate_query_path();

private:
	::odata::string_t normalize_query_options();

	odata_query_path* l_child_item;
	odata_query_path* l_sibling_item;

	std::vector<odata_query_option> m_query_options;
	::odata::string_t               m_resource_path;
	bool                            m_is_root;
};

}}
