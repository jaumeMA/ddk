
#include "ddk_container_exceptions.h"

namespace ddk
{
namespace detail
{

template<typename T,size_t rank,size_t ... ranks>
high_order_sub_array<T,rank,ranks...>::high_order_sub_array(T& i_ref)
: m_ref(i_ref)
{
}
template<typename T,size_t rank,size_t ... ranks>
high_order_sub_array<T,ranks...> high_order_sub_array<T,rank,ranks...>::operator[](size_t i_index)
{
	if(i_index < rank)
	{
		return *(&m_ref + i_index * sizeof(T));
	}
	else
	{
		throw bad_access_exception{ "Index out of bounds" };
	}
}
template<typename T,size_t rank,size_t ... ranks>
high_order_sub_array<const T,ranks...> high_order_sub_array<T,rank,ranks...>::operator[](size_t i_index) const
{
	if(i_index < rank)
	{
		return *(&m_ref + i_index * sizeof(T));
	}
	else
	{
		throw bad_access_exception{ "Index out of bounds" };
	}
}

template<typename T>
high_order_sub_array<T>::high_order_sub_array(T& i_ref)
: m_ref(i_ref)
{
}
template<typename T>
template<typename TT>
high_order_sub_array<T>& high_order_sub_array<T>::operator=(TT&& i_value)
{
	m_ref = std::forward<TT>(i_value);

	return *this;
}
template<typename T>
high_order_sub_array<T>::operator T&()
{
	return m_ref;
}
template<typename T>
high_order_sub_array<T>::operator const T&() const
{
	return m_ref;
}

}

template<typename T,size_t rank,size_t ... ranks>
template<typename ... Args>
high_order_array<T,rank,ranks...>::high_order_array(Args&& ... i_initValues)
: m_data{ i_initValues ... }
{
}
template<typename T,size_t rank,size_t ... ranks>
high_order_array<T,rank,ranks...>::high_order_array(const high_order_array<T,rank,ranks...>& other)
{
	if constexpr(std::is_trivially_copyable<T>::value)
	{
		memcpy(m_data,other.m_data,s_totalSize * sizeof(T));
	}
	else
	{
		for(size_t index = 0; index < s_totalSize; ++index)
		{
			m_data[index] = other.m_data[index];
		}
	}
}
template<typename T,size_t rank,size_t ... ranks>
template<typename TT>
high_order_array<T,rank,ranks...>::high_order_array(const high_order_array<TT,rank,ranks...>& other)
{
	for(size_t index = 0; index < s_totalSize; ++index)
	{
		m_data[index] = other.m_data[index];
	}
}
template<typename T,size_t rank,size_t ... ranks>
detail::high_order_sub_array<T,ranks...> high_order_array<T,rank,ranks...>::operator[](size_t i_index)
{
	if(i_index < rank)
	{
		return m_data[i_index];
	}
	else
	{
		throw bad_access_exception{ "Index out of bounds" };
	}
}
template<typename T,size_t rank,size_t ... ranks>
detail::high_order_sub_array<const T,ranks...> high_order_array<T,rank,ranks...>::operator[](size_t i_index) const
{
	if(i_index < rank)
	{
		return m_data[i_index];
	}
	else
	{
		throw bad_access_exception{ "Index out of bounds" };
	}
}
template<typename T,size_t rank,size_t ... ranks>
high_order_array<T,rank,ranks...>& high_order_array<T,rank,ranks...>::operator=(const high_order_array<T,rank,ranks...>& other)
{
	if constexpr(std::is_trivially_copyable<T>::value)
	{
		memcpy(m_data,other.m_data,s_totalSize * sizeof(T));
	}
	else
	{
		for(size_t index = 0; index < s_totalSize; ++index)
		{
			m_data[index] = other.m_data[index];
		}
	}
}
template<typename T,size_t rank,size_t ... ranks>
template<typename TT>
high_order_array<T,rank,ranks...>& high_order_array<T,rank,ranks...>::operator=(const high_order_array<TT,rank,ranks...>& other)
{
	for(size_t index = 0; index < s_totalSize; ++index)
	{
		m_data[index] = other.m_data[index];
	}
}
template<typename T,size_t rank,size_t ... ranks>
size_t high_order_array<T,rank,ranks...>::size() const
{
	return s_totalSize;
}

}