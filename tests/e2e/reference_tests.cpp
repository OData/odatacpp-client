/***
* ==++==
*
* Copyright (c) Microsoft Corporation. All rights reserved. 
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
* http://www.apache.org/licenses/LICENSE-2.0
* 
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*
* ==--==
* =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
*
* reference_tests.cpp
*
*
*
* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
****/

#include "e2e_tests.h"
#include "odata_wcf_service.h"

namespace tests { namespace e2e { namespace odata {

using namespace odata_wcf_service;

SUITE(reference_tests_raw_client)
{

TEST_FIXTURE(e2e_raw_client, add_reference)
{
	::utility::string_t orders_of_customer1 = U("Customers(1)/Orders");
	auto orders = client.get_data_from_server(orders_of_customer1).get();
	VERIFY_ARE_EQUAL(1, orders.size());

	auto orderEntity = std::dynamic_pointer_cast<odata_entity_value>(client.get_data_from_server(U("Orders(7)")).get()[0]);
	
	string_t orderId;
	orderEntity->try_get(PAYLOAD_ANNOTATION_ID, orderId);
	client.add_reference(orders_of_customer1, orderId).get();

	orders = client.get_data_from_server(orders_of_customer1).get();
	VERIFY_ARE_EQUAL(2, orders.size());
}

TEST_FIXTURE(e2e_raw_client, delete_reference_in_collection_valued_navigation_property)
{
	::utility::string_t orders_of_customer1 = U("Customers(1)/Orders");
	auto orders = client.get_data_from_server(orders_of_customer1).get();
	VERIFY_ARE_EQUAL(1, orders.size());

	auto orderEntity = std::dynamic_pointer_cast<odata_entity_value>(orders[0]);
	string_t orderId;
	orderEntity->try_get(PAYLOAD_ANNOTATION_ID, orderId);
	client.remove_reference(orders_of_customer1, orderId).get();

	orders = client.get_data_from_server(orders_of_customer1).get();
	VERIFY_ARE_EQUAL(0, orders.size());
}

// The test service seems do not support this, defect 2260922
//TEST_FIXTURE(e2e_raw_client, delete_reference_in_single_valued_navigation_property)
//{
//	::utility::string_t product_of_product_detail = U("ProductDetails(ProductID=6,ProductDetailID=1)/RelatedProduct");
//	auto product = client.get_data_from_server(product_of_product_detail).get();
//	VERIFY_ARE_EQUAL(1, product.size());
//
//	client.remove_reference(product_of_product_detail, U("")).get();
//
//	product = client.get_data_from_server(product_of_product_detail).get();
//	VERIFY_ARE_EQUAL(0, product.size());
//}


TEST_FIXTURE(e2e_raw_client, update_reference)
{
	::utility::string_t parent = U("People(1)/Parent");
	::utility::string_t person_id_property = U("PersonID");

	auto parentEntity = std::dynamic_pointer_cast<odata_entity_value>(client.get_data_from_server(parent).get()[0]);
	int32_t person_id;
	parentEntity->try_get(person_id_property, person_id);
	VERIFY_ARE_EQUAL(2, person_id);

	auto newParentEntity = std::dynamic_pointer_cast<odata_entity_value>(client.get_data_from_server(U("People(3)")).get()[0]);
	::utility::string_t newParentId;
	newParentEntity->try_get(PAYLOAD_ANNOTATION_ID, newParentId);

	client.update_reference(parent, newParentId).get();

	parentEntity = std::dynamic_pointer_cast<odata_entity_value>(client.get_data_from_server(parent).get()[0]);
	parentEntity->try_get(person_id_property, person_id);
	VERIFY_ARE_EQUAL(3, person_id);
}

}

SUITE(reference_tests)
{

TEST_FIXTURE(e2e_test_case, add_reference)  
{	
	auto customer = service_context->create_customers_query()->key(U("1"))->expand(U("Orders"))->execute_query().get()[0];
	VERIFY_ARE_EQUAL(1, customer->get_orders().size());
	
	auto order = service_context->create_orders_query()->key(U("7"))->execute_query().get()[0];
	service_context->add_reference(customer, U("Orders"), order).get();
	
	customer->load_orders().get();
	VERIFY_ARE_EQUAL(2, customer->get_orders().size());
}

TEST_FIXTURE(e2e_test_case, update_reference)
{
	auto people = service_context->create_people_query()->key(U("1"))->expand(U("Parent"))->execute_query().get()[0];
	VERIFY_ARE_EQUAL(2, people->get_parent()->get_personid());
	
	auto parent = service_context->create_people_query()->key(U("3"))->execute_query().get()[0];
	service_context->update_reference(people, U("Parent"), parent);

	people->load_parent().get();
	VERIFY_ARE_EQUAL(3, people->get_parent()->get_personid());
}

}

}}}