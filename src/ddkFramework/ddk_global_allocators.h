#pragma once

#include "ddk_fixed_size_allocator.h"
#include "ddk_system_allocator.h"

#define __DDK_APPEND_GLOBAL_ALLOCATOR(_ID,_EXPR) \
	const bool _##_ID = _EXPR;

#define _DDK_APPEND_GLOBAL_ALLOCATOR(_ID,_EXPR) \
	__DDK_APPEND_GLOBAL_ALLOCATOR(_ID,_EXPR)
#define DDK_APPEND_GLOBAL_ALLOCATOR(...) \
	_DDK_APPEND_GLOBAL_ALLOCATOR(__LINE__,::ddk::__append_global_allocator_map_entries(__VA_ARGS__))

namespace ddk
{

template<typename Allocator>
class fixed_size_or_allocator
{
public:
	template<typename AAllocator>
	fixed_size_or_allocator(size_t i_fixedSize,AAllocator&& i_secondaryAllocator);
	template<typename AAllocator>
	fixed_size_or_allocator(const slab_allocator* i_primaryAllocator, AAllocator&& i_secondaryAllocator);
	void* allocate(size_t i_size) const;
	template<typename TT>
	void deallocate(TT* i_ptr) const;

private:
	mutable detail::compressed_pair<const slab_allocator*,Allocator> m_allocator;
};

template<typename Allocator>
class fixed_size_allocate_or
{
public:
	fixed_size_allocate_or(size_t i_fixedSize, const Allocator& = Allocator{});

	fixed_size_or_allocator<Allocator> acquire() const;

private:
	size_t m_fixedSize;
	Allocator m_allocator;
};

//null deleter
class null_deleter: public resource_deleter_interface
{
public:
	void deallocate(const void* i_object) const final;
};

extern const ddk::null_deleter g_nullDeleter;

}

#include "ddk_global_allocators.inl"