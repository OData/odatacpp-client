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

#include "odata/client/odata_client.h"
#include "odata/codegen/odata_query_builder.h"
#include "cpprest/uri.h"
#include "cpprest/asyncrt_utils.h"
#include "cpprest/json.h"
#include "cpprest/http_client.h"
#include "odata/common/utility.h"
#include "odata/core/odata_core.h"

namespace odata { namespace codegen { 

class odata_service_context;

template<typename Executor, typename Builder>
class odata_service_query : public std::enable_shared_from_this<odata_service_query<Executor, Builder>>
{
public: 
	odata_service_query(const ::utility::string_t& query_root, std::shared_ptr<odata_service_context> client_context) 
		: m_client_context(client_context)
	{
		m_query_builder = std::make_shared<Builder>(query_root);
		m_query_executor = std::make_shared<Executor>(client_context);
	}

	::pplx::task<typename Executor::return_type> execute_query()
	{
	    if (m_query_executor && m_query_builder)
		{
			return m_query_executor->execute_query(m_query_builder->get_query_expression());
		}

		return ::pplx::task_from_result(typename Executor::return_type());
	}

	::pplx::task<typename Executor::return_type> execute_paged_query(::utility::string_t& next_link_url)
	{
	    if (m_query_executor && m_query_builder)
		{
			return m_query_executor->execute_paged_query(next_link_url);
		}

		return ::pplx::task_from_result(typename Executor::return_type());
	}

	// for function and action
	::pplx::task<typename Executor::return_type> execute_operation_query(const std::vector<std::shared_ptr<::odata::core::odata_parameter>>& parameters, bool is_function)
	{
	    if (m_query_executor && m_query_builder)
		{
			return m_query_executor->execute_operation_query(m_query_builder->get_query_expression(), parameters, is_function);
		}

		return ::pplx::task_from_result(typename Executor::return_type());
	}

	void set_query_builder(const std::shared_ptr<Builder>& builder)
	{
		m_query_builder = builder;
	}

	::utility::string_t get_query_expression()
	{
		if (m_query_builder)
		{
			return m_query_builder->get_query_expression();
		}

		return U("");
	}

	std::shared_ptr<odata_service_query<Executor, Builder>> top(int count)
	{
		if (m_query_builder)
		{
			m_query_builder->top(count);
		}

		return this->shared_from_this();
	}

	std::shared_ptr<odata_service_query<Executor, Builder>> key(const ::utility::string_t& key_clause)
	{
		if (m_query_builder)
		{
			m_query_builder->key(key_clause);
		}

		return this->shared_from_this();
	}

	std::shared_ptr<odata_service_query<Executor, Builder>> skip(int count)
	{
		if (m_query_builder)
		{
			m_query_builder->skip(count);
		}

		return this->shared_from_this();
	}

	std::shared_ptr<odata_service_query<Executor, Builder>> filter(const ::utility::string_t& filter_clause)
	{
		if (m_query_builder)
		{
			m_query_builder->filter(filter_clause);
		}

		return this->shared_from_this();
	}

	std::shared_ptr<odata_service_query<Executor, Builder>> select(const ::utility::string_t& select_clause)
	{
		if (m_query_builder)
		{
			m_query_builder->select(select_clause);
		}

		return this->shared_from_this();
	}

	std::shared_ptr<odata_service_query<Executor, Builder>> expand(const ::utility::string_t& expand_clause)
	{
		if (m_query_builder)
		{
			m_query_builder->expand(expand_clause);
		}

		return this->shared_from_this();
	}

	std::shared_ptr<odata_service_query<Executor, Builder>> expand(odata_query_path* expand_path_item)
	{
		if (m_query_builder)
		{
			m_query_builder->expand(expand_path_item);
		}

		return this->shared_from_this();
	}

	std::shared_ptr<odata_service_query<Executor, Builder>> orderby(const ::utility::string_t& orderby_clause)
	{
		if (m_query_builder)
		{
			m_query_builder->orderby(orderby_clause);
		}

		return this->shared_from_this();
	}

protected:
	std::shared_ptr<odata_service_context> m_client_context;
	std::shared_ptr<Executor>              m_query_executor;
	std::shared_ptr<Builder>               m_query_builder;
};

}}