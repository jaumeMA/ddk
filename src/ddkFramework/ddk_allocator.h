#pragma once

#include "ddk_arena.h"
#include "ddk_template_helper.h"
#include "ddk_type_concepts.h"
#include "ddk_concepts.h"

namespace ddk
{

template<typename T>
inline void* aligned_allocate(void*& i_ptr,size_t& i_remainingSize);

template<typename Deallocator>
class deallocator_proxy
{
public:
	typedef Deallocator deallocator;
	typedef typename deallocator::type type;
	typedef type* pointer;
	typedef const type* const_pointer;
	typedef std::ptrdiff_t difference_type;

	TEMPLATE(typename DDeallocator)
	REQUIRES(IS_CONSTRUCTIBLE(Deallocator,DDeallocator))
	deallocator_proxy(const DDeallocator& i_deallocator);

	template<typename T>
	inline void deallocate(T* i_address) const;

private:
	const Deallocator& m_deallocator;
};
template<typename Deallocator>
deallocator_proxy(const Deallocator&) -> deallocator_proxy<Deallocator>;

template<typename Allocator>
class allocator_proxy
{
public:
	typedef Allocator allocator;
	typedef typename allocator::type type;
	typedef type* pointer;
	typedef const type* const_pointer;
	typedef std::ptrdiff_t difference_type;

	TEMPLATE(typename AAllocator)
	REQUIRES(IS_CONSTRUCTIBLE(Allocator,AAllocator))
	allocator_proxy(const AAllocator& i_allocator);

	inline auto allocate(size_t i_size = 1) const;
	template<typename T>
	inline void deallocate(T* i_address) const;

private:
	const Allocator& m_allocator;
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

	inline void deallocate(T* i_address) const;
};

template<typename T,typename Deallocator>
class typed_deallocator_proxy : deallocator_proxy<Deallocator>
{
public:
	typedef Deallocator deallocator;
	typedef T type;
	typedef type* pointer;
	typedef const type* const_pointer;
	typedef std::ptrdiff_t difference_type;
	using deallocator_proxy<Deallocator>::deallocate;

	TEMPLATE(typename DDeallocator)
	REQUIRES(IS_CONSTRUCTIBLE(Deallocator,DDeallocator))
	typed_deallocator_proxy(const DDeallocator& i_deallocator);

	inline void deallocate(T* i_address) const;
};

template<typename ... Allocators>
struct allocator_variant
{
	static const size_t s_num_allocators = mpl::num_types<Allocators...>;
	static const size_t s_invalidAllocator = -1;
	typedef typename mpl::max_type<Allocators...>::type dominant_type;

public:
	typedef allocator_variant allocator;
	typedef typename mpl::reduce_to_common_type<Allocators...> type;
	typedef type* pointer;
	typedef const type* const_pointer;
	typedef std::ptrdiff_t difference_type;

	template<typename Allocator>
	allocator_variant(const Allocator& i_allocator);
	allocator_variant(const allocator_variant& other);
	allocator_variant(allocator_variant&& other);
	~allocator_variant();

	template<typename Allocator>
	allocator_variant& operator=(const Allocator& i_allocator);
	inline void* allocate(size_t i_size = 1) const;
	template<typename T>
	inline void deallocate(T* i_address) const;

private:
	void construct(const allocator_variant& other);
	void construct(allocator_variant&& other);
	void destroy();
	template<typename Allocator>
	inline static void* resolve_allocate(const typed_arena<dominant_type>&, size_t i_size);
	template<typename Allocator, typename T>
	inline static void resolve_deallocate(const typed_arena<dominant_type>&, T* i_ptr);
	template<typename Allocator>
	inline static void resolve_construct(typed_arena<dominant_type>&, const typed_arena<dominant_type>& other);
	template<typename Allocator>
	inline static void resolve_construct(typed_arena<dominant_type>&, typed_arena<dominant_type>&& other);
	template<typename Allocator>
	inline static void resolve_destroy(typed_arena<dominant_type>&);

	typed_arena<dominant_type> m_allocator;
	size_t m_currAllocator = s_invalidAllocator;
};

}

#include "ddk_allocator.inl"