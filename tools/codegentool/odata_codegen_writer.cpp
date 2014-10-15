/*
 * Copyright (c) Microsoft Corporation. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
 
#include "odata_codegen_writer.h"
#include "odata/client/odata_client.h"
#include "odata/edm/edm_model_utility.h"
#include <regex>

#define INDENT U("    ")

namespace odata { namespace codegen { namespace tools {

static ::utility::string_t normalize_namespace(const ::utility::string_t& _namespace)
{
	::utility::string_t ret = _namespace;

	std::replace(ret.begin(), ret.end(), U('.'), U('_'));

	return ret;
}

::pplx::task<int> odata_codegen_writer::begin_generate_file()
{
	if (!m_initializer)
	{
		return ::pplx::task_from_result(0);
	}

	::utility::ofstream_t ofs_header(m_initializer->get_config()._file_name + U(".h"));
	::utility::ofstream_t ofs_cpp(m_initializer->get_config()._file_name + U(".cpp"));

	generate_declare_include_files(ofs_header);
	generate_implement_include_files(ofs_cpp);

	for (auto schema_iter = m_initializer->m_code_gen_map.begin(); schema_iter != m_initializer->m_code_gen_map.end(); schema_iter++)
	{
		generate_declare_namespace_begin(ofs_header, normalize_namespace(schema_iter->first));
	    generate_implement_namespace_begin(ofs_cpp, normalize_namespace(schema_iter->first));

		generate_declare_class(ofs_header, schema_iter->second);

		generate_enum_types(ofs_header, ofs_cpp, schema_iter->second);
	    generate_complex_types(ofs_header, ofs_cpp, schema_iter->second);
	    generate_entity_types(ofs_header, ofs_cpp, schema_iter->second);

		// generate entity container in h file
		generate_entity_container(ofs_header, ofs_cpp, schema_iter->second);

		// generate derived creator in cpp
		generate_derived_creator(ofs_cpp, schema_iter->second);
        
        generate_declare_include_files_end(ofs_header);
	    generate_declare_namespace_end(ofs_header);
	    generate_implement_namespace_end(ofs_cpp);
	}

	ofs_header.close();
	ofs_cpp.close();

	return ::pplx::task_from_result(0);
}

void odata_codegen_writer::generate_declare_include_files(::utility::ofstream_t& out)
{
	output_line_content(out, U("#pragma once"));
	output_line_content(out, U("#include \"odata/codegen/code_generation.h\""));
	output_line_content(out, U("#include \"odata/codegen/odata_service_context.h\""));
	output_line_content(out, U("#include \"odata/codegen/odata_service_query.h\""));
	output_line_content(out, U("#include \"odata/codegen/odata_entityset_query_executor.h\""));
	output_line_content(out, U("#include \"odata/codegen/odata_singleton_query_executor.h\""));
	output_line_content(out, U("#include \"odata/codegen/odata_primitive_query_executor.h\""));
	output_line_content(out, U("#include \"odata/codegen/odata_primitive_query_executor.h\""));
	output_line_content(out, U("#include \"odata/codegen/odata_complex_query_executor.h\""));
	output_line_content(out, U("#include \"odata/codegen/odata_enum_query_executor.h\""));
	output_line_content(out, U("#include \"odata/codegen/odata_void_query_executor.h\""));
	output_line_content(out, U("#include \"odata/codegen/odata_query_builder.h\""));
	output_line_content(out, U("#include \"cpprest/json.h\""));
	output_empty_line(out, 1);
	output_line_content(out, U("using namespace std;"));
	output_line_content(out, U("using namespace ::odata::client;"));
	output_line_content(out, U("using namespace ::odata::core;"));
	output_line_content(out, U("using namespace ::odata::edm;"));
	output_line_content(out, U("using namespace ::odata::codegen;"));
	output_empty_line(out, 1);
}

void odata_codegen_writer::generate_implement_include_files(::utility::ofstream_t& out)
{
	out << U("#include \"") << m_initializer->get_config()._file_name  << U(".h\"") << std::endl;
	output_empty_line(out, 1);
}

void odata_codegen_writer::generate_declare_namespace_begin(::utility::ofstream_t& out, const ::utility::string_t& _namespace)
{
	output_line_content(out, U("namespace ") + normalize_namespace(_namespace));
	output_line_content(out, U("{"));
	output_empty_line(out, 1);
}

void odata_codegen_writer::generate_implement_namespace_begin(::utility::ofstream_t& out, const ::utility::string_t& _namespace)
{
	generate_declare_namespace_begin(out, _namespace);
}
    
void odata_codegen_writer::generate_declare_include_files_end(::utility::ofstream_t& out)
{
    output_line_content(out, U("#include \"odata/codegen/odata_function_param_formatter.h\""));
    output_empty_line(out, 1);
}

void odata_codegen_writer::generate_declare_namespace_end(::utility::ofstream_t& out)
{
	output_line_content(out, U("}"));
}

void odata_codegen_writer::generate_implement_namespace_end(::utility::ofstream_t& out)
{
	generate_declare_namespace_end(out);
}

// class XXX;
// class YYY;
void odata_codegen_writer::generate_declare_class(::utility::ofstream_t& o_header, schema_info& _schema_info)
{
	for (auto class_iter = _schema_info.m_class_vec.cbegin(); class_iter != _schema_info.m_class_vec.cend(); class_iter++)
	{
		auto _class_info = *class_iter;

		if (_class_info._type != CLASS_TYPE::E_CLASS_COMPLEX)
		{
			continue;
		}

		o_header << U("class ") << _class_info._class_name << U(";") << std::endl;
	}

	output_empty_line(o_header, 1);

	for (auto class_iter = _schema_info.m_class_vec.cbegin(); class_iter != _schema_info.m_class_vec.cend(); class_iter++)
	{
		auto _class_info = *class_iter;

		if (_class_info._type != CLASS_TYPE::E_CLASS_ENTITY)
		{
			continue;
		}

		o_header << U("class ") << _class_info._class_name << U(";") << std::endl;
	}

	output_empty_line(o_header, 1);
}

void odata_codegen_writer::generate_enum_types(::utility::ofstream_t& o_header, ::utility::ofstream_t& o_cpp, schema_info& _schema_info)
{
	for (auto class_iter = _schema_info.m_class_vec.cbegin(); class_iter != _schema_info.m_class_vec.cend(); class_iter++)
	{
		auto _class_info = *class_iter;

		if (_class_info._type != CLASS_TYPE::E_CLASS_ENUM)
		{
			continue;
		}

		// generate .h
		generate_declare_enum_begin(o_header, _class_info);

		auto class_property_map = (_schema_info.m_class_property_map)[_class_info._class_name];

		for (auto property_iter = class_property_map.cbegin(); property_iter != class_property_map.cend(); property_iter++)
		{
			auto _property_info = property_iter->second;

			generate_declare_enum_member(o_header, _property_info);
		}

		generate_declare_enum_end(o_header);

		// generate .cpp
        generate_implement_enum_type_to_string_begin(o_cpp, _class_info);
		for (auto property_iter = class_property_map.cbegin(); property_iter != class_property_map.cend(); property_iter++)
		{
			auto _property_info = property_iter->second;

			generate_implement_enum_type_to_string_member(o_cpp, _class_info, _property_info);
		}
		generate_implement_enum_type_to_string_end(o_cpp, _class_info);

		generate_implement_string_to_enum_type_begin(o_cpp, _class_info);
		for (auto property_iter = class_property_map.cbegin(); property_iter != class_property_map.cend(); property_iter++)
		{
			auto _property_info = property_iter->second;

			generate_implement_string_to_enum_type_member(o_cpp, _class_info, _property_info);
		}
		generate_implement_string_to_enum_type_end(o_cpp, _class_info);
	}

	generate_declare_enum_resolver_begin(o_header);
	for (auto class_iter = _schema_info.m_class_vec.cbegin(); class_iter != _schema_info.m_class_vec.cend(); class_iter++)
	{
		auto _class_info = *class_iter;

		if (_class_info._type != CLASS_TYPE::E_CLASS_ENUM)
		{
			continue;
		}   

	    generate_declare_enum_resolver_on(o_header, _class_info);   
	}
	generate_declare_enum_resolver_end(o_header);
}

void odata_codegen_writer::generate_complex_types(::utility::ofstream_t& o_header, ::utility::ofstream_t& o_cpp, schema_info& _schema_info)
{
	for (auto class_iter = _schema_info.m_class_vec.cbegin(); class_iter != _schema_info.m_class_vec.cend(); class_iter++)
	{
		auto class_info = *class_iter;

		if (class_info._type != CLASS_TYPE::E_CLASS_COMPLEX)
		{
			continue;
		}

		// generate .h
		generate_declare_class_begin(o_header, class_info._class_name, class_info._base_class_name);

		generate_declare_common_functions_in_complex(o_header, class_info._class_name);

		auto class_property_map = _schema_info.m_class_property_map[class_info._class_name];

		// generate class member
		for (auto property_iter = class_property_map.cbegin(); property_iter != class_property_map.cend(); property_iter++)
		{
			generate_declare_class_member_in_complex(o_header, property_iter->second);
		}

		// output class end
		generate_declare_class_end(o_header);

		// generate .cpp
		generate_implement_begin_constructor_in_complex(o_cpp, class_info);
		for (auto property_iter = class_property_map.cbegin(); property_iter != class_property_map.cend(); property_iter++)
		{
			auto _property_info = property_iter->second;
			if (!_property_info._default_value.empty())
			{
				generate_implement_on_constructor_in_complex(o_cpp, _property_info);
			}
		}
		generate_implement_end_constructor_in_complex(o_cpp, class_info);

		generate_implement_begin_destructor_in_complex(o_cpp, class_info);
	    for (auto property_iter = class_property_map.cbegin(); property_iter != class_property_map.cend(); property_iter++)
		{
			auto _property_info = property_iter->second;
			if (_property_info._is_nullable && (_property_info._type == PROPERTY_TYPE::E_PRIMITIVE 
				|| _property_info._type == PROPERTY_TYPE::E_ENUM || _property_info._type == PROPERTY_TYPE::E_COMPLEX))
			{
				generate_implement_on_destructor_in_complex(o_cpp, _property_info);
			}
		}
		generate_implement_end_destructor_in_complex(o_cpp, class_info);

		for (auto property_iter = class_property_map.cbegin(); property_iter != class_property_map.cend(); property_iter++)
		{
			generate_implement_class_member_in_complex(o_cpp, class_info, property_iter->second);
		}

		generate_implement_common_functions_in_complex(o_cpp, class_info);

		generate_implement_begin_materialize_in_complex(o_cpp, class_info);

		for (auto property_iter = class_property_map.cbegin(); property_iter != class_property_map.cend(); property_iter++)
		{
			generate_implement_on_materialize_in_complex(o_cpp, class_info, property_iter->second);
		}

		generate_implement_end_materialize_in_complex(o_cpp, class_info);

		generate_implement_begin_serialize_in_complex(o_cpp, class_info);

		for (auto property_iter = class_property_map.cbegin(); property_iter != class_property_map.cend(); property_iter++)
		{
			generate_implement_on_serialize_in_complex(o_cpp, class_info, property_iter->second);
		}

		generate_implement_end_serialize_in_complex(o_cpp, class_info);
	}	
}

void odata_codegen_writer::generate_entity_types(::utility::ofstream_t& o_header, ::utility::ofstream_t& o_cpp, schema_info& _schema_info)
{
	for (auto class_iter = _schema_info.m_class_vec.cbegin(); class_iter != _schema_info.m_class_vec.cend(); class_iter++)
	{
		auto class_info = *class_iter;

		if (class_info._type != CLASS_TYPE::E_CLASS_ENTITY)
		{
			continue;
		}

		// generate .h
		generate_declare_class_begin(o_header, class_info._class_name, class_info._base_class_name);

		generate_declare_common_functions_in_entity(o_header, class_info._class_name);

		auto class_property_map = _schema_info.m_class_property_map[class_info._class_name];

		std::vector<property_info> keys;
		// generate class member
		for (auto property_iter = class_property_map.cbegin(); property_iter != class_property_map.cend(); property_iter++)
		{
			if (property_iter->second._is_key)
			{
				keys.push_back(property_iter->second);
			}

			generate_declare_class_member_in_entity(o_header, class_info, property_iter->second);
		}

		// generate keys infomation
		generate_declare_class_keys_in_entity(o_header, class_info, keys);

		// output class end
		generate_declare_class_end(o_header);

		// generate .cpp
		generate_implement_begin_constructor_in_entity(o_cpp, class_info);
		for (auto property_iter = class_property_map.cbegin(); property_iter != class_property_map.cend(); property_iter++)
		{
			auto _property_info = property_iter->second;
			if (!_property_info._default_value.empty())
			{
				generate_implement_on_constructor_in_entity(o_cpp, _property_info);
			}
		}
		generate_implement_end_constructor_in_entity(o_cpp, class_info);

		generate_implement_begin_destructor_in_entity(o_cpp, class_info);
	    for (auto property_iter = class_property_map.cbegin(); property_iter != class_property_map.cend(); property_iter++)
		{
			auto _property_info = property_iter->second;
			if (_property_info._is_nullable && (_property_info._type == PROPERTY_TYPE::E_PRIMITIVE 
				|| _property_info._type == PROPERTY_TYPE::E_ENUM || _property_info._type == PROPERTY_TYPE::E_COMPLEX))
			{
				generate_implement_on_destructor_in_entity(o_cpp, _property_info);
			}
		}
		generate_implement_end_destructor_in_entity(o_cpp, class_info);

		for (auto property_iter = class_property_map.cbegin(); property_iter != class_property_map.cend(); property_iter++)
		{
			generate_implement_class_member_in_entity(o_cpp, class_info, property_iter->second);
		}

		generate_implement_common_functions_in_entity(o_cpp, class_info);

		generate_implement_begin_materialize_in_entity(o_cpp, class_info);

		for (auto property_iter = class_property_map.cbegin(); property_iter != class_property_map.cend(); property_iter++)
		{
			generate_implement_on_materialize_in_entity(o_cpp, class_info, property_iter->second);
		}

		generate_implement_end_materialize_in_entity(o_cpp, class_info);

		generate_implement_begin_serialize_in_entity(o_cpp, class_info);

		for (auto property_iter = class_property_map.cbegin(); property_iter != class_property_map.cend(); property_iter++)
		{
			generate_implement_on_serialize_in_entity(o_cpp, class_info, property_iter->second);
		}

		generate_implement_end_serialize_in_entity(o_cpp, class_info);
	}	
}


//class InMemoryEntities : public odata_service_context
//{
//public:
//	InMemoryEntities(const ::utility::string_t& baseAddress, client_options options = client_options()) : odata_service_context(baseAddress, options)
//	{
//	}
void odata_codegen_writer::generate_entity_container_begin(::utility::ofstream_t& o_header, const class_info& _class_info)
{
	o_header << U("class ") << _class_info._class_name << U(" : public odata_service_context") << std::endl;
	o_header << U("{") << std::endl;
	o_header << U("public:") << std::endl;
	o_header << INDENT <<  _class_info._class_name << U("(const ::utility::string_t& baseAddress, client_options options = client_options()) : odata_service_context(baseAddress, options)") << std::endl;
	o_header << INDENT << U("{") << std::endl;
	o_header << INDENT << U("}") << std::endl << std::endl;
}

	//std::shared_ptr<odata_service_query<odata_entityset_query_executor<Product>, odata_query_builder>> create_products_query() 
	//{ 
	//	return create_query<odata_entityset_query_executor<Product>, odata_query_builder>(U("Products"));
	//}
void odata_codegen_writer::generate_declare_entity_set_in_entity_container(::utility::ofstream_t& out, const class_info& _class_info, const property_info& _property_info)
{
	out << INDENT << U("std::shared_ptr<odata_service_query<odata_entityset_query_executor<") << _property_info._strong_type_name << U(">, odata_query_builder>> ") << _property_info._class_member_name << std::endl;
	out << INDENT << U("{") << std::endl;
	out << INDENT << INDENT << U("return create_query<odata_entityset_query_executor<") << _property_info._strong_type_name << U(">, odata_query_builder>(U(\"") << _property_info._edm_name << U("\"));") << std::endl; 
	out << INDENT << U("}") << std::endl << std::endl;
}

	//std::shared_ptr<odata_service_query<odata_singleton_query_executor<Customer>, odata_query_builder>> create_vipcustomer_query() 
	//{ 
	//	return create_query<odata_singleton_query_executor<Customer>, odata_query_builder>(U("VipCustomer"));
	//}
void odata_codegen_writer::generate_declare_singleton_in_entity_container(::utility::ofstream_t& out, const class_info& _class_info, const property_info& _property_info)
{
	out << INDENT << U("std::shared_ptr<odata_service_query<odata_singleton_query_executor<") << _property_info._strong_type_name << U(">, odata_query_builder>> ") << _property_info._class_member_name << std::endl;
	out << INDENT << U("{") << std::endl;
	out << INDENT << INDENT << U("return create_query<odata_singleton_query_executor<") << _property_info._strong_type_name << U(">, odata_query_builder>(U(\"") << _property_info._edm_name << U("\"));") << std::endl; 
	out << INDENT << U("}") << std::endl << std::endl;
}

	//typedef odata_enum_query_executor<Color, enum_type_resolver> enum_GetDefaultColor_excecutor;
	//DECLARE_FUNCTION_IMPORT_P0(InMemoryEntities, GetDefaultColor, enum_GetDefaultColor_excecutor);
void odata_codegen_writer::generate_declare_operation_imports_in_entity_container(::utility::ofstream_t& out, const class_info& _class_info, const property_info& _property_info)
{
	auto _operation_info = _property_info._operation_info;

	if (!_operation_info)
	{
		return ;
	}

	::utility::string_t _declare_name;
	if (_property_info._type == PROPERTY_TYPE::E_ACTION)
	{
		_declare_name = U("DECLARE_ACTION_IMPORT_P");
	}
	else
	{
		_declare_name = U("DECLARE_FUNCTION_IMPORT_P");
	}

	// typedef odata_enum_query_executor<Color, enum_type_resolver> enum_GetDefaultColor_excecutor;
	if ( _operation_info->_executor_name == U("odata_enum_query_executor"))
	{
		// DECLARE_FUNCTION_IMPORT_P0(InMemoryEntities, GetDefaultColor, enum_GetDefaultColor_excecutor);
		out << INDENT << U("typedef odata_enum_query_executor<") << _operation_info->_return_type << U(", enum_type_resolver> ")
			<< U("enum_") << _property_info._class_member_name << U("_excecutor;") << std::endl;

	    out << INDENT << _declare_name << _operation_info->vec_params.size() << U("(") << _class_info._class_name << U(", ") << _property_info._class_member_name 
		    << U(", ") << U("enum_") << _property_info._class_member_name << U("_excecutor");
	}
	else if (_operation_info->_executor_name == U("odata_void_query_executor"))
	{
	    out << INDENT << _declare_name << _operation_info->vec_params.size() << U("(") << _class_info._class_name << U(", ") << _property_info._class_member_name 
		    << U(", ") << _operation_info->_executor_name;
	}
	else
	{
	    out << INDENT << _declare_name << _operation_info->vec_params.size() << U("(") << _class_info._class_name << U(", ") << _property_info._class_member_name 
		    << U(", ") << _operation_info->_executor_name << U("<") << _operation_info->_return_type << U(">");
	}

	for (auto param_iter = _operation_info->vec_params.cbegin(); param_iter != _operation_info->vec_params.cend(); param_iter++)
	{
		out << U(", ") << param_iter->_edm_name << U(", ") << param_iter->_member_strong_type_name << U(", ") <<  param_iter->_member_name;
	}

	out << U(");") << std::endl << std::endl;
}

void odata_codegen_writer::generate_implement_operation_imports_in_entity_container(::utility::ofstream_t& out, const class_info& _class_info, const property_info& _property_info)
{
	auto _operation_info = _property_info._operation_info;

	if (!_operation_info)
	{
		return ;
	}

	::utility::string_t _declare_name;
	if (_property_info._type == PROPERTY_TYPE::E_ACTION)
	{
		_declare_name = U("IMPLEMENT_ACTION_IMPORT_P");
	}
	else
	{
		_declare_name = U("IMPLEMENT_FUNCTION_IMPORT_P");
	}

	if ( _operation_info->_executor_name == U("odata_enum_query_executor"))
	{
	    out << _declare_name << _operation_info->vec_params.size() << U("(") << _class_info._class_name << U(", ") << _property_info._class_member_name 
		    << U(", ") << _class_info._class_name << U("::enum_") << _property_info._class_member_name << U("_excecutor");
	}
	else if (_operation_info->_executor_name == U("odata_void_query_executor"))
	{
	    out << _declare_name << _operation_info->vec_params.size() << U("(") << _class_info._class_name << U(", ") << _property_info._class_member_name 
		    << U(", ") << _operation_info->_executor_name;
	}
	else
	{
	    out << _declare_name << _operation_info->vec_params.size() << U("(") << _class_info._class_name << U(", ") << _property_info._class_member_name 
		    << U(", ") << _operation_info->_executor_name << U("<") << _operation_info->_return_type << U(">");
	}

	for (auto param_iter = _operation_info->vec_params.cbegin(); param_iter != _operation_info->vec_params.cend(); param_iter++)
	{
		out << U(", ") << param_iter->_edm_name << U(", ") << param_iter->_member_strong_type_name << U(", ") <<  param_iter->_member_name;
	}

	out << U(");") << std::endl << std::endl;
}

void odata_codegen_writer::generate_entity_container_end(::utility::ofstream_t& o_header, const class_info& _class_info)
{
	o_header << U("};") << std::endl << std::endl;
}

void odata_codegen_writer::generate_entity_container(::utility::ofstream_t& o_header, ::utility::ofstream_t& o_cpp, schema_info& _schema_info)
{
	for (auto class_iter = _schema_info.m_class_vec.cbegin(); class_iter != _schema_info.m_class_vec.cend(); class_iter++)
	{
		auto _class_info = *class_iter;

		if (_class_info._type != CLASS_TYPE::E_CLASS_CONTAINER)
		{
			continue;
		}

		generate_entity_container_begin(o_header, _class_info);

		auto class_property_map = (_schema_info.m_class_property_map)[_class_info._class_name];

		for (auto property_iter = class_property_map.cbegin(); property_iter != class_property_map.cend(); property_iter++)
		{
			auto _property_info = property_iter->second;

			if (_property_info._type == PROPERTY_TYPE::E_CONTAINER_ENTITY_SET)
			{
				generate_declare_entity_set_in_entity_container(o_header, _class_info, _property_info);
			}
			else if (_property_info._type == PROPERTY_TYPE::E_CONTAINER_SINGLETON)
			{
				generate_declare_singleton_in_entity_container(o_header, _class_info, _property_info);
			}
			else if (_property_info._type == PROPERTY_TYPE::E_ACTION || _property_info._type == PROPERTY_TYPE::E_FUNCTION)
			{
				generate_declare_operation_imports_in_entity_container(o_header, _class_info, _property_info);
				generate_implement_operation_imports_in_entity_container(o_cpp, _class_info, _property_info);
			}
			else
			{
				continue;
			}
		}

		generate_entity_container_end(o_header, _class_info);
	}
}

//DECLARE_DERIVED_ENTITY_CREATOR_FUNC(Customer, customer)
//DECLARE_DERIVED_ENTITY_CREATOR_FUNC(Employee, employee)
//DECLARE_DERIVED_ENTITY_CREATOR_FUNC(CreditCardPI, creditcardpi)
//
//IMPLEMENT_EMPTY_DERIVED_TYPE_CREATOR_MAP(Account)
//IMPLEMENT_EMPTY_DERIVED_TYPE_CREATOR_MAP(Product)
//IMPLEMENT_EMPTY_DERIVED_TYPE_CREATOR_MAP(ProductDetail)
//IMPLEMENT_EMPTY_DERIVED_TYPE_CREATOR_MAP(GiftCard)
//IMPLEMENT_EMPTY_DERIVED_TYPE_CREATOR_MAP(Customer)
//IMPLEMENT_EMPTY_DERIVED_TYPE_CREATOR_MAP(Employee)
//IMPLEMENT_EMPTY_DERIVED_TYPE_CREATOR_MAP(Company)
//IMPLEMENT_EMPTY_DERIVED_TYPE_CREATOR_MAP(Statement)
//IMPLEMENT_EMPTY_DERIVED_TYPE_CREATOR_MAP(StoredPI)
//IMPLEMENT_EMPTY_DERIVED_TYPE_CREATOR_MAP(CreditRecord)
//IMPLEMENT_EMPTY_DERIVED_TYPE_CREATOR_MAP(Order)
//
//BEGIN_IMPLEMENT_ENTITY_DERIVED_TYPE_CREATOR_MAP(Person)
//  ON_IMPLEMENT_ENTITY_DERIVED_TYPE_CREATOR_MAP(Customer, customer)
//	ON_IMPLEMENT_ENTITY_DERIVED_TYPE_CREATOR_MAP(Employee, employee)
//END_IMPLEMENT_ENTITY_DERIVED_TYPE_CREATOR_MAP(Person)
//
//BEGIN_IMPLEMENT_ENTITY_DERIVED_TYPE_CREATOR_MAP(PaymentInstrument)
//  ON_IMPLEMENT_ENTITY_DERIVED_TYPE_CREATOR_MAP(CreditCardPI, creditcardpi)
//END_IMPLEMENT_ENTITY_DERIVED_TYPE_CREATOR_MAP(PaymentInstrument)
//
//DECLARE_DERIVED_COMPLEX_CREATOR_FUNC(HomeAddress, homeaddress)
//
//BEGIN_IMPLEMENT_COMPLEX_DERIVED_TYPE_CREATOR_MAP(Address)
//  ON_IMPLEMENT_COMPLEX_DERIVED_TYPE_CREATOR_MAP(HomeAddress, homeaddress)
//END_IMPLEMENT_COMPLEX_DERIVED_TYPE_CREATOR_MAP(Address)
//
//IMPLEMENT_COMPLEX_DERIVED_TYPE_CREATOR_MAP(HomeAddress)
//IMPLEMENT_COMPLEX_DERIVED_TYPE_CREATOR_MAP(AccountInfo)
void odata_codegen_writer::generate_derived_creator(::utility::ofstream_t& o_cpp, schema_info& _schema_info)
{
	for (auto class_iter = _schema_info.m_class_vec.cbegin(); class_iter != _schema_info.m_class_vec.cend(); class_iter++)
	{
		auto _class_info = *class_iter;

		if (_class_info._type != CLASS_TYPE::E_CLASS_COMPLEX)
		{
			continue;
		}

		auto find_iter = _schema_info.m_derived_classes.find(_class_info._class_name);
		if (find_iter != _schema_info.m_derived_classes.end())
		{
			for (auto derived_iter = find_iter->second.cbegin(); derived_iter != find_iter->second.cend(); derived_iter++)
			{
				auto derived_class_name = *derived_iter; 
				::utility::string_t derived_class_creator_name = derived_class_name;
				std::transform(derived_class_name.begin(), derived_class_name.end(), derived_class_creator_name.begin(), ::tolower);
				o_cpp << U("DECLARE_DERIVED_COMPLEX_CREATOR_FUNC(") << derived_class_name << U(", ") <<  derived_class_creator_name << U(")") << std::endl;
			}
		}
	}

	for (auto class_iter = _schema_info.m_class_vec.cbegin(); class_iter != _schema_info.m_class_vec.cend(); class_iter++)
	{
		auto _class_info = *class_iter;

		if (_class_info._type != CLASS_TYPE::E_CLASS_COMPLEX)
		{
			continue;
		}

		auto find_iter = _schema_info.m_derived_classes.find(_class_info._class_name);
		if (find_iter == _schema_info.m_derived_classes.end())
		{
			o_cpp << U("IMPLEMENT_COMPLEX_DERIVED_TYPE_CREATOR_MAP(") << _class_info._class_name << U(")") << std::endl;
		}
		else
		{
			o_cpp << std::endl << U("BEGIN_IMPLEMENT_COMPLEX_DERIVED_TYPE_CREATOR_MAP(") << _class_info._class_name << U(")") << std::endl;

			for (auto derived_iter = find_iter->second.cbegin(); derived_iter != find_iter->second.cend(); derived_iter++)
			{
				auto derived_class_name = *derived_iter; 
				::utility::string_t derived_class_creator_name = derived_class_name;
				std::transform(derived_class_name.begin(), derived_class_name.end(), derived_class_creator_name.begin(), ::tolower);
				o_cpp << INDENT << U("ON_IMPLEMENT_COMPLEX_DERIVED_TYPE_CREATOR_MAP(") << derived_class_name << U(", ") <<  derived_class_creator_name << U(")") << std::endl;
			}

			o_cpp << U("END_IMPLEMENT_COMPLEX_DERIVED_TYPE_CREATOR_MAP(") << _class_info._class_name << U(")") << std::endl << std::endl;
		}
	}

	output_empty_line(o_cpp, 1);

	for (auto class_iter = _schema_info.m_class_vec.cbegin(); class_iter != _schema_info.m_class_vec.cend(); class_iter++)
	{
		auto _class_info = *class_iter;

		if (_class_info._type != CLASS_TYPE::E_CLASS_ENTITY)
		{
			continue;
		}

		auto find_iter = _schema_info.m_derived_classes.find(_class_info._class_name);
		if (find_iter != _schema_info.m_derived_classes.end())
		{
			for (auto derived_iter = find_iter->second.cbegin(); derived_iter != find_iter->second.cend(); derived_iter++)
			{
				auto derived_class_name = *derived_iter; 
				::utility::string_t derived_class_creator_name = derived_class_name;
				std::transform(derived_class_name.begin(), derived_class_name.end(), derived_class_creator_name.begin(), ::tolower);
				o_cpp << U("DECLARE_DERIVED_ENTITY_CREATOR_FUNC(") << derived_class_name << U(", ") <<  derived_class_creator_name << U(")") << std::endl;
			}
		}
	}

	for (auto class_iter = _schema_info.m_class_vec.cbegin(); class_iter != _schema_info.m_class_vec.cend(); class_iter++)
	{
		auto _class_info = *class_iter;

		if (_class_info._type != CLASS_TYPE::E_CLASS_ENTITY)
		{
			continue;
		}

		auto find_iter = _schema_info.m_derived_classes.find(_class_info._class_name);
		if (find_iter == _schema_info.m_derived_classes.end())
		{
			o_cpp << U("IMPLEMENT_EMPTY_DERIVED_TYPE_CREATOR_MAP(") << _class_info._class_name << U(")") << std::endl;
		}
		else
		{
			o_cpp << std::endl << U("BEGIN_IMPLEMENT_ENTITY_DERIVED_TYPE_CREATOR_MAP(") << _class_info._class_name << U(")") << std::endl;

			for (auto derived_iter = find_iter->second.cbegin(); derived_iter != find_iter->second.cend(); derived_iter++)
			{
				auto derived_class_name = *derived_iter; 
				::utility::string_t derived_class_creator_name = derived_class_name;
				std::transform(derived_class_name.begin(), derived_class_name.end(), derived_class_creator_name.begin(), ::tolower);
				o_cpp << INDENT << U("ON_IMPLEMENT_ENTITY_DERIVED_TYPE_CREATOR_MAP(") << derived_class_name << U(", ") <<  derived_class_creator_name << U(")") << std::endl;
			}

			o_cpp << U("END_IMPLEMENT_ENTITY_DERIVED_TYPE_CREATOR_MAP(") << _class_info._class_name << U(")") << std::endl << std::endl;
		}
	}
}

//class Item : public Entity
//{
//public: 

void odata_codegen_writer::generate_declare_class_begin(::utility::ofstream_t& out, const ::utility::string_t& class_name, const ::utility::string_t& base_class_name)
{
	out << U("class ") << class_name << U(" : public ") << base_class_name << std::endl;
	out << U("{") << std::endl;
	out << U("public:") << std::endl;
}

void odata_codegen_writer::generate_declare_class_end(::utility::ofstream_t& out)
{
	out << U("};") << std::endl << std::endl;
}

//DECLARE_COMPLEX_CONSTRUCTOR(ItemBody);
//DECLARE_COMPLEX_DESTRUCTOR(ItemBody);
//DECLARE_EDM_INFO();
//ENABLE_PROPERTY_IN_COMPLEX_MAPPING();
void odata_codegen_writer::generate_declare_common_functions_in_complex(::utility::ofstream_t& out, const ::utility::string_t& class_name)
{
	out << INDENT << U("DECLARE_COMPLEX_CONSTRUCTOR(") << class_name << U(");") << std::endl;
	out << INDENT << U("DECLARE_COMPLEX_DESTRUCTOR(") << class_name << U(");") << std::endl;
	out << INDENT << U("DECLARE_EDM_INFO();") << std::endl;
	out << INDENT << U("ENABLE_PROPERTY_IN_COMPLEX_MAPPING();") << std::endl << std::endl;
}

//DECLARE_NULLABLE_ENUM_PROPERTY_IN_COMPLEX_MAPPING(contenttype, ContentType, BodyType);
//DECLARE_NULLABLE_PRIMITIVE_PROPERTY_IN_COMPLEX_MAPPING(content, Content, ::utility::string_t);
void odata_codegen_writer::generate_declare_class_member_in_complex(::utility::ofstream_t& out, const property_info& _property_info)
{
	::utility::string_t member_macro;

	if (_property_info._type == PROPERTY_TYPE::E_PRIMITIVE)
	{
		if (_property_info._is_nullable)
		{
			member_macro = U("DECLARE_NULLABLE_PRIMITIVE_PROPERTY_IN_COMPLEX_MAPPING(");
		}
		else
		{
			member_macro = U("DECLARE_PRIMITIVE_PROPERTY_IN_COMPLEX_MAPPING(");
		}
	}
	else if (_property_info._type == PROPERTY_TYPE::E_COMPLEX)
	{
		if (_property_info._is_nullable)
		{
			member_macro = U("DECLARE_NULLABLE_COMPLEX_PROPERTY_IN_COMPLEX_MAPPING(");
		}
		else
		{
			member_macro = U("DECLARE_COMPLEX_PROPERTY_IN_COMPLEX_MAPPING(");
		}
	}
	else if (_property_info._type == PROPERTY_TYPE::E_ENUM)
	{
		if (_property_info._is_nullable)
		{
			member_macro = U("DECLARE_NULLABLE_ENUM_PROPERTY_IN_COMPLEX_MAPPING(");
		}
		else
		{
			member_macro = U("DECLARE_ENUM_PROPERTY_IN_COMPLEX_MAPPING(");
		}
	}
	else if (_property_info._type == PROPERTY_TYPE::E_COLLECTION_PRIMITIVE)
	{
		member_macro = U("DECLARE_COLLECTION_PRIMITIVE_PROPERTY_IN_COMPLEX_MAPPING(");
	}
	else if (_property_info._type == PROPERTY_TYPE::E_COLLECTION_ENUM)
	{
		member_macro = U("DECLARE_COLLECTION_ENUM_PROPERTY_IN_COMPLEX_MAPPING(");
	}

	out << INDENT << member_macro << _property_info._class_member_name << U(", ") << _property_info._edm_name << U(", ") << _property_info._strong_type_name << U(");") << std::endl;
}

//BEGIN_COMPLEX_CONSTRUCTOR(AccountInfo, type_base)
// ON_PROPERTY_IN_COMPLEX_CONSTRUCTOR(giftcardid, 0)
//END_COMPLEX_CONSTRUCTOR(AccountInfo, type_base)
void odata_codegen_writer::generate_implement_begin_constructor_in_complex(::utility::ofstream_t& out, const class_info& _class_info)
{
	out << U("BEGIN_COMPLEX_CONSTRUCTOR(") << _class_info._class_name << U(", ") << _class_info._base_class_name << U(")") << std::endl;
}

void odata_codegen_writer::generate_implement_on_constructor_in_complex(::utility::ofstream_t& out, const property_info& _property_info)
{
	out << INDENT << U("ON_PROPERTY_IN_COMPLEX_CONSTRUCTOR(") << _property_info._class_member_name << U(", ") << _property_info._default_value << U(")") << std::endl;
}

void odata_codegen_writer::generate_implement_end_constructor_in_complex(::utility::ofstream_t& out, const class_info& _class_info)
{
	out << U("END_COMPLEX_CONSTRUCTOR(") << _class_info._class_name << U(", ") << _class_info._base_class_name << U(")") << std::endl << std::endl;
}

void odata_codegen_writer::generate_implement_begin_destructor_in_complex(::utility::ofstream_t& out, const class_info& _class_info)
{
	out << U("BEGIN_COMPLEX_DESTRUCTOR(") << _class_info._class_name << U(")") << std::endl;
}

void odata_codegen_writer::generate_implement_on_destructor_in_complex(::utility::ofstream_t& out, const property_info& _property_info)
{
	out << INDENT << U("ON_PROPERTY_IN_COMPLEX_DESTRUCTOR(") << _property_info._class_member_name << U(")") << std::endl;
}

void odata_codegen_writer::generate_implement_end_destructor_in_complex(::utility::ofstream_t& out, const class_info& _class_info)
{
	out << U("END_COMPLEX_DESTRUCTOR(") << _class_info._class_name << U(")") << std::endl << std::endl;
}

//IMPLEMENT_EDM_INFO(Address, Microsoft.Test.OData.Services.ODataWCFService, Address)
void odata_codegen_writer::generate_implement_common_functions_in_complex(::utility::ofstream_t& out, const class_info& _class_info)
{
	out << std::endl << U("IMPLEMENT_EDM_INFO(") << _class_info._class_name << U(", ") << _class_info._edm_namespace << U(", ") << _class_info._edm_name << U(")")  << std::endl  << std::endl;
}

//IMPLEMENT_PRIMITIVE_PROPERTY_IN_COMPLEX_MAPPING(AccountInfo, firstname, FirstName, ::utility::string_t);
//IMPLEMENT_PRIMITIVE_PROPERTY_IN_COMPLEX_MAPPING(AccountInfo, lastname, LastName, ::utility::string_t);
void odata_codegen_writer::generate_implement_class_member_in_complex(::utility::ofstream_t& out, const class_info& _class_info, const property_info& _property_info)
{
	::utility::string_t member_macro;

	if (_property_info._type == PROPERTY_TYPE::E_PRIMITIVE)
	{
		if (_property_info._is_nullable)
		{
			member_macro = U("IMPLEMENT_NULLABLE_PRIMITIVE_PROPERTY_IN_COMPLEX_MAPPING(");
		}
		else
		{
			member_macro = U("IMPLEMENT_PRIMITIVE_PROPERTY_IN_COMPLEX_MAPPING(");
		}
	}
	else if (_property_info._type == PROPERTY_TYPE::E_COMPLEX)
	{
		if (_property_info._is_nullable)
		{
			member_macro = U("IMPLEMENT_NULLABLE_COMPLEX_PROPERTY_IN_COMPLEX_MAPPING(");
		}
		else
		{
			member_macro = U("IMPLEMENT_COMPLEX_PROPERTY_IN_COMPLEX_MAPPING(");
		}
	}
	else if (_property_info._type == PROPERTY_TYPE::E_ENUM)
	{
		if (_property_info._is_nullable)
		{
			member_macro = U("IMPLEMENT_NULLABLE_ENUM_PROPERTY_IN_COMPLEX_MAPPING(");
		}
		else
		{
			member_macro = U("IMPLEMENT_ENUM_PROPERTY_IN_COMPLEX_MAPPING(");
		}
	}
	else if (_property_info._type == PROPERTY_TYPE::E_COLLECTION_PRIMITIVE)
	{
		member_macro = U("IMPLEMENT_COLLECTION_PRIMITIVE_PROPERTY_IN_COMPLEX_MAPPING(");
	}
	else if (_property_info._type == PROPERTY_TYPE::E_COLLECTION_ENUM)
	{
		member_macro = U("IMPLEMENT_COLLECTION_ENUM_PROPERTY_IN_COMPLEX_MAPPING(");
	}

	out << member_macro << _class_info._class_name << U(", ") << _property_info._class_member_name << U(", ") << _property_info._edm_name << U(", ") <<_property_info._strong_type_name  << U(");") << std::endl;
}

//BEGIN_PROPERTY_IN_COMPLEX_MAPPING(Address)
//    ON_PROPERTY_IN_COMPLEX_MAPPING(Address, street)
//    ON_PROPERTY_IN_COMPLEX_MAPPING(Address, city)
//    ON_PROPERTY_IN_COMPLEX_MAPPING(Address, postalcode)
//END_PROPERTY_IN_COMPLEX_MAPPING(Address)
//or
// BEGIN_PROPERTY_IN_COMPLEX_MAPPING_WITH_BASE_CLASS(HomeAddress, Address)
//    ON_PROPERTY_IN_COMPLEX_MAPPING(HomeAddress, familyname)
//END_PROPERTY_IN_COMPLEX_MAPPING_WITH_BASE_CLASS(HomeAddress, Address)
void odata_codegen_writer::generate_implement_begin_materialize_in_complex(::utility::ofstream_t& out, const class_info& _class_info)
{
	if (_class_info._base_class_name != U("type_base"))
	{
		out << U("BEGIN_PROPERTY_IN_COMPLEX_MAPPING_WITH_BASE_CLASS(") << _class_info._class_name << U(", ") << _class_info._base_class_name << U(")") << std::endl;
	}
	else
	{
		out << U("BEGIN_PROPERTY_IN_COMPLEX_MAPPING(") << _class_info._class_name << U(")") << std::endl;
	} 
}

void odata_codegen_writer::generate_implement_on_materialize_in_complex(::utility::ofstream_t& out, const class_info& _class_info, const property_info& _property_info)
{
	if (_property_info._type == PROPERTY_TYPE::E_ACTION || _property_info._type == PROPERTY_TYPE::E_FUNCTION)
	{
		return ;
	}

	out << INDENT << U("ON_PROPERTY_IN_COMPLEX_MAPPING(") << _class_info._class_name << U(", ") << _property_info._class_member_name << U(")") << std::endl;
}

void odata_codegen_writer::generate_implement_end_materialize_in_complex(::utility::ofstream_t& out, const class_info& _class_info)
{
	if (_class_info._base_class_name != U("type_base"))
	{
		out << U("END_PROPERTY_IN_COMPLEX_MAPPING_WITH_BASE_CLASS(") << _class_info._class_name << U(", ") << _class_info._base_class_name << U(")") << std::endl;
	}
	else
	{
		out << U("END_PROPERTY_IN_COMPLEX_MAPPING(") << _class_info._class_name << U(")") << std::endl << std::endl;
	}  
}

//BEGIN_SERIALIZE_PROPERTY_IN_COMPLEX_MAPPING(Address)
//	ON_SERIALIZE_PROPERTY_IN_COMPLEX_MAPPING(Address, street)
//	ON_SERIALIZE_PROPERTY_IN_COMPLEX_MAPPING(Address, city)
//	ON_SERIALIZE_PROPERTY_IN_COMPLEX_MAPPING(Address, postalcode)
//END_SERIALIZE_PROPERTY_IN_COMPLEX_MAPPING(Address)
void odata_codegen_writer::generate_implement_begin_serialize_in_complex(::utility::ofstream_t& out, const class_info& _class_info)
{
	out << U("BEGIN_SERIALIZE_PROPERTY_IN_COMPLEX_MAPPING(") << _class_info._class_name << U(")") << std::endl;
}

void odata_codegen_writer::generate_implement_on_serialize_in_complex(::utility::ofstream_t& out, const class_info& _class_info, const property_info& _property_info)
{
	if (_property_info._type == PROPERTY_TYPE::E_ACTION || _property_info._type == PROPERTY_TYPE::E_FUNCTION)
	{
		return ;
	}

	out << INDENT << U("ON_SERIALIZE_PROPERTY_IN_COMPLEX_MAPPING(") << _class_info._class_name << U(", ") << _property_info._class_member_name << U(")") << std::endl;
}

void odata_codegen_writer::generate_implement_end_serialize_in_complex(::utility::ofstream_t& out, const class_info& _class_info)
{
	out << U("END_SERIALIZE_PROPERTY_IN_COMPLEX_MAPPING(") << _class_info._class_name << U(")") << std::endl << std::endl;
}

//DECLARE_ENTITY_CONSTRUCTOR(Person);
//DECLARE_ENTITY_DESTRUCTOR(Person);
//DECLARE_EDM_INFO();
//ENABLE_PROPERTY_IN_ENTITY_MAPPING();	
void odata_codegen_writer::generate_declare_common_functions_in_entity(::utility::ofstream_t& out, const ::utility::string_t& class_name)
{
	out << INDENT << U("DECLARE_ENTITY_CONSTRUCTOR(") << class_name << U(");") << std::endl;
	out << INDENT << U("DECLARE_ENTITY_DESTRUCTOR(") << class_name << U(");") << std::endl;
	out << INDENT << U("DECLARE_EDM_INFO();") << std::endl;
	out << INDENT << U("ENABLE_PROPERTY_IN_ENTITY_MAPPING();") << std::endl << std::endl;
}

//DECLARE_GET_KEY_PROPERTY_STRING_ONE_PARAM(type_base, PersonID, personid);
void odata_codegen_writer::generate_declare_class_keys_in_entity(::utility::ofstream_t& out, const class_info& _class_info, const std::vector<property_info>& keys)
{
	if (keys.size() > 4)
	{
		throw std::runtime_error("too many keys. max key size is 4. you have to add support for more keys param yourself in code_generation.h");
	}

	out << std::endl;

	if (keys.size() == 0)
	{
		out << INDENT << U("DECLARE_GET_KEY_PROPERTY_STRING_NO_PARAM(");
	}
	else if (keys.size() == 1)
	{
		out << INDENT << U("DECLARE_GET_KEY_PROPERTY_STRING_ONE_PARAM(");
	}
	else if (keys.size() == 2)
	{
		out << INDENT << U("DECLARE_GET_KEY_PROPERTY_STRING_TWO_PARAM(");
	}
	else if (keys.size() == 3)
	{
		out << INDENT << U("DECLARE_GET_KEY_PROPERTY_STRING_THREE_PARAM(");
	}
	else if (keys.size() == 4)
	{
		out << INDENT << U("DECLARE_GET_KEY_PROPERTY_STRING_FOUR_PARAM(");
	}

	out << _class_info._base_class_name;

	for (auto iter_keys = keys.cbegin(); iter_keys != keys.cend(); iter_keys++)
	{
		out << U(", ") << iter_keys->_edm_name << U(", ") << iter_keys->_class_member_name;
	}

	out << U(");") << std::endl;
}

void odata_codegen_writer::generate_declare_class_member_in_entity(::utility::ofstream_t& out, const class_info& _class_info, const property_info& _property_info)
{
	if (_property_info._type == PROPERTY_TYPE::E_ACTION || _property_info._type == PROPERTY_TYPE::E_FUNCTION)
	{
	    generate_declare_operation_in_entity(out, _class_info, _property_info);

		return ;
	}

	::utility::string_t member_macro;

	if (_property_info._type == PROPERTY_TYPE::E_PRIMITIVE)
	{
		if (_property_info._is_nullable)
		{
			member_macro = U("DECLARE_NULLABLE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(");
		}
		else
		{
			member_macro = U("DECLARE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(");
		}
	}
	else if (_property_info._type == PROPERTY_TYPE::E_ENUM)
	{
		if (_property_info._is_nullable)
		{
			member_macro = U("DECLARE_NULLABLE_ENUM_PROPERTY_IN_ENTITY_MAPPING(");
		}
		else
		{
			member_macro = U("DECLARE_ENUM_PROPERTY_IN_ENTITY_MAPPING(");
		}
	}
	else if (_property_info._type == PROPERTY_TYPE::E_COMPLEX)
	{
		if (_property_info._is_nullable)
		{
			member_macro = U("DECLARE_NULLABLE_COMPLEX_PROPERTY_IN_ENTITY_MAPPING(");
		}
		else
		{
			member_macro = U("DECLARE_COMPLEX_PROPERTY_IN_ENTITY_MAPPING(");
		}
	}
	else if (_property_info._type == PROPERTY_TYPE::E_ENTITY)
	{
		member_macro = U("DECLARE_NAVIGATION_PROPERTY_IN_ENTITY_MAPPING(");
	}
	else if (_property_info._type == PROPERTY_TYPE::E_COLLECTION_PRIMITIVE)
	{
		member_macro = U("DECLARE_COLLECTION_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(");
	}
	else if (_property_info._type == PROPERTY_TYPE::E_COLLECTION_ENUM)
	{
		member_macro = U("DECLARE_COLLECTION_ENUM_PROPERTY_IN_ENTITY_MAPPING(");
	}
	else if (_property_info._type == PROPERTY_TYPE::E_COLLECTION_COMPLEX)
	{
		member_macro = U("DECLARE_COLLECTION_COMPLEX_PROPERTY_IN_ENTITY_MAPPING(");
	}
	else if (_property_info._type == PROPERTY_TYPE::E_COLLECTION_ENTITY)
	{
		member_macro = U("DECLARE_COLLECTION_NAVIGATION_PROPERTY_IN_ENTITY_MAPPING(");
	}

	out << INDENT << member_macro << _property_info._class_member_name << U(", ") << _property_info._edm_name << U(", ") << _property_info._strong_type_name << U(");") << std::endl;
}

//DECLARE_FUNCTION_P1(Product, GetProductDetails, odata_entityset_query_executor<ProductDetail>, count, int, count);
void odata_codegen_writer::generate_declare_operation_in_entity(::utility::ofstream_t& out, const class_info& _class_info, const property_info& _property_info)
{
	auto _operation_info = _property_info._operation_info;

	if (!_operation_info)
	{
		return ;
	}

	::utility::string_t _declare_name;
	if (_property_info._type == PROPERTY_TYPE::E_ACTION)
	{
		_declare_name = U("DECLARE_ACTION_P");
	}
	else
	{
		_declare_name = U("DECLARE_FUNCTION_P");
	}

	// typedef odata_enum_query_executor<Color, enum_type_resolver> enum_GetDefaultColor_excecutor;
	if ( _operation_info->_executor_name == U("odata_enum_query_executor"))
	{
		// DECLARE_FUNCTION_IMPORT_P0(InMemoryEntities, GetDefaultColor, enum_GetDefaultColor_excecutor);
		out << INDENT << U("typedef odata_enum_query_executor<") << _operation_info->_return_type << U(", enum_type_resolver> ")
			<< U("enum_") << _property_info._class_member_name << U("_excecutor;") << std::endl;

	    out << INDENT << _declare_name << _operation_info->vec_params.size() << U("(") << _class_info._class_name << U(", ") << _property_info._class_member_name 
		    << U(", ") << U("enum_") << _property_info._class_member_name << U("_excecutor");
	}
	else if (_operation_info->_executor_name == U("odata_void_query_executor"))
	{
	    out << INDENT << _declare_name << _operation_info->vec_params.size() << U("(") << _class_info._class_name << U(", ") << _property_info._class_member_name 
		    << U(", ") << _operation_info->_executor_name;
	}
	else
	{
	    out << INDENT << _declare_name << _operation_info->vec_params.size() << U("(") << _class_info._class_name << U(", ") << _property_info._class_member_name 
		    << U(", ") << _operation_info->_executor_name << U("<") << _operation_info->_return_type << U(">");
	}

	for (auto param_iter = _operation_info->vec_params.cbegin(); param_iter != _operation_info->vec_params.cend(); param_iter++)
	{
		out << U(", ") << param_iter->_edm_name << U(", ") << param_iter->_member_strong_type_name << U(", ") <<  param_iter->_member_name;
	}

	out << U(");") << std::endl;
}

//IMPLEMENT_EDM_INFO(Address, Microsoft.Test.OData.Services.ODataWCFService, Address)
void odata_codegen_writer::generate_implement_common_functions_in_entity(::utility::ofstream_t& out, const class_info& _class_info)
{
	generate_implement_common_functions_in_complex(out, _class_info);
}

void odata_codegen_writer::generate_implement_class_member_in_entity(::utility::ofstream_t& out, const class_info& _class_info, const property_info& _property_info)
{
	if (_property_info._type == PROPERTY_TYPE::E_ACTION || _property_info._type == PROPERTY_TYPE::E_FUNCTION)
	{
		generate_implement_operation_in_entity(out, _class_info, _property_info);

		return ;
	}

	::utility::string_t member_macro;

	if (_property_info._type == PROPERTY_TYPE::E_PRIMITIVE)
	{
		if (_property_info._is_nullable)
		{
			member_macro = U("IMPLEMENT_NULLABLE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(");
		}
		else
		{
			member_macro = U("IMPLEMENT_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(");
		}
	}
	else if (_property_info._type == PROPERTY_TYPE::E_ENUM)
	{
		if (_property_info._is_nullable)
		{
			member_macro = U("IMPLEMENT_NULLABLE_ENUM_PROPERTY_IN_ENTITY_MAPPING(");
		}
		else
		{
			member_macro = U("IMPLEMENT_ENUM_PROPERTY_IN_ENTITY_MAPPING(");
		}
	}
	else if (_property_info._type == PROPERTY_TYPE::E_COMPLEX)
	{
		if (_property_info._is_nullable)
		{
			member_macro = U("IMPLEMENT_NULLABLE_COMPLEX_PROPERTY_IN_ENTITY_MAPPING(");
		}
		else
		{
			member_macro = U("IMPLEMENT_COMPLEX_PROPERTY_IN_ENTITY_MAPPING(");
		}
	}
	else if (_property_info._type == PROPERTY_TYPE::E_ENTITY)
	{
		member_macro = U("IMPLEMENT_NAVIGATION_PROPERTY_IN_ENTITY_MAPPING(");
	}
	else if (_property_info._type == PROPERTY_TYPE::E_COLLECTION_PRIMITIVE)
	{
		member_macro = U("IMPLEMENT_COLLECTION_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(");
	}
	else if (_property_info._type == PROPERTY_TYPE::E_COLLECTION_ENUM)
	{
		member_macro = U("IMPLEMENT_COLLECTION_ENUM_PROPERTY_IN_ENTITY_MAPPING(");
	}
	else if (_property_info._type == PROPERTY_TYPE::E_COLLECTION_COMPLEX)
	{
		member_macro = U("IMPLEMENT_COLLECTION_COMPLEX_PROPERTY_IN_ENTITY_MAPPING(");
	}
	else if (_property_info._type == PROPERTY_TYPE::E_COLLECTION_ENTITY)
	{
		member_macro = U("IMPLEMENT_COLLECTION_NAVIGATION_PROPERTY_IN_ENTITY_MAPPING(");
	}

	out << member_macro << _class_info._class_name << U(", ") << _property_info._class_member_name << U(", ") << _property_info._edm_name << U(", ") <<_property_info._strong_type_name  << U(");") << std::endl;
}

//IMPLEMENT_FUNCTION_P1(Product, GetProductDetails, odata_entityset_query_executor<ProductDetail>, count, int, count);
void odata_codegen_writer::generate_implement_operation_in_entity(::utility::ofstream_t& out, const class_info& _class_info, const property_info& _property_info)
{
	auto _operation_info = _property_info._operation_info;

	if (!_operation_info)
	{
		return ;
	}

	::utility::string_t _declare_name;
	if (_property_info._type == PROPERTY_TYPE::E_ACTION)
	{
		_declare_name = U("IMPLEMENT_ACTION_P");
	}
	else
	{
		_declare_name = U("IMPLEMENT_FUNCTION_P");
	}

	if (_operation_info->_executor_name == U("odata_enum_query_executor"))
	{
	    out << _declare_name << _operation_info->vec_params.size() << U("(") << _class_info._class_name << U(", ") << _property_info._class_member_name 
		    << U(", ") << _class_info._class_name << U("::enum_") << _property_info._class_member_name << U("_excecutor");
	}
	else if (_operation_info->_executor_name == U("odata_void_query_executor"))
	{
	    out << _declare_name << _operation_info->vec_params.size() << U("(") << _class_info._class_name << U(", ") << _property_info._class_member_name 
		    << U(", ") << _operation_info->_executor_name;
	}
	else
	{
	    out << _declare_name << _operation_info->vec_params.size() << U("(") << _class_info._class_name << U(", ") << _property_info._class_member_name 
		    << U(", ") << _operation_info->_executor_name << U("<") << _operation_info->_return_type << U(">");
	}

	for (auto param_iter = _operation_info->vec_params.cbegin(); param_iter != _operation_info->vec_params.cend(); param_iter++)
	{
		out << U(", ") << param_iter->_edm_name << U(", ") << param_iter->_member_strong_type_name << U(", ") <<  param_iter->_member_name;
	}

	out << U(");") << std::endl;
}

//BEGIN_ENTITY_CONSTRUCTOR(GiftCard, type_base)
//    ON_PROPERTY_IN_ENTITY_CONSTRUCTOR(giftcardid, 0)
//    ON_PROPERTY_IN_ENTITY_CONSTRUCTOR(amount, 0.0)
//END_ENTITY_CONSTRUCTOR(GiftCard, type_base)
void odata_codegen_writer::generate_implement_begin_constructor_in_entity(::utility::ofstream_t& out, const class_info& _class_info)
{
    out << U("BEGIN_ENTITY_CONSTRUCTOR(") << _class_info._class_name << U(", ") << _class_info._base_class_name << U(")") << std::endl;
}

void odata_codegen_writer::generate_implement_on_constructor_in_entity(::utility::ofstream_t& out, const property_info& _property_info)
{
	out << INDENT << U("ON_PROPERTY_IN_ENTITY_CONSTRUCTOR(") << _property_info._class_member_name << U(", ") << _property_info._default_value << U(")") << std::endl;
}

void odata_codegen_writer::generate_implement_end_constructor_in_entity(::utility::ofstream_t& out, const class_info& _class_info)
{
	out << U("END_ENTITY_CONSTRUCTOR(") << _class_info._class_name << U(", ") << _class_info._base_class_name << U(")") << std::endl<< std::endl;
}

void odata_codegen_writer::generate_implement_begin_destructor_in_entity(::utility::ofstream_t& out, const class_info& _class_info)
{
	out << U("BEGIN_ENTITY_DESTRUCTOR(") << _class_info._class_name << U(")") << std::endl;
}

void odata_codegen_writer::generate_implement_on_destructor_in_entity(::utility::ofstream_t& out, const property_info& _property_info)
{
	out << INDENT << U("ON_PROPERTY_IN_ENTITY_DESTRUCTOR(") << _property_info._class_member_name << U(")") << std::endl;
}

void odata_codegen_writer::generate_implement_end_destructor_in_entity(::utility::ofstream_t& out, const class_info& _class_info)
{
	out << U("END_ENTITY_DESTRUCTOR(") << _class_info._class_name << U(")") << std::endl<< std::endl;
}

//BEGIN_PROPERTY_IN_ENTITY_MAPPING(Account)
//    ON_PROPERTY_IN_ENTITY_MAPPING(Account, accountid)
//    ON_PROPERTY_IN_ENTITY_MAPPING(Account, countryregion)
//	ON_PROPERTY_IN_ENTITY_MAPPING(Account, accountinfo)
//	ON_PROPERTY_IN_ENTITY_MAPPING(Account, giftcard)
//	ON_PROPERTY_IN_ENTITY_MAPPING(Account, mypaymentinstruments)
//END_PROPERTY_IN_ENTITY_MAPPING(Account)
// or
////BEGIN_PROPERTY_IN_ENTITY_MAPPING_WITH_BASE_CLASS(CreditCardPI, PaymentInstrument)
//    ON_PROPERTY_IN_ENTITY_MAPPING(CreditCardPI, cardnumber)
//    ON_PROPERTY_IN_ENTITY_MAPPING(CreditCardPI, cvv)
//END_PROPERTY_IN_ENTITY_MAPPING_WITH_BASE_CLASS(CreditCardPI, PaymentInstrument)
void odata_codegen_writer::generate_implement_begin_materialize_in_entity(::utility::ofstream_t& out, const class_info& _class_info)
{
	if (_class_info._base_class_name != U("type_base"))
	{
		out << U("BEGIN_PROPERTY_IN_ENTITY_MAPPING_WITH_BASE_CLASS(") << _class_info._class_name << U(", ") << _class_info._base_class_name << U(")") << std::endl;
	}
	else
	{
		out << U("BEGIN_PROPERTY_IN_ENTITY_MAPPING(") << _class_info._class_name << U(")") << std::endl;
	}   
}

void odata_codegen_writer::generate_implement_on_materialize_in_entity(::utility::ofstream_t& out, const class_info& _class_info, const property_info& _property_info)
{
	if (_property_info._type == PROPERTY_TYPE::E_ACTION || _property_info._type == PROPERTY_TYPE::E_FUNCTION)
	{
		return ;
	}

	out << INDENT << U("ON_PROPERTY_IN_ENTITY_MAPPING(") << _class_info._class_name << U(", ") << _property_info._class_member_name << U(")") << std::endl;
}



void odata_codegen_writer::generate_implement_end_materialize_in_entity(::utility::ofstream_t& out, const class_info& _class_info)
{
	if (_class_info._base_class_name != U("type_base"))
	{
		out << U("END_PROPERTY_IN_ENTITY_MAPPING_WITH_BASE_CLASS(") << _class_info._class_name << U(", ") << _class_info._base_class_name << U(")") << std::endl;
	}
	else
	{
		out << U("END_PROPERTY_IN_ENTITY_MAPPING(") << _class_info._class_name << U(")") << std::endl << std::endl;
	}  
	
}

//BEGIN_SERIALIZE_PROPERTY_IN_ENTITY_MAPPING(Account)
//    ON_SERIALIZE_PROPERTY_IN_ENTITY_MAPPING(Account, accountid)
//    ON_SERIALIZE_PROPERTY_IN_ENTITY_MAPPING(Account, countryregion)
//	ON_SERIALIZE_PROPERTY_IN_ENTITY_MAPPING(Account, accountinfo)
//END_SERIALIZE_PROPERTY_IN_ENTITY_MAPPING(Account)
void odata_codegen_writer::generate_implement_begin_serialize_in_entity(::utility::ofstream_t& out, const class_info& _class_info)
{
	out << U("BEGIN_SERIALIZE_PROPERTY_IN_ENTITY_MAPPING(") << _class_info._class_name << U(")") << std::endl;
}

void odata_codegen_writer::generate_implement_on_serialize_in_entity(::utility::ofstream_t& out, const class_info& _class_info, const property_info& _property_info)
{
	if (_property_info._type == PROPERTY_TYPE::E_ACTION || _property_info._type == PROPERTY_TYPE::E_FUNCTION 
		|| _property_info._type == PROPERTY_TYPE::E_ENTITY || _property_info._type == PROPERTY_TYPE::E_COLLECTION_ENTITY)
	{
		return ;
	}

	out << INDENT << U("ON_SERIALIZE_PROPERTY_IN_ENTITY_MAPPING(") << _class_info._class_name << U(", ") << _property_info._class_member_name << U(")") << std::endl;
}

void odata_codegen_writer::generate_implement_end_serialize_in_entity(::utility::ofstream_t& out, const class_info& _class_info)
{
	out << U("END_SERIALIZE_PROPERTY_IN_ENTITY_MAPPING(") << _class_info._class_name << U(")") << std::endl << std::endl;
}

void odata_codegen_writer::generate_declare_enum_begin(::utility::ofstream_t& out, const class_info& _class_info)
{
	out << U("enum ") << _class_info._class_name << std::endl;
	out << U("{") << std::endl;
}

void odata_codegen_writer::generate_declare_enum_member(::utility::ofstream_t& out, const property_info& _property_info)
{
	out << INDENT << _property_info._class_member_name << U(" = ") << _property_info._default_value << U(",") << std::endl;
}

void odata_codegen_writer::generate_declare_enum_end(::utility::ofstream_t& out)
{
	out << U("};") << std::endl << std::endl;
}

void odata_codegen_writer::generate_declare_enum_resolver_begin(::utility::ofstream_t& out)
{
	out << U("class enum_type_resolver") << std::endl;
	out << U("{") << std::endl;
	out << U("public:") << std::endl;
}

void odata_codegen_writer::generate_declare_enum_resolver_on(::utility::ofstream_t& out, const class_info& _class_info)
{
	out << INDENT << U("DECLARE_FUNCTION_ENUM_TYPE_FROM_STRING(") << _class_info._class_name << U(");") << std::endl;
	out << INDENT << U("DECLARE_FUNCTION_STRING_FROM_ENUM_TYPE(") << _class_info._class_name << U(");") << std::endl;
	out << INDENT << U("DECLARE_GET_ENUM_TYPE_NAMESPACE(") << _class_info._class_name << U(", ") << _class_info._edm_namespace << U(");") << std::endl;
}

void odata_codegen_writer::generate_declare_enum_resolver_end(::utility::ofstream_t& out)
{
	out << U("};") << std::endl << std::endl;
}

//BGEIN_IMPLEMENT_FUNCTION_ENUM_TYPE_FROM_STRING(Color)
//    ON_IMPLEMENT_FUNCTION_ENUM_TYPE_FROM_STRING(Red, Color::Red)
//    ON_IMPLEMENT_FUNCTION_ENUM_TYPE_FROM_STRING(Green, Color::Green)
//    ON_IMPLEMENT_FUNCTION_ENUM_TYPE_FROM_STRING(Blue, Color::Blue)
//END_IMPLEMENT_FUNCTION_ENUM_TYPE_FROM_STRING(Color)
void odata_codegen_writer::generate_implement_enum_type_to_string_begin(::utility::ofstream_t& out, const class_info& _class_info)
{
	out << U("IMPLEMENT_GET_ENUM_TYPE_NAMESPACE(") << _class_info._class_name << U(", ") << _class_info._edm_namespace << U(");") << std::endl;
	out << U("BGEIN_IMPLEMENT_FUNCTION_ENUM_TYPE_FROM_STRING(") << _class_info._class_name << U(")") << std::endl;
}

void odata_codegen_writer::generate_implement_enum_type_to_string_member(::utility::ofstream_t& out, const class_info& _class_info, const property_info& _property_info)
{
	out << INDENT << U("ON_IMPLEMENT_FUNCTION_ENUM_TYPE_FROM_STRING(") << _property_info._class_member_name << U(", ") << _class_info._class_name << U("::") << _property_info._class_member_name << U(")") << std::endl;
}

void odata_codegen_writer::generate_implement_enum_type_to_string_end(::utility::ofstream_t& out, const class_info& _class_info)
{
	out << U("END_IMPLEMENT_FUNCTION_ENUM_TYPE_FROM_STRING(") << _class_info._class_name << U(")") << std::endl << std::endl;
}

//BGEIN_IMPLEMENT_FUNCTION_STRING_FROM_ENUM_TYPE(Color)
//    ON_IMPLEMENT_FUNCTION_STRING_FROM_ENUM_TYPE(Color::Red, Red)
//    ON_IMPLEMENT_FUNCTION_STRING_FROM_ENUM_TYPE(Color::Green, Green)
//    ON_IMPLEMENT_FUNCTION_STRING_FROM_ENUM_TYPE(Color::Blue, Blue)
//END_IMPLEMENT_FUNCTION_STRING_FROM_ENUM_TYPE(Color)
void odata_codegen_writer::generate_implement_string_to_enum_type_begin(::utility::ofstream_t& out, const class_info& _class_info)
{
	out << U("BGEIN_IMPLEMENT_FUNCTION_STRING_FROM_ENUM_TYPE(") << _class_info._class_name << U(")") << std::endl;
}

void odata_codegen_writer::generate_implement_string_to_enum_type_member(::utility::ofstream_t& out, const class_info& _class_info, const property_info& _property_info)
{
	
	out << INDENT << U("ON_IMPLEMENT_FUNCTION_STRING_FROM_ENUM_TYPE(") << _class_info._class_name << U("::") << _property_info._class_member_name << U(", ") << _property_info._class_member_name << U(")") << std::endl;
}

void odata_codegen_writer::generate_implement_string_to_enum_type_end(::utility::ofstream_t& out, const class_info& _class_info)
{
	out << U("END_IMPLEMENT_FUNCTION_STRING_FROM_ENUM_TYPE(") << _class_info._class_name << U(")") << std::endl << std::endl;
}

void odata_codegen_writer::output_line_content(::utility::ostream_t& out, const ::utility::string_t& content, unsigned int indent_number)
{
	for (unsigned int i = 0; i < indent_number; i++)
	{
		out << INDENT;
	}

	out  << content << std::endl;
}

void odata_codegen_writer::output_empty_line(::utility::ostream_t& out, unsigned int line_number)
{
	for (unsigned int i = 0; i < line_number; i++)
	{
		out << std::endl;
	}	
}

}}}