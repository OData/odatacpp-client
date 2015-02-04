//---------------------------------------------------------------------
// <copyright file="codegen_basic_test.cpp" company="Microsoft">
//      Copyright (C) Microsoft Corporation. All rights reserved. See License.txt in the project root for license information.
// </copyright>
//---------------------------------------------------------------------

#include "codegen_tool_test_case.h"

namespace tests { namespace functional { namespace _odata {

SUITE(codegen_basic_test)
{

TEST_FIXTURE(codegen_tool_test_case, simple_model_test)  
{
	::std::string metadata_file_name("SimpleModel.xml"), baseline_file_name("simple_model_test_baseline"), generated_file_name("simple_model_test");
	set_test_parameters(metadata_file_name, baseline_file_name, generated_file_name);

	generate_code_files();
	compare_with_baseline();
}

TEST_FIXTURE(codegen_tool_test_case, default_service_test)  
{
	::std::string metadata_file_name("DefaultServiceModel.xml"), baseline_file_name("default_service_test_baseline"), generated_file_name("default_service_test");
	set_test_parameters(metadata_file_name, baseline_file_name, generated_file_name);

	generate_code_files();
	compare_with_baseline();
}

}

}}}