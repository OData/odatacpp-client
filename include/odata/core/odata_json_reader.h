//---------------------------------------------------------------------
// <copyright file="odata_json_reader.h" company="Microsoft">
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

class odata_json_reader
{
public:
	odata_json_reader(std::shared_ptr<::odata::edm::edm_model> model, ::utility::string_t service_root_url) 
		: m_model(model), m_service_root_url(std::move(service_root_url))
	{
	};
	
	ODATACPP_CLIENT_API std::shared_ptr<odata_payload> deserilize(const web::json::value& content);
   
private:
	odata_property_map handle_extract_entity_property(::web::json::value& value, std::shared_ptr<::odata::edm::edm_entity_type>& edm_entity_type);
    odata_property_map handle_extract_complex_property(::web::json::value& value, std::shared_ptr<::odata::edm::edm_complex_type>& edm_complex_type);
	std::shared_ptr<odata_collection_value> handle_extract_collection_property(std::shared_ptr<::odata::edm::edm_named_type> type, web::json::value& value);
	void handle_extract_entity_annotation(const ::utility::string_t& annotation, const ::utility::string_t& value, odata_property_map& result);
	void handle_extract_navigation_property(::web::json::value& value, std::shared_ptr<::odata::edm::edm_navigation_type> navigation_type, odata_property_map& result, const ::utility::string_t& name);
	::utility::string_t get_edit_link_from_context_url(const ::utility::string_t& context_url);
	void set_edit_link_for_entity_value(const std::shared_ptr<odata_entity_value>& entity_value, const ::utility::string_t& expect_type_name, const ::utility::string_t& edit_link, bool is_collection);
   
	std::shared_ptr<::odata::edm::edm_model> m_model;
	::utility::string_t m_service_root_url; 
    std::shared_ptr<::odata::edm::edm_entity_set> m_entity_set;
};

}}