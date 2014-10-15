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

