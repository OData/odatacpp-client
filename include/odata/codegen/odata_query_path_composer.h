//---------------------------------------------------------------------
// <copyright file="odata_query_path_composer.h" company="Microsoft">
//      Copyright (C) Microsoft Corporation. All rights reserved. See License.txt in the project root for license information.
// </copyright>
//---------------------------------------------------------------------

#pragma once

#include "odata/common/utility.h"
#include "odata/core/odata_core.h"

namespace odata { namespace codegen {

class attribute
{
public:
	attribute(const ::odata::string_t& exp) : _exp(exp)
	{
	}

	attribute(::odata::string_t&& exp) : _exp(std::move(exp))
	{
	}

	attribute(const attribute& exp) : _exp(exp._exp)
	{
	}

	attribute(attribute&& exp) : _exp(std::move(exp._exp))
	{
	}

	attribute& operator = (const attribute& exp)
	{
		_exp = exp._exp;
		return *this;
	}

	attribute& operator = (attribute&& exp)
	{
		_exp = std::move(exp._exp);
		return *this;
	}

	attribute& operator () ()
	{
		_exp.insert(0, _XPLATSTR("("));
		_exp += _XPLATSTR(")");

		return *this;
	}

	attribute& operator + (const attribute& exp)
	{
		_exp += _XPLATSTR(" add ");
		_exp += exp.evaluate();

		return *this;
	}

	attribute& operator - (const attribute& exp)
	{
		_exp += _XPLATSTR(" sub ");
		_exp += exp.evaluate();

		return *this;
	}

	attribute& operator * (const attribute& exp)
	{
		_exp += _XPLATSTR(" mul ");
		_exp += exp.evaluate();

		return *this;
	}

	attribute& operator / (const attribute& exp)
	{
		_exp += _XPLATSTR(" div ");
		_exp += exp.evaluate();

		return *this;
	}

	attribute& operator % (const attribute& exp)
	{
		_exp += _XPLATSTR(" mod ");
		_exp += exp.evaluate();

		return *this;
	}

	attribute& operator && (const attribute& exp)
	{
		_exp += _XPLATSTR(" and ");
		_exp += exp.evaluate();

		return *this;
	}

	attribute& operator || (const attribute& exp)
	{
		_exp += _XPLATSTR(" or ");
		_exp += exp.evaluate();

		return *this;
	}

	attribute& operator ! ()
	{
		_exp.insert(0, _XPLATSTR("not "));

		return *this;
	}

	attribute& operator | (const attribute& exp)
	{
		_exp += _XPLATSTR(',');
		_exp += exp.evaluate();

		return *this;
	}

	template<typename T>
	attribute& operator == (const T& exp)
	{
		_exp += _XPLATSTR(" eq ");
		_exp += exp;

		return *this;
	}

	template<typename T>
	attribute& operator != (const T& exp)
	{
		_exp += _XPLATSTR(" ne ");
		_exp += exp;

		return *this;
	}

	template<typename T>
	attribute& operator >= (const T& exp)
	{
		_exp += _XPLATSTR(" ge ");
		_exp += exp;

		return *this;
	}

	template<typename T>
	attribute& operator > (const T& exp)
	{
		_exp += _XPLATSTR(" gt ");
		_exp += exp;

		return *this;
	}

	template<typename T>
	attribute& operator <= (const T& exp)
	{
		_exp += _XPLATSTR(" le ");
		_exp += exp;

		return *this;
	}

	template<typename T>
	attribute& operator < (const T& exp)
	{
		_exp += _XPLATSTR(" lt ");
		_exp += exp;

		return *this;
	}

	attribute& contains(const ::odata::string_t& exp)
	{
		_exp.insert(0, _XPLATSTR("contains("));
		_exp += _XPLATSTR(",'");
		_exp += exp;
		_exp += _XPLATSTR("')");

		return *this;
	}

	attribute& concat(const ::odata::string_t& exp)
	{
		_exp.insert(0, _XPLATSTR("concat("));
		_exp += _XPLATSTR(",'");
		_exp += exp;
		_exp += _XPLATSTR("')");

		return *this;
	}

	attribute& tolower()
	{
		_exp.insert(0, _XPLATSTR("tolower("));
		_exp += _XPLATSTR("')");

		return *this;
	}

	attribute& toupper()
	{
		_exp.insert(0, _XPLATSTR("toupper("));
		_exp += _XPLATSTR("')");

		return *this;
	}

	attribute& trim()
	{
		_exp.insert(0, _XPLATSTR("trim("));
		_exp += _XPLATSTR("')");

		return *this;
	}

	attribute& endswith(const ::odata::string_t& exp)
	{
		_exp.insert(0, _XPLATSTR("endswith("));
		_exp += _XPLATSTR(",'");
		_exp += exp;
		_exp += _XPLATSTR("')");

		return *this;
	}

	attribute& startswith(const ::odata::string_t& exp)
	{
		_exp.insert(0, _XPLATSTR("startswith("));
		_exp += _XPLATSTR(",'");
		_exp += exp;
		_exp += _XPLATSTR("')");

		return *this;
	}

