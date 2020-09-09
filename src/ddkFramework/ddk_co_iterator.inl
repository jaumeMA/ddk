
#include "ddk_sync_executor.h"
#include "ddk_co_iterator_allocator_info.h"

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

				break;
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

				break;
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
	const size_t currPos = i_context.get_curr_index();
	auto&& itNext = (currPos != detail::co_forward_iterator_context_impl::npos) ? std::begin(i_iterable) + i_context.get_curr_index() : std::end(i_iterable);

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

				break;
			}
		}
		while (itNext != std::end(i_iterable));
	}

	suspend();

	return ddk::crash_on_return<typename Iterable::reference>::value();
}

//co forward iterator impl

template<typename T>
co_forward_iterator<T>::co_forward_iterator(const detail::none_t&)
{
}
template<typename T>
co_forward_iterator<T>::co_forward_iterator(const co_forward_iterator& other)
: m_function(other.m_function)
, m_context(other.m_context)
, m_caller(other.m_caller)
{
	typedef typename async_execute_interface<T>::start_result start_result;

	m_executor = make_async_executor(m_function(&m_context)) -> attach(m_caller);

	start_result execRes = m_executor->execute();

	DDK_ASSERT(execRes.hasError() == false || execRes.getError() == async_execute_interface<T>::AlreadyDone, "Error while executing iterator");
}
template<typename T>
template<typename Iterable>
co_forward_iterator<T>::co_forward_iterator(Iterable& i_iterable, typename std::enable_if<is_co_iterator<Iterable>::value==false>::type*)
: m_function([&i_iterable](const co_forward_iterator_context& i_context) -> reference { return visit_iterator_awaitable(i_iterable, i_context);  })
, m_context(0)
, m_caller(make_stack_allocator<typename co_iterator_allocator_info<Iterable>::allocator>(),co_iterator_allocator_info<Iterable>::s_max_num_pages)
{
	typedef typename async_execute_interface<T>::start_result start_result;
	typedef typename start_result::error_t start_error;

	m_executor = make_async_executor(m_function(&m_context)) -> attach(m_caller);

	const start_result execRes = m_executor->execute();

    if (execRes.hasError())
    {
        const start_error execError = execRes.getError();

        if (execError == async_execute_interface<T>::AlreadyDone)
        {
            m_context.close();
        }
    }
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

	const start_result execRes = m_executor->execute();

	if (execRes.hasError())
	{
		const start_error execError = execRes.getError();

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
co_forward_iterator<T> co_forward_iterator<T>::operator++(int)
{
	DDK_ASSERT(m_executor != nullptr, "Dereferencing void iterator");

	typedef typename async_execute_interface<T>::start_result start_result;
	typedef typename start_result::error_t start_error;

	co_forward_iterator<T> res = *this;

	m_context.incr();

	const start_result execRes = m_executor->execute();

	if (execRes.hasError())
	{
		const start_error execError = execRes.getError();

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

	return res;
}
template<typename T>
co_forward_iterator<T>& co_forward_iterator<T>::operator=(const co_forward_iterator& other)
{
	typedef typename async_execute_interface<T>::start_result start_result;

	m_function = other.m_function;

	m_context = other.m_context;

	m_caller = other.m_caller;

	m_executor = make_async_executor(m_function(&m_context)) -> attach(m_caller);

	const start_result execRes = m_executor->execute();

	DDK_ASSERT(execRes.hasError() == false || execRes.getError() == async_execute_interface<T>::AlreadyDone, "Error while executing iterator");

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
	return m_context.get_current() == other.m_context.get_current();
}

//co bidirectional iterator impl
template<typename T>
co_bidirectional_iterator<T>::co_bidirectional_iterator(const detail::none_t&)
{
}
template<typename T>
co_bidirectional_iterator<T>::co_bidirectional_iterator(const co_bidirectional_iterator& other)
: m_function(other.m_function)
, m_context(other.m_context)
, m_caller(other.m_caller)
{
	typedef typename async_execute_interface<T>::start_result start_result;

	m_executor = make_async_executor(m_function(&m_context))->attach(m_caller);

	const start_result execRes = m_executor->execute();

	DDK_ASSERT(execRes.hasError() == false || execRes.getError() == async_execute_interface<T>::AlreadyDone, "Error while executing iterator");
}
template<typename T>
template<typename Iterable>
co_bidirectional_iterator<T>::co_bidirectional_iterator(Iterable& i_iterable, typename std::enable_if<is_co_iterator<Iterable>::value == false>::type*)
: m_function([&i_iterable](const co_bidirectional_iterator_context& i_context) -> reference { return visit_iterator_awaitable(i_iterable, i_context);  })
, m_context(0)
, m_caller(make_stack_allocator<typename co_iterator_allocator_info<Iterable>::allocator>(),co_iterator_allocator_info<Iterable>::s_max_num_pages)
{
	typedef typename async_execute_interface<T>::start_result start_result;
	typedef typename start_result::error_t start_error;

	m_executor = make_async_executor(m_function(&m_context))->attach(m_caller);

	const start_result execRes = m_executor->execute();

    if (execRes.hasError())
    {
        const start_error execError = execRes.getError();

        if (execError == async_execute_interface<T>::AlreadyDone)
        {
            m_context.close();
        }
    }
}
template<typename T>
typename co_bidirectional_iterator<T>::reference co_bidirectional_iterator<T>::operator*()
{
	DDK_ASSERT(m_executor != nullptr, "Dereferencing void iterator");

	return m_executor->get_value();
}
template<typename T>
typename co_bidirectional_iterator<T>::const_reference co_bidirectional_iterator<T>::operator*() const
{
	DDK_ASSERT(m_executor != nullptr, "Dereferencing void iterator");

	return m_executor->get_value();
}
template<typename T>
co_bidirectional_iterator<T>& co_bidirectional_iterator<T>::operator++()
{
	DDK_ASSERT(m_executor != nullptr, "Dereferencing void iterator");

	typedef typename async_execute_interface<T>::start_result start_result;
	typedef typename start_result::error_t start_error;

	m_context.incr();

	const start_result execRes = m_executor->execute();

	if (execRes.hasError())
	{
		const start_error execError = execRes.getError();

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
co_bidirectional_iterator<T> co_bidirectional_iterator<T>::operator++(int)
{
	typedef typename async_execute_interface<T>::start_result start_result;
	typedef typename start_result::error_t start_error;

	DDK_ASSERT(m_executor != nullptr, "Dereferencing void iterator");

	co_bidirectional_iterator<T> res = *this;

	m_context.incr();

	const start_result execRes = m_executor->execute();

	if (execRes.hasError())
	{
		const start_error execError = execRes.getError();

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

	return res;
}
template<typename T>
co_bidirectional_iterator<T>& co_bidirectional_iterator<T>::operator--()
{
	DDK_ASSERT(m_executor != nullptr, "Dereferencing void iterator");

	typedef typename async_execute_interface<T>::start_result start_result;
	typedef typename start_result::error_t start_error;

	m_context.decr();

	const start_result execRes = m_executor->execute();

	if (execRes.hasError())
	{
		const start_error execError = execRes.getError();

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
co_bidirectional_iterator<T> co_bidirectional_iterator<T>::operator--(int)
{
	DDK_ASSERT(m_executor != nullptr, "Dereferencing void iterator");

	typedef typename async_execute_interface<T>::start_result start_result;
	typedef typename start_result::error_t start_error;

	co_bidirectional_iterator<T> res = *this;

	m_context.decr();

	const start_result execRes = m_executor->execute();

	if (execRes.hasError())
	{
		const start_error execError = execRes.getError();

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

	return res;
}
template<typename T>
co_bidirectional_iterator<T>& co_bidirectional_iterator<T>::operator=(const co_bidirectional_iterator& other)
{
	typedef typename async_execute_interface<T>::start_result start_result;

	m_function = other.m_function;

	m_context = other.m_context;

    m_caller = other.m_caller;

	m_executor = make_async_executor(m_function(&m_context))->attach(m_caller);

	const start_result execRes = m_executor->execute();

	DDK_ASSERT(execRes.hasError() == false || execRes.getError() == async_execute_interface<T>::AlreadyDone, "Error while executing iterator");

	return *this;
}
template<typename T>
bool co_bidirectional_iterator<T>::operator!=(const co_bidirectional_iterator<T>& other) const
{
	return m_context.get_current() != other.m_context.get_current();
}
template<typename T>
bool co_bidirectional_iterator<T>::operator==(const co_bidirectional_iterator<T>& other) const
{
	return m_context.get_current() == other.m_context.get_current();
}

template<typename T>
co_random_access_iterator<T>::co_random_access_iterator(const detail::none_t&)
{
}
template<typename T>
co_random_access_iterator<T>::co_random_access_iterator(const co_random_access_iterator& other)
: m_function(other.m_function)
, m_context(other.m_context)
, m_caller(other.m_caller)
{
	typedef typename async_execute_interface<T>::start_result start_result;

	m_executor = make_async_executor(m_function(&m_context)) -> attach(m_caller);

	const start_result execRes = m_executor->execute();

	DDK_ASSERT(execRes.hasError() == false || execRes.getError() == async_execute_interface<T>::AlreadyDone, "Error while executing iterator");
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

	const start_result execRes = m_executor->execute();

	if (execRes.hasError())
	{
		const start_error execError = execRes.getError();

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
co_random_access_iterator<T> co_random_access_iterator<T>::operator++(int)
{
	DDK_ASSERT(m_executor != nullptr, "Dereferencing void iterator");

	typedef typename async_execute_interface<T>::start_result start_result;
	typedef typename start_result::error_t start_error;

	co_random_access_iterator<T> res = *this;

	m_context.incr();

	const start_result execRes = m_executor->execute();

	if (execRes.hasError())
	{
		const start_error execError = execRes.getError();

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

	return res;
}
template<typename T>
co_random_access_iterator<T>& co_random_access_iterator<T>::operator--()
{
	DDK_ASSERT(m_executor != nullptr, "Dereferencing void iterator");

	typedef typename async_execute_interface<T>::start_result start_result;
	typedef typename start_result::error_t start_error;

	m_context.decr();

	const start_result execRes = m_executor->execute();

	if (execRes.hasError())
	{
		const start_error execError = execRes.getError();

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
co_random_access_iterator<T> co_random_access_iterator<T>::operator--(int)
{
	DDK_ASSERT(m_executor != nullptr, "Dereferencing void iterator");

	typedef typename async_execute_interface<T>::start_result start_result;
	typedef typename start_result::error_t start_error;

	co_random_access_iterator<T> res = *this;

	m_context.decr();

	const start_result execRes = m_executor->execute();

	if (execRes.hasError())
	{
		const start_error execError = execRes.getError();

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

	return res;
}
template<typename T>
co_random_access_iterator<T> co_random_access_iterator<T>::operator+(int i_shift)
{
	DDK_ASSERT(m_executor != nullptr, "Dereferencing void iterator");

	typedef typename async_execute_interface<T>::start_result start_result;
	typedef typename start_result::error_t start_error;

	co_random_access_iterator<T> res = *this;

	if(i_shift != 0)
	{
		res.m_context.shift(i_shift);

		const start_result execRes = res.m_executor->execute();

		if (execRes.hasError())
		{
			const start_error execError = execRes.getError();

			if (execError == async_execute_interface<T>::AlreadyDone)
			{
				res.m_context.close();
			}
			else
			{
				res.m_context.reject();
			}
		}
		else
		{
			res.m_context.accept();
		}
	}

	return res;
}
template<typename T>
co_random_access_iterator<T>& co_random_access_iterator<T>::operator[](size_t i_absPos)
{
	DDK_ASSERT(m_executor != nullptr, "Dereferencing void iterator");

	typedef typename async_execute_interface<T>::start_result start_result;
	typedef typename start_result::error_t start_error;

	if(i_absPos != m_context.get_current())
	{
		m_context.set_abs(i_absPos);

		const start_result execRes = m_executor->execute();

		if (execRes.hasError())
		{
			const start_error execError = execRes.getError();

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
	}

	return *this;
}
template<typename T>
co_random_access_iterator<T>& co_random_access_iterator<T>::operator=(const co_random_access_iterator& other)
{
	typedef typename async_execute_interface<T>::start_result start_result;

	m_function = other.m_function;

	m_context = other.m_context;

	m_caller = other.m_caller;

    m_executor = make_async_executor(m_function(&m_context)) -> attach(m_caller);

    const start_result execRes = m_executor->execute();

    DDK_ASSERT(execRes.hasError() == false || execRes == async_execute_interface<T>::AlreadyDone, "Error while executing iterator");

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
, m_caller(make_stack_allocator<typename co_iterator_allocator_info<Iterable>::allocator>(),co_iterator_allocator_info<Iterable>::s_max_num_pages)
{
	typedef typename async_execute_interface<T>::start_result start_result;
	typedef typename start_result::error_t start_error;

	m_executor = make_async_executor(m_function(&m_context))->attach(m_caller);

	const start_result execRes = m_executor->execute();

    if (execRes.hasError())
    {
        const start_error execError = execRes.getError();

        if (execError == async_execute_interface<T>::AlreadyDone)
        {
            m_context.close();
        }
    }
}

}
