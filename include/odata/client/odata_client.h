//---------------------------------------------------------------------
// <copyright file="odata_client.h" company="Microsoft">
//      Copyright (C) Microsoft Corporation. All rights reserved. See License.txt in the project root for license information.
// </copyright>
//---------------------------------------------------------------------

#pragma once

#include <functional>
#include "odata/client/odata_client_options.h"
#include "odata/common/utility.h"
#include "odata/communication/http_communication.h"
#include "odata/communication/http_implement.h"
#include "odata/communication/http_utility.h"
#include "odata/core/odata_core.h"
#include "odata/edm/odata_edm.h"
#include "odata/edm/edm_model_reader.h"

namespace odata { namespace client {

	struct http_result
	{
		http_result()                              = default;
		http_result(http_result const&)            = default;
		http_result(http_result&&)                 = default;
		http_result(::web::http::http_headers const &headers, ::web::http::status_code status) : headers_(headers), status_(status) {}
		http_result(::web::http::http_response const &response) : headers_(response.headers()), status_(response.status_code()) {}
		http_result& operator=(http_result const&) = default;
		http_result& operator=(http_result&&)      = default;
		::web::http::http_headers headers_;
		::web::http::status_code  status_;
	};

	class odata_client
	{
	public:
		/// <summary>
		/// Constructor
		/// </summary>
		/// <param name="baseAddress">The OData service endpoint used to create the client.</param>
		odata_client(::odata::string_t baseAddress, client_options options = client_options()) :
			m_client_proxy(), m_options(options), m_service_root_url(std::move(baseAddress)), m_model(), m_model_response_headers(), m_ShallLogCallback(), m_LogCallback()
		{
			m_client_proxy = ::odata::make_shared<::odata::communication::http_client_proxy<::odata::communication::http_client_impl, ::odata::communication::http_client_response>>(m_service_root_url, options.get_credential_setting());

			if (m_service_root_url.back() == _XPLATSTR('/'))
			{
				m_service_root_url.pop_back();
			}
		}

		::web::http::client::http_client_config& get_client_config()
		{
			return m_client_proxy->get_client_config();
		}

		const ::odata::string_t& get_service_root_url() const
		{
			return m_service_root_url;
		}

		::odata::string_t get_relative_path(const ::odata::string_t& full_path) const
		{
			if (::odata::common::is_relative_path(m_service_root_url, full_path))
			{
				return full_path;
			}

			return full_path.length() > m_service_root_url.length() ? full_path.substr(m_service_root_url.length() + 1, full_path.length() - m_service_root_url.length()) : _XPLATSTR(""); // Handles corner case of full_path = m_service_root_url!
		}

		const client_options& get_options() const { return m_options; }

		/// <summary>
		/// Retrieve the service's metadata (types, and other information).
		/// </summary>
		ODATACPP_CLIENT_API pplx::task<std::pair<std::shared_ptr<::odata::edm::edm_model>, http_result>> get_model();

		/// <summary>
		/// Get the elements of a given entity set.
		/// </summary>
		/// <param name="edm_entity_set">The entity set metadata record.</param>
		/// <param name="query_exp">The OData query string to use for the request.</param>
		/// <returns>The result entities with annotation infomation, possibly empty.</returns>
		ODATACPP_CLIENT_API pplx::task<std::pair<std::vector<std::shared_ptr<::odata::core::odata_entity_value>>, http_result>> get_entities(const ::odata::string_t& edm_entity_set);

		/// <summary>
		/// Add an entity to a given entity set.
		/// </summary>
		/// <param name="edm_entity_set">The entity set metadata record.</param>
		/// <param name="record">The entity to add.</param>
		/// <returns>An entity instance representing the new entity after it was inserted.</returns>
		ODATACPP_CLIENT_API pplx::task<std::pair<nullptr_t, http_result>> create_entity(const ::odata::string_t& entity_set_name, const std::shared_ptr<::odata::core::odata_entity_value>& entity_object);

		/// <summary>
		/// Update an existing entity of a given entity set.
		/// </summary>
		/// <param name="edm_entity_set">The entity set metadata record.</param>
		/// <param name="key">The primary key of the record that should be modified.</param>
		/// <param name="record">The entity value to write.</param>
		/// <returns>An entity instance representing the new entity after it was modified.</returns>
		/// <remarks>This does a surgical replacement of the record, modifying and adding properties found in the provided record.</remarks>
		ODATACPP_CLIENT_API pplx::task<std::pair<nullptr_t, http_result>> patch_entity(const ::odata::string_t& entity_set_name, const std::shared_ptr<::odata::core::odata_entity_value>& entity_object);

