/***
* ==++==
*
* Copyright (c) Microsoft Corporation. All rights reserved. 
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
* http://www.apache.org/licenses/LICENSE-2.0
* 
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*
* ==--==
* =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
*
* e2e_test_case.h
*
*
* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
****/

#pragma once
#include "odata/client/odata_client.h"
#include "odata_wcf_service.h"

namespace tests { namespace e2e { namespace odata {

using namespace ::odata::client;
using namespace ::utility;
using namespace odata_wcf_service;

class e2e_raw_client
{
public:
	odata_client client;

    e2e_raw_client() : client(U("http://odatae2etest.azurewebsites.net/cpptest/DefaultService")) {
		//Reset data source first.
		client.send_data_to_server(U("ResetDataSource")).get();
	}
};


class e2e_test_case
{
public:
	std::shared_ptr<InMemoryEntities> service_context;

    e2e_test_case() {
		service_context = std::make_shared<InMemoryEntities>(U("http://odatae2etest.azurewebsites.net/cpptest/DefaultService"));

		//Reset data source first.
		service_context->get_client()->send_data_to_server(U("ResetDataSource")).get(); //TODO: [tiano] use API instead of odata_client
	}
};

}}}