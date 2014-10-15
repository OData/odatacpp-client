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

#include "e2e_tests.h"

namespace tests { namespace e2e { namespace odata {

using namespace utility;
using namespace ::odata::core;
using namespace ::odata::edm;

SUITE(function_action_tests_raw_client)
{

TEST_FIXTURE(e2e_raw_client, invoke_unbound_function)
{
	auto function_ret = client.get_data_from_server(U("Company/Microsoft.Test.OData.Services.ODataWCFService.GetEmployeesCount()")).get();
	VERIFY_ARE_EQUAL(function_ret.size(), 1);
	VERIFY_ARE_EQUAL(edm_type_kind_t::Primitive, function_ret[0]->get_value_type()->get_type_kind());
	auto primitive_value = std::dynamic_pointer_cast<odata_primitive_value>(function_ret[0]);
	int32_t count = primitive_value->as<int32_t>();
	VERIFY_ARE_EQUAL(count, 2);
}

TEST_FIXTURE(e2e_raw_client, simple_function)
{
	::utility::string_t employees_count_path = U("Company/Microsoft.Test.OData.Services.ODataWCFService.GetEmployeesCount");
	std::vector<std::shared_ptr<odata_parameter>> parameters;
	std::vector<std::shared_ptr<odata_value>> returned_values;
	client.send_data_to_server(employees_count_path, parameters, returned_values, HTTP_GET).get();

	VERIFY_ARE_EQUAL(1, returned_values.size());
	VERIFY_ARE_EQUAL(2, std::dynamic_pointer_cast<odata_primitive_value>(returned_values[0])->as<int32_t>());
}

TEST_FIXTURE(e2e_raw_client, function_return_collection_of_complex_value)
{
	::utility::string_t product_details_path = U("Products(5)/Microsoft.Test.OData.Services.ODataWCFService.GetProductDetails");
	std::vector<std::shared_ptr<odata_parameter>> parameters;
	parameters.push_back(std::make_shared<odata_parameter>(U("count"), odata_primitive_value::make_primitive_value(3)));
	std::vector<std::shared_ptr<odata_value>> returned_values;
	client.send_data_to_server(product_details_path, parameters, returned_values, HTTP_GET).get();

	VERIFY_ARE_EQUAL(3, returned_values.size());
	auto detail1 = std::dynamic_pointer_cast<odata_entity_value>(returned_values[0]);
	int32_t product_id;
	detail1->try_get(U("ProductID"), product_id);
	VERIFY_ARE_EQUAL(5, product_id);
}

TEST_FIXTURE(e2e_raw_client, action_of_primitive_value)
{
	::utility::string_t company_revenue_path = U("Company/Revenue");
	auto revenue_odata_value = client.get_data_from_server(company_revenue_path).get()[0];
	auto revenue_primitive_value = std::dynamic_pointer_cast<odata_primitive_value>(revenue_odata_value);
	int32_t revenue = revenue_primitive_value->as<int32_t>();
	VERIFY_ARE_EQUAL(100000, revenue);

	std::vector<std::shared_ptr<odata_parameter>> parameters;
	parameters.push_back(std::make_shared<odata_parameter>(U("IncreaseValue"), odata_primitive_value::make_primitive_value(100000)));
	std::vector<std::shared_ptr<odata_value>> returned_values;
	client.send_data_to_server(U("Company/Microsoft.Test.OData.Services.ODataWCFService.IncreaseRevenue"), parameters, returned_values).get();
	
	VERIFY_ARE_EQUAL(1, returned_values.size());
	std::shared_ptr<odata_value> returned_value = returned_values[0];
	revenue_odata_value = client.get_data_from_server(company_revenue_path).get()[0];
	revenue_primitive_value = std::dynamic_pointer_cast<odata_primitive_value>(revenue_odata_value);
	revenue = revenue_primitive_value->as<int32_t>();
	VERIFY_ARE_EQUAL(200000, revenue);

	auto returned_primitive_value = std::dynamic_pointer_cast<odata_primitive_value>(returned_value);
	VERIFY_ARE_EQUAL(200000, returned_primitive_value->as<int32_t>());
}

TEST_FIXTURE(e2e_raw_client, action_of_enum_value)
{
	::utility::string_t product_user_access = U("Products(5)/UserAccess");
	auto user_access_value = std::dynamic_pointer_cast<odata_enum_value>(client.get_data_from_server(product_user_access).get()[0]);
	VERIFY_ARE_EQUAL(U("None"), user_access_value->to_string());

	auto model = client.get_model().get();
	auto new_access = std::make_shared<odata_enum_value>(model->find_enum_type(U("AccessLevel")), U("ReadWrite"));

	std::vector<std::shared_ptr<odata_parameter>> parameters;
	parameters.push_back(std::make_shared<odata_parameter>(U("accessRight"), new_access));
	std::vector<std::shared_ptr<odata_value>> returned_values;
	client.send_data_to_server(U("Products(5)/Microsoft.Test.OData.Services.ODataWCFService.AddAccessRight"), parameters, returned_values).get();

	VERIFY_ARE_EQUAL(1, returned_values.size());
	std::shared_ptr<odata_value> returned_value = returned_values[0];

	user_access_value = std::dynamic_pointer_cast<odata_enum_value>(client.get_data_from_server(product_user_access).get()[0]);
	VERIFY_ARE_EQUAL(U("ReadWrite"), user_access_value->to_string());

	auto returned_enum_value = std::dynamic_pointer_cast<odata_enum_value>(returned_value);
	VERIFY_ARE_EQUAL(U("ReadWrite"), returned_enum_value->to_string());
}

TEST_FIXTURE(e2e_raw_client, action_of_complex_value)
{
	::utility::string_t home_address = U("People(1)/HomeAddress");
	auto home_address_value = std::dynamic_pointer_cast<odata_complex_value>(client.get_data_from_server(home_address).get()[0]);
	std::shared_ptr<odata_value> property_value;
	home_address_value->get_property_value(U("City"), property_value);
	VERIFY_ARE_EQUAL(U("Tokyo"), std::dynamic_pointer_cast<odata_primitive_value>(property_value)->as<::utility::string_t>());

	auto model = client.get_model().get();
	auto address_type = model->find_complex_type(U("Address"));
	auto new_address1 = std::make_shared<odata_complex_value>(address_type);
	new_address1->set_value(U("City"), U("Shanghai"));
	new_address1->set_value(U("PostalCode"), U("200000"));
	new_address1->set_value(U("Street"), U("Zixing Road"));

	auto addresses = std::make_shared<odata_collection_value>(std::make_shared<edm_collection_type>(U("AddressCollection"), address_type));
	addresses->add_collection_value(new_address1);

	std::vector<std::shared_ptr<odata_parameter>> parameters;
	parameters.push_back(std::make_shared<odata_parameter>(U("addresses"), addresses));
	parameters.push_back(std::make_shared<odata_parameter>(U("index"), odata_primitive_value::make_primitive_value(0)));
	std::vector<std::shared_ptr<odata_value>> returned_values;
	client.send_data_to_server(U("People(1)/Microsoft.Test.OData.Services.ODataWCFService.ResetAddress"), parameters, returned_values).get();

	VERIFY_ARE_EQUAL(1, returned_values.size());
	std::shared_ptr<odata_value> returned_value = returned_values[0];

	home_address_value = std::dynamic_pointer_cast<odata_complex_value>(client.get_data_from_server(home_address).get()[0]);
	home_address_value->get_property_value(U("City"), property_value);
	VERIFY_ARE_EQUAL(U("Shanghai"), std::dynamic_pointer_cast<odata_primitive_value>(property_value)->as<::utility::string_t>());

	auto returned_entity_value = std::dynamic_pointer_cast<odata_entity_value>(returned_value);
	returned_entity_value->get_property_value(U("HomeAddress"), property_value);
	std::dynamic_pointer_cast<odata_complex_value>(property_value)->get_property_value(U("City"), property_value);
	VERIFY_ARE_EQUAL(U("Shanghai"), std::dynamic_pointer_cast<odata_primitive_value>(property_value)->as<::utility::string_t>());
}

}

SUITE(codegen_function_action_test)
{

TEST_FIXTURE(e2e_test_case, function_no_param_return_int)
{
	auto company_obj = service_context->create_company_query()->execute_query().get();
	auto ret = company_obj->GetEmployeesCount().get();

	VERIFY_ARE_EQUAL(ret.size(), 1);
	VERIFY_ARE_EQUAL(ret[0], 2);
}

TEST_FIXTURE(e2e_test_case, function_one_param_return_collection_entity)
{
	auto products = service_context->create_products_query()->execute_query().get();
	auto ret = products[0]->GetProductDetails(10).get();

	VERIFY_ARE_EQUAL(ret.size(), 5);

	VERIFY_ARE_EQUAL(ret[0]->get_edit_link(), U("http://odatae2etest.azurewebsites.net/cpptest/DefaultService/ProductDetails(ProductID=5,ProductDetailID=2)"));
	VERIFY_ARE_EQUAL(ret[0]->get_productid(), 5);
	VERIFY_ARE_EQUAL(ret[0]->get_productdetailid(), 2);
	VERIFY_ARE_EQUAL(ret[0]->get_productname(), U("CheeseCake"));
	VERIFY_ARE_EQUAL(ret[0]->get_description(), U("Cheese-flavored snack"));

	VERIFY_ARE_EQUAL(ret[4]->get_edit_link(), U("http://odatae2etest.azurewebsites.net/cpptest/DefaultService/ProductDetails(ProductID=5,ProductDetailID=6)"));
	VERIFY_ARE_EQUAL(ret[4]->get_productid(), 5);
	VERIFY_ARE_EQUAL(ret[4]->get_productdetailid(), 6);
	VERIFY_ARE_EQUAL(ret[4]->get_productname(), U("Gatorade"));
	VERIFY_ARE_EQUAL(ret[4]->get_description(), U("fitness drink!"));

	auto products_ret = ret[0]->GetRelatedProduct().get();

	VERIFY_ARE_EQUAL(products_ret.size(), 1);

	VERIFY_ARE_EQUAL(products_ret[0]->get_edit_link(), U("http://odatae2etest.azurewebsites.net/cpptest/DefaultService/Products(5)"));
	VERIFY_ARE_EQUAL(products_ret[0]->get_productid(), 5);
	VERIFY_ARE_EQUAL(products_ret[0]->get_name(), U("Cheetos"));
	VERIFY_ARE_EQUAL(products_ret[0]->get_quantityperunit(), U("100g Bag"));
	VERIFY_ARE_EQUAL(products_ret[0]->get_covercolors().size(), 3);
	VERIFY_ARE_EQUAL(products_ret[0]->get_covercolors()[0], Color::Green);
}

TEST_FIXTURE(e2e_test_case, function_no_param_return_complex)
{
	auto people = service_context->create_people_query()->key(U("1"))->execute_query().get();

	VERIFY_ARE_EQUAL(people.size(), 1);

	auto home_address_obj = people[0]->GetHomeAddress().get();

	VERIFY_ARE_EQUAL(home_address_obj.size(), 1);

	VERIFY_ARE_EQUAL(*(home_address_obj[0]->get_familyname()), U("Cats"));
	VERIFY_ARE_EQUAL(home_address_obj[0]->get_city(), U("Tokyo"));
	VERIFY_ARE_EQUAL(home_address_obj[0]->get_postalcode(), U("98052"));
	VERIFY_ARE_EQUAL(home_address_obj[0]->get_street(), U("1 Microsoft Way"));
}

TEST_FIXTURE(e2e_test_case, function_import_no_param_return_collection_entity)
{
	auto products = service_context->GetAllProducts().get();

	VERIFY_ARE_EQUAL(products.size(), 5);

	VERIFY_ARE_EQUAL(products[0]->get_edit_link(), U("http://odatae2etest.azurewebsites.net/cpptest/DefaultService/Products(5)"));
	VERIFY_ARE_EQUAL(products[0]->get_productid(), 5);
	VERIFY_ARE_EQUAL(products[0]->get_name(), U("Cheetos"));
	VERIFY_ARE_EQUAL(products[0]->get_quantityperunit(), U("100g Bag"));
	VERIFY_ARE_EQUAL(products[0]->get_covercolors().size(), 3);
	VERIFY_ARE_EQUAL(products[0]->get_covercolors()[0], Color::Green);
}

TEST_FIXTURE(e2e_test_case, action_function_import_two_param_return_collection_primitive)
{
	std::vector<::utility::string_t> emails;
	emails.push_back(U("email_1"));
	emails.push_back(U("email_2"));
	emails.push_back(U("email_3"));
	auto action_ret = service_context->ResetBossEmail(emails).get();

	VERIFY_ARE_EQUAL(action_ret.size(), 3);
	VERIFY_ARE_EQUAL(action_ret[0], U("email_1"));
	VERIFY_ARE_EQUAL(action_ret[1], U("email_2"));
	VERIFY_ARE_EQUAL(action_ret[2], U("email_3"));

	auto function_ret = service_context->GetBossEmails(0, 5).get();

	VERIFY_ARE_EQUAL(function_ret.size(), 3);
	VERIFY_ARE_EQUAL(function_ret[0], U("email_1"));
	VERIFY_ARE_EQUAL(function_ret[1], U("email_2"));
	VERIFY_ARE_EQUAL(function_ret[2], U("email_3"));
}

TEST_FIXTURE(e2e_test_case, function_import_one_param_return_entity)
{
	auto ret = service_context->GetPerson2(U("Tokyo")).get();

	VERIFY_ARE_EQUAL(ret.size(), 1);

	VERIFY_ARE_EQUAL(ret[0]->get_edit_link(), U("http://odatae2etest.azurewebsites.net/cpptest/DefaultService/People(1)/Microsoft.Test.OData.Services.ODataWCFService.Customer"));
	VERIFY_ARE_EQUAL(ret[0]->get_firstname(), U("Bob"));
	VERIFY_ARE_EQUAL(ret[0]->get_lastname(), U("Cat"));
}

TEST_FIXTURE(e2e_test_case, action_one_param_return_enum)
{
	auto products = service_context->create_products_query()->execute_query().get();

	VERIFY_ARE_EQUAL(*(products[0]->get_useraccess()), AccessLevel::None);

	auto ret = products[0]->AddAccessRight(AccessLevel::ReadWrite).get();

	VERIFY_ARE_EQUAL(ret.size(), 1);

	VERIFY_ARE_EQUAL(ret[0], AccessLevel::ReadWrite);

    products = service_context->create_products_query()->execute_query().get();

	VERIFY_ARE_EQUAL(*(products[0]->get_useraccess()), AccessLevel::ReadWrite);
}

TEST_FIXTURE(e2e_test_case, action_one_param_return_primitive)
{
	auto company_obj = service_context->create_company_query()->execute_query().get();

	VERIFY_ARE_EQUAL(company_obj->get_revenue(), 100000);

	auto ret = company_obj->IncreaseRevenue(212304540).get();

	VERIFY_ARE_EQUAL(ret.size(), 1);

	VERIFY_ARE_EQUAL(ret[0], 212404540);

	company_obj = service_context->create_company_query()->execute_query().get();

	VERIFY_ARE_EQUAL(company_obj->get_revenue(), 212404540);
}

TEST_FIXTURE(e2e_test_case, action_import_return_void)
{
	auto ret = service_context->Discount(32).get();

	VERIFY_ARE_EQUAL(ret, 0);
}

TEST_FIXTURE(e2e_test_case, action_import_one_param_return_complex)
{
	Address address(service_context);
	address.set_city(U("Shang Hai"));
	address.set_postalcode(U("123123"));
	address.set_street(U("ZiXing Road"));

	auto ret = service_context->ResetBossAddress(address).get();

	VERIFY_ARE_EQUAL(ret.size(), 1);

	VERIFY_ARE_EQUAL(ret[0]->get_city(), U("Shang Hai"));
	VERIFY_ARE_EQUAL(ret[0]->get_postalcode(), U("123123"));
	VERIFY_ARE_EQUAL(ret[0]->get_street(), U("ZiXing Road"));
}

TEST_FIXTURE(e2e_test_case, action_one_param_return_entity)
{
	std::vector<Address> addresses;

	Address address1(service_context);
	address1.set_city(U("Shang Hai"));
	address1.set_postalcode(U("123123"));
	address1.set_street(U("ZiXing Road"));
	addresses.push_back(address1);

	Address address2(service_context);
	address2.set_city(U("Bei Jing"));
	address2.set_postalcode(U("006767"));
	address2.set_street(U("TiananMen Road"));
	addresses.push_back(address2);

	Address address3(service_context);
	address3.set_city(U("La Sa"));
	address3.set_postalcode(U("221122"));
	address3.set_street(U("LaSa Road"));
	addresses.push_back(address3);

	auto people = service_context->create_people_query()->key(U("1"))->execute_query().get();

	auto ret = people[0]->ResetAddress(addresses, 2).get();

	people = service_context->create_people_query()->key(U("1"))->execute_query().get();

	VERIFY_ARE_EQUAL(people.size(), 1);

	VERIFY_ARE_EQUAL(people[0]->get_homeaddress()->get_city(), U("La Sa"));
	VERIFY_ARE_EQUAL(people[0]->get_homeaddress()->get_postalcode(), U("221122"));
	VERIFY_ARE_EQUAL(people[0]->get_homeaddress()->get_street(), U("LaSa Road"));
}

TEST_FIXTURE(e2e_test_case, action_one_param__datetime_return_entity)
{
	auto accounts = service_context->create_accounts_query()->key(U("101"))->execute_query().get();

	::utility::datetime dt = ::utility::datetime::from_string(U("2014-05-19T14:05:00Z"), ::utility::datetime::date_format::ISO_8601); 
	auto ret = accounts[0]->RefreshDefaultPI(dt).get();

	VERIFY_ARE_EQUAL(ret[0]->get_createddate().to_string(::utility::datetime::date_format::ISO_8601), U("2014-05-19T14:05:00Z"));
}

}

}}}