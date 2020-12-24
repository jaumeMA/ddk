
namespace ddk
{

template<typename T>
atomic_shared_reference_wrapper<T>::atomic_shared_reference_wrapper(const atomic_shared_reference_wrapper<T>& other)
: atomic_shared_pointer_wrapper<T>(other)
{
}
template<typename T>
atomic_shared_reference_wrapper<T>::atomic_shared_reference_wrapper(const shared_reference_wrapper<T>& other)
: atomic_shared_pointer_wrapper<T>(other)
{
}
template<typename T>
atomic_shared_reference_wrapper<T>::atomic_shared_reference_wrapper(atomic_shared_reference_wrapper<T>&& other)
: atomic_shared_pointer_wrapper<T>(std::move(other))
{
}
template<typename T>
atomic_shared_reference_wrapper<T>::atomic_shared_reference_wrapper(shared_reference_wrapper<T>&& other)
: atomic_shared_pointer_wrapper<T>(std::move(other))
{
}
template<typename T>
template<typename TT>
atomic_shared_reference_wrapper<T>::atomic_shared_reference_wrapper(const atomic_shared_reference_wrapper<TT>& other)
: atomic_shared_pointer_wrapper<T>(other)
{
}
template<typename T>
template<typename TT>
atomic_shared_reference_wrapper<T>::atomic_shared_reference_wrapper(const shared_reference_wrapper<TT>& other)
: atomic_shared_pointer_wrapper<T>(other)
{
}
template<typename T>
template<typename TT>
atomic_shared_reference_wrapper<T>::atomic_shared_reference_wrapper(atomic_shared_reference_wrapper<TT>&& other)
: atomic_shared_pointer_wrapper<T>(std::move(other))
{
}
template<typename T>
template<typename TT>
atomic_shared_reference_wrapper<T>::atomic_shared_reference_wrapper(shared_reference_wrapper<TT>&& other)
: atomic_shared_pointer_wrapper<T>(std::move(other))
{
}
template<typename T>
atomic_shared_reference_wrapper<T>& atomic_shared_reference_wrapper<T>::operator=(const atomic_shared_reference_wrapper<T>& other)
{
	atomic_shared_pointer_wrapper<T>::operator=(other);

	return *this;
}
template<typename T>
atomic_shared_reference_wrapper<T>& atomic_shared_reference_wrapper<T>::operator=(const shared_reference_wrapper<T>& other)
{
	atomic_shared_pointer_wrapper<T>::operator=(other);

	return *this;
}
template<typename T>
atomic_shared_reference_wrapper<T>& atomic_shared_reference_wrapper<T>::operator=(atomic_shared_reference_wrapper<T>&& other)
{
	atomic_shared_pointer_wrapper<T>::operator=(std::move(other));

	return *this;
}
template<typename T>
atomic_shared_reference_wrapper<T>& atomic_shared_reference_wrapper<T>::operator=(shared_reference_wrapper<T>&& other)
{
	atomic_shared_pointer_wrapper<T>::operator=(std::move(other));

	return *this;
}
template<typename T>
atomic_shared_reference_wrapper<T>::operator shared_reference_wrapper<T>() const
{
	return promote_to_ref(m_ptr);
}

}