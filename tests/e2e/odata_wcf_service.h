#pragma once
#include "odata/codegen/code_generation.h"
#include "odata/codegen/odata_service_context.h"
#include "odata/codegen/odata_service_query.h"
#include "odata/codegen/odata_entityset_query_executor.h"
#include "odata/codegen/odata_singleton_query_executor.h"
#include "odata/codegen/odata_primitive_query_executor.h"
#include "odata/codegen/odata_primitive_query_executor.h"
#include "odata/codegen/odata_complex_query_executor.h"
#include "odata/codegen/odata_enum_query_executor.h"
#include "odata/codegen/odata_void_query_executor.h"
#include "odata/codegen/odata_query_builder.h"
#include "cpprest/json.h"

using namespace std;
using namespace ::odata::client;
using namespace ::odata::core;
using namespace ::odata::edm;
using namespace ::odata::codegen;

namespace tests { namespace e2e { namespace odata { namespace odata_wcf_service {


class AccountInfo;
class Address;
class HomeAddress;
class CompanyAddress;

class Person;
class Statement;
class Department;
class Customer;
class ProductDetail;
class Employee;
class Product;
class ProductReview;
class Order;
class OrderDetail;
class Company;
class PublicCompany;
class Asset;
class GiftCard;
class Club;
class LabourUnion;
class Account;
class PaymentInstrument;
class CreditRecord;
class CreditCardPI;
class StoredPI;
class Subscription;

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

enum CompanyCategory
{
    IT = 0,
    Communication = 1,
    Electronics = 2,
    Others = 4,
};

class enum_type_resolver
{
public:
    DECLARE_FUNCTION_ENUM_TYPE_FROM_STRING(AccessLevel);
    DECLARE_FUNCTION_STRING_FROM_ENUM_TYPE(AccessLevel);
    DECLARE_GET_ENUM_TYPE_NAMESPACE(AccessLevel, Microsoft.Test.OData.Services.ODataWCFService);
    DECLARE_FUNCTION_ENUM_TYPE_FROM_STRING(Color);
    DECLARE_FUNCTION_STRING_FROM_ENUM_TYPE(Color);
    DECLARE_GET_ENUM_TYPE_NAMESPACE(Color, Microsoft.Test.OData.Services.ODataWCFService);
    DECLARE_FUNCTION_ENUM_TYPE_FROM_STRING(CompanyCategory);
    DECLARE_FUNCTION_STRING_FROM_ENUM_TYPE(CompanyCategory);
    DECLARE_GET_ENUM_TYPE_NAMESPACE(CompanyCategory, Microsoft.Test.OData.Services.ODataWCFService);
};

    
#include "odata/codegen/odata_function_param_formatter.h"
    
class AccountInfo : public type_base
{
public:
    DECLARE_COMPLEX_CONSTRUCTOR(AccountInfo);
    DECLARE_COMPLEX_DESTRUCTOR(AccountInfo);
    DECLARE_EDM_INFO();
    ENABLE_PROPERTY_IN_COMPLEX_MAPPING();

    DECLARE_PRIMITIVE_PROPERTY_IN_COMPLEX_MAPPING(lastname, LastName, ::utility::string_t);
    DECLARE_PRIMITIVE_PROPERTY_IN_COMPLEX_MAPPING(firstname, FirstName, ::utility::string_t);
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
};

class HomeAddress : public Address
{
public:
    DECLARE_COMPLEX_CONSTRUCTOR(HomeAddress);
    DECLARE_COMPLEX_DESTRUCTOR(HomeAddress);
    DECLARE_EDM_INFO();
    ENABLE_PROPERTY_IN_COMPLEX_MAPPING();

    DECLARE_NULLABLE_PRIMITIVE_PROPERTY_IN_COMPLEX_MAPPING(familyname, FamilyName, ::utility::string_t);
};

class CompanyAddress : public Address
{
public:
    DECLARE_COMPLEX_CONSTRUCTOR(CompanyAddress);
    DECLARE_COMPLEX_DESTRUCTOR(CompanyAddress);
    DECLARE_EDM_INFO();
    ENABLE_PROPERTY_IN_COMPLEX_MAPPING();

