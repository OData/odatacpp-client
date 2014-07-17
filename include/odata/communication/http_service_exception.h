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