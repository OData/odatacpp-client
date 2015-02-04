//---------------------------------------------------------------------
// <copyright file="edit_link_tests.cpp" company="Microsoft">
//      Copyright (C) Microsoft Corporation. All rights reserved. See License.txt in the project root for license information.
// </copyright>
//---------------------------------------------------------------------

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