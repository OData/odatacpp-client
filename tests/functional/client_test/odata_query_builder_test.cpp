//---------------------------------------------------------------------
// <copyright file="odata_query_builder_test.cpp" company="Microsoft">
//      Copyright (C) Microsoft Corporation. All rights reserved. See License.txt in the project root for license information.
// </copyright>
//---------------------------------------------------------------------

#include "../odata_tests.h"
#include "cpprest/json.h"
#include "odata/edm/odata_edm.h"
#include "odata/codegen/odata_query_builder.h"

using namespace ::odata::edm;
using namespace ::odata::core;
using namespace ::odata::client;
using namespace ::odata::codegen;
using namespace ::odata::common;

namespace tests { namespace functional { namespace _odata {

SUITE(odata_query_builder_test_cases)
{

TEST(no_query_test)
{
	auto query_builder = ::odata::make_shared<odata_query_builder>(_XPLATSTR("Accounts"));
	::odata::string_t expression = query_builder->get_query_expression();
	VERIFY_ARE_EQUAL(expression, _XPLATSTR("Accounts"));
}

TEST(key_test)
{
	auto query_builder = ::odata::make_shared<odata_query_builder>(_XPLATSTR("Accounts"));
	query_builder->key(_XPLATSTR("101"));
	::odata::string_t expression = query_builder->get_query_expression();
	VERIFY_ARE_EQUAL(expression, _XPLATSTR("Accounts(101)"));
}

TEST(top_test)
{
	auto query_builder = ::odata::make_shared<odata_query_builder>(_XPLATSTR("Accounts"));
	query_builder->top(10);
	::odata::string_t expression = query_builder->get_query_expression();
	VERIFY_ARE_EQUAL(expression, _XPLATSTR("Accounts?$top=10"));
}

TEST(orderby_test)
{
	auto query_builder = ::odata::make_shared<odata_query_builder>(_XPLATSTR("Accounts"));
	query_builder->orderby(_XPLATSTR("Name desc, Gender asc"));
	::odata::string_t expression = query_builder->get_query_expression();
	VERIFY_ARE_EQUAL(expression, _XPLATSTR("Accounts?$orderby=Name%20desc,%20Gender%20asc"));
}

TEST(select_test)
{
	auto query_builder = ::odata::make_shared<odata_query_builder>(_XPLATSTR("Accounts"));
	query_builder->select(_XPLATSTR("Name,OrderDate"));
	::odata::string_t expression = query_builder->get_query_expression();
	VERIFY_ARE_EQUAL(expression, _XPLATSTR("Accounts?$select=Name,OrderDate"));
}

TEST(expand_test)
{
	auto query_builder = ::odata::make_shared<odata_query_builder>(_XPLATSTR("Accounts"));
	query_builder->expand(_XPLATSTR("MyGiftCard"));
	::odata::string_t expression = query_builder->get_query_expression();
	VERIFY_ARE_EQUAL(expression, _XPLATSTR("Accounts?$expand=MyGiftCard"));
}

TEST(double_expand_test)
{
	auto query_builder = ::odata::make_shared<odata_query_builder>(_XPLATSTR("Accounts(101)"));
	query_builder->expand(_XPLATSTR("MyGiftCard"));
	query_builder->expand(_XPLATSTR("MyPaymentInstruments"));
	::odata::string_t expression = query_builder->get_query_expression();
	VERIFY_ARE_EQUAL(expression, _XPLATSTR("Accounts(101)?$expand=MyPaymentInstruments,MyGiftCard"));
}

TEST(select_and_expand_test)
{
	auto query_builder = ::odata::make_shared<odata_query_builder>(_XPLATSTR("Accounts(101)"));
	query_builder->expand(_XPLATSTR("MyPaymentInstruments"));
	query_builder->expand(_XPLATSTR("MyGiftCard"));
	query_builder->select(_XPLATSTR("AccountID,CountryRegion"));
	::odata::string_t expression = query_builder->get_query_expression();
	VERIFY_ARE_EQUAL(expression, _XPLATSTR("Accounts(101)?$select=AccountID,CountryRegion&$expand=MyGiftCard,MyPaymentInstruments"));
}

TEST(only_nested_expand_test)
{
	auto query_builder = ::odata::make_shared<odata_query_builder>(_XPLATSTR("Accounts(101)"));
	query_builder->expand(_XPLATSTR("MyGiftCard"));
	query_builder->expand(odata_query_path::creat_query_path(_XPLATSTR("MyPaymentInstruments"))->expand(_XPLATSTR("TheStoredPI")));
	query_builder->select(_XPLATSTR("AccountID,CountryRegion"));
	::odata::string_t expression = query_builder->get_query_expression();
	VERIFY_ARE_EQUAL(expression, _XPLATSTR("Accounts(101)?$select=AccountID,CountryRegion&$expand=MyPaymentInstruments($expand=TheStoredPI),MyGiftCard"));
}

TEST(select_in_expand_test)
{
	auto query_builder = ::odata::make_shared<odata_query_builder>(_XPLATSTR("Accounts"));
	query_builder->expand(odata_query_path::creat_query_path(_XPLATSTR("MyPaymentInstruments"))->select(_XPLATSTR("PaymentInstrumentID,FriendlyName")));
	query_builder->select(_XPLATSTR("AccountID,CountryRegion"));
	::odata::string_t expression = query_builder->get_query_expression();
	VERIFY_ARE_EQUAL(expression, _XPLATSTR("Accounts?$select=AccountID,CountryRegion&$expand=MyPaymentInstruments($select=PaymentInstrumentID,FriendlyName)"));
}

TEST(expand_in_expand_test)
{
	auto query_builder = ::odata::make_shared<odata_query_builder>(_XPLATSTR("Accounts"));
	query_builder->expand(odata_query_path::creat_query_path(_XPLATSTR("MyPaymentInstruments"))->select(_XPLATSTR("PaymentInstrumentID,FriendlyName"))->expand(odata_query_path::creat_query_path(_XPLATSTR("TheStoredPI"))));
	query_builder->select(_XPLATSTR("AccountID,CountryRegion"));
	::odata::string_t expression = query_builder->get_query_expression();
	VERIFY_ARE_EQUAL(expression, _XPLATSTR("Accounts?$select=AccountID,CountryRegion&$expand=MyPaymentInstruments($select=PaymentInstrumentID,FriendlyName;$expand=TheStoredPI)"));
}

TEST(three_layered_expand_test)
{
	auto query_builder = ::odata::make_shared<odata_query_builder>(_XPLATSTR("Customers"));
	query_builder->select(_XPLATSTR("PersonID"))->expand(
		odata_query_path::creat_query_path(_XPLATSTR("Orders"))->select(_XPLATSTR("OrderID"))->expand(
		odata_query_path::creat_query_path(_XPLATSTR("LoggedInEmployee"))->select(_XPLATSTR("PersonID"))->expand(_XPLATSTR("Company"))));
	::odata::string_t expression = query_builder->get_query_expression();
	VERIFY_ARE_EQUAL(expression, _XPLATSTR("Customers?$select=PersonID&$expand=Orders($select=OrderID;$expand=LoggedInEmployee($select=PersonID;$expand=Company))"));
}

}

}}}
