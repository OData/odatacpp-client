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
using namespace ::odata::codegen;

namespace Microsoft_OData_SampleService_Models_TripPin
{

class City;
class Location;
class EventLocation;
class AirportLocation;

class Trip;
class Airline;
class Airport;
class Person;
class Photo;
class PlanItem;
class Event;
class PublicTransportation;
class Flight;

enum PersonGender
{
    Unknown = 2,
    Female = 1,
    Male = 0,
};

class enum_type_resolver
{
public:
    DECLARE_FUNCTION_ENUM_TYPE_FROM_STRING(PersonGender);
    DECLARE_FUNCTION_STRING_FROM_ENUM_TYPE(PersonGender);
    DECLARE_GET_ENUM_TYPE_NAMESPACE(PersonGender, Microsoft.OData.SampleService.Models.TripPin);
};

class City : public type_base
{
public:
    DECLARE_COMPLEX_CONSTRUCTOR(City);
    DECLARE_COMPLEX_DESTRUCTOR(City);
    DECLARE_EDM_INFO();
    ENABLE_PROPERTY_IN_COMPLEX_MAPPING();

    DECLARE_PRIMITIVE_PROPERTY_IN_COMPLEX_MAPPING(country, Country, ::utility::string_t);
    DECLARE_PRIMITIVE_PROPERTY_IN_COMPLEX_MAPPING(region, Region, ::utility::string_t);
    DECLARE_PRIMITIVE_PROPERTY_IN_COMPLEX_MAPPING(name, Name, ::utility::string_t);
};

class Location : public type_base
{
public:
    DECLARE_COMPLEX_CONSTRUCTOR(Location);
    DECLARE_COMPLEX_DESTRUCTOR(Location);
    DECLARE_EDM_INFO();
    ENABLE_PROPERTY_IN_COMPLEX_MAPPING();

    DECLARE_COMPLEX_PROPERTY_IN_COMPLEX_MAPPING(city, City, City);
    DECLARE_PRIMITIVE_PROPERTY_IN_COMPLEX_MAPPING(address, Address, ::utility::string_t);
};

class EventLocation : public Location
{
public:
    DECLARE_COMPLEX_CONSTRUCTOR(EventLocation);
    DECLARE_COMPLEX_DESTRUCTOR(EventLocation);
    DECLARE_EDM_INFO();
    ENABLE_PROPERTY_IN_COMPLEX_MAPPING();

    DECLARE_NULLABLE_PRIMITIVE_PROPERTY_IN_COMPLEX_MAPPING(buildinginfo, BuildingInfo, ::utility::string_t);
};

class AirportLocation : public Location
{
public:
    DECLARE_COMPLEX_CONSTRUCTOR(AirportLocation);
    DECLARE_COMPLEX_DESTRUCTOR(AirportLocation);
    DECLARE_EDM_INFO();
    ENABLE_PROPERTY_IN_COMPLEX_MAPPING();

};

class Trip : public type_base
{
public:
    DECLARE_ENTITY_CONSTRUCTOR(Trip);
    DECLARE_ENTITY_DESTRUCTOR(Trip);
    DECLARE_EDM_INFO();
    ENABLE_PROPERTY_IN_ENTITY_MAPPING();

    DECLARE_FUNCTION_P0(Trip, GetInvolvedPeople, odata_entityset_query_executor<Person>);
    DECLARE_NULLABLE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(description, Description, ::utility::string_t);
    DECLARE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(budget, Budget, float);
    DECLARE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(tripid, TripId, int32_t);
    DECLARE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(endsat, EndsAt, ::utility::datetime);
    DECLARE_NULLABLE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(shareid, ShareId, ::utility::string_t);
    DECLARE_COLLECTION_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(tags, Tags, ::utility::string_t);
    DECLARE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(name, Name, ::utility::string_t);
    DECLARE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(startsat, StartsAt, ::utility::datetime);
    DECLARE_COLLECTION_NAVIGATION_PROPERTY_IN_ENTITY_MAPPING(photos, Photos, Photo);
    DECLARE_COLLECTION_NAVIGATION_PROPERTY_IN_ENTITY_MAPPING(planitems, PlanItems, PlanItem);

