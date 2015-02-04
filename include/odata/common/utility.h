//---------------------------------------------------------------------
// <copyright file="utility.h" company="Microsoft">
//      Copyright (C) Microsoft Corporation. All rights reserved. See License.txt in the project root for license information.
// </copyright>
//---------------------------------------------------------------------

#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <functional>
#include <memory>
#include <vector>
#include <utility>
#include <exception>
#include <algorithm>
#include <unordered_map>
#include <functional>
#include <list>
#include <thread> 
#include <mutex>
#include <iomanip>
#include "cpprest/uri.h"
#include "cpprest/asyncrt_utils.h"
#include "cpprest/json.h"
#include "cpprest/http_client.h"
#include "odata/common/platform.h"

namespace odata { namespace common
{

ODATACPP_CLIENT_API ::utility::string_t strip_string(const ::utility::string_t& escaped);

ODATACPP_CLIENT_API void split_string(::utility::string_t& source, const ::utility::string_t& delim, std::list<::utility::string_t>& ret);

template<typename T>
void to_string(const T& input, ::utility::string_t& result)
{
	::utility::stringstream_t ostr;
	ostr << input;
	result = ostr.str();
}

ODATACPP_CLIENT_API bool is_relative_path(const ::utility::string_t& root_url, const ::utility::string_t& path);

ODATACPP_CLIENT_API ::utility::string_t print_double(const double& db, int precision = 20);
ODATACPP_CLIENT_API ::utility::string_t print_float(const float& db, int precision = 16);

}}
