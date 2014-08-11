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