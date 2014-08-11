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
 
 ﻿#include "odata_codegen_initializer.h"
#include "odata_codegen_writer.h"
#include "odata/client/odata_client.h"
#include "cpprest/uri.h"
#include "cpprest/asyncrt_utils.h"
#include "cpprest/json.h"
#include "cpprest/http_client.h"
#include "cpprest/containerstream.h"
#include <iostream>
#include <chrono>
#include <string>

using namespace ::odata::codegen::tools;
using namespace concurrency;
using namespace concurrency::streams;
using namespace web::http;
using namespace web::http::client;
using namespace ::pplx;
using namespace ::web::http::client;
using namespace ::web::http;

#ifdef WIN32
int _tmain(int argc, ::utility::char_t* argv[])
#else
int main(int argc, ::utility::char_t* argv[])
#endif
{
	if (argc != 3 && argc != 5)
	{
		std::cout << "cmd argument error";
		return -1;
	}

	::utility::string_t metadata_url = *++argv;
	::utility::string_t file_name = *++argv;
	
	::utility::string_t user_name;
	::utility::string_t password;
	if (argc > 3)
	{
		user_name = *++argv;
		password = *++argv;
	}

	auto initializer = std::make_shared<odata_codegen_initializer>();
	initializer->begin_initialize(metadata_url, file_name, user_name, password).then(
	   [initializer] (int) -> int
	   {
		   auto writer = std::make_shared<odata_codegen_writer>(initializer);
		   return writer->begin_generate_file().wait();
	   }).wait();

	return 0;
}

