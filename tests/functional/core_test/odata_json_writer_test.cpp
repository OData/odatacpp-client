//---------------------------------------------------------------------
// <copyright file="odata_json_writer_test.cpp" company="Microsoft">
//      Copyright (C) Microsoft Corporation. All rights reserved. See License.txt in the project root for license information.
// </copyright>
//---------------------------------------------------------------------

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
		return ::odata::make_shared<odata_json_writer>(model);
	}

	return nullptr;
}

typedef std::unordered_map<::odata::string_t, ::odata::string_t> map_type;
const map_type::value_type init_test_values[] =
{
	map_type::value_type(_XPLATSTR("single_entity"), _XPLATSTR("{\"AccountID\":100,\"AccountInfo\":{\"FirstName\":\"Leo\",\"LastName\":\"Hu\"},\"CountryRegion\":\"China\"}")),

	map_type::value_type(_XPLATSTR("collection_of_entity"), _XPLATSTR("[{\"AccountID\":100,\"CountryRegion\":\"China\"},{\"AccountID\":200,\"CountryRegion\":\"USA\"},{\"AccountID\":300,\"CountryRegion\":\"JP\"}]")),

#ifdef __APPLE__
	map_type::value_type(_XPLATSTR("unicode_data_in_entity"), _XPLATSTR("\"CountryRegion\":\"上海\",\"AccountID\":100}")),
#else
	map_type::value_type(_XPLATSTR("unicode_data_in_entity"), _XPLATSTR("{\"AccountID\":100,\"CountryRegion\":\"上海\"}")),
#endif
	map_type::value_type(_XPLATSTR("single_complex"), _XPLATSTR("{\"FirstName\":\"Leo\",\"LastName\":\"Hu\"}")),
	map_type::value_type(_XPLATSTR("collection_of_complex"), _XPLATSTR("[{\"FirstName\":\"Leo\",\"LastName\":\"Hu\"},{\"FirstName\":\"Tian\",\"LastName\":\"Ouyang\"}]")),
	map_type::value_type(_XPLATSTR("single_primitive"), _XPLATSTR("100")),
	map_type::value_type(_XPLATSTR("max_i16"), _XPLATSTR("32767")),
	map_type::value_type(_XPLATSTR("min_i16"), _XPLATSTR("-32768")),
	map_type::value_type(_XPLATSTR("max_i32"), _XPLATSTR("2147483647")),
	map_type::value_type(_XPLATSTR("min_i32"), _XPLATSTR("-2147483648")),
	map_type::value_type(_XPLATSTR("max_i64"), _XPLATSTR("9223372036854775807")),
	map_type::value_type(_XPLATSTR("min_i64"), _XPLATSTR("-9223372036854775808")),
	map_type::value_type(_XPLATSTR("primitive_bool_true"), _XPLATSTR("true")),
	map_type::value_type(_XPLATSTR("primitive_bool_false"), _XPLATSTR("false")),
	map_type::value_type(_XPLATSTR("primitive_sbyte"), _XPLATSTR("234")),
	map_type::value_type(_XPLATSTR("primitive_byte"), _XPLATSTR("123")),
	map_type::value_type(_XPLATSTR("primitive_datetime"), _XPLATSTR("\"2013-12-31T00:00:00Z\"")),
	map_type::value_type(_XPLATSTR("primitive_duration"), _XPLATSTR("\"PT38M38S\"")),
	map_type::value_type(_XPLATSTR("primitive_binary"), _XPLATSTR("\"UUJBQkFDWDEyKlhjdmpr\"")),
	map_type::value_type(_XPLATSTR("collection_of_primitive"), _XPLATSTR("[100,500,400,300,200]")),
	map_type::value_type(_XPLATSTR("single_enum"), _XPLATSTR("\"Read\"")),
	map_type::value_type(_XPLATSTR("collection_of_enum"), _XPLATSTR("[\"Read\",\"Write\",\"ReadWrite\"]")),
};

const static map_type test_data_map(init_test_values, init_test_values + 22);

static bool check_writer_result(const ::odata::string_t& check_key, const ::odata::string_t& check_value)
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

	auto complex_value = ::odata::make_shared<odata_complex_value>(model->find_complex_type(_XPLATSTR("AccountInfo")));
	complex_value->set_value(_XPLATSTR("FirstName"), _XPLATSTR("Leo"));
	complex_value->set_value(_XPLATSTR("LastName"), _XPLATSTR("Hu"));

	auto entity_value = ::odata::make_shared<odata_entity_value>(model->find_entity_type(_XPLATSTR("Account")));
	entity_value->set_value(_XPLATSTR("AccountID"), (int32_t)100);
	entity_value->set_value(_XPLATSTR("CountryRegion"), _XPLATSTR("China"));
	entity_value->set_value(_XPLATSTR("AccountInfo"), complex_value);

	auto json_string = json_writer->serialize(entity_value).to_string();
	bool pass = check_writer_result(_XPLATSTR("single_entity"), json_string);
	VERIFY_ARE_EQUAL(pass, true);
}

