//---------------------------------------------------------------------
// <copyright file="utility.h" company="Microsoft">
//      Copyright (C) Microsoft Corporation. All rights reserved. See License.txt in the project root for license information.
// </copyright>
//---------------------------------------------------------------------

#pragma once

#include <string>
#ifdef __cpp_lib_string_view
	#include <string_view>
#endif
#include <fstream>
#include <iostream>
#include <sstream>
#include <functional>
#include <memory>
#include <utility>
#include <exception>
#include <algorithm>
#include <unordered_map>
#include <functional>
#include <thread>
#include <mutex>
#include <iomanip>
#include "cpprest/uri.h"
#include "cpprest/asyncrt_utils.h"
#include "cpprest/json.h"
#include "cpprest/http_client.h"
#include "odata/common/platform.h"

// Select std and boost replacements
#define _USE_BOOST_FLYWEIGHT_STRING
//#define _USE_BOOST_FLYWEIGHT_VECTOR
#define _USE_SHARED_PTR_TWO_PHASE_ALLOCATION
//#define _ODATA_TEST_INSTANCIATE_HASH_SPECIALISATIONS

// If we want to use boost::flyweight<std::string> we create an additional wrapper around this to simulate the behavior of std::string as closely as possible.
// The only exception here are functions that have iterators on the string to manipulate or return a reference to the hold string (non-const).
#if defined(_USE_BOOST_FLYWEIGHT_STRING) || defined(_USE_BOOST_FLYWEIGHT_VECTOR) || !defined(_BOOST_FLYWEIGHT_HASH_OPTIMIZATION)
	#include "boost/flyweight.hpp"
	#include "boost/flyweight/no_tracking.hpp"
	#include "boost/container_hash/hash.hpp"
#endif

// Extend std::hash to support boost::flyweight with optimum performance:
// This causes to hash the address of the flyweight object instead of the object itself, resulting in optimum performance for the hash operation.
// For an explanation see http://lists.boost.org/boost-users/2013/03/78007.php
// and https://stackoverflow.com/questions/8695379/how-to-make-boost-unordered-map-to-support-flyweightstring
// Hint: According to the standard extending the std-namespace in this case is exceptionally legal (for template specialization).
//       Beware! Extending the std-namespace in other use cases is an absolute no-go!
#ifndef _BOOST_FLYWEIGHT_HASH_OPTIMIZATION
namespace std
{
	template <typename T> struct hash<::boost::flyweight<T, ::boost::flyweights::no_tracking>>
	{
		using value_type = ::boost::flyweight<T, ::boost::flyweights::no_tracking>;
		size_t operator()(value_type const &Value) const
		{
			hash<void const*> hasher;
			return hasher(&Value.get());
		}
	};
}
#define _BOOST_FLYWEIGHT_HASH_OPTIMIZATION
#endif

#ifdef _USE_BOOST_FLYWEIGHT_STRING
namespace odata
{

	// ============================================================================================================
	// Begin extension:
	// Alternatively to ::utility::string_t we can use ::boost::flyweight<::utility::string_t> for storing strings.
	// Depending on the data processed this gives us releave on the memory burden that plain strings will cause.
	// This is mainly useful in case of many identical strings that otherwise clobber the memory.
	// ============================================================================================================

class string_t
{
	// @HINT: In order to make it fully functional we'd also need to add replacement classes for reference and iterator types!
	//        However this might prove to be extremely unperformant. Every time the reference or iterator is being written to
	//        we'd need to create a complete new flyweight string and swap it with the existing one. When looping over a string
	//        this will yield unearthly, prohibitively bad performance!
	//        So until someone comes up with a well founded reason to have those objects (can't imagine why) we'll not take
	//        the effort to implement this complicated and utmost useless helper classes that are dangerous to use for unawares!
	// @TODO: However there *is* a scenario when refactoring existing code, namely the use of mutating function of std::algorithm!
	//        Refactor this into creating a temporary ::utility::string_t and mutating this one, assigning back to the flyweight
	//        string if necessary. Alternatively one could also supply suitable overloads of all mutating algorithms if desirable.

	public:

		// Forward typedefs
		typedef ::utility::string_t::traits_type            traits_type;
		typedef ::utility::string_t::value_type             value_type;
		typedef ::utility::string_t::allocator_type         allocator_type;
		typedef ::utility::string_t::size_type              size_type;
		typedef ::utility::string_t::difference_type        difference_type;
		typedef ::utility::string_t::reference              reference;
		typedef ::utility::string_t::const_reference        const_reference;
		typedef ::utility::string_t::pointer                pointer;
		typedef ::utility::string_t::const_pointer          const_pointer;
		typedef ::utility::string_t::iterator               iterator;
		typedef ::utility::string_t::const_iterator         const_iterator;
		typedef ::utility::string_t::reverse_iterator       reverse_iterator;
		typedef ::utility::string_t::const_reverse_iterator const_reverse_iterator;

		// Member functions
		// ================

