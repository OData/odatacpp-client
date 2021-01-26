//---------------------------------------------------------------------
// <copyright file="odata_structured_value.h" company="Microsoft">
//      Copyright (C) Microsoft Corporation. All rights reserved. See License.txt in the project root for license information.
// </copyright>
//---------------------------------------------------------------------

#pragma once

#include "odata/edm/odata_edm.h"
#include "odata/core/odata_value.h"
#include "odata/core/odata_primitive_value.h"

namespace odata { namespace core
{
class odata_collection_value;
class odata_entity_value;
class odata_complex_value;
class odata_enum_value;

class odata_structured_value : public odata_value
{
public:
	odata_structured_value(std::shared_ptr<::odata::edm::edm_named_type> type)
		: odata_value(type), m_properties()
	{
	}

	odata_structured_value(std::shared_ptr<::odata::edm::edm_named_type> type, std::pair<odata_property_map, odata_property_map> properties)
		: odata_value(type), m_properties(std::move(properties))
	{
	}

	virtual ~odata_structured_value() = default;

	/// <summary>Returns a const reference to the property map.</summary>
	const odata_property_map& properties() const { return m_properties.first; }

	void set_properties(odata_property_map map) { m_properties.first = std::move(map); }

	/// <summary>Tells whether the structured object has a property of a given name.</summary>
	/// <param name="property_name">The name of the property</param>
	bool has_property(const ::odata::string_t& property_name) const
	{
		std::shared_ptr<odata_value> details;
		return m_properties.first.try_get(property_name, details);
	}

	/// <summary>Tells whether the structured object has a property of a given name.</summary>
	/// <param name="property_name">The name of the property</param>
	bool has_property(const ::odata::string_t& property_name, std::shared_ptr<::odata::edm::edm_named_type>& type) const
	{
		std::shared_ptr<odata_value> details;
		bool result = m_properties.first.try_get(property_name, details);
		if (result)
		{
			type = details->get_value_type();
		}
		return result;
	}

	bool get_property_value(const ::odata::string_t& property_name, std::shared_ptr<odata_value>& property_value)
	{
		return m_properties.first.try_get(property_name, property_value);
	}

	template<typename T>
	bool try_get(const ::odata::string_t& property_name, T& value) const
	{
		return m_properties.first.try_get(property_name, value);
	}

	void set_value(const ::odata::string_t& property_name, float float_value)
	{
		m_properties.first[property_name] = ::odata::make_shared<odata_primitive_value>(::odata::edm::edm_primitive_type::SINGLE(), ::odata::common::print_float(float_value));
	}

	void set_value(const ::odata::string_t& property_name, unsigned char ubyte_value)
	{
		m_properties.first[property_name] = ::odata::make_shared<odata_primitive_value>(::odata::edm::edm_primitive_type::BYTE(), ::utility::conversions::print_string(ubyte_value));
	}

	void set_value(const ::odata::string_t& property_name, char byte_value)
	{
		m_properties.first[property_name] = ::odata::make_shared<odata_primitive_value>(::odata::edm::edm_primitive_type::BYTE(), ::utility::conversions::print_string(byte_value));
	}

	void set_value(const ::odata::string_t& property_name, int32_t int32_value)
	{
		m_properties.first[property_name] = ::odata::make_shared<odata_primitive_value>(::odata::edm::edm_primitive_type::INT32(), ::utility::conversions::print_string(int32_value));
	}

	void set_value(const ::odata::string_t& property_name, const std::vector<unsigned char>& binary_data)
	{
		m_properties.first[property_name] = ::odata::make_shared<odata_primitive_value>(::odata::edm::edm_primitive_type::BINARY(), ::utility::conversions::to_base64(binary_data));
	}

	void set_value(const ::odata::string_t& property_name, bool boolean_value)
	{
		m_properties.first[property_name] = ::odata::make_shared<odata_primitive_value>(::odata::edm::edm_primitive_type::BOOLEAN(), ::utility::conversions::print_string(boolean_value));
	}

	void set_value(const ::odata::string_t& property_name, ::utility::datetime datetime_value)
	{
		m_properties.first[property_name] = ::odata::make_shared<odata_primitive_value>(::odata::edm::edm_primitive_type::DATETIMEOFFSET(), datetime_value.to_string(::utility::datetime::date_format::ISO_8601));
	}

	void set_value(const ::odata::string_t& property_name, ::utility::seconds duration_value)
	{
		m_properties.first[property_name] = ::odata::make_shared<odata_primitive_value>(::odata::edm::edm_primitive_type::DURATION(), ::utility::timespan::seconds_to_xml_duration(duration_value));
	}

