#include "odata/core/odata_json_reader_full.h"

using namespace ::web;
using namespace ::odata::edm;
using namespace ::odata::communication;
using namespace ::odata::utility;

namespace odata { namespace core
{

std::shared_ptr<odata_payload> entity_json_reader_full::deserilize(const web::json::value& response, std::shared_ptr<edm_entity_set> set)
{
	throw std::runtime_error("full metadata reader not implemented!"); 
}

}}