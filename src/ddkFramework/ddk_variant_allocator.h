//////////////////////////////////////////////////////////////////////////////
//
// Author: Jaume Moragues
// Distributed under the GNU Lesser General Public License, Version 3.0. (See a copy
// at https://www.gnu.org/licenses/lgpl-3.0.ca.html)
//
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ddk_variant_impl.h"

namespace ddk
{

template<typename ... Allocators>
class variant_allocator
{
public:
	variant_allocator(const variant_allocator&) = default;
	variant_allocator(variant_allocator&&) = default;
	TEMPLATE(typename ... Args)
	REQUIRES(IS_CONSTRUCTIBLE(variant<Allocators...>,Args...))
	variant_allocator(Args&& ... i_args);
	void* allocate(size_t i_size) const;
	void* reallocate(void* i_ptr,size_t i_newSize) const;
	template<typename T>
	inline void deallocate(T* i_address) const;
	TEMPLATE(typename Allocator)
	REQUIRES(IS_ASSIGNABLE(variant<Allocators...>,Allocator))
	variant_allocator& operator=(Allocator&& i_allocator);

private:
	variant<Allocators...> m_allocators;
};

}

#include "ddk_variant_allocator.inl"