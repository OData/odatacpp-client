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
using namespace ::odata::edm;
using namespace ::odata::codegen;

namespace tests { namespace e2e { namespace odata { namespace exchange_service {

class RecurrencePattern;
class PatternedRecurrence;
class Recipient;
class RecurrenceRange;
class Attendee;
class ItemBody;
class Location;
class ResponseStatus;

class Entity;
class Item;
class Calendar;
class Message;
class Attachment;
class User;
class Folder;
class FileAttachment;
class ItemAttachment;
class CalendarGroup;
class Event;
class Contact;
class ContactFolder;

enum BodyType
{
    HTML = 1,
    Text = 0,
};

enum RecurrencePatternType
{
    RelativeMonthly = 3,
    Weekly = 1,
    Daily = 0,
    AbsoluteMonthly = 2,
    AbsoluteYearly = 4,
    RelativeYearly = 5,
};

enum EventType
{
    SingleInstance = 0,
    SeriesMaster = 3,
    Exception = 2,
    Occurrence = 1,
};

enum Importance
{
    Normal = 0,
    Low = 1,
    High = 2,
};

enum AttendeeType
{
    Required = 0,
    Optional = 1,
    Resource = 2,
};

enum RecurrenceRangeType
{
    EndDate = 0,
    NoEnd = 1,
    Numbered = 2,
};

enum ResponseType
{
    ResponseType_None = 0,
    Accepted = 3,
    Organizer = 1,
    Declined = 4,
    TentativelyAccepted = 2,
    NotResponded = 5,
};

enum FreeBusyStatus
{
    Unknown = 0,
    Tentative = 2,
    Free = 1,
    Busy = 3,
    Oof = 4,
    WorkingElsewhere = 5,
};

enum MeetingMessageType
{
    MeetingCancelled = 2,
    None = 0,
    MeetingAccepted = 3,
    MeetingRequest = 1,
    MeetingTenativelyAccepted = 4,
    MeetingDeclined = 5,
};

enum WeekIndex
{
    Last = 4,
    Fourth = 3,
    First = 0,
    Second = 1,
    Third = 2,
};

enum DayOfWeek
{
    Thursday = 4,
    Sunday = 0,
    Monday = 1,
    Tuesday = 2,
    Wednesday = 3,
    Friday = 5,
    Saturday = 6,
};

class enum_type_resolver
{
public:
    DECLARE_FUNCTION_ENUM_TYPE_FROM_STRING(BodyType);
    DECLARE_FUNCTION_STRING_FROM_ENUM_TYPE(BodyType);
    DECLARE_GET_ENUM_TYPE_NAMESPACE(BodyType, Microsoft.Exchange.Services.OData.Model);
    DECLARE_FUNCTION_ENUM_TYPE_FROM_STRING(RecurrencePatternType);
    DECLARE_FUNCTION_STRING_FROM_ENUM_TYPE(RecurrencePatternType);
    DECLARE_GET_ENUM_TYPE_NAMESPACE(RecurrencePatternType, Microsoft.Exchange.Services.OData.Model);
    DECLARE_FUNCTION_ENUM_TYPE_FROM_STRING(EventType);
    DECLARE_FUNCTION_STRING_FROM_ENUM_TYPE(EventType);
    DECLARE_GET_ENUM_TYPE_NAMESPACE(EventType, Microsoft.Exchange.Services.OData.Model);
    DECLARE_FUNCTION_ENUM_TYPE_FROM_STRING(Importance);
    DECLARE_FUNCTION_STRING_FROM_ENUM_TYPE(Importance);
    DECLARE_GET_ENUM_TYPE_NAMESPACE(Importance, Microsoft.Exchange.Services.OData.Model);
    DECLARE_FUNCTION_ENUM_TYPE_FROM_STRING(AttendeeType);
    DECLARE_FUNCTION_STRING_FROM_ENUM_TYPE(AttendeeType);
    DECLARE_GET_ENUM_TYPE_NAMESPACE(AttendeeType, Microsoft.Exchange.Services.OData.Model);
    DECLARE_FUNCTION_ENUM_TYPE_FROM_STRING(RecurrenceRangeType);
    DECLARE_FUNCTION_STRING_FROM_ENUM_TYPE(RecurrenceRangeType);
    DECLARE_GET_ENUM_TYPE_NAMESPACE(RecurrenceRangeType, Microsoft.Exchange.Services.OData.Model);
    DECLARE_FUNCTION_ENUM_TYPE_FROM_STRING(ResponseType);
    DECLARE_FUNCTION_STRING_FROM_ENUM_TYPE(ResponseType);
    DECLARE_GET_ENUM_TYPE_NAMESPACE(ResponseType, Microsoft.Exchange.Services.OData.Model);
    DECLARE_FUNCTION_ENUM_TYPE_FROM_STRING(FreeBusyStatus);
    DECLARE_FUNCTION_STRING_FROM_ENUM_TYPE(FreeBusyStatus);
    DECLARE_GET_ENUM_TYPE_NAMESPACE(FreeBusyStatus, Microsoft.Exchange.Services.OData.Model);
    DECLARE_FUNCTION_ENUM_TYPE_FROM_STRING(MeetingMessageType);
    DECLARE_FUNCTION_STRING_FROM_ENUM_TYPE(MeetingMessageType);
    DECLARE_GET_ENUM_TYPE_NAMESPACE(MeetingMessageType, Microsoft.Exchange.Services.OData.Model);
    DECLARE_FUNCTION_ENUM_TYPE_FROM_STRING(WeekIndex);
    DECLARE_FUNCTION_STRING_FROM_ENUM_TYPE(WeekIndex);
    DECLARE_GET_ENUM_TYPE_NAMESPACE(WeekIndex, Microsoft.Exchange.Services.OData.Model);
    DECLARE_FUNCTION_ENUM_TYPE_FROM_STRING(DayOfWeek);
    DECLARE_FUNCTION_STRING_FROM_ENUM_TYPE(DayOfWeek);
    DECLARE_GET_ENUM_TYPE_NAMESPACE(DayOfWeek, System);
};

#include "odata/codegen/odata_function_param_formatter.h"

class RecurrencePattern : public type_base
{
public:
    DECLARE_COMPLEX_CONSTRUCTOR(RecurrencePattern);
    DECLARE_COMPLEX_DESTRUCTOR(RecurrencePattern);
    DECLARE_EDM_INFO();
    ENABLE_PROPERTY_IN_COMPLEX_MAPPING();

