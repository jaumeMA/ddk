#include "ddk_pool_stack_allocator.h"
#include "ddk_macros.h"
#include "ddk_reference_wrapper.h"
#include <algorithm>
#include <cmath>

#if defined(WIN32)

#include <windows.h>

#elif defined(__LINUX__) || defined(__APPLE__)

#include <sys/mman.h>

#endif

namespace ddk
{
namespace detail
{

pool_stack_allocator::FreeBuddy::FreeBuddy(void* i_addr, size_t i_size)
: m_addr(reinterpret_cast<char*>(i_addr))
, m_size(i_size)
{
}
void pool_stack_allocator::FreeBuddy::decr_addr(size_t i_shift)
{
	m_addr -= i_shift * s_pageSize;
}
void pool_stack_allocator::FreeBuddy::incr_pages(size_t i_size)
{
	m_size += i_size;
}
void pool_stack_allocator::FreeBuddy::decr_pages(size_t i_size)
{
	m_size -= i_size;
}
void* pool_stack_allocator::FreeBuddy::edge_addr()
{
	return m_addr - m_size * s_pageSize;
}
void* pool_stack_allocator::FreeBuddy::ref_addr(size_t i_disp)
{
	return m_addr - (m_size - i_disp) * s_pageSize;
}
void* pool_stack_allocator::FreeBuddy::addr() const
{
	return m_addr;
}
size_t pool_stack_allocator::FreeBuddy::pages() const
{
	return m_size;
}

pool_stack_allocator::Buddy::Buddy(void* i_addr, size_t i_size)
: m_initialAddr(reinterpret_cast<char*>(i_addr))
, m_size(i_size)
, m_freeBuddy(nullptr)
{
}
pool_stack_allocator::Buddy::Buddy(void* i_addr, size_t i_acquiredSize, size_t i_totalSize)
: m_initialAddr(reinterpret_cast<char*>(i_addr))
, m_size(i_acquiredSize)
, m_freeBuddy(nullptr)
{
	m_freeBuddy = new FreeBuddy(m_initialAddr,i_totalSize - i_acquiredSize);
}
pool_stack_allocator::Buddy::Buddy(Buddy&& other)
: m_initialAddr(nullptr)
, m_size(0)
, m_freeBuddy(nullptr)
{
	std::swap(m_initialAddr,other.m_initialAddr);
	std::swap(m_size,other.m_size);
	std::swap(m_freeBuddy,other.m_freeBuddy);
}
pool_stack_allocator::Buddy::~Buddy()
{
	if(m_freeBuddy)
	{
		DDK_FAIL("Pending free buddy on destruction");

		delete m_freeBuddy;

		m_freeBuddy = nullptr;
	}
}
bool pool_stack_allocator::Buddy::operator==(const Buddy& other) const
{
	return m_initialAddr == other.m_initialAddr;
}
bool pool_stack_allocator::Buddy::operator<(const Buddy& other) const
{
	return m_initialAddr < other.m_initialAddr;
}
void* pool_stack_allocator::Buddy::init_addr() const
{
	return m_initialAddr;
}
void* pool_stack_allocator::Buddy::curr_addr() const
{
	return m_initialAddr - m_size * s_pageSize;
}
void* pool_stack_allocator::Buddy::ref_addr(size_t i_disp) const
{
	return m_initialAddr - (m_size - i_disp) * s_pageSize;
}
void* pool_stack_allocator::Buddy::edge_addr() const
{
	return m_initialAddr - m_size * s_pageSize;
}
size_t pool_stack_allocator::Buddy::used_pages() const
{
	return m_size;
}
size_t pool_stack_allocator::Buddy::free_pages() const
{
	return (m_freeBuddy) ? m_freeBuddy->pages() : 0;
}
bool pool_stack_allocator::Buddy::empty() const
{
	return (m_freeBuddy) ? (m_freeBuddy->pages() == 0) : true;
}
void pool_stack_allocator::Buddy::shift_pages(size_t i_size) const
{
	m_size += i_size;

	if(m_freeBuddy)
	{
		m_freeBuddy->decr_addr(i_size);
		m_freeBuddy->decr_pages(i_size);
	}
}
bool pool_stack_allocator::Buddy::has_free_pages(size_t i_size) const
{
	return (m_freeBuddy) ? m_freeBuddy->pages() > i_size : false;
}
pool_stack_allocator::Buddy pool_stack_allocator::Buddy::ask_free_pages(size_t i_size) const
{
	if(m_freeBuddy && m_freeBuddy->pages() >= i_size)
	{
		const size_t newSize = std::min(m_freeBuddy->pages(),(m_size + m_freeBuddy->pages()) / 2);
		void* newRef = m_freeBuddy->ref_addr(newSize);

		m_freeBuddy->decr_pages(newSize);

		return Buddy(newRef,i_size,newSize);
	}
	else
	{
		return Buddy(m_initialAddr,i_size,m_size);
	}
}
void pool_stack_allocator::Buddy::release() const
{
	if(m_freeBuddy)
	{
		m_size += m_freeBuddy->pages();

		delete m_freeBuddy;

		m_freeBuddy = nullptr;
	}
}
void pool_stack_allocator::Buddy::merge(const Buddy& other) const
{
	if(m_freeBuddy == nullptr)
	{
		m_freeBuddy = new FreeBuddy(m_initialAddr + m_size * s_pageSize,0);
	}

	m_freeBuddy->incr_pages(other.m_size);

	if(other.m_freeBuddy)
	{
		m_freeBuddy->incr_pages(other.m_freeBuddy->pages());

		delete other.m_freeBuddy;

		other.m_freeBuddy = nullptr;
	}
}
pool_stack_allocator::Buddy::operator bool() const
{
	return m_initialAddr != nullptr;
}

pool_stack_allocator::pool_stack_allocator(stack_alloc_const_dist_ref i_nestedAllocator, size_t i_numStacks, size_t i_stackSize)
: m_nestedAllocator(i_nestedAllocator)
, m_maxPages(0)
{
	m_maxPages = i_stackSize * static_cast<size_t>(std::pow(2,std::ceil(std::log2(i_numStacks))));

	m_allocAddr = m_nestedAllocator->reserve(m_maxPages);
}
pool_stack_allocator::pool_stack_allocator(stack_alloc_const_dist_ref i_nestedAllocator,size_t i_maxPages)
: m_nestedAllocator(i_nestedAllocator)
, m_maxPages(i_maxPages)
{
	m_allocAddr = m_nestedAllocator->reserve(m_maxPages);
}
pool_stack_allocator::~pool_stack_allocator()
{
	DDK_ASSERT(m_buddyAllocator.empty(), "Pending allocations on destruction");

	m_nestedAllocator->release(m_allocAddr,m_maxPages);
}
stack_alloc_const_dist_ref pool_stack_allocator::share() const
{
	return make_distributed_reference<pool_stack_allocator>(m_nestedAllocator->share(),m_maxPages);
}
stack_alloc_dist_ref pool_stack_allocator::share()
{
	return make_distributed_reference<pool_stack_allocator>(m_nestedAllocator->share(),m_maxPages);
}
void* pool_stack_allocator::reserve(size_t i_size) const
{
	void* res = nullptr;

	if(m_buddyAllocator.empty())
	{
		std::pair<std::set<Buddy>::iterator,bool> insertRes = m_buddyAllocator.emplace(m_allocAddr,0,m_maxPages);

		if(insertRes.second)
		{
			res = insertRes.first->init_addr();
		}
	}
	else
	{
		//look for the biggest empty space
		std::set<Buddy>::iterator itChunk = std::max_element(m_buddyAllocator.begin(),m_buddyAllocator.end(),[](const Buddy& i_lhs, const Buddy& i_rhs){ return i_lhs.free_pages() < i_rhs.free_pages(); });

		//ask for space
		if(itChunk != m_buddyAllocator.end())
		{
			if(Buddy newBuddy = itChunk->ask_free_pages(0))
			{
				res = newBuddy.init_addr();

				auto res = m_buddyAllocator.insert(itChunk,std::move(newBuddy));
			}
		}
	}

	return res;
}
std::pair<void*,void*> pool_stack_allocator::allocate(void* i_ref, size_t i_size) const
{
	std::set<Buddy>::iterator itBuddy = m_buddyAllocator.find(i_ref);
	if(itBuddy != m_buddyAllocator.end())
	{
		itBuddy->shift_pages(i_size);

		return m_nestedAllocator->allocate(i_ref,i_size);
	}
	else
	{
		return {nullptr,nullptr};
	}
}
bool pool_stack_allocator::reallocate(execution_stack& i_stackAddr, void* i_reason) const
{
	std::set<Buddy>::const_iterator itBuddy = m_buddyAllocator.find(i_stackAddr.get_init());

	if(itBuddy != m_buddyAllocator.end())
	{
		if(itBuddy->has_free_pages(1))
		{
			if(m_nestedAllocator->reallocate(i_stackAddr,i_reason))
			{
				itBuddy->shift_pages(1);

				return true;
			}
		}
	}

	return false;
}
void pool_stack_allocator::deallocate(void* i_address, size_t i_size) const
{
	m_nestedAllocator->deallocate(i_address,i_size);
}
void pool_stack_allocator::release(void* i_ref, size_t i_size) const
{
	std::set<Buddy>::const_iterator itBuddy = m_buddyAllocator.find(i_ref);

	if(itBuddy != m_buddyAllocator.end())
	{
		std::set<Buddy>::const_iterator itNext = std::next(itBuddy);

		if(itNext != m_buddyAllocator.end())
		{
			itNext->merge(*itBuddy);
		}
		else
		{
			itBuddy->release();
		}

		m_buddyAllocator.erase(itBuddy);
	}
	else
	{
		DDK_FAIL("Trying to remove unexistant stack allocation");
	}
}
size_t pool_stack_allocator::get_num_guard_pages() const
{
	return m_nestedAllocator->get_num_guard_pages();
}

}
}
