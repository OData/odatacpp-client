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
* query_option_tests.cpp
*
*
*
* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
****/

#include "e2e_tests.h"
#include "odata_wcf_service.h"


namespace tests { namespace e2e { namespace odata {

using namespace odata_wcf_service;


SUITE(query_option_tests_raw_client)
{

/***
TEST_FIXTURE(e2e_raw_client, filter_option)  //TODO-tiano: bug #2163372
{
	auto entities = client.get_data_from_server(U("Accounts?$filter=Country eq 'US'")).get();
	VERIFY_ARE_EQUAL(entities.size(), 2);
}
***/

TEST_FIXTURE(e2e_raw_client, expand_option)
{
	auto entities = client.get_data_from_server(U("Accounts(101)?$expand=MyGiftCard")).get();
	VERIFY_ARE_EQUAL(entities.size(), 1);

	//verify the expand
}

}


SUITE(query_option_tests)
{

TEST_FIXTURE(e2e_test_case, filter_option)
{
	auto accounts = service_context->create_accounts_query()->filter(U("Country eq 'US'"))->execute_query().get();
	VERIFY_ARE_EQUAL(2, accounts.size());
	for (auto iter = accounts.cbegin(); iter != accounts.cend(); iter++)
	{
		VERIFY_ARE_EQUAL(U("US"), (*iter)->get_country());
	}

	//filter on derived type property
	//filter on property of navigation proerty entity
}

TEST_FIXTURE(e2e_test_case, orderby_option) 
{
	auto accounts = service_context->create_accounts_query()->orderby(U("Country"))->execute_query().get();
	VERIFY_ARE_EQUAL(7, accounts.size());
	VERIFY_ARE_EQUAL(U("CN"), accounts[0]->get_country());
	VERIFY_ARE_EQUAL(U("FR"), accounts[3]->get_country());
	VERIFY_ARE_EQUAL(U("US"), accounts[5]->get_country());
}

TEST_FIXTURE(e2e_test_case, select_option) 
{
	auto products = service_context->create_products_query()->key(U("5"))->select(U("Name, UserAccess"))->execute_query().get();
	VERIFY_ARE_EQUAL(1, products.size());
	std::shared_ptr<Product> product = products[0];
	VERIFY_ARE_EQUAL(U("http://odatae2etest.azurewebsites.net/cpptest/DefaultService/Products(5)"), product->get_edit_link());
	
	auto products2 = service_context->create_products_query()->key(U("5"))->select(U("*"))->execute_query().get();
	VERIFY_ARE_EQUAL(1, products2.size());
	std::shared_ptr<Product> product2 = products2[0];
	VERIFY_ARE_EQUAL(U("http://odatae2etest.azurewebsites.net/cpptest/DefaultService/Products(5)"), product2->get_edit_link());
}

TEST_FIXTURE(e2e_test_case, expand_option) 
{
	auto products = service_context->create_products_query()->key(U("5"))->expand(U("Details"))->execute_query().get();
	VERIFY_ARE_EQUAL(1, products.size());
	std::shared_ptr<Product> product = products[0];

	auto details = product->get_details();
	VERIFY_ARE_EQUAL(5, details.size());
	VERIFY_ARE_EQUAL(2, details[0]->get_productdetailid());
	VERIFY_ARE_EQUAL(U("http://odatae2etest.azurewebsites.net/cpptest/DefaultService/ProductDetails(ProductID=5,ProductDetailID=2)"), details[0]->get_edit_link());

	auto people = service_context->create_people_query()->key(U("1"))->expand(U("Parent"))->execute_query().get();
	VERIFY_ARE_EQUAL(1, people.size());
	auto person = people[0];

	auto parent = person->get_parent();
	VERIFY_ARE_EQUAL(U("http://odatae2etest.azurewebsites.net/cpptest/DefaultService/People(2)/Microsoft.Test.OData.Services.ODataWCFService.Customer"), parent->get_edit_link());
	VERIFY_ARE_EQUAL(U("Jill"), parent->get_firstname());
}

TEST_FIXTURE(e2e_test_case, mixed_query_options) 
{
	auto accounts = service_context->create_accounts_query()->key(U("103"))
		->expand(odata_query_path::creat_query_path(U("MyPaymentInstruments"))->select(U("PaymentInstrumentID,FriendlyName"))->orderby(U("FriendlyName")))
		->execute_query().get();
	VERIFY_ARE_EQUAL(1, accounts.size());

	auto account = accounts[0];
	auto pis = account->get_mypaymentinstruments();
	VERIFY_ARE_EQUAL(4, pis.size());
	VERIFY_ARE_EQUAL(101910, pis[0]->get_paymentinstrumentid());
	VERIFY_ARE_EQUAL(U("103 backup PI"), pis[0]->get_friendlyname());
	VERIFY_ARE_EQUAL(103901, pis[1]->get_paymentinstrumentid());
	VERIFY_ARE_EQUAL(U("103 frist PI"), pis[1]->get_friendlyname());
}

}

}}}