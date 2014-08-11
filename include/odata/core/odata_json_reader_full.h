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

class entity_json_reader_full
{
public:
	entity_json_reader_full(std::shared_ptr<::odata::edm::edm_model> model, const ::utility::string_t& service_root_url)
		: m_model(model), m_service_root_url(service_root_url)
	{
	};
	
	ODATACPP_API std::shared_ptr<odata_payload> deserilize(const ::web::json::value& response, std::shared_ptr<::odata::edm::edm_entity_set> set);
   
private:
    std::shared_ptr<::odata::edm::edm_model> m_model;
	::utility::string_t m_service_root_url; 

};

}}