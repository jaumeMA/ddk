
//#define DETECT_CYCLES

namespace ddk
{

template<typename T,template<typename> typename Pointer>
intrusive_list<T,Pointer>::intrusive_list()
: m_firstNode(nullptr)
, m_lastNode(nullptr)
{
}
template<typename T,template<typename> typename Pointer>
intrusive_list<T,Pointer>::intrusive_list(const intrusive_list<T>& other)
: m_firstNode(other.m_firstNode)
, m_lastNode(other.m_lastNode)
{
}
template<typename T,template<typename> typename Pointer>
intrusive_list<T,Pointer>::intrusive_list(intrusive_list<T>&& other)
: m_firstNode(nullptr)
, m_lastNode(nullptr)
{
	std::swap(m_firstNode,other.m_firstNode);
	std::swap(m_lastNode,other.m_lastNode);
}
template<typename T,template<typename> typename Pointer>
intrusive_list<T,Pointer>::~intrusive_list()
{
	clear();
}
template<typename T,template<typename> typename Pointer>
void intrusive_list<T,Pointer>::push(const pointer_t& i_node)
{
#ifdef DETECT_CYCLES
	DDK_ASSERT(already_contains_node(i_node) == false, "Trying to insert already present node, about to produce a cycle");
#endif

	if(m_firstNode == nullptr)
	{
		m_firstNode = i_node;
		m_lastNode = m_firstNode;
	}
	else
	{
		m_lastNode->set_next_node(i_node);
		m_lastNode = i_node;
	}
}
template<typename T,template<typename> typename Pointer>
void intrusive_list<T,Pointer>::pop()
{
	if(pointer_t firstNode = m_firstNode)
	{
		if(pointer_t firstNode = m_firstNode->extract_next_node())
		{
			m_firstNode = firstNode;
		}
		else
		{
			m_firstNode = m_lastNode = nullptr;
		}
	}
}
template<typename T,template<typename> typename Pointer>
void intrusive_list<T,Pointer>::clear()
{
	pointer_t currNode = m_firstNode;

	while(currNode)
	{
		pointer_t nextNode = currNode->extract_next_node();
		currNode = nextNode;
	}

	m_firstNode = nullptr;
	m_lastNode = nullptr;
}
template<typename T,template<typename> typename Pointer>
typename intrusive_list<T,Pointer>::iterator intrusive_list<T,Pointer>::begin()
{
	return iterator(m_firstNode);
}
template<typename T,template<typename> typename Pointer>
typename intrusive_list<T,Pointer>::const_iterator intrusive_list<T,Pointer>::begin() const
{
	return const_iterator(m_firstNode);
}
template<typename T,template<typename> typename Pointer>
typename intrusive_list<T,Pointer>::const_iterator intrusive_list<T,Pointer>::cbegin() const
{
	return const_iterator(m_firstNode);
}
template<typename T,template<typename> typename Pointer>
typename intrusive_list<T,Pointer>::iterator intrusive_list<T,Pointer>::end()
{
	return nullptr;
}
template<typename T,template<typename> typename Pointer>
typename intrusive_list<T,Pointer>::const_iterator intrusive_list<T,Pointer>::end() const
{
	return nullptr;
}
template<typename T,template<typename> typename Pointer>
typename intrusive_list<T,Pointer>::const_iterator intrusive_list<T,Pointer>::cend() const
{
	return nullptr;
}
template<typename T,template<typename> typename Pointer>
typename intrusive_list<T,Pointer>::iterator intrusive_list<T,Pointer>::erase(iterator i_it)
{
	pointer_t nextNode = nullptr;

	if(pointer_t currNode = i_it.extract())
	{
		if(m_firstNode == currNode)
		{
			m_firstNode = currNode->get_next_node();
		}
		if(m_lastNode == currNode)
		{
			m_lastNode = currNode->get_prev_node();
		}

		nextNode = detail::intrusive_node_impl<T,Pointer>::collapse(std::move(currNode));
	}

	return iterator(nextNode);
}
template<typename T,template<typename> typename Pointer>
size_t intrusive_list<T,Pointer>::size() const
{
	size_t res = 0;

	if(pointer_t currNode = m_firstNode)
	{
		do
		{
			++res;
		}
		while(currNode = currNode->get_next_node());
	}

	return res;
}
template<typename T,template<typename> typename Pointer>
bool intrusive_list<T,Pointer>::empty() const
{
	return m_firstNode == nullptr;
}
template<typename T,template<typename> typename Pointer>
bool intrusive_list<T,Pointer>::already_contains_node(pointer_t i_node) const
{
	if(pointer_t currNode = m_firstNode)
	{
		do
		{
			if(currNode == i_node)
			{
				return true;
			}
		}
		while(currNode = currNode->get_next_node());
	}

	return false;
}

}