    DECLARE_NULLABLE_ENUM_PROPERTY_IN_COMPLEX_MAPPING(type, Type, RecurrencePatternType);
    DECLARE_PRIMITIVE_PROPERTY_IN_COMPLEX_MAPPING(month, Month, int32_t);
    DECLARE_PRIMITIVE_PROPERTY_IN_COMPLEX_MAPPING(interval, Interval, int32_t);
    DECLARE_PRIMITIVE_PROPERTY_IN_COMPLEX_MAPPING(dayofmonth, DayOfMonth, int32_t);
    DECLARE_COLLECTION_ENUM_PROPERTY_IN_COMPLEX_MAPPING(daysofweek, DaysOfWeek, DayOfWeek);
    DECLARE_NULLABLE_ENUM_PROPERTY_IN_COMPLEX_MAPPING(firstdayofweek, FirstDayOfWeek, DayOfWeek);
    DECLARE_NULLABLE_ENUM_PROPERTY_IN_COMPLEX_MAPPING(index, Index, WeekIndex);
};

class PatternedRecurrence : public type_base
{
public:
    DECLARE_COMPLEX_CONSTRUCTOR(PatternedRecurrence);
    DECLARE_COMPLEX_DESTRUCTOR(PatternedRecurrence);
    DECLARE_EDM_INFO();
    ENABLE_PROPERTY_IN_COMPLEX_MAPPING();