		// Constructors
		string_t()                                                                                                                      : s_() {}
		explicit string_t(allocator_type const &alloc) noexcept                                                                         : s_(alloc) {}
		string_t(size_type count, value_type ch, allocator_type const &alloc = allocator_type())                                        : s_(count, ch, alloc) {}
		string_t(string_t const &other, size_type pos, size_type count = npos, allocator_type const &alloc = allocator_type())          : s_(other.s_, pos, count, alloc) {}
		string_t(::utility::string_t const &str, size_type pos, size_type count = npos, allocator_type const& alloc = allocator_type()) : s_(str, pos, count, alloc) {}
		string_t(value_type const *cstr, size_type count, allocator_type const &alloc = allocator_type())                               : s_(cstr, count, alloc) {}
		string_t(value_type const *cstr, allocator_type const &alloc = allocator_type())                                                : s_(cstr, alloc) {}
		template<typename InputIt> string_t(InputIt first, InputIt last, allocator_type const &alloc = allocator_type())                : s_(first, last, alloc) {}
		string_t(string_t const &other)                                                                                                 : s_(other.s_) {}
		string_t(string_t const &other, allocator_type const &alloc)                                                                    : s_(other.s_, alloc) {}
		string_t(::utility::string_t const &str)                                                                                        : s_(str) {}
		string_t(::utility::string_t const &str, allocator_type const& alloc)                                                           : s_(str, alloc) {}
		string_t(string_t &&other) noexcept                                                                                             : s_(std::move(other.s_)) {}
		string_t(string_t &&other, allocator_type const &alloc)                                                                         : s_(std::move(other.s_), alloc) {}
		string_t(::utility::string_t && str) noexcept                                                                                   : s_(std::move(str)) {}
		string_t(::utility::string_t && str, allocator_type const& alloc)                                                               : s_(std::move(str), alloc) {}
		string_t(std::initializer_list<value_type> ilist, allocator_type const &alloc = allocator_type())                               : s_(ilist, alloc) {}
		template<typename T> explicit string_t(T const &t, allocator_type const &alloc = allocator_type())                              : s_(::utility::string_t(t), alloc) {}
		template<typename T> string_t(T const &t, size_type pos, size_type n, allocator_type const &alloc = allocator_type())           : s_(::utility::string_t(t, pos, n), alloc) {}

		// Destructor
		~string_t() = default;

		// operator=
		string_t& operator=(string_t const &str)                     { s_ = str.s_;                     return *this; }
		string_t& operator=(::utility::string_t const& str)          { s_ = str;                        return *this; }
		string_t& operator=(string_t &&str) noexcept                 { s_ = std::move(str.s_);          return *this; }
		string_t& operator=(::utility::string_t&& str) noexcept      { s_ = std::move(str);             return *this; }
		string_t& operator=(value_type const *cstr)                  { s_ = cstr;                       return *this; }
		string_t& operator=(value_type ch)                           { s_ = ::utility::string_t(1, ch); return *this; }
		string_t& operator=(std::initializer_list<value_type> ilist) { s_ = ilist;                      return *this; }
		template<typename T> string_t& operator=(T const &t)         { s_ = t;                          return *this; }

		// Assign
		string_t& assign(size_type count, value_type ch)                                          { s_ = ::utility::string_t(count, ch);          return *this; }
		string_t& assign(string_t const& str)                                                     { s_ = str.s_;                                  return *this; }
		string_t& assign(::utility::string_t const& str)                                          { s_ = str;                                     return *this; }
		string_t& assign(string_t const& str, size_type pos, size_type count = npos)              { s_ = ::utility::string_t(str.s_, pos, count); return *this; }
		string_t& assign(::utility::string_t const& str, size_type pos, size_type count = npos)   { s_ = ::utility::string_t(str, pos, count);    return *this; }
		string_t& assign(string_t &&str) noexcept                                                 { s_ = std::move(str.s_);                       return *this; }
		string_t& assign(::utility::string_t &&str) noexcept                                      { s_ = std::move(str);                          return *this; }
		string_t& assign(value_type const *cstr, size_type count)                                 { s_ = ::utility::string_t(cstr, count);        return *this; }
		string_t& assign(value_type const *cstr)                                                  { s_ = cstr;                                    return *this; }
		template<typename InputIt> string_t& assign(InputIt first, InputIt last)                  { s_ = ::utility::string_t(first, last);        return *this; }
		string_t& assign(std::initializer_list<value_type> ilist)                                 { s_ = ::utility::string_t(ilist);              return *this; }
		template <typename T> string_t& assign(T const &t)                                        { s_ = ::utility::string_t(t);                  return *this; }
		template <typename T> string_t& assign(T const &t, size_type pos, size_type count = npos) { s_ = ::utility::string_t(t, pos, count);      return *this; }

		// get_allocator
		allocator_type get_allocator() const { return s_.get().get_allocator(); }

		// Element access
		// ==============

		// at
		//reference at(size_type pos); // @HINT: Needs reference replacement!
		const_reference at(size_type pos) const { return s_.get().at(pos); }

		// operator[]
		//reference operator[](size_type pos);  // @HINT: Needs reference replacement!
		const_reference operator[](size_type pos) const { return s_.get()[pos]; }

		// front
		//value_type& front();  // @HINT: Needs reference replacement!
		value_type const& front() const { return s_.get().front(); }

		// back
		//value_type& back();  // @HINT: Needs reference replacement!
		value_type const& back() const { return s_.get().back(); }

		// data
		value_type const* data() const noexcept { return s_.get().data(); }

		// c_str()
		value_type const* c_str() const noexcept { return s_.get().c_str(); }

		// operator basic_string_view
		#ifdef __cpp_lib_string_view
			operator std::basic_string_view<value_type, traits_type>() const noexcept { return std::basic_string_view<value_type, traits_type>(s_.get()); }
		#endif

		// Iterators
		// =========

		// (c)begin
		//iterator begin() noexcept; // @HINT: Needs iterator replacement!
		const_iterator begin()  const noexcept { return s_.get().begin();  }
		const_iterator cbegin() const noexcept { return s_.get().cbegin(); }

		// (c)end
		//iterator end() noexcept; // @HINT: Needs iterator replacement!
		const_iterator end()  const noexcept { return s_.get().end();  }
		const_iterator cend() const noexcept { return s_.get().cend(); }

		// (c)rbegin
		//reverse_iterator rbegin() noexcept; // @HINT: Needs iterator replacement!
		const_reverse_iterator rbegin()  const noexcept { return s_.get().rbegin();  }
		const_reverse_iterator crbegin() const noexcept { return s_.get().crbegin(); }

		// (c)rend
		//reverse_iterator rend() noexcept; // @HINT: Needs iterator replacement!
		const_reverse_iterator rend()  const noexcept { return s_.get().rend();  }
		const_reverse_iterator crend() const noexcept { return s_.get().crend(); }

