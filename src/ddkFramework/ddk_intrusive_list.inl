
namespace ewas
{

template<typename T>
intrusive_list<T>::intrusive_list()
: m_firstNode(nullptr)
, m_lastNode(nullptr)
{
}
template<typename T>
intrusive_list<T>::intrusive_list(const intrusive_list<T>& other)
: m_firstNode(other.m_firstNode)
, m_lastNode(other.m_lastNode)
{
}
template<typename T>
intrusive_list<T>::intrusive_list(intrusive_list<T>&& other)
: m_firstNode(nullptr)
, m_lastNode(nullptr)
{
	std::swap(m_firstNode,other.m_firstNode);
	std::swap(m_lastNode,other.m_lastNode);
}
template<typename T>
intrusive_list<T>::~intrusive_list()
{
	clear();
}
template<typename T>
void intrusive_list<T>::push(const intrusive_node_ref& i_node)
{
#ifdef DETECT_CYCLES
	EWAS_ASSERT(already_contains_node(i_node) == false, "Trying to insert already present node, about to produce a cycle");
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
template<typename T>
void intrusive_list<T>::pop()
{
	if(intrusive_node_ptr firstNode = m_firstNode)
	{
		if(intrusive_node_ptr firstNode = m_firstNode->extract_next_node())
		{
			m_firstNode = firstNode;
		}
		else
		{
			m_firstNode = m_lastNode = nullptr;
		}
	}
}
template<typename T>
void intrusive_list<T>::clear()
{
	intrusive_node_ptr currNode = m_firstNode;

	while(currNode)
	{
		intrusive_node_ptr nextNode = currNode->extract_next_node();
		currNode = nextNode;
	}

	m_firstNode = nullptr;
	m_lastNode = nullptr;
}
template<typename T>
typename intrusive_list<T>::iterator intrusive_list<T>::begin()
{
	return iterator(m_firstNode);
}
template<typename T>
typename intrusive_list<T>::const_iterator intrusive_list<T>::begin() const
{
	return const_iterator(m_firstNode);
}
template<typename T>
typename intrusive_list<T>::iterator intrusive_list<T>::end()
{
	return nullptr;
}
template<typename T>
typename intrusive_list<T>::const_iterator intrusive_list<T>::end() const
{
	return nullptr;
}
template<typename T>
typename intrusive_list<T>::iterator intrusive_list<T>::erase(iterator i_it)
{
	intrusive_node_ptr nextNode = nullptr;

	if(intrusive_node_ptr currNode = i_it.extract())
	{
		if(m_firstNode == currNode)
		{
			m_firstNode = currNode->extract_next_node();
		}
		if(m_lastNode == currNode)
		{
			m_lastNode = currNode->extract_prev_node();
		}

		nextNode = detail::intrusive_node<T>::collapse(std::move(currNode));
	}

	return iterator(nextNode);
}
template<typename T>
size_t intrusive_list<T>::size() const
{
	size_t res = 0;

	if(intrusive_node_ptr currNode = m_firstNode)
	{
		do
		{
			++res;
		}
		while(currNode = currNode->get_next_node());
	}

	return res;
}
template<typename T>
bool intrusive_list<T>::empty() const
{
	return m_firstNode == nullptr;
}
template<typename T>
bool intrusive_list<T>::already_contains_node(intrusive_node_ptr i_node) const
{
	if(intrusive_node_ptr currNode = m_firstNode)
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