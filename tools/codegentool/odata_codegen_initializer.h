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
	::utility::string_t _edm_name;
	::utility::string_t _edm_namespace;
	::utility::string_t _class_name;
	::utility::string_t _base_class_name;
	CLASS_TYPE          _type;

	class_info()
	{
		_type = CLASS_TYPE::E_CLASS_KNOWN;
	}
};

struct operation_param
{
	::utility::string_t _edm_name;
	::utility::string_t _memeber_name;
	::utility::string_t _memeber_type;
	::utility::string_t _memeber_strong_type_name;
};

struct operation_info
{
	std::vector<operation_param> vec_params;
    ::utility::string_t _return_type;
	::utility::string_t _executor_name;
};

struct property_info
{
	::utility::string_t _edm_name;
	::utility::string_t _class_memeber_name;
	// int* or vector<int>
	::utility::string_t _class_memeber_type;
	// int or single entity type name
	::utility::string_t _strong_type_name;
	PROPERTY_TYPE       _type;
	::utility::string_t _default_value;
	bool                _is_nullable;
	bool                _is_key;

	std::shared_ptr<operation_info>  _operation_info;   // used for action / function

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
	::utility::string_t _metadata_url;
	::utility::string_t _namespace;
	::utility::string_t _user_name;
	::utility::string_t _pass_word;
	::utility::string_t _file_name;

	code_gen_configuration()
	{
	}
};

// key is property name
typedef std::unordered_map<::utility::string_t, property_info>  class_property_info;
// key is enity complex enum type name
typedef std::unordered_map<::utility::string_t, class_property_info>  class_property_map;  // class memeber infomation
// key is enity complex enum type name
typedef std::unordered_map<::utility::string_t, class_info>  class_map;   // class infomation
typedef std::unordered_map<::utility::string_t, std::vector<::utility::string_t>> derived_class_map;

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
typedef std::unordered_map<::utility::string_t, schema_info> code_gen_map;

class odata_codegen_initializer
{
public:
	::pplx::task<int> begin_initialize(const ::utility::string_t metadata_url, const ::utility::string_t file_name, const ::utility::string_t user_name, const ::utility::string_t  password);
	::pplx::task<int> begin_initialize_code_gen_info(const ::utility::string_t& metadata_url, const ::utility::string_t& _user_name = U(""), const ::utility::string_t& _pass_word = U(""));

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

	::utility::string_t get_executor_name_from_edm_type(const std::shared_ptr<::odata::edm::edm_named_type>& type);
	void set_operation_info(property_info& operation_info, const std::shared_ptr<::odata::edm::edm_operation_type>& operation_type, schema_info& _schema_info);
private:
	friend class odata_codegen_writer;

	std::shared_ptr<::odata::edm::edm_model> m_model;
	code_gen_configuration                   m_config;
	code_gen_map                             m_code_gen_map;
};


}}}