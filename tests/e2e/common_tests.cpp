//---------------------------------------------------------------------
// <copyright file="common_tests.cpp" company="Microsoft">
//      Copyright (C) Microsoft Corporation. All rights reserved. See License.txt in the project root for license information.
// </copyright>
//---------------------------------------------------------------------

#include "e2e_tests.h"
#include "odata_wcf_service.h"

#ifdef __APPLE__
extern "C" UnitTest::TestList& UnitTest::GetTestList()
{
    static TestList s_list;
    return s_list;
}
#endif



namespace tests { namespace e2e { namespace odata {

using namespace odata_wcf_service;

SUITE(common_tests_raw_client)
{

TEST_FIXTURE(e2e_raw_client, query_entityset)
{
	::odata::string_t entity_set_name = _XPLATSTR("Products");

	auto entity_set = client.get_entities(entity_set_name).get();

	VERIFY_ARE_EQUAL(entity_set.size(), 5);
	VERIFY_ARE_EQUAL(edm_type_kind_t::Entity, entity_set[0]->get_value_type()->get_type_kind());

	std::shared_ptr<odata_entity_value> first_entity = entity_set[0];
	std::shared_ptr<odata_value> property_value;
	VERIFY_IS_TRUE(first_entity->get_property_value(_XPLATSTR("Name"), property_value));
	VERIFY_ARE_EQUAL(edm_type_kind_t::Primitive, property_value->get_value_type()->get_type_kind());
	auto primitive_value = std::dynamic_pointer_cast<odata_primitive_value>(property_value);
	::odata::string_t name = primitive_value->as<::odata::string_t>();
	VERIFY_ARE_EQUAL(_XPLATSTR("Cheetos"), name);
}

TEST_FIXTURE(e2e_raw_client, query_entity)
{
	auto query_result = client.get_data_from_server(_XPLATSTR("Products(6)")).get();
	VERIFY_ARE_EQUAL(query_result.size(), 1);
	VERIFY_ARE_EQUAL(edm_type_kind_t::Entity, query_result[0]->get_value_type()->get_type_kind());

	std::shared_ptr<odata_entity_value> entity = std::dynamic_pointer_cast<odata_entity_value>(query_result[0]);
	std::shared_ptr<odata_value> property_value;
	VERIFY_IS_TRUE(entity->get_property_value(_XPLATSTR("UnitPrice"), property_value));
	VERIFY_ARE_EQUAL(edm_type_kind_t::Primitive, property_value->get_value_type()->get_type_kind());
	auto primitive_value = std::dynamic_pointer_cast<odata_primitive_value>(property_value);
	double unit_price = primitive_value->as<double>();
	VERIFY_ARE_EQUAL(3.24, unit_price);
}

TEST_FIXTURE(e2e_raw_client, query_primitive_property)
{
	auto query_result = client.get_data_from_server(_XPLATSTR("Products(7)/QuantityInStock")).get();
	VERIFY_ARE_EQUAL(query_result.size(), 1);

	std::shared_ptr<odata_value> property_value = query_result[0];
	VERIFY_ARE_EQUAL(edm_type_kind_t::Primitive, property_value->get_value_type()->get_type_kind());
	auto primitive_value = std::dynamic_pointer_cast<odata_primitive_value>(property_value);
	int32_t quantity = primitive_value->as<int32_t>();
	VERIFY_ARE_EQUAL(20, quantity);
}

TEST_FIXTURE(e2e_raw_client, query_complex_property)
{
	auto query_result = client.get_data_from_server(_XPLATSTR("People(1)/HomeAddress")).get();
	VERIFY_ARE_EQUAL(query_result.size(), 1);
	VERIFY_ARE_EQUAL(edm_type_kind_t::Complex, query_result[0]->get_value_type()->get_type_kind());

	auto property_value = std::dynamic_pointer_cast<odata_complex_value>(query_result[0]);
	std::shared_ptr<odata_value> street_field;
	VERIFY_IS_TRUE(property_value->get_property_value(_XPLATSTR("Street"), street_field));
	VERIFY_ARE_EQUAL(edm_type_kind_t::Primitive, street_field->get_value_type()->get_type_kind());
	auto primitive_value = std::dynamic_pointer_cast<odata_primitive_value>(street_field);
	::odata::string_t street = primitive_value->as<::odata::string_t>();
	VERIFY_ARE_EQUAL(_XPLATSTR("1 Microsoft Way"), street);
}

TEST_FIXTURE(e2e_raw_client, query_collection_property)
{
	auto query_result = client.get_data_from_server(_XPLATSTR("People(4)/Numbers")).get();
	VERIFY_ARE_EQUAL(query_result.size(), 3);
	VERIFY_ARE_EQUAL(edm_type_kind_t::Primitive, query_result[0]->get_value_type()->get_type_kind());

	auto primitive_value = std::dynamic_pointer_cast<odata_primitive_value>(query_result[1]);
	::odata::string_t number = primitive_value->as<::odata::string_t>();
	VERIFY_ARE_EQUAL(_XPLATSTR("555-555-5555"), number);
}

TEST_FIXTURE(e2e_raw_client, query_basic_properties_in_entity)
{
	auto query_result = client.get_data_from_server(_XPLATSTR("People(4)")).get();
	VERIFY_ARE_EQUAL(query_result.size(), 1);
	VERIFY_ARE_EQUAL(edm_type_kind_t::Entity, query_result[0]->get_value_type()->get_type_kind());

	std::shared_ptr<odata_entity_value> entity = std::dynamic_pointer_cast<odata_entity_value>(query_result[0]);

	//collection property
	std::shared_ptr<odata_value> collection_property;
	VERIFY_IS_TRUE(entity->get_property_value(_XPLATSTR("Numbers"), collection_property));
	VERIFY_ARE_EQUAL(edm_type_kind_t::Collection, collection_property->get_value_type()->get_type_kind());

	auto collection_value = std::dynamic_pointer_cast<odata_collection_value>(collection_property);
	auto collection_vector = collection_value->get_collection_values();
	VERIFY_ARE_EQUAL(3, collection_vector.size());

	auto collectin_member = std::dynamic_pointer_cast<odata_primitive_value>(collection_vector[1]);
	VERIFY_ARE_EQUAL(_XPLATSTR("555-555-5555"), collectin_member->as<::odata::string_t>());

	//TODO-tiano: check the other property types
}

/*** $value not supported for now

TEST(query_property_value)
{
}

***/

TEST_FIXTURE(e2e_raw_client, create_entity)
{
	auto model = client.get_model().get();

	::odata::string_t entity_set_name = _XPLATSTR("Accounts");
	std::shared_ptr<odata_entity_value> entity = ::odata::make_shared<odata_entity_value>(model->find_container()->find_entity_set(entity_set_name)->get_entity_type());

	entity->set_value(_XPLATSTR("AccountID"), 130);
	entity->set_value(_XPLATSTR("CountryRegion"), _XPLATSTR("CN"));

	auto accountinfo_type = model->find_complex_type(_XPLATSTR("AccountInfo"));
	auto account_info = ::odata::make_shared<odata_complex_value>(accountinfo_type);
	::odata::string_t account_firstname = _XPLATSTR("cpp");
	::odata::string_t account_lastname = _XPLATSTR("client");
	account_info->set_value(_XPLATSTR("FirstName"), account_firstname);
	account_info->set_value(_XPLATSTR("LastName"), account_lastname);

	entity->set_value(_XPLATSTR("AccountInfo"), account_info);

	auto response_code = client.create_entity(entity_set_name, entity).get();
	VERIFY_ARE_EQUAL(201, response_code);

	//query the newly created entity
	auto query_result = client.get_data_from_server(_XPLATSTR("Accounts(130)")).get();
	VERIFY_ARE_EQUAL(query_result.size(), 1);

	auto new_entity = std::dynamic_pointer_cast<odata_entity_value>(query_result[0]);
	std::shared_ptr<odata_value> property_value;
	VERIFY_IS_TRUE(entity->get_property_value(_XPLATSTR("AccountID"), property_value));
	auto primitive_value = std::dynamic_pointer_cast<odata_primitive_value>(property_value);
	int32_t new_id = primitive_value->as<int32_t>();
	VERIFY_ARE_EQUAL(130, new_id);
}

/* TODO: [tiano]
TEST_FIXTURE(e2e_raw_client, create_entity_with_unescaped_string)
{
	auto model = client.get_model().get();

	::odata::string_t entity_set_name = _XPLATSTR("Accounts");
	std::shared_ptr<odata_entity_value> entity = ::odata::make_shared<odata_entity_value>(model->find_entity_set_type(entity_set_name));

	entity->set_value(_XPLATSTR("AccountID"), 130);
	entity->set_value(_XPLATSTR("CountryRegion"), _XPLATSTR("CN"));

	auto accountinfo_type = model->find_complex_type(_XPLATSTR("AccountInfo"));
	auto account_info = ::odata::make_shared<odata_complex_value>(accountinfo_type);
	::odata::string_t account_firstname = _XPLATSTR("Î¢Èí\nÉÏº£");
	::odata::string_t account_lastname = _XPLATSTR("Å¶µÃËû");
	account_info->set_value(_XPLATSTR("FirstName"), account_firstname);
	account_info->set_value(_XPLATSTR("LastName"), account_lastname);

	entity->set_value(_XPLATSTR("AccountInfo"), account_info);

	auto response_code = client.create_entity(entity_set_name, entity).get();
	VERIFY_ARE_EQUAL(201, response_code);

	//query the newly created entity
	auto query_result = client.get_data_from_server(_XPLATSTR("Accounts(130)")).get();
	VERIFY_ARE_EQUAL(query_result.size(), 1);

	auto new_entity = std::dynamic_pointer_cast<odata_entity_value>(query_result[0]);
	std::shared_ptr<odata_value> property_value;
	VERIFY_IS_TRUE(entity->get_property_value(_XPLATSTR("AccountID"), property_value));
	auto primitive_value = std::dynamic_pointer_cast<odata_primitive_value>(property_value);
	int32_t new_id = primitive_value->as<int32_t>();
	VERIFY_ARE_EQUAL(130, new_id);

	std::shared_ptr<odata_value> complex_property_value;
	new_entity->get_property_value(_XPLATSTR("AccountInfo"), complex_property_value);
	auto complex_value = std::dynamic_pointer_cast<odata_complex_value>(complex_property_value);
	::odata::string_t firstname, lastname;
	complex_value->try_get(_XPLATSTR("FirstName"), firstname);
	VERIFY_ARE_EQUAL(_XPLATSTR("Î¢Èí\nÉÏº£"), firstname);
	complex_value->try_get(_XPLATSTR("LastName"), lastname);
	VERIFY_ARE_EQUAL(_XPLATSTR("Å¶µÃËû"), lastname);
}
*/

TEST_FIXTURE(e2e_raw_client, update_entity_with_patch)
{
	auto model = client.get_model().get();

	//check the old value
	auto query_result = client.get_data_from_server(_XPLATSTR("Accounts(101)")).get();
	auto old_entity = std::dynamic_pointer_cast<odata_entity_value>(query_result[0]);
	std::shared_ptr<odata_value> old_value;
	old_entity->get_property_value(_XPLATSTR("CountryRegion"), old_value);
	auto old_country = std::dynamic_pointer_cast<odata_primitive_value>(old_value);
	VERIFY_ARE_EQUAL(_XPLATSTR("US"), old_country->as<::odata::string_t>());

	//update the entity with patch
	old_entity->set_value(_XPLATSTR("CountryRegion"), _XPLATSTR("GB"));

	auto response_code = client.patch_entity(_XPLATSTR("Accounts"), old_entity).get();
	VERIFY_ARE_EQUAL(204, response_code);

	//query the updated entity
	auto check_query = client.get_data_from_server(_XPLATSTR("Accounts(101)")).get();

	auto new_entity = std::dynamic_pointer_cast<odata_entity_value>(check_query[0]);
	std::shared_ptr<odata_value> property_value;
	VERIFY_IS_TRUE(new_entity->get_property_value(_XPLATSTR("CountryRegion"), property_value));
	auto primitive_value = std::dynamic_pointer_cast<odata_primitive_value>(property_value);
	::odata::string_t new_country = primitive_value->as<::odata::string_t>();
	VERIFY_ARE_EQUAL(_XPLATSTR("GB"), new_country);
}
}

SUITE(common_tests)
{

TEST_FIXTURE(e2e_test_case, query_entityset)
{
	auto people = service_context->create_people_query()->execute_query().get();

	VERIFY_ARE_EQUAL(5, people.size());
	VERIFY_ARE_EQUAL(_XPLATSTR("Peter"), people[4]->get_firstname());
}

TEST_FIXTURE(e2e_test_case, query_entity)
{
	auto people = service_context->create_people_query()->key(_XPLATSTR("4"))->execute_query().get();

	VERIFY_ARE_EQUAL(1, people.size());
	VERIFY_ARE_EQUAL(_XPLATSTR("Elmo"), people[0]->get_firstname());
}

TEST_FIXTURE(e2e_test_case, top_level_query)
{
	auto price = service_context->create_query<odata_primitive_query_executor<double>, odata_query_builder>(_XPLATSTR("Products(5)/UnitPrice"))->execute_query().get();
	VERIFY_ARE_EQUAL(1, price.size());
	VERIFY_ARE_EQUAL(3.24, price[0]);

	auto acct_info = service_context->create_query<odata_complex_query_executor<AccountInfo>, odata_query_builder>(_XPLATSTR("Accounts(101)/AccountInfo"))->execute_query().get();
	VERIFY_ARE_EQUAL(1, acct_info.size());
	VERIFY_ARE_EQUAL(_XPLATSTR("Alex"), acct_info[0]->get_firstname());

	auto numbers = service_context->create_query<odata_primitive_query_executor<::odata::string_t>, odata_query_builder>(_XPLATSTR("People(4)/Numbers"))->execute_query().get();
	VERIFY_ARE_EQUAL(3, numbers.size());
	VERIFY_ARE_EQUAL(_XPLATSTR("444-444-4444"), numbers[0]);

	/***
	//Issue: [tiano] $value doesn't work because the Accept header can't be json. Bug #2265002

	auto name = service_context->create_query<odata_primitive_query_executor<::odata::string_t>, odata_query_builder>(_XPLATSTR("Products(5)/Name/$value"))->execute_query().get();
	VERIFY_ARE_EQUAL(1, name.size());
	VERIFY_ARE_EQUAL(_XPLATSTR("Cheetos"), name[0]);

	auto count = service_context->create_query<odata_primitive_query_executor<int32_t>, odata_query_builder>(_XPLATSTR("Products/$count"))->execute_query().get();
	***/
}


TEST_FIXTURE(e2e_test_case, create_entity)
{
	auto new_account = ::odata::make_shared<Account>(service_context);
	new_account->set_accountid(140);
	new_account->set_countryregion(_XPLATSTR("DE"));
	auto acinfo = ::odata::make_shared<AccountInfo>(service_context);
	acinfo->set_firstname(_XPLATSTR("cpp"));
	acinfo->set_lastname(_XPLATSTR("test"));
	new_account->set_accountinfo(acinfo);

	auto status_code = service_context->add_object(_XPLATSTR("Accounts"), new_account).get();
	VERIFY_ARE_EQUAL(201, status_code);

	auto account = service_context->create_accounts_query()->key(_XPLATSTR("140"))->execute_query().get();
	VERIFY_ARE_EQUAL(_XPLATSTR("DE"), account[0]->get_countryregion());
	auto info = account[0]->get_accountinfo();
	VERIFY_ARE_EQUAL(_XPLATSTR("cpp"), info->get_firstname());
	VERIFY_ARE_EQUAL(_XPLATSTR("test"), info->get_lastname());
}

TEST_FIXTURE(e2e_test_case, update_entity)
{
	auto people = service_context->create_people_query()->key(_XPLATSTR("5"))->execute_query().get();
	VERIFY_ARE_EQUAL(1, people.size());

	people[0]->set_firstname(_XPLATSTR("cpp test updated"));
	auto status_code = service_context->update_object(people[0], HTTP_PUT).get();
	VERIFY_ARE_EQUAL(204, status_code);

	auto people_new = service_context->create_people_query()->key(_XPLATSTR("5"))->execute_query().get();
	VERIFY_ARE_EQUAL(_XPLATSTR("cpp test updated"), people_new[0]->get_firstname());
}

TEST_FIXTURE(e2e_test_case, delete_entity)
{
	auto accounts = service_context->create_accounts_query()->key(_XPLATSTR("107"))->execute_query().get();
	VERIFY_ARE_EQUAL(1, accounts.size());

	auto status_code = service_context->delete_object(accounts[0]).get();
	VERIFY_ARE_EQUAL(204, status_code);

	VERIFY_THROWS(service_context->create_accounts_query()->key(_XPLATSTR("107"))->execute_query().get(), ::odata::communication::service_exception); //TODO-tiano: check error codes
}

}

}}}