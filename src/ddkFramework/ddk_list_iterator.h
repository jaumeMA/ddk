//////////////////////////////////////////////////////////////////////////////
//
// Author: Jaume Moragues
// Distributed under the GNU Lesser General Public License, Version 3.0. (See a copy
// at https://www.gnu.org/licenses/lgpl-3.0.ca.html)
//
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include <iterator>
#include "ddk_lent_pointer_wrapper.h"

namespace ddk
{

template<typename T, template<typename> typename Pointer = lent_pointer_wrapper>
struct list_iterator
{
	template<typename,template<typename>typename>
	friend struct list_iterator;
	template<typename,typename>
	friend struct list_const_iterator;

	typedef Pointer<T> pointer_t;
	typedef Pointer<const T> const_pointer_t;
    typedef int difference_type;
	typedef typename T::value_type value_type;
	typedef typename mpl::static_if<std::is_const<T>::value,typename T::const_reference,typename T::reference>::type reference;
	typedef typename T::const_reference const_reference;
	typedef typename mpl::static_if<std::is_const<T>::value,typename T::const_pointer,typename T::pointer>::type pointer;
	typedef typename T::const_pointer const_pointer;
	typedef std::forward_iterator_tag iterator_category;

	list_iterator(const std::nullptr_t&)
	: m_currNode(nullptr)
	{
	}
	explicit list_iterator(const pointer_t& i_node)
	: m_currNode(i_node)
	{
	}
	list_iterator(const list_iterator& other)
	: m_currNode(other.m_currNode)
	{
	}
	list_iterator(list_iterator&& other)
	: m_currNode(std::move(other.m_currNode))
	{
	}
	template<typename TT>
	list_iterator(const list_iterator<TT,Pointer>& other)
	: m_currNode(other.m_currNode)
	{
	}
	template<typename TT>
	list_iterator(list_iterator<TT,Pointer>&& other)
	: m_currNode(std::move(other.m_currNode))
	{
	}
	list_iterator& operator=(const list_iterator& other)
	{
		m_currNode = other.m_currNode;

		return *this;
	}
	list_iterator& operator=(list_iterator&& other)
	{
		m_currNode = std::move(other.m_currNode);

		return *this;
	}
	template<typename TT>
	list_iterator& operator=(const list_iterator<TT,Pointer>& other)
	{
		m_currNode = other.m_currNode;

		return *this;
	}
	template<typename TT>
	list_iterator& operator=(list_iterator<TT,Pointer>&& other)
	{
		m_currNode = std::move(other.m_currNode);

		return *this;
	}
	list_iterator operator++(int)
	{
		list_iterator res(m_currNode);

		m_currNode = m_currNode->get_next_node();

		return res;
	}
	list_iterator& operator++()
	{
		m_currNode = m_currNode->get_next_node();

		return *this;
	}
	inline reference operator*()
	{
		return m_currNode->get();
	}
	inline const_reference operator*() const
	{
		return m_currNode->get();
	}
	inline pointer operator->()
	{
		return m_currNode->get_ptr();
	}
	inline const_pointer operator->() const
	{
		return m_currNode->get_ptr();
	}
	inline bool operator==(const list_iterator& other) const
	{
		return m_currNode == other.m_currNode;
	}
	inline bool operator!=(const list_iterator& other) const
	{
		return m_currNode != other.m_currNode;
	}
	const_pointer_t get() const
	{
		return m_currNode;
	}
	pointer_t get()
	{
		return *m_currNode;
	}
	pointer_t extract()
	{
		return std::move(m_currNode);
	}
	void reset()
	{
		m_currNode = nullptr;
	}

private:
	pointer_t m_currNode;
};

}
