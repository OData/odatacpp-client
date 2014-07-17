#pragma once

#include "cpprest/json.h"
#include "odata/communication/http_communication.h"
#include "odata/common/utility.h"
#include "odata/core/odata_core.h"
#include "odata/edm/odata_edm.h"

namespace odata { namespace core
{

class odata_json_operation_url_parameter_writer
{
public:
	odata_json_operation_url_parameter_writer(std::shared_ptr<::odata::edm::edm_model> model) : m_model(model)
	{
	}

	ODATACPP_API ::utility::string_t serialize(std::vector<std::shared_ptr<odata_parameter>> parameters);

private:
	void handle_serialize_odata_parameter(::utility::stringstream_t& ss, const std::shared_ptr<::odata::core::odata_parameter>& parameter, ::utility::char_t mark, ::utility::char_t separator);
	void handle_serialize_odata_value(::utility::stringstream_t& ss, const std::shared_ptr<::odata::edm::edm_named_type>& property_type, const std::shared_ptr<odata_value>& property_value);
	void handle_serialize_primitive_value(::utility::stringstream_t& ss, const std::shared_ptr<::odata::edm::edm_primitive_type>& p_primitive_type, const std::shared_ptr<odata_primitive_value>& p_value);
	void handle_serialize_enum_value(::utility::stringstream_t& ss, const std::shared_ptr<odata_enum_value>& p_value);	
	
	std::shared_ptr<::odata::edm::edm_model> m_model;
};

}}