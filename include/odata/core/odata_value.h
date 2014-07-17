#pragma once

#include "odata/edm/odata_edm.h"
#include "odata/core/odata_property_map.h"

namespace odata { namespace core
{

class odata_entity_value;

// A combination of the property type (see enum above) and a string representation of the property value.
class odata_value
{
public:
    /// <summary>Default constructor</summary>
    odata_value() : m_property_type(std::make_shared<::odata::edm::edm_named_type>()){}

    odata_value(std::shared_ptr<::odata::edm::edm_named_type>type, bool is_null_value = false) 
		: m_property_type(type), m_is_null_value(is_null_value)
	{
	}

    virtual ~odata_value(){};

    std::shared_ptr<::odata::edm::edm_named_type> get_value_type() const { return m_property_type; }

	void set_value_type(std::shared_ptr<::odata::edm::edm_named_type> property_type)
	{
		m_property_type = property_type;
	}

private:
    friend class entity;
    friend class odata_property_map;

    std::shared_ptr<::odata::edm::edm_named_type> m_property_type;
	bool m_is_null_value;
};

}}