    DECLARE_PRIMITIVE_PROPERTY_IN_COMPLEX_MAPPING(companyname, CompanyName, ::utility::string_t);
};

class Person : public type_base
{
public:
    DECLARE_ENTITY_CONSTRUCTOR(Person);
    DECLARE_ENTITY_DESTRUCTOR(Person);
    DECLARE_EDM_INFO();
    ENABLE_PROPERTY_IN_ENTITY_MAPPING();

    DECLARE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(personid, PersonID, int32_t);
    DECLARE_COLLECTION_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(numbers, Numbers, ::utility::string_t);
    DECLARE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(lastname, LastName, ::utility::string_t);
    DECLARE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(firstname, FirstName, ::utility::string_t);
    DECLARE_NULLABLE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(middlename, MiddleName, ::utility::string_t);
    DECLARE_NULLABLE_COMPLEX_PROPERTY_IN_ENTITY_MAPPING(homeaddress, HomeAddress, Address);
    DECLARE_COLLECTION_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(emails, Emails, ::utility::string_t);
    DECLARE_NAVIGATION_PROPERTY_IN_ENTITY_MAPPING(parent, Parent, Person);
    DECLARE_ACTION_P2(Person, ResetAddress, odata_entityset_query_executor<Person>, addresses, std::vector<Address>, addresses, index, int32_t, index);
    DECLARE_FUNCTION_P0(Person, GetHomeAddress, odata_complex_query_executor<HomeAddress>);

    DECLARE_GET_KEY_PROPERTY_STRING_ONE_PARAM(type_base, PersonID, personid);
};

class Statement : public type_base
{
public:
    DECLARE_ENTITY_CONSTRUCTOR(Statement);
    DECLARE_ENTITY_DESTRUCTOR(Statement);
    DECLARE_EDM_INFO();
    ENABLE_PROPERTY_IN_ENTITY_MAPPING();

    DECLARE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(statementid, StatementID, int32_t);
    DECLARE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(amount, Amount, double);
    DECLARE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(transactiontype, TransactionType, ::utility::string_t);
    DECLARE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(transactiondescription, TransactionDescription, ::utility::string_t);

    DECLARE_GET_KEY_PROPERTY_STRING_ONE_PARAM(type_base, StatementID, statementid);
};

class Department : public type_base
{
public:
    DECLARE_ENTITY_CONSTRUCTOR(Department);
    DECLARE_ENTITY_DESTRUCTOR(Department);
    DECLARE_EDM_INFO();
    ENABLE_PROPERTY_IN_ENTITY_MAPPING();

    DECLARE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(departmentid, DepartmentID, int32_t);
    DECLARE_NAVIGATION_PROPERTY_IN_ENTITY_MAPPING(company, Company, Company);
    DECLARE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(name, Name, ::utility::string_t);
    DECLARE_NULLABLE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(departmentno, DepartmentNO, ::utility::string_t);

    DECLARE_GET_KEY_PROPERTY_STRING_ONE_PARAM(type_base, DepartmentID, departmentid);
};

class Customer : public Person
{
public:
    DECLARE_ENTITY_CONSTRUCTOR(Customer);
    DECLARE_ENTITY_DESTRUCTOR(Customer);
    DECLARE_EDM_INFO();
    ENABLE_PROPERTY_IN_ENTITY_MAPPING();

    DECLARE_NAVIGATION_PROPERTY_IN_ENTITY_MAPPING(company, Company, Company);
    DECLARE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(city, City, ::utility::string_t);
    DECLARE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(birthday, Birthday, ::utility::datetime);
    DECLARE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(timebetweenlasttwoorders, TimeBetweenLastTwoOrders, ::utility::seconds);
    DECLARE_COLLECTION_NAVIGATION_PROPERTY_IN_ENTITY_MAPPING(orders, Orders, Order);

    DECLARE_GET_KEY_PROPERTY_STRING_NO_PARAM(Person);
};

class ProductDetail : public type_base
{
public:
    DECLARE_ENTITY_CONSTRUCTOR(ProductDetail);
    DECLARE_ENTITY_DESTRUCTOR(ProductDetail);
    DECLARE_EDM_INFO();
    ENABLE_PROPERTY_IN_ENTITY_MAPPING();

