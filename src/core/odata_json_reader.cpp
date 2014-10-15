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
 
#include "odata/core/odata_context_url_parser.h"
#include "odata/core/odata_entity_factory.h"
#include "odata/core/odata_json_reader.h"

using namespace ::web;
using namespace ::odata::edm;
using namespace ::odata::communication;
using namespace ::odata::common;

namespace odata { namespace core
{

enum RET_NAVI_ENTITY_TYPE
{
	NAVI_ENTITY_NONE = 0,
	NAVI_ENTITY_SINGLE,
	NAVI_ENTITY_COLLECTION,
	ENTITY_SINGLETON,
	ENTITY_ENTITY_SET,
};

::utility::string_t odata_json_reader::get_edit_link_from_context_url(const ::utility::string_t& context_url)
{
    ::utility::string_t ret = context_url;


	int index = ret.find(U("#"));
	ret = ret.substr(index + 1, ret.length() - index - 1);

	index = ret.find(U("/$entity"));
	if (index != -1)
	{
		ret = ret.substr(0, index);
	}

	int index_last_slash = ret.rfind(U("/"));

	int index_last_bracket = ret.find_first_of(U("("), ++index_last_slash);
	if (index_last_slash < index_last_bracket)
	{
		ret = ret.substr(0, index_last_bracket);
	}

	return ret;
}

void odata_json_reader::set_edit_link_for_entity_value(const std::shared_ptr<odata_entity_value>& entity_value, const ::utility::string_t& expect_type_name, 
									const ::utility::string_t& edit_link, bool is_collection)
{
	if (!entity_value)
	{
		return ;
	}

	auto key_string = is_collection ? entity_value->get_entity_key_string() : U("");
	entity_value->set_value(odata_json_constants::PAYLOAD_ANNOTATION_EDITLINK, edit_link + key_string);

	// check to see if it is an derived type
	if (expect_type_name != entity_value->get_value_type()->get_name())
	{
		::utility::string_t derived_edit_link = edit_link + key_string;
		derived_edit_link += U("/");
		if (entity_value->has_property(odata_json_constants::PAYLOAD_ANNOTATION_TYPE))
		{
			::utility::string_t derived_type;
			entity_value->try_get(odata_json_constants::PAYLOAD_ANNOTATION_TYPE, derived_type);
			if (!derived_type.empty() && derived_type[0] == U('#'))
				derived_type = derived_type.substr(1, derived_type.length() - 1);
			derived_edit_link += derived_type;
			entity_value->set_value(odata_json_constants::PAYLOAD_ANNOTATION_EDITLINK, derived_edit_link);
		}
	}
}

std::shared_ptr<odata_payload> odata_json_reader::deserilize(const web::json::value& content)
{
	auto payload = std::make_shared<odata_payload>();

	web::json::value annotation_value;
	if (content.has_field(U("@odata.context")))
	{
		annotation_value = content.at(U("@odata.context"));
		payload->set_context_url(annotation_value.is_null() ? U("") : annotation_value.as_string());
	}

	if (content.has_field(U("@odata.nextLink")))
	{
		annotation_value = content.at(U("@odata.nextLink"));
		payload->set_next_link(annotation_value.is_null() ? U("") : annotation_value.as_string());
	}

    auto context_url_parser = entity_factory<odata_contex_url_parser>::create_context_url_parser(m_model, m_service_root_url);

	auto return_type = context_url_parser->get_payload_content_type(payload->get_context_url());
	
	if (!return_type)
	{
		return payload;
	}

	web::json::value payload_content;
	::utility::string_t edit_link;
	RET_NAVI_ENTITY_TYPE entity_kind = RET_NAVI_ENTITY_TYPE::NAVI_ENTITY_NONE;

	if (return_type->get_type_kind() == edm_type_kind_t::Navigation)
	{
		// should get the type inside of navigation
		auto navigation_type = std::dynamic_pointer_cast<edm_navigation_type>(return_type);
		if (navigation_type)
		{
			return_type = navigation_type->get_navigation_type();
			if (!return_type)
			{
				return payload;
			}

			// compute part of edit link from @odata.context
			if (return_type->get_type_kind() == edm_type_kind_t::Entity)
			{
				// a single navigation property
				edit_link = get_edit_link_from_context_url(payload->get_context_url());
				entity_kind = RET_NAVI_ENTITY_TYPE::NAVI_ENTITY_SINGLE;
			}
			else
			{
				// a collection navigation property
				auto collection_return_type = std::dynamic_pointer_cast<edm_collection_type>(return_type); 
				if (collection_return_type && collection_return_type->get_element_type())
				{
					return_type = collection_return_type->get_element_type();
					edit_link = get_edit_link_from_context_url(payload->get_context_url());
					entity_kind = RET_NAVI_ENTITY_TYPE::NAVI_ENTITY_COLLECTION;
				}
			}
		}
	}
	
	payload_content = web::json::value::array();
	if (return_type->get_type_kind() == edm_type_kind_t::Collection)
	{
		if (content.has_field(U("value")))
		{
			payload_content = content.at(U("value"));		
		}
		else
		{
			payload_content[0] = content;
		}

		// get the element type of the collection
		auto collection_return_type = std::dynamic_pointer_cast<edm_collection_type>(return_type);
		if (collection_return_type && collection_return_type->get_element_type())
		{
			return_type = collection_return_type->get_element_type();
		}
	}
	else
	{
		if (return_type->get_type_kind() == edm_type_kind_t::Primitive || return_type->get_type_kind() == edm_type_kind_t::Enum)
		{
		    if (content.has_field(U("value")))
		    {
			    payload_content[0] = content.at(U("value"));
		    }
		}
		else
		{
			if (content.has_field(U("value")))
			{
				// An array of entities
				payload_content = content.at(U("value"));
			}
			else
			{
				// Single entity
				payload_content = web::json::value::array();
				payload_content[0] = content;
			}

			if (return_type->get_type_kind() == edm_type_kind_t::Entity)
			{
				// check if the context url represents a entity set or singleton
				edit_link = get_edit_link_from_context_url(payload->get_context_url());
				auto container = m_model->find_container();
				if (container)
				{
					auto entity_set = container->find_entity_set(edit_link);
					if (entity_set)
					{
						entity_kind = RET_NAVI_ENTITY_TYPE::ENTITY_ENTITY_SET;
					}
					else
					{
						auto singleton = container->find_singleton(edit_link);
						if (singleton)
						{
							entity_kind = RET_NAVI_ENTITY_TYPE::ENTITY_SINGLETON;
						}
					}
				}
			}
		}
	}

	if (!edit_link.empty())
	{
	    edit_link = m_service_root_url + U("/") + edit_link;
	}
    
	for (auto iter = payload_content.as_array().begin(); iter != payload_content.as_array().end(); ++iter)
	{
		web::json::value entity_json = *iter;
        if (return_type->get_type_kind() == edm_type_kind_t::Primitive)
		{
			payload->add_value(std::make_shared<odata_primitive_value>(return_type, strip_string(iter->serialize())));
		}
		else if (return_type->get_type_kind() == edm_type_kind_t::Complex)
		{
			auto complex_value = std::make_shared<odata_complex_value>(return_type);

			auto complex_return_type = std::dynamic_pointer_cast<edm_complex_type>(return_type);
			complex_value->set_properties(handle_extract_complex_property(*iter, complex_return_type));
			complex_value->set_value_type(complex_return_type);

			payload->add_value(complex_value);
		}
		else if (return_type->get_type_kind() == edm_type_kind_t::Entity)
		{
			auto entity_return_type = std::dynamic_pointer_cast<edm_entity_type>(return_type);
			
			auto entity_value = std::make_shared<odata_entity_value>(handle_extract_entity_property(*iter, entity_return_type), entity_return_type);
			payload->add_value(entity_value);

			// calculate edit link
			if (entity_value && !entity_value->has_property(odata_json_constants::PAYLOAD_ANNOTATION_EDITLINK))
			{
				if (entity_kind == RET_NAVI_ENTITY_TYPE::NAVI_ENTITY_SINGLE || entity_kind == RET_NAVI_ENTITY_TYPE::ENTITY_SINGLETON)
				{
					set_edit_link_for_entity_value(entity_value, return_type->get_name(), edit_link, false);
				}
				else if (entity_kind == RET_NAVI_ENTITY_TYPE::NAVI_ENTITY_COLLECTION || entity_kind == RET_NAVI_ENTITY_TYPE::ENTITY_ENTITY_SET)
				{
					set_edit_link_for_entity_value(entity_value, return_type->get_name(), edit_link, true);
				}
			}
		}
		else if (return_type->get_type_kind() == edm_type_kind_t::Enum)
		{
			payload->add_value(std::make_shared<odata_enum_value>(return_type, strip_string(iter->serialize())));
		}
	}

	return payload;
}

void odata_json_reader::handle_extract_navigation_property(web::json::value& value, std::shared_ptr<edm_navigation_type> navigation_type, odata_property_map& result, const ::utility::string_t& name)
{
	if (!navigation_type)
	{
		return ;
	}

	switch(value.type())
	{
	case json::value::Array:
		{
			auto collection = std::dynamic_pointer_cast<::odata::edm::edm_collection_type>(navigation_type->get_navigation_type());

			if (!collection)
			{
				break;
			}

			result[name] = handle_extract_collection_property(collection, value);
		}
		break;
    case json::value::Object:
		{
			auto navi_entity_type = std::dynamic_pointer_cast<::odata::edm::edm_entity_type>(navigation_type->get_navigation_type());

			result[name] = std::make_shared<odata_entity_value>(handle_extract_entity_property(value, navi_entity_type), navi_entity_type);
		}
		break;
	default:
		break;
	}
}

odata_property_map odata_json_reader::handle_extract_entity_property(web::json::value& value, std::shared_ptr<edm_entity_type>& entity_type)
{
    odata_property_map result;

	if (!entity_type)
	{
		return result;
	}

	if (value.has_field(odata_json_constants::PAYLOAD_ANNOTATION_EDITLINK))
	{
		auto annotation_type = 	value[odata_json_constants::PAYLOAD_ANNOTATION_EDITLINK];
		auto annotation_value = annotation_type.as_string();
		if (::odata::common::is_relative_path(m_service_root_url, annotation_value))
		{
			annotation_value = m_service_root_url + U("/") + annotation_value;
		}
		result[odata_json_constants::PAYLOAD_ANNOTATION_EDITLINK] = 
			std::make_shared<odata_primitive_value>(std::make_shared<edm_payload_annotation_type>(odata_json_constants::PAYLOAD_ANNOTATION_EDITLINK), annotation_value);
	}

	// find odata.type and check odata.type to see if it is a derived type
	if (value.has_field(odata_json_constants::PAYLOAD_ANNOTATION_TYPE))
	{
		auto annotation_type = 	value[odata_json_constants::PAYLOAD_ANNOTATION_TYPE];
		auto annotation_value = annotation_type.as_string();
		result[odata_json_constants::PAYLOAD_ANNOTATION_TYPE] = std::make_shared<odata_primitive_value>(std::make_shared<edm_payload_annotation_type>(odata_json_constants::PAYLOAD_ANNOTATION_TYPE), annotation_value);
		annotation_value = annotation_value.substr(1, annotation_value.length() - 1);
		auto ret_entity_type = m_model->find_entity_type(annotation_value);
		if (ret_entity_type)
		{
			entity_type = ret_entity_type;
		}
	}

	web::json::value original_value = value;
	
    for (auto iter = value.as_object().begin(); iter != value.as_object().end(); ++iter)
    {
        auto name = iter->first;
        auto& value = iter->second;

        auto index = name.find(U("odata."));
        if (index == ::utility::string_t::npos)
        {
			auto prop = entity_type->find_property(name);
			if (prop && prop->get_property_type() && prop->get_property_type()->get_type_kind() == edm_type_kind_t::Navigation)
			{
				// process navigation field
				auto navigation_type = std::dynamic_pointer_cast<edm_navigation_type>(prop->get_property_type());
				if (navigation_type)
				{
			        ::utility::string_t context_url_property = name + U("@odata.context");
					::utility::string_t edit_link;
					if (original_value.has_field(context_url_property))
					{
						edit_link = original_value[context_url_property].as_string();
						edit_link = get_edit_link_from_context_url(edit_link);
					}
					else if (!navigation_type->is_contained())
					{
						// not a contained navigation property
						auto binded_source = navigation_type->get_binded_navigation_source();
						if (binded_source)
						{
							edit_link = binded_source->get_name();
						}
					}

					handle_extract_navigation_property(value, navigation_type, result, name);

					if (edit_link.empty())
					{
						continue;
					}
					edit_link = m_service_root_url + U("/") + edit_link;

					if (!navigation_type->is_contained())
					{
						auto binded_source = navigation_type->get_binded_navigation_source();
						if (binded_source && binded_source->get_resource_type() == container_resource_type::E_RESOURCE_SINGLETON)
						{
							
						}

						if (binded_source->get_resource_type() == container_resource_type::E_RESOURCE_ENTITY_SET)
						{

						}
						else if (binded_source->get_resource_type() == container_resource_type::E_RESOURCE_SINGLETON)
						{

						}
					}
					else
					{
						
					}

                    // set edit_link
					auto navigation_value = result[name];
					if (navigation_value->get_value_type()->get_type_kind() == edm_type_kind_t::Collection)
					{
						auto collection_value = std::dynamic_pointer_cast<odata_collection_value>(navigation_value);
						if (collection_value)
						{
							for (auto collection_iter = collection_value->get_collection_values().cbegin(); collection_iter != collection_value->get_collection_values().cend(); collection_iter++)
							{
								auto element_value = std::dynamic_pointer_cast<odata_entity_value>(*collection_iter);
								if (element_value && !element_value->has_property(odata_json_constants::PAYLOAD_ANNOTATION_EDITLINK))
								{
									set_edit_link_for_entity_value(element_value, collection_value->get_value_type()->get_name(), edit_link, true);
								}
							}
						}
					}
					else if (navigation_value->get_value_type()->get_type_kind() == edm_type_kind_t::Entity)
					{
						auto element_value = std::dynamic_pointer_cast<odata_entity_value>(navigation_value);
						if (element_value && !element_value->has_property(odata_json_constants::PAYLOAD_ANNOTATION_EDITLINK))
						{
							bool is_collection = true;
							if (!navigation_type->is_contained())
							{
								auto binded_source = navigation_type->get_binded_navigation_source();
								if (binded_source && binded_source->get_resource_type() == container_resource_type::E_RESOURCE_SINGLETON)
								{
									is_collection = true;
								}
							}
							else
							{
								is_collection = false;
							}

							set_edit_link_for_entity_value(element_value, element_value->get_value_type()->get_name(), edit_link, is_collection);
						}
					}
				}
			}
			else
			{
				// Process fields that are not annotations
				switch(value.type())
				{
				case json::value::Array:
					{
						// A collection
						auto collection_prop = entity_type->find_property(name);
						if (!collection_prop)
						{
							continue;
						}

						result[name] = handle_extract_collection_property(collection_prop->get_property_type(), value);
					}
					break;
				case json::value::Object:
					{
						// A complex
						auto complex_prop = entity_type->find_property(name);
						if (!complex_prop)
						{
							// to do maybe the element is an entity
							continue;
						}

						auto p_detail = std::make_shared<odata_complex_value>(complex_prop->get_property_type());

						auto ct_type = m_model->find_complex_type(complex_prop->get_property_type()->get_name());
						if (ct_type && p_detail)
						{
							p_detail->set_properties(handle_extract_complex_property(value, ct_type));
							p_detail->set_value_type(ct_type);
						}

						result[name] = p_detail;
					}
					break;
				case json::value::Null:
					{
						auto value_prop = entity_type->find_property(name); 
						if (!value_prop)
						{
							continue;
						}

						result[name] = nullptr;
					}
					break;
				default:
					{
						auto primitive_prop = entity_type->find_property(name);
						if (!primitive_prop)
						{
							continue;
						}

						if (primitive_prop->get_property_type()->get_type_kind() == Enum)
						{
							result[name] = std::make_shared<odata_enum_value>(primitive_prop->get_property_type(), strip_string(value.serialize()));
						}
						else
						{
							result[name] = std::make_shared<odata_primitive_value>(primitive_prop->get_property_type(), strip_string(value.serialize()));
						}
					}
					break;
				}
			}
        }
        else
        {
            auto annotation = name.substr(index - 1);
            if (annotation == odata_json_constants::PAYLOAD_ANNOTATION_NAVIGATIONLINK)
            {
                auto pname = name.substr(0, index - 1);
				auto navigation_prop = entity_type->find_property(pname);
				if (!navigation_prop)
				{
					continue;
				}

				result[name] = std::make_shared<odata_primitive_value>(navigation_prop->get_property_type(), strip_string(value.serialize()));
            }
			else
			{
				if (name.find(U("@")) == 0)
				{
				    handle_extract_entity_annotation(annotation, strip_string(value.serialize()), result);
				}
			}
        }
    }

    return std::move(result);
}

odata_property_map odata_json_reader::handle_extract_complex_property(web::json::value& obj, std::shared_ptr<edm_complex_type>& complex_type)
{
    odata_property_map result;

	if (!complex_type)
	{
		return result;
	}

	// find odata.type and check odata.type to see if it is a derived type
	if (obj.has_field(odata_json_constants::PAYLOAD_ANNOTATION_TYPE))
	{
		auto annotation_type = 	obj[odata_json_constants::PAYLOAD_ANNOTATION_TYPE];
		auto annotation_value = annotation_type.as_string();
		result[odata_json_constants::PAYLOAD_ANNOTATION_TYPE] = std::make_shared<odata_primitive_value>(std::make_shared<edm_payload_annotation_type>(odata_json_constants::PAYLOAD_ANNOTATION_TYPE), annotation_value);
		annotation_value = annotation_value.substr(1, annotation_value.length() - 1);
		auto ret_complex_type = m_model->find_complex_type(annotation_value);
		if (ret_complex_type)
		{
			complex_type = ret_complex_type;
		}
	}

	for (auto iter = obj.as_object().begin(); iter != obj.as_object().end(); ++iter)
    {
        auto name = iter->first;
        auto& value = iter->second;

        auto index = name.find(U("odata."));
        if (index == ::utility::string_t::npos)
        {
            // Process fields that are not annotations
            switch(value.type())
            {
            case json::value::Array:
				{
                    // A collection
				    auto collection_prop = complex_type->find_property(name);
					if (!collection_prop)
					{
						continue;
					}

					result[name] = handle_extract_collection_property(collection_prop->get_property_type(), value);
				}
                break;
            case json::value::Object:
                {
                    // A complex
					auto complext_prop = complex_type->find_property(name);
					if (!complext_prop)
					{
						continue;
					}

                    auto p_detail = std::make_shared<odata_complex_value>(complext_prop->get_property_type());

                    auto ct_type = m_model->find_complex_type(complext_prop->get_property_type()->get_name());
                    if (ct_type)
					{
                        p_detail->set_properties(handle_extract_complex_property(value, ct_type));
						p_detail->set_value_type(ct_type);
					}

                    result[name] = p_detail;
                }
				break;
			case json::value::Null:
				{
					auto value_prop = complex_type->find_property(name);
					if (!value_prop)
					{
						// to do maybe the element is an entity
						continue;
					}

					result[name] = nullptr;
				}
				break;
            default:
                {
					auto primitive_prop = complex_type->find_property(name);
					if (!primitive_prop)
					{
						continue;
					}

						if (primitive_prop->get_property_type()->get_type_kind() == Enum)
						{
							result[name] = std::make_shared<odata_enum_value>(primitive_prop->get_property_type(), strip_string(value.serialize()));
						}
						else
						{
							result[name] = std::make_shared<odata_primitive_value>(primitive_prop->get_property_type(), strip_string(value.serialize()));
						}
                }
                break;
            }
        }
        else
        {
            auto annotation = name.substr(index - 1);
            if (annotation == odata_json_constants::PAYLOAD_ANNOTATION_NAVIGATIONLINK)
            {
                auto pname = name.substr(0, index - 1);
				auto navigation_prop = complex_type->find_property(pname);
				if (!navigation_prop)
				{
					continue;
				}

				result[name] = std::make_shared<odata_primitive_value>(navigation_prop->get_property_type(), strip_string(value.serialize()));
            }
			else
			{
				if (name.find(U("@")) == 0)
				{
					handle_extract_entity_annotation(annotation, strip_string(value.serialize()), result);
				}
			}
        }
    }

    return std::move(result);
}

std::shared_ptr<odata_collection_value> odata_json_reader::handle_extract_collection_property(std::shared_ptr<edm_named_type> type, web::json::value& value)
{
    if (!type)
	{
		return nullptr;
	}
                    
	// get elements of collection
	auto p_edm_collection_type = std::dynamic_pointer_cast<edm_collection_type>(type);
	if (!p_edm_collection_type)
	{
		return nullptr;
	}

	auto element_type = p_edm_collection_type->get_element_type();
	if (!element_type)
	{
		return nullptr;
	}
	
	auto p_collection_property = std::make_shared<odata_collection_value>(type);

	for (auto iter = value.as_array().begin(); iter != value.as_array().end(); iter++)
	{
		web::json::value element_value = *iter;

		if (element_type->get_type_kind() == edm_type_kind_t::Primitive)
		{
			p_collection_property->add_collection_value(std::make_shared<odata_primitive_value>(element_type, strip_string(element_value.serialize())));
		}
		else if (element_type->get_type_kind() == edm_type_kind_t::Complex)
		{
			auto p_complex = std::make_shared<odata_complex_value>(element_type);

			auto ct_type = m_model->find_complex_type(element_type->get_name());
			if (ct_type)
			{
                p_complex->set_properties(handle_extract_complex_property(element_value, ct_type));
				p_complex->set_value_type(ct_type);
			}

			p_collection_property->add_collection_value(p_complex);
		}
		else if (element_type->get_type_kind() == edm_type_kind_t::Entity)
		{
			auto entity_element_type = std::dynamic_pointer_cast<edm_entity_type>(element_type);
			auto p_entity = std::make_shared<odata_entity_value>(handle_extract_entity_property(element_value, entity_element_type), entity_element_type);
			
			p_collection_property->add_collection_value(p_entity);
		}
		else if (element_type->get_type_kind() == edm_type_kind_t::Enum)
		{
			p_collection_property->add_collection_value(std::make_shared<odata_enum_value>(element_type, strip_string(element_value.serialize())));
		}
		else
		{
			throw std::runtime_error("Can't parse odata_payload contains collection of collection!");
		}
	}

	return p_collection_property;
}

void odata_json_reader::handle_extract_entity_annotation(const ::utility::string_t& annotation, const ::utility::string_t& value, odata_property_map& result)
{
	if (annotation == odata_json_constants::PAYLOAD_ANNOTATION_EDITLINK)
	{
		::utility::string_t annotation_value = value;
		if (::odata::common::is_relative_path(m_service_root_url, annotation_value))
		{
			annotation_value = m_service_root_url + U("/") + annotation_value;
		}
		result[odata_json_constants::PAYLOAD_ANNOTATION_EDITLINK] = 
			std::make_shared<odata_primitive_value>(std::make_shared<edm_payload_annotation_type>(odata_json_constants::PAYLOAD_ANNOTATION_EDITLINK), annotation_value);
	}
	else if (annotation == odata_json_constants::PAYLOAD_ANNOTATION_READLINK)
	{
		result[odata_json_constants::PAYLOAD_ANNOTATION_READLINK] = 
			std::make_shared<odata_primitive_value>(std::make_shared<edm_payload_annotation_type>(odata_json_constants::PAYLOAD_ANNOTATION_READLINK), value);

	}
	else if (annotation == odata_json_constants::PAYLOAD_ANNOTATION_ID)
	{
		result[odata_json_constants::PAYLOAD_ANNOTATION_ID] = 
			std::make_shared<odata_primitive_value>(std::make_shared<edm_payload_annotation_type>(odata_json_constants::PAYLOAD_ANNOTATION_ID), value);
	}
	else if (annotation == odata_json_constants::PAYLOAD_ANNOTATION_TYPE)
	{
		result[odata_json_constants::PAYLOAD_ANNOTATION_TYPE] = 
			std::make_shared<odata_primitive_value>(std::make_shared<edm_payload_annotation_type>(odata_json_constants::PAYLOAD_ANNOTATION_TYPE), value);
	}
}

}}