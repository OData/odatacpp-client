#pragma once
#include "odata/codegen/code_generation.h"
#include "odata/codegen/odata_service_context.h"
#include "odata/codegen/odata_service_query.h"
#include "odata/codegen/odata_entityset_query_executor.h"
#include "odata/codegen/odata_singleton_query_executor.h"
#include "odata/codegen/odata_primitive_query_executor.h"
#include "odata/codegen/odata_complex_query_executor.h"
#include "odata/codegen/odata_query_builder.h"
#include "cpprest/json.h"

using namespace std;
using namespace ::odata::client;
using namespace ::odata::core;
using namespace ::odata::edm;
using namespace ::odata::codegen;

namespace tests { namespace functional { namespace _odata { 

enum AccessLevel
{
	None = 0,
	Read = 1,
	Write = 2,
	Execute = 4,
	ReadWrite = 3,
};


enum Color
{
	Red = 1,
	Green = 2,
	Blue = 4,
};

class enum_type_resolver
{
public:
    DECLARE_FUNCTION_ENUM_TYPE_FROM_STRING(AccessLevel);
    DECLARE_FUNCTION_STRING_FROM_ENUM_TYPE(AccessLevel);
    DECLARE_FUNCTION_ENUM_TYPE_FROM_STRING(Color);
    DECLARE_FUNCTION_STRING_FROM_ENUM_TYPE(Color);
};

class Product : public type_base
{
public:
	DECLARE_ENTITY_CONSTRUCTOR(Product);
	DECLARE_ENTITY_DESTRUCTOR(Product);
	DECLARE_EDM_INFO();
	ENABLE_PROPERTY_IN_ENTITY_MAPPING();

	DECLARE_GET_KEY_PROPERTY_STRING_ONE_PARAM(type_base, ProductID, productid);

	DECLARE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(productid, ProductID, int32_t);
	// enum 
	DECLARE_ENUM_PROPERTY_IN_ENTITY_MAPPING(useraccess, UserAccess, AccessLevel);
	// collection of enum 
	DECLARE_COLLECTION_ENUM_PROPERTY_IN_ENTITY_MAPPING(covercolors, CoverColors, Color);
	// nullable enum
	DECLARE_NULLABLE_ENUM_PROPERTY_IN_ENTITY_MAPPING(skincolor, SkinColor, Color);

	DECLARE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(pic, Picture, std::vector<unsigned char>);
	DECLARE_NULLABLE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(nullpic, NullPicture, std::vector<unsigned char>);
};


class AccountInfo : public type_base
{
public:
	DECLARE_COMPLEX_CONSTRUCTOR(AccountInfo);
	DECLARE_EDM_INFO();
	ENABLE_PROPERTY_IN_COMPLEX_MAPPING();

	DECLARE_PRIMITIVE_PROPERTY_IN_COMPLEX_MAPPING(firstname, FirstName, ::utility::string_t);
	DECLARE_PRIMITIVE_PROPERTY_IN_COMPLEX_MAPPING(lastname, LastName, ::utility::string_t);

	// todo add support for enum in complex type
};

class VipAccountInfo : public AccountInfo
{
public:
	DECLARE_COMPLEX_CONSTRUCTOR(VipAccountInfo);
	DECLARE_EDM_INFO();
	ENABLE_PROPERTY_IN_COMPLEX_MAPPING();

	DECLARE_PRIMITIVE_PROPERTY_IN_COMPLEX_MAPPING(vipinfo, VIP_Info, ::utility::string_t);
};

class GiftCard : public type_base
{
public:
	DECLARE_ENTITY_CONSTRUCTOR(GiftCard);
	DECLARE_EDM_INFO();
	ENABLE_PROPERTY_IN_ENTITY_MAPPING();

	DECLARE_GET_KEY_PROPERTY_STRING_ONE_PARAM(type_base, GiftCardID, giftcardid);

	DECLARE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(giftcardid, GiftCardID, int32_t);
	DECLARE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(giftcardno, GiftCardNO, ::utility::string_t);
	DECLARE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(amount, Amount, double);
	DECLARE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(experationdate, ExperationDate, ::utility::datetime);
};

class PaymentInstrument : public type_base
{
public:
	DECLARE_ENTITY_CONSTRUCTOR(PaymentInstrument);
	DECLARE_EDM_INFO();
	ENABLE_PROPERTY_IN_ENTITY_MAPPING();

