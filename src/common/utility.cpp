//---------------------------------------------------------------------
// <copyright file="utility.cpp" company="Microsoft">
//      Copyright (C) Microsoft Corporation. All rights reserved. See License.txt in the project root for license information.
// </copyright>
//---------------------------------------------------------------------

#include "odata/common/utility.h"

namespace odata { namespace common
{

::odata::string_t strip_string(const ::odata::string_t& escaped)
{
	::odata::string_t::size_type first = 0;
	::odata::string_t::size_type size = escaped.size();

	if (escaped.empty())
	{
		return escaped;
	}

	if (escaped.front() == _XPLATSTR('"'))
	{
		++first;
	}

	if (escaped.back() == _XPLATSTR('"'))
	{
		--size;
	}

	return escaped.substr(first, size - first);
}

void split_string(const ::odata::string_t& source, const ::odata::string_t& delim, std::list<::odata::string_t>& ret)
{
	ret.clear();

	if (delim.empty() || source.empty())
	{
		ret.emplace_back(source);
		return;
	}

	size_t last  = 0;
	size_t index = source.find(delim, last);

	while (index != std::string::npos)
	{
		ret.emplace_back(source, last, index - last);
		last  = index + delim.size();
		index = source.find(delim, last);
	}

	// Trailing string after the last delimiter
	if (last < source.size())
	{
		ret.emplace_back(source, last);
	}
}

bool is_relative_path(const ::odata::string_t& _root_url, const ::odata::string_t& _path)
{
	if (_root_url.empty() || _path.empty())
	{
		return false;
	}

	// Must exeptionally below be ::utility::string_t (and not ::odata::string_t) due the mutating property of std::transform!
	::utility::string_t root_url = _root_url;
	::utility::string_t path = _path;

	if (root_url.length() > path.length())
	{
		return true;
	}

	std::transform(root_url.begin(), root_url.end(), root_url.begin(), ::tolower);
	std::transform(path.begin(), path.end(), path.begin(), ::tolower);
	return path.find(root_url) != 0;
}

::odata::string_t print_double(const double& db, int precision)
{
	::utility::ostringstream_t oss;
	oss << std::setiosflags(std::ios::fixed) << std::setiosflags(std::ios::right) << std::setprecision(precision)  << db;
	if (oss.bad())
	{
		throw std::bad_cast();
	}

	::odata::string_t output = oss.str();
	size_t dot = output.find(_XPLATSTR('.'));
	if (dot > 0)
	{
		size_t i = output.length() - 1;
		for (; i --> dot;)
		{
			if (output[i] != _XPLATSTR('0'))
			{
				break;
			}
		}

		if (i == dot)
		{
			++i;
		}

		output.erase(i + 1);
	}

	return output;
}

::odata::string_t print_float(const float& db, int precision)
{
	::utility::ostringstream_t oss;
	oss << std::setiosflags(std::ios::fixed) << std::setiosflags(std::ios::right) << std::setprecision(precision)  << db;
	if (oss.bad())
	{
		throw std::bad_cast();
	}

	::odata::string_t output = std::move(oss.str());
	size_t dot = output.find(_XPLATSTR('.'));
	if (dot > 0)
	{
		size_t i = output.length() - 1;
		for (; i --> dot;)
		{
			if (output[i] != _XPLATSTR('0'))
			{
				break;
			}
		}

		if (i == dot)
		{
			++i;
		}

		output.erase(i + 1);
	}

	return output;
}

}}
