//---------------------------------------------------------------------
// <copyright file="derived_type_tests.cpp" company="Microsoft">
//      Copyright (C) Microsoft Corporation. All rights reserved. See License.txt in the project root for license information.
// </copyright>
//---------------------------------------------------------------------

#include "e2e_tests.h"
#include "odata_wcf_service.h"

namespace tests { namespace e2e { namespace odata {

using namespace odata_wcf_service;


SUITE(derived_type_tests_raw_client)
{

TEST_FIXTURE(e2e_raw_client, query_entity_collection)
{
	auto entities = client.get_data_from_server(_XPLATSTR("People/Microsoft.Test.OData.Services.ODataWCFService.Customer")).get();
	VERIFY_ARE_EQUAL(entities.size(), 2);

	std::shared_ptr<odata_entity_value> first_entity = std::dynamic_pointer_cast<odata_entity_value>(entities[0]);
	std::shared_ptr<odata_value> property_value;
	VERIFY_IS_TRUE(first_entity->get_property_value(_XPLATSTR("City"), property_value));
	VERIFY_ARE_EQUAL(edm_type_kind_t::Primitive, property_value->get_value_type()->get_type_kind());
	auto primitive_value = std::dynamic_pointer_cast<odata_primitive_value>(property_value);
	::odata::string_t city = primitive_value->as<::odata::string_t>();
	VERIFY_ARE_EQUAL(_XPLATSTR("London"), city);
}

TEST_FIXTURE(e2e_raw_client, query_single_entity)
{
	auto entities = client.get_data_from_server(_XPLATSTR("People(3)/Microsoft.Test.OData.Services.ODataWCFService.Employee")).get();
	VERIFY_ARE_EQUAL(entities.size(), 1);

	std::shared_ptr<odata_entity_value> first_entity = std::dynamic_pointer_cast<odata_entity_value>(entities[0]);
	std::shared_ptr<odata_value> property_value;
	VERIFY_IS_TRUE(first_entity->get_property_value(_XPLATSTR("DateHired"), property_value));
	VERIFY_ARE_EQUAL(edm_type_kind_t::Primitive, property_value->get_value_type()->get_type_kind());
	auto primitive_value = std::dynamic_pointer_cast<odata_primitive_value>(property_value);
	::odata::string_t date_hired = primitive_value->as<::odata::string_t>();
	VERIFY_ARE_EQUAL(_XPLATSTR("2010-12-13T00:00:00Z"), date_hired);
}

}


SUITE(derived_type_tests)
{

TEST_FIXTURE(e2e_test_case, query_entity_collection)
{
	auto people = service_context->create_people_query()->execute_query().get();
	VERIFY_ARE_EQUAL(people.size(), 5);

	auto customer = std::dynamic_pointer_cast<Customer>(people[0]);
	VERIFY_ARE_EQUAL(_XPLATSTR("London") ,customer->get_city());
}

TEST_FIXTURE(e2e_test_case, create_derived_type_entity)
{
	auto new_customer = ::odata::make_shared<Customer>(service_context);
	new_customer->set_personid(10);
	new_customer->set_firstname(_XPLATSTR("cpp"));
	new_customer->set_lastname(_XPLATSTR("test"));
	std::vector<::odata::string_t> numbers;
	numbers.push_back(_XPLATSTR("111-111-1112"));
	numbers.push_back(_XPLATSTR("111-222-2222"));
	new_customer->set_numbers(numbers);
	std::vector<::odata::string_t> emails;
	emails.push_back(_XPLATSTR("abc@mail.com"));
	new_customer->set_emails(emails);
	new_customer->set_birthday(::utility::datetime::utc_now());
	new_customer->set_city(_XPLATSTR("Shanghai"));
	::utility::seconds time(123);
	new_customer->set_timebetweenlasttwoorders(time);

	auto status_code = service_context->add_object(_XPLATSTR("People"), new_customer).get();
	VERIFY_ARE_EQUAL(201, status_code);
	VERIFY_ARE_EQUAL(_XPLATSTR("http://odatae2etest.azurewebsites.net/cpptest/DefaultService/People(10)"), new_customer->get_edit_link()); //Service issue: [tiano] should be People(10)/Microsoft.Test.OData.Services.ODataWCFService.Customer

	auto check_result = service_context->create_query<odata_singleton_query_executor<Customer>, odata_query_builder>(_XPLATSTR("People(10)/Microsoft.Test.OData.Services.ODataWCFService.Customer"))->execute_query().get();
	VERIFY_ARE_EQUAL(_XPLATSTR("Shanghai"), check_result->get_city());

	new_customer->set_city(_XPLATSTR("Beijing"));
	status_code = service_context->update_object(new_customer).get();
	VERIFY_ARE_EQUAL(204, status_code);
	check_result = service_context->create_query<odata_singleton_query_executor<Customer>, odata_query_builder>(_XPLATSTR("People(10)/Microsoft.Test.OData.Services.ODataWCFService.Customer"))->execute_query().get();
	VERIFY_ARE_EQUAL(_XPLATSTR("Beijing"), check_result->get_city());
}

TEST_FIXTURE(e2e_test_case, update_derived_type_entity)
{
	auto people = service_context->create_people_query()->key(_XPLATSTR("2"))->execute_query().get();
	VERIFY_ARE_EQUAL(people.size(), 1);
	auto customer = std::dynamic_pointer_cast<Customer>(people[0]);
	VERIFY_ARE_EQUAL(_XPLATSTR("Sydney") ,customer->get_city());

	customer->set_city(_XPLATSTR("Beijing"));
	::utility::seconds time(600);
	customer->set_timebetweenlasttwoorders(time);
	auto status_code = service_context->update_object(customer).get();
	VERIFY_ARE_EQUAL(204, status_code);

	auto new_people = service_context->create_people_query()->key(_XPLATSTR("2"))->execute_query().get();
	VERIFY_ARE_EQUAL(new_people.size(), 1);
	auto new_customer = std::dynamic_pointer_cast<Customer>(new_people[0]);
	VERIFY_ARE_EQUAL(_XPLATSTR("Beijing") ,new_customer->get_city());
}

TEST_FIXTURE(e2e_test_case, delete_derived_type_entity)
{
	auto people = service_context->create_people_query()->key(_XPLATSTR("2"))->execute_query().get();
	VERIFY_ARE_EQUAL(people.size(), 1);
	auto customer = std::dynamic_pointer_cast<Customer>(people[0]);
	VERIFY_ARE_EQUAL(_XPLATSTR("Sydney") ,customer->get_city());

	auto status_code = service_context->delete_object(customer).get();
	VERIFY_ARE_EQUAL(204, status_code);

	VERIFY_THROWS(service_context->create_people_query()->key(_XPLATSTR("2"))->execute_query().get(), ::odata::communication::service_exception); //TODO-tiano: check error codes
}

TEST_FIXTURE(e2e_test_case, query_derived_type_complex)
{
	auto people = service_context->create_people_query()->key(_XPLATSTR("1"))->execute_query().get();
	VERIFY_ARE_EQUAL(people.size(), 1);

	auto address = people[0]->get_homeaddress();
	VERIFY_ARE_EQUAL(address->get_city(), _XPLATSTR("Tokyo"));
	VERIFY_ARE_EQUAL(address->get_street(), _XPLATSTR("1 Microsoft Way"));
	VERIFY_ARE_EQUAL(address->get_postalcode(), _XPLATSTR("98052"));

	auto home = (HomeAddress*)(address);
	VERIFY_ARE_EQUAL(*(home->get_familyname()), _XPLATSTR("Cats"));
}

TEST_FIXTURE(e2e_test_case, update_derived_type_complex)
{
	auto people = service_context->create_people_query()->key(_XPLATSTR("1"))->execute_query().get();
	VERIFY_ARE_EQUAL(people.size(), 1);
	auto customer = std::dynamic_pointer_cast<Customer>(people[0]);

	::utility::seconds time(600);
	customer->set_timebetweenlasttwoorders(time);

	auto address = customer->get_homeaddress();
	auto home = (HomeAddress*)(address);
	home->set_city(_XPLATSTR("Shang Hai"));
	home->set_street(_XPLATSTR("Zi Zhu"));
	home->set_postalcode(_XPLATSTR("120012"));
	home->set_familyname(_XPLATSTR("Panda"));
	auto status_code = service_context->update_object(customer).get();
	VERIFY_ARE_EQUAL(204, status_code);

	people = service_context->create_people_query()->key(_XPLATSTR("1"))->execute_query().get();

	address = people[0]->get_homeaddress();
	VERIFY_ARE_EQUAL(address->get_city(), _XPLATSTR("Shang Hai"));
	VERIFY_ARE_EQUAL(address->get_street(), _XPLATSTR("Zi Zhu"));
	VERIFY_ARE_EQUAL(address->get_postalcode(), _XPLATSTR("120012"));

	home = (HomeAddress*)(address);
	VERIFY_ARE_EQUAL(*(home->get_familyname()), _XPLATSTR("Panda"));
}

TEST_FIXTURE(e2e_test_case, create_derived_type_complex_in_entity)
{
    auto new_customer = ::odata::make_shared<Customer>(service_context);
	new_customer->set_personid(10);
	new_customer->set_firstname(_XPLATSTR("cpp"));
	new_customer->set_lastname(_XPLATSTR("test"));
	std::vector<::odata::string_t> numbers;
	numbers.push_back(_XPLATSTR("111-111-1112"));
	numbers.push_back(_XPLATSTR("111-222-2222"));
	new_customer->set_numbers(numbers);
	std::vector<::odata::string_t> emails;
	emails.push_back(_XPLATSTR("abc@mail.com"));
	new_customer->set_emails(emails);
	new_customer->set_birthday(::utility::datetime::utc_now());
	new_customer->set_city(_XPLATSTR("Shanghai"));
	::utility::seconds time(123);
	new_customer->set_timebetweenlasttwoorders(time);

	auto homeaddress = ::odata::make_shared<HomeAddress>(service_context);
	homeaddress->set_city(_XPLATSTR("Shang Hai"));
	homeaddress->set_street(_XPLATSTR("Zi Zhu"));
	homeaddress->set_postalcode(_XPLATSTR("120012"));
	homeaddress->set_familyname(_XPLATSTR("Panda"));
	new_customer->set_homeaddress(homeaddress);

	auto status_code = service_context->add_object(_XPLATSTR("People"), new_customer).get();
	VERIFY_ARE_EQUAL(201, status_code);
	VERIFY_ARE_EQUAL(_XPLATSTR("http://odatae2etest.azurewebsites.net/cpptest/DefaultService/People(10)"), new_customer->get_edit_link()); //Service issue: [tiano] should be People(10)/Microsoft.Test.OData.Services.ODataWCFService.Customer

	auto check_result = service_context->create_query<odata_singleton_query_executor<Customer>, odata_query_builder>(_XPLATSTR("People(10)/Microsoft.Test.OData.Services.ODataWCFService.Customer"))->execute_query().get();
	VERIFY_ARE_EQUAL(_XPLATSTR("Shanghai"), check_result->get_city());

	new_customer->set_city(_XPLATSTR("Beijing"));
	status_code = service_context->update_object(new_customer).get();
	VERIFY_ARE_EQUAL(204, status_code);
	check_result = service_context->create_query<odata_singleton_query_executor<Customer>, odata_query_builder>(_XPLATSTR("People(10)/Microsoft.Test.OData.Services.ODataWCFService.Customer"))->execute_query().get();
	VERIFY_ARE_EQUAL(_XPLATSTR("Beijing"), check_result->get_city());
}

//Derived type in navigation properties?
//
}

}}}
