
#include <thread>

namespace ddk
{
namespace detail
{

template<typename T, typename ReferenceCounter>
atomic_shared_pointer_wrapper_impl<T,ReferenceCounter>::atomic_shared_pointer_wrapper_impl(std::nullptr_t)
{
}
template<typename T, typename ReferenceCounter>
atomic_shared_pointer_wrapper_impl<T,ReferenceCounter>::atomic_shared_pointer_wrapper_impl(T* i_data,IReferenceWrapperDeleter* i_refDeleter)
: m_ptr(i_data,i_refDeleter)
{
}
template<typename T, typename ReferenceCounter>
atomic_shared_pointer_wrapper_impl<T,ReferenceCounter>::atomic_shared_pointer_wrapper_impl(const atomic_shared_pointer_wrapper_impl& other)
{
	other.m_barrier.lock();

	m_ptr = other.m_ptr;

	other.m_barrier.unlock();
}
template<typename T, typename ReferenceCounter>
atomic_shared_pointer_wrapper_impl<T,ReferenceCounter>::atomic_shared_pointer_wrapper_impl(const shared_pointer_wrapper_impl<T,ReferenceCounter>& other)
{
	m_ptr = other;
}
template<typename T, typename ReferenceCounter>
atomic_shared_pointer_wrapper_impl<T,ReferenceCounter>::atomic_shared_pointer_wrapper_impl(atomic_shared_pointer_wrapper_impl&& other)
{
	other.m_barrier.lock();

	m_ptr = std::move(other.m_ptr);

	other.m_barrier.unlock();
}
template<typename T, typename ReferenceCounter>
atomic_shared_pointer_wrapper_impl<T,ReferenceCounter>::atomic_shared_pointer_wrapper_impl(shared_pointer_wrapper_impl<T,ReferenceCounter>&& other)
{
	m_ptr = std::move(other);
}
template<typename T, typename ReferenceCounter>
template<typename TT>
atomic_shared_pointer_wrapper_impl<T,ReferenceCounter>::atomic_shared_pointer_wrapper_impl(const atomic_shared_pointer_wrapper_impl<TT,ReferenceCounter>& other)
{
	other.m_barrier.lock();

	m_ptr = other.m_ptr;

	other.m_barrier.unlock();
}
template<typename T, typename ReferenceCounter>
template<typename TT>
atomic_shared_pointer_wrapper_impl<T,ReferenceCounter>::atomic_shared_pointer_wrapper_impl(const shared_pointer_wrapper_impl<TT,ReferenceCounter>& other)
{
	m_ptr = other;
}
template<typename T, typename ReferenceCounter>
template<typename TT>
atomic_shared_pointer_wrapper_impl<T,ReferenceCounter>::atomic_shared_pointer_wrapper_impl(atomic_shared_pointer_wrapper_impl<TT,ReferenceCounter>&& other)
: m_barrier(false)
{
	other.m_barrier.lock();

	m_ptr = std::move(other.m_ptr);

	other.m_barrier.unlock();
}
template<typename T, typename ReferenceCounter>
template<typename TT>
atomic_shared_pointer_wrapper_impl<T,ReferenceCounter>::atomic_shared_pointer_wrapper_impl(shared_pointer_wrapper_impl<TT,ReferenceCounter>&& other)
{
	m_ptr = std::move(other);
}
template<typename T, typename ReferenceCounter>
atomic_shared_pointer_wrapper_impl<T,ReferenceCounter>::~atomic_shared_pointer_wrapper_impl()
{
}
template<typename T, typename ReferenceCounter>
atomic_shared_pointer_wrapper_impl<T,ReferenceCounter>& atomic_shared_pointer_wrapper_impl<T,ReferenceCounter>::operator=(std::nullptr_t)
{
	m_barrier.lock();

	m_ptr = nullptr;

	m_barrier.unlock();

	return *this;
}
template<typename T, typename ReferenceCounter>
atomic_shared_pointer_wrapper_impl<T,ReferenceCounter>& atomic_shared_pointer_wrapper_impl<T,ReferenceCounter>::operator=(const atomic_shared_pointer_wrapper_impl& other)
{
	m_barrier.lock();

	other.m_barrier.lock();

	m_ptr = other.m_ptr;

	other.m_barrier.unlock();

	m_barrier.unlock();

	return *this;
}
template<typename T, typename ReferenceCounter>
atomic_shared_pointer_wrapper_impl<T,ReferenceCounter>& atomic_shared_pointer_wrapper_impl<T,ReferenceCounter>::operator=(const shared_pointer_wrapper_impl<T,ReferenceCounter>& other)
{
	m_barrier.lock();

	m_ptr = other;

	m_barrier.unlock();
}
template<typename T, typename ReferenceCounter>
atomic_shared_pointer_wrapper_impl<T,ReferenceCounter>& atomic_shared_pointer_wrapper_impl<T,ReferenceCounter>::operator=(atomic_shared_pointer_wrapper_impl&& other)
{
	m_barrier.lock();

	other.m_barrier.lock();

	m_ptr = std::move(other.m_ptr);

	other.m_barrier.unlock();

	m_barrier.unlock();

	return *this;
}
template<typename T, typename ReferenceCounter>
atomic_shared_pointer_wrapper_impl<T,ReferenceCounter>& atomic_shared_pointer_wrapper_impl<T,ReferenceCounter>::operator=(shared_pointer_wrapper_impl<T,ReferenceCounter>&& other)
{
	m_barrier.lock();

	m_ptr = std::move(other);

	m_barrier.unlock();

	return *this;
}
template<typename T, typename ReferenceCounter>
template<typename TT>
atomic_shared_pointer_wrapper_impl<T,ReferenceCounter>& atomic_shared_pointer_wrapper_impl<T,ReferenceCounter>::operator=(const atomic_shared_pointer_wrapper_impl<TT,ReferenceCounter>& other)
{
	m_barrier.lock();

	other.m_barrier.lock();

	m_ptr = other.m_ptr;

	other.m_barrier.unlock();

	m_barrier.unlock();

	return *this;
}
template<typename T, typename ReferenceCounter>
template<typename TT>
atomic_shared_pointer_wrapper_impl<T,ReferenceCounter>& atomic_shared_pointer_wrapper_impl<T,ReferenceCounter>::operator=(const shared_pointer_wrapper_impl<TT,ReferenceCounter>& other)
{
	m_barrier.lock();

	m_ptr = other;

	m_barrier.unlock();

	return *this;
}
template<typename T, typename ReferenceCounter>
template<typename TT>
atomic_shared_pointer_wrapper_impl<T,ReferenceCounter>& atomic_shared_pointer_wrapper_impl<T,ReferenceCounter>::operator=(atomic_shared_pointer_wrapper_impl<TT,ReferenceCounter>&& other)
{
	m_barrier.lock();

	other.m_barrier.lock();

	m_ptr = std::move(other.m_ptr);

	other.m_barrier.unlock();

	m_barrier.unlock();

	return *this;
}
template<typename T, typename ReferenceCounter>
template<typename TT>
atomic_shared_pointer_wrapper_impl<T,ReferenceCounter>& atomic_shared_pointer_wrapper_impl<T,ReferenceCounter>::operator=(shared_pointer_wrapper_impl<TT,ReferenceCounter>&& other)
{
	m_barrier.lock();

	m_ptr = std::move(other);

	m_barrier.unlock();

	return *this;
}
template<typename T,typename ReferenceCounter>
atomic_shared_pointer_wrapper_impl<T,ReferenceCounter>::operator shared_pointer_wrapper_impl<T,ReferenceCounter>&()
{
	return m_ptr;
}
template<typename T,typename ReferenceCounter>
atomic_shared_pointer_wrapper_impl<T,ReferenceCounter>::operator const shared_pointer_wrapper_impl<T,ReferenceCounter>&() const
{
	return m_ptr;
}
template<typename T, typename ReferenceCounter>
bool atomic_shared_pointer_wrapper_impl<T,ReferenceCounter>::operator==(std::nullptr_t) const
{
	return m_ptr == nullptr;
}
template<typename T, typename ReferenceCounter>
bool atomic_shared_pointer_wrapper_impl<T,ReferenceCounter>::operator!=(std::nullptr_t) const
{
	return m_ptr != nullptr;
}
template<typename T, typename ReferenceCounter>
T* atomic_shared_pointer_wrapper_impl<T,ReferenceCounter>::operator->()
{
	return m_ptr.get();
}
template<typename T, typename ReferenceCounter>
const T* atomic_shared_pointer_wrapper_impl<T,ReferenceCounter>::operator->() const
{
	return m_ptr.get();
}
template<typename T, typename ReferenceCounter>
T& atomic_shared_pointer_wrapper_impl<T,ReferenceCounter>::operator*()
{
	return *m_ptr;
}
template<typename T, typename ReferenceCounter>
const T& atomic_shared_pointer_wrapper_impl<T,ReferenceCounter>::operator*() const
{
	return *m_ptr;
}
template<typename T, typename ReferenceCounter>
atomic_shared_pointer_wrapper_impl<T,ReferenceCounter>::operator bool() const
{
	return m_ptr != nullptr;
}
template<typename T, typename ReferenceCounter>
void atomic_shared_pointer_wrapper_impl<T,ReferenceCounter>::clear()
{
	m_ptr.clear();
}
template<typename T, typename ReferenceCounter>
T* atomic_shared_pointer_wrapper_impl<T,ReferenceCounter>::get()
{
	return m_ptr.get();
}
template<typename T, typename ReferenceCounter>
const T* atomic_shared_pointer_wrapper_impl<T,ReferenceCounter>::get() const
{
	return m_ptr.get();
}
template<typename T, typename ReferenceCounter>
const IReferenceWrapperDeleter* atomic_shared_pointer_wrapper_impl<T,ReferenceCounter>::get_deleter() const
{
	return m_ptr.get_deleter();
}
template<typename T, typename ReferenceCounter>
bool atomic_shared_pointer_wrapper_impl<T,ReferenceCounter>::empty() const
{
	return m_ptr.empty();
}

}
}