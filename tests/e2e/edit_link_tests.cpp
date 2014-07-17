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
* singleton_tests.cpp
*
*
*
* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
****/

#include "e2e_tests.h"
#include "odata_wcf_service.h"

namespace tests { namespace e2e { namespace odata {

using namespace odata_wcf_service;

SUITE(edit_link_tests)
{


//http://services.odata.org/V4/Northwind/Northwind.svc/Products
TEST(query_entity_set)
{
	//auto norwind_service = std::make_shared<InMemoryEntities>(U("http://services.odata.org/V4/Northwind/Northwind.svc"));
	//auto ret = norwind_service->create_products_query()->top(3)->execute_query().get();
	
	//VERIFY_ARE_EQUAL(U("http://services.odata.org/V4/Northwind/Northwind.svc/Products(1)"), ret[0]->get_edit_link());
	//VERIFY_ARE_EQUAL(U("http://services.odata.org/V4/Northwind/Northwind.svc/Products(2)"), ret[1]->get_edit_link());
	//VERIFY_ARE_EQUAL(U("http://services.odata.org/V4/Northwind/Northwind.svc/Products(3)"), ret[2]->get_edit_link());
}

//http://services.odata.org/V4/Northwind/Northwind.svc/Products(1)
TEST(query_entity_set_with_key)
{
	//auto norwind_service = std::make_shared<InMemoryEntities>(U("http://services.odata.org/V4/Northwind/Northwind.svc"));
	//auto ret = norwind_service->create_products_query()->key(U("1"))->execute_query().get();

	//VERIFY_ARE_EQUAL(U("http://services.odata.org/V4/Northwind/Northwind.svc/Products(1)"), ret[0]->get_edit_link());
}

// todo we should find service that has not @editLink value
//http://odatae2etest.azurewebsites.net/cpptest/DefaultService/LabourUnion
//http://odatae2etest.azurewebsites.net/cpptest/DefaultService/Accounts?$expand=MyPaymentInstruments
//http://odatae2etest.azurewebsites.net/cpptest/DefaultService/Accounts(101)/MyPaymentInstruments
//http://odatae2etest.azurewebsites.net/cpptest/DefaultService/Accounts?$expand=MyGiftCard
//http://odatae2etest.azurewebsites.net/cpptest/DefaultService/Accounts(101)/MyGiftCard
//http://odatae2etest.azurewebsites.net/cpptest/DefaultService/Accounts(101)/MyPaymentInstruments?$expand=BillingStatements
//http://odatae2etest.azurewebsites.net/cpptest/DefaultService/Accounts(103)/MyPaymentInstruments(103901)/BillingStatements
//http://odatae2etest.azurewebsites.net/cpptest/DefaultService/Accounts(101)/MyPaymentInstruments?$expand=TheStoredPI
//http://odatae2etest.azurewebsites.net/cpptest/DefaultService/Accounts(101)/MyPaymentInstruments(101901)/TheStoredPI
}

}}}