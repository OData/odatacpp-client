#pragma once
#include "odata/client/odata_client.h"
#include "odata/common/utility.h"
#include "odata/core/odata_core.h"
#include "odata/edm/odata_edm.h"
#include "odata_codegen_initializer.h"

namespace odata { namespace codegen { namespace tools {



class odata_codegen_writer
{
public:
	odata_codegen_writer(const std::shared_ptr<odata_codegen_initializer>& initializer) : m_initializer(initializer)
	{
	};

	::pplx::task<int> begin_generate_file();

private:
	void generate_declare_include_files(::utility::ofstream_t& out);
	void generate_implement_include_files(::utility::ofstream_t& out);
	void generate_declare_namespace_begin(::utility::ofstream_t& out, const ::utility::string_t& _namespace);
	void generate_implement_namespace_begin(::utility::ofstream_t& out, const ::utility::string_t& _namespace);
	void generate_declare_namespace_end(::utility::ofstream_t& out);
	void generate_implement_namespace_end(::utility::ofstream_t& out);

	void generate_declare_class(::utility::ofstream_t& o_header, schema_info& _schema_info);
	void generate_enum_types(::utility::ofstream_t& o_header, ::utility::ofstream_t& o_cpp, schema_info& _schema_info);
	void generate_complex_types(::utility::ofstream_t& o_header, ::utility::ofstream_t& o_cpp, schema_info& _schema_info);
	void generate_entity_types(::utility::ofstream_t& o_header, ::utility::ofstream_t& o_cpp, schema_info& _schema_info);
	void generate_entity_container(::utility::ofstream_t& o_header, ::utility::ofstream_t& o_cpp, schema_info& _schema_info);
	void generate_entity_container_begin(::utility::ofstream_t& o_header, const class_info& _class_info);
	void generate_entity_container_end(::utility::ofstream_t& o_header, const class_info& _class_info);
	void generate_derived_creator(::utility::ofstream_t& o_cpp, schema_info& _schema_info);
	void generate_declare_entity_set_in_entity_container(::utility::ofstream_t& out, const class_info& _class_info, const property_info& _property_info);
	void generate_declare_singleton_in_entity_container(::utility::ofstream_t& out, const class_info& _class_info, const property_info& _property_info);
	void generate_declare_operation_imports_in_entity_container(::utility::ofstream_t& out, const class_info& _class_info, const property_info& _property_info);
	void generate_implement_operation_imports_in_entity_container(::utility::ofstream_t& out, const class_info& _class_info, const property_info& _property_info);

	// common headers for complex and entity
	void generate_declare_class_begin(::utility::ofstream_t& out, const ::utility::string_t& class_name, const ::utility::string_t& base_class_name);
	void generate_declare_class_end(::utility::ofstream_t& out);

	// complex type
	void generate_declare_common_functions_in_complex(::utility::ofstream_t& out, const ::utility::string_t& class_name);
	void generate_declare_class_member_in_complex(::utility::ofstream_t& out, const property_info& _property_info);
	void generate_implement_common_functions_in_complex(::utility::ofstream_t& out, const class_info& _class_info);
	void generate_implement_class_member_in_complex(::utility::ofstream_t& out, const class_info& _class_info, const property_info& _property_info);
	void generate_implement_begin_constructor_in_complex(::utility::ofstream_t& out, const class_info& _class_info);
	void generate_implement_on_constructor_in_complex(::utility::ofstream_t& out, const property_info& _property_info);
	void generate_implement_end_constructor_in_complex(::utility::ofstream_t& out, const class_info& _class_info);
	void generate_implement_begin_destructor_in_complex(::utility::ofstream_t& out, const class_info& _class_info);
	void generate_implement_on_destructor_in_complex(::utility::ofstream_t& out, const property_info& _property_info);
	void generate_implement_end_destructor_in_complex(::utility::ofstream_t& out, const class_info& _class_info);
	void generate_implement_begin_materialize_in_complex(::utility::ofstream_t& out, const class_info& _class_info);
	void generate_implement_on_materialize_in_complex(::utility::ofstream_t& out, const class_info& _class_info, const property_info& _property_info);
	void generate_implement_end_materialize_in_complex(::utility::ofstream_t& out, const class_info& _class_info);
	void generate_implement_begin_serialize_in_complex(::utility::ofstream_t& out, const class_info& _class_info);
	void generate_implement_on_serialize_in_complex(::utility::ofstream_t& out, const class_info& _class_info, const property_info& _property_info);
	void generate_implement_end_serialize_in_complex(::utility::ofstream_t& out, const class_info& _class_info);