    DECLARE_GET_KEY_PROPERTY_STRING_ONE_PARAM(type_base, TripId, tripid);
};

class Airline : public type_base
{
public:
    DECLARE_ENTITY_CONSTRUCTOR(Airline);
    DECLARE_ENTITY_DESTRUCTOR(Airline);
    DECLARE_EDM_INFO();
    ENABLE_PROPERTY_IN_ENTITY_MAPPING();

    DECLARE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(airlinecode, AirlineCode, ::utility::string_t);
    DECLARE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(name, Name, ::utility::string_t);

    DECLARE_GET_KEY_PROPERTY_STRING_ONE_PARAM(type_base, AirlineCode, airlinecode);
};

class Airport : public type_base
{
public:
    DECLARE_ENTITY_CONSTRUCTOR(Airport);
    DECLARE_ENTITY_DESTRUCTOR(Airport);
    DECLARE_EDM_INFO();
    ENABLE_PROPERTY_IN_ENTITY_MAPPING();

    DECLARE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(name, Name, ::utility::string_t);
    DECLARE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(icaocode, IcaoCode, ::utility::string_t);
    DECLARE_COMPLEX_PROPERTY_IN_ENTITY_MAPPING(location, Location, AirportLocation);
    DECLARE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(iatacode, IataCode, ::utility::string_t);

    DECLARE_GET_KEY_PROPERTY_STRING_ONE_PARAM(type_base, IcaoCode, icaocode);
};

class Person : public type_base
{
public:
    DECLARE_ENTITY_CONSTRUCTOR(Person);
    DECLARE_ENTITY_DESTRUCTOR(Person);
    DECLARE_EDM_INFO();
    ENABLE_PROPERTY_IN_ENTITY_MAPPING();

    DECLARE_FUNCTION_P1(Person, GetFriendsTrips, odata_entityset_query_executor<Trip>, userName, ::utility::string_t, userName);
    DECLARE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(lastname, LastName, ::utility::string_t);
    DECLARE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(username, UserName, ::utility::string_t);
    DECLARE_FUNCTION_P0(Person, GetFavoriteAirline, odata_entityset_query_executor<Airline>);
    DECLARE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(concurrency, Concurrency, int64_t);
    DECLARE_COLLECTION_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(emails, Emails, ::utility::string_t);
    DECLARE_COLLECTION_NAVIGATION_PROPERTY_IN_ENTITY_MAPPING(trips, Trips, Trip);
    DECLARE_ACTION_P2(Person, ShareTrip, odata_void_query_executor, userName, ::utility::string_t, userName, tripId, int32_t, tripId);
    DECLARE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(firstname, FirstName, ::utility::string_t);
    DECLARE_COLLECTION_COMPLEX_PROPERTY_IN_ENTITY_MAPPING(addressinfo, AddressInfo, Location);
    DECLARE_COLLECTION_NAVIGATION_PROPERTY_IN_ENTITY_MAPPING(friends, Friends, Person);
    DECLARE_NULLABLE_ENUM_PROPERTY_IN_ENTITY_MAPPING(gender, Gender, PersonGender);
    DECLARE_NAVIGATION_PROPERTY_IN_ENTITY_MAPPING(photo, Photo, Photo);

    DECLARE_GET_KEY_PROPERTY_STRING_ONE_PARAM(type_base, UserName, username);
};

class Photo : public type_base
{
public:
    DECLARE_ENTITY_CONSTRUCTOR(Photo);
    DECLARE_ENTITY_DESTRUCTOR(Photo);
    DECLARE_EDM_INFO();
    ENABLE_PROPERTY_IN_ENTITY_MAPPING();

    DECLARE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(id, Id, int64_t);
    DECLARE_NULLABLE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(name, Name, ::utility::string_t);

    DECLARE_GET_KEY_PROPERTY_STRING_ONE_PARAM(type_base, Id, id);
};

class PlanItem : public type_base
{
public:
    DECLARE_ENTITY_CONSTRUCTOR(PlanItem);
    DECLARE_ENTITY_DESTRUCTOR(PlanItem);
    DECLARE_EDM_INFO();
    ENABLE_PROPERTY_IN_ENTITY_MAPPING();

