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
 
 #include "odata/core/odata_json_writer_full.h"

using namespace ::web;
using namespace ::odata::edm;
using namespace ::odata::communication;

namespace odata { namespace core
{

web::json::value entity_json_writer_full::serialize(std::shared_ptr<odata_value> value_object)
{
	throw std::runtime_error("full metadata writer not implemented!"); 
}

web::json::value entity_json_writer_full::serialize(std::vector<std::shared_ptr<odata_value>> value_objects)
{
	throw std::runtime_error("full metadata writer not implemented!"); 
}

}}