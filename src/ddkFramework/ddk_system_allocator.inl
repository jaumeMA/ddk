
#include <memory>

namespace ddk
{

TEMPLATE(typename T)
REQUIRED(IS_CLASS(T))
void system_deleter::deallocate(T* i_ptr)
{
    i_ptr->~T();

    system_deleter::deallocate(static_cast<void*>(i_ptr));
}

template<typename T>
void typed_system_deleter<T>::deallocate(T* i_ptr)
{
    i_ptr->~T();

    system_deleter::deallocate(static_cast<void*>(i_ptr));
}
template<typename T>
void typed_system_deleter<T>::deallocate(const void* i_ptr,...)
{
    system_deleter::deallocate(i_ptr);
}

template<typename T>
TEMPLATE(typename TT)
REQUIRED(IS_BASE_OF(TT,T))
typed_system_allocator<T>::typed_system_allocator(const typed_system_allocator<TT>&)
{
}
template<typename T>
void* typed_system_allocator<T>::allocate()
{
	return allocate(sizeof(T));
}
template<typename T>
void* typed_system_allocator<T>::allocate(size_t i_size)
{
    return system_allocator::allocate(i_size);
}
template<typename T>
void* typed_system_allocator<T>::reallocate(void* ptr,size_t i_newSize)
{
    return system_allocator::reallocate(ptr,i_newSize);
}
template<typename T>
TEMPLATE(typename TT)
REQUIRED(IS_BASE_OF(TT,T))
typed_system_allocator<T>& typed_system_allocator<T>::operator=(const typed_system_allocator<TT>&)
{
    return *this;
}

}
