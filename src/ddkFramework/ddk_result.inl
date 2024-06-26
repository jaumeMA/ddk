
#include "ddk_error.h"

#if defined(DDK_DEBUG)
    #define CHECK_RESULT(_OBJ) DDK_ASSERT(static_cast<bool>((_OBJ).m_checked) == true,"Unchecked result!");
    #define SET_CHECK_RESULT(_OBJ) (_OBJ).m_checked = true;

#else
    #define CHECK_RESULT(_OBJ)
    #define SET_CHECK_RESULT(_OBJ)
#endif


namespace ddk
{

template<typename Error>
constexpr result<void,Error>::result(const result_success_t&)
: m_nestedRes()
{
}
template<typename Error>
constexpr result<void,Error>::result(const Error& i_error)
: m_nestedRes(i_error)
{
}
template<typename Error>
constexpr result<void,Error>::result(Error&& i_error)
: m_nestedRes(std::move(i_error))
{
}
template<typename Error>
TEMPLATE(typename EError)
REQUIRED(IS_CONSTRUCTIBLE(Error,EError))
constexpr result<void,Error>::result(const result<void,EError>& i_result)
: m_nestedRes(i_result.m_nestedRes)
{
}
template<typename Error>
TEMPLATE(typename EError)
REQUIRED(IS_CONSTRUCTIBLE(Error,EError))
constexpr result<void,Error>::result(result<void,EError>&& i_result)
: m_nestedRes(std::move(i_result.m_nestedRes))
{
    SET_CHECK_RESULT(i_result)
}
template<typename Error>
constexpr const Error& result<void,Error>::error() const &
{
    CHECK_RESULT(*this)

    return m_nestedRes.get();
}
template<typename Error>
constexpr Error result<void,Error>::error() &&
{
    CHECK_RESULT(*this)

    return std::move(m_nestedRes).extract();
}
template<typename Error>
constexpr result<void,Error>::operator bool() const
{
    SET_CHECK_RESULT(*this)

    return m_nestedRes.empty();
}
template<typename Error>
constexpr result<void,Error>& result<void,Error>::dismiss()
{
    SET_CHECK_RESULT(*this)

    return *this;
}
template<typename Error>
constexpr const result<void,Error>& result<void,Error>::dismiss() const
{
    SET_CHECK_RESULT(*this)

    return *this;
}
template<typename Error>
constexpr bool result<void,Error>::operator==(const result_success_t&) const
{
    SET_CHECK_RESULT(*this)

    return m_nestedRes.empty();
}
template<typename Error>
constexpr bool result<void,Error>::operator!=(const result_success_t&) const
{
    SET_CHECK_RESULT(*this)

    return m_nestedRes.empty() == false;
}
template<typename Error>
constexpr bool result<void,Error>::operator==(const Error& i_error) const
{
	SET_CHECK_RESULT(*this)

	return (m_nestedRes.empty() == false) ? *m_nestedRes == i_error : false;
}
template<typename Error>
constexpr bool result<void,Error>::operator!=(const Error& i_error) const
{
	SET_CHECK_RESULT(*this)

	return (m_nestedRes.empty() == false) ? *m_nestedRes != i_error : true;
}

template<typename T, typename Error>
TEMPLATE(typename ... Args)
REQUIRED(IS_CONSTRUCTIBLE(T,Args...))
constexpr result<T,Error>::result(Args&& ... i_args)
: m_nestedRes(mpl::static_number<0>{},std::forward<Args>(i_args)...)
{
}
template<typename T, typename Error>
constexpr result<T,Error>::result(const Error& i_error)
: m_nestedRes(mpl::static_number<1>{},i_error)
{
}
template<typename T,typename Error>
constexpr result<T,Error>::result(Error&& i_error)
: m_nestedRes(mpl::static_number<1>{},std::move(i_error))
{
}
template<typename T,typename Error>
TEMPLATE(typename TT,typename EError)
REQUIRED(IS_CONSTRUCTIBLE(T,TT),IS_CONSTRUCTIBLE(Error,EError))
constexpr result<T,Error>::result(const result<TT,EError>& i_result)
: m_nestedRes(i_result.m_nestedRes)
{
}
template<typename T,typename Error>
TEMPLATE(typename TT,typename EError)
REQUIRED(IS_CONSTRUCTIBLE(T,TT),IS_CONSTRUCTIBLE(Error,EError))
constexpr result<T,Error>::result(result<TT,EError>&& i_result)
: m_nestedRes(std::move(i_result.m_nestedRes))
{
    SET_CHECK_RESULT(i_result)
}
template<typename T, typename Error>
constexpr const Error& result<T,Error>::error() const &
{
    CHECK_RESULT(*this)

    return m_nestedRes.template get<Error>();
}
template<typename T,typename Error>
constexpr Error result<T,Error>::error() &&
{
    CHECK_RESULT(*this)

    return std::move(m_nestedRes).template extract<Error>();
}
template<typename T,typename Error>
constexpr typename result<T,Error>::const_pointer result<T,Error>::operator->() const
{
    CHECK_RESULT(*this)

    return m_nestedRes.template get_ptr<T>();
}
template<typename T,typename Error>
constexpr typename result<T,Error>::pointer result<T,Error>::operator->()
{
    CHECK_RESULT(*this)

    return m_nestedRes.template get_ptr<T>();
}
template<typename T, typename Error>
constexpr typename result<T,Error>::const_reference result<T,Error>::get() const
{
    CHECK_RESULT(*this)

    return m_nestedRes.template get<T>();
}
template<typename T,typename Error>
constexpr typename result<T,Error>::reference result<T,Error>::get()
{
    CHECK_RESULT(*this)

    return m_nestedRes.template get<T>();
}
template<typename T, typename Error>
constexpr typename result<T,Error>::value result<T,Error>::extract() &&
{
    CHECK_RESULT(*this)

    return std::move(m_nestedRes).template extract<T>();
}
template<typename T, typename Error>
constexpr result<T,Error>::operator bool() const
{
    SET_CHECK_RESULT(*this)

    return m_nestedRes.template is<Error>() == false;
}
template<typename T,typename Error>
constexpr result<T,Error>& result<T,Error>::dismiss()
{
    SET_CHECK_RESULT(*this)

    return *this;
}
template<typename T,typename Error>
constexpr const result<T,Error>& result<T,Error>::dismiss() const
{
    SET_CHECK_RESULT(*this)

    return *this;
}
template<typename T, typename Error>
constexpr bool result<T,Error>::operator==(const result_success_t&) const
{
    SET_CHECK_RESULT(*this)

    return m_nestedRes.template is<Error>() == false;
}
template<typename T, typename Error>
constexpr bool result<T,Error>::operator!=(const result_success_t&) const
{
    SET_CHECK_RESULT(*this)

    return m_nestedRes.template is<Error>();
}
template<typename T,typename Error>
constexpr bool result<T,Error>::operator==(const Error& i_error) const
{
	SET_CHECK_RESULT(*this)

	return (m_nestedRes.template is<Error>()) ? m_nestedRes.template get<Error>() == i_error : false;
}
template<typename T,typename Error>
constexpr bool result<T,Error>::operator!=(const Error& i_error) const
{
	SET_CHECK_RESULT(*this)

	return (m_nestedRes.template is<Error>()) ? m_nestedRes.template get<Error>() != i_error : true;
}

template<typename Result,typename TT,typename EError>
constexpr Result make_result(const result<TT,EError>& i_result)
{
    if constexpr (std::is_same<TT,void>::value)
    {
        return { success };
    }
    else
    {
        return { i_result.get() };
    }
}
template<typename Result,typename TT,typename EError>
constexpr Result make_result(result<TT,EError>&& i_result)
{
    if constexpr (std::is_same<TT,void>::value)
    {
        return { success };
    }
    else
    {
        return { std::move(i_result).extract() };
    }
}
template<typename Result>
constexpr Result make_result(const result_success_t& i_success)
{
    return { i_success };
}
TEMPLATE(typename Result,typename ... Args)
REQUIRED(IS_CONSTRUCTIBLE(typename Result::payload_t,Args...))
constexpr Result make_result(Args&& ... i_args)
{
    return  Result{ std::forward<Args>(i_args)... };
}
template<typename Result,typename TT,typename EError>
constexpr Result make_error(const result<TT,EError>& i_result)
{
    typedef typename Result::error_t error_t;

    return Result{ error_t{ i_result.error() } };
}
template<typename Result,typename TT,typename EError>
constexpr Result make_error(result<TT,EError>&& i_result)
{
    typedef typename Result::error_t error_t;

    return Result{ error_t{ std::move(i_result).error() } };
}

}
