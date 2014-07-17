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

#define HTTP_GET U("GET")
#define HTTP_POST U("POST")
#define HTTP_PATCH U("PATCH")
#define HTTP_DELETE U("DELETE")
#define HTTP_PUT U("PUT")

typedef ::web::http::http_response http_client_response;

template<typename _Http_Imple, typename _Http_Response>
class http_client_proxy
{
public:
	http_client_proxy(const ::utility::string_t& baseAddress, std::shared_ptr<::odata::client::odata_client_credential> credential_setting)
	{
		m_client_impl = std::make_shared<_Http_Imple>(baseAddress, credential_setting);
	}

	http_client_proxy(std::shared_ptr<_Http_Imple> client_impl) : m_client_impl(client_impl)
	{
	}

    pplx::task<_Http_Response> send_http_request(const ::utility::string_t& method, const ::utility::string_t& request_uri, const ::utility::string_t accept)
	{
		return m_client_impl->send_http_request(method, request_uri, accept);
	}

    pplx::task<_Http_Response> send_http_request(const ::utility::string_t& method, const ::utility::string_t& request_uri, const ::utility::string_t accept, ::web::json::value object)
	{
		return m_client_impl->send_http_request(method, request_uri, accept, object);
	}

    ::utility::string_t base_uri()
    {
        return m_client_impl->base_uri();
    }

private:
	std::shared_ptr<_Http_Imple>  m_client_impl;
};

}}