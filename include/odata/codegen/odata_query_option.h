//---------------------------------------------------------------------
// <copyright file="odata_query_option.h" company="Microsoft">
//      Copyright (C) Microsoft Corporation. All rights reserved. See License.txt in the project root for license information.
// </copyright>
//---------------------------------------------------------------------

#pragma once

#include "odata/common/utility.h"
#include "odata/core/odata_core.h"

namespace odata { namespace codegen { 

enum ODATA_QUERY_OPTION_TYPE
{
	KEY_QUERY = 0,
	FILTER_QUERY,
	TOP_QUERY,
	SKIP_QUERY,
	ORDER_BY,
	SELECT_QUERY,
	EXPAND_QUERY,
};

class odata_query_option
{
public:
	odata_query_option(ODATA_QUERY_OPTION_TYPE query_option_type, ::utility::string_t query_option_clause)
		: m_query_option_type(query_option_type), m_query_option_clause(std::move(query_option_clause))
	{
	}

	const ODATA_QUERY_OPTION_TYPE& get_query_option_type() const
	{
		return m_query_option_type;
	}

	const ::utility::string_t& get_query_option_clause() const
	{
		return m_query_option_clause;
	}

	void set_query_option_clause(::utility::string_t query_option_clause)
	{
		m_query_option_clause = std::move(query_option_clause);
	}

private:
	ODATA_QUERY_OPTION_TYPE    m_query_option_type;
	::utility::string_t m_query_option_clause;
};

class odata_query_option_comparator
{
public:
	bool operator()(const odata_query_option& left, const odata_query_option& right)
	{
		return left.get_query_option_type() < right.get_query_option_type();
	}
};

}}