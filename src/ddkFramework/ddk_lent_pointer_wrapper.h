#pragma once

#include "ddk_reference_counter.h"
#include "ddk_tagged_pointer.h"

namespace ddk
{

#ifdef DDK_DEBUG

#define THIS_OBJECT (*this)

#define REGISTER_STACK_TRACE(_OBJECT) \
	if (_OBJECT.m_refCounter) \
	{ \
		_OBJECT.m_refCounter->registerStackTrace(reinterpret_cast<size_t>(&_OBJECT)); \
	}
#define UNREGISTER_STACK_TRACE(_OBJECT) \
	if (_OBJECT.m_refCounter) \
	{ \
		_OBJECT.m_refCounter->unregisterStackTrace(reinterpret_cast<size_t>(&_OBJECT)); \
	}
#define COPY_STACK_TRACE(_OLD_OBJECT,_NEW_OBJECT) \
	if (_NEW_OBJECT.m_refCounter) \
	{ \
		_NEW_OBJECT.m_refCounter->copyStackTrace(reinterpret_cast<size_t>(&_OLD_OBJECT),reinterpret_cast<size_t>(&_NEW_OBJECT)); \
	}
#define MOVE_STACCK_TRACE(_OLD_OBJECT,_NEW_OBJECT) \
	if (_NEW_OBJECT.m_refCounter) \
	{ \
		_NEW_OBJECT.m_refCounter->reassignStackTrace(reinterpret_cast<size_t>(&_OLD_OBJECT),reinterpret_cast<size_t>(&_NEW_OBJECT)); \
	}

template<typename>
class lent_reference_wrapper;
template<typename>
class unique_pointer_wrapper;
template<typename>
class shared_pointer_wrapper;

template<typename T>
class lent_pointer_wrapper
{
	template<typename>
	friend class lent_pointer_wrapper;
	template<typename>
	friend class lent_reference_wrapper;
	template<typename TT>
	friend lent_pointer_wrapper<TT> lend(const unique_pointer_wrapper<TT>& i_uniqueRef);
	template<typename TTT, typename TT>
	friend lent_pointer_wrapper<TTT> dynamic_lent_cast(const lent_pointer_wrapper<TT>&);
	template<typename TTT, typename TT>
	friend lent_pointer_wrapper<TTT> dynamic_lent_cast(const lent_reference_wrapper<TT>&);
	template<typename TTT, typename TT>
	friend lent_pointer_wrapper<TTT> static_lent_cast(const lent_pointer_wrapper<TT>&);
	template<typename TTT, typename TT>
	friend lent_reference_wrapper<TTT> static_lent_cast(const lent_reference_wrapper<TT>&);
	template<typename TTT, typename TT>
	friend lent_pointer_wrapper<TTT> reinterpret_lent_cast(const lent_pointer_wrapper<TT>&);
	template<typename TT>
	lent_pointer_wrapper<TT> const_lent_cast(const lent_pointer_wrapper<const TT>&);
	template<typename TT>
	lent_reference_wrapper<TT> const_lent_cast(const lent_reference_wrapper<const TT>&);
	template<typename TT>
	friend lent_reference_wrapper<TT> promote_to_ref(const lent_pointer_wrapper<TT>&);
	template<typename TT>
	friend lent_pointer_wrapper<TT> __make_lent_pointer(TT*, const tagged_pointer<lent_reference_counter>&);

	lent_pointer_wrapper(T* i_data, const tagged_pointer<lent_reference_counter>& i_refCounter)
	: m_data(i_data)
	, m_refCounter(i_refCounter)
	{
		if (m_refCounter)
		{
			m_refCounter->incrementWeakReference();
		}

		REGISTER_STACK_TRACE(THIS_OBJECT);
	}

public:
	typedef tagged_pointer<lent_reference_counter> tagged_reference_counter;
	typedef T nested_type;

