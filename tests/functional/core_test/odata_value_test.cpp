//---------------------------------------------------------------------
// <copyright file="odata_value_test.cpp" company="Microsoft">
//      Copyright (C) Microsoft Corporation. All rights reserved. See License.txt in the project root for license information.
// </copyright>
//---------------------------------------------------------------------

#include "../odata_tests.h"
#include "cpprest/json.h"
#include "odata/edm/odata_edm.h"
#include "odata/edm/edm_model_reader.h"

using namespace ::odata::client;
using namespace ::odata::edm;
using namespace ::odata::core;

namespace tests { namespace functional { namespace _odata {

SUITE(odata_value_test_cases)
{

TEST(primitive_value)
{
	auto int32 = odata_primitive_value::make_primitive_value(-32);
	VERIFY_IS_NOT_NULL(int32);
	VERIFY_ARE_EQUAL(int32->get_value_type()->get_type_kind(), edm_type_kind_t::Primitive);
	auto primitive_type = std::dynamic_pointer_cast<edm_primitive_type>(int32->get_value_type());
	VERIFY_IS_NOT_NULL(primitive_type);
	VERIFY_ARE_EQUAL(primitive_type->get_primitive_kind(), edm_primitive_type_kind_t::Int32);

	// make a binary data
	unsigned char ini_binary[] = {'Q', 'B', 'A', 'B', 'A', 'C', 'X', '1', '2', '*', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'p'};
	std::vector<unsigned char> binary(ini_binary, ini_binary + 25);
	auto binary_input = odata_primitive_value::make_primitive_value(binary);
	VERIFY_ARE_EQUAL(binary_input->to_string(), _XPLATSTR("UUJBQkFDWDEyKjEyMzQ1Njc4OTBhYmNkZQ=="));
	auto binary_output = binary_input->as<std::vector<unsigned char>>();
	VERIFY_ARE_EQUAL(binary_output[7], '1');

	//int64
	auto i64 = odata_primitive_value::make_primitive_value((int64_t)-123123213);
	VERIFY_ARE_EQUAL(i64->get_value_type()->get_type_kind(), edm_type_kind_t::Primitive);
	primitive_type = std::dynamic_pointer_cast<edm_primitive_type>(i64->get_value_type());
	VERIFY_ARE_EQUAL(primitive_type->get_primitive_kind(), edm_primitive_type_kind_t::Int64);
	VERIFY_ARE_EQUAL(i64->as<int64_t>(), -123123213);

	//int16
	auto i16 = odata_primitive_value::make_primitive_value((int16_t)-256);
	VERIFY_ARE_EQUAL(i16->get_value_type()->get_type_kind(), edm_type_kind_t::Primitive);
	primitive_type = std::dynamic_pointer_cast<edm_primitive_type>(i16->get_value_type());
	VERIFY_ARE_EQUAL(primitive_type->get_primitive_kind(), edm_primitive_type_kind_t::Int16);
	VERIFY_ARE_EQUAL(i16->as<int16_t>(), -256);

	//double
	auto double_v = odata_primitive_value::make_primitive_value((double)-12123.2312);
	VERIFY_ARE_EQUAL(double_v->get_value_type()->get_type_kind(), edm_type_kind_t::Primitive);
	primitive_type = std::dynamic_pointer_cast<edm_primitive_type>(double_v->get_value_type());
	VERIFY_ARE_EQUAL(primitive_type->get_primitive_kind(), edm_primitive_type_kind_t::Double);
	VERIFY_ARE_EQUAL(double_v->as<double>(), -12123.2312);

	//bool
	bool v = false;
	auto bool_v = odata_primitive_value::make_primitive_value(v);
	VERIFY_ARE_EQUAL(int32->get_value_type()->get_type_kind(), edm_type_kind_t::Primitive);
	primitive_type = std::dynamic_pointer_cast<edm_primitive_type>(bool_v->get_value_type());
	VERIFY_ARE_EQUAL(primitive_type->get_primitive_kind(), edm_primitive_type_kind_t::Boolean);
	VERIFY_ARE_EQUAL(bool_v->as<bool>(), false);

	//date time
	::utility::datetime dt = ::utility::datetime::from_string(_XPLATSTR("2014-5-13T11:40:00Z"), ::utility::datetime::date_format::ISO_8601);
	auto date_v = odata_primitive_value::make_primitive_value(dt);
	VERIFY_ARE_EQUAL(date_v->get_value_type()->get_type_kind(), edm_type_kind_t::Primitive);
	primitive_type = std::dynamic_pointer_cast<edm_primitive_type>(date_v->get_value_type());
	VERIFY_ARE_EQUAL(primitive_type->get_primitive_kind(), edm_primitive_type_kind_t::DateTimeOffset);
	auto ck_dt = date_v->as<::utility::datetime>();
	VERIFY_ARE_EQUAL(ck_dt, dt);

	//duration
	::utility::seconds sd(3600);
	auto second_v = odata_primitive_value::make_primitive_value(sd);
	VERIFY_ARE_EQUAL(second_v->get_value_type()->get_type_kind(), edm_type_kind_t::Primitive);
	primitive_type = std::dynamic_pointer_cast<edm_primitive_type>(second_v->get_value_type());
	VERIFY_ARE_EQUAL(primitive_type->get_primitive_kind(), edm_primitive_type_kind_t::Duration);
	auto ck_second = second_v->as<::utility::seconds>();
    VERIFY_ARE_EQUAL(ck_second, sd);

	//float
	auto float_v = odata_primitive_value::make_primitive_value((float)-121.2312);
	VERIFY_ARE_EQUAL(float_v->get_value_type()->get_type_kind(), edm_type_kind_t::Primitive);
	primitive_type = std::dynamic_pointer_cast<edm_primitive_type>(float_v->get_value_type());
	VERIFY_ARE_EQUAL(primitive_type->get_primitive_kind(), edm_primitive_type_kind_t::Single);
	VERIFY_IS_TRUE(abs(float_v->as<float>() - -121.2312) < 0.00001);

	//string
	auto str_v = odata_primitive_value::make_primitive_value(_XPLATSTR("test string"));
	VERIFY_ARE_EQUAL(str_v->get_value_type()->get_type_kind(), edm_type_kind_t::Primitive);
	primitive_type = std::dynamic_pointer_cast<edm_primitive_type>(str_v->get_value_type());
	VERIFY_ARE_EQUAL(primitive_type->get_primitive_kind(), edm_primitive_type_kind_t::String);
	VERIFY_ARE_EQUAL(str_v->as<::odata::string_t>(), _XPLATSTR("test string"));
}

TEST(primitive_value_boundary_i64)
{

	// min i64
	{
	    auto i64 = odata_primitive_value::make_primitive_value((int64_t)-9223372036854775808);
	    VERIFY_ARE_EQUAL(i64->get_value_type()->get_type_kind(), edm_type_kind_t::Primitive);
	    auto primitive_type = std::dynamic_pointer_cast<edm_primitive_type>(i64->get_value_type());
	    VERIFY_ARE_EQUAL(primitive_type->get_primitive_kind(), edm_primitive_type_kind_t::Int64);
		VERIFY_IS_TRUE(i64->as<int64_t>() < 0);
	    VERIFY_ARE_EQUAL(i64->as<int64_t>(), -9223372036854775808);
	}

	// max i64
	{
	    auto i64 = odata_primitive_value::make_primitive_value((int64_t)9223372036854775807);
	    VERIFY_ARE_EQUAL(i64->get_value_type()->get_type_kind(), edm_type_kind_t::Primitive);
	    auto primitive_type = std::dynamic_pointer_cast<edm_primitive_type>(i64->get_value_type());
	    VERIFY_ARE_EQUAL(primitive_type->get_primitive_kind(), edm_primitive_type_kind_t::Int64);
		VERIFY_IS_TRUE(i64->as<int64_t>() > 0);
	    VERIFY_ARE_EQUAL(i64->as<int64_t>(), 9223372036854775807);
	}

	//overflow
	{
	    auto i64 = odata_primitive_value::make_primitive_value((int64_t)9923372036854775807);
	    VERIFY_ARE_EQUAL(i64->get_value_type()->get_type_kind(), edm_type_kind_t::Primitive);
	    auto primitive_type = std::dynamic_pointer_cast<edm_primitive_type>(i64->get_value_type());
	    VERIFY_ARE_EQUAL(primitive_type->get_primitive_kind(), edm_primitive_type_kind_t::Int64);
	    VERIFY_IS_TRUE(i64->as<int64_t>() < 0);
	}
}

TEST(primitive_value_boundary_i32)
{
	// max i32 2147483647
	{
	    auto i32 = odata_primitive_value::make_primitive_value((int32_t)2147483647);
		VERIFY_IS_TRUE(i32->as<int32_t>() > 0);
	    VERIFY_ARE_EQUAL(i32->as<int32_t>(), 2147483647);
	}

	// min i32 -2147483648
	{
	    auto i32 = odata_primitive_value::make_primitive_value((int32_t)-2147483648);
		VERIFY_IS_TRUE(i32->as<int32_t>() < 0);
	    VERIFY_ARE_EQUAL(i32->as<int32_t>(), -2147483648);
	}

	// overflow
	{
	    auto i32 = odata_primitive_value::make_primitive_value((int32_t)2147483648);
		VERIFY_IS_TRUE(i32->as<int32_t>() < 0);
	}

	// overflow
	{
	    auto i32 = odata_primitive_value::make_primitive_value((int32_t)-2147483649);
		VERIFY_IS_TRUE(i32->as<int32_t>() > 0);
	}
}

TEST(primitive_value_boundary_i16)
{
	// max i16 32767
	{
	    auto i16 = odata_primitive_value::make_primitive_value((int16_t)32767);
		VERIFY_IS_TRUE(i16->as<int16_t>() > 0);
	    VERIFY_ARE_EQUAL(i16->as<int16_t>(), 32767);
	}

	// min i16 -32768
	{
	    auto i16 = odata_primitive_value::make_primitive_value((int16_t)-32768);
		VERIFY_IS_TRUE(i16->as<int16_t>() < 0);
	    VERIFY_ARE_EQUAL(i16->as<int16_t>(), -32768);
	}

	// overflow
	{
	    auto i16 = odata_primitive_value::make_primitive_value((int16_t)32768);
		VERIFY_IS_TRUE(i16->as<int16_t>() < 0);
	}

	// overflow
	{
	    auto i16 = odata_primitive_value::make_primitive_value((int16_t)-32769);
		VERIFY_IS_TRUE(i16->as<int16_t>() > 0);
	}
}


TEST(structured_value)
{
    std::shared_ptr<::odata::edm::edm_named_type> type = nullptr;
	auto structurd_value = ::odata::make_shared<odata_structured_value>(type);
	VERIFY_IS_NOT_NULL(structurd_value);

	int16_t i16 = -8;
	structurd_value->set_value(_XPLATSTR("int16"), i16);
	int32_t i32 = -123;
	structurd_value->set_value(_XPLATSTR("int32"), i32);
	int64_t i64 = -121321321321321;
	structurd_value->set_value(_XPLATSTR("int64"), i64);
	unsigned char ini_binary[] = {'Q', 'B', 'A', 'B', 'A', 'C', 'X', '1', '2', '*'};
	std::vector<unsigned char> binary(ini_binary, ini_binary + 10);
	structurd_value->set_value(_XPLATSTR("binary"), binary);
	bool bl = true;
	structurd_value->set_value(_XPLATSTR("boolean"), bl);
	::utility::datetime dt = ::utility::datetime::from_string(_XPLATSTR("2013-12-31T00:00:00Z"), ::utility::datetime::date_format::ISO_8601);
	structurd_value->set_value(_XPLATSTR("datetime"), dt);
	::utility::char_t* ch_str = _XPLATSTR("char_t pointer input");
	structurd_value->set_value(_XPLATSTR("char_t pointer"), ch_str);
	::odata::string_t str = _XPLATSTR("string_t input");
	structurd_value->set_value(_XPLATSTR("string_t"), str);
	double db = -32342212.23424;
	structurd_value->set_value(_XPLATSTR("double"), db);

	bool b_get = false;
	b_get = structurd_value->try_get(_XPLATSTR("int16"), i16);
	VERIFY_ARE_EQUAL(b_get, true);
	VERIFY_ARE_EQUAL(i16, -8);
	b_get = structurd_value->try_get(_XPLATSTR("int32"), i32);
	VERIFY_ARE_EQUAL(b_get, true);
	VERIFY_ARE_EQUAL(i32, -123);
	b_get = structurd_value->try_get(_XPLATSTR("int64"), i64);
	VERIFY_ARE_EQUAL(b_get, true);
	VERIFY_ARE_EQUAL(i64, -121321321321321);
	b_get = structurd_value->try_get(_XPLATSTR("binary"), binary);
	VERIFY_ARE_EQUAL(b_get, true);
	VERIFY_ARE_EQUAL(binary.size(), 10);
	VERIFY_ARE_EQUAL(binary[5], 'C');
	b_get = structurd_value->try_get(_XPLATSTR("boolean"), bl);
	VERIFY_ARE_EQUAL(b_get, true);
	VERIFY_ARE_EQUAL(bl, true);
	b_get = structurd_value->try_get(_XPLATSTR("datetime"), dt);
	VERIFY_ARE_EQUAL(b_get, true);
	VERIFY_ARE_EQUAL(dt.to_string(::utility::datetime::date_format::ISO_8601), _XPLATSTR("2013-12-31T00:00:00Z"));
	b_get = structurd_value->try_get(_XPLATSTR("char_t pointer"), str);
	VERIFY_ARE_EQUAL(b_get, true);
	VERIFY_ARE_EQUAL(str, _XPLATSTR("char_t pointer input"));
	b_get = structurd_value->try_get(_XPLATSTR("string_t"), str);
	VERIFY_ARE_EQUAL(b_get, true);
	VERIFY_ARE_EQUAL(str, _XPLATSTR("string_t input"));
	b_get = structurd_value->try_get(_XPLATSTR("double"), db);
	VERIFY_ARE_EQUAL(b_get, true);
	double as = abs(db - -32342212.23424);
	VERIFY_ARE_EQUAL(as < 0.000001, true);
}

TEST(complex_value)
{
	auto model = get_test_model();
	VERIFY_IS_NOT_NULL(model);

	auto complex_value = ::odata::make_shared<odata_complex_value>(model->find_complex_type(_XPLATSTR("AccountInfo")));
	complex_value->set_value(_XPLATSTR("FirstName"), _XPLATSTR("Leo"));
	complex_value->set_value(_XPLATSTR("LastName"), _XPLATSTR("Hu"));

    std::shared_ptr<::odata::edm::edm_named_type> type = nullptr;
	auto structurd_value = ::odata::make_shared<odata_structured_value>(type);
	VERIFY_IS_NOT_NULL(structurd_value);
	structurd_value->set_value(_XPLATSTR("complex value"), complex_value);

	bool b_get = false;
	std::shared_ptr<odata_complex_value> verify = nullptr;
	b_get = structurd_value->try_get(_XPLATSTR("complex value"), verify);
	VERIFY_ARE_EQUAL(b_get, true);
	VERIFY_IS_NOT_NULL(verify);
	VERIFY_ARE_EQUAL(verify->properties().size(), 2);
	::odata::string_t str;
	verify->try_get(_XPLATSTR("FirstName"), str);
	VERIFY_ARE_EQUAL(str, _XPLATSTR("Leo"));
	verify->try_get(_XPLATSTR("LastName"), str);
	VERIFY_ARE_EQUAL(str, _XPLATSTR("Hu"));
}

TEST(entity_value)
{
	auto model = get_test_model();
	VERIFY_IS_NOT_NULL(model);

	auto entity_value = ::odata::make_shared<odata_entity_value>(model->find_entity_type(_XPLATSTR("Account")));
	entity_value->set_value(_XPLATSTR("AccountID"), (int32_t)100);
	entity_value->set_value(_XPLATSTR("CountryRegion"), _XPLATSTR("China"));

    std::shared_ptr<::odata::edm::edm_named_type> type = nullptr;
	auto structurd_value = ::odata::make_shared<odata_structured_value>(type);
	VERIFY_IS_NOT_NULL(structurd_value);
	structurd_value->set_value(_XPLATSTR("entity value"), entity_value);

	bool b_get = false;
	std::shared_ptr<odata_entity_value> verify = nullptr;
	b_get = structurd_value->try_get(_XPLATSTR("entity value"), verify);
	VERIFY_ARE_EQUAL(b_get, true);
	VERIFY_IS_NOT_NULL(verify);
	VERIFY_ARE_EQUAL(verify->properties().size(), 2);
	int32_t id;
	verify->try_get(_XPLATSTR("AccountID"), id);
	VERIFY_ARE_EQUAL(id, 100);
	::odata::string_t str;
	verify->try_get(_XPLATSTR("CountryRegion"), str);
	VERIFY_ARE_EQUAL(str, _XPLATSTR("China"));
}

TEST(enum_value)
{
	auto model = get_test_model();
	VERIFY_IS_NOT_NULL(model);

	auto enum_value = ::odata::make_shared<odata_enum_value>(model->find_enum_type(_XPLATSTR("AccessLevel")), _XPLATSTR("Read"));
	VERIFY_IS_NOT_NULL(enum_value);

    std::shared_ptr<::odata::edm::edm_named_type> type = nullptr;
	auto structurd_value = ::odata::make_shared<odata_structured_value>(type);
	VERIFY_IS_NOT_NULL(structurd_value);
	structurd_value->set_value(_XPLATSTR("enum value"), enum_value);

	bool b_get = false;
	std::shared_ptr<odata_enum_value> verify = nullptr;
	b_get = structurd_value->try_get(_XPLATSTR("enum value"), verify);
	VERIFY_ARE_EQUAL(b_get, true);
	VERIFY_IS_NOT_NULL(verify);
	VERIFY_ARE_EQUAL(verify->to_string(), _XPLATSTR("Read"));
}

TEST(collection_value)
{
	auto model = get_test_model();

	auto enum_value_1 = ::odata::make_shared<odata_enum_value>(model->find_enum_type(_XPLATSTR("AccessLevel")), _XPLATSTR("Read"));
	auto enum_value_2 = ::odata::make_shared<odata_enum_value>(model->find_enum_type(_XPLATSTR("AccessLevel")), _XPLATSTR("Write"));
	auto enum_value_3 = ::odata::make_shared<odata_enum_value>(model->find_enum_type(_XPLATSTR("AccessLevel")), _XPLATSTR("ReadWrite"));

	auto collection_type = ::odata::make_shared<edm_collection_type>(_XPLATSTR("collection value"), model->find_enum_type(_XPLATSTR("AccessLevel")));
	auto collection_value = ::odata::make_shared<odata_collection_value>(collection_type);
	collection_value->add_collection_value(enum_value_1);
	collection_value->add_collection_value(enum_value_2);
	collection_value->add_collection_value(enum_value_3);


    std::shared_ptr<::odata::edm::edm_named_type> type = nullptr;
	auto structurd_value = ::odata::make_shared<odata_structured_value>(type);
	VERIFY_IS_NOT_NULL(structurd_value);
	structurd_value->set_value(_XPLATSTR("collection value"), collection_value);

	bool b_get = false;
	std::shared_ptr<odata_collection_value> verify = nullptr;
	b_get = structurd_value->try_get(_XPLATSTR("collection value"), verify);
	VERIFY_ARE_EQUAL(b_get, true);
	VERIFY_ARE_EQUAL(verify->get_collection_values().size(), 3);
	enum_value_1 = std::dynamic_pointer_cast<odata_enum_value>(verify->get_collection_values()[0]);
	VERIFY_ARE_EQUAL(enum_value_1->to_string(), _XPLATSTR("Read"));
	enum_value_2 = std::dynamic_pointer_cast<odata_enum_value>(verify->get_collection_values()[1]);
	VERIFY_ARE_EQUAL(enum_value_2->to_string(), _XPLATSTR("Write"));
	enum_value_3 = std::dynamic_pointer_cast<odata_enum_value>(verify->get_collection_values()[2]);
	VERIFY_ARE_EQUAL(enum_value_3->to_string(), _XPLATSTR("ReadWrite"));
}

TEST(primitive_collection_value)
{
	auto model = get_test_model();

	auto p_value_1 = odata_primitive_value::make_primitive_value((double)-12123.2312);
	auto p_value_2 = odata_primitive_value::make_primitive_value((double)-123123213);
	auto p_value_3 = odata_primitive_value::make_primitive_value((double)-121.2312);

	auto collection_type = ::odata::make_shared<edm_collection_type>(_XPLATSTR("collection value"), ::odata::edm::edm_primitive_type::DOUBLE());
	auto collection_value = ::odata::make_shared<odata_collection_value>(collection_type);
	collection_value->add_collection_value(p_value_1);
	collection_value->add_collection_value(p_value_2);
	collection_value->add_collection_value(p_value_3);

	p_value_1 = std::dynamic_pointer_cast<odata_primitive_value>(collection_value->get_collection_values()[0]);
	VERIFY_IS_TRUE(abs(p_value_1->as<double>() - -12123.2312) < 0.000001);
	p_value_2 = std::dynamic_pointer_cast<odata_primitive_value>(collection_value->get_collection_values()[1]);
	VERIFY_IS_TRUE(abs(p_value_2->as<double>() - -123123213) < 0.000001);
	p_value_3 = std::dynamic_pointer_cast<odata_primitive_value>(collection_value->get_collection_values()[2]);
	VERIFY_IS_TRUE(abs(p_value_3->as<double>() - -121.2312) < 0.000001);
}

TEST(complex_collection_value)
{
	auto model = get_test_model();

	auto complex_value_1 = ::odata::make_shared<odata_complex_value>(model->find_complex_type(_XPLATSTR("AccountInfo")));
	complex_value_1->set_value(_XPLATSTR("FirstName"), _XPLATSTR("F_1"));
	complex_value_1->set_value(_XPLATSTR("LastName"), _XPLATSTR("L_1"));

	auto collection_type = ::odata::make_shared<edm_collection_type>(_XPLATSTR("collection value"), model->find_complex_type(_XPLATSTR("AccountInfo")));
	auto collection_value = ::odata::make_shared<odata_collection_value>(collection_type);
	collection_value->add_collection_value(complex_value_1);

	complex_value_1 = std::dynamic_pointer_cast<odata_complex_value>(collection_value->get_collection_values()[0]);
	::odata::string_t str;
	complex_value_1->try_get(_XPLATSTR("FirstName"), str);
	VERIFY_ARE_EQUAL(str, _XPLATSTR("F_1"));
	complex_value_1->try_get(_XPLATSTR("LastName"), str);
	VERIFY_ARE_EQUAL(str, _XPLATSTR("L_1"));
}

TEST(entity_collection_value)
{
	auto model = get_test_model();

	auto entity_value_1 = ::odata::make_shared<odata_entity_value>(model->find_entity_type(_XPLATSTR("Account")));
	entity_value_1->set_value(_XPLATSTR("AccountID"), (int32_t)100);
	entity_value_1->set_value(_XPLATSTR("CountryRegion"), _XPLATSTR("China"));

	auto entity_value_2 = ::odata::make_shared<odata_entity_value>(model->find_entity_type(_XPLATSTR("Account")));
	entity_value_2->set_value(_XPLATSTR("AccountID"), (int32_t)200);
	entity_value_2->set_value(_XPLATSTR("CountryRegion"), _XPLATSTR("JP"));

	auto entity_value_3 = ::odata::make_shared<odata_entity_value>(model->find_entity_type(_XPLATSTR("Account")));
	entity_value_3->set_value(_XPLATSTR("AccountID"), (int32_t)300);
	entity_value_3->set_value(_XPLATSTR("CountryRegion"), _XPLATSTR("GB"));

	auto collection_type = ::odata::make_shared<edm_collection_type>(_XPLATSTR("collection value"), model->find_entity_type(_XPLATSTR("Account")));
	auto collection_value = ::odata::make_shared<odata_collection_value>(collection_type);
	collection_value->add_collection_value(entity_value_1);
	collection_value->add_collection_value(entity_value_2);
	collection_value->add_collection_value(entity_value_3);

	entity_value_1 = std::dynamic_pointer_cast<odata_entity_value>(collection_value->get_collection_values()[0]);
	entity_value_2 = std::dynamic_pointer_cast<odata_entity_value>(collection_value->get_collection_values()[1]);
	entity_value_3 = std::dynamic_pointer_cast<odata_entity_value>(collection_value->get_collection_values()[2]);

	int32_t id;
	::odata::string_t str;
	entity_value_1->try_get(_XPLATSTR("AccountID"), id);
	VERIFY_ARE_EQUAL(id, 100);
	entity_value_1->try_get(_XPLATSTR("CountryRegion"), str);
	VERIFY_ARE_EQUAL(str, _XPLATSTR("China"));

	entity_value_2->try_get(_XPLATSTR("AccountID"), id);
	VERIFY_ARE_EQUAL(id, 200);
	entity_value_2->try_get(_XPLATSTR("CountryRegion"), str);
	VERIFY_ARE_EQUAL(str, _XPLATSTR("JP"));

	entity_value_3->try_get(_XPLATSTR("AccountID"), id);
	VERIFY_ARE_EQUAL(id, 300);
	entity_value_3->try_get(_XPLATSTR("CountryRegion"), str);
	VERIFY_ARE_EQUAL(str, _XPLATSTR("GB"));
}

}

}}}