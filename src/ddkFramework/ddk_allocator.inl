
namespace ddk
{

template<typename T>
void* aligned_allocate(void*& i_ptr,size_t& i_remainingSize)
{
	static const size_t alignment = alignof(T);

	if(void* res = std::align(alignment,sizeof(T),i_ptr,i_remainingSize))
	{
		i_ptr = reinterpret_cast<char*>(i_ptr) + sizeof(T);

		i_remainingSize -= sizeof(T);

		return res;
	}
	else
	{
		return nullptr;
	}
}

template<typename Deallocator>
TEMPLATE(typename DDeallocator)
REQUIRED(IS_CONSTRUCTIBLE(Deallocator,DDeallocator))
deallocator_proxy<Deallocator>::deallocator_proxy(const DDeallocator& i_deallocator)
: m_deallocator(i_deallocator)
{
}
template<typename Deallocator>
template<typename T>
void deallocator_proxy<Deallocator>::deallocate(T* i_address) const
{
	m_deallocator.deallocate(i_address);
}

template<typename Allocator>
TEMPLATE(typename AAllocator)
REQUIRED(IS_CONSTRUCTIBLE(Allocator,AAllocator))
allocator_proxy<Allocator>::allocator_proxy(const AAllocator& i_allocator)
: m_allocator(i_allocator)
{
}
template<typename Allocator>
auto allocator_proxy<Allocator>::allocate(size_t i_size) const
{
	return m_allocator.allocate(i_size);
}
template<typename Allocator>
template<typename T>
void allocator_proxy<Allocator>::deallocate(T* i_address) const
{
	m_allocator.deallocate(i_address);
}

template<typename T,typename Allocator>
TEMPLATE(typename AAllocator)
REQUIRED(IS_CONSTRUCTIBLE(AAllocator,Allocator))
typed_allocator_proxy<T,Allocator>::typed_allocator_proxy(const AAllocator& i_deallocator)
: allocator_proxy<Allocator>(i_deallocator)
{
}
template<typename T, typename Allocator>
void typed_allocator_proxy<T,Allocator>::deallocate(type* i_ptr) const
{
	if(i_ptr)
	{
		i_ptr->~T();

		allocator_proxy<Allocator>::deallocate(i_ptr);
	}
}

template<typename T,typename Deallocator>
TEMPLATE(typename DDeallocator)
REQUIRED(IS_CONSTRUCTIBLE(Deallocator,DDeallocator))
typed_deallocator_proxy<T,Deallocator>::typed_deallocator_proxy(const DDeallocator& i_deallocator)
: deallocator_proxy<Deallocator>(i_deallocator)
{
}
template<typename T,typename Deallocator>
void typed_deallocator_proxy<T,Deallocator>::deallocate(type* i_ptr) const
{
	if(i_ptr)
	{
		i_ptr->~T();

		deallocator_proxy<Deallocator>::deallocate(i_ptr);
	}
}

template<typename ... Allocators>
template<typename Allocator>
allocator_variant<Allocators...>::allocator_variant(const Allocator& i_allocator)
{
	static_assert(mpl::is_among_constructible_types<Allocator,Allocators...>, "You shall provide a match type among Allocators.");

	typedef typename mpl::nth_type_of<mpl::type_match_pos<Allocator,Allocators...>,Allocators...>::type allocator_t;

	m_currAllocator = mpl::type_match_pos<Allocator,Allocators...>;

	m_allocator.template construct<allocator_t>(i_allocator);
}
template<typename ... Allocators>
allocator_variant<Allocators...>::allocator_variant(const allocator_variant& other)
{
	construct(other);
}
template<typename ... Allocators>
allocator_variant<Allocators...>::allocator_variant(allocator_variant&& other)
{
	construct(std::move(other));
}
template<typename ... Allocators>
allocator_variant<Allocators...>::~allocator_variant()
{
	destroy();
}
template<typename ... Allocators>
template<typename Allocator>
allocator_variant<Allocators...>& allocator_variant<Allocators...>::operator=(const Allocator& i_allocator)
{
	static_assert(mpl::is_among_constructible_types<Allocator,Allocators...>,"You shall provide a match type among Allocators.");

	typedef typename mpl::nth_type_of<mpl::type_match_pos<Allocator,Allocators...>,Allocators...>::type allocator_t;

	destroy();

	m_currAllocator = mpl::type_match_pos<Allocator,Allocators...>;

	m_allocator.template construct<allocator_t>(i_allocator);

	return *this;
}
template<typename ... Allocators>
void allocator_variant<Allocators...>::construct(const allocator_variant& other)
{
	typedef void(*funcType)(typed_arena<dominant_type>&,const typed_arena<dominant_type>&);

	static const funcType funcTable[s_num_allocators] = { &allocator_variant<Allocators...>::resolve_construct<Allocators> ... };

	if(other.m_currAllocator != s_invalidAllocator)
	{
		m_currAllocator = other.m_currAllocator;

		(*funcTable[m_currAllocator])(m_allocator,other.m_allocator);
	}
}
template<typename ... Allocators>
void allocator_variant<Allocators...>::construct(allocator_variant&& other)
{
	typedef void(*funcType)(typed_arena<dominant_type>&,typed_arena<dominant_type>&&);

	static const funcType funcTable[s_num_allocators] = { &allocator_variant<Allocators...>::resolve_construct<Allocators> ... };

	if(other.m_currAllocator != s_invalidAllocator)
	{
		m_currAllocator = other.m_currAllocator;

		(*funcTable[m_currAllocator])(m_allocator,std::move(other.m_allocator));

		other.m_currAllocator = s_invalidAllocator;
	}
}
template<typename ... Allocators>
void* allocator_variant<Allocators...>::allocate(size_t i_size) const
{
	typedef void*(*funcType)(const typed_arena<dominant_type>&,size_t);

	static const funcType funcTable[s_num_allocators] = { &allocator_variant<Allocators...>::resolve_allocate<Allocators> ... };

	if(m_currAllocator != s_invalidAllocator)
	{
		return (*funcTable[m_currAllocator])(m_allocator,i_size);
	}
	else
	{
		return nullptr;
	}
}
template<typename ... Allocators>
template<typename T>
void allocator_variant<Allocators...>::deallocate(T* i_address) const
{
	typedef void(*funcType)(const typed_arena<dominant_type>&,T*);

	static const funcType funcTable[s_num_allocators] = { &allocator_variant<Allocators...>::resolve_deallocate<Allocators,T> ... };

	if(m_currAllocator != s_invalidAllocator)
	{
		(*funcTable[m_currAllocator])(m_allocator,i_address);
	}
}
template<typename ... Allocators>
void allocator_variant<Allocators...>::destroy()
{
	typedef void(*funcType)(typed_arena<dominant_type>&);

	static const funcType funcTable[s_num_allocators] = { &allocator_variant<Allocators...>::resolve_destroy<Allocators> ... };

	if(m_currAllocator != s_invalidAllocator)
	{
		(*funcTable[m_currAllocator])(m_allocator);
	}
}
template<typename ... Allocators>
template<typename Allocator>
void* allocator_variant<Allocators...>::resolve_allocate(const typed_arena<dominant_type>& i_allocator, size_t i_size)
{
	if(i_allocator.empty() == false)
	{
		return i_allocator.template get<Allocator>().allocate(i_size);
	}
	else
	{
		return nullptr;
	}
}
template<typename ... Allocators>
template<typename Allocator, typename T>
void allocator_variant<Allocators...>::resolve_deallocate(const typed_arena<dominant_type>& i_allocator, T* i_ptr)
{
	if(i_allocator.empty() == false)
	{
		i_allocator.template get<Allocator>().deallocate(i_ptr);
	}
}
template<typename ... Allocators>
template<typename Allocator>
void allocator_variant<Allocators...>::resolve_construct(typed_arena<dominant_type>& i_allocator,const typed_arena<dominant_type>& other)
{
	i_allocator.template construct<Allocator>(other.template get<Allocator>());
}
template<typename ... Allocators>
template<typename Allocator>
void allocator_variant<Allocators...>::resolve_construct(typed_arena<dominant_type>& i_allocator, typed_arena<dominant_type>&& other)
{
	i_allocator.template construct<Allocator>(other.template extract<Allocator>());
}
template<typename ... Allocators>
template<typename Allocator>
void allocator_variant<Allocators...>::resolve_destroy(typed_arena<dominant_type>& i_allocator)
{
	i_allocator.template destroy<Allocator>();
}

}
