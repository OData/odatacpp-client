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
	shared_ptr<edm_collection_type> collection_type = std::make_shared<edm_collection_type>(U("collection type"), int_type);
	shared_ptr<odata_collection_value> collection_value = std::make_shared<odata_collection_value>(collection_type);
	collection_value->add_collection_value(odata_primitive_value::make_primitive_value(10));
	collection_value->add_collection_value(odata_primitive_value::make_primitive_value(-10));

	VERIFY_ARE_EQUAL(edm_type_kind_t::Collection, collection_value->get_value_type()->get_type_kind());

	VERIFY_ARE_EQUAL(2, collection_value->get_collection_values().size());
}

TEST(collection_of_enum_value)
{
	shared_ptr<edm_enum_type> enum_type = make_shared<edm_enum_type>(U("Color"), U("namespace"), U("Edm.Int32"), false);
	auto collection_type = std::make_shared<edm_collection_type>(U("collection type"), enum_type);
	auto collection_value = std::make_shared<odata_collection_value>(collection_type);

	collection_value->add_collection_value(make_shared<odata_enum_value>(enum_type, U("Blue")));
	collection_value->add_collection_value(make_shared<odata_enum_value>(enum_type, U("Red")));

	VERIFY_ARE_EQUAL(edm_type_kind_t::Collection, collection_value->get_value_type()->get_type_kind());

	VERIFY_ARE_EQUAL(2, collection_value->get_collection_values().size());
}

}}}}