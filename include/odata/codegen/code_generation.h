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
	const type& get_##property_name() const { return property_name; } \
	void set_##property_name(const type& property_value) { property_name = property_value; } \
protected: \
	type property_name; \
	void get_##property_name##_from_entity(const shared_ptr<::odata::core::odata_entity_value>& pentity); \
	void set_##property_name##_to_entity(const shared_ptr<::odata::core::odata_entity_value>& pentity);

#define IMPLEMENT_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(myclass, property_name, edm_name, type) \
	void myclass::get_##property_name##_from_entity(const shared_ptr<::odata::core::odata_entity_value>& pentity) \
	{ \
		std::shared_ptr<::odata::core::odata_value> property_value; \
		if (!pentity->get_property_value(_XPLATSTR(#edm_name), property_value) || !property_value) \
		{ \
			return; \
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
			return; \
		} \
		pentity->set_value(_XPLATSTR(#edm_name), property_name); \
	}

#define DECLARE_PRIMITIVE_PROPERTY_IN_COMPLEX_MAPPING(property_name, edm_name, type) \
public : \
	const type& get_##property_name() const { return property_name; } \
	void set_##property_name(const type& property_value) { property_name = property_value; } \
protected: \
	type property_name; \
	void get_##property_name##_from_complex(const shared_ptr<::odata::core::odata_complex_value>& pcomplex); \
	void set_##property_name##_to_complex(const shared_ptr<::odata::core::odata_complex_value>& pcomplex);

#define IMPLEMENT_PRIMITIVE_PROPERTY_IN_COMPLEX_MAPPING(myclass, property_name, edm_name, type) \
	void myclass::get_##property_name##_from_complex(const shared_ptr<::odata::core::odata_complex_value>& pcomplex) \
	{ \
		std::shared_ptr<::odata::core::odata_value> property_value; \
		if (!pcomplex->get_property_value(_XPLATSTR(#edm_name), property_value) || !property_value) \
		{ \
			return; \
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
			return; \
		} \
		pcomplex->set_value(_XPLATSTR(#edm_name), property_name); \
	}

#define ENABLE_PROPERTY_IN_ENTITY_MAPPING() \
public: \
	void from_value(const shared_ptr<::odata::core::odata_entity_value>& pentity); \
	virtual std::shared_ptr<::odata::core::odata_entity_value> to_value(); \
	PROPMAP_ENTRY_IN_ENTITY* get_to_entity_map(); \
	void set_to_entity_map(PROPMAP_ENTRY_IN_ENTITY* _map); \
protected: \
	static PROPMAP_ENTRY_IN_ENTITY _property_entries[]; \
	PROPMAP_ENTRY_IN_ENTITY* get_mapping_map(); \
	static PROPMAP_ENTRY_IN_ENTITY _map_class_to_entity_entries[]; \
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
	static PROPMAP_ENTRY_IN_COMPLEX _property_entries[]; \
	PROPMAP_ENTRY_IN_COMPLEX* get_mapping_map(); \
	static PROPMAP_ENTRY_IN_COMPLEX _map_class_to_complex_entries[]; \
	PROPMAP_ENTRY_IN_COMPLEX* m_to_complex_map; \
	void map_complex_value_to_class_member(const shared_ptr<::odata::core::odata_complex_value>& pcomplex); \
	void map_class_member_to_complex_value(const std::shared_ptr<::odata::core::odata_complex_value>& complex_value);


#define DECLARE_EDM_INFO() \
	protected: \
	static ::odata::string_t m_namespace; \
	static ::odata::string_t m_typename; \
	public: \
	static ::odata::string_t get_full_name(); \
	static ::odata::string_t get_type_name();

#define IMPLEMENT_EDM_INFO(myclass, name_space, type_name) \
	::odata::string_t myclass::m_namespace = _XPLATSTR(#name_space); \
	::odata::string_t myclass::m_typename = _XPLATSTR(#type_name); \
	::odata::string_t myclass::get_full_name() \
	{ \
		return myclass::m_namespace + _XPLATSTR(".") + myclass::m_typename; \
	} \
	::odata::string_t myclass::get_type_name() \
	{ \
		return m_typename; \
	}

#define BEGIN_PROPERTY_IN_ENTITY_MAPPING(myclass) \
	PROPMAP_ENTRY_IN_ENTITY* myclass::get_mapping_map() \
	{ \
		return _property_entries; \
	} \
	PROPMAP_ENTRY_IN_ENTITY* myclass::get_to_entity_map() \
	{ \
		if (!m_to_entity_map) \
			m_to_entity_map = _map_class_to_entity_entries; \
		return m_to_entity_map; \
	} \
	void myclass::set_to_entity_map(PROPMAP_ENTRY_IN_ENTITY* _map) \
	{ \
	m_to_entity_map = _map; \
	} \
	PROPMAP_ENTRY_IN_ENTITY myclass::_property_entries[] = \
	{

#define BEGIN_PROPERTY_IN_ENTITY_MAPPING_WITH_BASE_CLASS(myclass, parentclass) \
	BEGIN_PROPERTY_IN_ENTITY_MAPPING(myclass)

#define END_PROPERTY_IN_ENTITY_MAPPING(myclass) \
		{(PROP_MAP_CALL_IN_ENTITY)0} \
	}; \
	std::shared_ptr<::odata::core::odata_entity_value> myclass::to_value() \
	{ \
		if (!m_service_context || !m_service_context->get_edm_model()) { return nullptr; } \
		auto entity_type = m_service_context->get_edm_model()->find_entity_type(m_typename); \
		auto entity_value = ::odata::make_shared<::odata::core::odata_entity_value>(entity_type); \
		map_class_member_to_entity_value(entity_value); \
		if (!m_namespace.empty() && !m_typename.empty()) \
		{ \
			entity_value->set_value(::odata::core::odata_json_constants::PAYLOAD_ANNOTATION_TYPE, ::odata::make_shared<odata_primitive_value>(::odata::make_shared<::odata::edm::edm_payload_annotation_type>(::odata::core::odata_json_constants::PAYLOAD_ANNOTATION_TYPE), _XPLATSTR("#") + m_namespace + _XPLATSTR(".") + m_typename)); \
		} \
		return entity_value; \
	} \
	void myclass::from_value(const shared_ptr<::odata::core::odata_entity_value>& pentity) \
	{ \
		map_entity_value_to_class_member(pentity); \
	}

#define END_PROPERTY_IN_ENTITY_MAPPING_WITH_BASE_CLASS(myclass, parentclass) \
		{(PROP_MAP_CALL_IN_ENTITY)0} \
	}; \
	std::shared_ptr<::odata::core::odata_entity_value> myclass::to_value() \
	{ \
		if (!m_service_context || !m_service_context->get_edm_model()) { return nullptr; } \
		auto entity_value = parentclass::to_value(); \
		auto entity_type = m_service_context->get_edm_model()->find_entity_type(m_typename); \
		entity_value->set_value_type(entity_type); \
		map_class_member_to_entity_value(entity_value); \
		if (!m_namespace.empty() && !m_typename.empty()) \
		{ \
entity_value->set_value(::odata::core::odata_json_constants::PAYLOAD_ANNOTATION_TYPE, ::odata::make_shared<odata_primitive_value>(::odata::make_shared<::odata::edm::edm_payload_annotation_type>(::odata::core::odata_json_constants::PAYLOAD_ANNOTATION_TYPE), _XPLATSTR("#") + m_namespace + _XPLATSTR(".") + m_typename)); \
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

#define ON_OPEN_PROPERTY_IN_ENTITY_MAPPING(myclass) \
	{(PROP_MAP_CALL_IN_ENTITY)&myclass::get_open_properties_from_entity},

#define BEGIN_PROPERTY_IN_COMPLEX_MAPPING(myclass) \
	PROPMAP_ENTRY_IN_COMPLEX* myclass::get_mapping_map() \
	{ \
		return _property_entries; \
	} \
	PROPMAP_ENTRY_IN_COMPLEX* myclass::get_to_complex_map() \
	{ \
		if (!m_to_complex_map) \
			m_to_complex_map = _map_class_to_complex_entries; \
		return m_to_complex_map; \
	} \
	void myclass::set_to_complex_map(PROPMAP_ENTRY_IN_COMPLEX* _map) \
	{ \
		m_to_complex_map = _map; \
	} \
	PROPMAP_ENTRY_IN_COMPLEX myclass::_property_entries[] = \
	{

#define BEGIN_PROPERTY_IN_COMPLEX_MAPPING_WITH_BASE_CLASS(myclass, parentclass) \
	BEGIN_PROPERTY_IN_COMPLEX_MAPPING(myclass)

#define END_PROPERTY_IN_COMPLEX_MAPPING(myclass) \
		{(PROP_MAP_CALL_IN_COMPLEX)0 } \
	}; \
	std::shared_ptr<::odata::core::odata_complex_value> myclass::to_value() \
	{ \
		if (!m_service_context || !m_service_context->get_edm_model()) { return nullptr; } \
		auto complex_type = m_service_context->get_edm_model()->find_complex_type(m_typename); \
		auto complex_value = ::odata::make_shared<::odata::core::odata_complex_value>(complex_type); \
		map_class_member_to_complex_value(complex_value); \
		if (!m_namespace.empty() && !m_typename.empty()) \
		{ \
			complex_value->set_value(::odata::core::odata_json_constants::PAYLOAD_ANNOTATION_TYPE, ::odata::make_shared<odata_primitive_value>(::odata::make_shared<::odata::edm::edm_payload_annotation_type>(::odata::core::odata_json_constants::PAYLOAD_ANNOTATION_TYPE), _XPLATSTR("#") + m_namespace + _XPLATSTR(".") + m_typename)); \
		} \
		return complex_value; \
	} \
	void myclass::from_value(const shared_ptr<::odata::core::odata_complex_value>& pcomplex) \
	{ \
		map_complex_value_to_class_member(pcomplex); \
	}

#define END_PROPERTY_IN_COMPLEX_MAPPING_WITH_BASE_CLASS(myclass, parentclass) \
	{(PROP_MAP_CALL_IN_COMPLEX)0 } \
	}; \
	std::shared_ptr<::odata::core::odata_complex_value> myclass::to_value() \
	{ \
		if (!m_service_context || !m_service_context->get_edm_model()) { return nullptr; } \
		auto complex_type = m_service_context->get_edm_model()->find_complex_type(m_typename); \
		auto complex_value = parentclass::to_value(); \
		complex_value->set_value_type(complex_type); \
		map_class_member_to_complex_value(complex_value); \
		if (!m_namespace.empty() && !m_typename.empty()) \
		{ \
			complex_value->set_value(::odata::core::odata_json_constants::PAYLOAD_ANNOTATION_TYPE, ::odata::make_shared<odata_primitive_value>(::odata::make_shared<::odata::edm::edm_payload_annotation_type>(::odata::core::odata_json_constants::PAYLOAD_ANNOTATION_TYPE), _XPLATSTR("#") + m_namespace + _XPLATSTR(".") + m_typename)); \
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

#define ON_OPEN_PROPERTY_IN_COMPLEX_MAPPING(myclass) \
	{(PROP_MAP_CALL_IN_COMPLEX)&myclass::get_open_properties_from_complex}, \

#define DECLARE_ENTITY_CONSTRUCTOR(myclass) \
	public: \
	myclass(const std::shared_ptr<odata_service_context>& service_context); \
	::odata::string_t get_root_url() { return m_service_context ? m_service_context->get_root_url() : _XPLATSTR(""); } \
	static create_map_for_entity_type::value_type _init_creator_map_values[]; \
	static std::unordered_map<::odata::string_t, CREATE_CALL_FOR_ENTITY> _derived_entity_creator_map;

#define BEGIN_ENTITY_CONSTRUCTOR(myclass, baseclass) \
	myclass::myclass(const std::shared_ptr<odata_service_context>& service_context) \
	: baseclass(service_context), m_to_entity_map(nullptr)

#define ON_PROPERTY_IN_ENTITY_CONSTRUCTOR(property_name, property_value) \
		, property_name(property_value)

#define ON_OPEN_PROPERTY_IN_ENTITY_CONSTRUCTOR() \
		, open_property_dictionary()

#define END_ENTITY_CONSTRUCTOR(myclass, baseclass) \
	{ }

#define DECLARE_COMPLEX_CONSTRUCTOR(myclass) \
	public: \
	myclass(const std::shared_ptr<odata_service_context>& service_context); \
	static create_map_for_complex_type::value_type _init_creator_map_values[]; \
	static std::unordered_map<::odata::string_t, CREATE_CALL_FOR_COMPLEX> _derived_complex_creator_map;

#define BEGIN_COMPLEX_CONSTRUCTOR(myclass, baseclass) \
	myclass::myclass(const std::shared_ptr<odata_service_context>& service_context) \
	: baseclass(service_context), m_to_complex_map(nullptr)

#define ON_PROPERTY_IN_COMPLEX_CONSTRUCTOR(property_name, property_value) \
		, property_name(property_value)

#define ON_OPEN_PROPERTY_IN_COMPLEX_CONSTRUCTOR() \
		, open_property_dictionary()

#define END_COMPLEX_CONSTRUCTOR(myclass, baseclass) \
	{ }

#define DECLARE_ENTITY_DESTRUCTOR(myclass) \
	public: \
	~myclass();

#define BEGIN_ENTITY_DESTRUCTOR(myclass) \
	myclass::~myclass() \
	{

#define ON_PROPERTY_IN_ENTITY_DESTRUCTOR(property_name)

#define ON_OPEN_PROPERTY_IN_ENTITY_DESTRUCTOR()

#define END_ENTITY_DESTRUCTOR(myclass) \
	}

#define DECLARE_COMPLEX_DESTRUCTOR(myclass) \
	public: \
	~myclass();

#define BEGIN_COMPLEX_DESTRUCTOR(myclass) \
	myclass::~myclass() \
	{

#define ON_PROPERTY_IN_COMPLEX_DESTRUCTOR(property_name)

#define ON_OPEN_PROPERTY_IN_COMPLEX_DESTRUCTOR()

#define END_COMPLEX_DESTRUCTOR(myclass) \
	}

#define BEGIN_SERIALIZE_PROPERTY_IN_ENTITY_MAPPING(myclass) \
	PROPMAP_ENTRY_IN_ENTITY myclass::_map_class_to_entity_entries[] = \
	{

#define ON_SERIALIZE_PROPERTY_IN_ENTITY_MAPPING(myclass, property_name) \
		{(PROP_MAP_CALL_IN_ENTITY)&myclass::set_##property_name##_to_entity},

#define ON_SERIALIZE_OPEN_PROPERTY_IN_ENTITY_MAPPING(myclass) \
		{(PROP_MAP_CALL_IN_ENTITY)&myclass::set_open_properties_to_entity},

#define END_SERIALIZE_PROPERTY_IN_ENTITY_MAPPING(myclass) \
		{(PROP_MAP_CALL_IN_ENTITY)0} \
	}; \
	void myclass::map_entity_value_to_class_member(const shared_ptr<::odata::core::odata_entity_value>& pentity) \
	{ \
		if (m_edit_link.empty()) \
		{ \
			m_edit_link = odata_entity_model_builder::compute_edit_link(get_root_url(), pentity, _XPLATSTR(""), false); \
		} \
		for (size_t i = 0;; ++i) \
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
		for (size_t i = 0;; ++i) \
		{ \
			PROPMAP_ENTRY_IN_ENTITY pentry = get_to_entity_map()[i]; \
			if (pentry.pfn == 0) \
			{ \
				break; \
			} \
			(this->*((PROP_MAP_CALL_IN_ENTITY)pentry.pfn))(pentity); \
		} \
	}

#define BEGIN_SERIALIZE_PROPERTY_IN_COMPLEX_MAPPING(myclass) \
	PROPMAP_ENTRY_IN_COMPLEX myclass::_map_class_to_complex_entries[] = \
	{

#define ON_SERIALIZE_PROPERTY_IN_COMPLEX_MAPPING(myclass, property_name) \
		{(PROP_MAP_CALL_IN_COMPLEX)&myclass::set_##property_name##_to_complex},

#define ON_SERIALIZE_OPEN_PROPERTY_IN_COMPLEX_MAPPING(myclass) \
		{(PROP_MAP_CALL_IN_COMPLEX)&myclass::set_open_properties_to_complex},

#define END_SERIALIZE_PROPERTY_IN_COMPLEX_MAPPING(myclass) \
		{(PROP_MAP_CALL_IN_COMPLEX)0} \
	}; \
	void myclass::map_complex_value_to_class_member(const shared_ptr<::odata::core::odata_complex_value>& pentity) \
	{ \
		for (size_t i = 0;; ++i) \
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
		for (size_t i = 0;; ++i) \
		{ \
			PROPMAP_ENTRY_IN_COMPLEX pentry = get_to_complex_map()[i]; \
			if (pentry.pfn == 0) \
			{ \
				break; \
			} \
			(this->*((PROP_MAP_CALL_IN_COMPLEX)pentry.pfn))(complex_value); \
		} \
	}

#define DECLARE_NAVIGATION_PROPERTY_IN_ENTITY_MAPPING(navigation_property_name, edm_name, type) \
public: \
	const std::shared_ptr<type>& get_##navigation_property_name() const { return navigation_property_name; } \
	void set_##navigation_property_name(const std::shared_ptr<type>& navigation_value) { navigation_property_name = navigation_value; } \
	::pplx::task<std::pair<std::shared_ptr<type>, ::odata::client::http_result>> load_##navigation_property_name(const std::shared_ptr<odata_query_builder>& builder = nullptr); \
protected: \
	std::shared_ptr<type> navigation_property_name; \
	void get_##navigation_property_name##_from_entity(const shared_ptr<::odata::core::odata_entity_value>& pentity); \
	void set_##navigation_property_name##_to_entity(const shared_ptr<::odata::core::odata_entity_value>& pentity);

#define IMPLEMENT_NAVIGATION_PROPERTY_IN_ENTITY_MAPPING(myclass, navigation_property_name, edm_name, type) \
	void myclass::get_##navigation_property_name##_from_entity(const shared_ptr<::odata::core::odata_entity_value>& pentity) \
	{ \
		std::shared_ptr<::odata::core::odata_value> property_value; \
		if (!pentity->get_property_value(_XPLATSTR(#edm_name), property_value) || !property_value) \
		{ \
			return; \
		} \
		if (property_value->get_value_type()->get_type_kind() == ::odata::edm::edm_type_kind_t::Entity) \
		{ \
			auto entity_value = std::dynamic_pointer_cast<::odata::core::odata_entity_value>(property_value); \
			navigation_property_name = create_instance_from_entity<type>(entity_value, m_service_context); \
		} \
	} \
	::pplx::task<std::pair<std::shared_ptr<type>, ::odata::client::http_result>> myclass::load_##navigation_property_name(const std::shared_ptr<odata_query_builder>& builder) \
	{ \
		if (m_service_context) \
		{\
		::odata::string_t path = m_service_context->get_relative_path(m_edit_link) + _XPLATSTR("/") + _XPLATSTR(#edm_name); \
			auto query = m_service_context->create_query<odata_singleton_query_executor<type>, odata_query_builder>(path); \
			if (builder) { query->set_query_builder(builder); } \
			return query->execute_query().then( \
				[this] (const std::pair<std::shared_ptr<type>, ::odata::client::http_result>& ret_value) -> std::pair<std::shared_ptr<type>, ::odata::client::http_result> \
				{ \
					navigation_property_name = ret_value.first; \
					return ret_value; \
				}); \
		}\
		return ::pplx::task_from_result(std::make_pair(std::shared_ptr<type>(), ::odata::client::http_result(::web::http::http_headers(), ::web::http::status_code(-1)))); \
	} \
	void myclass::set_##navigation_property_name##_to_entity(const shared_ptr<::odata::core::odata_entity_value>& pentity) \
	{ \
		if (!pentity || !navigation_property_name) \
		{ \
			return; \
		} \
		auto entity_type = std::dynamic_pointer_cast<::odata::edm::edm_entity_type>(pentity->get_value_type()); \
		if (!entity_type) \
		{ \
			return; \
		} \
		auto edm_property = entity_type->find_property(_XPLATSTR(#edm_name)); \
		if (!edm_property) \
		{ \
			return; \
		} \
		auto property_type = edm_property->get_property_type(); \
		auto navigation_value_type = std::dynamic_pointer_cast<::odata::edm::edm_navigation_type>(property_type); \
		if (!navigation_value_type) \
		{ \
			return; \
		} \
		if (!navigation_value_type->is_contained()) \
		{ \
			pentity->set_value(_XPLATSTR(#edm_name) + odata::core::odata_json_constants::PAYLOAD_ANNOTATION_BIND, m_service_context->get_relative_path(navigation_property_name->get_edit_link())); \
			return; \
		} \
		pentity->set_value(_XPLATSTR(#edm_name), navigation_property_name->to_value()); \
	}

#define DECLARE_NAVIGATION_PROPERTY_IN_COMPLEX_MAPPING(navigation_property_name, edm_name, type) \
public: \
	const std::shared_ptr<type>& get_##navigation_property_name() const { return navigation_property_name; } \
	void set_##navigation_property_name(const std::shared_ptr<type>& navigation_value) { navigation_property_name = navigation_value; } \
	::pplx::task<std::pair<std::shared_ptr<type>, ::odata::client::http_result>> load_##navigation_property_name(const std::shared_ptr<odata_query_builder>& builder = nullptr); \
protected: \
	std::shared_ptr<type> navigation_property_name; \
	void get_##navigation_property_name##_from_complex(const shared_ptr<::odata::core::odata_complex_value>& pcomplex); \
	void set_##navigation_property_name##_to_complex(const shared_ptr<::odata::core::odata_complex_value>& pcomplex);

#define IMPLEMENT_NAVIGATION_PROPERTY_IN_COMPLEX_MAPPING(myclass, navigation_property_name, edm_name, type) \
	void myclass::get_##navigation_property_name##_from_complex(const shared_ptr<::odata::core::odata_complex_value>& pcomplex) \
	{ \
		std::shared_ptr<::odata::core::odata_value> property_value; \
		if (!pcomplex->get_property_value(_XPLATSTR(#edm_name), property_value) || !property_value) \
		{ \
			return; \
		} \
		if (property_value->get_value_type()->get_type_kind() == ::odata::edm::edm_type_kind_t::Entity) \
		{ \
			auto entity_value = std::dynamic_pointer_cast<::odata::core::odata_entity_value>(property_value); \
			navigation_property_name = create_instance_from_entity<type>(entity_value, m_service_context); \
		} \
	} \
	::pplx::task<std::pair<std::shared_ptr<type>, ::odata::client::http_result>> myclass::load_##navigation_property_name(const std::shared_ptr<odata_query_builder>& builder) \
	{ \
		if (m_service_context) \
		{\
			::odata::string_t path = m_service_context->get_relative_path(m_edit_link) + _XPLATSTR("/") + _XPLATSTR(#edm_name); \
			auto query = m_service_context->create_query<odata_singleton_query_executor<type>, odata_query_builder>(path); \
			if (builder) { query->set_query_builder(builder); } \
			return query->execute_query().then( \
				[this] (const std::pair<std::shared_ptr<type>, ::odata::client::http_result>& ret_value) -> std::pair<std::shared_ptr<type>, ::odata::client::http_result> \
				{ \
					navigation_property_name = ret_value.first; \
					return ret_value; \
				}); \
		}\
		return ::pplx::task_from_result(std::make_pair(std::shared_ptr<type>(), ::odata::client::http_result(::web::http::http_headers(), ::web::http::status_code(-1)))); \
	} \
	void myclass::set_##navigation_property_name##_to_complex(const shared_ptr<::odata::core::odata_complex_value>& pcomplex) \
	{ \
		if (!pcomplex || !navigation_property_name) \
		{ \
			return; \
		} \
		auto entity_type = std::dynamic_pointer_cast<::odata::edm::edm_entity_type>(pcomplex->get_value_type()); \
		if (!entity_type) \
		{ \
			return; \
		} \
		auto edm_property = entity_type->find_property(_XPLATSTR(#edm_name)); \
		if (!edm_property) \
		{ \
			return; \
		} \
		auto property_type = edm_property->get_property_type(); \
		auto navigation_value_type = std::dynamic_pointer_cast<::odata::edm::edm_navigation_type>(property_type); \
		if (!navigation_value_type) \
		{ \
			return; \
		} \
		if (!navigation_value_type->is_contained()) \
		{ \
			pcomplex->set_value(_XPLATSTR(#edm_name) + odata::core::odata_json_constants::PAYLOAD_ANNOTATION_BIND, m_service_context->get_relative_path(navigation_property_name->get_edit_link())); \
			return; \
		} \
		pcomplex->set_value(_XPLATSTR(#edm_name), navigation_property_name->to_value()); \
	}

#define DECLARE_COLLECTION_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(property_name, edm_name, type) \
public: \
	const std::vector<type>& get_##property_name() const { return property_name; } \
	void set_##property_name(std::vector<type> values) { property_name = std::move(values); } \
	void add_to_##property_name(const type& property_value) { property_name.emplace_back(property_value); } \
protected: \
	std::vector<type> property_name; \
	void get_##property_name##_from_entity(const shared_ptr<::odata::core::odata_entity_value>& pentity); \
	void set_##property_name##_to_entity(const shared_ptr<::odata::core::odata_entity_value>& pentity);

#define IMPLEMENT_COLLECTION_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(myclass, property_name, edm_name, type) \
	void myclass::get_##property_name##_from_entity(const shared_ptr<::odata::core::odata_entity_value>& pentity) \
	{ \
		std::shared_ptr<::odata::core::odata_value> property_value; \
		if (!pentity->get_property_value(_XPLATSTR(#edm_name), property_value) || !property_value) \
		{ \
			return; \
		} \
		auto property_collection_value = static_pointer_cast<::odata::core::odata_collection_value>(property_value); \
		if (!property_collection_value) \
		{ \
			return; \
		} \
		for (auto iter = property_collection_value->get_collection_values().cbegin(); iter != property_collection_value->get_collection_values().cend(); ++iter) \
		{ \
			auto primitive_value = std::dynamic_pointer_cast<odata_primitive_value>(*iter); \
			if (!primitive_value) \
			{ \
				continue; \
			} \
			property_name.emplace_back(primitive_value->as<type>()); \
		} \
	} \
	void myclass::set_##property_name##_to_entity(const shared_ptr<::odata::core::odata_entity_value>& pentity) \
	{ \
		if (!pentity || !pentity->get_value_type()) \
		{ \
			return; \
		} \
		auto entity_type = std::dynamic_pointer_cast<::odata::edm::edm_entity_type>(pentity->get_value_type()); \
		if (!entity_type) \
		{ \
			return; \
		} \
		auto edm_property = entity_type->find_property(_XPLATSTR(#edm_name)); \
		if (!edm_property) \
		{ \
			return; \
		} \
		auto property_type = edm_property->get_property_type(); \
		auto collection_value_type = std::dynamic_pointer_cast<::odata::edm::edm_collection_type>(property_type); \
		if (!collection_value_type) \
		{ \
			return; \
		} \
		std::shared_ptr<::odata::core::odata_collection_value> collection_value = ::odata::make_shared<::odata::core::odata_collection_value>(collection_value_type); \
		for (size_t i = 0; i < property_name.size(); ++i) \
		{ \
			collection_value->add_collection_value(odata_primitive_value::make_primitive_value(property_name[i])); \
		} \
		pentity->set_value(_XPLATSTR(#edm_name), collection_value); \
	}

#define DECLARE_COLLECTION_PRIMITIVE_PROPERTY_IN_COMPLEX_MAPPING(property_name, edm_name, type) \
public: \
	const std::vector<type>& get_##property_name() const { return property_name; } \
	void set_##property_name(const std::vector<type>& values) { property_name = values; } \
	void add_to_##property_name(const type& property_value) { property_name.emplace_back(property_value); } \
protected: \
	std::vector<type> property_name; \
	void get_##property_name##_from_complex(const shared_ptr<::odata::core::odata_complex_value>& pcomplex); \
	void set_##property_name##_to_complex(const shared_ptr<::odata::core::odata_complex_value>& pcomplex);

#define IMPLEMENT_COLLECTION_PRIMITIVE_PROPERTY_IN_COMPLEX_MAPPING(myclass, property_name, edm_name, type) \
	void myclass::get_##property_name##_from_complex(const shared_ptr<::odata::core::odata_complex_value>& pcomplex) \
	{ \
		std::shared_ptr<::odata::core::odata_value> property_value; \
		if (!pcomplex->get_property_value(_XPLATSTR(#edm_name), property_value) || !property_value) \
		{ \
			return; \
		} \
		auto property_collection_value = static_pointer_cast<::odata::core::odata_collection_value>(property_value); \
		if (!property_collection_value) \
		{ \
			return; \
		} \
		for (auto iter = property_collection_value->get_collection_values().cbegin(); iter != property_collection_value->get_collection_values().cend(); ++iter) \
		{ \
			auto primitive_value = std::dynamic_pointer_cast<odata_primitive_value>(*iter); \
			if (!primitive_value) \
			{ \
				continue; \
			} \
			property_name.emplace_back(primitive_value->as<type>()); \
		} \
	} \
	void myclass::set_##property_name##_to_complex(const shared_ptr<::odata::core::odata_complex_value>& pcomplex) \
	{ \
		if (!pcomplex || !pcomplex->get_value_type()) \
		{ \
			return; \
		} \
		auto complex_type = std::dynamic_pointer_cast<::odata::edm::edm_complex_type>(pcomplex->get_value_type()); \
		if (!complex_type) \
		{ \
			return; \
		} \
		auto edm_property = complex_type->find_property(_XPLATSTR(#edm_name)); \
		if (!edm_property) \
		{ \
			return; \
		} \
		auto property_type = edm_property->get_property_type(); \
		auto collection_value_type = std::dynamic_pointer_cast<::odata::edm::edm_collection_type>(property_type); \
		if (!collection_value_type) \
		{ \
			return; \
		} \
		std::shared_ptr<::odata::core::odata_collection_value> collection_value = ::odata::make_shared<::odata::core::odata_collection_value>(collection_value_type); \
		for (size_t i = 0; i < property_name.size(); ++i) \
		{ \
			collection_value->add_collection_value(odata_primitive_value::make_primitive_value(property_name[i])); \
		} \
		pcomplex->set_value(_XPLATSTR(#edm_name), collection_value); \
	}

#define DECLARE_NULLABLE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(property_name, edm_name, type) \
public: \
	type* get_##property_name() const { return property_name.get(); } \
	void set_##property_name(type property_value) \
	{\
		property_name = ::odata::make_shared<type>(property_value); \
	} \
protected: \
	std::shared_ptr<type> property_name; \
	void get_##property_name##_from_entity(const shared_ptr<::odata::core::odata_entity_value>& pentity); \
	void set_##property_name##_to_entity(const shared_ptr<::odata::core::odata_entity_value>& pentity);

#define IMPLEMENT_NULLABLE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(myclass, property_name, edm_name, type) \
	void myclass::get_##property_name##_from_entity(const shared_ptr<::odata::core::odata_entity_value>& pentity) \
	{ \
		std::shared_ptr<::odata::core::odata_value> property_value; \
		if (!pentity->get_property_value(_XPLATSTR(#edm_name), property_value) || !property_value) \
		{ \
			return; \
		} \
		auto primitive_value = std::dynamic_pointer_cast<odata_primitive_value>(property_value); \
		if (primitive_value) \
		{ \
			property_name = ::odata::make_shared<type>(primitive_value->as<type>()); \
		} \
	} \
	void myclass::set_##property_name##_to_entity(const shared_ptr<::odata::core::odata_entity_value>& pentity) \
	{ \
		if (!pentity || !property_name) \
		{ \
			return; \
		} \
		pentity->set_value(_XPLATSTR(#edm_name), *property_name); \
	}

#define DECLARE_NULLABLE_PRIMITIVE_PROPERTY_IN_COMPLEX_MAPPING(property_name, edm_name, type) \
public: \
	type* get_##property_name() const { return property_name.get(); } \
	void set_##property_name(type property_value) \
	{ \
		property_name = ::odata::make_shared<type>(property_value); \
	} \
protected: \
	std::shared_ptr<type> property_name; \
	void get_##property_name##_from_complex(const shared_ptr<::odata::core::odata_complex_value>& pcomplex); \
	void set_##property_name##_to_complex(const shared_ptr<::odata::core::odata_complex_value>& pcomplex);

#define IMPLEMENT_NULLABLE_PRIMITIVE_PROPERTY_IN_COMPLEX_MAPPING(myclass, property_name, edm_name, type) \
	void myclass::get_##property_name##_from_complex(const shared_ptr<::odata::core::odata_complex_value>& pcomplex) \
	{ \
		std::shared_ptr<::odata::core::odata_value> property_value; \
		if (!pcomplex->get_property_value(_XPLATSTR(#edm_name), property_value) || !property_value) \
		{ \
			return; \
		} \
		auto primitive_value = std::dynamic_pointer_cast<odata_primitive_value>(property_value); \
		if (primitive_value) \
		{ \
			property_name = ::odata::make_shared<type>(primitive_value->as<type>()); \
		} \
	} \
	void myclass::set_##property_name##_to_complex(const shared_ptr<::odata::core::odata_complex_value>& pcomplex) \
	{ \
		if (!pcomplex || !property_name) \
		{ \
			return; \
		} \
		pcomplex->set_value(_XPLATSTR(#edm_name), *property_name); \
	}

#define DECLARE_NULLABLE_COMPLEX_PROPERTY_IN_ENTITY_MAPPING(property_name, edm_name, type) \
public: \
	type* get_##property_name() const { return property_name.get(); } \
	void set_##property_name(const std::shared_ptr<type>& property_value) { property_name = property_value; } \
protected: \
	std::shared_ptr<type> property_name; \
	void get_##property_name##_from_entity(const shared_ptr<::odata::core::odata_entity_value>& pentity); \
	void set_##property_name##_to_entity(const shared_ptr<::odata::core::odata_entity_value>& pentity);

#define IMPLEMENT_NULLABLE_COMPLEX_PROPERTY_IN_ENTITY_MAPPING(myclass, property_name, edm_name, type) \
	void myclass::get_##property_name##_from_entity(const shared_ptr<::odata::core::odata_entity_value>& pentity) \
	{ \
		std::shared_ptr<::odata::core::odata_value> property_value; \
		if (!pentity->get_property_value(_XPLATSTR(#edm_name), property_value) || !property_value) \
		{ \
			return; \
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
			return; \
		} \
		pentity->set_value(_XPLATSTR(#edm_name), property_name->to_value()); \
	}

#define DECLARE_NULLABLE_COMPLEX_PROPERTY_IN_COMPLEX_MAPPING(property_name, edm_name, type) \
public: \
	type* get_##property_name() const { return property_name.get(); } \
	void set_##property_name(const std::shared_ptr<type>& property_value) { property_name = property_value; } \
protected: \
	std::shared_ptr<type> property_name; \
	void get_##property_name##_from_complex(const shared_ptr<::odata::core::odata_complex_value>& pcomplex); \
	void set_##property_name##_to_complex(const shared_ptr<::odata::core::odata_complex_value>& pcomplex);

#define IMPLEMENT_NULLABLE_COMPLEX_PROPERTY_IN_COMPLEX_MAPPING(myclass, property_name, edm_name, type) \
	void myclass::get_##property_name##_from_complex(const shared_ptr<::odata::core::odata_complex_value>& pcomplex) \
	{ \
		std::shared_ptr<::odata::core::odata_value> property_value; \
		if (!pcomplex->get_property_value(_XPLATSTR(#edm_name), property_value) || !property_value) \
		{ \
			return; \
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
			return; \
		} \
		pcomplex->set_value(_XPLATSTR(#edm_name), property_name->to_value()); \
	}

// because we have to support derived complex type, so complex object in entity are all set with pointer type
#define DECLARE_COMPLEX_PROPERTY_IN_ENTITY_MAPPING(property_name, edm_name, type) \
	DECLARE_NULLABLE_COMPLEX_PROPERTY_IN_ENTITY_MAPPING(property_name, edm_name, type)

#define IMPLEMENT_COMPLEX_PROPERTY_IN_ENTITY_MAPPING(myclass, property_name, edm_name, type) \
	IMPLEMENT_NULLABLE_COMPLEX_PROPERTY_IN_ENTITY_MAPPING(myclass, property_name, edm_name, type)

#define DECLARE_COMPLEX_PROPERTY_IN_COMPLEX_MAPPING(property_name, edm_name, type) \
	DECLARE_NULLABLE_COMPLEX_PROPERTY_IN_COMPLEX_MAPPING(property_name, edm_name, type)

#define IMPLEMENT_COMPLEX_PROPERTY_IN_COMPLEX_MAPPING(myclass, property_name, edm_name, type) \
	IMPLEMENT_NULLABLE_COMPLEX_PROPERTY_IN_COMPLEX_MAPPING(myclass, property_name, edm_name, type)

#define DECLARE_OPEN_PROPERTY_DICTIONARY_IN_ENTITY_MAPPING() \
public: \
	std::unordered_map<::odata::string_t, shared_ptr<::odata::string_t>> const& get_open_properties() const \
	{ \
		return open_property_dictionary; \
	} \
	void set_open_properties(std::unordered_map<::odata::string_t, shared_ptr<::odata::string_t>> const& properties) \
	{ \
		open_property_dictionary = properties; \
	} \
	shared_ptr<::odata::string_t> const get_open_property_from_dictionary(const ::odata::string_t& property_name) const; \
	void set_open_property_to_dictionary(const ::odata::string_t& property_name, const ::odata::string_t& property_value) \
	{\
		open_property_dictionary[property_name] = ::odata::make_shared<::odata::string_t>(property_value); \
	} \
	size_t delete_open_property_from_dictionary(const ::odata::string_t& property_name) \
	{ \
		return open_property_dictionary.erase(property_name); \
	} \
protected: \
	std::unordered_map<::odata::string_t, shared_ptr<::odata::string_t>> open_property_dictionary; \
	void get_open_properties_from_entity(const shared_ptr<::odata::core::odata_entity_value>& pentity); \
	void set_open_properties_to_entity(const shared_ptr<::odata::core::odata_entity_value>& pentity);

#define IMPLEMENT_OPEN_PROPERTY_DICTIONARY_IN_ENTITY_MAPPING(myclass) \
shared_ptr<::odata::string_t> const myclass::get_open_property_from_dictionary(const ::odata::string_t& property_name) const \
{ \
	auto iter = open_property_dictionary.find(property_name); \
	return iter != open_property_dictionary.end() ? iter->second : nullptr; \
} \
void myclass::get_open_properties_from_entity(const shared_ptr<::odata::core::odata_entity_value>& pentity) \
{ \
	if (pentity->is_open()) \
	{ \
		for (auto const &open_property_value : pentity->open_properties()) \
		{ \
			if (!open_property_value.second || pentity->has_property(open_property_value.first)) \
			{ \
				continue; \
			} \
			auto primitive_value = std::dynamic_pointer_cast<odata_primitive_value>(open_property_value.second); \
			if (primitive_value) \
			{ \
				open_property_dictionary[open_property_value.first] = ::odata::make_shared<::odata::string_t>(primitive_value->as<::odata::string_t>()); \
			} \
		} \
	} \
} \
void myclass::set_open_properties_to_entity(const shared_ptr<::odata::core::odata_entity_value>& pentity) \
{ \
	if (pentity->is_open()) \
	{ \
		for (auto const &open_property_value : open_property_dictionary) \
		{ \
			if (!open_property_value.second || pentity->has_property(open_property_value.first)) \
			{ \
				continue; \
			} \
			pentity->set_open_value(open_property_value.first, *open_property_value.second); \
		} \
	} \
} \

#define DECLARE_OPEN_PROPERTY_DICTIONARY_IN_COMPLEX_MAPPING() \
public: \
	std::unordered_map<::odata::string_t, shared_ptr<::odata::string_t>> const& get_open_properties() const \
	{ \
		return open_property_dictionary; \
	} \
	void set_open_properties(std::unordered_map<::odata::string_t, shared_ptr<::odata::string_t>> const& properties) \
	{ \
		open_property_dictionary = properties; \
	} \
	shared_ptr<::odata::string_t> const get_open_property_from_dictionary(const ::odata::string_t& property_name) const; \
	void set_open_property_to_dictionary(const ::odata::string_t& property_name, const ::odata::string_t& property_value) \
	{ \
		open_property_dictionary[property_name] = ::odata::make_shared<::odata::string_t>(property_value); \
	} \
	size_t delete_open_property_from_dictionary(const ::odata::string_t& property_name) \
	{ \
		return open_property_dictionary.erase(property_name); \
	} \
protected: \
	std::unordered_map<::odata::string_t, shared_ptr<::odata::string_t>> open_property_dictionary; \
	void get_open_properties_from_complex(const shared_ptr<::odata::core::odata_complex_value>& pcomplex); \
	void set_open_properties_to_complex(const shared_ptr<::odata::core::odata_complex_value>& pcomplex);

#define IMPLEMENT_OPEN_PROPERTY_DICTIONARY_IN_COMPLEX_MAPPING(myclass) \
shared_ptr<::odata::string_t> const myclass::get_open_property_from_dictionary(const ::odata::string_t& property_name) const \
{ \
	auto iter = open_property_dictionary.find(property_name); \
	return iter != open_property_dictionary.end() ? iter->second : nullptr; \
} \
void myclass::get_open_properties_from_complex(const shared_ptr<::odata::core::odata_complex_value>& pcomplex) \
{ \
	if (pcomplex->is_open()) \
	{ \
		for (auto const &open_property_value : pcomplex->open_properties()) \
		{ \
			if (!open_property_value.second || pcomplex->has_property(open_property_value.first)) \
			{ \
				continue; \
			} \
			auto primitive_value = std::dynamic_pointer_cast<odata_primitive_value>(open_property_value.second); \
			if (primitive_value) \
			{ \
				open_property_dictionary[open_property_value.first] = ::odata::make_shared<::odata::string_t>(primitive_value->as<::odata::string_t>()); \
			} \
		} \
	} \
} \
void myclass::set_open_properties_to_complex(const shared_ptr<::odata::core::odata_complex_value>& pcomplex) \
{ \
	if (pcomplex->is_open()) \
	{ \
		for (auto const &open_property_value : open_property_dictionary) \
		{ \
			if (!open_property_value.second || pcomplex->has_property(open_property_value.first)) \
			{ \
				continue; \
			} \
			pcomplex->set_open_value(open_property_value.first, *open_property_value.second); \
		} \
	} \
}

#define DECLARE_COLLECTION_NAVIGATION_PROPERTY_IN_ENTITY_MAPPING(property_name, edm_name, type) \
public: \
	const std::vector<std::shared_ptr<type>>& get_##property_name() const { return property_name; } \
	void set_##property_name(const std::vector<std::shared_ptr<type>>& property_values) { property_name = property_values; } \
	void add_to_##property_name(const std::shared_ptr<type>& property_value) { property_name.emplace_back(property_value); } \
	::pplx::task<std::pair<std::vector<std::shared_ptr<type>>, ::odata::client::http_result>> load_##property_name(const std::shared_ptr<odata_query_builder>& builder = nullptr); \
protected: \
	std::vector<std::shared_ptr<type>> property_name; \
	void get_##property_name##_from_entity(const shared_ptr<::odata::core::odata_entity_value>& pentity); \
	void set_##property_name##_to_entity(const shared_ptr<::odata::core::odata_entity_value>& pentity);

#define IMPLEMENT_COLLECTION_NAVIGATION_PROPERTY_IN_ENTITY_MAPPING(myclass, property_name, edm_name, type) \
	void myclass::get_##property_name##_from_entity(const shared_ptr<::odata::core::odata_entity_value>& pentity) \
	{ \
		std::shared_ptr<::odata::core::odata_value> property_value; \
		if (!pentity->get_property_value(_XPLATSTR(#edm_name), property_value) || !property_value) \
		{ \
			return; \
		} \
		auto property_collection_value = static_pointer_cast<::odata::core::odata_collection_value>(property_value); \
		if (!property_collection_value) \
		{ \
			return; \
		} \
		for (auto iter = property_collection_value->get_collection_values().cbegin(); iter != property_collection_value->get_collection_values().cend(); ++iter) \
		{ \
			auto entity_value = std::dynamic_pointer_cast<::odata::core::odata_entity_value>(*iter); \
			if (!entity_value) \
			{ \
				continue; \
			} \
			std::shared_ptr<type> p_entity = create_instance_from_entity<type>(entity_value, m_service_context); \
			property_name.emplace_back(std::dynamic_pointer_cast<type>(p_entity)); \
		} \
	} \
	::pplx::task<std::pair<std::vector<std::shared_ptr<type>>, ::odata::client::http_result>> myclass::load_##property_name(const std::shared_ptr<odata_query_builder>& builder) \
	{ \
		if (m_service_context)\
		{\
			::odata::string_t path = m_service_context->get_relative_path(m_edit_link) + _XPLATSTR("/") + _XPLATSTR(#edm_name);\
			auto query = m_service_context->create_query<odata_entityset_query_executor<type>, odata_query_builder>(path);\
			if (builder) { query->set_query_builder(builder); } \
			return query->execute_query().then( \
				[this] (const std::pair<std::vector<std::shared_ptr<type>>, ::odata::client::http_result>& ret_values) -> std::pair<std::vector<std::shared_ptr<type>>, ::odata::client::http_result> \
				{ \
					property_name = ret_values.first; \
					return ret_values; \
				}); \
		} \
		return ::pplx::task_from_result(std::make_pair(std::vector<std::shared_ptr<type>>(), ::odata::client::http_result(::web::http::http_headers(), ::web::http::status_code(-1)))); \
	} \
	void myclass::set_##property_name##_to_entity(const shared_ptr<::odata::core::odata_entity_value>& pentity) \
	{ \
		if (!pentity || !pentity->get_value_type()) \
		{ \
			return; \
		} \
		auto entity_type = std::dynamic_pointer_cast<::odata::edm::edm_entity_type>(pentity->get_value_type()); \
		if (!entity_type) \
		{ \
			return; \
		} \
		auto edm_property = entity_type->find_property(_XPLATSTR(#edm_name)); \
		if (!edm_property) \
		{ \
			return; \
		} \
		auto property_type = edm_property->get_property_type(); \
		auto navigation_value_type = std::dynamic_pointer_cast<::odata::edm::edm_navigation_type>(property_type); \
		if (!navigation_value_type) \
		{ \
			return; \
		} \
		if (!navigation_value_type->is_contained()) \
		{ \
			::odata::string_t const BindName = _XPLATSTR(#edm_name) + odata::core::odata_json_constants::PAYLOAD_ANNOTATION_BIND; \
			auto PrimitiveType = ::odata::make_shared<::odata::edm::edm_named_type>(BindName, entity_type->get_namespace(), odata::edm::edm_type_kind_t::Primitive); \
			auto collection_value_type = ::odata::make_shared<::odata::edm::edm_collection_type>(BindName, PrimitiveType); \
			if (!collection_value_type) \
			{ \
				return; \
			} \
			std::shared_ptr<::odata::core::odata_collection_value> collection_value = ::odata::make_shared<::odata::core::odata_collection_value>(collection_value_type); \
			for (size_t i = 0; i < property_name.size(); ++i) \
			{ \
				if (property_name[i]) collection_value->add_collection_value(std::dynamic_pointer_cast<::odata::core::odata_value>(odata::core::odata_primitive_value::make_primitive_value(m_service_context->get_relative_path(property_name[i]->get_edit_link())))); \
			} \
			pentity->set_value(BindName, collection_value); \
			return; \
		} \
		auto navigation_type = std::dynamic_pointer_cast<::odata::edm::edm_named_type>(navigation_value_type->get_navigation_type()); \
		if (!navigation_type) \
		{ \
			return; \
		} \
		auto collection_value_type = std::dynamic_pointer_cast<::odata::edm::edm_collection_type>(navigation_type); \
		if (!collection_value_type) \
		{ \
			return; \
		} \
		std::shared_ptr<::odata::core::odata_collection_value> collection_value = ::odata::make_shared<::odata::core::odata_collection_value>(collection_value_type); \
		for (size_t i = 0; i < property_name.size(); ++i) \
		{ \
			if (property_name[i]) collection_value->add_collection_value(std::dynamic_pointer_cast<::odata::core::odata_value>(property_name[i]->to_value())); \
		} \
		pentity->set_value(_XPLATSTR(#edm_name), collection_value); \
	}

#define DECLARE_COLLECTION_NAVIGATION_PROPERTY_IN_COMPLEX_MAPPING(property_name, edm_name, type) \
public: \
	const std::vector<std::shared_ptr<type>>& get_##property_name() const { return property_name; } \
	void set_##property_name(const std::vector<std::shared_ptr<type>>& property_values) { property_name = property_values; } \
	void add_to_##property_name(const std::shared_ptr<type>& property_value) { property_name.emplace_back(property_value); } \
	::pplx::task<std::pair<std::vector<std::shared_ptr<type>>, ::odata::client::http_result>> load_##property_name(const std::shared_ptr<odata_query_builder>& builder = nullptr); \
protected: \
	std::vector<std::shared_ptr<type>> property_name; \
	void get_##property_name##_from_complex(const shared_ptr<::odata::core::odata_complex_value>& pcomplex); \
	void set_##property_name##_to_complex(const shared_ptr<::odata::core::odata_complex_value>& pcomplex);

#define IMPLEMENT_COLLECTION_NAVIGATION_PROPERTY_IN_COMPLEX_MAPPING(myclass, property_name, edm_name, type) \
	void myclass::get_##property_name##_from_complex(const shared_ptr<::odata::core::odata_complex_value>& pcomplex) \
	{ \
		std::shared_ptr<::odata::core::odata_value> property_value; \
		if (!pcomplex->get_property_value(_XPLATSTR(#edm_name), property_value) || !property_value) \
		{ \
			return; \
		} \
		auto property_collection_value = static_pointer_cast<::odata::core::odata_collection_value>(property_value); \
		if (!property_collection_value) \
		{ \
			return; \
		} \
		for (auto iter = property_collection_value->get_collection_values().cbegin(); iter != property_collection_value->get_collection_values().cend(); ++iter) \
		{ \
			auto entity_value = std::dynamic_pointer_cast<::odata::core::odata_entity_value>(*iter); \
			if (!entity_value) \
			{ \
				continue; \
			} \
			std::shared_ptr<type> p_entity = create_instance_from_entity<type>(entity_value, m_service_context); \
			property_name.emplace_back(std::dynamic_pointer_cast<type>(p_entity)); \
		 } \
	} \
	::pplx::task<std::pair<std::vector<std::shared_ptr<type>>, ::odata::client::http_result>> myclass::load_##property_name(const std::shared_ptr<odata_query_builder>& builder) \
	{ \
		if (m_service_context)\
		{\
			::odata::string_t path = m_service_context->get_relative_path(m_edit_link) + _XPLATSTR("/") + _XPLATSTR(#edm_name);\
			auto query = m_service_context->create_query<odata_entityset_query_executor<type>, odata_query_builder>(path);\
			if (builder) { query->set_query_builder(builder); } \
			return query->execute_query().then( \
				[this] (const std::pair<std::vector<std::shared_ptr<type>>, ::odata::client::http_result>& ret_values) -> std::pair<std::vector<std::shared_ptr<type>>, ::odata::client::http_result> \
				{ \
					property_name = ret_values.first; \
					return ret_values; \
				}); \
		} \
		return ::pplx::task_from_result(std::make_pair(std::vector<std::shared_ptr<type>>(), ::odata::client::http_result(::web::http::http_headers(), ::web::http::status_code(-1)))); \
	} \
	void myclass::set_##property_name##_to_complex(const shared_ptr<::odata::core::odata_complex_value>& pcomplex) \
	{ \
		if (!pcomplex || !pcomplex->get_value_type()) \
		{ \
			return; \
		} \
		auto complex_type = std::dynamic_pointer_cast<::odata::edm::edm_complex_type>(pcomplex->get_value_type()); \
		if (!complex_type) \
		{ \
			return; \
		} \
		auto edm_property = complex_type->find_property(_XPLATSTR(#edm_name)); \
		if (!edm_property) \
		{ \
			return; \
		} \
		auto property_type = edm_property->get_property_type(); \
		auto navigation_value_type = std::dynamic_pointer_cast<::odata::edm::edm_navigation_type>(property_type); \
		if (!navigation_value_type) \
		{ \
			return; \
		} \
		if (!navigation_value_type->is_contained()) \
		{ \
			::odata::string_t const BindName = _XPLATSTR(#edm_name) + odata::core::odata_json_constants::PAYLOAD_ANNOTATION_BIND; \
			auto PrimitiveType = ::odata::make_shared<::odata::edm::edm_named_type>(BindName, complex_type->get_namespace(), odata::edm::edm_type_kind_t::Primitive); \
			auto collection_value_type = ::odata::make_shared<::odata::edm::edm_collection_type>(BindName, PrimitiveType); \
			if (!collection_value_type) \
			{ \
				return; \
			} \
			std::shared_ptr<::odata::core::odata_collection_value> collection_value = ::odata::make_shared<::odata::core::odata_collection_value>(collection_value_type); \
			for (size_t i = 0; i < property_name.size(); ++i) \
			{ \
				if (property_name[i]) collection_value->add_collection_value(std::dynamic_pointer_cast<::odata::core::odata_value>(odata::core::odata_primitive_value::make_primitive_value(m_service_context->get_relative_path(property_name[i]->get_edit_link())))); \
			} \
			pcomplex->set_value(BindName, collection_value); \
			return; \
		} \
		auto navigation_type = std::dynamic_pointer_cast<::odata::edm::edm_named_type>(navigation_value_type->get_navigation_type()); \
		if (!navigation_type) \
		{ \
			return; \
		} \
		auto collection_value_type = std::dynamic_pointer_cast<::odata::edm::edm_collection_type>(navigation_type); \
		if (!collection_value_type) \
		{ \
			return; \
		} \
		std::shared_ptr<::odata::core::odata_collection_value> collection_value = ::odata::make_shared<::odata::core::odata_collection_value>(collection_value_type); \
		for (size_t i = 0; i < property_name.size(); ++i) \
		{ \
			if (property_name[i]) collection_value->add_collection_value(std::dynamic_pointer_cast<odata_complex_value>(property_name[i]->to_value())); \
		} \
		pcomplex->set_value(_XPLATSTR(#edm_name), collection_value); \
	}

#define DECLARE_COLLECTION_COMPLEX_PROPERTY_IN_ENTITY_MAPPING(property_name, edm_name, type) \
public: \
	const std::vector<std::shared_ptr<type>>& get_##property_name() const { return property_name; } \
	void set_##property_name(std::vector<std::shared_ptr<type>> property_values) { property_name = std::move(property_values); } \
	void add_to_##property_name(const std::shared_ptr<type>& property_value) { property_name.emplace_back(property_value); } \
protected: \
	std::vector<std::shared_ptr<type>> property_name; \
	void get_##property_name##_from_entity(const shared_ptr<::odata::core::odata_entity_value>& pentity); \
	void set_##property_name##_to_entity(const shared_ptr<::odata::core::odata_entity_value>& pentity);

#define IMPLEMENT_COLLECTION_COMPLEX_PROPERTY_IN_ENTITY_MAPPING(myclass, property_name, edm_name, type) \
	void myclass::get_##property_name##_from_entity(const shared_ptr<::odata::core::odata_entity_value>& pentity) \
	{ \
		std::shared_ptr<::odata::core::odata_value> property_value; \
		if (!pentity->get_property_value(_XPLATSTR(#edm_name), property_value) || !property_value) \
		{ \
			return; \
		} \
		auto property_collection_value = static_pointer_cast<::odata::core::odata_collection_value>(property_value); \
		if (!property_collection_value) \
		{ \
			return; \
		} \
		for (auto iter = property_collection_value->get_collection_values().cbegin(); iter != property_collection_value->get_collection_values().cend(); ++iter) \
		{ \
			auto complex_value = std::dynamic_pointer_cast<::odata::core::odata_complex_value>(*iter); \
			if (!complex_value) \
			{ \
				continue; \
			} \
			property_name.emplace_back(create_instance_from_complex<type>(complex_value, m_service_context)); \
		} \
	} \
	void myclass::set_##property_name##_to_entity(const shared_ptr<::odata::core::odata_entity_value>& pentity) \
	{ \
		if (!pentity || !pentity->get_value_type()) \
		{ \
			return; \
		} \
		auto entity_type = std::dynamic_pointer_cast<::odata::edm::edm_entity_type>(pentity->get_value_type()); \
		if (!entity_type) \
		{ \
			return; \
		} \
		auto edm_property = entity_type->find_property(_XPLATSTR(#edm_name)); \
		if (!edm_property) \
		{ \
			return; \
		} \
		auto property_type = edm_property->get_property_type(); \
		auto collection_value_type = std::dynamic_pointer_cast<::odata::edm::edm_collection_type>(property_type); \
		if (!collection_value_type) \
		{ \
			return; \
		} \
		std::shared_ptr<::odata::core::odata_collection_value> collection_value = ::odata::make_shared<::odata::core::odata_collection_value>(collection_value_type); \
		for (size_t i = 0; i < property_name.size(); ++i) \
		{ \
			if (property_name[i]) collection_value->add_collection_value(std::dynamic_pointer_cast<::odata::core::odata_value>(property_name[i]->to_value())); \
		} \
		pentity->set_value(_XPLATSTR(#edm_name), collection_value); \
	}

#define DECLARE_COLLECTION_COMPLEX_PROPERTY_IN_COMPLEX_MAPPING(property_name, edm_name, type) \
public: \
	const std::vector<std::shared_ptr<type>>& get_##property_name() const { return property_name; } \
	void set_##property_name(std::vector<std::shared_ptr<type>> property_values) { property_name = std::move(property_values); } \
	void add_to_##property_name(const std::shared_ptr<type>& property_value) { property_name.emplace_back(property_value); } \
protected: \
	std::vector<std::shared_ptr<type>> property_name; \
	void get_##property_name##_from_complex(const shared_ptr<::odata::core::odata_complex_value>& pcomplex); \
	void set_##property_name##_to_complex(const shared_ptr<::odata::core::odata_complex_value>& pcomplex);

#define IMPLEMENT_COLLECTION_COMPLEX_PROPERTY_IN_COMPLEX_MAPPING(myclass, property_name, edm_name, type) \
	void myclass::get_##property_name##_from_complex(const shared_ptr<::odata::core::odata_complex_value>& pcomplex) \
	{ \
		std::shared_ptr<::odata::core::odata_value> property_value; \
		if (!pcomplex->get_property_value(_XPLATSTR(#edm_name), property_value) || !property_value) \
		{ \
			return; \
		} \
		auto property_collection_value = static_pointer_cast<::odata::core::odata_collection_value>(property_value); \
		if (!property_collection_value) \
		{ \
			return; \
		} \
		for (auto iter = property_collection_value->get_collection_values().cbegin(); iter != property_collection_value->get_collection_values().cend(); ++iter) \
		{ \
			auto complex_value = std::dynamic_pointer_cast<::odata::core::odata_complex_value>(*iter); \
			if (!complex_value) \
			{ \
				continue; \
			} \
			property_name.emplace_back(create_instance_from_complex<type>(complex_value, m_service_context)); \
		} \
	} \
	void myclass::set_##property_name##_to_complex(const shared_ptr<::odata::core::odata_complex_value>& pcomplex) \
	{ \
		if (!pcomplex || !pcomplex->get_value_type()) \
		{ \
			return; \
		} \
		auto entity_type = std::dynamic_pointer_cast<::odata::edm::edm_entity_type>(pcomplex->get_value_type()); \
		if (!entity_type) \
		{ \
			return; \
		} \
		auto edm_property = entity_type->find_property(_XPLATSTR(#edm_name)); \
		if (!edm_property) \
		{ \
			return; \
		} \
		auto property_type = edm_property->get_property_type(); \
		auto collection_value_type = std::dynamic_pointer_cast<::odata::edm::edm_collection_type>(property_type); \
		if (!collection_value_type) \
		{ \
			return; \
		} \
		std::shared_ptr<::odata::core::odata_collection_value> collection_value = ::odata::make_shared<::odata::core::odata_collection_value>(collection_value_type); \
		for (size_t i = 0; i < property_name.size(); ++i) \
		{ \
			if (property_name[i]) collection_value->add_collection_value(std::dynamic_pointer_cast<::odata::core::odata_value>(property_name[i]->to_value())); \
		} \
		pcomplex->set_value(_XPLATSTR(#edm_name), collection_value); \
	}

#define DECLARE_COLLECTION_ENTITY_PROPERTY_IN_ENTITY_MAPPING(property_name, edm_name, type) \
public: \
	const std::vector<std::shared_ptr<type>>& get_##property_name() const { return property_name; } \
	void set_##property_name(std::vector<std::shared_ptr<type>> property_values) { property_name = std::move(property_values); } \
	void add_to_##property_name(const std::shared_ptr<type>& property_value) { property_name.emplace_back(property_value); } \
protected: \
	std::vector<std::shared_ptr<type>> property_name; \
	void get_##property_name##_from_entity(const shared_ptr<::odata::core::odata_entity_value>& pentity); \
	void set_##property_name##_to_entity(const shared_ptr<::odata::core::odata_entity_value>& pentity);

#define IMPLEMENT_COLLECTION_ENTITY_PROPERTY_IN_ENTITY_MAPPING(myclass, property_name, edm_name, type) \
	void myclass::get_##property_name##_from_entity(const shared_ptr<::odata::core::odata_entity_value>& pentity) \
	{ \
		std::shared_ptr<::odata::core::odata_value> property_value; \
		if (!pentity->get_property_value(_XPLATSTR(#edm_name), property_value) || !property_value) \
		{ \
			return; \
		} \
		auto property_collection_value = static_pointer_cast<::odata::core::odata_collection_value>(property_value); \
		if (!property_collection_value) \
		{ \
			return; \
		} \
		for (auto iter = property_collection_value->get_collection_values().cbegin(); iter != property_collection_value->get_collection_values().cend(); ++iter) \
		{ \
			auto entity_value = std::dynamic_pointer_cast<::odata::core::odata_entity_value>(*iter); \
			if (!entity_value) \
			{ \
				continue; \
			} \
			property_name.emplace_back(create_instance_from_entity<type>(entity_value, m_service_context)); \
		} \
	} \
	void myclass::set_##property_name##_to_entity(const shared_ptr<::odata::core::odata_entity_value>& pentity) \
	{ \
		if (!pentity || !pentity->get_value_type()) \
		{ \
			return; \
		} \
		auto entity_type = std::dynamic_pointer_cast<::odata::edm::edm_entity_type>(pentity->get_value_type()); \
		if (!entity_type) \
		{ \
			return; \
		} \
		auto edm_property = entity_type->find_property(_XPLATSTR(#edm_name)); \
		if (!edm_property) \
		{ \
			return; \
		} \
		auto property_type = edm_property->get_property_type(); \
		auto collection_value_type = std::dynamic_pointer_cast<::odata::edm::edm_collection_type>(property_type); \
		if (!collection_value_type) \
		{ \
			return; \
		} \
		std::shared_ptr<::odata::core::odata_collection_value> collection_value = ::odata::make_shared<::odata::core::odata_collection_value>(collection_value_type); \
		for (size_t i = 0; i < property_name.size(); ++i) \
		{ \
			if (property_name[i]) collection_value->add_collection_value(std::dynamic_pointer_cast<::odata::core::odata_value>(property_name[i]->to_value())); \
		} \
		pentity->set_value(_XPLATSTR(#edm_name), collection_value); \
	}

#define DECLARE_COLLECTION_ENTITY_PROPERTY_IN_COMPLEX_MAPPING(property_name, edm_name, type) \
public: \
	const std::vector<std::shared_ptr<type>>& get_##property_name() const { return property_name; } \
	void set_##property_name(std::vector<std::shared_ptr<type>> property_values) { property_name = std::move(property_values); } \
	void add_to_##property_name(const std::shared_ptr<type>& property_value) { property_name.emplace_back(property_value); } \
protected: \
	std::vector<std::shared_ptr<type>> property_name; \
	void get_##property_name##_from_complex(const shared_ptr<::odata::core::odata_complex_value>& pcomplex); \
	void set_##property_name##_to_complex(const shared_ptr<::odata::core::odata_complex_value>& pcomplex);

#define IMPLEMENT_COLLECTION_ENTITY_PROPERTY_IN_COMPLEX_MAPPING(myclass, property_name, edm_name, type) \
	void myclass::get_##property_name##_from_complex(const shared_ptr<::odata::core::odata_complex_value>& pcomplex) \
	{ \
		std::shared_ptr<::odata::core::odata_value> property_value; \
		if (!pcomplex->get_property_value(_XPLATSTR(#edm_name), property_value) || !property_value) \
		{ \
			return; \
		} \
		auto property_collection_value = static_pointer_cast<::odata::core::odata_collection_value>(property_value); \
		if (!property_collection_value) \
		{ \
			return; \
		} \
		for (auto iter = property_collection_value->get_collection_values().cbegin(); iter != property_collection_value->get_collection_values().cend(); ++iter) \
		{ \
			auto entity_value = std::dynamic_pointer_cast<::odata::core::odata_entity_value>(*iter); \
			if (!entity_value) \
			{ \
				continue; \
			} \
			property_name.emplace_back(create_instance_from_entity<type>(entity_value, m_service_context)); \
		} \
	} \
	void myclass::set_##property_name##_to_complex(const shared_ptr<::odata::core::odata_complex_value>& pcomplex) \
	{ \
		if (!pcomplex || !pcomplex->get_value_type()) \
		{ \
			return; \
		} \
		auto complex_type = std::dynamic_pointer_cast<::odata::edm::edm_complex_type>(pcomplex->get_value_type()); \
		if (!complex_type) \
		{ \
			return; \
		} \
		auto edm_property = complex_type->find_property(_XPLATSTR(#edm_name)); \
		if (!edm_property) \
		{ \
			return; \
		} \
		auto property_type = edm_property->get_property_type(); \
		auto collection_value_type = std::dynamic_pointer_cast<::odata::edm::edm_collection_type>(property_type); \
		if (!collection_value_type) \
		{ \
			return; \
		} \
		std::shared_ptr<::odata::core::odata_collection_value> collection_value = ::odata::make_shared<::odata::core::odata_collection_value>(collection_value_type); \
		for (size_t i = 0; i < property_name.size(); ++i) \
		{ \
			if (property_name[i]) collection_value->add_collection_value(std::dynamic_pointer_cast<::odata::core::odata_value>(property_name[i]->to_value())); \
		} \
		pcomplex->set_value(_XPLATSTR(#edm_name), collection_value); \
	}

#define DECLARE_ENUM_PROPERTY_IN_ENTITY_MAPPING(property_name, edm_name, type) \
public: \
	const type& get_##property_name() const { return property_name; } \
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
			return; \
		} \
		std::shared_ptr<::odata::core::odata_value> property_value; \
		if (!pentity->get_property_value(_XPLATSTR(#edm_name), property_value) || !property_value) \
		{ \
			return; \
		} \
		auto enum_value = std::dynamic_pointer_cast<::odata::core::odata_enum_value>(property_value); \
		if (enum_value) \
		{ \
			property_name = enum_type_resolver::get_enum_type_from_string(enum_value->to_string(), type()); \
		} \
	} \
	void myclass::set_##property_name##_to_entity(const shared_ptr<::odata::core::odata_entity_value>& pentity) \
	{ \
		if (!pentity) \
		{ \
			return; \
		} \
		auto entity_type = std::dynamic_pointer_cast<::odata::edm::edm_entity_type>(pentity->get_value_type()); \
		if (!entity_type) \
		{ \
			return; \
		} \
		auto edm_property = entity_type->find_property(_XPLATSTR(#edm_name)); \
		if (!edm_property) \
		{ \
			return; \
		} \
		auto property_type = edm_property->get_property_type(); \
		auto enum_value = ::odata::make_shared<::odata::core::odata_enum_value>(property_type, enum_type_resolver::get_string_from_enum_type(property_name)); \
		pentity->set_value(_XPLATSTR(#edm_name), enum_value); \
	}

#define DECLARE_ENUM_PROPERTY_IN_COMPLEX_MAPPING(property_name, edm_name, type) \
public: \
	const type& get_##property_name() const { return property_name; } \
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
			return; \
		} \
		std::shared_ptr<::odata::core::odata_value> property_value; \
		if (!pcomplex->get_property_value(_XPLATSTR(#edm_name), property_value) || !property_value) \
		{ \
			return; \
		} \
		auto enum_value = std::dynamic_pointer_cast<::odata::core::odata_enum_value>(property_value); \
		if (enum_value) \
		{ \
			property_name = enum_type_resolver::get_enum_type_from_string(enum_value->to_string(), type()); \
		} \
	} \
	void myclass::set_##property_name##_to_complex(const shared_ptr<::odata::core::odata_complex_value>& pcomplex) \
	{ \
		if (!pcomplex) \
		{ \
			return; \
		} \
		auto complex_type = std::dynamic_pointer_cast<::odata::edm::edm_complex_type>(pcomplex->get_value_type()); \
		if (!complex_type) \
		{ \
			return; \
		} \
		auto edm_property = complex_type->find_property(_XPLATSTR(#edm_name)); \
		if (!edm_property) \
		{ \
			return; \
		} \
		auto property_type = edm_property->get_property_type(); \
		auto enum_value = ::odata::make_shared<::odata::core::odata_enum_value>(property_type, enum_type_resolver::get_string_from_enum_type(property_name)); \
		pcomplex->set_value(_XPLATSTR(#edm_name), enum_value); \
	}

#define DECLARE_COLLECTION_ENUM_PROPERTY_IN_ENTITY_MAPPING(property_name, edm_name, type) \
public: \
	const std::vector<type>& get_##property_name() const { return property_name; } \
	void set_##property_name(const std::vector<type>& property_values) { property_name = property_values; } \
	void add_to_##property_name(type property_value) { property_name.emplace_back(property_value); } \
protected: \
	std::vector<type> property_name; \
	void get_##property_name##_from_entity(const shared_ptr<::odata::core::odata_entity_value>& pentity); \
	void set_##property_name##_to_entity(const shared_ptr<::odata::core::odata_entity_value>& pentity);

#define IMPLEMENT_COLLECTION_ENUM_PROPERTY_IN_ENTITY_MAPPING(myclass, property_name, edm_name, type) \
	void myclass::get_##property_name##_from_entity(const shared_ptr<::odata::core::odata_entity_value>& pentity) \
	{ \
		std::shared_ptr<::odata::core::odata_value> property_value; \
		if (!pentity->get_property_value(_XPLATSTR(#edm_name), property_value) || !property_value) \
		{ \
			return; \
		} \
		auto property_collection_value = static_pointer_cast<::odata::core::odata_collection_value>(property_value); \
		if (!property_collection_value) \
		{ \
			return; \
		} \
		for (auto iter = property_collection_value->get_collection_values().cbegin(); iter != property_collection_value->get_collection_values().cend(); ++iter) \
		{ \
			auto enum_value = std::dynamic_pointer_cast<::odata::core::odata_enum_value>(*iter); \
			if (!enum_value) \
			{ \
				continue; \
			} \
			property_name.emplace_back(enum_type_resolver::get_enum_type_from_string(enum_value->to_string(), type())); \
		} \
	} \
	void myclass::set_##property_name##_to_entity(const shared_ptr<::odata::core::odata_entity_value>& pentity) \
	{ \
		if (!pentity || !pentity->get_value_type()) \
		{ \
			return; \
		} \
		auto entity_type = std::dynamic_pointer_cast<::odata::edm::edm_entity_type>(pentity->get_value_type()); \
		if (!entity_type) \
		{ \
			return; \
		} \
		auto edm_property = entity_type->find_property(_XPLATSTR(#edm_name)); \
		if (!edm_property) \
		{ \
			return; \
		} \
		auto property_type = edm_property->get_property_type(); \
		auto collection_value_type = std::dynamic_pointer_cast<::odata::edm::edm_collection_type>(property_type); \
		if (!collection_value_type) \
		{ \
			return; \
		} \
		std::shared_ptr<::odata::core::odata_collection_value> collection_value = ::odata::make_shared<::odata::core::odata_collection_value>(collection_value_type); \
		for (size_t i = 0; i < property_name.size(); ++i) \
		{ \
			collection_value->add_collection_value(::odata::make_shared<::odata::core::odata_enum_value>(collection_value_type->get_element_type(), enum_type_resolver::get_string_from_enum_type(property_name[i]))); \
		} \
		pentity->set_value(_XPLATSTR(#edm_name), collection_value); \
	}

#define DECLARE_COLLECTION_ENUM_PROPERTY_IN_COMPLEX_MAPPING(property_name, edm_name, type) \
public: \
	const std::vector<type>& get_##property_name() const { return property_name; } \
	void set_##property_name(const std::vector<type>& property_values) { property_name = property_values; } \
	void add_to_##property_name(type property_value) { property_name.emplace_back(property_value); } \
protected: \
	std::vector<type> property_name; \
	void get_##property_name##_from_complex(const shared_ptr<::odata::core::odata_complex_value>& pcomplex); \
	void set_##property_name##_to_complex(const shared_ptr<::odata::core::odata_complex_value>& pcomplex);

#define IMPLEMENT_COLLECTION_ENUM_PROPERTY_IN_COMPLEX_MAPPING(myclass, property_name, edm_name, type) \
	void myclass::get_##property_name##_from_complex(const shared_ptr<::odata::core::odata_complex_value>& pcomplex) \
	{ \
		std::shared_ptr<::odata::core::odata_value> property_value; \
		if (!pcomplex->get_property_value(_XPLATSTR(#edm_name), property_value) || !property_value) \
		{ \
			return; \
		} \
		auto property_collection_value = static_pointer_cast<::odata::core::odata_collection_value>(property_value); \
		if (!property_collection_value) \
		{ \
			return; \
		} \
		for (auto iter = property_collection_value->get_collection_values().cbegin(); iter != property_collection_value->get_collection_values().cend(); ++iter) \
		{ \
			auto enum_value = std::dynamic_pointer_cast<::odata::core::odata_enum_value>(*iter); \
			if (!enum_value) \
			{ \
				continue; \
			} \
			property_name.emplace_back(enum_type_resolver::get_enum_type_from_string(enum_value->to_string(), type())); \
		} \
	} \
	void myclass::set_##property_name##_to_complex(const shared_ptr<::odata::core::odata_complex_value>& pcomplex) \
	{ \
		if (!pcomplex || !pcomplex->get_value_type()) \
		{ \
			return; \
		} \
		auto complex_type = std::dynamic_pointer_cast<::odata::edm::edm_complex_type>(pcomplex->get_value_type()); \
		if (!complex_type) \
		{ \
			return; \
		} \
		auto edm_property = complex_type->find_property(_XPLATSTR(#edm_name)); \
		if (!edm_property) \
		{ \
			return; \
		} \
		auto property_type = edm_property->get_property_type(); \
		auto collection_value_type = std::dynamic_pointer_cast<::odata::edm::edm_collection_type>(property_type); \
		if (!collection_value_type) \
		{ \
			return; \
		} \
		std::shared_ptr<::odata::core::odata_collection_value> collection_value = ::odata::make_shared<::odata::core::odata_collection_value>(collection_value_type); \
		for (size_t i = 0; i < property_name.size(); ++i) \
		{ \
			collection_value->add_collection_value(::odata::make_shared<::odata::core::odata_enum_value>(collection_value_type->get_element_type(), enum_type_resolver::get_string_from_enum_type(property_name[i]))); \
		} \
		pcomplex->set_value(_XPLATSTR(#edm_name), collection_value); \
	}

#define DECLARE_NULLABLE_ENUM_PROPERTY_IN_ENTITY_MAPPING(property_name, edm_name, type) \
public: \
	type* get_##property_name() const { return property_name.get(); } \
	void set_##property_name(type property_value) \
	{ \
		property_name = ::odata::make_shared<type>(property_value); \
	} \
protected: \
	std::shared_ptr<type> property_name; \
	void get_##property_name##_from_entity(const shared_ptr<::odata::core::odata_entity_value>& pentity); \
	void set_##property_name##_to_entity(const shared_ptr<::odata::core::odata_entity_value>& pentity);

#define IMPLEMENT_NULLABLE_ENUM_PROPERTY_IN_ENTITY_MAPPING(myclass, property_name, edm_name, type) \
	void myclass::get_##property_name##_from_entity(const shared_ptr<::odata::core::odata_entity_value>& pentity) \
	{ \
		std::shared_ptr<::odata::core::odata_value> property_value; \
		if (!pentity->get_property_value(_XPLATSTR(#edm_name), property_value) || !property_value) \
		{ \
			return; \
		} \
		auto enum_value = std::dynamic_pointer_cast<::odata::core::odata_enum_value>(property_value); \
		if (enum_value) \
		{ \
			property_name = ::odata::make_shared<type>(enum_type_resolver::get_enum_type_from_string(enum_value->to_string(), type())); \
		} \
	} \
	void myclass::set_##property_name##_to_entity(const shared_ptr<::odata::core::odata_entity_value>& pentity) \
	{ \
		if (!pentity || !property_name) \
		{ \
			return; \
		} \
		auto entity_type = std::dynamic_pointer_cast<::odata::edm::edm_entity_type>(pentity->get_value_type()); \
		if (!entity_type) \
		{ \
			return; \
		} \
		auto edm_property = entity_type->find_property(_XPLATSTR(#edm_name)); \
		if (!edm_property) \
		{ \
			return; \
		} \
		auto property_type = edm_property->get_property_type(); \
		auto enum_value = ::odata::make_shared<::odata::core::odata_enum_value>(property_type, enum_type_resolver::get_string_from_enum_type(*property_name)); \
		pentity->set_value(_XPLATSTR(#edm_name), enum_value); \
	}

#define DECLARE_NULLABLE_ENUM_PROPERTY_IN_COMPLEX_MAPPING(property_name, edm_name, type) \
public: \
	type* get_##property_name() const { return property_name.get(); } \
	void set_##property_name(type property_value) \
	{ \
		property_name = ::odata::make_shared<type>(property_value); \
	} \
protected: \
	std::shared_ptr<type> property_name; \
	void get_##property_name##_from_complex(const shared_ptr<::odata::core::odata_complex_value>& pcomplex); \
	void set_##property_name##_to_complex(const shared_ptr<::odata::core::odata_complex_value>& pcomplex);

#define IMPLEMENT_NULLABLE_ENUM_PROPERTY_IN_COMPLEX_MAPPING(myclass, property_name, edm_name, type) \
	void myclass::get_##property_name##_from_complex(const shared_ptr<::odata::core::odata_complex_value>& pcomplex) \
	{ \
		std::shared_ptr<::odata::core::odata_value> property_value; \
		if (!pcomplex->get_property_value(_XPLATSTR(#edm_name), property_value) || !property_value) \
		{ \
			return; \
		} \
		auto enum_value = std::dynamic_pointer_cast<::odata::core::odata_enum_value>(property_value); \
		if (enum_value) \
		{ \
			property_name = ::odata::make_shared<type>(enum_type_resolver::get_enum_type_from_string(enum_value->to_string(), type())); \
		} \
	} \
	void myclass::set_##property_name##_to_complex(const shared_ptr<::odata::core::odata_complex_value>& pcomplex) \
	{ \
		if (!pcomplex || !property_name) \
		{ \
			return; \
		} \
		auto complex_type = std::dynamic_pointer_cast<::odata::edm::edm_complex_type>(pcomplex->get_value_type()); \
		if (!complex_type) \
		{ \
			return; \
		} \
		auto edm_property = complex_type->find_property(_XPLATSTR(#edm_name)); \
		if (!edm_property) \
		{ \
			return; \
		} \
		auto property_type = edm_property->get_property_type(); \
		auto enum_value = ::odata::make_shared<::odata::core::odata_enum_value>(property_type, enum_type_resolver::get_string_from_enum_type(*property_name)); \
		pcomplex->set_value(_XPLATSTR(#edm_name), enum_value); \
	}

#define DECLARE_FUNCTION_ENUM_TYPE_FROM_STRING(enum_type) \
	static enum_type get_enum_type_from_string(const ::odata::string_t& enum_string, enum_type default_enum_value);

#define DECLARE_FUNCTION_STRING_FROM_ENUM_TYPE(enum_type) \
	static ::odata::string_t get_string_from_enum_type(const enum_type& enum_value);

#define DECLARE_GET_ENUM_TYPE_NAMESPACE(enum_type, name_space) \
	static ::odata::string_t get_enum_type_namespace(const enum_type& enum_value);

#define IMPLEMENT_GET_ENUM_TYPE_NAMESPACE(enum_type, name_space) \
	::odata::string_t enum_type_resolver::get_enum_type_namespace(const enum_type& /*enum_value*/) \
	{ \
		return _XPLATSTR(#name_space); \
	}

#define BGEIN_IMPLEMENT_FUNCTION_ENUM_TYPE_FROM_STRING(enum_type) \
	enum_type enum_type_resolver::get_enum_type_from_string(const ::odata::string_t& enum_string, enum_type default_enum_value) \
	{ \
		enum_type ret = default_enum_value;

#define ON_IMPLEMENT_FUNCTION_ENUM_TYPE_FROM_STRING(enum_value_string, enum_type) \
	if (enum_string == _XPLATSTR(#enum_value_string)) \
	{ \
		ret = enum_type; \
	}

#define END_IMPLEMENT_FUNCTION_ENUM_TYPE_FROM_STRING(enum_type) \
		return ret; \
	}

#define BGEIN_IMPLEMENT_FUNCTION_STRING_FROM_ENUM_TYPE(enum_type) \
	::odata::string_t enum_type_resolver::get_string_from_enum_type(const enum_type& enum_value) \
	{ \
		::odata::string_t ret;

#define ON_IMPLEMENT_FUNCTION_STRING_FROM_ENUM_TYPE(enum_type, enum_value_string) \
	if (enum_value == enum_type) \
	{ \
		ret = _XPLATSTR(#enum_value_string); \
	}

#define END_IMPLEMENT_FUNCTION_STRING_FROM_ENUM_TYPE(enum_type) \
		return ret; \
	}

#define IMPLEMENT_EMPTY_DERIVED_TYPE_CREATOR_MAP(myclass) \
	std::unordered_map<::odata::string_t, CREATE_CALL_FOR_ENTITY> myclass::_derived_entity_creator_map;

#define BEGIN_IMPLEMENT_ENTITY_DERIVED_TYPE_CREATOR_MAP(myclass) \
	create_map_for_entity_type::value_type myclass::_init_creator_map_values[] = {

#define ON_IMPLEMENT_ENTITY_DERIVED_TYPE_CREATOR_MAP(keyname, funcname) \
	create_map_for_entity_type::value_type(_XPLATSTR(#keyname), (CREATE_CALL_FOR_ENTITY)&create_##funcname),

#define END_IMPLEMENT_ENTITY_DERIVED_TYPE_CREATOR_MAP(myclass) \
	}; \
	std::unordered_map<::odata::string_t, CREATE_CALL_FOR_ENTITY> myclass::_derived_entity_creator_map(_init_creator_map_values, _init_creator_map_values + sizeof(_init_creator_map_values) / sizeof(create_map_for_entity_type::value_type));

#define DECLARE_DERIVED_ENTITY_CREATOR_FUNC(derivedclassname, funcname) \
	static std::shared_ptr<type_base> create_##funcname(const std::shared_ptr<::odata::codegen::odata_service_context>& service_context) \
	{ \
		return ::odata::make_shared<derivedclassname>(service_context); \
	}

#define IMPLEMENT_COMPLEX_DERIVED_TYPE_CREATOR_MAP(myclass) \
	std::unordered_map<::odata::string_t, CREATE_CALL_FOR_COMPLEX> myclass::_derived_complex_creator_map;

#define BEGIN_IMPLEMENT_COMPLEX_DERIVED_TYPE_CREATOR_MAP(myclass) \
	create_map_for_complex_type::value_type myclass::_init_creator_map_values[] = {

#define ON_IMPLEMENT_COMPLEX_DERIVED_TYPE_CREATOR_MAP(keyname, funcname) \
	create_map_for_complex_type::value_type(_XPLATSTR(#keyname), (CREATE_CALL_FOR_COMPLEX)&create_##funcname),

#define END_IMPLEMENT_COMPLEX_DERIVED_TYPE_CREATOR_MAP(myclass) \
	}; \
	std::unordered_map<::odata::string_t, CREATE_CALL_FOR_COMPLEX> myclass::_derived_complex_creator_map(_init_creator_map_values, _init_creator_map_values + sizeof(_init_creator_map_values) / sizeof(create_map_for_entity_type::value_type));

#define DECLARE_DERIVED_COMPLEX_CREATOR_FUNC(derivedclassname, funcname) \
	static std::shared_ptr<type_base> create_##funcname(const std::shared_ptr<::odata::codegen::odata_service_context>& service_context) \
	{ \
		return ::odata::make_shared<derivedclassname>(service_context); \
	}

#define DECLARE_GET_KEY_PROPERTY_STRING_NO_PARAM(baseclass) \
public: \
	::odata::string_t get_key_property_string(bool with_key_name = false) \
	{ \
		return baseclass::get_key_property_string(with_key_name); \
	}

#define DECLARE_GET_KEY_PROPERTY_STRING_ONE_PARAM(baseclass, Name_1, key_1) \
public: \
	::odata::string_t get_key_property_string(bool with_key_name = false) \
	{ \
		::utility::stringstream_t ostr; \
		::odata::string_t key_string = baseclass::get_key_property_string(with_key_name); \
		if (key_string.empty()) \
		{ \
			if (with_key_name) \
				ostr << _XPLATSTR(#Name_1) << _XPLATSTR("=") << ::utility::conversions::print_string(key_1); \
			else \
				ostr << ::utility::conversions::print_string(key_1); \
			key_string = ostr.str(); \
		} \
		else \
		{ \
			key_string = baseclass::get_key_property_string(true); \
			ostr << _XPLATSTR(#Name_1) << _XPLATSTR("=") << ::utility::conversions::print_string(key_1); \
			key_string += _XPLATSTR(",") + ostr.str(); \
		} \
		return key_string; \
	}

#define DECLARE_GET_KEY_PROPERTY_STRING_TWO_PARAM(baseclass, Name_1, key_1, Name_2, key_2) \
public: \
	::odata::string_t get_key_property_string(bool /*with_key_name*/ = false) \
	{ \
		::utility::stringstream_t ostr; \
		ostr << _XPLATSTR(#Name_1) << _XPLATSTR("=") << ::utility::conversions::print_string(key_1) << _XPLATSTR(",") << _XPLATSTR(#Name_2) << _XPLATSTR("=") + ::utility::conversions::print_string(key_2); \
		::odata::string_t key_string = baseclass::get_key_property_string(true); \
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
	::odata::string_t get_key_property_string(bool /*with_key_name*/ = false) \
	{ \
		::utility::stringstream_t ostr; \
		ostr << _XPLATSTR(#Name_1) << _XPLATSTR("=") << ::utility::conversions::print_string(key_1) << _XPLATSTR(",") << _XPLATSTR(#Name_2) << _XPLATSTR("=") << ::utility::conversions::print_string(key_2) << _XPLATSTR(",") << _XPLATSTR(#Name_3) << _XPLATSTR("=") << ::utility::conversions::print_string(key_3);\
		::odata::string_t key_string = baseclass::get_key_property_string(true); \
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
	::odata::string_t get_key_property_string(bool /*with_key_name*/ = false) \
	{ \
		::utility::stringstream_t ostr; \
		ostr << _XPLATSTR(#Name_1) << _XPLATSTR("=") << ::utility::conversions::print_string(key_1) << _XPLATSTR(",") << _XPLATSTR(#Name_2) << _XPLATSTR("=") << ::utility::conversions::print_string(key_2) << _XPLATSTR(",") << _XPLATSTR(#Name_3) << _XPLATSTR("=") << ::utility::conversions::print_string(key_3) << _XPLATSTR(",") << _XPLATSTR(#Name_4) << _XPLATSTR("=") << ::utility::conversions::print_string(key_4);\
		::odata::string_t key_string = baseclass::get_key_property_string(true); \
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
	::odata::string_t function_query_url = _XPLATSTR(#funcname); \
	std::vector<std::shared_ptr<odata_parameter>> parameters; \
	return create_query<executor, odata_query_builder>(function_query_url)->execute_operation_query(parameters, isfunction); \
}

#define DECLARE_OPERATION_IMPORT_P1(myclass, funcname, executor, isfunction, key1, type1, value1) \
public: \
::pplx::task<typename executor::return_type> funcname(type1 value1);

#define IMPLEMENT_OPERATION_IMPORT_P1(myclass, funcname, executor, isfunction, key1, type1, value1) \
::pplx::task<typename executor::return_type> myclass::funcname(type1 value1) \
{ \
	::odata::string_t function_query_url = _XPLATSTR(#funcname); \
	std::vector<std::shared_ptr<odata_parameter>> parameters; \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key1), format_function_parameter(value1))); \
	return create_query<executor, odata_query_builder>(function_query_url)->execute_operation_query(parameters, isfunction); \
}

#define DECLARE_OPERATION_IMPORT_P2(myclass, funcname, executor, isfunction, key1, type1, value1, key2, type2, value2) \
public: \
::pplx::task<typename executor::return_type> funcname(type1 value1, type2 value2);

#define IMPLEMENT_OPERATION_IMPORT_P2(myclass, funcname, executor, isfunction, key1, type1, value1, key2, type2, value2) \
::pplx::task<typename executor::return_type> myclass::funcname(type1 value1, type2 value2) \
{ \
	::odata::string_t function_query_url = _XPLATSTR(#funcname); \
	std::vector<std::shared_ptr<odata_parameter>> parameters; \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key1), format_function_parameter(value1))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key2), format_function_parameter(value2))); \
	return create_query<executor, odata_query_builder>(function_query_url)->execute_operation_query(parameters, isfunction); \
}

#define DECLARE_OPERATION_IMPORT_P3(myclass, funcname, executor, isfunction, key1, type1, value1, key2, type2, value2, key3, type3, value3) \
public: \
::pplx::task<typename executor::return_type> funcname(type1 value1, type2 value2, type3 value3);

#define IMPLEMENT_OPERATION_IMPORT_P3(myclass, funcname, executor, isfunction, key1, type1, value1, key2, type2, value2, key3, type3, value3) \
::pplx::task<typename executor::return_type> myclass::funcname(type1 value1, type2 value2, type3 value3) \
{ \
	::odata::string_t function_query_url = _XPLATSTR(#funcname); \
	std::vector<std::shared_ptr<odata_parameter>> parameters; \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key1), format_function_parameter(value1))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key2), format_function_parameter(value2))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key3), format_function_parameter(value3))); \
	return create_query<executor, odata_query_builder>(function_query_url)->execute_operation_query(parameters, isfunction); \
}

#define DECLARE_OPERATION_IMPORT_P4(myclass, funcname, executor, isfunction, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4) \
public: \
::pplx::task<typename executor::return_type> funcname(type1 value1, type2 value2, type3 value3, type4 value4);

#define IMPLEMENT_OPERATION_IMPORT_P4(myclass, funcname, executor, isfunction, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4) \
::pplx::task<typename executor::return_type> myclass::funcname(type1 value1, type2 value2, type3 value3, type4 value4) \
{ \
	::odata::string_t function_query_url = _XPLATSTR(#funcname); \
	std::vector<std::shared_ptr<odata_parameter>> parameters; \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key1), format_function_parameter(value1))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key2), format_function_parameter(value2))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key3), format_function_parameter(value3))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key4), format_function_parameter(value4))); \
	return create_query<executor, odata_query_builder>(function_query_url)->execute_operation_query(parameters, isfunction); \
}

#define DECLARE_OPERATION_IMPORT_P5(myclass, funcname, executor, isfunction, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5) \
public: \
::pplx::task<typename executor::return_type> funcname(type1 value1, type2 value2, type3 value3, type4 value4, type5 value5);

#define IMPLEMENT_OPERATION_IMPORT_P5(myclass, funcname, executor, isfunction, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5) \
::pplx::task<typename executor::return_type> myclass::funcname(type1 value1, type2 value2, type3 value3, type4 value4, type5 value5) \
{ \
	::odata::string_t function_query_url = _XPLATSTR(#funcname); \
	std::vector<std::shared_ptr<odata_parameter>> parameters; \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key1), format_function_parameter(value1))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key2), format_function_parameter(value2))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key3), format_function_parameter(value3))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key4), format_function_parameter(value4))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key5), format_function_parameter(value5))); \
	return create_query<executor, odata_query_builder>(function_query_url)->execute_operation_query(parameters, isfunction); \
}

#define DECLARE_OPERATION_IMPORT_P6(myclass, funcname, executor, isfunction, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6) \
public: \
::pplx::task<typename executor::return_type> funcname(type1 value1, type2 value2, type3 value3, type4 value4, type5 value5, type6 value6);

#define IMPLEMENT_OPERATION_IMPORT_P6(myclass, funcname, executor, isfunction, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6) \
::pplx::task<typename executor::return_type> myclass::funcname(type1 value1, type2 value2, type3 value3, type4 value4, type5 value5, type6 value6) \
{ \
	::odata::string_t function_query_url = _XPLATSTR(#funcname); \
	std::vector<std::shared_ptr<odata_parameter>> parameters; \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key1), format_function_parameter(value1))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key2), format_function_parameter(value2))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key3), format_function_parameter(value3))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key4), format_function_parameter(value4))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key5), format_function_parameter(value5))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key6), format_function_parameter(value6))); \
	return create_query<executor, odata_query_builder>(function_query_url)->execute_operation_query(parameters, isfunction); \
}

#define DECLARE_OPERATION_IMPORT_P7(myclass, funcname, executor, isfunction, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6, key7, type7, value7) \
public: \
::pplx::task<typename executor::return_type> funcname(type1 value1, type2 value2, type3 value3, type4 value4, type5 value5, type6 value6, type7 value7);

#define IMPLEMENT_OPERATION_IMPORT_P7(myclass, funcname, executor, isfunction, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6, key7, type7, value7) \
::pplx::task<typename executor::return_type> myclass::funcname(type1 value1, type2 value2, type3 value3, type4 value4, type5 value5, type6 value6, type7 value7) \
{ \
	::odata::string_t function_query_url = _XPLATSTR(#funcname); \
	std::vector<std::shared_ptr<odata_parameter>> parameters; \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key1), format_function_parameter(value1))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key2), format_function_parameter(value2))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key3), format_function_parameter(value3))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key4), format_function_parameter(value4))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key5), format_function_parameter(value5))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key6), format_function_parameter(value6))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key7), format_function_parameter(value7))); \
	return create_query<executor, odata_query_builder>(function_query_url)->execute_operation_query(parameters, isfunction); \
}

#define DECLARE_OPERATION_IMPORT_P8(myclass, funcname, executor, isfunction, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6, key7, type7, value7, key8, type8, value8) \
public: \
::pplx::task<typename executor::return_type> funcname(type1 value1, type2 value2, type3 value3, type4 value4, type5 value5, type6 value6, type7 value7, type8 value8);

#define IMPLEMENT_OPERATION_IMPORT_P8(myclass, funcname, executor, isfunction, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6, key7, type7, value7, key8, type8, value8) \
::pplx::task<typename executor::return_type> myclass::funcname(type1 value1, type2 value2, type3 value3, type4 value4, type5 value5, type6 value6, type7 value7, type8 value8) \
{ \
	::odata::string_t function_query_url = _XPLATSTR(#funcname); \
	std::vector<std::shared_ptr<odata_parameter>> parameters; \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key1), format_function_parameter(value1))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key2), format_function_parameter(value2))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key3), format_function_parameter(value3))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key4), format_function_parameter(value4))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key5), format_function_parameter(value5))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key6), format_function_parameter(value6))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key7), format_function_parameter(value7))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key8), format_function_parameter(value8))); \
	return create_query<executor, odata_query_builder>(function_query_url)->execute_operation_query(parameters, isfunction); \
}

#define DECLARE_OPERATION_IMPORT_P9(myclass, funcname, executor, isfunction, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6, key7, type7, value7, key8, type8, value8, key9, type9, value9) \
public: \
::pplx::task<typename executor::return_type> funcname(type1 value1, type2 value2, type3 value3, type4 value4, type5 value5, type6 value6, type7 value7, type8 value8, type9 value9);

#define IMPLEMENT_OPERATION_IMPORT_P9(myclass, funcname, executor, isfunction, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6, key7, type7, value7, key8, type8, value8, key9, type9, value9) \
::pplx::task<typename executor::return_type> myclass::funcname(type1 value1, type2 value2, type3 value3, type4 value4, type5 value5, type6 value6, type7 value7, type8 value8, type9 value9) \
{ \
	::odata::string_t function_query_url = _XPLATSTR(#funcname); \
	std::vector<std::shared_ptr<odata_parameter>> parameters; \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key1), format_function_parameter(value1))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key2), format_function_parameter(value2))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key3), format_function_parameter(value3))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key4), format_function_parameter(value4))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key5), format_function_parameter(value5))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key6), format_function_parameter(value6))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key7), format_function_parameter(value7))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key8), format_function_parameter(value8))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key9), format_function_parameter(value9))); \
	return create_query<executor, odata_query_builder>(function_query_url)->execute_operation_query(parameters, isfunction); \
}

#define DECLARE_OPERATION_IMPORT_P10(myclass, funcname, executor, isfunction, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6, key7, type7, value7, key8, type8, value8, key9, type9, value9, key10, type10, value10) \
public: \
::pplx::task<typename executor::return_type> funcname(type1 value1, type2 value2, type3 value3, type4 value4, type5 value5, type6 value6, type7 value7, type8 value8, type9 value9, type10 value10);

#define IMPLEMENT_OPERATION_IMPORT_P10(myclass, funcname, executor, isfunction, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6, key7, type7, value7, key8, type8, value8, key9, type9, value9, key10, type10, value10) \
::pplx::task<typename executor::return_type> myclass::funcname(type1 value1, type2 value2, type3 value3, type4 value4, type5 value5, type6 value6, type7 value7, type8 value8, type9 value9, type10 value10) \
{ \
	::odata::string_t function_query_url = _XPLATSTR(#funcname); \
	std::vector<std::shared_ptr<odata_parameter>> parameters; \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key1),  format_function_parameter(value1))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key2),  format_function_parameter(value2))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key3),  format_function_parameter(value3))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key4),  format_function_parameter(value4))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key5),  format_function_parameter(value5))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key6),  format_function_parameter(value6))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key7),  format_function_parameter(value7))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key8),  format_function_parameter(value8))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key9),  format_function_parameter(value9))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key10), format_function_parameter(value10))); \
	return create_query<executor, odata_query_builder>(function_query_url)->execute_operation_query(parameters, isfunction); \
}

#define DECLARE_OPERATION_IMPORT_P11(myclass, funcname, executor, isfunction, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6, key7, type7, value7, key8, type8, value8, key9, type9, value9, key10, type10, value10, key11, type11, value11) \
public: \
::pplx::task<typename executor::return_type> funcname(type1 value1, type2 value2, type3 value3, type4 value4, type5 value5, type6 value6, type7 value7, type8 value8, type9 value9, type10 value10, type11 value11);

#define IMPLEMENT_OPERATION_IMPORT_P11(myclass, funcname, executor, isfunction, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6, key7, type7, value7, key8, type8, value8, key9, type9, value9, key10, type10, value10, key11, type11, value11) \
::pplx::task<typename executor::return_type> myclass::funcname(type1 value1, type2 value2, type3 value3, type4 value4, type5 value5, type6 value6, type7 value7, type8 value8, type9 value9, type10 value10, type11 value11) \
{ \
	::odata::string_t function_query_url = _XPLATSTR(#funcname); \
	std::vector<std::shared_ptr<odata_parameter>> parameters; \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key1),  format_function_parameter(value1))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key2),  format_function_parameter(value2))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key3),  format_function_parameter(value3))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key4),  format_function_parameter(value4))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key5),  format_function_parameter(value5))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key6),  format_function_parameter(value6))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key7),  format_function_parameter(value7))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key8),  format_function_parameter(value8))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key9),  format_function_parameter(value9))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key10), format_function_parameter(value10))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key11), format_function_parameter(value11))); \
	return create_query<executor, odata_query_builder>(function_query_url)->execute_operation_query(parameters, isfunction); \
}

#define DECLARE_OPERATION_IMPORT_P12(myclass, funcname, executor, isfunction, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6, key7, type7, value7, key8, type8, value8, key9, type9, value9, key10, type10, value10, key11, type11, value11, key12, type12, value12) \
public: \
::pplx::task<typename executor::return_type> funcname(type1 value1, type2 value2, type3 value3, type4 value4, type5 value5, type6 value6, type7 value7, type8 value8, type9 value9, type10 value10, type11 value11, type12 value12);

#define IMPLEMENT_OPERATION_IMPORT_P12(myclass, funcname, executor, isfunction, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6, key7, type7, value7, key8, type8, value8, key9, type9, value9, key10, type10, value10, key11, type11, value11, key12, type12, value12) \
::pplx::task<typename executor::return_type> myclass::funcname(type1 value1, type2 value2, type3 value3, type4 value4, type5 value5, type6 value6, type7 value7, type8 value8, type9 value9, type10 value10, type11 value11, type12 value12) \
{ \
	::odata::string_t function_query_url = _XPLATSTR(#funcname); \
	std::vector<std::shared_ptr<odata_parameter>> parameters; \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key1),  format_function_parameter(value1))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key2),  format_function_parameter(value2))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key3),  format_function_parameter(value3))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key4),  format_function_parameter(value4))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key5),  format_function_parameter(value5))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key6),  format_function_parameter(value6))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key7),  format_function_parameter(value7))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key8),  format_function_parameter(value8))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key9),  format_function_parameter(value9))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key10), format_function_parameter(value10))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key11), format_function_parameter(value11))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key12), format_function_parameter(value12))); \
	return create_query<executor, odata_query_builder>(function_query_url)->execute_operation_query(parameters, isfunction); \
}

#define DECLARE_OPERATION_P0(myclass, funcname, executor, isfunction) \
public: \
::pplx::task<typename executor::return_type> funcname();

#define IMPLEMENT_OPERATION_P0(myclass, funcname, executor, isfunction) \
::pplx::task<typename executor::return_type> myclass::funcname() \
{ \
	::odata::string_t function_query_url = m_service_context->get_relative_path(m_edit_link) + _XPLATSTR("/"); \
	function_query_url += m_namespace; \
	function_query_url += _XPLATSTR("."); \
	function_query_url += _XPLATSTR(#funcname); \
	std::vector<std::shared_ptr<odata_parameter>> parameters; \
	return m_service_context->create_query<executor, odata_query_builder>(function_query_url)->execute_operation_query(parameters, isfunction); \
}

#define DECLARE_OPERATION_P1(myclass, funcname, executor, isfunction, key1, type1, value1) \
public: \
::pplx::task<typename executor::return_type> funcname(type1 value1);

#define IMPLEMENT_OPERATION_P1(myclass, funcname, executor, isfunction, key1, type1, value1) \
::pplx::task<typename executor::return_type> myclass::funcname(type1 value1) \
{ \
	::odata::string_t function_query_url = m_service_context->get_relative_path(m_edit_link) + _XPLATSTR("/"); \
	function_query_url += m_namespace; \
	function_query_url += _XPLATSTR("."); \
	function_query_url += _XPLATSTR(#funcname); \
	std::vector<std::shared_ptr<odata_parameter>> parameters; \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key1), format_function_parameter(value1))); \
	return m_service_context->create_query<executor, odata_query_builder>(function_query_url)->execute_operation_query(parameters, isfunction); \
}

#define DECLARE_OPERATION_P2(myclass, funcname, executor, isfunction, key1, type1, value1, key2, type2, value2) \
public: \
::pplx::task<typename executor::return_type> funcname(type1 value1, type2 value2);

#define IMPLEMENT_OPERATION_P2(myclass, funcname, executor, isfunction, key1, type1, value1, key2, type2, value2) \
		::pplx::task<typename executor::return_type> myclass::funcname(type1 value1, type2 value2) \
{ \
	::odata::string_t function_query_url = m_service_context->get_relative_path(m_edit_link) + _XPLATSTR("/"); \
	function_query_url += m_namespace; \
	function_query_url += _XPLATSTR("."); \
	function_query_url += _XPLATSTR(#funcname); \
	std::vector<std::shared_ptr<odata_parameter>> parameters; \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key1), format_function_parameter(value1))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key2), format_function_parameter(value2))); \
	return m_service_context->create_query<executor, odata_query_builder>(function_query_url)->execute_operation_query(parameters, isfunction); \
}

#define DECLARE_OPERATION_P3(myclass, funcname, executor, isfunction, key1, type1, value1, key2, type2, value2, key3, type3, value3) \
public: \
::pplx::task<typename executor::return_type> funcname(type1 value1, type2 value2, type3 value3);

#define IMPLEMENT_OPERATION_P3(myclass, funcname, executor, isfunction, key1, type1, value1, key2, type2, value2, key3, type3, value3) \
::pplx::task<typename executor::return_type> myclass::funcname(type1 value1, type2 value2, type3 value3) \
{ \
	::odata::string_t function_query_url = m_service_context->get_relative_path(m_edit_link) + _XPLATSTR("/"); \
	function_query_url += m_namespace; \
	function_query_url += _XPLATSTR("."); \
	function_query_url += _XPLATSTR(#funcname); \
	std::vector<std::shared_ptr<odata_parameter>> parameters; \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key1), format_function_parameter(value1))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key2), format_function_parameter(value2))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key3), format_function_parameter(value3))); \
	return m_service_context->create_query<executor, odata_query_builder>(function_query_url)->execute_operation_query(parameters, isfunction); \
}

#define DECLARE_OPERATION_P4(myclass, funcname, executor, isfunction, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4) \
public: \
::pplx::task<typename executor::return_type> funcname(type1 value1, type2 value2, type3 value3, type4 value4);

#define IMPLEMENT_OPERATION_P4(myclass, funcname, executor, isfunction, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4) \
::pplx::task<typename executor::return_type> myclass::funcname(type1 value1, type2 value2, type3 value3, type4 value4) \
{ \
	::odata::string_t function_query_url = m_service_context->get_relative_path(m_edit_link) + _XPLATSTR("/"); \
	function_query_url += m_namespace; \
	function_query_url += _XPLATSTR("."); \
	function_query_url += _XPLATSTR(#funcname); \
	std::vector<std::shared_ptr<odata_parameter>> parameters; \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key1), format_function_parameter(value1))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key2), format_function_parameter(value2))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key3), format_function_parameter(value3))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key4), format_function_parameter(value4))); \
	return m_service_context->create_query<executor, odata_query_builder>(function_query_url)->execute_operation_query(parameters, isfunction); \
}