TEST(collection_of_entity)
{
	auto json_writer = get_json_writer();
	VERIFY_IS_NOT_NULL(json_writer);
	auto model = get_test_model();

	auto entity_value_1 = ::odata::make_shared<odata_entity_value>(model->find_entity_type(_XPLATSTR("Account")));
	entity_value_1->set_value(_XPLATSTR("AccountID"), (int32_t)100);
	entity_value_1->set_value(_XPLATSTR("CountryRegion"), _XPLATSTR("China"));

	auto entity_value_2 = ::odata::make_shared<odata_entity_value>(model->find_entity_type(_XPLATSTR("Account")));
	entity_value_2->set_value(_XPLATSTR("AccountID"), (int32_t)200);
	entity_value_2->set_value(_XPLATSTR("CountryRegion"), _XPLATSTR("USA"));

	auto entity_value_3 = ::odata::make_shared<odata_entity_value>(model->find_entity_type(_XPLATSTR("Account")));
	entity_value_3->set_value(_XPLATSTR("AccountID"), (int32_t)300);
	entity_value_3->set_value(_XPLATSTR("CountryRegion"), _XPLATSTR("JP"));

	auto collection_type = ::odata::make_shared<edm_collection_type>(_XPLATSTR("entity values"), model->find_entity_type(_XPLATSTR("Account")));
	auto collection_value = ::odata::make_shared<odata_collection_value>(collection_type);

	collection_value->add_collection_value(entity_value_1);
	collection_value->add_collection_value(entity_value_2);
	collection_value->add_collection_value(entity_value_3);

	auto json_string = json_writer->serialize(collection_value).to_string();
	bool pass = check_writer_result(_XPLATSTR("collection_of_entity"), json_string);
	VERIFY_ARE_EQUAL(pass, true);
}

TEST(single_complex)
{
	auto json_writer = get_json_writer();
	VERIFY_IS_NOT_NULL(json_writer);
	auto model = get_test_model();

	auto complex_value = ::odata::make_shared<odata_complex_value>(model->find_complex_type(_XPLATSTR("AccountInfo")));
	complex_value->set_value(_XPLATSTR("FirstName"), _XPLATSTR("Leo"));
	complex_value->set_value(_XPLATSTR("LastName"), _XPLATSTR("Hu"));

	auto json_string = json_writer->serialize(complex_value).to_string();
	bool pass = check_writer_result(_XPLATSTR("single_complex"), json_string);
	VERIFY_ARE_EQUAL(pass, true);
}

TEST(collection_of_complex)
{
	auto json_writer = get_json_writer();
	VERIFY_IS_NOT_NULL(json_writer);
	auto model = get_test_model();

	auto complex_value_1 = ::odata::make_shared<odata_complex_value>(model->find_complex_type(_XPLATSTR("AccountInfo")));
	complex_value_1->set_value(_XPLATSTR("FirstName"), _XPLATSTR("Leo"));
	complex_value_1->set_value(_XPLATSTR("LastName"), _XPLATSTR("Hu"));

	auto complex_value_2 = ::odata::make_shared<odata_complex_value>(model->find_complex_type(_XPLATSTR("AccountInfo")));
	complex_value_2->set_value(_XPLATSTR("FirstName"), _XPLATSTR("Tian"));
	complex_value_2->set_value(_XPLATSTR("LastName"), _XPLATSTR("Ouyang"));

	auto collection_type = ::odata::make_shared<edm_collection_type>(_XPLATSTR("complex values"), model->find_complex_type(_XPLATSTR("AccountInfo")));
	auto collection_value = ::odata::make_shared<odata_collection_value>(collection_type);
	collection_value->add_collection_value(complex_value_1);
	collection_value->add_collection_value(complex_value_2);

	auto json_string = json_writer->serialize(collection_value).to_string();
	bool pass = check_writer_result(_XPLATSTR("collection_of_complex"), json_string);
	VERIFY_ARE_EQUAL(pass, true);
}

