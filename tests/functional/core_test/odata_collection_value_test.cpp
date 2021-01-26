//---------------------------------------------------------------------
// <copyright file="odata_collection_value_test.cpp" company="Microsoft">
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
using namespace std;

namespace tests { namespace functional { namespace _odata {

SUITE(odata_collection_value_test_cases)
{
TEST(collection_of_primitive_value)
{
	shared_ptr<edm_primitive_type> int_type = edm_primitive_type::INT32();
	shared_ptr<edm_collection_type> collection_type = ::odata::make_shared<edm_collection_type>(_XPLATSTR("collection type"), int_type);
	shared_ptr<odata_collection_value> collection_value = ::odata::make_shared<odata_collection_value>(collection_type);
	collection_value->add_collection_value(odata_primitive_value::make_primitive_value(10));
	collection_value->add_collection_value(odata_primitive_value::make_primitive_value(-10));

	VERIFY_ARE_EQUAL(edm_type_kind_t::Collection, collection_value->get_value_type()->get_type_kind());

	VERIFY_ARE_EQUAL(2, collection_value->get_collection_values().size());
}

TEST(collection_of_enum_value)
{
	shared_ptr<edm_enum_type> enum_type = make_shared<edm_enum_type>(_XPLATSTR("Color"), _XPLATSTR("namespace"), _XPLATSTR("Edm.Int32"), false);
	auto collection_type = ::odata::make_shared<edm_collection_type>(_XPLATSTR("collection type"), enum_type);
	auto collection_value = ::odata::make_shared<odata_collection_value>(collection_type);

	collection_value->add_collection_value(make_shared<odata_enum_value>(enum_type, _XPLATSTR("Blue")));
	collection_value->add_collection_value(make_shared<odata_enum_value>(enum_type, _XPLATSTR("Red")));

	VERIFY_ARE_EQUAL(edm_type_kind_t::Collection, collection_value->get_value_type()->get_type_kind());

	VERIFY_ARE_EQUAL(2, collection_value->get_collection_values().size());
}

}}}}