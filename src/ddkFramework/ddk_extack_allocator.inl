
namespace ddk
{

TEMPLATE(typename T)
REQUIRED(IS_CLASS(T))
void extack_allocator::deallocate(T* i_ptr) const
{
	i_ptr->~T();

	deallocate(static_cast<void*>(i_ptr));
}

}