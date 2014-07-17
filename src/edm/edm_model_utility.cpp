#include "odata/edm/edm_model_utility.h"

namespace odata { namespace edm
{

const std::unordered_map<::utility::string_t, std::shared_ptr<edm_named_type>>::value_type init_name_to_prmitive_value[] = 
{
	std::unordered_map<::utility::string_t, std::shared_ptr<edm_named_type>>::value_type(U("Edm.Binary"), edm_primitive_type::BINARY()),
	std::unordered_map<::utility::string_t, std::shared_ptr<edm_named_type>>::value_type(U("Edm.Boolean"), edm_primitive_type::BOOLEAN()),
	std::unordered_map<::utility::string_t, std::shared_ptr<edm_named_type>>::value_type(U("Edm.Byte"), edm_primitive_type::BYTE()),
	std::unordered_map<::utility::string_t, std::shared_ptr<edm_named_type>>::value_type(U("Edm.Duration"), edm_primitive_type::DURATION()),
	std::unordered_map<::utility::string_t, std::shared_ptr<edm_named_type>>::value_type(U("Edm.DateTimeOffset"), edm_primitive_type::DATETIMEOFFSET()),
	std::unordered_map<::utility::string_t, std::shared_ptr<edm_named_type>>::value_type(U("Edm.Double"), edm_primitive_type::DOUBLE()),
	std::unordered_map<::utility::string_t, std::shared_ptr<edm_named_type>>::value_type(U("Edm.Decimal"), edm_primitive_type::DECIMAL()),
	std::unordered_map<::utility::string_t, std::shared_ptr<edm_named_type>>::value_type(U("Edm.Guid"), edm_primitive_type::GUID()),
	std::unordered_map<::utility::string_t, std::shared_ptr<edm_named_type>>::value_type(U("Edm.Int16"), edm_primitive_type::INT16()),
	std::unordered_map<::utility::string_t, std::shared_ptr<edm_named_type>>::value_type(U("Edm.Int32"), edm_primitive_type::INT32()),
	std::unordered_map<::utility::string_t, std::shared_ptr<edm_named_type>>::value_type(U("Edm.Int64"), edm_primitive_type::INT64()),
	std::unordered_map<::utility::string_t, std::shared_ptr<edm_named_type>>::value_type(U("Edm.SByte"), edm_primitive_type::SBYTE()),
	std::unordered_map<::utility::string_t, std::shared_ptr<edm_named_type>>::value_type(U("Edm.Single"), edm_primitive_type::SINGLE()),
	std::unordered_map<::utility::string_t, std::shared_ptr<edm_named_type>>::value_type(U("Edm.Stream"), edm_primitive_type::STREAM()),
	std::unordered_map<::utility::string_t, std::shared_ptr<edm_named_type>>::value_type(U("Edm.String"), edm_primitive_type::STRING()),

};

const static std::unordered_map<::utility::string_t, std::shared_ptr<edm_named_type>> name_to_edm_primitive_type_map(init_name_to_prmitive_value, init_name_to_prmitive_value + 15);

const std::unordered_map<::utility::string_t, ::utility::string_t>::value_type init_edm_prmitive_type_name_to_strong_type_name[] = 
{
	std::unordered_map<::utility::string_t, ::utility::string_t>::value_type(U("Edm.Binary"), U("std::vector<unsigned char>")),
	std::unordered_map<::utility::string_t, ::utility::string_t>::value_type(U("Edm.Boolean"), U("bool")),
	std::unordered_map<::utility::string_t, ::utility::string_t>::value_type(U("Edm.Byte"), U("char")),
	std::unordered_map<::utility::string_t, ::utility::string_t>::value_type(U("Edm.Duration"), U("::utility::seconds")),
	std::unordered_map<::utility::string_t, ::utility::string_t>::value_type(U("Edm.DateTimeOffset"), U("::utility::datetime")),
	std::unordered_map<::utility::string_t, ::utility::string_t>::value_type(U("Edm.Double"), U("double")),
	std::unordered_map<::utility::string_t, ::utility::string_t>::value_type(U("Edm.Decimal"), U("long double")),
	std::unordered_map<::utility::string_t, ::utility::string_t>::value_type(U("Edm.Guid"), U("::utility::string_t")),
	std::unordered_map<::utility::string_t, ::utility::string_t>::value_type(U("Edm.Int16"), U("int16_t")),
	std::unordered_map<::utility::string_t, ::utility::string_t>::value_type(U("Edm.Int32"), U("int32_t")),
	std::unordered_map<::utility::string_t, ::utility::string_t>::value_type(U("Edm.Int64"), U("int64_t")),
	std::unordered_map<::utility::string_t, ::utility::string_t>::value_type(U("Edm.SByte"), U("unsigned char")),
	std::unordered_map<::utility::string_t, ::utility::string_t>::value_type(U("Edm.Single"), U("float")),
	std::unordered_map<::utility::string_t, ::utility::string_t>::value_type(U("Edm.String"), U("::utility::string_t")),
};

const static std::unordered_map<::utility::string_t, ::utility::string_t> edm_prmitive_type_name_to_strong_type_name_map(init_edm_prmitive_type_name_to_strong_type_name, init_edm_prmitive_type_name_to_strong_type_name + 14);

const std::unordered_map<::utility::string_t, ::utility::string_t>::value_type init_edm_prmitive_type_name_to_strong_type_default_value[] = 
{
	std::unordered_map<::utility::string_t, ::utility::string_t>::value_type(U("Edm.Binary"), U("")),
	std::unordered_map<::utility::string_t, ::utility::string_t>::value_type(U("Edm.Boolean"), U("false")),
	std::unordered_map<::utility::string_t, ::utility::string_t>::value_type(U("Edm.Byte"), U("'\0'")),
	std::unordered_map<::utility::string_t, ::utility::string_t>::value_type(U("Edm.Duration"), U("")),
	std::unordered_map<::utility::string_t, ::utility::string_t>::value_type(U("Edm.DateTimeOffset"), U("")),
	std::unordered_map<::utility::string_t, ::utility::string_t>::value_type(U("Edm.Double"), U("0.0")),
	std::unordered_map<::utility::string_t, ::utility::string_t>::value_type(U("Edm.Decimal"), U("0.0")),
	std::unordered_map<::utility::string_t, ::utility::string_t>::value_type(U("Edm.Guid"), U("")),
	std::unordered_map<::utility::string_t, ::utility::string_t>::value_type(U("Edm.Int16"), U("0")),
	std::unordered_map<::utility::string_t, ::utility::string_t>::value_type(U("Edm.Int32"), U("0")),
	std::unordered_map<::utility::string_t, ::utility::string_t>::value_type(U("Edm.Int64"), U("0")),
	std::unordered_map<::utility::string_t, ::utility::string_t>::value_type(U("Edm.SByte"), U("'\0'")),
	std::unordered_map<::utility::string_t, ::utility::string_t>::value_type(U("Edm.Single"), U("0.0")),
	std::unordered_map<::utility::string_t, ::utility::string_t>::value_type(U("Edm.String"), U("")),
};

const static std::unordered_map<::utility::string_t, ::utility::string_t> edm_prmitive_type_name_to_strong_type_default_value_map(init_edm_prmitive_type_name_to_strong_type_default_value, init_edm_prmitive_type_name_to_strong_type_default_value + 14);

std::shared_ptr<edm_named_type> edm_model_utility::get_edm_primitive_type_from_name(const ::utility::string_t& type_name)
{
	if (type_name.substr(0, 4) == U("Edm."))
	{
		auto find_iter = name_to_edm_primitive_type_map.find(type_name);
		if (find_iter != name_to_edm_primitive_type_map.end())
		{
			return find_iter->second;
		}
	}

	return nullptr;
}

::utility::string_t edm_model_utility::get_strong_type_name_from_edm_type_name(const std::shared_ptr<edm_primitive_type>& primitive_type)
{
	::utility::string_t ret;

	auto find_iter = edm_prmitive_type_name_to_strong_type_name_map.find(primitive_type->get_name());
	if (find_iter != edm_prmitive_type_name_to_strong_type_name_map.end())
	{
		ret = find_iter->second;
	}

	return ret;
}

::utility::string_t edm_model_utility::get_strong_type_default_value_from_edm_type_name(const std::shared_ptr<edm_primitive_type>& primitive_type)
{
	::utility::string_t ret;
	
	auto find_iter = edm_prmitive_type_name_to_strong_type_default_value_map.find(primitive_type->get_name());
	if (find_iter != edm_prmitive_type_name_to_strong_type_default_value_map.end())
	{
		ret = find_iter->second;
	}

	return ret;
}

std::shared_ptr<edm_named_type> edm_model_utility::get_edm_type_from_name(const ::utility::string_t& type_name)
{
	if (type_name.substr(0, 4) == U("Edm."))
	{
		return edm_model_utility::get_edm_primitive_type_from_name(type_name);
	}
	else if (type_name.substr(0, 10) == U("Collection"))
	{
		int index_first = type_name.find_first_of('(') + 1;
		int index_last = type_name.find_last_of(')');
		::utility::string_t element_name = type_name.substr(index_first, index_last - index_first);

		auto collection = std::make_shared<edm_collection_type>(type_name);

		if (collection)
		{
			auto element_type = edm_model_utility::get_edm_type_from_name(element_name);
			if (!element_type)
			{
				element_type = std::make_shared<edm_named_type>(element_name, U(""), edm_type_kind_t::Unknown);
			}

		    collection->set_element_type(element_type);
		}

		return collection;
	}
	else
	{
		return nullptr;
	}
}

::utility::string_t edm_model_utility::get_type_kind_name_from_edm_type(const std::shared_ptr<edm_named_type>& ptype)
{
    switch(ptype->get_type_kind())
    {
	case edm_type_kind_t::Primitive:
        return ::utility::string_t(U("Primitive Type."));
	case edm_type_kind_t::Collection:
        return ::utility::string_t(U("Collection Type."));
	case edm_type_kind_t::Complex:
        return ::utility::string_t(U("Complex Type."));
	case edm_type_kind_t::Enum:
        return ::utility::string_t(U("Enum Type."));
	case edm_type_kind_t::Entity:
        return ::utility::string_t(U("Entity Type."));
	case edm_type_kind_t::Navigation:
        return ::utility::string_t(U("Navigation Type."));
	case edm_type_kind_t::Operation:
        return ::utility::string_t(U("Operation Type."));
    default:
        return ::utility::string_t(U("Unknown Type."));
    }
}

bool edm_model_utility::get_primitive_kind_from_edm_type(const std::shared_ptr<edm_named_type>& edm_type, edm_primitive_type_kind_t& primitive_kind)
{
	if (edm_type && edm_type->get_type_kind() == edm_type_kind_t::Primitive)
	{
		std::shared_ptr<edm_primitive_type> primitive_type = std::dynamic_pointer_cast<edm_primitive_type>(edm_type);

		if (primitive_type)
		{
			primitive_kind = primitive_type->get_primitive_kind();
			return true;
		}
	}

	return false;
}

void edm_model_utility::resolve_edm_types_after_parsing(const std::shared_ptr<edm_model>& model)
{
	if (!model)
	{
		return ;
	}

	for (auto sc = model->get_schema().cbegin(); sc != model->get_schema().cend(); ++sc)
	{
		std::unordered_map<::utility::string_t, std::shared_ptr<edm_entity_type>> entity_types = (*sc)->get_entity_types();
		for (auto entity_type_iter = entity_types.cbegin(); entity_type_iter != entity_types.cend(); ++entity_type_iter)
		{
			resolve_type_under_structured_type(model, entity_type_iter->second);

			resovle_entity_base_type(model, entity_type_iter->second);
		}

		std::unordered_map<::utility::string_t, std::shared_ptr<edm_complex_type>> complex_types = (*sc)->get_complex_types();
		for (auto complex_type_iter = complex_types.cbegin(); complex_type_iter != complex_types.cend(); ++complex_type_iter)
		{
			resolve_type_under_structured_type(model, complex_type_iter->second);

			resovle_complex_base_type(model, complex_type_iter->second);
		}

		std::unordered_map<::utility::string_t, std::shared_ptr<edm_operation_type>> operation_types = (*sc)->get_operation_types();
		for (auto operation_type_iter = operation_types.cbegin(); operation_type_iter != operation_types.cend(); ++operation_type_iter)
		{
			resolve_type_under_operation_type(model, operation_type_iter->second);
		}

		std::unordered_map<::utility::string_t, std::shared_ptr<edm_entity_container>> entity_containers = (*sc)->get_containers();
		for (auto entity_container_iter = entity_containers.cbegin(); entity_container_iter != entity_containers.cend(); ++entity_container_iter)
		{
			resolve_type_under_entity_container(model, entity_container_iter->second);

			resolve_navigation_path_for_non_contained_navigation(model, entity_container_iter->second);
		}
	}
}


std::shared_ptr<edm_named_type> edm_model_utility::resolve_type_from_name(const std::shared_ptr<edm_model>& model, ::utility::string_t qualified_name)
{
	if (!model)
	{
		return nullptr;
	}

	auto enum_type = model->find_enum_type(qualified_name);
	if (enum_type)
	{
		return enum_type;
	}

	auto complex_type = model->find_complex_type(qualified_name);
	if (complex_type)
	{
		return complex_type;
	}
	
	auto entity_type = model->find_entity_type(qualified_name);
	if (entity_type)
	{
		return entity_type;
	}

	auto operation_type = model->find_operation_type(qualified_name);
	if (operation_type)
	{
		return operation_type;
	}

	if (qualified_name.substr(0, 4) == U("Edm."))
	{
		auto prmitive_type = edm_model_utility::get_edm_primitive_type_from_name(qualified_name);

		if (!prmitive_type)
		{
			return edm_named_type::EDM_UNKNOWN();
		}

		return prmitive_type;
	}
	else if (qualified_name.substr(0, 10) == U("Collection"))
	{
		int index_first = qualified_name.find_first_of('(') + 1;
		int index_last = qualified_name.find_last_of(')');

		if (index_first >= index_last)
		{
			auto err = std::string("Invalid collection type name : ") + ::utility::conversions::to_utf8string(qualified_name);
			throw std::invalid_argument(err);
		}

		::utility::string_t element_name = qualified_name.substr(index_first, index_last - index_first);

		auto collection = std::make_shared<edm_collection_type>(qualified_name);

		if (collection)
		{
			collection->set_element_type(resolve_type_from_name(model, element_name));
		}

		return collection;
	}

	return nullptr;
}

::utility::string_t edm_model_utility::get_collection_element_name(const ::utility::string_t& collection_full_name)
{
	::utility::string_t element_name;

	if (collection_full_name.substr(0, 10) == U("Collection"))
	{
		int index_first = collection_full_name.find_first_of('(') + 1;
		int index_last = collection_full_name.find_last_of(')');

		if (index_first >= index_last)
		{
			auto err = std::string("Invalid collection type name : ") + ::utility::conversions::to_utf8string(collection_full_name);
			throw std::invalid_argument(err);
		}

		element_name = collection_full_name.substr(index_first, index_last - index_first);
	}

	return element_name;
}

void edm_model_utility::resolve_type_under_structured_type(const std::shared_ptr<edm_model>& model, const std::shared_ptr<edm_structured_type>& structyred_type)
{
	if (!model)
	{
		return ;
	}

	if (structyred_type)
	{
		for (auto propery_iter = structyred_type->begin(); propery_iter != structyred_type->end(); ++propery_iter)
		{
			auto prop = propery_iter->second;

			auto property_type = prop->get_property_type();
			if (!property_type)
			{
				continue;
			}

			if (property_type->get_type_kind() == edm_type_kind_t::Unknown)
			{
				::utility::string_t type_name = property_type->get_name();

				auto resolved_type = resolve_type_from_name(model, type_name);
				if (resolved_type)
				{
					prop->set_property_type(resolved_type);
				}
			}
			else if (property_type->get_type_kind() == edm_type_kind_t::Collection)
			{
				auto collection_type = std::dynamic_pointer_cast<edm_collection_type>(property_type);
				if (collection_type)
				{
					::utility::string_t type_name = get_collection_element_name(collection_type->get_full_name());

					auto resolved_type = resolve_type_from_name(model, type_name);
					if (resolved_type)
					{
						collection_type->set_element_type(resolved_type);
					}
				}
			}
			else if (property_type->get_type_kind() == edm_type_kind_t::Navigation)
			{
                auto navigation_type = std::dynamic_pointer_cast<edm_navigation_type>(property_type);
				if (navigation_type)
				{
					auto resolved_type = resolve_type_from_name(model, navigation_type->get_name());
					if (resolved_type->get_type_kind() == edm_type_kind_t::Collection)
					{
						model->get_schema()[0]->m_collection_navigation_types.push_back(std::dynamic_pointer_cast<edm_collection_type>(resolved_type));
					}
					navigation_type->set_navigation_type(resolved_type);
				}
			}
		}
	}
}

void edm_model_utility::resolve_type_under_operation_type(const std::shared_ptr<edm_model>& model, const std::shared_ptr<edm_operation_type>& operationType)
{
	if (!model)
	{
		return ;
	}

	if (operationType)
	{
		for (auto paramter_type_iter = operationType->get_operation_parameters().cbegin(); paramter_type_iter != operationType->get_operation_parameters().cend(); ++paramter_type_iter)
		{
			auto resolved_type = resolve_undetermined_type(model, (*paramter_type_iter)->get_param_type());
			if (resolved_type)
			{
				(*paramter_type_iter)->set_param_type(resolved_type);
			}
		}

		auto resolved_type = resolve_undetermined_type(model, operationType->get_operation_return_type());
		if (resolved_type)
		{
			operationType->set_return_type(resolved_type);
		}
	}
}

std::shared_ptr<edm_named_type> edm_model_utility::resolve_undetermined_type(const std::shared_ptr<edm_model>& model, const std::shared_ptr<edm_named_type>& undeterminedType)
{
	if (!model)
	{
		return nullptr;
	}

	if (undeterminedType)
	{
	    auto resolved_type = std::make_shared<edm_named_type>(undeterminedType->get_name(), U(""), edm_type_kind_t::None);

		if (undeterminedType->get_type_kind() == edm_type_kind_t::Unknown)
		{
			::utility::string_t type_name = undeterminedType->get_name();
			if (type_name.substr(0, 10) == U("Collection"))
			{
				resolved_type = std::make_shared<edm_collection_type>(type_name);
				if (resolved_type)
				{
				    int index_first = type_name.find_first_of('(') + 1;
		            int index_last = type_name.find_last_of(')');

					if (index_first >= index_last)
					{
						auto err = std::string("Invalid collection type name : ") + ::utility::conversions::to_utf8string(type_name);
						throw std::invalid_argument(err);
					}

		            ::utility::string_t element_name = type_name.substr(index_first, index_last - index_first);
					auto element_type = resolve_undetermined_type(model, std::make_shared<edm_named_type>(element_name, U(""), edm_type_kind_t::Unknown));

					auto collection_type = std::dynamic_pointer_cast<edm_collection_type>(resolved_type);
					if (collection_type)
					{
						collection_type->set_element_type(element_type);
					}
				}
			}
			else
			{
				resolved_type = resolve_type_from_name(model, type_name);
			}
		}
		else if (undeterminedType->get_type_kind() == edm_type_kind_t::Collection)
		{
			auto collection_type = std::dynamic_pointer_cast<edm_collection_type>(resolved_type);
			if (collection_type)
			{
				auto baseTypeOfColloection = collection_type->get_element_type();
				if (baseTypeOfColloection && baseTypeOfColloection->get_type_kind() == edm_type_kind_t::Unknown)
				{
					auto element_type = resolve_undetermined_type(model, std::make_shared<edm_named_type>(baseTypeOfColloection->get_name(), U(""), edm_type_kind_t::Unknown));
					if (element_type)
					{
						collection_type->set_element_type(element_type);
					}
				}
			}
		}

		return resolved_type;
	}

	return nullptr;
}

void edm_model_utility::resolve_type_under_entity_container(const std::shared_ptr<edm_model>& model, const std::shared_ptr<edm_entity_container>& entity_container)
{
	if (!model || !entity_container)
	{
		return ;
	}

	auto operation_imports = entity_container->get_operation_imports();
	for (auto operation_import_iter = operation_imports.cbegin(); operation_import_iter != operation_imports.cend(); ++operation_import_iter)
	{
		auto op = operation_import_iter->second;
		if (op)
		{
			::utility::string_t operation_name = op->get_name();
			op->set_operation_type(model->find_operation_type(operation_name));
		}
	}

	auto singletons = entity_container->get_singletons();
	for (auto singleton_iter = singletons.cbegin(); singleton_iter != singletons.cend(); ++singleton_iter)
	{
		auto singleton = singleton_iter->second;
		if (!singleton)
		{
			continue;
		}

		singleton->set_entity_type(model->find_entity_type(singleton->get_entity_type_name()));
	}

	for (auto entity_set_iter = entity_container->begin(); entity_set_iter != entity_container->end(); ++entity_set_iter)
	{
		auto entity_set = entity_set_iter->second;
		if (!entity_set)
		{
			continue;
		}

		entity_set->set_entity_type(model->find_entity_type(entity_set->get_entity_type_name()));
	}
}

void edm_model_utility::resolve_navigation_path_for_non_contained_navigation(const std::shared_ptr<edm_model>& model, const std::shared_ptr<edm_entity_container>& entity_container)
{
	if (!model || !entity_container)
	{
		return ;
	}

	for (auto entity_set_iter = entity_container->begin(); entity_set_iter != entity_container->end(); ++entity_set_iter)
	{
		auto entity_set = entity_set_iter->second;
		if (!entity_set)
		{
			continue;
		}

		auto navigation_sources = entity_set->get_navigation_sources();
		for (auto navigation_source_iter = navigation_sources.begin(); navigation_source_iter != navigation_sources.end(); ++navigation_source_iter)
		{
			auto navigation_type = get_navigation_property_from_path(model, entity_set->get_entity_type(), navigation_source_iter->first);
			if (!navigation_type)
			{
				continue;
			}

			std::shared_ptr<edm_navigation_source> target = entity_container->find_entity_set(navigation_source_iter->second);
			if (!target)
			{
				target = entity_container->find_singleton(navigation_source_iter->second);
			}
			navigation_type->set_binded_navigation_source(target);
		}
	}
}

std::shared_ptr<edm_navigation_type> edm_model_utility::get_navigation_property_from_path(const std::shared_ptr<edm_model>& model, 
    const std::shared_ptr<edm_entity_type>& entity_type, const ::utility::string_t& navigation_path)
{
	std::shared_ptr<edm_navigation_type> navigation_type = nullptr;

	std::list<::utility::string_t> paths;
	::utility::string_t path = navigation_path;
	::odata::utility::split_string(path, U("/"), paths);

	if (paths.size() == 1)
	{
		// property name
		if (entity_type)
		{
			auto navigation_property = entity_type->find_property(paths.front());

			if (navigation_property)
			{
				navigation_type = std::dynamic_pointer_cast<edm_navigation_type>(navigation_property->get_property_type());
			}		
		}
	}
	else if (paths.size() == 2)
	{
		// full entity name/property name
		auto entity_type_name = paths.front();
		auto property_name = paths.back();

		if (model)
		{
			auto entity_type = model->find_entity_type(entity_type_name);

			if (entity_type)
			{
				auto navigation_property = entity_type->find_property(property_name);

				if (navigation_property)
				{
					navigation_type = std::dynamic_pointer_cast<edm_navigation_type>(navigation_property->get_property_type());
				}
			}
		}
	}

	return navigation_type;
}

void edm_model_utility::resovle_entity_base_type(const std::shared_ptr<edm_model>& model, const std::shared_ptr<edm_entity_type>& entity_type)
{
	if (!model)
	{
		return ;
	}

	if (!entity_type || entity_type->get_base_type_name().empty())
	{
		return ;
	}

	auto base_type = model->find_entity_type(entity_type->get_base_type_name());
	if (base_type)
	{
		entity_type->set_base_type(base_type);
	}
}

void edm_model_utility::resovle_complex_base_type(const std::shared_ptr<edm_model>& model, const std::shared_ptr<edm_complex_type>& complex_type)
{
	if (!model)
	{
		return ;
	}

	if (!complex_type || complex_type->get_base_type_name().empty())
	{
		return ;
	}

	auto base_type = model->find_complex_type(complex_type->get_base_type_name());
	if (base_type)
	{
		complex_type->set_base_type(base_type);
	}
}

std::shared_ptr<edm_named_type> edm_model_utility::get_collection_element_type(const std::shared_ptr<edm_named_type>& input_type)
{
	if (!input_type)
	{
		return nullptr;
	}

	if (input_type->get_type_kind() != edm_type_kind_t::Collection)
	{
		return nullptr;
	}

	auto collection_type = std::dynamic_pointer_cast<edm_collection_type>(input_type);
	if (!collection_type)
	{
		return nullptr;
	}

	return collection_type->get_element_type();
}

std::shared_ptr<edm_named_type> edm_model_utility::get_navigation_element_type(const std::shared_ptr<edm_named_type>& input_type)
{
	if (!input_type)
	{
		return nullptr;
	}

	if (input_type->get_type_kind() != edm_type_kind_t::Navigation)
	{
		return nullptr;
	}

	auto navigation_type = std::dynamic_pointer_cast<edm_navigation_type>(input_type);
	if (!navigation_type)
	{
		return nullptr;
	}

	return navigation_type->get_navigation_type();
}

std::shared_ptr<edm_named_type> edm_model_utility::get_property_type_from_name(const std::shared_ptr<edm_entity_type>& entity_type, const ::utility::string_t& property_name)
{
	if (!entity_type)
	{
		return nullptr;
	}

	auto entity_property = entity_type->find_property(property_name);
	if (!entity_property)
	{
		return nullptr;
	}

	auto property_type = entity_property->get_property_type();

	if (!property_type)
	{
		return nullptr;
	}

	if (property_type->get_type_kind() == edm_type_kind_t::Navigation)
	{
		property_type = get_navigation_element_type(property_type);
	}

	if (property_type->get_type_kind() == edm_type_kind_t::Collection)
	{
		property_type = get_collection_element_type(property_type);
	}

	return property_type;
}

}}