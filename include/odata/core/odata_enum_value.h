#pragma once

#include "odata/edm/odata_edm.h"
#include "odata/core/odata_value.h"

namespace odata { namespace core
{

class odata_enum_value : public odata_value
{
public:
    odata_enum_value(std::shared_ptr<::odata::edm::edm_named_type>type, const ::utility::string_t& stringRep) : odata_value(type), m_string_rep(stringRep)
    {
    }

    const ::utility::string_t& to_string() const
    {
        return m_string_rep; 
    }

private:
	::utility::string_t    m_string_rep;

};

}}