	lent_pointer_wrapper()
	: m_data(NULL)
	, m_refCounter(NULL)
	{
	}
	lent_pointer_wrapper(const std::nullptr_t&)
	: m_data(NULL)
	, m_refCounter(NULL)
	{
	}
	lent_pointer_wrapper(const lent_pointer_wrapper& other)
	: m_data(NULL)
	, m_refCounter(NULL)
	{
		m_data = other.m_data;

		if(m_refCounter = other.m_refCounter)
		{
			m_refCounter->incrementWeakReference();
		}

		REGISTER_STACK_TRACE(THIS_OBJECT);
	}
	lent_pointer_wrapper(lent_pointer_wrapper&& other)
	: m_data(NULL)
	, m_refCounter(NULL)
	{
		std::swap(m_data,other.m_data);
		std::swap(m_refCounter,other.m_refCounter);

		MOVE_STACCK_TRACE(other,THIS_OBJECT)
	}
	template<typename TT>
	lent_pointer_wrapper(const lent_pointer_wrapper<TT>& other, typename std::enable_if<std::is_base_of<T,TT>::value>::type* = NULL)
	: m_data(NULL)
	, m_refCounter(NULL)
	{
		m_data = other.m_data;

		if(m_refCounter = other.m_refCounter)
		{
			m_refCounter->incrementWeakReference();
		}

		REGISTER_STACK_TRACE(THIS_OBJECT);
	}
	template<typename TT>
	lent_pointer_wrapper(lent_pointer_wrapper<TT>&& other, typename std::enable_if<std::is_base_of<T,TT>::value>::type* = NULL)
	: m_data(NULL)
	, m_refCounter(NULL)
	{
		if((m_data = dynamic_cast<T*>(other.m_data)))
		{
			std::swap(m_refCounter,other.m_refCounter);
		}

		other.m_data = NULL;

		MOVE_STACCK_TRACE(other,THIS_OBJECT)
	}
	~lent_pointer_wrapper()
	{
		UNREGISTER_STACK_TRACE(THIS_OBJECT)

		if(m_refCounter)
		{
			m_refCounter->decrementWeakReference();
		}
		m_data = NULL;
	}
	lent_pointer_wrapper& operator=(const std::nullptr_t&)
	{
		UNREGISTER_STACK_TRACE(THIS_OBJECT)

		if (m_refCounter)
		{
			m_refCounter->decrementWeakReference();
		}

		m_data = NULL;
		m_refCounter = NULL;

		return *this;
	}
	lent_pointer_wrapper& operator=(const lent_pointer_wrapper& other)
	{
		if (m_data != other.m_data)
		{
			UNREGISTER_STACK_TRACE(THIS_OBJECT)

			if(m_refCounter)
			{
				m_refCounter->decrementWeakReference();
			}

			m_data = other.m_data;

			if(m_refCounter = other.m_refCounter)
			{
				m_refCounter->incrementWeakReference();
			}

			COPY_STACK_TRACE(other,THIS_OBJECT)
		}

		return *this;
	}
	lent_pointer_wrapper& operator=(lent_pointer_wrapper&& other)
	{
		if (m_data != other.m_data)
		{
			UNREGISTER_STACK_TRACE(THIS_OBJECT)

			if(m_refCounter)
			{
				m_refCounter->decrementWeakReference();
			}

			m_data = other.m_data;
			other.m_data = NULL;
			m_refCounter = other.m_refCounter;
			other.m_refCounter = NULL;

			MOVE_STACCK_TRACE(other,THIS_OBJECT)
		}

		return *this;
	}
	template<typename TT>
	lent_pointer_wrapper& operator=(const lent_pointer_wrapper<TT>& other)
	{
		static_assert(std::is_base_of<TT,T>::value, "You shall provide inherited classes");

		if (m_data != static_cast<const T*>(other.m_data))
		{
			UNREGISTER_STACK_TRACE(THIS_OBJECT)

			if(m_refCounter)
			{
				m_refCounter->decrementWeakReference();
			}

			m_data = other.m_data;

			if(m_refCounter = other.m_refCounter)
			{
				m_refCounter->incrementWeakReference();
			}

			COPY_STACK_TRACE(other,THIS_OBJECT)
		}

		return *this;
	}
	template<typename TT>
	lent_pointer_wrapper& operator=(lent_pointer_wrapper<TT>&& other)
	{
		static_assert(std::is_base_of<T,TT>::value, "You shall provide inherited classes");

		if (m_data != static_cast<const T*>(other.m_data))
		{
			UNREGISTER_STACK_TRACE(THIS_OBJECT)

			if(m_refCounter)
			{
				m_refCounter->decrementWeakReference();
			}

			m_data = other.m_data;
			other.m_data = NULL;
			m_refCounter = other.m_refCounter;
			other.m_refCounter = NULL;

			MOVE_STACCK_TRACE(other,THIS_OBJECT)
		}

		return *this;
	}
	bool operator==(std::nullptr_t) const
	{
		return m_data == NULL;
	}
	bool operator!=(std::nullptr_t) const
	{
		return m_data != NULL;
	}
	T* operator->()
	{
		return m_data;
	}
	const T* operator->() const
	{
		return m_data;
	}
	T& operator*()
	{
		if(m_data == NULL)
		{
			MAKE_IT_CRASH
		}

		return *m_data;
	}
	const T& operator*() const
	{
		if(m_data == NULL)
		{
			MAKE_IT_CRASH
		}

		return *m_data;
	}
	operator bool() const
	{
		return m_data != NULL;
	}
	void clear()
	{
		UNREGISTER_STACK_TRACE(THIS_OBJECT)

		m_data = NULL;

		if (m_refCounter)
		{
			m_refCounter->decrementWeakReference();

			m_refCounter = NULL;
		}
	}
	T* get()
	{
		return m_data;
	}
	const T* get() const
	{
		return m_data;
	}
	tagged_reference_counter get_reference_counter() const
	{
		return m_refCounter;
	}

private:
	T* m_data;
	tagged_reference_counter m_refCounter;
};

#else

template<typename T>
using lent_pointer_wrapper = T*;

#endif

}