    DECLARE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(description, Description, ::utility::string_t);
    DECLARE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(productid, ProductID, int32_t);
    DECLARE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(productdetailid, ProductDetailID, int32_t);
    DECLARE_NAVIGATION_PROPERTY_IN_ENTITY_MAPPING(relatedproduct, RelatedProduct, Product);
    DECLARE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(productname, ProductName, ::utility::string_t);
    DECLARE_COLLECTION_NAVIGATION_PROPERTY_IN_ENTITY_MAPPING(reviews, Reviews, ProductReview);
    DECLARE_FUNCTION_P0(ProductDetail, GetRelatedProduct, odata_entityset_query_executor<Product>);

    DECLARE_GET_KEY_PROPERTY_STRING_TWO_PARAM(type_base, ProductID, productid, ProductDetailID, productdetailid);
};

class Employee : public Person
{
public:
    DECLARE_ENTITY_CONSTRUCTOR(Employee);
    DECLARE_ENTITY_DESTRUCTOR(Employee);
    DECLARE_EDM_INFO();
    ENABLE_PROPERTY_IN_ENTITY_MAPPING();

    DECLARE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(datehired, DateHired, ::utility::datetime);
    DECLARE_NAVIGATION_PROPERTY_IN_ENTITY_MAPPING(company, Company, Company);

    DECLARE_GET_KEY_PROPERTY_STRING_NO_PARAM(Person);
};

class Product : public type_base
{
public:
    DECLARE_ENTITY_CONSTRUCTOR(Product);
    DECLARE_ENTITY_DESTRUCTOR(Product);
    DECLARE_EDM_INFO();
    ENABLE_PROPERTY_IN_ENTITY_MAPPING();

    DECLARE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(productid, ProductID, int32_t);
    DECLARE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(name, Name, ::utility::string_t);
    DECLARE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(quantityperunit, QuantityPerUnit, ::utility::string_t);
    DECLARE_NULLABLE_ENUM_PROPERTY_IN_ENTITY_MAPPING(skincolor, SkinColor, Color);
    DECLARE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(unitprice, UnitPrice, float);
    DECLARE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(discontinued, Discontinued, bool);
    DECLARE_COLLECTION_ENUM_PROPERTY_IN_ENTITY_MAPPING(covercolors, CoverColors, Color);
    DECLARE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(quantityinstock, QuantityInStock, int32_t);
    DECLARE_NULLABLE_ENUM_PROPERTY_IN_ENTITY_MAPPING(useraccess, UserAccess, AccessLevel);
    DECLARE_COLLECTION_NAVIGATION_PROPERTY_IN_ENTITY_MAPPING(details, Details, ProductDetail);
    typedef odata_enum_query_executor<AccessLevel, enum_type_resolver> enum_AddAccessRight_excecutor;
    DECLARE_ACTION_P1(Product, AddAccessRight, enum_AddAccessRight_excecutor, accessRight, AccessLevel, accessRight);
    DECLARE_FUNCTION_P1(Product, GetProductDetails, odata_entityset_query_executor<ProductDetail>, count, int32_t, count);

    DECLARE_GET_KEY_PROPERTY_STRING_ONE_PARAM(type_base, ProductID, productid);
};

class ProductReview : public type_base
{
public:
    DECLARE_ENTITY_CONSTRUCTOR(ProductReview);
    DECLARE_ENTITY_DESTRUCTOR(ProductReview);
    DECLARE_EDM_INFO();
    ENABLE_PROPERTY_IN_ENTITY_MAPPING();

    DECLARE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(revisionid, RevisionID, int32_t);
    DECLARE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(productid, ProductID, int32_t);
    DECLARE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(productdetailid, ProductDetailID, int32_t);
    DECLARE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(reviewtitle, ReviewTitle, ::utility::string_t);
    DECLARE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(comment, Comment, ::utility::string_t);
    DECLARE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(author, Author, ::utility::string_t);

