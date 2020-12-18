#pragma once

#include <iterator>

namespace ddk
{

template<typename>
struct list_iterator;

template<typename T>
struct list_iterator<lent_pointer_wrapper<T>>
{
	template<typename>
	friend struct list_iterator;
	template<typename>
	friend struct list_const_iterator;

	typedef typename T::value_type value_type;
	typedef typename mpl::static_if<std::is_const<T>::value,typename T::const_reference, typename T::reference>::type reference;
	typedef typename T::const_reference const_reference;
	typedef typename mpl::static_if<std::is_const<T>::value,typename T::const_pointer,typename T::pointer>::type pointer;
	typedef typename T::const_pointer const_pointer;
	typedef std::forward_iterator_tag iterator_category;

	list_iterator(const std::nullptr_t&)
	: m_currNode(nullptr)
	{
	}
	explicit list_iterator(lent_pointer_wrapper<T> i_node)
	: m_currNode(i_node)
	{
	}
	list_iterator(const list_iterator<lent_pointer_wrapper<T>>& other)
	: m_currNode(other.m_currNode)
	{
	}
	list_iterator(list_iterator<lent_pointer_wrapper<T>>&& other)
	: m_currNode(std::move(other.m_currNode))
	{
	}
	template<typename TT>
	list_iterator(const list_iterator<lent_pointer_wrapper<TT>>& other)
	: m_currNode(other.m_currNode)
	{
	}
	template<typename TT>
	list_iterator(list_iterator<lent_pointer_wrapper<TT>>&& other)
	: m_currNode(std::move(other.m_currNode))
	{
	}
	list_iterator& operator=(const list_iterator<lent_pointer_wrapper<T>>& other)
	{
		m_currNode = other.m_currNode;

		return *this;
	}
	list_iterator& operator=(list_iterator<lent_pointer_wrapper<T>>&& other)
	{
		m_currNode = std::move(other.m_currNode);

		return *this;
	}
	template<typename TT>
	list_iterator& operator=(const list_iterator<lent_pointer_wrapper<TT>>& other)
	{
		m_currNode = other.m_currNode;

		return *this;
	}
	template<typename TT>
	list_iterator& operator=(list_iterator<lent_pointer_wrapper<TT>>&& other)
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
	inline bool operator==(const list_iterator<lent_pointer_wrapper<T>>& other) const
	{
		return m_currNode == other.m_currNode;
	}
	inline bool operator!=(const list_iterator<lent_pointer_wrapper<T>>& other) const
	{
		return m_currNode != other.m_currNode;
	}
	lent_pointer_wrapper<const T> get() const
	{
		return m_currNode;
	}
	lent_pointer_wrapper<T> get()
	{
		return *m_currNode;
	}
	lent_pointer_wrapper<T> extract()
	{
		return std::move(m_currNode);
	}
	void reset()
	{
		m_currNode = nullptr;
	}

private:
	lent_pointer_wrapper<T> m_currNode;
};

//template<typename ListNodeType>
//struct list_const_iterator
//{
//	template<typename>
//	friend struct list_const_iterator;
//
//	typedef ListNodeType node_type;
//	typedef typename ListNodeType::reference_type reference_type;
//	typedef typename ListNodeType::const_reference_type const_reference_type;
//	typedef typename node_type::rreference_type rreference_type;
//	typedef typename ListNodeType::value_type value_type;
//	typedef typename ListNodeType::reference reference;
//	typedef typename ListNodeType::const_reference const_reference;
//	typedef typename ListNodeType::rreference rreference;
//	typedef typename ListNodeType::const_rreference const_rreference;
//	typedef typename ListNodeType::pointer pointer;
//	typedef typename ListNodeType::const_pointer const_pointer;
//	typedef std::ptrdiff_t difference_type;
//	typedef std::forward_iterator_tag iterator_category;
//
//	list_const_iterator(const std::nullptr_t&)
//	: m_currNode(nullptr)
//	{
//	}
//	explicit list_const_iterator(node_type i_node)
//	: m_currNode(i_node)
//	{
//	}
//	list_const_iterator(const list_const_iterator<ListNodeType>& other)
//	: m_currNode(other.m_currNode)
//	{
//	}
//	list_const_iterator(const list_iterator<ListNodeType>& other)
//	: m_currNode(other.m_currNode)
//	{
//	}
//	list_const_iterator(list_const_iterator<ListNodeType>&& other)
//	: m_currNode(std::move(other.m_currNode))
//	{
//	}
//	list_const_iterator(list_iterator<ListNodeType>&& other)
//	: m_currNode(std::move(other.m_currNode))
//	{
//	}
//	list_const_iterator operator++(int)
//	{
//		list_const_iterator res(m_currNode);
//
//		m_currNode = m_currNode->get_next_node();
//
//		return res;
//	}
//	list_const_iterator& operator++()
//	{
//		m_currNode = m_currNode->get_next_node();
//
//		return *this;
//	}
//	inline const_reference operator*() const
//	{
//		return m_currNode->get_value();
//	}
//	inline const_pointer operator->() const
//	{
//		return &(m_currNode->get_value());
//	}
//	inline bool operator==(const list_const_iterator<ListNodeType>& other) const
//	{
//		return m_currNode == other.m_currNode;
//	}
//	inline bool operator!=(const list_const_iterator<ListNodeType>& other) const
//	{
//		return m_currNode != other.m_currNode;
//	}
//	const_reference_type get() const
//	{
//		return *m_currNode;
//	}
//	rreference_type extract()
//	{
//		node_type res = std::move(m_currNode);
//
//		return std::move(*res);
//	}
//
//private:
//	node_type m_currNode;
//};

}
