#pragma once

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
    error_impl(const Error& i_errorCode);
    error_impl(const Error& i_errorCode, const std::string& i_errorDesc);
    error_impl(const error_impl&) = default;
    error_impl& operator=(const error_impl&) = default;
    Error get_code() const;
    std::string get_description() const;
    bool operator==(const Error& i_error) const;
    bool operator!=(const Error& i_error) const;

private:
    Error m_errorCode;
    std::string m_errorDesc;
};

template<typename Error, typename ... NestedErrors>
class error_impl : public error_impl<Error>
{
public:
    error_impl(const error_impl&) = default;
    error_impl& operator=(const error_impl&) = default;
    template<typename NestedError>
    error_impl(const Error& i_errorCode, NestedError&& i_nestedError);
    template<typename NestedError>
    error_impl(const Error& i_errorCode, const std::string& i_errorDesc, NestedError&& i_nestedError);
    template<typename NestedError>
    const NestedError& get_nested_error() const;
    template<typename Visitor>
    typename Visitor::result_type visit(Visitor&& i_visitor);

private:
    variant<NestedErrors...> m_nestedErrors;
};

}

template<typename Error, typename ... NestedErrors>
class error : public detail::error_impl<Error,NestedErrors...>
{
public:
    using detail::error_impl<Error,NestedErrors...>::error_impl;
    error(const error&) = default;
    error& operator=(const error&) = default;
};

template<typename Result, typename ... Args>
Result make_error(Args&& ... i_args);

}

#include "ddk_error.inl"