		// Capacity
		// ========

		// empty
		#ifdef __has_cpp_attribute
			#if __has_cpp_attribute(nodiscard)
				[[nodiscard]]
			#endif
		#endif
		bool empty() const noexcept { return s_.get().empty(); }

		// size/length
		size_type size()   const noexcept { return s_.get().size();   }
		size_type length() const noexcept { return s_.get().length(); }

		// max_size
		size_type max_size() const noexcept { return s_.get().max_size(); }

		// reserve
		void reserve(size_type /*new_cap*/ = 0) {} // Not safely/reasonably implementable, thus made it a no-op!

		// capacity
		size_type capacity() const noexcept { return s_.get().capacity(); }

		// shrink_to_fit
		void shrink_to_fit() {} // Not safely/reasonably implementable, thus made it a no-op!

		// Operations
		// ==========

		// clear
		void clear() noexcept { s_ = ::utility::string_t(); }

		// insert
		string_t& insert(size_type index, size_type count, value_type ch)                                                { auto s = s_.get(); s.insert(index, count, ch);                s_ = std::move(s); return *this; }
		string_t& insert(size_type index, value_type const *cstr)                                                        { auto s = s_.get(); s.insert(index, cstr);                     s_ = std::move(s); return *this; }
		string_t& insert(size_type index, value_type const * cstr, size_type count)                                      { auto s = s_.get(); s.insert(index, cstr, count);              s_ = std::move(s); return *this; }
		string_t& insert(size_type index, string_t const &str)                                                           { auto s = s_.get(); s.insert(index, str.s_);                   s_ = std::move(s); return *this; }
		string_t& insert(size_type index, ::utility::string_t const &str)                                                { auto s = s_.get(); s.insert(index, str);                      s_ = std::move(s); return *this; }
		string_t& insert(size_type index, string_t const &str, size_type index_str, size_type count = npos)              { auto s = s_.get(); s.insert(index, str.s_, index_str, count); s_ = std::move(s); return *this; }
		string_t& insert(size_type index, ::utility::string_t const &str, size_type index_str, size_type count = npos)   { auto s = s_.get(); s.insert(index, str, index_str, count);    s_ = std::move(s); return *this; }
		const_iterator insert(const_iterator pos, value_type ch)
		{
			auto s    = s_.get();
			auto iter = s.insert(s.begin() + (pos - s_.get().cbegin()), ch);
			s_        = std::move(s);
			return cbegin() + (iter - s.begin());
		}
		const_iterator insert(const_iterator pos, size_type count, value_type ch)
		{
			auto s = s_.get();
			auto iter = s.insert(s.begin() + (pos - s_.get().cbegin()), count, ch);
			s_ = std::move(s);
			return cbegin() + (iter - s.begin());
		}
		template<typename InputIt> const_iterator insert(const_iterator pos, InputIt first, InputIt last)
		{
			auto s = s_.get();
			auto iter = s.insert(s.begin() + (pos - s_.get().cbegin()), first, last);
			s_ = std::move(s);
			return cbegin() + (iter - s.begin());
		}
		const_iterator insert(const_iterator pos, std::initializer_list<value_type> ilist)
		{
			auto s = s_.get();
			auto iter = s.insert(s.begin() + (pos - s_.get().cbegin()), ilist);
			s_ = std::move(s);
			return cbegin() + (iter - s.begin());
		}
		template <typename T> string_t& insert(size_type pos, T const &t)                                                { auto s = s_.get(); s.insert(pos, t);                          s_ = std::move(s); return *this; }
		template <typename T> string_t& insert(size_type index, T const &t, size_type index_str, size_type count = npos) { auto s = s_.get(); s.insert(index, t, index_str, count);      s_ = std::move(s); return *this; }

		// erase
		string_t& erase(size_type index = 0, size_type count = npos) { auto s = s_.get(); s.erase(index, count); s_ = std::move(s); return *this; }
		const_iterator erase(const_iterator position)
		{
			auto s = s_.get();
			auto iter = s.erase(s.begin() + (position - s_.get().cbegin()));
			s_ = std::move(s);
			return cbegin() + (iter - s.begin());
		}
		const_iterator erase(const_iterator first, const_iterator last)
		{
			auto s = s_.get();
			auto iter = s.erase(s.begin() + (first - s_.get().cbegin()), s.begin() + (last - s_.get().cbegin()));
			s_ = std::move(s);
			return cbegin() + (iter - s.begin());
		}

		// push_back
		void push_back(value_type ch) { append(static_cast<size_type>(1U), ch); }

		// pop_back
		void pop_back() { auto s = s_.get(); s.pop_back(); s_ = std::move(s); }

		// append
		string_t& append(size_type count, value_type ch)                                          { auto s = s_.get(); s.append(count, ch);          s_ = std::move(s); return *this; }
		string_t& append(string_t const &str)                                                     { auto s = s_.get(); s.append(str.s_);             s_ = std::move(s); return *this; }
		string_t& append(::utility::string_t const &str)                                          { auto s = s_.get(); s.append(str);                s_ = std::move(s); return *this; }
		string_t& append(string_t const &str, size_type pos, size_type count = npos)              { auto s = s_.get(); s.append(str.s_, pos, count); s_ = std::move(s); return *this; }
		string_t& append(::utility::string_t const &str, size_type pos, size_type count = npos)   { auto s = s_.get(); s.append(str, pos, count);    s_ = std::move(s); return *this; }
		string_t& append(value_type const *cstr, size_type count)                                 { auto s = s_.get(); s.append(cstr, count);        s_ = std::move(s); return *this; }
		string_t& append(value_type const *cstr)                                                  { auto s = s_.get(); s.append(cstr);               s_ = std::move(s); return *this; }
		template<typename InputIt> string_t& append(InputIt first, InputIt last)                  { auto s = s_.get(); s.append(first, last);        s_ = std::move(s); return *this; }
		string_t& append(std::initializer_list<value_type> ilist)                                 { auto s = s_.get(); s.append(ilist);              s_ = std::move(s); return *this; }
		template <typename T> string_t& append(T const &t)                                        { auto s = s_.get(); s.append(t);                  s_ = std::move(s); return *this; }
		template <typename T> string_t& append(T const &t, size_type pos, size_type count = npos) { auto s = s_.get(); s.append(t, pos, count);      s_ = std::move(s); return *this; }

