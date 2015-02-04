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

std::unordered_map<::utility::string_t, std::shared_ptr<::odata::edm::edm_model>> odata_client::m_model_cache;

pplx::task<std::shared_ptr<edm_model>> odata_client::get_model()
{
    if (m_model)
	{
        return pplx::task_from_result(m_model);
	}

	auto find_iter = m_model_cache.find(m_service_root_url);
	if (find_iter != m_model_cache.end())
	{
		m_model = find_iter->second;
		return pplx::task_from_result(m_model);
	}

    return m_client_proxy->send_http_request(HTTP_GET, U("$metadata"), U("application/xml")).then(
      [this] (const http::http_response& response) -> std::shared_ptr<edm_model>
        {
            if (!http_utility::is_successful_status_code(response.status_code()))
            {
                throw service_exception(response.to_string());
            }

            m_model = schema_from_response(response);
			m_model_cache[m_service_root_url] = m_model;

            return m_model;
        });
}

std::shared_ptr<edm_model> odata_client::schema_from_response(const http_client_response& response)
{
    auto reader = std::make_shared<edm_model_reader>(response.body());

	if (reader)
	{
		reader->parse();
		return reader->get_model();
	}

	return nullptr;
}

std::shared_ptr<odata_payload> odata_client::entity_payload_from_response(const http_client_response& response, const std::shared_ptr<edm_entity_set>& set)
{
	if (response.headers().content_type().substr(0, 16) == U("application/json"))
    {
		auto reader = entity_factory<odata_json_reader>::create_reader_instance(m_model, m_service_root_url);

		return reader->deserilize(response.extract_json().get());
    }
    else
    {
		throw std::runtime_error("Atom payload not supported!"); 
    }

    return std::make_shared<odata_payload>();
}

std::shared_ptr<odata_payload> odata_client::parse_payload_from_response(const http_client_response& response)
{
	if (response.headers().content_type().substr(0,16) == U("application/json"))
    {
		auto reader = entity_factory<odata_json_reader>::create_reader_instance(m_model, m_service_root_url);

		return reader->deserilize(response.extract_json().get());
    }
    else
    {
		//throw std::runtime_error("Atom payload not supported!");
    }

	return std::make_shared<odata_payload>();
}

pplx::task<std::vector<std::shared_ptr<odata_entity_value>>> odata_client::get_entities(const ::utility::string_t& edm_entity_set)
{
    if (!m_model)
    {
		return get_model().then([this, edm_entity_set](std::shared_ptr<edm_model> model){
			return get_entities(edm_entity_set);
		});
    }

    auto ecnt_md = m_model->find_container();
    auto eset_md = ecnt_md ? ecnt_md->find_entity_set(edm_entity_set) : nullptr;
    
    return get_entities(eset_md, edm_entity_set);
}

pplx::task<std::vector<std::shared_ptr<odata_entity_value>>> odata_client::get_entities(const std::shared_ptr<edm_entity_set>& edm_entity_set, const ::utility::string_t& path)
{
    if (!m_model)
    {
		return get_model().then([this, edm_entity_set, path](std::shared_ptr<edm_model> model){
			return get_entities(edm_entity_set, path);
		});
    }

    ::utility::string_t accept = U("application/json");  
	if (!m_options.m_json_mdlevel.empty())
	{
		accept.append(U(";odata.metadata=")).append(m_options.m_json_mdlevel);
	}

	return m_client_proxy->send_http_request(HTTP_GET, path, accept).then(
      [this, path, edm_entity_set] (const http::http_response& response) -> std::vector<std::shared_ptr<odata_entity_value>>
        {
            if (!http_utility::is_successful_status_code(response.status_code()))
            {
                throw service_exception(response.to_string());
            }

            auto payload = entity_payload_from_response(response, edm_entity_set);
			std::vector<std::shared_ptr<odata_entity_value>> entities;
			for(auto iter = payload->get_values().cbegin(); iter != payload->get_values().cend(); iter++)
			{
				entities.push_back(std::dynamic_pointer_cast<odata_entity_value>(*iter));
			}

			if (!payload->get_next_link().empty())
			{
				auto next_entities = get_entities(edm_entity_set, payload->get_next_link()).get();
				entities.insert(entities.cend(), next_entities.cbegin(), next_entities.cend());
			}

			return entities;
        });
}

pplx::task<::web::http::status_code> odata_client::create_entity(const ::utility::string_t& entity_set_name, const std::shared_ptr<odata_entity_value>& entity_object)
{
    if (!m_model)
    {
		return get_model().then([this, entity_set_name, entity_object](std::shared_ptr<edm_model> model){
			return create_entity(entity_set_name, entity_object);
		});
    }

	return send_data_to_server(entity_set_name, entity_object, HTTP_POST);
}

