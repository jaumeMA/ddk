#pragma once

#include <type_traits>
#include <cstring>

#define TAG_POINTER(_PTR,_TAG) reinterpret_cast<size_t&>(*(&_PTR)) |= _TAG;
#define TAGGED_POINTER(_PTR,_TAG) (reinterpret_cast<const size_t&>(*(&_PTR)) & _TAG)

#define MASK_NUM_BITS 2

template<typename T>
class tagged_pointer
{
	static const short tag_mask = (1 << MASK_NUM_BITS) - 1;
	typedef T* interface_ptr;
	static const uintptr_t bitMask = ~tag_mask;

	template<typename TT>
	friend class tagged_pointer;
	template<typename TTT, typename TT>
	friend tagged_pointer<TTT> dynamic_tagged_cast(const tagged_pointer<TT>&);
	template<typename TTT, typename TT>
	friend tagged_pointer<TTT> static_tagged_cast(const tagged_pointer<TT>&);
	template<typename TTT, typename TT>
	friend tagged_pointer<TTT> reinterpret_tagged_cast(const tagged_pointer<TT>&);

public:
	typedef T raw_type;
	typedef typename std::add_pointer<T>::type value_pointer;
	typedef typename std::add_const<value_pointer>::type value_const_pointer;
	typedef typename std::add_lvalue_reference<T>::type value_reference;
	typedef typename std::add_const<value_reference>::type value_const_reference;

	tagged_pointer()
	: m_data(NULL)
	{
	}
	tagged_pointer(T* i_data)
	: m_data(i_data)
	{
	}
	tagged_pointer(T* i_data, short i_tag)
	: m_data(i_data)
	{
		DDK_ASSERT(i_tag <= tag_mask, "You shall provide a tag less than tag mask");

		m_data = as_pointer(as_number(i_data) | i_tag);
	}
	inline tagged_pointer(const tagged_pointer& other)
	: m_data(NULL)
	{
		m_data = other.m_data;
	}
	inline tagged_pointer(tagged_pointer&& other)
	: m_data(NULL)
	{
		std::swap(m_data,other.m_data);
	}
	template<typename TT>
	tagged_pointer(tagged_pointer<TT> other)
	: m_data(NULL)
	{
		static_assert(std::is_base_of<T,TT>::value, "You shall provider inherited type");

		m_data = other.m_data;
	}
	tagged_pointer& operator=(const tagged_pointer& other)
	{
		m_data = other.m_data;

		return *this;
	}
	template<typename TT>
	tagged_pointer& operator=(const tagged_pointer<TT>& other)
	{
		static_assert(std::is_base_of<T,TT>::value, "You shall provider inherited type");

		m_data = other.m_data;

		return *this;
	}
	inline void set_pointer(T* i_pointer)
	{
		m_data = as_pointer(as_number(i_pointer) | (as_number(m_data) & tag_mask));
	}
	inline void set_tag(short i_tag)
	{
		DDK_ASSERT(i_tag <= tag_mask, "You shall provide a tag less than tag mask");

		m_data = as_pointer((as_number(m_data) & bitMask) | i_tag);
	}
	inline short get_tag() const
	{
		return as_const_number(m_data) & tag_mask;
	}
	inline bool is_tagged() const
	{
		return (as_const_number(m_data) & tag_mask) != 0;
	}
	inline value_pointer operator->()
	{
		return as_pointer(as_number(m_data) & bitMask);
	}
	inline value_const_pointer operator->() const
	{
		return as_const_pointer(as_const_number(m_data) & bitMask);
	}
	inline value_reference operator*()
	{
		if (value_pointer pData = as_pointer(as_number(m_data) & bitMask))
		{
			return *pData;
		}
		else
		{
			MAKE_IT_CRASH

			return *as_pointer(0xDEAD);
		}
	}
	inline value_const_reference operator*() const
	{
		if (value_pointer pData = as_const_pointer(as_const_number(m_data) & bitMask))
		{
			return *pData;
		}
		else
		{
			MAKE_IT_CRASH

			return *as_pointer(0xDEAD);
		}
	}
	inline value_pointer get_pointer()
	{
		return as_pointer(as_number(m_data) & bitMask);
	}
	inline value_const_pointer get_pointer() const
	{
		return as_const_pointer(as_const_number(m_data) & bitMask);
	}
	inline value_pointer get_data()
	{
		return m_data;
	}
	inline value_const_pointer get_data() const
	{
		return m_data;
	}
	inline value_pointer extract_pointer()
	{
		value_pointer res = as_pointer(as_number(m_data) & bitMask);

		m_data = NULL;

		return res;
	}
	inline operator bool() const
	{
		return m_data != NULL;
	}
	inline bool operator==(const tagged_pointer& other) const
	{
		return m_data == other.m_data;
	}
	inline bool operator!=(const tagged_pointer& other) const
	{
		return m_data != other.m_data;
	}
	static inline T* tag_pointer(T* i_ptr, const short i_tag)
	{
		return as_pointer(as_number(i_ptr) | i_tag);
	}
	static inline uintptr_t tag_pointer_as_value(T* i_ptr, const short i_tag)
	{
		return as_number(i_ptr) | i_tag;
	}
	static inline T* untag_pointer(T* i_ptr)
	{
		return as_pointer(as_number(i_ptr) & bitMask);
	}
	static inline short get_tag(T* i_ptr)
	{
		return as_number(i_ptr) & tag_mask;
	}
	static inline bool is_tagged(T* i_ptr)
	{
		return (as_number(i_ptr) & tag_mask) != 0;
	}

private:
    union from_number_to_ptr
    {
        uintptr_t dataNumber;
        value_pointer dataPointer;
    };
    union from_ptr_to_number
    {
        value_pointer dataPointer;
        uintptr_t dataNumber;
    };

	static inline value_pointer as_pointer(uintptr_t i_value)
	{
        const from_number_to_ptr res = { i_value };

		return res.dataPointer;
	}
	static inline value_const_pointer as_const_pointer(uintptr_t i_value)
	{
        const from_number_to_ptr res = { i_value };

		return res.dataPointer;
	}
	static inline uintptr_t as_number(value_pointer i_ptr)
	{
        const from_ptr_to_number res = { i_ptr };

		return res.dataNumber;
	}
	static inline uintptr_t as_const_number(value_const_pointer i_ptr)
	{
        const from_ptr_to_number res = { i_ptr };

		return res.dataNumber;
	}

	T* m_data;
};

template<typename T, typename ... Args>
tagged_pointer<T> make_tagged_pointer(short i_tag, Args&& ... i_args)
{
	return tagged_pointer<T>(new T(std::forward<Args>(i_args)...), i_tag);
}

template<typename TT, typename T>
tagged_pointer<TT> dynamic_tagged_cast(const tagged_pointer<T>& i_ptr)
{
	TT* pData = dynamic_cast<TT*>(i_ptr.get_pointer());

	return (pData) ? tagged_pointer<TT>(pData,i_ptr.get_tag()) : tagged_pointer<TT>();
}
template<typename TT, typename T>
tagged_pointer<TT> static_tagged_cast(const tagged_pointer<T>& i_ptr)
{
	return tagged_pointer<TT>(static_cast<TT*>(i_ptr.get_pointer()),i_ptr.get_tag());
}
template<typename TT, typename T>
tagged_pointer<TT> reinterpret_tagged_cast(const tagged_pointer<T>& i_ptr)
{
	return tagged_pointer<TT>(reinterpret_cast<TT*>(i_ptr.get_pointer()),i_ptr.get_tag());
}
