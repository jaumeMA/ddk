
namespace ddk
{
namespace detail
{

template<typename T,bool Weakable>
shared_reference_wrapper_impl<T,Weakable>::shared_reference_wrapper_impl(T* i_data,const tagged_pointer<reference_counter>& i_refCounter,const IReferenceWrapperDeleter* i_refDeleter)
: shared_pointer_wrapper_impl<T,Weakable>(i_data,i_refCounter,i_refDeleter)
{
	DDK_ASSERT(i_data != NULL,"Trying to construct non null reference from void pointer");
}
template<typename T,bool Weakable>
shared_reference_wrapper_impl<T,Weakable>::shared_reference_wrapper_impl(const shared_reference_wrapper_impl& other)
: shared_pointer_wrapper_impl<T,Weakable>(other)
{
}
template<typename T,bool Weakable>
shared_reference_wrapper_impl<T,Weakable>::shared_reference_wrapper_impl(shared_reference_wrapper_impl&& other)
: shared_pointer_wrapper_impl<T,Weakable>(std::move(other))
{
}
template<typename T,bool Weakable>
template<typename TT>
shared_reference_wrapper_impl<T,Weakable>::shared_reference_wrapper_impl(const shared_reference_wrapper_impl<TT,Weakable>& other)
: shared_pointer_wrapper_impl<T,Weakable>(other)
{
}
template<typename T,bool Weakable>
template<typename TT>
shared_reference_wrapper_impl<T,Weakable>::shared_reference_wrapper_impl(shared_reference_wrapper_impl<TT,Weakable>&& other)
: shared_pointer_wrapper_impl<T,Weakable>(std::move(other))
{
}
template<typename T,bool Weakable>
shared_reference_wrapper_impl<T,Weakable>& shared_reference_wrapper_impl<T,Weakable>::operator=(const shared_reference_wrapper_impl<T,Weakable>& other)
{
	shared_pointer_wrapper_impl<T,Weakable>::operator=(other);

	return *this;
}
template<typename T,bool Weakable>
shared_reference_wrapper_impl<T,Weakable>& shared_reference_wrapper_impl<T,Weakable>::operator=(shared_reference_wrapper_impl<T,Weakable>&& other)
{
	shared_pointer_wrapper_impl<T,Weakable>::operator=(std::move(other));

	return *this;
}

}
}