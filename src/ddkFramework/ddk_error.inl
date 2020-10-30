
namespace ddk
{
namespace detail
{

template<typename Error>
error_impl<Error>::error_impl(const Error& i_errorCode)
: m_errorCode(i_errorCode)
{
}
template<typename Error>
error_impl<Error>::error_impl(const typename Error::__val__& i_errorCode)
: m_errorCode(i_errorCode)
{
}
template<typename Error>
error_impl<Error>::error_impl(const Error& i_errorCode, const std::string& i_errorDesc)
: m_errorCode(i_errorCode)
, m_errorDesc(i_errorDesc)
{
}
template<typename Error>
error_impl<Error>::error_impl(const typename Error::__val__& i_errorCode,const std::string& i_errorDesc)
: m_errorCode(i_errorCode)
,m_errorDesc(i_errorDesc)
{
}
template<typename Error>
Error error_impl<Error>::get_code() const
{
    return m_errorCode;
}
template<typename Error>
std::string error_impl<Error>::get_description() const
{
    return m_errorDesc;
}
template<typename Error>
bool error_impl<Error>::operator==(const error_impl<Error>& i_error) const
{
    return m_errorCode == i_error.m_errorCode;
}
template<typename Error>
bool error_impl<Error>::operator!=(const error_impl<Error>& i_error) const
{
    return m_errorCode == i_error.m_errorCode;
}

template<typename Error, typename ... NestedErrors>
template<typename NestedError>
error_impl<Error,NestedErrors...>::error_impl(const Error& i_errorCode, NestedError&& i_nestedError)
: error_impl<Error>(i_errorCode)
, m_nestedErrors(i_nestedError)
{
}
template<typename Error, typename ... NestedErrors>
template<typename NestedError>
error_impl<Error,NestedErrors...>::error_impl(const Error& i_errorCode, const std::string& i_errorDesc, NestedError&& i_nestedError)
: error_impl<Error>(i_errorCode,i_errorDesc)
, m_nestedErrors(i_nestedError)
{
}
template<typename Error, typename ... NestedErrors>
template<typename NestedError>
const NestedError& error_impl<Error,NestedErrors...>::get_nested_error() const
{
    return m_nestedErrors.template get<NestedError>();
}
template<typename Error, typename ... NestedErrors>
template<typename Visitor>
typename Visitor::result_type error_impl<Error,NestedErrors...>::visit(Visitor&& i_visitor)
{
    return m_nestedErrors.visit(i_visitor);
}

}

template<typename Result, typename ... Args>
Result make_error(Args&& ... i_args)
{
    return Result(typename Result::error_t(std::forward<Args>(i_args) ...));
}

}
