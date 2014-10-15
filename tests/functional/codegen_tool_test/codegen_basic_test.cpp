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