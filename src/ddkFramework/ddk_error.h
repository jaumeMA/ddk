//////////////////////////////////////////////////////////////////////////////
//
// Author: Jaume Moragues
// Distributed under the GNU Lesser General Public License, Version 3.0. (See a copy
// at https://www.gnu.org/licenses/lgpl-3.0.ca.html)
//
//////////////////////////////////////////////////////////////////////////////

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
	typedef variant<NestedErrors...> nested_error;

public:
	using error_impl<Error>::error_impl;
	using error_impl<Error>::operator==;
	using error_impl<Error>::operator!=;

	constexpr error_impl(const error_impl&) = default;
	constexpr error_impl(error_impl&&) = default;
	constexpr error_impl& operator=(error_impl&&) = default;
	constexpr error_impl& operator=(const error_impl&) = default;
	template<typename ... Args>
	constexpr error_impl(const Error& i_errorCode, Args&& ... i_args);
	template<typename ... Args>
	constexpr error_impl(const Error& i_errorCode,const std::string& i_errorDesc,Args&& ... i_args);
	template<typename NestedError>
	constexpr const NestedError& get_nested_error() const;
    template<typename Visitor>
	constexpr typename mpl::remove_qualifiers<Visitor>::return_type visit(Visitor&& i_visitor) const;

private:
	nested_error m_nestedErrors;
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
