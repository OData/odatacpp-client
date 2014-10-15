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

#include "cpprest/basic_types.h"
#include "odata/common/platform.h"

namespace odata { namespace core
{
class odata_json_constants
{
public:
    ODATACPP_CLIENT_API static const ::utility::string_t PAYLOAD_ANNOTATION_NAVIGATIONLINK;
    ODATACPP_CLIENT_API static const ::utility::string_t PAYLOAD_ANNOTATION_READLINK;
    ODATACPP_CLIENT_API static const ::utility::string_t PAYLOAD_ANNOTATION_EDITLINK;
    ODATACPP_CLIENT_API static const ::utility::string_t PAYLOAD_ANNOTATION_TYPE;
    ODATACPP_CLIENT_API static const ::utility::string_t PAYLOAD_ANNOTATION_ID;
};

}}