#define DECLARE_OPERATION_P5(myclass, funcname, executor, isfunction, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5) \
public: \
::pplx::task<typename executor::return_type> funcname(type1 value1, type2 value2, type3 value3, type4 value4, type5 value5);

#define IMPLEMENT_OPERATION_P5(myclass, funcname, executor, isfunction, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5) \
::pplx::task<typename executor::return_type> myclass::funcname(type1 value1, type2 value2, type3 value3, type4 value4, type5 value5) \
{ \
	::odata::string_t function_query_url = m_service_context->get_relative_path(m_edit_link) + _XPLATSTR("/"); \
	function_query_url += m_namespace; \
	function_query_url += _XPLATSTR("."); \
	function_query_url += _XPLATSTR(#funcname); \
	std::vector<std::shared_ptr<odata_parameter>> parameters; \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key1), format_function_parameter(value1))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key2), format_function_parameter(value2))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key3), format_function_parameter(value3))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key4), format_function_parameter(value4))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key5), format_function_parameter(value5))); \
	return m_service_context->create_query<executor, odata_query_builder>(function_query_url)->execute_operation_query(parameters, isfunction); \
}

#define DECLARE_OPERATION_P6(myclass, funcname, executor, isfunction, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6) \
public: \
::pplx::task<typename executor::return_type> funcname(type1 value1, type2 value2, type3 value3, type4 value4, type5 value5, type6 value6);

