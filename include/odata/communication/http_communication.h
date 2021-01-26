//---------------------------------------------------------------------
// <copyright file="http_communication.h" company="Microsoft">
//      Copyright (C) Microsoft Corporation. All rights reserved. See License.txt in the project root for license information.
// </copyright>
//---------------------------------------------------------------------

#pragma once

#include "odata/common/utility.h"
#include "cpprest/uri.h"
#include "cpprest/asyncrt_utils.h"
#include "cpprest/json.h"
#include "cpprest/http_client.h"
#include "odata/edm/odata_edm.h"
#include "odata/client/odata_client_options.h"
#include "odata/communication/http_service_exception.h"

namespace odata { namespace communication
{

#define HTTP_GET _XPLATSTR("GET")
#define HTTP_POST _XPLATSTR("POST")
#define HTTP_PATCH _XPLATSTR("PATCH")
#define HTTP_DELETE _XPLATSTR("DELETE")
#define HTTP_PUT _XPLATSTR("PUT")

typedef ::web::http::http_response http_client_response;

template<typename _Http_Imple, typename _Http_Response>
class http_client_proxy
{
public:
	http_client_proxy(const ::odata::string_t& baseAddress, std::shared_ptr<::odata::client::odata_client_credential> credential_setting) : m_client_impl(::odata::make_shared<_Http_Imple>(baseAddress, credential_setting))
	{
	}

	http_client_proxy(std::shared_ptr<_Http_Imple> client_impl) : m_client_impl(client_impl)
	{
	}

	pplx::task<_Http_Response> send_http_request(const ::odata::string_t& method, const ::odata::string_t& request_uri, const ::odata::string_t accept)
	{
		return m_client_impl->send_http_request(method, request_uri, accept);
	}

	pplx::task<_Http_Response> send_http_request(const ::odata::string_t& method, const ::odata::string_t& request_uri, const ::odata::string_t accept, ::web::json::value object)
	{
		return m_client_impl->send_http_request(method, request_uri, accept, object);
	}

	::odata::string_t base_uri()
	{
		return m_client_impl->base_uri();
	}

	::web::http::client::http_client_config& get_client_config()
	{
		return m_client_impl->get_client_config();
	}

	void clear_request_header()
	{
		m_client_impl->clear_request_header();
	}
	bool has_request_header(const ::odata::string_t& name) const
	{
		return m_client_impl->has_request_header(name);
	}
	void add_request_header(const ::odata::string_t& name, const ::odata::string_t& value)
	{
		m_client_impl->add_request_header(name, value);
	}
	void remove_request_header(const ::odata::string_t& name)
	{
		m_client_impl->remove_request_header(name);
	}
	::odata::string_t match_request_header(const ::odata::string_t& name) const
	{
		return m_client_impl->match_request_header(name);
	}

	void SetLogCallbacks(std::function<bool()> ShallLogCallback, std::function<void(wchar_t const*, wchar_t const*, size_t, bool, ::odata::string_t const &)> LogCallback)
	{
		m_client_impl->SetLogCallbacks(ShallLogCallback, LogCallback);
	}

private:
	std::shared_ptr<_Http_Imple> m_client_impl;
};

}}
