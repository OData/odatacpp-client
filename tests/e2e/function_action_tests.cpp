//---------------------------------------------------------------------
// <copyright file="function_action_tests.cpp" company="Microsoft">
//      Copyright (C) Microsoft Corporation. All rights reserved. See License.txt in the project root for license information.
// </copyright>
//---------------------------------------------------------------------

#include "e2e_tests.h"

namespace tests { namespace e2e { namespace odata {

using namespace utility;
using namespace ::odata::core;
using namespace ::odata::edm;

SUITE(function_action_tests_raw_client)
{

TEST_FIXTURE(e2e_raw_client, invoke_unbound_function)
{
	auto function_ret = client.get_data_from_server(_XPLATSTR("Company/Microsoft.Test.OData.Services.ODataWCFService.GetEmployeesCount()")).get();
	VERIFY_ARE_EQUAL(function_ret.size(), 1);
	VERIFY_ARE_EQUAL(edm_type_kind_t::Primitive, function_ret[0]->get_value_type()->get_type_kind());
	auto primitive_value = std::dynamic_pointer_cast<odata_primitive_value>(function_ret[0]);
	int32_t count = primitive_value->as<int32_t>();
	VERIFY_ARE_EQUAL(count, 2);
}

TEST_FIXTURE(e2e_raw_client, simple_function)
{
	::odata::string_t employees_count_path = _XPLATSTR("Company/Microsoft.Test.OData.Services.ODataWCFService.GetEmployeesCount");
	std::vector<std::shared_ptr<odata_parameter>> parameters;
	std::vector<std::shared_ptr<odata_value>> returned_values;
	client.send_data_to_server(employees_count_path, parameters, returned_values, HTTP_GET).get();

	VERIFY_ARE_EQUAL(1, returned_values.size());
	VERIFY_ARE_EQUAL(2, std::dynamic_pointer_cast<odata_primitive_value>(returned_values[0])->as<int32_t>());
}

TEST_FIXTURE(e2e_raw_client, function_return_collection_of_complex_value)
{
	::odata::string_t product_details_path = _XPLATSTR("Products(5)/Microsoft.Test.OData.Services.ODataWCFService.GetProductDetails");
	std::vector<std::shared_ptr<odata_parameter>> parameters;
	parameters.push_back(::odata::make_shared<odata_parameter>(_XPLATSTR("count"), odata_primitive_value::make_primitive_value(3)));
	std::vector<std::shared_ptr<odata_value>> returned_values;
	client.send_data_to_server(product_details_path, parameters, returned_values, HTTP_GET).get();

	VERIFY_ARE_EQUAL(3, returned_values.size());
	auto detail1 = std::dynamic_pointer_cast<odata_entity_value>(returned_values[0]);
	int32_t product_id;
	detail1->try_get(_XPLATSTR("ProductID"), product_id);
	VERIFY_ARE_EQUAL(5, product_id);
}

TEST_FIXTURE(e2e_raw_client, action_of_primitive_value)
{
	::odata::string_t company_revenue_path = _XPLATSTR("Company/Revenue");
	auto revenue_odata_value = client.get_data_from_server(company_revenue_path).get()[0];
	auto revenue_primitive_value = std::dynamic_pointer_cast<odata_primitive_value>(revenue_odata_value);
	int32_t revenue = revenue_primitive_value->as<int32_t>();
	VERIFY_ARE_EQUAL(100000, revenue);

	std::vector<std::shared_ptr<odata_parameter>> parameters;
	parameters.push_back(::odata::make_shared<odata_parameter>(_XPLATSTR("IncreaseValue"), odata_primitive_value::make_primitive_value(100000)));
	std::vector<std::shared_ptr<odata_value>> returned_values;
	client.send_data_to_server(_XPLATSTR("Company/Microsoft.Test.OData.Services.ODataWCFService.IncreaseRevenue"), parameters, returned_values).get();

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
	::odata::string_t product_user_access = _XPLATSTR("Products(5)/UserAccess");
	auto user_access_value = std::dynamic_pointer_cast<odata_enum_value>(client.get_data_from_server(product_user_access).get()[0]);
	VERIFY_ARE_EQUAL(_XPLATSTR("None"), user_access_value->to_string());

	auto model = client.get_model().get();
	auto new_access = ::odata::make_shared<odata_enum_value>(model->find_enum_type(_XPLATSTR("AccessLevel")), _XPLATSTR("ReadWrite"));

	std::vector<std::shared_ptr<odata_parameter>> parameters;
	parameters.push_back(::odata::make_shared<odata_parameter>(_XPLATSTR("accessRight"), new_access));
	std::vector<std::shared_ptr<odata_value>> returned_values;
	client.send_data_to_server(_XPLATSTR("Products(5)/Microsoft.Test.OData.Services.ODataWCFService.AddAccessRight"), parameters, returned_values).get();

	VERIFY_ARE_EQUAL(1, returned_values.size());
	std::shared_ptr<odata_value> returned_value = returned_values[0];

	user_access_value = std::dynamic_pointer_cast<odata_enum_value>(client.get_data_from_server(product_user_access).get()[0]);
	VERIFY_ARE_EQUAL(_XPLATSTR("ReadWrite"), user_access_value->to_string());

	auto returned_enum_value = std::dynamic_pointer_cast<odata_enum_value>(returned_value);
	VERIFY_ARE_EQUAL(_XPLATSTR("ReadWrite"), returned_enum_value->to_string());
}

TEST_FIXTURE(e2e_raw_client, action_of_complex_value)
{
	::odata::string_t home_address = _XPLATSTR("People(1)/HomeAddress");
	auto home_address_value = std::dynamic_pointer_cast<odata_complex_value>(client.get_data_from_server(home_address).get()[0]);
	std::shared_ptr<odata_value> property_value;
	home_address_value->get_property_value(_XPLATSTR("City"), property_value);
	VERIFY_ARE_EQUAL(_XPLATSTR("Tokyo"), std::dynamic_pointer_cast<odata_primitive_value>(property_value)->as<::odata::string_t>());

	auto model = client.get_model().get();
	auto address_type = model->find_complex_type(_XPLATSTR("Address"));
	auto new_address1 = ::odata::make_shared<odata_complex_value>(address_type);
	new_address1->set_value(_XPLATSTR("City"), _XPLATSTR("Shanghai"));
	new_address1->set_value(_XPLATSTR("PostalCode"), _XPLATSTR("200000"));
	new_address1->set_value(_XPLATSTR("Street"), _XPLATSTR("Zixing Road"));

	auto addresses = ::odata::make_shared<odata_collection_value>(::odata::make_shared<edm_collection_type>(_XPLATSTR("AddressCollection"), address_type));
	addresses->add_collection_value(new_address1);

	std::vector<std::shared_ptr<odata_parameter>> parameters;
	parameters.push_back(::odata::make_shared<odata_parameter>(_XPLATSTR("addresses"), addresses));
	parameters.push_back(::odata::make_shared<odata_parameter>(_XPLATSTR("index"), odata_primitive_value::make_primitive_value(0)));
	std::vector<std::shared_ptr<odata_value>> returned_values;
	client.send_data_to_server(_XPLATSTR("People(1)/Microsoft.Test.OData.Services.ODataWCFService.ResetAddress"), parameters, returned_values).get();

	VERIFY_ARE_EQUAL(1, returned_values.size());
	std::shared_ptr<odata_value> returned_value = returned_values[0];

	home_address_value = std::dynamic_pointer_cast<odata_complex_value>(client.get_data_from_server(home_address).get()[0]);
	home_address_value->get_property_value(_XPLATSTR("City"), property_value);
	VERIFY_ARE_EQUAL(_XPLATSTR("Shanghai"), std::dynamic_pointer_cast<odata_primitive_value>(property_value)->as<::odata::string_t>());

	auto returned_entity_value = std::dynamic_pointer_cast<odata_entity_value>(returned_value);
	returned_entity_value->get_property_value(_XPLATSTR("HomeAddress"), property_value);
	std::dynamic_pointer_cast<odata_complex_value>(property_value)->get_property_value(_XPLATSTR("City"), property_value);
	VERIFY_ARE_EQUAL(_XPLATSTR("Shanghai"), std::dynamic_pointer_cast<odata_primitive_value>(property_value)->as<::odata::string_t>());
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

	VERIFY_ARE_EQUAL(ret[0]->get_edit_link(), _XPLATSTR("http://odatae2etest.azurewebsites.net/cpptest/DefaultService/ProductDetails(ProductID=5,ProductDetailID=2)"));
	VERIFY_ARE_EQUAL(ret[0]->get_productid(), 5);
	VERIFY_ARE_EQUAL(ret[0]->get_productdetailid(), 2);
	VERIFY_ARE_EQUAL(ret[0]->get_productname(), _XPLATSTR("CheeseCake"));
	VERIFY_ARE_EQUAL(ret[0]->get_description(), _XPLATSTR("Cheese-flavored snack"));

	VERIFY_ARE_EQUAL(ret[4]->get_edit_link(), _XPLATSTR("http://odatae2etest.azurewebsites.net/cpptest/DefaultService/ProductDetails(ProductID=5,ProductDetailID=6)"));
	VERIFY_ARE_EQUAL(ret[4]->get_productid(), 5);
	VERIFY_ARE_EQUAL(ret[4]->get_productdetailid(), 6);
	VERIFY_ARE_EQUAL(ret[4]->get_productname(), _XPLATSTR("Gatorade"));
	VERIFY_ARE_EQUAL(ret[4]->get_description(), _XPLATSTR("fitness drink!"));

	auto products_ret = ret[0]->GetRelatedProduct().get();

	VERIFY_ARE_EQUAL(products_ret.size(), 1);

	VERIFY_ARE_EQUAL(products_ret[0]->get_edit_link(), _XPLATSTR("http://odatae2etest.azurewebsites.net/cpptest/DefaultService/Products(5)"));
	VERIFY_ARE_EQUAL(products_ret[0]->get_productid(), 5);
	VERIFY_ARE_EQUAL(products_ret[0]->get_name(), _XPLATSTR("Cheetos"));
	VERIFY_ARE_EQUAL(products_ret[0]->get_quantityperunit(), _XPLATSTR("100g Bag"));
	VERIFY_ARE_EQUAL(products_ret[0]->get_covercolors().size(), 3);
	VERIFY_ARE_EQUAL(products_ret[0]->get_covercolors()[0], Color::Green);
}

TEST_FIXTURE(e2e_test_case, function_no_param_return_complex)
{
	auto people = service_context->create_people_query()->key(_XPLATSTR("1"))->execute_query().get();

	VERIFY_ARE_EQUAL(people.size(), 1);

	auto home_address_obj = people[0]->GetHomeAddress().get();

	VERIFY_ARE_EQUAL(home_address_obj.size(), 1);

	VERIFY_ARE_EQUAL(*(home_address_obj[0]->get_familyname()), _XPLATSTR("Cats"));
	VERIFY_ARE_EQUAL(home_address_obj[0]->get_city(), _XPLATSTR("Tokyo"));
	VERIFY_ARE_EQUAL(home_address_obj[0]->get_postalcode(), _XPLATSTR("98052"));
	VERIFY_ARE_EQUAL(home_address_obj[0]->get_street(), _XPLATSTR("1 Microsoft Way"));
}

TEST_FIXTURE(e2e_test_case, function_import_no_param_return_collection_entity)
{
	auto products = service_context->GetAllProducts().get();

	VERIFY_ARE_EQUAL(products.size(), 5);

	VERIFY_ARE_EQUAL(products[0]->get_edit_link(), _XPLATSTR("http://odatae2etest.azurewebsites.net/cpptest/DefaultService/Products(5)"));
	VERIFY_ARE_EQUAL(products[0]->get_productid(), 5);
	VERIFY_ARE_EQUAL(products[0]->get_name(), _XPLATSTR("Cheetos"));
	VERIFY_ARE_EQUAL(products[0]->get_quantityperunit(), _XPLATSTR("100g Bag"));
	VERIFY_ARE_EQUAL(products[0]->get_covercolors().size(), 3);
	VERIFY_ARE_EQUAL(products[0]->get_covercolors()[0], Color::Green);
}

TEST_FIXTURE(e2e_test_case, action_function_import_two_param_return_collection_primitive)
{
	std::vector<::odata::string_t> emails;
	emails.push_back(_XPLATSTR("email_1"));
	emails.push_back(_XPLATSTR("email_2"));
	emails.push_back(_XPLATSTR("email_3"));
	auto action_ret = service_context->ResetBossEmail(emails).get();

	VERIFY_ARE_EQUAL(action_ret.size(), 3);
	VERIFY_ARE_EQUAL(action_ret[0], _XPLATSTR("email_1"));
	VERIFY_ARE_EQUAL(action_ret[1], _XPLATSTR("email_2"));
	VERIFY_ARE_EQUAL(action_ret[2], _XPLATSTR("email_3"));

	auto function_ret = service_context->GetBossEmails(0, 5).get();

	VERIFY_ARE_EQUAL(function_ret.size(), 3);
	VERIFY_ARE_EQUAL(function_ret[0], _XPLATSTR("email_1"));
	VERIFY_ARE_EQUAL(function_ret[1], _XPLATSTR("email_2"));
	VERIFY_ARE_EQUAL(function_ret[2], _XPLATSTR("email_3"));
}

TEST_FIXTURE(e2e_test_case, function_import_one_param_return_entity)
{
	auto ret = service_context->GetPerson2(_XPLATSTR("Tokyo")).get();

	VERIFY_ARE_EQUAL(ret.size(), 1);

	VERIFY_ARE_EQUAL(ret[0]->get_edit_link(), _XPLATSTR("http://odatae2etest.azurewebsites.net/cpptest/DefaultService/People(1)/Microsoft.Test.OData.Services.ODataWCFService.Customer"));
	VERIFY_ARE_EQUAL(ret[0]->get_firstname(), _XPLATSTR("Bob"));
	VERIFY_ARE_EQUAL(ret[0]->get_lastname(), _XPLATSTR("Cat"));
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
	address.set_city(_XPLATSTR("Shang Hai"));
	address.set_postalcode(_XPLATSTR("123123"));
	address.set_street(_XPLATSTR("ZiXing Road"));

	auto ret = service_context->ResetBossAddress(address).get();

	VERIFY_ARE_EQUAL(ret.size(), 1);

	VERIFY_ARE_EQUAL(ret[0]->get_city(), _XPLATSTR("Shang Hai"));
	VERIFY_ARE_EQUAL(ret[0]->get_postalcode(), _XPLATSTR("123123"));
	VERIFY_ARE_EQUAL(ret[0]->get_street(), _XPLATSTR("ZiXing Road"));
}

TEST_FIXTURE(e2e_test_case, action_one_param_return_entity)
{
	std::vector<Address> addresses;

	Address address1(service_context);
	address1.set_city(_XPLATSTR("Shang Hai"));
	address1.set_postalcode(_XPLATSTR("123123"));
	address1.set_street(_XPLATSTR("ZiXing Road"));
	addresses.push_back(address1);

	Address address2(service_context);
	address2.set_city(_XPLATSTR("Bei Jing"));
	address2.set_postalcode(_XPLATSTR("006767"));
	address2.set_street(_XPLATSTR("TiananMen Road"));
	addresses.push_back(address2);

	Address address3(service_context);
	address3.set_city(_XPLATSTR("La Sa"));
	address3.set_postalcode(_XPLATSTR("221122"));
	address3.set_street(_XPLATSTR("LaSa Road"));
	addresses.push_back(address3);

	auto people = service_context->create_people_query()->key(_XPLATSTR("1"))->execute_query().get();

	auto ret = people[0]->ResetAddress(addresses, 2).get();

	people = service_context->create_people_query()->key(_XPLATSTR("1"))->execute_query().get();

	VERIFY_ARE_EQUAL(people.size(), 1);

	VERIFY_ARE_EQUAL(people[0]->get_homeaddress()->get_city(), _XPLATSTR("La Sa"));
	VERIFY_ARE_EQUAL(people[0]->get_homeaddress()->get_postalcode(), _XPLATSTR("221122"));
	VERIFY_ARE_EQUAL(people[0]->get_homeaddress()->get_street(), _XPLATSTR("LaSa Road"));
}

TEST_FIXTURE(e2e_test_case, action_one_param__datetime_return_entity)
{
	auto accounts = service_context->create_accounts_query()->key(_XPLATSTR("101"))->execute_query().get();

	::utility::datetime dt = ::utility::datetime::from_string(_XPLATSTR("2014-05-19T14:05:00Z"), ::utility::datetime::date_format::ISO_8601);
	auto ret = accounts[0]->RefreshDefaultPI(dt).get();

	VERIFY_ARE_EQUAL(ret[0]->get_createddate().to_string(::utility::datetime::date_format::ISO_8601), _XPLATSTR("2014-05-19T14:05:00Z"));
}

}

}}}