		// operator+=
		string_t& operator+=(string_t const &str)                     { return append(str);                            }
		string_t& operator+=(::utility::string_t const &str)          { return append(str);                            }
		string_t& operator+=(value_type ch)                           { return append(static_cast<size_type>(1U), ch); }
		string_t& operator+=(value_type const *cstr)                  { return append(cstr);                           }
		string_t& operator+=(std::initializer_list<value_type> ilist) { return append(ilist);                          }
		template <typename T> string_t& operator+=(T const &t)        { return append(t);                              }

		// compare
		int compare(string_t const &str) const noexcept                                                                                { return s_.get().compare(str.s_); }
		int compare(::utility::string_t const &str) const noexcept                                                                     { return s_.get().compare(str); }
		int compare(size_type pos1, size_type count1, string_t const &str) const                                                       { return s_.get().compare(pos1, count1, str.s_); }
		int compare(size_type pos1, size_type count1, ::utility::string_t const &str) const                                            { return s_.get().compare(pos1, count1, str); }
		int compare(size_type pos1, size_type count1, string_t const &str, size_type pos2, size_type count2 = npos) const              { return s_.get().compare(pos1, count1, str.s_, pos2, count2); }
		int compare(size_type pos1, size_type count1, ::utility::string_t const &str, size_type pos2, size_type count2 = npos) const   { return s_.get().compare(pos1, count1, str, pos2, count2); }
		int compare(value_type const *cstr) const                                                                                      { return s_.get().compare(cstr); }
		int compare(size_type pos1, size_type count1, value_type const *cstr) const                                                    { return s_.get().compare(pos1, count1, cstr); }
		int compare(size_type pos1, size_type count1, value_type const *cstr, size_type count2) const                                  { return s_.get().compare(pos1, count1, cstr, count2); }
		template <typename T> int compare(T const &t) const noexcept                                                                   { return s_.get().compare(t); }
		template <typename T> int compare(size_type pos1, size_type count1, T const &t) const                                          { return s_.get().compare(pos1, count1, t); }
		template <typename T> int compare(size_type pos1, size_type count1, T const &t, size_type pos2, size_type count2 = npos) const { return s_.get().compare(pos1, count1, t, pos2, count2); }

		// starts_with
		#ifdef __cpp_lib_string_view
			bool starts_with(std::basic_string_view<value_type, traits_type> x) const noexcept { return s_.get().starts_with(x); }
		#endif
		#if __cplusplus > 201703L
			bool starts_with(value_type x) const noexcept { return s_.get().starts_with(x); }
			bool starts_with(value_type const* x) const   { return s_.get().starts_with(x); }
		#endif

		// ends_with
		#ifdef __cpp_lib_string_view
			bool ends_with(std::basic_string_view<value_type, traits_type> x) const noexcept { return s_.get().ends_with(x); }
		#endif
		#if __cplusplus > 201703L
			bool ends_with(value_type x) const noexcept { return s_.get().ends_with(x); }
			bool ends_with(value_type const *x) const   { return s_.get().ends_with(x); }
		#endif

		// replace
		string_t& replace(size_type pos, size_type count, string_t const &str)                                                       { auto s = s_.get(); s.replace(pos, count, str.s_.get());               s_ = s; return *this; }
		string_t& replace(size_type pos, size_type count, ::utility::string_t const &str)                                            { auto s = s_.get(); s.replace(pos, count, str);                        s_ = s; return *this; }
		string_t& replace(const_iterator first, const_iterator last, string_t const &str)
		{
			auto s = s_.get();
			auto iter = s.replace(s.begin() + (first - s_.get().cbegin()), s.begin() + (last - s_.get().cbegin()), str.s_.get());
			s_ = std::move(s);
			return *this;
		}
		string_t& replace(const_iterator first, const_iterator last, ::utility::string_t const &str)
		{
			auto s = s_.get();
			auto iter = s.replace(s.begin() + (first - s_.get().cbegin()), s.begin() + (last - s_.get().cbegin()), str);
			s_ = std::move(s);
			return *this;
		}
		string_t& replace(size_type pos, size_type count, string_t const &str, size_type pos2, size_type count2 = npos)              { auto s = s_.get(); s.replace(pos, count, str.s_.get(), pos2, count2); s_ = s; return *this; }
		string_t& replace(size_type pos, size_type count, ::utility::string_t const &str, size_type pos2, size_type count2 = npos)   { auto s = s_.get(); s.replace(pos, count, str, pos2, count2);          s_ = s; return *this; }
		template<typename InputIt> string_t& replace(const_iterator first, const_iterator last, InputIt first2, InputIt last2)
		{
			auto s = s_.get();
			auto iter = s.replace(s.begin() + (first - s_.get().cbegin()), s.begin() + (last - s_.get().cbegin()), first2, last2);
			s_ = std::move(s);
			return *this;
		}
		string_t& replace(size_type pos, size_type count, value_type const *cstr, size_type count2)                                  { auto s = s_.get(); s.replace(pos, count, cstr, count2);               s_ = s; return *this; }
		string_t& replace(const_iterator first, const_iterator last, value_type const *cstr, size_type count2)
		{
			auto s = s_.get();
			auto iter = s.replace(s.begin() + (first - s_.get().cbegin()), s.begin() + (last - s_.get().cbegin()), cstr, count2);
			s_ = std::move(s);
			return *this;
		}
		string_t& replace(size_type pos, size_type count, value_type const *cstr)                                                    { auto s = s_.get(); s.replace(pos, count, cstr);                       s_ = s; return *this; }
		string_t& replace(const_iterator first, const_iterator last, value_type const *cstr)
		{
			auto s = s_.get();
			auto iter = s.replace(s.begin() + (first - s_.get().cbegin()), s.begin() + (last - s_.get().cbegin()), cstr);
			s_ = std::move(s);
			return *this;
		}
		string_t& replace(size_type pos, size_type count, size_type count2, value_type ch)                                           { auto s = s_.get(); s.replace(pos, count, count2, ch);                 s_ = s; return *this; }
		string_t& replace(const_iterator first, const_iterator last, size_type count2, value_type ch)
		{
			auto s = s_.get();
			auto iter = s.replace(s.begin() + (first - s_.get().cbegin()), s.begin() + (last - s_.get().cbegin()), count2, ch);
			s_ = std::move(s);
			return *this;
		}
		string_t& replace(const_iterator first, const_iterator last, std::initializer_list<value_type> ilist)
		{
			auto s = s_.get();
			auto iter = s.replace(s.begin() + (first - s_.get().cbegin()), s.begin() + (last - s_.get().cbegin()), ilist);
			s_ = std::move(s);
			return *this;
		}
		template <typename T> string_t& replace(size_type pos, size_type count, T const &t)                                          { auto s = s_.get(); s.replace(pos, count, t);                          s_ = s; return *this; }
		template <typename T> string_t& replace(const_iterator first, const_iterator last, T const &t)
		{
			auto s = s_.get();
			auto iter = s.replace(s.begin() + (first - s_.get().cbegin()), s.begin() + (last - s_.get().cbegin()), t);
			s_ = std::move(s);
			return *this;
		}
		template <typename T> string_t& replace(size_type pos, size_type count, T const &t, size_type pos2, size_type count2 = npos) { auto s = s_.get(); s.replace(pos, count, t, pos2, count2);            s_ = s; return *this; }

