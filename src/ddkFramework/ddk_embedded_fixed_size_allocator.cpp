#include "ddk_embedded_fixed_size_allocator.h"

namespace ddk
{

embedded_fixed_size_allocator::Chunk::Chunk(void* i_addr)
: m_addr(reinterpret_cast<char*>(i_addr))
{
}
char* embedded_fixed_size_allocator::Chunk::address()
{
	return get_raw_ptr(m_addr);
}
const char* embedded_fixed_size_allocator::Chunk::address() const
{
	return get_raw_ptr(m_addr);
}

embedded_fixed_size_allocator::embedded_fixed_size_allocator(void* i_mem,size_t i_size,size_t i_numChunks)
: m_mem(reinterpret_cast<char*>(i_mem))
, m_size(i_size)
, m_unitSize(i_size / i_numChunks)
{
	for (size_t index = 0;index < i_numChunks;index++)
	{
		embedded_node<Chunk>* newBuddy = new (m_mem + index * m_unitSize) embedded_node<Chunk>(m_mem + index * m_unitSize);

		m_chunks.push(make_intrusive(*newBuddy));
	}
}
embedded_fixed_size_allocator::embedded_fixed_size_allocator(embedded_fixed_size_allocator&& other)
: m_mem(other.m_mem)
, m_size(other.m_size)
, m_unitSize(other.m_unitSize)
, m_chunks(std::move(other.m_chunks))
{
}
void* embedded_fixed_size_allocator::allocate(size_t i_size) const
{
	ddk::spin_guard sg(m_barrier);

	if (m_chunks.empty())
	{
		return nullptr;
	}
	else if(m_unitSize >= i_size)
	{
		ddk::atomic_post_increment(m_numCurrentAllocations);

		iterator itChunk = m_chunks.begin();

		void* res = itChunk->address();

		m_chunks.erase(itChunk);

		return res;
	}
	else
	{
		return nullptr;
	}
}
bool embedded_fixed_size_allocator::deallocate(const void* i_address) const
{
	if (void* addr = const_cast<void*>(i_address))
	{
		embedded_node<Chunk>* newChunk = new (addr) embedded_node<Chunk>(addr);

		ddk::spin_guard sg(m_barrier);

		m_chunks.push(make_intrusive(*newChunk));

		ddk::atomic_post_decrement(m_numCurrentAllocations);

		return true;
	}
	else
	{
		return false;
	}
}
size_t embedded_fixed_size_allocator::unit_size() const
{
	return m_unitSize;
}

}