//---------------------------------------------------------------------
// <copyright file="odata_parameter.h" company="Microsoft">
//      Copyright (C) Microsoft Corporation. All rights reserved. See License.txt in the project root for license information.
// </copyright>
//---------------------------------------------------------------------

#pragma once

#include "odata/core/odata_value.h"

namespace odata { namespace core
{
// Represents the parameter in function/action.
class odata_parameter
{
public:
    /// <summary>Default constructor</summary>
	odata_parameter() 
		: m_value(std::make_shared<::odata::core::odata_value>())
	{
	}

    odata_parameter(::utility::string_t name, const std::shared_ptr<::odata::core::odata_value>& value) 
		: m_name(std::move(name)), m_value(value)
	{
	}
	
	const ::utility::string_t& get_name() const { return m_name; }

	void set_name(::utility::string_t name)
	{
		m_name = std::move(name);
	}

    const std::shared_ptr<::odata::core::odata_value>& get_value() const { return m_value; }

	void set_value(const std::shared_ptr<::odata::core::odata_value>& value)
	{
		m_value = value;
	}

private:
	::utility::string_t m_name;
    std::shared_ptr<::odata::core::odata_value> m_value;
};

}}