		// substr
		string_t substr(size_type pos = 0, size_type count = npos) const { return string_t(s_.get().substr(pos, count)); }

		// copy
		size_type copy(value_type *dest, size_type count, size_type pos = 0) const { return s_.get().copy(dest, count, pos); }

		// resize
		void resize(size_type count)                { if (s_.get().size() != count) { auto s = s_.get(); s.resize(count); s_ = std::move(s); } }
		void resize(size_type count, value_type ch) { auto s = s_.get(); s.resize(count, ch); s_ = std::move(s); }

		// swap
		void swap(string_t &other) noexcept          { using namespace std; s_.swap(other.s_); }
		void swap(::utility::string_t &str) noexcept { auto s = std::move(str); str = s_.get(); s_ = std::move(s); }

		// Search
		// ======

		// find
		size_type find(string_t const &str, size_type pos = 0) const noexcept              { return s_.get().find(str.s_, pos); }
		size_type find(::utility::string_t const &str, size_type pos = 0) const noexcept   { return s_.get().find(str, pos); }
		size_type find(value_type const *cstr, size_type pos, size_type count) const       { return s_.get().find(cstr, pos, count); }
		size_type find(value_type const *cstr, size_type pos = 0) const                    { return s_.get().find(cstr, pos); }
		size_type find(value_type ch, size_type pos = 0) const noexcept                    { return s_.get().find(ch, pos); }
		template <typename T> size_type find(T const &t, size_type pos = 0) const noexcept { return s_.get().find(t, pos); }

		// rfind
		size_type rfind(string_t const &str, size_type pos = npos) const noexcept              { return s_.get().rfind(str.s_, pos); }
		size_type rfind(::utility::string_t const &str, size_type pos = npos) const noexcept   { return s_.get().rfind(str, pos); }
		size_type rfind(value_type const *cstr, size_type pos, size_type count) const          { return s_.get().rfind(cstr, pos, count); }
		size_type rfind(value_type const *cstr, size_type pos = npos) const                    { return s_.get().rfind(cstr, pos); }
		size_type rfind(value_type ch, size_type pos = npos) const noexcept                    { return s_.get().rfind(ch, pos); }
		template <typename T> size_type rfind(T const &t, size_type pos = npos) const noexcept { return s_.get().rfind(t, pos); }

		// find_first_of
		size_type find_first_of(string_t const &str, size_type pos = 0) const noexcept              { return s_.get().find_first_of(str.s_, pos); }
		size_type find_first_of(::utility::string_t const &str, size_type pos = 0) const noexcept   { return s_.get().find_first_of(str, pos); }
		size_type find_first_of(value_type const *cstr, size_type pos, size_type count) const       { return s_.get().find_first_of(cstr, pos, count); }
		size_type find_first_of(value_type const *cstr, size_type pos = 0) const                    { return s_.get().find_first_of(cstr, pos); }
		size_type find_first_of(value_type ch, size_type pos = 0) const noexcept                    { return s_.get().find_first_of(ch, pos); }
		template <typename T> size_type find_first_of(T const &t, size_type pos = 0) const noexcept { return s_.get().find_first_of(t, pos); }

		// find_first_not_of
		size_type find_first_not_of(string_t const &str, size_type pos = 0) const noexcept              { return s_.get().find_first_not_of(str.s_, pos); }
		size_type find_first_not_of(::utility::string_t const &str, size_type pos = 0) const noexcept   { return s_.get().find_first_not_of(str, pos); }
		size_type find_first_not_of(value_type const *cstr, size_type pos, size_type count) const       { return s_.get().find_first_not_of(cstr, pos, count); }
		size_type find_first_not_of(value_type const *cstr, size_type pos = 0) const                    { return s_.get().find_first_not_of(cstr, pos); }
		size_type find_first_not_of(value_type ch, size_type pos = 0) const noexcept                    { return s_.get().find_first_not_of(ch, pos); }
		template <typename T> size_type find_first_not_of(T const &t, size_type pos = 0) const noexcept { return s_.get().find_first_not_of(t, pos); }

