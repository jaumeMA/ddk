
#include "ddk_iterable_visitor.h"
#include "ddk_co_iterator_allocator_info.h"
#include "ddk_container_exceptions.h"

namespace ddk
{

//co forward iterator impl
template<typename T>
co_forward_iterator<T>::co_forward_iterator(const detail::none_t&)
: m_currState(iter::iterable_state::npos)
,m_executor(nullptr)
{
}
template<typename T>
co_forward_iterator<T>::co_forward_iterator(const co_forward_iterator& other)
: m_function(other.m_function)
,m_currState(other.m_currState)
,m_caller(other.m_caller)
,m_executor(m_function(iter::go_to_place(static_cast<int>(m_currState.position())),make_function(this,&co_forward_iterator<T>::acquire_iterable_value)))
{
	if(other.m_iteratorValueContainer.empty() == false)
	{
		m_iteratorValueContainer.template construct<reference>(other.m_iteratorValueContainer.template get<reference>());
	}
}
template<typename T>
co_forward_iterator<T>::co_forward_iterator(co_forward_iterator&& other)
: m_function(std::move(other.m_function))
,m_currState(std::move(other.m_currState))
,m_caller(std::move(other.m_caller))
,m_executor(m_function(iter::go_to_place(static_cast<int>(m_currState.position())),make_function(this,&co_forward_iterator<T>::acquire_iterable_value)))
{
	if(other.m_iteratorValueContainer.empty() == false)
	{
		m_iteratorValueContainer.template construct<reference>(other.m_iteratorValueContainer.template get<reference>());
	}
}
template<typename T>
template<typename Iterable>
co_forward_iterator<T>::co_forward_iterator(Iterable& i_iterable,typename std::enable_if<is_co_iterator<Iterable>::value == false>::type*)
: m_function([&i_iterable](const iter::shift_action& i_initialAction,const function<iter::const_forward_action(reference)>& i_sink) -> reference { return visit_iterator(i_iterable,i_sink,i_initialAction); })
, m_executor(m_function(iter::go_to_place(static_cast<int>(m_currState.position())),make_function(this,&co_forward_iterator<T>::acquire_iterable_value))
			 ,{ make_stack_allocator<typename co_iterator_allocator_info<Iterable>::allocator>(),co_iterator_allocator_info<Iterable>::s_max_num_pages })
{
	if(m_executor.execute() == false)
	{
		m_currState.reset();
	}
}
template<typename T>
co_forward_iterator<T>::~co_forward_iterator()
{
	m_iteratorValueContainer.template destroy<reference>();
}
template<typename T>
typename co_forward_iterator<T>::reference co_forward_iterator<T>::operator*()
{
	if(m_iteratorValueContainer.empty() == false)
	{
		return m_iteratorValueContainer.template get<reference>();
	}
	else
	{
		throw bad_access_exception{ "Acessing empty iterator" };
	}
}
template<typename T>
typename co_forward_iterator<T>::const_reference co_forward_iterator<T>::operator*() const
{
	if(m_iteratorValueContainer.empty() == false)
	{
		return m_iteratorValueContainer.template get<reference>();
	}
	else
	{
		throw bad_access_exception{ "Acessing empty iterator" };
	}
}
template<typename T>
co_forward_iterator<T>& co_forward_iterator<T>::operator++()
{
	DDK_ASSERT(m_executor != nullptr, "Dereferencing void iterator");

    m_currAction = iter::go_next_place;

	if (m_executor.execute() == false)
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

    m_currAction = iter::go_next_place;

	if (m_executor.execute() == false)
	{
        m_currState.reset();
    }

	return res;
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
iter::const_forward_action co_forward_iterator<T>::acquire_iterable_value(reference i_value)
{
	if(m_currAction == iter::go_no_place)
	{
		m_iteratorValueContainer.template set_value<reference>(std::forward<reference>(i_value));

		m_currState.apply(m_currAction);

		m_executor.yield();
	}

	const iter::go_forward_action outAction = m_currAction;

	m_currAction = iter::go_no_place;

	return outAction;
}

//co bidirectional iterator impl
//template<typename T>
//co_bidirectional_iterator<T>::co_bidirectional_iterator(const detail::none_t&)
//: m_currState(iter::iterable_state::npos)
//{
//}
//template<typename T>
//co_bidirectional_iterator<T>::co_bidirectional_iterator(const co_bidirectional_iterator& other)
//: m_function(other.m_function)
//, m_caller(other.m_caller)
//, m_executor(other.m_executor)
//{
//    if(m_executor->execute() != success)
//    {
//		m_currState.reset();
//	}
//}
//template<typename T>
//template<typename Iterable>
//co_bidirectional_iterator<T>::co_bidirectional_iterator(Iterable& i_iterable, typename std::enable_if<is_co_iterator<Iterable>::value == false>::type*)
//: m_function([&i_iterable](const iter::shift_action& i_initialAction, const function<iter::bidirectional_action(reference)>& i_sink) -> reference { return visit_iterator(i_iterable,i_sink,i_initialAction); })
//, m_executor({ make_stack_allocator<typename co_iterator_allocator_info<Iterable>::allocator>(),co_iterator_allocator_info<Iterable>::s_max_num_pages })
//{
//
//    if (m_executor->execute() != success)
//    {
//		m_currState.reset();
//	}
//}
//template<typename T>
//typename co_bidirectional_iterator<T>::reference co_bidirectional_iterator<T>::operator*()
//{
//    if(m_executor)
//    {
//        return m_executor->get_value();
//    }
//    else
//    {
//        throw bad_access_exception{"Acessing empty iterator"};
//    }
//}
//template<typename T>
//typename co_bidirectional_iterator<T>::const_reference co_bidirectional_iterator<T>::operator*() const
//{
//    if(m_executor)
//    {
//        return m_executor->get_value();
//    }
//    else
//    {
//        throw bad_access_exception{"Acessing empty iterator"};
//    }
//}
//template<typename T>
//co_bidirectional_iterator<T>& co_bidirectional_iterator<T>::operator++()
//{
//	DDK_ASSERT(m_executor != nullptr, "Dereferencing void iterator");
//
//    m_currAction = iter::go_next_place;
//
//	if (m_executor->execute() != success)
//	{
//        m_currState.reset();
//    }
//
//	return *this;
//}
//template<typename T>
//co_bidirectional_iterator<T> co_bidirectional_iterator<T>::operator++(int)
//{
//	DDK_ASSERT(m_executor != nullptr, "Dereferencing void iterator");
//
//	co_bidirectional_iterator<T> res = *this;
//
//    m_currAction = iter::go_next_place;
//
//	if (m_executor->execute() != success)
//	{
//        m_currState.reset();
//    }
//
//	return res;
//}
//template<typename T>
//co_bidirectional_iterator<T>& co_bidirectional_iterator<T>::operator--()
//{
//	DDK_ASSERT(m_executor != nullptr, "Dereferencing void iterator");
//
//    m_currAction = iter::go_prev_place;
//
//	if (m_executor->execute() != success)
//	{
//        m_currState.reset();
//    }
//
//	return *this;
//}
//template<typename T>
//co_bidirectional_iterator<T> co_bidirectional_iterator<T>::operator--(int)
//{
//	DDK_ASSERT(m_executor != nullptr, "Dereferencing void iterator");
//
//	co_bidirectional_iterator<T> res = *this;
//
//    m_currAction = iter::go_prev_place;
//
//	if (m_executor->execute() != success)
//	{
//        m_currState.reset();
//    }
//
//	return res;
//}
//template<typename T>
//bool co_bidirectional_iterator<T>::operator!=(const co_bidirectional_iterator<T>& other) const
//{
//	return m_currState != other.m_currState;
//}
//template<typename T>
//bool co_bidirectional_iterator<T>::operator==(const co_bidirectional_iterator<T>& other) const
//{
//	return m_currState == other.m_currState;
//}
//template<typename T>
//iter::bidirectional_action co_bidirectional_iterator<T>::acquire_iterable_value(reference i_value)
//{
//	m_iteratorValueContainer.template set_value<reference>(std::forward<reference>(i_value));
//	
//	m_currState.apply(m_currAction);
//
//	yield(i_value);
//
//    return m_currAction;
//}

template<typename T>
co_random_access_iterator<T>::co_random_access_iterator(const detail::none_t&)
: m_currState(iter::iterable_state::npos)
, m_currAction(iter::go_no_place)
,m_executor(nullptr)
{
}
template<typename T>
co_random_access_iterator<T>::co_random_access_iterator(const co_random_access_iterator& other)
: m_function(other.m_function)
, m_currState(other.m_currState)
, m_executor(m_function(iter::go_to_place(static_cast<int>(m_currState.position())),make_member_function(this,&co_random_access_iterator<T>::acquire_iterable_value)))
{
	if(other.m_iteratorValueContainer.empty() == false)
	{
		m_iteratorValueContainer.template construct<reference>(other.m_iteratorValueContainer.template get<reference>());
	}
}
template<typename T>
template<typename Iterable>
co_random_access_iterator<T>::co_random_access_iterator(Iterable& i_iterable, typename std::enable_if<is_co_iterator<Iterable>::value == false>::type*)
: m_function([&i_iterable](const iter::shift_action& i_initialAction, const detail::relative_function_impl<co_random_access_iterator<T>,iter::const_random_access_action,reference>& i_sink) -> reference { return visit_iterator(i_iterable,i_sink,i_initialAction); })
, m_executor(m_function(iter::go_to_place(static_cast<int>(m_currState.position())),make_member_function(this,&co_random_access_iterator<T>::acquire_iterable_value))
			,{ make_stack_allocator<typename co_iterator_allocator_info<Iterable>::allocator>(),co_iterator_allocator_info<Iterable>::s_max_num_pages })
{
    if (m_executor.execute() == false)
    {
		m_currState.reset();
	}
}
template<typename T>
co_random_access_iterator<T>::~co_random_access_iterator()
{
	m_iteratorValueContainer.template destroy<reference>();
}
template<typename T>
typename co_random_access_iterator<T>::reference co_random_access_iterator<T>::operator*()
{
    if(m_iteratorValueContainer.empty() == false)
    {
        return m_iteratorValueContainer.template get<reference>();
    }
    else
    {
        throw bad_access_exception{"Acessing empty iterator"};
    }
}
template<typename T>
typename co_random_access_iterator<T>::const_reference co_random_access_iterator<T>::operator*() const
{
    if(m_iteratorValueContainer.empty() == false)
    {
        return m_iteratorValueContainer.template get<reference>();
    }
    else
    {
        throw bad_access_exception{"Acessing empty iterator"};
    }
}
template<typename T>
co_random_access_iterator<T>& co_random_access_iterator<T>::operator++()
{
    m_currAction = iter::go_next_place;

	if (m_executor.execute() == false)
	{
        m_currState.reset();
    }

	return *this;
}
template<typename T>
co_random_access_iterator<T> co_random_access_iterator<T>::operator++(int)
{
	const co_random_access_iterator<T> res = *this;

    m_currAction = iter::go_next_place;

	if (m_executor.execute() == false)
	{
        m_currState.reset();
    }

	return res;
}
template<typename T>
co_random_access_iterator<T>& co_random_access_iterator<T>::operator--()
{
    m_currAction = iter::go_prev_place;

	if (m_executor.execute() == false)
	{
        m_currState.reset();
    }

	return *this;
}
template<typename T>
co_random_access_iterator<T> co_random_access_iterator<T>::operator--(int)
{
	const co_random_access_iterator<T> res = *this;

    m_currAction = iter::go_prev_place;

	if (m_executor.execute() == false)
	{
        m_currState.reset();
    }

	return res;
}
template<typename T>
co_random_access_iterator<T> co_random_access_iterator<T>::operator+(int i_shift) const
{
	co_random_access_iterator<T> res = *this;

	if(i_shift != 0)
	{
		res.m_currAction = iter::go_to_place(i_shift);

        if (res.m_executor.execute() == false)
        {
            res.m_currState.reset();
        }
	}

	return res;
}
template<typename T>
co_random_access_iterator<T> co_random_access_iterator<T>::operator[](size_t i_absPos) const
{
	co_random_access_iterator<T> res = *this;
	
	const size_t currPos = res.m_currState.position();

	if(i_absPos != currPos)
	{
        res.m_currAction = iter::go_to_place(static_cast<int>(i_absPos) - static_cast<int>(currPos));

		if (res.m_executor.execute())
		{
            res.m_currState.reset();
        }
	}

	return res;
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
iter::const_random_access_action co_random_access_iterator<T>::acquire_iterable_value(reference i_value)
{
	if(m_currAction)
	{
		m_iteratorValueContainer.template construct<reference>(i_value);

		m_currState.apply(m_currAction);

		m_executor.yield();
	}

    return std::move(m_currAction);
}

}
