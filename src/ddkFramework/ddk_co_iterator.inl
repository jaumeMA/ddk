
#include "ddk_sync_executor.h"

namespace ddk
{

template<typename Iterable>
typename Iterable::reference visit_iterator_awaitable(Iterable& i_iterable, const co_forward_iterator_context& i_context)
{
	auto&& itNext = std::begin(i_iterable);

	std::next(itNext, i_context.get_curr_index());

	if(itNext != std::end(i_iterable))
	{
		do
		{
			yield(*itNext);

			if (i_context.go_forward())
			{
				itNext = std::next(itNext);
			}
			else
			{
				DDK_FAIL("No movement indicated");
			}
		}
		while (itNext != std::end(i_iterable));
	}

	suspend();

	return ddk::crash_on_return<typename Iterable::reference>::value();
}
template<typename Iterable>
typename Iterable::reference visit_iterator_awaitable(Iterable& i_iterable, const co_bidirectional_iterator_context& i_context)
{
	auto&& itNext = std::begin(i_iterable);

	std::next(itNext,i_context.get_curr_index());

	if (itNext != std::end(i_iterable))
	{
		do
		{
			yield(*itNext);

			if (i_context.go_backward())
			{
				itNext = std::next(itNext);
			}
			else if(i_context.go_backward())
			{
				itNext = std::prev(itNext);
			}
			else
			{
				DDK_FAIL("No movement indicated");
			}
		}
		while (itNext != std::end(i_iterable));
	}

	suspend();

	return ddk::crash_on_return<typename Iterable::reference>::value();
}
template<typename Iterable>
typename Iterable::reference visit_iterator_awaitable(Iterable& i_iterable, const co_random_access_iterator_context& i_context)
{
	auto&& itNext = std::begin(i_iterable) + i_context.get_curr_index();

	if (itNext != std::end(i_iterable))
	{
		do
		{
			yield(*itNext);

			const size_t shift = i_context.shift();

			if (shift != 0)
			{
				itNext = std::next(itNext,shift);
			}
			else
			{
				DDK_FAIL("No movement indicated");
			}
		} 
		while (itNext != std::end(i_iterable));
	}

	suspend();

	return ddk::crash_on_return<typename Iterable::reference>::value();
}

template<typename T>
co_forward_iterator<T>::co_forward_iterator(const detail::none_t&)
{
}
template<typename T>
co_forward_iterator<T>::co_forward_iterator(const co_forward_iterator& other)
: m_function(other.m_function)
, m_context(other.m_context)
{
	typedef typename async_execute_interface<T>::start_result start_result;

	m_executor = make_async_executor(m_function,&m_context) -> attach(this_fiber);

	start_result execRes = m_executor->execute();

	DDK_ASSERT(execRes.hasError() == false, "Error while executing iterator");
}
template<typename T>
template<typename Iterable>
co_forward_iterator<T>::co_forward_iterator(Iterable& i_iterable, typename std::enable_if<is_co_iterator<Iterable>::value==false>::type*)
: m_function([&i_iterable](const co_forward_iterator_context& i_context) -> reference { return visit_iterator_awaitable(i_iterable, i_context);  })
{
	typedef typename async_execute_interface<T>::start_result start_result;

	m_executor = make_async_executor(m_function,&m_context) -> attach(this_fiber);

	start_result execRes = m_executor->execute();

	DDK_ASSERT(execRes.hasError() == false, "Error while executing iterator");
}
template<typename T>
typename co_forward_iterator<T>::reference co_forward_iterator<T>::operator*()
{
	DDK_ASSERT(m_executor != nullptr, "Dereferencing void iterator");

	return m_executor->get_value();
}
template<typename T>
typename co_forward_iterator<T>::const_reference co_forward_iterator<T>::operator*() const
{
	DDK_ASSERT(m_executor != nullptr, "Dereferencing void iterator");

	return m_executor->get_value();
}
template<typename T>
co_forward_iterator<T>& co_forward_iterator<T>::operator++()
{
	DDK_ASSERT(m_executor != nullptr, "Dereferencing void iterator");

	typedef typename async_execute_interface<T>::start_result start_result;
	typedef typename start_result::error_t start_error;

	m_context.incr();

	start_result execRes = m_executor->execute();

	if (execRes.hasError())
	{
		start_error execError = execRes.getError();

		if (execError == async_execute_interface<T>::AlreadyDone)
		{
			m_executor.clear();
			m_context.reject();
		}
	}
	else
	{
		m_context.accept();
	}

	return *this;
}
template<typename T>
co_forward_iterator<T>& co_forward_iterator<T>::operator++(int)
{
	DDK_ASSERT(m_executor != nullptr, "Dereferencing void iterator");

	typedef typename async_execute_interface<T>::start_result start_result;
	typedef typename start_result::error_t start_error;

	co_iterator<T> res = *this;

	m_context.incr();

	start_result execRes = m_executor->execute();

	if (execRes.hasError())
	{
		start_error execError = execRes.getError();

		if (execError == async_execute_interface<T>::AlreadyDone)
		{
			m_executor.clear();
			m_context.reject();
		}
	}
	else
	{
		m_context.accept();
	}

	return res;
}
template<typename T>
co_forward_iterator<T>& co_forward_iterator<T>::operator=(const co_forward_iterator& other)
{
	typedef typename async_execute_interface<T>::start_result start_result;

	m_function = other.m_function;

	m_context = other.m_context;
		
	m_executor = make_async_executor(m_function,&m_context) -> attach(this_fiber);

	start_result execRes = m_executor->execute();

	DDK_ASSERT(execRes.hasError() == false, "Error while executing iterator");

	return *this;
}
template<typename T>
bool co_forward_iterator<T>::operator!=(const co_forward_iterator<T>& other) const
{
	return m_context.get_current() != other.m_context.get_current();
}
template<typename T>
bool co_forward_iterator<T>::operator==(const co_forward_iterator<T>& other) const
{
	m_context.get_current() == other.m_context.get_current();
}

template<typename T>
co_random_access_iterator<T>::co_random_access_iterator(const detail::none_t&)
{
}
template<typename T>
co_random_access_iterator<T>::co_random_access_iterator(const co_random_access_iterator& other)
: m_function(other.m_function)
, m_context(other.m_context)
{
	typedef typename async_execute_interface<T>::start_result start_result;

	m_executor = make_async_executor(m_function,&m_context)->attach(this_fiber);

	start_result execRes = m_executor->execute();

	DDK_ASSERT(execRes.hasError() == false, "Error while executing iterator");
}
template<typename T>
typename co_random_access_iterator<T>::reference co_random_access_iterator<T>::operator*()
{
	DDK_ASSERT(m_executor != nullptr, "Dereferencing void iterator");

	return m_executor->get_value();
}
template<typename T>
typename co_random_access_iterator<T>::const_reference co_random_access_iterator<T>::operator*() const
{
	DDK_ASSERT(m_executor != nullptr, "Dereferencing void iterator");

	return m_executor->get_value();
}
template<typename T>
co_random_access_iterator<T>& co_random_access_iterator<T>::operator++()
{
	DDK_ASSERT(m_executor != nullptr, "Dereferencing void iterator");

	typedef typename async_execute_interface<T>::start_result start_result;
	typedef typename start_result::error_t start_error;

	m_context.incr();

	start_result execRes = m_executor->execute();

	if (execRes.hasError())
	{
		start_error execError = execRes.getError();

		if (execError == async_execute_interface<T>::AlreadyDone)
		{
			m_context.close();
		}
		else
		{
			m_context.reject();
		}
	}
	else
	{
		m_context.accept();
	}

	return *this;
}
template<typename T>
co_random_access_iterator<T>& co_random_access_iterator<T>::operator++(int)
{
	DDK_ASSERT(m_executor != nullptr, "Dereferencing void iterator");

	typedef typename async_execute_interface<T>::start_result start_result;
	typedef typename start_result::error_t start_error;

	co_iterator<T> res = *this;

	m_context.incr();

	start_result execRes = m_executor->execute();

	if (execRes.hasError())
	{
		start_error execError = execRes.getError();

		if (execError == async_execute_interface<T>::AlreadyDone)
		{
			m_executor.clear();
			m_context.reject();
		}
	}
	else
	{
		m_context.accept();
	}

	return res;
}
template<typename T>
co_random_access_iterator<T>& co_random_access_iterator<T>::operator=(const co_random_access_iterator& other)
{
	typedef typename async_execute_interface<T>::start_result start_result;

	m_function = other.m_function;

	m_context = other.m_context;

	m_executor = make_async_executor(m_function,&m_context)->attach(this_fiber);

	start_result execRes = m_executor->execute();

	DDK_ASSERT(execRes.hasError() == false, "Error while executing iterator");

	return *this;
}
template<typename T>
bool co_random_access_iterator<T>::operator!=(const co_random_access_iterator<T>& other) const
{
	return m_context.get_current() != other.m_context.get_current();
}
template<typename T>
bool co_random_access_iterator<T>::operator==(const co_random_access_iterator<T>& other) const
{
	return m_context.get_current() == other.m_context.get_current();
}
template<typename T>
template<typename Iterable>
co_random_access_iterator<T>::co_random_access_iterator(Iterable& i_iterable, typename std::enable_if<is_co_iterator<Iterable>::value == false>::type*)
: m_function([&i_iterable](const co_random_access_iterator_context& i_context) -> reference { return visit_iterator_awaitable(i_iterable, i_context);  })
, m_context(0)
{
	typedef typename async_execute_interface<T>::start_result start_result;

	m_executor = make_async_executor(m_function,&m_context)->attach(this_fiber);

	start_result execRes = m_executor->execute();

	DDK_ASSERT(execRes.hasError() == false, "Error while executing iterator");
}

}