//---------------------------------------------------------------------
// <copyright file="odata_client_options.h" company="Microsoft">
//      Copyright (C) Microsoft Corporation. All rights reserved. See License.txt in the project root for license information.
// </copyright>
//---------------------------------------------------------------------

#pragma once

#include "odata/common/utility.h"

namespace odata { namespace client {

class odata_client_credential
{
public:
	odata_client_credential(::odata::string_t username, ::odata::string_t password) :
		m_username(std::move(username)), m_password(std::move(password))
	{
	}

	const ::odata::string_t& get_username() const
	{
		return m_username;
	}

	const ::odata::string_t& get_password() const
	{
		return m_password;
	}

private:
	::odata::string_t m_username;
	::odata::string_t m_password;
};

/// <summary>
/// Represents options used when creating an OData client instance.
/// </summary>
class odata_client;

class client_options
{
public:
	/// <summary>
	/// Represents options used when creating an OData client instance.
	/// </summary>
	client_options() : m_prefer_json(true), m_verb_tunneling(false), m_json_mdlevel(_XPLATSTR("minimal")), m_credential(nullptr), m_suppress_odata_type(false)
	{
	}

	/// <summary>
	/// Sets the client to request responses formatted as JSON, when applicable
	/// </summary>
	client_options prefer_json(::odata::string_t mdlevel = _XPLATSTR("minimal"))
	{
		m_prefer_json = true;
		m_json_mdlevel = mdlevel;
		return *this;
	}

	/// <summary>
	/// Sets the client to request responses formatted as an Atom feed, when applicable
	/// </summary>
	client_options prefer_atom()
	{
		m_prefer_json = false;
		m_json_mdlevel.clear();
		return *this;
	}

	/// <summary>
	/// Use verb tunneling when making non-GET/POST requests.
	/// </summary>
	client_options do_verb_tunneling()
	{
		m_verb_tunneling = true;
		return *this;
	}

	void enable_client_credential(::odata::string_t username, ::odata::string_t password)
	{
		m_credential = ::odata::make_shared<odata_client_credential>(std::move(username), std::move(password));
	}

	void disable_client_credential()
	{
		m_credential = nullptr;
	}

	void set_suppress_odata_type(bool SuppressODataType)
	{
		m_suppress_odata_type = SuppressODataType;
	}

	const std::shared_ptr<odata_client_credential>& get_credential_setting() const
	{
		return m_credential;
	}

	bool get_suppress_odata_type() const
	{
		return m_suppress_odata_type;
	}

private:
	friend class odata_client;

	bool                                     m_prefer_json;
	bool                                     m_verb_tunneling;
	::odata::string_t                        m_json_mdlevel;
	std::shared_ptr<odata_client_credential> m_credential;
	bool                                     m_suppress_odata_type;
};

}}
