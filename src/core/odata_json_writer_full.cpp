#include "odata/core/odata_json_writer_full.h"

using namespace ::web;
using namespace ::odata::edm;
using namespace ::odata::communication;

namespace odata { namespace core
{

web::json::value entity_json_writer_full::serialize(std::shared_ptr<odata_value> value_object)
{
	throw std::runtime_error("full metadata writer not implemented!"); 
}

web::json::value entity_json_writer_full::serialize(std::vector<std::shared_ptr<odata_value>> value_objects)
{
	throw std::runtime_error("full metadata writer not implemented!"); 
}

}}