#include "odata/codegen/code_generation_base.h"
#include "odata/codegen/odata_service_context.h"

namespace odata { namespace codegen {

	web::http::http_headers const& type_base::get_model_response_header() const
	{
		return m_service_context->get_client()->get_model_response_header();
	}
	::odata::string_t type_base::match_model_response_header(const ::odata::string_t &name) const
	{
		return m_service_context->get_client()->match_model_response_header(name);
	}
	bool type_base::has_model_response_header(const ::odata::string_t &name) const
	{
		return m_service_context->get_client()->has_model_response_header(name);
	}

}}
