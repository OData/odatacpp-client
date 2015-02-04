//---------------------------------------------------------------------
// <copyright file="odata_json_constants.h" company="Microsoft">
//      Copyright (C) Microsoft Corporation. All rights reserved. See License.txt in the project root for license information.
// </copyright>
//---------------------------------------------------------------------

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