#define IMPLEMENT_OPERATION_P6(myclass, funcname, executor, isfunction, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6) \
::pplx::task<typename executor::return_type> myclass::funcname(type1 value1, type2 value2, type3 value3, type4 value4, type5 value5, type6 value6) \
{ \
	::odata::string_t function_query_url = m_service_context->get_relative_path(m_edit_link) + _XPLATSTR("/"); \
	function_query_url += m_namespace; \
	function_query_url += _XPLATSTR("."); \
	function_query_url += _XPLATSTR(#funcname); \
	std::vector<std::shared_ptr<odata_parameter>> parameters; \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key1), format_function_parameter(value1))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key2), format_function_parameter(value2))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key3), format_function_parameter(value3))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key4), format_function_parameter(value4))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key5), format_function_parameter(value5))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key6), format_function_parameter(value6))); \
	return m_service_context->create_query<executor, odata_query_builder>(function_query_url)->execute_operation_query(parameters, isfunction); \
}

#define DECLARE_OPERATION_P7(myclass, funcname, executor, isfunction, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6, key7, type7, value7) \
public: \
::pplx::task<typename executor::return_type> funcname(type1 value1, type2 value2, type3 value3, type4 value4, type5 value5, type6 value6, type7 value7);

#define IMPLEMENT_OPERATION_P7(myclass, funcname, executor, isfunction, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6, key7, type7, value7) \
::pplx::task<typename executor::return_type> myclass::funcname(type1 value1, type2 value2, type3 value3, type4 value4, type5 value5, type6 value6, type7 value7) \
{ \
	::odata::string_t function_query_url = m_service_context->get_relative_path(m_edit_link) + _XPLATSTR("/"); \
	function_query_url += m_namespace; \
	function_query_url += _XPLATSTR("."); \
	function_query_url += _XPLATSTR(#funcname); \
	std::vector<std::shared_ptr<odata_parameter>> parameters; \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key1), format_function_parameter(value1))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key2), format_function_parameter(value2))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key3), format_function_parameter(value3))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key4), format_function_parameter(value4))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key5), format_function_parameter(value5))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key6), format_function_parameter(value6))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key7), format_function_parameter(value7))); \
	return m_service_context->create_query<executor, odata_query_builder>(function_query_url)->execute_operation_query(parameters, isfunction); \
}

