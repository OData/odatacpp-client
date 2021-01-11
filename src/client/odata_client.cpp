//---------------------------------------------------------------------
// <copyright file="odata_client.cpp" company="Microsoft">
//      Copyright (C) Microsoft Corporation. All rights reserved. See License.txt in the project root for license information.
// </copyright>
//---------------------------------------------------------------------

#include "odata/client/odata_client.h"
#include "odata/core/odata_json_reader.h"
#include "odata/core/odata_json_writer.h"
#include "odata/core/odata_json_operation_url_parameter_writer.h"
#include "odata/core/odata_json_operation_payload_parameter_writer.h"
#include "odata/core/odata_entity_factory.h"

using namespace ::web;
using namespace ::odata::communication;
using namespace ::odata::core;
using namespace ::odata::edm;

namespace odata { namespace client {

std::unordered_map<::odata::string_t, std::shared_ptr<edm_model>> odata_client::m_model_cache;

pplx::task<std::pair<std::shared_ptr<edm_model>, http_result>> odata_client::get_model()
{
	if (m_model)
	{
		return pplx::task_from_result(std::make_pair(m_model, http_result(200U)));
	}

	auto find_iter = m_model_cache.find(m_service_root_url);
	if (find_iter != m_model_cache.end())
	{
		m_model = find_iter->second;
		return pplx::task_from_result(std::make_pair(m_model, http_result(200U)));
	}

	return m_client_proxy->send_http_request(HTTP_GET, _XPLATSTR("$metadata"), _XPLATSTR("application/xml")).then(
		[this] (const http::http_response& response) -> std::pair<std::shared_ptr<edm_model>, http_result>
		{
			doLog(__FUNCTIONW__, __FILEW__, __LINE__, response);

			m_model_response_headers = response.headers();

			if (!http_utility::is_successful_status_code(response.status_code()))
			{
				throw service_exception(response.to_string());
			}

			m_model = schema_from_response(response);
			m_model_cache[m_service_root_url] = m_model;

			return std::make_pair(m_model, http_result(response));
		});
}

std::shared_ptr<edm_model> odata_client::schema_from_response(const http_client_response& response)
{
	auto reader = ::odata::make_shared<edm_model_reader>(response.body());

	if (reader)
	{
		reader->parse();
		return reader->get_model();
	}

	return nullptr;
}

std::shared_ptr<odata_payload> odata_client::entity_payload_from_response(const http_client_response& response, const std::shared_ptr<edm_entity_set>& /*set*/)
{
	if (response.headers().content_type().compare(0, 16, _XPLATSTR("application/json")) == 0)
	{
		auto reader = entity_factory<odata_json_reader>::create_reader_instance(m_model, m_service_root_url);

		return reader->deserialize(response.extract_json().get());
	}
	else
	{
		throw std::runtime_error("Atom payload not supported!");
	}
}

std::shared_ptr<odata_payload> odata_client::parse_payload_from_response(const http_client_response& response)
{
	if (response.headers().content_type().compare(0,16, _XPLATSTR("application/json")) == 0)
	{
		auto reader = entity_factory<odata_json_reader>::create_reader_instance(m_model, m_service_root_url);

		return reader->deserialize(response.extract_json().get());
	}
	else
	{
		//throw std::runtime_error("Atom payload not supported!");
	}

	return ::odata::make_shared<odata_payload>();
}

pplx::task<std::pair<std::vector<std::shared_ptr<odata_entity_value>>, http_result>> odata_client::get_entities(const ::odata::string_t& edm_entity_set)
{
	if (!m_model)
	{
		return get_model().then([this, edm_entity_set](std::pair<std::shared_ptr<edm_model>, http_result> model){
			return get_entities(edm_entity_set);
		});
	}

	auto ecnt_md = m_model->find_container();
	auto eset_md = ecnt_md ? ecnt_md->find_entity_set(edm_entity_set) : nullptr;

	return get_entities(eset_md, edm_entity_set);
}

pplx::task<std::pair<std::vector<std::shared_ptr<odata_entity_value>>, http_result>> odata_client::get_entities(const std::shared_ptr<edm_entity_set>& edm_entity_set, const ::odata::string_t& path)
{
	if (!m_model)
	{
		return get_model().then([this, edm_entity_set, path](std::pair<std::shared_ptr<edm_model>, http_result> model){
			return get_entities(edm_entity_set, path);
		});
	}

	::odata::string_t accept = _XPLATSTR("application/json");
	if (!m_options.m_json_mdlevel.empty())
	{
		accept.append(_XPLATSTR(";odata.metadata=")).append(m_options.m_json_mdlevel);
	}

	return m_client_proxy->send_http_request(HTTP_GET, path, accept).then(
		[this, path, edm_entity_set] (const http::http_response& response) -> std::pair<std::vector<std::shared_ptr<odata_entity_value>>, http_result>
		{
			doLog(__FUNCTIONW__, __FILEW__, __LINE__, response);

			if (!http_utility::is_successful_status_code(response.status_code()))
			{
				throw service_exception(response.to_string());
			}

			auto payload = entity_payload_from_response(response, edm_entity_set);
			std::vector<std::shared_ptr<odata_entity_value>> entities;
			entities.reserve(payload->get_values().size());
			for(auto iter = payload->get_values().cbegin(); iter != payload->get_values().cend(); ++iter)
			{
				entities.emplace_back(std::dynamic_pointer_cast<odata_entity_value>(*iter));
			}

			if (!payload->get_next_link().empty())
			{
				auto next_entities = get_entities(edm_entity_set, payload->get_next_link()).get();
				entities.reserve(entities.size() + next_entities.first.size());
				std::move(next_entities.first.cbegin(), next_entities.first.cend(), std::back_inserter(entities));
			}

			return std::make_pair(std::move(entities), http_result(response));
		});
}

pplx::task<std::pair<nullptr_t, http_result>> odata_client::create_entity(const ::odata::string_t& entity_set_name, const std::shared_ptr<odata_entity_value>& entity_object)
{
	if (!m_model)
	{
		return get_model().then([this, entity_set_name, entity_object](std::pair<std::shared_ptr<edm_model>, http_result> model){
			return create_entity(entity_set_name, entity_object);
		});
	}

	return send_data_to_server(::web::http::uri::encode_uri(entity_set_name), entity_object, HTTP_POST);
}

pplx::task<std::pair<nullptr_t, http_result>> odata_client::add_reference(const ::odata::string_t& path, const ::odata::string_t& referenceEntityId)
{
	if (!m_model)
	{
		return get_model().then([this, path, referenceEntityId](std::pair<std::shared_ptr<edm_model>, http_result> model){
			return add_reference(path, referenceEntityId);
		});
	}

	auto entity = ::odata::make_shared<odata_entity_value>(::odata::make_shared<edm_entity_type>(_XPLATSTR(""), _XPLATSTR("")));
	entity->set_value(odata_json_constants::PAYLOAD_ANNOTATION_ID, ::odata::make_shared<odata_primitive_value>(::odata::make_shared<edm_payload_annotation_type>(odata_json_constants::PAYLOAD_ANNOTATION_ID), referenceEntityId));

	return send_data_to_server(::web::http::uri::encode_uri(path +  _XPLATSTR("/$ref")), std::move(entity), HTTP_POST);
}

pplx::task<std::pair<nullptr_t, http_result>> odata_client::remove_reference(const ::odata::string_t& path, const ::odata::string_t& referenceEntityId)
{
	if (!m_model)
	{
		return get_model().then([this, path, referenceEntityId](std::pair<std::shared_ptr<edm_model>, http_result> model){
			return remove_reference(path, referenceEntityId);
		});
	}

	::odata::string_t ss(path);
	ss += _XPLATSTR("/$ref");
	if (!referenceEntityId.empty())
	{
		ss += _XPLATSTR("?$id=");
		ss += referenceEntityId;
	}

	return send_data_to_server(::web::http::uri::encode_uri(ss), HTTP_DELETE);
}

pplx::task<std::pair<nullptr_t, http_result>> odata_client::update_reference(const ::odata::string_t& path, const ::odata::string_t& referenceEntityId)
{
	if (!m_model)
	{
		return get_model().then([this, path, referenceEntityId](std::pair<std::shared_ptr<edm_model>, http_result> model){
			return update_reference(path, referenceEntityId);
		});
	}

	auto entity = ::odata::make_shared<odata_entity_value>(::odata::make_shared<edm_entity_type>(_XPLATSTR(""), _XPLATSTR("")));
	entity->set_value(odata_json_constants::PAYLOAD_ANNOTATION_ID, ::odata::make_shared<odata_primitive_value>(::odata::make_shared<edm_payload_annotation_type>(odata_json_constants::PAYLOAD_ANNOTATION_ID), referenceEntityId));

	return send_data_to_server(::web::http::uri::encode_uri(path + _XPLATSTR("/$ref")), std::move(entity), HTTP_PUT);
}

pplx::task<std::pair<nullptr_t, http_result>> odata_client::patch_entity(const ::odata::string_t& entity_set_name, const std::shared_ptr<odata_entity_value>& entity_object)
{
	if (!m_model)
	{
		return get_model().then([this, entity_set_name, entity_object](std::pair<std::shared_ptr<edm_model>, http_result> model){
			return patch_entity(entity_set_name, entity_object);
		});
	}

	::odata::string_t key = entity_object->get_entity_key_string();
	::odata::string_t path = entity_set_name + key;

	return send_data_to_server(::web::http::uri::encode_uri(path), entity_object, HTTP_PATCH);
}

pplx::task<std::pair<nullptr_t, http_result>> odata_client::put_entity(const ::odata::string_t& entity_set_name, const std::shared_ptr<odata_entity_value>& entity_object)
{
	if (!m_model)
	{
		return get_model().then([this, entity_set_name, entity_object](std::pair<std::shared_ptr<edm_model>, http_result> model){
			return put_entity(entity_set_name, entity_object);
		});
	}

	::odata::string_t key = entity_object->get_entity_key_string();
	::odata::string_t path = entity_set_name + key;

	return send_data_to_server(::web::http::uri::encode_uri(path), entity_object, HTTP_PUT);
}

pplx::task<std::pair<nullptr_t, http_result>> odata_client::delete_entity(const ::odata::string_t& entity_set_name, const std::shared_ptr<odata_entity_value>& entity_object)
{
	if (!m_model)
	{
		return get_model().then([this, entity_set_name, entity_object](std::pair<std::shared_ptr<edm_model>, http_result> model){
			return delete_entity(entity_set_name, entity_object);
		});
	}

	::odata::string_t key = entity_object->get_entity_key_string();
	::odata::string_t path = entity_set_name + key;

	::odata::string_t accept = _XPLATSTR("application/json");

	return m_client_proxy->send_http_request(HTTP_DELETE, path, accept).then(
		[this, entity_set_name] (const http::http_response& response) -> std::pair<nullptr_t, http_result>
		{
			doLog(__FUNCTIONW__, __FILEW__, __LINE__, response);

			if (!http_utility::is_successful_status_code(response.status_code()))
			{
				throw service_exception(response.to_string());
			}

			return std::make_pair(nullptr, http_result(response));
		});
}

pplx::task<std::pair<std::vector<std::shared_ptr<odata_value>>, http_result>> odata_client::get_data_from_server(const ::odata::string_t& path)
{
	if (!m_model)
	{
		return get_model().then([this, path](std::pair<std::shared_ptr<edm_model>, http_result> model){
			return get_data_from_server(path);
		});
	}

	::odata::string_t accept = _XPLATSTR("application/json");
	if (!m_options.m_json_mdlevel.empty())
	{
		accept.append(_XPLATSTR(";odata.metadata=")).append(m_options.m_json_mdlevel);
	}

	return m_client_proxy->send_http_request(HTTP_GET, path, accept).then(
		[this, path] (const http::http_response& response) -> std::pair<std::vector<std::shared_ptr<odata_value>>, http_result>
		{
			doLog(__FUNCTIONW__, __FILEW__, __LINE__, response);

			if (!http_utility::is_successful_status_code(response.status_code()))
			{
				throw service_exception(response.to_string());
			}

			auto payload = parse_payload_from_response(response);

			::odata::string_t next_link = payload->get_next_link();
			if (!next_link.empty())
			{
				auto next_entities = get_data_from_server(get_relative_path(next_link)).get();
				payload->insert_values(next_entities.first);
			}

			return std::make_pair(payload->get_values(), http_result(response));
		});
}

pplx::task<std::pair<std::shared_ptr<odata_payload>, http_result>> odata_client::get_paged_data_from_server(const ::odata::string_t& path)
{
	if (!m_model)
	{
		return get_model().then([this, path](std::pair<std::shared_ptr<edm_model>, http_result> model){
			return get_paged_data_from_server(path);
		});
	}

	::odata::string_t accept = _XPLATSTR("application/json");
	if (!m_options.m_json_mdlevel.empty())
	{
		accept.append(_XPLATSTR(";odata.metadata=")).append(m_options.m_json_mdlevel);
	}

	return m_client_proxy->send_http_request(HTTP_GET, path, accept).then(
		[this, path] (const http::http_response& response) -> std::pair<std::shared_ptr<odata_payload>, http_result>
		{
			doLog(__FUNCTIONW__, __FILEW__, __LINE__, response);

			if (!http_utility::is_successful_status_code(response.status_code()))
			{
				throw service_exception(response.to_string());
			}

			return std::make_pair(parse_payload_from_response(response), http_result(response));
		});
}

pplx::task<std::pair<nullptr_t, http_result>> odata_client::send_data_to_server(const ::odata::string_t& path, const ::odata::string_t& send_type)
{
	if (!m_model)
	{
		return get_model().then([this, path, send_type](std::pair<std::shared_ptr<edm_model>, http_result> model){
			return send_data_to_server(path, send_type);
		});
	}

	::odata::string_t accept = _XPLATSTR("application/json");

	return m_client_proxy->send_http_request(send_type, path, accept).then(
		[this] (const http::http_response& response) -> std::pair<nullptr_t, http_result>
		{
			doLog(__FUNCTIONW__, __FILEW__, __LINE__, response);

			if (!http_utility::is_successful_status_code(response.status_code()))
			{
				throw service_exception(response.to_string());
			}

			return std::make_pair(nullptr, http_result(response));
		});

}

pplx::task<std::pair<nullptr_t, http_result>> odata_client::send_data_to_server(const ::odata::string_t& path, const std::vector<std::shared_ptr<odata_parameter>>& send_parameters, std::vector<std::shared_ptr<odata_value>>& returned_values, const ::odata::string_t& send_type)
{
	if (send_type != HTTP_POST && send_type != HTTP_GET)
	{
		throw std::invalid_argument("Only Post and Get are allowed as send_type.");
	}

	if (!m_model)
	{
		return get_model().then([this, path, send_parameters, &returned_values, send_type](std::pair<std::shared_ptr<edm_model>, http_result> model)
		{
			return send_data_to_server(path, send_parameters, returned_values, send_type);
		});
	}

	::odata::string_t accept = _XPLATSTR("application/json");

	web::json::value value_context;
	::odata::string_t new_path = path;
	if (send_type == HTTP_POST)
	{
		auto writer = ::odata::make_shared<odata_json_operation_payload_parameter_writer>(m_model, get_options());
		value_context = writer->serialize(send_parameters);
	}
	else if (send_type == HTTP_GET)
	{
		auto writer = ::odata::make_shared<odata_json_operation_url_parameter_writer>(m_model);
		new_path += http::uri::encode_uri(writer->serialize(send_parameters));
	}

	if (value_context.is_null())
	{
		auto response = m_client_proxy->send_http_request(send_type, new_path, accept).get();

		doLog(__FUNCTIONW__, __FILEW__, __LINE__, response);

		if (!http_utility::is_successful_status_code(response.status_code()))
		{
			throw service_exception(response.to_string());
		}

		if (response.status_code() != 204)
		{
			returned_values = parse_payload_from_response(response)->get_values();
		}

		return pplx::task_from_result(std::make_pair(nullptr, http_result(response)));
	}
	else
	{
		auto response = m_client_proxy->send_http_request(send_type, new_path, accept, value_context).get();

		doLog(__FUNCTIONW__, __FILEW__, __LINE__, response);

		if (!http_utility::is_successful_status_code(response.status_code()))
		{
			throw service_exception(response.to_string());
		}

		if (response.status_code() != 204)
		{
			returned_values = parse_payload_from_response(response)->get_values();
		}

		return pplx::task_from_result(std::make_pair(nullptr, http_result(response)));
	}
}


pplx::task<std::pair<nullptr_t, http_result>> odata_client::send_data_to_server(const ::odata::string_t& path, const std::shared_ptr<odata_value>& send_value, const ::odata::string_t& send_type)
{
	if (!m_model)
	{
		return get_model().then([this, path, send_value, send_type](std::pair<std::shared_ptr<edm_model>, http_result> model){
			return send_data_to_server(path, send_value, send_type);
		});
	}

	::odata::string_t accept = _XPLATSTR("application/json");

	auto writer = ::odata::make_shared<odata_json_writer>(m_model, get_options());

	auto value_context = writer->serialize(send_value);

	return m_client_proxy->send_http_request(send_type, path, accept, value_context).then(
		[this, send_value] (const http::http_response& response) -> std::pair<nullptr_t, http_result>
		{
			doLog(__FUNCTIONW__, __FILEW__, __LINE__, response);

			if (!http_utility::is_successful_status_code(response.status_code()))
			{
				throw service_exception(response.to_string());
				}

			auto headers = response.headers();
			if (headers.has(_XPLATSTR("Location")))
			{
				auto edit_link = headers[_XPLATSTR("Location")];
				auto entity_value = std::dynamic_pointer_cast<odata_entity_value>(send_value);
				if (entity_value)
				{
					entity_value->set_value(odata_json_constants::PAYLOAD_ANNOTATION_EDITLINK, ::odata::make_shared<odata_primitive_value>(::odata::make_shared<edm_payload_annotation_type>(odata_json_constants::PAYLOAD_ANNOTATION_EDITLINK), edit_link));
				}
			}

			return std::make_pair(nullptr, http_result(response));
		});
}

}}
