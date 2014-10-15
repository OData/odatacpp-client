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

namespace tests { namespace functional { namespace _odata {

SUITE(odata_entity_value_test_cases)
{
TEST(get_entity_key_string)
{
	auto model = get_test_model();
	VERIFY_IS_NOT_NULL(model);

	auto entity_value = std::make_shared<odata_entity_value>(model->find_entity_type(U("ProductReview")));
	entity_value->set_value(U("ProductID"), 1);
	entity_value->set_value(U("ProductDetailID"), 2);
    entity_value->set_value(U("ReviewTitle"), U("title"));
    entity_value->set_value(U("RevisionID"), 3);

    auto key =  entity_value->get_entity_key_string();
    VERIFY_ARE_EQUAL(U("(ProductID=1,ProductDetailID=2,ReviewTitle='title',RevisionID=3)"), key);
}
}

}}}