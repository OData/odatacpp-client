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
#include "odata/core/odata_core.h"

namespace odata { namespace codegen { 

class atrribute
{
public:
	atrribute(const ::utility::string_t& exp) : _exp(exp)
	{}

	atrribute& operator && (const atrribute& exp)
	{
		::utility::stringstream_t ostr;

		ostr << _exp << U(" and ") << exp.evaluate();

		_exp = ostr.str();

		return *this;
	}

	atrribute& operator || (const atrribute& exp)
	{
		::utility::stringstream_t ostr;

		ostr << _exp << U(" or ") << exp.evaluate();

		_exp = ostr.str();

		return *this;
	}

	atrribute& operator !()
	{
		::utility::stringstream_t ostr;

		ostr << U("not ") << _exp;

		_exp = ostr.str();

		return *this;
	}

	atrribute& operator |(const atrribute& exp)
	{
		::utility::stringstream_t ostr;

		ostr << _exp << U(",") << exp.evaluate();

		_exp = ostr.str();

		return *this;
	}

	template<typename T>
	atrribute& operator == (const T& exp)
	{
		::utility::stringstream_t ostr;

		ostr << _exp << U(" eq ") << exp;

		_exp = ostr.str();

		return *this;
	}

	template<typename T>
	atrribute& operator != (const T& exp)
	{
		::utility::stringstream_t ostr;

		ostr << _exp << U(" ne ") << exp;

		_exp = ostr.str();

		return *this;
	}

	template<typename T>
	atrribute& operator >= (const T& exp)
	{
		::utility::stringstream_t ostr;

		ostr << _exp << U(" ge ") << exp;

		_exp = ostr.str();

		return *this;
	}

	template<typename T>
	atrribute& operator > (const T& exp)
	{
		::utility::stringstream_t ostr;

		ostr << _exp << U(" gt ") << exp;

		_exp = ostr.str();

		return *this;
	}

	template<typename T>
	atrribute& operator <= (const T& exp)
	{
		::utility::stringstream_t ostr;

		ostr << _exp << U(" le ") << exp;

		_exp = ostr.str();

		return *this;
	}

	template<typename T>
	atrribute& operator < (const T& exp)
	{
		::utility::stringstream_t ostr;

		ostr << _exp << U(" lt ") << exp;

		_exp = ostr.str();

		return *this;
	}

	atrribute& contains(const ::utility::string_t& exp)
	{
		::utility::stringstream_t ostr;

		ostr << U("contains") << U("(") << _exp << U(",'") << exp << U("')");

		_exp = ostr.str();

		return *this;
	}

	atrribute& concat(const ::utility::string_t& exp)
	{
		::utility::stringstream_t ostr;

		ostr << U("concat") << U("(") << _exp << U(",'") << exp << U("')");

		_exp = ostr.str();

		return *this;
	}

	atrribute& tolower()
	{
		::utility::stringstream_t ostr;

		ostr << U("tolower") << U("(") << _exp << U("')");

		_exp = ostr.str();

		return *this;
	}

	atrribute& toupper()
	{
		::utility::stringstream_t ostr;

		ostr << U("toupper") << U("(") << _exp << U("')");

		_exp = ostr.str();

		return *this;
	}

	atrribute& trim()
	{
		::utility::stringstream_t ostr;

		ostr << U("trim") << U("(") << _exp << U("')");

		_exp = ostr.str();

		return *this;
	}

	atrribute& endswith(const ::utility::string_t& exp)
	{
		::utility::stringstream_t ostr;

		ostr << U("endswith") << U("(") << _exp << U(",'") << exp << U("')");

		_exp = ostr.str();

		return *this;
	}

	atrribute& startswith(const ::utility::string_t& exp)
	{
		::utility::stringstream_t ostr;

		ostr << U("startswith") << U("(") << _exp << U(",'") << exp << U("')");

		_exp = ostr.str();

		return *this;
	}

	atrribute& substring(int index)
	{
		::utility::stringstream_t ostr;

		ostr << U("substring") << U("(") << _exp << U(",'") << index << U("')");

		_exp = ostr.str();

		return *this;
	}

	atrribute& substring(int index, int length)
	{
		::utility::stringstream_t ostr;

		ostr << U("substring") << U("(") << _exp << U(",'") << index << U(",'") << length << U("')");

		_exp = ostr.str();

		return *this;
	}

	const ::utility::string_t& evaluate() const
	{
		return _exp;
	}

private:
	::utility::string_t _exp;
};

}}