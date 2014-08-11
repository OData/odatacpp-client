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

#include "odata/edm/odata_edm.h"

namespace odata { namespace core
{

class odata_value;
class odata_complex_value;
class odata_entity_value;
class odata_primitive_value;
class odata_enum_value;
class odata_collection_value;

typedef ::utility::string_t odata_property_key;
typedef ::utility::string_t odata_property_name;
typedef std::unordered_map<odata_property_name, std::shared_ptr<odata_value>> odata_properties;

/// <summary>
/// A type that holds the properties for an entity or a complex.
///     Key (odata_property_name) - The property name 
///     Value (property_value) - see above 
/// </summary>
class odata_property_map 
{
public:
    odata_property_map()
    {
    }

    odata_property_map(odata_property_map& property_map)
	{
		m_properties = std::move(property_map.m_properties);
	}
    
    odata_property_map(odata_property_map&& property_map)
	{
		m_properties = std::move(property_map.m_properties);
	}
    
    odata_property_map(const odata_property_map& property_map)
	{
		m_properties = std::move(property_map.m_properties);
	}
    
    odata_property_map(const odata_property_map&& property_map)
	{
		m_properties = std::move(property_map.m_properties);
	}

	odata_property_map& operator = (odata_property_map&& property_map)
	{
		m_properties = std::move(property_map.m_properties);
		return *this;
	}
    
    odata_property_map& operator = (odata_property_map& property_map)
	{
		m_properties = std::move(property_map.m_properties);
		return *this;
	}
    
    odata_property_map& operator = (const odata_property_map&& property_map)
	{
		m_properties = std::move(property_map.m_properties);
		return *this;
	}
    
    odata_property_map& operator = (const odata_property_map& property_map)
	{
		m_properties = std::move(property_map.m_properties);
		return *this;
	}

	ODATACPP_API std::shared_ptr<odata_value>& operator[](odata_property_name&& _Keyval)
    {
         return m_properties[_Keyval];
    }

	ODATACPP_API std::shared_ptr<odata_value>& operator[](odata_property_name& _Keyval)
    {
         return m_properties[_Keyval];
    }

	ODATACPP_API std::shared_ptr<odata_value>& operator[](const odata_property_name& _Keyval)
    {
         return m_properties[_Keyval];
    }

    ODATACPP_API odata_properties::iterator erase(odata_properties::const_iterator _Where)
    {
        return m_properties.erase(_Where);
    }

    ODATACPP_API odata_properties::size_type erase(const odata_property_name& _Keyval)
    {
        return m_properties.erase(_Keyval);
    }

    ODATACPP_API bool try_get(const ::utility::string_t& property_name, std::shared_ptr<odata_value>& value) const;
	ODATACPP_API bool try_get(const ::utility::string_t& property_name, std::shared_ptr<odata_complex_value>& value) const;
	ODATACPP_API bool try_get(const ::utility::string_t& property_name, std::shared_ptr<odata_primitive_value>& value) const;
	ODATACPP_API bool try_get(const ::utility::string_t& property_name, std::shared_ptr<odata_entity_value>& value) const;
	ODATACPP_API bool try_get(const ::utility::string_t& property_name, std::shared_ptr<odata_enum_value>& value) const;
	ODATACPP_API bool try_get(const ::utility::string_t& property_name, std::shared_ptr<odata_collection_value>& value) const;
	ODATACPP_API bool try_get(const ::utility::string_t& property_name, char& value) const;
	ODATACPP_API bool try_get(const ::utility::string_t& property_name, unsigned char& value) const;
    ODATACPP_API bool try_get(const ::utility::string_t& property_name, std::vector<unsigned char>& value) const;
    ODATACPP_API bool try_get(const ::utility::string_t& property_name, bool& value) const;
    ODATACPP_API bool try_get(const ::utility::string_t& property_name, ::utility::datetime& value) const;
    ODATACPP_API bool try_get(const ::utility::string_t& property_name, ::utility::seconds& value) const;
    ODATACPP_API bool try_get(const ::utility::string_t& property_name, double& value) const;
	ODATACPP_API bool try_get(const ::utility::string_t& property_name, float& value) const;
    ODATACPP_API bool try_get(const ::utility::string_t& property_name, int32_t& value) const;
	ODATACPP_API bool try_get(const ::utility::string_t& property_name, int16_t& value) const;
    ODATACPP_API bool try_get(const ::utility::string_t& property_name, int64_t& value) const;
    ODATACPP_API bool try_get(const ::utility::string_t& property_name, ::utility::string_t& value) const;
    ODATACPP_API bool try_get(const ::utility::string_t& property_name, odata_property_map& value) const;

    /// <summary>
    /// Gets the beginning const_iterator of the properties
    /// </summary>
    odata_properties::const_iterator cbegin() const
    {
        return m_properties.cbegin();
    }

    /// <summary>
    /// Gets the end const_iterator of the properties
    /// </summary>
    odata_properties::const_iterator cend() const
    {
        return m_properties.cend();
    }

	int size() const 
	{
		return m_properties.size();
	}

private:
    friend class entity;
    odata_properties m_properties;

	ODATACPP_API std::pair<bool, std::shared_ptr<odata_value>> find_property(const ::utility::string_t& property_name) const;
};

}}