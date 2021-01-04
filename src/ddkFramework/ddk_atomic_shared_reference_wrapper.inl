
namespace ddk
{
namespace detail
{

template<typename T, bool Weakable>
atomic_shared_reference_wrapper_impl<T,Weakable>::atomic_shared_reference_wrapper_impl(const atomic_shared_reference_wrapper_impl& other)
: atomic_shared_pointer_wrapper<T>(other)
{
}
template<typename T,bool Weakable>
atomic_shared_reference_wrapper_impl<T,Weakable>::atomic_shared_reference_wrapper_impl(const shared_reference_wrapper_impl<T,Weakable>& other)
: atomic_shared_pointer_wrapper<T>(other)
{
}
template<typename T,bool Weakable>
atomic_shared_reference_wrapper_impl<T,Weakable>::atomic_shared_reference_wrapper_impl(atomic_shared_reference_wrapper_impl&& other)
: atomic_shared_pointer_wrapper<T>(std::move(other))
{
}
template<typename T,bool Weakable>
atomic_shared_reference_wrapper_impl<T,Weakable>::atomic_shared_reference_wrapper_impl(shared_reference_wrapper_impl<T,Weakable>&& other)
: atomic_shared_pointer_wrapper<T>(std::move(other))
{
}
template<typename T,bool Weakable>
template<typename TT>
atomic_shared_reference_wrapper_impl<T,Weakable>::atomic_shared_reference_wrapper_impl(const atomic_shared_reference_wrapper_impl<TT,Weakable>& other)
: atomic_shared_pointer_wrapper<T>(other)
{
}
template<typename T,bool Weakable>
template<typename TT>
atomic_shared_reference_wrapper_impl<T,Weakable>::atomic_shared_reference_wrapper_impl(const shared_reference_wrapper_impl<TT,Weakable>& other)
: atomic_shared_pointer_wrapper<T>(other)
{
}
template<typename T,bool Weakable>
template<typename TT>
atomic_shared_reference_wrapper_impl<T,Weakable>::atomic_shared_reference_wrapper_impl(atomic_shared_reference_wrapper_impl<TT,Weakable>&& other)
: atomic_shared_pointer_wrapper<T>(std::move(other))
{
}
template<typename T,bool Weakable>
template<typename TT>
atomic_shared_reference_wrapper_impl<T,Weakable>::atomic_shared_reference_wrapper_impl(shared_reference_wrapper_impl<TT,Weakable>&& other)
: atomic_shared_pointer_wrapper<T>(std::move(other))
{
}
template<typename T,bool Weakable>
atomic_shared_reference_wrapper_impl<T,Weakable>& atomic_shared_reference_wrapper_impl<T,Weakable>::operator=(const atomic_shared_reference_wrapper_impl<T,Weakable>& other)
{
	atomic_shared_pointer_wrapper<T>::operator=(other);

	return *this;
}
template<typename T,bool Weakable>
atomic_shared_reference_wrapper_impl<T,Weakable>& atomic_shared_reference_wrapper_impl<T,Weakable>::operator=(const shared_reference_wrapper_impl<T,Weakable>& other)
{
	atomic_shared_pointer_wrapper<T>::operator=(other);

	return *this;
}
template<typename T,bool Weakable>
atomic_shared_reference_wrapper_impl<T,Weakable>& atomic_shared_reference_wrapper_impl<T,Weakable>::operator=(atomic_shared_reference_wrapper_impl<T,Weakable>&& other)
{
	atomic_shared_pointer_wrapper<T>::operator=(std::move(other));

	return *this;
}
template<typename T,bool Weakable>
atomic_shared_reference_wrapper_impl<T,Weakable>& atomic_shared_reference_wrapper_impl<T,Weakable>::operator=(shared_reference_wrapper_impl<T,Weakable>&& other)
{
	atomic_shared_pointer_wrapper<T>::operator=(std::move(other));

	return *this;
}
template<typename T,bool Weakable>
atomic_shared_reference_wrapper_impl<T,Weakable>::operator shared_reference_wrapper_impl<T,Weakable>&()
{
	return promote_to_ref(m_ptr);
}
template<typename T,bool Weakable>
atomic_shared_reference_wrapper_impl<T,Weakable>::operator const shared_reference_wrapper_impl<T,Weakable>&() const
{
	return promote_to_ref(m_ptr);
}

}
}