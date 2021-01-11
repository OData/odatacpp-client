//---------------------------------------------------------------------
// <copyright file="code_generation_helper.h" company="Microsoft">
//      Copyright (C) Microsoft Corporation. All rights reserved. See License.txt in the project root for license information.
// </copyright>
//---------------------------------------------------------------------

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

	::odata::string_t real_type_name = entity_value->get_value_type()->get_name();
	::odata::string_t expected_type_name = T::get_type_name();
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
		ret = ::odata::make_shared<T>(service_context);
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

	::odata::string_t real_type_name = complex_value->get_value_type()->get_name();
	::odata::string_t expected_type_name = T::get_type_name();
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
		ret = ::odata::make_shared<T>(service_context);
		ret->from_value(complex_value);
	}

	return ret;
}

}}