    DECLARE_NULLABLE_COMPLEX_PROPERTY_IN_COMPLEX_MAPPING(pattern, Pattern, RecurrencePattern);
    DECLARE_NULLABLE_COMPLEX_PROPERTY_IN_COMPLEX_MAPPING(range, Range, RecurrenceRange);
};

class Recipient : public type_base
{
public:
    DECLARE_COMPLEX_CONSTRUCTOR(Recipient);
    DECLARE_COMPLEX_DESTRUCTOR(Recipient);
    DECLARE_EDM_INFO();
    ENABLE_PROPERTY_IN_COMPLEX_MAPPING();

    DECLARE_NULLABLE_PRIMITIVE_PROPERTY_IN_COMPLEX_MAPPING(name, Name, ::utility::string_t);
    DECLARE_NULLABLE_PRIMITIVE_PROPERTY_IN_COMPLEX_MAPPING(address, Address, ::utility::string_t);
};

class RecurrenceRange : public type_base
{
public:
    DECLARE_COMPLEX_CONSTRUCTOR(RecurrenceRange);
    DECLARE_COMPLEX_DESTRUCTOR(RecurrenceRange);
    DECLARE_EDM_INFO();
    ENABLE_PROPERTY_IN_COMPLEX_MAPPING();

    DECLARE_NULLABLE_ENUM_PROPERTY_IN_COMPLEX_MAPPING(type, Type, RecurrenceRangeType);
    DECLARE_NULLABLE_PRIMITIVE_PROPERTY_IN_COMPLEX_MAPPING(startdate, StartDate, ::utility::datetime);
    DECLARE_NULLABLE_PRIMITIVE_PROPERTY_IN_COMPLEX_MAPPING(enddate, EndDate, ::utility::datetime);
    DECLARE_PRIMITIVE_PROPERTY_IN_COMPLEX_MAPPING(numberofoccurrences, NumberOfOccurrences, int32_t);
};

class Attendee : public Recipient
{
public:
    DECLARE_COMPLEX_CONSTRUCTOR(Attendee);
    DECLARE_COMPLEX_DESTRUCTOR(Attendee);
    DECLARE_EDM_INFO();
    ENABLE_PROPERTY_IN_COMPLEX_MAPPING();

    DECLARE_NULLABLE_COMPLEX_PROPERTY_IN_COMPLEX_MAPPING(status, Status, ResponseStatus);
    DECLARE_NULLABLE_ENUM_PROPERTY_IN_COMPLEX_MAPPING(type, Type, AttendeeType);
};

class ItemBody : public type_base
{
public:
    DECLARE_COMPLEX_CONSTRUCTOR(ItemBody);
    DECLARE_COMPLEX_DESTRUCTOR(ItemBody);
    DECLARE_EDM_INFO();
    ENABLE_PROPERTY_IN_COMPLEX_MAPPING();

    DECLARE_NULLABLE_ENUM_PROPERTY_IN_COMPLEX_MAPPING(contenttype, ContentType, BodyType);
    DECLARE_NULLABLE_PRIMITIVE_PROPERTY_IN_COMPLEX_MAPPING(content, Content, ::utility::string_t);
};

class Location : public type_base
{
public:
    DECLARE_COMPLEX_CONSTRUCTOR(Location);
    DECLARE_COMPLEX_DESTRUCTOR(Location);
    DECLARE_EDM_INFO();
    ENABLE_PROPERTY_IN_COMPLEX_MAPPING();

    DECLARE_NULLABLE_PRIMITIVE_PROPERTY_IN_COMPLEX_MAPPING(displayname, DisplayName, ::utility::string_t);
};

class ResponseStatus : public Recipient
{
public:
    DECLARE_COMPLEX_CONSTRUCTOR(ResponseStatus);
    DECLARE_COMPLEX_DESTRUCTOR(ResponseStatus);
    DECLARE_EDM_INFO();
    ENABLE_PROPERTY_IN_COMPLEX_MAPPING();

