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

void edm_model_reader::handle_begin_element(const ::utility::string_t& elementName)
{
	if (elementName == U("Edmx"))
	{
		while (move_to_next_attribute())
		{
			if(get_current_element_name() == U("Version"))
			{
				m_model->set_version(get_current_element_text());
			}
		}
	}
    else if (elementName == U("Schema"))
    {
        ::utility::string_t namesp;
        ::utility::string_t alias;

        while (move_to_next_attribute())
        {
            if(get_current_element_name() == U("Namespace"))
            {
                namesp = get_current_element_text();
            }

            if(get_current_element_name() == U("Alias"))
            {
                alias = get_current_element_text();
            }
        }

        m_current_schema = m_model->add_schema(namesp, alias);
    }
    else if (elementName == U("EntityContainer"))
    {
        ::utility::string_t name;
		::utility::string_t extends;
        bool is_default = true;

        while (move_to_next_attribute())
        {
            if (get_current_element_name() == U("Name"))
            {
                name = get_current_element_text();
            }
			else if (get_current_element_name() == U("Extends"))
			{
				extends = get_current_element_text();
			}
        }

        m_current_container = std::make_shared<edm_entity_container>(name, is_default);
    }
    else if (elementName == U("EntitySet"))
    {
        ::utility::string_t name;
        ::utility::string_t type;

        while (move_to_next_attribute())
        {
            if(get_current_element_name() == U("Name"))
            {
                name = get_current_element_text();
            }
            else if (get_current_element_name() == U("EntityType"))
            {
                type = get_current_element_text();
            }
        }

        m_current_entity_set = std::make_shared<edm_entity_set>(name, type);
    }
    else if (elementName == U("Singleton"))
    {
        ::utility::string_t name;
        ::utility::string_t type;

        while (move_to_next_attribute())
        {
            if(get_current_element_name() == U("Name"))
            {
                name = get_current_element_text();
            }
            else if (get_current_element_name() == U("Type"))
            {
                type = get_current_element_text();
            }
        }

		m_current_singleton = std::make_shared<edm_singleton>(name, type);
    }
	else if (elementName == U("FunctionImport") || elementName == U("ActionImport"))
	{
		::utility::string_t name;
		::utility::string_t entity_set_path;
		bool is_in_service_document;
		OperationImportKind operation_import_kind = elementName == U("FunctionImport") ? OperationImportKind::FunctionImport : OperationImportKind::ActionImport;
		::utility::string_t operation_name;

        while (move_to_next_attribute())
        {
            if(get_current_element_name() == U("Name"))
            {
                name = get_current_element_text();
            }
            else if (get_current_element_name() == U("EntitySet"))
            {
                entity_set_path = get_current_element_text();
            }
            else if (get_current_element_name() == U("IncludeInServiceDocument"))
            {
                is_in_service_document = get_current_element_text()  == U("true") ? true : false;;
            }
            else if (get_current_element_name() == U("Function") || get_current_element_name() == U("Action"))
            {
                operation_name = get_current_element_text();
            }
        }

        if (m_current_container)
		{
			m_current_container->add_operation_import(std::make_shared<edm_operation_import>(name, operation_name, entity_set_path, is_in_service_document, operation_import_kind));
		}
	}
    else if (elementName == U("EntityType"))
    {
        ::utility::string_t name;
		::utility::string_t baseType;
		bool isAbstract = false;
		bool isOpenType = false;
		bool hasStream = false;

        while (move_to_next_attribute())
        {
            if (get_current_element_name() == U("Name"))
            {
                name = get_current_element_text();
            }
			else if (get_current_element_name() == U("BaseType"))
            {
                baseType = get_current_element_text();
            }
			else if (get_current_element_name() == U("Abstract"))
            {
                isAbstract = get_current_element_text() == U("true") ? true : false;
            }
			else if (get_current_element_name() == U("OpenType"))
            {
                isOpenType = get_current_element_text() == U("true") ? true : false;
            }
			else if (get_current_element_name() == U("HasStream"))
            {
                hasStream = get_current_element_text() == U("true") ? true : false;
            }
        }

		m_current_st = new edm_entity_type(name, m_current_schema->get_name(), baseType, isAbstract, isOpenType, hasStream);
    }
    else if (elementName == U("ComplexType"))
	{
        ::utility::string_t name;
		::utility::string_t baseType;
		bool isAbstract = false;
		bool isOpenType = false;

        while (move_to_next_attribute())
        {
            if (get_current_element_name() == U("Name"))
            {
                name = get_current_element_text();
            }
			else if (get_current_element_name() == U("BaseType"))
            {
                baseType = get_current_element_text();
            }
			else if (get_current_element_name() == U("Abstract"))
            {
                isAbstract = get_current_element_text() == U("true") ? true : false;
            }
			else if (get_current_element_name() == U("OpenType"))
            {
                isOpenType = get_current_element_text() == U("true") ? true : false;
            }
        }

        m_current_st = new edm_complex_type(name, m_current_schema->get_name(), baseType, isAbstract, isOpenType);
    }
	else if (elementName == U("EnumType"))
	{
		::utility::string_t name;
		::utility::string_t underlying_type = U("Edm.Int32");
		bool is_flag = false;

        while (move_to_next_attribute())
        {
            if (get_current_element_name() == U("Name"))
            {
                name = get_current_element_text();
            }
			else if (get_current_element_name() == U("UnderlyingType"))
            {
                underlying_type = get_current_element_text();
            }
			else if (get_current_element_name() == U("IsFlags"))
            {
                is_flag = get_current_element_text() == U("true") ? true : false;
            }
        }

		m_current_enum = new edm_enum_type(name, m_current_schema->get_name(), underlying_type, is_flag);
	}
	else if (elementName == U("Function") || elementName == U("Action"))
	{
		::utility::string_t name;
		::utility::string_t path;
		bool is_bound = false;
		bool is_composable = false;
		EdmOperationKind operation_kind = elementName == U("Function") ? EdmOperationKind::Function : EdmOperationKind::Action;

        while (move_to_next_attribute())
        {
            if (get_current_element_name() == U("Name"))
            {
                name = get_current_element_text();
            }
			else if (get_current_element_name() == U("EntitySetPath"))
            {
                path = get_current_element_text();
            }
			else if (get_current_element_name() == U("IsBound"))
            {
                is_bound = get_current_element_text() == U("true") ? true : false;
            }
			else if (get_current_element_name() == U("IsComposable"))
            {
                is_composable = get_current_element_text() == U("true") ? true : false;
            }
        }

		m_current_operation = new edm_operation_type(name, m_current_schema->get_name(),  is_bound, path, operation_kind, is_composable);
	}
    else if (elementName == U("Property") || elementName == U("Member"))
    {
        _process_property();
    }
    else if (elementName == U("NavigationPropertyBinding"))
    {
        _process_navigation_property_binding();
    }
    else if (elementName == U("NavigationProperty"))
    {
        _process_navigation_property();
    }
	else if (elementName == U("Parameter"))
	{
		_process_operation_parameter();
	}
	else if (elementName == U("ReturnType"))
	{
		_process_operation_return_type();
	}
    else if (m_parsing_key && elementName == U("PropertyRef"))
    {
        while (move_to_next_attribute())
        {
            if (get_current_element_name() == U("Name"))
            {
                dynamic_cast<edm_entity_type*>(m_current_st)->add_key_property(get_current_element_text());
                break;
            }
        }
        
    }
    else if (elementName == U("Key"))
    {
        m_parsing_key = true;
    }
#ifdef WIN32
    m_reader->MoveToElement();
#endif
	
}

