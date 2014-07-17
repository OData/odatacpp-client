#pragma once
#include "odata/common/utility.h"
#include "cpprest/json.h"
#include "odata/core/odata_core.h"

namespace odata { namespace codegen { 

#define SAFE_DELETE(value) \
	if (value) \
	{ \
	    delete value; \
		value = nullptr; \
	}

class odata_service_context;

class type_base 
{
public:
	type_base(const std::shared_ptr<odata_service_context>& service_context) : m_service_context(service_context){}
	virtual ~type_base(){}

	virtual void from_value(const std::shared_ptr<::odata::core::odata_entity_value>& pentity) {}
	virtual void from_value(const std::shared_ptr<::odata::core::odata_complex_value>& pentity) {}

	const ::utility::string_t& get_edit_link()
	{
		return m_edit_link;
	}

	void set_edit_link(const ::utility::string_t& edit_link)
	{
		m_edit_link = edit_link;
	}

	::utility::string_t get_key_property_string(bool with_key_name = false)
	{
		return U("");
	}

protected:
	::utility::string_t m_edit_link;
	std::shared_ptr<odata_service_context> m_service_context;
};


typedef void (type_base::*PROP_MAP_CALL_IN_ENTITY)(const std::shared_ptr<::odata::core::odata_entity_value>&);
typedef void (type_base::*PROP_MAP_CALL_IN_COMPLEX)(const std::shared_ptr<::odata::core::odata_complex_value>&);

typedef std::shared_ptr<type_base> (*CREATE_CALL_FOR_ENTITY)(const std::shared_ptr<::odata::codegen::odata_service_context>&);
typedef std::unordered_map<::utility::string_t, CREATE_CALL_FOR_ENTITY> create_map_for_entity_type; 

typedef std::shared_ptr<type_base> (*CREATE_CALL_FOR_COMPLEX)(const std::shared_ptr<::odata::codegen::odata_service_context>&);
typedef std::unordered_map<::utility::string_t, CREATE_CALL_FOR_COMPLEX> create_map_for_complex_type; 

struct PROPMAP_ENTRY_IN_ENTITY 
{  
    PROP_MAP_CALL_IN_ENTITY pfn;
};  

struct PROPMAP_ENTRY_IN_COMPLEX
{  
    PROP_MAP_CALL_IN_COMPLEX pfn;
}; 

}}