    DECLARE_GET_KEY_PROPERTY_STRING_FOUR_PARAM(type_base, RevisionID, revisionid, ProductID, productid, ProductDetailID, productdetailid, ReviewTitle, reviewtitle);
};

class Order : public type_base
{
public:
    DECLARE_ENTITY_CONSTRUCTOR(Order);
    DECLARE_ENTITY_DESTRUCTOR(Order);
    DECLARE_EDM_INFO();
    ENABLE_PROPERTY_IN_ENTITY_MAPPING();

    DECLARE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(orderid, OrderID, int32_t);
    DECLARE_NULLABLE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(shelflife, ShelfLife, ::utility::seconds);
    DECLARE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(orderdate, OrderDate, ::utility::datetime);
    DECLARE_NAVIGATION_PROPERTY_IN_ENTITY_MAPPING(loggedinemployee, LoggedInEmployee, Employee);
    DECLARE_COLLECTION_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(ordershelflifes, OrderShelfLifes, ::utility::seconds);
    DECLARE_NAVIGATION_PROPERTY_IN_ENTITY_MAPPING(customerfororder, CustomerForOrder, Customer);
    DECLARE_COLLECTION_NAVIGATION_PROPERTY_IN_ENTITY_MAPPING(orderdetails, OrderDetails, OrderDetail);

    DECLARE_GET_KEY_PROPERTY_STRING_ONE_PARAM(type_base, OrderID, orderid);
};

class OrderDetail : public type_base
{
public:
    DECLARE_ENTITY_CONSTRUCTOR(OrderDetail);
    DECLARE_ENTITY_DESTRUCTOR(OrderDetail);
    DECLARE_EDM_INFO();
    ENABLE_PROPERTY_IN_ENTITY_MAPPING();

    DECLARE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(unitprice, UnitPrice, float);
    DECLARE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(quantity, Quantity, int32_t);
    DECLARE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(orderid, OrderID, int32_t);
    DECLARE_NAVIGATION_PROPERTY_IN_ENTITY_MAPPING(associatedorder, AssociatedOrder, Order);
    DECLARE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(productid, ProductID, int32_t);
    DECLARE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(orderplaced, OrderPlaced, ::utility::datetime);
    DECLARE_COLLECTION_NAVIGATION_PROPERTY_IN_ENTITY_MAPPING(productordered, ProductOrdered, Product);

    DECLARE_GET_KEY_PROPERTY_STRING_TWO_PARAM(type_base, OrderID, orderid, ProductID, productid);
};

class Company : public type_base
{
public:
    DECLARE_ENTITY_CONSTRUCTOR(Company);
    DECLARE_ENTITY_DESTRUCTOR(Company);
    DECLARE_EDM_INFO();
    ENABLE_PROPERTY_IN_ENTITY_MAPPING();

    DECLARE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(companyid, CompanyID, int32_t);
    DECLARE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(revenue, Revenue, int64_t);
    DECLARE_NULLABLE_ENUM_PROPERTY_IN_ENTITY_MAPPING(companycategory, CompanyCategory, CompanyCategory);
    DECLARE_NULLABLE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(name, Name, ::utility::string_t);
    DECLARE_COLLECTION_NAVIGATION_PROPERTY_IN_ENTITY_MAPPING(departments, Departments, Department);
    DECLARE_NAVIGATION_PROPERTY_IN_ENTITY_MAPPING(coredepartment, CoreDepartment, Department);
    DECLARE_NULLABLE_COMPLEX_PROPERTY_IN_ENTITY_MAPPING(address, Address, Address);
    DECLARE_COLLECTION_NAVIGATION_PROPERTY_IN_ENTITY_MAPPING(employees, Employees, Employee);
    DECLARE_NAVIGATION_PROPERTY_IN_ENTITY_MAPPING(vipcustomer, VipCustomer, Customer);
    DECLARE_ACTION_P1(Company, IncreaseRevenue, odata_primitive_query_executor<int64_t>, IncreaseValue, int64_t, IncreaseValue);
    DECLARE_FUNCTION_P0(Company, GetEmployeesCount, odata_primitive_query_executor<int32_t>);