	attribute& substring(int index)
	{
		_exp.insert(0, _XPLATSTR("substring("));
		_exp += _XPLATSTR(",'");
		_exp += utility::conversions::details::to_string_t(index);
		_exp += _XPLATSTR("')");

		return *this;
	}

	attribute& substring(int index, int length)
	{
		_exp.insert(0, _XPLATSTR("substring("));
		_exp += _XPLATSTR(",'");
		_exp += utility::conversions::details::to_string_t(index);
		_exp += _XPLATSTR("','");
		_exp += utility::conversions::details::to_string_t(length);
		_exp += _XPLATSTR("')");

		return *this;
	}

	attribute& substringof(const ::odata::string_t& exp)
	{
		_exp.insert(0, _XPLATSTR("substringof("));
		_exp += _XPLATSTR(",'");
		_exp += exp;
		_exp += _XPLATSTR("')");

		return *this;
	}

	attribute& length()
	{
		_exp.insert(0, _XPLATSTR("length("));
		_exp += _XPLATSTR(")");

		return *this;
	}

	attribute& indexof(const ::odata::string_t& exp)
	{
		_exp.insert(0, _XPLATSTR("indexof("));
		_exp += _XPLATSTR(",'");
		_exp +=  exp;
		_exp += _XPLATSTR("')");

		return *this;
	}

	attribute& replace(const ::odata::string_t& expSrch, const ::odata::string_t& expRepl)
	{
		_exp.insert(0, _XPLATSTR("replace("));
		_exp += _XPLATSTR(",'");
		_exp += expSrch;
		_exp += _XPLATSTR("','");
		_exp += expRepl;
		_exp += _XPLATSTR("')");

		return *this;
	}

	attribute& year()
	{
		_exp.insert(0, _XPLATSTR("year("));
		_exp += _XPLATSTR(")");

		return *this;
	}

	attribute& years()
	{
		_exp.insert(0, _XPLATSTR("years("));
		_exp += _XPLATSTR(")");

		return *this;
	}

	attribute& month()
	{
		_exp.insert(0, _XPLATSTR("month("));
		_exp += _XPLATSTR(")");

		return *this;
	}

	attribute& months()
	{
		_exp.insert(0, _XPLATSTR("months("));
		_exp += _XPLATSTR(")");

		return *this;
	}

	attribute& day()
	{
		_exp.insert(0, _XPLATSTR("day("));
		_exp += _XPLATSTR(")");

		return *this;
	}

	attribute& days()
	{
		_exp.insert(0, _XPLATSTR("days("));
		_exp += _XPLATSTR(")");

		return *this;
	}

	attribute& hour()
	{
		_exp.insert(0, _XPLATSTR("hour("));
		_exp += _XPLATSTR(")");

		return *this;
	}

	attribute& hours()
	{
		_exp.insert(0, _XPLATSTR("hours("));
		_exp += _XPLATSTR(")");

		return *this;
	}

	attribute& minute()
	{
		_exp.insert(0, _XPLATSTR("minute("));
		_exp += _XPLATSTR(")");

		return *this;
	}

	attribute& minutes()
	{
		_exp.insert(0, _XPLATSTR("minutes("));
		_exp += _XPLATSTR(")");

		return *this;
	}

	attribute& second()
	{
		_exp.insert(0, _XPLATSTR("second("));
		_exp += _XPLATSTR(")");

		return *this;
	}

	attribute& seconds()
	{
		_exp.insert(0, _XPLATSTR("seconds("));
		_exp += _XPLATSTR(")");

		return *this;
	}

	attribute& round()
	{
		_exp.insert(0, _XPLATSTR("round("));
		_exp += _XPLATSTR(")");

		return *this;
	}

	attribute& floor()
	{
		_exp.insert(0, _XPLATSTR("floor("));
		_exp += _XPLATSTR(")");

		return *this;
	}

	attribute& ceiling()
	{
		_exp.insert(0, _XPLATSTR("ceiling("));
		_exp += _XPLATSTR(")");

		return *this;
	}

	attribute& isof()
	{
		_exp.insert(0, _XPLATSTR("isof("));
		_exp += _XPLATSTR(")");

		return *this;
	}

	attribute& isof(const ::odata::string_t& exp)
	{
		_exp.insert(0, _XPLATSTR("isof("));
		_exp += _XPLATSTR(",");
		_exp += exp;
		_exp += _XPLATSTR(")");

		return *this;
	}

	attribute& cast()
	{
		_exp.insert(0, _XPLATSTR("cast("));
		_exp += _XPLATSTR(")");

		return *this;
	}

	attribute& cast(const ::odata::string_t& exp)
	{
		_exp.insert(0, _XPLATSTR("cast("));
		_exp += _XPLATSTR(",");
		_exp += exp;
		_exp += _XPLATSTR(")");

		return *this;
	}

	const ::odata::string_t& evaluate() const
	{
		return _exp;
	}

private:
	::odata::string_t _exp;
};

}}