	// entity type
	void generate_declare_common_functions_in_entity(::utility::ofstream_t& out, const ::utility::string_t& class_name);
	void generate_declare_class_keys_in_entity(::utility::ofstream_t& out, const class_info& _class_info, const std::vector<property_info>& keys);
	void generate_declare_class_member_in_entity(::utility::ofstream_t& out, const class_info& _class_info, const property_info& _property_info);
	void generate_declare_operation_in_entity(::utility::ofstream_t& out, const class_info& _class_info, const property_info& _property_info);
	void generate_implement_common_functions_in_entity(::utility::ofstream_t& out, const class_info& _class_info);
	void generate_implement_class_member_in_entity(::utility::ofstream_t& out, const class_info& _class_info, const property_info& _property_info);
	void generate_implement_begin_constructor_in_entity(::utility::ofstream_t& out, const class_info& _class_info);
	void generate_implement_on_constructor_in_entity(::utility::ofstream_t& out, const property_info& _property_info);
	void generate_implement_end_constructor_in_entity(::utility::ofstream_t& out, const class_info& _class_info);
	void generate_implement_begin_destructor_in_entity(::utility::ofstream_t& out, const class_info& _class_info);
	void generate_implement_on_destructor_in_entity(::utility::ofstream_t& out, const property_info& _property_info);
	void generate_implement_end_destructor_in_entity(::utility::ofstream_t& out, const class_info& _class_info);
	void generate_implement_begin_materialize_in_entity(::utility::ofstream_t& out, const class_info& _class_info);
	void generate_implement_on_materialize_in_entity(::utility::ofstream_t& out, const class_info& _class_info, const property_info& _property_info);
	void generate_implement_end_materialize_in_entity(::utility::ofstream_t& out, const class_info& _class_info);
	void generate_implement_begin_serialize_in_entity(::utility::ofstream_t& out, const class_info& _class_info);
	void generate_implement_on_serialize_in_entity(::utility::ofstream_t& out, const class_info& _class_info, const property_info& _property_info);
	void generate_implement_end_serialize_in_entity(::utility::ofstream_t& out, const class_info& _class_info);
	void generate_implement_operation_in_entity(::utility::ofstream_t& out, const class_info& _class_info, const property_info& _property_info);

	// enum type
	void generate_declare_enum_begin(::utility::ofstream_t& out, const class_info& _class_info);
	void generate_declare_enum_member(::utility::ofstream_t& out, const property_info& _property_info);
	void generate_declare_enum_end(::utility::ofstream_t& out);
	void generate_declare_enum_resolver_begin(::utility::ofstream_t& out);
	void generate_declare_enum_resolver_on(::utility::ofstream_t& out, const class_info& _class_info);
	void generate_declare_enum_resolver_end(::utility::ofstream_t& out);
	void generate_implement_enum_type_to_string_begin(::utility::ofstream_t& out, const class_info& _class_info);
	void generate_implement_enum_type_to_string_member(::utility::ofstream_t& out, const class_info& _class_info, const property_info& _property_info);
	void generate_implement_enum_type_to_string_end(::utility::ofstream_t& out, const class_info& _class_info);
	void generate_implement_string_to_enum_type_begin(::utility::ofstream_t& out, const class_info& _class_info);
	void generate_implement_string_to_enum_type_member(::utility::ofstream_t& out, const class_info& _class_info, const property_info& _property_info);
	void generate_implement_string_to_enum_type_end(::utility::ofstream_t& out, const class_info& _class_info);

	void output_line_content(::utility::ostream_t& out, const ::utility::string_t& content, unsigned int indent_number = 0);
	void output_empty_line(::utility::ostream_t& out, unsigned int line_number = 1);

private:
	std::shared_ptr<odata_codegen_initializer> m_initializer;
};


}}}