//////////////////////////////////////////////////////////////////////////////
//
// Author: Jaume Moragues
// Distributed under the GNU Lesser General Public License, Version 3.0. (See a copy
// at https://www.gnu.org/licenses/lgpl-3.0.ca.html)
//
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ddk_lent_reference_wrapper.h"
#include "ddk_arena.h"
#include "ddk_template_helper.h"
#include "ddk_type_concepts.h"
#include "ddk_concepts.h"

namespace ddk
{

template<typename T>
inline void* aligned_allocate(void*& i_ptr,size_t& i_remainingSize);

template<typename Allocator>
class allocator_proxy
{
public:
	typedef Allocator allocator;
	typedef typename allocator::type type;
	typedef type* pointer;
	typedef const type* const_pointer;
	typedef std::ptrdiff_t difference_type;

	allocator_proxy(const Allocator& i_allocator);

	inline auto allocate(size_t i_size = 1) const;
	inline void* reallocate(void*,size_t) const;
	TEMPLATE(typename T)
	REQUIRES(IS_CLASS(T))
	inline void deallocate(T* i_address) const;
	inline void deallocate(void* i_address) const;

private:
	const Allocator* m_allocator;
};
template<typename Allocator>
allocator_proxy(const Allocator&) -> allocator_proxy<Allocator>;

class allocator_interface
{
public:
	typedef allocator_interface allocator;
	typedef void type;
	typedef void* pointer;
	typedef const void* const_pointer;
	typedef std::ptrdiff_t difference_type;

	virtual ~allocator_interface() = default;
	virtual void* allocate(size_t) const = 0;
	virtual void* reallocate(void*,size_t) const = 0;
	virtual void deallocate(void*) const = 0;
};
typedef allocator_proxy<allocator_interface> allocator_interface_proxy;

using allocator_lent_ref = lent_reference_wrapper<allocator_interface>;
using allocator_const_lent_ref = lent_reference_wrapper<const allocator_interface>;
using allocator_lent_ptr = lent_pointer_wrapper<allocator_interface>;
using allocator_const_lent_ptr = lent_pointer_wrapper<const allocator_interface>;

}

#include "ddk_allocator.inl"