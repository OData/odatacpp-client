//---------------------------------------------------------------------
// <copyright file="codegen_tool_test_case.h" company="Microsoft">
//      Copyright (C) Microsoft Corporation. All rights reserved. See License.txt in the project root for license information.
// </copyright>
//---------------------------------------------------------------------

#pragma once

#include "cpprest/basic_types.h"
#include "unittestpp.h"

namespace tests { namespace functional { namespace _odata {

class codegen_tool_test_case
{
public:
	::std::string m_metadata_file_name;
	::std::string m_baseline_file_name;
	::std::string m_generated_file_name;
	::std::string m_codegen_tool_file_name;

	codegen_tool_test_case() {};

	void set_test_parameters(::std::string &metadata_file_name, ::std::string &baseline_file_name, ::std::string &generated_file_name)
	{
		m_metadata_file_name = metadata_file_name;
		m_baseline_file_name = baseline_file_name;
		m_generated_file_name = generated_file_name;
#if defined(_MSC_VER) && (_MSC_VER >= 1800)
#ifdef _DEBUG
		m_codegen_tool_file_name = "odata_codegen_tool.vs12d.exe";
#else
        m_codegen_tool_file_name = "odata_codegen_tool.vs12.exe";
#endif
#else
#ifdef _DEBUG
		m_codegen_tool_file_name = "odata_codegen_tool.vs11d.exe";
#else
        m_codegen_tool_file_name = "odata_codegen_tool.vs11.exe";
#endif
#endif
	};

	void generate_code_files()
	{
		::std::string codegen_command = ".\\" + m_codegen_tool_file_name + " -f \".\\baseline\\" + m_metadata_file_name + "\" -o \"" + m_generated_file_name + "\"";
		auto t = codegen_command.c_str();
		int codegen_status = system(t);
		VERIFY_ARE_EQUAL(0, codegen_status);
	};

	void compare_with_baseline()
	{
		::std::string header_file_string, header_baseline_string, cpp_file_string, cpp_baseline_string;

		try
		{
			::std::ifstream header_file(".\\" + m_generated_file_name + ".h");
			::std::stringstream header_file_buffer;
			header_file_buffer << header_file.rdbuf();
			header_file.close();

			::std::ifstream header_baseline(".\\baseline\\" + m_baseline_file_name + ".h");
			::std::stringstream header_baseline_buffer;
			header_baseline_buffer << header_baseline.rdbuf();
			header_baseline.close();

			header_file_string = header_file_buffer.str();
			header_baseline_string = header_baseline_buffer.str();

			::std::ifstream cpp_file(".\\" + m_generated_file_name + ".cpp");
			::std::stringstream cpp_file_buffer;
			cpp_file_buffer << cpp_file.rdbuf();
			cpp_file.close();

			::std::ifstream cpp_baseline(".\\baseline\\" + m_baseline_file_name + ".cpp");
			::std::stringstream cpp_baseline_buffer;
			cpp_baseline_buffer << cpp_baseline.rdbuf();
			cpp_baseline.close();

			cpp_file_string = cpp_file_buffer.str();
			cpp_baseline_string = cpp_baseline_buffer.str();		
		}
		catch (const std::exception &e)
		{
			::std::string error_msg = "error when trying to read generated files or baseline files - ";
			error_msg += e.what();
			::std::cout << error_msg << ::std::endl;

			CHECK(false);
		}

		bool header_equal = header_baseline_string == header_file_string;
		bool cpp_equal = cpp_baseline_string == cpp_file_string;
		CHECK(header_equal);
		CHECK(cpp_equal);	
	}

};

}}}