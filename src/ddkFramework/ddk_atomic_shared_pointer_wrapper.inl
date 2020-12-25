
#include <thread>

namespace ddk
{

template<typename T>
atomic_shared_pointer_wrapper<T>::atomic_shared_pointer_wrapper(std::nullptr_t)
{
}
template<typename T>
atomic_shared_pointer_wrapper<T>::atomic_shared_pointer_wrapper(T* i_data,IReferenceWrapperDeleter* i_refDeleter)
: m_ptr(i_data,i_refDeleter)
{
}
template<typename T>
atomic_shared_pointer_wrapper<T>::atomic_shared_pointer_wrapper(const atomic_shared_pointer_wrapper& other)
{
	other.m_barrier.lock();

	m_ptr = other.m_ptr;

	other.m_barrier.unlock();
}
template<typename T>
atomic_shared_pointer_wrapper<T>::atomic_shared_pointer_wrapper(const shared_pointer_wrapper<T>& other)
{
	m_ptr = other;
}
template<typename T>
atomic_shared_pointer_wrapper<T>::atomic_shared_pointer_wrapper(atomic_shared_pointer_wrapper&& other)
{
	other.m_barrier.lock();

	m_ptr = std::move(other.m_ptr);

	other.m_barrier.unlock();
}
template<typename T>
atomic_shared_pointer_wrapper<T>::atomic_shared_pointer_wrapper(shared_pointer_wrapper<T>&& other)
{
	m_ptr = std::move(other);
}
template<typename T>
template<typename TT>
atomic_shared_pointer_wrapper<T>::atomic_shared_pointer_wrapper(const atomic_shared_pointer_wrapper<TT>& other)
{
	other.m_barrier.lock();

	m_ptr = other.m_ptr;

	other.m_barrier.unlock();
}
template<typename T>
template<typename TT>
atomic_shared_pointer_wrapper<T>::atomic_shared_pointer_wrapper(const shared_pointer_wrapper<TT>& other)
{
	m_ptr = other;
}
template<typename T>
template<typename TT>
atomic_shared_pointer_wrapper<T>::atomic_shared_pointer_wrapper(atomic_shared_pointer_wrapper<TT>&& other)
: m_barrier(false)
{
	other.m_barrier.lock();

	m_ptr = std::move(other.m_ptr);

	other.m_barrier.unlock();
}
template<typename T>
template<typename TT>
atomic_shared_pointer_wrapper<T>::atomic_shared_pointer_wrapper(shared_pointer_wrapper<TT>&& other)
{
	m_ptr = std::move(other);
}
template<typename T>
atomic_shared_pointer_wrapper<T>::~atomic_shared_pointer_wrapper()
{
}
template<typename T>
atomic_shared_pointer_wrapper<T>& atomic_shared_pointer_wrapper<T>::operator=(std::nullptr_t)
{
	m_barrier.lock();

	m_ptr = nullptr;

	m_barrier.unlock();

	return *this;
}
template<typename T>
atomic_shared_pointer_wrapper<T>& atomic_shared_pointer_wrapper<T>::operator=(const atomic_shared_pointer_wrapper& other)
{
	m_barrier.lock();

	other.m_barrier.lock();

	m_ptr = other.m_ptr;

	other.m_barrier.unlock();

	m_barrier.unlock();

	return *this;
}
template<typename T>
atomic_shared_pointer_wrapper<T>& atomic_shared_pointer_wrapper<T>::operator=(const shared_pointer_wrapper<T>& other)
{
	m_barrier.lock();

	m_ptr = other;

	m_barrier.unlock();
}
template<typename T>
atomic_shared_pointer_wrapper<T>& atomic_shared_pointer_wrapper<T>::operator=(atomic_shared_pointer_wrapper&& other)
{
	m_barrier.lock();

	other.m_barrier.lock();

	m_ptr = std::move(other.m_ptr);

	other.m_barrier.unlock();

	m_barrier.unlock();

	return *this;
}
template<typename T>
atomic_shared_pointer_wrapper<T>& atomic_shared_pointer_wrapper<T>::operator=(shared_pointer_wrapper<T>&& other)
{
	m_barrier.lock();

	m_ptr = std::move(other);

	m_barrier.unlock();

	return *this;
}
template<typename T>
template<typename TT>
atomic_shared_pointer_wrapper<T>& atomic_shared_pointer_wrapper<T>::operator=(const atomic_shared_pointer_wrapper<TT>& other)
{
	m_barrier.lock();

	other.m_barrier.lock();

	m_ptr = other.m_ptr;

	other.m_barrier.unlock();

	m_barrier.unlock();

	return *this;
}
template<typename T>
template<typename TT>
atomic_shared_pointer_wrapper<T>& atomic_shared_pointer_wrapper<T>::operator=(const shared_pointer_wrapper<TT>& other)
{
	m_barrier.lock();

	m_ptr = other;

	m_barrier.unlock();

	return *this;
}
template<typename T>
template<typename TT>
atomic_shared_pointer_wrapper<T>& atomic_shared_pointer_wrapper<T>::operator=(atomic_shared_pointer_wrapper<TT>&& other)
{
	m_barrier.lock();

	other.m_barrier.lock();

	m_ptr = std::move(other.m_ptr);

	other.m_barrier.unlock();

	m_barrier.unlock();

	return *this;
}
template<typename T>
template<typename TT>
atomic_shared_pointer_wrapper<T>& atomic_shared_pointer_wrapper<T>::operator=(shared_pointer_wrapper<TT>&& other)
{
	m_barrier.lock();

	m_ptr = std::move(other);

	m_barrier.unlock();

	return *this;
}
template<typename T>
atomic_shared_pointer_wrapper<T>::operator shared_pointer_wrapper<T>() const
{
	return m_ptr;
}
template<typename T>
bool atomic_shared_pointer_wrapper<T>::operator==(std::nullptr_t) const
{
	return m_ptr == nullptr;
}
template<typename T>
bool atomic_shared_pointer_wrapper<T>::operator!=(std::nullptr_t) const
{
	return m_ptr != nullptr;
}
template<typename T>
T* atomic_shared_pointer_wrapper<T>::operator->()
{
	return m_ptr.get();
}
template<typename T>
const T* atomic_shared_pointer_wrapper<T>::operator->() const
{
	return m_ptr.get();
}
template<typename T>
T& atomic_shared_pointer_wrapper<T>::operator*()
{
	return *m_ptr;
}
template<typename T>
const T& atomic_shared_pointer_wrapper<T>::operator*() const
{
	return *m_ptr;
}
template<typename T>
atomic_shared_pointer_wrapper<T>::operator bool() const
{
	return m_ptr != nullptr;
}
template<typename T>
void atomic_shared_pointer_wrapper<T>::clear()
{
	m_ptr.clear();
}
template<typename T>
T* atomic_shared_pointer_wrapper<T>::get()
{
	return m_ptr.get();
}
template<typename T>
const T* atomic_shared_pointer_wrapper<T>::get() const
{
	return m_ptr.get();
}
template<typename T>
const IReferenceWrapperDeleter* atomic_shared_pointer_wrapper<T>::get_deleter() const
{
	return m_ptr.get_deleter();
}
template<typename T>
bool atomic_shared_pointer_wrapper<T>::empty() const
{
	return m_ptr.empty();
}

}