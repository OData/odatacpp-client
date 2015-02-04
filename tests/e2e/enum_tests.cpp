//---------------------------------------------------------------------
// <copyright file="enum_tests.cpp" company="Microsoft">
//      Copyright (C) Microsoft Corporation. All rights reserved. See License.txt in the project root for license information.
// </copyright>
//---------------------------------------------------------------------

#include "e2e_tests.h"
#include "odata_wcf_service.h"
#include "odata/codegen/odata_entityset_query_executor.h"
#include "odata/codegen/odata_singleton_query_executor.h"
#include "odata/codegen/odata_primitive_query_executor.h"
#include "odata/codegen/odata_complex_query_executor.h"
#include "odata/codegen/odata_void_query_executor.h"
#include "odata/codegen/odata_enum_query_executor.h"

namespace tests { namespace e2e { namespace odata {

using namespace odata_wcf_service;

SUITE(enum_tests_raw_client)
{
TEST_FIXTURE(e2e_raw_client, query_enum_in_entity)
{
	auto entities = client.get_entities(U("Products")).get();
	auto firstEntity = entities[0];
	std::shared_ptr<odata_value> skinColor;
	firstEntity->get_property_value(U("SkinColor"), skinColor);
	VERIFY_ARE_EQUAL(skinColor->get_value_type()->get_type_kind(), Enum);

	auto skinColorEnum = std::dynamic_pointer_cast<odata_enum_value>(skinColor);
	VERIFY_ARE_EQUAL(U("Red"), skinColorEnum->to_string());
}

TEST_FIXTURE(e2e_raw_client, query_enum_property)
{
	auto data = client.get_data_from_server(U("Products(5)/SkinColor")).get();
	VERIFY_ARE_EQUAL(data.size(), 1);

	auto skinColor = std::dynamic_pointer_cast<odata_enum_value>(data[0]);
	VERIFY_ARE_EQUAL(U("Red"), skinColor->to_string());
}

TEST_FIXTURE(e2e_raw_client, update_enum_property)
{
	auto model = client.get_model().get();

	//check the old value
	auto old_data = client.get_data_from_server(U("Products(5)")).get();
	auto old_entity = std::dynamic_pointer_cast<odata_entity_value>(old_data[0]);
	std::shared_ptr<odata_value> old_value;
	old_entity->get_property_value(U("UserAccess"), old_value);
	auto old_enum = std::dynamic_pointer_cast<odata_enum_value>(old_value);
	VERIFY_ARE_EQUAL(U("None"), old_enum->to_string());

	//update the entity with patch
	auto access_level_type = model->find_enum_type(U("AccessLevel"));
	auto new_access = std::make_shared<odata_enum_value>(access_level_type, U("ReadWrite"));
	old_entity->set_value(U("UserAccess"), std::dynamic_pointer_cast<odata_value>(new_access));

	auto response_code = client.patch_entity(U("Products"), old_entity).get();
	VERIFY_ARE_EQUAL(204, response_code);

	//check the new value
	auto new_data = client.get_data_from_server(U("Products(5)")).get();
	auto new_entity = std::dynamic_pointer_cast<odata_entity_value>(new_data[0]);
	std::shared_ptr<odata_value> new_value;
	new_entity->get_property_value(U("UserAccess"), new_value);
	auto new_enum = std::dynamic_pointer_cast<odata_enum_value>(new_value);
	VERIFY_ARE_EQUAL(U("ReadWrite"), new_enum->to_string());
}

}

SUITE(enum_tests)
{

TEST_FIXTURE(e2e_test_case, query_with_enum)
{	
	auto products = service_context->create_products_query()->key(U("5"))->execute_query().get();
	VERIFY_ARE_EQUAL(1, products.size());

	auto product = products[0];
	VERIFY_ARE_EQUAL(Red, *(product->get_skincolor())); //Nullable property is a shared_ptr
	VERIFY_ARE_EQUAL(None, *(product->get_useraccess()));  //Non-nulllable property is the actual enum value
	VERIFY_ARE_EQUAL(3, product->get_covercolors().size());
	VERIFY_ARE_EQUAL(Blue, product->get_covercolors()[2]);

	auto skin_color = service_context->create_query<odata_enum_query_executor<Color, enum_type_resolver>, odata_query_builder>(U("Products(5)/SkinColor"))->execute_query().get();
	VERIFY_ARE_EQUAL(1, skin_color.size());
	VERIFY_ARE_EQUAL(Red, skin_color[0]);
	auto user_access = service_context->create_query<odata_enum_query_executor<AccessLevel, enum_type_resolver>, odata_query_builder>(U("Products(5)/UserAccess"))->execute_query().get();
	VERIFY_ARE_EQUAL(1, user_access.size());
	VERIFY_ARE_EQUAL(None, user_access[0]);
	auto cover_colors = service_context->create_query<odata_enum_query_executor<Color, enum_type_resolver>, odata_query_builder>(U("Products(5)/CoverColors"))->execute_query().get();
	VERIFY_ARE_EQUAL(3, cover_colors.size());
	VERIFY_ARE_EQUAL(Blue, cover_colors[2]);
}

TEST_FIXTURE(e2e_test_case, update_with_enum)
{	
	auto products = service_context->create_products_query()->key(U("5"))->execute_query().get();
	VERIFY_ARE_EQUAL(1, products.size());

	auto product = products[0];
	product->set_skincolor(Green);
	product->set_useraccess(ReadWrite);
	std::vector<Color> new_cover_colors;
	new_cover_colors.push_back(Green);
	new_cover_colors.push_back(Blue);
	product->set_covercolors(new_cover_colors);
	
	auto status_code = service_context->update_object(product).get();
	VERIFY_ARE_EQUAL(204, status_code);

	auto skin_color = service_context->create_query<odata_enum_query_executor<Color, enum_type_resolver>, odata_query_builder>(U("Products(5)/SkinColor"))->execute_query().get();
	VERIFY_ARE_EQUAL(1, skin_color.size());
	VERIFY_ARE_EQUAL(Green, skin_color[0]);
	auto user_access = service_context->create_query<odata_enum_query_executor<AccessLevel, enum_type_resolver>, odata_query_builder>(U("Products(5)/UserAccess"))->execute_query().get();
	VERIFY_ARE_EQUAL(1, user_access.size());
	VERIFY_ARE_EQUAL(ReadWrite, user_access[0]);
	auto cover_colors = service_context->create_query<odata_enum_query_executor<Color, enum_type_resolver>, odata_query_builder>(U("Products(5)/CoverColors"))->execute_query().get();
	VERIFY_ARE_EQUAL(2, cover_colors.size());
	VERIFY_ARE_EQUAL(Blue, cover_colors[1]);
}

}

}}}