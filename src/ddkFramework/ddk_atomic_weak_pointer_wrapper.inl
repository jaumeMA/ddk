
namespace ddk
{

template<typename T>
atomic_weak_pointer_wrapper<T>::atomic_weak_pointer_wrapper(const atomic_weak_pointer_wrapper& other)
{
	other.m_barrier.lock();

	m_ptr = other.m_ptr;

	other.m_barrier.unlock();
}
template<typename T>
atomic_weak_pointer_wrapper<T>::atomic_weak_pointer_wrapper(const weak_pointer_wrapper<T>& other)
{
	m_ptr = other;
}
template<typename T>
atomic_weak_pointer_wrapper<T>::atomic_weak_pointer_wrapper(atomic_weak_pointer_wrapper&& other)
{
	other.m_barrier.lock();

	m_ptr = std::move(other.m_ptr);

	other.m_barrier.unlock();
}
template<typename T>
atomic_weak_pointer_wrapper<T>::atomic_weak_pointer_wrapper(weak_pointer_wrapper<T>&& other)
{
	m_ptr = std::move(other);
}
template<typename T>
template<typename TT>
atomic_weak_pointer_wrapper<T>::atomic_weak_pointer_wrapper(const atomic_weak_pointer_wrapper<TT>& other)
{
	other.m_barrier.lock();

	m_ptr = other.m_ptr;

	other.m_barrier.unlock();
}
template<typename T>
template<typename TT>
atomic_weak_pointer_wrapper<T>::atomic_weak_pointer_wrapper(const weak_pointer_wrapper<TT>& other)
{
	m_ptr = other.m_ptr;
}
template<typename T>
template<typename TT>
atomic_weak_pointer_wrapper<T>::atomic_weak_pointer_wrapper(atomic_weak_pointer_wrapper<TT>&& other)
{
	other.m_barrier.lock();

	m_ptr = std::move(other.m_ptr);

	other.m_barrier.unlock();
}
template<typename T>
template<typename TT>
atomic_weak_pointer_wrapper<T>::atomic_weak_pointer_wrapper(weak_pointer_wrapper<TT>&& other)
{
	m_ptr = std::move(other);
}
template<typename T>
atomic_weak_pointer_wrapper<T>::~atomic_weak_pointer_wrapper()
{
}
template<typename T>
atomic_weak_pointer_wrapper<T>& atomic_weak_pointer_wrapper<T>::operator=(std::nullptr_t)
{
	m_barrier.lock();

	m_ptr = nullptr;

	m_barrier.unlock();

	return *this;
}
template<typename T>
atomic_weak_pointer_wrapper<T>& atomic_weak_pointer_wrapper<T>::operator=(const atomic_weak_pointer_wrapper& other)
{
	other.m_barrier.lock();

	m_barrier.lock();

	m_ptr = other.m_ptr;

	m_barrier.unlock();

	other.m_barrier.unlock();

	return *this;
}
template<typename T>
atomic_weak_pointer_wrapper<T>& atomic_weak_pointer_wrapper<T>::operator=(const weak_pointer_wrapper<T>& other)
{
	m_barrier.lock();

	m_ptr = other;

	m_barrier.unlock();

	return *this;
}
template<typename T>
atomic_weak_pointer_wrapper<T>& atomic_weak_pointer_wrapper<T>::operator=(atomic_weak_pointer_wrapper&& other)
{
	other.m_barrier.lock();

	m_barrier.lock();

	m_ptr = std::move(other.m_ptr);

	m_barrier.unlock();

	other.m_barrier.unlock();

	return *this;
}
template<typename T>
atomic_weak_pointer_wrapper<T>& atomic_weak_pointer_wrapper<T>::operator=(weak_pointer_wrapper<T>&& other)
{
	m_barrier.lock();

	m_ptr = std::move(other);

	m_barrier.unlock();

	return *this;
}
template<typename T>
template<typename TT>
atomic_weak_pointer_wrapper<T>& atomic_weak_pointer_wrapper<T>::operator=(const atomic_weak_pointer_wrapper<TT>& other)
{
	other.m_barrier.lock();

	m_barrier.lock();

	m_ptr = other.m_ptr;

	m_barrier.unlock();

	other.m_barrier.unlock();

	return *this;
}
template<typename T>
template<typename TT>
atomic_weak_pointer_wrapper<T>& atomic_weak_pointer_wrapper<T>::operator=(const weak_pointer_wrapper<TT>& other)
{
	m_barrier.lock();

	m_ptr = other;

	m_barrier.unlock();

	return *this;
}
template<typename T>
template<typename TT>
atomic_weak_pointer_wrapper<T>& atomic_weak_pointer_wrapper<T>::operator=(atomic_weak_pointer_wrapper<TT>&& other)
{
	other.m_barrier.lock();

	m_barrier.lock();

	m_ptr = std::move(other.m_ptr);

	m_barrier.unlock();

	other.m_barrier.unlock();

	return *this;
}
template<typename T>
template<typename TT>
atomic_weak_pointer_wrapper<T>& atomic_weak_pointer_wrapper<T>::operator=(weak_pointer_wrapper<TT>&& other)
{
	m_barrier.lock();

	m_ptr = std::move(other);

	m_barrier.unlock();

	return *this;
}
template<typename T>
bool atomic_weak_pointer_wrapper<T>::operator==(const std::nullptr_t&) const
{
	return m_ptr == nullptr;
}
template<typename T>
bool atomic_weak_pointer_wrapper<T>::operator!=(const std::nullptr_t&) const
{
	return m_ptr != nullptr;
}
template<typename T>
atomic_weak_pointer_wrapper<T>::operator weak_pointer_wrapper<T>() const
{
	return m_ptr;
}
template<typename T>
atomic_shared_pointer_wrapper<T> atomic_weak_pointer_wrapper<T>::share() const
{
	m_barrier.lock();

	atomic_shared_pointer_wrapper<T> res = m_ptr.share();

	m_barrier.unlock();

	return res;
}

}