    DECLARE_NULLABLE_ENUM_PROPERTY_IN_COMPLEX_MAPPING(response, Response, ResponseType);
    DECLARE_NULLABLE_PRIMITIVE_PROPERTY_IN_COMPLEX_MAPPING(time, Time, ::utility::datetime);
};

class Entity : public type_base
{
public:
    DECLARE_ENTITY_CONSTRUCTOR(Entity);
    DECLARE_ENTITY_DESTRUCTOR(Entity);
    DECLARE_EDM_INFO();
    ENABLE_PROPERTY_IN_ENTITY_MAPPING();

    DECLARE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(id, Id, ::utility::string_t);

    DECLARE_GET_KEY_PROPERTY_STRING_ONE_PARAM(type_base, Id, id);
};

class Item : public Entity
{
public:
    DECLARE_ENTITY_CONSTRUCTOR(Item);
    DECLARE_ENTITY_DESTRUCTOR(Item);
    DECLARE_EDM_INFO();
    ENABLE_PROPERTY_IN_ENTITY_MAPPING();

    DECLARE_NULLABLE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(changekey, ChangeKey, ::utility::string_t);
    DECLARE_NULLABLE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(classname, ClassName, ::utility::string_t);
    DECLARE_NULLABLE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(subject, Subject, ::utility::string_t);
    DECLARE_NULLABLE_ENUM_PROPERTY_IN_ENTITY_MAPPING(importance, Importance, Importance);
    DECLARE_NULLABLE_COMPLEX_PROPERTY_IN_ENTITY_MAPPING(body, Body, ItemBody);
    DECLARE_NULLABLE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(hasattachments, HasAttachments, bool);
    DECLARE_NULLABLE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(bodypreview, BodyPreview, ::utility::string_t);
    DECLARE_COLLECTION_NAVIGATION_PROPERTY_IN_ENTITY_MAPPING(attachments, Attachments, Attachment);
    DECLARE_COLLECTION_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(categories, Categories, ::utility::string_t);

    DECLARE_GET_KEY_PROPERTY_STRING_NO_PARAM(Entity);
};

class Calendar : public Entity
{
public:
    DECLARE_ENTITY_CONSTRUCTOR(Calendar);
    DECLARE_ENTITY_DESTRUCTOR(Calendar);
    DECLARE_EDM_INFO();
    ENABLE_PROPERTY_IN_ENTITY_MAPPING();

    DECLARE_NULLABLE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(name, Name, ::utility::string_t);
    DECLARE_COLLECTION_NAVIGATION_PROPERTY_IN_ENTITY_MAPPING(events, Events, Event);
    DECLARE_NULLABLE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(changekey, ChangeKey, ::utility::string_t);

    DECLARE_GET_KEY_PROPERTY_STRING_NO_PARAM(Entity);
};

class Message : public Item
{
public:
    DECLARE_ENTITY_CONSTRUCTOR(Message);
    DECLARE_ENTITY_DESTRUCTOR(Message);
    DECLARE_EDM_INFO();
    ENABLE_PROPERTY_IN_ENTITY_MAPPING();

