//---------------------------------------------------------------------
// <copyright file="codegen_test.cpp" company="Microsoft">
//      Copyright (C) Microsoft Corporation. All rights reserved. See License.txt in the project root for license information.
// </copyright>
//---------------------------------------------------------------------

#include "../odata_tests.h"
#include "odata/edm/edm_type.h"
#include "odata/codegen/code_generation.h"
#include "codegen_test_service.h"

using namespace ::odata::edm;
using namespace ::odata::core;
using namespace ::odata::common;

namespace tests { namespace functional { namespace _odata {

static std::shared_ptr<odata_entity_value> make_account_object()
{
	auto model = get_test_model();

	auto complex_value = ::odata::make_shared<odata_complex_value>(model->find_complex_type(_XPLATSTR("AccountInfo")));
	complex_value->set_value(_XPLATSTR("FirstName"), _XPLATSTR("Leo"));
	complex_value->set_value(_XPLATSTR("LastName"), _XPLATSTR("Hu"));

	auto entity_value = ::odata::make_shared<odata_entity_value>(model->find_entity_type(_XPLATSTR("Account")));
	entity_value->set_value(_XPLATSTR("AccountID"), (int32_t)989899);
	entity_value->set_value(_XPLATSTR("CountryRegion"), _XPLATSTR("China"));
	entity_value->set_value(_XPLATSTR("AccountInfo"), complex_value);
	entity_value->set_value(::odata::core::odata_json_constants::PAYLOAD_ANNOTATION_EDITLINK, _XPLATSTR("http://0.0.0.1/Account(989899)"));

	return entity_value;
}

static std::shared_ptr<odata_entity_value> make_giftcard_object()
{
	auto model = get_test_model();

	auto entity_value = ::odata::make_shared<odata_entity_value>(model->find_entity_type(_XPLATSTR("GiftCard")));
	entity_value->set_value(_XPLATSTR("GiftCardID"), (int32_t)77566);
	entity_value->set_value(_XPLATSTR("GiftCardNO"), _XPLATSTR("@@2232"));

	return entity_value;
}

static std::shared_ptr<odata_entity_value> make_paymentinstrument_object()
{
	auto model = get_test_model();

	auto entity_value = ::odata::make_shared<odata_entity_value>(model->find_entity_type(_XPLATSTR("PaymentInstrument")));

	return entity_value;
}

static std::shared_ptr<odata_entity_value> make_creditcardpi_object()
{
	auto model = get_test_model();

	auto entity_value = ::odata::make_shared<odata_entity_value>(model->find_entity_type(_XPLATSTR("CreditCardPI")));

	return entity_value;
}

static std::shared_ptr<odata_entity_value> make_account_object_with_navigation()
{
	auto model = get_test_model();

	auto entity_value = make_account_object();
	auto giftcard_value = make_giftcard_object();
	entity_value->set_value(_XPLATSTR("MyGiftCard"), giftcard_value);

	auto pi_1 = make_paymentinstrument_object();
	pi_1->set_value(_XPLATSTR("PaymentInstrumentID"), 100000);
	pi_1->set_value(_XPLATSTR("FriendlyName"), _XPLATSTR("PI 1"));

	auto pi_2 = make_creditcardpi_object();
	pi_2->set_value(_XPLATSTR("@odata.type"), "#Microsoft.Test.OData.Services.ODataWCFService.CreditCardPI");
	pi_2->set_value(_XPLATSTR("PaymentInstrumentID"), 200000);
	pi_2->set_value(_XPLATSTR("FriendlyName"), _XPLATSTR("PI 2"));
	pi_2->set_value(_XPLATSTR("CardNumber"), _XPLATSTR("777777"));
	pi_2->set_value(::odata::core::odata_json_constants::PAYLOAD_ANNOTATION_EDITLINK, _XPLATSTR("http://0.0.0.1/PaymentInstrument(200000)"));

	auto collection_value = ::odata::make_shared<odata_collection_value>(model->find_entity_type(_XPLATSTR("PaymentInstrument")));
	collection_value->add_collection_value(pi_1);
	collection_value->add_collection_value(pi_2);

	entity_value->set_value(_XPLATSTR("MyPaymentInstruments"), collection_value);

	return entity_value;
}

static std::shared_ptr<odata_entity_value> make_person_object()
{
	auto model = get_test_model();

	auto entity_value = ::odata::make_shared<odata_entity_value>(model->find_entity_type(_XPLATSTR("Person")));
	entity_value->set_value(_XPLATSTR("PersonID"), (int32_t)12123);
	entity_value->set_value(_XPLATSTR("FirstName"), _XPLATSTR("Leo"));
	entity_value->set_value(_XPLATSTR("MiddleName"), _XPLATSTR("Middle Name"));
	auto collection_value = ::odata::make_shared<odata_collection_value>(edm_primitive_type::STRING());
	collection_value->add_collection_value(odata_primitive_value::make_primitive_value(_XPLATSTR("string 1")));
	collection_value->add_collection_value(odata_primitive_value::make_primitive_value(_XPLATSTR("string 2")));
	collection_value->add_collection_value(odata_primitive_value::make_primitive_value(_XPLATSTR("string 3")));
	entity_value->set_value(_XPLATSTR("Numbers"), collection_value);

	return entity_value;
}

static std::shared_ptr<odata_complex_value> make_accountinfo_object()
{
	auto model = get_test_model();

	auto complex_value = ::odata::make_shared<odata_complex_value>(model->find_complex_type(_XPLATSTR("AccountInfo")));
	complex_value->set_value(_XPLATSTR("FirstName"), _XPLATSTR("Leo  "));
	complex_value->set_value(_XPLATSTR("LastName"), _XPLATSTR("  Hu"));

	return complex_value;
}

static std::shared_ptr<odata_complex_value> make_address_object()
{
	auto model = get_test_model();

	auto complex_value = ::odata::make_shared<odata_complex_value>(model->find_complex_type(_XPLATSTR("Address")));
	complex_value->set_value(_XPLATSTR("Street"), _XPLATSTR("Road 1111"));
	complex_value->set_value(_XPLATSTR("City"), _XPLATSTR("Shang Hai"));
	complex_value->set_value(_XPLATSTR("Code"), 12123);
	auto collection_value = ::odata::make_shared<odata_collection_value>(edm_primitive_type::INT32());
	collection_value->add_collection_value(odata_primitive_value::make_primitive_value(2213213));
	collection_value->add_collection_value(odata_primitive_value::make_primitive_value(-212312));
	collection_value->add_collection_value(odata_primitive_value::make_primitive_value(0));
	complex_value->set_value(_XPLATSTR("Numbers"), collection_value);

	complex_value->set_value(_XPLATSTR("Color"), ::odata::make_shared<odata_enum_value>(model->find_enum_type(_XPLATSTR("Color")), _XPLATSTR("Red")));

	auto enum_collection_value = ::odata::make_shared<odata_collection_value>(model->find_complex_type(_XPLATSTR("Color")));
	enum_collection_value->add_collection_value(::odata::make_shared<odata_enum_value>(model->find_enum_type(_XPLATSTR("Color")), _XPLATSTR("Red")));
	enum_collection_value->add_collection_value(::odata::make_shared<odata_enum_value>(model->find_enum_type(_XPLATSTR("Color")), _XPLATSTR("Blue")));
	enum_collection_value->add_collection_value(::odata::make_shared<odata_enum_value>(model->find_enum_type(_XPLATSTR("Color")), _XPLATSTR("Green")));
	complex_value->set_value(_XPLATSTR("CoverColors"), enum_collection_value);

	return complex_value;
}

static std::shared_ptr<odata_entity_value> make_employee_object()
{
	auto model = get_test_model();

	auto entity_value = ::odata::make_shared<odata_entity_value>(model->find_entity_type(_XPLATSTR("Employee")));
	entity_value->set_value(_XPLATSTR("PersonID"), (int32_t)10001);
	entity_value->set_value(_XPLATSTR("FirstName"), _XPLATSTR("Henry"));
	auto complex_type = model->find_complex_type(_XPLATSTR("Microsoft.Test.OData.Services.ODataWCFService.Address"));
	auto collection_value = ::odata::make_shared<odata_collection_value>(complex_type);
	auto complex_value_1 = ::odata::make_shared<odata_complex_value>(complex_type);
	complex_value_1->set_value(_XPLATSTR("Street"), _XPLATSTR("Street 1"));
	complex_value_1->set_value(_XPLATSTR("City"), _XPLATSTR("City 1"));
	auto complex_value_2 = ::odata::make_shared<odata_complex_value>(complex_type);
	complex_value_2->set_value(_XPLATSTR("Street"), _XPLATSTR("Street 2"));
	complex_value_2->set_value(_XPLATSTR("City"), _XPLATSTR("City 2"));
	auto complex_value_3 = ::odata::make_shared<odata_complex_value>(complex_type);
	complex_value_3->set_value(_XPLATSTR("Street"), _XPLATSTR("Street 3"));
	complex_value_3->set_value(_XPLATSTR("City"), _XPLATSTR("City 3"));
	collection_value->add_collection_value(complex_value_1);
	collection_value->add_collection_value(complex_value_2);
	collection_value->add_collection_value(complex_value_3);
	entity_value->set_value(_XPLATSTR("WorkAddresses"), collection_value);

	return entity_value;
}

static std::shared_ptr<odata_entity_value> make_product_object()
{
	auto model = get_test_model();

	auto entity_value = ::odata::make_shared<odata_entity_value>(model->find_entity_type(_XPLATSTR("Product")));
	entity_value->set_value(_XPLATSTR("ProductID"), (int32_t)64454);
	entity_value->set_value(_XPLATSTR("Name"), _XPLATSTR("Henry"));
	entity_value->set_value(_XPLATSTR("UserAccess"), ::odata::make_shared<odata_enum_value>(model->find_enum_type(_XPLATSTR("AccessLevel")), _XPLATSTR("ReadWrite")));

	auto collection_value = ::odata::make_shared<odata_collection_value>(model->find_complex_type(_XPLATSTR("Color")));
	collection_value->add_collection_value(::odata::make_shared<odata_enum_value>(model->find_enum_type(_XPLATSTR("Color")), _XPLATSTR("Red")));
	collection_value->add_collection_value(::odata::make_shared<odata_enum_value>(model->find_enum_type(_XPLATSTR("Color")), _XPLATSTR("Blue")));
	collection_value->add_collection_value(::odata::make_shared<odata_enum_value>(model->find_enum_type(_XPLATSTR("Color")), _XPLATSTR("Green")));

	entity_value->set_value(_XPLATSTR("CoverColors"), collection_value);

	unsigned char ini_binary[] = {'Q', 'B', 'A', 'B', 'A', 'C', 'X', '1', '2', '*', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'p'};
	std::vector<unsigned char> binary(ini_binary, ini_binary + 25);
	entity_value->set_value(_XPLATSTR("Picture"), binary);

	entity_value->set_value(_XPLATSTR("NullPicture"), binary);

	return entity_value;
}

SUITE(codegen_test)
{

TEST(map_primitive_to_entity)
{
	auto test_value = make_account_object();
	VERIFY_IS_NOT_NULL(test_value);
	auto service_context = ::odata::make_shared<InMemoryEntities>(_XPLATSTR("http://0.0.0.1"));

	auto account = ::odata::make_shared<Account>(service_context);
	account->from_value(test_value);

	VERIFY_ARE_EQUAL(account->get_accountid(), 989899);
	VERIFY_ARE_EQUAL(account->get_countryregion(), _XPLATSTR("China"));
}

TEST(map_collection_primitive_to_entity)
{
	auto test_value = make_person_object();
	VERIFY_IS_NOT_NULL(test_value);
	auto service_context = ::odata::make_shared<InMemoryEntities>(_XPLATSTR("http://0.0.0.1"));

	auto person = ::odata::make_shared<Person>(service_context);
	person->from_value(test_value);

	VERIFY_ARE_EQUAL(person->get_personid(), 12123);
	VERIFY_ARE_EQUAL(person->get_firstname(), _XPLATSTR("Leo"));
	VERIFY_ARE_EQUAL(person->get_numbers().size(), 3);
	VERIFY_ARE_EQUAL(person->get_numbers()[0], _XPLATSTR("string 1"));
	VERIFY_ARE_EQUAL(person->get_numbers()[1], _XPLATSTR("string 2"));
	VERIFY_ARE_EQUAL(person->get_numbers()[2], _XPLATSTR("string 3"));
}

TEST(map_nullable_primitive_to_entity)
{
	auto test_value = make_person_object();
	VERIFY_IS_NOT_NULL(test_value);
	auto service_context = ::odata::make_shared<InMemoryEntities>(_XPLATSTR("http://0.0.0.1"));

	auto person = ::odata::make_shared<Person>(service_context);
	person->from_value(test_value);

	VERIFY_ARE_EQUAL(person->get_personid(), 12123);
	VERIFY_ARE_EQUAL(person->get_firstname(), _XPLATSTR("Leo"));
	VERIFY_IS_NOT_NULL(person->get_middlename());
	VERIFY_ARE_EQUAL(*(person->get_middlename()), _XPLATSTR("Middle Name"));
}

TEST(map_primitive_to_complex)
{
	auto test_value = make_accountinfo_object();
	VERIFY_IS_NOT_NULL(test_value);
	auto service_context = ::odata::make_shared<InMemoryEntities>(_XPLATSTR("http://0.0.0.1"));

	auto accountinfo = ::odata::make_shared<AccountInfo>(service_context);
	accountinfo->from_value(test_value);

	VERIFY_ARE_EQUAL(accountinfo->get_firstname(), _XPLATSTR("Leo  "));
	VERIFY_ARE_EQUAL(accountinfo->get_lastname(), _XPLATSTR("  Hu"));
}

TEST(map_collection_primitive_to_complex)
{
	auto test_value = make_address_object();
	VERIFY_IS_NOT_NULL(test_value);
	auto service_context = ::odata::make_shared<InMemoryEntities>(_XPLATSTR("http://0.0.0.1"));

	auto address = ::odata::make_shared<Address>(service_context);
	address->from_value(test_value);

	VERIFY_ARE_EQUAL(address->get_numbers().size(), 3);
	VERIFY_ARE_EQUAL(address->get_numbers()[0], 2213213);
	VERIFY_ARE_EQUAL(address->get_numbers()[1], -212312);
	VERIFY_ARE_EQUAL(address->get_numbers()[2], 0);
}

TEST(map_nullable_primitive_to_complex)
{
	auto test_value = make_address_object();
	VERIFY_IS_NOT_NULL(test_value);
	auto service_context = ::odata::make_shared<InMemoryEntities>(_XPLATSTR("http://0.0.0.1"));

	auto address = ::odata::make_shared<Address>(service_context);
	VERIFY_IS_NULL(address->get_code());
	address->from_value(test_value);
	VERIFY_IS_NOT_NULL(address->get_code());
	VERIFY_ARE_EQUAL(*(address->get_code()), 12123);
}

TEST(map_complex_to_entity)
{
	auto test_value = make_account_object();
	VERIFY_IS_NOT_NULL(test_value);
	auto service_context = ::odata::make_shared<InMemoryEntities>(_XPLATSTR("http://0.0.0.1"));

	auto account = ::odata::make_shared<Account>(service_context);
	account->from_value(test_value);

	auto account_info = account->get_accountinfo();
	VERIFY_ARE_EQUAL(account_info->get_firstname(), _XPLATSTR("Leo"));
	VERIFY_ARE_EQUAL(account_info->get_lastname(), _XPLATSTR("Hu"));
}

TEST(map_derived_complex_to_entity)
{
	auto test_value = make_account_object();
	VERIFY_IS_NOT_NULL(test_value);
	auto service_context = ::odata::make_shared<InMemoryEntities>(_XPLATSTR("http://0.0.0.1"));

	auto complex_value = ::odata::make_shared<odata_complex_value>(get_test_model()->find_complex_type(_XPLATSTR("VIPAccountInfo")));
	complex_value->set_value(_XPLATSTR("VIP_Info"), _XPLATSTR("Diamond"));
	complex_value->set_value(_XPLATSTR("FirstName"), _XPLATSTR("Henry"));
	complex_value->set_value(_XPLATSTR("LastName"), _XPLATSTR("Yin"));

	test_value->set_value(_XPLATSTR("VIPAccountInfo"), complex_value);

	auto account = ::odata::make_shared<Account>(service_context);
	account->from_value(test_value);

	auto account_info = account->get_accountinfo();
	VERIFY_ARE_EQUAL(account_info->get_firstname(), _XPLATSTR("Leo"));
	VERIFY_ARE_EQUAL(account_info->get_lastname(), _XPLATSTR("Hu"));

	auto vip_info = account->get_vipinfo();
	VERIFY_ARE_EQUAL(vip_info->get_firstname(), _XPLATSTR("Henry"));
	VERIFY_ARE_EQUAL(vip_info->get_lastname(), _XPLATSTR("Yin"));
	VERIFY_ARE_EQUAL(vip_info->get_vipinfo(), _XPLATSTR("Diamond"));
}

TEST(map_collection_complex_to_entity)
{
	auto test_value = make_employee_object();
	VERIFY_IS_NOT_NULL(test_value);
	auto service_context = ::odata::make_shared<InMemoryEntities>(_XPLATSTR("http://0.0.0.1"));

	auto employee = ::odata::make_shared<Employee>(service_context);
	employee->from_value(test_value);

	VERIFY_ARE_EQUAL(employee->get_addresses().size(), 3);
	VERIFY_ARE_EQUAL(employee->get_addresses()[0]->get_street(), _XPLATSTR("Street 1"));
	VERIFY_ARE_EQUAL(employee->get_addresses()[0]->get_city(), _XPLATSTR("City 1"));
	VERIFY_ARE_EQUAL(employee->get_addresses()[1]->get_street(), _XPLATSTR("Street 2"));
	VERIFY_ARE_EQUAL(employee->get_addresses()[1]->get_city(), _XPLATSTR("City 2"));
	VERIFY_ARE_EQUAL(employee->get_addresses()[2]->get_street(), _XPLATSTR("Street 3"));
	VERIFY_ARE_EQUAL(employee->get_addresses()[2]->get_city(), _XPLATSTR("City 3"));
}

TEST(map_nullable_complex_to_entity_with_null_value)
{
	auto test_value = make_person_object();
	VERIFY_IS_NOT_NULL(test_value);
	auto service_context = ::odata::make_shared<InMemoryEntities>(_XPLATSTR("http://0.0.0.1"));

	auto person = ::odata::make_shared<Person>(service_context);
	person->from_value(test_value);

	VERIFY_IS_NULL(person->get_address());
}

TEST(map_nullable_complex_to_entity_with_value)
{
	auto test_value = make_person_object();
	auto address = make_address_object();
	test_value->set_value(_XPLATSTR("HomeAddress"), address);
	VERIFY_IS_NOT_NULL(test_value);
	auto service_context = ::odata::make_shared<InMemoryEntities>(_XPLATSTR("http://0.0.0.1"));

	auto person = ::odata::make_shared<Person>(service_context);
	person->from_value(test_value);

	auto address_member = person->get_address();
	VERIFY_IS_NOT_NULL(address_member);
	VERIFY_ARE_EQUAL(address_member->get_city(), _XPLATSTR("Shang Hai"));
	VERIFY_ARE_EQUAL(address_member->get_street(), _XPLATSTR("Road 1111"));
}

TEST(map_binary_to_entity)
{
	auto test_value = make_product_object();
	VERIFY_IS_NOT_NULL(test_value);
	auto service_context = ::odata::make_shared<InMemoryEntities>(_XPLATSTR("http://0.0.0.1"));

	auto product = ::odata::make_shared<Product>(service_context);
	product->from_value(test_value);

	auto binary = product->get_pic();
	VERIFY_ARE_EQUAL(binary[7], '1');
	VERIFY_ARE_EQUAL(binary[9], '*');

	auto null_binary = *(product->get_nullpic());
	VERIFY_ARE_EQUAL(null_binary[7], '1');
	VERIFY_ARE_EQUAL(null_binary[9], '*');
}

TEST(map_enum_to_entity)
{
	auto test_value = make_product_object();
	VERIFY_IS_NOT_NULL(test_value);
	auto service_context = ::odata::make_shared<InMemoryEntities>(_XPLATSTR("http://0.0.0.1"));

	auto product = ::odata::make_shared<Product>(service_context);
	product->from_value(test_value);

	VERIFY_ARE_EQUAL(product->get_useraccess(), AccessLevel::ReadWrite);
}

TEST(map_collection_enum_to_entity)
{
	auto test_value = make_product_object();
	VERIFY_IS_NOT_NULL(test_value);
	auto service_context = ::odata::make_shared<InMemoryEntities>(_XPLATSTR("http://0.0.0.1"));

	auto product = ::odata::make_shared<Product>(service_context);
	product->from_value(test_value);

	VERIFY_ARE_EQUAL(product->get_covercolors().size(), 3);
	VERIFY_ARE_EQUAL(product->get_covercolors()[0], Color::Red);
	VERIFY_ARE_EQUAL(product->get_covercolors()[1], Color::Blue);
	VERIFY_ARE_EQUAL(product->get_covercolors()[2], Color::Green);
}

TEST(map_nullable_enum_to_entity_with_null)
{
	auto test_value = make_product_object();
	VERIFY_IS_NOT_NULL(test_value);
	auto service_context = ::odata::make_shared<InMemoryEntities>(_XPLATSTR("http://0.0.0.1"));

	auto product = ::odata::make_shared<Product>(service_context);
	product->from_value(test_value);

	VERIFY_IS_NULL(product->get_skincolor());
}

TEST(map_nullable_enum_to_entity_with_value)
{
	auto test_value = make_product_object();
	VERIFY_IS_NOT_NULL(test_value);
	auto model = get_test_model();
	test_value->set_value(_XPLATSTR("SkinColor"), ::odata::make_shared<odata_enum_value>(model->find_enum_type(_XPLATSTR("Color")), _XPLATSTR("Red")));
	auto service_context = ::odata::make_shared<InMemoryEntities>(_XPLATSTR("http://0.0.0.1"));

	auto product = ::odata::make_shared<Product>(service_context);
	product->from_value(test_value);

	VERIFY_IS_NOT_NULL(product->get_skincolor());
	VERIFY_ARE_EQUAL(*(product->get_skincolor()), Color::Red);
}

TEST(map_enum_to_complex)
{
	auto test_value = make_address_object();
	VERIFY_IS_NOT_NULL(test_value);
	auto service_context = ::odata::make_shared<InMemoryEntities>(_XPLATSTR("http://0.0.0.1"));

	auto address = ::odata::make_shared<Address>(service_context);
	address->from_value(test_value);

	VERIFY_ARE_EQUAL(address->get_color(), Color::Red);
}

TEST(map_collection_enum_to_complex)
{
	auto test_value = make_address_object();
	VERIFY_IS_NOT_NULL(test_value);
	auto service_context = ::odata::make_shared<InMemoryEntities>(_XPLATSTR("http://0.0.0.1"));

	auto address = ::odata::make_shared<Address>(service_context);
	address->from_value(test_value);

	VERIFY_ARE_EQUAL(address->get_covercolors().size(), 3);
	VERIFY_ARE_EQUAL(address->get_covercolors()[0], Color::Red);
	VERIFY_ARE_EQUAL(address->get_covercolors()[1], Color::Blue);
	VERIFY_ARE_EQUAL(address->get_covercolors()[2], Color::Green);
}

TEST(map_nullable_enum_to_complex_with_null)
{
	auto test_value = make_address_object();
	VERIFY_IS_NOT_NULL(test_value);
	auto service_context = ::odata::make_shared<InMemoryEntities>(_XPLATSTR("http://0.0.0.1"));

	auto address = ::odata::make_shared<Address>(service_context);
	address->from_value(test_value);

	VERIFY_IS_NULL(address->get_skincolor());
}

TEST(map_nullable_enum_to_complex_with_value)
{
	auto test_value = make_address_object();
	VERIFY_IS_NOT_NULL(test_value);
	auto model = get_test_model();
	test_value->set_value(_XPLATSTR("SkinColor"), ::odata::make_shared<odata_enum_value>(model->find_enum_type(_XPLATSTR("Color")), _XPLATSTR("Red")));
	auto service_context = ::odata::make_shared<InMemoryEntities>(_XPLATSTR("http://0.0.0.1"));

	auto address = ::odata::make_shared<Address>(service_context);
	address->from_value(test_value);

	VERIFY_IS_NOT_NULL(address->get_skincolor());
	VERIFY_ARE_EQUAL(*(address->get_skincolor()), Color::Red);
}

TEST(map_navigation_to_entity)
{
	auto test_value = make_account_object_with_navigation();
	VERIFY_IS_NOT_NULL(test_value);
	auto service_context = ::odata::make_shared<InMemoryEntities>(_XPLATSTR("http://0.0.0.1"));

	auto account = ::odata::make_shared<Account>(service_context);
	account->from_value(test_value);

	auto giftcard = account->get_giftcard();
	VERIFY_IS_NOT_NULL(giftcard);
	VERIFY_ARE_EQUAL(giftcard->get_edit_link(), _XPLATSTR(""));
	VERIFY_ARE_EQUAL(giftcard->get_giftcardid(), 77566);
	VERIFY_ARE_EQUAL(giftcard->get_giftcardno(), _XPLATSTR("@@2232"));
}

TEST(map_collection_navigation_to_entity)
{
	auto test_value = make_account_object_with_navigation();
	VERIFY_IS_NOT_NULL(test_value);
	auto service_context = ::odata::make_shared<InMemoryEntities>(_XPLATSTR("http://0.0.0.1"));

	auto account = ::odata::make_shared<Account>(service_context);
	account->from_value(test_value);

	auto pis = account->get_mypaymentinstruments();
	VERIFY_ARE_EQUAL(pis.size(), 2);
	VERIFY_ARE_EQUAL(pis[0]->get_edit_link(), _XPLATSTR(""));
	VERIFY_ARE_EQUAL(pis[0]->get_paymentinstrumentid(), 100000);
	VERIFY_ARE_EQUAL(pis[0]->get_friendlyname(), _XPLATSTR("PI 1"));
}

TEST(map_derived_navigation_to_entity)
{
	auto test_value = make_account_object_with_navigation();
	VERIFY_IS_NOT_NULL(test_value);
	auto service_context = ::odata::make_shared<InMemoryEntities>(_XPLATSTR("http://0.0.0.1"));

	auto account = ::odata::make_shared<Account>(service_context);
	account->from_value(test_value);

	auto pis = account->get_mypaymentinstruments();
	VERIFY_ARE_EQUAL(pis.size(), 2);

	auto credpi = std::dynamic_pointer_cast<CreditCardPI>(pis[1]);
	VERIFY_IS_NOT_NULL(credpi);
	VERIFY_ARE_EQUAL(credpi->get_edit_link(), _XPLATSTR("http://0.0.0.1/PaymentInstrument(200000)"));
	VERIFY_ARE_EQUAL(credpi->get_paymentinstrumentid(), 200000);
	VERIFY_ARE_EQUAL(credpi->get_friendlyname(), _XPLATSTR("PI 2"));
	VERIFY_ARE_EQUAL(credpi->get_cardnumber(), _XPLATSTR("777777"));
}

class test_service_context : public InMemoryEntities
{
public:
	test_service_context(const ::odata::string_t& baseAddress, client_options options = client_options())
		: InMemoryEntities(baseAddress, options)
	{
		m_model = get_test_model();
	}

	~test_service_context()
	{
		std::wcout << _XPLATSTR("destroy test_service_context") << std::endl;
	}
};

TEST(get_primitive_value_from_entity_class)
{
	auto service_context = ::odata::make_shared<test_service_context>(_XPLATSTR("http://0.0.0.1"));
	auto account = ::odata::make_shared<Account>(service_context);
	account->set_accountid(1001212);

	auto entity_value = account->to_value();
	int32_t id;
	auto ok = entity_value->try_get(_XPLATSTR("AccountID"), id);
	VERIFY_ARE_EQUAL(ok, true);
	VERIFY_ARE_EQUAL(id, 1001212);
}

TEST(get_collection_primitive_value_from_entity_class)
{
	auto service_context = ::odata::make_shared<test_service_context>(_XPLATSTR("http://0.0.0.1"));
	auto account = ::odata::make_shared<Person>(service_context);
	std::vector<::odata::string_t> numbers;
	numbers.push_back(_XPLATSTR("1"));
	numbers.push_back(_XPLATSTR("2"));
	numbers.push_back(_XPLATSTR("3"));
	numbers.push_back(_XPLATSTR("4"));
	numbers.push_back(_XPLATSTR("567890"));
	account->set_numbers(numbers);

	auto entity_value = account->to_value();
	std::shared_ptr<::odata::core::odata_value> value;
	auto ok = entity_value->get_property_value(_XPLATSTR("Numbers"), value);
	auto collection_value = std::dynamic_pointer_cast<::odata::core::odata_collection_value>(value);
	VERIFY_IS_NOT_NULL(collection_value);
	VERIFY_ARE_EQUAL(collection_value->get_collection_values().size(), 5);
	auto v1 =  std::dynamic_pointer_cast<::odata::core::odata_primitive_value>(collection_value->get_collection_values()[0]);
	VERIFY_IS_NOT_NULL(v1);
	VERIFY_ARE_EQUAL(v1->to_string(), _XPLATSTR("1"));
	auto v5 =  std::dynamic_pointer_cast<::odata::core::odata_primitive_value>(collection_value->get_collection_values()[4]);
	VERIFY_IS_NOT_NULL(v5);
	VERIFY_ARE_EQUAL(v5->to_string(), _XPLATSTR("567890"));
}

TEST(get_nullable_primitive_value_from_entity_class)
{
	auto service_context = ::odata::make_shared<test_service_context>(_XPLATSTR("http://0.0.0.1"));
	auto account = ::odata::make_shared<Person>(service_context);
	account->set_middlename(_XPLATSTR("Middle Name"));

	auto entity_value = account->to_value();
	::odata::string_t middlename;
	auto ok = entity_value->try_get(_XPLATSTR("MiddleName"), middlename);
	VERIFY_ARE_EQUAL(ok, true);
	VERIFY_ARE_EQUAL(middlename, _XPLATSTR("Middle Name"));
}

TEST(get_primitive_value_from_complex_class)
{
	auto service_context = ::odata::make_shared<test_service_context>(_XPLATSTR("http://0.0.0.1"));
	auto address = ::odata::make_shared<Address>(service_context);
	address->set_city(_XPLATSTR("Shangh Hai"));

	auto entity_value = address->to_value();
	::odata::string_t city;
	auto ok = entity_value->try_get(_XPLATSTR("City"), city);
	VERIFY_ARE_EQUAL(ok, true);
	VERIFY_ARE_EQUAL(city, _XPLATSTR("Shangh Hai"));
}

TEST(get_collection_primitive_value_from_complex_class)
{
	auto service_context = ::odata::make_shared<test_service_context>(_XPLATSTR("http://0.0.0.1"));
	auto address = ::odata::make_shared<Address>(service_context);
	std::vector<int32_t> numbers;
	numbers.push_back(100);
	numbers.push_back(2100);
	numbers.push_back(3100);
	numbers.push_back(5100);
	numbers.push_back(567890);
	address->set_numbers(numbers);

	auto entity_value = address->to_value();
	std::shared_ptr<::odata::core::odata_value> value;
	auto ok = entity_value->get_property_value(_XPLATSTR("Numbers"), value);
	auto collection_value = std::dynamic_pointer_cast<::odata::core::odata_collection_value>(value);
	VERIFY_IS_NOT_NULL(collection_value);
	VERIFY_ARE_EQUAL(collection_value->get_collection_values().size(), 5);
	auto v1 =  std::dynamic_pointer_cast<::odata::core::odata_primitive_value>(collection_value->get_collection_values()[0]);
	VERIFY_IS_NOT_NULL(v1);
	VERIFY_ARE_EQUAL(v1->to_string(), _XPLATSTR("100"));
	auto v5 =  std::dynamic_pointer_cast<::odata::core::odata_primitive_value>(collection_value->get_collection_values()[4]);
	VERIFY_IS_NOT_NULL(v5);
	VERIFY_ARE_EQUAL(v5->to_string(), _XPLATSTR("567890"));
}

TEST(get_nullable_primitive_value_from_complex_class)
{
	auto service_context = ::odata::make_shared<test_service_context>(_XPLATSTR("http://0.0.0.1"));
	auto address = ::odata::make_shared<Address>(service_context);
	address->set_code(-1231233);

	auto entity_value = address->to_value();
	int32_t code;
	auto ok = entity_value->try_get(_XPLATSTR("Code"), code);
	VERIFY_ARE_EQUAL(ok, true);
	VERIFY_ARE_EQUAL(code, -1231233);
}

TEST(get_complex_value_from_entity_class)
{
	auto service_context = ::odata::make_shared<test_service_context>(_XPLATSTR("http://0.0.0.1"));
	auto account = ::odata::make_shared<Account>(service_context);
	auto accountinfo = ::odata::make_shared<AccountInfo>(service_context);
	accountinfo->set_firstname(_XPLATSTR("Leo"));
	accountinfo->set_lastname(_XPLATSTR("Hu"));
	account->set_accountinfo(accountinfo);

	auto entity_value = account->to_value();
	std::shared_ptr<::odata::core::odata_value> value;
	auto ok = entity_value->get_property_value(_XPLATSTR("AccountInfo"), value);
	auto complex_value = std::dynamic_pointer_cast<::odata::core::odata_complex_value>(value);
	VERIFY_IS_NOT_NULL(complex_value);
	::odata::string_t firstname, lastname;
	complex_value->try_get(_XPLATSTR("FirstName"), firstname);
	complex_value->try_get(_XPLATSTR("LastName"), lastname);
	VERIFY_ARE_EQUAL(firstname, _XPLATSTR("Leo"));
	VERIFY_ARE_EQUAL(lastname, _XPLATSTR("Hu"));
}

TEST(get_derived_complex_value_from_entity_class)
{
	auto service_context = ::odata::make_shared<test_service_context>(_XPLATSTR("http://0.0.0.1"));
	auto account = ::odata::make_shared<Account>(service_context);
	auto  vipinfo = ::odata::make_shared<VipAccountInfo>(service_context);
	vipinfo->set_firstname(_XPLATSTR("Leo"));
	vipinfo->set_lastname(_XPLATSTR("Hu"));
	vipinfo->set_vipinfo(_XPLATSTR("Gold"));
	account->set_vipinfo(vipinfo);

	auto entity_value = account->to_value();
	std::shared_ptr<::odata::core::odata_value> value;
	auto ok = entity_value->get_property_value(_XPLATSTR("VIPAccountInfo"), value);
	auto complex_value = std::dynamic_pointer_cast<::odata::core::odata_complex_value>(value);
	VERIFY_IS_NOT_NULL(complex_value);
	::odata::string_t firstname, lastname, vip;
	complex_value->try_get(_XPLATSTR("FirstName"), firstname);
	complex_value->try_get(_XPLATSTR("LastName"), lastname);
	complex_value->try_get(_XPLATSTR("VIP_Info"), vip);
	VERIFY_ARE_EQUAL(firstname, _XPLATSTR("Leo"));
	VERIFY_ARE_EQUAL(lastname, _XPLATSTR("Hu"));
	VERIFY_ARE_EQUAL(vip, _XPLATSTR("Gold"));
}

TEST(get_collection_complex_value_from_entity_class)
{
	auto service_context = ::odata::make_shared<test_service_context>(_XPLATSTR("http://0.0.0.1"));
	auto employee = ::odata::make_shared<Employee>(service_context);
  	std::vector<std::shared_ptr<Address>> addresses;
	auto address_1 = ::odata::make_shared<Address>(service_context);
	address_1->set_street(_XPLATSTR("zixinglu"));
	addresses.push_back(address_1);
	auto address_2 = ::odata::make_shared<Address>(service_context);
	address_2->set_street(_XPLATSTR("dongchuanlu"));
	addresses.push_back(address_2);
	employee->set_addresses(addresses);

	auto entity_value = employee->to_value();
	std::shared_ptr<::odata::core::odata_value> value;
	auto ok = entity_value->get_property_value(_XPLATSTR("WorkAddresses"), value);
	auto collection_value = std::dynamic_pointer_cast<::odata::core::odata_collection_value>(value);
	VERIFY_IS_NOT_NULL(collection_value);
	VERIFY_ARE_EQUAL(collection_value->get_collection_values().size(), 2);
	::odata::string_t street;
	auto c1 =  std::dynamic_pointer_cast<::odata::core::odata_complex_value>(collection_value->get_collection_values()[0]);
	VERIFY_IS_NOT_NULL(c1);
	c1->try_get(_XPLATSTR("Street"), street);
	VERIFY_ARE_EQUAL(street, _XPLATSTR("zixinglu"));
	auto c2 =  std::dynamic_pointer_cast<::odata::core::odata_complex_value>(collection_value->get_collection_values()[1]);
	VERIFY_IS_NOT_NULL(c2);
	c2->try_get(_XPLATSTR("Street"), street);
	VERIFY_ARE_EQUAL(street, _XPLATSTR("dongchuanlu"));
}

TEST(get_nullable_complex_value_from_entity_class)
{
	auto service_context = ::odata::make_shared<test_service_context>(_XPLATSTR("http://0.0.0.1"));
	auto person = ::odata::make_shared<Person>(service_context);
  	std::vector<std::shared_ptr<Address>> addresses;
	auto address = ::odata::make_shared<Address>(service_context);
	address->set_street(_XPLATSTR("zixinglu 999"));
	person->set_address(address);

	auto entity_value = person->to_value();
	std::shared_ptr<::odata::core::odata_value> value;
	auto ok = entity_value->get_property_value(_XPLATSTR("HomeAddress"), value);
	auto complex_value = std::dynamic_pointer_cast<::odata::core::odata_complex_value>(value);
	VERIFY_IS_NOT_NULL(complex_value);
	::odata::string_t street;
	complex_value->try_get(_XPLATSTR("Street"), street);
	VERIFY_ARE_EQUAL(street, _XPLATSTR("zixinglu 999"));
}

TEST(get_enum_value_from_entity_class)
{
	auto service_context = ::odata::make_shared<test_service_context>(_XPLATSTR("http://0.0.0.1"));
	auto product = ::odata::make_shared<Product>(service_context);
	product->set_useraccess(AccessLevel::Write);

	auto entity_value = product->to_value();
	std::shared_ptr<::odata::core::odata_value> value;
	auto ok = entity_value->get_property_value(_XPLATSTR("UserAccess"), value);
	auto enum_value = std::dynamic_pointer_cast<::odata::core::odata_enum_value>(value);
	VERIFY_IS_NOT_NULL(enum_value);
	VERIFY_ARE_EQUAL(enum_value->to_string(), _XPLATSTR("Write"));
}

TEST(get_collection_enum_value_from_entity_class)
{
	auto service_context = ::odata::make_shared<test_service_context>(_XPLATSTR("http://0.0.0.1"));
	auto product = ::odata::make_shared<Product>(service_context);
	std::vector<Color> colors;
	colors.push_back(Color::Red);
	colors.push_back(Color::Blue);
	product->set_covercolors(colors);

	auto entity_value = product->to_value();
	std::shared_ptr<::odata::core::odata_value> value;
	auto ok = entity_value->get_property_value(_XPLATSTR("CoverColors"), value);
	auto collection_value = std::dynamic_pointer_cast<::odata::core::odata_collection_value>(value);
	VERIFY_IS_NOT_NULL(collection_value);
	VERIFY_ARE_EQUAL(collection_value->get_collection_values().size(), 2);
	auto e1 =  std::dynamic_pointer_cast<::odata::core::odata_enum_value>(collection_value->get_collection_values()[0]);
	VERIFY_ARE_EQUAL(e1->get_value_type()->get_type_kind(), edm_type_kind_t::Enum);
	VERIFY_ARE_EQUAL(e1->to_string(), _XPLATSTR("Red"));
	auto e2 =  std::dynamic_pointer_cast<::odata::core::odata_enum_value>(collection_value->get_collection_values()[1]);
	VERIFY_ARE_EQUAL(e2->to_string(), _XPLATSTR("Blue"));
}

TEST(get_nullable_enum_value_from_entity_class)
{
	auto service_context = ::odata::make_shared<test_service_context>(_XPLATSTR("http://0.0.0.1"));
	auto product = ::odata::make_shared<Product>(service_context);
	product->set_skincolor(Color::Green);

	auto entity_value = product->to_value();
	std::shared_ptr<::odata::core::odata_value> value;
	auto ok = entity_value->get_property_value(_XPLATSTR("SkinColor"), value);
	auto enum_value = std::dynamic_pointer_cast<::odata::core::odata_enum_value>(value);
	VERIFY_IS_NOT_NULL(enum_value);
	VERIFY_ARE_EQUAL(enum_value->to_string(), _XPLATSTR("Green"));
}

TEST(get_enum_value_from_complex_class)
{
	auto service_context = ::odata::make_shared<test_service_context>(_XPLATSTR("http://0.0.0.1"));
	auto address = ::odata::make_shared<Address>(service_context);
	address->set_color(Color::Red);

	auto entity_value = address->to_value();
	std::shared_ptr<::odata::core::odata_value> value;
	auto ok = entity_value->get_property_value(_XPLATSTR("Color"), value);
	auto enum_value = std::dynamic_pointer_cast<::odata::core::odata_enum_value>(value);
	VERIFY_IS_NOT_NULL(enum_value);
	VERIFY_ARE_EQUAL(enum_value->to_string(), _XPLATSTR("Red"));
}

TEST(get_collection_enum_value_from_complex_class)
{
	auto service_context = ::odata::make_shared<test_service_context>(_XPLATSTR("http://0.0.0.1"));
	auto address = ::odata::make_shared<Address>(service_context);
	std::vector<Color> colors;
	colors.push_back(Color::Red);
	colors.push_back(Color::Blue);
	address->set_covercolors(colors);

	auto entity_value = address->to_value();
	std::shared_ptr<::odata::core::odata_value> value;
	auto ok = entity_value->get_property_value(_XPLATSTR("CoverColors"), value);
	auto collection_value = std::dynamic_pointer_cast<::odata::core::odata_collection_value>(value);
	VERIFY_IS_NOT_NULL(collection_value);
	VERIFY_ARE_EQUAL(collection_value->get_collection_values().size(), 2);
	auto e1 =  std::dynamic_pointer_cast<::odata::core::odata_enum_value>(collection_value->get_collection_values()[0]);
	VERIFY_ARE_EQUAL(e1->to_string(), _XPLATSTR("Red"));
	auto e2 =  std::dynamic_pointer_cast<::odata::core::odata_enum_value>(collection_value->get_collection_values()[1]);
	VERIFY_ARE_EQUAL(e2->to_string(), _XPLATSTR("Blue"));
}

TEST(get_nullable_enum_value_from_complex_class)
{
	auto service_context = ::odata::make_shared<test_service_context>(_XPLATSTR("http://0.0.0.1"));
	auto address = ::odata::make_shared<Address>(service_context);
	address->set_skincolor(Color::Green);

	auto entity_value = address->to_value();
	std::shared_ptr<::odata::core::odata_value> value;
	auto ok = entity_value->get_property_value(_XPLATSTR("SkinColor"), value);
	auto enum_value = std::dynamic_pointer_cast<::odata::core::odata_enum_value>(value);
	VERIFY_IS_NOT_NULL(enum_value);
	VERIFY_ARE_EQUAL(enum_value->to_string(), _XPLATSTR("Green"));
}

TEST(get_key_string_with_one_key)
{
    auto service_context = ::odata::make_shared<test_service_context>(_XPLATSTR("http://0.0.0.1"));
	auto product = ::odata::make_shared<Product>(service_context);
	product->set_productid(10001);

	auto key_string = product->get_key_property_string();
	VERIFY_ARE_EQUAL(key_string, _XPLATSTR("10001"));
}

TEST(get_key_string_with_double_key)
{
    auto service_context = ::odata::make_shared<test_service_context>(_XPLATSTR("http://0.0.0.1"));
	auto productdetail = ::odata::make_shared<ProductDetail>(service_context);
	productdetail->set_productid(10001);
	productdetail->set_productdetailedid(20002);

	auto key_string = productdetail->get_key_property_string();
	VERIFY_ARE_EQUAL(key_string, _XPLATSTR("ProductID=10001,ProductDetailID=20002"));
}

TEST(get_key_string_with_one_key_from_base_class)
{
    auto service_context = ::odata::make_shared<test_service_context>(_XPLATSTR("http://0.0.0.1"));
	auto employee = ::odata::make_shared<Employee>(service_context);
	employee->set_personid(10101);

	auto key_string = employee->get_key_property_string();
	VERIFY_ARE_EQUAL(key_string, _XPLATSTR("10101"));
}

TEST(get_double_key_string_with_one_key_from_base_class)
{
    auto service_context = ::odata::make_shared<test_service_context>(_XPLATSTR("http://0.0.0.1"));
	auto customer = ::odata::make_shared<Customer>(service_context);
	customer->set_personid(10101);
	customer->set_city(_XPLATSTR("Shang Hai"));

	auto key_string = customer->get_key_property_string();
	VERIFY_ARE_EQUAL(key_string, _XPLATSTR("PersonID=10101,City=Shang Hai"));
}

TEST(get_fourfold_key_string_with_two_key_from_base_class)
{
	auto service_context = ::odata::make_shared<test_service_context>(_XPLATSTR("http://0.0.0.1"));
	auto vip = ::odata::make_shared<VipCustomer>(service_context);
	vip->set_personid(10101);
	vip->set_city(_XPLATSTR("Shang Hai"));
	vip->set_vipid(70000);
	vip->set_viptype(_XPLATSTR("Gold"));

	auto key_string = vip->get_key_property_string();
	VERIFY_ARE_EQUAL(key_string, _XPLATSTR("PersonID=10101,City=Shang HaiVIPID=70000,VIPType=Gold"));
}

TEST (change_entity_mapping_array)
{
	auto service_context = ::odata::make_shared<test_service_context>(_XPLATSTR("http://0.0.0.1"));
	auto vip = ::odata::make_shared<VipCustomer>(service_context);
	vip->set_personid(10101);
	vip->set_city(_XPLATSTR("Shang Hai"));
	vip->set_vipid(70000);
	vip->set_viptype(_XPLATSTR("Gold"));

	auto vip2 = ::odata::make_shared<VipCustomer>(service_context);
	vip2->set_personid(10102);
	vip2->set_city(_XPLATSTR("Shen Yang"));
	vip2->set_vipid(90000);
	vip2->set_viptype(_XPLATSTR("White Gold"));

	PROPMAP_ENTRY_IN_ENTITY _to_entity_map[] =
	{
		{(PROP_MAP_CALL_IN_ENTITY)0}
	};

	auto vip_map = vip->get_to_entity_map();
	vip->set_to_entity_map(_to_entity_map);
	auto customer_map = (std::dynamic_pointer_cast<Customer>(vip))->get_to_entity_map();
	(std::dynamic_pointer_cast<Customer>(vip))->set_to_entity_map(_to_entity_map);
	auto person_map = (std::dynamic_pointer_cast<Person>(vip))->get_to_entity_map();
	(std::dynamic_pointer_cast<Person>(vip))->set_to_entity_map(_to_entity_map);
	auto entity_value = vip->to_value();
	VERIFY_ARE_EQUAL(entity_value->properties().size(), 1);

	vip->set_to_entity_map(vip_map);
	entity_value = vip->to_value();
	VERIFY_ARE_EQUAL(entity_value->properties().size(), 3);
	::odata::string_t vip_type;
	entity_value->try_get(_XPLATSTR("VIPType"), vip_type);
	VERIFY_ARE_EQUAL(vip_type, _XPLATSTR("Gold"));

	(std::dynamic_pointer_cast<Customer>(vip))->set_to_entity_map(customer_map);
	entity_value = vip->to_value();
	VERIFY_ARE_EQUAL(entity_value->properties().size(), 4);
	VERIFY_ARE_EQUAL(entity_value->has_property(_XPLATSTR("PersonID")), false);

	(std::dynamic_pointer_cast<Person>(vip))->set_to_entity_map(person_map);
	entity_value = vip->to_value();
	VERIFY_ARE_EQUAL(entity_value->properties().size(), 8);
	int32_t persion_id;
	entity_value->try_get(_XPLATSTR("PersonID"), persion_id);
	VERIFY_ARE_EQUAL(persion_id, 10101);

	auto entity_value_2 = vip2->to_value();
	VERIFY_ARE_EQUAL(entity_value_2->properties().size(), 8);
	entity_value_2->try_get(_XPLATSTR("VIPType"), vip_type);
	VERIFY_ARE_EQUAL(vip_type, _XPLATSTR("White Gold"));
}

}

}}}