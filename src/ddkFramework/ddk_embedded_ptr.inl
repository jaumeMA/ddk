
namespace ddk
{

template<typename T>
embedded_ptr<T>::embedded_ptr(const embedded_ptr& other)
{
	if (other)
	{
		m_rel = other.m_rel + reinterpret_cast<const char*>(&other) - reinterpret_cast<char*>(this);
	}
}
template<typename T>
embedded_ptr<T>::embedded_ptr(embedded_ptr&& other)
{
	if (other)
	{
		m_rel = other.m_rel + reinterpret_cast<const char*>(&other) - reinterpret_cast<char*>(this);
		other.m_rel = 0;
	}
}
template<typename T>
embedded_ptr<T>::embedded_ptr(const std::nullptr_t&)
: m_rel(0)
{
}
template<typename T>
embedded_ptr<T>::embedded_ptr(T* i_ptr)
{
	if (i_ptr)
	{
		m_rel = reinterpret_cast<char*>(i_ptr) - reinterpret_cast<char*>(this);
	}
}
template<typename T>
TEMPLATE(typename TT)
REQUIRED(IS_BASE_OF(T,TT))
embedded_ptr<T>::embedded_ptr(const embedded_ptr<TT>& other)
{
	if (other)
	{
		m_rel = other.m_rel + reinterpret_cast<const char*>(&other) - reinterpret_cast<char*>(this);
		other.m_rel = 0;
	}
}
template<typename T>
TEMPLATE(typename TT)
REQUIRED(IS_BASE_OF(T,TT))
embedded_ptr<T>::embedded_ptr(embedded_ptr<TT>&& other)
{
	if (other)
	{
		m_rel = other.m_rel + reinterpret_cast<const char*>(&other) - reinterpret_cast<char*>(this);
	}
}
template<typename T>
TEMPLATE(typename TT)
REQUIRED(IS_BASE_OF(T,TT))
embedded_ptr<T>::embedded_ptr(TT* i_ptr)
{
	if (i_ptr)
	{
		m_rel = reinterpret_cast<char*>(i_ptr) - reinterpret_cast<char*>(this);
	}
}
template<typename T>
embedded_ptr<T>& embedded_ptr<T>::operator=(const embedded_ptr& other)
{
	if (other)
	{
		m_rel = other.m_rel + reinterpret_cast<const char*>(&other) - reinterpret_cast<char*>(this);
	}
	else
	{
		m_rel = 0;
	}

	return *this;
}
template<typename T>
TEMPLATE(typename TT)
REQUIRED(IS_BASE_OF(T,TT))
embedded_ptr<T>& embedded_ptr<T>::operator=(const embedded_ptr<TT>& other)
{
	if (other)
	{
		m_rel = other.m_rel + reinterpret_cast<const char*>(&other) - reinterpret_cast<char*>(this);
	}
	else
	{
		m_rel = 0;
	}

	return *this;
}
template<typename T>
T* embedded_ptr<T>::operator->()
{
	return reinterpret_cast<T*>(reinterpret_cast<char*>(this) + m_rel);
}
template<typename T>
const T* embedded_ptr<T>::operator->() const
{
	return reinterpret_cast<const T*>(reinterpret_cast<const char*>(this) + m_rel);
}
template<typename T>
T& embedded_ptr<T>::operator*()
{
	return *reinterpret_cast<T*>(reinterpret_cast<char*>(this) + m_rel);
}
template<typename T>
const T& embedded_ptr<T>::operator*() const
{
	return *reinterpret_cast<const T*>(reinterpret_cast<const char*>(this) + m_rel);
}
template<typename T>
embedded_ptr<T>::operator bool() const
{
	return m_rel != 0;
}
template<typename T>
bool embedded_ptr<T>::operator==(const embedded_ptr& other) const
{
	if (m_rel != 0 && other.m_rel != 0)
	{
		return (reinterpret_cast<const char*>(this) + m_rel) == (reinterpret_cast<const char*>(&other) + other.m_rel);
	}
	else
	{
		return m_rel == 0 && other.m_rel == 0;
	}
}
template<typename T>
bool embedded_ptr<T>::operator!=(const embedded_ptr& other) const
{
	if (m_rel != 0 && other.m_rel != 0)
	{
		return (reinterpret_cast<const char*>(this) + m_rel) != (reinterpret_cast<const char*>(&other) + other.m_rel);
	}
	else
	{
		return m_rel != 0 || other.m_rel != 0;
	}
}

}