pplx::task<::web::http::status_code> odata_client::add_reference(const ::utility::string_t& path, const ::utility::string_t& referenceEntityId)
{
	if (!m_model)
    {
		return get_model().then([this, path, referenceEntityId](std::shared_ptr<edm_model> model){
			return add_reference(path, referenceEntityId);
		});
    }

	auto entity = std::make_shared<odata_entity_value>(std::make_shared<edm_entity_type>(U(""), U("")));
	entity->set_value(odata_json_constants::PAYLOAD_ANNOTATION_ID, std::make_shared<odata_primitive_value>(std::make_shared<edm_payload_annotation_type>(odata_json_constants::PAYLOAD_ANNOTATION_ID), referenceEntityId));

	::utility::stringstream_t ss;
	ss << path << U("/$ref");
	return send_data_to_server(ss.str(), entity, HTTP_POST);
}

pplx::task<::web::http::status_code> odata_client::remove_reference(const ::utility::string_t& path, const ::utility::string_t& referenceEntityId)
{
	if (!m_model)
    {
		return get_model().then([this, path, referenceEntityId](std::shared_ptr<edm_model> model){
			return remove_reference(path, referenceEntityId);
		});
    }

	::utility::stringstream_t ss;
	ss << path << U("/$ref");
	if (!referenceEntityId.empty())
	{
		ss << U("?$id=") << referenceEntityId;
	}

	return send_data_to_server(ss.str(), HTTP_DELETE);
}

pplx::task<::web::http::status_code> odata_client::update_reference(const ::utility::string_t& path, const ::utility::string_t& referenceEntityId)
{
	if (!m_model)
    {
		return get_model().then([this, path, referenceEntityId](std::shared_ptr<edm_model> model){
			return update_reference(path, referenceEntityId);
		});
    }

	auto entity = std::make_shared<odata_entity_value>(std::make_shared<edm_entity_type>(U(""), U("")));
	entity->set_value(odata_json_constants::PAYLOAD_ANNOTATION_ID, std::make_shared<odata_primitive_value>(std::make_shared<edm_payload_annotation_type>(odata_json_constants::PAYLOAD_ANNOTATION_ID), referenceEntityId));

	::utility::stringstream_t ss;
	ss << path << U("/$ref");
	return send_data_to_server(ss.str(), entity, HTTP_PUT);
}

pplx::task<::web::http::status_code> odata_client::patch_entity(const ::utility::string_t& entity_set_name, const std::shared_ptr<odata_entity_value>& entity_object)
{
    if (!m_model)
    {
		return get_model().then([this, entity_set_name, entity_object](std::shared_ptr<edm_model> model){
			return patch_entity(entity_set_name, entity_object);
		});
    }
    
	::utility::string_t key = entity_object->get_entity_key_string();
	::utility::string_t path = entity_set_name + key;
	
	return send_data_to_server(path, entity_object, HTTP_PATCH);
}

pplx::task<::web::http::status_code> odata_client::put_entity(const ::utility::string_t& entity_set_name, const std::shared_ptr<odata_entity_value>& entity_object)
{
    if (!m_model)
    {
		return get_model().then([this, entity_set_name, entity_object](std::shared_ptr<edm_model> model){
			return put_entity(entity_set_name, entity_object);
		});
    }

	::utility::string_t key = entity_object->get_entity_key_string();
	::utility::string_t path = entity_set_name + key;

	return send_data_to_server(path, entity_object, HTTP_PUT);
}

pplx::task<::web::http::status_code> odata_client::delete_entity(const ::utility::string_t& entity_set_name, const std::shared_ptr<odata_entity_value>& entity_object)
{
    if (!m_model)
    {
		return get_model().then([this, entity_set_name, entity_object](std::shared_ptr<edm_model> model){
			return delete_entity(entity_set_name, entity_object);
		});
    }

	::utility::string_t key = entity_object->get_entity_key_string();
	::utility::string_t path = entity_set_name + key;

	::utility::string_t accept = U("application/json");

	return m_client_proxy->send_http_request(HTTP_DELETE, path, accept).then(
      [this, entity_set_name] (const http::http_response& response) -> ::web::http::status_code
        {
            if (!http_utility::is_successful_status_code(response.status_code()))
            {
                throw service_exception(response.to_string());
            }

            return response.status_code();
        });
}

pplx::task<std::vector<std::shared_ptr<odata_value>>> odata_client::get_data_from_server(const ::utility::string_t& path)
{
    if (!m_model)
    {
        return get_model().then([this, path](std::shared_ptr<edm_model> model){
            return get_data_from_server(path);
        });
    }

    ::utility::string_t accept = U("application/json");
    if (!m_options.m_json_mdlevel.empty())
    {
        accept.append(U(";odata.metadata=")).append(m_options.m_json_mdlevel);
    }

    return m_client_proxy->send_http_request(HTTP_GET, path, accept).then(
        [this, path] (const http::http_response& response) -> std::vector<std::shared_ptr<odata_value>>
    {
        if (!http_utility::is_successful_status_code(response.status_code()))
        {
            throw service_exception(response.to_string());
        }

        auto payload = parse_payload_from_response(response);

        ::utility::string_t next_link = payload->get_next_link();
        if (!next_link.empty())
        {
            auto next_entities = get_data_from_server(get_relative_path(next_link)).get();
            payload->insert_values(next_entities);
        }

        return payload->get_values();		
    });
}

