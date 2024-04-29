//////////////////////////////////////////////////////////////////////////////
//
// Author: Jaume Moragues
// Distributed under the GNU Lesser General Public License, Version 3.0. (See a copy
// at https://www.gnu.org/licenses/lgpl-3.0.ca.html)
//
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ddk_result_success.h"
#include "ddk_result_failure.h"
#include "ddk_variant_impl.h"
#include "ddk_optional.h"
#include "ddk_class_rules.h"

namespace ddk
{

#if defined(DDK_DEBUG)

struct result_checker
{
	result_checker(bool i_check);
	result_checker(const result_checker& other) = default;
	result_checker(result_checker&& other);
	~result_checker();

	result_checker& operator=(bool i_checked);
	result_checker& operator=(const result_checker& other);
	result_checker& operator=(result_checker&& other);
	operator bool() const;

	bool _checked = false;
};

#endif

template<typename,typename>
class result;

template<typename Error>
class result<void,Error>
{
	template<typename,typename>
	friend class result;

#if defined(DDK_DEBUG)
	friend inline void set_checked(const result& i_result)
	{
		i_result.m_checked = true;
	}
#endif

public:
	typedef void payload_t;
	typedef Error error_t;

	constexpr result(const result_success_t&);
	constexpr result(const Error& i_error);
	constexpr result(Error&& i_error);
	TEMPLATE(typename EError)
	REQUIRES(IS_CONSTRUCTIBLE(Error,EError))
	constexpr result(const result<void,EError>& i_result);
	TEMPLATE(typename EError)
	REQUIRES(IS_CONSTRUCTIBLE(Error,EError))
	constexpr result(result<void,EError>&& i_result);
	constexpr result(const result& other) = default;
	constexpr result(result&& other) = default;
	constexpr result& operator=(const result& other) = default;
	constexpr result& operator=(result&& other) = default;
	constexpr const Error& error() const&;
	constexpr Error error() &&;
	constexpr explicit operator bool() const;
	constexpr result& dismiss();
	constexpr const result& dismiss() const;
	constexpr bool operator==(const result_success_t&) const;
	constexpr bool operator!=(const result_success_t&) const;
	constexpr bool operator==(const Error& i_error) const;
	constexpr bool operator!=(const Error& i_error) const;

private:
	optional<Error> m_nestedRes;

#if defined(DDK_DEBUG)
	mutable result_checker m_checked = false;
#endif
};

template<typename T, typename Error>
class result : contravariant_rules<T>
{
	template<typename,typename>
	friend class result;

#if defined(DDK_DEBUG)
	friend inline void set_checked(const result& i_result)
	{
		i_result.m_checked = true;
	}
#endif

	friend std::ostringstream& operator<<(std::ostringstream& o_stream, const result<T,Error>& i_result)
	{
		if (i_result)
		{
			o_stream << "Success: " << i_result.get();
		}
		else
		{
			o_stream << "Error: " << i_result.error();
		}

		return o_stream;
	}
	friend std::string operator<<(const std::string& i_str, const result<T,Error>& i_result)
	{
		std::ostringstream res(i_str);

		if (i_result)
		{
			res << "Success " << i_result.get();
		}
		else
		{
			res << "Error: " << i_result.error();
		}

		return res.str();
	}

public:
	typedef T payload_t;
	typedef Error error_t;
	typedef T value;
	typedef typename embedded_type<T>::ref_type reference;
	typedef typename embedded_type<T>::cref_type const_reference;
	typedef typename embedded_type<T>::rref_type rreference;
	typedef typename embedded_type<T>::pointer_type pointer;
	typedef typename embedded_type<T>::cpointer_type const_pointer;

	TEMPLATE(typename ... Args)
	REQUIRES(IS_CONSTRUCTIBLE(T,Args...))
	constexpr result(Args&& ... i_args);
	constexpr result(const Error& i_error);
	constexpr result(Error&& i_error);
	TEMPLATE(typename TT, typename EError)
	REQUIRES(IS_CONSTRUCTIBLE(T,TT),IS_CONSTRUCTIBLE(Error,EError))
	constexpr result(const result<TT,EError>& i_result);
	TEMPLATE(typename TT, typename EError)
	REQUIRES(IS_CONSTRUCTIBLE(T,TT),IS_CONSTRUCTIBLE(Error,EError))
	constexpr result(result<TT,EError>&& i_result);
	constexpr result(const result& other) = default;
	constexpr result(result&& other) = default;
	constexpr result& operator=(const result& other) = default;
	constexpr result& operator=(result&& other) = default;
	constexpr const Error& error() const&;
	constexpr Error error() &&;
	constexpr const_reference get() const;
	constexpr reference get();
	constexpr const_pointer operator->() const;
	constexpr pointer operator->();
	constexpr value extract() &&;
	constexpr explicit operator bool() const;
	constexpr result& dismiss();
	constexpr const result& dismiss() const;
	constexpr bool operator==(const result_success_t&) const;
	constexpr bool operator!=(const result_success_t&) const;
	constexpr bool operator==(const Error& i_error) const;
	constexpr bool operator!=(const Error& i_error) const;

private:
	detail::variant_impl<T,Error> m_nestedRes;

#if defined(DDK_DEBUG)
	mutable result_checker m_checked = false;
#endif
};

template<typename Result, typename TT,typename EError>
constexpr inline Result make_result(const result<TT,EError>& i_result);
template<typename Result,typename TT,typename EError>
constexpr inline Result make_result(result<TT,EError>&& i_result);
template<typename Result>
constexpr inline Result make_result(const result_success_t& i_success);
TEMPLATE(typename Result, typename ... Args)
REQUIRES(IS_CONSTRUCTIBLE(typename Result::payload_t,Args...))
constexpr inline Result make_result(Args&& ... i_args);
template<typename Result,typename TT,typename EError>
constexpr inline Result make_error(const result<TT,EError>& i_result);
template<typename Result,typename TT,typename EError>
constexpr inline Result make_error(result<TT,EError>&& i_result);

}

#include "ddk_result.inl"