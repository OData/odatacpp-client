//---------------------------------------------------------------------
// <copyright file="edm_type.cpp" company="Microsoft">
//      Copyright (C) Microsoft Corporation. All rights reserved. See License.txt in the project root for license information.
// </copyright>
//---------------------------------------------------------------------

#include "odata/edm/odata_edm.h"

namespace odata { namespace edm
{


std::shared_ptr<edm_named_type> edm_named_type::_EDM_UNKNOWN = nullptr;

std::shared_ptr<edm_named_type> edm_named_type::EDM_UNKNOWN()
{
	std::lock_guard<std::mutex> lock(mutex());

	if (!_EDM_UNKNOWN)
	{
		_EDM_UNKNOWN.reset(new edm_named_type(edm_type_kind_t::Unknown));
	}

	return _EDM_UNKNOWN;
}

std::mutex &edm_named_type::mutex()
{
	static std::mutex _mutex;
	return _mutex;
}

std::shared_ptr<edm_primitive_type> edm_primitive_type::_BINARY = nullptr;
std::shared_ptr<edm_primitive_type> edm_primitive_type::_BOOLEAN = nullptr;
std::shared_ptr<edm_primitive_type> edm_primitive_type::_BYTE = nullptr;
std::shared_ptr<edm_primitive_type> edm_primitive_type::_DATETIMEOFFSET = nullptr;
std::shared_ptr<edm_primitive_type> edm_primitive_type::_DURATION = nullptr;
std::shared_ptr<edm_primitive_type> edm_primitive_type::_DECIMAL = nullptr;
std::shared_ptr<edm_primitive_type> edm_primitive_type::_DOUBLE = nullptr;
std::shared_ptr<edm_primitive_type> edm_primitive_type::_GUID = nullptr;
std::shared_ptr<edm_primitive_type> edm_primitive_type::_INT16 = nullptr;
std::shared_ptr<edm_primitive_type> edm_primitive_type::_INT32 = nullptr;
std::shared_ptr<edm_primitive_type> edm_primitive_type::_INT64 = nullptr;
std::shared_ptr<edm_primitive_type> edm_primitive_type::_SBYTE = nullptr;
std::shared_ptr<edm_primitive_type> edm_primitive_type::_SINGLE = nullptr;
std::shared_ptr<edm_primitive_type> edm_primitive_type::_STRING = nullptr;
std::shared_ptr<edm_primitive_type> edm_primitive_type::_STREAM = nullptr;
std::shared_ptr<edm_primitive_type>edm_primitive_type:: _UNKNOWN = nullptr;

std::shared_ptr<edm_primitive_type> edm_primitive_type::BINARY()
{
	std::lock_guard<std::mutex> lock(mutex());

	if (!_BINARY)
	{
		_BINARY.reset(new edm_primitive_type(_XPLATSTR("Edm.Binary"), edm_primitive_type_kind_t::Binary));
	}

	return _BINARY;
}

std::shared_ptr<edm_primitive_type> edm_primitive_type::BOOLEAN()
{
	std::lock_guard<std::mutex> lock(mutex());

	if (!_BOOLEAN)
	{
		_BOOLEAN.reset(new edm_primitive_type(_XPLATSTR("Edm.Boolean"), edm_primitive_type_kind_t::Boolean));
	}

	return _BOOLEAN;
}

std::shared_ptr<edm_primitive_type> edm_primitive_type::BYTE()
{
	std::lock_guard<std::mutex> lock(mutex());

	if (!_BYTE)
	{
		_BYTE.reset(new edm_primitive_type(_XPLATSTR("Edm.Byte"), edm_primitive_type_kind_t::Byte));
	}

	return _BYTE;
}

std::shared_ptr<edm_primitive_type> edm_primitive_type::DATETIMEOFFSET()
{
	std::lock_guard<std::mutex> lock(mutex());

	if (!_DATETIMEOFFSET)
	{
		_DATETIMEOFFSET.reset(new edm_primitive_type(_XPLATSTR("Edm.DateTimeOffset"), edm_primitive_type_kind_t::DateTimeOffset));
	}

	return _DATETIMEOFFSET;
}

std::shared_ptr<edm_primitive_type> edm_primitive_type::DURATION()
{
	std::lock_guard<std::mutex> lock(mutex());

	if (!_DURATION)
	{
		_DURATION.reset(new edm_primitive_type(_XPLATSTR("Edm.Duration"), edm_primitive_type_kind_t::Duration));
	}

	return _DURATION;
}

std::shared_ptr<edm_primitive_type> edm_primitive_type::DECIMAL()
{
	std::lock_guard<std::mutex> lock(mutex());

	if (!_DECIMAL)
	{
		_DECIMAL.reset(new edm_primitive_type(_XPLATSTR("Edm.Decimal"), edm_primitive_type_kind_t::Decimal));
	}

	return _DECIMAL;
}

std::shared_ptr<edm_primitive_type> edm_primitive_type::DOUBLE()
{
	std::lock_guard<std::mutex> lock(mutex());

	if (!_DOUBLE)
	{
		_DOUBLE.reset(new edm_primitive_type(_XPLATSTR("Edm.Double"), edm_primitive_type_kind_t::Double));
	}

	return _DOUBLE;
}

std::shared_ptr<edm_primitive_type> edm_primitive_type::GUID()
{
	std::lock_guard<std::mutex> lock(mutex());

	if (!_GUID)
	{
		_GUID.reset(new edm_primitive_type(_XPLATSTR("Edm.Guid"), edm_primitive_type_kind_t::Guid));
	}

	return _GUID;
}

std::shared_ptr<edm_primitive_type> edm_primitive_type::INT16()
{
	std::lock_guard<std::mutex> lock(mutex());

	if (!_INT16)
	{
		_INT16.reset(new edm_primitive_type(_XPLATSTR("Edm.Int16"), edm_primitive_type_kind_t::Int16));
	}

	return _INT16;
}

std::shared_ptr<edm_primitive_type> edm_primitive_type::INT32()
{
	std::lock_guard<std::mutex> lock(mutex());

	if (!_INT32)
	{
		_INT32.reset(new edm_primitive_type(_XPLATSTR("Edm.Int32"), edm_primitive_type_kind_t::Int32));
	}

	return _INT32;
}

std::shared_ptr<edm_primitive_type> edm_primitive_type::INT64()
{
	std::lock_guard<std::mutex> lock(mutex());

	if (!_INT64)
	{
		_INT64.reset(new edm_primitive_type(_XPLATSTR("Edm.Int64"), edm_primitive_type_kind_t::Int64));
	}

	return _INT64;
}

std::shared_ptr<edm_primitive_type> edm_primitive_type::SBYTE()
{
	std::lock_guard<std::mutex> lock(mutex());

	if (!_SBYTE)
	{
		_SBYTE.reset(new edm_primitive_type(_XPLATSTR("Edm.SByte"), edm_primitive_type_kind_t::SByte));
	}

	return _SBYTE;
}

std::shared_ptr<edm_primitive_type> edm_primitive_type::SINGLE()
{
	std::lock_guard<std::mutex> lock(mutex());

	if (!_SINGLE)
	{
		_SINGLE.reset(new edm_primitive_type(_XPLATSTR("Edm.Single"), edm_primitive_type_kind_t::Single));
	}

	return _SINGLE;
}

std::shared_ptr<edm_primitive_type> edm_primitive_type::STRING()
{
	std::lock_guard<std::mutex> lock(mutex());

	if (!_STRING)
	{
		_STRING.reset(new edm_primitive_type(_XPLATSTR("Edm.String"), edm_primitive_type_kind_t::String));
	}

	return _STRING;
}

std::shared_ptr<edm_primitive_type> edm_primitive_type::STREAM()
{
	std::lock_guard<std::mutex> lock(mutex());

	if (!_STREAM)
	{
		_STREAM.reset(new edm_primitive_type(_XPLATSTR("Edm.Stream"), edm_primitive_type_kind_t::Stream));
	}

	return _STREAM;
}

std::shared_ptr<edm_primitive_type> edm_primitive_type::UNKNOWN()
{
	std::lock_guard<std::mutex> lock(mutex());

	if (!_UNKNOWN)
	{
		_UNKNOWN.reset(new edm_primitive_type(_XPLATSTR("UNKNOWN"), edm_primitive_type_kind_t::NoneVal));
	}

	return _UNKNOWN;
}

std::mutex &edm_primitive_type::mutex()
{
	static std::mutex _mutex;
	return _mutex;
}

std::shared_ptr<edm_property_type> edm_structured_type::find_property(::odata::string_t name) const
{
	auto find_iter = m_properties.find(name);
	if (find_iter != m_properties.end())
	{
		return find_iter->second;
	}
	else if (m_base_type)
	{
		// find property type in base type
		return m_base_type->find_property(name);
	}

	return nullptr;
}

}}
