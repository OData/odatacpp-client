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

    msg.headers().add(U("Authorization"), U("Basic YWRtaW5Ab2F1dGgzLmNjc2N0cC5uZXQ6UGEkJHcwcmQ="));
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

    return m_client->request(request);
}

pplx::task<http::http_response> http_client_impl::send_http_request(const ::utility::string_t& method, const ::utility::string_t& request_uri, const ::utility::string_t accept, ::web::json::value object)
{
	http::uri_builder bldr;
    bldr.set_path(request_uri);

	auto request = _build_request(method, bldr, accept, object);

    return m_client->request(request);
}

}}