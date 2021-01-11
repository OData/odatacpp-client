//---------------------------------------------------------------------
// <copyright file="odata_entity_model_builder.cpp" company="Microsoft">
//      Copyright (C) Microsoft Corporation. All rights reserved. See License.txt in the project root for license information.
// </copyright>
//---------------------------------------------------------------------

#include "odata/core/odata_entity_model_builder.h"
#include "odata/core/odata_json_constants.h"
#include "odata/common/utility.h"

using namespace ::odata::edm;

namespace odata { namespace core
{

::odata::string_t odata_entity_model_builder::compute_edit_link(const ::odata::string_t& root_url, std::shared_ptr<odata_entity_value> entity_value, const ::odata::string_t& parent_edit_link, bool is_collection_navigation)
{
	::odata::string_t edit_link;

	if (entity_value)
	{
		std::shared_ptr<odata_value> edit_link_value;
		if (entity_value->get_property_value(odata_json_constants::PAYLOAD_ANNOTATION_EDITLINK, edit_link_value))
		{
			auto primitive_value = std::dynamic_pointer_cast<odata_primitive_value>(edit_link_value);

			if (!primitive_value)
			{
				return std::move(edit_link);
			}

			edit_link = primitive_value->to_string();

			if (::odata::common::is_relative_path(root_url, edit_link))
			{
				edit_link = root_url + _XPLATSTR("/") + edit_link;
			}
		}
		else
		{
			if (is_collection_navigation)
			{
				::odata::string_t key_string = get_entity_key_value_string(entity_value);
				edit_link = parent_edit_link + _XPLATSTR("(") + key_string + _XPLATSTR(")");
			}
			else
			{
				edit_link = parent_edit_link;
			}
		}
	}

	return std::move(edit_link);
}

::odata::string_t odata_entity_model_builder::get_entity_key_value_string(const std::shared_ptr<odata_entity_value>& entity_value)
{
	::odata::string_t key_string;

	if (!entity_value)
	{
		return std::move(key_string);
	}

	std::vector<::odata::string_t> keys;

	auto parent_type = std::dynamic_pointer_cast<edm_entity_type>(entity_value->get_value_type());
	while (parent_type)
	{
		keys.reserve(keys.size() + parent_type->key().size());
		keys.insert(keys.end(), parent_type->key().cbegin(), parent_type->key().cend());
		parent_type = std::dynamic_pointer_cast<edm_entity_type>(parent_type->get_base_type());
	}

	for (size_t i = 0; i < keys.size(); ++i)
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
						key_string += _XPLATSTR(",");
					}

					key_string += primitive_property_value->to_string();
				}
			}
		}
	}

	return std::move(key_string);
}

}}