void edm_model_reader::handle_end_element(const ::utility::string_t& elementName)
{
    if (elementName == U("EntityContainer"))
    {
        m_current_schema->add_container(m_current_container);
        m_current_container.reset();
    }

    if (elementName == U("EntityType"))
    {
        m_current_schema->add_entity_type(std::shared_ptr<edm_entity_type>(static_cast<edm_entity_type*>(m_current_st)));
        m_current_st = nullptr;
    }

    if (elementName == U("ComplexType"))
    {
        m_current_schema->add_complex_type(std::shared_ptr<edm_complex_type>(static_cast<edm_complex_type*>(m_current_st)));
        m_current_st = nullptr;
    }

    if (elementName == U("Key"))
    {
        m_parsing_key = false;
    }

	if (elementName == U("EnumType"))
	{
		m_current_schema->add_enum_type(std::shared_ptr<edm_enum_type>(static_cast<edm_enum_type*>(m_current_enum)));
        m_current_enum = nullptr;
	}

	if (elementName == U("Function") || elementName == U("Action"))
	{
		m_current_schema->add_operation_type(std::shared_ptr<edm_operation_type>(static_cast<edm_operation_type*>(m_current_operation)));
		m_current_operation = nullptr;
	}

	if (elementName == U("EntitySet"))
	{
        if (m_current_container)
		{
			m_current_container->add_entity_set(m_current_entity_set);
		}

		m_current_entity_set = nullptr;		
	}

	if (elementName == U("Singleton"))
	{
        if (m_current_container)
		{
			m_current_container->add_singleton(m_current_singleton);
		}

		m_current_singleton = nullptr;
	}
}

