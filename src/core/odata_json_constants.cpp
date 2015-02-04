//---------------------------------------------------------------------
// <copyright file="odata_json_constants.cpp" company="Microsoft">
//      Copyright (C) Microsoft Corporation. All rights reserved. See License.txt in the project root for license information.
// </copyright>
//---------------------------------------------------------------------

#include "odata/core/odata_json_constants.h"

namespace odata { namespace core
{
    const ::utility::string_t odata_json_constants::PAYLOAD_ANNOTATION_NAVIGATIONLINK = U("@odata.navigationLink");
    const ::utility::string_t odata_json_constants::PAYLOAD_ANNOTATION_READLINK(U("@odata.readLink"));
    const ::utility::string_t odata_json_constants::PAYLOAD_ANNOTATION_EDITLINK(U("@odata.editLink"));
    const ::utility::string_t odata_json_constants::PAYLOAD_ANNOTATION_TYPE(U("@odata.type"));
    const ::utility::string_t odata_json_constants::PAYLOAD_ANNOTATION_ID(U("@odata.id"));
}}