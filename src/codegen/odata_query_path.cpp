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
 
#include "odata/common/utility.h"
#include "odata/codegen/odata_query_path.h"

using namespace ::odata::common;

namespace odata { namespace codegen { 

::utility::string_t odata_query_path::normalize_query_options()
{
	if (m_query_options.size() > 0)
	{
		std::sort(m_query_options.begin(), m_query_options.end(), odata_query_option_comparator());

		::utility::stringstream_t ostr;

		bool first = true;
		for (auto iter = m_query_options.cbegin(); iter != m_query_options.cend(); iter++)
		{
			if (!first)
			{
				if (m_is_root)
					ostr << U("&");
				else
					ostr << U(";");
			}
			else
			{
				first = false;
			}
			
			ostr << iter->get_query_option_clause();	
		}

		return std::move(ostr.str());
	}
	else
	{
		return U("");
	}
}

::utility::string_t odata_query_path::evaluate_query_path()
{
	::utility::stringstream_t ostr;

	ostr << m_resource_path;

	if (m_is_root && (m_query_options.size() > 0 || l_child_item))
	{
		ostr << U("?");
	}

	if (!m_is_root && (m_query_options.size() > 0 || l_child_item))
		ostr << U("(");

	ostr << normalize_query_options();

	if (l_child_item)
	{
		if (m_query_options.size() > 0)
		{
			if (m_is_root)
				ostr << U("&");
			else
				ostr << U(";");
		}

		ostr << U("$expand=");

		bool first = true;

		odata_query_path* child = l_child_item;
		while(child)
		{
			if (!first)
			{
				ostr << U(",");
			}
			else
			{
				first = false;
			}
			
			ostr << child->evaluate_query_path();

			child = child->l_sibling_item;
		}
	}

	if (!m_is_root && (m_query_options.size() > 0 || l_child_item))
		ostr << U(")");

	return std::move(ostr.str());
}

}}
