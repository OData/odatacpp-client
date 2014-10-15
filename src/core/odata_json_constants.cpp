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
 
#include "odata/core/odata_json_constants.h"

namespace odata { namespace core
{
    const ::utility::string_t odata_json_constants::PAYLOAD_ANNOTATION_NAVIGATIONLINK = U("@odata.navigationLink");
    const ::utility::string_t odata_json_constants::PAYLOAD_ANNOTATION_READLINK(U("@odata.readLink"));
    const ::utility::string_t odata_json_constants::PAYLOAD_ANNOTATION_EDITLINK(U("@odata.editLink"));
    const ::utility::string_t odata_json_constants::PAYLOAD_ANNOTATION_TYPE(U("@odata.type"));
    const ::utility::string_t odata_json_constants::PAYLOAD_ANNOTATION_ID(U("@odata.id"));
}}