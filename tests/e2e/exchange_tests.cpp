//---------------------------------------------------------------------
// <copyright file="exchange_tests.cpp" company="Microsoft">
//      Copyright (C) Microsoft Corporation. All rights reserved. See License.txt in the project root for license information.
// </copyright>
//---------------------------------------------------------------------

#include "e2e_tests.h"
#include "exchange_service.h"

namespace tests { namespace e2e { namespace odata {

using namespace exchange_service;

class exchange_test_case
{
public:
	std::shared_ptr<EntityContainer> service_context;

	exchange_test_case() {
		service_context = ::odata::make_shared<EntityContainer>(_XPLATSTR("http://odatae2etest.azurewebsites.net/cpptemp/DefaultService/"));
	}
};

SUITE(exchange_tests)
{

TEST_FIXTURE(exchange_test_case, query_users)
{
	//auto users = service_context->create_users_query()->execute_query().get();

	//VERIFY_IS_TRUE(users.size() > 0);
}


}

}}}
