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
 
 #include "codegen_test_service.h"
#include "odata/core/odata_json_writer_minimal.h"

namespace tests { namespace functional { namespace _odata { 

IMPLEMENT_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(Product, productid, ProductID, int32_t);
IMPLEMENT_ENUM_PROPERTY_IN_ENTITY_MAPPING(Product, useraccess, UserAccess, AccessLevel);
IMPLEMENT_COLLECTION_ENUM_PROPERTY_IN_ENTITY_MAPPING(Product, covercolors, CoverColors, Color);
IMPLEMENT_NULLABLE_ENUM_PROPERTY_IN_ENTITY_MAPPING(Product, skincolor, SkinColor, Color);
IMPLEMENT_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(Product, pic, Picture, std::vector<unsigned char>);
IMPLEMENT_NULLABLE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(Product, nullpic, NullPicture, std::vector<unsigned char>);


IMPLEMENT_PRIMITIVE_PROPERTY_IN_COMPLEX_MAPPING(AccountInfo, firstname, FirstName, ::utility::string_t);
IMPLEMENT_PRIMITIVE_PROPERTY_IN_COMPLEX_MAPPING(AccountInfo, lastname, LastName, ::utility::string_t);

IMPLEMENT_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(GiftCard, giftcardid, GiftCardID, int32_t);
IMPLEMENT_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(GiftCard, giftcardno, GiftCardNO, ::utility::string_t);
IMPLEMENT_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(GiftCard, amount, Amount, double);
IMPLEMENT_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(GiftCard, experationdate, ExperationDate, ::utility::datetime);

IMPLEMENT_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(PaymentInstrument, paymentinstrumentid, PaymentInstrumentID, int32_t);
IMPLEMENT_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(PaymentInstrument, friendlyname, FriendlyName, ::utility::string_t);
IMPLEMENT_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(PaymentInstrument, createdate, CreatedDate, ::utility::datetime);

IMPLEMENT_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(CreditCardPI, cardnumber, CardNumber, ::utility::string_t);
IMPLEMENT_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(CreditCardPI, cvv, CVV, ::utility::string_t);

IMPLEMENT_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(Account, accountid, AccountID, int32_t);
IMPLEMENT_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(Account, country, Country, ::utility::string_t);
IMPLEMENT_COMPLEX_PROPERTY_IN_ENTITY_MAPPING(Account, accountinfo, AccountInfo, AccountInfo);
IMPLEMENT_COMPLEX_PROPERTY_IN_ENTITY_MAPPING(Account, vipinfo, VIPAccountInfo, VipAccountInfo);
IMPLEMENT_NAVIGATION_PROPERTY_IN_ENTITY_MAPPING(Account, giftcard, MyGiftCard, GiftCard);
IMPLEMENT_COLLECTION_NAVIGATION_PROPERTY_IN_ENTITY_MAPPING(Account, mypaymentinstruments, MyPaymentInstruments, PaymentInstrument);
/*
void Account::get_mypaymentinstruments_from_entity(const shared_ptr<::odata::core::odata_entity_value>& pentity)
{
    std::shared_ptr<::odata::core::odata_value> property_value;
    
    if (!pentity->get_property_value(L"MyPaymentInstruments", property_value) || !property_value)
    { return ; }
    
    auto property_collection_value = static_pointer_cast<::odata::core::odata_collection_value>(property_value);
    if (!property_collection_value) { return ; }
    
    for (auto iter = property_collection_value->get_collection_values().cbegin(); iter != property_collection_value->get_collection_values().cend(); iter++)
    {
        auto entity_value = std::dynamic_pointer_cast<::odata::core::odata_entity_value>(*iter);
        if (!entity_value) { continue ; }
        ::utility::string_t real_type_name = entity_value->get_value_type()->get_name();
        ::utility::string_t expected_type_name = PaymentInstrument::get_type_name();
        if (expected_type_name != real_type_name)
        {
            auto create_pfn = PaymentInstrument::_derived_entity_creator_map[real_type_name];
            if (!create_pfn) return ;
            
            auto p_derived = (*create_pfn)(m_service_context);
            if (!p_derived) return ;
            p_derived->set_edit_link(odata_entity_model_builder::compute_edit_link(get_root_url(), entity_value, m_edit_link + U("/MyPaymentInstruments"), true));
            p_derived->from_value(entity_value); mypaymentinstruments.push_back(std::dynamic_pointer_cast<PaymentInstrument>(p_derived));
        }
        else
        {
            auto collection_element = std::make_shared<PaymentInstrument>(m_service_context);
            collection_element->set_edit_link(odata_entity_model_builder::compute_edit_link(get_root_url(), entity_value, m_edit_link + U("/") + U("MyPaymentInstruments"), true));
            collection_element->from_value(entity_value);
            mypaymentinstruments.push_back(collection_element);
        }
    }
}
::pplx::task<void> Account::load_mypaymentinstruments(const std::shared_ptr<odata_query_builder>& builder)
{
    if (m_service_context)
    {
        ::utility::string_t path = m_service_context->get_relative_path(m_edit_link) + L"/" + L"MyPaymentInstruments";
        ::utility::string_t edit_link = m_edit_link + L"/" + L"MyPaymentInstruments";
        auto query = m_service_context->create_query<odata_entityset_query_executor<PaymentInstrument>, odata_query_builder>(path);
        if (builder) { query->set_query_builder(builder); }
        return query->execute_query().then( [this, edit_link] (const std::vector<std::shared_ptr<PaymentInstrument>>& ret_values) -> void
        {
            for (auto iter = ret_values.cbegin(); iter != ret_values.cend(); iter++)
            {
                ::utility::string_t edit_link_placeholder = L"{edit_link}";
                if ((*iter)->get_edit_link().empty() || (*iter)->get_edit_link().substr(0, edit_link_placeholder.size()) == edit_link_placeholder)
                {
                    if ((*iter)->get_edit_link().empty()) { (*iter)->set_edit_link(edit_link + L"(" + (*iter)->get_key_property_string() + L")");
                        (*iter)->set_edit_link(edit_link + L"(" + (*iter)->get_key_property_string() + L")");
                    }
                    else
                    {
                        (*iter)->set_edit_link(edit_link + L"(" + (*iter)->get_key_property_string() + L")" + (*iter)->get_edit_link().substr(edit_link_placeholder.size())); } } } mypaymentinstruments = ret_values;
        });
    }
    return ::pplx::task_from_result();
};
*/
    
    
IMPLEMENT_PRIMITIVE_PROPERTY_IN_COMPLEX_MAPPING(Address, street, Street, ::utility::string_t);
IMPLEMENT_PRIMITIVE_PROPERTY_IN_COMPLEX_MAPPING(Address, city, City, ::utility::string_t);
IMPLEMENT_PRIMITIVE_PROPERTY_IN_COMPLEX_MAPPING(Address, postalcode, PostalCode, ::utility::string_t);
IMPLEMENT_COLLECTION_PRIMITIVE_PROPERTY_IN_COMPLEX_MAPPING(Address, numbers, Numbers, int32_t);
IMPLEMENT_NULLABLE_PRIMITIVE_PROPERTY_IN_COMPLEX_MAPPING(Address, code, Code, int32_t);
IMPLEMENT_ENUM_PROPERTY_IN_COMPLEX_MAPPING(Address, color, Color, Color);
IMPLEMENT_COLLECTION_ENUM_PROPERTY_IN_COMPLEX_MAPPING(Address, covercolors, CoverColors, Color);
IMPLEMENT_NULLABLE_ENUM_PROPERTY_IN_COMPLEX_MAPPING(Address, skincolor, SkinColor, Color);

IMPLEMENT_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(Person, personid, PersonID, int32_t);
IMPLEMENT_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(Person, firstname, FirstName, ::utility::string_t);
IMPLEMENT_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(Person, lastname, LastName, ::utility::string_t);
IMPLEMENT_NULLABLE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(Person, middlename, MiddleName, ::utility::string_t);
IMPLEMENT_NULLABLE_COMPLEX_PROPERTY_IN_ENTITY_MAPPING(Person, address, HomeAddress, Address);
IMPLEMENT_COLLECTION_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(Person, numbers, Numbers, ::utility::string_t);
IMPLEMENT_COLLECTION_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(Person, emails, Emails, ::utility::string_t);

IMPLEMENT_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(Employee, datehired, DateHired, ::utility::datetime);
IMPLEMENT_COLLECTION_COMPLEX_PROPERTY_IN_ENTITY_MAPPING(Employee, addresses, WorkAddresses, Address);

IMPLEMENT_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(Customer, city, City, ::utility::string_t);
IMPLEMENT_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(Customer, birthday, Birthday, ::utility::datetime);

IMPLEMENT_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(ProductDetail, productid, ProductID, int32_t);
IMPLEMENT_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(ProductDetail, productdetailedid, ProductDetailID, int32_t);
IMPLEMENT_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(ProductDetail, productname, ProductName, ::utility::string_t);
IMPLEMENT_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(ProductDetail, description, Description, ::utility::string_t);

IMPLEMENT_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(VipCustomer, vipid, VIPID, int32_t);
IMPLEMENT_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(VipCustomer, viptype, VIPType, ::utility::string_t);

IMPLEMENT_PRIMITIVE_PROPERTY_IN_COMPLEX_MAPPING(VipAccountInfo, vipinfo, VIP_Info, ::utility::string_t);
 //   void VipAccountInfo::get_vipinfo_from_complex(const shared_ptr<::odata::core::odata_complex_value>& pcomplex) 
	//{ 
	//    std::shared_ptr<::odata::core::odata_value> property_value; 
 //       if (!pcomplex->get_property_value(U("VIP_Info"), property_value) || !property_value) 
	//    { 
	//		return ; 
	//	} 
	//	auto primitive_value = std::dynamic_pointer_cast<odata_primitive_value>(property_value); 
	//    if (primitive_value) 
	//    { 
 //           vipinfo = primitive_value->as<::utility::string_t>(); 
 //       } 
 //   }
	//void VipAccountInfo::set_vipinfo_to_complex(const shared_ptr<::odata::core::odata_complex_value>& pcomplex) 
	//{ 
	//}

BEGIN_PROPERTY_IN_COMPLEX_MAPPING(AccountInfo)
    ON_PROPERTY_IN_COMPLEX_MAPPING(AccountInfo, firstname)
    ON_PROPERTY_IN_COMPLEX_MAPPING(AccountInfo, lastname)
END_PROPERTY_IN_COMPLEX_MAPPING(AccountInfo)

BEGIN_COMPLEX_CONSTRUCTOR(AccountInfo, type_base)
END_COMPLEX_CONSTRUCTOR(AccountInfo, type_base)

BEGIN_SERIALIZE_PROPERTY_IN_COMPLEX_MAPPING(AccountInfo)
	ON_SERIALIZE_PROPERTY_IN_COMPLEX_MAPPING(AccountInfo, firstname)
	ON_SERIALIZE_PROPERTY_IN_COMPLEX_MAPPING(AccountInfo, lastname)
END_SERIALIZE_PROPERTY_IN_COMPLEX_MAPPING(AccountInfo)

IMPLEMENT_EDM_INFO(AccountInfo, Microsoft.Test.OData.Services.ODataWCFService, AccountInfo)

BEGIN_PROPERTY_IN_ENTITY_MAPPING(GiftCard)
    ON_PROPERTY_IN_ENTITY_MAPPING(GiftCard, giftcardid)
    ON_PROPERTY_IN_ENTITY_MAPPING(GiftCard, giftcardno)
	ON_PROPERTY_IN_ENTITY_MAPPING(GiftCard, amount)
	ON_PROPERTY_IN_ENTITY_MAPPING(GiftCard, experationdate)
END_PROPERTY_IN_ENTITY_MAPPING(GiftCard)

BEGIN_ENTITY_CONSTRUCTOR(GiftCard, type_base)
    ON_PROPERTY_IN_ENTITY_CONSTRUCTOR(giftcardid, 0)
    ON_PROPERTY_IN_ENTITY_CONSTRUCTOR(amount, 0.0)
END_ENTITY_CONSTRUCTOR(GiftCard, type_base)

BEGIN_SERIALIZE_PROPERTY_IN_ENTITY_MAPPING(GiftCard)
    ON_SERIALIZE_PROPERTY_IN_ENTITY_MAPPING(GiftCard, giftcardid)
    ON_SERIALIZE_PROPERTY_IN_ENTITY_MAPPING(GiftCard, giftcardno)
	ON_SERIALIZE_PROPERTY_IN_ENTITY_MAPPING(GiftCard, amount)
    ON_SERIALIZE_PROPERTY_IN_ENTITY_MAPPING(GiftCard, experationdate)
END_SERIALIZE_PROPERTY_IN_ENTITY_MAPPING(GiftCard)

IMPLEMENT_EDM_INFO(GiftCard, Microsoft.Test.OData.Services.ODataWCFService, GiftCard)

BEGIN_PROPERTY_IN_ENTITY_MAPPING(Account)
    ON_PROPERTY_IN_ENTITY_MAPPING(Account, accountid)
    ON_PROPERTY_IN_ENTITY_MAPPING(Account, country)
	ON_PROPERTY_IN_ENTITY_MAPPING(Account, accountinfo)
	ON_PROPERTY_IN_ENTITY_MAPPING(Account, giftcard)
	ON_PROPERTY_IN_ENTITY_MAPPING(Account, mypaymentinstruments)
	ON_PROPERTY_IN_ENTITY_MAPPING(Account, vipinfo)
END_PROPERTY_IN_ENTITY_MAPPING(Account)

BEGIN_ENTITY_CONSTRUCTOR(Account, type_base)
    ON_PROPERTY_IN_ENTITY_CONSTRUCTOR(accountid, 0)
    ON_PROPERTY_IN_ENTITY_CONSTRUCTOR(giftcard, nullptr)
	ON_PROPERTY_IN_ENTITY_CONSTRUCTOR(accountinfo, nullptr)
	ON_PROPERTY_IN_ENTITY_CONSTRUCTOR(vipinfo, nullptr)
END_ENTITY_CONSTRUCTOR(Account, type_base)

BEGIN_SERIALIZE_PROPERTY_IN_ENTITY_MAPPING(Account)
    ON_SERIALIZE_PROPERTY_IN_ENTITY_MAPPING(Account, accountid)
    ON_SERIALIZE_PROPERTY_IN_ENTITY_MAPPING(Account, country)
	ON_SERIALIZE_PROPERTY_IN_ENTITY_MAPPING(Account, accountinfo)
	ON_SERIALIZE_PROPERTY_IN_ENTITY_MAPPING(Account, vipinfo)
END_SERIALIZE_PROPERTY_IN_ENTITY_MAPPING(Account)

IMPLEMENT_EDM_INFO(Account, Microsoft.Test.OData.Services.ODataWCFService, Account)

BEGIN_PROPERTY_IN_COMPLEX_MAPPING_WITH_BASE_CLASS(VipAccountInfo, AccountInfo)
    ON_PROPERTY_IN_COMPLEX_MAPPING(VipAccountInfo, vipinfo)
END_PROPERTY_IN_COMPLEX_MAPPING_WITH_BASE_CLASS(VipAccountInfo, AccountInfo)

BEGIN_COMPLEX_CONSTRUCTOR(VipAccountInfo, AccountInfo)
END_COMPLEX_CONSTRUCTOR(VipAccountInfo, AccountInfo)

BEGIN_SERIALIZE_PROPERTY_IN_COMPLEX_MAPPING(VipAccountInfo)
    ON_SERIALIZE_PROPERTY_IN_COMPLEX_MAPPING(VipAccountInfo, vipinfo)
END_SERIALIZE_PROPERTY_IN_COMPLEX_MAPPING(VipAccountInfo)

IMPLEMENT_EDM_INFO(VipAccountInfo, Microsoft.Test.OData.Services.ODataWCFService, VIPAccountInfo)

BEGIN_PROPERTY_IN_COMPLEX_MAPPING(Address)
    ON_PROPERTY_IN_COMPLEX_MAPPING(Address, street)
    ON_PROPERTY_IN_COMPLEX_MAPPING(Address, city)
    ON_PROPERTY_IN_COMPLEX_MAPPING(Address, postalcode)
    ON_PROPERTY_IN_COMPLEX_MAPPING(Address, numbers)
    ON_PROPERTY_IN_COMPLEX_MAPPING(Address, code)
    ON_PROPERTY_IN_COMPLEX_MAPPING(Address, color)
    ON_PROPERTY_IN_COMPLEX_MAPPING(Address, covercolors)
    ON_PROPERTY_IN_COMPLEX_MAPPING(Address, skincolor)
END_PROPERTY_IN_COMPLEX_MAPPING(Address)

BEGIN_COMPLEX_CONSTRUCTOR(Address, type_base)
    ON_PROPERTY_IN_ENTITY_CONSTRUCTOR(code, nullptr)
    ON_PROPERTY_IN_ENTITY_CONSTRUCTOR(skincolor, nullptr)
END_COMPLEX_CONSTRUCTOR(Address, type_base)

BEGIN_COMPLEX_DESTRUCTOR(Address)
    ON_PROPERTY_IN_ENTITY_DESTRUCTOR(code)
    ON_PROPERTY_IN_ENTITY_DESTRUCTOR(skincolor)
END_COMPLEX_DESTRUCTOR(Address)

BEGIN_SERIALIZE_PROPERTY_IN_COMPLEX_MAPPING(Address)
	ON_SERIALIZE_PROPERTY_IN_COMPLEX_MAPPING(Address, street)
	ON_SERIALIZE_PROPERTY_IN_COMPLEX_MAPPING(Address, city)
	ON_SERIALIZE_PROPERTY_IN_COMPLEX_MAPPING(Address, postalcode)
	ON_SERIALIZE_PROPERTY_IN_COMPLEX_MAPPING(Address, numbers)
	ON_SERIALIZE_PROPERTY_IN_COMPLEX_MAPPING(Address, code)
	ON_SERIALIZE_PROPERTY_IN_COMPLEX_MAPPING(Address, color)
	ON_SERIALIZE_PROPERTY_IN_COMPLEX_MAPPING(Address, covercolors)
	ON_SERIALIZE_PROPERTY_IN_COMPLEX_MAPPING(Address, skincolor)
END_SERIALIZE_PROPERTY_IN_COMPLEX_MAPPING(Address)

IMPLEMENT_EDM_INFO(Address, Microsoft.Test.OData.Services.ODataWCFService, Address)

BEGIN_PROPERTY_IN_ENTITY_MAPPING(PaymentInstrument)
    ON_PROPERTY_IN_ENTITY_MAPPING(PaymentInstrument, paymentinstrumentid)
    ON_PROPERTY_IN_ENTITY_MAPPING(PaymentInstrument, friendlyname)
    ON_PROPERTY_IN_ENTITY_MAPPING(PaymentInstrument, createdate)
END_PROPERTY_IN_ENTITY_MAPPING(PaymentInstrument)

BEGIN_ENTITY_CONSTRUCTOR(PaymentInstrument, type_base)
END_ENTITY_CONSTRUCTOR(PaymentInstrument, type_base)

BEGIN_SERIALIZE_PROPERTY_IN_ENTITY_MAPPING(PaymentInstrument)
	ON_SERIALIZE_PROPERTY_IN_ENTITY_MAPPING(PaymentInstrument, paymentinstrumentid)
	ON_SERIALIZE_PROPERTY_IN_ENTITY_MAPPING(PaymentInstrument, friendlyname)
	ON_SERIALIZE_PROPERTY_IN_ENTITY_MAPPING(PaymentInstrument, createdate)
END_SERIALIZE_PROPERTY_IN_ENTITY_MAPPING(PaymentInstrument)

IMPLEMENT_EDM_INFO(PaymentInstrument, Microsoft.Test.OData.Services.ODataWCFService, PaymentInstrument)

BEGIN_PROPERTY_IN_ENTITY_MAPPING_WITH_BASE_CLASS(CreditCardPI, PaymentInstrument)
    ON_PROPERTY_IN_ENTITY_MAPPING(CreditCardPI, cardnumber)
    ON_PROPERTY_IN_ENTITY_MAPPING(CreditCardPI, cvv)
END_PROPERTY_IN_ENTITY_MAPPING_WITH_BASE_CLASS(CreditCardPI, PaymentInstrument)

BEGIN_ENTITY_CONSTRUCTOR(CreditCardPI, PaymentInstrument)
END_ENTITY_CONSTRUCTOR(CreditCardPI, PaymentInstrument)

BEGIN_SERIALIZE_PROPERTY_IN_ENTITY_MAPPING(CreditCardPI)
	ON_SERIALIZE_PROPERTY_IN_ENTITY_MAPPING(CreditCardPI, cardnumber)
	ON_SERIALIZE_PROPERTY_IN_ENTITY_MAPPING(CreditCardPI, cvv)
END_SERIALIZE_PROPERTY_IN_ENTITY_MAPPING(CreditCardPI)

IMPLEMENT_EDM_INFO(CreditCardPI, Microsoft.Test.OData.Services.ODataWCFService, CreditCardPI)

BEGIN_PROPERTY_IN_ENTITY_MAPPING(Person)
    ON_PROPERTY_IN_ENTITY_MAPPING(Person, personid)
    ON_PROPERTY_IN_ENTITY_MAPPING(Person, firstname)
	ON_PROPERTY_IN_ENTITY_MAPPING(Person, lastname)
	ON_PROPERTY_IN_ENTITY_MAPPING(Person, numbers)
	ON_PROPERTY_IN_ENTITY_MAPPING(Person, middlename)
	ON_PROPERTY_IN_ENTITY_MAPPING(Person, address)
END_PROPERTY_IN_ENTITY_MAPPING(Person)

BEGIN_SERIALIZE_PROPERTY_IN_ENTITY_MAPPING(Person)
    ON_SERIALIZE_PROPERTY_IN_ENTITY_MAPPING(Person, personid)
    ON_SERIALIZE_PROPERTY_IN_ENTITY_MAPPING(Person, firstname)
	ON_SERIALIZE_PROPERTY_IN_ENTITY_MAPPING(Person, lastname)
	ON_SERIALIZE_PROPERTY_IN_ENTITY_MAPPING(Person, numbers)
	ON_SERIALIZE_PROPERTY_IN_ENTITY_MAPPING(Person, middlename)
	ON_SERIALIZE_PROPERTY_IN_ENTITY_MAPPING(Person, address)
END_SERIALIZE_PROPERTY_IN_ENTITY_MAPPING(Person)

BEGIN_ENTITY_CONSTRUCTOR(Person, type_base)
    ON_PROPERTY_IN_ENTITY_CONSTRUCTOR(middlename, nullptr) 
    ON_PROPERTY_IN_ENTITY_CONSTRUCTOR(address, nullptr)   
END_ENTITY_CONSTRUCTOR(Person, type_base)

BEGIN_ENTITY_DESTRUCTOR(Person)
    ON_PROPERTY_IN_ENTITY_DESTRUCTOR(middlename)
END_ENTITY_DESTRUCTOR(Person)

IMPLEMENT_EDM_INFO(Person, Microsoft.Test.OData.Services.ODataWCFService, Person)

BEGIN_PROPERTY_IN_ENTITY_MAPPING(Product)
    ON_PROPERTY_IN_ENTITY_MAPPING(Product, useraccess)
    ON_PROPERTY_IN_ENTITY_MAPPING(Product, covercolors)
    ON_PROPERTY_IN_ENTITY_MAPPING(Product, skincolor)
    ON_PROPERTY_IN_ENTITY_MAPPING(Product, productid)
    ON_PROPERTY_IN_ENTITY_MAPPING(Product, pic)
    ON_PROPERTY_IN_ENTITY_MAPPING(Product, nullpic)
END_PROPERTY_IN_ENTITY_MAPPING(Product)

BEGIN_SERIALIZE_PROPERTY_IN_ENTITY_MAPPING(Product)
    ON_SERIALIZE_PROPERTY_IN_ENTITY_MAPPING(Product, useraccess)
    ON_SERIALIZE_PROPERTY_IN_ENTITY_MAPPING(Product, covercolors)
    ON_SERIALIZE_PROPERTY_IN_ENTITY_MAPPING(Product, skincolor)
    ON_SERIALIZE_PROPERTY_IN_ENTITY_MAPPING(Product, productid)
    ON_SERIALIZE_PROPERTY_IN_ENTITY_MAPPING(Product, pic)
    ON_SERIALIZE_PROPERTY_IN_ENTITY_MAPPING(Product, nullpic)
END_SERIALIZE_PROPERTY_IN_ENTITY_MAPPING(Product)

BEGIN_ENTITY_CONSTRUCTOR(Product, type_base)
    ON_PROPERTY_IN_ENTITY_CONSTRUCTOR(useraccess, AccessLevel::None) 
    ON_PROPERTY_IN_ENTITY_CONSTRUCTOR(skincolor, nullptr)  
    ON_PROPERTY_IN_ENTITY_CONSTRUCTOR(productid, 0)  
END_ENTITY_CONSTRUCTOR(Product, type_base)

BEGIN_ENTITY_DESTRUCTOR(Product)
    ON_PROPERTY_IN_ENTITY_DESTRUCTOR(skincolor)
END_ENTITY_DESTRUCTOR(Product)

IMPLEMENT_EDM_INFO(Product, Microsoft.Test.OData.Services.ODataWCFService, Product)

BGEIN_IMPLEMENT_FUNCTION_ENUM_TYPE_FROM_STRING(AccessLevel)
    ON_IMPLEMENT_FUNCTION_ENUM_TYPE_FROM_STRING(None, AccessLevel::None)
    ON_IMPLEMENT_FUNCTION_ENUM_TYPE_FROM_STRING(Read, AccessLevel::Read)
    ON_IMPLEMENT_FUNCTION_ENUM_TYPE_FROM_STRING(Write, AccessLevel::Write)
    ON_IMPLEMENT_FUNCTION_ENUM_TYPE_FROM_STRING(Execute, AccessLevel::Execute)
    ON_IMPLEMENT_FUNCTION_ENUM_TYPE_FROM_STRING(ReadWrite, AccessLevel::ReadWrite)
END_IMPLEMENT_FUNCTION_ENUM_TYPE_FROM_STRING(AccessLevel)

BGEIN_IMPLEMENT_FUNCTION_STRING_FROM_ENUM_TYPE(AccessLevel)
    ON_IMPLEMENT_FUNCTION_STRING_FROM_ENUM_TYPE(AccessLevel::None, None)
    ON_IMPLEMENT_FUNCTION_STRING_FROM_ENUM_TYPE(AccessLevel::Read, Read)
    ON_IMPLEMENT_FUNCTION_STRING_FROM_ENUM_TYPE(AccessLevel::Write, Write)
    ON_IMPLEMENT_FUNCTION_STRING_FROM_ENUM_TYPE(AccessLevel::Execute, Execute)
    ON_IMPLEMENT_FUNCTION_STRING_FROM_ENUM_TYPE(AccessLevel::ReadWrite, ReadWrite)
END_IMPLEMENT_FUNCTION_STRING_FROM_ENUM_TYPE(AccessLevel)

BGEIN_IMPLEMENT_FUNCTION_ENUM_TYPE_FROM_STRING(Color)
    ON_IMPLEMENT_FUNCTION_ENUM_TYPE_FROM_STRING(Red, Color::Red)
    ON_IMPLEMENT_FUNCTION_ENUM_TYPE_FROM_STRING(Green, Color::Green)
    ON_IMPLEMENT_FUNCTION_ENUM_TYPE_FROM_STRING(Blue, Color::Blue)
END_IMPLEMENT_FUNCTION_ENUM_TYPE_FROM_STRING(Color)

BGEIN_IMPLEMENT_FUNCTION_STRING_FROM_ENUM_TYPE(Color)
    ON_IMPLEMENT_FUNCTION_STRING_FROM_ENUM_TYPE(Color::Red, Red)
    ON_IMPLEMENT_FUNCTION_STRING_FROM_ENUM_TYPE(Color::Green, Green)
    ON_IMPLEMENT_FUNCTION_STRING_FROM_ENUM_TYPE(Color::Blue, Blue)
END_IMPLEMENT_FUNCTION_STRING_FROM_ENUM_TYPE(Color)

BEGIN_PROPERTY_IN_ENTITY_MAPPING_WITH_BASE_CLASS(Employee, Person)
    ON_PROPERTY_IN_ENTITY_MAPPING(Employee, datehired)
    ON_PROPERTY_IN_ENTITY_MAPPING(Employee, addresses)
END_PROPERTY_IN_ENTITY_MAPPING_WITH_BASE_CLASS(Employee, Person)

BEGIN_SERIALIZE_PROPERTY_IN_ENTITY_MAPPING(Employee)
    ON_SERIALIZE_PROPERTY_IN_ENTITY_MAPPING(Employee, datehired)
    ON_SERIALIZE_PROPERTY_IN_ENTITY_MAPPING(Employee, addresses)
END_SERIALIZE_PROPERTY_IN_ENTITY_MAPPING(Employee)

BEGIN_ENTITY_CONSTRUCTOR(Employee, Person)
END_ENTITY_CONSTRUCTOR(Employee, Person)

IMPLEMENT_EDM_INFO(Employee, Microsoft.Test.OData.Services.ODataWCFService, Employee)

BEGIN_PROPERTY_IN_ENTITY_MAPPING_WITH_BASE_CLASS(Customer, Person)
    ON_PROPERTY_IN_ENTITY_MAPPING(Customer, birthday)
END_PROPERTY_IN_ENTITY_MAPPING_WITH_BASE_CLASS(Customer, Person)

BEGIN_SERIALIZE_PROPERTY_IN_ENTITY_MAPPING(Customer)
    ON_SERIALIZE_PROPERTY_IN_ENTITY_MAPPING(Customer, birthday)
END_SERIALIZE_PROPERTY_IN_ENTITY_MAPPING(Customer)

BEGIN_ENTITY_CONSTRUCTOR(Customer, Person)
END_ENTITY_CONSTRUCTOR(Customer, Person)

IMPLEMENT_EDM_INFO(Customer, Microsoft.Test.OData.Services.ODataWCFService, Customer)

BEGIN_PROPERTY_IN_ENTITY_MAPPING_WITH_BASE_CLASS(VipCustomer, Customer)
    ON_PROPERTY_IN_ENTITY_MAPPING(VipCustomer, vipid)
    ON_PROPERTY_IN_ENTITY_MAPPING(VipCustomer, viptype)
END_PROPERTY_IN_ENTITY_MAPPING_WITH_BASE_CLASS(VipCustomer, Customer)

BEGIN_SERIALIZE_PROPERTY_IN_ENTITY_MAPPING(VipCustomer)
    ON_SERIALIZE_PROPERTY_IN_ENTITY_MAPPING(VipCustomer, vipid)
    ON_SERIALIZE_PROPERTY_IN_ENTITY_MAPPING(VipCustomer, viptype)
END_SERIALIZE_PROPERTY_IN_ENTITY_MAPPING(VipCustomer)

BEGIN_ENTITY_CONSTRUCTOR(VipCustomer, Customer)
END_ENTITY_CONSTRUCTOR(VipCustomer, Customer)

IMPLEMENT_EDM_INFO(VipCustomer, Microsoft.Test.OData.Services.ODataWCFService, VipCustomer)

BEGIN_PROPERTY_IN_ENTITY_MAPPING(ProductDetail)
    ON_PROPERTY_IN_ENTITY_MAPPING(ProductDetail, productid)
    ON_PROPERTY_IN_ENTITY_MAPPING(ProductDetail, productdetailedid)
    ON_PROPERTY_IN_ENTITY_MAPPING(ProductDetail, productname)
    ON_PROPERTY_IN_ENTITY_MAPPING(ProductDetail, description)
END_PROPERTY_IN_ENTITY_MAPPING(ProductDetail)

BEGIN_ENTITY_CONSTRUCTOR(ProductDetail, type_base)
END_ENTITY_CONSTRUCTOR(ProductDetail, type_base)

BEGIN_SERIALIZE_PROPERTY_IN_ENTITY_MAPPING(ProductDetail)
	ON_SERIALIZE_PROPERTY_IN_ENTITY_MAPPING(ProductDetail, productid)
	ON_SERIALIZE_PROPERTY_IN_ENTITY_MAPPING(ProductDetail, productdetailedid)
	ON_SERIALIZE_PROPERTY_IN_ENTITY_MAPPING(ProductDetail, productname)
	ON_SERIALIZE_PROPERTY_IN_ENTITY_MAPPING(ProductDetail, description)
END_SERIALIZE_PROPERTY_IN_ENTITY_MAPPING(ProductDetail)

IMPLEMENT_EDM_INFO(ProductDetail, Microsoft.Test.OData.Services.ODataWCFService, ProductDetail)

DECLARE_DERIVED_ENTITY_CREATOR_FUNC(Customer, customer)
DECLARE_DERIVED_ENTITY_CREATOR_FUNC(Employee, employee)
DECLARE_DERIVED_ENTITY_CREATOR_FUNC(CreditCardPI, creditcardpi)

IMPLEMENT_EMPTY_DERIVED_TYPE_CREATOR_MAP(Account)
IMPLEMENT_EMPTY_DERIVED_TYPE_CREATOR_MAP(Product)
IMPLEMENT_EMPTY_DERIVED_TYPE_CREATOR_MAP(GiftCard)
IMPLEMENT_EMPTY_DERIVED_TYPE_CREATOR_MAP(Customer)
IMPLEMENT_EMPTY_DERIVED_TYPE_CREATOR_MAP(Employee)
IMPLEMENT_EMPTY_DERIVED_TYPE_CREATOR_MAP(ProductDetail)
IMPLEMENT_EMPTY_DERIVED_TYPE_CREATOR_MAP(VipCustomer)

BEGIN_IMPLEMENT_ENTITY_DERIVED_TYPE_CREATOR_MAP(Person)
    ON_IMPLEMENT_ENTITY_DERIVED_TYPE_CREATOR_MAP(Customer, customer)
	ON_IMPLEMENT_ENTITY_DERIVED_TYPE_CREATOR_MAP(Employee, employee)
END_IMPLEMENT_ENTITY_DERIVED_TYPE_CREATOR_MAP(Person)

BEGIN_IMPLEMENT_ENTITY_DERIVED_TYPE_CREATOR_MAP(PaymentInstrument)
    ON_IMPLEMENT_ENTITY_DERIVED_TYPE_CREATOR_MAP(CreditCardPI, creditcardpi)
END_IMPLEMENT_ENTITY_DERIVED_TYPE_CREATOR_MAP(PaymentInstrument)


DECLARE_DERIVED_COMPLEX_CREATOR_FUNC(VipAccountInfo, vipaccountinfo)

BEGIN_IMPLEMENT_COMPLEX_DERIVED_TYPE_CREATOR_MAP(AccountInfo)
    ON_IMPLEMENT_COMPLEX_DERIVED_TYPE_CREATOR_MAP(VipAccountInfo, vipaccountinfo)
END_IMPLEMENT_COMPLEX_DERIVED_TYPE_CREATOR_MAP(AccountInfo)

IMPLEMENT_COMPLEX_DERIVED_TYPE_CREATOR_MAP(Address)
IMPLEMENT_COMPLEX_DERIVED_TYPE_CREATOR_MAP(VipAccountInfo)

}}}