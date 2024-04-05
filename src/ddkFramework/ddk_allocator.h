#pragma once

#include "ddk_arena.h"
#include "ddk_template_helper.h"
#include "ddk_type_concepts.h"
#include "ddk_concepts.h"

namespace ddk
{

template<typename T>
inline void* aligned_allocate(void*& i_ptr,size_t& i_remainingSize);

template<typename Deleter>
class deleter_proxy
{
public:
	deleter_proxy(const Deleter& i_deleter);

	template<typename T>
	inline void deallocate(T* i_address) const;

private:
	const Deleter* m_deleter;
};
template<typename Deleter>
deleter_proxy(const Deleter&) -> deleter_proxy<Deleter>;

template<typename Allocator>
class allocator_proxy : public deleter_proxy<Allocator>
{
public:
	typedef Allocator allocator;
	typedef typename allocator::type type;
	typedef type* pointer;
	typedef const type* const_pointer;
	typedef std::ptrdiff_t difference_type;

	allocator_proxy(const Allocator& i_allocator);

	inline auto allocate(size_t i_size = 1) const;

private:
	const Allocator* m_allocator;
};
template<typename Allocator>
allocator_proxy(const Allocator&) -> allocator_proxy<Allocator>;

template<typename T, typename Allocator>
class typed_allocator_proxy : allocator_proxy<Allocator>
{
public:
	typedef Allocator allocator;
	typedef T type;
	typedef type* pointer;
	typedef const type* const_pointer;
	typedef std::ptrdiff_t difference_type;
	using allocator_proxy<Allocator>::allocate;
	using allocator_proxy<Allocator>::deallocate;

	TEMPLATE(typename AAllocator)
	REQUIRES(IS_CONSTRUCTIBLE(Allocator,AAllocator))
	typed_allocator_proxy(const AAllocator& i_deallocator);
};

template<typename T,typename Deleter>
class typed_deleter_proxy : deleter_proxy<Deleter>
{
public:
	typedef Deleter deleter;
	typedef T type;
	typedef type* pointer;
	typedef const type* const_pointer;
	typedef std::ptrdiff_t difference_type;
	using deleter_proxy<Deleter>::deallocate;

	TEMPLATE(typename DDeleter)
	REQUIRES(IS_CONSTRUCTIBLE(Deleter,DDeleter))
	typed_deleter_proxy(const DDeleter& i_deleter);
};

class allocator_interface
{
public:
	virtual ~allocator_interface() = default;
	virtual void* allocate(size_t) const = 0;
	virtual void* reallocate(void*,size_t) const = 0;
	virtual void deallocate(const void*) const = 0;
};

using allocator_lent_ref = lent_reference_wrapper<allocator_interface>;
using allocator_const_lent_ref = lent_reference_wrapper<const allocator_interface>;
using allocator_lent_ptr = lent_pointer_wrapper<allocator_interface>;
using allocator_const_lent_ptr = lent_pointer_wrapper<const allocator_interface>;


}

#include "ddk_allocator.inl"