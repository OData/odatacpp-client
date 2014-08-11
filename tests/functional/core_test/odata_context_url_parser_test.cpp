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
#include "odata/core/odata_core.h"
#include "odata/core/odata_context_url_parser.h"
#include "odata/edm/edm_model_utility.h"

using namespace ::odata::core;
using namespace ::odata::edm;

namespace tests { namespace functional { namespace _odata {

SUITE(odata_context_url_parser)
{

TEST(single_entity_test)
{
	auto model = get_test_model();
	VERIFY_IS_NOT_NULL(model);
	auto context_url_parser = std::make_shared<odata_contex_url_parser>(model, g_service_root_url);
	VERIFY_IS_NOT_NULL(context_url_parser);
	::utility::string_t single_entity_string = U("http://odatae2etest.azurewebsites.net/cpptest/DefaultService/$metadata#Accounts/$entity");
	auto return_type = context_url_parser->get_payload_content_type(single_entity_string);
	VERIFY_IS_NOT_NULL(return_type);
	VERIFY_ARE_EQUAL(return_type->get_type_kind(), edm_type_kind_t::Entity);
	VERIFY_ARE_EQUAL(return_type->get_name(), U("Account"));
}

TEST(entities_test)
{
	auto model = get_test_model();
	VERIFY_IS_NOT_NULL(model);
	auto context_url_parser = std::make_shared<odata_contex_url_parser>(model, g_service_root_url);
	VERIFY_IS_NOT_NULL(context_url_parser);
	::utility::string_t _entities_string = U("http://odatae2etest.azurewebsites.net/cpptest/DefaultService/$metadata#Accounts");
	auto return_type = context_url_parser->get_payload_content_type(_entities_string);
	VERIFY_IS_NOT_NULL(return_type);
	VERIFY_ARE_EQUAL(return_type->get_type_kind(), edm_type_kind_t::Entity);
	VERIFY_ARE_EQUAL(return_type->get_name(), U("Account"));
}

TEST(derived_entity)
{
	auto model = get_test_model();
	VERIFY_IS_NOT_NULL(model);
	auto context_url_parser = std::make_shared<odata_contex_url_parser>(model, g_service_root_url);
	VERIFY_IS_NOT_NULL(context_url_parser);
	::utility::string_t _entities_string = U("http://odatae2etest.azurewebsites.net/cpptest/DefaultService/$metadata#People/Microsoft.Test.OData.Services.ODataWCFService.Customer/$entity");
	auto return_type = context_url_parser->get_payload_content_type(_entities_string);
	VERIFY_IS_NOT_NULL(return_type);
	VERIFY_ARE_EQUAL(return_type->get_type_kind(), edm_type_kind_t::Entity);
	VERIFY_ARE_EQUAL(return_type->get_name(), U("Customer"));
}

TEST(project_entity)
{
	auto model = get_test_model();
	VERIFY_IS_NOT_NULL(model);
	auto context_url_parser = std::make_shared<odata_contex_url_parser>(model, g_service_root_url);
	VERIFY_IS_NOT_NULL(context_url_parser);
	::utility::string_t _entities_string = U("http://odatae2etest.azurewebsites.net/cpptest/DefaultService/$metadata#Customers(PersonID,Orders)/$entity");
	auto return_type = context_url_parser->get_payload_content_type(_entities_string);
	VERIFY_IS_NOT_NULL(return_type);
	VERIFY_ARE_EQUAL(return_type->get_type_kind(), edm_type_kind_t::Entity);
	VERIFY_ARE_EQUAL(return_type->get_name(), U("Customer"));
}

TEST(collection_project_entities)
{
	auto model = get_test_model();
	VERIFY_IS_NOT_NULL(model);
	auto context_url_parser = std::make_shared<odata_contex_url_parser>(model, g_service_root_url);
	VERIFY_IS_NOT_NULL(context_url_parser);
	::utility::string_t _entities_string = U("http://odatae2etest.azurewebsites.net/cpptest/DefaultService/$metadata#Customers(PersonID,Orders)");
	auto return_type = context_url_parser->get_payload_content_type(_entities_string);
	VERIFY_IS_NOT_NULL(return_type);
	VERIFY_ARE_EQUAL(return_type->get_type_kind(), edm_type_kind_t::Entity);
	VERIFY_ARE_EQUAL(return_type->get_name(), U("Customer"));
}

TEST(collection_project_expand_entity)
{
	auto model = get_test_model();
	VERIFY_IS_NOT_NULL(model);
	auto context_url_parser = std::make_shared<odata_contex_url_parser>(model, g_service_root_url);
	VERIFY_IS_NOT_NULL(context_url_parser);
	::utility::string_t _entities_string = U("http://odatae2etest.azurewebsites.net/cpptest/DefaultService/$metadata#Customers(PersonID,Orders,Orders(OrderID,OrderDate))/$entity");
	auto return_type = context_url_parser->get_payload_content_type(_entities_string);
	VERIFY_IS_NOT_NULL(return_type);
	VERIFY_ARE_EQUAL(return_type->get_type_kind(), edm_type_kind_t::Entity);
	VERIFY_ARE_EQUAL(return_type->get_name(), U("Customer"));
}

TEST(collection_project_expand_entities)
{
	auto model = get_test_model();
	VERIFY_IS_NOT_NULL(model);
	auto context_url_parser = std::make_shared<odata_contex_url_parser>(model, g_service_root_url);
	VERIFY_IS_NOT_NULL(context_url_parser);
	::utility::string_t _entities_string = U("http://odatae2etest.azurewebsites.net/cpptest/DefaultService/$metadata#Customers(PersonID,Orders,Orders(OrderID,OrderDate))");
	auto return_type = context_url_parser->get_payload_content_type(_entities_string);
	VERIFY_IS_NOT_NULL(return_type);
	VERIFY_ARE_EQUAL(return_type->get_type_kind(), edm_type_kind_t::Entity);
	VERIFY_ARE_EQUAL(return_type->get_name(), U("Customer"));
}

TEST(single_complex_test)
{
	auto model = get_test_model();
	VERIFY_IS_NOT_NULL(model);
	auto context_url_parser = std::make_shared<odata_contex_url_parser>(model, g_service_root_url);
	VERIFY_IS_NOT_NULL(context_url_parser);
	::utility::string_t _single_complex = U("http://odatae2etest.azurewebsites.net/cpptest/DefaultService/$metadata#Accounts(102)/AccountInfo");
	auto return_type = context_url_parser->get_payload_content_type(_single_complex);
	VERIFY_IS_NOT_NULL(return_type);
	VERIFY_ARE_EQUAL(return_type->get_type_kind(), edm_type_kind_t::Complex);
	VERIFY_ARE_EQUAL(return_type->get_name(), U("AccountInfo"));
}

TEST(single_complex_test_2)
{
	auto model = get_test_model();
	VERIFY_IS_NOT_NULL(model);
	auto context_url_parser = std::make_shared<odata_contex_url_parser>(model, g_service_root_url);
	VERIFY_IS_NOT_NULL(context_url_parser);
	::utility::string_t _single_complex = U("http://odatae2etest.azurewebsites.net/cpptest/DefaultService/$metadata#Microsoft.Test.OData.Services.ODataWCFService.HomeAddress");
	auto return_type = context_url_parser->get_payload_content_type(_single_complex);
	VERIFY_IS_NOT_NULL(return_type);
	VERIFY_ARE_EQUAL(return_type->get_type_kind(), edm_type_kind_t::Complex);
	VERIFY_ARE_EQUAL(return_type->get_name(), U("HomeAddress"));
}

TEST(derived_complex_test)
{
	auto model = get_test_model();
	VERIFY_IS_NOT_NULL(model);
	auto context_url_parser = std::make_shared<odata_contex_url_parser>(model, g_service_root_url);
	VERIFY_IS_NOT_NULL(context_url_parser);
	::utility::string_t _single_complex = U("http://odatae2etest.azurewebsites.net/cpptest/DefaultService/$metadata#People(1)/HomeAddress/Microsoft.Test.OData.Services.ODataWCFService.HomeAddress");
	auto return_type = context_url_parser->get_payload_content_type(_single_complex);
	VERIFY_IS_NOT_NULL(return_type);
	VERIFY_ARE_EQUAL(return_type->get_type_kind(), edm_type_kind_t::Complex);
	VERIFY_ARE_EQUAL(return_type->get_name(), U("HomeAddress"));
}

TEST(single_primitive_test_double)
{
	auto model = get_test_model();
	VERIFY_IS_NOT_NULL(model);
	auto context_url_parser = std::make_shared<odata_contex_url_parser>(model, g_service_root_url);
	VERIFY_IS_NOT_NULL(context_url_parser);

	::utility::string_t _single_primitive = U("http://odatae2etest.azurewebsites.net/cpptest/DefaultService/$metadata#Edm.Double");
	auto return_type = context_url_parser->get_payload_content_type(_single_primitive);
	VERIFY_IS_NOT_NULL(return_type);
	VERIFY_ARE_EQUAL(return_type->get_type_kind(), edm_type_kind_t::Primitive);
	VERIFY_ARE_EQUAL(return_type->get_name(), U("Edm.Double"));
}

TEST(single_primitive_test_binary)
{
	auto model = get_test_model();
	VERIFY_IS_NOT_NULL(model);
	auto context_url_parser = std::make_shared<odata_contex_url_parser>(model, g_service_root_url);
	VERIFY_IS_NOT_NULL(context_url_parser);

	::utility::string_t _single_primitive = U("http://odatae2etest.azurewebsites.net/cpptest/DefaultService/$metadata#Edm.Binary");
	auto return_type = context_url_parser->get_payload_content_type(_single_primitive);
	VERIFY_IS_NOT_NULL(return_type);
	VERIFY_ARE_EQUAL(return_type->get_type_kind(), edm_type_kind_t::Primitive);
	VERIFY_ARE_EQUAL(return_type->get_name(), U("Edm.Binary"));
}

TEST(collection_of_primitive_test)
{
	auto model = get_test_model();
	VERIFY_IS_NOT_NULL(model);
	auto context_url_parser = std::make_shared<odata_contex_url_parser>(model, g_service_root_url);
	VERIFY_IS_NOT_NULL(context_url_parser);
	::utility::string_t _multiple_primitive = U("http://odatae2etest.azurewebsites.net/cpptest/DefaultService/$metadata#People(2)/Numbers");
	auto return_type = context_url_parser->get_payload_content_type(_multiple_primitive);
	VERIFY_IS_NOT_NULL(return_type);
	VERIFY_ARE_EQUAL(return_type->get_type_kind(), edm_type_kind_t::Collection);
	auto element_type = edm_model_utility::get_collection_element_type(return_type);
	VERIFY_ARE_EQUAL(element_type->get_type_kind(), edm_type_kind_t::Primitive);
	VERIFY_ARE_EQUAL(element_type->get_name(), U("Edm.String"));
}

TEST(collection_of_primitive_test_string)
{
	auto model = get_test_model();
	VERIFY_IS_NOT_NULL(model);
	auto context_url_parser = std::make_shared<odata_contex_url_parser>(model, g_service_root_url);
	VERIFY_IS_NOT_NULL(context_url_parser);
	::utility::string_t _multiple_primitive = U("http://odatae2etest.azurewebsites.net/cpptest/DefaultService/$metadata#Collection(Edm.String)");
	auto return_type = context_url_parser->get_payload_content_type(_multiple_primitive);
	VERIFY_IS_NOT_NULL(return_type);
	VERIFY_ARE_EQUAL(return_type->get_type_kind(), edm_type_kind_t::Collection);
	auto element_type = edm_model_utility::get_collection_element_type(return_type);
	VERIFY_ARE_EQUAL(element_type->get_type_kind(), edm_type_kind_t::Primitive);
	VERIFY_ARE_EQUAL(element_type->get_name(), U("Edm.String"));
}

TEST(collection_of_primitive_test_int32)
{
	auto model = get_test_model();
	VERIFY_IS_NOT_NULL(model);
	auto context_url_parser = std::make_shared<odata_contex_url_parser>(model, g_service_root_url);
	VERIFY_IS_NOT_NULL(context_url_parser);
	::utility::string_t _multiple_primitive = U("http://odatae2etest.azurewebsites.net/cpptest/DefaultService/$metadata#Collection(Edm.Int32)");
	auto return_type = context_url_parser->get_payload_content_type(_multiple_primitive);
	VERIFY_IS_NOT_NULL(return_type);
	VERIFY_ARE_EQUAL(return_type->get_type_kind(), edm_type_kind_t::Collection);
	auto element_type = edm_model_utility::get_collection_element_type(return_type);
	VERIFY_ARE_EQUAL(element_type->get_type_kind(), edm_type_kind_t::Primitive);
	VERIFY_ARE_EQUAL(element_type->get_name(), U("Edm.Int32"));
}

TEST(collection_of_primitive_test_single)
{
	auto model = get_test_model();
	VERIFY_IS_NOT_NULL(model);
	auto context_url_parser = std::make_shared<odata_contex_url_parser>(model, g_service_root_url);
	VERIFY_IS_NOT_NULL(context_url_parser);
	::utility::string_t _multiple_primitive = U("http://odatae2etest.azurewebsites.net/cpptest/DefaultService/$metadata#Collection(Edm.Single)");
	auto return_type = context_url_parser->get_payload_content_type(_multiple_primitive);
	VERIFY_IS_NOT_NULL(return_type);
	VERIFY_ARE_EQUAL(return_type->get_type_kind(), edm_type_kind_t::Collection);
	auto element_type = edm_model_utility::get_collection_element_type(return_type);
	VERIFY_ARE_EQUAL(element_type->get_type_kind(), edm_type_kind_t::Primitive);
	VERIFY_ARE_EQUAL(element_type->get_name(), U("Edm.Single"));
}

TEST(single_enum_test)
{
	auto model = get_test_model();
	VERIFY_IS_NOT_NULL(model);
	auto context_url_parser = std::make_shared<odata_contex_url_parser>(model, g_service_root_url);
	VERIFY_IS_NOT_NULL(context_url_parser);
	::utility::string_t _text_url = U("http://odatae2etest.azurewebsites.net/cpptest/DefaultService/$metadata#Products(5)/SkinColor");
	auto return_type = context_url_parser->get_payload_content_type(_text_url);
	VERIFY_IS_NOT_NULL(return_type);
	VERIFY_ARE_EQUAL(return_type->get_type_kind(), edm_type_kind_t::Enum);
}

TEST(collection_of_enum_test)
{
	auto model = get_test_model();
	VERIFY_IS_NOT_NULL(model);
	auto context_url_parser = std::make_shared<odata_contex_url_parser>(model, g_service_root_url);
	VERIFY_IS_NOT_NULL(context_url_parser);
	::utility::string_t _text_url = U("http://odatae2etest.azurewebsites.net/cpptest/DefaultService/$metadata#Products(5)/CoverColors");
	auto return_type = context_url_parser->get_payload_content_type(_text_url);
	VERIFY_IS_NOT_NULL(return_type);
	VERIFY_ARE_EQUAL(return_type->get_type_kind(), edm_type_kind_t::Collection);
	auto element_type = edm_model_utility::get_collection_element_type(return_type);
	VERIFY_ARE_EQUAL(element_type->get_type_kind(), edm_type_kind_t::Enum);
}

TEST(single_entity_with_single_navigation_test)
{
	auto model = get_test_model();
	VERIFY_IS_NOT_NULL(model);
	auto context_url_parser = std::make_shared<odata_contex_url_parser>(model, g_service_root_url);
	VERIFY_IS_NOT_NULL(context_url_parser);
	::utility::string_t _text_url = U("http://odatae2etest.azurewebsites.net/cpptest/DefaultService/$metadata#Accounts(101)/MyGiftCard/$entity");
	auto return_type = context_url_parser->get_payload_content_type(_text_url);
	VERIFY_IS_NOT_NULL(return_type);
	VERIFY_ARE_EQUAL(return_type->get_type_kind(), edm_type_kind_t::Navigation);
	auto element_type = edm_model_utility::get_navigation_element_type(return_type);
	VERIFY_ARE_EQUAL(element_type->get_type_kind(), edm_type_kind_t::Entity);
}

TEST(single_entity_with_collection_of_navigation_test)
{
	auto model = get_test_model();
	VERIFY_IS_NOT_NULL(model);
	auto context_url_parser = std::make_shared<odata_contex_url_parser>(model, g_service_root_url);
	VERIFY_IS_NOT_NULL(context_url_parser);
	::utility::string_t _text_url = U("http://odatae2etest.azurewebsites.net/cpptest/DefaultService/$metadata#Accounts(101)/MyPaymentInstruments");
	auto return_type = context_url_parser->get_payload_content_type(_text_url);
	VERIFY_IS_NOT_NULL(return_type);
	VERIFY_ARE_EQUAL(return_type->get_type_kind(), edm_type_kind_t::Navigation);
	auto element_type = edm_model_utility::get_navigation_element_type(return_type);
	VERIFY_ARE_EQUAL(element_type->get_type_kind(), edm_type_kind_t::Collection);
	auto collection_element_type = edm_model_utility::get_collection_element_type(element_type);
	VERIFY_ARE_EQUAL(collection_element_type->get_type_kind(), edm_type_kind_t::Entity);
}

TEST(single_entity_with_collection_of_navigation_and_select_test)
{
	auto model = get_test_model();
	VERIFY_IS_NOT_NULL(model);
	auto context_url_parser = std::make_shared<odata_contex_url_parser>(model, g_service_root_url);
	VERIFY_IS_NOT_NULL(context_url_parser);
	::utility::string_t _text_url = U("http://odatae2etest.azurewebsites.net/cpptest/DefaultService/$metadata#Accounts(101)/MyPaymentInstruments(PaymentInstrumentID,FriendlyName)");
	auto return_type = context_url_parser->get_payload_content_type(_text_url);
	VERIFY_IS_NOT_NULL(return_type);
	VERIFY_ARE_EQUAL(return_type->get_type_kind(), edm_type_kind_t::Navigation);
	auto element_type = edm_model_utility::get_navigation_element_type(return_type);
	VERIFY_ARE_EQUAL(element_type->get_type_kind(), edm_type_kind_t::Collection);
	auto collection_element_type = edm_model_utility::get_collection_element_type(element_type);
	VERIFY_ARE_EQUAL(collection_element_type->get_type_kind(), edm_type_kind_t::Entity);
}

TEST(navigation_of_navigation_test)
{
	auto model = get_test_model();
	VERIFY_IS_NOT_NULL(model);
	auto context_url_parser = std::make_shared<odata_contex_url_parser>(model, g_service_root_url);
	VERIFY_IS_NOT_NULL(context_url_parser);
	::utility::string_t _text_url = U("http://odatae2etest.azurewebsites.net/cpptest/DefaultService/$metadata#Accounts(101)/MyPaymentInstruments(101901)/BillingStatements");
	auto return_type = context_url_parser->get_payload_content_type(_text_url);
	VERIFY_IS_NOT_NULL(return_type);
	VERIFY_ARE_EQUAL(return_type->get_type_kind(), edm_type_kind_t::Navigation);
	auto element_type = edm_model_utility::get_navigation_element_type(return_type);
	VERIFY_ARE_EQUAL(element_type->get_type_kind(), edm_type_kind_t::Collection);
	auto collection_element_type = edm_model_utility::get_collection_element_type(element_type);
	VERIFY_ARE_EQUAL(collection_element_type->get_type_kind(), edm_type_kind_t::Entity);
}

}

}}}