TEST(single_primitive)
{
	auto json_writer = get_json_writer();
	VERIFY_IS_NOT_NULL(json_writer);
	auto model = get_test_model();

	auto int32_value = ::odata::make_shared<odata_primitive_value>(edm_primitive_type::INT32(), ::utility::conversions::print_string(100));

	auto json_string = json_writer->serialize(int32_value).to_string();
	bool pass = check_writer_result(_XPLATSTR("single_primitive"), json_string);
	VERIFY_ARE_EQUAL(pass, true);
}

TEST(single_primitive_int16_min)
{
	auto json_writer = get_json_writer();
	VERIFY_IS_NOT_NULL(json_writer);
	auto model = get_test_model();

	int16_t value = -32768;
	auto odata_value = ::odata::make_shared<odata_primitive_value>(edm_primitive_type::INT16(), ::utility::conversions::print_string(value));
	auto json_string = json_writer->serialize(odata_value).to_string();
	bool pass = check_writer_result(_XPLATSTR("min_i16"), json_string);
	VERIFY_ARE_EQUAL(pass, true);
}

TEST(single_primitive_int16_max)
{
	auto json_writer = get_json_writer();
	VERIFY_IS_NOT_NULL(json_writer);
	auto model = get_test_model();

	int16_t value = 32767;
	auto odata_value = ::odata::make_shared<odata_primitive_value>(edm_primitive_type::INT16(), ::utility::conversions::print_string(value));
	auto json_string = json_writer->serialize(odata_value).to_string();
	bool pass = check_writer_result(_XPLATSTR("max_i16"), json_string);
	VERIFY_ARE_EQUAL(pass, true);
}

TEST(single_primitive_int32_max)
{
	auto json_writer = get_json_writer();
	VERIFY_IS_NOT_NULL(json_writer);
	auto model = get_test_model();

	int32_t value = 2147483647;
	auto odata_value = ::odata::make_shared<odata_primitive_value>(edm_primitive_type::INT32(), ::utility::conversions::print_string(value));
	auto json_string = json_writer->serialize(odata_value).to_string();
	bool pass = check_writer_result(_XPLATSTR("max_i32"), json_string);
	VERIFY_ARE_EQUAL(pass, true);
}

TEST(single_primitive_int32_min)
{
	auto json_writer = get_json_writer();
	VERIFY_IS_NOT_NULL(json_writer);
	auto model = get_test_model();

	int32_t value = -2147483648;
	auto odata_value = ::odata::make_shared<odata_primitive_value>(edm_primitive_type::INT32(), ::utility::conversions::print_string(value));
	auto json_string = json_writer->serialize(odata_value).to_string();
	bool pass = check_writer_result(_XPLATSTR("min_i32"), json_string);
	VERIFY_ARE_EQUAL(pass, true);
}

TEST(single_primitive_int64_max)
{
	auto json_writer = get_json_writer();
	VERIFY_IS_NOT_NULL(json_writer);
	auto model = get_test_model();

	int64_t value = 9223372036854775807;
	auto odata_value = ::odata::make_shared<odata_primitive_value>(edm_primitive_type::INT64(), ::utility::conversions::print_string(value));
	auto json_string = json_writer->serialize(odata_value).to_string();
	bool pass = check_writer_result(_XPLATSTR("max_i64"), json_string);
	VERIFY_ARE_EQUAL(pass, true);
}

TEST(single_primitive_int64_min)
{
	auto json_writer = get_json_writer();
	VERIFY_IS_NOT_NULL(json_writer);
	auto model = get_test_model();

	int64_t value = -9223372036854775808;
	auto odata_value = ::odata::make_shared<odata_primitive_value>(edm_primitive_type::INT64(), ::utility::conversions::print_string(value));
	auto json_string = json_writer->serialize(odata_value).to_string();
	bool pass = check_writer_result(_XPLATSTR("min_i64"), json_string);
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
//	bool pass = check_writer_result(_XPLATSTR("primitive_float"), json_string);
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
	bool pass = check_writer_result(_XPLATSTR("primitive_bool_true"), json_string);
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
	bool pass = check_writer_result(_XPLATSTR("primitive_bool_false"), json_string);
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
	bool pass = check_writer_result(_XPLATSTR("primitive_binary"), json_string);
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
	bool pass = check_writer_result(_XPLATSTR("primitive_sbyte"), json_string);
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
	bool pass = check_writer_result(_XPLATSTR("primitive_byte"), json_string);
	VERIFY_ARE_EQUAL(pass, true);
}

