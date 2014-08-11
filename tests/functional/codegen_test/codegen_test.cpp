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
#include "odata/edm/edm_type.h"
#include "odata/codegen/code_generation.h"
#include "codegen_test_service.h"

using namespace ::odata::edm;
using namespace ::odata::core;
using namespace ::odata::utility;

namespace tests { namespace functional { namespace _odata {

static std::shared_ptr<odata_entity_value> make_account_object()
{
	auto model = get_test_model();

	auto complex_value = std::make_shared<odata_complex_value>(model->find_complex_type(U("AccountInfo")));
	complex_value->set_value(U("FirstName"), U("Leo"));
	complex_value->set_value(U("LastName"), U("Hu"));

	auto entity_value = std::make_shared<odata_entity_value>(model->find_entity_type(U("Account")));
	entity_value->set_value(U("AccountID"), (int32_t)989899);
	entity_value->set_value(U("Country"), U("China"));
	entity_value->set_value(U("AccountInfo"), complex_value);
	entity_value->set_value(PAYLOAD_ANNOTATION_EDITLINK, U("http://0.0.0.1/Account(989899)"));

	return entity_value;
}

static std::shared_ptr<odata_entity_value> make_giftcard_object()
{
	auto model = get_test_model();

	auto entity_value = std::make_shared<odata_entity_value>(model->find_entity_type(U("GiftCard")));
	entity_value->set_value(U("GiftCardID"), (int32_t)77566);
	entity_value->set_value(U("GiftCardNO"), U("@@2232"));

	return entity_value;
}

static std::shared_ptr<odata_entity_value> make_paymentinstrument_object()
{
	auto model = get_test_model();

	auto entity_value = std::make_shared<odata_entity_value>(model->find_entity_type(U("PaymentInstrument")));

	return entity_value;
}

static std::shared_ptr<odata_entity_value> make_creditcardpi_object()
{
	auto model = get_test_model();

	auto entity_value = std::make_shared<odata_entity_value>(model->find_entity_type(U("CreditCardPI")));

	return entity_value;
}

static std::shared_ptr<odata_entity_value> make_account_object_with_navigation()
{
	auto model = get_test_model();

	auto entity_value = make_account_object();
	auto giftcard_value = make_giftcard_object();
	entity_value->set_value(U("MyGiftCard"), giftcard_value);

	auto pi_1 = make_paymentinstrument_object();
	pi_1->set_value(U("PaymentInstrumentID"), 100000);
	pi_1->set_value(U("FriendlyName"), U("PI 1"));

	auto pi_2 = make_creditcardpi_object(); 
	pi_2->set_value(U("@odata.type"), "#Microsoft.Test.OData.Services.ODataWCFService.CreditCardPI");
	pi_2->set_value(U("PaymentInstrumentID"), 200000);
	pi_2->set_value(U("FriendlyName"), U("PI 2"));
	pi_2->set_value(U("CardNumber"), U("777777"));
	pi_2->set_value(PAYLOAD_ANNOTATION_EDITLINK, U("http://0.0.0.1/PaymentInstrument(200000)"));

	auto collection_value = std::make_shared<odata_collection_value>(model->find_entity_type(U("PaymentInstrument")));
	collection_value->add_collection_value(pi_1);
	collection_value->add_collection_value(pi_2);

	entity_value->set_value(U("MyPaymentInstruments"), collection_value);

	return entity_value;
}

static std::shared_ptr<odata_entity_value> make_person_object()
{
	auto model = get_test_model();

	auto entity_value = std::make_shared<odata_entity_value>(model->find_entity_type(U("Person")));
	entity_value->set_value(U("PersonID"), (int32_t)12123);
	entity_value->set_value(U("FirstName"), U("Leo"));
	entity_value->set_value(U("MiddleName"), U("Middle Name"));
	auto collection_value = std::make_shared<odata_collection_value>(edm_primitive_type::STRING());
	collection_value->add_collection_value(odata_primitive_value::make_primitive_value(U("string 1")));
	collection_value->add_collection_value(odata_primitive_value::make_primitive_value(U("string 2")));
	collection_value->add_collection_value(odata_primitive_value::make_primitive_value(U("string 3")));
	entity_value->set_value(U("Numbers"), collection_value);

	return entity_value;
}

static std::shared_ptr<odata_complex_value> make_accountinfo_object()
{
	auto model = get_test_model();

	auto complex_value = std::make_shared<odata_complex_value>(model->find_complex_type(U("AccountInfo")));
	complex_value->set_value(U("FirstName"), U("Leo  "));
	complex_value->set_value(U("LastName"), U("  Hu"));

	return complex_value;
}

static std::shared_ptr<odata_complex_value> make_address_object()
{
	auto model = get_test_model();

	auto complex_value = std::make_shared<odata_complex_value>(model->find_complex_type(U("Address")));
	complex_value->set_value(U("Street"), U("Road 1111"));
	complex_value->set_value(U("City"), U("Shang Hai"));
	complex_value->set_value(U("Code"), 12123);
	auto collection_value = std::make_shared<odata_collection_value>(edm_primitive_type::INT32());
	collection_value->add_collection_value(odata_primitive_value::make_primitive_value(2213213));
	collection_value->add_collection_value(odata_primitive_value::make_primitive_value(-212312));
	collection_value->add_collection_value(odata_primitive_value::make_primitive_value(0));
	complex_value->set_value(U("Numbers"), collection_value);

	complex_value->set_value(U("Color"), std::make_shared<odata_enum_value>(model->find_enum_type(U("Color")), U("Red")));

	auto enum_collection_value = std::make_shared<odata_collection_value>(model->find_complex_type(U("Color")));
	enum_collection_value->add_collection_value(std::make_shared<odata_enum_value>(model->find_enum_type(U("Color")), U("Red")));
	enum_collection_value->add_collection_value(std::make_shared<odata_enum_value>(model->find_enum_type(U("Color")), U("Blue")));
	enum_collection_value->add_collection_value(std::make_shared<odata_enum_value>(model->find_enum_type(U("Color")), U("Green")));
	complex_value->set_value(U("CoverColors"), enum_collection_value);

	return complex_value;
}

static std::shared_ptr<odata_entity_value> make_employee_object()
{
	auto model = get_test_model();

	auto entity_value = std::make_shared<odata_entity_value>(model->find_entity_type(U("Employee")));
	entity_value->set_value(U("PersonID"), (int32_t)10001);
	entity_value->set_value(U("FirstName"), U("Henry"));
	auto complex_type = model->find_complex_type(U("Microsoft.Test.OData.Services.ODataWCFService.Address"));
	auto collection_value = std::make_shared<odata_collection_value>(complex_type);
	auto complex_value_1 = std::make_shared<odata_complex_value>(complex_type);
	complex_value_1->set_value(U("Street"), U("Street 1"));
	complex_value_1->set_value(U("City"), U("City 1"));
	auto complex_value_2 = std::make_shared<odata_complex_value>(complex_type);
	complex_value_2->set_value(U("Street"), U("Street 2"));
	complex_value_2->set_value(U("City"), U("City 2"));
	auto complex_value_3 = std::make_shared<odata_complex_value>(complex_type);
	complex_value_3->set_value(U("Street"), U("Street 3"));
	complex_value_3->set_value(U("City"), U("City 3"));
	collection_value->add_collection_value(complex_value_1);
	collection_value->add_collection_value(complex_value_2);
	collection_value->add_collection_value(complex_value_3);
	entity_value->set_value(U("WorkAddresses"), collection_value);

	return entity_value;
}

static std::shared_ptr<odata_entity_value> make_product_object()
{
	auto model = get_test_model();

	auto entity_value = std::make_shared<odata_entity_value>(model->find_entity_type(U("Product")));
	entity_value->set_value(U("ProductID"), (int32_t)64454);
	entity_value->set_value(U("Name"), U("Henry"));
	entity_value->set_value(U("UserAccess"), std::make_shared<odata_enum_value>(model->find_enum_type(U("AccessLevel")), U("ReadWrite")));

	auto collection_value = std::make_shared<odata_collection_value>(model->find_complex_type(U("Color")));
	collection_value->add_collection_value(std::make_shared<odata_enum_value>(model->find_enum_type(U("Color")), U("Red")));
	collection_value->add_collection_value(std::make_shared<odata_enum_value>(model->find_enum_type(U("Color")), U("Blue")));
	collection_value->add_collection_value(std::make_shared<odata_enum_value>(model->find_enum_type(U("Color")), U("Green")));

	entity_value->set_value(U("CoverColors"), collection_value);

	unsigned char ini_binary[] = {'Q', 'B', 'A', 'B', 'A', 'C', 'X', '1', '2', '*', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'p'};
	std::vector<unsigned char> binary(ini_binary, ini_binary + 25);
	entity_value->set_value(U("Picture"), binary);

	entity_value->set_value(U("NullPicture"), binary);

	return entity_value;
}

SUITE(codegen_test)
{

TEST(map_primitive_to_entity)
{
	auto test_value = make_account_object();
	VERIFY_IS_NOT_NULL(test_value);
	auto service_context = std::make_shared<InMemoryEntities>(U("http://0.0.0.1"));

	auto account = std::make_shared<Account>(service_context);
	account->from_value(test_value);

	VERIFY_ARE_EQUAL(account->get_accountid(), 989899);
	VERIFY_ARE_EQUAL(account->get_country(), U("China"));
}

TEST(map_collection_primitive_to_entity)
{
	auto test_value = make_person_object();
	VERIFY_IS_NOT_NULL(test_value);
	auto service_context = std::make_shared<InMemoryEntities>(U("http://0.0.0.1"));

	auto person = std::make_shared<Person>(service_context);
	person->from_value(test_value);

	VERIFY_ARE_EQUAL(person->get_personid(), 12123);
	VERIFY_ARE_EQUAL(person->get_firstname(), U("Leo"));
	VERIFY_ARE_EQUAL(person->get_numbers().size(), 3);
	VERIFY_ARE_EQUAL(person->get_numbers()[0], U("string 1"));
	VERIFY_ARE_EQUAL(person->get_numbers()[1], U("string 2"));
	VERIFY_ARE_EQUAL(person->get_numbers()[2], U("string 3"));
}

TEST(map_nullable_primitive_to_entity)
{
	auto test_value = make_person_object();
	VERIFY_IS_NOT_NULL(test_value);
	auto service_context = std::make_shared<InMemoryEntities>(U("http://0.0.0.1"));

	auto person = std::make_shared<Person>(service_context);
	person->from_value(test_value);

	VERIFY_ARE_EQUAL(person->get_personid(), 12123);
	VERIFY_ARE_EQUAL(person->get_firstname(), U("Leo"));
	VERIFY_IS_NOT_NULL(person->get_middlename());
	VERIFY_ARE_EQUAL(*(person->get_middlename()), U("Middle Name"));
}

TEST(map_primitive_to_complex)
{
	auto test_value = make_accountinfo_object();
	VERIFY_IS_NOT_NULL(test_value);
	auto service_context = std::make_shared<InMemoryEntities>(U("http://0.0.0.1"));

	auto accountinfo = std::make_shared<AccountInfo>(service_context);
	accountinfo->from_value(test_value);

	VERIFY_ARE_EQUAL(accountinfo->get_firstname(), U("Leo  "));
	VERIFY_ARE_EQUAL(accountinfo->get_lastname(), U("  Hu"));
}

TEST(map_collection_primitive_to_complex)
{
	auto test_value = make_address_object();
	VERIFY_IS_NOT_NULL(test_value);
	auto service_context = std::make_shared<InMemoryEntities>(U("http://0.0.0.1"));

	auto address = std::make_shared<Address>(service_context);
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
	auto service_context = std::make_shared<InMemoryEntities>(U("http://0.0.0.1"));

	auto address = std::make_shared<Address>(service_context);
	VERIFY_IS_NULL(address->get_code());
	address->from_value(test_value);
	VERIFY_IS_NOT_NULL(address->get_code());
	VERIFY_ARE_EQUAL(*(address->get_code()), 12123);
}

TEST(map_complex_to_entity)
{
	auto test_value = make_account_object();
	VERIFY_IS_NOT_NULL(test_value);
	auto service_context = std::make_shared<InMemoryEntities>(U("http://0.0.0.1"));

	auto account = std::make_shared<Account>(service_context);
	account->from_value(test_value);

	auto account_info = account->get_accountinfo();
	VERIFY_ARE_EQUAL(account_info->get_firstname(), U("Leo"));
	VERIFY_ARE_EQUAL(account_info->get_lastname(), U("Hu"));
}

TEST(map_derived_complex_to_entity)
{
	auto test_value = make_account_object();
	VERIFY_IS_NOT_NULL(test_value);
	auto service_context = std::make_shared<InMemoryEntities>(U("http://0.0.0.1"));

	auto complex_value = std::make_shared<odata_complex_value>(get_test_model()->find_complex_type(U("VIPAccountInfo")));
	complex_value->set_value(U("VIP_Info"), U("Diamond"));
	complex_value->set_value(U("FirstName"), U("Henry"));
	complex_value->set_value(U("LastName"), U("Yin"));

	test_value->set_value(U("VIPAccountInfo"), complex_value);

	auto account = std::make_shared<Account>(service_context);
	account->from_value(test_value);

	auto account_info = account->get_accountinfo();
	VERIFY_ARE_EQUAL(account_info->get_firstname(), U("Leo"));
	VERIFY_ARE_EQUAL(account_info->get_lastname(), U("Hu"));

	auto vip_info = account->get_vipinfo();
	VERIFY_ARE_EQUAL(vip_info->get_firstname(), U("Henry"));
	VERIFY_ARE_EQUAL(vip_info->get_lastname(), U("Yin"));
	VERIFY_ARE_EQUAL(vip_info->get_vipinfo(), U("Diamond"));
}

TEST(map_collection_complex_to_entity)
{
	auto test_value = make_employee_object();
	VERIFY_IS_NOT_NULL(test_value);
	auto service_context = std::make_shared<InMemoryEntities>(U("http://0.0.0.1"));
	
	auto employee = std::make_shared<Employee>(service_context);
	employee->from_value(test_value);

	VERIFY_ARE_EQUAL(employee->get_addresses().size(), 3);
	VERIFY_ARE_EQUAL(employee->get_addresses()[0]->get_street(), U("Street 1"));
	VERIFY_ARE_EQUAL(employee->get_addresses()[0]->get_city(), U("City 1"));
	VERIFY_ARE_EQUAL(employee->get_addresses()[1]->get_street(), U("Street 2"));
	VERIFY_ARE_EQUAL(employee->get_addresses()[1]->get_city(), U("City 2"));
	VERIFY_ARE_EQUAL(employee->get_addresses()[2]->get_street(), U("Street 3"));
	VERIFY_ARE_EQUAL(employee->get_addresses()[2]->get_city(), U("City 3"));
}

TEST(map_nullable_complex_to_entity_with_null_value)
{
	auto test_value = make_person_object();
	VERIFY_IS_NOT_NULL(test_value);
	auto service_context = std::make_shared<InMemoryEntities>(U("http://0.0.0.1"));

	auto person = std::make_shared<Person>(service_context);
	person->from_value(test_value);

	VERIFY_IS_NULL(person->get_address());
}

TEST(map_nullable_complex_to_entity_with_value)
{
	auto test_value = make_person_object();
	auto address = make_address_object();
	test_value->set_value(U("HomeAddress"), address);
	VERIFY_IS_NOT_NULL(test_value);
	auto service_context = std::make_shared<InMemoryEntities>(U("http://0.0.0.1"));

	auto person = std::make_shared<Person>(service_context);
	person->from_value(test_value);

	auto address_member = person->get_address();
	VERIFY_IS_NOT_NULL(address_member);
	VERIFY_ARE_EQUAL(address_member->get_city(), U("Shang Hai"));
	VERIFY_ARE_EQUAL(address_member->get_street(), U("Road 1111"));
}

TEST(map_binary_to_entity)
{
	auto test_value = make_product_object();
	VERIFY_IS_NOT_NULL(test_value);
	auto service_context = std::make_shared<InMemoryEntities>(U("http://0.0.0.1"));

	auto product = std::make_shared<Product>(service_context);
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
	auto service_context = std::make_shared<InMemoryEntities>(U("http://0.0.0.1"));

	auto product = std::make_shared<Product>(service_context);
	product->from_value(test_value);

	VERIFY_ARE_EQUAL(product->get_useraccess(), AccessLevel::ReadWrite);
}

TEST(map_collection_enum_to_entity)
{
	auto test_value = make_product_object();
	VERIFY_IS_NOT_NULL(test_value);
	auto service_context = std::make_shared<InMemoryEntities>(U("http://0.0.0.1"));

	auto product = std::make_shared<Product>(service_context);
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
	auto service_context = std::make_shared<InMemoryEntities>(U("http://0.0.0.1"));

	auto product = std::make_shared<Product>(service_context);
	product->from_value(test_value);

	VERIFY_IS_NULL(product->get_skincolor());
}

TEST(map_nullable_enum_to_entity_with_value)
{
	auto test_value = make_product_object();
	VERIFY_IS_NOT_NULL(test_value);
	auto model = get_test_model();
	test_value->set_value(U("SkinColor"), std::make_shared<odata_enum_value>(model->find_enum_type(U("Color")), U("Red")));
	auto service_context = std::make_shared<InMemoryEntities>(U("http://0.0.0.1"));

	auto product = std::make_shared<Product>(service_context);
	product->from_value(test_value);

	VERIFY_IS_NOT_NULL(product->get_skincolor());
	VERIFY_ARE_EQUAL(*(product->get_skincolor()), Color::Red);
}

TEST(map_enum_to_complex)
{
	auto test_value = make_address_object();
	VERIFY_IS_NOT_NULL(test_value);
	auto service_context = std::make_shared<InMemoryEntities>(U("http://0.0.0.1"));

	auto address = std::make_shared<Address>(service_context);
	address->from_value(test_value);

	VERIFY_ARE_EQUAL(address->get_color(), Color::Red);
}

TEST(map_collection_enum_to_complex)
{
	auto test_value = make_address_object();
	VERIFY_IS_NOT_NULL(test_value);
	auto service_context = std::make_shared<InMemoryEntities>(U("http://0.0.0.1"));

	auto address = std::make_shared<Address>(service_context);
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
	auto service_context = std::make_shared<InMemoryEntities>(U("http://0.0.0.1"));

	auto address = std::make_shared<Address>(service_context);
	address->from_value(test_value);

	VERIFY_IS_NULL(address->get_skincolor());
}

TEST(map_nullable_enum_to_complex_with_value)
{
	auto test_value = make_address_object();
	VERIFY_IS_NOT_NULL(test_value);
	auto model = get_test_model();
	test_value->set_value(U("SkinColor"), std::make_shared<odata_enum_value>(model->find_enum_type(U("Color")), U("Red")));
	auto service_context = std::make_shared<InMemoryEntities>(U("http://0.0.0.1"));

	auto address = std::make_shared<Address>(service_context);
	address->from_value(test_value);

	VERIFY_IS_NOT_NULL(address->get_skincolor());
	VERIFY_ARE_EQUAL(*(address->get_skincolor()), Color::Red);
}

TEST(map_navigation_to_entity)
{
	auto test_value = make_account_object_with_navigation();
	VERIFY_IS_NOT_NULL(test_value);
	auto service_context = std::make_shared<InMemoryEntities>(U("http://0.0.0.1"));

	auto account = std::make_shared<Account>(service_context);
	account->from_value(test_value);

	auto giftcard = account->get_giftcard();
	VERIFY_IS_NOT_NULL(giftcard);
	VERIFY_ARE_EQUAL(giftcard->get_edit_link(), U(""));
	VERIFY_ARE_EQUAL(giftcard->get_giftcardid(), 77566);
	VERIFY_ARE_EQUAL(giftcard->get_giftcardno(), U("@@2232"));
}

TEST(map_collection_navigation_to_entity)
{
	auto test_value = make_account_object_with_navigation();
	VERIFY_IS_NOT_NULL(test_value);
	auto service_context = std::make_shared<InMemoryEntities>(U("http://0.0.0.1"));

	auto account = std::make_shared<Account>(service_context);
	account->from_value(test_value);

	auto pis = account->get_mypaymentinstruments();
	VERIFY_ARE_EQUAL(pis.size(), 2);
	VERIFY_ARE_EQUAL(pis[0]->get_edit_link(), U(""));
	VERIFY_ARE_EQUAL(pis[0]->get_paymentinstrumentid(), 100000);
	VERIFY_ARE_EQUAL(pis[0]->get_friendlyname(), U("PI 1"));
}

TEST(map_derived_navigation_to_entity)
{
	auto test_value = make_account_object_with_navigation();
	VERIFY_IS_NOT_NULL(test_value);
	auto service_context = std::make_shared<InMemoryEntities>(U("http://0.0.0.1"));

	auto account = std::make_shared<Account>(service_context);
	account->from_value(test_value);

	auto pis = account->get_mypaymentinstruments();
	VERIFY_ARE_EQUAL(pis.size(), 2);

	auto credpi = std::dynamic_pointer_cast<CreditCardPI>(pis[1]);
	VERIFY_IS_NOT_NULL(credpi);
	VERIFY_ARE_EQUAL(credpi->get_edit_link(), U("http://0.0.0.1/PaymentInstrument(200000)"));
	VERIFY_ARE_EQUAL(credpi->get_paymentinstrumentid(), 200000);
	VERIFY_ARE_EQUAL(credpi->get_friendlyname(), U("PI 2"));
	VERIFY_ARE_EQUAL(credpi->get_cardnumber(), U("777777"));
}

class test_service_context : public InMemoryEntities
{
public:
	test_service_context(const ::utility::string_t& baseAddress, client_options options = client_options()) 
		: InMemoryEntities(baseAddress, options)
	{
		m_model = get_test_model();
	}

	~test_service_context()
	{
		std::wcout << U("destroy test_service_context") << std::endl;
	}
};

TEST(get_primitive_value_from_entity_class)
{
	auto service_context = std::make_shared<test_service_context>(U("http://0.0.0.1"));
	auto account = std::make_shared<Account>(service_context);
	account->set_accountid(1001212);

	auto entity_value = account->to_value();
	int32_t id;
	auto ok = entity_value->try_get(U("AccountID"), id);
	VERIFY_ARE_EQUAL(ok, true);
	VERIFY_ARE_EQUAL(id, 1001212);
}

TEST(get_collection_primitive_value_from_entity_class)
{
	auto service_context = std::make_shared<test_service_context>(U("http://0.0.0.1"));
	auto account = std::make_shared<Person>(service_context);
	std::vector<::utility::string_t> numbers;
	numbers.push_back(U("1"));
	numbers.push_back(U("2"));
	numbers.push_back(U("3"));
	numbers.push_back(U("4"));
	numbers.push_back(U("567890"));
	account->set_numbers(numbers);

	auto entity_value = account->to_value();
	std::shared_ptr<::odata::core::odata_value> value;
	auto ok = entity_value->get_property_value(U("Numbers"), value);
	auto collection_value = std::dynamic_pointer_cast<::odata::core::odata_collection_value>(value);
	VERIFY_IS_NOT_NULL(collection_value);
	VERIFY_ARE_EQUAL(collection_value->get_collection_values().size(), 5);
	auto v1 =  std::dynamic_pointer_cast<::odata::core::odata_primitive_value>(collection_value->get_collection_values()[0]);
	VERIFY_IS_NOT_NULL(v1);
	VERIFY_ARE_EQUAL(v1->to_string(), U("1"));
	auto v5 =  std::dynamic_pointer_cast<::odata::core::odata_primitive_value>(collection_value->get_collection_values()[4]);
	VERIFY_IS_NOT_NULL(v5);
	VERIFY_ARE_EQUAL(v5->to_string(), U("567890"));
}

TEST(get_nullable_primitive_value_from_entity_class)
{
	auto service_context = std::make_shared<test_service_context>(U("http://0.0.0.1"));
	auto account = std::make_shared<Person>(service_context);
	account->set_middlename(U("Middle Name"));

	auto entity_value = account->to_value();
	::utility::string_t middlename;
	auto ok = entity_value->try_get(U("MiddleName"), middlename);
	VERIFY_ARE_EQUAL(ok, true);
	VERIFY_ARE_EQUAL(middlename, U("Middle Name"));
}

TEST(get_primitive_value_from_complex_class)
{
	auto service_context = std::make_shared<test_service_context>(U("http://0.0.0.1"));
	auto address = std::make_shared<Address>(service_context);
	address->set_city(U("Shangh Hai"));

	auto entity_value = address->to_value();
	::utility::string_t city;
	auto ok = entity_value->try_get(U("City"), city);
	VERIFY_ARE_EQUAL(ok, true);
	VERIFY_ARE_EQUAL(city, U("Shangh Hai"));
}

TEST(get_collection_primitive_value_from_complex_class)
{
	auto service_context = std::make_shared<test_service_context>(U("http://0.0.0.1"));
	auto address = std::make_shared<Address>(service_context);
	std::vector<int32_t> numbers;
	numbers.push_back(100);
	numbers.push_back(2100);
	numbers.push_back(3100);
	numbers.push_back(5100);
	numbers.push_back(567890);
	address->set_numbers(numbers);

	auto entity_value = address->to_value();
	std::shared_ptr<::odata::core::odata_value> value;
	auto ok = entity_value->get_property_value(U("Numbers"), value);
	auto collection_value = std::dynamic_pointer_cast<::odata::core::odata_collection_value>(value);
	VERIFY_IS_NOT_NULL(collection_value);
	VERIFY_ARE_EQUAL(collection_value->get_collection_values().size(), 5);
	auto v1 =  std::dynamic_pointer_cast<::odata::core::odata_primitive_value>(collection_value->get_collection_values()[0]);
	VERIFY_IS_NOT_NULL(v1);
	VERIFY_ARE_EQUAL(v1->to_string(), U("100"));
	auto v5 =  std::dynamic_pointer_cast<::odata::core::odata_primitive_value>(collection_value->get_collection_values()[4]);
	VERIFY_IS_NOT_NULL(v5);
	VERIFY_ARE_EQUAL(v5->to_string(), U("567890"));
}

TEST(get_nullable_primitive_value_from_complex_class)
{
	auto service_context = std::make_shared<test_service_context>(U("http://0.0.0.1"));
	auto address = std::make_shared<Address>(service_context);
	address->set_code(-1231233);

	auto entity_value = address->to_value();
	int32_t code;
	auto ok = entity_value->try_get(U("Code"), code);
	VERIFY_ARE_EQUAL(ok, true);
	VERIFY_ARE_EQUAL(code, -1231233);
}

TEST(get_complex_value_from_entity_class)
{
	auto service_context = std::make_shared<test_service_context>(U("http://0.0.0.1"));
	auto account = std::make_shared<Account>(service_context);
	auto accountinfo = std::make_shared<AccountInfo>(service_context); 
	accountinfo->set_firstname(U("Leo"));
	accountinfo->set_lastname(U("Hu"));
	account->set_accountinfo(accountinfo);

	auto entity_value = account->to_value();
	std::shared_ptr<::odata::core::odata_value> value;
	auto ok = entity_value->get_property_value(U("AccountInfo"), value);
	auto complex_value = std::dynamic_pointer_cast<::odata::core::odata_complex_value>(value);
	VERIFY_IS_NOT_NULL(complex_value);
	::utility::string_t firstname, lastname;
	complex_value->try_get(U("FirstName"), firstname);
	complex_value->try_get(U("LastName"), lastname);
	VERIFY_ARE_EQUAL(firstname, U("Leo"));
	VERIFY_ARE_EQUAL(lastname, U("Hu"));
}

TEST(get_derived_complex_value_from_entity_class)
{
	auto service_context = std::make_shared<test_service_context>(U("http://0.0.0.1"));
	auto account = std::make_shared<Account>(service_context);
	auto  vipinfo = std::make_shared<VipAccountInfo>(service_context);
	vipinfo->set_firstname(U("Leo"));
	vipinfo->set_lastname(U("Hu"));
	vipinfo->set_vipinfo(U("Gold"));
	account->set_vipinfo(vipinfo);

	auto entity_value = account->to_value();
	std::shared_ptr<::odata::core::odata_value> value;
	auto ok = entity_value->get_property_value(U("VIPAccountInfo"), value);
	auto complex_value = std::dynamic_pointer_cast<::odata::core::odata_complex_value>(value);
	VERIFY_IS_NOT_NULL(complex_value);
	::utility::string_t firstname, lastname, vip;
	complex_value->try_get(U("FirstName"), firstname);
	complex_value->try_get(U("LastName"), lastname);
	complex_value->try_get(U("VIP_Info"), vip);
	VERIFY_ARE_EQUAL(firstname, U("Leo"));
	VERIFY_ARE_EQUAL(lastname, U("Hu"));
	VERIFY_ARE_EQUAL(vip, U("Gold"));
}

TEST(get_collection_complex_value_from_entity_class)
{
	auto service_context = std::make_shared<test_service_context>(U("http://0.0.0.1"));
	auto employee = std::make_shared<Employee>(service_context);
  	std::vector<std::shared_ptr<Address>> addresses;
	auto address_1 = std::make_shared<Address>(service_context);
	address_1->set_street(U("zixinglu"));
	addresses.push_back(address_1);
	auto address_2 = std::make_shared<Address>(service_context);
	address_2->set_street(U("dongchuanlu"));
	addresses.push_back(address_2);
	employee->set_addresses(addresses); 

	auto entity_value = employee->to_value();
	std::shared_ptr<::odata::core::odata_value> value;
	auto ok = entity_value->get_property_value(U("WorkAddresses"), value);
	auto collection_value = std::dynamic_pointer_cast<::odata::core::odata_collection_value>(value);
	VERIFY_IS_NOT_NULL(collection_value);
	VERIFY_ARE_EQUAL(collection_value->get_collection_values().size(), 2);
	::utility::string_t street;
	auto c1 =  std::dynamic_pointer_cast<::odata::core::odata_complex_value>(collection_value->get_collection_values()[0]);
	VERIFY_IS_NOT_NULL(c1);
	c1->try_get(U("Street"), street);
	VERIFY_ARE_EQUAL(street, U("zixinglu"));
	auto c2 =  std::dynamic_pointer_cast<::odata::core::odata_complex_value>(collection_value->get_collection_values()[1]);
	VERIFY_IS_NOT_NULL(c2);
	c2->try_get(U("Street"), street);
	VERIFY_ARE_EQUAL(street, U("dongchuanlu"));
}

TEST(get_nullable_complex_value_from_entity_class)
{
	auto service_context = std::make_shared<test_service_context>(U("http://0.0.0.1"));
	auto person = std::make_shared<Person>(service_context);
  	std::vector<std::shared_ptr<Address>> addresses;
	auto address = std::make_shared<Address>(service_context);
	address->set_street(U("zixinglu 999"));
	person->set_address(address);

	auto entity_value = person->to_value();
	std::shared_ptr<::odata::core::odata_value> value;
	auto ok = entity_value->get_property_value(U("HomeAddress"), value);
	auto complex_value = std::dynamic_pointer_cast<::odata::core::odata_complex_value>(value);
	VERIFY_IS_NOT_NULL(complex_value);
	::utility::string_t street;
	complex_value->try_get(U("Street"), street);
	VERIFY_ARE_EQUAL(street, U("zixinglu 999"));
}

TEST(get_enum_value_from_entity_class)
{
	auto service_context = std::make_shared<test_service_context>(U("http://0.0.0.1"));
	auto product = std::make_shared<Product>(service_context);
	product->set_useraccess(AccessLevel::Write);

	auto entity_value = product->to_value();
	std::shared_ptr<::odata::core::odata_value> value;
	auto ok = entity_value->get_property_value(U("UserAccess"), value);
	auto enum_value = std::dynamic_pointer_cast<::odata::core::odata_enum_value>(value);
	VERIFY_IS_NOT_NULL(enum_value);
	VERIFY_ARE_EQUAL(enum_value->to_string(), U("Write"));
}

TEST(get_collection_enum_value_from_entity_class)
{
	auto service_context = std::make_shared<test_service_context>(U("http://0.0.0.1"));
	auto product = std::make_shared<Product>(service_context);
	std::vector<Color> colors;
	colors.push_back(Color::Red);
	colors.push_back(Color::Blue);
	product->set_covercolors(colors);

	auto entity_value = product->to_value();
	std::shared_ptr<::odata::core::odata_value> value;
	auto ok = entity_value->get_property_value(U("CoverColors"), value);
	auto collection_value = std::dynamic_pointer_cast<::odata::core::odata_collection_value>(value);
	VERIFY_IS_NOT_NULL(collection_value);
	VERIFY_ARE_EQUAL(collection_value->get_collection_values().size(), 2);
	auto e1 =  std::dynamic_pointer_cast<::odata::core::odata_enum_value>(collection_value->get_collection_values()[0]);
	VERIFY_ARE_EQUAL(e1->get_value_type()->get_type_kind(), edm_type_kind_t::Enum);
	VERIFY_ARE_EQUAL(e1->to_string(), U("Red"));
	auto e2 =  std::dynamic_pointer_cast<::odata::core::odata_enum_value>(collection_value->get_collection_values()[1]);
	VERIFY_ARE_EQUAL(e2->to_string(), U("Blue"));
}

TEST(get_nullable_enum_value_from_entity_class)
{
	auto service_context = std::make_shared<test_service_context>(U("http://0.0.0.1"));
	auto product = std::make_shared<Product>(service_context);
	product->set_skincolor(Color::Green);

	auto entity_value = product->to_value();
	std::shared_ptr<::odata::core::odata_value> value;
	auto ok = entity_value->get_property_value(U("SkinColor"), value);
	auto enum_value = std::dynamic_pointer_cast<::odata::core::odata_enum_value>(value);
	VERIFY_IS_NOT_NULL(enum_value);
	VERIFY_ARE_EQUAL(enum_value->to_string(), U("Green"));
}

TEST(get_enum_value_from_complex_class)
{
	auto service_context = std::make_shared<test_service_context>(U("http://0.0.0.1"));
	auto address = std::make_shared<Address>(service_context);
	address->set_color(Color::Red);

	auto entity_value = address->to_value();
	std::shared_ptr<::odata::core::odata_value> value;
	auto ok = entity_value->get_property_value(U("Color"), value);
	auto enum_value = std::dynamic_pointer_cast<::odata::core::odata_enum_value>(value);
	VERIFY_IS_NOT_NULL(enum_value);
	VERIFY_ARE_EQUAL(enum_value->to_string(), U("Red"));
}

TEST(get_collection_enum_value_from_complex_class)
{
	auto service_context = std::make_shared<test_service_context>(U("http://0.0.0.1"));
	auto address = std::make_shared<Address>(service_context);
	std::vector<Color> colors;
	colors.push_back(Color::Red);
	colors.push_back(Color::Blue);
	address->set_covercolors(colors);

	auto entity_value = address->to_value();
	std::shared_ptr<::odata::core::odata_value> value;
	auto ok = entity_value->get_property_value(U("CoverColors"), value);
	auto collection_value = std::dynamic_pointer_cast<::odata::core::odata_collection_value>(value);
	VERIFY_IS_NOT_NULL(collection_value);
	VERIFY_ARE_EQUAL(collection_value->get_collection_values().size(), 2);
	auto e1 =  std::dynamic_pointer_cast<::odata::core::odata_enum_value>(collection_value->get_collection_values()[0]);
	VERIFY_ARE_EQUAL(e1->to_string(), U("Red"));
	auto e2 =  std::dynamic_pointer_cast<::odata::core::odata_enum_value>(collection_value->get_collection_values()[1]);
	VERIFY_ARE_EQUAL(e2->to_string(), U("Blue"));
}

TEST(get_nullable_enum_value_from_complex_class)
{
	auto service_context = std::make_shared<test_service_context>(U("http://0.0.0.1"));
	auto address = std::make_shared<Address>(service_context);
	address->set_skincolor(Color::Green);

	auto entity_value = address->to_value();
	std::shared_ptr<::odata::core::odata_value> value;
	auto ok = entity_value->get_property_value(U("SkinColor"), value);
	auto enum_value = std::dynamic_pointer_cast<::odata::core::odata_enum_value>(value);
	VERIFY_IS_NOT_NULL(enum_value);
	VERIFY_ARE_EQUAL(enum_value->to_string(), U("Green"));
}

TEST(get_key_string_with_one_key)
{
    auto service_context = std::make_shared<test_service_context>(U("http://0.0.0.1"));
	auto product = std::make_shared<Product>(service_context);
	product->set_productid(10001);

	auto key_string = product->get_key_property_string();
	VERIFY_ARE_EQUAL(key_string, U("10001"));
}

TEST(get_key_string_with_double_key)
{
    auto service_context = std::make_shared<test_service_context>(U("http://0.0.0.1"));
	auto productdetail = std::make_shared<ProductDetail>(service_context);
	productdetail->set_productid(10001);
	productdetail->set_productdetailedid(20002);

	auto key_string = productdetail->get_key_property_string();
	VERIFY_ARE_EQUAL(key_string, U("ProductID=10001,ProductDetailID=20002"));
}

TEST(get_key_string_with_one_key_from_base_class)
{
    auto service_context = std::make_shared<test_service_context>(U("http://0.0.0.1"));
	auto employee = std::make_shared<Employee>(service_context);
	employee->set_personid(10101);

	auto key_string = employee->get_key_property_string();
	VERIFY_ARE_EQUAL(key_string, U("10101"));
}

TEST(get_double_key_string_with_one_key_from_base_class)
{
    auto service_context = std::make_shared<test_service_context>(U("http://0.0.0.1"));
	auto customer = std::make_shared<Customer>(service_context);
	customer->set_personid(10101);
	customer->set_city(U("Shang Hai"));

	auto key_string = customer->get_key_property_string();
	VERIFY_ARE_EQUAL(key_string, U("PersonID=10101,City=Shang Hai"));
}

TEST(get_fourfold_key_string_with_two_key_from_base_class)
{
	auto service_context = std::make_shared<test_service_context>(U("http://0.0.0.1"));
	auto vip = std::make_shared<VipCustomer>(service_context);
	vip->set_personid(10101);
	vip->set_city(U("Shang Hai"));
	vip->set_vipid(70000);
	vip->set_viptype(U("Gold"));

	auto key_string = vip->get_key_property_string();
	VERIFY_ARE_EQUAL(key_string, U("PersonID=10101,City=Shang HaiVIPID=70000,VIPType=Gold"));
}

TEST (change_entity_mapping_array)
{
	auto service_context = std::make_shared<test_service_context>(U("http://0.0.0.1")); 
	auto vip = std::make_shared<VipCustomer>(service_context);
	vip->set_personid(10101);
	vip->set_city(U("Shang Hai"));
	vip->set_vipid(70000);
	vip->set_viptype(U("Gold"));

	auto vip2 = std::make_shared<VipCustomer>(service_context);
	vip2->set_personid(10102);
	vip2->set_city(U("Shen Yang"));
	vip2->set_vipid(90000);
	vip2->set_viptype(U("White Gold"));

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
	::utility::string_t vip_type;
	entity_value->try_get(U("VIPType"), vip_type);
	VERIFY_ARE_EQUAL(vip_type, U("Gold"));

	(std::dynamic_pointer_cast<Customer>(vip))->set_to_entity_map(customer_map);
	entity_value = vip->to_value();
	VERIFY_ARE_EQUAL(entity_value->properties().size(), 4);
	VERIFY_ARE_EQUAL(entity_value->has_property(U("PersonID")), false);

	(std::dynamic_pointer_cast<Person>(vip))->set_to_entity_map(person_map);
	entity_value = vip->to_value();
	VERIFY_ARE_EQUAL(entity_value->properties().size(), 8);
	int32_t persion_id;
	entity_value->try_get(U("PersonID"), persion_id);
	VERIFY_ARE_EQUAL(persion_id, 10101);

	auto entity_value_2 = vip2->to_value();
	VERIFY_ARE_EQUAL(entity_value_2->properties().size(), 8);
	entity_value_2->try_get(U("VIPType"), vip_type);
	VERIFY_ARE_EQUAL(vip_type, U("White Gold"));
}

}

}}}