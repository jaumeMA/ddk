#pragma once

#include "ddk_reference_counter.h"
#include "ddk_reference_wrapper_deleter.h"
#include "ddk_lent_reference_wrapper.h"
#include <type_traits>
#include <utility>
#include "ddk_tagged_pointer.h"
#include "ddk_reference_tag.h"

namespace ddk
{

template<typename>
class unique_reference_wrapper;

template<typename T>
class unique_pointer_wrapper
{
	template<typename>
	friend class unique_pointer_wrapper;
	template<typename>
	friend class unique_reference_wrapper;

	template<typename TTT, typename TT>
	friend unique_reference_wrapper<TTT> static_unique_cast(unique_reference_wrapper<TT>);
	template<typename TTT, typename TT>
	friend unique_pointer_wrapper<TTT> static_unique_cast(unique_pointer_wrapper<TT>);
	template<typename TTT,typename TT>
	friend unique_pointer_wrapper<TTT> dynamic_unique_cast(unique_pointer_wrapper<TT>&&);
	template<typename TTT,typename TT>
	friend unique_pointer_wrapper<TTT> dynamic_unique_cast(unique_reference_wrapper<TT>&&);
	template<typename TT>
	friend unique_reference_wrapper<TT> const_unique_cast(unique_reference_wrapper<const TT>);
	template<typename TT>
	friend unique_pointer_wrapper<TT> const_unique_cast(unique_pointer_wrapper<const TT>);
	template<typename TT>
	friend lent_pointer_wrapper<TT> lend(unique_pointer_wrapper<TT>&);
	template<typename TT>
	friend lent_pointer_wrapper<TT> lend(const unique_pointer_wrapper<TT>&);
	template<typename TT>
	friend lent_reference_wrapper<TT> lend(unique_reference_wrapper<TT>&);
	template<typename TT>
	friend lent_reference_wrapper<TT> lend(const unique_reference_wrapper<TT>&);
	template<typename TT>
	friend unique_reference_wrapper<TT> promote_to_ref(unique_pointer_wrapper<TT>);
    template<typename TT>
	friend inline unique_pointer_wrapper<TT> __make_unique_pointer(TT* i_data, const tagged_pointer<unique_reference_counter>& i_refCounter, const IReferenceWrapperDeleter* i_refDeleter);
    template<typename TT>
	friend inline unique_pointer_wrapper<TT> __make_unique_pointer(TT* i_data, tagged_pointer<unique_reference_counter>&& i_refCounter, const IReferenceWrapperDeleter* i_refDeleter);

public:
	typedef tagged_pointer<unique_reference_counter> tagged_reference_counter;
	typedef T nested_type;

	unique_pointer_wrapper(const unique_pointer_wrapper&) = delete;
	unique_pointer_wrapper& operator=(const unique_pointer_wrapper&) = delete;
	unique_pointer_wrapper()
	: m_data(nullptr)
	, m_refCounter(nullptr)
	, m_deleter(nullptr)
	{
	}
	unique_pointer_wrapper(std::nullptr_t)
	: m_data(nullptr)
	, m_refCounter(nullptr)
	, m_deleter(nullptr)
	{
	}
	unique_pointer_wrapper(unique_pointer_wrapper&& other)
	: m_data(nullptr)
	, m_refCounter(nullptr)
	, m_deleter(other.m_deleter)
	{
		std::swap(m_data,other.m_data);
		std::swap(m_refCounter,other.m_refCounter);
	}
	template<typename TT>
	unique_pointer_wrapper(unique_pointer_wrapper<TT>&& other)
	: m_data(nullptr)
	, m_refCounter(nullptr)
	, m_deleter(other.m_deleter)
	{
		static_assert(std::is_base_of<T,TT>::value, "You shall provide inherited classes");

		if((m_data = other.m_data))
		{
			std::swap(m_refCounter,other.m_refCounter);
		}

		other.m_data = nullptr;
	}
	~unique_pointer_wrapper()
	{
		if(m_refCounter)
		{
			clearIfCounterVoid(m_refCounter->removeStrongReference());
		}
	}
	unique_pointer_wrapper& operator=(std::nullptr_t)
	{
		if (m_refCounter)
		{
			clearIfCounterVoid(m_refCounter->removeStrongReference());
		}

		return *this;
	}
	unique_pointer_wrapper& operator=(unique_pointer_wrapper&& other)
	{
		if (m_data != other.m_data)
		{
			if(m_refCounter)
			{
				clearIfCounterVoid(m_refCounter->removeStrongReference());
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
	unique_pointer_wrapper& operator=(unique_pointer_wrapper<TT>&& other)
	{
		static_assert(std::is_base_of<T,TT>::value, "You shall provide inherited classes");

		if (m_data != static_cast<const T*>(other.m_data))
		{
			if(m_refCounter)
			{
				clearIfCounterVoid(m_refCounter->removeStrongReference());
			}

			if((m_data = other.m_data))
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
			clearIfCounterVoid(m_refCounter->removeStrongReference());
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
	bool empty() const
	{
		return m_data == nullptr;
	}
	const IReferenceWrapperDeleter* get_deleter() const
	{
		return m_deleter;
	}

private:
	unique_pointer_wrapper(T* i_data, const tagged_reference_counter& i_refCounter, const IReferenceWrapperDeleter* i_refDeleter = nullptr)
	: m_data(i_data)
	, m_refCounter(i_refCounter)
	, m_deleter(i_refDeleter)
	{
		m_refCounter->addStrongReference();
	}
	unique_pointer_wrapper(T* i_data, tagged_pointer<unique_reference_counter>&& i_refCounter, const IReferenceWrapperDeleter* i_refDeleter = nullptr)
	: m_data(i_data)
	, m_refCounter(std::move(i_refCounter))
	, m_deleter(i_refDeleter)
	{
	}
	void clearIfCounterVoid(bool i_hasRefs)
	{
		if(i_hasRefs == false)
		{
            DDK_ASSERT(m_refCounter->hasWeakReferences() == false, "Still lent references alive while destroying unique reference");

			const short tagCategory = m_refCounter.get_tag();
			unique_reference_counter* refCounter = m_refCounter.extract_pointer();

			if (tagCategory == ReferenceAllocationType::Dynamic)
			{
				delete refCounter;
			}
			else if(tagCategory == ReferenceAllocationType::Contiguous)
			{
				refCounter->~unique_reference_counter();
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

		m_refCounter = nullptr;
		m_data = nullptr;
	}
	unique_reference_counter* extract_reference_counter()
	{
		return m_refCounter.extract_pointer();
	}

	T* m_data;
	tagged_reference_counter m_refCounter;
	const IReferenceWrapperDeleter* m_deleter;
};

namespace mpl
{

template<typename T>
struct is_copy_constructible<unique_pointer_wrapper<T>>
{
    static const bool value = false;
};

}
}
