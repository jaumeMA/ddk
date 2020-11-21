
#include "ddk_sync_executor.h"

namespace ddk
{

template<typename T>
awaited_result<T>::awaited_result(const detail::none_t&)
{
}
template<typename T>
template<typename Arg>
awaited_result<T>::awaited_result(Arg&& i_arg)
{
	m_content.template construct<T>(std::forward<Arg>(i_arg));
}
template<typename T>
awaited_result<T>::awaited_result(awaited_result&& other)
{
	if(other.m_content.empty() == false)
	{
		m_content.template construct<T>(other.m_content.template extract<T>());
	}
}
template<typename T>
awaited_result<T>::~awaited_result()
{
	if(m_content.empty() == false)
	{
        m_content.template destroy<T>();
	}
}
template<typename T>
awaited_result<T>& awaited_result<T>::operator=(awaited_result<T>&& other)
{
	if(other.m_content.empty() == false)
	{
	    if(m_content.empty())
	    {
            m_content.template construct<T>(other.m_content.template extract<T>());
	    }
        else
        {
            m_content.template assign<T>(other.m_content.template extract<T>());
        }
	}
    else if(m_content.empty() == false)
    {
        m_content.template destroy<T>();
    }

	return *this;
}
template<typename T>
typename awaited_result<T>::const_reference awaited_result<T>::get() const
{
	return m_content.template get<T>();
}
template<typename T>
typename awaited_result<T>::const_pointer awaited_result<T>::get_ptr() const
{
	return m_content.template get_ptr<T>();
}
template<typename T>
typename awaited_result<T>::reference awaited_result<T>::get()
{
	return m_content.template get<T>();
}
template<typename T>
typename awaited_result<T>::pointer awaited_result<T>::get_ptr()
{
	return m_content.template get_ptr<T>();
}
template<typename T>
awaited_result<T>::operator bool() const
{
	return m_content.empty() == false;
}
template<typename T>
awaited_result<T>::operator const_reference() const
{
	return m_content.template get<T>();
}
template<typename T>
awaited_result<T>::operator reference()
{
	return m_content.template get<T>();
}
template<typename T>
void awaited_result<T>::set(reference i_content)
{
	m_content.template assign<T>(std::forward<reference>(i_content));
}

template<typename T, typename Result>
awaitable<T,Result>::awaitable(const function<T()>& i_function, const detail::this_thread_t& i_thread)
: m_executor(i_function)
{
}
template<typename T, typename Result>
awaitable<T,Result>::awaitable(const awaitable& other)
: m_executor(other.m_executor)
{
}
template<typename T, typename Result>
awaitable<T,Result>::awaitable(awaitable&& other)
: m_executor(std::move(other.m_executor))
{
}
template<typename T, typename Result>
Result awaitable<T,Result>::resume()
{
	Result res;

	m_executor.execute(make_function(&res,&Result::set));

	return res;
}

template<typename Result>
awaitable<void,Result>::awaitable(const ddk::function<void()>& i_function, const detail::this_thread_t& i_thread)
: m_executor(make_function([i_function]() { i_function(); return _void; }))
{
}

}