		/// <summary>
		/// Replace existing entity of a given entity set.
		/// </summary>
		/// <param name="edm_entity_set">The entity set metadata record.</param>
		/// <param name="key">The primary key of the record that should be modified.</param>
		/// <param name="record">The entity value to write.</param>
		/// <returns>An entity instance representing the new entity after it was modified.</returns>
		/// <remarks>This does a total replacement of the existing entity with a given entity value.</remarks>
		ODATACPP_CLIENT_API pplx::task<std::pair<nullptr_t, http_result>> put_entity(const ::odata::string_t& entity_set_name, const std::shared_ptr<::odata::core::odata_entity_value>& entity_object);

		/// <summary>
		/// Remove an entity from a given entity set.
		/// </summary>
		/// <param name="edm_entity_set">The entity set metadata record.</param>
		/// <param name="key">The primary key of the record that should be deleted.</param>
		ODATACPP_CLIENT_API pplx::task<std::pair<nullptr_t, http_result>> delete_entity(const ::odata::string_t& entity_set_name, const std::shared_ptr<::odata::core::odata_entity_value>& entity_object);

		/// <summary>
		/// Add a reference to a collection-valued navigation property.
		/// </summary>
		/// <param name="edm_entity_set">The path of the navigation property.</param>
		/// <param name="key">The id of referenced entity.</param>
		ODATACPP_CLIENT_API pplx::task<std::pair<nullptr_t, http_result>> add_reference(const ::odata::string_t& path, const ::odata::string_t& referenceEntityId);

		/// <summary>
		/// Remove reference to a collection-valued or single-valued navigation property.
		/// </summary>
		/// <param name="edm_entity_set">The path of the navigation property.</param>
		/// <param name="key">The id of referenced entity if the navigation property is collection-values, or empty if it is single-valued.</param>
		ODATACPP_CLIENT_API pplx::task<std::pair<nullptr_t, http_result>> remove_reference(const ::odata::string_t& path, const ::odata::string_t& referenceEntityId);

		/// <summary>
		/// Update reference to a single-valued navigation property.
		/// </summary>
		/// <param name="edm_entity_set">The path of the navigation property.</param>
		/// <param name="key">The id of referenced entity.</param>
		ODATACPP_CLIENT_API pplx::task<std::pair<nullptr_t, http_result>> update_reference(const ::odata::string_t& path, const ::odata::string_t& referenceEntityId);

		/// <summary>
		/// Get odata value of any odata type data defined in edm library including primitive, complex, entity type from server.
		/// </summary>
		/// <param name="path">The request path which can be set with select, filter, expand or even function or action options.</param>
		ODATACPP_CLIENT_API pplx::task<std::pair<std::vector<std::shared_ptr<::odata::core::odata_value>>, http_result>> get_data_from_server(const ::odata::string_t& path);

		/// <summary>
		/// The size of result data of a request maybe too large for one http response, so the server may return the data one page after another
		/// </summary>
		/// <param name="path">The request path which can be set with select, filter, expand or even function or action options.</param>
		/// <param name="path">The result data set.</param>
		/// <returns>The odata_payload class containing next link url, if no more data next link url is empty.</returns>
		ODATACPP_CLIENT_API pplx::task<std::pair<std::shared_ptr<::odata::core::odata_payload>, http_result>> get_paged_data_from_server(const ::odata::string_t& path);

		/// <summary>
		/// Send odata value of any odata type data defined in edm library including primitive, complex, entity type to server.
		/// </summary>
		/// <param name="path">Path of OData value.</param>
		/// <param name="send_value">OData value to be sent to the server.</param>
		/// <param name="send_type">POST, PATCH or PUT options.</param>
		ODATACPP_CLIENT_API pplx::task<std::pair<nullptr_t, http_result>> send_data_to_server(const ::odata::string_t& path, const std::shared_ptr<::odata::core::odata_value>& send_value, const ::odata::string_t& send_type = HTTP_POST);

		/// <summary>
		/// Send a vector of odata parameters to server.
		/// </summary>
		/// <param name="path">Path to send the parameters, could be function/action path.</param>
		/// <param name="send_parameters">OData parameters to be sent to the server.</param>
		/// <param name="send_type">POST, PATCH or PUT options.</param>
		ODATACPP_CLIENT_API pplx::task<std::pair<nullptr_t, http_result>> send_data_to_server(const ::odata::string_t& path, const std::vector<std::shared_ptr<::odata::core::odata_parameter>>& send_parameters, std::vector<std::shared_ptr<::odata::core::odata_value>>& returned_value, const ::odata::string_t& send_type = HTTP_POST);

