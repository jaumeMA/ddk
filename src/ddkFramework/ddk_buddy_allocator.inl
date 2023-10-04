
namespace ddk
{

template<typename T>
void buddy_allocator::deallocate(T* i_ptr)
{
	i_ptr->~T();

	deallocate(static_cast<void*>(i_ptr));
}

}