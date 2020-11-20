#pragma once

#include <utility>
#include "ddk_template_helper.h"
#include <type_traits>

namespace ddk
{

//internal type
template<typename T>
struct embedded_type;

template<typename T>
struct embedded_type<T&>
{
public:
    //avoid right references
    typedef typename std::remove_cv<T>::type raw_type;
    typedef T internal_type;
    typedef T& ref_type;
    typedef typename std::add_const<T>::type& cref_type;
    typedef T& rref_type;
    typedef T* pointer_type;
    typedef typename std::add_const<T>::type* cpointer_type;

public:
	inline embedded_type(internal_type& other)
	: m_data(other)
	{
	}
	inline embedded_type(const embedded_type<T&>& other)
	: m_data(other.m_data)
	{
	}
    embedded_type() = delete;
    inline T& operator=(internal_type& other) = delete;
    inline T& operator=(internal_type&& other) = delete;
    inline T& operator=(embedded_type<T&>&& other) = delete;
    template<typename Type>
	inline embedded_type& operator=(Type&& val)
	{
		new(&m_data)embedded_type(std::forward<Type>(val));

		return *this;
	}
	inline bool operator==(const embedded_type<T&>& other) const
	{
		return m_data == other.m_data;
	}
	inline rref_type extract()
	{
		return m_data;
	}
	inline cref_type get() const
	{
		return m_data;
	}
	inline ref_type get()
	{
		return m_data;
	}
	inline pointer_type get_ptr()
	{
		return &m_data;
	}
	inline cref_type operator*() const
	{
		return m_data;
	}
	inline ref_type operator*()
	{
		return m_data;
	}
	inline pointer_type operator->()
	{
		return &m_data;
	}
	inline cpointer_type operator->() const
	{
		return &m_data;
	}
	inline operator ref_type()
	{
		return m_data;
	}
	inline operator cref_type() const
	{
		return m_data;
	}
    template<typename Type>
	inline static bool construct(void* address, Type&& val)
	{
	  return new(address)embedded_type(std::forward<Type>(val)) != nullptr;
	}
	inline static bool destroy(void* address)
	{
		//in references nothing needs to be done
		return true;
	}
    template<typename Type>
	inline static bool assign(void* address, Type&& val)
	{
		//references must be reconstructed every time
		return new(address)embedded_type(std::forward<Type>(val)) != nullptr;
	}
	inline static bool swap(void* addressA, internal_type& valA, void* addressB, internal_type& valB)
	{
		internal_type& tmp = valA;

		return construct<internal_type&>(addressA, valB) && construct<internal_type&>(addressB, tmp);
	}

private:
    internal_type&			m_data;
};

template<typename T>
struct embedded_type<T&&>
{
public:
    //avoid right references
    typedef typename std::remove_cv<T>::type raw_type;
    typedef T internal_type;
    typedef T&& ref_type;
    typedef typename std::add_const<T>::type&& cref_type;
    typedef T&& rref_type;
    typedef T* pointer_type;
    typedef typename std::add_const<T>::type* cpointer_type;