		/// <summary>
		/// Send empty data to server.
		/// </summary>
		/// <param name="path">Path of OData value.</param>
		/// <param name="send_type">POST, PATCH or PUT options.</param>
		ODATACPP_CLIENT_API pplx::task<std::pair<nullptr_t, http_result>> send_data_to_server(const ::odata::string_t& path, const ::odata::string_t& send_type = HTTP_POST);
	private:
		std::shared_ptr<::odata::edm::edm_model> schema_from_response(const ::odata::communication::http_client_response& response);
		std::shared_ptr<::odata::core::odata_payload> entity_payload_from_response(const ::odata::communication::http_client_response&, const std::shared_ptr<::odata::edm::edm_entity_set>& set);
		std::shared_ptr<::odata::core::odata_payload> parse_payload_from_response(const ::odata::communication::http_client_response&);
		pplx::task<std::pair<std::vector<std::shared_ptr<::odata::core::odata_entity_value>>, http_result>> get_entities(const std::shared_ptr<::odata::edm::edm_entity_set>& edm_entity_set, const ::odata::string_t& path = _XPLATSTR(""));

		static std::unordered_map<::odata::string_t, std::shared_ptr<::odata::edm::edm_model>> m_model_cache;
		std::shared_ptr<::odata::communication::http_client_proxy<::odata::communication::http_client_impl, ::odata::communication::http_client_response>> m_client_proxy;
		client_options m_options;
		::odata::string_t m_service_root_url;
		std::shared_ptr<::odata::edm::edm_model> m_model;

		web::http::http_headers m_model_response_headers; //!< The http headers from the query for the model. These are kept separately as every command used will automatically query the model.

	public:
		void clear_request_header()
		{
			m_client_proxy->clear_request_header();
		}
		bool has_request_header(const ::odata::string_t& name) const
		{
			return m_client_proxy->has_request_header(name);
		}
		void add_request_header(const ::odata::string_t& name, const ::odata::string_t& value)
		{
			m_client_proxy->add_request_header(name, value);
		}
		void remove_request_header(const ::odata::string_t& name)
		{
			m_client_proxy->remove_request_header(name);
		}
		::odata::string_t match_request_header(const ::odata::string_t& name) const
		{
			return m_client_proxy->match_request_header(name);
		}

		web::http::http_headers const& get_model_response_header() const
		{
			return m_model_response_headers;
		}
		bool has_model_response_header(const ::odata::string_t& name) const
		{
			return m_model_response_headers.has(name);
		}
		::odata::string_t match_model_response_header(const ::odata::string_t& name) const
		{
			::odata::string_t value;
			m_model_response_headers.match(name, value);
			return value;
		}

		bool clear_metadata_model()
		{
			auto find_iter = m_model_cache.find(m_service_root_url);
			if (find_iter == m_model_cache.end())
			{
				return false;
			}

			// Erase the model from the cache and from the instance itself
			m_model_cache.erase(find_iter);
			m_model.reset();
			return true;
		}

		bool get_supress_odata_type() const
		{
			return m_options.get_suppress_odata_type();
		}

		void set_supress_odata_type(bool suppress_odata_type)
		{
			m_options.set_suppress_odata_type(suppress_odata_type);
		}

	// Logging capability for http responses. Be careful when using, this is a comment from
	// the casablanca REST SDK method to_string():
	// "Generates a string representation of the message, including the body when possible.
	// Mainly this should be used for debugging purposes as it has to copy the message body
	// and doesn't have excellent performance."
	// Thus besides setting the callback itself we have a separate callback that needs to
	// check in advance if any logging should happen, preventing th call to to_string()
	// if logging is disabled by some means (e. g. logging level).
	private:
		std::function<bool()>                                                                       m_ShallLogCallback;
		std::function<void(wchar_t const*, wchar_t const*, size_t, bool, ::odata::string_t const&)> m_LogCallback;

		void doLog(wchar_t const* Function, wchar_t const *File, size_t Line, web::http::http_response const &response) const
		{
			if (m_LogCallback && m_ShallLogCallback && m_ShallLogCallback())
			{
				m_LogCallback(Function, File, Line, false, response.to_string()); // false == This is a response
			}
		}

	public:
		void SetLogCallbacks(std::function<bool()> ShallLogCallback, std::function<void(wchar_t const*, wchar_t const*, size_t, bool, ::odata::string_t const &)> LogCallback)
		{
			if (ShallLogCallback && LogCallback)
			{
				m_ShallLogCallback = ShallLogCallback;
				m_LogCallback      = LogCallback;
			}
			else
			{
				m_ShallLogCallback = nullptr;
				m_LogCallback      = nullptr;
			}
			m_client_proxy->SetLogCallbacks(ShallLogCallback, LogCallback);
		}
	};

}}
