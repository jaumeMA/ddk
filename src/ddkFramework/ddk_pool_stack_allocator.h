#pragma once

#include "ewas_stack_allocator_interface.h"
#include <set>

namespace ewas
{
namespace detail
{

class pool_stack_allocator : public stack_allocator_interface
{
	struct FreeBuddy
	{
	public:
		FreeBuddy(void* i_addr, size_t i_size);

		inline void decr_addr(size_t i_shift);
		inline void decr_pages(size_t i_size);
		inline void incr_pages(size_t i_size);
		inline void* ref_addr(size_t i_disp);
		inline void* edge_addr();
		inline void* addr() const;
		inline size_t pages() const;

	private:
		char* m_addr = nullptr;
		mutable size_t m_size = 0;
	};

	struct Buddy
	{
	public:
		Buddy(void* i_addr, size_t i_size = 0);
		Buddy(void* i_addr, size_t i_acquiredSize, size_t i_totalSize);
		Buddy(const Buddy&) = delete;
		Buddy(Buddy&& other);
		~Buddy();

		Buddy& operator=(const Buddy&) = delete;
		inline void* init_addr() const;
		inline void* curr_addr() const;
		inline void* ref_addr(size_t i_disp) const;
		inline void* edge_addr() const;
		inline size_t used_pages() const;
		inline size_t free_pages() const;
		inline bool empty() const;
		inline void shift_pages(size_t i_size) const;
		inline Buddy ask_free_pages(size_t i_size) const;
		inline bool has_free_pages(size_t i_size) const;
		inline void release() const;
		inline void merge(const Buddy& other) const;
		inline bool operator==(const Buddy& other) const;
		inline bool operator<(const Buddy& other) const;
		operator bool() const;

	private:
		char* m_initialAddr = nullptr;
		mutable size_t m_size = 0;
		mutable FreeBuddy* m_freeBuddy = nullptr;
	};
	static const size_t k_numGuardPages = 3;

public:
	pool_stack_allocator(stack_alloc_const_shared_ref i_nestedAllocator, size_t i_numStacks, size_t i_stackSize);
	~pool_stack_allocator();

private:
	void* reserve(size_t) const override;
	void* allocate(void* i_ref, size_t i_size) const override;
	bool reallocate(std::pair<void*,void*>& i_stackAddr, void* i_reason) const override;
	void deallocate(void*,size_t) const override;
	void release(void*,size_t) const override;

	stack_alloc_const_shared_ref m_nestedAllocator;
	mutable std::set<Buddy> m_buddyAllocator;
	void* m_allocAddr = nullptr;
	size_t m_maxPages = 0;
};

}
}