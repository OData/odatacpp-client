//---------------------------------------------------------------------
// <copyright file="odata_entity_model_builder.h" company="Microsoft">
//      Copyright (C) Microsoft Corporation. All rights reserved. See License.txt in the project root for license information.
// </copyright>
//---------------------------------------------------------------------

#pragma once

#include "odata/core/odata_entity_value.h"
#include "odata/edm/odata_edm.h"

namespace odata { namespace core
{

class odata_entity_model_builder
{
public:
	ODATACPP_CLIENT_API static ::odata::string_t compute_edit_link(const ::odata::string_t& root_url, std::shared_ptr<odata_entity_value> entity_value, const ::odata::string_t& parent_edit_link, bool is_collection_navigation);
	ODATACPP_CLIENT_API static ::odata::string_t get_entity_key_value_string(const std::shared_ptr<odata_entity_value>& entity_value);
};

}}
