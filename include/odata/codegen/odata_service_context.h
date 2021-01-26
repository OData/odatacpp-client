//---------------------------------------------------------------------
// <copyright file="odata_service_context.h" company="Microsoft">
//      Copyright (C) Microsoft Corporation. All rights reserved. See License.txt in the project root for license information.
// </copyright>
//---------------------------------------------------------------------

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
	odata_service_context(const ::odata::string_t& baseAddress, ::odata::client::client_options options =  ::odata::client::client_options())
		: m_client(::odata::make_shared<::odata::client::odata_client>(baseAddress, options)), m_model()
	{
	}

	virtual ~odata_service_context() = default;

	const std::shared_ptr<::odata::edm::edm_model>& get_edm_model()
	{
		if (!m_model)
		{
			m_model = m_client->get_model().get().first;
		}

		return m_model;
	}

	const std::shared_ptr<::odata::client::odata_client>& get_client() const
	{
		return m_client;
	}

	const ::odata::string_t& get_root_url() const
	{
		return m_client->get_service_root_url();
	}

	::odata::string_t get_relative_path(const ::odata::string_t& full_path) const
	{
		return m_client->get_relative_path(full_path);
	}

	template<typename Executor, typename Builder>
	std::shared_ptr<odata_service_query<Executor, Builder>> create_query(const ::odata::string_t& resource_path)
	{
		return ::odata::make_shared<odata_service_query<Executor, Builder>>(resource_path, shared_from_this());
	}

	template<typename Type>
	::pplx::task<std::pair<nullptr_t, ::odata::client::http_result>> add_object(const ::odata::string_t& path, const std::shared_ptr<Type>& p_object)
	{
		auto model = m_client->get_model().get().first;

		if (m_client && model && p_object)
		{
			auto entity_value = std::dynamic_pointer_cast<::odata::core::odata_entity_value>(p_object->to_value());
			if (!entity_value)
			{
				return ::pplx::task_from_result(std::make_pair(nullptr, ::odata::client::http_result(::web::http::http_headers(), ::web::http::status_code(-1))));
			}

			entity_value->set_value(::odata::core::odata_json_constants::PAYLOAD_ANNOTATION_TYPE, Type::get_full_name());

			return m_client->create_entity(path, entity_value).then(
				[this, entity_value, p_object] (const std::pair<nullptr_t, ::odata::client::http_result>& ret_code) -> std::pair<nullptr_t, ::odata::client::http_result>
				{
					::odata::string_t edit_link;
					if (entity_value->try_get(::odata::core::odata_json_constants::PAYLOAD_ANNOTATION_EDITLINK, edit_link))
					{
						p_object->set_edit_link(edit_link);
					}

					return ret_code;
				});
		}

		return ::pplx::task_from_result(std::make_pair(nullptr, ::odata::client::http_result(::web::http::http_headers(), ::web::http::status_code(-1))));
	}

	template<typename Type>
	::pplx::task<std::pair<nullptr_t, ::odata::client::http_result>> update_object(const std::shared_ptr<Type>& p_object, const ::odata::string_t &update_type = HTTP_PATCH)
	{
		if (m_client && p_object)
		{
			return m_client->send_data_to_server(::web::http::uri::encode_uri(get_relative_path(p_object->get_edit_link())), p_object->to_value(), update_type);
		}

		return ::pplx::task_from_result(std::make_pair(nullptr, ::odata::client::http_result(::web::http::http_headers(), ::web::http::status_code(-1))));
	}

	template<typename Type>
	::pplx::task<std::pair<nullptr_t, ::odata::client::http_result>> update_object(const std::vector<std::shared_ptr<Type>>& objects, const ::odata::string_t &update_type = HTTP_PATCH)
	{
		if (m_client && !objects.empty())
		{
			auto entity_type = m_model->find_entity_type(objects.front()->get_type_name());
			if (entity_type)
			{
				auto collection_type = ::odata::make_shared<::odata::edm::edm_collection_type>(_XPLATSTR(""), entity_type);
				auto Collection      = ::odata::make_shared<::odata::core::odata_collection_value>(collection_type);
				for (auto &p_object : objects)
				{
					Collection->add_collection_value(p_object->to_value());
				}
				::odata::string_t link = objects.front()->get_edit_link();
				::odata::string_t key  = objects.front()->get_key_property_string();
				size_t            keypos = link.rfind(key);
				if ((keypos >= 0) && (keypos != ::odata::string_t::npos))
				{
					link.erase(keypos - 1);
				}
				return m_client->send_data_to_server(::web::http::uri::encode_uri(get_relative_path(link)), Collection, update_type);
			}
		}

		return ::pplx::task_from_result(std::make_pair(nullptr, ::odata::client::http_result(::web::http::http_headers(), ::web::http::status_code(-1))));
	}

	template<typename Type>
	::pplx::task<std::pair<nullptr_t, ::odata::client::http_result>> delete_object(const std::shared_ptr<Type>& p_object)
	{
		if (m_client && p_object)
		{
			return m_client->send_data_to_server(::web::http::uri::encode_uri(get_relative_path(p_object->get_edit_link())), HTTP_DELETE);
		}

		return ::pplx::task_from_result(std::make_pair(nullptr, ::odata::client::http_result(::web::http::http_headers(), ::web::http::status_code(-1))));
	}

	// @TODO: How can we make a multiple instance delete???

	template<typename ParentType, typename Type>
	::pplx::task<std::pair<nullptr_t, ::odata::client::http_result>> add_related_object(const std::shared_ptr<ParentType>& p_parent, const ::odata::string_t& path, const std::shared_ptr<Type>& p_object)
	{
		auto &model = get_edm_model();
		if (!p_parent || !model || !p_object)
		{
			return ::pplx::task_from_result(std::make_pair(nullptr, ::odata::client::http_result(::web::http::http_headers(), ::web::http::status_code(-1))));
		}

		return add_object(get_relative_path(p_parent->get_edit_link() + _XPLATSTR("/") + path), p_object);
	}

	template<typename ParentType, typename Type>
	::pplx::task<std::pair<nullptr_t, ::odata::client::http_result>> update_related_object(const std::shared_ptr<ParentType>& p_parent, const ::odata::string_t& path, const std::shared_ptr<Type>& p_object)
	{
		if (!p_parent)
		{
			return ::pplx::task_from_result(std::make_pair(nullptr, ::odata::client::http_result(::web::http::http_headers(), ::web::http::status_code(-1))));
		}

		auto full_navigation_path = p_parent->get_edit_link() + _XPLATSTR("/") + path;
		auto navigation_path = get_relative_path(full_navigation_path);

		if (m_client && p_object)
		{
			return m_client->send_data_to_server(::web::http::uri::encode_uri(get_relative_path(navigation_path)), p_object->to_value(), HTTP_PATCH).then
				( [this, p_object, full_navigation_path] (::odata::client::http_result ret_code) -> std::pair<nullptr_t, ::odata::client::http_result>
				{
					if (p_object)
					{
						p_object->set_edit_link(full_navigation_path);
					}

					return std::make_pair(nullptr, ret_code);
				});
		}

		return ::pplx::task_from_result(std::make_pair(nullptr, ::odata::client::http_result(::web::http::http_headers(), ::web::http::status_code(-1))));
	}

	template<typename ParentType, typename Type>
	::pplx::task<std::pair<nullptr_t, ::odata::client::http_result>> add_reference(const std::shared_ptr<ParentType>& p_parent, const ::odata::string_t& path, const std::shared_ptr<Type>& p_object)
	{
		if (m_client && p_parent && p_object)
		{
			return m_client->add_reference(get_relative_path(p_parent->get_edit_link() + _XPLATSTR("/") + path), p_object->get_edit_link());
		}

		return ::pplx::task_from_result(std::make_pair(nullptr, ::odata::client::http_result(::web::http::http_headers(), ::web::http::status_code(-1))));
	}

	template<typename ParentType, typename Type>
	::pplx::task<std::pair<nullptr_t, ::odata::client::http_result>> update_reference(const std::shared_ptr<ParentType>& p_parent, const ::odata::string_t& path, const std::shared_ptr<Type>& p_object)
	{
		if (m_client && p_parent && p_object)
		{
			return m_client->update_reference(get_relative_path(p_parent->get_edit_link() + _XPLATSTR("/") + path), p_object->get_edit_link());
		}

		return ::pplx::task_from_result(std::make_pair(nullptr, ::odata::client::http_result(::web::http::http_headers(), ::web::http::status_code(-1))));
	}

protected:
	std::shared_ptr<::odata::client::odata_client> m_client;
	std::shared_ptr<::odata::edm::edm_model>       m_model;
};

}}
