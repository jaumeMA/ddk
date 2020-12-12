
#if defined(DDK_DEBUG)
    #define SET_CHECK_RESULT(_VALUE) m_checked = _VALUE;
    #define CHECK_RESULT DDK_ASSERT(m_checked==true, "Unchecked result on destroy!");
#else
    #define SET_CHECK_RESULT(_VALUE) 
    #define CHECK_RESULT
#endif


namespace ddk
{

template<typename Error>
result<void,Error>::result(const result_success_t&)
: m_nestedRes()
#if defined(DDK_DEBUG)
, m_checked(false)
#endif
{
}
template<typename Error>
result<void,Error>::result(const Error& i_error)
: m_nestedRes(i_error)
{
}
template<typename Error>
result<void,Error>::result(const result& other)
: m_nestedRes(other.m_nestedRes)
#if defined(DDK_DEBUG)
, m_checked(false)
#endif
{
}
template<typename Error>
result<void,Error>::result(result&& other)
: m_nestedRes(std::move(other.m_nestedRes))
#if defined(DDK_DEBUG)
, m_checked(other.m_checked)
#endif
{
#if defined(DDK_DEBUG)
    other.m_checked = true;
#endif
}
template<typename Error>
result<void,Error>::~result()
{
    CHECK_RESULT
}
template<typename Error>
result<void,Error>& result<void,Error>::operator=(const result& other)
{
    CHECK_RESULT

    m_nestedRes = other.m_nestedRes;
    m_checked = false;

    return *this;
}
template<typename Error>
result<void,Error>& result<void,Error>::operator=(result&& other)
{
    CHECK_RESULT

    m_nestedRes = std::move(other.m_nestedRes);
    m_checked = false;
    std::swap(m_checked,other.m_checked);

    return *this;
}
template<typename Error>
Error result<void,Error>::error() const
{
    SET_CHECK_RESULT(true)

    return *m_nestedRes;
}
template<typename Error>
result<void,Error>::operator bool() const
{
    SET_CHECK_RESULT(true)

    return m_nestedRes.empty();
}
template<typename Error>
bool result<void,Error>::operator==(const result_success_t&) const
{
    SET_CHECK_RESULT(true)

    return m_nestedRes.empty();
}
template<typename Error>
bool result<void,Error>::operator!=(const result_success_t&) const
{
    SET_CHECK_RESULT(true)

    return m_nestedRes.empty() == false;
}
template<typename Error>
bool result<void,Error>::operator==(const Error& i_error) const
{
	SET_CHECK_RESULT(true)
		
	return (m_nestedRes.empty() == false) ? *m_nestedRes == i_error : false;
}
template<typename Error>
bool result<void,Error>::operator!=(const Error& i_error) const
{
	SET_CHECK_RESULT(true)
		
	return (m_nestedRes.empty() == false) ? *m_nestedRes != i_error : true;
}

template<typename T, typename Error>
result<T,Error>::result(const T& i_payload)
: m_nestedRes(i_payload)
{
}
template<typename T, typename Error>
result<T,Error>::result(T&& i_payload)
: m_nestedRes(std::move(i_payload))
#if defined(DDK_DEBUG)
, m_checked(false)
#endif
{
}
template<typename T, typename Error>
result<T,Error>::result(const Error& i_error)
: m_nestedRes(i_error)
#if defined(DDK_DEBUG)
, m_checked(false)
#endif
{
}
template<typename T, typename Error>
result<T,Error>::result(const result& other)
: m_nestedRes(other.m_nestedRes)
#if defined(DDK_DEBUG)
, m_checked(false)
#endif
{
}
template<typename T, typename Error>
result<T,Error>::result(result&& other)
: m_nestedRes(std::move(other.m_nestedRes))
#if defined(DDK_DEBUG)
, m_checked(other.m_checked)
#endif
{
#if defined(DDK_DEBUG)
    other.m_checked = true;
#endif
}
template<typename T, typename Error>
result<T,Error>::~result()
{
    CHECK_RESULT
}
template<typename T, typename Error>
result<T,Error>& result<T,Error>::operator=(const result& other)
{
    CHECK_RESULT
    SET_CHECK_RESULT(false)

    m_nestedRes = other.m_nestedRes;

    return *this;
}
template<typename T, typename Error>
result<T,Error>& result<T,Error>::operator=(result&& other)
{
	CHECK_RESULT
#if defined(DDK_DEBUG)
    m_checked = true;
    std::swap(m_checked,other.m_checked);
#endif

    m_nestedRes = std::move(other.m_nestedRes);

    return *this;
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
    SET_CHECK_RESULT(true)

    return m_nestedRes.template is<Error>() == false;
}
template<typename T, typename Error>
bool result<T,Error>::operator==(const result_success_t&) const
{
    SET_CHECK_RESULT(true)

    return m_nestedRes.template is<Error>() == false;
}
template<typename T, typename Error>
bool result<T,Error>::operator!=(const result_success_t&) const
{
    SET_CHECK_RESULT(true)

    return m_nestedRes.template is<Error>();
}
template<typename T,typename Error>
bool result<T,Error>::operator==(const Error& i_error) const
{
	SET_CHECK_RESULT(true)
		
	return (m_nestedRes.template is<Error>()) ? m_nestedRes.template get<Error>() == i_error : false;
}
template<typename T,typename Error>
bool result<T,Error>::operator!=(const Error& i_error) const
{
	SET_CHECK_RESULT(true)
		
	return (m_nestedRes.template is<Error>()) ? m_nestedRes.template get<Error>() != i_error : true;
}

}
