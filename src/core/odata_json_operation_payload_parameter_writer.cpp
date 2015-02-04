//---------------------------------------------------------------------
// <copyright file="odata_json_operation_payload_parameter_writer.cpp" company="Microsoft">
//      Copyright (C) Microsoft Corporation. All rights reserved. See License.txt in the project root for license information.
// </copyright>
//---------------------------------------------------------------------

#include "odata/core/odata_json_operation_payload_parameter_writer.h"

using namespace ::odata::edm;

namespace odata { namespace core
{

::web::json::value odata_json_operation_payload_parameter_writer::serialize(std::vector<std::shared_ptr<odata_parameter>> parameters)
{
	if (parameters.empty())
	{
		return web::json::value();
	}

	::utility::stringstream_t ss;
	ss << U("{");

	auto iter = parameters.cbegin();
	handle_serialize_odata_parameter(ss, *iter, U('"'), U(':'));
	iter++;
	for(; iter != parameters.cend(); iter++)
	{
		ss << U(",");
		handle_serialize_odata_parameter(ss, *iter, U('"'), U(':'));
	}

	ss << U("}");

	return web::json::value::parse(ss);
}

void odata_json_operation_payload_parameter_writer::handle_serialize_odata_parameter(
	::utility::stringstream_t& ss, const std::shared_ptr<::odata::core::odata_parameter>& parameter, ::utility::char_t mark, ::utility::char_t separator)
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

void odata_json_operation_payload_parameter_writer::handle_serialize_odata_properties(::utility::stringstream_t& ss, const odata_property_map& properties)
{
	if (properties.size() <= 0)
	{
		ss << "null";

		return ;
	}

	ss << U("{");

	bool first = true;

	for (auto iter = properties.cbegin(); iter != properties.cend(); iter++)
	{	
		if (!iter->second)
		{
			if (!first)
			{
				ss << U(",");
			}

			ss << U('"') << iter->first << U('"') << U(":") << U("null");

			first = false;
		}
		else
		{
			auto property_type = iter->second->get_value_type();

			if (!is_type_serializable(property_type))
			{
				continue;
			}

			if (!first)
			{
				ss << U(",");
			}

			ss << U('"') << iter->first << U('"') << U(":");

			handle_serialize_odata_value(ss, property_type, iter->second);

			first = false;
		}
		
	}

	ss << U("}");
}

void odata_json_operation_payload_parameter_writer::handle_serialize_odata_value(
	::utility::stringstream_t& ss, const std::shared_ptr<::odata::edm::edm_named_type>& property_type, const std::shared_ptr<odata_value>& property_value)
{
	if (!property_type || !property_value)
	{
		ss << "null";

		return ;
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
	case edm_type_kind_t::PayloadAnnotation:
		{
			auto p_value = std::dynamic_pointer_cast<odata_primitive_value>(property_value);

			if (p_value)
			{
				ss << U('"') << p_value->to_string() << U('"');
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
			auto p_value = std::dynamic_pointer_cast<odata_complex_value>(property_value);

			if (p_value)
			{
				handle_serialize_odata_properties(ss, p_value->properties());
			}
			else
			{
				ss << "null";
			}
		}
		break;
	case edm_type_kind_t::Collection:
		{
			auto p_value = std::dynamic_pointer_cast<odata_collection_value>(property_value);

			if (p_value)
			{
				handle_serialize_collection_value(ss, p_value);
			}
			else
			{
				ss << "[]";
			}
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

void odata_json_operation_payload_parameter_writer::handle_serialize_primitive_value(
	::utility::stringstream_t& ss, const std::shared_ptr<::odata::edm::edm_primitive_type>& p_primitive_type, const std::shared_ptr<odata_primitive_value>& p_value)
{
	if (!p_primitive_type || !p_value)
	{
		return ;
	}

	switch(p_primitive_type->get_primitive_kind())
	{
	case edm_primitive_type_kind_t::Boolean:
		{
			if (p_value->as<bool>())
			{
				ss << U("true");
			}
			else
			{
				ss << U("false");
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
	case edm_primitive_type_kind_t::Binary:
	case edm_primitive_type_kind_t::DateTimeOffset:
	case edm_primitive_type_kind_t::Duration:
		{
			ss << U('"') << p_value->to_string() << U('"');
		}
		break;		
	case edm_primitive_type_kind_t::String:
		{
			ss << U('"') << p_value->to_string() << U('"');
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

void odata_json_operation_payload_parameter_writer::handle_serialize_enum_value(::utility::stringstream_t& ss, const std::shared_ptr<odata_enum_value>& p_value)
{
	if (!p_value)
	{
		return ;
	}

	ss << U('"') << p_value->to_string() << U('"');
}

bool odata_json_operation_payload_parameter_writer::is_type_serializable(const std::shared_ptr<edm_named_type>& property_type)
{
	if (property_type)
	{
		if (property_type->get_type_kind() == edm_type_kind_t::Collection
			|| property_type->get_type_kind() == edm_type_kind_t::Entity
			|| property_type->get_type_kind() == edm_type_kind_t::Complex
			|| property_type->get_type_kind() == edm_type_kind_t::Primitive
			|| property_type->get_type_kind() == edm_type_kind_t::Enum
	        || (property_type->get_type_kind() == edm_type_kind_t::PayloadAnnotation && property_type->get_name() == odata_json_constants::PAYLOAD_ANNOTATION_TYPE))
		{
			return true;
		}
	}

	return false;
}

void odata_json_operation_payload_parameter_writer::handle_serialize_collection_value(::utility::stringstream_t& ss, const std::shared_ptr<odata_collection_value>& p_value)
{
	if (!p_value)
	{
		return ;
	}

	if (p_value->get_collection_values().size() == 0)
	{
		ss << U("[]");

		return ;
	}

	ss << U("[");

	bool first = true;

	auto element_type = p_value->get_collection_values()[0]->get_value_type();

	for (auto iter = p_value->get_collection_values().cbegin(); iter != p_value->get_collection_values().cend(); iter++)
	{
		if (!is_type_serializable(element_type))
		{
			continue;
		}

		if (!first)
		{
			ss << U(",");
		}

		handle_serialize_odata_value(ss, element_type, *iter);

		first = false;
	}

	ss << U("]");
}

}}