#define DECLARE_OPERATION_P8(myclass, funcname, executor, isfunction, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6, key7, type7, value7, key8, type8, value8) \
public: \
::pplx::task<typename executor::return_type> funcname(type1 value1, type2 value2, type3 value3, type4 value4, type5 value5, type6 value6, type7 value7, type8 value8);

#define IMPLEMENT_OPERATION_P8(myclass, funcname, executor, isfunction, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6, key7, type7, value7, key8, type8, value8) \
::pplx::task<typename executor::return_type> myclass::funcname(type1 value1, type2 value2, type3 value3, type4 value4, type5 value5, type6 value6, type7 value7, type8 value8) \
{ \
	::odata::string_t function_query_url = m_service_context->get_relative_path(m_edit_link) + _XPLATSTR("/"); \
	function_query_url += m_namespace; \
	function_query_url += _XPLATSTR("."); \
	function_query_url += _XPLATSTR(#funcname); \
	std::vector<std::shared_ptr<odata_parameter>> parameters; \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key1), format_function_parameter(value1))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key2), format_function_parameter(value2))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key3), format_function_parameter(value3))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key4), format_function_parameter(value4))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key5), format_function_parameter(value5))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key6), format_function_parameter(value6))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key7), format_function_parameter(value7))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key8), format_function_parameter(value8))); \
	return m_service_context->create_query<executor, odata_query_builder>(function_query_url)->execute_operation_query(parameters, isfunction); \
}