    DECLARE_GET_KEY_PROPERTY_STRING_ONE_PARAM(type_base, CompanyID, companyid);
};

class PublicCompany : public Company
{
public:
    DECLARE_ENTITY_CONSTRUCTOR(PublicCompany);
    DECLARE_ENTITY_DESTRUCTOR(PublicCompany);
    DECLARE_EDM_INFO();
    ENABLE_PROPERTY_IN_ENTITY_MAPPING();

    DECLARE_COLLECTION_NAVIGATION_PROPERTY_IN_ENTITY_MAPPING(assets, Assets, Asset);
    DECLARE_NULLABLE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(stockexchange, StockExchange, ::utility::string_t);
    DECLARE_NAVIGATION_PROPERTY_IN_ENTITY_MAPPING(club, Club, Club);
    DECLARE_NAVIGATION_PROPERTY_IN_ENTITY_MAPPING(labourunion, LabourUnion, LabourUnion);

    DECLARE_GET_KEY_PROPERTY_STRING_NO_PARAM(Company);
};

class Asset : public type_base
{
public:
    DECLARE_ENTITY_CONSTRUCTOR(Asset);
    DECLARE_ENTITY_DESTRUCTOR(Asset);
    DECLARE_EDM_INFO();
    ENABLE_PROPERTY_IN_ENTITY_MAPPING();

    DECLARE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(assetid, AssetID, int32_t);
    DECLARE_NULLABLE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(name, Name, ::utility::string_t);
    DECLARE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(number, Number, int32_t);

    DECLARE_GET_KEY_PROPERTY_STRING_ONE_PARAM(type_base, AssetID, assetid);
};

class GiftCard : public type_base
{
public:
    DECLARE_ENTITY_CONSTRUCTOR(GiftCard);
    DECLARE_ENTITY_DESTRUCTOR(GiftCard);
    DECLARE_EDM_INFO();
    ENABLE_PROPERTY_IN_ENTITY_MAPPING();

    DECLARE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(giftcardid, GiftCardID, int32_t);
    DECLARE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(giftcardno, GiftCardNO, ::utility::string_t);
    DECLARE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(amount, Amount, double);
    DECLARE_FUNCTION_P1(GiftCard, GetActualAmount, odata_primitive_query_executor<double>, bonusRate, double, bonusRate);
    DECLARE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(experationdate, ExperationDate, ::utility::datetime);
    DECLARE_NULLABLE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(ownername, OwnerName, ::utility::string_t);

    DECLARE_GET_KEY_PROPERTY_STRING_ONE_PARAM(type_base, GiftCardID, giftcardid);
};

class Club : public type_base
{
public:
    DECLARE_ENTITY_CONSTRUCTOR(Club);
    DECLARE_ENTITY_DESTRUCTOR(Club);
    DECLARE_EDM_INFO();
    ENABLE_PROPERTY_IN_ENTITY_MAPPING();

    DECLARE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(clubid, ClubID, int32_t);
    DECLARE_NULLABLE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(name, Name, ::utility::string_t);

    DECLARE_GET_KEY_PROPERTY_STRING_ONE_PARAM(type_base, ClubID, clubid);
};

class LabourUnion : public type_base
{
public:
    DECLARE_ENTITY_CONSTRUCTOR(LabourUnion);
    DECLARE_ENTITY_DESTRUCTOR(LabourUnion);
    DECLARE_EDM_INFO();
    ENABLE_PROPERTY_IN_ENTITY_MAPPING();

    DECLARE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(labourunionid, LabourUnionID, int32_t);
    DECLARE_NULLABLE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(name, Name, ::utility::string_t);

    DECLARE_GET_KEY_PROPERTY_STRING_ONE_PARAM(type_base, LabourUnionID, labourunionid);
};

class Account : public type_base
{
public:
    DECLARE_ENTITY_CONSTRUCTOR(Account);
    DECLARE_ENTITY_DESTRUCTOR(Account);
    DECLARE_EDM_INFO();
    ENABLE_PROPERTY_IN_ENTITY_MAPPING();

