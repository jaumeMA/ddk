#pragma once

#include "reference_counter.h"
#include "reference_wrapper_deleter.h"
#include <type_traits>
#include <utility>
#include "tagged_pointer.h"
#include "lent_reference_wrapper.h"

#define ENSURE_SCOPE_LIFETIME(_PTR) const auto ___##_PTR = _PTR;

namespace ddk
{

template<typename T>
class shared_pointer_wrapper
{
	template<typename>
	friend class shared_pointer_wrapper;
	template<typename>
	friend class shared_reference_wrapper;
	template<typename TT,typename TTT>
	friend shared_pointer_wrapper<TT> static_shared_cast(const shared_pointer_wrapper<TTT>&);
	template<typename TT,typename TTT>
	friend shared_reference_wrapper<TT> static_shared_cast(const shared_reference_wrapper<TTT>&);
	template<typename TT,typename TTT>
	friend shared_pointer_wrapper<TT> dynamic_shared_cast(const shared_pointer_wrapper<TTT>&);
	template<typename TT,typename TTT>
	friend shared_pointer_wrapper<TT> dynamic_shared_cast(const shared_reference_wrapper<TTT>&);
	template<typename TT>
	friend shared_reference_wrapper<TT> const_shared_cast(const shared_reference_wrapper<const TT>&);
	template<typename TT>
	friend shared_pointer_wrapper<TT> const_shared_cast(const shared_pointer_wrapper<const TT>&);
	template<typename TT>
	friend lent_pointer_wrapper<TT> lend(shared_pointer_wrapper<TT>&);
	template<typename TT>
	friend lent_pointer_wrapper<TT> lend(const shared_pointer_wrapper<TT>&);
	template<typename TT>
	friend lent_reference_wrapper<TT> lend(shared_reference_wrapper<TT>&);
	template<typename TT>
	friend lent_reference_wrapper<TT> lend(const shared_reference_wrapper<TT>&);
	template<typename TT>
	friend shared_reference_wrapper<TT> promote_to_ref(const shared_pointer_wrapper<TT>&);

	template<typename TT>
	friend shared_pointer_wrapper<TT> __make_shared_pointer(TT* i_data, const tagged_pointer<shared_reference_counter>& i_refCounter, const IReferenceWrapperDeleter* i_refDeleter);

public:
	typedef tagged_pointer<shared_reference_counter> tagged_reference_counter;
	typedef T nested_type;