    DECLARE_NULLABLE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(parentfolderid, ParentFolderId, ::utility::string_t);
    DECLARE_NULLABLE_COMPLEX_PROPERTY_IN_ENTITY_MAPPING(from, From, Recipient);
    DECLARE_NULLABLE_COMPLEX_PROPERTY_IN_ENTITY_MAPPING(sender, Sender, Recipient);
    DECLARE_COLLECTION_COMPLEX_PROPERTY_IN_ENTITY_MAPPING(torecipients, ToRecipients, Recipient);
    DECLARE_ACTION_P1(Message, ReplyAll, odata_void_query_executor, Comment, ::utility::string_t, Comment);
    DECLARE_NULLABLE_COMPLEX_PROPERTY_IN_ENTITY_MAPPING(uniquebody, UniqueBody, ItemBody);
    DECLARE_COLLECTION_COMPLEX_PROPERTY_IN_ENTITY_MAPPING(ccrecipients, CcRecipients, Recipient);
    DECLARE_NULLABLE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(conversationid, ConversationId, ::utility::string_t);
    DECLARE_COLLECTION_COMPLEX_PROPERTY_IN_ENTITY_MAPPING(bccrecipients, BccRecipients, Recipient);
    DECLARE_ACTION_P1(Message, Move, odata_entityset_query_executor<Message>, DestinationId, ::utility::string_t, DestinationId);
    DECLARE_COLLECTION_COMPLEX_PROPERTY_IN_ENTITY_MAPPING(replyto, ReplyTo, Recipient);
    DECLARE_NULLABLE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(datetimereceived, DateTimeReceived, ::utility::datetime);
    DECLARE_NULLABLE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(lastmodifiedtime, LastModifiedTime, ::utility::datetime);
    DECLARE_ACTION_P1(Message, Copy, odata_entityset_query_executor<Message>, DestinationId, ::utility::string_t, DestinationId);
    DECLARE_NULLABLE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(datetimesent, DateTimeSent, ::utility::datetime);
    DECLARE_NULLABLE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(isdeliveryreceiptrequested, IsDeliveryReceiptRequested, bool);
    DECLARE_NULLABLE_ENUM_PROPERTY_IN_ENTITY_MAPPING(meetingmessagetype, MeetingMessageType, MeetingMessageType);
    DECLARE_NULLABLE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(isreadreceiptrequested, IsReadReceiptRequested, bool);
    DECLARE_NULLABLE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(isdraft, IsDraft, bool);
    DECLARE_NULLABLE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(isread, IsRead, bool);
    DECLARE_NULLABLE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(eventid, EventId, ::utility::string_t);
    DECLARE_NULLABLE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(datetimecreated, DateTimeCreated, ::utility::datetime);
    DECLARE_ACTION_P0(Message, CreateReply, odata_entityset_query_executor<Message>);
    DECLARE_ACTION_P0(Message, CreateForward, odata_entityset_query_executor<Message>);
    DECLARE_ACTION_P0(Message, CreateReplyAll, odata_entityset_query_executor<Message>);
    DECLARE_ACTION_P1(Message, Reply, odata_void_query_executor, Comment, ::utility::string_t, Comment);
    DECLARE_ACTION_P2(Message, Forward, odata_void_query_executor, Comment, ::utility::string_t, Comment, ToRecipients, std::vector<Recipient>, ToRecipients);
    DECLARE_ACTION_P0(Message, Send, odata_void_query_executor);

    DECLARE_GET_KEY_PROPERTY_STRING_NO_PARAM(Item);
};

class Attachment : public Entity
{
public:
    DECLARE_ENTITY_CONSTRUCTOR(Attachment);
    DECLARE_ENTITY_DESTRUCTOR(Attachment);
    DECLARE_EDM_INFO();
    ENABLE_PROPERTY_IN_ENTITY_MAPPING();

    DECLARE_NULLABLE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(name, Name, ::utility::string_t);
    DECLARE_NULLABLE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(contenttype, ContentType, ::utility::string_t);
    DECLARE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(size, Size, int32_t);
    DECLARE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(isinline, IsInline, bool);
    DECLARE_NULLABLE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(lastmodifiedtime, LastModifiedTime, ::utility::datetime);

    DECLARE_GET_KEY_PROPERTY_STRING_NO_PARAM(Entity);
};

class User : public Entity
{
public:
    DECLARE_ENTITY_CONSTRUCTOR(User);
    DECLARE_ENTITY_DESTRUCTOR(User);
    DECLARE_EDM_INFO();
    ENABLE_PROPERTY_IN_ENTITY_MAPPING();