		// find_last_of
		size_type find_last_of(string_t const &str, size_type pos = npos) const noexcept              { return s_.get().find_last_of(str.s_, pos); }
		size_type find_last_of(::utility::string_t const &str, size_type pos = npos) const noexcept   { return s_.get().find_last_of(str, pos); }
		size_type find_last_of(value_type const *cstr, size_type pos, size_type count) const          { return s_.get().find_last_of(cstr, pos, count); }
		size_type find_last_of(value_type const *cstr, size_type pos = npos) const                    { return s_.get().find_last_of(cstr, pos); }
		size_type find_last_of(value_type ch, size_type pos = npos) const noexcept                    { return s_.get().find_last_of(ch, pos); }
		template <typename T> size_type find_last_of(T const& t, size_type pos = npos) const noexcept { return s_.get().find_last_of(t, pos); }

		// find_last_not_of
		size_type find_last_not_of(string_t const &str, size_type pos = npos) const noexcept              { return s_.get().find_last_not_of(str.s_, pos); }
		size_type find_last_not_of(::utility::string_t const &str, size_type pos = npos) const noexcept   { return s_.get().find_last_not_of(str, pos); }
		size_type find_last_not_of(value_type const *cstr, size_type pos, size_type count) const          { return s_.get().find_last_not_of(cstr, pos, count); }
		size_type find_last_not_of(value_type const *cstr, size_type pos = npos) const                    { return s_.get().find_last_not_of(cstr, pos); }
		size_type find_last_not_of(value_type ch, size_type pos = npos) const noexcept                    { return s_.get().find_last_not_of(ch, pos); }
		template <typename T> size_type find_last_not_of(T const &t, size_type pos = npos) const noexcept { return s_.get().find_last_not_of(t, pos); }

		// Constants
		// =========

		// npos
		static const size_type npos = ::utility::string_t::npos;

		// Non-member functions
		// ====================

		// operator+
		friend string_t operator+(string_t             const &lhs, string_t             const &rhs) { return string_t(lhs.s_.get()   + rhs.s_.get()); }
		friend string_t operator+(::utility::string_t  const &lhs, string_t             const &rhs) { return string_t(lhs            + rhs.s_.get()); }
		friend string_t operator+(string_t             const &lhs, ::utility::string_t  const &rhs) { return string_t(lhs.s_.get()   + rhs); }
		friend string_t operator+(string_t             const &lhs, string_t::value_type const *rhs) { return string_t(lhs.s_.get()   + rhs); }
		friend string_t operator+(string_t             const &lhs, string_t::value_type        rhs) { return string_t(lhs.s_.get()   + rhs); }
		friend string_t operator+(string_t::value_type const *lhs, string_t             const &rhs) { return string_t(lhs            + rhs.s_.get()); }
		friend string_t operator+(string_t::value_type        lhs, string_t             const &rhs) { return string_t(lhs            + rhs.s_.get()); }
		friend string_t operator+(string_t                  &&lhs, string_t                  &&rhs) { return string_t(lhs.s_.get()   + rhs.s_.get()); }
		friend string_t operator+(::utility::string_t       &&lhs, string_t                  &&rhs) { return string_t(std::move(lhs) + rhs.s_.get()); }
		friend string_t operator+(string_t                  &&lhs, ::utility::string_t       &&rhs) { return string_t(lhs.s_.get()   + std::move(rhs)); }
		friend string_t operator+(string_t                  &&lhs, string_t             const &rhs) { return string_t(lhs.s_.get()   + rhs.s_.get()); }
		friend string_t operator+(::utility::string_t       &&lhs, string_t             const &rhs) { return string_t(std::move(lhs) + rhs.s_.get()); }
		friend string_t operator+(string_t                  &&lhs, ::utility::string_t  const &rhs) { return string_t(lhs.s_.get()   + rhs); }
		friend string_t operator+(string_t                  &&lhs, string_t::value_type const *rhs) { return string_t(lhs.s_.get()   + rhs); }
		friend string_t operator+(string_t                  &&lhs, string_t::value_type        rhs) { return string_t(lhs.s_.get()   + rhs); }
		friend string_t operator+(string_t             const &lhs, string_t                  &&rhs) { return string_t(lhs.s_.get()   + rhs.s_.get()); }
		friend string_t operator+(::utility::string_t  const &lhs, string_t                  &&rhs) { return string_t(lhs            + rhs.s_.get()); }
		friend string_t operator+(string_t             const &lhs, ::utility::string_t       &&rhs) { return string_t(lhs.s_.get()   + std::move(rhs)); }
		friend string_t operator+(string_t::value_type const *lhs, string_t                  &&rhs) { return string_t(lhs            + rhs.s_.get()); }
		friend string_t operator+(string_t::value_type        lhs, string_t                  &&rhs) { return string_t(lhs            + rhs.s_.get()); }

