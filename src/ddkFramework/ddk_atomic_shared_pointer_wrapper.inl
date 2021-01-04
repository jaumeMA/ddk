
#include <thread>

namespace ddk
{
namespace detail
{

template<typename T, bool Weakable>
atomic_shared_pointer_wrapper_impl<T,Weakable>::atomic_shared_pointer_wrapper_impl(std::nullptr_t)
{
}
template<typename T, bool Weakable>
atomic_shared_pointer_wrapper_impl<T,Weakable>::atomic_shared_pointer_wrapper_impl(T* i_data,IReferenceWrapperDeleter* i_refDeleter)
: m_ptr(i_data,i_refDeleter)
{
}
template<typename T, bool Weakable>
atomic_shared_pointer_wrapper_impl<T,Weakable>::atomic_shared_pointer_wrapper_impl(const atomic_shared_pointer_wrapper_impl& other)
{
	other.m_barrier.lock();

	m_ptr = other.m_ptr;

	other.m_barrier.unlock();
}
template<typename T, bool Weakable>
atomic_shared_pointer_wrapper_impl<T,Weakable>::atomic_shared_pointer_wrapper_impl(const shared_pointer_wrapper_impl<T,Weakable>& other)
{
	m_ptr = other;
}
template<typename T, bool Weakable>
atomic_shared_pointer_wrapper_impl<T,Weakable>::atomic_shared_pointer_wrapper_impl(atomic_shared_pointer_wrapper_impl&& other)
{
	other.m_barrier.lock();

	m_ptr = std::move(other.m_ptr);

	other.m_barrier.unlock();
}
template<typename T, bool Weakable>
atomic_shared_pointer_wrapper_impl<T,Weakable>::atomic_shared_pointer_wrapper_impl(shared_pointer_wrapper_impl<T,Weakable>&& other)
{
	m_ptr = std::move(other);
}
template<typename T, bool Weakable>
TEMPLATE(typename TT,bool WWeakable)
REQUIRED_COND(Weakable || Weakable == WWeakable)
atomic_shared_pointer_wrapper_impl<T,Weakable>::atomic_shared_pointer_wrapper_impl(const atomic_shared_pointer_wrapper_impl<TT,WWeakable>& other)
{
	other.m_barrier.lock();

	m_ptr = other.m_ptr;

	other.m_barrier.unlock();
}
template<typename T, bool Weakable>
TEMPLATE(typename TT,bool WWeakable)
REQUIRED_COND(Weakable || Weakable == WWeakable)
atomic_shared_pointer_wrapper_impl<T,Weakable>::atomic_shared_pointer_wrapper_impl(const shared_pointer_wrapper_impl<TT,WWeakable>& other)
{
	m_ptr = other;
}
template<typename T, bool Weakable>
TEMPLATE(typename TT,bool WWeakable)
REQUIRED_COND(Weakable || Weakable == WWeakable)
atomic_shared_pointer_wrapper_impl<T,Weakable>::atomic_shared_pointer_wrapper_impl(atomic_shared_pointer_wrapper_impl<TT,WWeakable>&& other)
: m_barrier(false)
{
	other.m_barrier.lock();

	m_ptr = std::move(other.m_ptr);

	other.m_barrier.unlock();
}
template<typename T, bool Weakable>
TEMPLATE(typename TT,bool WWeakable)
REQUIRED_COND(Weakable || Weakable == WWeakable)
atomic_shared_pointer_wrapper_impl<T,Weakable>::atomic_shared_pointer_wrapper_impl(shared_pointer_wrapper_impl<TT,WWeakable>&& other)
{
	m_ptr = std::move(other);
}
template<typename T, bool Weakable>
atomic_shared_pointer_wrapper_impl<T,Weakable>::~atomic_shared_pointer_wrapper_impl()
{
}
template<typename T, bool Weakable>
atomic_shared_pointer_wrapper_impl<T,Weakable>& atomic_shared_pointer_wrapper_impl<T,Weakable>::operator=(std::nullptr_t)
{
	m_barrier.lock();

	m_ptr = nullptr;

	m_barrier.unlock();

	return *this;
}
template<typename T, bool Weakable>
atomic_shared_pointer_wrapper_impl<T,Weakable>& atomic_shared_pointer_wrapper_impl<T,Weakable>::operator=(const atomic_shared_pointer_wrapper_impl& other)
{
	m_barrier.lock();

	other.m_barrier.lock();

	m_ptr = other.m_ptr;

	other.m_barrier.unlock();

	m_barrier.unlock();

	return *this;
}
template<typename T, bool Weakable>
atomic_shared_pointer_wrapper_impl<T,Weakable>& atomic_shared_pointer_wrapper_impl<T,Weakable>::operator=(const shared_pointer_wrapper_impl<T,Weakable>& other)
{
	m_barrier.lock();

	m_ptr = other;

	m_barrier.unlock();
}
template<typename T, bool Weakable>
atomic_shared_pointer_wrapper_impl<T,Weakable>& atomic_shared_pointer_wrapper_impl<T,Weakable>::operator=(atomic_shared_pointer_wrapper_impl&& other)
{
	m_barrier.lock();

	other.m_barrier.lock();

	m_ptr = std::move(other.m_ptr);

	other.m_barrier.unlock();

	m_barrier.unlock();

	return *this;
}
template<typename T, bool Weakable>
atomic_shared_pointer_wrapper_impl<T,Weakable>& atomic_shared_pointer_wrapper_impl<T,Weakable>::operator=(shared_pointer_wrapper_impl<T,Weakable>&& other)
{
	m_barrier.lock();

	m_ptr = std::move(other);

	m_barrier.unlock();

	return *this;
}
template<typename T, bool Weakable>
TEMPLATE(typename TT,bool WWeakable)
REQUIRED_COND(Weakable || Weakable == WWeakable)
atomic_shared_pointer_wrapper_impl<T,Weakable>& atomic_shared_pointer_wrapper_impl<T,Weakable>::operator=(const atomic_shared_pointer_wrapper_impl<TT,WWeakable>& other)
{
	m_barrier.lock();

	other.m_barrier.lock();

	m_ptr = other.m_ptr;

	other.m_barrier.unlock();

	m_barrier.unlock();

	return *this;
}
template<typename T, bool Weakable>
TEMPLATE(typename TT,bool WWeakable)
REQUIRED_COND(Weakable || Weakable == WWeakable)
atomic_shared_pointer_wrapper_impl<T,Weakable>& atomic_shared_pointer_wrapper_impl<T,Weakable>::operator=(const shared_pointer_wrapper_impl<TT,WWeakable>& other)
{
	m_barrier.lock();

	m_ptr = other;

	m_barrier.unlock();

	return *this;
}
template<typename T, bool Weakable>
TEMPLATE(typename TT,bool WWeakable)
REQUIRED_COND(Weakable || Weakable == WWeakable)
atomic_shared_pointer_wrapper_impl<T,Weakable>& atomic_shared_pointer_wrapper_impl<T,Weakable>::operator=(atomic_shared_pointer_wrapper_impl<TT,WWeakable>&& other)
{
	m_barrier.lock();

	other.m_barrier.lock();

	m_ptr = std::move(other.m_ptr);

	other.m_barrier.unlock();

	m_barrier.unlock();

	return *this;
}
template<typename T, bool Weakable>
TEMPLATE(typename TT,bool WWeakable)
REQUIRED_COND(Weakable || Weakable == WWeakable)
atomic_shared_pointer_wrapper_impl<T,Weakable>& atomic_shared_pointer_wrapper_impl<T,Weakable>::operator=(shared_pointer_wrapper_impl<TT,WWeakable>&& other)
{
	m_barrier.lock();

	m_ptr = std::move(other);

	m_barrier.unlock();

	return *this;
}
template<typename T,bool Weakable>
atomic_shared_pointer_wrapper_impl<T,Weakable>::operator shared_pointer_wrapper_impl<T,Weakable>&()
{
	return m_ptr;
}
template<typename T,bool Weakable>
atomic_shared_pointer_wrapper_impl<T,Weakable>::operator const shared_pointer_wrapper_impl<T,Weakable>&() const
{
	return m_ptr;
}
template<typename T, bool Weakable>
bool atomic_shared_pointer_wrapper_impl<T,Weakable>::operator==(std::nullptr_t) const
{
	return m_ptr == nullptr;
}
template<typename T, bool Weakable>
bool atomic_shared_pointer_wrapper_impl<T,Weakable>::operator!=(std::nullptr_t) const
{
	return m_ptr != nullptr;
}
template<typename T, bool Weakable>
T* atomic_shared_pointer_wrapper_impl<T,Weakable>::operator->()
{
	return m_ptr.get();
}
template<typename T, bool Weakable>
const T* atomic_shared_pointer_wrapper_impl<T,Weakable>::operator->() const
{
	return m_ptr.get();
}
template<typename T, bool Weakable>
T& atomic_shared_pointer_wrapper_impl<T,Weakable>::operator*()
{
	return *m_ptr;
}
template<typename T, bool Weakable>
const T& atomic_shared_pointer_wrapper_impl<T,Weakable>::operator*() const
{
	return *m_ptr;
}
template<typename T, bool Weakable>
atomic_shared_pointer_wrapper_impl<T,Weakable>::operator bool() const
{
	return m_ptr != nullptr;
}
template<typename T, bool Weakable>
void atomic_shared_pointer_wrapper_impl<T,Weakable>::clear()
{
	m_ptr.clear();
}
template<typename T, bool Weakable>
T* atomic_shared_pointer_wrapper_impl<T,Weakable>::get()
{
	return m_ptr.get();
}
template<typename T, bool Weakable>
const T* atomic_shared_pointer_wrapper_impl<T,Weakable>::get() const
{
	return m_ptr.get();
}
template<typename T, bool Weakable>
const IReferenceWrapperDeleter* atomic_shared_pointer_wrapper_impl<T,Weakable>::get_deleter() const
{
	return m_ptr.get_deleter();
}
template<typename T, bool Weakable>
bool atomic_shared_pointer_wrapper_impl<T,Weakable>::empty() const
{
	return m_ptr.empty();
}

}
}