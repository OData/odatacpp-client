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
	auto service_context = ::odata::make_shared<DefaultContainer>(_XPLATSTR("http://services.odata.org/V4/(S(khax213ynjvk4pqdhdgcadvw))/TripPinServiceRW/"));
	auto people = service_context->create_people_query()->execute_query().get().first;
}

void RequestIndividualResource()
{
	auto service_context = ::odata::make_shared<DefaultContainer>(_XPLATSTR("http://services.odata.org/V4/(S(khax213ynjvk4pqdhdgcadvw))/TripPinServiceRW/"));
	auto people = service_context->create_people_query()->key(_XPLATSTR("'russellwhyte'"))->execute_query().get().first;
}

void Queries()
{
	auto service_context = ::odata::make_shared<DefaultContainer>(_XPLATSTR("http://services.odata.org/V4/(S(khax213ynjvk4pqdhdgcadvw))/TripPinServiceRW/"));
	auto people = service_context->create_people_query()->filter(_XPLATSTR("Trips/any(d:d/Budget gt 3000)"))->select(_XPLATSTR("FirstName, LastName"))->top(2)->execute_query().get().first;
}

void CreateNewResource()
{
    auto service_context = ::odata::make_shared<DefaultContainer>(_XPLATSTR("http://services.odata.org/V4/(S(khax213ynjvk4pqdhdgcadvw))/TripPinServiceRW/"));
    auto lewis = ::odata::make_shared<Person>(service_context);
    lewis->set_username(_XPLATSTR("lewisblack"));
    lewis->set_firstname(_XPLATSTR("Lewis"));
    lewis->set_lastname(_XPLATSTR("Black"));
    lewis->set_emails({ _XPLATSTR("lewisblack@example.com") });
    auto location = ::odata::make_shared<Location>(service_context);
    location->set_address(_XPLATSTR("187 Suffolk Ln."));
    auto city = ::odata::make_shared<City>(service_context);
    city->set_countryregion(_XPLATSTR("United States"));
    city->set_name(_XPLATSTR("Boise"));
    city->set_region(_XPLATSTR("ID"));
    location->set_city(city);
    lewis->set_addressinfo({ location });
    lewis->set_gender(PersonGender::Male);
    lewis->set_concurrency(635519729375200400);
    service_context->add_object(_XPLATSTR("People"), lewis).get().first;
}

void RelateResource()
{
    auto service_context = ::odata::make_shared<DefaultContainer>(_XPLATSTR("http://services.odata.org/V4/(S(khax213ynjvk4pqdhdgcadvw))/TripPinServiceRW/"));
    auto ronald = service_context->create_people_query()->key(_XPLATSTR("'ronaldmundy'"))->execute_query().get().first[0];
    auto russell = service_context->create_people_query()->key(_XPLATSTR("'russellwhyte'"))->expand(_XPLATSTR("Trips"))->execute_query().get().first[0];
    auto trip = russell->get_trips()[0];
    service_context->add_reference(ronald, _XPLATSTR("Trips"), trip).get().first;
}

void InvokeFunction()
{
    auto service_context = ::odata::make_shared<DefaultContainer>(_XPLATSTR("http://services.odata.org/V4/(S(khax213ynjvk4pqdhdgcadvw))/TripPinServiceRW/"));
    auto russell = service_context->create_people_query()->key(_XPLATSTR("'russellwhyte'"))->expand(_XPLATSTR("Trips"))->execute_query().get().first[0];
    auto trip = russell->get_trips()[0];
    auto people = trip->GetInvolvedPeople().get().first;
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