	DECLARE_GET_KEY_PROPERTY_STRING_ONE_PARAM(type_base, PaymentInstrumentID, paymentinstrumentid);

	DECLARE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(paymentinstrumentid, PaymentInstrumentID, int32_t);
	DECLARE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(friendlyname, FriendlyName, ::utility::string_t);
	DECLARE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(createdate, CreatedDate, ::utility::datetime);
};

class CreditCardPI : public PaymentInstrument
{
public:
	DECLARE_ENTITY_CONSTRUCTOR(CreditCardPI);
	DECLARE_EDM_INFO();
	ENABLE_PROPERTY_IN_ENTITY_MAPPING();

	DECLARE_GET_KEY_PROPERTY_STRING_NO_PARAM(PaymentInstrument);

	DECLARE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(cardnumber, CardNumber, ::utility::string_t);
	DECLARE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(cvv, CVV, ::utility::string_t);

	// todo support navigation in navigation
};

class Account : public type_base
{
public:
	DECLARE_ENTITY_CONSTRUCTOR(Account);
	DECLARE_EDM_INFO();
    ENABLE_PROPERTY_IN_ENTITY_MAPPING();

	DECLARE_GET_KEY_PROPERTY_STRING_ONE_PARAM(type_base, AccountID, accountid);

	// primitive property
    DECLARE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(accountid, AccountID, int32_t);
	DECLARE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(country, Country, ::utility::string_t);
	// complex property
	DECLARE_COMPLEX_PROPERTY_IN_ENTITY_MAPPING(accountinfo, AccountInfo, AccountInfo);
	DECLARE_COMPLEX_PROPERTY_IN_ENTITY_MAPPING(vipinfo, VIPAccountInfo, VipAccountInfo);
	// navigation property
	DECLARE_NAVIGATION_PROPERTY_IN_ENTITY_MAPPING(giftcard, MyGiftCard, GiftCard);
	// collection of navigation property
	DECLARE_COLLECTION_NAVIGATION_PROPERTY_IN_ENTITY_MAPPING(mypaymentinstruments, MyPaymentInstruments, PaymentInstrument);

	// todo add support for collection of complex 
};

class Address : public type_base
{
public:
	DECLARE_COMPLEX_CONSTRUCTOR(Address);
	DECLARE_COMPLEX_DESTRUCTOR(Address);
	DECLARE_EDM_INFO();
	ENABLE_PROPERTY_IN_COMPLEX_MAPPING();
	DECLARE_PRIMITIVE_PROPERTY_IN_COMPLEX_MAPPING(street, Street, ::utility::string_t);
	DECLARE_PRIMITIVE_PROPERTY_IN_COMPLEX_MAPPING(city, City, ::utility::string_t);
	DECLARE_PRIMITIVE_PROPERTY_IN_COMPLEX_MAPPING(postalcode, PostalCode, ::utility::string_t);
	DECLARE_COLLECTION_PRIMITIVE_PROPERTY_IN_COMPLEX_MAPPING(numbers, Numbers, int32_t);
	DECLARE_NULLABLE_PRIMITIVE_PROPERTY_IN_COMPLEX_MAPPING(code, Code, int32_t);

	// enum 
	DECLARE_ENUM_PROPERTY_IN_COMPLEX_MAPPING(color, Color, Color);
	// collection of enum 
	DECLARE_COLLECTION_ENUM_PROPERTY_IN_COMPLEX_MAPPING(covercolors, CoverColors, Color);
	// nullable enum
	DECLARE_NULLABLE_ENUM_PROPERTY_IN_COMPLEX_MAPPING(skincolor, SkinColor, Color);
};

class Person : public type_base
{
public:
	DECLARE_ENTITY_CONSTRUCTOR(Person);
	DECLARE_ENTITY_DESTRUCTOR(Person);
	DECLARE_EDM_INFO();
	ENABLE_PROPERTY_IN_ENTITY_MAPPING();

