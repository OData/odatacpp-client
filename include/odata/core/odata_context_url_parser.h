//---------------------------------------------------------------------
// <copyright file="odata_context_url_parser.h" company="Microsoft">
//      Copyright (C) Microsoft Corporation. All rights reserved. See License.txt in the project root for license information.
// </copyright>
//---------------------------------------------------------------------

#pragma once

#include "odata/common/utility.h"
#include "odata/core/odata_core.h"
#include "cpprest/json.h"
#include "odata/edm/odata_edm.h"

namespace odata { namespace core
{

class odata_contex_url_parser
{
public:
	odata_contex_url_parser(std::shared_ptr<::odata::edm::edm_model> model, ::utility::string_t service_root_url) 
		: m_model(model), m_service_root_url(std::move(service_root_url))
	{
	}

	ODATACPP_CLIENT_API std::shared_ptr<::odata::edm::edm_named_type> get_payload_content_type(const ::utility::string_t& context_url);

private:
	std::shared_ptr<::odata::edm::edm_named_type> parse_context_url(std::list<::utility::string_t>& paths, const std::shared_ptr<::odata::edm::edm_named_type>& current_type);
	std::shared_ptr<::odata::edm::edm_named_type> parse_complex_or_primitive(const ::utility::string_t& current_path);

	std::shared_ptr<::odata::edm::edm_model>  m_model;
	::utility::string_t m_service_root_url; 
};

}}