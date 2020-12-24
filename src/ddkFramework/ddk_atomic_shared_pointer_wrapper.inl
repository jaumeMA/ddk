
#include <thread>

namespace ddk
{

template<typename T>
atomic_shared_pointer_wrapper<T>::atomic_shared_pointer_wrapper()
: m_barrier(false)
{
}
template<typename T>
atomic_shared_pointer_wrapper<T>::atomic_shared_pointer_wrapper(std::nullptr_t)
: m_barrier(false)
{
}
template<typename T>
atomic_shared_pointer_wrapper<T>::atomic_shared_pointer_wrapper(T* i_data,IReferenceWrapperDeleter* i_refDeleter)
: m_barrier(false)
, m_ptr(i_data,i_refDeleter)
{
}
template<typename T>
atomic_shared_pointer_wrapper<T>::atomic_shared_pointer_wrapper(const atomic_shared_pointer_wrapper& other)
: m_barrier(false)
{
	other.raiseBarrier();

	m_ptr = other.m_ptr;

	other.dropBarrier();
}
template<typename T>
atomic_shared_pointer_wrapper<T>::atomic_shared_pointer_wrapper(const shared_pointer_wrapper<T>& other)
: m_barrier(false)
{
	m_ptr = other;
}
template<typename T>
atomic_shared_pointer_wrapper<T>::atomic_shared_pointer_wrapper(atomic_shared_pointer_wrapper&& other)
: m_barrier(false)
{
	other.raiseBarrier();

	m_ptr = std::move(other.m_ptr);

	other.dropBarrier();
}
template<typename T>
atomic_shared_pointer_wrapper<T>::atomic_shared_pointer_wrapper(shared_pointer_wrapper<T>&& other)
: m_barrier(false)
{
	m_ptr = std::move(other);
}
template<typename T>
template<typename TT>
atomic_shared_pointer_wrapper<T>::atomic_shared_pointer_wrapper(const atomic_shared_pointer_wrapper<TT>& other)
: m_barrier(false)
{
	other.raiseBarrier();

	m_ptr = other.m_ptr;

	other.dropBarrier();
}
template<typename T>
template<typename TT>
atomic_shared_pointer_wrapper<T>::atomic_shared_pointer_wrapper(const shared_pointer_wrapper<TT>& other)
: m_barrier(false)
{
	m_ptr = other;
}
template<typename T>
template<typename TT>
atomic_shared_pointer_wrapper<T>::atomic_shared_pointer_wrapper(atomic_shared_pointer_wrapper<TT>&& other)
: m_barrier(false)
{
	other.raiseBarrier();

	m_ptr = std::move(other.m_ptr);

	other.dropBarrier();
}
template<typename T>
template<typename TT>
atomic_shared_pointer_wrapper<T>::atomic_shared_pointer_wrapper(shared_pointer_wrapper<TT>&& other)
: m_barrier(false)
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
	raiseBarrier();

	m_ptr = nullptr;

	dropBarrier();

	return *this;
}
template<typename T>
atomic_shared_pointer_wrapper<T>& atomic_shared_pointer_wrapper<T>::operator=(const atomic_shared_pointer_wrapper& other)
{
	raiseBarrier();

	other.raiseBarrier();

	m_ptr = other.m_ptr;

	other.dropBarrier();

	dropBarrier();

	return *this;
}
template<typename T>
atomic_shared_pointer_wrapper<T>& atomic_shared_pointer_wrapper<T>::operator=(const shared_pointer_wrapper<T>& other)
{
	raiseBarrier();

	m_ptr = other;

	dropBarrier();
}
template<typename T>
atomic_shared_pointer_wrapper<T>& atomic_shared_pointer_wrapper<T>::operator=(atomic_shared_pointer_wrapper&& other)
{
	raiseBarrier();

	other.raiseBarrier();

	m_ptr = std::move(other.m_ptr);

	other.dropBarrier();

	dropBarrier();

	return *this;
}
template<typename T>
atomic_shared_pointer_wrapper<T>& atomic_shared_pointer_wrapper<T>::operator=(shared_pointer_wrapper<T>&& other)
{
	raiseBarrier();

	m_ptr = std::move(other);

	dropBarrier();

	return *this;
}
template<typename T>
template<typename TT>
atomic_shared_pointer_wrapper<T>& atomic_shared_pointer_wrapper<T>::operator=(const atomic_shared_pointer_wrapper<TT>& other)
{
	raiseBarrier();

	other.raiseBarrier();

	m_ptr = other.m_ptr;

	other.dropBarrier();

	dropBarrier();

	return *this;
}
template<typename T>
template<typename TT>
atomic_shared_pointer_wrapper<T>& atomic_shared_pointer_wrapper<T>::operator=(const shared_pointer_wrapper<TT>& other)
{
	raiseBarrier();

	m_ptr = other;

	dropBarrier();

	return *this;
}
template<typename T>
template<typename TT>
atomic_shared_pointer_wrapper<T>& atomic_shared_pointer_wrapper<T>::operator=(atomic_shared_pointer_wrapper<TT>&& other)
{
	raiseBarrier();

	other.raiseBarrier();

	m_ptr = std::move(other.m_ptr);

	other.dropBarrier();

	dropBarrier();

	return *this;
}
template<typename T>
template<typename TT>
atomic_shared_pointer_wrapper<T>& atomic_shared_pointer_wrapper<T>::operator=(shared_pointer_wrapper<TT>&& other)
{
	raiseBarrier();

	m_ptr = std::move(other);

	dropBarrier();

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
template<typename T>
void atomic_shared_pointer_wrapper<T>::raiseBarrier() const
{
	while(atomic_compare_exchange(m_barrier,false,true) == false) { std::this_thread::yield(); }
}
template<typename T>
void atomic_shared_pointer_wrapper<T>::dropBarrier() const
{
	while(atomic_compare_exchange(m_barrier,true,false) == false) { std::this_thread::yield(); }
}
template<typename T>
bool atomic_shared_pointer_wrapper<T>::isBarred() const
{
	return m_barrier.load();
}

}