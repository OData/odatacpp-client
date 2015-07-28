//---------------------------------------------------------------------
// <copyright file="http_implement.cpp" company="Microsoft">
//      Copyright (C) Microsoft Corporation. All rights reserved. See License.txt in the project root for license information.
// </copyright>
//---------------------------------------------------------------------

#include "odata/communication/http_implement.h"
#include "odata/edm/edm_model_reader.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1800)
#include <ppltasks.h>
namespace pplx = Concurrency;
#else 
#include "pplx/pplxtasks.h"
#endif


#include <functional>
#include "cpprest/uri.h"
#include "cpprest/asyncrt_utils.h"
#include "cpprest/json.h"
#include "cpprest/http_client.h"

using namespace ::pplx;
using namespace ::web;

namespace odata { namespace communication
{

http::http_request http_client_impl::_build_get_request(http::method method, http::uri_builder request_uri, const ::utility::string_t& accept) const
{
    http::http_request msg(method);

    if (!accept.empty())
        msg.headers().add(U("Accept"), accept);

    msg.set_request_uri(request_uri.to_uri());

    return msg;
}

http::http_request http_client_impl::_build_request(http::method method, http::uri_builder request_uri, const ::utility::string_t& accept, json::value object) const
{
    http::http_request msg(method);

    if (!accept.empty())
        msg.headers().add(U("Accept"), accept);

    msg.set_request_uri(request_uri.to_uri());

    if (method == http::methods::POST || method == http::methods::PUT || method == http::methods::PATCH)
    {
        if(object.is_null() || !object.is_object())
            throw std::invalid_argument("POST, PUT, and PATCH requests require a payload");

        msg.set_body(object);
    }

    return msg;
}

pplx::task<http::http_response> http_client_impl::send_http_request(const ::utility::string_t& method, const ::utility::string_t& request_uri, const ::utility::string_t accept)
{
	http::uri_builder bldr;
    bldr.set_path(request_uri);

	auto request = _build_get_request(method, bldr, accept);

    return getOrCreateClient()->request(request);
}

pplx::task<http::http_response> http_client_impl::send_http_request(const ::utility::string_t& method, const ::utility::string_t& request_uri, const ::utility::string_t accept, ::web::json::value object)
{
	http::uri_builder bldr;
    bldr.set_path(request_uri);

	auto request = _build_request(method, bldr, accept, object);

    return getOrCreateClient()->request(request);
}

}}
