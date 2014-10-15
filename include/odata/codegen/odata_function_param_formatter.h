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

template<typename T>
std::shared_ptr<::odata::core::odata_value> format_function_primitive_parameter(const T& t)
{
	return ::odata::core::odata_primitive_value::make_primitive_value(t);
}

template<typename T>
std::shared_ptr<::odata::core::odata_value> format_function_class_parameter(T t)
{
	return t.to_value();
}

template<typename T>
std::shared_ptr<::odata::core::odata_value> format_function_parameter(const T& t, std::false_type fa, std::true_type tr)
{
	return format_function_class_parameter(t);
}

template<typename T>
std::shared_ptr<::odata::core::odata_value> format_function_parameter(const T& t, std::false_type fa, std::false_type fa2nd)
{
	return format_function_primitive_parameter(t);
}

template<typename T>
std::shared_ptr<::odata::core::odata_value> format_function_parameter(const T& t, std::false_type fa)
{
	// a primitive or a class 
	return format_function_parameter(t, fa, integral_constant<bool, std::is_base_of<::odata::codegen::type_base, T>::value>());
}

template<typename T>
std::shared_ptr<::odata::core::odata_value> format_function_enum_parameter(const T& t)
{
	::utility::stringstream_t ostr;

	ostr << enum_type_resolver::get_string_from_enum_type(t);

	return std::make_shared<::odata::core::odata_enum_value>(std::make_shared<::odata::edm::edm_enum_type>(U(""), U(""), U(""), false), ostr.str());
}

template<typename T>
std::shared_ptr<::odata::core::odata_value> format_function_parameter(const T& t, std::true_type tr)
{
	return format_function_enum_parameter(t);
}

template<typename T>
std::shared_ptr<::odata::core::odata_value> format_function_parameter(const T& t)
{
	// a enum or other
	return format_function_parameter(t, integral_constant<bool, std::is_enum<T>::value>());
}

// shared_ptr version
template<typename T>
std::shared_ptr<::odata::core::odata_value> format_function_parameter(const std::shared_ptr<T>& t)
{
	// a enum or other
	return t ? format_function_parameter(*(t.get()), integral_constant<bool, std::is_enum<T>::value>()) : nullptr;
}

template<typename T>
std::shared_ptr<::odata::core::odata_value> format_function_parameter(const std::vector<T>& t)
{
	auto collection = std::make_shared<::odata::core::odata_collection_value>(std::make_shared<::odata::edm::edm_collection_type>(U("action parameter")));

	for (auto iter = t.cbegin(); iter != t.cend(); ++iter)
	{
		collection->add_collection_value(format_function_parameter(*iter, integral_constant<bool, std::is_enum<T>::value>()));
	}

	return collection;
}
