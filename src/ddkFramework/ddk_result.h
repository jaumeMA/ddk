#pragma once

#include "ddk_variant.h"
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

struct result_success_t{};
const result_success_t success = result_success_t();

template<typename,typename>
class result;

template<typename Error>
class result<void,Error>
{
#if defined(DDK_DEBUG)
	friend inline void set_checked(const result& i_result)
	{
		i_result.m_checked = true;
	}
#endif

	friend std::ostringstream& operator<<(std::ostringstream& o_stream, const result<void,Error>& i_result)
	{
		if (i_result)
		{
			o_stream << "Success";
		}
		else
		{
			o_stream << "Error: " << i_result.error();
		}

		return o_stream;
	}
	friend std::string operator<<(const std::string& i_str, const result<void,Error>& i_result)
	{
		std::ostringstream res(i_str);

		if (i_result)
		{
			res << "Success";
		}
		else
		{
			res << "Error: " << i_result.error();
		}

		return res.str();
	}

public:
	typedef void payload_t;
	typedef Error error_t;

	result(const result_success_t&);
	result(const Error& i_error);
	result(const result& other) = default;
	result(result&& other) = default;
	result& operator=(const result& other) = default;
	result& operator=(result&& other) = default;
	const Error& error() const;
	explicit operator bool() const;
	result& dismiss();
	const result& dismiss() const;
	bool operator==(const result_success_t&) const;
	bool operator!=(const result_success_t&) const;
	bool operator==(const Error& i_error) const;
	bool operator!=(const Error& i_error) const;

private:
	optional<Error> m_nestedRes;

#if defined(DDK_DEBUG)
	mutable result_checker m_checked = false;
#endif
};

template<typename T, typename Error>
class result : contravariant_rules<T>
{
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

	TEMPLATE(typename TT)
	REQUIRES(IS_CONSTRUCTIBLE(T,TT))
	result(TT&& i_payload);
	result(const Error& i_error);
	result(const result& other) = default;
	result(result&& other) = default;
	result& operator=(const result& other) = default;
	result& operator=(result&& other) = default;
	const Error& error() const;
	const_reference get() const;
	reference get();
	const_pointer operator->() const;
	pointer operator->();
	value extract() &&;
	explicit operator bool() const;
	result& dismiss();
	const result& dismiss() const;
	bool operator==(const result_success_t&) const;
	bool operator!=(const result_success_t&) const;
	bool operator==(const Error& i_error) const;
	bool operator!=(const Error& i_error) const;

private:
	variant<T,Error> m_nestedRes;

#if defined(DDK_DEBUG)
	mutable result_checker m_checked = false;
#endif
};


template<typename Result>
inline Result make_result()
{
	return  Result(success);
}
template<typename Result, typename ... Args>
inline Result make_result(Args&& ... i_args)
{
	return  typename Result::payload_t(std::forward<Args>(i_args) ...);
}

}

#include "ddk_result.inl"