	shared_pointer_wrapper()
	: m_data(nullptr)
	, m_refCounter(nullptr)
	, m_deleter(nullptr)
	{
	}
	shared_pointer_wrapper(std::nullptr_t)
	: m_data(nullptr)
	, m_refCounter(nullptr)
	, m_deleter(nullptr)
	{
	}
	shared_pointer_wrapper(T* i_data, IReferenceWrapperDeleter* i_refDeleter = nullptr)
	: m_data(i_data)
	, m_refCounter(nullptr)
	, m_deleter(i_refDeleter)
	{
		if (m_data)
		{
            m_refCounter = make_tagged_pointer<shared_reference_counter>(ReferenceAllocationType::Dynamic);

			m_refCounter->incrementSharedReference();
		}
	}
	shared_pointer_wrapper(const shared_pointer_wrapper& other)
	: m_data(nullptr)
	, m_refCounter(nullptr)
	, m_deleter(other.m_deleter)
	{
		m_data = other.m_data;

		if((m_refCounter = other.m_refCounter))
		{
			m_refCounter->incrementSharedReference();
		}
	}
	shared_pointer_wrapper(shared_pointer_wrapper&& other)
	: m_data(nullptr)
	, m_refCounter(nullptr)
	, m_deleter(other.m_deleter)
	{
		std::swap(m_data,other.m_data);
		std::swap(m_refCounter,other.m_refCounter);
	}
	template<typename TT>
	shared_pointer_wrapper(const shared_pointer_wrapper<TT>& other)
	: m_data(nullptr)
	, m_refCounter(nullptr)
	, m_deleter(other.m_deleter)
	{
		static_assert(std::is_base_of<T,TT>::value, "You shall provide a base class of T");

		if((m_data = dynamic_cast<T*>(other.m_data)))
		{
			if((m_refCounter = other.m_refCounter))
			{
				m_refCounter->incrementSharedReference();
			}
		}
	}
	template<typename TT>
	shared_pointer_wrapper(shared_pointer_wrapper<TT>&& other)
	: m_data(nullptr)
	, m_refCounter(nullptr)
	, m_deleter(other.m_deleter)
	{
		static_assert(std::is_base_of<T,TT>::value, "You shall provide a base class of T");

		if((m_data = dynamic_cast<T*>(other.m_data)))
		{
			std::swap(m_refCounter,other.m_refCounter);
		}

		other.m_data = nullptr;
	}
	~shared_pointer_wrapper()
	{
		if(m_refCounter)
		{
			clearIfCounterVoid(m_refCounter->decrementSharedReference());
		}
	}
	shared_pointer_wrapper& operator=(std::nullptr_t)
	{
		if (m_refCounter)
		{
			clearIfCounterVoid(m_refCounter->decrementSharedReference());
		}

		return *this;
	}
	shared_pointer_wrapper& operator=(const shared_pointer_wrapper& other)
	{
		if (m_data != other.m_data)
		{
			if(m_refCounter)
			{
				clearIfCounterVoid(m_refCounter->decrementSharedReference());
			}

			m_deleter = other.m_deleter;
			m_data = other.m_data;

			if((m_refCounter = other.m_refCounter))
			{
				m_refCounter->incrementSharedReference();
			}
		}

		return *this;
	}
	shared_pointer_wrapper& operator=(shared_pointer_wrapper&& other)
	{
		if (m_data != other.m_data)
		{
			if(m_refCounter)
			{
				clearIfCounterVoid(m_refCounter->decrementSharedReference());
			}

			m_deleter = other.m_deleter;
			m_data = other.m_data;
			other.m_data = nullptr;
			m_refCounter = other.m_refCounter;
			other.m_refCounter = nullptr;
		}

		return *this;
	}
	template<typename TT>
	shared_pointer_wrapper& operator=(const shared_pointer_wrapper<TT>& other)
	{
		static_assert(std::is_base_of<T,TT>::value, "You shall provide inherited classes");

		if (m_data != static_cast<const T*>(other.m_data))
		{
			if(m_refCounter)
			{
				clearIfCounterVoid(m_refCounter->decrementSharedReference());
			}

			if((m_data = dynamic_cast<T*>(other.m_data)))
			{
				m_deleter = other.m_deleter;
				m_refCounter = other.m_refCounter;

				if(m_refCounter)
				{
					m_refCounter->incrementSharedReference();
				}
			}
		}

		return *this;
	}
	template<typename TT>
	shared_pointer_wrapper& operator=(shared_pointer_wrapper<TT>&& other)
	{
		static_assert(std::is_base_of<T,TT>::value, "You shall provide inherited classes");

		if (m_data != static_cast<const T*>(other.m_data))
		{
			if(m_refCounter)
			{
				clearIfCounterVoid(m_refCounter->decrementSharedReference());
			}

			if((m_data = dynamic_cast<T*>(other.m_data)))
			{
				m_deleter = other.m_deleter;
			}

			other.m_data = nullptr;
			m_refCounter = other.m_refCounter;
			other.m_refCounter = nullptr;
		}

		return *this;
	}
	bool operator==(std::nullptr_t) const
	{
		return m_data == nullptr;
	}
	bool operator!=(std::nullptr_t) const
	{
		return m_data != nullptr;
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
		if(m_data == nullptr)
		{
			MAKE_IT_CRASH
		}

		return *m_data;
	}
	const T& operator*() const
	{
		if(m_data == nullptr)
		{
			MAKE_IT_CRASH
		}

		return *m_data;
	}
	operator bool() const
	{
		return m_data != nullptr;
	}
	void clear()
	{
		if (m_refCounter)
		{
			clearIfCounterVoid(m_refCounter->decrementSharedReference());
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
	const IReferenceWrapperDeleter* get_deleter() const
	{
		return m_deleter;
	}
	bool empty() const
	{
		return m_data == nullptr;
	}

private:
	shared_pointer_wrapper(T* i_data, const tagged_reference_counter& i_refCounter, const IReferenceWrapperDeleter* i_refDeleter = nullptr)
	: m_data(i_data)
	, m_refCounter(i_refCounter)
	, m_deleter(i_refDeleter)
	{
		if(m_refCounter)
		{
			m_refCounter->incrementSharedReference();
		}
	}
	void clearIfCounterVoid(size_t i_currNumRefs)
	{
		if(i_currNumRefs == 0)
		{
			EWAS_ASSERT(m_refCounter->hasWeakReferences() == false, "Still lent references alive while destroying unique reference");

			const short tagCategory = m_refCounter.get_tag();
			shared_reference_counter* refCounter = m_refCounter.extract_pointer();

			if (tagCategory == ReferenceAllocationType::Dynamic)
			{
				delete refCounter;
			}
			else if(tagCategory == ReferenceAllocationType::Contiguous)
			{
				refCounter->~shared_reference_counter();
			}

			if(m_deleter)
			{
				m_deleter->Deallocate(allocator_address_reference_wrapper(m_data));
			}
			else
			{
				::delete(m_data);
			}
		}

		m_data = nullptr;
		m_refCounter = nullptr;
	}

	T* m_data;
	tagged_reference_counter m_refCounter;
	const IReferenceWrapperDeleter* m_deleter;
};

template<typename T>
inline shared_pointer_wrapper<T> __make_shared_pointer(T* i_data, const tagged_pointer<shared_reference_counter>& i_refCounter, const IReferenceWrapperDeleter* i_refDeleter)
{
	return shared_pointer_wrapper<T>(i_data,i_refCounter,i_refDeleter);
}

}
