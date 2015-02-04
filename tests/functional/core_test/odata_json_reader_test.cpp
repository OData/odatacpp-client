//---------------------------------------------------------------------
// <copyright file="odata_json_reader_test.cpp" company="Microsoft">
//      Copyright (C) Microsoft Corporation. All rights reserved. See License.txt in the project root for license information.
// </copyright>
//---------------------------------------------------------------------

#include "../odata_tests.h"
#include "cpprest/json.h"
#include "odata/core/odata_core.h"
#include "odata/core/odata_json_reader.h"

using namespace ::odata::core;
using namespace ::odata::edm;

namespace tests { namespace functional { namespace _odata {

static std::shared_ptr<odata_json_reader> get_json_reader()
{
	auto model = get_test_model();
	if (model)
	{
		return std::make_shared<odata_json_reader>(model, g_service_root_url);
	}

	return nullptr;
}

SUITE(odata_json_reader)
{

TEST(single_entity_test)
{
	auto json_reader = get_json_reader();
	VERIFY_IS_NOT_NULL(json_reader);

	::utility::string_t _entity_payload = U(
		"{\"@odata.context\":\"http://odatae2etest.azurewebsites.net/cpptest/DefaultService/$metadata#Accounts/$entity\", \
		\"@odata.editLink\":\"http://odatae2etest.azurewebsites.net/cpptest/DefaultService/Accounts(101)\", \
		\"AccountID\":101,\"CountryRegion\":\"US\",\"AccountInfo\":{\"FirstName\":\"Alex\",\"LastName\":\"Green\"}}"
		);

	web::json::value json_payload = web::json::value::parse(_entity_payload);
	auto return_payload = json_reader->deserilize(json_payload);
	VERIFY_IS_NOT_NULL(return_payload);
	auto entity_values = return_payload->get_values();
	VERIFY_ARE_EQUAL(entity_values.size(), 1);
	auto entity_value = std::dynamic_pointer_cast<odata_entity_value>(entity_values[0]);
	VERIFY_IS_NOT_NULL(entity_value);
	int account_id = 0;
	VERIFY_ARE_EQUAL(entity_value->try_get(U("AccountID"), account_id), true);
	VERIFY_ARE_EQUAL(account_id, 101);
	std::shared_ptr<odata_value> property_value;
	VERIFY_ARE_EQUAL(entity_value->try_get(U("AccountInfo"), property_value), true);
	VERIFY_IS_NOT_NULL(property_value);
	auto complex_value = std::dynamic_pointer_cast<odata_complex_value>(property_value);
	VERIFY_IS_NOT_NULL(complex_value);
	::utility::string_t first_name;
	VERIFY_ARE_EQUAL(complex_value->try_get(U("FirstName"), first_name), true);
	VERIFY_ARE_EQUAL(first_name, U("Alex"));
}

TEST(entities_test)
{
	auto json_reader = get_json_reader();
	VERIFY_IS_NOT_NULL(json_reader);

    ::utility::string_t _entity_payload = U(
	   "{\"@odata.context\":\"http://odatae2etest.azurewebsites.net/cpptest/DefaultService/$metadata#Departments\", \
		\"value\":[{\"@odata.id\":\"http://odatae2etest.azurewebsites.net/cpptest/DefaultService/Departments(1)\", \
		\"@odata.editLink\":\"http://odatae2etest.azurewebsites.net/cpptest/DefaultService/Departments(1)\", \
		\"DepartmentID\":1,\"Name\":\"D1\"},{\"@odata.id\":\"http://odatae2etest.azurewebsites.net/cpptest/DefaultService/Departments(2)\", \
		\"@odata.editLink\":\"http://odatae2etest.azurewebsites.net/cpptest/DefaultService/Departments(2)\", \
		\"DepartmentID\":2,\"Name\":\"D2\"}]}"
		);

	web::json::value json_payload = web::json::value::parse(_entity_payload);
	auto return_payload = json_reader->deserilize(json_payload);
	VERIFY_IS_NOT_NULL(return_payload);	
	auto entity_values = return_payload->get_values();
	VERIFY_ARE_EQUAL(entity_values.size(), 2);
	auto entity_value = std::dynamic_pointer_cast<odata_entity_value>(entity_values[1]);
	VERIFY_IS_NOT_NULL(entity_value);
	::utility::string_t name;
	VERIFY_ARE_EQUAL(entity_value->try_get(U("Name"), name), true);
	VERIFY_ARE_EQUAL(name, U("D2"));
	int32_t id;
	VERIFY_ARE_EQUAL(entity_value->try_get(U("DepartmentID"), id), true);
	VERIFY_ARE_EQUAL(id, 2);
}

TEST(derived_entity_test)
{
	auto json_reader = get_json_reader();
	VERIFY_IS_NOT_NULL(json_reader);

	::utility::string_t _entity_payload = U("{\"@odata.context\":\"http://odatae2etest.azurewebsites.net/cpptest/DefaultService/$metadata#People\", \
		\"value\":[{\"@odata.type\":\"#Microsoft.Test.OData.Services.ODataWCFService.Customer\",\"@odata.id\":\"http://odatae2etest.azurewebsites.net/cpptest/DefaultService/People(1)\",\"@odata.editLink\":\"http://odatae2etest.azurewebsites.net/cpptest/DefaultService/People(1)/Microsoft.Test.OData.Services.ODataWCFService.Customer\",\"PersonID\":1,\"FirstName\":\"Bob\",\"LastName\":\"Cat\",\"MiddleName\":null,\"HomeAddress\":{\"Street\":\"1 Microsoft Way\",\"City\":\"London\",\"PostalCode\":\"98052\"},\"Home\":{\"type\":\"Point\",\"coordinates\":[23.1,32.1],\"crs\":{\"type\":\"name\",\"properties\":{\"name\":\"EPSG:4326\"}}},\"Numbers\":[\"111-111-1111\"],\"Emails\":[\"abc@abc.com\"],\"City\":\"London\",\"Birthday\":\"1957-04-03T00:00:00Z\",\"TimeBetweenLastTwoOrders\":\"PT0.0000001S\"}, \
	{\"@odata.type\":\"#Microsoft.Test.OData.Services.ODataWCFService.Customer\",\"@odata.id\":\"http://odatae2etest.azurewebsites.net/cpptest/DefaultService/People(2)\",\"@odata.editLink\":\"http://odatae2etest.azurewebsites.net/cpptest/DefaultService/People(2)/Microsoft.Test.OData.Services.ODataWCFService.Customer\",\"PersonID\":2,\"FirstName\":\"Jill\",\"LastName\":\"Jones\",\"MiddleName\":null,\"HomeAddress\":null,\"Home\":{\"type\":\"Point\",\"coordinates\":[161.8,15.0],\"crs\":{\"type\":\"name\",\"properties\":{\"name\":\"EPSG:4326\"}}},\"Numbers\":[],\"Emails\":[],\"City\":\"Sydney\",\"Birthday\":\"1983-01-15T00:00:00Z\",\"TimeBetweenLastTwoOrders\":\"PT0.0000002S\"}, \
	{\"@odata.type\":\"#Microsoft.Test.OData.Services.ODataWCFService.Employee\",\"@odata.id\":\"http://odatae2etest.azurewebsites.net/cpptest/DefaultService/People(3)\",\"@odata.editLink\":\"http://odatae2etest.azurewebsites.net/cpptest/DefaultService/People(3)/Microsoft.Test.OData.Services.ODataWCFService.Employee\",\"PersonID\":3,\"FirstName\":\"Jacob\",\"LastName\":\"Zip\",\"MiddleName\":null,\"HomeAddress\":null,\"Home\":{\"type\":\"Point\",\"coordinates\":[161.8,15.0],\"crs\":{\"type\":\"name\",\"properties\":{\"name\":\"EPSG:4326\"}}},\"Numbers\":[\"333-333-3333\"],\"Emails\":[null],\"DateHired\":\"2010-12-13T00:00:00Z\",\"Office\":{\"type\":\"Point\",\"coordinates\":[162.0,15.0],\"crs\":{\"type\":\"name\",\"properties\":{\"name\":\"EPSG:4326\"}}}}, \
	{\"@odata.type\":\"#Microsoft.Test.OData.Services.ODataWCFService.Employee\",\"@odata.id\":\"http://odatae2etest.azurewebsites.net/cpptest/DefaultService/People(4)\",\"@odata.editLink\":\"http://odatae2etest.azurewebsites.net/cpptest/DefaultService/People(4)/Microsoft.Test.OData.Services.ODataWCFService.Employee\",\"PersonID\":4,\"FirstName\":\"Elmo\",\"LastName\":\"Rogers\",\"MiddleName\":null,\"HomeAddress\":null,\"Home\":{\"type\":\"Point\",\"coordinates\":[-61.8,-15.0],\"crs\":{\"type\":\"name\",\"properties\":{\"name\":\"EPSG:4326\"}}},\"Numbers\":[\"444-444-4444\",\"555-555-5555\",\"666-666-6666\"],\"Emails\":[\"def@def.org\",\"lmn@lmn.com\"],\"DateHired\":\"2008-03-27T00:00:00Z\",\"Office\":{\"type\":\"Point\",\"coordinates\":[-62.0,-15.0],\"crs\":{\"type\":\"name\",\"properties\":{\"name\":\"EPSG:4326\"}}}}, \
	{\"@odata.id\":\"http://odatae2etest.azurewebsites.net/cpptest/DefaultService/People(5)\",\"@odata.editLink\":\"http://odatae2etest.azurewebsites.net/cpptest/DefaultService/People(5)\",\"PersonID\":5,\"FirstName\":\"Peter\",\"LastName\":\"Bee\",\"MiddleName\":null,\"HomeAddress\":null,\"Home\":{\"type\":\"Point\",\"coordinates\":[-261.8,-16.0],\"crs\":{\"type\":\"name\",\"properties\":{\"name\":\"EPSG:4326\"}}},\"Numbers\":[\"555-555-5555\"],\"Emails\":[\"def@test.msn\"]}]}");

	web::json::value json_payload = web::json::value::parse(_entity_payload);
	auto return_values = json_reader->deserilize(json_payload)->get_values();
	VERIFY_ARE_EQUAL(return_values.size(), 5);

	auto entity_value_1 = std::dynamic_pointer_cast<odata_entity_value>(return_values[0]);
	VERIFY_IS_NOT_NULL(entity_value_1);
	auto entity_type_1 = std::dynamic_pointer_cast<edm_entity_type>(entity_value_1->get_value_type());
	VERIFY_IS_NOT_NULL(entity_type_1);
	VERIFY_ARE_EQUAL(entity_type_1->get_name(), U("Customer"));
	::utility::string_t city_1;
	VERIFY_ARE_EQUAL(entity_value_1->try_get(U("City"), city_1), true);
	VERIFY_ARE_EQUAL(city_1, U("London"));
	std::shared_ptr<odata_value> _value;
	entity_value_1->get_property_value(U("HomeAddress"), _value);
	auto complex_value_1 = std::dynamic_pointer_cast<odata_complex_value>(_value);
	VERIFY_IS_NOT_NULL(complex_value_1);
	::utility::string_t street_1;
	VERIFY_ARE_EQUAL(complex_value_1->try_get(U("City"), city_1), true);
	VERIFY_ARE_EQUAL(city_1, U("London"));
	VERIFY_ARE_EQUAL(complex_value_1->try_get(U("Street"), street_1), true);
	VERIFY_ARE_EQUAL(street_1, U("1 Microsoft Way"));

	auto entity_value_2 = std::dynamic_pointer_cast<odata_entity_value>(return_values[2]);
	VERIFY_IS_NOT_NULL(entity_value_2);
	auto entity_type_2 = std::dynamic_pointer_cast<edm_entity_type>(entity_value_2->get_value_type());
	VERIFY_IS_NOT_NULL(entity_type_2);
	VERIFY_ARE_EQUAL(entity_type_2->get_name(), U("Employee"));
	::utility::datetime dt;
	VERIFY_ARE_EQUAL(entity_value_2->try_get(U("DateHired"), dt), true);
	VERIFY_ARE_EQUAL(dt.to_string(::utility::datetime::date_format::ISO_8601), U("2010-12-13T00:00:00Z"));
}

TEST(single_complex_test)
{
	auto model = get_test_model();
	VERIFY_IS_NOT_NULL(model);
	auto json_reader = std::make_shared<odata_json_reader>(model, g_service_root_url);
	VERIFY_IS_NOT_NULL(json_reader);

    ::utility::string_t _entity_payload = U(
	   "{\"@odata.context\":\"http://odatae2etest.azurewebsites.net/cpptest/DefaultService/$metadata#Accounts(101)/AccountInfo\", \
		\"FirstName\":\"Alex\",\"LastName\":\"Green\"}"
		);

	web::json::value json_payload = web::json::value::parse(_entity_payload);
	auto return_payload = json_reader->deserilize(json_payload);
	VERIFY_IS_NOT_NULL(return_payload);	
	auto entity_values = return_payload->get_values();
	VERIFY_ARE_EQUAL(entity_values.size(), 1);
	auto complex_value = std::dynamic_pointer_cast<odata_complex_value>(entity_values[0]);
	VERIFY_IS_NOT_NULL(complex_value);
	::utility::string_t first_name, last_name;
	VERIFY_ARE_EQUAL(complex_value->try_get(U("FirstName"), first_name), true);
	VERIFY_ARE_EQUAL(first_name, U("Alex"));
	VERIFY_ARE_EQUAL(complex_value->try_get(U("LastName"), last_name), true);
	VERIFY_ARE_EQUAL(last_name, U("Green"));

}

TEST(derived_complex_test)
{
	auto model = get_test_model();
	VERIFY_IS_NOT_NULL(model);
	auto json_reader = std::make_shared<odata_json_reader>(model, g_service_root_url);
	VERIFY_IS_NOT_NULL(json_reader);

    ::utility::string_t _entity_payload = U(
	   "{\"@odata.context\":\"http://odatae2etest.azurewebsites.net/cpptest/DefaultService/$metadata#Accounts(101)/VIPAccountInfo\", \
		\"FirstName\":\"Alex\",\"LastName\":\"Green\",\"VIP_Info\":\"Gold\"}"
		);

	web::json::value json_payload = web::json::value::parse(_entity_payload);
	auto return_payload = json_reader->deserilize(json_payload);
	VERIFY_IS_NOT_NULL(return_payload);	
	auto entity_values = return_payload->get_values();
	VERIFY_ARE_EQUAL(entity_values.size(), 1);
	auto complex_value = std::dynamic_pointer_cast<odata_complex_value>(entity_values[0]);
	VERIFY_IS_NOT_NULL(complex_value);
	::utility::string_t first_name, last_name, vip_info;
	VERIFY_ARE_EQUAL(complex_value->try_get(U("FirstName"), first_name), true);
	VERIFY_ARE_EQUAL(first_name, U("Alex"));
	VERIFY_ARE_EQUAL(complex_value->try_get(U("LastName"), last_name), true);
	VERIFY_ARE_EQUAL(last_name, U("Green"));
	VERIFY_ARE_EQUAL(complex_value->try_get(U("VIP_Info"), vip_info), true);
	VERIFY_ARE_EQUAL(vip_info, U("Gold"));
}

TEST(derived_complex_test_2)
{
	auto model = get_test_model();
	VERIFY_IS_NOT_NULL(model);
	auto json_reader = std::make_shared<odata_json_reader>(model, g_service_root_url);
	VERIFY_IS_NOT_NULL(json_reader);

    ::utility::string_t _entity_payload = U(
	   "{\"@odata.context\":\"http://odatae2etest.azurewebsites.net/cpptest/DefaultService/$metadata#Accounts(101)/AccountInfo\", \
		\"@odata.type\":\"#Microsoft.Test.OData.Services.ODataWCFService.VIPAccountInfo\",\"FirstName\":\"Alex\",\"LastName\":\"Green\",\"VIP_Info\":\"Gold\"}"
		);

	web::json::value json_payload = web::json::value::parse(_entity_payload);
	auto return_payload = json_reader->deserilize(json_payload);
	VERIFY_IS_NOT_NULL(return_payload);	
	auto entity_values = return_payload->get_values();
	VERIFY_ARE_EQUAL(entity_values.size(), 1);
	auto complex_value = std::dynamic_pointer_cast<odata_complex_value>(entity_values[0]);
	VERIFY_IS_NOT_NULL(complex_value);
	::utility::string_t first_name, last_name, vip_info;
	VERIFY_ARE_EQUAL(complex_value->try_get(U("FirstName"), first_name), true);
	VERIFY_ARE_EQUAL(first_name, U("Alex"));
	VERIFY_ARE_EQUAL(complex_value->try_get(U("LastName"), last_name), true);
	VERIFY_ARE_EQUAL(last_name, U("Green"));
	VERIFY_ARE_EQUAL(complex_value->try_get(U("VIP_Info"), vip_info), true);
	VERIFY_ARE_EQUAL(vip_info, U("Gold"));
	auto complex_type = complex_value->get_value_type();
	VERIFY_IS_NOT_NULL(complex_type);
	VERIFY_ARE_EQUAL(complex_type->get_name(), U("VIPAccountInfo"));
}

TEST(single_primitive_test)
{
	auto json_reader = get_json_reader();
	VERIFY_IS_NOT_NULL(json_reader);

	::utility::string_t _entity_payload = U(
	   "{\"@odata.context\":\"http://odatae2etest.azurewebsites.net/cpptest/DefaultService/$metadata#Accounts(101)/AccountInfo/FirstName\", \
		\"value\":\"Alex\"}"
		);

 	web::json::value json_payload = web::json::value::parse(_entity_payload);
	auto return_payload = json_reader->deserilize(json_payload);
	VERIFY_IS_NOT_NULL(return_payload);	
	auto entity_values = return_payload->get_values();
	VERIFY_ARE_EQUAL(entity_values.size(), 1);   
	auto primitive_value = std::dynamic_pointer_cast<odata_primitive_value>(entity_values[0]);
	VERIFY_IS_NOT_NULL(primitive_value);
	VERIFY_ARE_EQUAL(primitive_value->get_value_type()->get_type_kind(), edm_type_kind_t::Primitive);
	VERIFY_ARE_EQUAL(primitive_value->to_string(), U("Alex"));
}

TEST(collection_of_primitive_test)
{
	auto json_reader = get_json_reader();
	VERIFY_IS_NOT_NULL(json_reader);

	::utility::string_t _entity_payload = U(
	    "{\"@odata.context\":\"http://odatae2etest.azurewebsites.net/cpptest/DefaultService/$metadata#People(4)/Numbers\", \
		\"value\":[\"444-444-4444\",\"555-555-5555\",\"666-666-6666\"]}"
		);

 	web::json::value json_payload = web::json::value::parse(_entity_payload);
	auto return_payload = json_reader->deserilize(json_payload);
	VERIFY_IS_NOT_NULL(return_payload);	
	auto entity_values = return_payload->get_values();
	VERIFY_ARE_EQUAL(entity_values.size(), 3);
	auto primitive_value = std::dynamic_pointer_cast<odata_primitive_value>(entity_values[0]);
	VERIFY_IS_NOT_NULL(primitive_value);
	VERIFY_ARE_EQUAL(primitive_value->get_value_type()->get_type_kind(), edm_type_kind_t::Primitive);
	VERIFY_ARE_EQUAL(primitive_value->to_string(), U("444-444-4444"));
    primitive_value = std::dynamic_pointer_cast<odata_primitive_value>(entity_values[1]);
	VERIFY_IS_NOT_NULL(primitive_value);
	VERIFY_ARE_EQUAL(primitive_value->get_value_type()->get_type_kind(), edm_type_kind_t::Primitive);
	VERIFY_ARE_EQUAL(primitive_value->to_string(), U("555-555-5555"));
    primitive_value = std::dynamic_pointer_cast<odata_primitive_value>(entity_values[2]);
	VERIFY_IS_NOT_NULL(primitive_value);
	VERIFY_ARE_EQUAL(primitive_value->get_value_type()->get_type_kind(), edm_type_kind_t::Primitive);
	VERIFY_ARE_EQUAL(primitive_value->to_string(), U("666-666-6666"));
}

TEST(json_primitive_int16)
{
	auto json_reader = get_json_reader();
	VERIFY_IS_NOT_NULL(json_reader);

	::utility::string_t _payload = U("{\"@odata.context\":\"http://odatae2etest.azurewebsites.net/cpptest/DefaultService/$metadata#Products(5)/TestInt16\", \
		\"value\":32}");

 	web::json::value json_payload = web::json::value::parse(_payload);
	auto return_payload = json_reader->deserilize(json_payload);
	auto primitive_value = std::dynamic_pointer_cast<odata_primitive_value>(return_payload->get_values()[0]);
	auto primitive_type = std::dynamic_pointer_cast<edm_primitive_type>(primitive_value->get_value_type());
	VERIFY_ARE_EQUAL(primitive_type->get_primitive_kind(), edm_primitive_type_kind_t::Int16);
	VERIFY_ARE_EQUAL(primitive_type->get_type_kind(), edm_type_kind_t::Primitive);
	VERIFY_ARE_EQUAL(primitive_value->as<int16_t>(), 32);
}

TEST(json_primitive_int16_max)
{
	auto json_reader = get_json_reader();
	VERIFY_IS_NOT_NULL(json_reader);

	::utility::string_t _payload = U("{\"@odata.context\":\"http://odatae2etest.azurewebsites.net/cpptest/DefaultService/$metadata#Products(5)/TestInt16\", \
		\"value\":32767}");

 	web::json::value json_payload = web::json::value::parse(_payload);
	auto return_payload = json_reader->deserilize(json_payload);
	auto primitive_value = std::dynamic_pointer_cast<odata_primitive_value>(return_payload->get_values()[0]);
	auto primitive_type = std::dynamic_pointer_cast<edm_primitive_type>(primitive_value->get_value_type());
	VERIFY_ARE_EQUAL(primitive_type->get_primitive_kind(), edm_primitive_type_kind_t::Int16);
	VERIFY_ARE_EQUAL(primitive_type->get_type_kind(), edm_type_kind_t::Primitive);
	VERIFY_ARE_EQUAL(primitive_value->as<int16_t>(), 32767);
}

TEST(json_primitive_int16_min)
{
	auto json_reader = get_json_reader();
	VERIFY_IS_NOT_NULL(json_reader);

	::utility::string_t _payload = U("{\"@odata.context\":\"http://odatae2etest.azurewebsites.net/cpptest/DefaultService/$metadata#Products(5)/TestInt16\", \
		\"value\":-32768}");

 	web::json::value json_payload = web::json::value::parse(_payload);
	auto return_payload = json_reader->deserilize(json_payload);
	auto primitive_value = std::dynamic_pointer_cast<odata_primitive_value>(return_payload->get_values()[0]);
	auto primitive_type = std::dynamic_pointer_cast<edm_primitive_type>(primitive_value->get_value_type());
	VERIFY_ARE_EQUAL(primitive_type->get_primitive_kind(), edm_primitive_type_kind_t::Int16);
	VERIFY_ARE_EQUAL(primitive_type->get_type_kind(), edm_type_kind_t::Primitive);
	VERIFY_ARE_EQUAL(primitive_value->as<int16_t>(), -32768);
}

TEST(json_primitive_int32)
{
	auto json_reader = get_json_reader();
	VERIFY_IS_NOT_NULL(json_reader);

	::utility::string_t _payload = U("{\"@odata.context\":\"http://odatae2etest.azurewebsites.net/cpptest/DefaultService/$metadata#Products(5)/ProductID\", \
		\"value\":5}");

 	web::json::value json_payload = web::json::value::parse(_payload);
	auto return_payload = json_reader->deserilize(json_payload);
	auto primitive_value = std::dynamic_pointer_cast<odata_primitive_value>(return_payload->get_values()[0]);
	auto primitive_type = std::dynamic_pointer_cast<edm_primitive_type>(primitive_value->get_value_type());
	VERIFY_ARE_EQUAL(primitive_type->get_primitive_kind(), edm_primitive_type_kind_t::Int32);
	VERIFY_ARE_EQUAL(primitive_type->get_type_kind(), edm_type_kind_t::Primitive);
	VERIFY_ARE_EQUAL(primitive_value->as<int32_t>(), 5);
}

TEST(json_primitive_int32_max)
{
	auto json_reader = get_json_reader();
	VERIFY_IS_NOT_NULL(json_reader);

	::utility::string_t _payload = U("{\"@odata.context\":\"http://odatae2etest.azurewebsites.net/cpptest/DefaultService/$metadata#Products(5)/ProductID\", \
		\"value\":2147483647}");

 	web::json::value json_payload = web::json::value::parse(_payload);
	auto return_payload = json_reader->deserilize(json_payload);
	auto primitive_value = std::dynamic_pointer_cast<odata_primitive_value>(return_payload->get_values()[0]);
	auto primitive_type = std::dynamic_pointer_cast<edm_primitive_type>(primitive_value->get_value_type());
	VERIFY_ARE_EQUAL(primitive_type->get_primitive_kind(), edm_primitive_type_kind_t::Int32);
	VERIFY_ARE_EQUAL(primitive_type->get_type_kind(), edm_type_kind_t::Primitive);
	VERIFY_ARE_EQUAL(primitive_value->as<int32_t>(), 2147483647);
}

TEST(json_primitive_int32_min)
{
	auto json_reader = get_json_reader();
	VERIFY_IS_NOT_NULL(json_reader);

	::utility::string_t _payload = U("{\"@odata.context\":\"http://odatae2etest.azurewebsites.net/cpptest/DefaultService/$metadata#Products(5)/ProductID\", \
		\"value\":-2147483648}");

 	web::json::value json_payload = web::json::value::parse(_payload);
	auto return_payload = json_reader->deserilize(json_payload);
	auto primitive_value = std::dynamic_pointer_cast<odata_primitive_value>(return_payload->get_values()[0]);
	auto primitive_type = std::dynamic_pointer_cast<edm_primitive_type>(primitive_value->get_value_type());
	VERIFY_ARE_EQUAL(primitive_type->get_primitive_kind(), edm_primitive_type_kind_t::Int32);
	VERIFY_ARE_EQUAL(primitive_type->get_type_kind(), edm_type_kind_t::Primitive);
	VERIFY_ARE_EQUAL(primitive_value->as<int32_t>(), -2147483648);
}

TEST(json_primitive_int64)
{
	auto json_reader = get_json_reader();
	VERIFY_IS_NOT_NULL(json_reader);

	::utility::string_t _payload = U("{\"@odata.context\":\"http://odatae2etest.azurewebsites.net/cpptest/DefaultService/$metadata#Company/Revenue\", \
		\"value\":100000}");

 	web::json::value json_payload = web::json::value::parse(_payload);
	auto return_payload = json_reader->deserilize(json_payload);
	auto primitive_value = std::dynamic_pointer_cast<odata_primitive_value>(return_payload->get_values()[0]);
	auto primitive_type = std::dynamic_pointer_cast<edm_primitive_type>(primitive_value->get_value_type());
	VERIFY_ARE_EQUAL(primitive_type->get_primitive_kind(), edm_primitive_type_kind_t::Int64);
	VERIFY_ARE_EQUAL(primitive_type->get_type_kind(), edm_type_kind_t::Primitive);
	VERIFY_ARE_EQUAL(primitive_value->as<int64_t>(), 100000);
}

TEST(json_primitive_int64_max)
{
	auto json_reader = get_json_reader();
	VERIFY_IS_NOT_NULL(json_reader);

	::utility::string_t _payload = U("{\"@odata.context\":\"http://odatae2etest.azurewebsites.net/cpptest/DefaultService/$metadata#Company/Revenue\", \
		\"value\":9223372036854775807}");

 	web::json::value json_payload = web::json::value::parse(_payload);
	auto return_payload = json_reader->deserilize(json_payload);
	auto primitive_value = std::dynamic_pointer_cast<odata_primitive_value>(return_payload->get_values()[0]);
	auto primitive_type = std::dynamic_pointer_cast<edm_primitive_type>(primitive_value->get_value_type());
	VERIFY_ARE_EQUAL(primitive_type->get_primitive_kind(), edm_primitive_type_kind_t::Int64);
	VERIFY_ARE_EQUAL(primitive_type->get_type_kind(), edm_type_kind_t::Primitive);
	VERIFY_ARE_EQUAL(primitive_value->as<int64_t>(), 9223372036854775807);
}

TEST(json_primitive_int64_min)
{
	auto json_reader = get_json_reader();
	VERIFY_IS_NOT_NULL(json_reader);

	::utility::string_t _payload = U("{\"@odata.context\":\"http://odatae2etest.azurewebsites.net/cpptest/DefaultService/$metadata#Company/Revenue\", \
		\"value\":-9223372036854775808}");

 	web::json::value json_payload = web::json::value::parse(_payload);
	auto return_payload = json_reader->deserilize(json_payload);
	auto primitive_value = std::dynamic_pointer_cast<odata_primitive_value>(return_payload->get_values()[0]);
	auto primitive_type = std::dynamic_pointer_cast<edm_primitive_type>(primitive_value->get_value_type());
	VERIFY_ARE_EQUAL(primitive_type->get_primitive_kind(), edm_primitive_type_kind_t::Int64);
	VERIFY_ARE_EQUAL(primitive_type->get_type_kind(), edm_type_kind_t::Primitive);
	VERIFY_ARE_EQUAL(primitive_value->as<int64_t>(), -9223372036854775808);
}

TEST(json_primitive_bool)
{
	auto json_reader = get_json_reader();
	VERIFY_IS_NOT_NULL(json_reader);

	::utility::string_t _payload = U("{\"@odata.context\":\"http://odatae2etest.azurewebsites.net/cpptest/DefaultService/$metadata#Products(5)/Discontinued\", \
		\"value\":false}");

 	web::json::value json_payload = web::json::value::parse(_payload);
	auto return_payload = json_reader->deserilize(json_payload);
	auto primitive_value = std::dynamic_pointer_cast<odata_primitive_value>(return_payload->get_values()[0]);
	auto primitive_type = std::dynamic_pointer_cast<edm_primitive_type>(primitive_value->get_value_type());
	VERIFY_ARE_EQUAL(primitive_type->get_primitive_kind(), edm_primitive_type_kind_t::Boolean);
	VERIFY_ARE_EQUAL(primitive_type->get_type_kind(), edm_type_kind_t::Primitive);
	VERIFY_ARE_EQUAL(primitive_value->as<bool>(), false);
}

TEST(json_primitive_binary)
{
	auto json_reader = get_json_reader();
	VERIFY_IS_NOT_NULL(json_reader);

	::utility::string_t _payload = U("{\"@odata.context\":\"http://odatae2etest.azurewebsites.net/cpptest/DefaultService/$metadata#Products(5)/TestBinary\", \
		\"value\":\"UUJBQkFDWDEyKjEyMzQ1Njc4OTBhYmNkZQ==\"}");

 	web::json::value json_payload = web::json::value::parse(_payload);
	auto return_payload = json_reader->deserilize(json_payload);
	auto primitive_value = std::dynamic_pointer_cast<odata_primitive_value>(return_payload->get_values()[0]);
	auto primitive_type = std::dynamic_pointer_cast<edm_primitive_type>(primitive_value->get_value_type());
	VERIFY_ARE_EQUAL(primitive_type->get_primitive_kind(), edm_primitive_type_kind_t::Binary);
	VERIFY_ARE_EQUAL(primitive_type->get_type_kind(), edm_type_kind_t::Primitive);
	auto binary_output = primitive_value->as<std::vector<unsigned char>>();
	VERIFY_ARE_EQUAL(binary_output.size(), 25);
	VERIFY_ARE_EQUAL(binary_output[0], 'Q');
	VERIFY_ARE_EQUAL(binary_output[7], '1');
}

TEST(json_primitive_sbyte)
{
	auto json_reader = get_json_reader();
	VERIFY_IS_NOT_NULL(json_reader);

	::utility::string_t _payload = U("{\"@odata.context\":\"http://odatae2etest.azurewebsites.net/cpptest/DefaultService/$metadata#Products(5)/TestSByte\", \
		\"value\":45}");

 	web::json::value json_payload = web::json::value::parse(_payload);
	auto return_payload = json_reader->deserilize(json_payload);
	auto primitive_value = std::dynamic_pointer_cast<odata_primitive_value>(return_payload->get_values()[0]);
	auto primitive_type = std::dynamic_pointer_cast<edm_primitive_type>(primitive_value->get_value_type());
	VERIFY_ARE_EQUAL(primitive_type->get_primitive_kind(), edm_primitive_type_kind_t::SByte);
	VERIFY_ARE_EQUAL(primitive_type->get_type_kind(), edm_type_kind_t::Primitive);
	VERIFY_ARE_EQUAL(primitive_value->as<unsigned char>(), 45);
}

TEST(json_primitive_byte)
{
	auto json_reader = get_json_reader();
	VERIFY_IS_NOT_NULL(json_reader);

	::utility::string_t _payload = U("{\"@odata.context\":\"http://odatae2etest.azurewebsites.net/cpptest/DefaultService/$metadata#Products(5)/TestByte\", \
		\"value\":-23}");

 	web::json::value json_payload = web::json::value::parse(_payload);
	auto return_payload = json_reader->deserilize(json_payload);
	auto primitive_value = std::dynamic_pointer_cast<odata_primitive_value>(return_payload->get_values()[0]);
	auto primitive_type = std::dynamic_pointer_cast<edm_primitive_type>(primitive_value->get_value_type());
	VERIFY_ARE_EQUAL(primitive_type->get_primitive_kind(), edm_primitive_type_kind_t::Byte);
	VERIFY_ARE_EQUAL(primitive_type->get_type_kind(), edm_type_kind_t::Primitive);
	VERIFY_ARE_EQUAL(primitive_value->as<char>(), -23);
}

TEST(json_primitive_double)
{
	auto json_reader = get_json_reader();
	VERIFY_IS_NOT_NULL(json_reader);

	::utility::string_t _payload = U("{\"@odata.context\":\"http://odatae2etest.azurewebsites.net/cpptest/DefaultService/$metadata#Accounts(101)/MyGiftCard/Amount\", \
		\"value\":123.3434}");

 	web::json::value json_payload = web::json::value::parse(_payload);
	auto return_payload = json_reader->deserilize(json_payload);
	auto primitive_value = std::dynamic_pointer_cast<odata_primitive_value>(return_payload->get_values()[0]);
	auto primitive_type = std::dynamic_pointer_cast<edm_primitive_type>(primitive_value->get_value_type());
	VERIFY_ARE_EQUAL(primitive_type->get_primitive_kind(), edm_primitive_type_kind_t::Double);
	VERIFY_ARE_EQUAL(primitive_type->get_type_kind(), edm_type_kind_t::Primitive);
	VERIFY_IS_TRUE(abs(primitive_value->as<double>() - 123.3434) < 0.000001);
}

TEST(json_primitive_float)
{
	auto json_reader = get_json_reader();
	VERIFY_IS_NOT_NULL(json_reader);

	::utility::string_t _payload = U("{\"@odata.context\":\"http://odatae2etest.azurewebsites.net/cpptest/DefaultService/$metadata#Products(5)/TestFloat\", \
		\"value\":-123.4}");

 	web::json::value json_payload = web::json::value::parse(_payload);
	auto return_payload = json_reader->deserilize(json_payload);
	auto primitive_value = std::dynamic_pointer_cast<odata_primitive_value>(return_payload->get_values()[0]);
	auto primitive_type = std::dynamic_pointer_cast<edm_primitive_type>(primitive_value->get_value_type());
	VERIFY_ARE_EQUAL(primitive_type->get_primitive_kind(), edm_primitive_type_kind_t::Single);
	VERIFY_ARE_EQUAL(primitive_type->get_type_kind(), edm_type_kind_t::Primitive);
	VERIFY_IS_TRUE(abs(primitive_value->as<float>() - -123.4) < 0.00001);
}

TEST(json_primitive_duration)
{
	auto json_reader = get_json_reader();
	VERIFY_IS_NOT_NULL(json_reader);

	::utility::string_t _payload = U("{\"@odata.context\":\"http://odatae2etest.azurewebsites.net/cpptest/DefaultService/$metadata#Customers(2)/TimeBetweenLastTwoOrders\", \
		\"value\":\"PT1H38M23S\"}");

 	web::json::value json_payload = web::json::value::parse(_payload);
	auto return_payload = json_reader->deserilize(json_payload);
	auto primitive_value = std::dynamic_pointer_cast<odata_primitive_value>(return_payload->get_values()[0]);
	auto primitive_type = std::dynamic_pointer_cast<edm_primitive_type>(primitive_value->get_value_type());
	VERIFY_ARE_EQUAL(primitive_type->get_primitive_kind(), edm_primitive_type_kind_t::Duration);
	VERIFY_ARE_EQUAL(primitive_type->get_type_kind(), edm_type_kind_t::Primitive);
	VERIFY_ARE_EQUAL(primitive_value->as<::utility::seconds>().count(), 5903);
}

TEST(json_primitive_datetime)
{
	auto json_reader = get_json_reader();
	VERIFY_IS_NOT_NULL(json_reader);

	::utility::string_t _payload = U("{\"@odata.context\":\"http://odatae2etest.azurewebsites.net/cpptest/DefaultService/$metadata#People(3)/Microsoft.Test.OData.Services.ODataWCFService.Employee/DateHired\", \
		\"value\":\"2010-12-13T00:00:00Z\"}");

 	web::json::value json_payload = web::json::value::parse(_payload);
	auto return_payload = json_reader->deserilize(json_payload);
	auto primitive_value = std::dynamic_pointer_cast<odata_primitive_value>(return_payload->get_values()[0]);
	auto primitive_type = std::dynamic_pointer_cast<edm_primitive_type>(primitive_value->get_value_type());
	VERIFY_ARE_EQUAL(primitive_type->get_primitive_kind(), edm_primitive_type_kind_t::DateTimeOffset);
	VERIFY_ARE_EQUAL(primitive_type->get_type_kind(), edm_type_kind_t::Primitive);
	VERIFY_ARE_EQUAL(primitive_value->as<::utility::datetime>().to_string(::utility::datetime::ISO_8601), U("2010-12-13T00:00:00Z"));
}

TEST(collection_of_primitive_with_single_return_test)
{
	auto json_reader = get_json_reader();
	VERIFY_IS_NOT_NULL(json_reader);

	::utility::string_t _entity_payload = U(
	    "{\"@odata.context\":\"http://odatae2etest.azurewebsites.net/cpptest/DefaultService/$metadata#People(4)/Numbers\", \
		\"value\":[\"111-11-1111\"]}"
		);

 	web::json::value json_payload = web::json::value::parse(_entity_payload);
	auto return_payload = json_reader->deserilize(json_payload);
	VERIFY_IS_NOT_NULL(return_payload);	
	auto entity_values = return_payload->get_values();
	VERIFY_ARE_EQUAL(entity_values.size(), 1);
	auto primitive_value = std::dynamic_pointer_cast<odata_primitive_value>(entity_values[0]);
	VERIFY_IS_NOT_NULL(primitive_value);
	VERIFY_ARE_EQUAL(primitive_value->get_value_type()->get_type_kind(), edm_type_kind_t::Primitive);
	VERIFY_ARE_EQUAL(primitive_value->as<::utility::string_t>(), U("111-11-1111"));
}

TEST(collection_of_primitive_with_empty_return_test)
{
	auto json_reader = get_json_reader();
	VERIFY_IS_NOT_NULL(json_reader);

	::utility::string_t _entity_payload = U(
	    "{\"@odata.context\":\"http://odatae2etest.azurewebsites.net/cpptest/DefaultService/$metadata#People(4)/Numbers\", \
		\"value\":[]}"
		);

 	web::json::value json_payload = web::json::value::parse(_entity_payload);
	auto return_payload = json_reader->deserilize(json_payload);
	VERIFY_IS_NOT_NULL(return_payload);	
	auto entity_values = return_payload->get_values();
	VERIFY_ARE_EQUAL(entity_values.size(), 0);
}

TEST(single_enum_test)
{
	auto json_reader = get_json_reader();
	VERIFY_IS_NOT_NULL(json_reader);

	::utility::string_t _entity_payload = U(
		"{\"@odata.context\":\"http://odatae2etest.azurewebsites.net/cpptest/DefaultService/$metadata#Products(5)/SkinColor\",\"value\":\"Red\"}"
		);

 	web::json::value json_payload = web::json::value::parse(_entity_payload);
	auto return_payload = json_reader->deserilize(json_payload);
	VERIFY_IS_NOT_NULL(return_payload);	
	auto return_values = return_payload->get_values();
	VERIFY_ARE_EQUAL(return_values.size(), 1);
	auto enum_value = std::dynamic_pointer_cast<odata_enum_value>(return_values[0]);
	VERIFY_IS_NOT_NULL(enum_value);
	VERIFY_ARE_EQUAL(enum_value->get_value_type()->get_type_kind(), edm_type_kind_t::Enum);
	VERIFY_ARE_EQUAL(enum_value->to_string(), U("Red"));
}

TEST(collection_of_enum_test)
{
	auto json_reader = get_json_reader();
	VERIFY_IS_NOT_NULL(json_reader);

	::utility::string_t _entity_payload = U(
	    "{\"@odata.context\":\"http://odatae2etest.azurewebsites.net/cpptest/DefaultService/$metadata#Products(5)/CoverColors\", \
		\"value\":[\"Green\",\"Blue\",\"Blue\"]}"
		);

 	web::json::value json_payload = web::json::value::parse(_entity_payload);
	auto return_payload = json_reader->deserilize(json_payload);
	VERIFY_IS_NOT_NULL(return_payload);	
	auto return_values = return_payload->get_values();
	VERIFY_ARE_EQUAL(return_values.size(), 3);
	auto enum_value = std::dynamic_pointer_cast<odata_enum_value>(return_values[0]);
	VERIFY_IS_NOT_NULL(enum_value);
	VERIFY_ARE_EQUAL(enum_value->get_value_type()->get_type_kind(), edm_type_kind_t::Enum);
	VERIFY_ARE_EQUAL(enum_value->to_string(), U("Green"));
	enum_value = std::dynamic_pointer_cast<odata_enum_value>(return_values[1]);
	VERIFY_IS_NOT_NULL(enum_value);
	VERIFY_ARE_EQUAL(enum_value->get_value_type()->get_type_kind(), edm_type_kind_t::Enum);
	VERIFY_ARE_EQUAL(enum_value->to_string(), U("Blue"));
	enum_value = std::dynamic_pointer_cast<odata_enum_value>(return_values[2]);
	VERIFY_IS_NOT_NULL(enum_value);
	VERIFY_ARE_EQUAL(enum_value->get_value_type()->get_type_kind(), edm_type_kind_t::Enum);
	VERIFY_ARE_EQUAL(enum_value->to_string(), U("Blue"));
}

TEST(collection_of_enum_with_single_return_test)
{
	auto json_reader = get_json_reader();
	VERIFY_IS_NOT_NULL(json_reader);

	::utility::string_t _entity_payload = U(
	    "{\"@odata.context\":\"http://odatae2etest.azurewebsites.net/cpptest/DefaultService/$metadata#Products(5)/CoverColors\", \
		\"value\":[\"Green\"]}"
		);

 	web::json::value json_payload = web::json::value::parse(_entity_payload);
	auto return_payload = json_reader->deserilize(json_payload);
	VERIFY_IS_NOT_NULL(return_payload);	
	auto return_values = return_payload->get_values();
	VERIFY_ARE_EQUAL(return_values.size(), 1);
	auto enum_value = std::dynamic_pointer_cast<odata_enum_value>(return_values[0]);
	VERIFY_IS_NOT_NULL(enum_value);
	VERIFY_ARE_EQUAL(enum_value->get_value_type()->get_type_kind(), edm_type_kind_t::Enum);
	VERIFY_ARE_EQUAL(enum_value->to_string(), U("Green"));

}

TEST(collection_of_enum_with_empty_return_test)
{
	auto json_reader = get_json_reader();
	VERIFY_IS_NOT_NULL(json_reader);

	::utility::string_t _entity_payload = U(
	    "{\"@odata.context\":\"http://odatae2etest.azurewebsites.net/cpptest/DefaultService/$metadata#Products(5)/CoverColors\", \
		\"value\":[]}"
		);

 	web::json::value json_payload = web::json::value::parse(_entity_payload);
	auto return_payload = json_reader->deserilize(json_payload);
	VERIFY_IS_NOT_NULL(return_payload);	
	auto return_values = return_payload->get_values();
	VERIFY_ARE_EQUAL(return_values.size(), 0);
}

TEST(singleton_test)
{
	auto json_reader = get_json_reader();
	VERIFY_IS_NOT_NULL(json_reader);

	::utility::string_t _entity_payload = U(
		"{\"@odata.context\":\"http://odatae2etest.azurewebsites.net/cpptest/DefaultService/$metadata#DefaultStoredPI\", \
		\"@odata.id\":\"http://odatae2etest.azurewebsites.net/cpptest/DefaultService/DefaultStoredPI\", \
		\"@odata.editLink\":\"http://odatae2etest.azurewebsites.net/cpptest/DefaultService/DefaultStoredPI\", \
		\"StoredPIID\":800,\"PIName\":\"The Default Stored PI\",\"PIType\":\"CreditCard\",\"CreatedDate\":\"2013-12-31T00:00:00Z\"}"
		);

	::web::json::value json_payload = web::json::value::parse(_entity_payload);
	auto return_payload = json_reader->deserilize(json_payload);
	VERIFY_IS_NOT_NULL(return_payload);
	auto entity_values = return_payload->get_values();
	VERIFY_ARE_EQUAL(entity_values.size(), 1);
	auto entity_value = std::dynamic_pointer_cast<odata_entity_value>(entity_values[0]);
	VERIFY_IS_NOT_NULL(entity_value);
	int storedpid = 0;
	VERIFY_ARE_EQUAL(entity_value->try_get(U("StoredPIID"), storedpid), true);
	VERIFY_ARE_EQUAL(storedpid, 800);
    ::utility::string_t pinname;
	VERIFY_ARE_EQUAL(entity_value->try_get(U("PIName"), pinname), true);
	VERIFY_ARE_EQUAL(pinname, U("The Default Stored PI"));
    ::utility::string_t pitype;
	VERIFY_ARE_EQUAL(entity_value->try_get(U("PIType"), pitype), true);
	VERIFY_ARE_EQUAL(pitype, U("CreditCard"));
	::utility::datetime createdate;
	VERIFY_ARE_EQUAL(entity_value->try_get(U("CreatedDate"), createdate), true);
	VERIFY_ARE_EQUAL(createdate.to_string(::utility::datetime::date_format::ISO_8601), U("2013-12-31T00:00:00Z"));
}

TEST(single_entity_with_single_navigation_test)
{
	auto json_reader = get_json_reader();
	VERIFY_IS_NOT_NULL(json_reader);

	::utility::string_t _entity_payload = U(
		"{\"@odata.context\":\"http://odatae2etest.azurewebsites.net/cpptest/DefaultService/$metadata#Accounts(101)/MyGiftCard/$entity\", \
		\"GiftCardID\":301,\"GiftCardNO\":\"AAA123A\",\"Amount\":19.9,\"ExperationDate\":\"2013-12-30T00:00:00Z\"}"
		);

	::web::json::value json_payload = web::json::value::parse(_entity_payload);
	auto return_payload = json_reader->deserilize(json_payload);
	VERIFY_IS_NOT_NULL(return_payload);
	auto entity_values = return_payload->get_values();
	VERIFY_ARE_EQUAL(entity_values.size(), 1);
	auto entity_value = std::dynamic_pointer_cast<odata_entity_value>(entity_values[0]);
	VERIFY_IS_NOT_NULL(entity_value);
	int cardid = 0;
	VERIFY_ARE_EQUAL(entity_value->try_get(U("GiftCardID"), cardid), true);
	VERIFY_ARE_EQUAL(cardid, 301);
    ::utility::string_t cardno;
	VERIFY_ARE_EQUAL(entity_value->try_get(U("GiftCardNO"), cardno), true);
	VERIFY_ARE_EQUAL(cardno, U("AAA123A"));
    double amount;
	VERIFY_ARE_EQUAL(entity_value->try_get(U("Amount"), amount), true);
	VERIFY_ARE_EQUAL(amount, 19.9);
	::utility::datetime experationdate;
	VERIFY_ARE_EQUAL(entity_value->try_get(U("ExperationDate"), experationdate), true);
	VERIFY_ARE_EQUAL(experationdate.to_string(::utility::datetime::date_format::ISO_8601), U("2013-12-30T00:00:00Z"));
}

TEST(single_entity_with_collection_of_navigation_test)
{
	auto json_reader = get_json_reader();
	VERIFY_IS_NOT_NULL(json_reader);

	::utility::string_t _entity_payload = U(
		"{\"@odata.context\":\"http://odatae2etest.azurewebsites.net/cpptest/DefaultService/$metadata#Accounts(101)/MyPaymentInstruments\", \
		\"value\":[{\"PaymentInstrumentID\":101901,\"FriendlyName\":\"101 first PI\",\"CreatedDate\":\"2012-11-01T00:00:00Z\"}, \
		{\"@odata.type\":\"#Microsoft.Test.OData.Services.ODataWCFService.CreditCardPI\",\"PaymentInstrumentID\":101902,\"FriendlyName\": \
		\"101 frist credit PI\",\"CreatedDate\":\"2012-11-01T00:00:00Z\",\"CardNumber\":\"6000000000000000\",\"CVV\":\"234\",\"HolderName\":\"Alex\", \
		\"Balance\":100.0,\"ExperationDate\":\"2022-11-01T00:00:00Z\"}, \
		{\"@odata.type\":\"#Microsoft.Test.OData.Services.ODataWCFService.CreditCardPI\",\"PaymentInstrumentID\":101903, \
		\"FriendlyName\":\"101 second credit PI\",\"CreatedDate\":\"2012-11-01T00:00:00Z\",\"CardNumber\":\"8000000000000000\",\"CVV\":\"012\", \
		\"HolderName\":\"James\",\"Balance\":300.0,\"ExperationDate\":\"2022-10-02T00:00:00Z\"}]}"
		);

    ::web::json::value json_payload = web::json::value::parse(_entity_payload);
	auto return_payload = json_reader->deserilize(json_payload);
	VERIFY_IS_NOT_NULL(return_payload);
	auto entity_values = return_payload->get_values();
	VERIFY_ARE_EQUAL(entity_values.size(), 3);
	auto entity_value = std::dynamic_pointer_cast<odata_entity_value>(entity_values[0]);
	VERIFY_IS_NOT_NULL(entity_value);
	int payid = 0;
	VERIFY_ARE_EQUAL(entity_value->try_get(U("PaymentInstrumentID"), payid), true);
	VERIFY_ARE_EQUAL(payid, 101901);
    entity_value = std::dynamic_pointer_cast<odata_entity_value>(entity_values[1]);
	VERIFY_IS_NOT_NULL(entity_value);
	VERIFY_ARE_EQUAL(entity_value->try_get(U("PaymentInstrumentID"), payid), true);
	VERIFY_ARE_EQUAL(payid, 101902);
    entity_value = std::dynamic_pointer_cast<odata_entity_value>(entity_values[2]);
	VERIFY_IS_NOT_NULL(entity_value);
	VERIFY_ARE_EQUAL(entity_value->try_get(U("PaymentInstrumentID"), payid), true);
	VERIFY_ARE_EQUAL(payid, 101903);
}

TEST(collection_of_entity_with_single_navigation_test)
{
	auto json_reader = get_json_reader();
	VERIFY_IS_NOT_NULL(json_reader);
	::utility::string_t _entity_payload = U(
		"{\"@odata.context\":\"http://odatae2etest.azurewebsites.net/cpptest/DefaultService/$metadata#Employees\",  \
		\"value\":[{\"@odata.id\":\"http://odatae2etest.azurewebsites.net/cpptest/DefaultService/Employees(PersonID=3)\", \
		\"@odata.editLink\":\"http://odatae2etest.azurewebsites.net/cpptest/DefaultService/Employees(PersonID=3)\", \
		\"PersonID\":3,\"FirstName\":\"Jacob\",\"LastName\":\"Zip\",\"MiddleName\":null,\"HomeAddress\":null,\"Home\":{\"type\":\"Point\", \
		\"coordinates\":[161.8,15.0],\"crs\":{\"type\":\"name\",\"properties\":{\"name\":\"EPSG:4326\"}}},\"Numbers\":[\"333-333-3333\"], \
		\"Emails\":[null],\"DateHired\":\"2010-12-13T00:00:00Z\",\"Office\":{\"type\":\"Point\",\"coordinates\":[162.0,15.0],\"crs\":{\"type\": \
		\"name\",\"properties\":{\"name\":\"EPSG:4326\"}}},\"Company\":{\"@odata.id\": \
		\"http://odatae2etest.azurewebsites.net/cpptest/DefaultService/Company\", \
		\"@odata.editLink\":\"http://odatae2etest.azurewebsites.net/cpptest/DefaultService/Company\", \
		\"CompanyID\":0,\"CompanyCategory\":\"IT\",\"Revenue\":100000,\"Name\":\"MS\",\"Address\":{\"Street\": \
		\"1 Microsoft Way\",\"City\":\"Redmond\",\"PostalCode\":\"98052\"}}},{\"@odata.id\": \
		\"http://odatae2etest.azurewebsites.net/cpptest/DefaultService/Employees(PersonID=4)\", \
		\"@odata.editLink\":\"http://odatae2etest.azurewebsites.net/cpptest/DefaultService/Employees(PersonID=4)\", \
		\"PersonID\":4,\"FirstName\":\"Elmo\",\"LastName\":\"Rogers\",\"MiddleName\":null,\"HomeAddress\":null,\"Home\": \
		{\"type\":\"Point\",\"coordinates\":[-61.8,-15.0],\"crs\":{\"type\":\"name\",\"properties\":{\"name\":\"EPSG:4326\"}}}, \
		\"Numbers\":[\"444-444-4444\",\"555-555-5555\",\"666-666-6666\"],\"Emails\":[\"def@def.org\",\"lmn@lmn.com\"], \
		\"DateHired\":\"2008-03-27T00:00:00Z\",\"Office\":{\"type\":\"Point\",\"coordinates\":[-62.0,-15.0],\"crs\":{\"type\":\"name\",\"properties\": \
		{\"name\":\"EPSG:4326\"}}},\"Company\":{\"@odata.id\":\"http://odatae2etest.azurewebsites.net/cpptest/DefaultService/Company\", \
		\"@odata.editLink\":\"http://odatae2etest.azurewebsites.net/cpptest/DefaultService/Company\",\"CompanyID\":0,\"CompanyCategory\": \
		\"IT\",\"Revenue\":100000,\"Name\":\"MS\",\"Address\":{\"Street\":\"1 Microsoft Way\",\"City\":\"Redmond\",\"PostalCode\":\"98052\"}}}]}"
	    );

    ::web::json::value json_payload = web::json::value::parse(_entity_payload);
	auto return_payload = json_reader->deserilize(json_payload);
	VERIFY_IS_NOT_NULL(return_payload);
	auto entity_values = return_payload->get_values();
	VERIFY_ARE_EQUAL(entity_values.size(), 2);
	auto entity_value = std::dynamic_pointer_cast<odata_entity_value>(entity_values[0]);
	VERIFY_IS_NOT_NULL(entity_value);
	std::shared_ptr<odata_value> property_value;
	auto ok = entity_value->get_property_value(U("Company"), property_value);
	auto contained_value = std::dynamic_pointer_cast<odata_entity_value>(property_value);
	VERIFY_IS_NOT_NULL(contained_value);
	VERIFY_ARE_EQUAL(contained_value->properties().size(), 7);
}

TEST(collection_of_entity_with_collection_of_navigation_test)
{
	auto json_reader = get_json_reader();
	VERIFY_IS_NOT_NULL(json_reader);

	::utility::string_t _entity_payload = U("{\"@odata.context\":\"http://odatae2etest.azurewebsites.net/cpptest/DefaultService/$metadata#Accounts\",\"value\":[{\"@odata.id\":\"http://odatae2etest.azurewebsites.net/cpptest/DefaultService/Accounts(101)\",\"@odata.editLink\":\"http://odatae2etest.azurewebsites.net/cpptest/DefaultService/Accounts(101)\",\"AccountID\":101,\"CountryRegion\":\"US\",\"AccountInfo\":{\"FirstName\":\"Alex\",\"LastName\":\"Green\"},\"MyPaymentInstruments@odata.context\":\"http://odatae2etest.azurewebsites.net/cpptest/DefaultService/$metadata#Accounts(101)/MyPaymentInstruments\",\"MyPaymentInstruments\":[{\"PaymentInstrumentID\":101901,\"FriendlyName\":\"101 first PI\",\"CreatedDate\":\"2012-11-01T00:00:00Z\"},{\"@odata.type\":\"#Microsoft.Test.OData.Services.ODataWCFService.CreditCardPI\",\"PaymentInstrumentID\":101902,\"FriendlyName\":\"101 frist credit PI\",\"CreatedDate\":\"2012-11-01T00:00:00Z\",\"CardNumber\":\"6000000000000000\",\"CVV\":\"234\",\"HolderName\":\"Alex\",\"Balance\":100.0,\"ExperationDate\":\"2022-11-01T00:00:00Z\"},{\"@odata.type\":\"#Microsoft.Test.OData.Services.ODataWCFService.CreditCardPI\",\"PaymentInstrumentID\":101903,\"FriendlyName\":\"101 second credit PI\",\"CreatedDate\":\"2012-11-01T00:00:00Z\",\"CardNumber\":\"8000000000000000\",\"CVV\":\"012\",\"HolderName\":\"James\",\"Balance\":300.0,\"ExperationDate\":\"2022-10-02T00:00:00Z\"}]},{\"@odata.id\":\"http://odatae2etest.azurewebsites.net/cpptest/DefaultService/Accounts(102)\",\"@odata.editLink\":\"http://odatae2etest.azurewebsites.net/cpptest/DefaultService/Accounts(102)\",\"AccountID\":102,\"CountryRegion\":\"GB\",\"AccountInfo\":{\"FirstName\":\"James\",\"LastName\":\"Bunder\"},\"MyPaymentInstruments@odata.context\":\"http://odatae2etest.azurewebsites.net/cpptest/DefaultService/$metadata#Accounts(102)/MyPaymentInstruments\",\"MyPaymentInstruments\":[]},{\"@odata.id\":\"http://odatae2etest.azurewebsites.net/cpptest/DefaultService/Accounts(103)\",\"@odata.editLink\":\"http://odatae2etest.azurewebsites.net/cpptest/DefaultService/Accounts(103)\",\"AccountID\":103,\"CountryRegion\":\"CN\",\"AccountInfo\":{\"FirstName\":\"Adam\",\"LastName\":\"Homes\"},\"MyPaymentInstruments@odata.context\":\"http://odatae2etest.azurewebsites.net/cpptest/DefaultService/$metadata#Accounts(103)/MyPaymentInstruments\",\"MyPaymentInstruments\":[{\"PaymentInstrumentID\":103901,\"FriendlyName\":\"103 frist PI\",\"CreatedDate\":\"2013-10-01T00:00:00Z\"},{\"PaymentInstrumentID\":103902,\"FriendlyName\":\"103 second PI\",\"CreatedDate\":\"2013-01-01T00:00:00Z\"},{\"PaymentInstrumentID\":103905,\"FriendlyName\":\"103 new PI\",\"CreatedDate\":\"2013-10-29T00:00:00Z\"},{\"PaymentInstrumentID\":101910,\"FriendlyName\":\"103 backup PI\",\"CreatedDate\":\"2013-06-15T00:00:00Z\"}]},{\"@odata.id\":\"http://odatae2etest.azurewebsites.net/cpptest/DefaultService/Accounts(104)\",\"@odata.editLink\":\"http://odatae2etest.azurewebsites.net/cpptest/DefaultService/Accounts(104)\",\"AccountID\":104,\"CountryRegion\":\"CN\",\"AccountInfo\":{\"FirstName\":\"Adrian\",\"LastName\":\"Green\"},\"MyPaymentInstruments@odata.context\":\"http://odatae2etest.azurewebsites.net/cpptest/DefaultService/$metadata#Accounts(104)/MyPaymentInstruments\",\"MyPaymentInstruments\":[]},{\"@odata.id\":\"http://odatae2etest.azurewebsites.net/cpptest/DefaultService/Accounts(105)\",\"@odata.editLink\":\"http://odatae2etest.azurewebsites.net/cpptest/DefaultService/Accounts(105)\",\"AccountID\":105,\"CountryRegion\":\"US\",\"AccountInfo\":{\"FirstName\":\"Lily\",\"LastName\":\"Green\"},\"MyPaymentInstruments@odata.context\":\"http://odatae2etest.azurewebsites.net/cpptest/DefaultService/$metadata#Accounts(105)/MyPaymentInstruments\",\"MyPaymentInstruments\":[]},{\"@odata.id\":\"http://odatae2etest.azurewebsites.net/cpptest/DefaultService/Accounts(106)\",\"@odata.editLink\":\"http://odatae2etest.azurewebsites.net/cpptest/DefaultService/Accounts(106)\",\"AccountID\":106,\"CountryRegion\":\"CN\",\"AccountInfo\":{\"FirstName\":\"Allen\",\"LastName\":\"Ivorson\"},\"MyPaymentInstruments@odata.context\":\"http://odatae2etest.azurewebsites.net/cpptest/DefaultService/$metadata#Accounts(106)/MyPaymentInstruments\",\"MyPaymentInstruments\":[]},{\"@odata.id\":\"http://odatae2etest.azurewebsites.net/cpptest/DefaultService/Accounts(107)\",\"@odata.editLink\":\"http://odatae2etest.azurewebsites.net/cpptest/DefaultService/Accounts(107)\",\"AccountID\":107,\"CountryRegion\":\"FR\",\"AccountInfo\":{\"FirstName\":\"Albert\",\"LastName\":\"Ivorson\"},\"MyPaymentInstruments@odata.context\":\"http://odatae2etest.azurewebsites.net/cpptest/DefaultService/$metadata#Accounts(107)/MyPaymentInstruments\",\"MyPaymentInstruments\":[]}]}");

    ::web::json::value json_payload = web::json::value::parse(_entity_payload);
	auto return_payload = json_reader->deserilize(json_payload);
	VERIFY_IS_NOT_NULL(return_payload);
	auto entity_values = return_payload->get_values();
	VERIFY_ARE_EQUAL(entity_values.size(), 7);
	auto entity_value = std::dynamic_pointer_cast<odata_entity_value>(entity_values[0]);
	VERIFY_IS_NOT_NULL(entity_value);
	std::shared_ptr<odata_value> property_value;
	auto ok = entity_value->get_property_value(U("MyPaymentInstruments"), property_value);
	auto contained_values = std::dynamic_pointer_cast<odata_collection_value>(property_value);
	VERIFY_IS_NOT_NULL(contained_values);
	VERIFY_ARE_EQUAL(contained_values->get_collection_values().size(), 3);
	auto contained_value = std::dynamic_pointer_cast<odata_entity_value>(contained_values->get_collection_values()[0]);
	VERIFY_IS_NOT_NULL(contained_value);
	VERIFY_ARE_EQUAL(contained_value->properties().size(), 4);
}


// @editLink value are all removed in edit link tests
//http://services.odata.org/V4/Northwind/Northwind.svc/Products
TEST(edit_link_entity_set)
{
	auto json_reader = get_json_reader();
	VERIFY_IS_NOT_NULL(json_reader);

	::utility::string_t _entity_payload = U("{\"@odata.context\":\"http://odatae2etest.azurewebsites.net/cpptest/DefaultService/$metadata#Products(ProductID)\", \
		\"value\":[{\"ProductID\":1},{\"ProductID\":2}]}");

    ::web::json::value json_payload = web::json::value::parse(_entity_payload);
	auto return_payload = json_reader->deserilize(json_payload);
	auto entity_values = return_payload->get_values();

	VERIFY_ARE_EQUAL(entity_values.size(), 2);
	auto entity_value = std::dynamic_pointer_cast<odata_entity_value>(entity_values[0]);
	::utility::string_t edit_link;
	auto ok = entity_value->try_get(U("@odata.editLink"), edit_link);
	VERIFY_ARE_EQUAL(edit_link, U("http://odatae2etest.azurewebsites.net/cpptest/DefaultService/Products(1)"));

	entity_value = std::dynamic_pointer_cast<odata_entity_value>(entity_values[1]);
	ok = entity_value->try_get(U("@odata.editLink"), edit_link);
	VERIFY_ARE_EQUAL(edit_link, U("http://odatae2etest.azurewebsites.net/cpptest/DefaultService/Products(2)"));
}

TEST(edit_link_entity_relative_path)
{
	::utility::string_t _entity_payload = U(
		"{\"@odata.context\":\"http://odatae2etest.azurewebsites.net/cpptest/DefaultService/$metadata#Accounts/$entity\", \
		\"@odata.editLink\":\"Accounts(101)\", \
		\"AccountID\":101,\"CountryRegion\":\"%0A%E4%B8%AD%E5%8D%8E%E4%BA%BA%E6%B0%91%E5%85%B1%E5%92%8C%E5%9B%BD\",\"AccountInfo\":{\"FirstName\":\"Alex\",\"LastName\":\"Green\"}}"
		);

	auto json_reader = get_json_reader();
	VERIFY_IS_NOT_NULL(json_reader);

	::web::json::value json_payload = web::json::value::parse(_entity_payload);
	auto return_payload = json_reader->deserilize(json_payload);
	auto entity_values = return_payload->get_values();

	VERIFY_ARE_EQUAL(entity_values.size(), 1);
	auto entity_value = std::dynamic_pointer_cast<odata_entity_value>(entity_values[0]);
	::utility::string_t edit_link;
	auto ok = entity_value->try_get(U("@odata.editLink"), edit_link);
	VERIFY_ARE_EQUAL(edit_link, U("http://odatae2etest.azurewebsites.net/cpptest/DefaultService/Accounts(101)"));
}

//http://services.odata.org/V4/Northwind/Northwind.svc/Products(1)
TEST(edit_link_entity_set_with_key)
{
	auto json_reader = get_json_reader();
	VERIFY_IS_NOT_NULL(json_reader);

	::utility::string_t _entity_payload 
		= U("{\"@odata.context\":\"http://odatae2etest.azurewebsites.net/cpptest/DefaultService/$metadata#Products(ProductID)/$entity\", \"ProductID\":1}");

    ::web::json::value json_payload = web::json::value::parse(_entity_payload);
	auto return_payload = json_reader->deserilize(json_payload);
	auto entity_values = return_payload->get_values();

	VERIFY_ARE_EQUAL(entity_values.size(), 1);
	auto entity_value = std::dynamic_pointer_cast<odata_entity_value>(entity_values[0]);
	::utility::string_t edit_link;
	auto ok = entity_value->try_get(U("@odata.editLink"), edit_link);
	VERIFY_ARE_EQUAL(edit_link, U("http://odatae2etest.azurewebsites.net/cpptest/DefaultService/Products(1)"));
}

//http://odatae2etest.azurewebsites.net/cpptest/DefaultService/LabourUnion
TEST(edit_link_singleton)
{
	auto json_reader = get_json_reader();
	VERIFY_IS_NOT_NULL(json_reader);

	::utility::string_t _entity_payload 
		= U("{\"@odata.context\":\"http://odatae2etest.azurewebsites.net/cpptest/DefaultService/$metadata#LabourUnion\",\"LabourUnionID\":0,\"Name\":\"MS Labour Union\"}");

    ::web::json::value json_payload = web::json::value::parse(_entity_payload);
	auto return_payload = json_reader->deserilize(json_payload);
	auto entity_values = return_payload->get_values();

	VERIFY_ARE_EQUAL(entity_values.size(), 1);
	auto entity_value = std::dynamic_pointer_cast<odata_entity_value>(entity_values[0]);
	::utility::string_t edit_link;
	auto ok = entity_value->try_get(U("@odata.editLink"), edit_link);
	VERIFY_ARE_EQUAL(edit_link, U("http://odatae2etest.azurewebsites.net/cpptest/DefaultService/LabourUnion"));
}

//http://odatae2etest.azurewebsites.net/cpptest/DefaultService/Accounts?$expand=MyPaymentInstruments
TEST(edit_link_contained_navigation)
{
	auto json_reader = get_json_reader();
	VERIFY_IS_NOT_NULL(json_reader);

	::utility::string_t _entity_payload 
		= U("{\"@odata.context\":\"http://odatae2etest.azurewebsites.net/cpptest/DefaultService/$metadata#Accounts(AccountID,MyPaymentInstruments,MyPaymentInstruments(PaymentInstrumentID))\", \
		\"value\":[{\"AccountID\":101,\"MyPaymentInstruments@odata.context\":\"http://odatae2etest.azurewebsites.net/cpptest/DefaultService/$metadata#Accounts(101)/MyPaymentInstruments(PaymentInstrumentID)\",\"MyPaymentInstruments\":[{\"PaymentInstrumentID\":101901}]}]}");

    ::web::json::value json_payload = web::json::value::parse(_entity_payload);
	auto return_payload = json_reader->deserilize(json_payload);
	auto entity_values = return_payload->get_values();

	VERIFY_ARE_EQUAL(entity_values.size(), 1);
	auto entity_value = std::dynamic_pointer_cast<odata_entity_value>(entity_values[0]);
	::utility::string_t edit_link;
	auto ok = entity_value->try_get(U("@odata.editLink"), edit_link);
	VERIFY_ARE_EQUAL(edit_link, U("http://odatae2etest.azurewebsites.net/cpptest/DefaultService/Accounts(101)"));

	std::shared_ptr<odata_value> property_value;
	ok = entity_value->get_property_value(U("MyPaymentInstruments"), property_value);
	auto contained_values = std::dynamic_pointer_cast<odata_collection_value>(property_value);
	auto contained_value = std::dynamic_pointer_cast<odata_entity_value>(contained_values->get_collection_values()[0]);
	edit_link;
	ok = contained_value->try_get(U("@odata.editLink"), edit_link);
	VERIFY_ARE_EQUAL(edit_link, U("http://odatae2etest.azurewebsites.net/cpptest/DefaultService/Accounts(101)/MyPaymentInstruments(101901)"));
}

TEST(edit_link_contained_navigation_derived_type)
{
	auto json_reader = get_json_reader();
	VERIFY_IS_NOT_NULL(json_reader);

	::utility::string_t _entity_payload 
		= U("{\"@odata.context\":\"http://odatae2etest.azurewebsites.net/cpptest/DefaultService/$metadata#Accounts(AccountID,MyPaymentInstruments,MyPaymentInstruments(PaymentInstrumentID))\", \
		\"value\":[{\"AccountID\":101,\"MyPaymentInstruments@odata.context\":\"http://odatae2etest.azurewebsites.net/cpptest/DefaultService/$metadata#Accounts(101)/MyPaymentInstruments(PaymentInstrumentID)\",\"MyPaymentInstruments\":[{\"@odata.type\": \"#Microsoft.Test.OData.Services.ODataWCFService.CreditCardPI\",\"PaymentInstrumentID\":101901}]}]}");

    ::web::json::value json_payload = web::json::value::parse(_entity_payload);
	auto return_payload = json_reader->deserilize(json_payload);
	auto entity_values = return_payload->get_values();

	VERIFY_ARE_EQUAL(entity_values.size(), 1);
	auto entity_value = std::dynamic_pointer_cast<odata_entity_value>(entity_values[0]);
	::utility::string_t edit_link;
	auto ok = entity_value->try_get(U("@odata.editLink"), edit_link);
	VERIFY_ARE_EQUAL(edit_link, U("http://odatae2etest.azurewebsites.net/cpptest/DefaultService/Accounts(101)"));

	std::shared_ptr<odata_value> property_value;
	ok = entity_value->get_property_value(U("MyPaymentInstruments"), property_value);
	auto contained_values = std::dynamic_pointer_cast<odata_collection_value>(property_value);
	auto contained_value = std::dynamic_pointer_cast<odata_entity_value>(contained_values->get_collection_values()[0]);
	edit_link;
	ok = contained_value->try_get(U("@odata.editLink"), edit_link);
	VERIFY_ARE_EQUAL(edit_link, U("http://odatae2etest.azurewebsites.net/cpptest/DefaultService/Accounts(101)/MyPaymentInstruments(101901)/Microsoft.Test.OData.Services.ODataWCFService.CreditCardPI"));
}

//http://odatae2etest.azurewebsites.net/cpptest/DefaultService/Accounts(101)/MyPaymentInstruments
TEST(edit_link_contained_navigation_property_derived_type)
{
	auto json_reader = get_json_reader();
	VERIFY_IS_NOT_NULL(json_reader);

	::utility::string_t _entity_payload 
		= U("{\"@odata.context\":\"http://odatae2etest.azurewebsites.net/cpptest/DefaultService/$metadata#Accounts(101)/MyPaymentInstruments(PaymentInstrumentID)\", \
		\"value\":[{\"@odata.type\":\"#Microsoft.Test.OData.Services.ODataWCFService.CreditCardPI\",\"PaymentInstrumentID\":101902}]}");

    ::web::json::value json_payload = web::json::value::parse(_entity_payload);
	auto return_payload = json_reader->deserilize(json_payload);
	auto entity_values = return_payload->get_values();

	VERIFY_ARE_EQUAL(entity_values.size(), 1);
	auto entity_value = std::dynamic_pointer_cast<odata_entity_value>(entity_values[0]);
	::utility::string_t edit_link;
	auto ok = entity_value->try_get(U("@odata.editLink"), edit_link);
	VERIFY_ARE_EQUAL(edit_link, U("http://odatae2etest.azurewebsites.net/cpptest/DefaultService/Accounts(101)/MyPaymentInstruments(101902)/Microsoft.Test.OData.Services.ODataWCFService.CreditCardPI"));
}

//http://odatae2etest.azurewebsites.net/cpptest/DefaultService/Accounts?$expand=MyGiftCard
TEST(edit_link_singleton_contained_navigation)
{
	auto json_reader = get_json_reader();
	VERIFY_IS_NOT_NULL(json_reader);

	::utility::string_t _entity_payload 
		= U("{\"@odata.context\":\"http://odatae2etest.azurewebsites.net/cpptest/DefaultService/$metadata#Accounts(AccountID,MyGiftCard,MyGiftCard(GiftCardID))\", \
		\"value\":[{\"AccountID\":101,\"MyGiftCard@odata.context\":\"http://odatae2etest.azurewebsites.net/cpptest/DefaultService/$metadata#Accounts(101)/MyGiftCard(GiftCardID)/$entity\",\"MyGiftCard\":{\"GiftCardID\":301}}]}");

    ::web::json::value json_payload = web::json::value::parse(_entity_payload);
	auto return_payload = json_reader->deserilize(json_payload);
	auto entity_values = return_payload->get_values();

	VERIFY_ARE_EQUAL(entity_values.size(), 1);
	auto entity_value = std::dynamic_pointer_cast<odata_entity_value>(entity_values[0]);
	::utility::string_t edit_link;
	auto ok = entity_value->try_get(U("@odata.editLink"), edit_link);
	VERIFY_ARE_EQUAL(edit_link, U("http://odatae2etest.azurewebsites.net/cpptest/DefaultService/Accounts(101)"));

	std::shared_ptr<odata_value> property_value;
	ok = entity_value->get_property_value(U("MyGiftCard"), property_value);
	auto contained_value = std::dynamic_pointer_cast<odata_entity_value>(property_value);
	edit_link;
	ok = contained_value->try_get(U("@odata.editLink"), edit_link);
	VERIFY_ARE_EQUAL(edit_link, U("http://odatae2etest.azurewebsites.net/cpptest/DefaultService/Accounts(101)/MyGiftCard"));
}

//http://odatae2etest.azurewebsites.net/cpptest/DefaultService/Accounts(101)/MyGiftCard
TEST(edit_link_singleton_contained_navigation_property)
{
	auto json_reader = get_json_reader();
	VERIFY_IS_NOT_NULL(json_reader);

	::utility::string_t _entity_payload 
		= U("{\"@odata.context\":\"http://odatae2etest.azurewebsites.net/cpptest/DefaultService/$metadata#Accounts(101)/MyGiftCard(GiftCardID)/$entity\",\"GiftCardID\":301}");

    ::web::json::value json_payload = web::json::value::parse(_entity_payload);
	auto return_payload = json_reader->deserilize(json_payload);
	auto entity_values = return_payload->get_values();

	VERIFY_ARE_EQUAL(entity_values.size(), 1);
	auto entity_value = std::dynamic_pointer_cast<odata_entity_value>(entity_values[0]);
	::utility::string_t edit_link;
	auto ok = entity_value->try_get(U("@odata.editLink"), edit_link);
	VERIFY_ARE_EQUAL(edit_link, U("http://odatae2etest.azurewebsites.net/cpptest/DefaultService/Accounts(101)/MyGiftCard"));
}

//http://odatae2etest.azurewebsites.net/cpptest/DefaultService/Accounts(101)/MyPaymentInstruments?$expand=BillingStatements
TEST(edit_link_contained_in_contained_expand)
{
	auto json_reader = get_json_reader();
	VERIFY_IS_NOT_NULL(json_reader);

	::utility::string_t _entity_payload 
		= U("{\"@odata.context\":\"http://odatae2etest.azurewebsites.net/cpptest/DefaultService/$metadata#Accounts(103)/MyPaymentInstruments(PaymentInstrumentID,BillingStatements,BillingStatements(StatementID))/$entity\",\"PaymentInstrumentID\":103901,\"BillingStatements@odata.context\":\"http://odatae2etest.azurewebsites.net/cpptest/DefaultService/$metadata#Accounts(103)/MyPaymentInstruments(103901)/BillingStatements(StatementID)\", \
		\"BillingStatements\":[{\"StatementID\":103901001},{\"StatementID\":103901002}]}");

    ::web::json::value json_payload = web::json::value::parse(_entity_payload);
	auto return_payload = json_reader->deserilize(json_payload);
	auto entity_values = return_payload->get_values();

	VERIFY_ARE_EQUAL(entity_values.size(), 1);
	auto entity_value = std::dynamic_pointer_cast<odata_entity_value>(entity_values[0]);
	::utility::string_t edit_link;
	auto ok = entity_value->try_get(U("@odata.editLink"), edit_link);
	VERIFY_ARE_EQUAL(edit_link, U("http://odatae2etest.azurewebsites.net/cpptest/DefaultService/Accounts(103)/MyPaymentInstruments(103901)"));

	std::shared_ptr<odata_value> property_value;
	ok = entity_value->get_property_value(U("BillingStatements"), property_value);
	auto contained_values = std::dynamic_pointer_cast<odata_collection_value>(property_value);
	auto contained_value = std::dynamic_pointer_cast<odata_entity_value>(contained_values->get_collection_values()[0]);
	edit_link;
	ok = contained_value->try_get(U("@odata.editLink"), edit_link);
	VERIFY_ARE_EQUAL(edit_link, U("http://odatae2etest.azurewebsites.net/cpptest/DefaultService/Accounts(103)/MyPaymentInstruments(103901)/BillingStatements(103901001)"));
}

//http://odatae2etest.azurewebsites.net/cpptest/DefaultService/Accounts(103)/MyPaymentInstruments(103901)/BillingStatements
TEST(edit_link_contained_in_contained_property)
{
	auto json_reader = get_json_reader();
	VERIFY_IS_NOT_NULL(json_reader);

	::utility::string_t _entity_payload 
		= U("{\"@odata.context\":\"http://odatae2etest.azurewebsites.net/cpptest/DefaultService/$metadata#Accounts(103)/MyPaymentInstruments(103901)/BillingStatements/$entity\", \
		\"StatementID\":103901001,\"TransactionType\":\"OnlinePurchase\",\"TransactionDescription\":\"Digital goods: App\",\"Amount\":100.0}");

    ::web::json::value json_payload = web::json::value::parse(_entity_payload);
	auto return_payload = json_reader->deserilize(json_payload);
	auto entity_values = return_payload->get_values();

	VERIFY_ARE_EQUAL(entity_values.size(), 1);
	auto entity_value = std::dynamic_pointer_cast<odata_entity_value>(entity_values[0]);
	::utility::string_t edit_link;
	auto ok = entity_value->try_get(U("@odata.editLink"), edit_link);
	VERIFY_ARE_EQUAL(edit_link, U("http://odatae2etest.azurewebsites.net/cpptest/DefaultService/Accounts(103)/MyPaymentInstruments(103901)/BillingStatements(103901001)"));
}

//http://odatae2etest.azurewebsites.net/cpptest/DefaultService/Accounts(101)/MyPaymentInstruments(101901)/TheStoredPI
TEST(edit_link_none_contained_in_contained_property)
{
	auto json_reader = get_json_reader();
	VERIFY_IS_NOT_NULL(json_reader);

	::utility::string_t _entity_payload 
		= U("{\"@odata.context\":\"http://odatae2etest.azurewebsites.net/cpptest/DefaultService/$metadata#StoredPIs(StoredPIID)/$entity\",\"StoredPIID\":802}");

    ::web::json::value json_payload = web::json::value::parse(_entity_payload);
	auto return_payload = json_reader->deserilize(json_payload);
	auto entity_values = return_payload->get_values();

	VERIFY_ARE_EQUAL(entity_values.size(), 1);
	auto entity_value = std::dynamic_pointer_cast<odata_entity_value>(entity_values[0]);
	::utility::string_t edit_link;
	auto ok = entity_value->try_get(U("@odata.editLink"), edit_link);
	VERIFY_ARE_EQUAL(edit_link, U("http://odatae2etest.azurewebsites.net/cpptest/DefaultService/StoredPIs(802)"));
}

//http://odatae2etest.azurewebsites.net/cpptest/DefaultService/Accounts(101)/MyPaymentInstruments?$expand=TheStoredPI
TEST(edit_link_none_contained_in_contained)
{
	auto json_reader = get_json_reader();
	VERIFY_IS_NOT_NULL(json_reader);

	::utility::string_t _entity_payload 
		= U("{\"@odata.context\":\"http://odatae2etest.azurewebsites.net/cpptest/DefaultService/$metadata#Accounts(101)/MyPaymentInstruments(PaymentInstrumentID,TheStoredPI,TheStoredPI(StoredPIID))\", \
		\"value\":[{\"PaymentInstrumentID\":101901,\"TheStoredPI\":{\"StoredPIID\":802}}]}");

    ::web::json::value json_payload = web::json::value::parse(_entity_payload);
	auto return_payload = json_reader->deserilize(json_payload);
	auto entity_values = return_payload->get_values();

	VERIFY_ARE_EQUAL(entity_values.size(), 1);
	auto entity_value = std::dynamic_pointer_cast<odata_entity_value>(entity_values[0]);
	::utility::string_t edit_link;
	auto ok = entity_value->try_get(U("@odata.editLink"), edit_link);
	VERIFY_ARE_EQUAL(edit_link, U("http://odatae2etest.azurewebsites.net/cpptest/DefaultService/Accounts(101)/MyPaymentInstruments(101901)"));

	std::shared_ptr<odata_value> property_value;
	ok = entity_value->get_property_value(U("TheStoredPI"), property_value);
	auto contained_value = std::dynamic_pointer_cast<odata_entity_value>(property_value);
	edit_link;
	ok = contained_value->try_get(U("@odata.editLink"), edit_link);
	VERIFY_ARE_EQUAL(edit_link, U("http://odatae2etest.azurewebsites.net/cpptest/DefaultService/StoredPIs(802)"));
}


TEST(read_unicode_data_in_entity)
{
	auto json_reader = get_json_reader();
	VERIFY_IS_NOT_NULL(json_reader);

	::utility::string_t _entity_payload_escaped = U(
		"{\"@odata.context\":\"http://odatae2etest.azurewebsites.net/cpptest/DefaultService/$metadata#Accounts/$entity\", \
		\"@odata.editLink\":\"http://odatae2etest.azurewebsites.net/cpptest/DefaultService/Accounts(101)\", \
		\"AccountID\":101,\"CountryRegion\":\"\u4e0a\u6d77\",\"AccountInfo\":{\"FirstName\":\"Alex\",\"LastName\":\"Green\"}}"
		);
	
    ::web::json::value json_payload_escaped = web::json::value::parse(_entity_payload_escaped);
	auto return_payload_escaped = json_reader->deserilize(json_payload_escaped);
	auto entity_values = return_payload_escaped->get_values();

	VERIFY_ARE_EQUAL(entity_values.size(), 1);
	auto entity_value = std::dynamic_pointer_cast<odata_entity_value>(entity_values[0]);
	::utility::string_t countryregion;
	auto ok = entity_value->try_get(U("CountryRegion"), countryregion);
	VERIFY_ARE_EQUAL(countryregion, U("上海"));

	::utility::string_t _entity_payload_unescaped = U(
		"{\"@odata.context\":\"http://odatae2etest.azurewebsites.net/cpptest/DefaultService/$metadata#Accounts/$entity\", \
		\"@odata.editLink\":\"http://odatae2etest.azurewebsites.net/cpptest/DefaultService/Accounts(101)\", \
		\"AccountID\":101,\"CountryRegion\":\"上海\",\"AccountInfo\":{\"FirstName\":\"Alex\",\"LastName\":\"Green\"}}"
		);

	::web::json::value json_payload_unescaped = web::json::value::parse(_entity_payload_unescaped);
	auto return_payload_unescaped = json_reader->deserilize(json_payload_unescaped);
	entity_values = return_payload_unescaped->get_values();

	VERIFY_ARE_EQUAL(entity_values.size(), 1);
	entity_value = std::dynamic_pointer_cast<odata_entity_value>(entity_values[0]);
	ok = entity_value->try_get(U("CountryRegion"), countryregion);
	VERIFY_ARE_EQUAL(countryregion, U("上海"));
}


//
// to do add read all primitive type data test

TEST(read_binary_data)
{
	auto json_reader = get_json_reader();
	VERIFY_IS_NOT_NULL(json_reader);

	::utility::string_t _entity_payload = U("{\"@odata.context\":\"http://odatae2etest.azurewebsites.net/cpptest/DefaultService/$metadata#Products/$entity\", \
		\"@odata.editLink\":\"http://odatae2etest.azurewebsites.net/cpptest/DefaultService/Products(5)\", \
		\"ProductID\":5,\"Picture\":\"UUJBQkFDWDEyKjEyMzQ1Njc4OTBhYmNkZQ==\"}");

	::web::json::value json_payload = web::json::value::parse(_entity_payload);
	auto return_payload = json_reader->deserilize(json_payload);
	auto entity_values = return_payload->get_values();

	VERIFY_ARE_EQUAL(entity_values.size(), 1);
	auto entity_value = std::dynamic_pointer_cast<odata_entity_value>(entity_values[0]);

    std::shared_ptr<odata_value> property_value;
	entity_value->get_property_value(U("Picture"), property_value);
	auto primitive_value = std::dynamic_pointer_cast<odata_primitive_value>(property_value); 
	auto binary_output = primitive_value->as<std::vector<unsigned char>>();
	VERIFY_ARE_EQUAL(binary_output[7], '1');
	VERIFY_ARE_EQUAL(binary_output[9], '*');
}

}

}}}