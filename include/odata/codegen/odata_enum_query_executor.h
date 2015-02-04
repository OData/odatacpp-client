//---------------------------------------------------------------------
// <copyright file="odata_enum_query_executor.h" company="Microsoft">
//      Copyright (C) Microsoft Corporation. All rights reserved. See License.txt in the project root for license information.
// </copyright>
//---------------------------------------------------------------------

#pragma once

#include "odata/client/odata_client.h"
#include "odata/common/utility.h"
#include "odata/core/odata_core.h"
#include "odata/codegen/odata_service_context.h"
#include "cpprest/uri.h"
#include "cpprest/asyncrt_utils.h"
#include "cpprest/json.h"
#include "cpprest/http_client.h"

namespace odata { namespace codegen {

template<typename ElementType, typename Resolver>
class odata_enum_query_executor
{
public:
	odata_enum_query_executor(std::shared_ptr<::odata::codegen::odata_service_context> client_context) : m_client_context(client_context)
	{
	}

	typedef typename std::vector<ElementType> return_type;

	::pplx::task<return_type> execute_query(const ::utility::string_t& query_expression)
	{
		if (!m_client_context || !m_client_context->get_client())
		{
			return ::pplx::task_from_result(std::vector<ElementType>());
		}

		return m_client_context->get_client()->get_data_from_server(query_expression).then(
            [this] (const std::vector<std::shared_ptr<odata::core::odata_value>>& values) -> std::vector<ElementType>
			{
				std::vector<ElementType> vec;

				for(auto iter = values.cbegin(); iter != values.cend(); iter++)
				{
					auto enum_value = std::dynamic_pointer_cast<::odata::core::odata_enum_value>(*iter);
					if (enum_value)
					{
						ElementType _value = Resolver::get_enum_type_from_string(enum_value->to_string(), ElementType());
						vec.push_back(_value);
					}
				}
			
				return std::move(vec); 
			});
	}

	::pplx::task<return_type> execute_operation_query(const ::utility::string_t& query_expression, const std::vector<std::shared_ptr<::odata::core::odata_parameter>>& parameters, bool is_function)
	{
		if (!m_client_context || !m_client_context->get_client())
		{
			return ::pplx::task_from_result(return_type());
		}

		auto client_context = m_client_context;
		std::vector<std::shared_ptr<::odata::core::odata_value>> ret_values;
		m_client_context->get_client()->send_data_to_server(query_expression, parameters, ret_values, is_function ? HTTP_GET : HTTP_POST).get();

		std::vector<ElementType> vec;
		for(auto iter = ret_values.cbegin(); iter != ret_values.cend(); iter++)
		{
			auto enum_value = std::dynamic_pointer_cast<::odata::core::odata_enum_value>(*iter);
			if (enum_value)
			{
				ElementType _value = Resolver::get_enum_type_from_string(enum_value->to_string(), ElementType());
				vec.push_back(_value);
			}
		}
			
		return ::pplx::task_from_result(std::move(vec)); 
	}

protected:
	std::shared_ptr<::odata::codegen::odata_service_context> m_client_context;
};

}}
