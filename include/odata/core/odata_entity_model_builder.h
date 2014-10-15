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

#include "odata/core/odata_entity_value.h"
#include "odata/edm/odata_edm.h"

namespace odata { namespace core
{

class odata_entity_model_builder
{
public:
	ODATACPP_CLIENT_API static ::utility::string_t compute_edit_link(
		const ::utility::string_t& root_url, 
		std::shared_ptr<odata_entity_value> entity_value, 
		const ::utility::string_t& parent_edit_link, 
		bool is_collection_navigation);

	ODATACPP_CLIENT_API static ::utility::string_t get_entity_key_value_string(const std::shared_ptr<odata_entity_value>& entity_value);
};

}}