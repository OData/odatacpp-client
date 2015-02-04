//---------------------------------------------------------------------
// <copyright file="trippin_sample.cpp" company="Microsoft">
//      Copyright (C) Microsoft Corporation. All rights reserved. See License.txt in the project root for license information.
// </copyright>
//---------------------------------------------------------------------

#include <iostream>
#include "odata/client/odata_client.h"
#include "trippin_service.h"
#include "odata/codegen/odata_query_path_composer.h"

using namespace ::std;
using namespace ::odata::client;
using namespace Microsoft_OData_SampleService_Models_TripPin;

void RequestResources()
{
	auto service_context = std::make_shared<DefaultContainer>(U("http://services.odata.org/V4/(S(khax213ynjvk4pqdhdgcadvw))/TripPinServiceRW/"));
	auto people = service_context->create_people_query()->execute_query().get();
}

void RequestIndividualResource()
{
	auto service_context = std::make_shared<DefaultContainer>(U("http://services.odata.org/V4/(S(khax213ynjvk4pqdhdgcadvw))/TripPinServiceRW/"));
	auto people = service_context->create_people_query()->key(U("'russellwhyte'"))->execute_query().get();
}

void Queries()
{
	auto service_context = std::make_shared<DefaultContainer>(U("http://services.odata.org/V4/(S(khax213ynjvk4pqdhdgcadvw))/TripPinServiceRW/"));
	auto people = service_context->create_people_query()->filter(U("Trips/any(d:d/Budget gt 3000)"))->select(U("FirstName, LastName"))->top(2)->execute_query().get();
}

void CreateNewResource()
{
    auto service_context = std::make_shared<DefaultContainer>(U("http://services.odata.org/V4/(S(khax213ynjvk4pqdhdgcadvw))/TripPinServiceRW/"));
    auto lewis = std::make_shared<Person>(service_context);
    lewis->set_username(U("lewisblack"));
    lewis->set_firstname(U("Lewis"));
    lewis->set_lastname(U("Black"));
    lewis->set_emails({ U("lewisblack@example.com") });
    auto location = std::make_shared<Location>(service_context);
    location->set_address(U("187 Suffolk Ln."));
    auto city = std::make_shared<City>(service_context);
    city->set_countryregion(U("United States"));
    city->set_name(U("Boise"));
    city->set_region(U("ID"));
    location->set_city(city);
    lewis->set_addressinfo({ location });
    lewis->set_gender(PersonGender::Male);
    lewis->set_concurrency(635519729375200400);
    service_context->add_object(U("People"), lewis).get();
}

void RelateResource()
{
    auto service_context = std::make_shared<DefaultContainer>(U("http://services.odata.org/V4/(S(khax213ynjvk4pqdhdgcadvw))/TripPinServiceRW/"));
    auto ronald = service_context->create_people_query()->key(U("'ronaldmundy'"))->execute_query().get()[0];
    auto russell = service_context->create_people_query()->key(U("'russellwhyte'"))->expand(U("Trips"))->execute_query().get()[0];
    auto trip = russell->get_trips()[0];
    service_context->add_reference(ronald, U("Trips"), trip).get();
}

void InvokeFunction()
{
    auto service_context = std::make_shared<DefaultContainer>(U("http://services.odata.org/V4/(S(khax213ynjvk4pqdhdgcadvw))/TripPinServiceRW/"));
    auto russell = service_context->create_people_query()->key(U("'russellwhyte'"))->expand(U("Trips"))->execute_query().get()[0];
    auto trip = russell->get_trips()[0];
    auto people = trip->GetInvolvedPeople().get();
}

int main()
{
    RequestResources();
    RequestIndividualResource();
    Queries();
    CreateNewResource();
    RelateResource();
    InvokeFunction();

    return 0;
}
