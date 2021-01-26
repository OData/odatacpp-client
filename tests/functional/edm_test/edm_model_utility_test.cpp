//---------------------------------------------------------------------
// <copyright file="edm_model_utility_test.cpp" company="Microsoft">
//      Copyright (C) Microsoft Corporation. All rights reserved. See License.txt in the project root for license information.
// </copyright>
//---------------------------------------------------------------------

#include "../odata_tests.h"
#include "cpprest/json.h"
#include "odata/edm/odata_edm.h"
#include "odata/edm/edm_model_utility.h"

using namespace ::odata::client;
using namespace ::odata::edm;

namespace tests { namespace functional { namespace _odata {

struct edm_type_comparision_object
{
	::odata::string_t primitive_name;
	edm_primitive_type_kind_t expected_primitive_type;
};

#define edm_primitive_type_test_contents_size 15
static edm_type_comparision_object edm_primitive_type_test_contents[edm_primitive_type_test_contents_size] =
{
	{_XPLATSTR("Edm.Binary"), edm_primitive_type_kind_t::Binary},
	{_XPLATSTR("Edm.Boolean"), edm_primitive_type_kind_t::Boolean},
	{_XPLATSTR("Edm.Byte"), edm_primitive_type_kind_t::Byte},
	{_XPLATSTR("Edm.Duration"), edm_primitive_type_kind_t::Duration},
	{_XPLATSTR("Edm.DateTimeOffset"), edm_primitive_type_kind_t::DateTimeOffset},
	{_XPLATSTR("Edm.Double"), edm_primitive_type_kind_t::Double},
	{_XPLATSTR("Edm.Decimal"), edm_primitive_type_kind_t::Decimal},
	{_XPLATSTR("Edm.Guid"), edm_primitive_type_kind_t::Guid},
	{_XPLATSTR("Edm.Int16"), edm_primitive_type_kind_t::Int16},
	{_XPLATSTR("Edm.Int32"), edm_primitive_type_kind_t::Int32},
	{_XPLATSTR("Edm.Int64"), edm_primitive_type_kind_t::Int64},
	{_XPLATSTR("Edm.SByte"), edm_primitive_type_kind_t::SByte},
	{_XPLATSTR("Edm.Single"), edm_primitive_type_kind_t::Single},
	{_XPLATSTR("Edm.Stream"), edm_primitive_type_kind_t::Stream},
	{_XPLATSTR("Edm.String"), edm_primitive_type_kind_t::String},
};

SUITE(odata_edm_model_utility_tests)
{

TEST(get_edm_primitive_type_from_name)
{
	for (int i = 0; i < edm_primitive_type_test_contents_size; i++)
	{
	    auto return_type = edm_model_utility::get_edm_primitive_type_from_name(edm_primitive_type_test_contents[i].primitive_name);
	    VERIFY_IS_NOT_NULL(return_type);
	    VERIFY_ARE_EQUAL(return_type->get_type_kind(), edm_type_kind_t::Primitive);
	    auto primitive_type = std::dynamic_pointer_cast<edm_primitive_type>(return_type);
	    VERIFY_IS_NOT_NULL(primitive_type);
	    VERIFY_ARE_EQUAL(primitive_type->get_primitive_kind(), edm_primitive_type_test_contents[i].expected_primitive_type);
	}
}

TEST(get_edm_primitive_type_from_name_nagative_case)
{
	auto return_type = edm_model_utility::get_edm_primitive_type_from_name(_XPLATSTR(""));
	VERIFY_IS_NULL(return_type);
    return_type = edm_model_utility::get_edm_primitive_type_from_name(_XPLATSTR("Edm."));
	VERIFY_IS_NULL(return_type);
    return_type = edm_model_utility::get_edm_primitive_type_from_name(_XPLATSTR("Edm.Nothing"));
	VERIFY_IS_NULL(return_type);
}

struct get_edm_type_from_name_test_object
{
	::odata::string_t input_name;
	edm_type_kind_t expected_edm_type;
	edm_primitive_type_kind_t expected_primitive_type;
};

#define get_edm_type_from_name_test_size 15
static get_edm_type_from_name_test_object get_edm_type_from_name_test_content[get_edm_type_from_name_test_size] =
{
	{_XPLATSTR("Collection(Edm.Binary)"), edm_type_kind_t::Collection, edm_primitive_type_kind_t::Binary},
	{_XPLATSTR("Collection(Edm.Boolean)"), edm_type_kind_t::Collection, edm_primitive_type_kind_t::Boolean},
	{_XPLATSTR("Collection(Edm.Byte)"), edm_type_kind_t::Collection, edm_primitive_type_kind_t::Byte},
	{_XPLATSTR("Collection(Edm.Duration)"), edm_type_kind_t::Collection, edm_primitive_type_kind_t::Duration},
	{_XPLATSTR("Collection(Edm.DateTimeOffset)"), edm_type_kind_t::Collection, edm_primitive_type_kind_t::DateTimeOffset},
	{_XPLATSTR("Collection(Edm.Double)"), edm_type_kind_t::Collection, edm_primitive_type_kind_t::Double},
	{_XPLATSTR("Collection(Edm.Decimal)"), edm_type_kind_t::Collection, edm_primitive_type_kind_t::Decimal},
	{_XPLATSTR("Collection(Edm.Guid)"), edm_type_kind_t::Collection, edm_primitive_type_kind_t::Guid},
	{_XPLATSTR("Collection(Edm.Int16)"), edm_type_kind_t::Collection, edm_primitive_type_kind_t::Int16},
	{_XPLATSTR("Collection(Edm.Int32)"), edm_type_kind_t::Collection, edm_primitive_type_kind_t::Int32},
	{_XPLATSTR("Collection(Edm.Int64)"), edm_type_kind_t::Collection, edm_primitive_type_kind_t::Int64},
	{_XPLATSTR("Collection(Edm.SByte)"), edm_type_kind_t::Collection, edm_primitive_type_kind_t::SByte},
	{_XPLATSTR("Collection(Edm.Single)"), edm_type_kind_t::Collection, edm_primitive_type_kind_t::Single},
	{_XPLATSTR("Collection(Edm.Stream)"), edm_type_kind_t::Collection, edm_primitive_type_kind_t::Stream},
	{_XPLATSTR("Collection(Edm.String)"), edm_type_kind_t::Collection, edm_primitive_type_kind_t::String},
};

TEST(get_edm_type_from_name_collection_primitive_case)
{
	for (int i = 0; i < get_edm_type_from_name_test_size; i++)
	{
		auto return_type = edm_model_utility::get_edm_type_from_name(get_edm_type_from_name_test_content[i].input_name);
	    VERIFY_IS_NOT_NULL(return_type);
		VERIFY_ARE_EQUAL(return_type->get_type_kind(), get_edm_type_from_name_test_content[i].expected_edm_type);
		auto collection_type = std::dynamic_pointer_cast<edm_collection_type>(return_type);
	    VERIFY_IS_NOT_NULL(collection_type);
		auto element_type = collection_type->get_element_type();
	    VERIFY_IS_NOT_NULL(element_type);
		auto primitive_type = std::dynamic_pointer_cast<edm_primitive_type>(element_type);
	    VERIFY_ARE_EQUAL(primitive_type->get_primitive_kind(), get_edm_type_from_name_test_content[i].expected_primitive_type);
	}
}

TEST(get_edm_type_from_name_collection_undetermined_case)
{
    auto return_type = edm_model_utility::get_edm_type_from_name(_XPLATSTR("Collection(OData.Test)"));
	VERIFY_IS_NOT_NULL(return_type);
	VERIFY_ARE_EQUAL(return_type->get_type_kind(), edm_type_kind_t::Collection);
	auto collection_type = std::dynamic_pointer_cast<edm_collection_type>(return_type);
	VERIFY_IS_NOT_NULL(collection_type);
	auto element_type = collection_type->get_element_type();
	VERIFY_IS_NULL(element_type);
}

TEST(get_edm_type_from_name_nagative_case)
{
    auto return_type = edm_model_utility::get_edm_type_from_name(_XPLATSTR("OData.Test"));
	VERIFY_IS_NULL(return_type);
}

TEST(get_primitive_kind_from_edm_type)
{
	auto primitive_type = edm_primitive_type::BINARY();
	auto complex_type = ::odata::make_shared<edm_complex_type>(_XPLATSTR(""), _XPLATSTR(""));
	auto entity_type = ::odata::make_shared<edm_entity_type>(_XPLATSTR(""), _XPLATSTR(""));

	edm_primitive_type_kind_t out_put;
	bool ok = edm_model_utility::get_primitive_kind_from_edm_type(primitive_type, out_put);
	VERIFY_ARE_EQUAL(ok, true);
	VERIFY_ARE_EQUAL(out_put, edm_primitive_type_kind_t::Binary);

	ok = edm_model_utility::get_primitive_kind_from_edm_type(complex_type, out_put);
	VERIFY_ARE_EQUAL(ok, false);

	ok = edm_model_utility::get_primitive_kind_from_edm_type(entity_type, out_put);
	VERIFY_ARE_EQUAL(ok, false);
}

TEST(resolve_edm_types_after_parsing_type_in_entity)
{
	::odata::string_t name_space = _XPLATSTR("odata.functional.test");
	auto model = ::odata::make_shared<edm_model>();
	auto schema = model->add_schema(_XPLATSTR("schema"), name_space);
	auto complex_address = ::odata::make_shared<edm_complex_type>(_XPLATSTR("complex_address"), name_space);
	auto entity_account = ::odata::make_shared<edm_entity_type>(_XPLATSTR("entity_account"), name_space);
	auto property_address = ::odata::make_shared<edm_property_type>(_XPLATSTR("Address"));
	property_address->set_property_type(::odata::make_shared<edm_named_type>(_XPLATSTR("complex_address"), _XPLATSTR(""), edm_type_kind_t::Unknown));
	entity_account->add_property(property_address);
	schema->add_complex_type(complex_address);
	schema->add_entity_type(entity_account);
	edm_model_utility::resolve_edm_types_after_parsing(model);

	auto prop = entity_account->find_property(_XPLATSTR("Address"));
	VERIFY_IS_NOT_NULL(prop);
	auto prop_type = prop->get_property_type();
	VERIFY_IS_NOT_NULL(prop_type);
	VERIFY_ARE_EQUAL(prop_type->get_type_kind(), edm_type_kind_t::Complex);
}

TEST(resolve_edm_types_after_parsing_type_in_complex)
{
	::odata::string_t name_space = _XPLATSTR("odata.functional.test");
	auto model = ::odata::make_shared<edm_model>();
	auto schema = model->add_schema(_XPLATSTR("schema"), name_space);
	auto complex_address = ::odata::make_shared<edm_complex_type>(_XPLATSTR("complex_address"), name_space);
	auto complex_account = ::odata::make_shared<edm_complex_type>(_XPLATSTR("complex_account"), name_space);
	auto property_address = ::odata::make_shared<edm_property_type>(_XPLATSTR("Address"));
	property_address->set_property_type(::odata::make_shared<edm_named_type>(_XPLATSTR("complex_address"), _XPLATSTR(""), edm_type_kind_t::Unknown));
	complex_account->add_property(property_address);
	schema->add_complex_type(complex_address);
	schema->add_complex_type(complex_account);
	edm_model_utility::resolve_edm_types_after_parsing(model);

	auto prop = complex_account->find_property(_XPLATSTR("Address"));
	VERIFY_IS_NOT_NULL(prop);
	auto prop_type = prop->get_property_type();
	VERIFY_IS_NOT_NULL(prop_type);
	VERIFY_ARE_EQUAL(prop_type->get_type_kind(), edm_type_kind_t::Complex);
}

TEST(resolve_edm_types_after_parsing_type_in_operation)
{
	::odata::string_t name_space = _XPLATSTR("odata.functional.test");
	auto model = ::odata::make_shared<edm_model>();
	auto schema = model->add_schema(_XPLATSTR("schema"), name_space);
	std::shared_ptr<edm_operation_type>  operation;
	operation.reset(new edm_operation_type(_XPLATSTR("operation"), name_space, false, _XPLATSTR(""), EdmOperationKind::Function, false));
	auto complex_address = ::odata::make_shared<edm_complex_type>(_XPLATSTR("complex_address"), name_space);
	auto complex_account = ::odata::make_shared<edm_complex_type>(_XPLATSTR("complex_account"), name_space);
	operation->set_return_type(::odata::make_shared<edm_named_type>(_XPLATSTR("complex_address"), _XPLATSTR(""), edm_type_kind_t::Unknown));
	auto param = ::odata::make_shared<edm_operation_parameter>();
	param->set_param_type(::odata::make_shared<edm_named_type>(_XPLATSTR("complex_account"), _XPLATSTR(""), edm_type_kind_t::Unknown));
	operation->add_operation_parameter(param);
	schema->add_complex_type(complex_address);
	schema->add_complex_type(complex_account);
	schema->add_operation_type(operation);
	edm_model_utility::resolve_edm_types_after_parsing(model);

	auto verify_return_type = operation->get_operation_return_type();
	VERIFY_IS_NOT_NULL(verify_return_type);
	VERIFY_ARE_EQUAL(verify_return_type->get_type_kind(), edm_type_kind_t::Complex);
	auto verify_param = operation->get_operation_parameters()[0];
	VERIFY_IS_NOT_NULL(verify_param);
	VERIFY_ARE_EQUAL(verify_param->get_param_type()->get_type_kind(), edm_type_kind_t::Complex);
}

}

}}}