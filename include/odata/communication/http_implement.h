//---------------------------------------------------------------------
// <copyright file="http_implement.h" company="Microsoft">
//      Copyright (C) Microsoft Corporation. All rights reserved. See License.txt in the project root for license information.
// </copyright>
//---------------------------------------------------------------------

#pragma once

#include <functional>
#include "odata/communication/http_communication.h"
#include "cpprest/uri.h"
#include "cpprest/asyncrt_utils.h"
#include "cpprest/json.h"
#include "cpprest/http_client.h"
#include "odata/edm/odata_edm.h"

namespace odata { namespace communication
{

class http_client_impl
{
public:
	http_client_impl(const ::utility::string_t& baseAddress, std::shared_ptr<::odata::client::odata_client_credential> credential_setting)
	{
		if (credential_setting)
		{
			::web::http::client::http_client_config client_config;
			client_config.set_credentials(::web::http::client::credentials(credential_setting->get_username(), credential_setting->get_password()));
			m_client = std::make_shared<::web::http::client::http_client>(::web::http::uri(baseAddress), client_config);
		}
		else
		{
            m_client = std::make_shared<::web::http::client::http_client>(::web::http::uri(baseAddress));
		}
		
	}

	::utility::string_t base_uri()
    {
		return m_client->base_uri().to_string();
    }

	pplx::task<::web::http::http_response> send_http_request(const ::utility::string_t& method, const ::utility::string_t& request_uri, const ::utility::string_t accept);
	pplx::task<::web::http::http_response> send_http_request(const ::utility::string_t& method, const ::utility::string_t& request_uri, const ::utility::string_t accept, ::web::json::value object);

private:
	std::shared_ptr<::web::http::client::http_client> m_client;

    ::web::http::http_request _build_get_request(::web::http::method method, ::web::http::uri_builder request_uri, const ::utility::string_t& accept = U("")) const;
    ::web::http::http_request _build_request(::web::http::method method, ::web::http::uri_builder request_uri, const ::utility::string_t& accept, ::web::json::value object) const;
};

}}