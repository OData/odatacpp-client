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

    class odata_client
    {
    public:
        /// <summary>
        /// Constructor
        /// </summary>
        /// <param name="baseAddress">The OData service endpoint used to create the client.</param>
        odata_client(::utility::string_t baseAddress, client_options options = client_options()) :
            m_options(options), m_service_root_url(std::move(baseAddress))
        {
            m_client_proxy = std::make_shared<::odata::communication::http_client_proxy<::odata::communication::http_client_impl, ::odata::communication::http_client_response>>(m_service_root_url, options.get_credential_setting());

            if (m_service_root_url.back() == '/')
            {
                m_service_root_url.pop_back();
            }
        }

        const ::utility::string_t& get_service_root_url() const
        {
            return m_service_root_url;
        }

        ::utility::string_t get_relative_path(const ::utility::string_t& full_path)
        {
            if (::odata::common::is_relative_path(m_service_root_url, full_path))
            {
                return full_path;
            }

            return full_path.substr(m_service_root_url.length() + 1, full_path.length() - m_service_root_url.length());
        }

        /// <summary>
        /// Retrieve the service's metadata (types, and other information).
        /// </summary>
        ODATACPP_CLIENT_API pplx::task<std::shared_ptr<::odata::edm::edm_model>> get_model();

        /// <summary>
        /// Get the elements of a given entity set.
        /// </summary>
        /// <param name="edm_entity_set">The entity set metadata record.</param>
        /// <param name="query_exp">The OData query string to use for the request.</param>
        /// <returns>The result entities with annotation infomation, possibly empty.</returns>
        ODATACPP_CLIENT_API pplx::task<std::vector<std::shared_ptr<::odata::core::odata_entity_value>>> get_entities(const ::utility::string_t& edm_entity_set);

        /// <summary>
        /// Add an entity to a given entity set.
        /// </summary>
        /// <param name="edm_entity_set">The entity set metadata record.</param>
        /// <param name="record">The entity to add.</param>
        /// <returns>An entity instance representing the new entity after it was inserted.</returns>
        ODATACPP_CLIENT_API pplx::task<::web::http::status_code> create_entity(const ::utility::string_t& entity_set_name, const std::shared_ptr<::odata::core::odata_entity_value>& entity_object);

        /// <summary>
        /// Update an existing entity of a given entity set.
        /// </summary>
        /// <param name="edm_entity_set">The entity set metadata record.</param>
        /// <param name="key">The primary key of the record that should be modified.</param>
        /// <param name="record">The entity value to write.</param>
        /// <returns>An entity instance representing the new entity after it was modified.</returns>
        /// <remarks>This does a surgical replacement of the record, modifying and adding properties found in the provided record.</remarks>
        ODATACPP_CLIENT_API pplx::task<::web::http::status_code> patch_entity(const ::utility::string_t& entity_set_name, const std::shared_ptr<::odata::core::odata_entity_value>& entity_object);

        /// <summary>
        /// Replace existing entity of a given entity set. 
        /// </summary>
        /// <param name="edm_entity_set">The entity set metadata record.</param>
        /// <param name="key">The primary key of the record that should be modified.</param>
        /// <param name="record">The entity value to write.</param>
        /// <returns>An entity instance representing the new entity after it was modified.</returns>
        /// <remarks>This does a total replacement of the existing entity with a given entity value.</remarks>
        ODATACPP_CLIENT_API pplx::task<::web::http::status_code> put_entity(const ::utility::string_t& entity_set_name, const std::shared_ptr<::odata::core::odata_entity_value>& entity_object);  

        /// <summary>
        /// Remove an entity from a given entity set.
        /// </summary>
        /// <param name="edm_entity_set">The entity set metadata record.</param>
        /// <param name="key">The primary key of the record that should be deleted.</param>
        ODATACPP_CLIENT_API pplx::task<::web::http::status_code> delete_entity(const ::utility::string_t& entity_set_name, const std::shared_ptr<::odata::core::odata_entity_value>& entity_object);

        /// <summary>
        /// Add a reference to a collection-valued navigation property.
        /// </summary>
        /// <param name="edm_entity_set">The path of the navigation property.</param>
        /// <param name="key">The id of referenced entity.</param>
        ODATACPP_CLIENT_API pplx::task<::web::http::status_code> add_reference(const ::utility::string_t& path, const ::utility::string_t& referenceEntityId);

        /// <summary>
        /// Remove reference to a collection-valued or single-valued navigation property.
        /// </summary>
        /// <param name="edm_entity_set">The path of the navigation property.</param>
        /// <param name="key">The id of referenced entity if the navigation property is collection-values, or empty if it is single-valued.</param>
        ODATACPP_CLIENT_API pplx::task<::web::http::status_code> remove_reference(const ::utility::string_t& path, const ::utility::string_t& referenceEntityId);

        /// <summary>
        /// Update reference to a single-valued navigation property.
        /// </summary>
        /// <param name="edm_entity_set">The path of the navigation property.</param>
        /// <param name="key">The id of referenced entity.</param>
        ODATACPP_CLIENT_API pplx::task<::web::http::status_code> update_reference(const ::utility::string_t& path, const ::utility::string_t& referenceEntityId);

        /// <summary>
        /// Get odata value of any odata type data defined in edm library including primitive, complex, entity type from server.
        /// </summary>
        /// <param name="path">The request path which can be set with select, filter, expand or even function or action options.</param>
        ODATACPP_CLIENT_API pplx::task<std::vector<std::shared_ptr<::odata::core::odata_value>>> get_data_from_server(const ::utility::string_t& path);

        /// <summary>
        /// The size of result data of a request maybe too large for one http response, so the server may return the data one page after another
        /// </summary>
        /// <param name="path">The request path which can be set with select, filter, expand or even function or action options.</param>
        /// <param name="path">The result data set.</param>
        /// <returns>The odata_payload class containing next link url, if no more data next link url is empty.</returns>
        ODATACPP_CLIENT_API pplx::task<std::shared_ptr<::odata::core::odata_payload>> get_paged_data_from_server(const ::utility::string_t& path);

        /// <summary>
        /// Send odata value of any odata type data defined in edm library including primitive, complex, entity type to server.
        /// </summary>
        /// <param name="path">Path of OData value.</param>
        /// <param name="send_value">OData value to be sent to the server.</param>
        /// <param name="send_type">POST, PATCH or PUT options.</param>
        ODATACPP_CLIENT_API pplx::task<::web::http::status_code> send_data_to_server(const ::utility::string_t& path, const std::shared_ptr<::odata::core::odata_value>& send_value, const ::utility::string_t& send_type = HTTP_POST);

        /// <summary>
        /// Send a vector of odata parameters to server.
        /// </summary>
        /// <param name="path">Path to send the parameters, could be function/action path.</param>
        /// <param name="send_parameters">OData parameters to be sent to the server.</param>
        /// <param name="send_type">POST, PATCH or PUT options.</param>
        ODATACPP_CLIENT_API pplx::task<::web::http::status_code> send_data_to_server(const ::utility::string_t& path, const std::vector<std::shared_ptr<::odata::core::odata_parameter>>& send_parameters, std::vector<std::shared_ptr<::odata::core::odata_value>>& returnd_value, const ::utility::string_t& send_type = HTTP_POST);

        /// <summary>
        /// Send empty data to server.
        /// </summary>
        /// <param name="path">Path of OData value.</param>
        /// <param name="send_type">POST, PATCH or PUT options.</param>
        ODATACPP_CLIENT_API pplx::task<::web::http::status_code> send_data_to_server(const ::utility::string_t& path, const ::utility::string_t& send_type = HTTP_POST);
    private:
        std::shared_ptr<::odata::edm::edm_model> schema_from_response(const ::odata::communication::http_client_response& response);
        std::shared_ptr<::odata::core::odata_payload> entity_payload_from_response(const ::odata::communication::http_client_response&, const std::shared_ptr<::odata::edm::edm_entity_set>& set);
        std::shared_ptr<::odata::core::odata_payload> parse_payload_from_response(const ::odata::communication::http_client_response&);
        pplx::task<std::vector<std::shared_ptr<::odata::core::odata_entity_value>>> get_entities(const std::shared_ptr<::odata::edm::edm_entity_set>& edm_entity_set, const ::utility::string_t& path = U(""));

        static std::unordered_map<::utility::string_t, std::shared_ptr<::odata::edm::edm_model>> m_model_cache;
        std::shared_ptr<::odata::communication::http_client_proxy<::odata::communication::http_client_impl, ::odata::communication::http_client_response>> m_client_proxy;
        client_options m_options;
        ::utility::string_t m_service_root_url;
        std::shared_ptr<::odata::edm::edm_model> m_model;
    };

}}