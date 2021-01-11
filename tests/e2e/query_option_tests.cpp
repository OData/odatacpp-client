//---------------------------------------------------------------------
// <copyright file="query_option_tests.cpp" company="Microsoft">
//      Copyright (C) Microsoft Corporation. All rights reserved. See License.txt in the project root for license information.
// </copyright>
//---------------------------------------------------------------------

#include "e2e_tests.h"
#include "odata_wcf_service.h"


namespace tests { namespace e2e { namespace odata {

using namespace odata_wcf_service;


SUITE(query_option_tests_raw_client)
{

/***
TEST_FIXTURE(e2e_raw_client, filter_option)  //TODO-tiano: bug #2163372
{
	auto entities = client.get_data_from_server(_XPLATSTR("Accounts?$filter=CountryRegion eq 'US'")).get();
	VERIFY_ARE_EQUAL(entities.size(), 2);
}
***/

TEST_FIXTURE(e2e_raw_client, expand_option)
{
	auto entities = client.get_data_from_server(_XPLATSTR("Accounts(101)?$expand=MyGiftCard")).get();
	VERIFY_ARE_EQUAL(entities.size(), 1);

	//verify the expand
}

}


SUITE(query_option_tests)
{

TEST_FIXTURE(e2e_test_case, filter_option)
{
	auto accounts = service_context->create_accounts_query()->filter(_XPLATSTR("CountryRegion eq 'US'"))->execute_query().get();
	VERIFY_ARE_EQUAL(2, accounts.size());
	for (auto iter = accounts.cbegin(); iter != accounts.cend(); ++iter)
	{
		VERIFY_ARE_EQUAL(_XPLATSTR("US"), (*iter)->get_countryregion());
	}

	//filter on derived type property
	//filter on property of navigation proerty entity
}

TEST_FIXTURE(e2e_test_case, orderby_option)
{
	auto accounts = service_context->create_accounts_query()->orderby(_XPLATSTR("CountryRegion"))->execute_query().get();
	VERIFY_ARE_EQUAL(7, accounts.size());
	VERIFY_ARE_EQUAL(_XPLATSTR("CN"), accounts[0]->get_countryregion());
	VERIFY_ARE_EQUAL(_XPLATSTR("FR"), accounts[3]->get_countryregion());
	VERIFY_ARE_EQUAL(_XPLATSTR("US"), accounts[5]->get_countryregion());
}

TEST_FIXTURE(e2e_test_case, select_option)
{
	auto products = service_context->create_products_query()->key(_XPLATSTR("5"))->select(_XPLATSTR("Name, UserAccess"))->execute_query().get();
	VERIFY_ARE_EQUAL(1, products.size());
	std::shared_ptr<Product> product = products[0];
	VERIFY_ARE_EQUAL(_XPLATSTR("http://odatae2etest.azurewebsites.net/cpptest/DefaultService/Products(5)"), product->get_edit_link());

	auto products2 = service_context->create_products_query()->key(_XPLATSTR("5"))->select(_XPLATSTR("*"))->execute_query().get();
	VERIFY_ARE_EQUAL(1, products2.size());
	std::shared_ptr<Product> product2 = products2[0];
	VERIFY_ARE_EQUAL(_XPLATSTR("http://odatae2etest.azurewebsites.net/cpptest/DefaultService/Products(5)"), product2->get_edit_link());
}

TEST_FIXTURE(e2e_test_case, expand_option)
{
	auto products = service_context->create_products_query()->key(_XPLATSTR("5"))->expand(_XPLATSTR("Details"))->execute_query().get();
	VERIFY_ARE_EQUAL(1, products.size());
	std::shared_ptr<Product> product = products[0];

	auto details = product->get_details();
	VERIFY_ARE_EQUAL(5, details.size());
	VERIFY_ARE_EQUAL(2, details[0]->get_productdetailid());
	VERIFY_ARE_EQUAL(_XPLATSTR("http://odatae2etest.azurewebsites.net/cpptest/DefaultService/ProductDetails(ProductID=5,ProductDetailID=2)"), details[0]->get_edit_link());

	auto people = service_context->create_people_query()->key(_XPLATSTR("1"))->expand(_XPLATSTR("Parent"))->execute_query().get();
	VERIFY_ARE_EQUAL(1, people.size());
	auto person = people[0];

	auto parent = person->get_parent();
	VERIFY_ARE_EQUAL(_XPLATSTR("http://odatae2etest.azurewebsites.net/cpptest/DefaultService/People(2)/Microsoft.Test.OData.Services.ODataWCFService.Customer"), parent->get_edit_link());
	VERIFY_ARE_EQUAL(_XPLATSTR("Jill"), parent->get_firstname());
}

TEST_FIXTURE(e2e_test_case, mixed_query_options)
{
	auto accounts = service_context->create_accounts_query()->key(_XPLATSTR("103"))
		->expand(odata_query_path::creat_query_path(_XPLATSTR("MyPaymentInstruments"))->select(_XPLATSTR("PaymentInstrumentID,FriendlyName"))->orderby(_XPLATSTR("FriendlyName")))
		->execute_query().get();
	VERIFY_ARE_EQUAL(1, accounts.size());

	auto account = accounts[0];
	auto pis = account->get_mypaymentinstruments();
	VERIFY_ARE_EQUAL(4, pis.size());
	VERIFY_ARE_EQUAL(101910, pis[0]->get_paymentinstrumentid());
	VERIFY_ARE_EQUAL(_XPLATSTR("103 backup PI"), pis[0]->get_friendlyname());
	VERIFY_ARE_EQUAL(103901, pis[1]->get_paymentinstrumentid());
	VERIFY_ARE_EQUAL(_XPLATSTR("103 frist PI"), pis[1]->get_friendlyname());
}

}

}}}
