
namespace ddk
{

template<typename T,typename Allocator>
lock_free_stack<T,Allocator>::lock_free_stack()
: m_head(leaf_node)
, m_tail(leaf_node)
{
	if(void* mem = m_allocator.allocate(1))
	{
		lock_free_stack_node<T>* divider = new (mem) lock_free_stack_node<T>(divider_node);

		m_head.set_next(divider);
		m_tail.set_next(divider);
	}

}
template<typename T,typename Allocator>
lock_free_stack<T,Allocator>::~lock_free_stack()
{
	clear();
}
template<typename T,typename Allocator>
void lock_free_stack<T,Allocator>::clear()
{
	while(empty() == false)
	{
		pop();
	}

	deallocate_node(m_head.get_next());
}
template<typename T,typename Allocator>
template<typename ... Args>
void lock_free_stack<T,Allocator>::push(Args&& ... i_args)
{
	if(lock_free_stack_node<T>* newNode = allocate_node(std::forward<Args>(i_args) ...))
	{
		_push(newNode);
	}
}
template<typename T,typename Allocator>
optional<T> lock_free_stack<T,Allocator>::pop()
{
	lock_free_stack_node<T>* firstNode = nullptr;
	lock_free_stack_node<T>* nextNode = nullptr;

	if(empty() == false)
	{
		do
		{
			firstNode = m_head.get_next();

			if(firstNode->is_divider())
			{
				return none;
			}

			nextNode = firstNode->get_next();
		} while(m_head.compareAndExchangeNextNode(firstNode,nextNode) == false);

		if(firstNode->is_divider() == false)
		{
			T res = firstNode->extract_value();

			deallocate_node(firstNode);

			return std::move(res);
		}
		else
		{
			DDK_FAIL("Unconsistent situation");
		}
	}

	return none;
}
template<typename T,typename Allocator>
bool lock_free_stack<T,Allocator>::empty() const
{
	return m_head.get_next()->is_divider();
}
template<typename T,typename Allocator>
void lock_free_stack<T,Allocator>::_push(lock_free_stack_node<T>* i_newNode)
{
	static const int s_sleepTime = 5;
	lock_free_stack_node<T>* lastNode = nullptr;
	size_t numOfRetries = 0;

	do
	{
		lastNode = m_tail.get_next();
	} while(m_tail.compareAndExchangeNextNode(lastNode,i_newNode) == false);

	while(lastNode->is_divider() == false)
	{
		std::this_thread::yield();
	}

	lastNode->set_value(i_newNode->extract_value());

	lastNode->set_next(i_newNode);

	i_newNode->set_divider(true);

	lastNode->set_divider(false);
}
template<typename T,typename Allocator>
template<typename ... Args>
lock_free_stack_node<T>* lock_free_stack<T,Allocator>::allocate_node(Args&& ... i_args)
{
	if(void* mem = m_allocator.allocate(1))
	{
		return new (mem) lock_free_stack_node<T>(std::forward<Args>(i_args) ...);
	}
	else
	{
		return nullptr;
	}
}
template<typename T,typename Allocator>
void lock_free_stack<T,Allocator>::deallocate_node(lock_free_stack_node<T>* i_node)
{
	if(i_node)
	{
		i_node->~lock_free_stack_node<T>();

		m_allocator.deallocate(i_node);
	}
}

}