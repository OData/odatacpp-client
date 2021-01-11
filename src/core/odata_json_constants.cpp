//---------------------------------------------------------------------
// <copyright file="odata_json_constants.cpp" company="Microsoft">
//      Copyright (C) Microsoft Corporation. All rights reserved. See License.txt in the project root for license information.
// </copyright>
//---------------------------------------------------------------------

#include "odata/core/odata_json_constants.h"
#include "odata/common/utility.h"

namespace odata { namespace core
{
	const ::odata::string_t odata_json_constants::PAYLOAD_ANNOTATION_NAVIGATIONLINK = _XPLATSTR("@odata.navigationLink");
	const ::odata::string_t odata_json_constants::PAYLOAD_ANNOTATION_READLINK(_XPLATSTR("@odata.readLink"));
	const ::odata::string_t odata_json_constants::PAYLOAD_ANNOTATION_EDITLINK(_XPLATSTR("@odata.editLink"));
	const ::odata::string_t odata_json_constants::PAYLOAD_ANNOTATION_TYPE(_XPLATSTR("@odata.type"));
	const ::odata::string_t odata_json_constants::PAYLOAD_ANNOTATION_ID(_XPLATSTR("@odata.id"));
	const ::odata::string_t odata_json_constants::PAYLOAD_ANNOTATION_BIND(_XPLATSTR("@odata.bind"));
}}
