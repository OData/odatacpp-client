#include "odata/core/odata_structured_value.h"
#include "odata/core/odata_collection_value.h"
#include "odata/core/odata_complex_value.h"
#include "odata/core/odata_enum_value.h"
#include "odata/core/odata_entity_value.h"
#include "odata/edm/edm_model_utility.h"

using namespace web;
using namespace odata::edm;

namespace odata { namespace core
{

void odata_structured_value::set_value(const ::utility::string_t& property_name, const ::utility::string_t& string_value)
{
	bool setted = false;
	std::shared_ptr<odata_value> property_value;
	if (get_property_value(property_name, property_value))
	{
		std::shared_ptr<edm_named_type> property_type = property_value->get_value_type();
		if (property_type && (property_type->get_type_kind() == edm_type_kind_t::Primitive))
		{
			setted = true;
			m_properties[property_name] = std::make_shared<odata_primitive_value>(std::dynamic_pointer_cast<edm_primitive_type>(property_type), string_value);
		}
	}
		
	if (!setted)
	{
		m_properties[property_name] = std::make_shared<odata_primitive_value>(edm_primitive_type::STRING(), string_value);
	}
}

void odata_structured_value::set_value(const ::utility::string_t& property_name, std::shared_ptr<odata_primitive_value> property_value)
{
    m_properties[property_name] = property_value;
}

void odata_structured_value::set_value(const ::utility::string_t& property_name, std::shared_ptr<odata_complex_value> property_value)
{
	m_properties[property_name] = property_value;
}

void odata_structured_value::set_value(const ::utility::string_t& property_name, std::shared_ptr<odata_entity_value> property_value)
{
	m_properties[property_name] = property_value;
}

void odata_structured_value::set_value(const ::utility::string_t& property_name, std::shared_ptr<odata_enum_value> property_value)
{
	m_properties[property_name] = property_value;
}

void odata_structured_value::set_value(const ::utility::string_t& property_name, std::shared_ptr<odata_collection_value> property_value)
{
	m_properties[property_name] = property_value;
}

}}