    DECLARE_NULLABLE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(displayname, DisplayName, ::utility::string_t);
    DECLARE_COLLECTION_NAVIGATION_PROPERTY_IN_ENTITY_MAPPING(folders, Folders, Folder);
    DECLARE_NULLABLE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(alias, Alias, ::utility::string_t);
    DECLARE_NAVIGATION_PROPERTY_IN_ENTITY_MAPPING(drafts, Drafts, Folder);
    DECLARE_NULLABLE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(mailboxguid, MailboxGuid, ::utility::string_t);
    DECLARE_COLLECTION_NAVIGATION_PROPERTY_IN_ENTITY_MAPPING(messages, Messages, Message);
    DECLARE_NAVIGATION_PROPERTY_IN_ENTITY_MAPPING(rootfolder, RootFolder, Folder);
    DECLARE_NAVIGATION_PROPERTY_IN_ENTITY_MAPPING(inbox, Inbox, Folder);
    DECLARE_NAVIGATION_PROPERTY_IN_ENTITY_MAPPING(sentitems, SentItems, Folder);
    DECLARE_NAVIGATION_PROPERTY_IN_ENTITY_MAPPING(deleteditems, DeletedItems, Folder);
    DECLARE_COLLECTION_NAVIGATION_PROPERTY_IN_ENTITY_MAPPING(calendars, Calendars, Calendar);
    DECLARE_NAVIGATION_PROPERTY_IN_ENTITY_MAPPING(calendar, Calendar, Calendar);
    DECLARE_COLLECTION_NAVIGATION_PROPERTY_IN_ENTITY_MAPPING(calendargroups, CalendarGroups, CalendarGroup);
    DECLARE_COLLECTION_NAVIGATION_PROPERTY_IN_ENTITY_MAPPING(events, Events, Event);
    DECLARE_COLLECTION_NAVIGATION_PROPERTY_IN_ENTITY_MAPPING(contacts, Contacts, Contact);
    DECLARE_COLLECTION_NAVIGATION_PROPERTY_IN_ENTITY_MAPPING(contactfolders, ContactFolders, ContactFolder);

    DECLARE_GET_KEY_PROPERTY_STRING_NO_PARAM(Entity);
};

class Folder : public Entity
{
public:
    DECLARE_ENTITY_CONSTRUCTOR(Folder);
    DECLARE_ENTITY_DESTRUCTOR(Folder);
    DECLARE_EDM_INFO();
    ENABLE_PROPERTY_IN_ENTITY_MAPPING();

    DECLARE_NULLABLE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(parentfolderid, ParentFolderId, ::utility::string_t);
    DECLARE_NULLABLE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(childfoldercount, ChildFolderCount, int32_t);
    DECLARE_NULLABLE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(unreaditemcount, UnreadItemCount, int32_t);
    DECLARE_COLLECTION_NAVIGATION_PROPERTY_IN_ENTITY_MAPPING(childfolders, ChildFolders, Folder);
    DECLARE_NULLABLE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(displayname, DisplayName, ::utility::string_t);
    DECLARE_NULLABLE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(classname, ClassName, ::utility::string_t);
    DECLARE_NULLABLE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(totalcount, TotalCount, int32_t);
    DECLARE_COLLECTION_NAVIGATION_PROPERTY_IN_ENTITY_MAPPING(messages, Messages, Message);

    DECLARE_GET_KEY_PROPERTY_STRING_NO_PARAM(Entity);
};

class FileAttachment : public Attachment
{
public:
    DECLARE_ENTITY_CONSTRUCTOR(FileAttachment);
    DECLARE_ENTITY_DESTRUCTOR(FileAttachment);
    DECLARE_EDM_INFO();
    ENABLE_PROPERTY_IN_ENTITY_MAPPING();

    DECLARE_NULLABLE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(contentid, ContentId, ::utility::string_t);
    DECLARE_NULLABLE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(contentlocation, ContentLocation, ::utility::string_t);
    DECLARE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(iscontactphoto, IsContactPhoto, bool);
    DECLARE_NULLABLE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(contentbytes, ContentBytes, std::vector<unsigned char>);

    DECLARE_GET_KEY_PROPERTY_STRING_NO_PARAM(Attachment);
};

class ItemAttachment : public Attachment
{
public:
    DECLARE_ENTITY_CONSTRUCTOR(ItemAttachment);
    DECLARE_ENTITY_DESTRUCTOR(ItemAttachment);
    DECLARE_EDM_INFO();
    ENABLE_PROPERTY_IN_ENTITY_MAPPING();

    DECLARE_NAVIGATION_PROPERTY_IN_ENTITY_MAPPING(item, Item, Item);

