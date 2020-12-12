
namespace ddk
{

template<typename T>
void typed_system_reference_wrapper_allocator<T>::Deallocate(const void* i_object) const
{
	m_privateAlloc.deallocate(const_cast<void*>(i_object));
}

template<typename T>
const IReferenceWrapperDeleter* get_reference_wrapper_deleter(const system_allocator&)
{
	static typed_system_reference_wrapper_allocator<T> s_allocator;

	return &s_allocator;
}

}