#define DECLARE_OPERATION_P9(myclass, funcname, executor, isfunction, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6, key7, type7, value7, key8, type8, value8, key9, type9, value9) \
public: \
::pplx::task<typename executor::return_type> funcname(type1 value1, type2 value2, type3 value3, type4 value4, type5 value5, type6 value6, type7 value7, type8 value8, type9 value9);

#define IMPLEMENT_OPERATION_P9(myclass, funcname, executor, isfunction, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6, key7, type7, value7, key8, type8, value8, key9, type9, value9) \
::pplx::task<typename executor::return_type> myclass::funcname(type1 value1, type2 value2, type3 value3, type4 value4, type5 value5, type6 value6, type7 value7, type8 value8, type9 value9) \
{ \
	::odata::string_t function_query_url = m_service_context->get_relative_path(m_edit_link) + _XPLATSTR("/"); \
	function_query_url += m_namespace; \
	function_query_url += _XPLATSTR("."); \
	function_query_url += _XPLATSTR(#funcname); \
	std::vector<std::shared_ptr<odata_parameter>> parameters; \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key1), format_function_parameter(value1))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key2), format_function_parameter(value2))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key3), format_function_parameter(value3))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key4), format_function_parameter(value4))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key5), format_function_parameter(value5))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key6), format_function_parameter(value6))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key7), format_function_parameter(value7))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key8), format_function_parameter(value8))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key9), format_function_parameter(value9))); \
	return m_service_context->create_query<executor, odata_query_builder>(function_query_url)->execute_operation_query(parameters, isfunction); \
}