    DECLARE_FUNCTION_P0(Account, GetDefaultPI, odata_entityset_query_executor<PaymentInstrument>);
    DECLARE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(accountid, AccountID, int32_t);
    DECLARE_NAVIGATION_PROPERTY_IN_ENTITY_MAPPING(mygiftcard, MyGiftCard, GiftCard);
    DECLARE_COLLECTION_NAVIGATION_PROPERTY_IN_ENTITY_MAPPING(activesubscriptions, ActiveSubscriptions, Subscription);
    DECLARE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(country, Country, ::utility::string_t);
    DECLARE_COLLECTION_NAVIGATION_PROPERTY_IN_ENTITY_MAPPING(mypaymentinstruments, MyPaymentInstruments, PaymentInstrument);
    DECLARE_ACTION_P1(Account, RefreshDefaultPI, odata_entityset_query_executor<PaymentInstrument>, newDate, ::utility::datetime, newDate);
    DECLARE_NULLABLE_COMPLEX_PROPERTY_IN_ENTITY_MAPPING(accountinfo, AccountInfo, AccountInfo);
    DECLARE_COLLECTION_NAVIGATION_PROPERTY_IN_ENTITY_MAPPING(availablesubscriptiontemplatess, AvailableSubscriptionTemplatess, Subscription);
    DECLARE_FUNCTION_P0(Account, GetAccountInfo, odata_complex_query_executor<AccountInfo>);

    DECLARE_GET_KEY_PROPERTY_STRING_ONE_PARAM(type_base, AccountID, accountid);
};

class PaymentInstrument : public type_base
{
public:
    DECLARE_ENTITY_CONSTRUCTOR(PaymentInstrument);
    DECLARE_ENTITY_DESTRUCTOR(PaymentInstrument);
    DECLARE_EDM_INFO();
    ENABLE_PROPERTY_IN_ENTITY_MAPPING();

    DECLARE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(paymentinstrumentid, PaymentInstrumentID, int32_t);
    DECLARE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(friendlyname, FriendlyName, ::utility::string_t);
    DECLARE_NAVIGATION_PROPERTY_IN_ENTITY_MAPPING(backupstoredpi, BackupStoredPI, StoredPI);
    DECLARE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(createddate, CreatedDate, ::utility::datetime);
    DECLARE_NAVIGATION_PROPERTY_IN_ENTITY_MAPPING(thestoredpi, TheStoredPI, StoredPI);
    DECLARE_COLLECTION_NAVIGATION_PROPERTY_IN_ENTITY_MAPPING(billingstatements, BillingStatements, Statement);

    DECLARE_GET_KEY_PROPERTY_STRING_ONE_PARAM(type_base, PaymentInstrumentID, paymentinstrumentid);
};

class CreditRecord : public type_base
{
public:
    DECLARE_ENTITY_CONSTRUCTOR(CreditRecord);
    DECLARE_ENTITY_DESTRUCTOR(CreditRecord);
    DECLARE_EDM_INFO();
    ENABLE_PROPERTY_IN_ENTITY_MAPPING();

    DECLARE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(creditrecordid, CreditRecordID, int32_t);
    DECLARE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(isgood, IsGood, bool);
    DECLARE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(createddate, CreatedDate, ::utility::datetime);
    DECLARE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(reason, Reason, ::utility::string_t);

    DECLARE_GET_KEY_PROPERTY_STRING_ONE_PARAM(type_base, CreditRecordID, creditrecordid);
};

class CreditCardPI : public PaymentInstrument
{
public:
    DECLARE_ENTITY_CONSTRUCTOR(CreditCardPI);
    DECLARE_ENTITY_DESTRUCTOR(CreditCardPI);
    DECLARE_EDM_INFO();
    ENABLE_PROPERTY_IN_ENTITY_MAPPING();

    DECLARE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(holdername, HolderName, ::utility::string_t);
    DECLARE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(cardnumber, CardNumber, ::utility::string_t);
    DECLARE_COLLECTION_NAVIGATION_PROPERTY_IN_ENTITY_MAPPING(creditrecords, CreditRecords, CreditRecord);
    DECLARE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(experationdate, ExperationDate, ::utility::datetime);
    DECLARE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(cvv, CVV, ::utility::string_t);
    DECLARE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(balance, Balance, double);

