#include "odata/common/utility.h"

namespace odata { namespace utility
{

::utility::string_t strip_string(const ::utility::string_t& escaped)
{
    ::utility::string_t::size_type first = 0;
    ::utility::string_t::size_type size = escaped.size();

	if (escaped.empty())
	{
		return escaped;
	}

    if (escaped[0] == U('"'))
	{
        first += 1;
	}

    if (escaped[size - 1] == U('"'))
	{
        size -= 1;
	}

    return escaped.substr(first, size - first);
}

void split_string(::utility::string_t& source, const ::utility::string_t& delim, std::list<::utility::string_t>& ret)
{
	ret.clear();

	if (delim.empty() || source.empty())
	{
		ret.push_back(source);
		return ;
	}

	size_t last = 0;
	size_t index = source.find(delim, last);

    while (index!=std::string::npos)
    {
		ret.push_back(source.substr(last, index - last));
		last = index + delim.size();
		index = source.find(delim, last);
	}

    if(index - last > 0)
	{
		ret.push_back(source.substr(last, index - last));
	}
}

bool is_relative_path(const ::utility::string_t& _root_url, const ::utility::string_t& _path)
{
	if (_root_url.empty() || _path.empty())
	{
		return false;
	}

	::utility::string_t root_url = _root_url;
	::utility::string_t path = _path;

	if (root_url.length() > path.length())
	{
		return true;
	}

	std::transform(root_url.begin(), root_url.end(), root_url.begin(), ::tolower);
	std::transform(path.begin(), path.end(), path.begin(), ::tolower);

	size_t index = path.find(root_url);

	return path.find(root_url) != 0 ? true : false;
}

::utility::string_t print_double(const double& db, int precision)
{
    ::utility::ostringstream_t oss;
	oss << std::setiosflags(std::ios::fixed) << std::setiosflags(std::ios::right) << std::setprecision(precision)  << db;
    if (oss.bad())
	{
        throw std::bad_cast();
	}

	::utility::string_t output = oss.str();
	int dot = output.find(U('.'));
	if (dot > 0)
	{
		int i = output.length() - 1;
		for (i = output.length() - 1; i > dot; i--)
		{
			if (output[i] != U('0'))
			{
				break;
			}
		}

		if (i == dot)
		{
			i++;
		}

		output = output.substr(0, i + 1);
	}

    return output;
}

::utility::string_t print_float(const float& db, int precision)
{
    ::utility::ostringstream_t oss;
	oss << std::setiosflags(std::ios::fixed) << std::setiosflags(std::ios::right) << std::setprecision(precision)  << db;
    if (oss.bad())
	{
        throw std::bad_cast();
	}

	::utility::string_t output = oss.str();
	int dot = output.find(U('.'));
	if (dot > 0)
	{
		int i = output.length() - 1;
		for (i = output.length() - 1; i > dot; i--)
		{
			if (output[i] != U('0'))
			{
				break;
			}
		}

		if (i == dot)
		{
			i++;
		}

		output = output.substr(0, i + 1);
	}

    return output;
}

}}