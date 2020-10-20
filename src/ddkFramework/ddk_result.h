#pragma once

#include "ddk_variant.h"
#include "ddk_optional.h"
#include "ddk_macros.h"
#include "ddk_formatter.h"
#include "ddk_error.h"

namespace ddk
{

struct result_success_t{};
const result_success_t success = result_success_t();

template<typename,typename>
class result;

template<typename Error>
class result<void,Error>
{
	friend std::ostringstream& operator<<(std::ostringstream& o_stream, const result<void,Error>& i_result)
	{
		if (i_result.hasError())
		{
			o_stream << "Error: " << i_result.getError();
		}
		else
		{
			o_stream << "Success";
		}

		return o_stream;
	}
	friend std::string operator<<(const std::string& i_str, const result<void,Error>& i_result)
	{
		std::string res;

		if (i_result.hasError())
		{
			res = "Error: " << i_result.getError();
		}
		else
		{
			res = "Success";
		}

		return res;
	}

public:
	typedef void payload_t;
	typedef Error error_t;

	result(const result_success_t&);
	result(const Error& i_error);
	result(const result& other);
	result(result&& other);
	~result();
	result& operator=(const result& other);
	result& operator=(result&& other);
	Error error() const;
	void clear();
	explicit operator bool() const;
	bool operator==(const result_success_t&) const;
	bool operator!=(const result_success_t&) const;

private:
	optional<Error> m_nestedRes;

#if defined(DDK_DEBUG)
	mutable bool m_checked = false;
#endif
};

template<typename T, typename Error>
class result
{
	friend std::ostringstream& operator<<(std::ostringstream& o_stream, const result<T,Error>& i_result)
	{
		if (i_result.hasError())
		{
			o_stream << "Error: " << i_result.getError();
		}
		else
		{
			o_stream << "Success: " << i_result.getT();
		}

		return o_stream;
	}
	friend std::string operator<<(const std::string& i_str, const result<T,Error>& i_result)
	{
		std::ostringstream res;

		if (i_result.hasError())
		{
			res << "Error: " << i_result.getError();
		}
		else
		{
			res << "Success " << i_result.getT();
		}

		return res.str();
	}

public:
	typedef T payload_t;
	typedef Error error_t;

	result(const T& i_payload);
	result(T&& i_payload);
	result(const Error& i_error);
	result(const result& other);
	result(result&& other);
	~result();
	result& operator=(const result& other);
	result& operator=(result&& other);
	Error error() const;
	T get() const;
	T extract();
	void clear();
	explicit operator bool() const;
	bool operator==(const result_success_t&) const;
	bool operator!=(const result_success_t&) const;

private:
	variant<T,Error> m_nestedRes;

#if defined(DDK_DEBUG)
	mutable bool m_checked = false;
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