void edm_model_reader::handle_element(const ::utility::string_t&)
{
}

void edm_model_reader::_process_property()
{
	if (m_current_st)
	{
        ::utility::string_t property_name;
        bool is_nullable;
        std::shared_ptr<edm_named_type> type;
        unsigned int max_length = undefined_value;
        bool is_unicode;
	    unsigned int scale = undefined_value;
	    unsigned int precision = undefined_value;

		while (move_to_next_attribute())
		{
			auto name  = get_current_element_name();
			auto value = get_current_element_text();

			if (name == U("Name"))
			{
				property_name = value;
			}
			else if (name == U("Nullable"))
			{
				is_nullable = (value == U("true"));
			}
			else if (name == U("Type"))
			{           
				type = edm_model_utility::get_edm_type_from_name(value);
				
				if (!type)
				{
					// we have to parse the real type after first round parsing
				    type = std::make_shared<edm_named_type>(value, U(""), edm_type_kind_t::Unknown);
				}
			}
			else if (name == U("MaxLength"))
			{
				web::http::bind(value.c_str(), max_length);
			}
			else if (name == U("Unicode"))
			{
				is_unicode = (value == U("true"));
			}
			else if (name == U("Scale"))
			{
				web::http::bind(value.c_str(), scale);
			}
			else if (name == U("Precision"))
			{
				web::http::bind(value.c_str(), precision);
			}
		}

        auto prop = std::make_shared<edm_property_type>(property_name, is_nullable, max_length, is_unicode, scale);
        prop->set_property_type(type);
        prop->set_precision(precision);
		m_current_st->add_property(prop);
	}
	else if (m_current_enum)
	{
        ::utility::string_t enum_name;
        unsigned long enum_value;

		while (move_to_next_attribute())
		{
			auto name  = get_current_element_name();
			auto value = get_current_element_text();

			if (name == U("Name"))
			{
				enum_name = value;
			}
			else if (name == U("Value"))
			{
				web::http::bind(value.c_str(), enum_value);
			}
		}

        auto prop = std::make_shared<edm_enum_member>(enum_name, enum_value);
		m_current_enum->add_enum_member(prop);
	}
}

void edm_model_reader::_process_navigation_property()
{
	if (m_current_st)
	{
        ::utility::string_t property_name;
		::utility::string_t partner_name;
		::utility::string_t type_name;
		bool is_contained = false;
        bool is_nullable = false;

		while (move_to_next_attribute())
		{
			auto name  = get_current_element_name();
			auto value = get_current_element_text();

			if (name == U("Name"))
			{
				property_name = value;
			}
			else if (name == U("Nullable"))
			{
				is_nullable = (value == U("true"));
			}
			else if (name == U("Partner"))
			{
				partner_name = value;
			}
			else if (name == U("Type"))
			{
				type_name = value;
			}
			else if (name == U("ContainsTarget"))
			{
				is_contained = (value == U("true"));
			}
		}

        
		auto type = std::make_shared<edm_navigation_type>(type_name, partner_name, is_contained); 
        auto prop = std::make_shared<edm_property_type>(property_name, is_nullable, type);
		m_current_st->add_property(prop);
	}
}

void edm_model_reader::_process_navigation_property_binding()
{
	::utility::string_t property_path_name;
	::utility::string_t target_name;

	while (move_to_next_attribute())
	{
		auto name  = get_current_element_name();
		auto value = get_current_element_text();

		if (name == U("Path"))
		{
			property_path_name = value;
		}
		else if (name == U("Target"))
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
        ::utility::string_t param_name;
        std::shared_ptr<edm_named_type> param_type;

		while (move_to_next_attribute())
		{
			auto name  = get_current_element_name();
			auto value = get_current_element_text();

			if (name == U("Name"))
			{
				param_name = value;
			}
			else if (name == U("Type"))
			{
				param_type = std::make_shared<edm_named_type>(value, U(""), edm_type_kind_t::Unknown);
			}
		}

        auto param = std::make_shared<edm_operation_parameter>(param_name, param_type);

		m_current_operation->add_operation_parameter(param);
	}
}

void edm_model_reader::_process_operation_return_type()
{
	if (m_current_operation)
	{
		auto return_type = std::make_shared<edm_named_type>();

		while (move_to_next_attribute())
		{
			auto name  = get_current_element_name();
			auto value = get_current_element_text();

			if (name == U("Type"))
			{
				return_type = std::make_shared<edm_named_type>(value, U(""), edm_type_kind_t::Unknown);
			}
		}		

		m_current_operation->set_return_type(return_type);
	}
}

}}
