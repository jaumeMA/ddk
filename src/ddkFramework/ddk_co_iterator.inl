
#include "ddk_iterable_visitor.h"
#include "ddk_co_iterator_allocator_info.h"
#include "ddk_container_exceptions.h"

namespace ddk
{

//co forward iterator impl
template<typename T>
co_forward_iterator<T>::co_forward_iterator(const detail::none_t&)
: m_currState(iter::iterable_state::npos)
{
}
template<typename T>
co_forward_iterator<T>::co_forward_iterator(const co_forward_iterator& other)
: m_function(other.m_function)
, m_currState(other.m_currState)
, m_caller(other.m_caller)
{
	m_executor = make_async_executor(m_function(iter::go_to_place(static_cast<int>(m_currState.position())),make_function(this,&co_forward_iterator<T>::acquire_iterable_value))) -> attach(m_caller);

    if(m_executor->execute() != success)
    {
		m_currState.reset();
	}
}
template<typename T>
template<typename Iterable>
co_forward_iterator<T>::co_forward_iterator(Iterable& i_iterable, typename std::enable_if<is_co_iterator<Iterable>::value==false>::type*)
: m_function([&i_iterable](const iter::shift_action& i_initialAction, const function<iter::forward_action(reference)>& i_sink) -> reference { return visit_iterator(i_iterable,i_sink,i_initialAction); })
, m_caller(make_stack_allocator<typename co_iterator_allocator_info<Iterable>::allocator>(),co_iterator_allocator_info<Iterable>::s_max_num_pages)
{
	m_executor = make_async_executor(m_function(iter::go_to_place(static_cast<int>(m_currState.position())),make_function(this,&co_forward_iterator<T>::acquire_iterable_value))) -> attach(m_caller);

    if (m_executor->execute() != success)
    {
		m_currState.reset();
	}
}
template<typename T>
typename co_forward_iterator<T>::reference co_forward_iterator<T>::operator*()
{
    if(m_executor)
    {
        return m_executor->get_value();
    }
    else
    {
        throw bad_access_exception{"Acessing empty iterator"};
    }
}
template<typename T>
typename co_forward_iterator<T>::const_reference co_forward_iterator<T>::operator*() const
{
    if(m_executor)
    {
        return m_executor->get_value();
    }
    else
    {
        throw bad_access_exception{"Acessing empty iterator"};
    }
}
template<typename T>
co_forward_iterator<T>& co_forward_iterator<T>::operator++()
{
	DDK_ASSERT(m_executor != nullptr, "Dereferencing void iterator");

    m_currAction.set(iter::go_next_place);

	if (m_executor->execute() != success)
	{
        m_currState.reset();
    }

	return *this;
}
template<typename T>
co_forward_iterator<T> co_forward_iterator<T>::operator++(int)
{
	DDK_ASSERT(m_executor != nullptr, "Dereferencing void iterator");

	const co_forward_iterator<T> res = *this;

    m_currAction.set(iter::go_next_place);

	if (m_executor->execute() != success)
	{
        m_currState.reset();
    }

	return res;
}
template<typename T>
co_forward_iterator<T>& co_forward_iterator<T>::operator=(const co_forward_iterator& other)
{
	typedef typename async_execute_interface<T>::start_result start_result;

	m_function = other.m_function;

	m_currState = other.m_currState;

	m_caller = other.m_caller;

	m_executor = make_async_executor(m_function(iter::go_to_place(static_cast<int>(m_currState.position())),make_function(this,&co_forward_iterator<T>::acquire_iterable_value))) -> attach(m_caller);

	const start_result execRes = m_executor->execute();

	DDK_ASSERT(execRes == success || execRes.error() == async_execute_interface<T>::AlreadyDone, "Error while executing iterator");

	return *this;
}
template<typename T>
bool co_forward_iterator<T>::operator!=(const co_forward_iterator<T>& other) const
{
	return m_currState != other.m_currState;
}
template<typename T>
bool co_forward_iterator<T>::operator==(const co_forward_iterator<T>& other) const
{
	return m_currState == other.m_currState;
}
template<typename T>
iter::forward_action co_forward_iterator<T>::acquire_iterable_value(reference i_value)
{
	if(iter::action_result actionRes = iter::iterable_state::forward_result())
	{
		m_currAction.set(actionRes.get());

		yield(i_value);
	}

    return m_currAction.get();
}

//co bidirectional iterator impl
template<typename T>
co_bidirectional_iterator<T>::co_bidirectional_iterator(const detail::none_t&)
: m_currState(iter::iterable_state::npos)
{
}
template<typename T>
co_bidirectional_iterator<T>::co_bidirectional_iterator(const co_bidirectional_iterator& other)
: m_function(other.m_function)
, m_caller(other.m_caller)
{
	m_executor = make_async_executor(m_function(iter::go_to_place(static_cast<int>(m_currState.position())),make_function(this,&co_bidirectional_iterator<T>::acquire_iterable_value))) -> attach(m_caller);

    if(m_executor->execute() != success)
    {
		m_currState.reset();
	}
}
template<typename T>
template<typename Iterable>
co_bidirectional_iterator<T>::co_bidirectional_iterator(Iterable& i_iterable, typename std::enable_if<is_co_iterator<Iterable>::value == false>::type*)
: m_function([&i_iterable](const iter::shift_action& i_initialAction, const function<iter::bidirectional_action(reference)>& i_sink) -> reference { return visit_iterator(i_iterable,i_sink,i_initialAction); })
, m_caller(make_stack_allocator<typename co_iterator_allocator_info<Iterable>::allocator>(),co_iterator_allocator_info<Iterable>::s_max_num_pages)
{
	m_executor = make_async_executor(m_function(iter::go_to_place(static_cast<int>(m_currState.position())),make_function(this,&co_bidirectional_iterator<T>::acquire_iterable_value))) -> attach(m_caller);

    if (m_executor->execute() != success)
    {
		m_currState.reset();
	}
}
template<typename T>
typename co_bidirectional_iterator<T>::reference co_bidirectional_iterator<T>::operator*()
{
    if(m_executor)
    {
        return m_executor->get_value();
    }
    else
    {
        throw bad_access_exception{"Acessing empty iterator"};
    }
}
template<typename T>
typename co_bidirectional_iterator<T>::const_reference co_bidirectional_iterator<T>::operator*() const
{
    if(m_executor)
    {
        return m_executor->get_value();
    }
    else
    {
        throw bad_access_exception{"Acessing empty iterator"};
    }
}
template<typename T>
co_bidirectional_iterator<T>& co_bidirectional_iterator<T>::operator++()
{
	DDK_ASSERT(m_executor != nullptr, "Dereferencing void iterator");

    m_currAction.set(iter::go_next_place);

	if (m_executor->execute() != success)
	{
        m_currState.reset();
    }

	return *this;
}
template<typename T>
co_bidirectional_iterator<T> co_bidirectional_iterator<T>::operator++(int)
{
	DDK_ASSERT(m_executor != nullptr, "Dereferencing void iterator");

	co_bidirectional_iterator<T> res = *this;

    m_currAction.set(iter::go_next_place);

	if (m_executor->execute() != success)
	{
        m_currState.reset();
    }

	return res;
}
template<typename T>
co_bidirectional_iterator<T>& co_bidirectional_iterator<T>::operator--()
{
	DDK_ASSERT(m_executor != nullptr, "Dereferencing void iterator");

    m_currAction.set(iter::go_prev_place);

	if (m_executor->execute() != success)
	{
        m_currState.reset();
    }

	return *this;
}
template<typename T>
co_bidirectional_iterator<T> co_bidirectional_iterator<T>::operator--(int)
{
	DDK_ASSERT(m_executor != nullptr, "Dereferencing void iterator");

	co_bidirectional_iterator<T> res = *this;

    m_currAction.set(iter::go_prev_place);

	if (m_executor->execute() != success)
	{
        m_currState.reset();
    }

	return res;
}
template<typename T>
co_bidirectional_iterator<T>& co_bidirectional_iterator<T>::operator=(const co_bidirectional_iterator& other)
{
	typedef typename async_execute_interface<T>::start_result start_result;

	m_function = other.m_function;

    m_currState = other.m_currState;

    m_caller = other.m_caller;

	m_executor = make_async_executor(m_function(iter::go_to_place(static_cast<int>(m_currState.position())),make_function(this,&co_bidirectional_iterator<T>::acquire_iterable_value))) -> attach(m_caller);

	const start_result execRes = m_executor->execute();

	DDK_ASSERT(execRes == success || execRes.error() == async_execute_interface<T>::AlreadyDone, "Error while executing iterator");

	return *this;
}
template<typename T>
bool co_bidirectional_iterator<T>::operator!=(const co_bidirectional_iterator<T>& other) const
{
	return m_currState != other.m_currState;
}
template<typename T>
bool co_bidirectional_iterator<T>::operator==(const co_bidirectional_iterator<T>& other) const
{
	return m_currState == other.m_currState;
}
template<typename T>
iter::bidirectional_action co_bidirectional_iterator<T>::acquire_iterable_value(reference i_value)
{
	if(iter::action_result actionRes = iter::iterable_state::forward_result())
	{
		m_currAction.set(actionRes.get());

		yield(i_value);
	}

    return m_currAction.get();
}

template<typename T>
co_random_access_iterator<T>::co_random_access_iterator(const detail::none_t&)
: m_currState(iter::iterable_state::npos)
{
}
template<typename T>
co_random_access_iterator<T>::co_random_access_iterator(const co_random_access_iterator& other)
: m_function(other.m_function)
, m_currState(other.m_currState)
, m_caller(other.m_caller)
{
	typedef typename async_execute_interface<T>::start_result start_result;

	m_executor = make_async_executor(m_function(iter::go_to_place(static_cast<int>(m_currState.position())),make_function(this,&co_random_access_iterator<T>::acquire_iterable_value))) -> attach(m_caller);

    if(m_executor->execute() != success)
    {
		m_currState.reset();
	}
}
template<typename T>
template<typename Iterable>
co_random_access_iterator<T>::co_random_access_iterator(Iterable& i_iterable, typename std::enable_if<is_co_iterator<Iterable>::value == false>::type*)
: m_function([&i_iterable](const iter::shift_action& i_initialAction, const function<iter::random_access_action(reference)>& i_sink) -> reference { return visit_iterator(i_iterable,i_sink,i_initialAction); })
, m_caller(make_stack_allocator<typename co_iterator_allocator_info<Iterable>::allocator>(),co_iterator_allocator_info<Iterable>::s_max_num_pages)
{
	typedef typename async_execute_interface<T>::start_result start_result;

	m_executor = make_async_executor(m_function(iter::go_to_place(static_cast<int>(m_currState.position())),make_function(this,&co_random_access_iterator<T>::acquire_iterable_value))) -> attach(m_caller);

    if (m_executor->execute() != success)
    {
		m_currState.reset();
	}
}
template<typename T>
typename co_random_access_iterator<T>::reference co_random_access_iterator<T>::operator*()
{
    if(m_executor)
    {
        return m_executor->get_value();
    }
    else
    {
        throw bad_access_exception{"Acessing empty iterator"};
    }
}
template<typename T>
typename co_random_access_iterator<T>::const_reference co_random_access_iterator<T>::operator*() const
{
    if(m_executor)
    {
        return m_executor->get_value();
    }
    else
    {
        throw bad_access_exception{"Acessing empty iterator"};
    }
}
template<typename T>
co_random_access_iterator<T>& co_random_access_iterator<T>::operator++()
{
	DDK_ASSERT(m_executor != nullptr, "Dereferencing void iterator");

	typedef typename async_execute_interface<T>::start_result start_result;

    m_currAction.set(iter::go_next_place);

	if (m_executor->execute() != success)
	{
        m_currState.reset();
    }

	return *this;
}
template<typename T>
co_random_access_iterator<T> co_random_access_iterator<T>::operator++(int)
{
	DDK_ASSERT(m_executor != nullptr, "Dereferencing void iterator");

	const co_random_access_iterator<T> res = *this;

    m_currAction.set(iter::go_next_place);

	if (m_executor->execute() != success)
	{
        m_currState.reset();
    }

	return res;
}
template<typename T>
co_random_access_iterator<T>& co_random_access_iterator<T>::operator--()
{
	DDK_ASSERT(m_executor != nullptr, "Dereferencing void iterator");

    m_currAction.set(iter::go_prev_place);

	if (m_executor->execute() != success)
	{
        m_currState.reset();
    }

	return *this;
}
template<typename T>
co_random_access_iterator<T> co_random_access_iterator<T>::operator--(int)
{
	DDK_ASSERT(m_executor != nullptr, "Dereferencing void iterator");

	const co_random_access_iterator<T> res = *this;

    m_currAction.set(iter::go_prev_place);

	if (m_executor->execute() != success)
	{
        m_currState.reset();
    }

	return res;
}
template<typename T>
co_random_access_iterator<T> co_random_access_iterator<T>::operator+(int i_shift) const
{
	DDK_ASSERT(m_executor != nullptr, "Dereferencing void iterator");

	co_random_access_iterator<T> res = *this;

	if(i_shift != 0)
	{
		res.m_currAction.set(iter::go_to_place(i_shift));

        if (res.m_executor->execute() != success)
        {
            res.m_currState.reset();
        }
	}

	return res;
}
template<typename T>
co_random_access_iterator<T> co_random_access_iterator<T>::operator[](size_t i_absPos) const
{
	DDK_ASSERT(m_executor != nullptr, "Dereferencing void iterator");

	co_random_access_iterator<T> res = *this;
	
	const size_t currPos = res.m_currState.position();

	if(i_absPos != currPos)
	{
        res.m_currAction.set(iter::go_to_place(static_cast<int>(i_absPos) - static_cast<int>(currPos)));

		if (res.m_executor->execute() != success)
		{
            res.m_currState.reset();
        }
	}

	return res;
}
template<typename T>
co_random_access_iterator<T>& co_random_access_iterator<T>::operator=(const co_random_access_iterator& other)
{
	typedef typename async_execute_interface<T>::start_result start_result;

	m_function = other.m_function;

	m_currState = other.m_currState;

	m_caller = other.m_caller;

	m_executor = make_async_executor(m_function(iter::go_to_place(static_cast<int>(m_currState.position())),make_function(this,&co_random_access_iterator<T>::acquire_iterable_value))) -> attach(m_caller);

    const start_result execRes = m_executor->execute();

    DDK_ASSERT(execRes == success || execRes == async_execute_interface<T>::AlreadyDone, "Error while executing iterator");

	return *this;
}
template<typename T>
bool co_random_access_iterator<T>::operator!=(const co_random_access_iterator<T>& other) const
{
	return m_currState != other.m_currState;
}
template<typename T>
bool co_random_access_iterator<T>::operator==(const co_random_access_iterator<T>& other) const
{
	return m_currState == other.m_currState;
}
template<typename T>
iter::random_access_action co_random_access_iterator<T>::acquire_iterable_value(reference i_value)
{
	if(iter::action_result actionRes = iter::iterable_state::forward_result())
	{
		m_currState.apply(actionRes.get());

		yield(i_value);
	}

    return m_currAction.get();
}

}
