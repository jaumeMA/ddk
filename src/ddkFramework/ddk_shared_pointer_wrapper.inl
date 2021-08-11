
#include "ddk_pointer_exceptions.h"

namespace ddk
{
namespace detail
{

template<typename T, typename ReferenceCounter>
shared_pointer_wrapper_impl<T,ReferenceCounter>::shared_pointer_wrapper_impl()
: m_data(nullptr)
, m_refCounter(nullptr)
{
}
template<typename T, typename ReferenceCounter>
shared_pointer_wrapper_impl<T,ReferenceCounter>::shared_pointer_wrapper_impl(std::nullptr_t)
: m_data(nullptr)
, m_refCounter(nullptr)
{
}
template<typename T, typename ReferenceCounter>
shared_pointer_wrapper_impl<T,ReferenceCounter>::shared_pointer_wrapper_impl(const shared_pointer_wrapper_impl& other)
: m_data(other.m_data)
, m_refCounter(other.m_refCounter)
{
	if(m_refCounter)
	{
		m_refCounter->incrementSharedReference();
	}
}
template<typename T, typename ReferenceCounter>
shared_pointer_wrapper_impl<T,ReferenceCounter>::shared_pointer_wrapper_impl(shared_pointer_wrapper_impl&& other)
: m_data(nullptr)
, m_refCounter(nullptr)
{
	std::swap(m_data,other.m_data);
	std::swap(m_refCounter,other.m_refCounter);
}
template<typename T, typename ReferenceCounter>
template<typename TT>
shared_pointer_wrapper_impl<T,ReferenceCounter>::shared_pointer_wrapper_impl(const shared_pointer_wrapper_impl<TT,ReferenceCounter>& other)
: m_data(other.m_data)
, m_refCounter(other.m_refCounter)
{
	static_assert(std::is_base_of<T,TT>::value,"You shall provide a base class of T");

	if(m_refCounter)
	{
		m_refCounter->incrementSharedReference();
	}
}
template<typename T, typename ReferenceCounter>
template<typename TT>
shared_pointer_wrapper_impl<T,ReferenceCounter>::shared_pointer_wrapper_impl(shared_pointer_wrapper_impl<TT,ReferenceCounter>&& other)
: m_data(nullptr)
, m_refCounter(nullptr)
{
	static_assert(std::is_base_of<T,TT>::value,"You shall provide a base class of T");

	if((m_data = other.m_data))
	{
		std::swap(m_refCounter,other.m_refCounter);
	}

	other.m_data = nullptr;
}
template<typename T, typename ReferenceCounter>
shared_pointer_wrapper_impl<T,ReferenceCounter>::shared_pointer_wrapper_impl(T* i_data,const tagged_reference_counter& i_refCounter, bool i_alreadyIncremented)
: m_data(i_data)
, m_refCounter(i_refCounter)
{
	if(m_refCounter && !i_alreadyIncremented)
	{
		m_refCounter->incrementSharedReference();
	}
}
template<typename T, typename ReferenceCounter>
shared_pointer_wrapper_impl<T,ReferenceCounter>::~shared_pointer_wrapper_impl()
{
	if(m_refCounter)
	{
		clearIfCounterVoid(m_refCounter->decrementSharedReference());
	}
}
template<typename T, typename ReferenceCounter>
shared_pointer_wrapper_impl<T,ReferenceCounter>& shared_pointer_wrapper_impl<T,ReferenceCounter>::operator=(std::nullptr_t)
{
	if(m_refCounter)
	{
		clearIfCounterVoid(m_refCounter->decrementSharedReference());
	}

	return *this;
}
template<typename T, typename ReferenceCounter>
shared_pointer_wrapper_impl<T,ReferenceCounter>& shared_pointer_wrapper_impl<T,ReferenceCounter>::operator=(const shared_pointer_wrapper_impl& other)
{
	if(m_data != other.m_data)
	{
		if(m_refCounter)
		{
			clearIfCounterVoid(m_refCounter->decrementSharedReference());
		}

		m_data = other.m_data;

		if((m_refCounter = other.m_refCounter))
		{
			m_refCounter->incrementSharedReference();
		}
	}

	return *this;
}
template<typename T, typename ReferenceCounter>
shared_pointer_wrapper_impl<T,ReferenceCounter>& shared_pointer_wrapper_impl<T,ReferenceCounter>::operator=(shared_pointer_wrapper_impl&& other)
{
	if(m_data != other.m_data)
	{
		if(m_refCounter)
		{
			clearIfCounterVoid(m_refCounter->decrementSharedReference());
		}

		m_data = other.m_data;
		other.m_data = nullptr;
		m_refCounter = other.m_refCounter;
		other.m_refCounter = nullptr;
	}

	return *this;
}
template<typename T, typename ReferenceCounter>
template<typename TT>
shared_pointer_wrapper_impl<T,ReferenceCounter>& shared_pointer_wrapper_impl<T,ReferenceCounter>::operator=(const shared_pointer_wrapper_impl<TT,ReferenceCounter>& other)
{
	static_assert(std::is_base_of<T,TT>::value,"You shall provide inherited classes");

	if(m_data != static_cast<const T*>(other.m_data))
	{
		if(m_refCounter)
		{
			clearIfCounterVoid(m_refCounter->decrementSharedReference());
		}

		if((m_data = static_cast<T*>(other.m_data)))
		{
			m_refCounter = other.m_refCounter;

			if(m_refCounter)
			{
				m_refCounter->incrementSharedReference();
			}
		}
	}

	return *this;
}
template<typename T, typename ReferenceCounter>
template<typename TT>
shared_pointer_wrapper_impl<T,ReferenceCounter>& shared_pointer_wrapper_impl<T,ReferenceCounter>::operator=(shared_pointer_wrapper_impl<TT,ReferenceCounter>&& other)
{
	static_assert(std::is_base_of<T,TT>::value,"You shall provide inherited classes");

	if(m_data != static_cast<const T*>(other.m_data))
	{
		if(m_refCounter)
		{
			clearIfCounterVoid(m_refCounter->decrementSharedReference());
		}

		m_data = static_cast<T*>(other.m_data);
		other.m_data = nullptr;
		m_refCounter = other.m_refCounter;
		other.m_refCounter = nullptr;
	}

	return *this;
}
template<typename T, typename ReferenceCounter>
bool shared_pointer_wrapper_impl<T,ReferenceCounter>::operator==(std::nullptr_t) const
{
	return m_data == nullptr;
}
template<typename T, typename ReferenceCounter>
bool shared_pointer_wrapper_impl<T,ReferenceCounter>::operator!=(std::nullptr_t) const
{
	return m_data != nullptr;
}
template<typename T, typename ReferenceCounter>
T* shared_pointer_wrapper_impl<T,ReferenceCounter>::operator->()
{
	return m_data;
}
template<typename T, typename ReferenceCounter>
const T* shared_pointer_wrapper_impl<T,ReferenceCounter>::operator->() const
{
	return m_data;
}
template<typename T, typename ReferenceCounter>
T& shared_pointer_wrapper_impl<T,ReferenceCounter>::operator*()
{
	if(m_data == nullptr)
	{
		MAKE_IT_CRASH
	}

	return *m_data;
}
template<typename T, typename ReferenceCounter>
const T& shared_pointer_wrapper_impl<T,ReferenceCounter>::operator*() const
{
	if(m_data == nullptr)
	{
		MAKE_IT_CRASH
	}

	return *m_data;
}
template<typename T, typename ReferenceCounter>
shared_pointer_wrapper_impl<T,ReferenceCounter>::operator bool() const
{
	return m_data != nullptr;
}
template<typename T, typename ReferenceCounter>
void shared_pointer_wrapper_impl<T,ReferenceCounter>::clear()
{
	if(m_refCounter)
	{
		clearIfCounterVoid(m_refCounter->decrementSharedReference());
	}
}
template<typename T, typename ReferenceCounter>
T* shared_pointer_wrapper_impl<T,ReferenceCounter>::get()
{
	return m_data;
}
template<typename T, typename ReferenceCounter>
const T* shared_pointer_wrapper_impl<T,ReferenceCounter>::get() const
{
	return m_data;
}
template<typename T, typename ReferenceCounter>
bool shared_pointer_wrapper_impl<T,ReferenceCounter>::empty() const
{
	return m_data == nullptr;
}
template<typename T, typename ReferenceCounter>
void shared_pointer_wrapper_impl<T,ReferenceCounter>::clearIfCounterVoid(size_t i_currNumRefs)
{
	if(i_currNumRefs == 0)
	{
		DDK_ASSERT(m_refCounter->hasLentReferences() == false,"Still lent references alive while destroying unique reference");

		const short tagCategory = m_refCounter.get_tag();

		if(ReferenceCounter* refCounter = m_refCounter.extract_pointer())
		{
			refCounter->destroy_shared_resource(tagCategory);
		}
		else
		{
			throw unmanaged_pointer_exception{"Deallocating shared pointer with empty reference counter"};
		}
	}

	m_data = nullptr;
	m_refCounter = nullptr;
}

}
}
