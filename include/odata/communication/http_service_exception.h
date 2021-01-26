//---------------------------------------------------------------------
// <copyright file="http_service_exception.h" company="Microsoft">
//      Copyright (C) Microsoft Corporation. All rights reserved. See License.txt in the project root for license information.
// </copyright>
//---------------------------------------------------------------------

#pragma once

#include <locale>  // std::wstring_convert
#include <codecvt> // std::codecvt_utf8

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
	/// <remarks>
	/// On Windows string_t is wide, however method what() always is narrow,
	/// thus we need to convert the string. On POSIX this is not needed.
	/// </remarks>
	#ifdef _UTF16_STRINGS
	explicit service_exception(::odata::string_t error) : m_error()
	{
		try
		{
			// This conversion method was only introduced with C++11 but has already
			// been deprecated with C++17 without giving any alternatives to use!
			// So as long as this is the case we use it anyway to be portable.
			std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> cv;
			m_error = cv.to_bytes(error);
		}
		catch(...)
		{
			// to_bytes() might throw std::range_error for invalid input data!
			m_error.clear();
		}
	}
	#else
	explicit service_exception(::odata::string_t error) : m_error(error)
	{
	}
	#endif

	/// <summary>
	/// Destructor
	/// </summary>
	~service_exception() _noexcept = default;

	virtual char const* what() const override
	{
		return m_error.c_str();
	}

private:
	std::string m_error;
};

}}
