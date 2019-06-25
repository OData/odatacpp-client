//---------------------------------------------------------------------
// <copyright file="odata_property_map.cpp" company="Microsoft">
//      Copyright (C) Microsoft Corporation. All rights reserved. See License.txt in the project root for license information.
// </copyright>
//---------------------------------------------------------------------

#include "odata/core/odata_core.h"
#include "odata/edm/edm_model_utility.h"
#include "cpprest/http_msg.h"
#include "cpprest/details/http_helpers.h"

using namespace ::pplx;
using namespace ::web;
using namespace ::odata::edm;

namespace odata { namespace core
{

bool odata_property_map::try_get(const ::utility::string_t& property_name, char& value) const
{
    auto found = find_property(property_name);
    if (found.first)
    {
	    auto primitive_value = std::dynamic_pointer_cast<odata_primitive_value>(found.second);
		if (primitive_value)
		{
            const auto& propValue = primitive_value->to_string();
#ifdef _UTF16_STRINGS
            std::istringstream iss(::utility::conversions::utf16_to_utf8(propValue));
#else
            std::istringstream iss(propValue);
#endif
            iss >> value;

            return true;
		}
    }
    return false;
}

bool odata_property_map::try_get(const ::utility::string_t& property_name, unsigned char& value) const
{
    auto found = find_property(property_name);
    if (found.first)
    {
	    auto primitive_value = std::dynamic_pointer_cast<odata_primitive_value>(found.second);
		if (primitive_value)
		{
            const auto& propValue = primitive_value->to_string();
#ifdef _UTF16_STRINGS
            std::istringstream iss(::utility::conversions::utf16_to_utf8(propValue));
#else
            std::istringstream iss(propValue);
#endif
            iss >> value;

            return true;
		}
    }
    return false;
}

bool odata_property_map::try_get(const ::utility::string_t& property_name, std::vector<unsigned char>& value) const
{
	auto found = find_property(property_name);
    if (found.first)
    {
		auto primitive_value = std::dynamic_pointer_cast<odata_primitive_value>(found.second);
		if (primitive_value)
		{
		    std::vector<unsigned char> temp(::utility::conversions::from_base64(primitive_value->to_string()));
            value.swap(temp);
		    return true;
		}
    }
    return false;
}

bool odata_property_map::try_get(const ::utility::string_t& property_name, bool& value) const
{
    auto found = find_property(property_name);
    if (found.first)
    {
	    auto primitive_value = std::dynamic_pointer_cast<odata_primitive_value>(found.second);
		if (primitive_value)
		{
            const auto& propValue = primitive_value->to_string();
            if (propValue == U("true") || propValue == U("1")) value = true;
            else value = false;
            return true;
		}
    }
    return false;
}

bool odata_property_map::try_get(const ::utility::string_t& property_name, ::utility::datetime& value) const
{
   auto found = find_property(property_name);
    if (found.first)
    {
	    auto primitive_value = std::dynamic_pointer_cast<odata_primitive_value>(found.second);
		if (primitive_value)
		{
            value = ::utility::datetime::from_string(primitive_value->to_string(), ::utility::datetime::ISO_8601);
            return true;
		}
    }
    return false;
}

bool odata_property_map::try_get(const ::utility::string_t& property_name, ::utility::seconds& value) const
{
   auto found = find_property(property_name);
    if (found.first)
    {
	    auto primitive_value = std::dynamic_pointer_cast<odata_primitive_value>(found.second);
		if (primitive_value)
		{
			value = ::utility::timespan::xml_duration_to_seconds(primitive_value->to_string());
            return true;
		}
    }
    return false;
}

bool odata_property_map::try_get(const ::utility::string_t& property_name, double& value) const
{
    auto found = find_property(property_name);
    if (found.first)
    {
	    auto primitive_value = std::dynamic_pointer_cast<odata_primitive_value>(found.second);
		if (primitive_value)
		{
            const auto& propValue = primitive_value->to_string();
            http::bind(propValue, value);
            return true;
		}
    }
    return false;
}

bool odata_property_map::try_get(const ::utility::string_t& property_name, float& value) const
{
    auto found = find_property(property_name);
    if (found.first)
    {
	    auto primitive_value = std::dynamic_pointer_cast<odata_primitive_value>(found.second);
		if (primitive_value)
		{
            const auto& propValue = primitive_value->to_string();
            http::bind(propValue, value);
            return true;
		}
    }
    return false;
}

bool odata_property_map::try_get(const ::utility::string_t& property_name, int16_t& value) const
{
    auto found = find_property(property_name);
    if (found.first)
    {
	    auto primitive_value = std::dynamic_pointer_cast<odata_primitive_value>(found.second);
		if (primitive_value)
		{
            const auto& propValue = primitive_value->to_string();
            http::bind(propValue, value);
            return true;
		}
    }
    return false;
}

bool odata_property_map::try_get(const ::utility::string_t& property_name, int32_t& value) const
{
    auto found = find_property(property_name);
    if (found.first)
    {
	    auto primitive_value = std::dynamic_pointer_cast<odata_primitive_value>(found.second);
		if (primitive_value)
		{
            const auto& propValue = primitive_value->to_string();
            http::bind(propValue, value);
            return true;
		}
    }
    return false;
}

bool odata_property_map::try_get(const ::utility::string_t& property_name, int64_t& value) const
{
    auto found = find_property(property_name);
    if (found.first)
    {
	    auto primitive_value = std::dynamic_pointer_cast<odata_primitive_value>(found.second);
		if (primitive_value)
		{
            const auto& propValue = primitive_value->to_string();
            http::bind(propValue, value);
            return true;
		}
    }
    return false;
}

bool odata_property_map::try_get(const ::utility::string_t& property_name, ::utility::string_t& value) const
{
    auto found = find_property(property_name);
    if (found.first)
    {
	    auto primitive_value = std::dynamic_pointer_cast<odata_primitive_value>(found.second);
		if (primitive_value)
		{
            value = primitive_value->to_string();
            return true;
		}
    }
    return false;
}

bool odata_property_map::try_get(const ::utility::string_t& property_name, odata_property_map& value) const
{
	auto found = find_property(property_name);
    if (found.first)
    {
		std::shared_ptr<odata_complex_value> p_complex_value = std::dynamic_pointer_cast<odata_complex_value>(found.second);
		if (p_complex_value)
		{
		    value = p_complex_value->properties();
            return true;
		}
    }
    return false;
}

bool odata_property_map::try_get(const ::utility::string_t& property_name, std::shared_ptr<odata_value>& value) const
{
    auto found = find_property(property_name);
    if (found.first)
    {
        value = found.second;
        return true;
    }
    return false;
}

bool odata_property_map::try_get(const ::utility::string_t& property_name, std::shared_ptr<odata_complex_value>& value) const
{
    auto found = find_property(property_name);
    if (found.first)
    {
		value = std::dynamic_pointer_cast<odata_complex_value>(found.second);
        return true;
    }
    return false;
}

bool odata_property_map::try_get(const ::utility::string_t& property_name, std::shared_ptr<odata_primitive_value>& value) const
{
    auto found = find_property(property_name);
    if (found.first)
    {
        value = std::dynamic_pointer_cast<odata_primitive_value>(found.second);
        return true;
    }
    return false;
}

bool odata_property_map::try_get(const ::utility::string_t& property_name, std::shared_ptr<odata_entity_value>& value) const
{
    auto found = find_property(property_name);
    if (found.first)
    {
        value = std::dynamic_pointer_cast<odata_entity_value>(found.second);
        return true;
    }
    return false;
}

bool odata_property_map::try_get(const ::utility::string_t& property_name, std::shared_ptr<odata_enum_value>& value) const
{
    auto found = find_property(property_name);
    if (found.first)
    {
        value = std::dynamic_pointer_cast<odata_enum_value>(found.second);
        return true;
    }
    return false;
}

bool odata_property_map::try_get(const ::utility::string_t& property_name, std::shared_ptr<odata_collection_value>& value) const
{
    auto found = find_property(property_name);
    if (found.first)
    {
        value = std::dynamic_pointer_cast<odata_collection_value>(found.second);
        return true;
    }
    return false;
}

std::pair<bool, std::shared_ptr<odata_value>> odata_property_map::find_property(const ::utility::string_t& property_name) const
 {
    const auto& findIt = m_properties.find(property_name);
    bool found = (findIt != m_properties.end());

    if (found)
    {
        const auto& propDetails = findIt->second;

		if (!propDetails)
		{
			return make_pair(found, propDetails);
		}

        return make_pair(found, propDetails);	 
    }

    return make_pair(found, std::shared_ptr<odata_value>());	 
 }
}}
