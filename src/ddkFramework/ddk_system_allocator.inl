
#if defined(__LINUX__)

namespace 
inline void* align(std::size_t alignment,std::size_t size,
                    void*& ptr,std::size_t& space) {
    std::uintptr_t pn = reinterpret_cast<std::uintptr_t>(ptr);
    std::uintptr_t aligned = (pn + alignment - 1) & -alignment;
    std::size_t padding = aligned - pn;
    if(space < size + padding) return nullptr;
    space -= padding;
    return ptr = reinterpret_cast<void*>(aligned);
}

#endif

namespace ddk
{

template<typename T>
void* system_allocator::aligned_allocate(void*& i_ptr,size_t& i_remainingSize)
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

template<typename T>
void* typed_system_allocator<T>::allocate(size_t numUnits) const
{
	return allocate(numUnits,sizeof(T));
}
template<typename T>
void* typed_system_allocator<T>::reallocate(void *ptr,size_t numUnits) const
{
	return reallocate(ptr,numUnits,sizeof(T));
}

template<typename T>
resource_deleter_const_lent_ref get_reference_wrapper_deleter(const typed_system_allocator<T>& i_allocator)
{
	return get_reference_wrapper_deleter(static_cast<const system_allocator&>(i_allocator));
}

}