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
 
 

#include "../odata_tests.h"
#include "cpprest/json.h"
#include "odata/edm/odata_edm.h"
#include "odata/codegen/odata_query_builder.h"

using namespace ::odata::edm;
using namespace ::odata::core;
using namespace ::odata::client;
using namespace ::odata::codegen;
using namespace ::odata::utility;

namespace tests { namespace functional { namespace _odata {

SUITE(odata_query_builder_test_cases)
{

TEST(no_query_test)
{
	auto query_builder = std::make_shared<odata_query_builder>(U("Accounts"));
	::utility::string_t expression = query_builder->get_query_expression();
	VERIFY_ARE_EQUAL(expression, U("Accounts"));
}

TEST(key_test)
{
	auto query_builder = std::make_shared<odata_query_builder>(U("Accounts"));
	query_builder->key(U("101"));
	::utility::string_t expression = query_builder->get_query_expression();
	VERIFY_ARE_EQUAL(expression, U("Accounts(101)"));
}

TEST(top_test)
{
	auto query_builder = std::make_shared<odata_query_builder>(U("Accounts"));
	query_builder->top(10);
	::utility::string_t expression = query_builder->get_query_expression();
	VERIFY_ARE_EQUAL(expression, U("Accounts?$top=10"));
}

TEST(orderby_test)
{
	auto query_builder = std::make_shared<odata_query_builder>(U("Accounts"));
	query_builder->orderby(U("Name desc, Gender asc"));
	::utility::string_t expression = query_builder->get_query_expression();
	VERIFY_ARE_EQUAL(expression, U("Accounts?$orderby=Name%20desc,%20Gender%20asc"));
}

TEST(select_test)
{
	auto query_builder = std::make_shared<odata_query_builder>(U("Accounts"));
	query_builder->select(U("Name,OrderDate"));
	::utility::string_t expression = query_builder->get_query_expression();
	VERIFY_ARE_EQUAL(expression, U("Accounts?$select=Name,OrderDate"));
}

TEST(expand_test)
{
	auto query_builder = std::make_shared<odata_query_builder>(U("Accounts"));
	query_builder->expand(U("MyGiftCard"));
	::utility::string_t expression = query_builder->get_query_expression();
	VERIFY_ARE_EQUAL(expression, U("Accounts?$expand=MyGiftCard"));
}

TEST(double_expand_test)
{
	auto query_builder = std::make_shared<odata_query_builder>(U("Accounts(101)"));
	query_builder->expand(U("MyGiftCard"));
	query_builder->expand(U("MyPaymentInstruments"));
	::utility::string_t expression = query_builder->get_query_expression();
	VERIFY_ARE_EQUAL(expression, U("Accounts(101)?$expand=MyPaymentInstruments,MyGiftCard"));
}

TEST(select_and_expand_test)
{
	auto query_builder = std::make_shared<odata_query_builder>(U("Accounts(101)"));
	query_builder->expand(U("MyPaymentInstruments"));
	query_builder->expand(U("MyGiftCard"));
	query_builder->select(U("AccountID,Country"));
	::utility::string_t expression = query_builder->get_query_expression();
	VERIFY_ARE_EQUAL(expression, U("Accounts(101)?$select=AccountID,Country&$expand=MyGiftCard,MyPaymentInstruments"));
}

TEST(only_nested_expand_test)
{
	auto query_builder = std::make_shared<odata_query_builder>(U("Accounts(101)"));
	query_builder->expand(U("MyGiftCard"));
	query_builder->expand(odata_query_path::creat_query_path(U("MyPaymentInstruments"))->expand(U("TheStoredPI")));
	query_builder->select(U("AccountID,Country"));
	::utility::string_t expression = query_builder->get_query_expression();
	VERIFY_ARE_EQUAL(expression, U("Accounts(101)?$select=AccountID,Country&$expand=MyPaymentInstruments($expand=TheStoredPI),MyGiftCard"));
}

TEST(select_in_expand_test)
{
	auto query_builder = std::make_shared<odata_query_builder>(U("Accounts"));
	query_builder->expand(odata_query_path::creat_query_path(U("MyPaymentInstruments"))->select(U("PaymentInstrumentID,FriendlyName")));
	query_builder->select(U("AccountID,Country"));
	::utility::string_t expression = query_builder->get_query_expression();
	VERIFY_ARE_EQUAL(expression, U("Accounts?$select=AccountID,Country&$expand=MyPaymentInstruments($select=PaymentInstrumentID,FriendlyName)"));
}

TEST(expand_in_expand_test)
{
	auto query_builder = std::make_shared<odata_query_builder>(U("Accounts"));
	query_builder->expand(odata_query_path::creat_query_path(U("MyPaymentInstruments"))->select(U("PaymentInstrumentID,FriendlyName"))->expand(odata_query_path::creat_query_path(U("TheStoredPI"))));
	query_builder->select(U("AccountID,Country"));
	::utility::string_t expression = query_builder->get_query_expression();
	VERIFY_ARE_EQUAL(expression, U("Accounts?$select=AccountID,Country&$expand=MyPaymentInstruments($select=PaymentInstrumentID,FriendlyName;$expand=TheStoredPI)"));
}

TEST(three_layered_expand_test)
{
	auto query_builder = std::make_shared<odata_query_builder>(U("Customers"));
	query_builder->select(U("PersonID"))->expand(
		odata_query_path::creat_query_path(U("Orders"))->select(U("OrderID"))->expand(
		odata_query_path::creat_query_path(U("LoggedInEmployee"))->select(U("PersonID"))->expand(U("Company"))));
	::utility::string_t expression = query_builder->get_query_expression();
	VERIFY_ARE_EQUAL(expression, U("Customers?$select=PersonID&$expand=Orders($select=OrderID;$expand=LoggedInEmployee($select=PersonID;$expand=Company))"));
}

}

}}}