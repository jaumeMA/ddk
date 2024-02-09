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
    constexpr error_impl(const Error& i_error);
	constexpr error_impl(const Error& i_error, const std::string& i_errorDesc);
	constexpr error_impl(const error_impl&) = default;
	constexpr error_impl(error_impl&&) = default;
	constexpr error_impl& operator=(const error_impl&) = default;
	constexpr error_impl& operator=(error_impl&&) = default;
	constexpr Error get_error() const;
	constexpr const std::string& what() const;
	constexpr bool operator==(const error_impl<Error>& i_error) const;
	constexpr bool operator!=(const error_impl<Error>& i_error) const;

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

	constexpr error_impl(const error_impl&) = default;
	constexpr error_impl(error_impl&&) = default;
	constexpr error_impl& operator=(error_impl&&) = default;
	constexpr error_impl& operator=(const error_impl&) = default;
    template<typename NestedError>
	constexpr error_impl(const Error& i_errorCode, NestedError&& i_nestedError);
	template<typename NestedError>
	constexpr error_impl(const Error& i_errorCode,const std::string& i_errorDesc,NestedError&& i_nestedError);
	template<typename NestedError>
	constexpr const NestedError& get_nested_error() const;
    template<typename Visitor>
	constexpr typename mpl::remove_qualifiers<Visitor>::return_type visit(Visitor&& i_visitor) const;

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

	constexpr error(const error&) = default;
    constexpr error& operator=(const error&) = default;

};

template<typename Result, typename ... Args>
constexpr Result make_error(Args&& ... i_args);

}

#include "ddk_error.inl"
