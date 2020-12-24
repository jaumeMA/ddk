
namespace ddk
{

template<typename T>
atomic_weak_pointer_wrapper<T>::atomic_weak_pointer_wrapper()
: m_barrier(false)
{
}
template<typename T>
atomic_weak_pointer_wrapper<T>::atomic_weak_pointer_wrapper(const atomic_weak_pointer_wrapper& other)
: m_barrier(false)
{
	other.raiseBarrier();

	m_ptr = other.m_ptr;

	other.dropBarrier();
}
template<typename T>
atomic_weak_pointer_wrapper<T>::atomic_weak_pointer_wrapper(const weak_pointer_wrapper<T>& other)
: m_barrier(false)
{
	m_ptr = other;
}
template<typename T>
atomic_weak_pointer_wrapper<T>::atomic_weak_pointer_wrapper(atomic_weak_pointer_wrapper&& other)
: m_barrier(false)
{
	other.raiseBarrier();

	m_ptr = std::move(other.m_ptr);

	other.dropBarrier();
}
template<typename T>
atomic_weak_pointer_wrapper<T>::atomic_weak_pointer_wrapper(weak_pointer_wrapper<T>&& other)
: m_barrier(false)
{
	m_ptr = std::move(other);
}
template<typename T>
template<typename TT>
atomic_weak_pointer_wrapper<T>::atomic_weak_pointer_wrapper(const atomic_weak_pointer_wrapper<TT>& other)
: m_barrier(false)
{
	other.raiseBarrier();

	m_ptr = other.m_ptr;

	other.dropBarrier();
}
template<typename T>
template<typename TT>
atomic_weak_pointer_wrapper<T>::atomic_weak_pointer_wrapper(const weak_pointer_wrapper<TT>& other)
: m_barrier(false)
{
	m_ptr = other.m_ptr;
}
template<typename T>
template<typename TT>
atomic_weak_pointer_wrapper<T>::atomic_weak_pointer_wrapper(atomic_weak_pointer_wrapper<TT>&& other)
: m_barrier(false)
{
	other.raiseBarrier();

	m_ptr = std::move(other.m_ptr);

	other.dropBarrier();
}
template<typename T>
template<typename TT>
atomic_weak_pointer_wrapper<T>::atomic_weak_pointer_wrapper(weak_pointer_wrapper<TT>&& other)
: m_barrier(false)
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
	raiseBarrier();

	m_ptr = nullptr;

	dropBarrier();

	return *this;
}
template<typename T>
atomic_weak_pointer_wrapper<T>& atomic_weak_pointer_wrapper<T>::operator=(const atomic_weak_pointer_wrapper& other)
{
	other.raiseBarrier();

	raiseBarrier();

	m_ptr = other.m_ptr;

	dropBarrier();

	other.dropBarrier();

	return *this;
}
template<typename T>
atomic_weak_pointer_wrapper<T>& atomic_weak_pointer_wrapper<T>::operator=(const weak_pointer_wrapper<T>& other)
{
	raiseBarrier();

	m_ptr = other;

	dropBarrier();

	return *this;
}
template<typename T>
atomic_weak_pointer_wrapper<T>& atomic_weak_pointer_wrapper<T>::operator=(atomic_weak_pointer_wrapper&& other)
{
	other.raiseBarrier();

	raiseBarrier();

	m_ptr = std::move(other.m_ptr);

	dropBarrier();

	other.dropBarrier();

	return *this;
}
template<typename T>
atomic_weak_pointer_wrapper<T>& atomic_weak_pointer_wrapper<T>::operator=(weak_pointer_wrapper<T>&& other)
{
	raiseBarrier();

	m_ptr = std::move(other);

	dropBarrier();

	return *this;
}
template<typename T>
template<typename TT>
atomic_weak_pointer_wrapper<T>& atomic_weak_pointer_wrapper<T>::operator=(const atomic_weak_pointer_wrapper<TT>& other)
{
	other.raiseBarrier();

	raiseBarrier();

	m_ptr = other.m_ptr;

	dropBarrier();

	other.dropBarrier();

	return *this;
}
template<typename T>
template<typename TT>
atomic_weak_pointer_wrapper<T>& atomic_weak_pointer_wrapper<T>::operator=(const weak_pointer_wrapper<TT>& other)
{
	raiseBarrier();

	m_ptr = other;

	dropBarrier();

	return *this;
}
template<typename T>
template<typename TT>
atomic_weak_pointer_wrapper<T>& atomic_weak_pointer_wrapper<T>::operator=(atomic_weak_pointer_wrapper<TT>&& other)
{
	other.raiseBarrier();

	raiseBarrier();

	m_ptr = std::move(other.m_ptr);

	dropBarrier();

	other.dropBarrier();

	return *this;
}
template<typename T>
template<typename TT>
atomic_weak_pointer_wrapper<T>& atomic_weak_pointer_wrapper<T>::operator=(weak_pointer_wrapper<TT>&& other)
{
	raiseBarrier();

	m_ptr = std::move(other);

	dropBarrier();

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
	raiseBarrier();

	atomic_shared_pointer_wrapper<T> res = m_ptr.share();

	dropBarrier();

	return res;
}
template<typename T>
void atomic_weak_pointer_wrapper<T>::raiseBarrier() const
{
	while(atomic_compare_exchange(m_barrier,false,true) == false) { std::this_thread::yield(); }
}
template<typename T>
void atomic_weak_pointer_wrapper<T>::dropBarrier() const
{
	while(atomic_compare_exchange(m_barrier,true,false) == false) { std::this_thread::yield(); }
}
template<typename T>
bool atomic_weak_pointer_wrapper<T>::isBarred() const
{
	return m_barrier.load();
}

}