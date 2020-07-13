#pragma once

#include "ddk_lend_from_this.h"
#include "ddk_reference_wrapper.h"
#include "ddk_arena.h"

namespace ddk
{
namespace detail
{

template<typename T>
struct intrusive_node : public lend_from_this<intrusive_node<T>,intrusive_node<T>>
{
public:
	typedef lent_reference_wrapper<intrusive_node<T>> intrusive_node_ref;
	typedef lent_reference_wrapper<const intrusive_node<T>> intrusive_node_const_ref;
	typedef lent_pointer_wrapper<intrusive_node<T>> intrusive_node_ptr;
	typedef lent_pointer_wrapper<const intrusive_node<T>> intrusive_node_const_ptr;
	typedef intrusive_node_ptr reference_type;
    typedef intrusive_node_const_ptr const_reference_type;
	typedef intrusive_node_ptr rreference_type;
	typedef T value_type;
	typedef typename std::add_lvalue_reference<value_type>::type reference;
	typedef typename std::add_lvalue_reference<typename std::add_const<value_type>::type>::type const_reference;
	typedef typename std::add_rvalue_reference<value_type>::type rreference;
	typedef typename std::add_const<rreference>::type const_rreference;
	typedef typename std::add_pointer<value_type>::type pointer;
	typedef typename std::add_pointer<typename std::add_const<value_type>::type>::type const_pointer;

	intrusive_node()
	: m_prevNode(nullptr)
	, m_nextNode(nullptr)
	{
	}
	intrusive_node(const intrusive_node& other) = delete;
	intrusive_node(intrusive_node&&) = delete;
	template<typename ... Args>
	intrusive_node(Args&& ... i_args)
	: m_prevNode(nullptr)
	, m_nextNode(nullptr)
	{
		m_content.template construct<T>(std::forward<Args>(i_args) ...);
	}
	~intrusive_node()
	{
		m_prevNode = nullptr;
		m_nextNode = nullptr;
		m_content.template destroy<T>();
	}
	intrusive_node& operator=(const intrusive_node& other) = delete;
	intrusive_node& operator=(intrusive_node&& other)
	{
		m_content.template destroy<T>();

		if(other.m_content.empty() == false)
		{
			m_content.template assign<T>(other.m_content.template extract<T>());
		}

		return *this;
	}
	inline intrusive_node_ptr get_prev_node()
	{
		return m_prevNode;
	}
	inline intrusive_node_const_ptr get_prev_node() const
	{
		return m_prevNode;
	}
	inline intrusive_node_ptr get_next_node()
	{
		return m_nextNode;
	}
	inline intrusive_node_const_ptr get_next_node() const
	{
		return m_nextNode;
	}
	inline intrusive_node_ptr extract_prev_node()
	{
		intrusive_node_ptr res = m_prevNode;

		if(m_prevNode)
		{
			m_prevNode->m_nextNode = nullptr;
			m_prevNode = nullptr;
		}

		return res;
	}
	inline intrusive_node_ptr extract_next_node()
	{
		intrusive_node_ptr res = m_nextNode;

		if(m_nextNode)
		{
			m_nextNode->m_prevNode = nullptr;
			m_nextNode = nullptr;
		}

		return res;
	}
	inline void clear()
	{
		if(m_nextNode)
		{
			m_nextNode->clear();
			m_nextNode = nullptr;
		}
		m_prevNode = nullptr;
	}
	inline void clear_prev_node()
	{
		m_prevNode = nullptr;
	}
	inline void clear_next_node()
	{
		m_nextNode = nullptr;
	}
	inline void set_prev_node(const intrusive_node_ptr& i_node)
	{
		if(get_raw_ptr(m_prevNode) != this)
		{
			m_prevNode = i_node;
			m_prevNode->m_nextNode = this->template ref_from_this<intrusive_node<T>>(*this);
		}
		else
		{
			DDK_FAIL("Auto pointing!");
		}
	}
	inline void set_next_node(const intrusive_node_ptr& i_node)
	{
		if(get_raw_ptr(m_nextNode) != this)
		{
			m_nextNode = i_node;
			m_nextNode->m_prevNode = this->template ref_from_this<intrusive_node<T>>(*this);
		}
		else
		{
			DDK_FAIL("Auto pointing!");
		}
	}
	static intrusive_node_ptr collapse(intrusive_node_ptr i_node)
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
	inline T& get_value()
	{
		return m_content.template get<T>();
	}
	inline const T& get_value() const
	{
		return m_content.template get<T>();
	}
	inline T extract_value()
	{
		m_nextNode = nullptr;

		return m_content.template extract<T>();
	}
	inline reference operator*()
	{
		return m_content.template get<T>();
	}
	inline const_reference operator*() const
	{
		return m_content.template get<T>();
	}
	inline pointer operator->()
	{
		return m_content.template get_ptr<T>();
	}
	inline const_pointer operator->() const
	{
		return m_content.template get_ptr<T>();
	}
	inline bool operator==(const T& other) const
	{
		return m_content.template compare<T>(other);
	}
	inline bool empty() const
	{
		return m_content.empty();
	}
	explicit operator bool() const
	{
		return m_content.empty() == false;
	}

private:
	intrusive_node_ptr m_prevNode;
	intrusive_node_ptr m_nextNode;
	typed_arena<T> m_content;
};

}
}
