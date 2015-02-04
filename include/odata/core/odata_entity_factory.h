//---------------------------------------------------------------------
// <copyright file="odata_entity_factory.h" company="Microsoft">
//      Copyright (C) Microsoft Corporation. All rights reserved. See License.txt in the project root for license information.
// </copyright>
//---------------------------------------------------------------------

#pragma once

#include "odata/common/utility.h"
#include "odata/core/odata_core.h"
#include "odata/edm/odata_edm.h"
#include "cpprest/json.h"

namespace odata { namespace core
{

template<typename _Entity_Impl>
class entity_factory
{
public:
	static std::shared_ptr<_Entity_Impl> create_reader_instance(std::shared_ptr<::odata::edm::edm_model> model, const ::utility::string_t& service_root_url)
	{
		return std::make_shared<_Entity_Impl>(model, service_root_url);
	}

	static std::shared_ptr<_Entity_Impl> create_writer_instance(std::shared_ptr<::odata::edm::edm_model> model)
	{
		return std::make_shared<_Entity_Impl>(model);
	}

	static std::shared_ptr<_Entity_Impl> create_context_url_parser(std::shared_ptr<::odata::edm::edm_model> model, const ::utility::string_t& service_root_url)
	{
		return std::make_shared<_Entity_Impl>(model, service_root_url); 
	}
};

}}