		// operator==, !=, <, <=, >, >=
		friend bool operator==(string_t             const &lhs, string_t             const &rhs) { return lhs.s_.get() == rhs.s_.get(); }
		friend bool operator!=(string_t             const &lhs, string_t             const &rhs) { return lhs.s_.get() != rhs.s_.get(); }
		friend bool operator< (string_t             const &lhs, string_t             const &rhs) { return lhs.s_.get() <  rhs.s_.get(); }
		friend bool operator<=(string_t             const &lhs, string_t             const &rhs) { return lhs.s_.get() <= rhs.s_.get(); }
		friend bool operator> (string_t             const &lhs, string_t             const &rhs) { return lhs.s_.get() >  rhs.s_.get(); }
		friend bool operator>=(string_t             const &lhs, string_t             const &rhs) { return lhs.s_.get() >= rhs.s_.get(); }
		friend bool operator==(::utility::string_t  const &lhs, string_t             const &rhs) { return lhs          == rhs.s_.get(); }
		friend bool operator==(string_t             const &lhs, ::utility::string_t  const &rhs) { return lhs.s_.get() == rhs; }
		friend bool operator!=(::utility::string_t  const &lhs, string_t             const &rhs) { return lhs          != rhs.s_.get(); }
		friend bool operator!=(string_t             const &lhs, ::utility::string_t  const &rhs) { return lhs.s_.get() != rhs; }
		friend bool operator< (::utility::string_t  const &lhs, string_t             const &rhs) { return lhs          <  rhs.s_.get(); }
		friend bool operator< (string_t             const &lhs, ::utility::string_t  const &rhs) { return lhs.s_.get() <  rhs; }
		friend bool operator<=(::utility::string_t  const &lhs, string_t             const &rhs) { return lhs          <= rhs.s_.get(); }
		friend bool operator<=(string_t             const &lhs, ::utility::string_t  const &rhs) { return lhs.s_.get() <= rhs; }
		friend bool operator> (::utility::string_t  const &lhs, string_t             const &rhs) { return lhs          >  rhs.s_.get(); }
		friend bool operator> (string_t             const &lhs, ::utility::string_t  const &rhs) { return lhs.s_.get() >  rhs; }
		friend bool operator>=(::utility::string_t  const &lhs, string_t             const &rhs) { return lhs          >= rhs.s_.get(); }
		friend bool operator>=(string_t             const &lhs, ::utility::string_t  const &rhs) { return lhs.s_.get() >= rhs; }
		friend bool operator==(string_t::value_type const *lhs, string_t             const &rhs) { return lhs          == rhs.s_.get(); }
		friend bool operator==(string_t             const &lhs, string_t::value_type const *rhs) { return lhs.s_.get() == rhs; }
		friend bool operator!=(string_t::value_type const *lhs, string_t             const &rhs) { return lhs          != rhs.s_.get(); }
		friend bool operator!=(string_t             const &lhs, string_t::value_type const *rhs) { return lhs.s_.get() != rhs; }
		friend bool operator< (string_t::value_type const *lhs, string_t             const &rhs) { return lhs          <  rhs.s_.get(); }
		friend bool operator< (string_t             const &lhs, string_t::value_type const *rhs) { return lhs.s_.get() <  rhs; }
		friend bool operator<=(string_t::value_type const *lhs, string_t             const &rhs) { return lhs          <= rhs.s_.get(); }
		friend bool operator<=(string_t             const &lhs, string_t::value_type const *rhs) { return lhs.s_.get() <= rhs; }
		friend bool operator> (string_t::value_type const *lhs, string_t             const &rhs) { return lhs          >  rhs.s_.get(); }
		friend bool operator> (string_t             const &lhs, string_t::value_type const *rhs) { return lhs.s_.get() >  rhs; }
		friend bool operator>=(string_t::value_type const *lhs, string_t             const &rhs) { return lhs          >= rhs.s_.get(); }
		friend bool operator>=(string_t             const &lhs, string_t::value_type const *rhs) { return lhs.s_.get() >= rhs; }

		// swap
		friend void swap(string_t            &lhs, string_t            &rhs) noexcept { lhs.swap(rhs); }
		friend void swap(::utility::string_t &lhs, string_t            &rhs) noexcept { rhs.swap(lhs); }
		friend void swap(string_t            &lhs, ::utility::string_t &rhs) noexcept { lhs.swap(rhs); }

		// erase, erase_if
		template <typename U>    friend size_type erase(string_t    &c, U const &value) { auto it = std::remove   (c.begin(), c.end(), value); auto r = std::distance(it, c.end()); c.erase(it, c.end()); return r; }
		template <typename Pred> friend size_type erase_if(string_t &c, Pred pred)      { auto it = std::remove_if(c.begin(), c.end(), pred);  auto r = std::distance(it, c.end()); c.erase(it, c.end()); return r; }

		// Input/output
		// ============

		// operator<<, >>
		friend std::basic_ostream<string_t::value_type, string_t::traits_type>& operator<<(std::basic_ostream<string_t::value_type, string_t::traits_type>& os, string_t const& str) { return os << str.s_; }
		friend std::basic_istream<string_t::value_type, string_t::traits_type>& operator>>(std::basic_istream<string_t::value_type, string_t::traits_type> &is, string_t       &str) { return is >> str.s_; }

		// getline
		friend std::basic_istream<string_t::value_type, string_t::traits_type>& getline(std::basic_istream<string_t::value_type, string_t::traits_type>  &input, string_t &str, string_t::value_type delim) { ::utility::string_t s; std::getline(input, s, delim); str.s_ = std::move(s); return input; }
		friend std::basic_istream<string_t::value_type, string_t::traits_type>& getline(std::basic_istream<string_t::value_type, string_t::traits_type> &&input, string_t &str, string_t::value_type delim) { ::utility::string_t s; std::getline(input, s, delim); str.s_ = std::move(s); return input; }
		friend std::basic_istream<string_t::value_type, string_t::traits_type>& getline(std::basic_istream<string_t::value_type, string_t::traits_type>  &input, string_t &str)                             { ::utility::string_t s; std::getline(input, s);        str.s_ = std::move(s); return input; }
		friend std::basic_istream<string_t::value_type, string_t::traits_type>& getline(std::basic_istream<string_t::value_type, string_t::traits_type> &&input, string_t &str)                             { ::utility::string_t s; std::getline(input, s);        str.s_ = std::move(s); return input; }

		// Numeric conversions
		// ===================

		// stoi, stol, stoll
		friend int       stoi (string_t const &str, string_t::size_type *pos = 0, int base = 10) { return std::stoi (str.s_, pos, base); }
		friend long      stol (string_t const &str, string_t::size_type *pos = 0, int base = 10) { return std::stol (str.s_, pos, base); }
		friend long long stoll(string_t const &str, string_t::size_type *pos = 0, int base = 10) { return std::stoll(str.s_, pos, base); }

