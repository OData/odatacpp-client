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
 
 #include "odata_codegen_initializer.h"
#include "odata/client/odata_client.h"
#include "odata/edm/edm_model_utility.h"
#include <regex>

using namespace ::odata::edm;

namespace odata { namespace codegen { namespace tools {

#define BASE_TYPE_NAME U("type_base")
#define DATA_SERVICE_CONTEXT_NAME U("odata_service_context")

::pplx::task<int> odata_codegen_initializer::begin_initialize(const ::utility::string_t metadata_url, const ::utility::string_t file_name, const ::utility::string_t user_name, const ::utility::string_t  password)
{
	m_config._metadata_url = metadata_url;
	m_config._file_name = file_name;
	m_config._user_name = user_name;
	m_config._pass_word = password;

	return begin_initialize_code_gen_info(m_config._metadata_url, m_config._user_name, m_config._pass_word);
}

::pplx::task<int> odata_codegen_initializer::begin_initialize_code_gen_info(const ::utility::string_t& metadata_url,  const ::utility::string_t& _user_name, const ::utility::string_t& _pass_word)
{
	reset_class_info();

	std::shared_ptr<::odata::client::odata_client> client = nullptr;
	if (!_user_name.empty() && !_pass_word.empty())
	{
	    ::odata::client::client_options options;
	    options.enable_client_credential(_user_name, _pass_word);
		client = std::make_shared<::odata::client::odata_client>(metadata_url, options);
	}
	else
	{
		client = std::make_shared<::odata::client::odata_client>(metadata_url);
	}
    
	if (!client)
	{
		return ::pplx::task_from_result(-1);
	}

    m_model = client->get_model().get();
	if (!m_model)
	{
		return ::pplx::task_from_result(-1);
	}
	
	auto entity_container = m_model->find_container();
	if (!entity_container)
	{
		throw std::runtime_error("must have an entity container in model!");
	}

	::utility::string_t ns_entity_container;
	for (auto schema = m_model->get_schema().cbegin(); schema != m_model->get_schema().cend(); schema++)
	{
		if ((*schema)->find_container() != nullptr)
		{
			ns_entity_container = (*schema)->get_name();
		}
	}

	// merge namesapce to one that has entity container
	// todo : support same class name in different namesapce
	for (auto schema = m_model->get_schema().cbegin(); schema != m_model->get_schema().cend(); schema++)
	{
		schema_info _schema_info;
		initialize_class_info(*schema, _schema_info);
		initialize_property_info_of_class(*schema, _schema_info);

		m_code_gen_map[(*schema)->get_name()] = _schema_info;
	}

	schema_info total;
	for (auto map_iter = m_code_gen_map.begin(); map_iter != m_code_gen_map.end(); map_iter++)
	{
		schema_info _schema_info = map_iter->second;

		total.m_class_property_map.insert(_schema_info.m_class_property_map.begin(), _schema_info.m_class_property_map.end());
		total.m_class_map.insert(_schema_info.m_class_map.begin(), _schema_info.m_class_map.end());
		total.m_class_vec.insert(total.m_class_vec.end(), _schema_info.m_class_vec.begin(), _schema_info.m_class_vec.end());
		total.m_derived_classes.insert(_schema_info.m_derived_classes.begin(), _schema_info.m_derived_classes.end());
	}

	m_code_gen_map.clear();
	m_code_gen_map[ns_entity_container] = total;

	return ::pplx::task_from_result(0);
}

void odata_codegen_initializer::reset_class_info()
{
	m_code_gen_map.clear();
}

static ::utility::string_t normalize_namespace(const ::utility::string_t& _namespace)
{
	::utility::string_t ret = _namespace;

	std::replace(ret.begin(), ret.end(), U('.'), U('_'));

	return ret;
}

void odata_codegen_initializer::initialize_class_info(const std::shared_ptr<edm_schema>& schema, schema_info& _schema_info)
{
	if (!schema)
	{
		return ;
	}

	auto enum_types = schema->get_enum_types();
	for (auto enum_type_iter = enum_types.cbegin(); enum_type_iter != enum_types.cend(); enum_type_iter++)
	{
		class_info _info;

		auto enum_type = enum_type_iter->second;
		_info._edm_name = enum_type->get_name();
		_info._class_name = enum_type->get_name();
		_info._edm_namespace = schema->get_name();
		_info._type = CLASS_TYPE::E_CLASS_ENUM;

		_schema_info.m_class_map[enum_type->get_name()] = _info;	
		_schema_info.m_class_vec.push_back(_info);
	}

	auto complex_types = schema->get_complex_types();
	for (auto complex_type_iter = complex_types.cbegin(); complex_type_iter != complex_types.cend(); complex_type_iter++)
	{
		class_info _info;

		auto complex_type = complex_type_iter->second;
		_info._edm_name = complex_type->get_name();
		_info._class_name = complex_type->get_name();
		_info._edm_namespace = schema->get_name();
		_info._type = CLASS_TYPE::E_CLASS_COMPLEX;
		_info._base_class_name = complex_type->get_base_type_name();
		if (_info._base_class_name.empty())
		{
			_info._base_class_name = BASE_TYPE_NAME;
		}
		else
		{
			if (_info._base_class_name.find(complex_type->get_namespace()) == 0)
			{
				_info._base_class_name = 
					_info._base_class_name.substr(complex_type->get_namespace().length() + 1, _info._base_class_name.length() - complex_type->get_namespace().length() - 1);
			}
		}

		_schema_info.m_class_map[complex_type->get_name()] = _info;
	}

	auto entity_types = schema->get_entity_types();
	for (auto entity_type_iter = entity_types.cbegin(); entity_type_iter != entity_types.cend(); entity_type_iter++)
	{
		class_info _info;

		auto entity_type = entity_type_iter->second;
		_info._edm_name = entity_type->get_name();
		_info._class_name = entity_type->get_name();
		_info._edm_namespace = schema->get_name();
		_info._type = CLASS_TYPE::E_CLASS_ENTITY;
		_info._base_class_name = entity_type->get_base_type_name();
		if (_info._base_class_name.empty())
		{
			_info._base_class_name = BASE_TYPE_NAME;
		}
		else
		{
			if (_info._base_class_name.find(entity_type->get_namespace()) == 0)
			{
				_info._base_class_name = 
					_info._base_class_name.substr(entity_type->get_namespace().length() + 1, _info._base_class_name.length() - entity_type->get_namespace().length() - 1);
			}
		}

		_schema_info.m_class_map[entity_type->get_name()] = _info;
	}


	auto entity_containers = schema->get_containers();
	if (entity_containers.size() > 1)
	{
		throw std::runtime_error("only support model with one entity container!");
	}

	for (auto entity_container_iter = entity_containers.cbegin(); entity_container_iter != entity_containers.cend(); entity_container_iter++)
	{
		class_info _info;

		auto container_type = entity_container_iter->second;
		_info._edm_name = container_type->get_name();
		_info._class_name = container_type->get_name();
		_info._edm_namespace = schema->get_name();
		_info._base_class_name = DATA_SERVICE_CONTEXT_NAME;
		_info._type = CLASS_TYPE::E_CLASS_CONTAINER;

		_schema_info.m_class_map[container_type->get_name()] = _info;	
	}

	// derived class dependency
	std::unordered_map<::utility::string_t, ::utility::string_t> map_types_to_be_added;
	map_types_to_be_added[BASE_TYPE_NAME] = BASE_TYPE_NAME;
	map_types_to_be_added[DATA_SERVICE_CONTEXT_NAME] = DATA_SERVICE_CONTEXT_NAME;

	bool b_continue = true;
	while (b_continue)
	{
		b_continue = false;

		for (auto class_iter = _schema_info.m_class_map.cbegin(); class_iter != _schema_info.m_class_map.cend(); class_iter++)
		{
			if (map_types_to_be_added.find(class_iter->second._base_class_name) != map_types_to_be_added.end()
				&& map_types_to_be_added.find(class_iter->second._class_name) == map_types_to_be_added.end())
			{
				map_types_to_be_added[class_iter->second._class_name] = class_iter->second._class_name;

				_schema_info.m_class_vec.push_back(class_iter->second);

				b_continue = true;
			}
		}
	}

	for (size_t i = 0; i < _schema_info.m_class_vec.size(); i++)
	{
		auto _class_info = _schema_info.m_class_vec[i];
		if ((_class_info._type == CLASS_TYPE::E_CLASS_COMPLEX || _class_info._type == CLASS_TYPE::E_CLASS_ENTITY) 
			&& (_class_info._base_class_name != BASE_TYPE_NAME))
		{
			std::vector<::utility::string_t> vec_derived_class;
			if (_schema_info.m_derived_classes.find(_class_info._base_class_name) == _schema_info.m_derived_classes.end())
			{
				vec_derived_class.push_back(_class_info._class_name);
				_schema_info.m_derived_classes[_class_info._base_class_name] = vec_derived_class;
			}
			else
			{
				_schema_info.m_derived_classes[_class_info._base_class_name].push_back(_class_info._class_name);
			}
		}
	}
}

void odata_codegen_initializer::initialize_property_info_of_class(const std::shared_ptr<edm_schema>& schema, schema_info& _schema_info)
{
	if (!schema)
	{
		return ;
	}

	auto entity_types = schema->get_entity_types();
	for (auto entity_type_iter = entity_types.cbegin(); entity_type_iter != entity_types.cend(); entity_type_iter++)
	{
		auto entity_type = entity_type_iter->second;

		class_property_info  class_property_map;
		for (auto property_iter = entity_type->begin(); property_iter != entity_type->end(); property_iter++)
		{
	    	auto property_type = property_iter->second;

			if (property_type->get_property_type()->get_type_kind() == edm_type_kind_t::Unknown)
			{
				continue;
			}
			
			property_info property_info;

			property_info._edm_name = property_type->get_name();
			property_info._class_member_name = property_type->get_name();
			property_info._is_nullable = property_type->is_nullable();
			std::transform(property_info._class_member_name.begin(), property_info._class_member_name.end(), property_info._class_member_name.begin(), ::tolower);

			auto find_iter = std::find(entity_type->key().begin(), entity_type->key().end(), property_info._edm_name);
			property_info._is_key = find_iter == entity_type->key().end() ? false : true;

			if (property_type->get_property_type()->get_type_kind() == edm_type_kind_t::Primitive)
			{
				property_info._strong_type_name = edm_model_utility::get_strong_type_name_from_edm_type_name(std::dynamic_pointer_cast<edm_primitive_type>(property_type->get_property_type()));
				property_info._type = PROPERTY_TYPE::E_PRIMITIVE;
			}
			else if (property_type->get_property_type()->get_type_kind() == edm_type_kind_t::Complex)
			{
				property_info._strong_type_name = _schema_info.m_class_map[property_type->get_property_type()->get_name()]._class_name;
				property_info._type = PROPERTY_TYPE::E_COMPLEX;
			}
			else if (property_type->get_property_type()->get_type_kind() == edm_type_kind_t::Navigation)
			{
				auto navigation_type = std::dynamic_pointer_cast<edm_navigation_type>(property_type->get_property_type());
				if (navigation_type->get_navigation_type()->get_type_kind() == edm_type_kind_t::Entity)
				{
					// entity is contained in navigation
					property_info._strong_type_name = _schema_info.m_class_map[navigation_type->get_navigation_type()->get_name()]._class_name;
					property_info._type = PROPERTY_TYPE::E_ENTITY;
				}
				else if (navigation_type->get_navigation_type()->get_type_kind() == edm_type_kind_t::Collection)
				{
					// collection of entity
					auto collection_entity_type = std::dynamic_pointer_cast<edm_collection_type>(navigation_type->get_navigation_type());
					property_info._strong_type_name = _schema_info.m_class_map[collection_entity_type->get_element_type()->get_name()]._class_name;
					property_info._type = PROPERTY_TYPE::E_COLLECTION_ENTITY;
				}
			}
			else if (property_type->get_property_type()->get_type_kind() == edm_type_kind_t::Enum)
			{
				if (_schema_info.m_class_map.find(property_type->get_property_type()->get_name()) == _schema_info.m_class_map.end())
				{
					property_info._strong_type_name = property_type->get_property_type()->get_name();
				}
				else
				{
					property_info._strong_type_name = _schema_info.m_class_map[property_type->get_property_type()->get_name()]._class_name;
				}
				property_info._type = PROPERTY_TYPE::E_ENUM;

			}
			else if (property_type->get_property_type()->get_type_kind() == edm_type_kind_t::Collection)
			{
				// collection of primitive complex enum
				auto collection_type = std::dynamic_pointer_cast<edm_collection_type>(property_type->get_property_type());
				auto element_type = collection_type->get_element_type();
				if (element_type->get_type_kind() == edm_type_kind_t::Complex)
				{
					property_info._strong_type_name = _schema_info.m_class_map[element_type->get_name()]._class_name;
				    property_info._type = PROPERTY_TYPE::E_COLLECTION_COMPLEX;
				}
				else if (element_type->get_type_kind() == edm_type_kind_t::Primitive)
				{
				    property_info._strong_type_name = edm_model_utility::get_strong_type_name_from_edm_type_name(std::dynamic_pointer_cast<edm_primitive_type>(element_type));
				    property_info._type = PROPERTY_TYPE::E_COLLECTION_PRIMITIVE;
				}
				else if (element_type->get_type_kind() == edm_type_kind_t::Enum)
				{
					if (_schema_info.m_class_map.find(element_type->get_name()) == _schema_info.m_class_map.end())
					{
						property_info._strong_type_name = element_type->get_name();
					}
					else
					{
						property_info._strong_type_name = _schema_info.m_class_map[element_type->get_name()]._class_name;
					}
				    property_info._type = PROPERTY_TYPE::E_COLLECTION_ENUM;
				}
			}

			set_default_value(property_info, property_type);
			set_strong_type_name(property_info);

			class_property_map[property_type->get_name()] = property_info;
		}

		_schema_info.m_class_property_map[entity_type->get_name()] = class_property_map;
	}

	auto complex_types = schema->get_complex_types();
	for (auto complex_type_iter = complex_types.cbegin(); complex_type_iter != complex_types.cend(); complex_type_iter++)
	{
		auto complex_type = complex_type_iter->second;

		class_property_info  class_property_map;
		for (auto property_iter = complex_type->begin(); property_iter != complex_type->end(); property_iter++)
		{
			auto property_type = property_iter->second;

			if (property_type->get_property_type()->get_type_kind() == edm_type_kind_t::Unknown)
			{
				continue;
			}

			property_info property_info;

			property_info._edm_name = property_type->get_name();
			property_info._class_member_name = property_type->get_name();
			property_info._is_nullable = property_type->is_nullable();
			property_info._default_value = property_type->default_value();
			std::transform(property_info._class_member_name.begin(), property_info._class_member_name.end(), property_info._class_member_name.begin(), ::tolower);

			if (property_type->get_property_type()->get_type_kind() == edm_type_kind_t::Primitive)
			{
				property_info._strong_type_name = edm_model_utility::get_strong_type_name_from_edm_type_name(std::dynamic_pointer_cast<edm_primitive_type>(property_type->get_property_type()));
				property_info._type = PROPERTY_TYPE::E_PRIMITIVE;
			}
			else if (property_type->get_property_type()->get_type_kind() == edm_type_kind_t::Complex)
			{
				property_info._strong_type_name = _schema_info.m_class_map[property_type->get_property_type()->get_name()]._class_name;
				property_info._type = PROPERTY_TYPE::E_COMPLEX;
			}
			else if (property_type->get_property_type()->get_type_kind() == edm_type_kind_t::Enum)
			{
				if (_schema_info.m_class_map.find(property_type->get_property_type()->get_name()) == _schema_info.m_class_map.end())
				{
					property_info._strong_type_name = property_type->get_property_type()->get_name();
				}
				else
				{
					property_info._strong_type_name = _schema_info.m_class_map[property_type->get_property_type()->get_name()]._class_name;
				}
				property_info._type = PROPERTY_TYPE::E_ENUM;

			}
			else if (property_type->get_property_type()->get_type_kind() == edm_type_kind_t::Collection)
			{
				// collection of primitive complex enum
				auto collection_type = std::dynamic_pointer_cast<edm_collection_type>(property_type->get_property_type());
				auto element_type = collection_type->get_element_type();
				if (element_type->get_type_kind() == edm_type_kind_t::Complex)
				{
					property_info._strong_type_name = _schema_info.m_class_map[element_type->get_name()]._class_name;
				    property_info._type = PROPERTY_TYPE::E_COLLECTION_COMPLEX;
				}
				else if (element_type->get_type_kind() == edm_type_kind_t::Primitive)
				{
				    property_info._strong_type_name = edm_model_utility::get_strong_type_name_from_edm_type_name(std::dynamic_pointer_cast<edm_primitive_type>(element_type));
				    property_info._type = PROPERTY_TYPE::E_COLLECTION_PRIMITIVE;
				}
				else if (element_type->get_type_kind() == edm_type_kind_t::Enum)
				{
					if (_schema_info.m_class_map.find(element_type->get_name()) == _schema_info.m_class_map.end())
					{
						property_info._strong_type_name = element_type->get_name();
					}
					else
					{
						property_info._strong_type_name = _schema_info.m_class_map[element_type->get_name()]._class_name;
					}				    
				    property_info._type = PROPERTY_TYPE::E_COLLECTION_ENUM;
				}
			}

			set_default_value(property_info, property_type);
			set_strong_type_name(property_info);

			class_property_map[property_type->get_name()] = property_info;
		}

		_schema_info.m_class_property_map[complex_type->get_name()] = class_property_map;
	}

	auto operation_types = schema->get_operation_types();
	for (auto operation_type_iter = operation_types.cbegin(); operation_type_iter != operation_types.cend(); operation_type_iter++)
	{
		auto operation_type = operation_type_iter->second;

		if (operation_type->is_bound() && operation_type->get_operation_parameters().size() > 0)
		{
			auto bounded_type = operation_type->get_operation_parameters()[0]->get_param_type();
			if (!bounded_type)
			{
				continue;
			}

			// only support function action bounded to single entity
			if (bounded_type->get_type_kind() != edm_type_kind_t::Entity)
			{
				continue;
			}

			// find the entity which function / action bouned to
			if (_schema_info.m_class_property_map.find(bounded_type->get_name()) == _schema_info.m_class_property_map.end())
			{
				continue;
			}

			auto class_property_map = _schema_info.m_class_property_map[bounded_type->get_name()];

			property_info function_property_info;

			function_property_info._edm_name = operation_type->get_name();
			function_property_info._class_member_name = operation_type->get_name();
			set_operation_info(function_property_info, operation_type, _schema_info);

			class_property_map[operation_type->get_name()] = function_property_info;

			_schema_info.m_class_property_map[bounded_type->get_name()] = class_property_map;
		}
	}

	std::map<::utility::string_t, ::utility::string_t> enum_member_name;
	auto enum_types = schema->get_enum_types();
	for (auto enum_type_iter = enum_types.cbegin(); enum_type_iter != enum_types.cend(); enum_type_iter++)
	{
		class_property_info  class_property_map;

		auto enum_type = std::dynamic_pointer_cast<edm_enum_type>(enum_type_iter->second);
		auto enum_members = enum_type->get_enum_members();
		for (auto enum_member_iter = enum_members.cbegin(); enum_member_iter != enum_members.cend(); enum_member_iter++)
		{
			auto enum_member = *enum_member_iter;
		    property_info property_info;

			property_info._edm_name = enum_member->get_enum_member_name();

			if (enum_member_name.find(enum_member->get_enum_member_name()) == enum_member_name.end())
			{
				property_info._class_member_name = enum_member->get_enum_member_name();
			}
			else
			{
				property_info._class_member_name = enum_type->get_name() + U("_") + enum_member->get_enum_member_name();
			}
			
			enum_member_name[property_info._class_member_name] = property_info._class_member_name;
			property_info._default_value = ::utility::conversions::print_string(enum_member->get_enum_member_value());

			class_property_map[enum_member->get_enum_member_name()] = property_info;
		}

		_schema_info.m_class_property_map[enum_type->get_name()] = class_property_map;
	}

	auto entity_containers = schema->get_containers();
	if (entity_containers.size() == 1)
	{
		auto entity_container = schema->find_container();

		class_property_info  class_property_map;
		for (auto entity_set_iter = entity_container->begin(); entity_set_iter != entity_container->end(); entity_set_iter++)
		{
			auto entity_set = entity_set_iter->second;
			
			property_info property_info;

			property_info._edm_name = entity_set->get_name();
			property_info._strong_type_name = entity_set->get_entity_type()->get_name();
			property_info._class_member_name = entity_set->get_name();
			std::transform(property_info._class_member_name.begin(), property_info._class_member_name.end(), property_info._class_member_name.begin(), ::tolower);
			property_info._class_member_name = U("create_") + property_info._class_member_name + U("_query()");
			property_info._type = PROPERTY_TYPE::E_CONTAINER_ENTITY_SET;

			class_property_map[property_info._class_member_name] = property_info;
		}

		for (auto singleton_iter = entity_container->get_singletons().cbegin(); singleton_iter != entity_container->get_singletons().cend(); singleton_iter++)
		{
			auto singleton = singleton_iter->second;

			property_info property_info;

			property_info._edm_name = singleton->get_name();
			property_info._strong_type_name = singleton->get_entity_type()->get_name();
			property_info._class_member_name = singleton->get_name();
			std::transform(property_info._class_member_name.begin(), property_info._class_member_name.end(), property_info._class_member_name.begin(), ::tolower);
			property_info._class_member_name = U("create_") + property_info._class_member_name + U("_query()");
			property_info._type = PROPERTY_TYPE::E_CONTAINER_SINGLETON;

			class_property_map[property_info._class_member_name] = property_info;
		}

		for (auto operation_imports_iter = entity_container->get_operation_imports().cbegin(); operation_imports_iter != entity_container->get_operation_imports().cend(); operation_imports_iter++)
		{
			auto operation_import = operation_imports_iter->second;

			if (!operation_import)
			{
				continue;
			}

			auto operation_type = operation_import->get_operation_type();

			if (!operation_type)
			{
				continue;
			}

			property_info function_property_info;

			function_property_info._edm_name = operation_type->get_name(); 
			function_property_info._class_member_name = operation_type->get_name();
			set_operation_info(function_property_info, operation_type, _schema_info);

			class_property_map[operation_import->get_name()] = function_property_info;
		}

		_schema_info.m_class_property_map[entity_container->get_name()] = class_property_map;
	}

}

void odata_codegen_initializer::set_operation_info(property_info& _info, const std::shared_ptr<::odata::edm::edm_operation_type>& operation_type, schema_info& _schema_info)
{
	if (!operation_type)
	{
		return ;
	}

	_info._type = operation_type->is_function() ? PROPERTY_TYPE::E_FUNCTION : PROPERTY_TYPE::E_ACTION;

	auto _operation_info = std::make_shared<operation_info>();

	auto ret_type = operation_type->get_operation_return_type();
	if (!ret_type)
	{
		_operation_info->_return_type = U("void");
		_operation_info->_executor_name = U("odata_void_query_executor");
	}
	else
	{
		if (ret_type->get_type_kind() == edm_type_kind_t::Collection)
		{
			auto element_type = std::dynamic_pointer_cast<edm_collection_type>(ret_type)->get_element_type();
			if (!element_type)
			{
				return ;
			}

			_operation_info->_return_type = element_type->get_name();

			if (element_type->get_type_kind() == edm_type_kind_t::Primitive)
			{
				_operation_info->_return_type = edm_model_utility::get_strong_type_name_from_edm_type_name(std::dynamic_pointer_cast<edm_primitive_type>(element_type));
			}
			else if (element_type->get_type_kind() == edm_type_kind_t::Complex)
			{
				_operation_info->_return_type = _schema_info.m_class_map[element_type->get_name()]._class_name;
			}
			else if (element_type->get_type_kind() == edm_type_kind_t::Entity)
			{
				_operation_info->_return_type = _schema_info.m_class_map[element_type->get_name()]._class_name;
			}
			else if (element_type->get_type_kind() == edm_type_kind_t::Enum)
			{
				_operation_info->_return_type = _schema_info.m_class_map[element_type->get_name()]._class_name;
			}

			_operation_info->_executor_name = get_executor_name_from_edm_type(element_type);
		}
		else
		{
			if (ret_type->get_type_kind() == edm_type_kind_t::Primitive)
			{
				_operation_info->_return_type = edm_model_utility::get_strong_type_name_from_edm_type_name(std::dynamic_pointer_cast<edm_primitive_type>(ret_type));
			}
			else if (ret_type->get_type_kind() == edm_type_kind_t::Complex)
			{
				_operation_info->_return_type = _schema_info.m_class_map[ret_type->get_name()]._class_name;
			}
			else if (ret_type->get_type_kind() == edm_type_kind_t::Entity)
			{
				_operation_info->_return_type = _schema_info.m_class_map[ret_type->get_name()]._class_name;
			}
			else if (ret_type->get_type_kind() == edm_type_kind_t::Enum)
			{
				_operation_info->_return_type = _schema_info.m_class_map[ret_type->get_name()]._class_name;
			}

			_operation_info->_executor_name = get_executor_name_from_edm_type(ret_type);
		}
	}

	bool first = true;
	for (auto param_iter = operation_type->get_operation_parameters().cbegin(); param_iter != operation_type->get_operation_parameters().cend(); param_iter++)
	{
		if (first && operation_type->is_bound())
		{
			first = false;
			continue;
		}

		operation_param param;

		auto param_type = (*param_iter)->get_param_type();

		if (!param_type)
		{
			continue;
		}

		param._edm_name = (*param_iter)->get_param_name();
		if (param_type->get_type_kind() == edm_type_kind_t::Primitive)
		{
			param._member_strong_type_name = edm_model_utility::get_strong_type_name_from_edm_type_name(std::dynamic_pointer_cast<edm_primitive_type>(param_type));
		}
		else if (param_type->get_type_kind() == edm_type_kind_t::Complex)
		{
			param._member_strong_type_name = _schema_info.m_class_map[param_type->get_name()]._class_name;
		}
		else if (param_type->get_type_kind() == edm_type_kind_t::Entity)
		{
			param._member_strong_type_name = _schema_info.m_class_map[param_type->get_name()]._class_name;
		}
		else if (param_type->get_type_kind() == edm_type_kind_t::Enum)
		{
			param._member_strong_type_name = _schema_info.m_class_map[param_type->get_name()]._class_name;
		}
		else if (param_type->get_type_kind() == edm_type_kind_t::Collection)
		{		
            auto collection_type = std::dynamic_pointer_cast<edm_collection_type>(param_type);
			auto element_type = collection_type->get_element_type();

			if (element_type->get_type_kind() == edm_type_kind_t::Complex)
			{
				param._member_strong_type_name = _schema_info.m_class_map[element_type->get_name()]._class_name;
			}
			else if (element_type->get_type_kind() == edm_type_kind_t::Primitive)
			{
				param._member_strong_type_name = edm_model_utility::get_strong_type_name_from_edm_type_name(std::dynamic_pointer_cast<edm_primitive_type>(element_type));
			}
			else if (element_type->get_type_kind() == edm_type_kind_t::Enum)
			{
				param._member_strong_type_name = _schema_info.m_class_map[element_type->get_name()]._class_name;
			}

			param._member_strong_type_name = U("std::vector<") + param._member_strong_type_name;
			param._member_strong_type_name += U(">");
		}

		param._member_name = (*param_iter)->get_param_name();

		_operation_info->vec_params.push_back(param);
	}
			
	_info._operation_info = _operation_info;
}

::utility::string_t odata_codegen_initializer::get_executor_name_from_edm_type(const std::shared_ptr<::odata::edm::edm_named_type>& type)
{
	::utility::string_t ret;

	if (type)
	{
		if (type->get_type_kind() == edm_type_kind_t::Entity)
		{
			ret = U("odata_entityset_query_executor");
		}
		else if (type->get_type_kind() == edm_type_kind_t::Complex)
		{
			ret = U("odata_complex_query_executor");
		}
		else if (type->get_type_kind() == edm_type_kind_t::Enum)
		{
			ret = U("odata_enum_query_executor");
		}
		else if (type->get_type_kind() == edm_type_kind_t::Primitive)
		{
			ret = U("odata_primitive_query_executor");
		}
	}

	return ret;
}

void odata_codegen_initializer::set_default_value(property_info& _property_info, const std::shared_ptr<::odata::edm::edm_property_type>& property_type)
{
	if (!property_type)
	{
		return ;
	}

	if (!_property_info._default_value.empty())
	{
		return ;
	}

	if (property_type->get_property_type()->get_type_kind() == edm_type_kind_t::Primitive)
	{
		if (_property_info._is_nullable == true)
	    {
			_property_info._default_value = U("nullptr");
	    }
		else
		{
			_property_info._default_value 
				= edm_model_utility::get_strong_type_default_value_from_edm_type_name(std::dynamic_pointer_cast<edm_primitive_type>(property_type->get_property_type()));
		}
	}
	else if (property_type->get_property_type()->get_type_kind() == edm_type_kind_t::Complex)
	{
		if (_property_info._is_nullable == true)
	    {
			_property_info._default_value = U("nullptr");
	    }
	}
	else if (property_type->get_property_type()->get_type_kind() == edm_type_kind_t::Navigation)
	{
		if (_property_info._type == PROPERTY_TYPE::E_ENTITY)
		{
			if (_property_info._is_nullable == true)
			{
				_property_info._default_value = U("nullptr");
			}
		}
	}
	else if (property_type->get_property_type()->get_type_kind() == edm_type_kind_t::Enum)
	{
		if (_property_info._is_nullable == true)
	    {
			_property_info._default_value = U("nullptr");
	    }
	}
}

void odata_codegen_initializer::set_strong_type_name(property_info& _property_info)
{
	_property_info._class_member_type = _property_info._strong_type_name;

	if (_property_info._type == PROPERTY_TYPE::E_COLLECTION_PRIMITIVE 
		|| _property_info._type == PROPERTY_TYPE::E_COLLECTION_COMPLEX
		|| _property_info._type == PROPERTY_TYPE::E_COLLECTION_ENTITY
		|| _property_info._type == PROPERTY_TYPE::E_COLLECTION_ENUM)
	{
		_property_info._class_member_type = U("std::vector<") + _property_info._strong_type_name + U(">");
	}
	else if (_property_info._type == PROPERTY_TYPE::E_ENTITY || _property_info._type == PROPERTY_TYPE::E_COMPLEX)
	{
		if (_property_info._is_nullable == true)
		{
			_property_info._class_member_type = U("std::shared_ptr<") + _property_info._strong_type_name + U(">");
		}
	}
	else if (_property_info._type == PROPERTY_TYPE::E_PRIMITIVE || _property_info._type == PROPERTY_TYPE::E_ENUM)
	{
		if (_property_info._is_nullable == true)
		{
			_property_info._class_member_type = _property_info._strong_type_name;
		}
	}
}

}}}