pplx::task<std::shared_ptr<odata_payload>> odata_client::get_paged_data_from_server(const ::utility::string_t& path)
{
    if (!m_model)
    {
		return get_model().then([this, path](std::shared_ptr<edm_model> model){
			return get_paged_data_from_server(path);
		});
    }

	::utility::string_t accept = U("application/json");
	if (!m_options.m_json_mdlevel.empty())
	{
		accept.append(U(";odata.metadata=")).append(m_options.m_json_mdlevel);
	}

	return m_client_proxy->send_http_request(HTTP_GET, path, accept).then(
      [this, path] (const http::http_response& response) -> std::shared_ptr<odata_payload>
        {
            if (!http_utility::is_successful_status_code(response.status_code()))
            {
                throw service_exception(response.to_string());
            }

            return parse_payload_from_response(response);
        });
}

pplx::task<::web::http::status_code> odata_client::send_data_to_server(const ::utility::string_t& path, const ::utility::string_t& send_type)
{
    if (!m_model)
    {
		return get_model().then([this, path, send_type](std::shared_ptr<edm_model> model){
			return send_data_to_server(path, send_type);
		});
    }

	::utility::string_t accept = U("application/json");

	return m_client_proxy->send_http_request(send_type, path, accept).then(
      [this] (const http::http_response& response) -> ::web::http::status_code
        {
            if (!http_utility::is_successful_status_code(response.status_code()))
            {
                throw service_exception(response.to_string());
            }

			return response.status_code();
        });

}

pplx::task<::web::http::status_code> odata_client::send_data_to_server(const ::utility::string_t& path, const std::vector<std::shared_ptr<::odata::core::odata_parameter>>& send_parameters, std::vector<std::shared_ptr<::odata::core::odata_value>>& returnd_values, const ::utility::string_t& send_type)
{
	if (send_type != HTTP_POST && send_type != HTTP_GET)
	{
		throw std::invalid_argument("Only Post and Get are allowed as send_type.");
	}

	if (!m_model)
    {
		return get_model().then([this, path, send_parameters, &returnd_values, send_type](std::shared_ptr<edm_model> model){
			return send_data_to_server(path, send_parameters, returnd_values, send_type);
		});
    }

	::utility::string_t accept = U("application/json");

	web::json::value value_context;
	::utility::string_t new_path = path;
	if (send_type == HTTP_POST)
	{
		auto writer = std::make_shared<::odata::core::odata_json_operation_payload_parameter_writer>(m_model);
		if (!send_parameters.empty())
			value_context = writer->serialize(send_parameters);
	}
	else if (send_type == HTTP_GET && !send_parameters.empty())
	{
		auto writer = std::make_shared<::odata::core::odata_json_operation_url_parameter_writer>(m_model);
		new_path += ::web::http::uri::encode_uri(writer->serialize(send_parameters));
	}

	if (value_context.is_null())
	{
		auto response = m_client_proxy->send_http_request(send_type, new_path, accept).get();

		if (!http_utility::is_successful_status_code(response.status_code()))
		{
			throw service_exception(response.to_string());
		}

		if (response.status_code() != 204)
		{
			returnd_values = parse_payload_from_response(response)->get_values();
		}

		return pplx::task_from_result(response.status_code());
	} 
	else
	{
		auto response = m_client_proxy->send_http_request(send_type, new_path, accept, value_context).get();

		if (!http_utility::is_successful_status_code(response.status_code()))
		{
			throw service_exception(response.to_string());
		}

		if (response.status_code() != 204)
		{
			returnd_values = parse_payload_from_response(response)->get_values();
		}
		
		return pplx::task_from_result(response.status_code());	
	}
}


pplx::task<::web::http::status_code> odata_client::send_data_to_server(const ::utility::string_t& path, const std::shared_ptr<odata_value>& send_value, const ::utility::string_t& send_type)
{
    if (!m_model)
    {
		return get_model().then([this, path, send_value, send_type](std::shared_ptr<edm_model> model){
			return send_data_to_server(path, send_value, send_type);
		});
    }

	::utility::string_t accept = U("application/json");

	auto writer = std::make_shared<::odata::core::odata_json_writer>(m_model);

	auto value_context = writer->serialize(send_value);
	
	auto ss = value_context.serialize();

	return m_client_proxy->send_http_request(send_type, path, accept, value_context).then(
      [this, send_value] (const http::http_response& response) -> ::web::http::status_code
        {
            if (!http_utility::is_successful_status_code(response.status_code()))
            {
                throw service_exception(response.to_string());
            }

			auto headers = response.headers();
			if (headers.has(U("Location")))
			{
				auto edit_link = headers[U("Location")];
				auto entity_value = std::dynamic_pointer_cast<odata_entity_value>(send_value);
				if (entity_value)
				{
					entity_value->set_value(odata_json_constants::PAYLOAD_ANNOTATION_EDITLINK, std::make_shared<odata_primitive_value>(std::make_shared<edm_payload_annotation_type>(odata_json_constants::PAYLOAD_ANNOTATION_EDITLINK), edit_link));
				}
			}

			return response.status_code();
        });
}

}}