#pragma once

#include "odata/edm/odata_edm.h"
#include "odata/common/xmlhelpers.h"
#include "odata/communication/http_communication.h"

namespace odata { namespace edm
{

class edm_model_reader : public ::odata::edm::xml_reader
{
public:
    edm_model_reader(concurrency::streams::istream stream) : 
		xml_reader(stream), m_parsing_key(false), m_model(std::make_shared<edm_model>()), 
		m_current_st(nullptr), m_current_enum(nullptr), m_current_operation(nullptr)
    {
    }

	/*edm_model_reader(const char* content, unsigned int size) :
		xml_reader(content, size), m_parsing_key(false), m_model(std::make_shared<edm_model>()), 
		m_current_st(nullptr), m_current_enum(nullptr), m_current_operation(nullptr)
	{

	}*/

    std::shared_ptr<edm_model> get_model()
    {
        return m_model;
    }

	ODATACPP_API bool parse();

protected:
    ODATACPP_API virtual void handle_begin_element(const ::utility::string_t& elementName);
    ODATACPP_API virtual void handle_end_element(const ::utility::string_t& elementName);
    ODATACPP_API virtual void handle_element(const ::utility::string_t& elementName);


private:
    void _process_property();
	void _process_navigation_property();
	void _process_operation_parameter();
	void _process_operation_return_type();
	void _process_navigation_property_binding();

    bool m_parsing_key;

    // These variables are used to cache values for each entity
    std::shared_ptr<edm_model> m_model;
    std::shared_ptr<edm_schema> m_current_schema;
    std::shared_ptr<edm_entity_container> m_current_container;
	std::shared_ptr<edm_entity_set> m_current_entity_set;
	std::shared_ptr<edm_singleton> m_current_singleton;
    edm_structured_type* m_current_st;
	edm_enum_type* m_current_enum;
	edm_operation_type* m_current_operation;
};

}}