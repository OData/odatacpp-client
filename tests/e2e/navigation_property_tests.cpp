//---------------------------------------------------------------------
// <copyright file="navigation_property_tests.cpp" company="Microsoft">
//      Copyright (C) Microsoft Corporation. All rights reserved. See License.txt in the project root for license information.
// </copyright>
//---------------------------------------------------------------------

#include "e2e_tests.h"
#include "odata_wcf_service.h"

namespace tests { namespace e2e { namespace odata {

using namespace odata_wcf_service;


SUITE(navigation_property_tests_raw_client)
{
	
TEST_FIXTURE(e2e_raw_client, query_navigaton_collection)
{
	auto entities = client.get_data_from_server(U("Products(5)/Details")).get();
	VERIFY_ARE_EQUAL(entities.size(), 5);

	std::shared_ptr<odata_entity_value> first_entity = std::dynamic_pointer_cast<odata_entity_value>(entities[1]);
	std::shared_ptr<odata_value> property_value;
	VERIFY_IS_TRUE(first_entity->get_property_value(U("@odata.editLink"), property_value));
	VERIFY_ARE_EQUAL(edm_type_kind_t::PayloadAnnotation, property_value->get_value_type()->get_type_kind());
	auto primitive_value = std::dynamic_pointer_cast<odata_primitive_value>(property_value);
	::utility::string_t edit_link = primitive_value->as<::utility::string_t>();
	VERIFY_ARE_EQUAL(U("http://odatae2etest.azurewebsites.net/cpptest/DefaultService/ProductDetails(ProductID=5,ProductDetailID=3)"), edit_link);
}

//other cases: add_link, delete_link, set_link, add_related_object, update_related_object

}


SUITE(navigation_property_tests)
{

TEST_FIXTURE(e2e_test_case, query_navigaton_collection)
{
	auto products = service_context->create_products_query()->key(U("5"))->execute_query().get();
	VERIFY_ARE_EQUAL(1, products.size());
	auto product = products[0];

	product->load_details().get();
	VERIFY_ARE_EQUAL(5, product->get_details().size());
	auto detail = product->get_details()[0];
	VERIFY_ARE_EQUAL(U("http://odatae2etest.azurewebsites.net/cpptest/DefaultService/ProductDetails(ProductID=5,ProductDetailID=2)"), detail->get_edit_link());
	VERIFY_ARE_EQUAL(U("CheeseCake"), detail->get_productname());
}

TEST_FIXTURE(e2e_test_case, query_navigaton_single)
{
	auto people = service_context->create_people_query()->key(U("1"))->execute_query().get();
	VERIFY_ARE_EQUAL(1, people.size());
	auto person = people[0];
	
	person->load_parent().get();
	auto parent = person->get_parent();
	VERIFY_ARE_EQUAL(U("http://odatae2etest.azurewebsites.net/cpptest/DefaultService/People(2)/Microsoft.Test.OData.Services.ODataWCFService.Customer"), parent->get_edit_link());
	VERIFY_ARE_EQUAL(U("Jill"), parent->get_firstname());
}

//Add, Update, Delete on navigation properties

}

}}}