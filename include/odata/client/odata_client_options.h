#pragma once

#include "odata/common/utility.h"

namespace odata { namespace client {

class odata_client_credential
{
public:
	odata_client_credential(const ::utility::string_t& username, const ::utility::string_t& password) :
		m_username(username), m_password(password)
	{
	}

	const ::utility::string_t& get_username() const
	{
		return m_username;
	}

	const ::utility::string_t& get_password() const
	{
		return m_password;
	}

private:
	::utility::string_t m_username;
	::utility::string_t m_password;
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
	client_options() : m_prefer_json(true), m_verb_tunneling(false),m_json_mdlevel(U("minimal")), m_credential(nullptr)
    {
    }

    /// <summary>
    /// Sets the client to request responses formatted as JSON, when applicable
    /// </summary>
    client_options prefer_json(::utility::string_t mdlevel = U("minimal"))
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

	void enable_client_credential(const ::utility::string_t& username, const ::utility::string_t& password)
	{
		m_credential = std::make_shared<odata_client_credential>(username, password);
	}

	void disable_client_credential()
	{
		m_credential = nullptr;
	}

	const std::shared_ptr<odata_client_credential>& get_credential_setting()
	{
		return m_credential;
	}

private:
	friend class odata_client;

    bool m_prefer_json;
    bool m_verb_tunneling;
    ::utility::string_t m_json_mdlevel;
	std::shared_ptr<odata_client_credential> m_credential;
};

}}