
#include <memory>

namespace ddk
{

template<typename T>
void typed_system_allocator<T>::deallocate(T* i_ptr) const
{
    if(i_ptr)
    {
        if constexpr (std::is_class<T>::value)
        {
            i_ptr->~T();
        }

        system_allocator::deallocate(i_ptr);
    }
}
template<typename T>
void typed_system_allocator<T>::deallocate(const void* i_ptr,...) const
{
    system_allocator::deallocate(i_ptr);
}

template<typename T>
TEMPLATE(typename TT)
REQUIRED(IS_BASE_OF(TT,T))
typed_system_allocator<T>::typed_system_allocator(const typed_system_allocator<TT>&)
{
}
template<typename T>
void* typed_system_allocator<T>::allocate() const
{
	return allocate(sizeof(T));
}
template<typename T>
TEMPLATE(typename TT)
REQUIRED(IS_BASE_OF(TT,T))
typed_system_allocator<T>& typed_system_allocator<T>::operator=(const typed_system_allocator<TT>&)
{
    return *this;
}

}
