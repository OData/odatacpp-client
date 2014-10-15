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
#include "odata/codegen/code_generation_base.h"
#include "odata/codegen/odata_service_query.h"
#include "odata/common/utility.h"
#include "odata/core/odata_core.h"
#include "odata/edm/edm_model_utility.h"

namespace odata { namespace codegen { 

class odata_service_context : public std::enable_shared_from_this<odata_service_context>
{
public:
	odata_service_context(const ::utility::string_t& baseAddress, ::odata::client::client_options options =  ::odata::client::client_options())
	{
		m_client = std::make_shared<::odata::client::odata_client>(baseAddress, options);
		m_model = nullptr;
	}

    virtual ~odata_service_context(){}

	const std::shared_ptr<::odata::edm::edm_model>& get_edm_model() 
	{
		if (!m_model)
		{
			m_model = m_client->get_model().get();
		}

		return m_model;
	}

	const std::shared_ptr<::odata::client::odata_client>& get_client() const
	{
		return m_client;
	}

	const ::utility::string_t& get_root_url()
	{
		return m_client->get_service_root_url();
	}

	::utility::string_t get_relative_path(const ::utility::string_t& full_path)
	{
		return m_client->get_relative_path(full_path);
	}

	template<typename Executor, typename Builder>
	std::shared_ptr<odata_service_query<Executor, Builder>> create_query(const ::utility::string_t& resource_path)
	{
		return std::make_shared<odata_service_query<Executor, Builder>>(resource_path, shared_from_this());
	}

    template<typename Type>
	::pplx::task<::web::http::status_code> add_object(const ::utility::string_t& path, const std::shared_ptr<Type>& p_object)
	{
		auto model = m_client->get_model().get();

		if (m_client && model && p_object)
		{
			auto entity_value = std::dynamic_pointer_cast<::odata::core::odata_entity_value>(p_object->to_value());
			if (!entity_value)
			{
				return ::pplx::task_from_result(::web::http::status_code(-1));
			}

			entity_value->set_value(::odata::core::odata_json_constants::PAYLOAD_ANNOTATION_TYPE, Type::get_full_name());

			return m_client->create_entity(path, entity_value).then(
				[this, entity_value, p_object] (const ::web::http::status_code& ret_code) -> ::web::http::status_code
			    {
					::utility::string_t edit_link;
					if (entity_value->try_get(::odata::core::odata_json_constants::PAYLOAD_ANNOTATION_EDITLINK, edit_link))
					{
						p_object->set_edit_link(edit_link);
					}

					return ret_code;
			    });
		}

		return ::pplx::task_from_result(::web::http::status_code(-1));
	}

	template<typename Type>
	::pplx::task<::web::http::status_code> update_object(const std::shared_ptr<Type>& p_object, const ::utility::string_t &update_type = HTTP_PATCH)
	{
		if (m_client && p_object)
		{
			return m_client->send_data_to_server(get_relative_path(p_object->get_edit_link()), p_object->to_value(), update_type);
		}

		return ::pplx::task_from_result(::web::http::status_code(-1));
	}

	template<typename Type>
    ::pplx::task<::web::http::status_code> delete_object(const std::shared_ptr<Type>& p_object)
	{
		if (m_client && p_object)
		{
			return m_client->send_data_to_server(get_relative_path(p_object->get_edit_link()), HTTP_DELETE);
		}

		return ::pplx::task_from_result(::web::http::status_code(-1));
	}

	template<typename ParentType, typename Type>
	::pplx::task<::web::http::status_code> add_related_object(const std::shared_ptr<ParentType>& p_parent, const ::utility::string_t& path,  const std::shared_ptr<Type>& p_object)
	{
		auto model = get_edm_model();
		if (!p_parent || !model || !p_object) 
		{
			return ::pplx::task_from_result(::web::http::status_code(-1));
		}

		return add_object(get_relative_path(p_parent->get_edit_link() + U("/") + path), p_object);
	}

	template<typename ParentType, typename Type>
	::pplx::task<::web::http::status_code> update_related_object(const std::shared_ptr<ParentType>& p_parent, const ::utility::string_t& path,  const std::shared_ptr<Type>& p_object)
	{
		if (!p_parent)
		{
			return ::pplx::task_from_result(::web::http::status_code(-1));
		}
		
		auto full_navigation_path = p_parent->get_edit_link() + U("/") + path;
		auto navigation_path = get_relative_path(full_navigation_path);

		if (m_client && p_object)
		{
			return m_client->send_data_to_server(get_relative_path(navigation_path), p_object->to_value(), HTTP_PATCH).then
				( [this, p_object, full_navigation_path] (::web::http::status_code ret_code) -> ::web::http::status_code
			    {
					if (p_object)
					{
						p_object->set_edit_link(full_navigation_path);
					}

					return ret_code;
			    });
		}

		return ::pplx::task_from_result(::web::http::status_code(-1));
	}

	template<typename ParentType, typename Type>
	::pplx::task<::web::http::status_code> add_reference(const std::shared_ptr<ParentType>& p_parent, const ::utility::string_t& path, const std::shared_ptr<Type>& p_object)
	{
		if (m_client && p_parent && p_object)
		{
			return m_client->add_reference(get_relative_path(p_parent->get_edit_link() + U("/") + path), p_object->get_edit_link());
		}

		return ::pplx::task_from_result(::web::http::status_code(-1));
	}

	template<typename ParentType, typename Type>
	::pplx::task<::web::http::status_code> update_reference(const std::shared_ptr<ParentType>& p_parent, const ::utility::string_t& path, const std::shared_ptr<Type>& p_object)
	{
		if (m_client && p_parent && p_object)
		{
			return m_client->update_reference(get_relative_path(p_parent->get_edit_link() + U("/") + path), p_object->get_edit_link());
		}

		return ::pplx::task_from_result(::web::http::status_code(-1));
	}

protected:
	std::shared_ptr<::odata::client::odata_client>         m_client;
	std::shared_ptr<::odata::edm::edm_model>               m_model;
};

}}