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
 
 #pragma once

#include "odata/common/utility.h"
#include "cpprest/asyncrt_utils.h"

namespace odata { namespace communication
{

/// <summary>
/// Represents an OData service operation exception
/// </summary>
class service_exception : public std::exception
{
public:

    /// <summary>
    /// Constructor
    /// </summary>
    /// <param name="error">A string value containing the service error.</param>
    explicit service_exception(::utility::string_t error) : m_error(error)
    {
    }

    /// <summary>
    /// Destructor
    /// </summary>
    ~service_exception() _noexcept {}

	const ::utility::string_t& what()
	{
		return m_error;
	}

private:
    ::utility::string_t m_error;
};

}}