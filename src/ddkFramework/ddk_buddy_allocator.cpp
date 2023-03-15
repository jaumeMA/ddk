#include "ddk_buddy_allocator.h"
#include "ddk_allocator_exceptions.h"
#include "ddk_macros.h"
#include "ddk_template_helper.h"
#include <algorithm>

namespace ddk
{

buddy_allocator::Buddy::Buddy(char* i_addr)
: m_addr(i_addr)
{
}
char* buddy_allocator::Buddy::address() const
{
	return m_addr;
}
bool buddy_allocator::Buddy::operator<(const Buddy& other) const
{
	return m_addr < other.m_addr;
}
bool buddy_allocator::Buddy::operator==(const Buddy& other) const
{
	return m_addr == other.m_addr;
}

buddy_allocator::MemoryChunk::MemoryChunk(size_t i_size)
: m_size(i_size)
{
}
size_t buddy_allocator::MemoryChunk::size() const
{
	return m_size;
}

buddy_allocator::buddy_allocator(void* i_mem, size_t i_size, size_t i_pageSize)
: m_mem(static_cast<char*>(i_mem))
, m_size(i_size)
, m_pageOrder(mpl::get_power_of_two(i_pageSize))
, m_order(mpl::get_power_of_two(m_size) - m_pageOrder)
{
	DDK_ASSERT(mpl::is_power_of_two(i_size), "You shall provide an arena whose size is power of two");

	m_buddyAllocator.emplace(m_order,buddies{m_mem});
}
void* buddy_allocator::allocate(size_t i_size) const
{
	const unsigned char sizeOrder = std::max(mpl::get_power_of_two(mpl::next_power_of_two(i_size)) - m_pageOrder,0);

	buddy_container::iterator itBuddy = m_buddyAllocator.lower_bound(sizeOrder);
	if (itBuddy == m_buddyAllocator.end())
	{
		//so it seems there is no more room
		return nullptr;
	}
	else if (itBuddy->first != sizeOrder)
	{
		//divide by two encountered entry until we reach proper order
		unsigned char currOrder = itBuddy->first;
		buddies& orderBuddies = itBuddy->second;
		order_buddy_const_iterator itCurrBuddy = orderBuddies.begin();
		char* currAddr = itCurrBuddy->address();

		//take first entry of current order and divide it
		while (currOrder > sizeOrder && currOrder > 0)
		{
			currOrder--;

			//place only second buddy
			m_buddyAllocator[currOrder].insert(currAddr + static_cast<int>(m_size >> (m_order - currOrder)));
		}

		//add it to used memory chunks
		std::pair<memory_chunks::iterator,bool> insertRes = m_usedMemoryChunks.try_emplace(currAddr,1 << (currOrder + m_pageOrder));

		//erase original buddy since its been divided
		if (insertRes.second)
		{
			orderBuddies.erase(itCurrBuddy);

			if (orderBuddies.empty())
			{
				m_buddyAllocator.erase(itBuddy);
			}

			return currAddr;
		}
		else
		{
			throw bad_allocation_exception{ "Resolved chunk is under used while allocation" };
		}
	}
	else
	{
		//extract acquired buddy and recover its address
		buddies& orderBuddies = itBuddy->second;
		order_buddy_iterator itOrderBuddy = itBuddy->second.begin();

		char* currAddr = itOrderBuddy->address();

		//add it to used memory chunks
		m_usedMemoryChunks.try_emplace(currAddr,1 << (sizeOrder + m_pageOrder));

		orderBuddies.erase(itOrderBuddy);

		if (orderBuddies.empty())
		{
			m_buddyAllocator.erase(itBuddy);
		}

		return currAddr;
	}
}
void* buddy_allocator::reallocate(void* i_ptr,size_t i_newSize) const
{
	TODO("Pending");

	return nullptr;
}
void buddy_allocator::deallocate(void* i_ptr) const
{
	char* currAddr = static_cast<char*>(i_ptr);
	memory_chunks::iterator itChunk = m_usedMemoryChunks.find(currAddr);
	if(itChunk != m_usedMemoryChunks.end())
	{
		//obtain its order and place it again in available buddies
		unsigned char currOrder = mpl::get_power_of_two(itChunk->second.size()) - m_pageOrder;

recover_buddy:
		char* targetAddr = ((currAddr - m_mem) % (2 << (currOrder + m_pageOrder + 1)) == 0) ? currAddr + static_cast<int>(m_size >> (m_order - currOrder)) : currAddr - static_cast<int>(m_size >> (m_order - currOrder));
		buddy_container::iterator itCurrBuddies = m_buddyAllocator.find(currOrder);

		if (itCurrBuddies != m_buddyAllocator.end())
		{
			buddies& currBuddies = itCurrBuddies->second;
			order_buddy_const_iterator itBuddy = itCurrBuddies->second.find(targetAddr);

			if (itBuddy != currBuddies.end())
			{
				currBuddies.erase(itBuddy);
				if (currBuddies.empty())
				{
					m_buddyAllocator.erase(itCurrBuddies);
				}
				
				currAddr = std::min(targetAddr,currAddr);
				currOrder++;

				goto recover_buddy;
			}
			else
			{
				currBuddies.emplace(currAddr);
			}
		}
		else
		{
			m_buddyAllocator[currOrder].emplace(currAddr);
		}

		m_usedMemoryChunks.erase(itChunk);
	}
	else
	{
		throw bad_deallocation_exception{"Could not find associated with provided address"};
	}
}
bool buddy_allocator::empty() const
{
	return m_buddyAllocator.empty();
}

}