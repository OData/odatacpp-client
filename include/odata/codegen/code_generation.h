//---------------------------------------------------------------------
// <copyright file="code_generation.h" company="Microsoft">
//      Copyright (C) Microsoft Corporation. All rights reserved. See License.txt in the project root for license information.
// </copyright>
//---------------------------------------------------------------------

#pragma once

#include "odata/common/utility.h"
#include "odata/codegen/code_generation_base.h"
#include "odata/codegen/code_generation_helper.h"
#include "odata/codegen/odata_entityset_query_executor.h"
#include "odata/codegen/odata_singleton_query_executor.h"
#include "odata/codegen/odata_service_context.h"
#include "odata/core/odata_core.h"
#include "odata/core/odata_json_writer.h"

namespace odata { namespace codegen { 

#define DECLARE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(property_name, edm_name, type) \
public: \
    const type& get_##property_name() { return property_name; } const \
	void set_##property_name(const type& property_value) { property_name = property_value; } \
protected: \
	type property_name;  \
    void get_##property_name##_from_entity(const shared_ptr<::odata::core::odata_entity_value>& pentity); \
	void set_##property_name##_to_entity(const shared_ptr<::odata::core::odata_entity_value>& pentity);

#define IMPLEMENT_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(myclass, property_name, edm_name, type) \
    void myclass::get_##property_name##_from_entity(const shared_ptr<::odata::core::odata_entity_value>& pentity) \
	{ \
        std::shared_ptr<::odata::core::odata_value> property_value; \
		if (!pentity->get_property_value(U(#edm_name), property_value) || !property_value) \
	    { \
			return ; \
		} \
		auto primitive_value = std::dynamic_pointer_cast<odata_primitive_value>(property_value); \
	    if (primitive_value) \
	    { \
            property_name = primitive_value->as<type>(); \
        } \
    } \
	void myclass::set_##property_name##_to_entity(const shared_ptr<::odata::core::odata_entity_value>& pentity) \
	{ \
	    if (!pentity) \
	    { \
		    return ; \
        } \
		pentity->set_value(U(#edm_name), property_name); \
	}

// because we have to support derived complex type, so complex object in entity are all set with pointer type
#define DECLARE_COMPLEX_PROPERTY_IN_ENTITY_MAPPING(property_name, edm_name, type) \
    DECLARE_NULLABLE_COMPLEX_PROPERTY_IN_ENTITY_MAPPING(property_name, edm_name, type)

#define IMPLEMENT_COMPLEX_PROPERTY_IN_ENTITY_MAPPING(myclass, property_name, edm_name, type) \
	IMPLEMENT_NULLABLE_COMPLEX_PROPERTY_IN_ENTITY_MAPPING(myclass, property_name, edm_name, type)

#define DECLARE_PRIMITIVE_PROPERTY_IN_COMPLEX_MAPPING(property_name, edm_name, type) \
public : \
	const type& get_##property_name() { return property_name; } const \
	void set_##property_name(const type& property_value) { property_name = property_value; } \
protected: \
    type property_name;  \
    void get_##property_name##_from_complex(const shared_ptr<::odata::core::odata_complex_value>& pcomplex); \
	void set_##property_name##_to_complex(const shared_ptr<::odata::core::odata_complex_value>& pcomplex);

#define IMPLEMENT_PRIMITIVE_PROPERTY_IN_COMPLEX_MAPPING(myclass, property_name, edm_name, type) \
    void myclass::get_##property_name##_from_complex(const shared_ptr<::odata::core::odata_complex_value>& pcomplex) \
	{ \
	    std::shared_ptr<::odata::core::odata_value> property_value; \
        if (!pcomplex->get_property_value(U(#edm_name), property_value) || !property_value) \
	    { \
			return ; \
		} \
		auto primitive_value = std::dynamic_pointer_cast<odata_primitive_value>(property_value); \
	    if (primitive_value) \
	    { \
            property_name = primitive_value->as<type>(); \
        } \
    }\
	void myclass::set_##property_name##_to_complex(const shared_ptr<::odata::core::odata_complex_value>& pcomplex) \
	{ \
	    if (!pcomplex) \
	    { \
		    return ; \
        } \
		pcomplex->set_value(U(#edm_name), property_name); \
	}

#define IMPLEMENT_EDM_INFO(myclass, name_space, type_name) \
	::utility::string_t myclass::m_namespace = U(#name_space); \
	::utility::string_t myclass::m_typename = U(#type_name); \
	::utility::string_t myclass::get_full_name() \
    { \
        return myclass::m_namespace + U(".") + myclass::m_typename; \
	} \
    ::utility::string_t myclass::get_type_name() \
	{ \
		return m_typename; \
	}

#define DECLARE_ENTITY_CONSTRUCTOR(myclass) \
	public: \
	myclass(const std::shared_ptr<odata_service_context>& service_context); \
	::utility::string_t get_root_url() { return m_service_context ? m_service_context->get_root_url() : U(""); } \
    static create_map_for_entity_type::value_type _init_creator_map_values[]; \
	static std::unordered_map<::utility::string_t, CREATE_CALL_FOR_ENTITY> _derived_entity_creator_map; 

#define ENABLE_PROPERTY_IN_ENTITY_MAPPING() \
public: \
    void from_value(const shared_ptr<::odata::core::odata_entity_value>& pentity); \
	virtual std::shared_ptr<::odata::core::odata_entity_value> to_value(); \
	PROPMAP_ENTRY_IN_ENTITY* get_to_entity_map(); \
	void set_to_entity_map(PROPMAP_ENTRY_IN_ENTITY* _map); \
protected: \
    static PROPMAP_ENTRY_IN_ENTITY _property_entrties[]; \
    PROPMAP_ENTRY_IN_ENTITY* get_mapping_map(); \
	static PROPMAP_ENTRY_IN_ENTITY _map_class_to_entity_entrties[]; \
	PROPMAP_ENTRY_IN_ENTITY* m_to_entity_map; \
	void map_entity_value_to_class_member(const shared_ptr<::odata::core::odata_entity_value>& pentity); \
	void map_class_member_to_entity_value(const std::shared_ptr<::odata::core::odata_entity_value>& entity_value); 

#define ENABLE_PROPERTY_IN_COMPLEX_MAPPING() \
public: \
	void from_value(const shared_ptr<::odata::core::odata_complex_value>& pcomplex); \
	virtual std::shared_ptr<::odata::core::odata_complex_value> to_value(); \
	PROPMAP_ENTRY_IN_COMPLEX* get_to_complex_map(); \
	void set_to_complex_map(PROPMAP_ENTRY_IN_COMPLEX* _map); \
protected: \
    static PROPMAP_ENTRY_IN_COMPLEX _property_entrties[]; \
    PROPMAP_ENTRY_IN_COMPLEX* get_mapping_map(); \
	static PROPMAP_ENTRY_IN_COMPLEX _map_class_to_complex_entrties[]; \
	PROPMAP_ENTRY_IN_COMPLEX* m_to_complex_map; \
	void map_complex_value_to_class_member(const shared_ptr<::odata::core::odata_complex_value>& pcomplex); \
	void map_class_member_to_complex_value(const std::shared_ptr<::odata::core::odata_complex_value>& complex_value); 


#define DECLARE_EDM_INFO() \
    protected: \
	static ::utility::string_t m_namespace; \
	static ::utility::string_t m_typename; \
	public: \
	static ::utility::string_t get_full_name(); \
	static ::utility::string_t get_type_name(); \

#define BEGIN_PROPERTY_IN_ENTITY_MAPPING(myclass) \
    PROPMAP_ENTRY_IN_ENTITY* myclass::get_mapping_map()  \
    { \
        return _property_entrties;  \
    } \
	PROPMAP_ENTRY_IN_ENTITY* myclass::get_to_entity_map() \
	{ \
	    if (!m_to_entity_map) \
	        m_to_entity_map = _map_class_to_entity_entrties; \
	    return m_to_entity_map;  \
	} \
	void myclass::set_to_entity_map(PROPMAP_ENTRY_IN_ENTITY* _map) \
	{ \
	    m_to_entity_map = _map; \
	} \
    PROPMAP_ENTRY_IN_ENTITY myclass::_property_entrties[] =   \
    {

#define BEGIN_PROPERTY_IN_ENTITY_MAPPING_WITH_BASE_CLASS(myclass, parentclass) \
    BEGIN_PROPERTY_IN_ENTITY_MAPPING(myclass)

#define END_PROPERTY_IN_ENTITY_MAPPING(myclass) \
        {(PROP_MAP_CALL_IN_ENTITY)0}   \
    };  \
	std::shared_ptr<::odata::core::odata_entity_value> myclass::to_value() \
	{ \
	    if (!m_service_context || !m_service_context->get_edm_model()) { return nullptr; } \
	    auto entity_type = m_service_context->get_edm_model()->find_entity_type(m_typename); \
		auto entity_value = std::make_shared<::odata::core::odata_entity_value>(entity_type); \
		map_class_member_to_entity_value(entity_value); \
		if (!m_namespace.empty() && !m_typename.empty()) \
        { \
		    entity_value->set_value(::odata::core::odata_json_constants::PAYLOAD_ANNOTATION_TYPE, std::make_shared<odata_primitive_value>(std::make_shared<::odata::edm::edm_payload_annotation_type>(::odata::core::odata_json_constants::PAYLOAD_ANNOTATION_TYPE), U("#") + m_namespace + U(".") + m_typename)); \
	    } \
		return entity_value; \
	} \
    void myclass::from_value(const shared_ptr<::odata::core::odata_entity_value>& pentity) \
	{ \
	    map_entity_value_to_class_member(pentity); \
	}

#define END_PROPERTY_IN_ENTITY_MAPPING_WITH_BASE_CLASS(myclass, parentclass) \
        {(PROP_MAP_CALL_IN_ENTITY)0}   \
    };  \
	std::shared_ptr<::odata::core::odata_entity_value> myclass::to_value() \
	{ \
	    if (!m_service_context || !m_service_context->get_edm_model()) { return nullptr; } \
		auto entity_value = parentclass::to_value(); \
	    auto entity_type = m_service_context->get_edm_model()->find_entity_type(m_typename); \
		entity_value->set_value_type(entity_type); \
		map_class_member_to_entity_value(entity_value); \
		if (!m_namespace.empty() && !m_typename.empty()) \
        { \
entity_value->set_value(::odata::core::odata_json_constants::PAYLOAD_ANNOTATION_TYPE, std::make_shared<odata_primitive_value>(std::make_shared<::odata::edm::edm_payload_annotation_type>(::odata::core::odata_json_constants::PAYLOAD_ANNOTATION_TYPE), U("#") + m_namespace + U(".") + m_typename)); \
	    } \
		return entity_value; \
	} \
    void myclass::from_value(const shared_ptr<::odata::core::odata_entity_value>& pentity) \
	{ \
	    parentclass::from_value(pentity); \
	    map_entity_value_to_class_member(pentity); \
	}

#define ON_PROPERTY_IN_ENTITY_MAPPING(myclass, property_name) \
	{(PROP_MAP_CALL_IN_ENTITY)&myclass::get_##property_name##_from_entity},   

#define BEGIN_PROPERTY_IN_COMPLEX_MAPPING(myclass) \
    PROPMAP_ENTRY_IN_COMPLEX* myclass::get_mapping_map()  \
    { \
        return _property_entrties;  \
    } \
	PROPMAP_ENTRY_IN_COMPLEX* myclass::get_to_complex_map() \
	{ \
	    if (!m_to_complex_map) \
	        m_to_complex_map = _map_class_to_complex_entrties; \
	    return m_to_complex_map;  \
	} \
	void myclass::set_to_complex_map(PROPMAP_ENTRY_IN_COMPLEX* _map) \
	{ \
	    m_to_complex_map = _map; \
	} \
    PROPMAP_ENTRY_IN_COMPLEX myclass::_property_entrties[] =   \
    {   

#define BEGIN_PROPERTY_IN_COMPLEX_MAPPING_WITH_BASE_CLASS(myclass, parentclass) \
	BEGIN_PROPERTY_IN_COMPLEX_MAPPING(myclass)

#define END_PROPERTY_IN_COMPLEX_MAPPING(myclass) \
        {(PROP_MAP_CALL_IN_COMPLEX)0 }   \
    };  \
	std::shared_ptr<::odata::core::odata_complex_value> myclass::to_value() \
	{ \
	    if (!m_service_context || !m_service_context->get_edm_model()) { return nullptr; } \
	    auto complex_type = m_service_context->get_edm_model()->find_complex_type(m_typename); \
		auto complex_value = std::make_shared<::odata::core::odata_complex_value>(complex_type); \
		map_class_member_to_complex_value(complex_value); \
		if (!m_namespace.empty() && !m_typename.empty()) \
        { \
		    complex_value->set_value(::odata::core::odata_json_constants::PAYLOAD_ANNOTATION_TYPE, std::make_shared<odata_primitive_value>(std::make_shared<::odata::edm::edm_payload_annotation_type>(::odata::core::odata_json_constants::PAYLOAD_ANNOTATION_TYPE), U("#") + m_namespace + U(".") + m_typename)); \
	    } \
		return complex_value; \
	} \
	void myclass::from_value(const shared_ptr<::odata::core::odata_complex_value>& pcomplex) \
	{ \
	    map_complex_value_to_class_member(pcomplex); \
	}

#define END_PROPERTY_IN_COMPLEX_MAPPING_WITH_BASE_CLASS(myclass, parentclass) \
        {(PROP_MAP_CALL_IN_COMPLEX)0 }   \
    };  \
	std::shared_ptr<::odata::core::odata_complex_value> myclass::to_value() \
	{ \
	    if (!m_service_context || !m_service_context->get_edm_model()) { return nullptr; } \
	    auto complex_type = m_service_context->get_edm_model()->find_complex_type(m_typename); \
		auto complex_value = parentclass::to_value(); \
		complex_value->set_value_type(complex_type); \
		map_class_member_to_complex_value(complex_value); \
		if (!m_namespace.empty() && !m_typename.empty()) \
        { \
		    complex_value->set_value(::odata::core::odata_json_constants::PAYLOAD_ANNOTATION_TYPE, std::make_shared<odata_primitive_value>(std::make_shared<::odata::edm::edm_payload_annotation_type>(::odata::core::odata_json_constants::PAYLOAD_ANNOTATION_TYPE), U("#") + m_namespace + U(".") + m_typename)); \
	    } \
		return complex_value; \
	} \
	void myclass::from_value(const shared_ptr<::odata::core::odata_complex_value>& pcomplex) \
	{ \
	    parentclass::from_value(pcomplex); \
	    map_complex_value_to_class_member(pcomplex); \
	}

#define ON_PROPERTY_IN_COMPLEX_MAPPING(myclass, property_name) \
	{(PROP_MAP_CALL_IN_COMPLEX)&myclass::get_##property_name##_from_complex}, \

#define BEGIN_ENTITY_CONSTRUCTOR(myclass, baseclass) \
	myclass::myclass(const std::shared_ptr<odata_service_context>& service_context) \
	: baseclass(service_context), m_to_entity_map(nullptr)

#define ON_PROPERTY_IN_ENTITY_CONSTRUCTOR(property_name, property_value) \
	    , property_name(property_value)

#define END_ENTITY_CONSTRUCTOR(myclass, baseclass) \
	{ } 

#define DECLARE_COMPLEX_CONSTRUCTOR(myclass) \
	public: \
	myclass(const std::shared_ptr<odata_service_context>& service_context); \
    static create_map_for_complex_type::value_type _init_creator_map_values[]; \
	static std::unordered_map<::utility::string_t, CREATE_CALL_FOR_COMPLEX> _derived_complex_creator_map; 

#define BEGIN_COMPLEX_CONSTRUCTOR(myclass, baseclass) \
	myclass::myclass(const std::shared_ptr<odata_service_context>& service_context) \
	    : baseclass(service_context), m_to_complex_map(nullptr)

#define ON_PROPERTY_IN_COMPLEX_CONSTRUCTOR(property_name, property_value) \
	    , property_name(property_value)

#define END_COMPLEX_CONSTRUCTOR(myclass, baseclass) \
    { } 

#define DECLARE_COMPLEX_DESTRUCTOR(myclass) \
	public: \
	~myclass();

#define BEGIN_COMPLEX_DESTRUCTOR(myclass) \
	myclass::~myclass() \
	{ 

#define ON_PROPERTY_IN_COMPLEX_DESTRUCTOR(property_name) 

#define END_COMPLEX_DESTRUCTOR(myclass) \
	}

#define DECLARE_ENTITY_DESTRUCTOR(myclass) \
	public: \
	~myclass();

#define BEGIN_ENTITY_DESTRUCTOR(myclass) \
	myclass::~myclass() \
	{ 

#define ON_PROPERTY_IN_ENTITY_DESTRUCTOR(property_name)

#define END_ENTITY_DESTRUCTOR(myclass) \
	}

#define BEGIN_SERIALIZE_PROPERTY_IN_COMPLEX_MAPPING(myclass) \
	PROPMAP_ENTRY_IN_COMPLEX myclass::_map_class_to_complex_entrties[] =   \
    { 

#define ON_SERIALIZE_PROPERTY_IN_COMPLEX_MAPPING(myclass, property_name) \
	    {(PROP_MAP_CALL_IN_COMPLEX)&myclass::set_##property_name##_to_complex},

#define END_SERIALIZE_PROPERTY_IN_COMPLEX_MAPPING(myclass) \
	    {(PROP_MAP_CALL_IN_COMPLEX)0} \
	}; \
	void myclass::map_complex_value_to_class_member(const shared_ptr<::odata::core::odata_complex_value>& pentity) \
	{ \
		for (int i = 0;; i++) \
		{ \
			PROPMAP_ENTRY_IN_COMPLEX pentry = get_mapping_map()[i]; \
			if (pentry.pfn == 0) \
			{ \
				break; \
			} \
			(this->*((PROP_MAP_CALL_IN_COMPLEX)pentry.pfn))(pentity); \
		} \
	} \
	void myclass::map_class_member_to_complex_value(const std::shared_ptr<::odata::core::odata_complex_value>& complex_value) \
	{ \
		for (int i = 0;; i++) \
		{ \
			PROPMAP_ENTRY_IN_COMPLEX pentry = get_to_complex_map()[i]; \
			if (pentry.pfn == 0) \
			{ \
				break; \
			} \
			(this->*((PROP_MAP_CALL_IN_COMPLEX)pentry.pfn))(complex_value); \
		} \
	}

#define BEGIN_SERIALIZE_PROPERTY_IN_ENTITY_MAPPING(myclass) \
	PROPMAP_ENTRY_IN_ENTITY myclass::_map_class_to_entity_entrties[] =   \
    { 

#define ON_SERIALIZE_PROPERTY_IN_ENTITY_MAPPING(myclass, property_name) \
	    {(PROP_MAP_CALL_IN_ENTITY)&myclass::set_##property_name##_to_entity},

#define END_SERIALIZE_PROPERTY_IN_ENTITY_MAPPING(myclass) \
	    {(PROP_MAP_CALL_IN_ENTITY)0} \
	}; \
	void myclass::map_entity_value_to_class_member(const shared_ptr<::odata::core::odata_entity_value>& pentity) \
	{ \
	    if (m_edit_link.empty()) \
	    { \
		    m_edit_link = odata_entity_model_builder::compute_edit_link(get_root_url(), pentity, U(""), false); \
		} \
		for (int i = 0;; i++) \
		{ \
			PROPMAP_ENTRY_IN_ENTITY pentry = get_mapping_map()[i]; \
			if (pentry.pfn == 0) \
			{ \
				break; \
			} \
			(this->*((PROP_MAP_CALL_IN_ENTITY)pentry.pfn))(pentity); \
		} \
	} \
	void myclass::map_class_member_to_entity_value(const std::shared_ptr<::odata::core::odata_entity_value>& pentity) \
	{ \
		for (int i = 0;; i++) \
		{ \
			PROPMAP_ENTRY_IN_ENTITY pentry = get_to_entity_map()[i]; \
			if (pentry.pfn == 0) \
			{ \
				break; \
			} \
			(this->*((PROP_MAP_CALL_IN_ENTITY)pentry.pfn))(pentity); \
		} \
	}

#define DECLARE_NAVIGATION_PROPERTY_IN_ENTITY_MAPPING(navigation_property_name, edm_name, type) \
public: \
	const std::shared_ptr<type>& get_##navigation_property_name() { return navigation_property_name; } const \
	void set_##navigation_property_name(const std::shared_ptr<type>& navigation_value) { navigation_property_name = navigation_value; } \
	::pplx::task<void> load_##navigation_property_name(const std::shared_ptr<odata_query_builder>& builder = nullptr); \
protected: \
	std::shared_ptr<type> navigation_property_name;  \
	void get_##navigation_property_name##_from_entity(const shared_ptr<::odata::core::odata_entity_value>& pentity); 

#define IMPLEMENT_NAVIGATION_PROPERTY_IN_ENTITY_MAPPING(myclass, navigation_property_name, edm_name, type) \
	void myclass::get_##navigation_property_name##_from_entity(const shared_ptr<::odata::core::odata_entity_value>& pentity) \
	{ \
        std::shared_ptr<::odata::core::odata_value> property_value; \
		if (!pentity->get_property_value(U(#edm_name), property_value) || !property_value) \
	    { \
			return ; \
		} \
	    if (property_value->get_value_type()->get_type_kind() == ::odata::edm::edm_type_kind_t::Entity) \
	    { \
			auto entity_value = std::dynamic_pointer_cast<::odata::core::odata_entity_value>(property_value); \
			navigation_property_name = create_instance_from_entity<type>(entity_value, m_service_context); \
        } \
    } \
	::pplx::task<void> myclass::load_##navigation_property_name(const std::shared_ptr<odata_query_builder>& builder) \
	{ \
	    if (m_service_context) \
		{\
		    ::utility::string_t path = m_service_context->get_relative_path(m_edit_link) + U("/") + U(#edm_name); \
			auto query = m_service_context->create_query<odata_singleton_query_executor<type>, odata_query_builder>(path); \
			if (builder) { query->set_query_builder(builder); } \
			return query->execute_query().then( \
				[this] (const std::shared_ptr<type>& ret_value) -> void \
			    { \
				    navigation_property_name = ret_value; \
			    }); \
		}\
		return ::pplx::task_from_result(); \
	} 

#define DECLARE_COLLECTION_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(property_name, edm_name, type) \
public: \
	const std::vector<type>& get_##property_name() { return property_name; } const \
	void set_##property_name(std::vector<type> values) { property_name = std::move(values); } \
	void add_to_##property_name(const type& property_value) { property_name.push_back(property_value); } \
protected: \
	std::vector<type> property_name; \
	void get_##property_name##_from_entity(const shared_ptr<::odata::core::odata_entity_value>& pentity); \
	void set_##property_name##_to_entity(const shared_ptr<::odata::core::odata_entity_value>& pentity);

#define IMPLEMENT_COLLECTION_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(myclass, property_name, edm_name, type) \
	void myclass::get_##property_name##_from_entity(const shared_ptr<::odata::core::odata_entity_value>& pentity) \
	{ \
		std::shared_ptr<::odata::core::odata_value> property_value; \
		if (!pentity->get_property_value(U(#edm_name), property_value) || !property_value) \
		{ \
			return ; \
		} \
		auto property_collection_value = static_pointer_cast<::odata::core::odata_collection_value>(property_value); \
		if (!property_collection_value) \
		{ \
			return ; \
		} \
		for (auto iter = property_collection_value->get_collection_values().cbegin(); iter != property_collection_value->get_collection_values().cend(); iter++) \
		{ \
		    auto primitive_value = std::dynamic_pointer_cast<odata_primitive_value>(*iter); \
			if (!primitive_value) \
	        { \
			    continue ; \
			} \
			property_name.push_back(primitive_value->as<type>()); \
		} \
	} \
	void myclass::set_##property_name##_to_entity(const shared_ptr<::odata::core::odata_entity_value>& pentity) \
	{ \
		if (!pentity || !pentity->get_value_type()) \
		{ \
			return ; \
		} \
	    auto entity_type = std::dynamic_pointer_cast<::odata::edm::edm_entity_type>(pentity->get_value_type()); \
		if (!entity_type) \
		{ \
			return ; \
		} \
		auto edm_property = entity_type->find_property(U(#edm_name)); \
		if (!edm_property) \
		{ \
			return ; \
		} \
		auto property_type = edm_property->get_property_type(); \
		auto collection_value_type = std::dynamic_pointer_cast<::odata::edm::edm_collection_type>(property_type); \
		if (!collection_value_type) \
		{ \
			return ; \
		} \
		std::shared_ptr<::odata::core::odata_collection_value> collection_value = std::make_shared<::odata::core::odata_collection_value>(collection_value_type); \
		for (size_t i = 0; i < property_name.size(); i++) \
		{ \
			collection_value->add_collection_value(odata_primitive_value::make_primitive_value(property_name[i])); \
		} \
		pentity->set_value(U(#edm_name), collection_value); \
	}

#define DECLARE_COLLECTION_PRIMITIVE_PROPERTY_IN_COMPLEX_MAPPING(property_name, edm_name, type) \
public: \
	const std::vector<type>& get_##property_name() { return property_name; } const \
	void set_##property_name(const std::vector<type>& values) { property_name = values; } \
	void add_to_##property_name(const type& property_value) { property_name.push_back(property_value); } \
protected: \
	std::vector<type> property_name; \
	void get_##property_name##_from_complex(const shared_ptr<::odata::core::odata_complex_value>& pcomplex); \
	void set_##property_name##_to_complex(const shared_ptr<::odata::core::odata_complex_value>& pcomplex); 

#define IMPLEMENT_COLLECTION_PRIMITIVE_PROPERTY_IN_COMPLEX_MAPPING(myclass, property_name, edm_name, type) \
	void myclass::get_##property_name##_from_complex(const shared_ptr<::odata::core::odata_complex_value>& pcomplex) \
	{ \
		std::shared_ptr<::odata::core::odata_value> property_value; \
		if (!pcomplex->get_property_value(U(#edm_name), property_value) || !property_value) \
		{ \
			return ; \
		} \
		auto property_collection_value = static_pointer_cast<::odata::core::odata_collection_value>(property_value); \
		if (!property_collection_value) \
		{ \
			return ; \
		} \
		for (auto iter = property_collection_value->get_collection_values().cbegin(); iter != property_collection_value->get_collection_values().cend(); iter++) \
		{ \
		    auto primitive_value = std::dynamic_pointer_cast<odata_primitive_value>(*iter); \
			if (!primitive_value) \
	        { \
			    continue ; \
			} \
			property_name.push_back(primitive_value->as<type>()); \
		} \
	} \
	void myclass::set_##property_name##_to_complex(const shared_ptr<::odata::core::odata_complex_value>& pcomplex) \
	{ \
		if (!pcomplex || !pcomplex->get_value_type()) \
		{ \
			return ; \
		} \
	    auto complex_type = std::dynamic_pointer_cast<::odata::edm::edm_complex_type>(pcomplex->get_value_type()); \
		if (!complex_type) \
		{ \
			return ; \
		} \
		auto edm_property = complex_type->find_property(U(#edm_name)); \
		if (!edm_property) \
		{ \
			return ; \
		} \
		auto property_type = edm_property->get_property_type(); \
		auto collection_value_type = std::dynamic_pointer_cast<::odata::edm::edm_collection_type>(property_type); \
		if (!collection_value_type) \
		{ \
			return ; \
		} \
		std::shared_ptr<::odata::core::odata_collection_value> collection_value = std::make_shared<::odata::core::odata_collection_value>(collection_value_type); \
		for (size_t i = 0; i < property_name.size(); i++) \
		{ \
			collection_value->add_collection_value(odata_primitive_value::make_primitive_value(property_name[i])); \
		} \
		pcomplex->set_value(U(#edm_name), collection_value); \
	}

#define DECLARE_NULLABLE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(property_name, edm_name, type) \
public: \
	type* get_##property_name() { return property_name.get(); } const \
	void set_##property_name(type property_value) \
	{\
		property_name = std::make_shared<type>(property_value); \
	} \
protected: \
    std::shared_ptr<type> property_name; \
	void get_##property_name##_from_entity(const shared_ptr<::odata::core::odata_entity_value>& pentity); \
	void set_##property_name##_to_entity(const shared_ptr<::odata::core::odata_entity_value>& pentity);

#define IMPLEMENT_NULLABLE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(myclass, property_name, edm_name, type) \
	void myclass::get_##property_name##_from_entity(const shared_ptr<::odata::core::odata_entity_value>& pentity) \
	{ \
		std::shared_ptr<::odata::core::odata_value> property_value; \
		if (!pentity->get_property_value(U(#edm_name), property_value) || !property_value) \
		{ \
			return ; \
		} \
		auto primitive_value = std::dynamic_pointer_cast<odata_primitive_value>(property_value); \
		if (primitive_value) \
		{ \
			property_name = std::make_shared<type>(primitive_value->as<type>()); \
		} \
	} \
	void myclass::set_##property_name##_to_entity(const shared_ptr<::odata::core::odata_entity_value>& pentity) \
	{ \
	    if (!pentity || !property_name) \
	    { \
		    return ; \
        } \
		pentity->set_value(U(#edm_name), *property_name); \
	}

#define DECLARE_NULLABLE_PRIMITIVE_PROPERTY_IN_COMPLEX_MAPPING(property_name, edm_name, type) \
public: \
	type* get_##property_name() { return property_name.get(); } const \
	void set_##property_name(type property_value)  \
    { \
		property_name = std::make_shared<type>(property_value);  \
	} \
protected: \
    std::shared_ptr<type> property_name; \
	void get_##property_name##_from_complex(const shared_ptr<::odata::core::odata_complex_value>& pcomplex); \
	void set_##property_name##_to_complex(const shared_ptr<::odata::core::odata_complex_value>& pcomplex); 

#define IMPLEMENT_NULLABLE_PRIMITIVE_PROPERTY_IN_COMPLEX_MAPPING(myclass, property_name, edm_name, type) \
	void myclass::get_##property_name##_from_complex(const shared_ptr<::odata::core::odata_complex_value>& pcomplex) \
	{ \
		std::shared_ptr<::odata::core::odata_value> property_value; \
		if (!pcomplex->get_property_value(U(#edm_name), property_value) || !property_value) \
		{ \
			return ; \
		} \
		auto primitive_value = std::dynamic_pointer_cast<odata_primitive_value>(property_value); \
		if (primitive_value) \
		{ \
			property_name = std::make_shared<type>(primitive_value->as<type>()); \
		} \
	} \
	void myclass::set_##property_name##_to_complex(const shared_ptr<::odata::core::odata_complex_value>& pcomplex) \
	{ \
	    if (!pcomplex || !property_name) \
	    { \
		    return ; \
        } \
		pcomplex->set_value(U(#edm_name), *property_name); \
	}

#define DECLARE_NULLABLE_COMPLEX_PROPERTY_IN_ENTITY_MAPPING(property_name, edm_name, type) \
public: \
	type* get_##property_name() { return property_name.get(); } const \
	void set_##property_name(const std::shared_ptr<type>& property_value) { property_name = property_value; } \
protected: \
    std::shared_ptr<type> property_name; \
	void get_##property_name##_from_entity(const shared_ptr<::odata::core::odata_entity_value>& pentity); \
	void set_##property_name##_to_entity(const shared_ptr<::odata::core::odata_entity_value>& pentity);

#define IMPLEMENT_NULLABLE_COMPLEX_PROPERTY_IN_ENTITY_MAPPING(myclass, property_name, edm_name, type) \
	void myclass::get_##property_name##_from_entity(const shared_ptr<::odata::core::odata_entity_value>& pentity) \
	{ \
		std::shared_ptr<::odata::core::odata_value> property_value; \
		if (!pentity->get_property_value(U(#edm_name), property_value) || !property_value) \
		{ \
			return ; \
		} \
		if (property_value->get_value_type()->get_type_kind() == ::odata::edm::edm_type_kind_t::Complex) \
		{ \
			property_name = create_instance_from_complex<type>(std::dynamic_pointer_cast<::odata::core::odata_complex_value>(property_value), m_service_context); \
		} \
	} \
	void myclass::set_##property_name##_to_entity(const shared_ptr<::odata::core::odata_entity_value>& pentity) \
	{ \
	    if (!pentity || !property_name) \
	    { \
		    return ; \
        } \
		pentity->set_value(U(#edm_name), property_name->to_value()); \
	}

#define DECLARE_NULLABLE_COMPLEX_PROPERTY_IN_COMPLEX_MAPPING(property_name, edm_name, type) \
public: \
	type* get_##property_name() { return property_name.get(); } const \
	void set_##property_name(const std::shared_ptr<type>& property_value) { property_name = property_value; } \
protected: \
    std::shared_ptr<type> property_name; \
	void get_##property_name##_from_complex(const shared_ptr<::odata::core::odata_complex_value>& pcomplex); \
	void set_##property_name##_to_complex(const shared_ptr<::odata::core::odata_complex_value>& pcomplex);

#define IMPLEMENT_NULLABLE_COMPLEX_PROPERTY_IN_COMPLEX_MAPPING(myclass, property_name, edm_name, type) \
	void myclass::get_##property_name##_from_complex(const shared_ptr<::odata::core::odata_complex_value>& pcomplex) \
	{ \
		std::shared_ptr<::odata::core::odata_value> property_value; \
		if (!pcomplex->get_property_value(U(#edm_name), property_value) || !property_value) \
		{ \
			return ; \
		} \
		if (property_value->get_value_type()->get_type_kind() == ::odata::edm::edm_type_kind_t::Complex) \
		{ \
			property_name = create_instance_from_complex<type>(std::dynamic_pointer_cast<::odata::core::odata_complex_value>(property_value), m_service_context); \
		} \
	} \
	void myclass::set_##property_name##_to_complex(const shared_ptr<::odata::core::odata_complex_value>& pcomplex) \
	{ \
	    if (!pcomplex || !property_name) \
	    { \
		    return ; \
        } \
		pcomplex->set_value(U(#edm_name), property_name->to_value()); \
	}

#define DECLARE_COMPLEX_PROPERTY_IN_COMPLEX_MAPPING(property_name, edm_name, type) \
	DECLARE_NULLABLE_COMPLEX_PROPERTY_IN_COMPLEX_MAPPING(property_name, edm_name, type)

#define IMPLEMENT_COMPLEX_PROPERTY_IN_COMPLEX_MAPPING(myclass, property_name, edm_name, type) \
	IMPLEMENT_NULLABLE_COMPLEX_PROPERTY_IN_COMPLEX_MAPPING(myclass, property_name, edm_name, type)

#define DECLARE_COLLECTION_NAVIGATION_PROPERTY_IN_ENTITY_MAPPING(property_name, edm_name, type) \
public: \
	const std::vector<std::shared_ptr<type>>& get_##property_name() { return property_name; } const \
	void set_##property_name(const std::vector<std::shared_ptr<type>>& property_values) { property_name = property_values; } \
	void add_to_##property_name(const std::shared_ptr<type>& property_value) { property_name.push_back(property_value); } \
	::pplx::task<void> load_##property_name(const std::shared_ptr<odata_query_builder>& builder = nullptr); \
protected: \
	std::vector<std::shared_ptr<type>> property_name; \
	void get_##property_name##_from_entity(const shared_ptr<::odata::core::odata_entity_value>& pentity); 

#define IMPLEMENT_COLLECTION_NAVIGATION_PROPERTY_IN_ENTITY_MAPPING(myclass, property_name, edm_name, type) \
	void myclass::get_##property_name##_from_entity(const shared_ptr<::odata::core::odata_entity_value>& pentity) \
	{ \
		std::shared_ptr<::odata::core::odata_value> property_value;  \
		if (!pentity->get_property_value(U(#edm_name), property_value) || !property_value) \
		{ \
			return ; \
		} \
		auto property_collection_value = static_pointer_cast<::odata::core::odata_collection_value>(property_value); \
		if (!property_collection_value) \
		{ \
			return ; \
		} \
		for (auto iter = property_collection_value->get_collection_values().cbegin(); iter != property_collection_value->get_collection_values().cend(); iter++) \
		{ \
			auto entity_value = std::dynamic_pointer_cast<::odata::core::odata_entity_value>(*iter); \
			if (!entity_value) \
	        { \
			    continue ; \
			} \
			std::shared_ptr<type> p_entity = create_instance_from_entity<type>(entity_value, m_service_context); \
			property_name.push_back(std::dynamic_pointer_cast<type>(p_entity)); \
		} \
	} \
	::pplx::task<void> myclass::load_##property_name(const std::shared_ptr<odata_query_builder>& builder) \
	{ \
	    if (m_service_context)\
		{\
		    ::utility::string_t path = m_service_context->get_relative_path(m_edit_link) + U("/") + U(#edm_name);\
			auto query = m_service_context->create_query<odata_entityset_query_executor<type>, odata_query_builder>(path);\
			if (builder) { query->set_query_builder(builder); } \
			return query->execute_query().then( \
			    [this] (const std::vector<std::shared_ptr<type>>& ret_values) -> void \
			    { \
	                property_name = ret_values; \
		        }); \
		}\
		return ::pplx::task_from_result(); \
	}

#define DECLARE_COLLECTION_COMPLEX_PROPERTY_IN_ENTITY_MAPPING(property_name, edm_name, type) \
public: \
	const std::vector<std::shared_ptr<type>>& get_##property_name() { return property_name; } const\
	void set_##property_name(std::vector<std::shared_ptr<type>> property_values) { property_name = std::move(property_values); } \
	void add_to_##property_name(const std::shared_ptr<type>& property_value) { property_name.push_back(property_value); } \
protected: \
	std::vector<std::shared_ptr<type>> property_name; \
	void get_##property_name##_from_entity(const shared_ptr<::odata::core::odata_entity_value>& pentity); \
	void set_##property_name##_to_entity(const shared_ptr<::odata::core::odata_entity_value>& pentity); 

#define IMPLEMENT_COLLECTION_COMPLEX_PROPERTY_IN_ENTITY_MAPPING(myclass, property_name, edm_name, type) \
	void myclass::get_##property_name##_from_entity(const shared_ptr<::odata::core::odata_entity_value>& pentity) \
	{ \
		std::shared_ptr<::odata::core::odata_value> property_value;  \
		if (!pentity->get_property_value(U(#edm_name), property_value) || !property_value) \
		{ \
			return ; \
		} \
		auto property_collection_value = static_pointer_cast<::odata::core::odata_collection_value>(property_value); \
		if (!property_collection_value) \
		{ \
			return ; \
		} \
		for (auto iter = property_collection_value->get_collection_values().cbegin(); iter != property_collection_value->get_collection_values().cend(); iter++) \
		{ \
			auto complex_value = std::dynamic_pointer_cast<::odata::core::odata_complex_value>(*iter); \
			if (!complex_value) \
	        { \
			    continue ; \
			} \
			property_name.push_back(create_instance_from_complex<type>(complex_value, m_service_context)); \
		} \
	} \
	void myclass::set_##property_name##_to_entity(const shared_ptr<::odata::core::odata_entity_value>& pentity) \
	{ \
		if (!pentity || !pentity->get_value_type()) \
		{ \
			return ; \
		} \
	    auto entity_type = std::dynamic_pointer_cast<::odata::edm::edm_entity_type>(pentity->get_value_type()); \
		if (!entity_type) \
		{ \
			return ; \
		} \
		auto edm_property = entity_type->find_property(U(#edm_name)); \
		if (!edm_property) \
		{ \
			return ; \
		} \
		auto property_type = edm_property->get_property_type(); \
		auto collection_value_type = std::dynamic_pointer_cast<::odata::edm::edm_collection_type>(property_type); \
		if (!collection_value_type) \
		{ \
			return ; \
		} \
		std::shared_ptr<::odata::core::odata_collection_value> collection_value = std::make_shared<::odata::core::odata_collection_value>(collection_value_type); \
		for (size_t i = 0; i < property_name.size(); i++) \
		{ \
		    if (property_name[i]) collection_value->add_collection_value(std::dynamic_pointer_cast<::odata::core::odata_value>(property_name[i]->to_value())); \
		} \
		pentity->set_value(U(#edm_name), collection_value); \
	}

#define DECLARE_ENUM_PROPERTY_IN_ENTITY_MAPPING(property_name, edm_name, type) \
public: \
	const type& get_##property_name() { return property_name; } const \
	void set_##property_name(const type& property_value) { property_name = property_value; } \
protected: \
	type property_name; \
	void get_##property_name##_from_entity(const shared_ptr<::odata::core::odata_entity_value>& pentity); \
	void set_##property_name##_to_entity(const shared_ptr<::odata::core::odata_entity_value>& pentity); 

#define IMPLEMENT_ENUM_PROPERTY_IN_ENTITY_MAPPING(myclass, property_name, edm_name, type) \
	void myclass::get_##property_name##_from_entity(const shared_ptr<::odata::core::odata_entity_value>& pentity) \
	{ \
		if (!pentity) \
		{ \
			return ; \
		} \
        std::shared_ptr<::odata::core::odata_value> property_value; \
		if (!pentity->get_property_value(U(#edm_name), property_value) || !property_value) \
	    { \
			return ; \
		} \
		auto enum_value = std::dynamic_pointer_cast<::odata::core::odata_enum_value>(property_value); \
	    if (enum_value) \
	    { \
		    property_name = enum_type_resolver::get_enum_type_from_string(enum_value->to_string(), type()); \
        } \
	} \
	void myclass::set_##property_name##_to_entity(const shared_ptr<::odata::core::odata_entity_value>& pentity) \
	{ \
		if (!pentity)  \
	    {  \
		    return ;  \
        }  \
		auto entity_type = std::dynamic_pointer_cast<::odata::edm::edm_entity_type>(pentity->get_value_type()); \
		if (!entity_type) \
		{ \
			return ; \
		} \
		auto edm_property = entity_type->find_property(U(#edm_name)); \
		if (!edm_property) \
		{ \
			return ; \
		} \
		auto property_type = edm_property->get_property_type(); \
		auto enum_value = std::make_shared<::odata::core::odata_enum_value>(property_type, enum_type_resolver::get_string_from_enum_type(property_name)); \
		pentity->set_value(U(#edm_name), enum_value);  \
	}

#define DECLARE_ENUM_PROPERTY_IN_COMPLEX_MAPPING(property_name, edm_name, type) \
public: \
	const type& get_##property_name() { return property_name; } const \
	void set_##property_name(const type& property_value) { property_name = property_value; } \
protected: \
	type property_name; \
	void get_##property_name##_from_complex(const shared_ptr<::odata::core::odata_complex_value>& pcomplex); \
	void set_##property_name##_to_complex(const shared_ptr<::odata::core::odata_complex_value>& pcomplex); 

#define IMPLEMENT_ENUM_PROPERTY_IN_COMPLEX_MAPPING(myclass, property_name, edm_name, type) \
	void myclass::get_##property_name##_from_complex(const shared_ptr<::odata::core::odata_complex_value>& pcomplex) \
	{ \
		if (!pcomplex) \
		{ \
			return ; \
		} \
        std::shared_ptr<::odata::core::odata_value> property_value; \
		if (!pcomplex->get_property_value(U(#edm_name), property_value) || !property_value) \
	    { \
			return ; \
		} \
		auto enum_value = std::dynamic_pointer_cast<::odata::core::odata_enum_value>(property_value); \
	    if (enum_value) \
	    { \
		    property_name = enum_type_resolver::get_enum_type_from_string(enum_value->to_string(), type()); \
        } \
	} \
	void myclass::set_##property_name##_to_complex(const shared_ptr<::odata::core::odata_complex_value>& pcomplex) \
	{ \
		if (!pcomplex)  \
	    {  \
		    return ;  \
        }  \
		auto complex_type = std::dynamic_pointer_cast<::odata::edm::edm_complex_type>(pcomplex->get_value_type()); \
		if (!complex_type) \
		{ \
			return ; \
		} \
		auto edm_property = complex_type->find_property(U(#edm_name)); \
		if (!edm_property) \
		{ \
			return ; \
		} \
		auto property_type = edm_property->get_property_type(); \
		auto enum_value = std::make_shared<::odata::core::odata_enum_value>(property_type, enum_type_resolver::get_string_from_enum_type(property_name)); \
		pcomplex->set_value(U(#edm_name), enum_value);  \
	}

#define DECLARE_COLLECTION_ENUM_PROPERTY_IN_ENTITY_MAPPING(property_name, edm_name, type) \
public: \
	const std::vector<type>& get_##property_name() { return property_name; } const \
	void set_##property_name(const std::vector<type>& property_values) { property_name = property_values; } \
	void add_to_##property_name(type property_value) { property_name.push_back(property_value); } \
protected: \
	std::vector<type> property_name; \
	void get_##property_name##_from_entity(const shared_ptr<::odata::core::odata_entity_value>& pentity); \
	void set_##property_name##_to_entity(const shared_ptr<::odata::core::odata_entity_value>& pentity); 

#define IMPLEMENT_COLLECTION_ENUM_PROPERTY_IN_ENTITY_MAPPING(myclass, property_name, edm_name, type) \
	void myclass::get_##property_name##_from_entity(const shared_ptr<::odata::core::odata_entity_value>& pentity) \
	{ \
		std::shared_ptr<::odata::core::odata_value> property_value; \
		if (!pentity->get_property_value(U(#edm_name), property_value) || !property_value) \
		{ \
			return ; \
		} \
		auto property_collection_value = static_pointer_cast<::odata::core::odata_collection_value>(property_value); \
		if (!property_collection_value) \
		{ \
			return ; \
		} \
		for (auto iter = property_collection_value->get_collection_values().cbegin(); iter != property_collection_value->get_collection_values().cend(); iter++) \
		{ \
		    auto enum_value = std::dynamic_pointer_cast<::odata::core::odata_enum_value>(*iter); \
			if (!enum_value) \
	        { \
			    continue ; \
			} \
			property_name.push_back(enum_type_resolver::get_enum_type_from_string(enum_value->to_string(), type())); \
		} \
	} \
	void myclass::set_##property_name##_to_entity(const shared_ptr<::odata::core::odata_entity_value>& pentity) \
	{ \
		if (!pentity || !pentity->get_value_type()) \
		{ \
			return ; \
		} \
	    auto entity_type = std::dynamic_pointer_cast<::odata::edm::edm_entity_type>(pentity->get_value_type()); \
		if (!entity_type) \
		{ \
			return ; \
		} \
		auto edm_property = entity_type->find_property(U(#edm_name)); \
		if (!edm_property) \
		{ \
			return ; \
		} \
		auto property_type = edm_property->get_property_type(); \
		auto collection_value_type = std::dynamic_pointer_cast<::odata::edm::edm_collection_type>(property_type); \
		if (!collection_value_type) \
		{ \
			return ; \
		} \
		std::shared_ptr<::odata::core::odata_collection_value> collection_value = std::make_shared<::odata::core::odata_collection_value>(collection_value_type); \
		for (size_t i = 0; i < property_name.size(); i++) \
		{ \
		    collection_value->add_collection_value(std::make_shared<::odata::core::odata_enum_value>(collection_value_type->get_element_type(), enum_type_resolver::get_string_from_enum_type(property_name[i]))); \
		} \
		pentity->set_value(U(#edm_name), collection_value); \
	}

#define DECLARE_COLLECTION_ENUM_PROPERTY_IN_COMPLEX_MAPPING(property_name, edm_name, type) \
public: \
	const std::vector<type>& get_##property_name() { return property_name; } const \
	void set_##property_name(const std::vector<type>& property_values) { property_name = property_values; } \
	void add_to_##property_name(type property_value) { property_name.push_back(property_value); } \
protected: \
	std::vector<type> property_name; \
	void get_##property_name##_from_complex(const shared_ptr<::odata::core::odata_complex_value>& pcomplex); \
	void set_##property_name##_to_complex(const shared_ptr<::odata::core::odata_complex_value>& pcomplex); 

#define IMPLEMENT_COLLECTION_ENUM_PROPERTY_IN_COMPLEX_MAPPING(myclass, property_name, edm_name, type) \
	void myclass::get_##property_name##_from_complex(const shared_ptr<::odata::core::odata_complex_value>& pcomplex) \
	{ \
		std::shared_ptr<::odata::core::odata_value> property_value; \
		if (!pcomplex->get_property_value(U(#edm_name), property_value) || !property_value) \
		{ \
			return ; \
		} \
		auto property_collection_value = static_pointer_cast<::odata::core::odata_collection_value>(property_value); \
		if (!property_collection_value) \
		{ \
			return ; \
		} \
		for (auto iter = property_collection_value->get_collection_values().cbegin(); iter != property_collection_value->get_collection_values().cend(); iter++) \
		{ \
		    auto enum_value = std::dynamic_pointer_cast<::odata::core::odata_enum_value>(*iter); \
			if (!enum_value) \
	        { \
			    continue ; \
			} \
			property_name.push_back(enum_type_resolver::get_enum_type_from_string(enum_value->to_string(), type())); \
		} \
	} \
	void myclass::set_##property_name##_to_complex(const shared_ptr<::odata::core::odata_complex_value>& pcomplex) \
	{ \
		if (!pcomplex || !pcomplex->get_value_type()) \
		{ \
			return ; \
		} \
	    auto complex_type = std::dynamic_pointer_cast<::odata::edm::edm_complex_type>(pcomplex->get_value_type()); \
		if (!complex_type) \
		{ \
			return ; \
		} \
		auto edm_property = complex_type->find_property(U(#edm_name)); \
		if (!edm_property) \
		{ \
			return ; \
		} \
		auto property_type = edm_property->get_property_type(); \
		auto collection_value_type = std::dynamic_pointer_cast<::odata::edm::edm_collection_type>(property_type); \
		if (!collection_value_type) \
		{ \
			return ; \
		} \
		std::shared_ptr<::odata::core::odata_collection_value> collection_value = std::make_shared<::odata::core::odata_collection_value>(collection_value_type); \
		for (size_t i = 0; i < property_name.size(); i++) \
		{ \
		    collection_value->add_collection_value(std::make_shared<::odata::core::odata_enum_value>(collection_value_type->get_element_type(), enum_type_resolver::get_string_from_enum_type(property_name[i]))); \
		} \
		pcomplex->set_value(U(#edm_name), collection_value); \
	}

#define DECLARE_NULLABLE_ENUM_PROPERTY_IN_ENTITY_MAPPING(property_name, edm_name, type) \
public: \
	type* get_##property_name() { return property_name.get(); } const \
	void set_##property_name(type property_value) \
	{ \
		property_name = std::make_shared<type>(property_value); \
	} \
protected: \
	std::shared_ptr<type> property_name; \
	void get_##property_name##_from_entity(const shared_ptr<::odata::core::odata_entity_value>& pentity); \
	void set_##property_name##_to_entity(const shared_ptr<::odata::core::odata_entity_value>& pentity); 

#define IMPLEMENT_NULLABLE_ENUM_PROPERTY_IN_ENTITY_MAPPING(myclass, property_name, edm_name, type) \
	void myclass::get_##property_name##_from_entity(const shared_ptr<::odata::core::odata_entity_value>& pentity) \
	{ \
		std::shared_ptr<::odata::core::odata_value> property_value; \
		if (!pentity->get_property_value(U(#edm_name), property_value) || !property_value) \
		{ \
			return ; \
		} \
		auto enum_value = std::dynamic_pointer_cast<::odata::core::odata_enum_value>(property_value); \
		if (enum_value) \
		{ \
		    property_name = std::make_shared<type>(enum_type_resolver::get_enum_type_from_string(enum_value->to_string(), type())); \
		} \
	} \
	void myclass::set_##property_name##_to_entity(const shared_ptr<::odata::core::odata_entity_value>& pentity) \
	{ \
		if (!pentity || !property_name) \
	    { \
		    return ; \
        }  \
		auto entity_type = std::dynamic_pointer_cast<::odata::edm::edm_entity_type>(pentity->get_value_type()); \
		if (!entity_type) \
		{ \
			return ; \
		} \
		auto edm_property = entity_type->find_property(U(#edm_name)); \
		if (!edm_property) \
		{ \
			return ; \
		} \
		auto property_type = edm_property->get_property_type(); \
		auto enum_value = std::make_shared<::odata::core::odata_enum_value>(property_type, enum_type_resolver::get_string_from_enum_type(*property_name)); \
		pentity->set_value(U(#edm_name), enum_value); \
	}

#define DECLARE_NULLABLE_ENUM_PROPERTY_IN_COMPLEX_MAPPING(property_name, edm_name, type) \
public: \
	type* get_##property_name() { return property_name.get(); } const \
	void set_##property_name(type property_value) \
	{  \
		property_name = std::make_shared<type>(property_value); \
	} \
protected: \
	std::shared_ptr<type> property_name; \
	void get_##property_name##_from_complex(const shared_ptr<::odata::core::odata_complex_value>& pcomplex); \
	void set_##property_name##_to_complex(const shared_ptr<::odata::core::odata_complex_value>& pcomplex);

#define IMPLEMENT_NULLABLE_ENUM_PROPERTY_IN_COMPLEX_MAPPING(myclass, property_name, edm_name, type) \
	void myclass::get_##property_name##_from_complex(const shared_ptr<::odata::core::odata_complex_value>& pcomplex) \
	{ \
		std::shared_ptr<::odata::core::odata_value> property_value; \
		if (!pcomplex->get_property_value(U(#edm_name), property_value) || !property_value) \
		{ \
			return ; \
		} \
		auto enum_value = std::dynamic_pointer_cast<::odata::core::odata_enum_value>(property_value); \
		if (enum_value) \
		{ \
			property_name = std::make_shared<type>(enum_type_resolver::get_enum_type_from_string(enum_value->to_string(), type())); \
		} \
	} \
	void myclass::set_##property_name##_to_complex(const shared_ptr<::odata::core::odata_complex_value>& pcomplex) \
	{ \
		if (!pcomplex || !property_name) \
	    { \
		    return ; \
        }  \
		auto complex_type = std::dynamic_pointer_cast<::odata::edm::edm_complex_type>(pcomplex->get_value_type()); \
		if (!complex_type) \
		{ \
			return ; \
		} \
		auto edm_property = complex_type->find_property(U(#edm_name)); \
		if (!edm_property) \
		{ \
			return ; \
		} \
		auto property_type = edm_property->get_property_type(); \
		auto enum_value = std::make_shared<::odata::core::odata_enum_value>(property_type, enum_type_resolver::get_string_from_enum_type(*property_name)); \
		pcomplex->set_value(U(#edm_name), enum_value); \
	}

#define DECLARE_FUNCTION_ENUM_TYPE_FROM_STRING(enum_type) \
	static enum_type get_enum_type_from_string(const ::utility::string_t& enum_string, enum_type default_enum_value);

#define DECLARE_FUNCTION_STRING_FROM_ENUM_TYPE(enum_type) \
	static ::utility::string_t get_string_from_enum_type(const enum_type& enum_value);

#define DECLARE_GET_ENUM_TYPE_NAMESPACE(enum_type, name_space) \
	static ::utility::string_t get_enum_type_namespace(const enum_type& enum_value); \

#define IMPLEMENT_GET_ENUM_TYPE_NAMESPACE(enum_type, name_space) \
	::utility::string_t enum_type_resolver::get_enum_type_namespace(const enum_type& enum_value) \
	{ \
		return U(#name_space); \
	}

#define BGEIN_IMPLEMENT_FUNCTION_ENUM_TYPE_FROM_STRING(enum_type) \
	enum_type enum_type_resolver::get_enum_type_from_string(const ::utility::string_t& enum_string, enum_type default_enum_value) \
	{ \
	    enum_type ret = default_enum_value;

#define ON_IMPLEMENT_FUNCTION_ENUM_TYPE_FROM_STRING(enum_value_string, enum_type) \
	if (enum_string == U(#enum_value_string)) \
	{ \
		ret = enum_type; \
	}

#define END_IMPLEMENT_FUNCTION_ENUM_TYPE_FROM_STRING(enum_type) \
	    return ret; \
    }

#define BGEIN_IMPLEMENT_FUNCTION_STRING_FROM_ENUM_TYPE(enum_type) \
	::utility::string_t enum_type_resolver::get_string_from_enum_type(const enum_type& enum_value) \
	{ \
	    ::utility::string_t ret;

#define ON_IMPLEMENT_FUNCTION_STRING_FROM_ENUM_TYPE(enum_type, enum_value_string) \
	if (enum_value == enum_type) \
	{ \
	    ret = U(#enum_value_string); \
	}

#define END_IMPLEMENT_FUNCTION_STRING_FROM_ENUM_TYPE(enum_type) \
	    return ret; \
    }

#define IMPLEMENT_EMPTY_DERIVED_TYPE_CREATOR_MAP(myclass) \
    std::unordered_map<::utility::string_t, CREATE_CALL_FOR_ENTITY> myclass::_derived_entity_creator_map;

#define BEGIN_IMPLEMENT_ENTITY_DERIVED_TYPE_CREATOR_MAP(myclass) \
    create_map_for_entity_type::value_type myclass::_init_creator_map_values[] = {

#define ON_IMPLEMENT_ENTITY_DERIVED_TYPE_CREATOR_MAP(keyname, funcname) \
    create_map_for_entity_type::value_type(U(#keyname), (CREATE_CALL_FOR_ENTITY)&create_##funcname),

#define END_IMPLEMENT_ENTITY_DERIVED_TYPE_CREATOR_MAP(myclass) \
    }; \
    std::unordered_map<::utility::string_t, CREATE_CALL_FOR_ENTITY> myclass::_derived_entity_creator_map(_init_creator_map_values, _init_creator_map_values + sizeof(_init_creator_map_values) / sizeof(create_map_for_entity_type::value_type));

#define DECLARE_DERIVED_ENTITY_CREATOR_FUNC(derivedclassname, funcname) \
    static std::shared_ptr<type_base> create_##funcname(const std::shared_ptr<::odata::codegen::odata_service_context>& service_context) \
    { \
        return std::make_shared<derivedclassname>(service_context); \
    }

#define IMPLEMENT_COMPLEX_DERIVED_TYPE_CREATOR_MAP(myclass) \
    std::unordered_map<::utility::string_t, CREATE_CALL_FOR_COMPLEX> myclass::_derived_complex_creator_map;

#define BEGIN_IMPLEMENT_COMPLEX_DERIVED_TYPE_CREATOR_MAP(myclass) \
    create_map_for_complex_type::value_type myclass::_init_creator_map_values[] = {

#define ON_IMPLEMENT_COMPLEX_DERIVED_TYPE_CREATOR_MAP(keyname, funcname) \
    create_map_for_complex_type::value_type(U(#keyname), (CREATE_CALL_FOR_COMPLEX)&create_##funcname),

#define END_IMPLEMENT_COMPLEX_DERIVED_TYPE_CREATOR_MAP(myclass) \
    }; \
    std::unordered_map<::utility::string_t, CREATE_CALL_FOR_COMPLEX> myclass::_derived_complex_creator_map(_init_creator_map_values, _init_creator_map_values + sizeof(_init_creator_map_values) / sizeof(create_map_for_entity_type::value_type));

#define DECLARE_DERIVED_COMPLEX_CREATOR_FUNC(derivedclassname, funcname) \
    static std::shared_ptr<type_base> create_##funcname(const std::shared_ptr<::odata::codegen::odata_service_context>& service_context) \
    { \
        return std::make_shared<derivedclassname>(service_context); \
    }

#define DECLARE_GET_KEY_PROPERTY_STRING_NO_PARAM(baseclass) \
public: \
	::utility::string_t get_key_property_string(bool with_key_name = false) \
	{ \
		return baseclass::get_key_property_string(with_key_name); \
	}

#define DECLARE_GET_KEY_PROPERTY_STRING_ONE_PARAM(baseclass, Name_1, key_1) \
public: \
	::utility::string_t get_key_property_string(bool with_key_name = false) \
	{ \
	    ::utility::stringstream_t ostr; \
		::utility::string_t key_string = baseclass::get_key_property_string(with_key_name); \
		if (key_string.empty()) \
		{ \
		    if (with_key_name) \
			    ostr << U(#Name_1) << U("=") << ::utility::conversions::print_string(key_1); \
			else \
		        ostr << ::utility::conversions::print_string(key_1); \
			key_string = ostr.str(); \
		} \
		else \
		{ \
		    key_string = baseclass::get_key_property_string(true); \
			ostr << U(#Name_1) << U("=") << ::utility::conversions::print_string(key_1); \
			key_string += U(",") + ostr.str(); \
		} \
		return key_string; \
	}

#define DECLARE_GET_KEY_PROPERTY_STRING_TWO_PARAM(baseclass, Name_1, key_1, Name_2, key_2) \
public: \
	::utility::string_t get_key_property_string(bool with_key_name = false) \
	{ \
	    ::utility::stringstream_t ostr; \
		ostr << U(#Name_1) << U("=") << ::utility::conversions::print_string(key_1) << U(",") << U(#Name_2) << U("=") + ::utility::conversions::print_string(key_2); \
		::utility::string_t key_string = baseclass::get_key_property_string(true); \
		if (key_string.empty()) \
		{ \
			key_string = ostr.str(); \
		} \
		else \
		{ \
			key_string += ostr.str(); \
		} \
		return key_string; \
	}

#define DECLARE_GET_KEY_PROPERTY_STRING_THREE_PARAM(baseclass, Name_1, key_1, Name_2, key_2, Name_3, key_3) \
public: \
	::utility::string_t get_key_property_string(bool with_key_name = false) \
	{ \
	    ::utility::stringstream_t ostr; \
		ostr << U(#Name_1) << U("=") << ::utility::conversions::print_string(key_1) << U(",") << U(#Name_2) << U("=") << ::utility::conversions::print_string(key_2) << U(",") << U(#Name_3) << U("=") << ::utility::conversions::print_string(key_3);\
		::utility::string_t key_string = baseclass::get_key_property_string(true); \
		if (key_string.empty()) \
		{ \
			key_string = ostr.str(); \
		} \
		else \
		{ \
			key_string += ostr.str(); \
		} \
		return key_string; \
	}

#define DECLARE_GET_KEY_PROPERTY_STRING_FOUR_PARAM(baseclass, Name_1, key_1, Name_2, key_2, Name_3, key_3, Name_4, key_4) \
public: \
	::utility::string_t get_key_property_string(bool with_key_name = false) \
	{ \
	    ::utility::stringstream_t ostr; \
		ostr << U(#Name_1) << U("=") << ::utility::conversions::print_string(key_1) << U(",") << U(#Name_2) << U("=") << ::utility::conversions::print_string(key_2) << U(",") << U(#Name_3) << U("=") << ::utility::conversions::print_string(key_3) << U(",") << U(#Name_4) << U("=") << ::utility::conversions::print_string(key_4);\
		::utility::string_t key_string = baseclass::get_key_property_string(true); \
		if (key_string.empty()) \
		{ \
			key_string = ostr.str(); \
		} \
		else \
		{ \
			key_string += ostr.str(); \
		} \
		return key_string; \
	}

#define DECLARE_OPERATION_IMPORT_P0(myclass, funcname, executor, isfunction) \
public: \
::pplx::task<typename executor::return_type> funcname();

#define IMPLEMENT_OPERATION_IMPORT_P0(myclass, funcname, executor, isfunction) \
::pplx::task<typename executor::return_type> myclass::funcname() \
{ \
    ::utility::string_t function_query_url = U(#funcname); \
	std::vector<std::shared_ptr<odata_parameter>> parameters; \
	return create_query<executor, odata_query_builder>(function_query_url)->execute_operation_query(parameters, isfunction); \
}

#define DECLARE_OPERATION_IMPORT_P1(myclass, funcname, executor, isfunction, key1, type1, value1) \
public: \
::pplx::task<typename executor::return_type> funcname(type1 value1);

#define IMPLEMENT_OPERATION_IMPORT_P1(myclass, funcname, executor, isfunction, key1, type1, value1) \
::pplx::task<typename executor::return_type> myclass::funcname(type1 value1) \
{ \
    ::utility::string_t function_query_url = U(#funcname); \
	std::vector<std::shared_ptr<odata_parameter>> parameters; \
	parameters.push_back(std::make_shared<odata_parameter>(U(#key1), format_function_parameter(value1))); \
	return create_query<executor, odata_query_builder>(function_query_url)->execute_operation_query(parameters, isfunction); \
}

#define DECLARE_OPERATION_IMPORT_P2(myclass, funcname, executor, isfunction, key1, type1, value1, key2, type2, value2) \
public: \
::pplx::task<typename executor::return_type> funcname(type1 value1, type2 value2);

#define IMPLEMENT_OPERATION_IMPORT_P2(myclass, funcname, executor, isfunction, key1, type1, value1, key2, type2, value2) \
::pplx::task<typename executor::return_type> myclass::funcname(type1 value1, type2 value2) \
{ \
    ::utility::string_t function_query_url = U(#funcname); \
	std::vector<std::shared_ptr<odata_parameter>> parameters; \
	parameters.push_back(std::make_shared<odata_parameter>(U(#key1), format_function_parameter(value1))); \
	parameters.push_back(std::make_shared<odata_parameter>(U(#key2), format_function_parameter(value2))); \
	return create_query<executor, odata_query_builder>(function_query_url)->execute_operation_query(parameters, isfunction);  \
}

#define DECLARE_OPERATION_IMPORT_P3(myclass, funcname, executor, isfunction, key1, type1, value1, key2, type2, value2, key3, type3, value3) \
public: \
::pplx::task<typename executor::return_type> funcname(type1 value1, type2 value2, type3 value3);

#define IMPLEMENT_OPERATION_IMPORT_P3(myclass, funcname, executor, isfunction, key1, type1, value1, key2, type2, value2, key3, type3, value3) \
::pplx::task<typename EXECUTOR::return_type> myclass::funcname(type1 value1, type2 value2, type3 value3) \
{ \
    ::utility::string_t function_query_url = U(#funcname); \
	std::vector<std::shared_ptr<odata_parameter>> parameters; \
	parameters.push_back(std::make_shared<odata_parameter>(U(#key1), format_function_parameter(value1))); \
	parameters.push_back(std::make_shared<odata_parameter>(U(#key2), format_function_parameter(value2))); \
	parameters.push_back(std::make_shared<odata_parameter>(U(#key3), format_function_parameter(value3))); \
	return create_query<executor, odata_query_builder>(function_query_url)->execute_operation_query(parameters, isfunction);  \
}

#define DECLARE_OPERATION_IMPORT_P4(myclass, funcname, executor, isfunction, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4) \
public: \
::pplx::task<typename executor::return_type> funcname(type1 value1, type2 value2, type3 value3, type4 value4);

#define IMPLEMENT_OPERATION_IMPORT_P4(myclass, funcname, executor, isfunction, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4) \
::pplx::task<typename EXECUTOR::return_type> myclass::funcname(type1 value1, type2 value2, type3 value3, type4 value4) \
{ \
    ::utility::string_t function_query_url = U(#funcname); \
	std::vector<std::shared_ptr<odata_parameter>> parameters; \
	parameters.push_back(std::make_shared<odata_parameter>(U(#key1), format_function_parameter(value1))); \
	parameters.push_back(std::make_shared<odata_parameter>(U(#key2), format_function_parameter(value2))); \
	parameters.push_back(std::make_shared<odata_parameter>(U(#key3), format_function_parameter(value3))); \
	parameters.push_back(std::make_shared<odata_parameter>(U(#key4), format_function_parameter(value4))); \
	return create_query<executor, odata_query_builder>(function_query_url)->execute_operation_query(parameters, isfunction);  \
}

#define DECLARE_OPERATION_IMPORT_P5(myclass, funcname, executor, isfunction, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5) \
public: \
::pplx::task<typename executor::return_type> funcname(type1 value1, type2 value2, type3 value3, type4 value4, type5 value5);

#define IMPLEMENT_OPERATION_IMPORT_P5(myclass, funcname, executor, isfunction, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5) \
::pplx::task<typename EXECUTOR::return_type> myclass::funcname(type1 value1, type2 value2, type3 value3, type4 value4, type5 value5) \
{ \
    ::utility::string_t function_query_url = U(#funcname); \
	std::vector<std::shared_ptr<odata_parameter>> parameters; \
	parameters.push_back(std::make_shared<odata_parameter>(U(#key1), format_function_parameter(value1))); \
	parameters.push_back(std::make_shared<odata_parameter>(U(#key2), format_function_parameter(value2))); \
	parameters.push_back(std::make_shared<odata_parameter>(U(#key3), format_function_parameter(value3))); \
	parameters.push_back(std::make_shared<odata_parameter>(U(#key4), format_function_parameter(value4))); \
	parameters.push_back(std::make_shared<odata_parameter>(U(#key5), format_function_parameter(value5))); \
	return create_query<executor, odata_query_builder>(function_query_url)->execute_operation_query(parameters, isfunction);  \
}

#define DECLARE_OPERATION_P0(myclass, funcname, executor, isfunction) \
public: \
::pplx::task<typename executor::return_type> funcname();

#define IMPLEMENT_OPERATION_P0(myclass, funcname, executor, isfunction) \
::pplx::task<typename executor::return_type> myclass::funcname() \
{ \
	::utility::string_t function_query_url = m_service_context->get_relative_path(m_edit_link) + U("/"); \
	function_query_url += m_namespace; \
	function_query_url += U("."); \
	function_query_url += U(#funcname); \
	std::vector<std::shared_ptr<odata_parameter>> parameters; \
	return m_service_context->create_query<executor, odata_query_builder>(function_query_url)->execute_operation_query(parameters, isfunction); \
}

#define DECLARE_OPERATION_P1(myclass, funcname, executor, isfunction, key1, type1, value1) \
public: \
::pplx::task<typename executor::return_type> funcname(type1 value1);

#define IMPLEMENT_OPERATION_P1(myclass, funcname, executor, isfunction, key1, type1, value1) \
::pplx::task<typename executor::return_type> myclass::funcname(type1 value1) \
{ \
	::utility::string_t function_query_url = m_service_context->get_relative_path(m_edit_link) + U("/"); \
	function_query_url += m_namespace; \
	function_query_url += U("."); \
	function_query_url += U(#funcname); \
	std::vector<std::shared_ptr<odata_parameter>> parameters; \
	parameters.push_back(std::make_shared<odata_parameter>(U(#key1), format_function_parameter(value1))); \
	return m_service_context->create_query<executor, odata_query_builder>(function_query_url)->execute_operation_query(parameters, isfunction); \
}

#define DECLARE_OPERATION_P2(myclass, funcname, executor, isfunction, key1, type1, value1, key2, type2, value2) \
public: \
::pplx::task<typename executor::return_type> funcname(type1 value1, type2 value2);

#define IMPLEMENT_OPERATION_P2(myclass, funcname, executor, isfunction, key1, type1, value1, key2, type2, value2) \
                ::pplx::task<typename executor::return_type> myclass::funcname(type1 value1, type2 value2) \
{ \
	::utility::string_t function_query_url = m_service_context->get_relative_path(m_edit_link) + U("/"); \
	function_query_url += m_namespace; \
	function_query_url += U("."); \
	function_query_url += U(#funcname); \
	std::vector<std::shared_ptr<odata_parameter>> parameters; \
	parameters.push_back(std::make_shared<odata_parameter>(U(#key1), format_function_parameter(value1))); \
	parameters.push_back(std::make_shared<odata_parameter>(U(#key2), format_function_parameter(value2))); \
	return m_service_context->create_query<executor, odata_query_builder>(function_query_url)->execute_operation_query(parameters, isfunction); \
}

#define DECLARE_OPERATION_P3(myclass, funcname, executor, isfunction, key1, type1, value1, key2, type2, value2, key3, type3, value3) \
public: \
::pplx::task<typename executor::return_type> funcname(type1 value1, type2 value2, type3 value3);

#define IMPLEMENT_OPERATION_P3(myclass, funcname, executor, isfunction, key1, type1, value1, key2, type2, value2, key3, type3, value3) \
::pplx::task<typename executor::return_type> myclass::funcname(type1 value1, type2 value2, type3 value3) \
{ \
	::utility::string_t function_query_url = m_service_context->get_relative_path(m_edit_link) + U("/"); \
	function_query_url += m_namespace; \
	function_query_url += U("."); \
	function_query_url += U(#funcname); \
	std::vector<std::shared_ptr<odata_parameter>> parameters; \
	parameters.push_back(std::make_shared<odata_parameter>(U(#key1), format_function_parameter(value1))); \
	parameters.push_back(std::make_shared<odata_parameter>(U(#key2), format_function_parameter(value2))); \
	parameters.push_back(std::make_shared<odata_parameter>(U(#key3), format_function_parameter(value3))); \
	return m_service_context->create_query<executor, odata_query_builder>(function_query_url)->execute_operation_query(parameters, isfunction); \
}

#define DECLARE_OPERATION_P4(myclass, funcname, executor, isfunction, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4) \
public: \
::pplx::task<typename executor::return_type> funcname(type1 value1, type2 value2, type3 value3, type4 value4);

#define IMPLEMENT_OPERATION_P4(myclass, funcname, executor, isfunction, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4) \
::pplx::task<typename executor::return_type> myclass::funcname(type1 value1, type2 value2, type3 value3, type4 value4) \
{ \
	::utility::string_t function_query_url = m_service_context->get_relative_path(m_edit_link) + U("/"); \
	function_query_url += m_namespace; \
	function_query_url += U("."); \
	function_query_url += U(#funcname); \
	std::vector<std::shared_ptr<odata_parameter>> parameters; \
	parameters.push_back(std::make_shared<odata_parameter>(U(#key1), format_function_parameter(value1))); \
	parameters.push_back(std::make_shared<odata_parameter>(U(#key2), format_function_parameter(value2))); \
	parameters.push_back(std::make_shared<odata_parameter>(U(#key3), format_function_parameter(value3))); \
	parameters.push_back(std::make_shared<odata_parameter>(U(#key4), format_function_parameter(value4))); \
	return m_service_context->create_query<executor, odata_query_builder>(function_query_url)->execute_operation_query(parameters, isfunction); \
}

#define DECLARE_OPERATION_P5(myclass, funcname, executor, isfunction, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5) \
public: \
::pplx::task<typename executor::return_type> funcname(type1 value1, type2 value2, type3 value3, type4 value4, type5 value5);

#define IMPLEMENT_OPERATION_P5(myclass, funcname, executor, isfunction, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5) \
::pplx::task<typename executor::return_type> myclass::funcname(type1 value1, type2 value2, type3 value3, type4 value4, type5 value5) \
{ \
	::utility::string_t function_query_url = m_service_context->get_relative_path(m_edit_link) + U("/"); \
	function_query_url += m_namespace; \
	function_query_url += U("."); \
	function_query_url += U(#funcname); \
	std::vector<std::shared_ptr<odata_parameter>> parameters; \
	parameters.push_back(std::make_shared<odata_parameter>(U(#key1), format_function_parameter(value1))); \
	parameters.push_back(std::make_shared<odata_parameter>(U(#key2), format_function_parameter(value2))); \
	parameters.push_back(std::make_shared<odata_parameter>(U(#key3), format_function_parameter(value3))); \
	parameters.push_back(std::make_shared<odata_parameter>(U(#key4), format_function_parameter(value4))); \
	parameters.push_back(std::make_shared<odata_parameter>(U(#key5), format_function_parameter(value5))); \
	return m_service_context->create_query<executor, odata_query_builder>(function_query_url)->execute_operation_query(parameters, isfunction); \
}

#define DECLARE_FUNCTION_IMPORT_P0(myclass, funcname, executor) \
	DECLARE_OPERATION_IMPORT_P0(myclass, funcname, executor, true)

#define IMPLEMENT_FUNCTION_IMPORT_P0(myclass, funcname, executor) \
	IMPLEMENT_OPERATION_IMPORT_P0(myclass, funcname, executor, true)

#define DECLARE_FUNCTION_IMPORT_P1(myclass, funcname, executor, key1, type1, value1) \
	DECLARE_OPERATION_IMPORT_P1(myclass, funcname, executor, true, key1, type1, value1)

#define IMPLEMENT_FUNCTION_IMPORT_P1(myclass, funcname, executor, key1, type1, value1) \
	IMPLEMENT_OPERATION_IMPORT_P1(myclass, funcname, executor, true, key1, type1, value1)

#define DECLARE_FUNCTION_IMPORT_P2(myclass, funcname, executor, key1, type1, value1, key2, type2, value2) \
	DECLARE_OPERATION_IMPORT_P2(myclass, funcname, executor, true, key1, type1, value1, key2, type2, value2)

#define IMPLEMENT_FUNCTION_IMPORT_P2(myclass, funcname, executor, key1, type1, value1, key2, type2, value2) \
	IMPLEMENT_OPERATION_IMPORT_P2(myclass, funcname, executor, true, key1, type1, value1, key2, type2, value2)

#define DECLARE_FUNCTION_IMPORT_P3(myclass, funcname, executor, key1, type1, value1, key2, type2, value2, key3, type3, value3) \
	DECLARE_OPERATION_IMPORT_P3(myclass, funcname, executor, true, key1, type1, value1, key2, type2, value2, key3, type3, value3)

#define IMPLEMENT_FUNCTION_IMPORT_P3(myclass, funcname, executor, key1, type1, value1, key2, type2, value2, key3, type3, value3) \
	IMPLEMENT_OPERATION_IMPORT_P3(myclass, funcname, executor, true, key1, type1, value1, key2, type2, value2, key3, type3, value3)

#define IMPLEMENT_FUNCTION_IMPORT_P4(myclass, funcname, executor, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4) \
	IMPLEMENT_OPERATION_IMPORT_P4(myclass, funcname, executor, true, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4)

#define IMPLEMENT_FUNCTION_IMPORT_P5(myclass, funcname, executor, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5) \
	IMPLEMENT_OPERATION_IMPORT_P5(myclass, funcname, executor, true, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5)

#define DECLARE_FUNCTION_P0(myclass, funcname, executor) \
    DECLARE_OPERATION_P0(myclass, funcname, executor, true)

#define IMPLEMENT_FUNCTION_P0(myclass, funcname, executor) \
    IMPLEMENT_OPERATION_P0(myclass, funcname, executor, true)

#define DECLARE_FUNCTION_P1(myclass, funcname, executor, key1, type1, value1) \
    DECLARE_OPERATION_P1(myclass, funcname, executor, true, key1, type1, value1)

#define IMPLEMENT_FUNCTION_P1(myclass, funcname, executor, key1, type1, value1) \
    IMPLEMENT_OPERATION_P1(myclass, funcname, executor, true, key1, type1, value1)

#define DECLARE_FUNCTION_P2(myclass, funcname, executor, key1, type1, value1, key2, type2, value2) \
    DECLARE_OPERATION_P2(myclass, funcname, executor, true, key1, type1, value1, key2, type2, value2)

#define IMPLEMENT_FUNCTION_P2(myclass, funcname, executor, key1, type1, value1, key2, type2, value2) \
    IMPLEMENT_OPERATION_P2(myclass, funcname, executor, true, key1, type1, value1, key2, type2, value2)

#define DECLARE_FUNCTION_P3(myclass, funcname, executor, key1, type1, value1, key2, type2, value2, key3, type3, value3) \
    DECLARE_OPERATION_P3(myclass, funcname, executor, true, key1, type1, value1, key2, type2, value2, key3, type3, value3)

#define IMPLEMENT_FUNCTION_P3(myclass, funcname, executor, key1, type1, value1, key2, type2, value2, key3, type3, value3) \
   IMPLEMENT_OPERATION_P3(myclass, funcname, executor, true, key1, type1, value1, key2, type2, value2, key3, type3, value3)

#define IMPLEMENT_FUNCTION_P4(myclass, funcname, executor, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4) \
   IMPLEMENT_OPERATION_P4(myclass, funcname, executor, true, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4)

#define IMPLEMENT_FUNCTION_P5(myclass, funcname, executor, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5) \
   IMPLEMENT_OPERATION_P5(myclass, funcname, executor, true, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5)

#define DECLARE_ACTION_IMPORT_P0(myclass, funcname, executor) \
	DECLARE_OPERATION_IMPORT_P0(myclass, funcname, executor, false)

#define IMPLEMENT_ACTION_IMPORT_P0(myclass, funcname, executor) \
	IMPLEMENT_OPERATION_IMPORT_P0(myclass, funcname, executor, false)

#define DECLARE_ACTION_IMPORT_P1(myclass, funcname, executor, key1, type1, value1) \
	DECLARE_OPERATION_IMPORT_P1(myclass, funcname, executor, false, key1, type1, value1)

#define IMPLEMENT_ACTION_IMPORT_P1(myclass, funcname, executor, key1, type1, value1) \
	IMPLEMENT_OPERATION_IMPORT_P1(myclass, funcname, executor, false, key1, type1, value1)

#define DECLARE_ACTION_IMPORT_P2(myclass, funcname, executor, key1, type1, value1, key2, type2, value2) \
	DECLARE_OPERATION_IMPORT_P2(myclass, funcname, executor, false, key1, type1, value1, key2, type2, value2)

#define IMPLEMENT_ACTION_IMPORT_P2(myclass, funcname, executor, key1, type1, value1, key2, type2, value2) \
	IMPLEMENT_OPERATION_IMPORT_P2(myclass, funcname, executor, false, key1, type1, value1, key2, type2, value2)

#define DECLARE_ACTION_IMPORT_P3(myclass, funcname, executor, key1, type1, value1, key2, type2, value2, key3, type3, value3) \
	DECLARE_OPERATION_IMPORT_P3(myclass, funcname, executor, false, key1, type1, value1, key2, type2, value2, key3, type3, value3)

#define DECLARE_ACTION_IMPORT_P4(myclass, funcname, executor, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4) \
	DECLARE_OPERATION_IMPORT_P4(myclass, funcname, executor, false, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4)

#define DECLARE_ACTION_IMPORT_P5(myclass, funcname, executor, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5) \
	DECLARE_OPERATION_IMPORT_P5(myclass, funcname, executor, false, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5)

#define IMPLEMENT_ACTION_IMPORT_P3(myclass, funcname, executor, key1, type1, value1, key2, type2, value2, key3, type3, value3) \
	IMPLEMENT_OPERATION_IMPORT_P3(myclass, funcname, executor, false, key1, type1, value1, key2, type2, value2, key3, type3, value3)

#define DECLARE_ACTION_P0(myclass, funcname, executor) \
    DECLARE_OPERATION_P0(myclass, funcname, executor, false)

#define IMPLEMENT_ACTION_P0(myclass, funcname, executor) \
    IMPLEMENT_OPERATION_P0(myclass, funcname, executor, false)

#define DECLARE_ACTION_P1(myclass, funcname, executor, key1, type1, value1) \
    DECLARE_OPERATION_P1(myclass, funcname, executor, false, key1, type1, value1)

#define IMPLEMENT_ACTION_P1(myclass, funcname, executor, key1, type1, value1) \
    IMPLEMENT_OPERATION_P1(myclass, funcname, executor, false, key1, type1, value1)

#define DECLARE_ACTION_P2(myclass, funcname, executor, key1, type1, value1, key2, type2, value2) \
    DECLARE_OPERATION_P2(myclass, funcname, executor, false, key1, type1, value1, key2, type2, value2)

#define IMPLEMENT_ACTION_P2(myclass, funcname, executor, key1, type1, value1, key2, type2, value2) \
    IMPLEMENT_OPERATION_P2(myclass, funcname, executor, false, key1, type1, value1, key2, type2, value2)

#define DECLARE_ACTION_P3(myclass, funcname, executor, key1, type1, value1, key2, type2, value2, key3, type3, value3) \
    DECLARE_OPERATION_P3(myclass, funcname, executor, false, key1, type1, value1, key2, type2, value2, key3, type3, value3)

#define IMPLEMENT_ACTION_P3(myclass, funcname, executor, key1, type1, value1, key2, type2, value2, key3, type3, value3) \
   IMPLEMENT_OPERATION_P3(myclass, funcname, executor, false, key1, type1, value1, key2, type2, value2, key3, type3, value3)

#define DECLARE_ACTION_P4(myclass, funcname, executor, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4) \
    DECLARE_OPERATION_P4(myclass, funcname, executor, false, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4)

#define IMPLEMENT_ACTION_P4(myclass, funcname, executor, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4) \
   IMPLEMENT_OPERATION_P4(myclass, funcname, executor, false, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4)

#define DECLARE_ACTION_P5(myclass, funcname, executor, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5) \
    DECLARE_OPERATION_P5(myclass, funcname, executor, false, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5)

#define IMPLEMENT_ACTION_P5(myclass, funcname, executor, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5) \
   IMPLEMENT_OPERATION_P5(myclass, funcname, executor, false, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5)

}}
