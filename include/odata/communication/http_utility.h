//---------------------------------------------------------------------
// <copyright file="http_utility.h" company="Microsoft">
//      Copyright (C) Microsoft Corporation. All rights reserved. See License.txt in the project root for license information.
// </copyright>
//---------------------------------------------------------------------

#pragma once

#include <functional>
#include "cpprest/http_client.h"


namespace odata { namespace communication
{

class http_utility
{
public:
	static bool is_successful_status_code(::web::http::status_code code)
	{
		return (200 <= code) && (code < 300);
	}
};

}}