	DECLARE_GET_KEY_PROPERTY_STRING_ONE_PARAM(type_base, PersonID, personid);

    DECLARE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(personid, PersonID, int32_t);
	DECLARE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(firstname, FirstName, ::utility::string_t);
	DECLARE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(lastname, LastName, ::utility::string_t);

	// nullable property
	DECLARE_NULLABLE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(middlename, MiddleName, ::utility::string_t);
	DECLARE_NULLABLE_COMPLEX_PROPERTY_IN_ENTITY_MAPPING(address, HomeAddress, Address);

	// collection of primitive
	DECLARE_COLLECTION_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(numbers, Numbers, ::utility::string_t);
	DECLARE_COLLECTION_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(emails, Emails, ::utility::string_t);
};

class Employee : public Person
{
public:
	DECLARE_ENTITY_CONSTRUCTOR(Employee);
	DECLARE_EDM_INFO();
	ENABLE_PROPERTY_IN_ENTITY_MAPPING();

	DECLARE_GET_KEY_PROPERTY_STRING_NO_PARAM(Person)

	DECLARE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(datehired, DateHired, ::utility::datetime);
	DECLARE_COLLECTION_COMPLEX_PROPERTY_IN_ENTITY_MAPPING(addresses, WorkAddresses, Address);
};

class Customer : public Person
{
public:
	DECLARE_ENTITY_CONSTRUCTOR(Customer);
	DECLARE_EDM_INFO();
	ENABLE_PROPERTY_IN_ENTITY_MAPPING();

	DECLARE_GET_KEY_PROPERTY_STRING_ONE_PARAM(Person, City, city)

	DECLARE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(city, City, ::utility::string_t);
	DECLARE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(birthday, Birthday, ::utility::datetime);
};

class VipCustomer : public Customer
{
public:
	DECLARE_ENTITY_CONSTRUCTOR(VipCustomer);
	DECLARE_EDM_INFO();
	ENABLE_PROPERTY_IN_ENTITY_MAPPING();

	DECLARE_GET_KEY_PROPERTY_STRING_TWO_PARAM(Customer, VIPID, vipid, VIPType, viptype);

    DECLARE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(vipid, VIPID, int32_t);
	DECLARE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(viptype, VIPType, ::utility::string_t);
};

class ProductDetail : public type_base
{
public:
	DECLARE_ENTITY_CONSTRUCTOR(ProductDetail);
	DECLARE_EDM_INFO();
	ENABLE_PROPERTY_IN_ENTITY_MAPPING();

	DECLARE_GET_KEY_PROPERTY_STRING_TWO_PARAM(type_base,  ProductID, productid, ProductDetailID, productdetailedid);

    DECLARE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(productid, ProductID, int32_t);
    DECLARE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(productdetailedid, ProductDetailID, int32_t);

	DECLARE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(productname, ProductName, ::utility::string_t);
	DECLARE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(description, Description, ::utility::string_t);
};

class InMemoryEntities : public odata_service_context
{
public:
	InMemoryEntities(const ::utility::string_t& baseAddress, client_options options = client_options()) : odata_service_context(baseAddress, options)
	{
	}

	std::shared_ptr<odata_service_query<odata_entityset_query_executor<Product>, odata_query_builder>> create_products_query() 
	{ 
		return create_query<odata_entityset_query_executor<Product>, odata_query_builder>(U("Products"));
	}

	std::shared_ptr<odata_service_query<odata_entityset_query_executor<Person>, odata_query_builder>> create_people_query() 
	{ 
		return create_query<odata_entityset_query_executor<Person>, odata_query_builder>(U("People"));
	}

	std::shared_ptr<odata_service_query<odata_entityset_query_executor<Account>, odata_query_builder>> create_accounts_query() 
	{ 
		return create_query<odata_entityset_query_executor<Account>, odata_query_builder>(U("Accounts"));
	}

	::web::http::status_code add_account_object(std::shared_ptr<Account> account_object)
	{
		return m_client->create_entity(U("Accounts"), std::dynamic_pointer_cast<odata_entity_value>(account_object->to_value())).get();
	}
};

}}}
