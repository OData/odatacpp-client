//---------------------------------------------------------------------
// <copyright file="edm_model_reader_test.cpp" company="Microsoft">
//      Copyright (C) Microsoft Corporation. All rights reserved. See License.txt in the project root for license information.
// </copyright>
//---------------------------------------------------------------------

#include "../odata_tests.h"
#include "cpprest/json.h"
#include "odata/edm/odata_edm.h"
#include "odata/edm/edm_model_reader.h"

using namespace ::odata::client;

#ifdef __APPLE__
extern "C" UnitTest::TestList& UnitTest::GetTestList()
{
    static TestList s_list;
    return s_list;
}
#endif

using namespace ::odata::edm;

namespace tests { namespace functional { namespace _odata {

const char*  edm_model_version_test_string = 
"<edmx:Edmx xmlns:edmx=\"http://docs.oasis-open.org/odata/ns/edmx\" Version=\"4.0\"> \
    <edmx:DataServices> \
	</edmx:DataServices> \
	</edmx:Edmx>  \
";

const char*  edm_model_enum_type_test_string = 
"<edmx:Edmx xmlns:edmx=\"http://docs.oasis-open.org/odata/ns/edmx\" Version=\"4.0\"> \
    <edmx:DataServices> \
	  <Schema xmlns=\"http://docs.oasis-open.org/odata/ns/edm\" Namespace=\"Microsoft.Test.OData.Services.ODataWCFService\"> \
	    <EnumType Name=\"AccessLevel\" IsFlags=\"true\"> \
          <Member Name=\"None\" Value=\"0\"/> \
          <Member Name=\"Read\" Value=\"1\"/> \
          <Member Name=\"Write\" Value=\"2\"/> \
          <Member Name=\"Execute\" Value=\"4\"/> \
          <Member Name=\"ReadWrite\" Value=\"3\"/> \
		</EnumType> \
	  </Schema> \
	</edmx:DataServices> \
	</edmx:Edmx>  \
";

const char*  edm_model_complex_type_test_string = 
"<edmx:Edmx xmlns:edmx=\"http://docs.oasis-open.org/odata/ns/edmx\" Version=\"4.0\"> \
    <edmx:DataServices> \
	  <Schema xmlns=\"http://docs.oasis-open.org/odata/ns/edm\" Namespace=\"Microsoft.Test.OData.Services.ODataWCFService\"> \
        <ComplexType Name=\"AccountInfo\"> \
          <Property Name=\"FirstName\" Type=\"Edm.String\" Nullable=\"false\"/> \
          <Property Name=\"LastName\" Type=\"Edm.String\" Nullable=\"false\"/> \
         </ComplexType> \
	  </Schema> \
	</edmx:DataServices> \
	</edmx:Edmx>  \
";

const char*  edm_model_entity_type_test_string = 
"<edmx:Edmx xmlns:edmx=\"http://docs.oasis-open.org/odata/ns/edmx\" Version=\"4.0\"> \
    <edmx:DataServices> \
	  <Schema xmlns=\"http://docs.oasis-open.org/odata/ns/edm\" Namespace=\"Microsoft.Test.OData.Services.ODataWCFService\"> \
	    <ComplexType Name=\"Address\">\
          <Property Name=\"Street\" Type=\"Edm.String\" Nullable=\"false\"/>\
          <Property Name=\"City\" Type=\"Edm.String\" Nullable=\"false\"/>\
          <Property Name=\"PostalCode\" Type=\"Edm.String\" Nullable=\"false\"/>\
        </ComplexType>\
		<EntityType Name=\"Person\">\
			<Key>\
			    <PropertyRef Name=\"PersonID\"/>\
			</Key>\
			<Property Name=\"PersonID\" Type=\"Edm.Int32\" Nullable=\"false\"/>\
			<Property Name=\"FirstName\" Type=\"Edm.String\" Nullable=\"false\"/>\
			<Property Name=\"LastName\" Type=\"Edm.String\" Nullable=\"false\"/>\
			<Property Name=\"MiddleName\" Type=\"Edm.String\"/>\
			<Property Name=\"HomeAddress\" Type=\"Microsoft.Test.OData.Services.ODataWCFService.Address\"/>\
			<Property Name=\"Home\" Type=\"Edm.GeographyPoint\" SRID=\"4326\"/>\
			<Property Name=\"Numbers\" Type=\"Collection(Edm.String)\" Nullable=\"false\"/>\
			<Property Name=\"Emails\" Type=\"Collection(Edm.String)\"/>\
			<NavigationProperty Name=\"Parent\" Type=\"Microsoft.Test.OData.Services.ODataWCFService.Person\" Nullable=\"false\"/>\
		</EntityType>\
	  </Schema> \
	</edmx:DataServices> \
	</edmx:Edmx>  \
";

const char* edm_model_collection_of_complex_type_test_string = 
"<edmx:Edmx xmlns:edmx=\"http://docs.oasis-open.org/odata/ns/edmx\" Version=\"4.0\"> \
    <edmx:DataServices> \
	  <Schema xmlns=\"http://docs.oasis-open.org/odata/ns/edm\" Namespace=\"Microsoft.Test.OData.Services.ODataWCFService\"> \
	    <ComplexType Name=\"Address\">\
          <Property Name=\"Street\" Type=\"Edm.String\" Nullable=\"false\"/>\
          <Property Name=\"City\" Type=\"Edm.String\" Nullable=\"false\"/>\
          <Property Name=\"PostalCode\" Type=\"Edm.String\" Nullable=\"false\"/>\
        </ComplexType>\
		<EntityType Name=\"Person\">\
			<Key>\
			    <PropertyRef Name=\"PersonID\"/>\
			</Key>\
			<Property Name=\"PersonID\" Type=\"Edm.Int32\" Nullable=\"false\"/>\
			<Property Name=\"FirstName\" Type=\"Edm.String\" Nullable=\"false\"/>\
			<Property Name=\"LastName\" Type=\"Edm.String\" Nullable=\"false\"/>\
			<Property Name=\"MiddleName\" Type=\"Edm.String\"/>\
			<Property Name=\"Addresses\" Type=\"Collection(Microsoft.Test.OData.Services.ODataWCFService.Address)\"/>\
			<NavigationProperty Name=\"Parent\" Type=\"Microsoft.Test.OData.Services.ODataWCFService.Person\" Nullable=\"false\"/>\
		</EntityType>\
	  </Schema> \
	</edmx:DataServices> \
	</edmx:Edmx>  \
";

const char* edm_model_collection_of_navigation_type_test_string = 
"<edmx:Edmx xmlns:edmx=\"http://docs.oasis-open.org/odata/ns/edmx\" Version=\"4.0\"> \
    <edmx:DataServices> \
	  <Schema xmlns=\"http://docs.oasis-open.org/odata/ns/edm\" Namespace=\"Microsoft.Test.OData.Services.ODataWCFService\"> \
	    <ComplexType Name=\"Address\">\
          <Property Name=\"Street\" Type=\"Edm.String\" Nullable=\"false\"/>\
          <Property Name=\"City\" Type=\"Edm.String\" Nullable=\"false\"/>\
          <Property Name=\"PostalCode\" Type=\"Edm.String\" Nullable=\"false\"/>\
        </ComplexType>\
		<EntityType Name=\"Person\">\
			<Key>\
			    <PropertyRef Name=\"PersonID\"/>\
			</Key>\
			<Property Name=\"PersonID\" Type=\"Edm.Int32\" Nullable=\"false\"/>\
			<Property Name=\"FirstName\" Type=\"Edm.String\" Nullable=\"false\"/>\
			<Property Name=\"LastName\" Type=\"Edm.String\" Nullable=\"false\"/>\
			<Property Name=\"MiddleName\" Type=\"Edm.String\"/>\
			<NavigationProperty Name=\"Parent\" Type=\"Collection(Microsoft.Test.OData.Services.ODataWCFService.Order)\" Nullable=\"false\"/>\
		</EntityType>\
		<EntityType Name=\"Order\">\
			<Key>\
			    <PropertyRef Name=\"OrderID\"/>\
			</Key>\
			<Property Name=\"OrderID\" Type=\"Edm.Int32\" Nullable=\"false\"/>\
		</EntityType>\
	  </Schema> \
	</edmx:DataServices> \
	</edmx:Edmx>  \
";

SUITE(odata_edm_model_reader_tests)
{
	// /name:odata_edm_model_reader_tests:*

TEST(collection_of_navigation_type)
{
	//auto model_reader = ::odata::make_shared<edm_model_reader>(edm_model_collection_of_navigation_type_test_string, strlen(edm_model_collection_of_navigation_type_test_string));
	//model_reader->parse();
	//auto model = model_reader->get_model();

	auto model_reader = ::odata::make_shared<edm_model_reader>(concurrency::streams::bytestream::open_istream(std::move(std::string(test_model_string))));
	model_reader->parse();
	auto model = model_reader->get_model();
}

TEST(collection_of_complex_type)
{
	auto model_reader = ::odata::make_shared<edm_model_reader>(concurrency::streams::bytestream::open_istream(std::move(std::string(edm_model_collection_of_complex_type_test_string))));
	model_reader->parse();
	auto model = model_reader->get_model();

	VERIFY_IS_NOT_NULL(model);
	auto enity_type = model->find_entity_type(_XPLATSTR("Person"));
	VERIFY_IS_NOT_NULL(enity_type);
	auto addresses = enity_type->find_property(_XPLATSTR("Addresses"));
	VERIFY_ARE_EQUAL(addresses->get_property_type()->get_type_kind(), edm_type_kind_t::Collection);
	auto collection_type = std::dynamic_pointer_cast<edm_collection_type>(addresses->get_property_type());
	auto collection_element_type = collection_type->get_element_type();
	VERIFY_ARE_EQUAL(collection_element_type->get_type_kind(), edm_type_kind_t::Complex);
	VERIFY_ARE_EQUAL(collection_element_type->get_full_name(), _XPLATSTR("Microsoft.Test.OData.Services.ODataWCFService.Address"));
}


TEST(edm_model_reader_entity_type_test)
{
	auto model_reader = ::odata::make_shared<edm_model_reader>(concurrency::streams::bytestream::open_istream(std::move(std::string(edm_model_entity_type_test_string))));
	model_reader->parse();
	auto model = model_reader->get_model();
	VERIFY_IS_NOT_NULL(model);
	auto enity_type = model->find_entity_type(_XPLATSTR("Person"));
	VERIFY_IS_NOT_NULL(enity_type);
	VERIFY_ARE_EQUAL(enity_type->get_type_kind(), edm_type_kind_t::Entity);
	VERIFY_ARE_EQUAL(enity_type->key().size(), 1);
	VERIFY_ARE_EQUAL(enity_type->key()[0], _XPLATSTR("PersonID"));
	auto property_home_address = enity_type->find_property(_XPLATSTR("HomeAddress"));
	VERIFY_IS_NOT_NULL(property_home_address);
	VERIFY_ARE_EQUAL(property_home_address->get_property_type()->get_type_kind(), edm_type_kind_t::Complex);
	auto navigation_property = enity_type->find_property(_XPLATSTR("Parent"));
	VERIFY_IS_NOT_NULL(navigation_property);
	VERIFY_ARE_EQUAL(navigation_property->get_property_type()->get_type_kind(), edm_type_kind_t::Navigation);
	auto navigation_type = std::dynamic_pointer_cast<edm_navigation_type>(navigation_property->get_property_type());
	VERIFY_IS_NOT_NULL(navigation_type);
	VERIFY_ARE_EQUAL(navigation_type->get_navigation_type()->get_type_kind(), edm_type_kind_t::Entity);
	VERIFY_ARE_EQUAL(navigation_type->get_navigation_type()->get_name(), _XPLATSTR("Person"));
}


TEST(edm_model_reader_complex_type_test)
{
	auto model_reader = ::odata::make_shared<edm_model_reader>(concurrency::streams::bytestream::open_istream(std::move(std::string(edm_model_complex_type_test_string))));
	model_reader->parse();
	auto model = model_reader->get_model();
	VERIFY_IS_NOT_NULL(model);
	auto complex_type = model->find_complex_type(_XPLATSTR("AccountInfo"));
	VERIFY_IS_NOT_NULL(complex_type);
	VERIFY_ARE_EQUAL(complex_type->get_type_kind(), edm_type_kind_t::Complex);
	auto property_last_name = complex_type->find_property(_XPLATSTR("LastName"));
	VERIFY_IS_NOT_NULL(property_last_name);
	VERIFY_ARE_EQUAL(property_last_name->is_nullable(), false);
	auto property_type = property_last_name->get_property_type();
	VERIFY_IS_NOT_NULL(property_type);
	VERIFY_ARE_EQUAL(property_type->get_type_kind(), edm_type_kind_t::Primitive);
	auto primitive_type = std::dynamic_pointer_cast<edm_primitive_type>(property_type);
	VERIFY_IS_NOT_NULL(primitive_type);
	VERIFY_ARE_EQUAL(primitive_type->get_primitive_kind(), edm_primitive_type_kind_t::String);
}

TEST(edm_model_reader_enum_type_test)
{
	auto model_reader = ::odata::make_shared<edm_model_reader>(concurrency::streams::bytestream::open_istream(std::move(std::string(edm_model_enum_type_test_string))));
	model_reader->parse();
	auto model = model_reader->get_model();
	VERIFY_IS_NOT_NULL(model);
	auto enum_type = model->find_enum_type(_XPLATSTR("AccessLevel"));
	VERIFY_IS_NOT_NULL(enum_type);
	VERIFY_ARE_EQUAL(enum_type->get_type_kind(), edm_type_kind_t::Enum);
	VERIFY_ARE_EQUAL(enum_type->get_enum_members().size(), 5);
	VERIFY_ARE_EQUAL(enum_type->is_flag(), true);
}
	
TEST(edm_model_reader_version_test)
{
	auto model_reader = ::odata::make_shared<edm_model_reader>(concurrency::streams::bytestream::open_istream(std::move(std::string(edm_model_version_test_string))));
	model_reader->parse();
	auto model = model_reader->get_model();
	VERIFY_IS_NOT_NULL(model);
	auto version = model->get_version();
	VERIFY_ARE_EQUAL(version, _XPLATSTR("4.0"));
}

}

}}}
