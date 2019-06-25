//---------------------------------------------------------------------
// <copyright file="odata_codegen_tools.cpp" company="Microsoft">
//      Copyright (C) Microsoft Corporation. All rights reserved. See License.txt in the project root for license information.
// </copyright>
//---------------------------------------------------------------------

#include "odata_codegen_initializer.h"
#include "odata_codegen_writer.h"
#include "odata/client/odata_client.h"
#include "cpprest/uri.h"
#include "cpprest/asyncrt_utils.h"
#include "cpprest/json.h"
#include "cpprest/http_client.h"
#include "cpprest/containerstream.h"
#include "cpprest/filestream.h"
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

void print_help()
{
	::std::cout << ::std::endl << "[Usage] - " << ::std::endl
		<< "    " << "odata_codegen_tools.exe -f \"metadata file path\" -o \"output file name\""  << ::std::endl
		<< "Or" << ::std::endl
		<< "    " << "odata_codegen_tools.exe -m \"service root url\" -o \"ouput file name\" [-u \"user name\" -p \"password\"]" << ::std::endl << ::std::endl
		<< "[Example] - " << ::std::endl
		<< "    " << "odata_codegen_tools.exe -f \"c:\\files\\test.xml\" -o \"test\""  << ::std::endl
		<< "    " << "odata_codegen_tools.exe -m \"http://service/odata\" -o \"newfile\" [-u \"abc\" -p \"ppp\"]" << ::std::endl << ::std::endl;
		
}

#ifdef WIN32
int _tmain(int argc, ::utility::char_t* argv[])
#else
int main(int argc, ::utility::char_t* argv[])
#endif
{
	::utility::string_t metadata_url;
	::utility::string_t metadata_file;
	::utility::string_t output_file_name;
	::utility::string_t user_name;
	::utility::string_t password;

	bool argument_error = false;
	if (argc < 3)
	{
		argument_error = true;
	}
	else 
	{
		for (int i = 1; i < argc; i++)
		{
			::utility::string_t option = argv[i];

			if (option == U("-f") || option == U("-m") || option == U("-o") || option == U("-u") || option == U("-p"))
			{
				if (++i >= argc)
				{
					argument_error = true;
					break;
				}

				if (option == U("-f"))
				{
					metadata_file = argv[i];
				}
				else if (option == U("-m"))
				{
					metadata_url = argv[i];
				}
				else if (option == U("-o"))
				{
					output_file_name = argv[i];
				}
				else if (option == U("-u"))
				{
					user_name = argv[i];
				}
				else if (option == U("-p"))
				{
					password = argv[i];
				}
			}
			else
			{
				argument_error = true;
				break;
			}
		}
	}

	if (metadata_file.empty() && metadata_url.empty())
	{
		argument_error = true;
	}

	if (argument_error)
	{
		std::cerr << "cmd argument error";
		print_help();
		return -1;
	}

	try
	{
		auto initializer = std::make_shared<odata_codegen_initializer>();
		if (!metadata_file.empty())
		{
			initializer->initialize_from_metadata_file(metadata_file, output_file_name).then(
			   [initializer] (int) -> int
			   {
				   auto writer = std::make_shared<odata_codegen_writer>(initializer);
				   return writer->begin_generate_file().wait();
			   }).wait();;
		}
		else if (!metadata_url.empty())
		{
			initializer->initialize_from_service_metadata(metadata_url, output_file_name, user_name, password).then(
			   [initializer] (int) -> int
			   {
				   auto writer = std::make_shared<odata_codegen_writer>(initializer);
				   return writer->begin_generate_file().wait();
			   }).wait();
		}
			
	}
	catch (const ::std::exception &e)
	{
		::std::string error_msg = "failed to generate code files: ";
		error_msg += e.what();
		std::cerr << error_msg;

		return -1;
	}

	return 0;
}