    DECLARE_GET_KEY_PROPERTY_STRING_NO_PARAM(PaymentInstrument);
};

class StoredPI : public type_base
{
public:
    DECLARE_ENTITY_CONSTRUCTOR(StoredPI);
    DECLARE_ENTITY_DESTRUCTOR(StoredPI);
    DECLARE_EDM_INFO();
    ENABLE_PROPERTY_IN_ENTITY_MAPPING();

    DECLARE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(storedpiid, StoredPIID, int32_t);
    DECLARE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(piname, PIName, ::utility::string_t);
    DECLARE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(pitype, PIType, ::utility::string_t);
    DECLARE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(createddate, CreatedDate, ::utility::datetime);

    DECLARE_GET_KEY_PROPERTY_STRING_ONE_PARAM(type_base, StoredPIID, storedpiid);
};

class Subscription : public type_base
{
public:
    DECLARE_ENTITY_CONSTRUCTOR(Subscription);
    DECLARE_ENTITY_DESTRUCTOR(Subscription);
    DECLARE_EDM_INFO();
    ENABLE_PROPERTY_IN_ENTITY_MAPPING();

    DECLARE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(subscriptionid, SubscriptionID, int32_t);
    DECLARE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(templateguid, TemplateGuid, ::utility::string_t);
    DECLARE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(category, Category, ::utility::string_t);
    DECLARE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(title, Title, ::utility::string_t);
    DECLARE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(createddate, CreatedDate, ::utility::datetime);

    DECLARE_GET_KEY_PROPERTY_STRING_ONE_PARAM(type_base, SubscriptionID, subscriptionid);
};

class InMemoryEntities : public odata_service_context
{
public:
    InMemoryEntities(const ::utility::string_t& baseAddress, client_options options = client_options()) : odata_service_context(baseAddress, options)
    {
    }

    std::shared_ptr<odata_service_query<odata_entityset_query_executor<Person>, odata_query_builder>> create_people_query()
    {
        return create_query<odata_entityset_query_executor<Person>, odata_query_builder>(U("People"));
    }

    DECLARE_ACTION_IMPORT_P1(InMemoryEntities, ResetBossEmail, odata_primitive_query_executor<::utility::string_t>, emails, std::vector<::utility::string_t>, emails);

    DECLARE_FUNCTION_IMPORT_P2(InMemoryEntities, GetBossEmails, odata_primitive_query_executor<::utility::string_t>, start, int32_t, start, count, int32_t, count);

    std::shared_ptr<odata_service_query<odata_entityset_query_executor<Customer>, odata_query_builder>> create_customers_query()
    {
        return create_query<odata_entityset_query_executor<Customer>, odata_query_builder>(U("Customers"));
    }

    std::shared_ptr<odata_service_query<odata_entityset_query_executor<Department>, odata_query_builder>> create_departments_query()
    {
        return create_query<odata_entityset_query_executor<Department>, odata_query_builder>(U("Departments"));
    }

    std::shared_ptr<odata_service_query<odata_singleton_query_executor<Company>, odata_query_builder>> create_company_query()
    {
        return create_query<odata_singleton_query_executor<Company>, odata_query_builder>(U("Company"));
    }

    std::shared_ptr<odata_service_query<odata_entityset_query_executor<Employee>, odata_query_builder>> create_employees_query()
    {
        return create_query<odata_entityset_query_executor<Employee>, odata_query_builder>(U("Employees"));
    }

    std::shared_ptr<odata_service_query<odata_entityset_query_executor<StoredPI>, odata_query_builder>> create_storedpis_query()
    {
        return create_query<odata_entityset_query_executor<StoredPI>, odata_query_builder>(U("StoredPIs"));
    }

    std::shared_ptr<odata_service_query<odata_entityset_query_executor<ProductDetail>, odata_query_builder>> create_productdetails_query()
    {
        return create_query<odata_entityset_query_executor<ProductDetail>, odata_query_builder>(U("ProductDetails"));
    }

    std::shared_ptr<odata_service_query<odata_entityset_query_executor<Product>, odata_query_builder>> create_products_query()
    {
        return create_query<odata_entityset_query_executor<Product>, odata_query_builder>(U("Products"));
    }

