#pragma once

#include "ddk_variant_impl.h"
#include <sstream>

namespace ddk
{
namespace detail
{

template<typename,typename...>
class error_impl;

template<typename Error>
class error_impl<Error>
{
	friend inline std::ostringstream& operator<<(std::ostringstream& i_lhs, const error_impl& i_rhs)
	{
		i_lhs << i_rhs.m_errorDesc;

		return i_lhs;
	}
	friend inline std::string operator<<(const std::string& i_lhs,const error_impl& i_rhs)
	{
		std::ostringstream res(i_lhs);

		res << i_rhs.m_errorDesc;

		return res.str();
	}

public:
    error_impl(const Error& i_error);
	error_impl(const Error& i_error, const std::string& i_errorDesc);
	error_impl(const error_impl&) = default;
	error_impl(error_impl&&) = default;
	error_impl& operator=(const error_impl&) = default;
	error_impl& operator=(error_impl&&) = default;
	Error get_error() const;
    const std::string& what() const;
	bool operator==(const error_impl<Error>& i_error) const;
	bool operator!=(const error_impl<Error>& i_error) const;

private:
    Error m_error;
    std::string m_errorDesc;
};

template<typename Error, typename ... NestedErrors>
class error_impl : public error_impl<Error>
{
	friend inline std::ostringstream& operator<<(std::ostringstream& i_lhs,const error_impl& i_rhs)
	{
		visit([&i_lhs](auto&& i_error)
		{
			i_lhs << i_error;
		},i_rhs.m_nestedErrors);

		return i_lhs;
	}
	friend inline std::string operator<<(const std::string& i_lhs,const error_impl& i_rhs)
	{
		std::ostringstream res(i_lhs);

		visit([&res](auto&& i_error)
		{
			res << i_error;
		},i_rhs.m_nestedErrors);

		return res.str();
	}

public:
	using error_impl<Error>::error_impl;
	using error_impl<Error>::operator==;
	using error_impl<Error>::operator!=;

	error_impl(const error_impl&) = default;
	error_impl(error_impl&&) = default;
	error_impl& operator=(error_impl&&) = default;
	error_impl& operator=(const error_impl&) = default;
    template<typename NestedError>
    error_impl(const Error& i_errorCode, NestedError&& i_nestedError);
	template<typename NestedError>
	error_impl(const Error& i_errorCode,const std::string& i_errorDesc,NestedError&& i_nestedError);
	template<typename NestedError>
    const NestedError& get_nested_error() const;
    template<typename Visitor>
    typename mpl::remove_qualifiers<Visitor>::return_type visit(Visitor&& i_visitor) const;

private:
    variant_impl<NestedErrors...> m_nestedErrors;
};

}

template<typename Error, typename ... NestedErrors>
class error : public detail::error_impl<Error,NestedErrors...>
{
public:
	typedef Error error_code;
	using detail::error_impl<Error,NestedErrors...>::error_impl;
	using detail::error_impl<Error,NestedErrors...>::operator==;
	using detail::error_impl<Error,NestedErrors...>::operator!=;

	error(const error&) = default;
    error& operator=(const error&) = default;

};

template<typename Result, typename ... Args>
Result make_error(Args&& ... i_args);

}

#include "ddk_error.inl"
