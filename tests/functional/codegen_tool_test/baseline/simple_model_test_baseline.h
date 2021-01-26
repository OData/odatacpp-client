//---------------------------------------------------------------------
// <copyright file="simple_model_test_baseline.h" company="Microsoft">
//      Copyright (C) Microsoft Corporation. All rights reserved. See License.txt in the project root for license information.
// </copyright>
//---------------------------------------------------------------------

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

namespace Simple
{


class TestType;

class enum_type_resolver
{
public:
};

class TestType : public type_base
{
public:
    DECLARE_ENTITY_CONSTRUCTOR(TestType);
    DECLARE_ENTITY_DESTRUCTOR(TestType);
    DECLARE_EDM_INFO();
    ENABLE_PROPERTY_IN_ENTITY_MAPPING();

    DECLARE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(keyprop, KeyProp, int32_t);
    DECLARE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(valueprop, ValueProp, ::odata::string_t);

    DECLARE_GET_KEY_PROPERTY_STRING_ONE_PARAM(type_base, KeyProp, keyprop);
};

class DefaultContainer : public odata_service_context
{
public:
    DefaultContainer(const ::odata::string_t& baseAddress, client_options options = client_options()) : odata_service_context(baseAddress, options)
    {
    }

    std::shared_ptr<odata_service_query<odata_entityset_query_executor<TestType>, odata_query_builder>> create_testtypeset_query()
    {
        return create_query<odata_entityset_query_executor<TestType>, odata_query_builder>(_XPLATSTR("TestTypeSet"));
    }

};

#include "odata/codegen/odata_function_param_formatter.h"

}
