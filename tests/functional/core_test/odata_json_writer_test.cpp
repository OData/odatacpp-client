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
#include "odata/core/odata_json_writer.h"

using namespace ::odata::core;
using namespace ::odata::edm;

namespace tests { namespace functional { namespace _odata {

static std::shared_ptr<odata_json_writer> get_json_writer()
{
	auto model = get_test_model();
	if (model)
	{
		return std::make_shared<odata_json_writer>(model);
	}

	return nullptr;
}

typedef std::unordered_map<::utility::string_t, ::utility::string_t> map_type;
const map_type::value_type init_test_values[] = 
{
	map_type::value_type(U("single_entity"), U("{\"AccountID\":100,\"AccountInfo\":{\"FirstName\":\"Leo\",\"LastName\":\"Hu\"},\"CountryRegion\":\"China\"}")),

	map_type::value_type(U("collection_of_entity"), U("[{\"AccountID\":100,\"CountryRegion\":\"China\"},{\"AccountID\":200,\"CountryRegion\":\"USA\"},{\"AccountID\":300,\"CountryRegion\":\"JP\"}]")),
    
#ifdef __APPLE__
	map_type::value_type(U("unicode_data_in_entity"), U("\"CountryRegion\":\"上海\",\"AccountID\":100}")),
#else
	map_type::value_type(U("unicode_data_in_entity"), U("{\"AccountID\":100,\"CountryRegion\":\"上海\"}")),
#endif
	map_type::value_type(U("single_complex"), U("{\"FirstName\":\"Leo\",\"LastName\":\"Hu\"}")),
	map_type::value_type(U("collection_of_complex"), U("[{\"FirstName\":\"Leo\",\"LastName\":\"Hu\"},{\"FirstName\":\"Tian\",\"LastName\":\"Ouyang\"}]")),
	map_type::value_type(U("single_primitive"), U("100")),
	map_type::value_type(U("max_i16"), U("32767")),
	map_type::value_type(U("min_i16"), U("-32768")),
	map_type::value_type(U("max_i32"), U("2147483647")),
	map_type::value_type(U("min_i32"), U("-2147483648")),
	map_type::value_type(U("max_i64"), U("9223372036854775807")),
	map_type::value_type(U("min_i64"), U("-9223372036854775808")),
	map_type::value_type(U("primitive_bool_true"), U("true")),
	map_type::value_type(U("primitive_bool_false"), U("false")),
	map_type::value_type(U("primitive_sbyte"), U("234")),
	map_type::value_type(U("primitive_byte"), U("123")),
	map_type::value_type(U("primitive_datetime"), U("\"2013-12-31T00:00:00Z\"")),
	map_type::value_type(U("primitive_duration"), U("\"PT38M38S\"")),
	map_type::value_type(U("primitive_binary"), U("\"UUJBQkFDWDEyKlhjdmpr\"")),
	map_type::value_type(U("collection_of_primitive"), U("[100,500,400,300,200]")),
	map_type::value_type(U("single_enum"), U("\"Read\"")),
	map_type::value_type(U("collection_of_enum"), U("[\"Read\",\"Write\",\"ReadWrite\"]")),
};

const static map_type test_data_map(init_test_values, init_test_values + 22);

static bool check_writer_result(const ::utility::string_t& check_key, const ::utility::string_t& check_value)
{
	auto find_iter = test_data_map.find(check_key);
	if (find_iter != test_data_map.end())
	{
		return find_iter->second == check_value ? true : false;
	}

	return false;
}

SUITE(odata_json_writer_test_cases)
{

TEST(single_entity)
{
	auto json_writer = get_json_writer();
	VERIFY_IS_NOT_NULL(json_writer);
	auto model = get_test_model();

	auto complex_value = std::make_shared<odata_complex_value>(model->find_complex_type(U("AccountInfo")));
	complex_value->set_value(U("FirstName"), U("Leo"));
	complex_value->set_value(U("LastName"), U("Hu"));

	auto entity_value = std::make_shared<odata_entity_value>(model->find_entity_type(U("Account")));
	entity_value->set_value(U("AccountID"), (int32_t)100);
	entity_value->set_value(U("CountryRegion"), U("China"));
	entity_value->set_value(U("AccountInfo"), complex_value);

	auto json_string = json_writer->serialize(entity_value).to_string();
	bool pass = check_writer_result(U("single_entity"), json_string);
	VERIFY_ARE_EQUAL(pass, true);
}

TEST(collection_of_entity)
{
	auto json_writer = get_json_writer();
	VERIFY_IS_NOT_NULL(json_writer);
	auto model = get_test_model();

	auto entity_value_1 = std::make_shared<odata_entity_value>(model->find_entity_type(U("Account")));
	entity_value_1->set_value(U("AccountID"), (int32_t)100);
	entity_value_1->set_value(U("CountryRegion"), U("China"));

	auto entity_value_2 = std::make_shared<odata_entity_value>(model->find_entity_type(U("Account")));
	entity_value_2->set_value(U("AccountID"), (int32_t)200);
	entity_value_2->set_value(U("CountryRegion"), U("USA"));

	auto entity_value_3 = std::make_shared<odata_entity_value>(model->find_entity_type(U("Account")));
	entity_value_3->set_value(U("AccountID"), (int32_t)300);
	entity_value_3->set_value(U("CountryRegion"), U("JP"));

	auto collection_type = std::make_shared<edm_collection_type>(U("entity values"), model->find_entity_type(U("Account")));
	auto collection_value = std::make_shared<odata_collection_value>(collection_type);

	collection_value->add_collection_value(entity_value_1);
	collection_value->add_collection_value(entity_value_2);
	collection_value->add_collection_value(entity_value_3);

	auto json_string = json_writer->serialize(collection_value).to_string();
	bool pass = check_writer_result(U("collection_of_entity"), json_string);
	VERIFY_ARE_EQUAL(pass, true);
}

TEST(single_complex)
{
	auto json_writer = get_json_writer();
	VERIFY_IS_NOT_NULL(json_writer);
	auto model = get_test_model();

	auto complex_value = std::make_shared<odata_complex_value>(model->find_complex_type(U("AccountInfo")));
	complex_value->set_value(U("FirstName"), U("Leo"));
	complex_value->set_value(U("LastName"), U("Hu"));

	auto json_string = json_writer->serialize(complex_value).to_string();
	bool pass = check_writer_result(U("single_complex"), json_string);
	VERIFY_ARE_EQUAL(pass, true);
}

TEST(collection_of_complex)
{
	auto json_writer = get_json_writer();
	VERIFY_IS_NOT_NULL(json_writer);
	auto model = get_test_model();

	auto complex_value_1 = std::make_shared<odata_complex_value>(model->find_complex_type(U("AccountInfo")));
	complex_value_1->set_value(U("FirstName"), U("Leo"));
	complex_value_1->set_value(U("LastName"), U("Hu"));

	auto complex_value_2 = std::make_shared<odata_complex_value>(model->find_complex_type(U("AccountInfo")));
	complex_value_2->set_value(U("FirstName"), U("Tian"));
	complex_value_2->set_value(U("LastName"), U("Ouyang"));

	auto collection_type = std::make_shared<edm_collection_type>(U("complex values"), model->find_complex_type(U("AccountInfo")));
	auto collection_value = std::make_shared<odata_collection_value>(collection_type);
	collection_value->add_collection_value(complex_value_1);
	collection_value->add_collection_value(complex_value_2);

	auto json_string = json_writer->serialize(collection_value).to_string();
	bool pass = check_writer_result(U("collection_of_complex"), json_string);
	VERIFY_ARE_EQUAL(pass, true);
}

TEST(single_primitive)
{
	auto json_writer = get_json_writer();
	VERIFY_IS_NOT_NULL(json_writer);
	auto model = get_test_model();

	auto int32_value = std::make_shared<odata_primitive_value>(edm_primitive_type::INT32(), ::utility::conversions::print_string(100));

	auto json_string = json_writer->serialize(int32_value).to_string();
	bool pass = check_writer_result(U("single_primitive"), json_string);
	VERIFY_ARE_EQUAL(pass, true);
}

TEST(single_primitive_int16_min)
{
	auto json_writer = get_json_writer();
	VERIFY_IS_NOT_NULL(json_writer);
	auto model = get_test_model();

	int16_t value = -32768;
	auto odata_value = std::make_shared<odata_primitive_value>(edm_primitive_type::INT16(), ::utility::conversions::print_string(value));
	auto json_string = json_writer->serialize(odata_value).to_string();
	bool pass = check_writer_result(U("min_i16"), json_string);
	VERIFY_ARE_EQUAL(pass, true);
}

TEST(single_primitive_int16_max)
{
	auto json_writer = get_json_writer();
	VERIFY_IS_NOT_NULL(json_writer);
	auto model = get_test_model();

	int16_t value = 32767;
	auto odata_value = std::make_shared<odata_primitive_value>(edm_primitive_type::INT16(), ::utility::conversions::print_string(value));
	auto json_string = json_writer->serialize(odata_value).to_string();
	bool pass = check_writer_result(U("max_i16"), json_string);
	VERIFY_ARE_EQUAL(pass, true);
}

TEST(single_primitive_int32_max)
{
	auto json_writer = get_json_writer();
	VERIFY_IS_NOT_NULL(json_writer);
	auto model = get_test_model();

	int32_t value = 2147483647;
	auto odata_value = std::make_shared<odata_primitive_value>(edm_primitive_type::INT32(), ::utility::conversions::print_string(value));
	auto json_string = json_writer->serialize(odata_value).to_string();
	bool pass = check_writer_result(U("max_i32"), json_string);
	VERIFY_ARE_EQUAL(pass, true);
}

TEST(single_primitive_int32_min)
{
	auto json_writer = get_json_writer();
	VERIFY_IS_NOT_NULL(json_writer);
	auto model = get_test_model();

	int32_t value = -2147483648;
	auto odata_value = std::make_shared<odata_primitive_value>(edm_primitive_type::INT32(), ::utility::conversions::print_string(value));
	auto json_string = json_writer->serialize(odata_value).to_string();
	bool pass = check_writer_result(U("min_i32"), json_string);
	VERIFY_ARE_EQUAL(pass, true);
}

TEST(single_primitive_int64_max)
{
	auto json_writer = get_json_writer();
	VERIFY_IS_NOT_NULL(json_writer);
	auto model = get_test_model();

	int64_t value = 9223372036854775807;
	auto odata_value = std::make_shared<odata_primitive_value>(edm_primitive_type::INT64(), ::utility::conversions::print_string(value));
	auto json_string = json_writer->serialize(odata_value).to_string();
	bool pass = check_writer_result(U("max_i64"), json_string);
	VERIFY_ARE_EQUAL(pass, true);
}

TEST(single_primitive_int64_min)
{
	auto json_writer = get_json_writer();
	VERIFY_IS_NOT_NULL(json_writer);
	auto model = get_test_model();

	int64_t value = -9223372036854775808;
	auto odata_value = std::make_shared<odata_primitive_value>(edm_primitive_type::INT64(), ::utility::conversions::print_string(value));
	auto json_string = json_writer->serialize(odata_value).to_string();
	bool pass = check_writer_result(U("min_i64"), json_string);
	VERIFY_ARE_EQUAL(pass, true);
}

//TEST(single_primitive_float)
//{
//	auto json_writer = get_json_writer();
//	VERIFY_IS_NOT_NULL(json_writer);
//	auto model = get_test_model();
//
//	float value = 123213.43;
//	auto odata_value = odata_primitive_value::make_primitive_value(value);
//	auto json_string = json_writer->serialize(odata_value).to_string();
//	bool pass = check_writer_result(U("primitive_float"), json_string);
//	VERIFY_ARE_EQUAL(pass, true);
//}
//
//TEST(single_primitive_double)
//{
//	auto json_writer = get_json_writer();
//	VERIFY_IS_NOT_NULL(json_writer);
//	auto model = get_test_model();
//
//}

TEST(single_primitive_bool_true)
{
	auto json_writer = get_json_writer();
	VERIFY_IS_NOT_NULL(json_writer);
	auto model = get_test_model();

	bool value = true;
	auto odata_value = odata_primitive_value::make_primitive_value(value);
	auto json_string = json_writer->serialize(odata_value).to_string();
	bool pass = check_writer_result(U("primitive_bool_true"), json_string);
	VERIFY_ARE_EQUAL(pass, true);
}

TEST(single_primitive_bool_false)
{
	auto json_writer = get_json_writer();
	VERIFY_IS_NOT_NULL(json_writer);
	auto model = get_test_model();

	bool value = false;
	auto odata_value = odata_primitive_value::make_primitive_value(value);
	auto json_string = json_writer->serialize(odata_value).to_string();
	bool pass = check_writer_result(U("primitive_bool_false"), json_string);
	VERIFY_ARE_EQUAL(pass, true);
}

TEST(single_primitive_binary)
{
	auto json_writer = get_json_writer();
	VERIFY_IS_NOT_NULL(json_writer);
	auto model = get_test_model();

	unsigned char ini_binary[] = {'Q', 'B', 'A', 'B', 'A', 'C', 'X', '1', '2', '*', 'X', 'c', 'v', 'j', 'k'};
	std::vector<unsigned char> value(ini_binary, ini_binary + 15);
	auto odata_value = odata_primitive_value::make_primitive_value(value);
    auto json_string = json_writer->serialize(odata_value).to_string();
	bool pass = check_writer_result(U("primitive_binary"), json_string);
	VERIFY_ARE_EQUAL(pass, true);
}

TEST(single_primitive_sbyte)
{
	auto json_writer = get_json_writer();
	VERIFY_IS_NOT_NULL(json_writer);
	auto model = get_test_model();

	unsigned char value = 234;
	auto odata_value = odata_primitive_value::make_primitive_value(value);
	auto json_string = json_writer->serialize(odata_value).to_string();
	bool pass = check_writer_result(U("primitive_sbyte"), json_string);
	VERIFY_ARE_EQUAL(pass, true);
}

TEST(single_primitive_byte)
{
	auto json_writer = get_json_writer();
	VERIFY_IS_NOT_NULL(json_writer);
	auto model = get_test_model();

	char value = 123;
	auto odata_value = odata_primitive_value::make_primitive_value(value);
	auto json_string = json_writer->serialize(odata_value).to_string();
	bool pass = check_writer_result(U("primitive_byte"), json_string);
	VERIFY_ARE_EQUAL(pass, true);
}

TEST(single_primitive_duration)
{
	auto json_writer = get_json_writer();
	VERIFY_IS_NOT_NULL(json_writer);
	auto model = get_test_model();

	auto value = ::utility::datetime::from_string(U("2013-12-31T00:00:00Z"), ::utility::datetime::date_format::ISO_8601);
	auto odata_value = odata_primitive_value::make_primitive_value(value);
	auto json_string = json_writer->serialize(odata_value).to_string();
	bool pass = check_writer_result(U("primitive_datetime"), json_string);
	VERIFY_ARE_EQUAL(pass, true);
}

TEST(single_primitive_datetime)
{
	auto json_writer = get_json_writer();
	VERIFY_IS_NOT_NULL(json_writer);
	auto model = get_test_model();

	::utility::seconds value(2318);
	auto odata_value = odata_primitive_value::make_primitive_value(value);
	auto json_string = json_writer->serialize(odata_value).to_string();
	bool pass = check_writer_result(U("primitive_duration"), json_string);
	VERIFY_ARE_EQUAL(pass, true);
}

TEST(collection_of_primitive)
{
	auto json_writer = get_json_writer();
	VERIFY_IS_NOT_NULL(json_writer);
	auto model = get_test_model();

	auto int32_value_1 = std::make_shared<odata_primitive_value>(edm_primitive_type::INT32(), ::utility::conversions::print_string(100));
	auto int32_value_2 = std::make_shared<odata_primitive_value>(edm_primitive_type::INT32(), ::utility::conversions::print_string(500));
	auto int32_value_3 = std::make_shared<odata_primitive_value>(edm_primitive_type::INT32(), ::utility::conversions::print_string(400));
	auto int32_value_4 = std::make_shared<odata_primitive_value>(edm_primitive_type::INT32(), ::utility::conversions::print_string(300));
	auto int32_value_5 = std::make_shared<odata_primitive_value>(edm_primitive_type::INT32(), ::utility::conversions::print_string(200));

	auto collection_type = std::make_shared<edm_collection_type>(U("primitive values"), edm_primitive_type::INT32());
	auto collection_value = std::make_shared<odata_collection_value>(collection_type);
	collection_value->add_collection_value(int32_value_1);
	collection_value->add_collection_value(int32_value_2);
	collection_value->add_collection_value(int32_value_3);
	collection_value->add_collection_value(int32_value_4);
	collection_value->add_collection_value(int32_value_5);

	auto json_string = json_writer->serialize(collection_value).to_string();
	bool pass = check_writer_result(U("collection_of_primitive"), json_string);
	VERIFY_ARE_EQUAL(pass, true);
}

TEST(single_enum)
{
	auto json_writer = get_json_writer();
	VERIFY_IS_NOT_NULL(json_writer);
	auto model = get_test_model();

	auto enum_value = std::make_shared<odata_enum_value>(model->find_enum_type(U("AccessLevel")), U("Read"));

	auto json_string = json_writer->serialize(enum_value).to_string();
	bool pass = check_writer_result(U("single_enum"), json_string);
	VERIFY_ARE_EQUAL(pass, true);
}

TEST(collection_of_enum)
{
	auto json_writer = get_json_writer();
	VERIFY_IS_NOT_NULL(json_writer);
	auto model = get_test_model();

	auto enum_value_1 = std::make_shared<odata_enum_value>(model->find_enum_type(U("AccessLevel")), U("Read"));
	auto enum_value_2 = std::make_shared<odata_enum_value>(model->find_enum_type(U("AccessLevel")), U("Write"));
	auto enum_value_3 = std::make_shared<odata_enum_value>(model->find_enum_type(U("AccessLevel")), U("ReadWrite"));

	auto collection_type = std::make_shared<edm_collection_type>(U("enum values"), model->find_enum_type(U("AccessLevel")));
	auto collection_value = std::make_shared<odata_collection_value>(collection_type);
	collection_value->add_collection_value(enum_value_1);
	collection_value->add_collection_value(enum_value_2);
	collection_value->add_collection_value(enum_value_3);

	auto json_string = json_writer->serialize(collection_value).to_string();
	bool pass = check_writer_result(U("collection_of_enum"), json_string);
	VERIFY_ARE_EQUAL(pass, true);
}

TEST(unicode_data_in_entity)
{
	auto json_writer = get_json_writer();
	VERIFY_IS_NOT_NULL(json_writer);
	auto model = get_test_model();

	auto entity_value = std::make_shared<odata_entity_value>(model->find_entity_type(U("Account")));
	entity_value->set_value(U("AccountID"), (int32_t)100);
	entity_value->set_value(U("CountryRegion"), U("上海"));

	auto json_string = json_writer->serialize(entity_value).serialize();
	bool pass = check_writer_result(U("unicode_data_in_entity"), json_string);

	VERIFY_ARE_EQUAL(pass, true);
}

}

}}}