	void set_value(const ::odata::string_t& property_name, double double_precision_value)
	{
		m_properties.first[property_name] = ::odata::make_shared<odata_primitive_value>(::odata::edm::edm_primitive_type::DOUBLE(), ::odata::common::print_double(double_precision_value));
	}

	void set_value(const ::odata::string_t& property_name, int64_t int64_value)
	{
		m_properties.first[property_name] = ::odata::make_shared<odata_primitive_value>(::odata::edm::edm_primitive_type::INT64(), ::utility::conversions::print_string(int64_value));
	}

	void set_value(const ::odata::string_t& property_name, uint64_t u_int64_value)
	{
		m_properties.first[property_name] = ::odata::make_shared<odata_primitive_value>(::odata::edm::edm_primitive_type::INT64(), ::utility::conversions::print_string(u_int64_value));
	}

	void set_value(const ::odata::string_t& property_name, int16_t int16_value)
	{
		m_properties.first[property_name] = ::odata::make_shared<odata_primitive_value>(::odata::edm::edm_primitive_type::INT16(), ::utility::conversions::print_string(int16_value));
	}

	void set_value(const ::odata::string_t& property_name, const ::utility::char_t* string_value)
	{
		m_properties.first[property_name] = ::odata::make_shared<odata_primitive_value>(::odata::edm::edm_primitive_type::STRING(), string_value);
	}

	void set_value(const ::odata::string_t& property_name, std::shared_ptr<odata_value> property_value)
	{
		m_properties.first[property_name] = property_value;
	}

	ODATACPP_CLIENT_API void set_value(const ::odata::string_t& property_name, std::shared_ptr<odata_primitive_value> property_value);

	ODATACPP_CLIENT_API void set_value(const ::odata::string_t& property_name, std::shared_ptr<odata_complex_value> property_value);

	ODATACPP_CLIENT_API void set_value(const ::odata::string_t& property_name, std::shared_ptr<odata_entity_value> property_value);

	ODATACPP_CLIENT_API void set_value(const ::odata::string_t& property_name, std::shared_ptr<odata_enum_value> property_value);

	ODATACPP_CLIENT_API void set_value(const ::odata::string_t& property_name, std::shared_ptr<odata_collection_value> property_value);

	ODATACPP_CLIENT_API void set_value(const ::odata::string_t& property_name, const ::odata::string_t& string_value);

	/// <summary>
	/// Removes a property from the structured object if it is present.
	/// </summary>
	/// <param name="string_value">A string representing the property value.</param>
	odata_structured_value& remove(const ::odata::string_t& property_name)
	{
		m_properties.first.erase(property_name);
		return *this;
	}

	const bool is_open() const
	{
		auto type = get_value_type();
		if (type->get_type_kind() == ::odata::edm::edm_type_kind_t::Entity)
		{
			 return std::dynamic_pointer_cast<::odata::edm::edm_entity_type>(type)->is_open();
		}
		if (type->get_type_kind() == ::odata::edm::edm_type_kind_t::Complex)
		{
			return std::dynamic_pointer_cast<::odata::edm::edm_complex_type>(type)->is_open();
		}
		return false;
	}

	/// <summary>Returns a const reference to the complete property map.</summary>
	const std::pair<odata_property_map, odata_property_map>& all_properties() const { return m_properties; }

	const odata_property_map& open_properties() const { return m_properties.second; }

	void set_properties(std::pair<odata_property_map, odata_property_map> maps) { m_properties = std::move(maps); }

	void set_open_properties(odata_property_map map) { m_properties.second = std::move(map); }

	bool has_open_property(const ::odata::string_t& property_name) const
	{
		std::shared_ptr<odata_value> details;
		return m_properties.second.try_get(property_name, details);
	}

	bool has_open_property(const ::odata::string_t& property_name, std::shared_ptr<::odata::edm::edm_named_type>& type) const
	{
		std::shared_ptr<odata_value> details;
		bool result = m_properties.second.try_get(property_name, details);
		if (result)
		{
			type = details->get_value_type();
		}
		return result;
	}

	bool get_open_property_value(const ::odata::string_t& property_name, std::shared_ptr<odata_value>& property_value)
	{
		return m_properties.second.try_get(property_name, property_value);
	}

	bool try_get_open(const ::odata::string_t& property_name, ::odata::string_t& value) const
	{
		return m_properties.second.try_get(property_name, value);
	}

	ODATACPP_CLIENT_API void set_open_value(const ::odata::string_t& property_name, const ::odata::string_t& string_value);

	odata_structured_value& remove_open(const ::odata::string_t& property_name)
	{
		m_properties.second.erase(property_name);
		return *this;
	}

protected:
	std::pair<odata_property_map, odata_property_map> m_properties;
};

}}
