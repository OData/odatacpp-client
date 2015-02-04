//---------------------------------------------------------------------
// <copyright file="odata_json_writer.h" company="Microsoft">
//      Copyright (C) Microsoft Corporation. All rights reserved. See License.txt in the project root for license information.
// </copyright>
//---------------------------------------------------------------------

#pragma once

#include "cpprest/json.h"
#include "odata/communication/http_communication.h"
#include "odata/common/utility.h"
#include "odata/core/odata_core.h"
#include "odata/core/odata_json_constants.h"
#include "odata/edm/odata_edm.h"

namespace odata { namespace core
{

class odata_json_writer
{
public:
	odata_json_writer(std::shared_ptr<::odata::edm::edm_model> model) : m_model(model)
	{
	}

	ODATACPP_CLIENT_API ::web::json::value serialize(std::shared_ptr<odata_value> value_object);
	ODATACPP_CLIENT_API ::web::json::value serialize(std::vector<std::shared_ptr<odata_parameter>> parameters);

private:
	void handle_serialize_odata_parameter(::utility::stringstream_t& ss, const std::shared_ptr<::odata::core::odata_parameter>& parameter, ::utility::char_t mark, ::utility::char_t separator);
	void handle_serialize_odata_properties(::utility::stringstream_t& ss, const odata_property_map& properties);
	void handle_serialize_odata_value(::utility::stringstream_t& ss, const std::shared_ptr<::odata::edm::edm_named_type>& property_type, const std::shared_ptr<odata_value>& property_value);
	void handle_serialize_primitive_value(::utility::stringstream_t& ss, const std::shared_ptr<::odata::edm::edm_primitive_type>& p_primitive_type, const std::shared_ptr<odata_primitive_value>& p_value);
	void handle_serialize_enum_value(::utility::stringstream_t& ss, const std::shared_ptr<odata_enum_value>& p_value);
    void handle_serialize_collection_value(::utility::stringstream_t& ss, const std::shared_ptr<odata_collection_value>& p_value);
	bool is_type_serializable(const std::shared_ptr<::odata::edm::edm_named_type>& property_type);

	std::shared_ptr<::odata::edm::edm_model> m_model;
};

}}