TEST(single_primitive_duration)
{
	auto json_writer = get_json_writer();
	VERIFY_IS_NOT_NULL(json_writer);
	auto model = get_test_model();

	auto value = ::utility::datetime::from_string(_XPLATSTR("2013-12-31T00:00:00Z"), ::utility::datetime::date_format::ISO_8601);
	auto odata_value = odata_primitive_value::make_primitive_value(value);
	auto json_string = json_writer->serialize(odata_value).to_string();
	bool pass = check_writer_result(_XPLATSTR("primitive_datetime"), json_string);
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
	bool pass = check_writer_result(_XPLATSTR("primitive_duration"), json_string);
	VERIFY_ARE_EQUAL(pass, true);
}

TEST(collection_of_primitive)
{
	auto json_writer = get_json_writer();
	VERIFY_IS_NOT_NULL(json_writer);
	auto model = get_test_model();

	auto int32_value_1 = ::odata::make_shared<odata_primitive_value>(edm_primitive_type::INT32(), ::utility::conversions::print_string(100));
	auto int32_value_2 = ::odata::make_shared<odata_primitive_value>(edm_primitive_type::INT32(), ::utility::conversions::print_string(500));
	auto int32_value_3 = ::odata::make_shared<odata_primitive_value>(edm_primitive_type::INT32(), ::utility::conversions::print_string(400));
	auto int32_value_4 = ::odata::make_shared<odata_primitive_value>(edm_primitive_type::INT32(), ::utility::conversions::print_string(300));
	auto int32_value_5 = ::odata::make_shared<odata_primitive_value>(edm_primitive_type::INT32(), ::utility::conversions::print_string(200));

	auto collection_type = ::odata::make_shared<edm_collection_type>(_XPLATSTR("primitive values"), edm_primitive_type::INT32());
	auto collection_value = ::odata::make_shared<odata_collection_value>(collection_type);
	collection_value->add_collection_value(int32_value_1);
	collection_value->add_collection_value(int32_value_2);
	collection_value->add_collection_value(int32_value_3);
	collection_value->add_collection_value(int32_value_4);
	collection_value->add_collection_value(int32_value_5);

	auto json_string = json_writer->serialize(collection_value).to_string();
	bool pass = check_writer_result(_XPLATSTR("collection_of_primitive"), json_string);
	VERIFY_ARE_EQUAL(pass, true);
}

TEST(single_enum)
{
	auto json_writer = get_json_writer();
	VERIFY_IS_NOT_NULL(json_writer);
	auto model = get_test_model();

	auto enum_value = ::odata::make_shared<odata_enum_value>(model->find_enum_type(_XPLATSTR("AccessLevel")), _XPLATSTR("Read"));

	auto json_string = json_writer->serialize(enum_value).to_string();
	bool pass = check_writer_result(_XPLATSTR("single_enum"), json_string);
	VERIFY_ARE_EQUAL(pass, true);
}

TEST(collection_of_enum)
{
	auto json_writer = get_json_writer();
	VERIFY_IS_NOT_NULL(json_writer);
	auto model = get_test_model();

	auto enum_value_1 = ::odata::make_shared<odata_enum_value>(model->find_enum_type(_XPLATSTR("AccessLevel")), _XPLATSTR("Read"));
	auto enum_value_2 = ::odata::make_shared<odata_enum_value>(model->find_enum_type(_XPLATSTR("AccessLevel")), _XPLATSTR("Write"));
	auto enum_value_3 = ::odata::make_shared<odata_enum_value>(model->find_enum_type(_XPLATSTR("AccessLevel")), _XPLATSTR("ReadWrite"));

	auto collection_type = ::odata::make_shared<edm_collection_type>(_XPLATSTR("enum values"), model->find_enum_type(_XPLATSTR("AccessLevel")));
	auto collection_value = ::odata::make_shared<odata_collection_value>(collection_type);
	collection_value->add_collection_value(enum_value_1);
	collection_value->add_collection_value(enum_value_2);
	collection_value->add_collection_value(enum_value_3);

	auto json_string = json_writer->serialize(collection_value).to_string();
	bool pass = check_writer_result(_XPLATSTR("collection_of_enum"), json_string);
	VERIFY_ARE_EQUAL(pass, true);
}

TEST(unicode_data_in_entity)
{
	auto json_writer = get_json_writer();
	VERIFY_IS_NOT_NULL(json_writer);
	auto model = get_test_model();

	auto entity_value = ::odata::make_shared<odata_entity_value>(model->find_entity_type(_XPLATSTR("Account")));
	entity_value->set_value(_XPLATSTR("AccountID"), (int32_t)100);
	entity_value->set_value(_XPLATSTR("CountryRegion"), _XPLATSTR("上海"));

	auto json_string = json_writer->serialize(entity_value).serialize();
	bool pass = check_writer_result(_XPLATSTR("unicode_data_in_entity"), json_string);

	VERIFY_ARE_EQUAL(pass, true);
}

}

}}}