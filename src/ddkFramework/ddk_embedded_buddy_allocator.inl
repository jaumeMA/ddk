
namespace ddk
{

template<unsigned char NumOrders>
embedded_buddy_allocator<NumOrders>::Buddy::Buddy(char* i_addr)
: m_addr(i_addr)
{
}
template<unsigned char NumOrders>
char* embedded_buddy_allocator<NumOrders>::Buddy::address()
{
	return m_addr;
}
template<unsigned char NumOrders>
const char* embedded_buddy_allocator<NumOrders>::Buddy::address() const
{
	return m_addr;
}
template<unsigned char NumOrders>
char* embedded_buddy_allocator<NumOrders>::Buddy::publish(unsigned char i_order)
{
	//mark its order
	return detail::set_embed_buddy_order(m_addr,i_order);
}
template<unsigned char NumOrders>
const char* embedded_buddy_allocator<NumOrders>::Buddy::publish(unsigned char i_order) const
{
	//mark its order
	return detail::set_embed_buddy_order(m_addr,i_order);
}

template<unsigned char NumOrders>
embedded_buddy_allocator<NumOrders>::embedded_buddy_allocator(void* i_mem,size_t i_size)
: m_mem(static_cast<char*>(i_mem))
, m_size(i_size)
, m_pageOrder(mpl::get_power_of_two(i_size) - NumOrders)
{
	DDK_ASSERT(mpl::is_power_of_two(i_size),"You shall provide an arena whose size is power of two");

	embedded_node<Buddy>* newBuddy = new (i_mem) embedded_node<Buddy>(m_mem);

	m_buddyLists[NumOrders - 1].push(make_intrusive(*newBuddy));
}
template<unsigned char NumOrders>
void* embedded_buddy_allocator<NumOrders>::allocate(size_t i_size) const
{
	//here we add 1 since we are going to mark this mem chunk with its order at the beginning
	const unsigned char sizeOrder = std::max(mpl::get_power_of_two(mpl::next_power_of_two(i_size + sizeof(detail::k_embedBuddyMark))) - m_pageOrder,0);

	if (sizeOrder > NumOrders)
	{
		throw bad_allocation_exception{"Allocation order exceeds limits"};
	}
	else
	{
		//look for the closes entry to sizeOrder non empty
		unsigned char currOrder = NumOrders;
		for (unsigned char order = NumOrders-1;order >= sizeOrder;order--)
		{
			if (m_buddyLists[order].empty() == false)
			{
				currOrder = order;
			}
		}

		if (currOrder == NumOrders)
		{
			return nullptr;
		}
		else if (currOrder != sizeOrder)
		{
			//divide by two encountered entry until we reach proper order
			buddy_list& orderBuddies = m_buddyLists[currOrder];
			buddy_iterator itCurrBuddy = orderBuddies.begin();
			char* currAddr = itCurrBuddy->address();

			//take first entry of current order and divide it
			while (currOrder > sizeOrder)
			{
				currOrder--;

				//place only second buddy
				char* targetAddr = currAddr + static_cast<int>(m_size >> (NumOrders - 1 - currOrder));

				embedded_node<Buddy>* newBuddy = new (targetAddr) embedded_node<Buddy>(targetAddr);

				m_buddyLists[currOrder].push(make_intrusive(*newBuddy));
			}

			char* publicAddr = itCurrBuddy->publish(currOrder + m_pageOrder);

			//erase original buddy since its been divided
			orderBuddies.erase(itCurrBuddy);

			return publicAddr;
		}
		else
		{
			//extract acquired buddy and recover its address
			buddy_list& orderBuddies = m_buddyLists[currOrder];
			buddy_iterator itOrderBuddy = orderBuddies.begin();

			char* publicAddr = itOrderBuddy->publish(currOrder + m_pageOrder);

			orderBuddies.erase(itOrderBuddy);

			return publicAddr;
		}
	}
}
template<unsigned char NumOrders>
void* embedded_buddy_allocator<NumOrders>::reallocate(void* i_ptr,size_t i_newSize) const
{
	TODO("Pending");
}
template<unsigned char NumOrders>
void embedded_buddy_allocator<NumOrders>::deallocate(void* i_ptr) const
{
	if (i_ptr != nullptr)
	{
		//recover order from its mark
		unsigned char currOrder = detail::get_embed_buddy_order(i_ptr) - m_pageOrder;

		if (currOrder < NumOrders)
		{
			char* currAddr = static_cast<char*>(i_ptr);

		recover_buddy:
			char* targetAddr = ((currAddr - m_mem) % (2 << (currOrder + m_pageOrder + 1)) == 0) ? currAddr + static_cast<int>(m_size >> (NumOrders - 1 - currOrder)) : currAddr - static_cast<int>(m_size >> (NumOrders - 1 - currOrder));

			if (m_buddyLists[currOrder].empty() == false)
			{
				buddy_list& currBuddies = m_buddyLists[currOrder];

				//here apply some fast search alg for non random access lists
				for(buddy_iterator itBuddy = currBuddies.begin();itBuddy!=currBuddies.end();++itBuddy)
				{
					if (itBuddy->address() == targetAddr)
					{
						currBuddies.erase(itBuddy);

						currAddr = std::min(currAddr,targetAddr);
						currOrder++;

						goto recover_buddy;
					}
				}

				embedded_node<Buddy>* newBuddy = new (currAddr) embedded_node<Buddy>(currAddr);

				currBuddies.push(make_intrusive(*newBuddy));
			}
			else
			{
				embedded_node<Buddy>* newBuddy = new (currAddr) embedded_node<Buddy>(currAddr);

				m_buddyLists[currOrder].push(make_intrusive(*newBuddy));
			}
		}
		else
		{
			throw bad_deallocation_exception{ "Could not find associated with provided address" };
		}
	}
}
template<unsigned char NumOrders>
bool embedded_buddy_allocator<NumOrders>::empty() const
{
	for (unsigned char currOrder = 0;currOrder < NumOrders;currOrder++)
	{
		if (m_buddyLists[currOrder].empty() == false)
		{
			return false;
		}
	}

	return true;
}

}