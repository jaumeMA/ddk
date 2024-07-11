
#include "ddk_container_exceptions.h"
#include "ddk_allocator_exceptions.h"
#include "ddk_reference_wrapper.h"

namespace ddk
{
namespace detail
{

template<typename T,template<typename>typename Pointer>
template<typename ... Args>
intrusive_node_impl<T,Pointer>::intrusive_node_impl(Args&& ... i_args)
: m_value(std::forward<Args>(i_args) ...)
{
}
template<typename T,template<typename>typename Pointer>
intrusive_node_impl<T,Pointer>::~intrusive_node_impl()
{
}
template<typename T,template<typename>typename Pointer>
typename intrusive_node_impl<T,Pointer>::intrusive_node_ptr intrusive_node_impl<T,Pointer>::get_prev_node()
{
	return m_prevNode;
}
template<typename T,template<typename>typename Pointer>
typename intrusive_node_impl<T,Pointer>::intrusive_node_const_ptr intrusive_node_impl<T,Pointer>::get_prev_node() const
{
	return m_prevNode;
}
template<typename T,template<typename>typename Pointer>
typename intrusive_node_impl<T,Pointer>::intrusive_node_ptr intrusive_node_impl<T,Pointer>::get_next_node()
{
	return m_nextNode;
}
template<typename T,template<typename>typename Pointer>
typename intrusive_node_impl<T,Pointer>::intrusive_node_const_ptr intrusive_node_impl<T,Pointer>::get_next_node() const
{
	return m_nextNode;
}
template<typename T,template<typename>typename Pointer>
typename intrusive_node_impl<T,Pointer>::intrusive_node_ptr intrusive_node_impl<T,Pointer>::extract_prev_node()
{
	intrusive_node_ptr res = m_prevNode;

	if(m_prevNode)
	{
		m_prevNode->m_nextNode = nullptr;
		m_prevNode = nullptr;
	}

	return res;
}
template<typename T,template<typename>typename Pointer>
typename intrusive_node_impl<T,Pointer>::intrusive_node_ptr intrusive_node_impl<T,Pointer>::extract_next_node()
{
	intrusive_node_ptr res = m_nextNode;

	if(m_nextNode)
	{
		m_nextNode->m_prevNode = nullptr;
		m_nextNode = nullptr;
	}

	return res;
}
template<typename T,template<typename>typename Pointer>
void intrusive_node_impl<T,Pointer>::clear()
{
	if(m_nextNode)
	{
		m_nextNode->clear();
		m_nextNode = nullptr;
	}
	m_prevNode = nullptr;
}
template<typename T,template<typename>typename Pointer>
void intrusive_node_impl<T,Pointer>::clear_prev_node()
{
	m_prevNode = nullptr;
}
template<typename T,template<typename>typename Pointer>
void intrusive_node_impl<T,Pointer>::clear_next_node()
{
	m_nextNode = nullptr;
}
template<typename T,template<typename>typename Pointer>
void intrusive_node_impl<T,Pointer>::set_prev_node(const intrusive_node_ptr& i_node)
{
	if(get_raw_ptr(m_prevNode) != this)
	{
		m_prevNode = i_node;
		m_prevNode->m_nextNode = lend(*this);
	}
	else
	{
		DDK_FAIL("Auto pointing!");
	}
}
template<typename T,template<typename>typename Pointer>
void intrusive_node_impl<T,Pointer>::set_next_node(const intrusive_node_ptr& i_node)
{
	if(get_raw_ptr(m_nextNode) != this)
	{
		m_nextNode = i_node;
		m_nextNode->m_prevNode = lend(*this);
	}
	else
	{
		DDK_FAIL("Auto pointing!");
	}
}
template<typename T,template<typename>typename Pointer>
typename intrusive_node_impl<T,Pointer>::intrusive_node_ptr intrusive_node_impl<T,Pointer>::collapse(intrusive_node_ptr i_node)
{
	//DDK_ASSERT(get_raw_ptr(i_prevNode->m_nextNode) == this, "Breaking linked list");
	intrusive_node_ptr res = i_node->m_nextNode;
	intrusive_node_ptr prevNode = i_node->m_prevNode;
	intrusive_node_ptr nextNode = i_node->m_nextNode;

	i_node->m_prevNode = nullptr;
	i_node->m_nextNode = nullptr;
	i_node = nullptr;

	if(prevNode)
	{
		prevNode->m_nextNode = nextNode;
	}

	if(nextNode)
	{
		nextNode->m_prevNode = prevNode;
	}

	return res;
}
template<typename T,template<typename>typename Pointer>
typename intrusive_node_impl<T,Pointer>::reference intrusive_node_impl<T,Pointer>::get()
{
	return m_value;
}
template<typename T,template<typename>typename Pointer>
typename intrusive_node_impl<T,Pointer>::const_reference intrusive_node_impl<T,Pointer>::get() const
{
	return m_value;
}
template<typename T,template<typename>typename Pointer>
typename intrusive_node_impl<T,Pointer>::rreference intrusive_node_impl<T,Pointer>::extract() &&
{
	return std::move(m_value);
}
template<typename T,template<typename>typename Pointer>
typename intrusive_node_impl<T,Pointer>::pointer intrusive_node_impl<T,Pointer>::get_ptr()
{
	return &m_value;
}
template<typename T,template<typename>typename Pointer>
typename intrusive_node_impl<T,Pointer>::const_pointer intrusive_node_impl<T,Pointer>::get_ptr() const
{
	return &m_value;
}
template<typename T,template<typename>typename Pointer>
intrusive_node_impl<T,Pointer>::operator T&()
{
	return m_value;
}
template<typename T,template<typename>typename Pointer>
intrusive_node_impl<T,Pointer>::operator const T&() const
{
	return m_value;
}

}

template<typename T,template<typename>typename Pointer>
intrusive_node<T,Pointer>::intrusive_node(intrusive_node&& other)
: m_impl(std::move(other.m_impl))
{
}
template<typename T,template<typename>typename Pointer>
template<typename ... Args>
intrusive_node<T,Pointer>::intrusive_node(Args&& ... i_args)
: m_impl(std::forward<Args>(i_args) ...)
{
}
template<typename T,template<typename>typename Pointer>
intrusive_node<T,Pointer>::~intrusive_node()
{
}
template<typename T,template<typename>typename Pointer>
intrusive_node<T,Pointer>& intrusive_node<T,Pointer>::operator=(intrusive_node&& other)
{
	m_impl = std::move(m_impl);

	return *this;
}
template<typename T,template<typename>typename Pointer>
void intrusive_node<T,Pointer>::clear()
{
	m_impl.clear();
}
template<typename T,template<typename>typename Pointer>
typename intrusive_node<T,Pointer>::reference intrusive_node<T,Pointer>::get()
{
	return m_impl.get();
}
template<typename T,template<typename>typename Pointer>
typename intrusive_node<T,Pointer>::const_reference intrusive_node<T,Pointer>::get() const
{
	return m_impl.get();
}
template<typename T,template<typename>typename Pointer>
typename intrusive_node<T,Pointer>::rreference intrusive_node<T,Pointer>::extract() &&
{
	return m_impl.extract();
}
template<typename T,template<typename>typename Pointer>
typename intrusive_node<T,Pointer>::pointer intrusive_node<T,Pointer>::operator->()
{
	return m_impl.get_ptr();
}
template<typename T,template<typename>typename Pointer>
typename intrusive_node<T,Pointer>::const_pointer intrusive_node<T,Pointer>::operator->() const
{
	return m_impl.get_ptr();
}
template<typename T,template<typename>typename Pointer>
bool intrusive_node<T,Pointer>::operator==(const T& other) const
{
	return m_impl == other;
}

}
