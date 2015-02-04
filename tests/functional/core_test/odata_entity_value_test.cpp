//---------------------------------------------------------------------
// <copyright file="odata_entity_value_test.cpp" company="Microsoft">
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