    DECLARE_NULLABLE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(endsat, EndsAt, ::utility::datetime);
    DECLARE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(planitemid, PlanItemId, int32_t);
    DECLARE_NULLABLE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(duration, Duration, ::utility::seconds);
    DECLARE_NULLABLE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(startsat, StartsAt, ::utility::datetime);
    DECLARE_NULLABLE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(confirmationcode, ConfirmationCode, ::utility::string_t);

    DECLARE_GET_KEY_PROPERTY_STRING_ONE_PARAM(type_base, PlanItemId, planitemid);
};

class Event : public PlanItem
{
public:
    DECLARE_ENTITY_CONSTRUCTOR(Event);
    DECLARE_ENTITY_DESTRUCTOR(Event);
    DECLARE_EDM_INFO();
    ENABLE_PROPERTY_IN_ENTITY_MAPPING();

    DECLARE_NULLABLE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(description, Description, ::utility::string_t);
    DECLARE_COMPLEX_PROPERTY_IN_ENTITY_MAPPING(occursat, OccursAt, EventLocation);

    DECLARE_GET_KEY_PROPERTY_STRING_NO_PARAM(PlanItem);
};

class PublicTransportation : public PlanItem
{
public:
    DECLARE_ENTITY_CONSTRUCTOR(PublicTransportation);
    DECLARE_ENTITY_DESTRUCTOR(PublicTransportation);
    DECLARE_EDM_INFO();
    ENABLE_PROPERTY_IN_ENTITY_MAPPING();

    DECLARE_NULLABLE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(seatnumber, SeatNumber, ::utility::string_t);

    DECLARE_GET_KEY_PROPERTY_STRING_NO_PARAM(PlanItem);
};

class Flight : public PublicTransportation
{
public:
    DECLARE_ENTITY_CONSTRUCTOR(Flight);
    DECLARE_ENTITY_DESTRUCTOR(Flight);
    DECLARE_EDM_INFO();
    ENABLE_PROPERTY_IN_ENTITY_MAPPING();

    DECLARE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(flightnumber, FlightNumber, ::utility::string_t);
    DECLARE_NAVIGATION_PROPERTY_IN_ENTITY_MAPPING(from, From, Airport);
    DECLARE_NAVIGATION_PROPERTY_IN_ENTITY_MAPPING(to, To, Airport);
    DECLARE_NAVIGATION_PROPERTY_IN_ENTITY_MAPPING(airline, Airline, Airline);

    DECLARE_GET_KEY_PROPERTY_STRING_NO_PARAM(PublicTransportation);
};

class DefaultContainer : public odata_service_context
{
public:
    DefaultContainer(const ::utility::string_t& baseAddress, client_options options = client_options()) : odata_service_context(baseAddress, options)
    {
    }

    DECLARE_ACTION_IMPORT_P0(DefaultContainer, ResetDataSource, odata_void_query_executor);

    DECLARE_FUNCTION_IMPORT_P2(DefaultContainer, GetNearestAirport, odata_entityset_query_executor<Airport>, lat, double, lat, lon, double, lon);

    std::shared_ptr<odata_service_query<odata_singleton_query_executor<Person>, odata_query_builder>> create_me_query()
    {
        return create_query<odata_singleton_query_executor<Person>, odata_query_builder>(U("Me"));
    }

    std::shared_ptr<odata_service_query<odata_entityset_query_executor<Photo>, odata_query_builder>> create_photos_query()
    {
        return create_query<odata_entityset_query_executor<Photo>, odata_query_builder>(U("Photos"));
    }

    std::shared_ptr<odata_service_query<odata_entityset_query_executor<Person>, odata_query_builder>> create_people_query()
    {
        return create_query<odata_entityset_query_executor<Person>, odata_query_builder>(U("People"));
    }

    std::shared_ptr<odata_service_query<odata_entityset_query_executor<Airline>, odata_query_builder>> create_airlines_query()
    {
        return create_query<odata_entityset_query_executor<Airline>, odata_query_builder>(U("Airlines"));
    }

    std::shared_ptr<odata_service_query<odata_entityset_query_executor<Airport>, odata_query_builder>> create_airports_query()
    {
        return create_query<odata_entityset_query_executor<Airport>, odata_query_builder>(U("Airports"));
    }

};

#include "odata/codegen/odata_function_param_formatter.h"

}