    std::shared_ptr<odata_service_query<odata_entityset_query_executor<ProductReview>, odata_query_builder>> create_productreviews_query()
    {
        return create_query<odata_entityset_query_executor<ProductReview>, odata_query_builder>(U("ProductReviews"));
    }

    DECLARE_ACTION_IMPORT_P1(InMemoryEntities, Discount, odata_void_query_executor, percentage, int32_t, percentage);

    std::shared_ptr<odata_service_query<odata_entityset_query_executor<Order>, odata_query_builder>> create_orders_query()
    {
        return create_query<odata_entityset_query_executor<Order>, odata_query_builder>(U("Orders"));
    }

    std::shared_ptr<odata_service_query<odata_singleton_query_executor<Person>, odata_query_builder>> create_boss_query()
    {
        return create_query<odata_singleton_query_executor<Person>, odata_query_builder>(U("Boss"));
    }

    std::shared_ptr<odata_service_query<odata_entityset_query_executor<OrderDetail>, odata_query_builder>> create_orderdetails_query()
    {
        return create_query<odata_entityset_query_executor<OrderDetail>, odata_query_builder>(U("OrderDetails"));
    }

    std::shared_ptr<odata_service_query<odata_entityset_query_executor<Account>, odata_query_builder>> create_accounts_query()
    {
        return create_query<odata_entityset_query_executor<Account>, odata_query_builder>(U("Accounts"));
    }

    std::shared_ptr<odata_service_query<odata_entityset_query_executor<Subscription>, odata_query_builder>> create_subscriptiontemplates_query()
    {
        return create_query<odata_entityset_query_executor<Subscription>, odata_query_builder>(U("SubscriptionTemplates"));
    }

    DECLARE_FUNCTION_IMPORT_P0(InMemoryEntities, GetAllProducts, odata_entityset_query_executor<Product>);

    std::shared_ptr<odata_service_query<odata_singleton_query_executor<Customer>, odata_query_builder>> create_vipcustomer_query()
    {
        return create_query<odata_singleton_query_executor<Customer>, odata_query_builder>(U("VipCustomer"));
    }

    std::shared_ptr<odata_service_query<odata_singleton_query_executor<Company>, odata_query_builder>> create_publiccompany_query()
    {
        return create_query<odata_singleton_query_executor<Company>, odata_query_builder>(U("PublicCompany"));
    }

    std::shared_ptr<odata_service_query<odata_singleton_query_executor<LabourUnion>, odata_query_builder>> create_labourunion_query()
    {
        return create_query<odata_singleton_query_executor<LabourUnion>, odata_query_builder>(U("LabourUnion"));
    }

    std::shared_ptr<odata_service_query<odata_singleton_query_executor<StoredPI>, odata_query_builder>> create_defaultstoredpi_query()
    {
        return create_query<odata_singleton_query_executor<StoredPI>, odata_query_builder>(U("DefaultStoredPI"));
    }

    DECLARE_ACTION_IMPORT_P1(InMemoryEntities, ResetBossAddress, odata_complex_query_executor<Address>, address, Address, address);

    typedef odata_enum_query_executor<Color, enum_type_resolver> enum_GetDefaultColor_excecutor;
    DECLARE_FUNCTION_IMPORT_P0(InMemoryEntities, GetDefaultColor, enum_GetDefaultColor_excecutor);

    DECLARE_ACTION_IMPORT_P0(InMemoryEntities, ResetDataSource, odata_void_query_executor);

    DECLARE_FUNCTION_IMPORT_P1(InMemoryEntities, GetPerson, odata_entityset_query_executor<Person>, address, Address, address);

    DECLARE_FUNCTION_IMPORT_P1(InMemoryEntities, GetPerson2, odata_entityset_query_executor<Person>, city, ::utility::string_t, city);

    DECLARE_FUNCTION_IMPORT_P1(InMemoryEntities, GetProductsByAccessLevel, odata_primitive_query_executor<::utility::string_t>, accessLevel, AccessLevel, accessLevel);

};

}}}}