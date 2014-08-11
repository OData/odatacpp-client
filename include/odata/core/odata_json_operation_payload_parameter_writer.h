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
 
 #pragma once

#include "cpprest/json.h"
#include "odata/communication/http_communication.h"
#include "odata/common/utility.h"
#include "odata/core/odata_core.h"
#include "odata/edm/odata_edm.h"

namespace odata { namespace core
{

class odata_json_operation_payload_parameter_writer
{
public:
	odata_json_operation_payload_parameter_writer(std::shared_ptr<::odata::edm::edm_model> model) : m_model(model)
	{
	}

	ODATACPP_API ::web::json::value serialize(std::vector<std::shared_ptr<odata_parameter>> parameters);

private:
	void handle_serialize_odata_parameter(::utility::stringstream_t& ss, const std::shared_ptr<::odata::core::odata_parameter>& parameter, ::utility::char_t mark, ::utility::char_t separator);
	void handle_serialize_odata_value(::utility::stringstream_t& ss, const std::shared_ptr<::odata::edm::edm_named_type>& property_type, const std::shared_ptr<odata_value>& property_value);
	void handle_serialize_primitive_value(::utility::stringstream_t& ss, const std::shared_ptr<::odata::edm::edm_primitive_type>& p_primitive_type, const std::shared_ptr<odata_primitive_value>& p_value);
	void handle_serialize_enum_value(::utility::stringstream_t& ss, const std::shared_ptr<odata_enum_value>& p_value);
	void handle_serialize_collection_value(::utility::stringstream_t& ss, const std::shared_ptr<odata_collection_value>& p_value);
	void handle_serialize_odata_properties(::utility::stringstream_t& ss, const odata_property_map& properties);
	bool is_type_serializable(const std::shared_ptr<::odata::edm::edm_named_type>& property_type);
	
	std::shared_ptr<::odata::edm::edm_model> m_model;
};

}}