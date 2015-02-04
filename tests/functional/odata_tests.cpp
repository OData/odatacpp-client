//---------------------------------------------------------------------
// <copyright file="odata_tests.cpp" company="Microsoft">
//      Copyright (C) Microsoft Corporation. All rights reserved. See License.txt in the project root for license information.
// </copyright>
//---------------------------------------------------------------------

#include "odata_tests.h"

// only used for functional test which is mannualy changed that is different from the original edm model
const char* test_model_string =
"<edmx:Edmx xmlns:edmx=\"http://docs.oasis-open.org/odata/ns/edmx\" Version=\"4.0\"> \
  <edmx:DataServices> \
    <Schema xmlns=\"http://docs.oasis-open.org/odata/ns/edm\" Namespace=\"Microsoft.Test.OData.Services.ODataWCFService\"> \
	  <ComplexType Name=\"Address\"> \
	    <Property Name=\"Street\" Type=\"Edm.String\" Nullable=\"false\"/> \
	    <Property Name=\"City\" Type=\"Edm.String\" Nullable=\"false\"/> \
	    <Property Name=\"PostalCode\" Type=\"Edm.String\" Nullable=\"false\"/> \
		<Property Name=\"Numbers\" Type=\"Collection(Edm.Int32)\"/> \
		<Property Name=\"Code\" Type=\"Edm.Int32\"/> \
		<Property Name=\"Color\" Type=\"Microsoft.Test.OData.Services.ODataWCFService.Color\" Nullable=\"false\"/> \
	    <Property Name=\"CoverColors\" Type=\"Collection(Microsoft.Test.OData.Services.ODataWCFService.Color)\" Nullable=\"false\"/> \
		<Property Name=\"SkinColor\" Type=\"Microsoft.Test.OData.Services.ODataWCFService.Color\" Nullable=\"false\"/> \
	  </ComplexType> \
	  <ComplexType Name=\"HomeAddress\" BaseType=\"Microsoft.Test.OData.Services.ODataWCFService.Address\"> \
	    <Property Name=\"FamilyName\" Type=\"Edm.String\"/> \
	  </ComplexType> \
	  <EnumType Name=\"AccessLevel\" IsFlags=\"true\"> \
		<Member Name=\"None\" Value=\"0\"/> \
		<Member Name=\"Read\" Value=\"1\"/> \
		<Member Name=\"Write\" Value=\"2\"/> \
		<Member Name=\"Execute\" Value=\"4\"/> \
		<Member Name=\"ReadWrite\" Value=\"3\"/> \
	  </EnumType> \
	  <EnumType Name=\"Color\"> \
	    <Member Name=\"Red\" Value=\"1\"/> \
	    <Member Name=\"Green\" Value=\"2\"/> \
	    <Member Name=\"Blue\" Value=\"4\"/> \
	  </EnumType> \
	  <EnumType Name=\"CompanyCategory\"> \
	    <Member Name=\"IT\" Value=\"0\"/> \
	    <Member Name=\"Communication\" Value=\"1\"/> \
	    <Member Name=\"Electronics\" Value=\"2\"/> \
	    <Member Name=\"Others\" Value=\"4\"/> \
	  </EnumType> \
	<EntityType Name=\"Person\"> \
	  <Key> \
	    <PropertyRef Name=\"PersonID\"/> \
	  </Key> \
	  <Property Name=\"PersonID\" Type=\"Edm.Int32\" Nullable=\"false\"/> \
	  <Property Name=\"FirstName\" Type=\"Edm.String\" Nullable=\"false\"/> \
	  <Property Name=\"LastName\" Type=\"Edm.String\" Nullable=\"false\"/> \
	  <Property Name=\"MiddleName\" Type=\"Edm.String\"/> \
	  <Property Name=\"HomeAddress\" Type=\"Microsoft.Test.OData.Services.ODataWCFService.Address\"/> \
	  <Property Name=\"Home\" Type=\"Edm.GeographyPoint\" SRID=\"4326\"/> \
	  <Property Name=\"Numbers\" Type=\"Collection(Edm.String)\" Nullable=\"false\"/> \
	  <Property Name=\"Emails\" Type=\"Collection(Edm.String)\"/> \
	  <NavigationProperty Name=\"Parent\" Type=\"Microsoft.Test.OData.Services.ODataWCFService.Person\" Nullable=\"false\"/> \
	</EntityType> \
	<EntityType Name=\"Customer\" BaseType=\"Microsoft.Test.OData.Services.ODataWCFService.Person\"> \
	  <Property Name=\"City\" Type=\"Edm.String\" Nullable=\"false\"/> \
	  <Property Name=\"Birthday\" Type=\"Edm.DateTimeOffset\" Nullable=\"false\"/> \
	  <Property Name=\"TimeBetweenLastTwoOrders\" Type=\"Edm.Duration\" Nullable=\"false\"/> \
	  <NavigationProperty Name=\"Orders\" Type=\"Collection(Microsoft.Test.OData.Services.ODataWCFService.Order)\"/> \
	  <NavigationProperty Name=\"Company\" Type=\"Microsoft.Test.OData.Services.ODataWCFService.Company\" Nullable=\"false\" Partner=\"VipCustomer\"/> \
	</EntityType> \
	"
	"<EntityType Name=\"Employee\" BaseType=\"Microsoft.Test.OData.Services.ODataWCFService.Person\"> \
	  <Property Name=\"DateHired\" Type=\"Edm.DateTimeOffset\" Nullable=\"false\"/> \
	  <Property Name=\"Office\" Type=\"Edm.GeographyPoint\" SRID=\"4326\"/> \
	  <Property Name=\"WorkAddresses\" Type=\"Collection(Microsoft.Test.OData.Services.ODataWCFService.Address)\"/> \
	  <NavigationProperty Name=\"Company\" Type=\"Microsoft.Test.OData.Services.ODataWCFService.Company\" Nullable=\"false\" Partner=\"Employees\"/> \
	</EntityType> \
	<EntityType Name=\"Product\"> \
	  <Key> \
	    <PropertyRef Name=\"ProductID\"/> \
	  </Key> \
	  <Property Name=\"ProductID\" Type=\"Edm.Int32\" Nullable=\"false\"/> \
	  <Property Name=\"TestInt16\" Type=\"Edm.Int16\" Nullable=\"true\"/> \
	  <Property Name=\"TestBinary\" Type=\"Edm.Binary\" Nullable=\"true\"/> \
	  <Property Name=\"TestSByte\" Type=\"Edm.SByte\" Nullable=\"true\"/> \
	  <Property Name=\"TestByte\" Type=\"Edm.Byte\" Nullable=\"true\"/> \
	  <Property Name=\"TestFloat\" Type=\"Edm.Single\" Nullable=\"true\"/> \
	  <Property Name=\"Name\" Type=\"Edm.String\" Nullable=\"false\"/> \
	  <Property Name=\"QuantityPerUnit\" Type=\"Edm.String\" Nullable=\"false\"/> \
	  <Property Name=\"UnitPrice\" Type=\"Edm.Single\" Nullable=\"false\"/> \
	  <Property Name=\"QuantityInStock\" Type=\"Edm.Int32\" Nullable=\"false\"/> \
	  <Property Name=\"Discontinued\" Type=\"Edm.Boolean\" Nullable=\"false\"/> \
	  <Property Name=\"Picture\" Type=\"Edm.Binary\" Nullable=\"false\"/> \
	  <Property Name=\"NullPicture\" Type=\"Edm.Binary\" Nullable=\"true\"/> \
	  <Property Name=\"UserAccess\" Type=\"Microsoft.Test.OData.Services.ODataWCFService.AccessLevel\"/> \
	  <Property Name=\"SkinColor\" Type=\"Microsoft.Test.OData.Services.ODataWCFService.Color\"/> \
	  <Property Name=\"CoverColors\" Type=\"Collection(Microsoft.Test.OData.Services.ODataWCFService.Color)\" Nullable=\"false\"/> \
	  <NavigationProperty Name=\"Details\" Type=\"Collection(Microsoft.Test.OData.Services.ODataWCFService.ProductDetail)\"> \
	    <ReferentialConstraint Property=\"ProductID\" ReferencedProperty=\"ProductID\"/> \
	  </NavigationProperty> \
	</EntityType> \
	<EntityType Name=\"ProductDetail\"> \
	  <Key> \
	    <PropertyRef Name=\"ProductID\"/> \
	    <PropertyRef Name=\"ProductDetailID\"/> \
	  </Key> \
	  <Property Name=\"ProductID\" Type=\"Edm.Int32\" Nullable=\"false\"/> \
	  <Property Name=\"ProductDetailID\" Type=\"Edm.Int32\" Nullable=\"false\"/> \
	  <Property Name=\"Description\" Type=\"Edm.String\" Nullable=\"false\"/> \
	  <NavigationProperty Name=\"RelatedProduct\" Type=\"Microsoft.Test.OData.Services.ODataWCFService.Product\"/> \
	  <NavigationProperty Name=\"Reviews\" Type=\"Collection(Microsoft.Test.OData.Services.ODataWCFService.ProductReview)\"> \
	    <ReferentialConstraint Property=\"ProductID\" ReferencedProperty=\"ProductID\"/> \
	    <ReferentialConstraint Property=\"ProductDetailID\" ReferencedProperty=\"ProductDetailID\"/> \
	  </NavigationProperty> \
	</EntityType> \
	<EntityType Name=\"ProductReview\"> \
	  <Key> \
	    <PropertyRef Name=\"ProductID\"/> \
	    <PropertyRef Name=\"ProductDetailID\"/> \
	    <PropertyRef Name=\"ReviewTitle\"/> \
	    <PropertyRef Name=\"RevisionID\"/> \
	  </Key> \
	  <Property Name=\"ProductID\" Type=\"Edm.Int32\" Nullable=\"false\"/> \
	  <Property Name=\"ProductDetailID\" Type=\"Edm.Int32\" Nullable=\"false\"/> \
	  <Property Name=\"ReviewTitle\" Type=\"Edm.String\" Nullable=\"false\"/> \
	  <Property Name=\"RevisionID\" Type=\"Edm.Int32\" Nullable=\"false\"/> \
	  <Property Name=\"Comment\" Type=\"Edm.String\" Nullable=\"false\"/> \
	  <Property Name=\"Author\" Type=\"Edm.String\" Nullable=\"false\"/> \
	</EntityType> \
	<EntityType Name=\"Order\"> \
	  <Key> \
	    <PropertyRef Name=\"OrderID\"/> \
	  </Key> \
	  <Property Name=\"OrderID\" Type=\"Edm.Int32\" Nullable=\"false\"/> \
	  <Property Name=\"OrderDate\" Type=\"Edm.DateTimeOffset\" Nullable=\"false\"/> \
	  <Property Name=\"ShelfLife\" Type=\"Edm.Duration\"/> \
	  <Property Name=\"OrderShelfLifes\" Type=\"Collection(Edm.Duration)\"/> \
	  <NavigationProperty Name=\"LoggedInEmployee\" Type=\"Microsoft.Test.OData.Services.ODataWCFService.Employee\" Nullable=\"false\"/> \
	  <NavigationProperty Name=\"CustomerForOrder\" Type=\"Microsoft.Test.OData.Services.ODataWCFService.Customer\" Nullable=\"false\"/> \
	  <NavigationProperty Name=\"OrderDetails\" Type=\"Collection(Microsoft.Test.OData.Services.ODataWCFService.OrderDetail)\"/> \
	</EntityType> \
	"
	"<EntityType Name=\"OrderDetail\"> \
	  <Key> \
	    <PropertyRef Name=\"OrderID\"/> \
	    <PropertyRef Name=\"ProductID\"/> \
	  </Key> \
	  <Property Name=\"OrderID\" Type=\"Edm.Int32\" Nullable=\"false\"/> \
	  <Property Name=\"ProductID\" Type=\"Edm.Int32\" Nullable=\"false\"/> \
	  <Property Name=\"OrderPlaced\" Type=\"Edm.DateTimeOffset\" Nullable=\"false\"/> \
	  <Property Name=\"Quantity\" Type=\"Edm.Int32\" Nullable=\"false\"/> \
	  <Property Name=\"UnitPrice\" Type=\"Edm.Single\" Nullable=\"false\"/> \
	  <NavigationProperty Name=\"ProductOrdered\" Type=\"Collection(Microsoft.Test.OData.Services.ODataWCFService.Product)\"/> \
	  <NavigationProperty Name=\"AssociatedOrder\" Type=\"Microsoft.Test.OData.Services.ODataWCFService.Order\" Nullable=\"false\"/> \
	</EntityType> \
	<EntityType Name=\"Department\"> \
	  <Key> \
	    <PropertyRef Name=\"DepartmentID\"/> \
	  </Key> \
	  <Property Name=\"DepartmentID\" Type=\"Edm.Int32\" Nullable=\"false\"/> \
	  <Property Name=\"Name\" Type=\"Edm.String\" Nullable=\"false\"/> \
	  <NavigationProperty Name=\"Company\" Type=\"Microsoft.Test.OData.Services.ODataWCFService.Company\" Nullable=\"false\" Partner=\"Departments\"/> \
	</EntityType> \
	<EntityType Name=\"Company\"> \
	  <Key> \
	    <PropertyRef Name=\"CompanyID\"/> \
	  </Key> \
	  <Property Name=\"CompanyID\" Type=\"Edm.Int32\" Nullable=\"false\"/> \
	  <Property Name=\"CompanyCategory\" Type=\"Microsoft.Test.OData.Services.ODataWCFService.CompanyCategory\"/> \
	  <Property Name=\"Revenue\" Type=\"Edm.Int64\" Nullable=\"false\"/> \
	  <Property Name=\"Name\" Type=\"Edm.String\"/> \
	  <Property Name=\"Address\" Type=\"Microsoft.Test.OData.Services.ODataWCFService.Address\"/> \
	  <NavigationProperty Name=\"Employees\" Type=\"Collection(Microsoft.Test.OData.Services.ODataWCFService.Employee)\" Partner=\"Company\"/> \
	  <NavigationProperty Name=\"VipCustomer\" Type=\"Microsoft.Test.OData.Services.ODataWCFService.Customer\" Nullable=\"false\" Partner=\"Company\"/> \
	  <NavigationProperty Name=\"Departments\" Type=\"Collection(Microsoft.Test.OData.Services.ODataWCFService.Department)\" Partner=\"Company\"/> \
	  <NavigationProperty Name=\"CoreDepartment\" Type=\"Microsoft.Test.OData.Services.ODataWCFService.Department\" Nullable=\"false\"/> \
	</EntityType> \
	<EntityType Name=\"PublicCompany\" BaseType=\"Microsoft.Test.OData.Services.ODataWCFService.Company\"> \
	  <Property Name=\"StockExchange\" Type=\"Edm.String\"/> \
	  <NavigationProperty Name=\"Assets\" Type=\"Collection(Microsoft.Test.OData.Services.ODataWCFService.Asset)\" ContainsTarget=\"true\"/> \
	  <NavigationProperty Name=\"Club\" Type=\"Microsoft.Test.OData.Services.ODataWCFService.Club\" Nullable=\"false\" ContainsTarget=\"true\"/> \
	  <NavigationProperty Name=\"LabourUnion\" Type=\"Microsoft.Test.OData.Services.ODataWCFService.LabourUnion\" Nullable=\"false\"/> \
	</EntityType> \
	<EntityType Name=\"Asset\"> \
	  <Key> \
	    <PropertyRef Name=\"AssetID\"/> \
	  </Key> \
	  <Property Name=\"AssetID\" Type=\"Edm.Int32\" Nullable=\"false\"/> \
	  <Property Name=\"Name\" Type=\"Edm.String\"/> \
	  <Property Name=\"Number\" Type=\"Edm.Int32\" Nullable=\"false\"/> \
	</EntityType> \
	<EntityType Name=\"Club\"> \
	  <Key> \
	    <PropertyRef Name=\"ClubID\"/> \
	  </Key> \
	  <Property Name=\"ClubID\" Type=\"Edm.Int32\" Nullable=\"false\"/> \
	  <Property Name=\"Name\" Type=\"Edm.String\"/> \
	</EntityType> \
	"
	"<EntityType Name=\"LabourUnion\"> \
	  <Key> \
	    <PropertyRef Name=\"LabourUnionID\"/> \
	  </Key> \
	  <Property Name=\"LabourUnionID\" Type=\"Edm.Int32\" Nullable=\"false\"/> \
	  <Property Name=\"Name\" Type=\"Edm.String\"/> \
	</EntityType> \
	<Action Name=\"AddAccessRight\" IsBound=\"true\"> \
	  <Parameter Name=\"product\" Type=\"Microsoft.Test.OData.Services.ODataWCFService.Product\" Nullable=\"false\"/> \
	  <Parameter Name=\"accessRight\" Type=\"Microsoft.Test.OData.Services.ODataWCFService.AccessLevel\"/> \
	  <ReturnType Type=\"Microsoft.Test.OData.Services.ODataWCFService.AccessLevel\"/> \
	</Action> \
	<Function Name=\"GetDefaultColor\" IsComposable=\"true\"> \
	  <ReturnType Type=\"Microsoft.Test.OData.Services.ODataWCFService.Color\"/> \
	</Function> \
	<Function Name=\"GetEmployeesCount\" IsBound=\"true\"> \
	  <Parameter Name=\"p\" Type=\"Microsoft.Test.OData.Services.ODataWCFService.Company\" Nullable=\"false\"/> \
	  <ReturnType Type=\"Edm.Int32\" Nullable=\"false\"/> \
	</Function> \
	<Action Name=\"IncreaseRevenue\" IsBound=\"true\"> \
	  <Parameter Name=\"p\" Type=\"Microsoft.Test.OData.Services.ODataWCFService.Company\" Nullable=\"false\"/> \
	  <Parameter Name=\"IncreaseValue\" Type=\"Edm.Int64\"/> \
	  <ReturnType Type=\"Edm.Int64\" Nullable=\"false\"/> \
	</Action> \
	<Function Name=\"GetActualAmount\" IsBound=\"true\"> \
	  <Parameter Name=\"giftcard\" Type=\"Microsoft.Test.OData.Services.ODataWCFService.GiftCard\" Nullable=\"false\"/> \
	  <Parameter Name=\"bonusRate\" Type=\"Edm.Double\"/> \
	  <ReturnType Type=\"Edm.Double\" Nullable=\"false\"/> \
	</Function> \
	<Function Name=\"GetDefaultPI\" IsBound=\"true\" EntitySetPath=\"account/MyPaymentInstruments\"> \
	  <Parameter Name=\"account\" Type=\"Microsoft.Test.OData.Services.ODataWCFService.Account\" Nullable=\"false\"/> \
	  <ReturnType Type=\"Microsoft.Test.OData.Services.ODataWCFService.PaymentInstrument\"/> \
	</Function> \
	<Action Name=\"RefreshDefaultPI\" IsBound=\"true\" EntitySetPath=\"account/MyPaymentInstruments\"> \
	  <Parameter Name=\"account\" Type=\"Microsoft.Test.OData.Services.ODataWCFService.Account\" Nullable=\"false\"/> \
	  <Parameter Name=\"newDate\" Type=\"Edm.DateTimeOffset\"/> \
	  <ReturnType Type=\"Microsoft.Test.OData.Services.ODataWCFService.PaymentInstrument\"/> \
	</Action> \
	<ComplexType Name=\"AccountInfo\"> \
	  <Property Name=\"FirstName\" Type=\"Edm.String\" Nullable=\"false\"/> \
	  <Property Name=\"LastName\" Type=\"Edm.String\" Nullable=\"false\"/> \
	</ComplexType> \
	<ComplexType Name=\"VIPAccountInfo\" BaseType=\"Microsoft.Test.OData.Services.ODataWCFService.AccountInfo\"> \
	  <Property Name=\"VIP_Info\" Type=\"Edm.String\" Nullable=\"false\"/> \
	</ComplexType> \
	<EntityType Name=\"Account\"> \
	  <Key> \
	    <PropertyRef Name=\"AccountID\"/> \
	  </Key> \
	  <Property Name=\"AccountID\" Type=\"Edm.Int32\" Nullable=\"false\"/> \
	  <Property Name=\"CountryRegion\" Type=\"Edm.String\" Nullable=\"false\"/> \
	  <Property Name=\"AccountInfo\" Type=\"Microsoft.Test.OData.Services.ODataWCFService.AccountInfo\"/> \
	  <Property Name=\"VIPAccountInfo\" Type=\"Microsoft.Test.OData.Services.ODataWCFService.VIPAccountInfo\"/> \
	  <NavigationProperty Name=\"MyGiftCard\" Type=\"Microsoft.Test.OData.Services.ODataWCFService.GiftCard\" ContainsTarget=\"true\"/> \
	  <NavigationProperty Name=\"MyPaymentInstruments\" Type=\"Collection(Microsoft.Test.OData.Services.ODataWCFService.PaymentInstrument)\" ContainsTarget=\"true\"/> \
	  <NavigationProperty Name=\"ActiveSubscriptions\" Type=\"Collection(Microsoft.Test.OData.Services.ODataWCFService.Subscription)\" ContainsTarget=\"true\"/> \
	  <NavigationProperty Name=\"AvailableSubscriptionTemplatess\" Type=\"Collection(Microsoft.Test.OData.Services.ODataWCFService.Subscription)\"/> \
	</EntityType> \
	"
	"<EntityType Name=\"GiftCard\"> \
	  <Key> \
	    <PropertyRef Name=\"GiftCardID\"/> \
	  </Key> \
	  <Property Name=\"GiftCardID\" Type=\"Edm.Int32\" Nullable=\"false\"/> \
	  <Property Name=\"GiftCardNO\" Type=\"Edm.String\" Nullable=\"false\"/> \
	  <Property Name=\"Amount\" Type=\"Edm.Double\" Nullable=\"false\"/> \
	  <Property Name=\"ExperationDate\" Type=\"Edm.DateTimeOffset\" Nullable=\"false\"/> \
	</EntityType> \
	<EntityType Name=\"PaymentInstrument\"> \
	  <Key> \
	    <PropertyRef Name=\"PaymentInstrumentID\"/> \
	  </Key> \
	  <Property Name=\"PaymentInstrumentID\" Type=\"Edm.Int32\" Nullable=\"false\"/> \
	  <Property Name=\"FriendlyName\" Type=\"Edm.String\" Nullable=\"false\"/> \
	  <Property Name=\"CreatedDate\" Type=\"Edm.DateTimeOffset\" Nullable=\"false\"/> \
	  <NavigationProperty Name=\"TheStoredPI\" Type=\"Microsoft.Test.OData.Services.ODataWCFService.StoredPI\" Nullable=\"false\"/> \
	  <NavigationProperty Name=\"BillingStatements\" Type=\"Collection(Microsoft.Test.OData.Services.ODataWCFService.Statement)\" ContainsTarget=\"true\"/> \
	  <NavigationProperty Name=\"BackupStoredPI\" Type=\"Microsoft.Test.OData.Services.ODataWCFService.StoredPI\" Nullable=\"false\"/> \
	</EntityType> \
	<EntityType Name=\"CreditCardPI\" BaseType=\"Microsoft.Test.OData.Services.ODataWCFService.PaymentInstrument\"> \
	  <Property Name=\"CardNumber\" Type=\"Edm.String\" Nullable=\"false\"/> \
	  <Property Name=\"CVV\" Type=\"Edm.String\" Nullable=\"false\"/> \
	  <Property Name=\"HolderName\" Type=\"Edm.String\" Nullable=\"false\"/> \
	  <Property Name=\"Balance\" Type=\"Edm.Double\" Nullable=\"false\"/> \
	  <Property Name=\"ExperationDate\" Type=\"Edm.DateTimeOffset\" Nullable=\"false\"/> \
	  <NavigationProperty Name=\"CreditRecords\" Type=\"Collection(Microsoft.Test.OData.Services.ODataWCFService.CreditRecord)\" ContainsTarget=\"true\"/> \
	</EntityType> \
	<EntityType Name=\"StoredPI\"> \
	  <Key> \
	    <PropertyRef Name=\"StoredPIID\"/> \
	  </Key> \
	  <Property Name=\"StoredPIID\" Type=\"Edm.Int32\" Nullable=\"false\"/> \
	  <Property Name=\"PIName\" Type=\"Edm.String\" Nullable=\"false\"/> \
	  <Property Name=\"PIType\" Type=\"Edm.String\" Nullable=\"false\"/> \
	  <Property Name=\"CreatedDate\" Type=\"Edm.DateTimeOffset\" Nullable=\"false\"/> \
	</EntityType> \
	<EntityType Name=\"Statement\"> \
	  <Key> \
	    <PropertyRef Name=\"StatementID\"/> \
	  </Key> \
	  <Property Name=\"StatementID\" Type=\"Edm.Int32\" Nullable=\"false\"/> \
	  <Property Name=\"TransactionType\" Type=\"Edm.String\" Nullable=\"false\"/> \
	  <Property Name=\"TransactionDescription\" Type=\"Edm.String\" Nullable=\"false\"/> \
	  <Property Name=\"Amount\" Type=\"Edm.Double\" Nullable=\"false\"/> \
	</EntityType> \
	"
	"<EntityType Name=\"CreditRecord\"> \
	  <Key> \
	    <PropertyRef Name=\"CreditRecordID\"/> \
	  </Key> \
	  <Property Name=\"CreditRecordID\" Type=\"Edm.Int32\" Nullable=\"false\"/> \
	  <Property Name=\"IsGood\" Type=\"Edm.Boolean\" Nullable=\"false\"/> \
	  <Property Name=\"Reason\" Type=\"Edm.String\" Nullable=\"false\"/> \
	  <Property Name=\"CreatedDate\" Type=\"Edm.DateTimeOffset\" Nullable=\"false\"/> \
	</EntityType> \
	<EntityType Name=\"Subscription\"> \
	  <Key> \
	    <PropertyRef Name=\"SubscriptionID\"/> \
	  </Key> \
	  <Property Name=\"SubscriptionID\" Type=\"Edm.Int32\" Nullable=\"false\"/> \
	  <Property Name=\"TemplateGuid\" Type=\"Edm.String\" Nullable=\"false\"/> \
	  <Property Name=\"Title\" Type=\"Edm.String\" Nullable=\"false\"/> \
	  <Property Name=\"Category\" Type=\"Edm.String\" Nullable=\"false\"/> \
	  <Property Name=\"CreatedDate\" Type=\"Edm.DateTimeOffset\" Nullable=\"false\"/> \
	</EntityType> \
	<EntityContainer Name=\"InMemoryEntities\"> \
	  <EntitySet Name=\"People\" EntityType=\"Microsoft.Test.OData.Services.ODataWCFService.Person\"> \
	    <NavigationPropertyBinding Path=\"Parent\" Target=\"People\"/> \
	  </EntitySet> \
	  <Singleton Name=\"Boss\" Type=\"Microsoft.Test.OData.Services.ODataWCFService.Person\"/> \
	  <EntitySet Name=\"Customers\" EntityType=\"Microsoft.Test.OData.Services.ODataWCFService.Customer\"> \
	    <NavigationPropertyBinding Path=\"Orders\" Target=\"Orders\"/> \
	  </EntitySet> \
	  <Singleton Name=\"VipCustomer\" Type=\"Microsoft.Test.OData.Services.ODataWCFService.Customer\"> \
	    <NavigationPropertyBinding Path=\"Orders\" Target=\"Orders\"/> \
	    <NavigationPropertyBinding Path=\"Company\" Target=\"Company\"/> \
	  </Singleton> \
	  <EntitySet Name=\"Employees\" EntityType=\"Microsoft.Test.OData.Services.ODataWCFService.Employee\"> \
	    <NavigationPropertyBinding Path=\"Company\" Target=\"Company\"/> \
	  </EntitySet> \
	  <EntitySet Name=\"Products\" EntityType=\"Microsoft.Test.OData.Services.ODataWCFService.Product\"> \
	    <NavigationPropertyBinding Path=\"Details\" Target=\"ProductDetails\"/> \
	  </EntitySet> \
	  <EntitySet Name=\"ProductDetails\" EntityType=\"Microsoft.Test.OData.Services.ODataWCFService.ProductDetail\"> \
	    <NavigationPropertyBinding Path=\"RelatedProduct\" Target=\"Products\"/> \
	    <NavigationPropertyBinding Path=\"Reviews\" Target=\"ProductReviews\"/> \
	  </EntitySet> \
	  <EntitySet Name=\"ProductReviews\" EntityType=\"Microsoft.Test.OData.Services.ODataWCFService.ProductReview\"/> \
	  <EntitySet Name=\"Orders\" EntityType=\"Microsoft.Test.OData.Services.ODataWCFService.Order\"> \
	    <NavigationPropertyBinding Path=\"LoggedInEmployee\" Target=\"Employees\"/> \
	    <NavigationPropertyBinding Path=\"CustomerForOrder\" Target=\"Customers\"/> \
	    <NavigationPropertyBinding Path=\"OrderDetails\" Target=\"OrderDetails\"/> \
	  </EntitySet> \
	  <EntitySet Name=\"OrderDetails\" EntityType=\"Microsoft.Test.OData.Services.ODataWCFService.OrderDetail\"> \
	    <NavigationPropertyBinding Path=\"AssociatedOrder\" Target=\"Orders\"/> \
	    <NavigationPropertyBinding Path=\"ProductOrdered\" Target=\"Products\"/> \
	  </EntitySet> \
	  <EntitySet Name=\"Departments\" EntityType=\"Microsoft.Test.OData.Services.ODataWCFService.Department\"> \
	    <NavigationPropertyBinding Path=\"Company\" Target=\"Company\"/> \
	  </EntitySet> \
	"
	"  <Singleton Name=\"Company\" Type=\"Microsoft.Test.OData.Services.ODataWCFService.Company\"> \
	    <NavigationPropertyBinding Path=\"Employees\" Target=\"Employees\"/> \
	    <NavigationPropertyBinding Path=\"VipCustomer\" Target=\"VipCustomer\"/> \
	    <NavigationPropertyBinding Path=\"Departments\" Target=\"Departments\"/> \
	    <NavigationPropertyBinding Path=\"CoreDepartment\" Target=\"Departments\"/> \
	  </Singleton> \
	  <Singleton Name=\"PublicCompany\" Type=\"Microsoft.Test.OData.Services.ODataWCFService.Company\"> \
	    <NavigationPropertyBinding Path=\"Microsoft.Test.OData.Services.ODataWCFService.PublicCompany/LabourUnion\" Target=\"LabourUnion\"/> \
	  </Singleton> \
	  <Singleton Name=\"LabourUnion\" Type=\"Microsoft.Test.OData.Services.ODataWCFService.LabourUnion\"/> \
	  <FunctionImport Name=\"GetDefaultColor\" Function=\"Microsoft.Test.OData.Services.ODataWCFService.GetDefaultColor\"/> \
	  <EntitySet Name=\"Accounts\" EntityType=\"Microsoft.Test.OData.Services.ODataWCFService.Account\"> \
	    <NavigationPropertyBinding Path=\"Microsoft.Test.OData.Services.ODataWCFService.PaymentInstrument/TheStoredPI\" Target=\"StoredPIs\"/> \
	    <NavigationPropertyBinding Path=\"AvailableSubscriptionTemplatess\" Target=\"SubscriptionTemplates\"/> \
	    <NavigationPropertyBinding Path=\"Microsoft.Test.OData.Services.ODataWCFService.PaymentInstrument/BackupStoredPI\" Target=\"DefaultStoredPI\"/> \
	  </EntitySet> \
	  <EntitySet Name=\"StoredPIs\" EntityType=\"Microsoft.Test.OData.Services.ODataWCFService.StoredPI\"/> \
	  <EntitySet Name=\"SubscriptionTemplates\" EntityType=\"Microsoft.Test.OData.Services.ODataWCFService.Subscription\"/> \
	  <Singleton Name=\"DefaultStoredPI\" Type=\"Microsoft.Test.OData.Services.ODataWCFService.StoredPI\"/> \
	</EntityContainer> \
	</Schema> \
  </edmx:DataServices> \
</edmx:Edmx> \
";

std::shared_ptr<::odata::edm::edm_model> g_test_model;
::utility::string_t g_service_root_url = U("http://odatae2etest.azurewebsites.net/cpptest/DefaultService");
std::shared_ptr<::odata::edm::edm_model> get_test_model()
{
	if (!g_test_model)
	{
		auto model_reader = std::make_shared<::odata::edm::edm_model_reader>(concurrency::streams::bytestream::open_istream(std::move(std::string(test_model_string))));

		model_reader->parse();
		g_test_model = model_reader->get_model();
	}

	return g_test_model;
}