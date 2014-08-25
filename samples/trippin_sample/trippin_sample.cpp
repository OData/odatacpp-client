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
 
 // ExchangeSample.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include "odata/client/odata_client.h"
#include "trippin_service.h"
#include "odata/codegen/odata_query_path_composer.h"

using namespace ::std;
using namespace ::odata::client;
using namespace Microsoft_OData_SampleService_Models_TripPin;

const ::utility::string_t trippin_service_root(U("http://services.odata.org/V4/(S(khax213ynjvk4pqdhdgcadvw))/TripPinServiceRW/"));

int main()
{
    auto service_context = std::make_shared<DefaultContainer>(trippin_service_root);
    auto airlines = service_context->create_airlines_query()->execute_query().get();
    for(auto iter = airlines.cbegin(); iter != airlines.cend(); iter++)
    {
        wcout << (*iter)->get_name() << endl;
    }

    return 0;
}