		// stoul, stoull
		//friend unsigned int       stoui (string_t const &str, string_t::size_type *pos = 0, int base = 10) { return std::stoui (str.s_, pos, base); } // Why is there no stoui() in the standard?
		friend unsigned long      stoul (string_t const &str, string_t::size_type *pos = 0, int base = 10) { return std::stoul (str.s_, pos, base); }
		friend unsigned long long stoull(string_t const &str, string_t::size_type *pos = 0, int base = 10) { return std::stoull(str.s_, pos, base); }

		// stof, stod, stold
		friend float       stof (string_t const &str, string_t::size_type *pos = 0) { return std::stof (str.s_, pos); }
		friend double      stod (string_t const &str, string_t::size_type *pos = 0) { return std::stod (str.s_, pos); }
		friend long double stold(string_t const &str, string_t::size_type *pos = 0) { return std::stold(str.s_, pos); }

		// to_string
		#ifdef _UTF16_STRINGS
			friend string_t to_string_t(int value)                { return string_t(std::to_wstring(value)); }
			friend string_t to_string_t(long value)               { return string_t(std::to_wstring(value)); }
			friend string_t to_string_t(long long value)          { return string_t(std::to_wstring(value)); }
			friend string_t to_string_t(unsigned value)           { return string_t(std::to_wstring(value)); }
			friend string_t to_string_t(unsigned long value)      { return string_t(std::to_wstring(value)); }
			friend string_t to_string_t(unsigned long long value) { return string_t(std::to_wstring(value)); }
			friend string_t to_string_t(float value)              { return string_t(std::to_wstring(value)); }
			friend string_t to_string_t(double value)             { return string_t(std::to_wstring(value)); }
			friend string_t to_string_t(long double value)        { return string_t(std::to_wstring(value)); }
		#else
			friend string_t to_string_t(int value)                { return string_t(std::to_string(value)); }
			friend string_t to_string_t(long value)               { return string_t(std::to_string(value)); }
			friend string_t to_string_t(long long value)          { return string_t(std::to_string(value)); }
			friend string_t to_string_t(unsigned value)           { return string_t(std::to_string(value)); }
			friend string_t to_string_t(unsigned long value)      { return string_t(std::to_string(value)); }
			friend string_t to_string_t(unsigned long long value) { return string_t(std::to_string(value)); }
			friend string_t to_string_t(float value)              { return string_t(std::to_string(value)); }
			friend string_t to_string_t(double value)             { return string_t(std::to_string(value)); }
			friend string_t to_string_t(long double value)        { return string_t(std::to_string(value)); }
		#endif

		// Literals
		// ========

		// operator""_s_t
		friend string_t operator ""_s_t(string_t::value_type const *cstr, string_t::size_type len) { return string_t(cstr, len); }

		// Helper classes
		// ==============

		// std::hash and ::boost::hash
		friend struct std::hash<string_t>;
		friend std::size_t hash_value(string_t const &t) { return std::hash<::boost::flyweight<::utility::string_t>>()(t.s_); }

		// Additional cast operators into ::utility::string_t to ease switching the source code between plain ::utility::string_t and boot::flyweight<::utility::string_t>
		operator ::utility::string_t const&() const { return s_.get(); }
		//operator ::utility::string_t       ()       { return s_.get(); } // *Does* copy, only use if neccessary! Currently this commented out, as it's not needed. If activated some of the operator mutation must be commented out!

	private:
		// The encapsulated string flyweight
		::boost::flyweight<::utility::string_t> s_;
};

} // namespace odata

// std::hash
namespace std
{
	template<> struct hash<::odata::string_t> { size_t operator()(::odata::string_t const &t) const { return hash<::boost::flyweight<::utility::string_t>>()(t.s_); } };
}

#else

namespace odata
{

	typedef ::utility::string_t string_t;

	// operator""_s_t, need to offer an analogously named, forwarding literal operator:
	inline string_t operator ""_s_t(string_t::value_type const* cstr, string_t::size_type len) { return string_t(cstr, len); }

} // namespace odata

#endif

// ============================================================================================================
// End extension
// ============================================================================================================

// ============================================================================================================
// Begin extension:
// Select the way shared_ptrs are allocated:
// Contrary to the best practice we might decide here to do a separate malloc for the object and the shared_ptr
// control block. That does yield a memory saving as long as the object is not yet constructed or already deleted
// ============================================================================================================

namespace odata
{
	#ifdef _USE_SHARED_PTR_TWO_PHASE_ALLOCATION
		template<typename T, typename ... A> std::shared_ptr<T> make_shared(A&& ... a) { return std::shared_ptr<T>(new T(std::forward<A>(a) ...)); }
	#else
		template<typename T, typename ... A> std::shared_ptr<T> make_shared(A&& ... a) { return std::make_shared<T>(std::forward<A>(a) ...); }
	#endif
} // namespace odata

// ============================================================================================================
// End extension
// ============================================================================================================

namespace odata { namespace common
{

ODATACPP_CLIENT_API ::odata::string_t strip_string(::odata::string_t const &escaped);

ODATACPP_CLIENT_API void split_string(::odata::string_t const &source, ::odata::string_t const &delim, std::list<::odata::string_t> &ret);

template<typename T>
void to_string(T const &input, ::odata::string_t &result)
{
	::utility::stringstream_t ostr;
	ostr << input;
	result = ostr.str();
}

ODATACPP_CLIENT_API bool is_relative_path(::odata::string_t const &root_url, ::odata::string_t const &path);

ODATACPP_CLIENT_API ::odata::string_t print_double(double const &db, int precision = 20);
ODATACPP_CLIENT_API ::odata::string_t print_float(float const &db, int precision = 16);

}}
