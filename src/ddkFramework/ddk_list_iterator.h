#pragma once

#include <iterator>

namespace ddk
{

template<typename ListNodeType>
struct list_iterator
{
	template<typename>
	friend struct list_iterator;
	template<typename>
	friend struct list_const_iterator;

	typedef ListNodeType node_type;
	typedef typename node_type::reference_type reference_type;
	typedef typename std::add_const<reference_type> const_reference_type;
	typedef typename node_type::rreference_type rreference_type;
	typedef typename node_type::value_type value_type;
	typedef typename std::add_lvalue_reference<value_type>::type reference;
	typedef typename std::add_lvalue_reference<typename std::add_const<value_type>::type>::type const_reference;
	typedef typename std::add_rvalue_reference<value_type>::type rreference;
	typedef typename std::add_const<rreference>::type const_rreference;
	typedef typename std::add_pointer<value_type>::type pointer;
	typedef typename std::add_const<pointer>::type const_pointer;
	typedef std::ptrdiff_t difference_type;
	typedef std::forward_iterator_tag iterator_category;

	list_iterator(const std::nullptr_t&)
	: m_currNode(nullptr)
	{
	}
	explicit list_iterator(reference_type i_node)
	: m_currNode(i_node)
	{
	}
	list_iterator(const list_iterator<ListNodeType>& other)
	: m_currNode(other.m_currNode)
	{
	}
	list_iterator(list_iterator<ListNodeType>&& other)
	: m_currNode(std::move(other.m_currNode))
	{
	}
	list_iterator& operator=(const list_iterator<ListNodeType>& other)
	{
		m_currNode = other.m_currNode;

		return *this;
	}
	list_iterator& operator=(list_iterator<ListNodeType>&& other)
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
		return m_currNode->get_value();
	}
	inline const_reference operator*() const
	{
		return m_currNode->get_value();
	}
	inline pointer operator->()
	{
		return &(m_currNode->get_value());
	}
	inline const_pointer operator->() const
	{
		return &(m_currNode->get_value());
	}
	inline bool operator==(const list_iterator<ListNodeType>& other) const
	{
		return m_currNode == other.m_currNode;
	}
	inline bool operator!=(const list_iterator<ListNodeType>& other) const
	{
		return m_currNode != other.m_currNode;
	}
	const_reference_type get() const
	{
		return m_currNode;
	}
	reference_type get()
	{
		return m_currNode;
	}
	rreference_type extract()
	{
		return std::move(m_currNode);
	}
	void reset()
	{
		m_currNode = nullptr;
	}

private:
	reference_type m_currNode;
};

template<typename ListNodeType>
struct list_const_iterator
{
	template<typename>
	friend struct list_const_iterator;

	typedef ListNodeType node_type;
	typedef typename ListNodeType::reference_type reference_type;
	typedef typename ListNodeType::const_reference_type const_reference_type;
	typedef typename node_type::rreference_type rreference_type;
	typedef typename ListNodeType::value_type value_type;
	typedef typename ListNodeType::reference reference;
	typedef typename ListNodeType::const_reference const_reference;
	typedef typename ListNodeType::rreference rreference;
	typedef typename ListNodeType::const_rreference const_rreference;
	typedef typename ListNodeType::pointer pointer;
	typedef typename ListNodeType::const_pointer const_pointer;
	typedef std::ptrdiff_t difference_type;
	typedef std::forward_iterator_tag iterator_category;

	list_const_iterator(const std::nullptr_t&)
	: m_currNode(nullptr)
	{
	}
	explicit list_const_iterator(const_reference_type i_node)
	: m_currNode(i_node)
	{
	}
	list_const_iterator(const list_const_iterator<ListNodeType>& other)
	: m_currNode(other.m_currNode)
	{
	}
	list_const_iterator(const list_iterator<ListNodeType>& other)
	: m_currNode(other.m_currNode)
	{
	}
	list_const_iterator(list_const_iterator<ListNodeType>&& other)
	: m_currNode(std::move(other.m_currNode))
	{
	}
	list_const_iterator(list_iterator<ListNodeType>&& other)
	: m_currNode(std::move(other.m_currNode))
	{
	}
	list_const_iterator operator++(int)
	{
		list_const_iterator res(m_currNode);

		m_currNode = m_currNode->get_next_node();

		return res;
	}
	list_const_iterator& operator++()
	{
		m_currNode = m_currNode->get_next_node();

		return *this;
	}
	inline const_reference operator*() const
	{
		return m_currNode->get_value();
	}
	inline const_pointer operator->() const
	{
		return &(m_currNode->get_value());
	}
	inline bool operator==(const list_const_iterator<ListNodeType>& other) const
	{
		return m_currNode == other.m_currNode;
	}
	inline bool operator!=(const list_const_iterator<ListNodeType>& other) const
	{
		return m_currNode != other.m_currNode;
	}
	const_reference_type get() const
	{
		return m_currNode;
	}
	rreference_type extract()
	{
		return std::move(m_currNode);
	}

private:
	const_reference_type m_currNode;
};

}