	inline embedded_type(T&& other)
	: m_data(std::move(other))
	{
	}
	inline embedded_type(embedded_type&& other)
	: m_data(std::move(other.m_data))
	{
	}
    //this is a ref, we do not allow assignments beyond construction
    embedded_type() = delete;
    embedded_type(internal_type& other) = delete;
    T& operator=(internal_type& other) = delete;
    T& operator=(internal_type&& other) = delete;
    T& operator=(embedded_type<const T&>&& other) = delete;
	inline bool operator==(const embedded_type<T&&>& other) const
	{
		return m_data == other.m_data;
	}
	inline cref_type get() const
	{
		return std::move(m_data);
	}
	inline ref_type get()
	{
		return std::move(m_data);
	}
	inline pointer_type get_ptr()
	{
		return &m_data;
	}
	inline rref_type extract()
	{
		return std::move(m_data);
	}
	inline cref_type operator*() const
	{
		return m_data;
	}
	inline ref_type operator*()
	{
		return m_data;
	}
	inline pointer_type operator->()
	{
		return &m_data;
	}
	inline cpointer_type operator->() const
	{
		return &m_data;
	}
	inline operator ref_type()
	{
		return m_data;
	}
	inline operator cref_type() const
	{
		return m_data;
	}
    template<typename Type>
	inline static bool construct(void* address, Type&& val)
	{
	  return new(address) embedded_type(std::move(val)) != nullptr;
	}
	inline static bool destroy(void* address)
	{
		//in references nothing needs to be done
		return true;
	}
    template<typename Type>
	inline static bool assign(void* address, Type&& val)
	{
		//references must be reconstructed every time

		return construct<Type>(address, std::forward<Type>(val));
	}
	inline static bool swap(void* addressA, internal_type&& valA, void* addressB, internal_type&& valB)
	{
		internal_type&& tmp = std::move(valA);

		return construct<internal_type&&>(addressA, std::move(valB)) && construct<internal_type&&>(addressB, std::move(tmp));
	}

private:
    internal_type&&			m_data;
};

template<typename T>
struct embedded_type
{
public:
    //avoid deleted methods
    typedef typename std::remove_cv<T>::type raw_type;
    typedef T internal_type;
    typedef T& ref_type;
    typedef typename std::add_const<T>::type& cref_type;
    typedef T&& rref_type;
    typedef T* pointer_type;
    typedef typename std::add_const<T>::type* cpointer_type;

	inline embedded_type(const embedded_type<T>& other)
	: m_data(other.m_data)
	{
	}
	inline embedded_type(embedded_type<T>&& other)
	: m_data(std::move(other.m_data))
	{
	}
	template<typename ... Args>
	inline embedded_type(Args&& ... i_args)
	: m_data(std::forward<Args>(i_args) ...)
	{
	}
	inline embedded_type& operator=(const internal_type& other)
	{
		m_data = other;

		return *this;
	}
	inline embedded_type& operator=(internal_type&& other)
	{
		m_data = std::move(other);

		return *this;
	}
	inline embedded_type& operator=(const embedded_type<T>& other)
	{
		m_data = other.m_data;

		return *this;
	}
	inline embedded_type& operator=(embedded_type<T>&& other)
	{
		m_data = std::move(other.m_data);

		return *this;
	}
	inline bool operator==(const embedded_type<T>& other) const
	{
		return m_data == other.m_data;
	}
	inline cref_type get() const
	{
		return std::forward<embedded_type<T>::cref_type>(m_data);
	}
	inline ref_type get()
	{
		return std::forward<embedded_type<T>::ref_type>(m_data);
	}
	inline cpointer_type get_ptr() const
	{
		return &m_data;
	}
	inline pointer_type get_ptr()
	{
		return &m_data;
	}
	inline rref_type extract()
	{
		return std::forward<embedded_type<T>::rref_type>(m_data);
	}
	inline cref_type operator*() const
	{
		return std::forward<embedded_type<T>::cref_type>(m_data);
	}
	inline ref_type operator*()
	{
		return std::forward<embedded_type<T>::ref_type>(m_data);
	}
	inline pointer_type operator->()
	{
		return &m_data;
	}
	inline cpointer_type operator->() const
	{
		return &m_data;
	}
	inline operator rref_type()
	{
		return std::forward<embedded_type<T>::rref_type>(m_data);
	}
    template<typename ... Args>
	inline static bool construct(void* address, Args&& ... i_args)
	{
		return new(address) embedded_type(std::forward<Args>(i_args) ...) != nullptr;
	}
	inline static bool destroy(void* address)
	{
		embedded_type<T>* _data = reinterpret_cast<embedded_type<T>* >(address);

		_data->m_data.~T();

		return true;
	}
    template<typename Type>
	inline static bool assign(void* address, Type&& val)
	{
		reinterpret_cast<embedded_type<T>* >(address)->m_data = std::forward<Type>(val);

		return true;
	}
	inline static bool swap(void* addressA, internal_type& valA, void* addressB, internal_type& valB)
	{
		internal_type tmp = valA;

		embedded_type<T>* _Adata = reinterpret_cast<embedded_type<T>* >(addressA);
		*_Adata = valB;

		embedded_type<T>* _Bdata = reinterpret_cast<embedded_type<T>* >(addressB);
		*_Bdata = tmp;

		return true;
	}

private:
    internal_type  m_data;
};

}