#define DECLARE_OPERATION_P10(myclass, funcname, executor, isfunction, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6, key7, type7, value7, key8, type8, value8, key9, type9, value9, key10, type10, value10) \
public: \
::pplx::task<typename executor::return_type> funcname(type1 value1, type2 value2, type3 value3, type4 value4, type5 value5, type6 value6, type7 value7, type8 value8, type9 value9, type10 value10);

#define IMPLEMENT_OPERATION_P10(myclass, funcname, executor, isfunction, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6, key7, type7, value7, key8, type8, value8, key9, type9, value9, key10, type10, value10) \
::pplx::task<typename executor::return_type> myclass::funcname(type1 value1, type2 value2, type3 value3, type4 value4, type5 value5, type6 value6, type7 value7, type8 value8, type9 value9, type10 value10) \
{ \
	::odata::string_t function_query_url = m_service_context->get_relative_path(m_edit_link) + _XPLATSTR("/"); \
	function_query_url += m_namespace; \
	function_query_url += _XPLATSTR("."); \
	function_query_url += _XPLATSTR(#funcname); \
	std::vector<std::shared_ptr<odata_parameter>> parameters; \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key1),  format_function_parameter(value1))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key2),  format_function_parameter(value2))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key3),  format_function_parameter(value3))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key4),  format_function_parameter(value4))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key5),  format_function_parameter(value5))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key6),  format_function_parameter(value6))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key7),  format_function_parameter(value7))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key8),  format_function_parameter(value8))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key9),  format_function_parameter(value9))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key10), format_function_parameter(value10))); \
	return m_service_context->create_query<executor, odata_query_builder>(function_query_url)->execute_operation_query(parameters, isfunction); \
}

