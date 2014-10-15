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
#include "odata/codegen/odata_query_option.h"
#include "odata/core/odata_core.h"

namespace odata { namespace codegen { 

class odata_query_path
{
private:
    odata_query_path(::utility::string_t path, bool is_root = false) 
		: m_resource_path(std::move(path)), m_is_root(is_root), l_child_item(nullptr), l_sibling_item(nullptr)
	{
	}

	odata_query_path(const odata_query_path& path)
	{

	}

	odata_query_path& operator = (const odata_query_path& path)
	{
		return *this;
	}

public:
	static odata_query_path* creat_query_path(const ::utility::string_t& path, bool is_root = false)
	{
		return new odata_query_path(path, is_root);
	}

	~odata_query_path()
	{
		if (l_sibling_item)
		{
			delete l_sibling_item;
		}

		if (l_child_item)
		{
			delete l_child_item;
		}

		l_sibling_item = nullptr;
		l_child_item = nullptr;
	}

	const ::utility::string_t& get_resource_path() const 
	{
		return m_resource_path;
	}
       
    odata_query_path* top(int count)
	{
		::utility::string_t query_clause = U("$top=");
		::utility::string_t string_count;
		::odata::common::to_string(count, string_count);
		query_clause += string_count;

		m_query_options.push_back(odata_query_option(ODATA_QUERY_OPTION_TYPE::TOP_QUERY, query_clause));

		return this;
	}

	odata_query_path* skip(int count)
	{
		::utility::string_t query_clause = U("$skip=");
		::utility::string_t string_count;
		::odata::common::to_string(count, string_count);
		query_clause += string_count;

		m_query_options.push_back(odata_query_option(ODATA_QUERY_OPTION_TYPE::SKIP_QUERY, query_clause));

		return this;
	}

    odata_query_path* orderby(const ::utility::string_t& orderby_clause)
	{
		m_query_options.push_back(odata_query_option(ODATA_QUERY_OPTION_TYPE::ORDER_BY, U("$orderby=") + orderby_clause));

		return this;
	}

    odata_query_path* filter(const ::utility::string_t& filter_clause)
	{
		m_query_options.push_back(odata_query_option(ODATA_QUERY_OPTION_TYPE::FILTER_QUERY, U("$filter=") + filter_clause));

		return this;
	}

    odata_query_path* select(const ::utility::string_t& select_clause)
	{
		m_query_options.push_back(odata_query_option(ODATA_QUERY_OPTION_TYPE::SELECT_QUERY, U("$select=") + select_clause));

		return this;
	}

	odata_query_path* key(const ::utility::string_t& key)
	{
		m_resource_path += U("(") + key + U(")");

		return this;
	}

	odata_query_path* expand(const ::utility::string_t& expand_path)
	{
		expand(new odata_query_path(expand_path));

		return this;
	}

    odata_query_path* expand(odata_query_path* expand_item)
	{
		if (expand_item)
		{
			expand_item->l_sibling_item = this->l_child_item;
			this->l_child_item = expand_item;
		}
		
		return this;
	}

	ODATACPP_CLIENT_API ::utility::string_t evaluate_query_path();

private:
	::utility::string_t normalize_query_options();

    odata_query_path* l_child_item;
    odata_query_path* l_sibling_item;

	std::vector<odata_query_option> m_query_options;
    ::utility::string_t      m_resource_path;
	bool                            m_is_root;
};

}}