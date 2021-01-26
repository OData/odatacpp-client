//---------------------------------------------------------------------
// <copyright file="odata_enum_value.h" company="Microsoft">
//      Copyright (C) Microsoft Corporation. All rights reserved. See License.txt in the project root for license information.
// </copyright>
//---------------------------------------------------------------------

#pragma once

#include "odata/edm/odata_edm.h"
#include "odata/core/odata_value.h"

namespace odata { namespace core
{

class odata_enum_value : public odata_value
{
public:
	odata_enum_value(std::shared_ptr<::odata::edm::edm_named_type>type, ::odata::string_t stringRep) : odata_value(type), m_string_rep(std::move(stringRep))
	{
	}

	const ::odata::string_t& to_string() const
	{
		return m_string_rep;
	}

private:
	::odata::string_t m_string_rep;
};

}}
