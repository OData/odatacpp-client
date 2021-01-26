//---------------------------------------------------------------------
// <copyright file="common_utility_test.cpp" company="Microsoft">
//      Copyright (C) Microsoft Corporation. All rights reserved. See License.txt in the project root for license information.
// </copyright>
//---------------------------------------------------------------------

#include "../odata_tests.h"
#include "odata/common/utility.h"

using namespace ::odata::edm;
using namespace ::odata::core;
using namespace ::odata::common;

namespace tests { namespace functional { namespace _odata {

SUITE(common_utility_test)
{

TEST(strip_string_empty_test)
{
	::odata::string_t case_string = _XPLATSTR("");
	auto ret = strip_string(case_string);
	VERIFY_ARE_EQUAL(ret, _XPLATSTR(""));
}

TEST(strip_string_empty_quote_test)
{
	::odata::string_t case_string = _XPLATSTR("\"\"");
	auto ret = strip_string(case_string);
	VERIFY_ARE_EQUAL(ret, _XPLATSTR(""));
}

TEST(strip_string_with_quote_test)
{
	::odata::string_t case_string = _XPLATSTR("\"121232123\"");
	auto ret = strip_string(case_string);
	VERIFY_ARE_EQUAL(ret, _XPLATSTR("121232123"));
}

TEST(strip_string_without_quote_test)
{
	::odata::string_t case_string = _XPLATSTR("121232123");
	auto ret = strip_string(case_string);
	VERIFY_ARE_EQUAL(ret, _XPLATSTR("121232123"));
}

TEST(strip_string_with_single_left_quote_test)
{
	::odata::string_t case_string = _XPLATSTR("\"121232123");
	auto ret = strip_string(case_string);
	VERIFY_ARE_EQUAL(ret, _XPLATSTR("121232123"));
}

TEST(strip_string_with_single_right_quote_test)
{
	::odata::string_t case_string = _XPLATSTR("121232123\"");
	auto ret = strip_string(case_string);
	VERIFY_ARE_EQUAL(ret, _XPLATSTR("121232123"));
}

TEST(split_string_empty_src_with_empty_delim_test)
{
	::odata::string_t case_string = _XPLATSTR("");
	::odata::string_t delim_string = _XPLATSTR("");
	std::list<::odata::string_t> ret;

	split_string(case_string, delim_string, ret);
	VERIFY_ARE_EQUAL(ret.size(), 1);
}

TEST(split_string_with_no_empty_input_test)
{
	::odata::string_t case_string = _XPLATSTR("23123.23232.32323");
	::odata::string_t delim_string = _XPLATSTR(".");
	std::list<::odata::string_t> ret;
	ret.push_back(_XPLATSTR("1"));
	ret.push_back(_XPLATSTR("2"));
	ret.push_back(_XPLATSTR("3"));
	ret.push_back(_XPLATSTR("4"));
	ret.push_back(_XPLATSTR("5"));

	split_string(case_string, delim_string, ret);
	VERIFY_ARE_EQUAL(ret.size(), 3);
}

TEST(split_string_empty_src_with_delim_test)
{
	::odata::string_t case_string = _XPLATSTR("");
	::odata::string_t delim_string = _XPLATSTR("..");
	std::list<::odata::string_t> ret;

	split_string(case_string, delim_string, ret);
	VERIFY_ARE_EQUAL(ret.size(), 1);
}

TEST(split_string_src_with_empty_delim_test)
{
	::odata::string_t case_string = _XPLATSTR("adfadfadfdas");
	::odata::string_t delim_string = _XPLATSTR("");
	std::list<::odata::string_t> ret;

	split_string(case_string, delim_string, ret);
	VERIFY_ARE_EQUAL(ret.size(), 1);
}

TEST(split_string_src_with_delim_notfind_test)
{
	::odata::string_t case_string = _XPLATSTR("adfadfadfdas");
	::odata::string_t delim_string = _XPLATSTR("..");
	std::list<::odata::string_t> ret;

	split_string(case_string, delim_string, ret);
	VERIFY_ARE_EQUAL(ret.size(), 1);
}

TEST(split_string_src_with_delim_find_test)
{
	::odata::string_t case_string = _XPLATSTR("..adf..ad..fa..dfdas..");
	::odata::string_t delim_string = _XPLATSTR("..");
	std::list<::odata::string_t> ret;

	split_string(case_string, delim_string, ret);
	VERIFY_ARE_EQUAL(ret.size(), 6);
}

TEST(split_string_src_with_duplicated_delim_test)
{
	::odata::string_t case_string = _XPLATSTR("...adf..ad....fa..dfdas...");
	::odata::string_t delim_string = _XPLATSTR("..");
	std::list<::odata::string_t> ret;

	split_string(case_string, delim_string, ret);
	VERIFY_ARE_EQUAL(ret.size(), 7);
}

TEST(split_string_src_with_single_char_delim_test)
{
	::odata::string_t case_string = _XPLATSTR("...adf..ad....fa..dfdas...f");
	::odata::string_t delim_string = _XPLATSTR("f");
	std::list<::odata::string_t> ret;

	split_string(case_string, delim_string, ret);
	VERIFY_ARE_EQUAL(ret.size(), 5);
}

TEST(split_string_src_with_equal_delim_test)
{
	::odata::string_t case_string = _XPLATSTR("...adf..ad....fa..dfdas...f");
	::odata::string_t delim_string = _XPLATSTR("...adf..ad....fa..dfdas...f");
	std::list<::odata::string_t> ret;

	split_string(case_string, delim_string, ret);
	VERIFY_ARE_EQUAL(ret.size(), 2);
}

TEST(split_string_src_with_exceed_length_delim_test)
{
	::odata::string_t case_string = _XPLATSTR("...adf..ad....fa..dfdas...f");
	::odata::string_t delim_string = _XPLATSTR("...adf..ad....fa..dfdas...fdfdfdfdf");
	std::list<::odata::string_t> ret;

	split_string(case_string, delim_string, ret);
	VERIFY_ARE_EQUAL(ret.size(), 1);
	VERIFY_ARE_EQUAL(ret.front(), _XPLATSTR("...adf..ad....fa..dfdas...f"));
}

TEST(is_relative_path_empty_root_wity_empty_path)
{
	::odata::string_t root_string = _XPLATSTR("");
	::odata::string_t path_string = _XPLATSTR("");

    bool ret = is_relative_path(root_string, path_string);
	VERIFY_ARE_EQUAL(ret, false);
}

TEST(is_relative_path_empty_root)
{
	::odata::string_t root_string = _XPLATSTR("");
	::odata::string_t path_string = _XPLATSTR("Accounts");

    bool ret = is_relative_path(root_string, path_string);
	VERIFY_ARE_EQUAL(ret, false);
}

TEST(is_relative_path_empty_path)
{
	::odata::string_t root_string = _XPLATSTR("http://odatae2etest.azurewebsites.net/cpptest/DefaultService");
	::odata::string_t path_string = _XPLATSTR("");

    bool ret = is_relative_path(root_string, path_string);
	VERIFY_ARE_EQUAL(ret, false);
}

TEST(is_relative_path_valid_root_wity_full_path)
{
	::odata::string_t root_string = _XPLATSTR("http://odatae2etest.azurewebsites.net/cpptest/DefaultService");
	::odata::string_t path_string = _XPLATSTR("http://odatae2etest.azurewebsites.net/cpptest/DefaultService/Accounts");

    bool ret = is_relative_path(root_string, path_string);
	VERIFY_ARE_EQUAL(ret, false);
}

TEST(is_relative_path_valid_root_wity_relative_path)
{
	::odata::string_t root_string = _XPLATSTR("http://http://odatae2etest.azurewebsites.net/cpptest/DefaultService");
	::odata::string_t path_string = _XPLATSTR("http");

    bool ret = is_relative_path(root_string, path_string);
	VERIFY_ARE_EQUAL(ret, true);
}

TEST(is_relative_path_valid_root_wity_uppercase_full_path)
{
	::odata::string_t root_string = _XPLATSTR("http://odatae2etest.azurewebsites.net/cpptest/DefaultService");
	::odata::string_t path_string = _XPLATSTR("http://odatae2etest.azurewebsites.net/cpptest/DefaultService/Accounts");

    bool ret = is_relative_path(root_string, path_string);
	VERIFY_ARE_EQUAL(ret, false);
}

TEST(print_double_zero_input)
{
	double zero = 0.0;
	::odata::string_t ret = print_double(zero);
	VERIFY_ARE_EQUAL(ret, _XPLATSTR("0.0"));
}

TEST(print_double_zero_input_2)
{
	double zero = 0;
	::odata::string_t ret = print_double(zero);
	VERIFY_ARE_EQUAL(ret, _XPLATSTR("0.0"));
}

TEST(print_double_minus_zero_input)
{
	double zero = -0.0;
	::odata::string_t ret = print_double(zero);
	VERIFY_ARE_EQUAL(ret, _XPLATSTR("-0.0"));
}

TEST(print_double_minus_zero_input_2)
{
	double zero = -0;
	::odata::string_t ret = print_double(zero);
	VERIFY_ARE_EQUAL(ret, _XPLATSTR("0.0"));
}

TEST(print_double_valid_input)
{
	double zero = 212.1234;
	::odata::string_t ret = print_double(zero, 4);
	VERIFY_ARE_EQUAL(ret, _XPLATSTR("212.1234"));
}

TEST(print_double_right_exceed_precision_input)
{
	double zero = 123123123.1111222323;
	::odata::string_t ret = print_double(zero, 8);
	VERIFY_ARE_EQUAL(ret, _XPLATSTR("123123123.11112224"));
}

TEST(print_double_set_right_precision_input)
{
	double zero = -4212.1111222323;
	::odata::string_t ret = print_double(zero, 13);
	VERIFY_ARE_EQUAL(ret, _XPLATSTR("-4212.1111222322997"));
}

TEST(print_double_zero_precision_input)
{
	double zero = -4212.1111222323;
	::odata::string_t ret = print_double(zero, 0);
	VERIFY_ARE_EQUAL(ret, _XPLATSTR("-4212"));
}

TEST(print_double_minux_precision_input)
{
	double zero = -4212.11;
	::odata::string_t ret = print_double(zero, 9);
	VERIFY_ARE_EQUAL(ret, _XPLATSTR("-4212.11"));
}

TEST(print_double_large_double_input)
{
	double zero = -12123123123213124212.11;
	::odata::string_t ret = print_double(zero, 1);
#ifdef WIN32
	VERIFY_ARE_EQUAL(ret, _XPLATSTR("-12123123123213124000.0"));
#else
    VERIFY_ARE_EQUAL(ret, _XPLATSTR("-12123123123213123584.0"));
#endif
}

TEST(print_double_large_double_input_2)
{
	double zero = 1234567891234567.7674567611117999;
	::odata::string_t ret = print_double(zero, 1);
#ifdef WIN32
	VERIFY_ARE_EQUAL(ret, _XPLATSTR("1234567891234567.7"));
#else
    VERIFY_ARE_EQUAL(ret, _XPLATSTR("1234567891234567.8"));
#endif
}

TEST(print_double_large_double_input_3)
{
	double zero = -12345.7674567611117999;
	::odata::string_t ret = print_double(zero, 12);
	VERIFY_ARE_EQUAL(ret, _XPLATSTR("-12345.767456761112"));
}

}

}}}