#define DECLARE_OPERATION_P11(myclass, funcname, executor, isfunction, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6, key7, type7, value7, key8, type8, value8, key9, type9, value9, key10, type10, value10, key11, type11, value11) \
public: \
::pplx::task<typename executor::return_type> funcname(type1 value1, type2 value2, type3 value3, type4 value4, type5 value5, type6 value6, type7 value7, type8 value8, type9 value9, type10 value10, type11 value11);

#define IMPLEMENT_OPERATION_P11(myclass, funcname, executor, isfunction, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6, key7, type7, value7, key8, type8, value8, key9, type9, value9, key10, type10, value10, key11, type11, value11) \
::pplx::task<typename executor::return_type> myclass::funcname(type1 value1, type2 value2, type3 value3, type4 value4, type5 value5, type6 value6, type7 value7, type8 value8, type9 value9, type10 value10, type11 value11) \
{ \
	::odata::string_t function_query_url = m_service_context->get_relative_path(m_edit_link) + _XPLATSTR("/"); \
	function_query_url += m_namespace; \
	function_query_url += _XPLATSTR("."); \
	function_query_url += _XPLATSTR(#funcname); \
	std::vector<std::shared_ptr<odata_parameter>> parameters; \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key1),  format_function_parameter(value1))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key2),  format_function_parameter(value2))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key3),  format_function_parameter(value3))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key4),  format_function_parameter(value4))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key5),  format_function_parameter(value5))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key6),  format_function_parameter(value6))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key7),  format_function_parameter(value7))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key8),  format_function_parameter(value8))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key9),  format_function_parameter(value9))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key10), format_function_parameter(value10))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key11), format_function_parameter(value11))); \
	return m_service_context->create_query<executor, odata_query_builder>(function_query_url)->execute_operation_query(parameters, isfunction); \
}

#define DECLARE_OPERATION_P12(myclass, funcname, executor, isfunction, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6, key7, type7, value7, key8, type8, value8, key9, type9, value9, key10, type10, value10, key11, type11, value11, key12, type12, value12) \
public: \
::pplx::task<typename executor::return_type> funcname(type1 value1, type2 value2, type3 value3, type4 value4, type5 value5, type6 value6, type7 value7, type8 value8, type9 value9, type10 value10, type11 value11, type12 value12);

