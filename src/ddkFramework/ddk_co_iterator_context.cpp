#include "ddk_co_iterator_context.h"

namespace ddk
{
namespace detail
{

co_forward_iterator_context_impl::co_forward_iterator_context_impl(size_t i_currIndex)
: m_currIndex(npos)
, m_targetIndex(i_currIndex)
{
}
void co_forward_iterator_context_impl::incr()
{
	++m_targetIndex;
}
void co_forward_iterator_context_impl::accept()
{
	m_currIndex = m_targetIndex;
}
void co_forward_iterator_context_impl::reject()
{
	m_targetIndex = m_currIndex;
}
size_t co_forward_iterator_context_impl::get_current() const
{
	return m_currIndex;
}
size_t co_forward_iterator_context_impl::get_target() const
{
	return m_targetIndex;
}

void co_bidirectional_iterator_context_impl::decr()
{
	--m_targetIndex;
}

void co_random_access_iterator_context_impl::shift(size_t i_shift)
{
	m_targetIndex += i_shift;
}
void co_random_access_iterator_context_impl::set_abs(size_t i_index)
{
	m_targetIndex = i_index;
}

}

co_forward_iterator_context::co_forward_iterator_context(const detail::co_forward_iterator_context_impl& i_impl)
: m_impl(i_impl)
{
}
size_t co_forward_iterator_context::get_curr_index() const
{
	return m_impl.get_current();
}
bool co_forward_iterator_context::go_forward() const
{
	return m_impl.get_target() > m_impl.get_current();
}

co_bidirectional_iterator_context::co_bidirectional_iterator_context(const detail::co_bidirectional_iterator_context_impl& i_impl)
: m_impl(i_impl)
{
}
size_t co_bidirectional_iterator_context::get_curr_index() const
{
	return m_impl.get_current();
}
bool co_bidirectional_iterator_context::go_forward() const
{
	return m_impl.get_target() > m_impl.get_current();
}
bool co_bidirectional_iterator_context::go_backward() const
{
	return m_impl.get_target() < m_impl.get_current();
}

co_random_access_iterator_context::co_random_access_iterator_context(const detail::co_random_access_iterator_context_impl& i_impl)
: m_impl(i_impl)
{
}
size_t co_random_access_iterator_context::get_curr_index() const
{
	return m_impl.get_current();
}
size_t co_random_access_iterator_context::shift() const
{
	return m_impl.get_target() - m_impl.get_current();
}

}