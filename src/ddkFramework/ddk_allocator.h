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
	TEMPLATE(typename DDeleter)
	REQUIRES(IS_CONSTRUCTIBLE(Deleter,DDeleter))
	deleter_proxy(const DDeleter& i_deleter);

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
	TEMPLATE(typename AAllocator)
	REQUIRES(IS_CONSTRUCTIBLE(Allocator,AAllocator))
	allocator_proxy(const AAllocator& i_allocator);

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