#define IMPLEMENT_OPERATION_P12(myclass, funcname, executor, isfunction, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6, key7, type7, value7, key8, type8, value8, key9, type9, value9, key10, type10, value10, key11, type11, value11, key12, type12, value12) \
::pplx::task<typename executor::return_type> myclass::funcname(type1 value1, type2 value2, type3 value3, type4 value4, type5 value5, type6 value6, type7 value7, type8 value8, type9 value9, type10 value10, type11 value11, type12 value12) \
{ \
	::odata::string_t function_query_url = m_service_context->get_relative_path(m_edit_link) + _XPLATSTR("/"); \
	function_query_url += m_namespace; \
	function_query_url += _XPLATSTR("."); \
	function_query_url += _XPLATSTR(#funcname); \
	std::vector<std::shared_ptr<odata_parameter>> parameters; \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key1),  format_function_parameter(value1))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key2),  format_function_parameter(value2))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key3),  format_function_parameter(value3))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key4),  format_function_parameter(value4))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key5),  format_function_parameter(value5))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key6),  format_function_parameter(value6))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key7),  format_function_parameter(value7))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key8),  format_function_parameter(value8))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key9),  format_function_parameter(value9))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key10), format_function_parameter(value10))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key11), format_function_parameter(value11))); \
	parameters.emplace_back(::odata::make_shared<odata_parameter>(_XPLATSTR(#key12), format_function_parameter(value12))); \
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

#define DECLARE_FUNCTION_IMPORT_P4(myclass, funcname, executor, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4) \
	DECLARE_OPERATION_IMPORT_P4(myclass, funcname, executor, true, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4)

#define IMPLEMENT_FUNCTION_IMPORT_P4(myclass, funcname, executor, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4) \
	IMPLEMENT_OPERATION_IMPORT_P4(myclass, funcname, executor, true, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4)

#define DECLARE_FUNCTION_IMPORT_P5(myclass, funcname, executor, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5) \
	DECLARE_OPERATION_IMPORT_P5(myclass, funcname, executor, true, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5)

#define IMPLEMENT_FUNCTION_IMPORT_P5(myclass, funcname, executor, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5) \
	IMPLEMENT_OPERATION_IMPORT_P5(myclass, funcname, executor, true, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5)

#define DECLARE_FUNCTION_IMPORT_P6(myclass, funcname, executor, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6) \
	DECLARE_OPERATION_IMPORT_P6(myclass, funcname, executor, true, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6)

#define IMPLEMENT_FUNCTION_IMPORT_P6(myclass, funcname, executor, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6) \
	IMPLEMENT_OPERATION_IMPORT_P6(myclass, funcname, executor, true, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6)

#define DECLARE_FUNCTION_IMPORT_P7(myclass, funcname, executor, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6, key7, type7, value7) \
	DECLARE_OPERATION_IMPORT_P7(myclass, funcname, executor, true, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6, key7, type7, value7)

#define IMPLEMENT_FUNCTION_IMPORT_P7(myclass, funcname, executor, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6, key7, type7, value7) \
	IMPLEMENT_OPERATION_IMPORT_P7(myclass, funcname, executor, true, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6, key7, type7, value7)

#define DECLARE_FUNCTION_IMPORT_P8(myclass, funcname, executor, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6, key7, type7, value7, key8, type8, value8) \
	DECLARE_OPERATION_IMPORT_P8(myclass, funcname, executor, true, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6, key7, type7, value7, key8, type8, value8)

#define IMPLEMENT_FUNCTION_IMPORT_P8(myclass, funcname, executor, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6, key7, type7, value7, key8, type8, value8) \
	IMPLEMENT_OPERATION_IMPORT_P8(myclass, funcname, executor, true, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6, key7, type7, value7, key8, type8, value8)

#define DECLARE_FUNCTION_IMPORT_P9(myclass, funcname, executor, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6, key7, type7, value7, key8, type8, value8, key9, type9, value9) \
	DECLARE_OPERATION_IMPORT_P9(myclass, funcname, executor, true, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6, key7, type7, value7, key8, type8, value8, key9, type9, value9)

#define IMPLEMENT_FUNCTION_IMPORT_P9(myclass, funcname, executor, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6, key7, type7, value7, key8, type8, value8, key9, type9, value9) \
	IMPLEMENT_OPERATION_IMPORT_P9(myclass, funcname, executor, true, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6, key7, type7, value7, key8, type8, value8, key9, type9, value9)

#define DECLARE_FUNCTION_IMPORT_P10(myclass, funcname, executor, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6, key7, type7, value7, key8, type8, value8, key9, type9, value9, key10, type10, value10) \
	DECLARE_OPERATION_IMPORT_P10(myclass, funcname, executor, true, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6, key7, type7, value7, key8, type8, value8, key9, type9, value9, key10, type10, value10)

#define IMPLEMENT_FUNCTION_IMPORT_P10(myclass, funcname, executor, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6, key7, type7, value7, key8, type8, value8, key9, type9, value9, key10, type10, value10) \
	IMPLEMENT_OPERATION_IMPORT_P10(myclass, funcname, executor, true, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6, key7, type7, value7, key8, type8, value8, key9, type9, value9, key10, type10, value10)

#define DECLARE_FUNCTION_IMPORT_P11(myclass, funcname, executor, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6, key7, type7, value7, key8, type8, value8, key9, type9, value9, key10, type10, value10, key11, type11, value11) \
	DECLARE_OPERATION_IMPORT_P11(myclass, funcname, executor, true, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6, key7, type7, value7, key8, type8, value8, key9, type9, value9, key10, type10, value10, key11, type11, value11)

#define IMPLEMENT_FUNCTION_IMPORT_P11(myclass, funcname, executor, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6, key7, type7, value7, key8, type8, value8, key9, type9, value9, key10, type10, value10, key11, type11, value11) \
	IMPLEMENT_OPERATION_IMPORT_P11(myclass, funcname, executor, true, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6, key7, type7, value7, key8, type8, value8, key9, type9, value9, key10, type10, value10, key11, type11, value11)

#define DECLARE_FUNCTION_IMPORT_P12(myclass, funcname, executor, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6, key7, type7, value7, key8, type8, value8, key9, type9, value9, key10, type10, value10, key11, type11, value11, key12, type12, value12) \
	DECLARE_OPERATION_IMPORT_P12(myclass, funcname, executor, true, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6, key7, type7, value7, key8, type8, value8, key9, type9, value9, key10, type10, value10, key11, type11, value11, key12, type12, value12)

#define IMPLEMENT_FUNCTION_IMPORT_P12(myclass, funcname, executor, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6, key7, type7, value7, key8, type8, value8, key9, type9, value9, key10, type10, value10, key11, type11, value11, key12, type12, value12) \
	IMPLEMENT_OPERATION_IMPORT_P12(myclass, funcname, executor, true, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6, key7, type7, value7, key8, type8, value8, key9, type9, value9, key10, type10, value10, key11, type11, value11, key12, type12, value12)

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

#define DECLARE_FUNCTION_P4(myclass, funcname, executor, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4) \
	DECLARE_OPERATION_P4(myclass, funcname, executor, true, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4)

#define IMPLEMENT_FUNCTION_P4(myclass, funcname, executor, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4) \
	IMPLEMENT_OPERATION_P4(myclass, funcname, executor, true, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4)

#define DECLARE_FUNCTION_P5(myclass, funcname, executor, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5) \
	DECLARE_OPERATION_P5(myclass, funcname, executor, true, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5)

#define IMPLEMENT_FUNCTION_P5(myclass, funcname, executor, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5) \
	IMPLEMENT_OPERATION_P5(myclass, funcname, executor, true, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5)

#define DECLARE_FUNCTION_P6(myclass, funcname, executor, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6) \
	DECLARE_OPERATION_P6(myclass, funcname, executor, true, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6)

#define IMPLEMENT_FUNCTION_P6(myclass, funcname, executor, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6) \
	IMPLEMENT_OPERATION_P6(myclass, funcname, executor, true, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6)

#define DECLARE_FUNCTION_P7(myclass, funcname, executor, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6, key7, type7, value7) \
	DECLARE_OPERATION_P7(myclass, funcname, executor, true, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6, key7, type7, value7)

#define IMPLEMENT_FUNCTION_P7(myclass, funcname, executor, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6, key7, type7, value7) \
	IMPLEMENT_OPERATION_P7(myclass, funcname, executor, true, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6, key7, type7, value7)

#define DECLARE_FUNCTION_P8(myclass, funcname, executor, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6, key7, type7, value7, key8, type8, value8) \
	DECLARE_OPERATION_P8(myclass, funcname, executor, true, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6, key7, type7, value7, key8, type8, value8)

#define IMPLEMENT_FUNCTION_P8(myclass, funcname, executor, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6, key7, type7, value7, key8, type8, value8) \
	IMPLEMENT_OPERATION_P8(myclass, funcname, executor, true, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6, key7, type7, value7, key8, type8, value8)

#define DECLARE_FUNCTION_P9(myclass, funcname, executor, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6, key7, type7, value7, key8, type8, value8, key9, type9, value9) \
	DECLARE_OPERATION_P9(myclass, funcname, executor, true, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6, key7, type7, value7, key8, type8, value8, key9, type9, value9)

#define IMPLEMENT_FUNCTION_P9(myclass, funcname, executor, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6, key7, type7, value7, key8, type8, value8, key9, type9, value9) \
	IMPLEMENT_OPERATION_P9(myclass, funcname, executor, true, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6, key7, type7, value7, key8, type8, value8, key9, type9, value9)

#define DECLARE_FUNCTION_P10(myclass, funcname, executor, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6, key7, type7, value7, key8, type8, value8, key9, type9, value9, key10, type10, value10) \
	DECLARE_OPERATION_P10(myclass, funcname, executor, true, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6, key7, type7, value7, key8, type8, value8, key9, type9, value9, key10, type10, value10)

#define IMPLEMENT_FUNCTION_P10(myclass, funcname, executor, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6, key7, type7, value7, key8, type8, value8, key9, type9, value9, key10, type10, value10) \
	IMPLEMENT_OPERATION_P10(myclass, funcname, executor, true, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6, key7, type7, value7, key8, type8, value8, key9, type9, value9, key10, type10, value10)

#define DECLARE_FUNCTION_P11(myclass, funcname, executor, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6, key7, type7, value7, key8, type8, value8, key9, type9, value9, key10, type10, value10, key11, type11, value11) \
	DECLARE_OPERATION_P11(myclass, funcname, executor, true, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6, key7, type7, value7, key8, type8, value8, key9, type9, value9, key10, type10, value10, key11, type11, value11)

#define IMPLEMENT_FUNCTION_P11(myclass, funcname, executor, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6, key7, type7, value7, key8, type8, value8, key9, type9, value9, key10, type10, value10, key11, type11, value11) \
	IMPLEMENT_OPERATION_P11(myclass, funcname, executor, true, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6, key7, type7, value7, key8, type8, value8, key9, type9, value9, key10, type10, value10, key11, type11, value11)

#define DECLARE_FUNCTION_P12(myclass, funcname, executor, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6, key7, type7, value7, key8, type8, value8, key9, type9, value9, key10, type10, value10, key11, type11, value11, key12, type12, value12) \
	DECLARE_OPERATION_P12(myclass, funcname, executor, true, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6, key7, type7, value7, key8, type8, value8, key9, type9, value9, key10, type10, value10, key11, type11, value11, key12, type12, value12)

#define IMPLEMENT_FUNCTION_P12(myclass, funcname, executor, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6, key7, type7, value7, key8, type8, value8, key9, type9, value9, key10, type10, value10, key11, type11, value11, key12, type12, value12) \
	IMPLEMENT_OPERATION_P12(myclass, funcname, executor, true, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6, key7, type7, value7, key8, type8, value8, key9, type9, value9, key10, type10, value10, key11, type11, value11, key12, type12, value12)

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

#define IMPLEMENT_ACTION_IMPORT_P3(myclass, funcname, executor, key1, type1, value1, key2, type2, value2, key3, type3, value3) \
	IMPLEMENT_OPERATION_IMPORT_P3(myclass, funcname, executor, false, key1, type1, value1, key2, type2, value2, key3, type3, value3)

#define DECLARE_ACTION_IMPORT_P4(myclass, funcname, executor, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4) \
	DECLARE_OPERATION_IMPORT_P4(myclass, funcname, executor, false, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4)

#define IMPLEMENT_ACTION_IMPORT_P4(myclass, funcname, executor, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4) \
	IMPLEMENT_OPERATION_IMPORT_P4(myclass, funcname, executor, false, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4)

#define DECLARE_ACTION_IMPORT_P5(myclass, funcname, executor, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5) \
	DECLARE_OPERATION_IMPORT_P5(myclass, funcname, executor, false, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5)

#define IMPLEMENT_ACTION_IMPORT_P5(myclass, funcname, executor, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5) \
	IMPLEMENT_OPERATION_IMPORT_P5(myclass, funcname, executor, false, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5)

#define DECLARE_ACTION_IMPORT_P6(myclass, funcname, executor, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6) \
	DECLARE_OPERATION_IMPORT_P6(myclass, funcname, executor, false, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6)

#define IMPLEMENT_ACTION_IMPORT_P6(myclass, funcname, executor, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6) \
	IMPLEMENT_OPERATION_IMPORT_P6(myclass, funcname, executor, false, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6)

#define DECLARE_ACTION_IMPORT_P7(myclass, funcname, executor, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6, key7, type7, value7) \
	DECLARE_OPERATION_IMPORT_P7(myclass, funcname, executor, false, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6, key7, type7, value7)

#define IMPLEMENT_ACTION_IMPORT_P7(myclass, funcname, executor, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6, key7, type7, value7) \
	IMPLEMENT_OPERATION_IMPORT_P7(myclass, funcname, executor, false, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6, key7, type7, value7)

#define DECLARE_ACTION_IMPORT_P8(myclass, funcname, executor, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6, key7, type7, value7, key8, type8, value8) \
	DECLARE_OPERATION_IMPORT_P8(myclass, funcname, executor, false, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6, key7, type7, value7, key8, type8, value8)

#define IMPLEMENT_ACTION_IMPORT_P8(myclass, funcname, executor, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6, key7, type7, value7, key8, type8, value8) \
	IMPLEMENT_OPERATION_IMPORT_P8(myclass, funcname, executor, false, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6, key7, type7, value7, key8, type8, value8)

#define DECLARE_ACTION_IMPORT_P9(myclass, funcname, executor, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6, key7, type7, value7, key8, type8, value8, key9, type9, value9) \
	DECLARE_OPERATION_IMPORT_P9(myclass, funcname, executor, false, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6, key7, type7, value7, key8, type8, value8, key9, type9, value9)

#define IMPLEMENT_ACTION_IMPORT_P9(myclass, funcname, executor, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6, key7, type7, value7, key8, type8, value8, key9, type9, value9) \
	IMPLEMENT_OPERATION_IMPORT_P9(myclass, funcname, executor, false, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6, key7, type7, value7, key8, type8, value8, key9, type9, value9)

#define DECLARE_ACTION_IMPORT_P10(myclass, funcname, executor, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6, key7, type7, value7, key8, type8, value8, key9, type9, value9, key10, type10, value10) \
	DECLARE_OPERATION_IMPORT_P10(myclass, funcname, executor, false, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6, key7, type7, value7, key8, type8, value8, key9, type9, value9, key10, type10, value10)

#define IMPLEMENT_ACTION_IMPORT_P10(myclass, funcname, executor, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6, key7, type7, value7, key8, type8, value8, key9, type9, value9, key10, type10, value10) \
	IMPLEMENT_OPERATION_IMPORT_P10(myclass, funcname, executor, false, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6, key7, type7, value7, key8, type8, value8, key9, type9, value9, key10, type10, value10)

#define DECLARE_ACTION_IMPORT_P11(myclass, funcname, executor, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6, key7, type7, value7, key8, type8, value8, key9, type9, value9, key10, type10, value10, key11, type11, value11) \
	DECLARE_OPERATION_IMPORT_P11(myclass, funcname, executor, false, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6, key7, type7, value7, key8, type8, value8, key9, type9, value9, key10, type10, value10, key11, type11, value11)

#define IMPLEMENT_ACTION_IMPORT_P11(myclass, funcname, executor, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6, key7, type7, value7, key8, type8, value8, key9, type9, value9, key10, type10, value10, key11, type11, value11) \
	IMPLEMENT_OPERATION_IMPORT_P11(myclass, funcname, executor, false, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6, key7, type7, value7, key8, type8, value8, key9, type9, value9, key10, type10, value10, key11, type11, value11)

#define DECLARE_ACTION_IMPORT_P12(myclass, funcname, executor, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6, key7, type7, value7, key8, type8, value8, key9, type9, value9, key10, type10, value10, key11, type11, value11, key12, type12, value12) \
	DECLARE_OPERATION_IMPORT_P12(myclass, funcname, executor, false, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6, key7, type7, value7, key8, type8, value8, key9, type9, value9, key10, type10, value10, key11, type11, value11, key12, type12, value12)

#define IMPLEMENT_ACTION_IMPORT_P12(myclass, funcname, executor, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6, key7, type7, value7, key8, type8, value8, key9, type9, value9, key10, type10, value10, key11, type11, value11, key12, type12, value12) \
	IMPLEMENT_OPERATION_IMPORT_P12(myclass, funcname, executor, false, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6, key7, type7, value7, key8, type8, value8, key9, type9, value9, key10, type10, value10, key11, type11, value11, key12, type12, value12)

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

#define DECLARE_ACTION_P6(myclass, funcname, executor, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6) \
	DECLARE_OPERATION_P6(myclass, funcname, executor, false, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6)

#define IMPLEMENT_ACTION_P6(myclass, funcname, executor, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6) \
	IMPLEMENT_OPERATION_P6(myclass, funcname, executor, false, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6)

#define DECLARE_ACTION_P7(myclass, funcname, executor, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6, key7, type7, value7) \
	DECLARE_OPERATION_P7(myclass, funcname, executor, false, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6, key7, type7, value7)

#define IMPLEMENT_ACTION_P7(myclass, funcname, executor, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6, key7, type7, value7) \
	IMPLEMENT_OPERATION_P7(myclass, funcname, executor, false, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6, key7, type7, value7)

#define DECLARE_ACTION_P8(myclass, funcname, executor, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6, key7, type7, value7, key8, type8, value8) \
	DECLARE_OPERATION_P8(myclass, funcname, executor, false, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6, key7, type7, value7, key8, type8, value8)

#define IMPLEMENT_ACTION_P8(myclass, funcname, executor, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6, key7, type7, value7, key8, type8, value8) \
	IMPLEMENT_OPERATION_P8(myclass, funcname, executor, false, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6, key7, type7, value7, key8, type8, value8)

#define DECLARE_ACTION_P9(myclass, funcname, executor, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6, key7, type7, value7, key8, type8, value8, key9, type9, value9) \
	DECLARE_OPERATION_P9(myclass, funcname, executor, false, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6, key7, type7, value7, key8, type8, value8, key9, type9, value9)

#define IMPLEMENT_ACTION_P9(myclass, funcname, executor, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6, key7, type7, value7, key8, type8, value8, key9, type9, value9) \
	IMPLEMENT_OPERATION_P9(myclass, funcname, executor, false, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6, key7, type7, value7, key8, type8, value8, key9, type9, value9)

#define DECLARE_ACTION_P10(myclass, funcname, executor, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6, key7, type7, value7, key8, type8, value8, key9, type9, value9, key10, type10, value10) \
	DECLARE_OPERATION_P10(myclass, funcname, executor, false, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6, key7, type7, value7, key8, type8, value8, key9, type9, value9, key10, type10, value10)

#define IMPLEMENT_ACTION_P10(myclass, funcname, executor, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6, key7, type7, value7, key8, type8, value8, key9, type9, value9, key10, type10, value10) \
	IMPLEMENT_OPERATION_P10(myclass, funcname, executor, false, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6, key7, type7, value7, key8, type8, value8, key9, type9, value9, key10, type10, value10)

#define DECLARE_ACTION_P11(myclass, funcname, executor, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6, key7, type7, value7, key8, type8, value8, key9, type9, value9, key10, type10, value10, key11, type11, value11) \
	DECLARE_OPERATION_P11(myclass, funcname, executor, false, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6, key7, type7, value7, key8, type8, value8, key9, type9, value9, key10, type10, value10, key11, type11, value11)

#define IMPLEMENT_ACTION_P11(myclass, funcname, executor, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6, key7, type7, value7, key8, type8, value8, key9, type9, value9, key10, type10, value10, key11, type11, value11) \
	IMPLEMENT_OPERATION_P11(myclass, funcname, executor, false, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6, key7, type7, value7, key8, type8, value8, key9, type9, value9, key10, type10, value10, key11, type11, value11)

#define DECLARE_ACTION_P12(myclass, funcname, executor, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6, key7, type7, value7, key8, type8, value8, key9, type9, value9, key10, type10, value10, key11, type11, value11, key12, type12, value12) \
	DECLARE_OPERATION_P12(myclass, funcname, executor, false, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6, key7, type7, value7, key8, type8, value8, key9, type9, value9, key10, type10, value10, key11, type11, value11, key12, type12, value12)

#define IMPLEMENT_ACTION_P12(myclass, funcname, executor, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6, key7, type7, value7, key8, type8, value8, key9, type9, value9, key10, type10, value10, key11, type11, value11, key12, type12, value12) \
	IMPLEMENT_OPERATION_P12(myclass, funcname, executor, false, key1, type1, value1, key2, type2, value2, key3, type3, value3, key4, type4, value4, key5, type5, value5, key6, type6, value6, key7, type7, value7, key8, type8, value8, key9, type9, value9, key10, type10, value10, key11, type11, value11, key12, type12, value12)

}}
