//---------------------------------------------------------------------
// <copyright file="odata_codegen_initializer.h" company="Microsoft">
//      Copyright (C) Microsoft Corporation. All rights reserved. See License.txt in the project root for license information.
// </copyright>
//---------------------------------------------------------------------

#pragma once

#include "odata/client/odata_client.h"
#include "odata/common/utility.h"
#include "odata/core/odata_core.h"
#include "odata/edm/odata_edm.h"

namespace odata { namespace codegen { namespace tools {

enum PROPERTY_TYPE
{
	E_PRIMITIVE = 0,
	E_COMPLEX,
	E_ENTITY,
	E_ENUM,
	E_COLLECTION_PRIMITIVE,
	E_COLLECTION_COMPLEX,
	E_COLLECTION_ENTITY,
	E_COLLECTION_ENUM,
	E_FUNCTION,  // currently means function bounded to single entity
	E_FUNCTION_IMPORT,
	E_ACTION,    // currently means function bounded to single entity
	E_ACTION_IMPORT,
	E_CONTAINER_ENTITY_SET,
	E_CONTAINER_SINGLETON,
	E_KNOWN,
};

enum CLASS_TYPE
{
	E_CLASS_COMPLEX = 0,
	E_CLASS_ENTITY,
	E_CLASS_ENUM,
	E_CLASS_CONTAINER,
	E_CLASS_KNOWN,
};

struct class_info
{
	::odata::string_t _edm_name;
	::odata::string_t _edm_namespace;
	::odata::string_t _class_name;
	::odata::string_t _base_class_name;
	CLASS_TYPE        _type;

	class_info()
	{
		_type = CLASS_TYPE::E_CLASS_KNOWN;
	}
};

struct operation_param
{
	::odata::string_t _edm_name;
	::odata::string_t _member_name;
	::odata::string_t _member_type;
	::odata::string_t _member_strong_type_name;
};

struct operation_info
{
	std::vector<operation_param> vec_params;
	::odata::string_t _return_type;
	::odata::string_t _executor_name;
};

struct property_info
{
	::odata::string_t _edm_name;
	::utility::string_t _class_member_name;
	// int* or vector<int>
	::odata::string_t _class_member_type;
	// int or single entity type name
	::odata::string_t _strong_type_name;
	PROPERTY_TYPE     _type;
	::odata::string_t _default_value;
	bool              _is_nullable;
	bool              _is_key;

	std::shared_ptr<operation_info> _operation_info; // used for action / function

	property_info()
	{
		_operation_info = nullptr;
		_is_nullable = true;
		_type = E_KNOWN;
		_is_key = false;
		_is_nullable = false;
	}
};

struct code_gen_configuration
{
	::odata::string_t _namespace;
	::odata::string_t _file_name;

	code_gen_configuration()
	{
	}
};

// key is property name
typedef std::unordered_map<::odata::string_t, property_info>  class_property_info;
// key is enity complex enum type name
typedef std::unordered_map<::odata::string_t, class_property_info>  class_property_map;  // class member infomation
// key is enity complex enum type name
typedef std::unordered_map<::odata::string_t, class_info>  class_map;   // class infomation
typedef std::unordered_map<::odata::string_t, std::vector<::odata::string_t>> derived_class_map;

struct schema_info  // one model may contain several schemas (namespaces)
{
	class_property_map  m_class_property_map;
	class_map m_class_map;
	std::vector<class_info> m_class_vec; // m_class_map has to be sorted because classes have base-drive relationship
	derived_class_map m_derived_classes;

	void clear()
	{
		m_class_property_map.clear();
		m_class_map.clear();
		m_class_vec.clear();
		m_derived_classes.clear();
	}
};

// key is namespace name
typedef std::unordered_map<::odata::string_t, schema_info> code_gen_map;

class odata_codegen_initializer
{
public:
	::pplx::task<int> initialize_from_metadata_file(const ::odata::string_t metadata_file, const ::odata::string_t output_file_name);
	::pplx::task<int> initialize_from_service_metadata(const ::odata::string_t metadata_url, const ::odata::string_t output_file_name, const ::odata::string_t user_name = _XPLATSTR(""), const ::odata::string_t  password = _XPLATSTR(""));
	::pplx::task<int> begin_initialize_code_gen_info(::std::shared_ptr<::odata::edm::edm_model> model);

	const code_gen_configuration& get_config() const
	{
		return m_config;
	}

	void set_config(const code_gen_configuration& config)
	{
		m_config = config;
	}

	const code_gen_map& get_code_gen_info() const
	{
		return m_code_gen_map;
	}

private:
	void reset_class_info();
	void initialize_class_info(const std::shared_ptr<::odata::edm::edm_schema>& schema, schema_info& _schema_info);
	void initialize_property_info_of_class(const std::shared_ptr<::odata::edm::edm_schema>& schema, schema_info& _schema_info);

	void set_default_value(property_info& _property_info, const std::shared_ptr<::odata::edm::edm_property_type>& property_type);
	void set_strong_type_name(property_info& _property_info);

	::odata::string_t get_executor_name_from_edm_type(const std::shared_ptr<::odata::edm::edm_named_type>& type);
	void set_operation_info(property_info& operation_info, const std::shared_ptr<::odata::edm::edm_operation_type>& operation_type, schema_info& _schema_info);
private:
	friend class odata_codegen_writer;

	std::shared_ptr<::odata::edm::edm_model> m_model;
	code_gen_configuration                   m_config;
	code_gen_map                             m_code_gen_map;
};

}}}
