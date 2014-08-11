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
#include "odata/core/odata_value.h"

namespace odata { namespace core
{

class odata_primitive_value : public odata_value
{
public:
    odata_primitive_value(std::shared_ptr<::odata::edm::edm_named_type>type, const ::utility::string_t& stringRep) : odata_value(type), m_string_rep(stringRep)
    {
    }

	static std::shared_ptr<odata_primitive_value> make_primitive_value(int16_t int16_value)
	{
		return std::make_shared<odata_primitive_value>(::odata::edm::edm_primitive_type::INT16(), ::utility::conversions::print_string(int16_value));
	}

	static std::shared_ptr<odata_primitive_value> make_primitive_value(int32_t int32_value)
	{
		return std::make_shared<odata_primitive_value>(::odata::edm::edm_primitive_type::INT32(), ::utility::conversions::print_string(int32_value));
	}

	static std::shared_ptr<odata_primitive_value> make_primitive_value(const std::vector<unsigned char>& binary_data)
	{
		return std::make_shared<odata_primitive_value>(::odata::edm::edm_primitive_type::BINARY(), ::utility::conversions::to_base64(binary_data));
	}

	static std::shared_ptr<odata_primitive_value> make_primitive_value(bool boolean_value)
	{
		return std::make_shared<odata_primitive_value>(::odata::edm::edm_primitive_type::BOOLEAN(), ::utility::conversions::print_string(boolean_value));
	}

	static std::shared_ptr<odata_primitive_value> make_primitive_value(::utility::datetime datetime_value)
	{
		return std::make_shared<odata_primitive_value>(::odata::edm::edm_primitive_type::DATETIMEOFFSET(), datetime_value.to_string(::utility::datetime::date_format::ISO_8601));
	}

	static std::shared_ptr<odata_primitive_value> make_primitive_value(::utility::seconds duration_value)
	{
		return std::make_shared<odata_primitive_value>(::odata::edm::edm_primitive_type::DURATION(), ::utility::timespan::seconds_to_xml_duration(duration_value));
	}

	static std::shared_ptr<odata_primitive_value> make_primitive_value(double double_precision_value)
	{
		return std::make_shared<odata_primitive_value>(::odata::edm::edm_primitive_type::DOUBLE(), ::odata::utility::print_double(double_precision_value));
	}

	static std::shared_ptr<odata_primitive_value> make_primitive_value(float float_precision_value)
	{
		return std::make_shared<odata_primitive_value>(::odata::edm::edm_primitive_type::SINGLE(), ::odata::utility::print_float(float_precision_value));
	}

	static std::shared_ptr<odata_primitive_value> make_primitive_value(int64_t int64_value)
	{
		return std::make_shared<odata_primitive_value>(::odata::edm::edm_primitive_type::INT64(), ::utility::conversions::print_string(int64_value));
	}

	static std::shared_ptr<odata_primitive_value> make_primitive_value(uint64_t u_int64_value)
	{
		return std::make_shared<odata_primitive_value>(::odata::edm::edm_primitive_type::INT64(), ::utility::conversions::print_string(u_int64_value));
	}

	static std::shared_ptr<odata_primitive_value> make_primitive_value(const ::utility::string_t& string_value)
	{
		return std::make_shared<odata_primitive_value>(::odata::edm::edm_primitive_type::STRING(), string_value);
	}

	static std::shared_ptr<odata_primitive_value> make_primitive_value(const ::utility::char_t* string_value)
	{
		return std::make_shared<odata_primitive_value>(::odata::edm::edm_primitive_type::STRING(), string_value);
	}

    const ::utility::string_t& to_string() const
    {
        return m_string_rep; 
    }

    template<typename T> T as()
    {
        T result;
        _try_get(result);
        return result;
    }

private:
	::utility::string_t    m_string_rep;

    ODATACPP_API void _try_get(std::vector<unsigned char>& value) const;
    ODATACPP_API void _try_get(unsigned char& value) const;
    ODATACPP_API void _try_get(char& value) const;
    ODATACPP_API void _try_get(bool& value) const;
    ODATACPP_API void _try_get(::utility::datetime& value) const;
	ODATACPP_API void _try_get(::utility::seconds& value) const;
    ODATACPP_API void _try_get(double& value) const;
	ODATACPP_API void _try_get(float& value) const;
	ODATACPP_API void _try_get(int16_t& value) const;
    ODATACPP_API void _try_get(int32_t& value) const;
    ODATACPP_API void _try_get(int64_t& value) const;
	ODATACPP_API void _try_get(uint64_t& value) const;
    ODATACPP_API void _try_get(::utility::string_t& value) const;
};

}}