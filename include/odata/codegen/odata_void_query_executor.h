//---------------------------------------------------------------------
// <copyright file="odata_void_query_executor.h" company="Microsoft">
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

class odata_void_query_executor
{
public:
	odata_void_query_executor(std::shared_ptr<::odata::codegen::odata_service_context> client_context) : m_client_context(client_context)
	{
	}

	typedef std::pair<int, ::odata::client::http_result> return_type;

	::pplx::task<return_type> execute_operation_query(const ::odata::string_t& query_expression, const std::vector<std::shared_ptr<::odata::core::odata_parameter>>& parameters, bool is_function)
	{
		if (!m_client_context || !m_client_context->get_client())
		{
			return ::pplx::task_from_result(std::make_pair(-1, ::odata::client::http_result(::web::http::http_headers(), ::web::http::status_code(-1))));
		}

		std::vector<std::shared_ptr<::odata::core::odata_value>> ret_values;
		auto status_code = m_client_context->get_client()->send_data_to_server(query_expression, parameters, ret_values, is_function ? HTTP_GET : HTTP_POST).get().second;

		return ::pplx::task_from_result(std::make_pair(0, std::move(status_code)));
	}

protected:
	std::shared_ptr<::odata::codegen::odata_service_context> m_client_context;
};

}}
