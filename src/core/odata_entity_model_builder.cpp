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
 
#include "odata/core/odata_entity_model_builder.h"
#include "odata/core/odata_json_constants.h"
#include "odata/common/utility.h"

using namespace ::odata::edm;

namespace odata { namespace core
{

::utility::string_t odata_entity_model_builder::compute_edit_link(
		const ::utility::string_t& root_url, 
		std::shared_ptr<odata_entity_value> entity_value, 
		const ::utility::string_t& parent_edit_link, 
		bool is_collection_navigation)
{
	::utility::string_t edit_link;

	if (entity_value)
	{
	    std::shared_ptr<odata_value> edit_link_value;
        if (entity_value->get_property_value(odata_json_constants::PAYLOAD_ANNOTATION_EDITLINK, edit_link_value))
	    { 
			auto primitive_value = std::dynamic_pointer_cast<odata_primitive_value>(edit_link_value);

			if (!primitive_value)
			{
				return edit_link;
			}

			edit_link = primitive_value->to_string();

			if (::odata::common::is_relative_path(root_url, edit_link))
			{
				edit_link = root_url + U("/") + edit_link;
			}
		}
		else
		{
			if (is_collection_navigation)
			{
				::utility::string_t key_string = get_entity_key_value_string(entity_value);
				edit_link = parent_edit_link + U("(") + key_string + U(")");
			}
			else
			{
				edit_link = parent_edit_link;
			}
		}
	}

	return edit_link;
}

::utility::string_t odata_entity_model_builder::get_entity_key_value_string(const std::shared_ptr<odata_entity_value>& entity_value)
{
	::utility::string_t key_string;

	if (!entity_value)
	{
		return key_string;
	}

	std::vector<::utility::string_t> keys;

	auto parent_type = std::dynamic_pointer_cast<edm_entity_type>(entity_value->get_value_type());
	while (parent_type)
	{
		keys.insert(keys.end(), parent_type->key().cbegin(), parent_type->key().cend());
		parent_type = std::dynamic_pointer_cast<edm_entity_type>(parent_type->get_base_type());
	}

	for (size_t i = 0; i < keys.size(); i++)
	{
		std::shared_ptr<odata_value> property_value;
		entity_value->get_property_value(keys[i], property_value);
		if (property_value)
		{
			std::shared_ptr<edm_named_type> property_type = property_value->get_value_type();
			if (property_type && property_type->get_type_kind() == edm_type_kind_t::Primitive)
			{
				auto primitive_property_value = std::dynamic_pointer_cast<odata_primitive_value>(property_value);
				if (primitive_property_value)
				{
					if (i != 0)
					{
						key_string += U(",");
					}

					key_string += primitive_property_value->to_string();
				}
			}
		}
	}

	return key_string;
}

}}