    DECLARE_GET_KEY_PROPERTY_STRING_NO_PARAM(Attachment);
};

class CalendarGroup : public Entity
{
public:
    DECLARE_ENTITY_CONSTRUCTOR(CalendarGroup);
    DECLARE_ENTITY_DESTRUCTOR(CalendarGroup);
    DECLARE_EDM_INFO();
    ENABLE_PROPERTY_IN_ENTITY_MAPPING();

    DECLARE_NULLABLE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(name, Name, ::utility::string_t);
    DECLARE_NULLABLE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(changekey, ChangeKey, ::utility::string_t);
    DECLARE_COLLECTION_NAVIGATION_PROPERTY_IN_ENTITY_MAPPING(calendars, Calendars, Calendar);
    DECLARE_NULLABLE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(classid, ClassId, ::utility::string_t);

    DECLARE_GET_KEY_PROPERTY_STRING_NO_PARAM(Entity);
};

class Event : public Item
{
public:
    DECLARE_ENTITY_CONSTRUCTOR(Event);
    DECLARE_ENTITY_DESTRUCTOR(Event);
    DECLARE_EDM_INFO();
    ENABLE_PROPERTY_IN_ENTITY_MAPPING();

    DECLARE_NULLABLE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(start, Start, ::utility::datetime);
    DECLARE_NULLABLE_ENUM_PROPERTY_IN_ENTITY_MAPPING(type, Type, EventType);
    DECLARE_NULLABLE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(end, End, ::utility::datetime);
    DECLARE_NULLABLE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(isallday, IsAllDay, bool);
    DECLARE_NULLABLE_COMPLEX_PROPERTY_IN_ENTITY_MAPPING(location, Location, Location);
    DECLARE_NULLABLE_ENUM_PROPERTY_IN_ENTITY_MAPPING(showas, ShowAs, FreeBusyStatus);
    DECLARE_NULLABLE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(iscancelled, IsCancelled, bool);
    DECLARE_NULLABLE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(isorganizer, IsOrganizer, bool);
    DECLARE_NULLABLE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(responserequested, ResponseRequested, bool);
    DECLARE_NULLABLE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(seriesid, SeriesId, ::utility::string_t);
    DECLARE_COLLECTION_COMPLEX_PROPERTY_IN_ENTITY_MAPPING(attendees, Attendees, Attendee);
    DECLARE_NULLABLE_COMPLEX_PROPERTY_IN_ENTITY_MAPPING(recurrence, Recurrence, PatternedRecurrence);
    DECLARE_NAVIGATION_PROPERTY_IN_ENTITY_MAPPING(calendar, Calendar, Calendar);
    DECLARE_ACTION_P1(Event, Accept, odata_void_query_executor, Comment, ::utility::string_t, Comment);
    DECLARE_ACTION_P1(Event, Decline, odata_void_query_executor, Comment, ::utility::string_t, Comment);
    DECLARE_ACTION_P1(Event, TentativelyAccept, odata_void_query_executor, Comment, ::utility::string_t, Comment);

    DECLARE_GET_KEY_PROPERTY_STRING_NO_PARAM(Item);
};

class Contact : public Item
{
public:
    DECLARE_ENTITY_CONSTRUCTOR(Contact);
    DECLARE_ENTITY_DESTRUCTOR(Contact);
    DECLARE_EDM_INFO();
    ENABLE_PROPERTY_IN_ENTITY_MAPPING();

