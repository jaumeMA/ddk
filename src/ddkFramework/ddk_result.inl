
#include "ddk_error.h"

#if defined(DDK_DEBUG)
    #define CHECK_RESULT DDK_ASSERT(static_cast<bool>(m_checked) == true,"Unchecked result!");
    #define SET_CHECK_RESULT m_checked = true;

#else
    #define SET_CHECK_RESULT
    #define CHECK_RESULT
#endif


namespace ddk
{

template<typename Error>
result<void,Error>::result(const result_success_t&)
: m_nestedRes()
{
}
template<typename Error>
result<void,Error>::result(const Error& i_error)
: m_nestedRes(i_error)
{
}
template<typename Error>
Error result<void,Error>::error() const
{
    CHECK_RESULT

    return *m_nestedRes;
}
template<typename Error>
result<void,Error>::operator bool() const
{
    SET_CHECK_RESULT

    return m_nestedRes.empty();
}
template<typename Error>
result<void,Error>& result<void,Error>::dismiss()
{
    SET_CHECK_RESULT

    return *this;
}
template<typename Error>
const result<void,Error>& result<void,Error>::dismiss() const
{
    SET_CHECK_RESULT

    return *this;
}
template<typename Error>
bool result<void,Error>::operator==(const result_success_t&) const
{
    SET_CHECK_RESULT

    return m_nestedRes.empty();
}
template<typename Error>
bool result<void,Error>::operator!=(const result_success_t&) const
{
    SET_CHECK_RESULT

    return m_nestedRes.empty() == false;
}
template<typename Error>
bool result<void,Error>::operator==(const Error& i_error) const
{
	SET_CHECK_RESULT

	return (m_nestedRes.empty() == false) ? *m_nestedRes == i_error : false;
}
template<typename Error>
bool result<void,Error>::operator!=(const Error& i_error) const
{
	SET_CHECK_RESULT

	return (m_nestedRes.empty() == false) ? *m_nestedRes != i_error : true;
}

template<typename T, typename Error>
TEMPLATE(typename TT)
REQUIRED(IS_CONSTRUCTIBLE(T,TT))
result<T,Error>::result(TT&& i_payload)
: m_nestedRes(std::forward<TT>(i_payload))
{
}
template<typename T, typename Error>
result<T,Error>::result(const Error& i_error)
: m_nestedRes(i_error)
{
}
template<typename T, typename Error>
Error result<T,Error>::error() const
{
    CHECK_RESULT

    return m_nestedRes.template get<Error>();
}
template<typename T, typename Error>
T result<T,Error>::get() const
{
    CHECK_RESULT

    return m_nestedRes.template get<T>();
}
template<typename T, typename Error>
T result<T,Error>::extract() &&
{
    CHECK_RESULT

    return std::move(m_nestedRes).template extract<T>();
}
template<typename T, typename Error>
result<T,Error>::operator bool() const
{
    SET_CHECK_RESULT

    return m_nestedRes.template is<Error>() == false;
}
template<typename T,typename Error>
result<T,Error>& result<T,Error>::dismiss()
{
    SET_CHECK_RESULT

    return *this;
}
template<typename T,typename Error>
const result<T,Error>& result<T,Error>::dismiss() const
{
    SET_CHECK_RESULT

    return *this;
}
template<typename T, typename Error>
bool result<T,Error>::operator==(const result_success_t&) const
{
    SET_CHECK_RESULT

    return m_nestedRes.template is<Error>() == false;
}
template<typename T, typename Error>
bool result<T,Error>::operator!=(const result_success_t&) const
{
    SET_CHECK_RESULT

    return m_nestedRes.template is<Error>();
}
template<typename T,typename Error>
bool result<T,Error>::operator==(const Error& i_error) const
{
	SET_CHECK_RESULT

	return (m_nestedRes.template is<Error>()) ? m_nestedRes.template get<Error>() == i_error : false;
}
template<typename T,typename Error>
bool result<T,Error>::operator!=(const Error& i_error) const
{
	SET_CHECK_RESULT

	return (m_nestedRes.template is<Error>()) ? m_nestedRes.template get<Error>() != i_error : true;
}

}
