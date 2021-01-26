//---------------------------------------------------------------------
// <copyright file="odata_query_path.cpp" company="Microsoft">
//      Copyright (C) Microsoft Corporation. All rights reserved. See License.txt in the project root for license information.
// </copyright>
//---------------------------------------------------------------------

#include "odata/common/utility.h"
#include "odata/codegen/odata_query_path.h"

using namespace ::odata::common;

namespace odata { namespace codegen {

::odata::string_t odata_query_path::normalize_query_options()
{
	if (!m_query_options.empty())
	{
		std::vector<odata_query_option> query_options = m_query_options;
		std::sort(query_options.begin(), query_options.end(), odata_query_option_comparator());
		m_query_options = query_options;

		::odata::string_t ss;

		bool first = true;
		for (auto iter = m_query_options.cbegin(); iter != m_query_options.cend(); ++iter)
		{
			if (!first)
			{
				ss += m_is_root ? _XPLATSTR("&") : _XPLATSTR(";");
			}
			else
			{
				first = false;
			}

			ss += iter->get_query_option_clause();
		}

		return std::move(ss);
	}
	else
	{
		return _XPLATSTR("");
	}
}

::odata::string_t odata_query_path::evaluate_query_path()
{
	::odata::string_t ss(m_resource_path);

	if (!m_query_options.empty() || l_child_item)
	{
		ss += m_is_root ? _XPLATSTR("?") : _XPLATSTR("(");
	}

	ss += normalize_query_options();

	if (l_child_item)
	{
		if (!m_query_options.empty())
		{
			ss += m_is_root ? _XPLATSTR("&") : _XPLATSTR(";");
		}

		ss += _XPLATSTR("$expand=");

		bool first = true;

		odata_query_path* child = l_child_item;
		while(child)
		{
			if (!first)
			{
				ss += _XPLATSTR(",");
			}
			else
			{
				first = false;
			}

			ss += child->evaluate_query_path();

			child = child->l_sibling_item;
		}
	}

	if (!m_is_root && (!m_query_options.empty() || l_child_item))
	{
		ss += _XPLATSTR(")");
	}

	return std::move(ss);
}

}}
