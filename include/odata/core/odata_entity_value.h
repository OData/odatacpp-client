//---------------------------------------------------------------------
// <copyright file="odata_entity_value.h" company="Microsoft">
//      Copyright (C) Microsoft Corporation. All rights reserved. See License.txt in the project root for license information.
// </copyright>
//---------------------------------------------------------------------

#pragma once

#include "odata/edm/odata_edm.h"
#include "odata/core/odata_value.h"
#include "odata/core/odata_structured_value.h"

namespace odata { namespace core
{

class odata_entity_value : public odata_structured_value
{
public:
	odata_entity_value(std::shared_ptr<::odata::edm::edm_entity_type> type) : odata_structured_value(type)
	{}

    odata_entity_value(odata_property_map properties, std::shared_ptr<::odata::edm::edm_entity_type> type) : odata_structured_value(type, properties)
	{}

	ODATACPP_CLIENT_API ::utility::string_t get_entity_key_string();

private:
    ::utility::string_t to_key(std::shared_ptr<odata_primitive_value> value);
};

}}