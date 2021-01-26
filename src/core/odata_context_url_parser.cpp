//---------------------------------------------------------------------
// <copyright file="odata_context_url_parser.cpp" company="Microsoft">
//      Copyright (C) Microsoft Corporation. All rights reserved. See License.txt in the project root for license information.
// </copyright>
//---------------------------------------------------------------------

#include "odata/core/odata_context_url_parser.h"
#include "odata/edm/edm_model_utility.h"

using namespace ::odata::edm;

namespace odata { namespace core
{

std::shared_ptr<edm_named_type> odata_contex_url_parser::get_payload_content_type(const ::odata::string_t& context_url)
{
	if (!m_model || m_service_root_url.empty())
	{
		return nullptr;
	}

	::odata::string_t path(context_url, context_url.find(_XPLATSTR("$metadata#")) + 10);

	std::list<::odata::string_t> paths;
	::odata::common::split_string(path, _XPLATSTR("/"), paths);

	return parse_context_url(paths, nullptr);
}

std::shared_ptr<edm_named_type> odata_contex_url_parser::parse_complex_or_primitive(const ::odata::string_t& current_path)
{
	std::shared_ptr<edm_named_type> current_type = edm_model_utility::get_edm_primitive_type_from_name(current_path);

	if (!current_type)
	{
		current_type = m_model->find_complex_type(current_path);
	}

	if (!current_type)
	{
		current_type = m_model->find_entity_type(current_path);
	}

	return current_type;
}

std::shared_ptr<edm_named_type> odata_contex_url_parser::parse_context_url(std::list<::odata::string_t>& paths, const std::shared_ptr<edm_named_type>& prev_type)
{
	if (paths.empty())
	{
		return prev_type;
	}

	::odata::string_t current_path = std::move(paths.front());
	paths.pop_front();

	// check if current path segment is an end word like $entity collection or edm. of primitive type
	if (current_path == _XPLATSTR("$entity"))
	{
		return prev_type;
	}

	size_t index_collection = current_path.find(_XPLATSTR("Collection"));
	if (index_collection == 0)
	{
		current_path = current_path.substr(11, current_path.length() - 12);

		auto return_type = ::odata::make_shared<edm_collection_type>(_XPLATSTR(""));
		return_type->set_element_type(parse_complex_or_primitive(current_path));

		return return_type;
	}

	index_collection = current_path.find(_XPLATSTR("collection"));
	if (index_collection == 0)
	{
		current_path = current_path.substr(11, current_path.length() - 12);

		auto return_type = ::odata::make_shared<edm_collection_type>(_XPLATSTR(""));
		return_type->set_element_type(parse_complex_or_primitive(current_path));

		return return_type;
	}

	if (current_path.compare(0, 4, _XPLATSTR("Edm.")) == 0)
	{
		return parse_complex_or_primitive(current_path);
	}

	// delete content that is in '()'
	size_t index = current_path.find_first_of(_XPLATSTR("("));
	if (index != ::odata::string_t::npos)
	{
		current_path.erase(index);
	}

	// first check if current path segemnt is entity type
	std::shared_ptr<edm_named_type> current_type = m_model->find_entity_type(current_path);
	if (current_type)
	{
		return parse_context_url(paths, current_type);
	}

	// second check if current path segemnt is enum type
	current_type = m_model->find_enum_type(current_path);
	if (current_type)
	{
		return parse_context_url(paths, current_type);
	}

	// third check if current path segement is complex type
	current_type = m_model->find_complex_type(current_path);
	if (current_type)
	{
		return parse_context_url(paths, current_type);
	}

	//  last check if current path segment is entity set
	std::shared_ptr<edm_entity_container> container = m_model->find_container();
	if (container)
	{
		std::shared_ptr<edm_entity_set> current_set = container->find_entity_set(current_path);
		if (current_set)
		{
			return parse_context_url(paths, m_model->find_entity_type(current_set->get_entity_type_name()));
		}

		std::shared_ptr<edm_singleton> current_singleton = container->find_singleton(current_path);
		if (current_singleton)
		{
			return parse_context_url(paths, m_model->find_entity_type(current_singleton->get_entity_type_name()));
		}
	}

	// final check if current path segment is property type
	if (prev_type)
	{
		// In the case of mutliple level property, we need to find the behind type of property.
		std::shared_ptr<edm_named_type> real_prev_type = prev_type;
		if (real_prev_type->get_type_kind() == edm_type_kind_t::Navigation)
		{
			std::shared_ptr<edm_named_type> navigation_type = std::dynamic_pointer_cast<edm_navigation_type>(prev_type)->get_navigation_type();
			if (navigation_type->get_type_kind() == edm_type_kind_t::Collection)
			{
				real_prev_type = std::dynamic_pointer_cast<edm_collection_type>(navigation_type)->get_element_type();
			}
			else
			{
				real_prev_type = navigation_type;
			}
		}

		std::shared_ptr<edm_structured_type> structure_type = std::dynamic_pointer_cast<edm_structured_type>(real_prev_type);

		if (structure_type)
		{
			std::shared_ptr<edm_property_type> property_type = structure_type->find_property(current_path);

			if (property_type)
			{
				return parse_context_url(paths, property_type->get_property_type());
			}
		}
	}

	return nullptr;
}

}}
