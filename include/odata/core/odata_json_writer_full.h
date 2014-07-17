#pragma once

#include "cpprest/json.h"
#include "odata/communication/http_communication.h"
#include "odata/common/utility.h"
#include "odata/core/odata_core.h"
#include "odata/edm/odata_edm.h"


namespace odata { namespace core
{

class entity_json_writer_full
{
public:
	entity_json_writer_full(std::shared_ptr<::odata::edm::edm_model> schema) : m_model(schema)
	{
	}

	ODATACPP_API ::web::json::value serialize(std::shared_ptr<odata_value> value_object);
	ODATACPP_API ::web::json::value serialize(std::vector<std::shared_ptr<odata_value>> value_objects);


private:
	std::shared_ptr<::odata::edm::edm_model> m_model;
};

}}



