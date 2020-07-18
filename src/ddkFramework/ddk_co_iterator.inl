
#include "ddk_sync_executor.h"

namespace ddk
{

template<typename T>
co_iterator<T>::co_iterator(const detail::none_t&)
: m_currIndex(npos)
{
}
template<typename T>
co_iterator<T>::co_iterator(const co_iterator& other)
: m_function(other.m_function)
, m_currIndex(other.m_currIndex)
{
	typedef typename async_execute_interface<T>::start_result start_result;

	m_executor = make_async_executor(m_function,m_currIndex) -> attach(this_fiber);

	start_result execRes = m_executor->execute();

	DDK_ASSERT(execRes.hasError() == false, "Error while executing iterator");
}
template<typename T>
co_iterator<T>::co_iterator(const std::function<reference(size_t)>& i_function)
: m_function(i_function)
, m_currIndex(0)
{
	typedef typename async_execute_interface<T>::start_result start_result;

	m_executor = make_async_executor(m_function,m_currIndex) -> attach(this_fiber);

	start_result execRes = m_executor->execute();

	DDK_ASSERT(execRes.hasError() == false, "Error while executing iterator");
}
template<typename T>
typename co_iterator<T>::reference co_iterator<T>::operator*()
{
	DDK_ASSERT(m_executor != nullptr, "Dereferencing void iterator");

	return m_executor->get_value();
}
template<typename T>
typename co_iterator<T>::const_reference co_iterator<T>::operator*() const
{
	DDK_ASSERT(m_executor != nullptr, "Dereferencing void iterator");

	return m_executor->get_value();
}
template<typename T>
co_iterator<T>& co_iterator<T>::operator++()
{
	DDK_ASSERT(m_executor != nullptr, "Dereferencing void iterator");

	typedef typename async_execute_interface<T>::start_result start_result;
	typedef typename start_result::error_t start_error;

	start_result execRes = m_executor->execute();

	if (execRes.hasError())
	{
		start_error execError = execRes.getError();

		if (execError == async_execute_interface<T>::AlreadyDone)
		{
			m_executor.clear();
			m_currIndex = npos;
		}
	}
	else
	{
		++m_currIndex;
	}

	return *this;
}
template<typename T>
co_iterator<T>& co_iterator<T>::operator++(int)
{
	DDK_ASSERT(m_executor != nullptr, "Dereferencing void iterator");

	typedef typename async_execute_interface<T>::start_result start_result;
	typedef typename start_result::error_t start_error;

	co_iterator<T> res = *this;

	start_result execRes = m_executor->execute();

	if (execRes.hasError())
	{
		start_error execError = execRes.getError();

		if (execError == async_execute_interface<T>::AlreadyDone)
		{
			m_executor.clear();
			m_currIndex = npos;
		}
	}
	else
	{
		++m_currIndex;
	}

	return res;
}
template<typename T>
co_iterator<T>& co_iterator<T>::operator=(const co_iterator& other)
{
	typedef typename async_execute_interface<T>::start_result start_result;

	m_function = other.m_function;

	m_currIndex = other.m_currIndex;
		
	m_executor = make_async_executor(m_function,m_currIndex) -> attach(this_fiber);

	start_result execRes = m_executor->execute();

	DDK_ASSERT(execRes.hasError() == false, "Error while executing iterator");

	return *this;
}
template<typename T>
bool co_iterator<T>::operator!=(const co_iterator<T>& other) const
{
	return m_currIndex != other.m_currIndex;
}
template<typename T>
bool co_iterator<T>::operator==(const co_iterator<T>& other) const
{
	m_currIndex == other.m_currIndex;
}

}