//---------------------------------------------------------------------
// <copyright file="edm_model_reader.cpp" company="Microsoft">
//      Copyright (C) Microsoft Corporation. All rights reserved. See License.txt in the project root for license information.
// </copyright>
//---------------------------------------------------------------------

#include "odata/edm/edm_model_reader.h"
#include "odata/edm/edm_model_utility.h"

namespace odata { namespace edm
{
bool edm_model_reader::parse()
{
	bool ret = ::odata::common::xml_reader::parse();

	edm_model_utility::resolve_edm_types_after_parsing(m_model);

	return ret;
}

void edm_model_reader::handle_begin_element(const ::odata::string_t& elementName)
{
	if (elementName == _XPLATSTR("Edmx"))
	{
		while (move_to_next_attribute())
		{
			if(get_current_element_name() == _XPLATSTR("Version"))
			{
				m_model->set_version(get_current_element_text());
			}
		}
	}
	else if (elementName == _XPLATSTR("Schema"))
	{
		::odata::string_t namesp;
		::odata::string_t alias;

		while (move_to_next_attribute())
		{
			if(get_current_element_name() == _XPLATSTR("Namespace"))
			{
				namesp = get_current_element_text();
			}

			if(get_current_element_name() == _XPLATSTR("Alias"))
			{
				alias = get_current_element_text();
			}
		}

		m_current_schema = m_model->add_schema(namesp, alias);
	}
	else if (elementName == _XPLATSTR("EntityContainer"))
	{
		::odata::string_t name;
		::odata::string_t extends;
		bool is_default = true;

		while (move_to_next_attribute())
		{
			if (get_current_element_name() == _XPLATSTR("Name"))
			{
				name = get_current_element_text();
			}
			else if (get_current_element_name() == _XPLATSTR("Extends"))
			{
				extends = get_current_element_text();
			}
		}

		m_current_container = ::odata::make_shared<edm_entity_container>(name, is_default);
	}
	else if (elementName == _XPLATSTR("EntitySet"))
	{
		::odata::string_t name;
		::odata::string_t type;

		while (move_to_next_attribute())
		{
			if(get_current_element_name() == _XPLATSTR("Name"))
			{
				name = get_current_element_text();
			}
			else if (get_current_element_name() == _XPLATSTR("EntityType"))
			{
				type = get_current_element_text();
			}
		}

		m_current_entity_set = ::odata::make_shared<edm_entity_set>(name, type);
	}
	else if (elementName == _XPLATSTR("Singleton"))
	{
		::odata::string_t name;
		::odata::string_t type;

		while (move_to_next_attribute())
		{
			if(get_current_element_name() == _XPLATSTR("Name"))
			{
				name = get_current_element_text();
			}
			else if (get_current_element_name() == _XPLATSTR("Type"))
			{
				type = get_current_element_text();
			}
		}

		m_current_singleton = ::odata::make_shared<edm_singleton>(name, type);
	}
	else if (elementName == _XPLATSTR("FunctionImport") || elementName == _XPLATSTR("ActionImport"))
	{
		::odata::string_t name;
		::odata::string_t entity_set_path;
		bool is_in_service_document;
		OperationImportKind operation_import_kind = elementName == _XPLATSTR("FunctionImport") ? OperationImportKind::FunctionImport : OperationImportKind::ActionImport;
		::odata::string_t operation_name;

		while (move_to_next_attribute())
		{
			if(get_current_element_name() == _XPLATSTR("Name"))
			{
				name = get_current_element_text();
			}
			else if (get_current_element_name() == _XPLATSTR("EntitySet"))
			{
				entity_set_path = get_current_element_text();
			}
			else if (get_current_element_name() == _XPLATSTR("IncludeInServiceDocument"))
			{
				is_in_service_document = get_current_element_text() == _XPLATSTR("true");
			}
			else if (get_current_element_name() == _XPLATSTR("Function") || get_current_element_name() == _XPLATSTR("Action"))
			{
				operation_name = get_current_element_text();
			}
		}

		if (m_current_container)
		{
			m_current_container->add_operation_import(::odata::make_shared<edm_operation_import>(name, operation_name, entity_set_path, is_in_service_document, operation_import_kind));
		}
	}
	else if (elementName == _XPLATSTR("EntityType"))
	{
		::odata::string_t name;
		::odata::string_t baseType;
		bool isAbstract = false;
		bool isOpenType = false;
		bool hasStream = false;

		while (move_to_next_attribute())
		{
			if (get_current_element_name() == _XPLATSTR("Name"))
			{
				name = get_current_element_text();
			}
			else if (get_current_element_name() == _XPLATSTR("BaseType"))
			{
				baseType = get_current_element_text();
			}
			else if (get_current_element_name() == _XPLATSTR("Abstract"))
			{
				isAbstract = get_current_element_text() == _XPLATSTR("true");
			}
			else if (get_current_element_name() == _XPLATSTR("OpenType"))
			{
				isOpenType = get_current_element_text() == _XPLATSTR("true");
			}
			else if (get_current_element_name() == _XPLATSTR("HasStream"))
			{
				hasStream = get_current_element_text() == _XPLATSTR("true");
			}
		}

		m_current_st = new edm_entity_type(name, m_current_schema->get_name(), baseType, isAbstract, isOpenType, hasStream);
	}
	else if (elementName == _XPLATSTR("ComplexType"))
	{
		::odata::string_t name;
		::odata::string_t baseType;
		bool isAbstract = false;
		bool isOpenType = false;

		while (move_to_next_attribute())
		{
			if (get_current_element_name() == _XPLATSTR("Name"))
			{
				name = get_current_element_text();
			}
			else if (get_current_element_name() == _XPLATSTR("BaseType"))
			{
				baseType = get_current_element_text();
			}
			else if (get_current_element_name() == _XPLATSTR("Abstract"))
			{
				isAbstract = get_current_element_text() == _XPLATSTR("true");
			}
			else if (get_current_element_name() == _XPLATSTR("OpenType"))
			{
				isOpenType = get_current_element_text() == _XPLATSTR("true");
			}
		}

		m_current_st = new edm_complex_type(name, m_current_schema->get_name(), baseType, isAbstract, isOpenType);
	}
	else if (elementName == _XPLATSTR("EnumType"))
	{
		::odata::string_t name;
		::odata::string_t underlying_type = _XPLATSTR("Edm.Int32");
		bool is_flag = false;

		while (move_to_next_attribute())
		{
			if (get_current_element_name() == _XPLATSTR("Name"))
			{
				name = get_current_element_text();
			}
			else if (get_current_element_name() == _XPLATSTR("UnderlyingType"))
			{
				underlying_type = get_current_element_text();
			}
			else if (get_current_element_name() == _XPLATSTR("IsFlags"))
			{
				is_flag = get_current_element_text() == _XPLATSTR("true");
			}
		}

		m_current_enum = new edm_enum_type(name, m_current_schema->get_name(), underlying_type, is_flag);
	}
	else if (elementName == _XPLATSTR("Function") || elementName == _XPLATSTR("Action"))
	{
		::odata::string_t name;
		::odata::string_t path;
		bool is_bound = false;
		bool is_composable = false;
		EdmOperationKind operation_kind = elementName == _XPLATSTR("Function") ? EdmOperationKind::Function : EdmOperationKind::Action;

		while (move_to_next_attribute())
		{
			if (get_current_element_name() == _XPLATSTR("Name"))
			{
				name = get_current_element_text();
			}
			else if (get_current_element_name() == _XPLATSTR("EntitySetPath"))
			{
				path = get_current_element_text();
			}
			else if (get_current_element_name() == _XPLATSTR("IsBound"))
			{
				is_bound = get_current_element_text() == _XPLATSTR("true");
			}
			else if (get_current_element_name() == _XPLATSTR("IsComposable"))
			{
				is_composable = get_current_element_text() == _XPLATSTR("true");
			}
		}

		m_current_operation = new edm_operation_type(name, m_current_schema->get_name(),  is_bound, path, operation_kind, is_composable);
	}
	else if (elementName == _XPLATSTR("Property") || elementName == _XPLATSTR("Member"))
	{
		_process_property();
	}
	else if (elementName == _XPLATSTR("NavigationPropertyBinding"))
	{
		_process_navigation_property_binding();
	}
	else if (elementName == _XPLATSTR("NavigationProperty"))
	{
		_process_navigation_property();
	}
	else if (elementName == _XPLATSTR("Parameter"))
	{
		_process_operation_parameter();
	}
	else if (elementName == _XPLATSTR("ReturnType"))
	{
		_process_operation_return_type();
	}
	else if (m_parsing_key && elementName == _XPLATSTR("PropertyRef"))
	{
		while (move_to_next_attribute())
		{
			if (get_current_element_name() == _XPLATSTR("Name"))
			{
				dynamic_cast<edm_entity_type*>(m_current_st)->add_key_property(get_current_element_text());
				break;
			}
		}
	}
	else if (elementName == _XPLATSTR("Key"))
	{
		m_parsing_key = true;
	}
#ifdef WIN32
	m_reader->MoveToElement();
#endif

}

void edm_model_reader::handle_end_element(const ::odata::string_t& elementName)
{
	if (elementName == _XPLATSTR("EntityContainer"))
	{
		m_current_schema->add_container(m_current_container);
		m_current_container.reset();
	}

	if (elementName == _XPLATSTR("EntityType"))
	{
		m_current_schema->add_entity_type(std::shared_ptr<edm_entity_type>(static_cast<edm_entity_type*>(m_current_st)));
		m_current_st = nullptr;
	}

	if (elementName == _XPLATSTR("ComplexType"))
	{
		m_current_schema->add_complex_type(std::shared_ptr<edm_complex_type>(static_cast<edm_complex_type*>(m_current_st)));
		m_current_st = nullptr;
	}

	if (elementName == _XPLATSTR("Key"))
	{
		m_parsing_key = false;
	}

	if (elementName == _XPLATSTR("EnumType"))
	{
		m_current_schema->add_enum_type(std::shared_ptr<edm_enum_type>(static_cast<edm_enum_type*>(m_current_enum)));
		m_current_enum = nullptr;
	}

	if (elementName == _XPLATSTR("Function") || elementName == _XPLATSTR("Action"))
	{
		m_current_schema->add_operation_type(std::shared_ptr<edm_operation_type>(static_cast<edm_operation_type*>(m_current_operation)));
		m_current_operation = nullptr;
	}

	if (elementName == _XPLATSTR("EntitySet"))
	{
		if (m_current_container)
		{
			m_current_container->add_entity_set(m_current_entity_set);
		}

		m_current_entity_set = nullptr;
	}

	if (elementName == _XPLATSTR("Singleton"))
	{
		if (m_current_container)
		{
			m_current_container->add_singleton(m_current_singleton);
		}

		m_current_singleton = nullptr;
	}
}

void edm_model_reader::handle_element(const ::odata::string_t&)
{
}

void edm_model_reader::_process_property()
{
	if (m_current_st)
	{
		::odata::string_t property_name;
		bool is_nullable = false;
		std::shared_ptr<edm_named_type> type;
		unsigned int max_length = undefined_value;
		bool is_unicode = false;
		unsigned int scale = undefined_value;
		unsigned int precision = undefined_value;

		while (move_to_next_attribute())
		{
			auto name  = get_current_element_name();
			auto value = get_current_element_text();

			if (name == _XPLATSTR("Name"))
			{
				property_name = value;
			}
			else if (name == _XPLATSTR("Nullable"))
			{
				is_nullable = (value == _XPLATSTR("true"));
			}
			else if (name == _XPLATSTR("Type"))
			{
				type = edm_model_utility::get_edm_type_from_name(value);

				if (!type)
				{
					// we have to parse the real type after first round parsing
					type = ::odata::make_shared<edm_named_type>(value, _XPLATSTR(""), edm_type_kind_t::Unknown);
				}
			}
			else if (name == _XPLATSTR("MaxLength"))
			{
				web::http::bind(value.c_str(), max_length);
			}
			else if (name == _XPLATSTR("Unicode"))
			{
				is_unicode = (value == _XPLATSTR("true"));
			}
			else if (name == _XPLATSTR("Scale"))
			{
				web::http::bind(value.c_str(), scale);
			}
			else if (name == _XPLATSTR("Precision"))
			{
				web::http::bind(value.c_str(), precision);
			}
		}

		auto prop = ::odata::make_shared<edm_property_type>(property_name, is_nullable, max_length, is_unicode, scale);
		prop->set_property_type(type);
		prop->set_precision(precision);
		m_current_st->add_property(prop);
	}
	else if (m_current_enum)
	{
		::odata::string_t enum_name;
		unsigned long enum_value = (std::numeric_limits<unsigned long>::max)(); // max signifies that no explicit value was specified!

		while (move_to_next_attribute())
		{
			auto name  = get_current_element_name();
			auto value = get_current_element_text();

			if (name == _XPLATSTR("Name"))
			{
				enum_name = value;
			}
			else if (name == _XPLATSTR("Value"))
			{
				web::http::bind(value.c_str(), enum_value);
			}
		}

		auto prop = ::odata::make_shared<edm_enum_member>(enum_name, enum_value);
		m_current_enum->add_enum_member(prop);
	}
}

void edm_model_reader::_process_navigation_property()
{
	if (m_current_st)
	{
		::odata::string_t property_name;
		::odata::string_t partner_name;
		::odata::string_t type_name;
		bool is_contained = false;
		bool is_nullable = false;

		while (move_to_next_attribute())
		{
			auto name  = get_current_element_name();
			auto value = get_current_element_text();

			if (name == _XPLATSTR("Name"))
			{
				property_name = value;
			}
			else if (name == _XPLATSTR("Nullable"))
			{
				is_nullable = (value == _XPLATSTR("true"));
			}
			else if (name == _XPLATSTR("Partner"))
			{
				partner_name = value;
			}
			else if (name == _XPLATSTR("Type"))
			{
				type_name = value;
			}
			else if (name == _XPLATSTR("ContainsTarget"))
			{
				is_contained = (value == _XPLATSTR("true"));
			}
		}

		auto type = ::odata::make_shared<edm_navigation_type>(type_name, partner_name, is_contained);
		auto prop = ::odata::make_shared<edm_property_type>(property_name, is_nullable, type);
		m_current_st->add_property(prop);
	}
}

void edm_model_reader::_process_navigation_property_binding()
{
	::odata::string_t property_path_name;
	::odata::string_t target_name;

	while (move_to_next_attribute())
	{
		auto name  = get_current_element_name();
		auto value = get_current_element_text();

		if (name == _XPLATSTR("Path"))
		{
			property_path_name = value;
		}
		else if (name == _XPLATSTR("Target"))
		{
			target_name = value;
		}
	}

	if (m_current_entity_set)
	{
		m_current_entity_set->add_navigation_source(property_path_name, target_name);
	}
	else if (m_current_singleton)
	{
		m_current_singleton->add_navigation_source(property_path_name, target_name);
	}
}

void edm_model_reader::_process_operation_parameter()
{
	if (m_current_operation)
	{
		::odata::string_t param_name;
		std::shared_ptr<edm_named_type> param_type;

		while (move_to_next_attribute())
		{
			auto name  = get_current_element_name();
			auto value = get_current_element_text();

			if (name == _XPLATSTR("Name"))
			{
				param_name = value;
			}
			else if (name == _XPLATSTR("Type"))
			{
				param_type = ::odata::make_shared<edm_named_type>(value, _XPLATSTR(""), edm_type_kind_t::Unknown);
			}
		}

		auto param = ::odata::make_shared<edm_operation_parameter>(param_name, param_type);

		m_current_operation->add_operation_parameter(param);
	}
}

void edm_model_reader::_process_operation_return_type()
{
	if (m_current_operation)
	{
		auto return_type = ::odata::make_shared<edm_named_type>();

		while (move_to_next_attribute())
		{
			auto name  = get_current_element_name();
			auto value = get_current_element_text();

			if (name == _XPLATSTR("Type"))
			{
				return_type = ::odata::make_shared<edm_named_type>(value, _XPLATSTR(""), edm_type_kind_t::Unknown);
			}
		}

		m_current_operation->set_return_type(return_type);
	}
}

}}