    DECLARE_NULLABLE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(parentfolderid, ParentFolderId, ::utility::string_t);
    DECLARE_NULLABLE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(birthday, Birthday, ::utility::datetime);
    DECLARE_NULLABLE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(fileas, FileAs, ::utility::string_t);
    DECLARE_NULLABLE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(givenname, GivenName, ::utility::string_t);
    DECLARE_NULLABLE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(displayname, DisplayName, ::utility::string_t);
    DECLARE_NULLABLE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(mobilephone1, MobilePhone1, ::utility::string_t);
    DECLARE_NULLABLE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(initials, Initials, ::utility::string_t);
    DECLARE_NULLABLE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(homephone1, HomePhone1, ::utility::string_t);
    DECLARE_NULLABLE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(middlename, MiddleName, ::utility::string_t);
    DECLARE_NULLABLE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(nickname, NickName, ::utility::string_t);
    DECLARE_NULLABLE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(surname, Surname, ::utility::string_t);
    DECLARE_NULLABLE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(emailaddress1, EmailAddress1, ::utility::string_t);
    DECLARE_NULLABLE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(title, Title, ::utility::string_t);
    DECLARE_NULLABLE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(imaddress2, ImAddress2, ::utility::string_t);
    DECLARE_NULLABLE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(generation, Generation, ::utility::string_t);
    DECLARE_NULLABLE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(profession, Profession, ::utility::string_t);
    DECLARE_NULLABLE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(emailaddress2, EmailAddress2, ::utility::string_t);
    DECLARE_NULLABLE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(emailaddress3, EmailAddress3, ::utility::string_t);
    DECLARE_NULLABLE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(imaddress1, ImAddress1, ::utility::string_t);
    DECLARE_NULLABLE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(imaddress3, ImAddress3, ::utility::string_t);
    DECLARE_NULLABLE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(jobtitle, JobTitle, ::utility::string_t);
    DECLARE_NULLABLE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(companyname, CompanyName, ::utility::string_t);
    DECLARE_NULLABLE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(businessphone2, BusinessPhone2, ::utility::string_t);
    DECLARE_NULLABLE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(department, Department, ::utility::string_t);
    DECLARE_NULLABLE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(officelocation, OfficeLocation, ::utility::string_t);
    DECLARE_NULLABLE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(businesshomepage, BusinessHomePage, ::utility::string_t);
    DECLARE_NULLABLE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(assistantname, AssistantName, ::utility::string_t);
    DECLARE_NULLABLE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(manager, Manager, ::utility::string_t);
    DECLARE_NULLABLE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(homephone2, HomePhone2, ::utility::string_t);
    DECLARE_NULLABLE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(businessphone1, BusinessPhone1, ::utility::string_t);
    DECLARE_NULLABLE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(otherphone, OtherPhone, ::utility::string_t);
    DECLARE_NULLABLE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(datetimecreated, DateTimeCreated, ::utility::datetime);
    DECLARE_NULLABLE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(lastmodifiedtime, LastModifiedTime, ::utility::datetime);

    DECLARE_GET_KEY_PROPERTY_STRING_NO_PARAM(Item);
};

class ContactFolder : public Entity
{
public:
    DECLARE_ENTITY_CONSTRUCTOR(ContactFolder);
    DECLARE_ENTITY_DESTRUCTOR(ContactFolder);
    DECLARE_EDM_INFO();
    ENABLE_PROPERTY_IN_ENTITY_MAPPING();

    DECLARE_NULLABLE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(parentfolderid, ParentFolderId, ::utility::string_t);
    DECLARE_COLLECTION_NAVIGATION_PROPERTY_IN_ENTITY_MAPPING(childfolders, ChildFolders, ContactFolder);
    DECLARE_NULLABLE_PRIMITIVE_PROPERTY_IN_ENTITY_MAPPING(displayname, DisplayName, ::utility::string_t);
    DECLARE_COLLECTION_NAVIGATION_PROPERTY_IN_ENTITY_MAPPING(contacts, Contacts, Contact);

    DECLARE_GET_KEY_PROPERTY_STRING_NO_PARAM(Entity);
};

class EntityContainer : public odata_service_context
{
public:
    EntityContainer(const ::utility::string_t& baseAddress, client_options options = client_options()) : odata_service_context(baseAddress, options)
    {
    }

    std::shared_ptr<odata_service_query<odata_entityset_query_executor<User>, odata_query_builder>> create_users_query()
    {
        return create_query<odata_entityset_query_executor<User>, odata_query_builder>(U("Users"));
    }

    std::shared_ptr<odata_service_query<odata_singleton_query_executor<User>, odata_query_builder>> create_me_query()
    {
        return create_query<odata_singleton_query_executor<User>, odata_query_builder>(U("Me"));
    }

};

}}}}