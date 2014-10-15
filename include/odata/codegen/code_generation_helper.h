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
#include "odata/codegen/code_generation_base.h"

namespace odata { namespace codegen { 

template<typename T>
std::shared_ptr<T> create_instance_from_entity(const std::shared_ptr<::odata::core::odata_entity_value>& entity_value, const std::shared_ptr<odata_service_context>& service_context)
{
	std::shared_ptr<T> ret = nullptr;

	if (!entity_value) 
	{
		return ret; 
	}

	::utility::string_t real_type_name = entity_value->get_value_type()->get_name(); 
	::utility::string_t expected_type_name = T::get_type_name(); 
	if (expected_type_name != real_type_name) 
	{ 
		auto create_pfn = T::_derived_entity_creator_map[real_type_name];
		if (!create_pfn) 
		{
			return ret;
		}
		std::shared_ptr<type_base> p_derived = (*create_pfn)(service_context); 
		if (!p_derived) 
		{
			return ret;
		}
		p_derived->from_value(entity_value);
		ret = std::dynamic_pointer_cast<T>(p_derived); 
	} 
	else 
	{ 
        ret = std::make_shared<T>(service_context); 
		ret->from_value(entity_value); 
	} 

	return ret;
}

template<typename T>
std::shared_ptr<T> create_instance_from_complex(const std::shared_ptr<::odata::core::odata_complex_value>& complex_value, const std::shared_ptr<odata_service_context>& service_context)
{
	std::shared_ptr<T> ret = nullptr;

	if (!complex_value) 
	{
		return ret; 
	}

	::utility::string_t real_type_name = complex_value->get_value_type()->get_name(); 
	::utility::string_t expected_type_name = T::get_type_name(); 
	if (expected_type_name != real_type_name) 
	{ 
		auto create_pfn = T::_derived_complex_creator_map[real_type_name];
		if (!create_pfn) 
		{
			return ret;
		}
		std::shared_ptr<type_base> p_derived = (*create_pfn)(service_context); 
		if (!p_derived) 
		{
			return ret;
		}
		p_derived->from_value(complex_value);
		ret = std::dynamic_pointer_cast<T>(p_derived); 
	} 
	else 
	{ 
        ret = std::make_shared<T>(service_context); 
		ret->from_value(complex_value); 
	} 

	return ret;
}

}}