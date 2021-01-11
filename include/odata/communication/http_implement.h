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
#include "odata/common/utility.h"

namespace odata { namespace communication
{

class http_client_impl
{
public:
	http_client_impl(const ::odata::string_t& address, std::shared_ptr<::odata::client::odata_client_credential> credential)
	: m_client(), baseAddress(address), credential_setting(credential), client_config(), m_request_headers(), m_ShallLogCallback(), m_LogCallback()
	{
	}

	::odata::string_t base_uri()
	{
		return getOrCreateClient()->base_uri().to_string();
	}

	::web::http::client::http_client_config& get_client_config()
	{
		return client_config;
	}

	pplx::task<::web::http::http_response> send_http_request(const ::odata::string_t& method, const ::odata::string_t& request_uri, const ::odata::string_t accept);
	pplx::task<::web::http::http_response> send_http_request(const ::odata::string_t& method, const ::odata::string_t& request_uri, const ::odata::string_t accept, ::web::json::value object);

private:
	/// <summary>
	/// If the client hasn't already been constructed, the method constructs one
	/// and returns it, while also caching it for future
	/// </summary>
	/// <Explanation>
	/// Delays the construction of m_client until its first use
	/// This allows others to use and modify the client_config object
	/// (e.g. change the callback function associated with it) before
	/// m_client is created.
	/// </Explanation>
	std::shared_ptr<::web::http::client::http_client>& getOrCreateClient()
	{
		if (m_client == nullptr)
		{
			if (credential_setting)
				client_config.set_credentials(::web::http::client::credentials(credential_setting->get_username(), credential_setting->get_password()));
			m_client = ::odata::make_shared<::web::http::client::http_client>(::web::http::uri(baseAddress), client_config);
		}
		return m_client;
	}
	std::shared_ptr<::web::http::client::http_client> m_client;
	::odata::string_t baseAddress;
	std::shared_ptr<::odata::client::odata_client_credential> credential_setting;
	::web::http::client::http_client_config client_config;
	::web::http::http_request _build_get_request(::web::http::method method, ::web::http::uri_builder request_uri, const ::odata::string_t& accept = _XPLATSTR("")) const;
	::web::http::http_request _build_request(::web::http::method method, ::web::http::uri_builder request_uri, const ::odata::string_t& accept, ::web::json::value object) const;

	web::http::http_headers m_request_headers;

public:
	void clear_request_header()
	{
		m_request_headers.clear();
	}
	bool has_request_header(const ::odata::string_t& name) const
	{
		return m_request_headers.has(name);
	}
	void add_request_header(const ::odata::string_t& name, const ::odata::string_t& value)
	{
		m_request_headers.add(name, value);
	}
	void remove_request_header(const ::odata::string_t& name)
	{
		m_request_headers.remove(name);
	}
	::odata::string_t match_request_header(const ::odata::string_t& name) const
	{
		::odata::string_t value;
		m_request_headers.match(name, value);
		return value;
	}

	// Logging capability for http requests. Be careful when using, this is a comment from
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

		void doLog(wchar_t const* Function, wchar_t const *File, size_t Line, web::http::http_request const &request) const
		{
			if (m_LogCallback && m_ShallLogCallback && m_ShallLogCallback())
			{
				m_LogCallback(Function, File, Line, true, request.to_string()); // true == This is a request
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
	}
};

}}
