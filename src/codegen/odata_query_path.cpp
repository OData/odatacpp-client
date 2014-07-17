#include "odata/common/utility.h"
#include "odata/codegen/odata_query_path.h"

using namespace ::odata::utility;

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
