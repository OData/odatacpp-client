//---------------------------------------------------------------------
// <copyright file="odata_json_operation_url_parameter_writer.cpp" company="Microsoft">
//      Copyright (C) Microsoft Corporation. All rights reserved. See License.txt in the project root for license information.
// </copyright>
//---------------------------------------------------------------------

#include "odata/core/odata_json_operation_url_parameter_writer.h"

using namespace ::odata::edm;

namespace odata { namespace core
{

::odata::string_t odata_json_operation_url_parameter_writer::serialize(std::vector<std::shared_ptr<odata_parameter>> parameters)
{
	if (parameters.empty())
	{
		return _XPLATSTR("()");
	}

	::utility::stringstream_t ss;
	ss << _XPLATSTR("(");

	auto iter = parameters.cbegin();
	handle_serialize_odata_parameter(ss, *iter, _XPLATSTR('\0'), _XPLATSTR('='));
	++iter;
	for(; iter != parameters.cend(); ++iter)
	{
		ss << ",";
		handle_serialize_odata_parameter(ss, *iter, _XPLATSTR('\0'), _XPLATSTR('='));
	}

	ss << _XPLATSTR(")");

	return ss.str();
}

void odata_json_operation_url_parameter_writer::handle_serialize_odata_parameter(::utility::stringstream_t& ss, const std::shared_ptr<::odata::core::odata_parameter>& parameter, ::utility::char_t mark, ::utility::char_t separator)
{
	if (parameter && parameter->get_value() && !parameter->get_name().empty())
	{
		if (mark)
		{
			ss << mark << parameter->get_name() << mark;
		}
		else
		{
			ss << parameter->get_name();
		}

		ss << separator;

		handle_serialize_odata_value(ss, parameter->get_value()->get_value_type(), parameter->get_value());
	}
}

void odata_json_operation_url_parameter_writer::handle_serialize_odata_value(::utility::stringstream_t& ss, const std::shared_ptr<::odata::edm::edm_named_type>& property_type, const std::shared_ptr<odata_value>& property_value)
{
	// @TODO: Check if this is really needed! Cnsider that odata_enum_value is again handled below but depends on what get_type_kind() returned! Dealing with odata_enum_value twice appears illogical and inconsistent!
	auto p_value = std::dynamic_pointer_cast<odata_enum_value>(property_value);
	if (p_value)
	{
		return handle_serialize_enum_value(ss, p_value);
	}

	if (!property_type || !property_value)
	{
		ss << "null";

		return;
	}

	switch(property_type->get_type_kind())
	{
	case edm_type_kind_t::Primitive:
		{
			auto p_value = std::dynamic_pointer_cast<odata_primitive_value>(property_value);
			auto p_primitive_type = std::dynamic_pointer_cast<edm_primitive_type>(property_type);

			if (p_value && p_primitive_type)
			{
				handle_serialize_primitive_value(ss, p_primitive_type, p_value);
			}
			else
			{
				ss << "null";
			}
		}
		break;
	case edm_type_kind_t::Enum:
		{
			auto p_value = std::dynamic_pointer_cast<odata_enum_value>(property_value);

			if (p_value)
			{
				handle_serialize_enum_value(ss, p_value);
			}
			else
			{
				ss << "null";
			}
		}
		break;
	case edm_type_kind_t::Complex:
		{
			throw std::runtime_error("writer unsupported type!");
		}
		break;
	case edm_type_kind_t::Collection:
		{
			throw std::runtime_error("writer unsupported type!");
		}
		break;
	case edm_type_kind_t::Entity:
		{
			throw std::runtime_error("writer unsupported type!");
		}
		break;
	default:
		{
			throw std::runtime_error("writer unsupported type!");
		}
		break;
	}
}

void odata_json_operation_url_parameter_writer::handle_serialize_primitive_value(::utility::stringstream_t& ss, const std::shared_ptr<::odata::edm::edm_primitive_type>& p_primitive_type, const std::shared_ptr<odata_primitive_value>& p_value)
{
	if (!p_primitive_type || !p_value)
	{
		return;
	}

	switch(p_primitive_type->get_primitive_kind())
	{
	case edm_primitive_type_kind_t::Boolean:
		{
			if (p_value->as<bool>())
			{
				ss << _XPLATSTR("true");
			}
			else
			{
				ss << _XPLATSTR("false");
			}
		}
		break;
	case edm_primitive_type_kind_t::Byte:
	case edm_primitive_type_kind_t::Decimal:
	case edm_primitive_type_kind_t::Double:
	case edm_primitive_type_kind_t::Int16:
	case edm_primitive_type_kind_t::Int32:
	case edm_primitive_type_kind_t::Int64:
	case edm_primitive_type_kind_t::Single:
	case edm_primitive_type_kind_t::SByte:
		{
			ss << p_value->to_string();
		}
		break;
	case edm_primitive_type_kind_t::Guid:
	case edm_primitive_type_kind_t::String:
	case edm_primitive_type_kind_t::Binary:
	case edm_primitive_type_kind_t::DateTimeOffset:
	case edm_primitive_type_kind_t::Duration:
		{
			ss << _XPLATSTR('\'') << p_value->to_string() << _XPLATSTR('\'');
		}
		break;
	case edm_primitive_type_kind_t::Stream:
		{
			throw std::runtime_error("stream primitive value not implemented!");
		}
		break;
	default:
		{
			throw std::runtime_error("unknown value not implemented!");
		}
		break;
	}
}

void odata_json_operation_url_parameter_writer::handle_serialize_enum_value(::utility::stringstream_t& ss, const std::shared_ptr<odata_enum_value>& p_value)
{
	if (!p_value)
	{
		return;
	}

	ss << p_value->to_string();
}

}}
