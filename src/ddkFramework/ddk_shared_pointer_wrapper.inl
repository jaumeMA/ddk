
namespace ddk
{
namespace detail
{

template<typename T, bool Weakable>
shared_pointer_wrapper_impl<T,Weakable>::shared_pointer_wrapper_impl()
: m_data(nullptr)
, m_refCounter(nullptr)
, m_deleter(nullptr)
{
}
template<typename T, bool Weakable>
shared_pointer_wrapper_impl<T,Weakable>::shared_pointer_wrapper_impl(std::nullptr_t)
: m_data(nullptr)
, m_refCounter(nullptr)
, m_deleter(nullptr)
{
}
template<typename T, bool Weakable>
shared_pointer_wrapper_impl<T,Weakable>::shared_pointer_wrapper_impl(T* i_data,IReferenceWrapperDeleter* i_refDeleter)
: m_data(i_data)
, m_refCounter(nullptr)
, m_deleter(i_refDeleter)
{
	if(m_data)
	{
		m_refCounter = make_tagged_pointer<reference_counter>(ReferenceAllocationType::Dynamic);

		m_refCounter->incrementSharedReference();
	}
}
template<typename T, bool Weakable>
shared_pointer_wrapper_impl<T,Weakable>::shared_pointer_wrapper_impl(const shared_pointer_wrapper_impl& other)
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
template<typename T, bool Weakable>
shared_pointer_wrapper_impl<T,Weakable>::shared_pointer_wrapper_impl(shared_pointer_wrapper_impl&& other)
: m_data(nullptr)
, m_refCounter(nullptr)
, m_deleter(other.m_deleter)
{
	std::swap(m_data,other.m_data);
	std::swap(m_refCounter,other.m_refCounter);
}
template<typename T, bool Weakable>
TEMPLATE(typename TT,bool WWeakable)
REQUIRED_COND(Weakable == false || Weakable == WWeakable)
shared_pointer_wrapper_impl<T,Weakable>::shared_pointer_wrapper_impl(const shared_pointer_wrapper_impl<TT,WWeakable>& other)
: m_data(nullptr)
, m_refCounter(nullptr)
, m_deleter(other.m_deleter)
{
	static_assert(std::is_base_of<T,TT>::value,"You shall provide a base class of T");

	if((m_data = other.m_data))
	{
		if((m_refCounter = other.m_refCounter))
		{
			m_refCounter->incrementSharedReference();
		}
	}
}
template<typename T, bool Weakable>
TEMPLATE(typename TT,bool WWeakable)
REQUIRED_COND(Weakable == false || Weakable == WWeakable)
shared_pointer_wrapper_impl<T,Weakable>::shared_pointer_wrapper_impl(shared_pointer_wrapper_impl<TT,WWeakable>&& other)
: m_data(nullptr)
, m_refCounter(nullptr)
, m_deleter(other.m_deleter)
{
	static_assert(std::is_base_of<T,TT>::value,"You shall provide a base class of T");

	if((m_data = other.m_data))
	{
		std::swap(m_refCounter,other.m_refCounter);
	}

	other.m_data = nullptr;
}
template<typename T, bool Weakable>
shared_pointer_wrapper_impl<T,Weakable>::~shared_pointer_wrapper_impl()
{
	if(m_refCounter)
	{
		clearIfCounterVoid(m_refCounter->decrementSharedReference());
	}
}
template<typename T, bool Weakable>
shared_pointer_wrapper_impl<T,Weakable>& shared_pointer_wrapper_impl<T,Weakable>::operator=(std::nullptr_t)
{
	if(m_refCounter)
	{
		clearIfCounterVoid(m_refCounter->decrementSharedReference());
	}

	return *this;
}
template<typename T, bool Weakable>
shared_pointer_wrapper_impl<T,Weakable>& shared_pointer_wrapper_impl<T,Weakable>::operator=(const shared_pointer_wrapper_impl& other)
{
	if(m_data != other.m_data)
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
template<typename T, bool Weakable>
shared_pointer_wrapper_impl<T,Weakable>& shared_pointer_wrapper_impl<T,Weakable>::operator=(shared_pointer_wrapper_impl&& other)
{
	if(m_data != other.m_data)
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
template<typename T, bool Weakable>
TEMPLATE(typename TT,bool WWeakable)
REQUIRED_COND(Weakable == false || Weakable == WWeakable)
shared_pointer_wrapper_impl<T,Weakable>& shared_pointer_wrapper_impl<T,Weakable>::operator=(const shared_pointer_wrapper_impl<TT,WWeakable>& other)
{
	static_assert(std::is_base_of<T,TT>::value,"You shall provide inherited classes");

	if(m_data != static_cast<const T*>(other.m_data))
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
template<typename T, bool Weakable>
TEMPLATE(typename TT,bool WWeakable)
REQUIRED_COND(Weakable == false || Weakable == WWeakable)
shared_pointer_wrapper_impl<T,Weakable>& shared_pointer_wrapper_impl<T,Weakable>::operator=(shared_pointer_wrapper_impl<TT,WWeakable>&& other)
{
	static_assert(std::is_base_of<T,TT>::value,"You shall provide inherited classes");

	if(m_data != static_cast<const T*>(other.m_data))
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
template<typename T, bool Weakable>
bool shared_pointer_wrapper_impl<T,Weakable>::operator==(std::nullptr_t) const
{
	return m_data == nullptr;
}
template<typename T, bool Weakable>
bool shared_pointer_wrapper_impl<T,Weakable>::operator!=(std::nullptr_t) const
{
	return m_data != nullptr;
}
template<typename T, bool Weakable>
T* shared_pointer_wrapper_impl<T,Weakable>::operator->()
{
	return m_data;
}
template<typename T, bool Weakable>
const T* shared_pointer_wrapper_impl<T,Weakable>::operator->() const
{
	return m_data;
}
template<typename T, bool Weakable>
T& shared_pointer_wrapper_impl<T,Weakable>::operator*()
{
	if(m_data == nullptr)
	{
		MAKE_IT_CRASH
	}

	return *m_data;
}
template<typename T, bool Weakable>
const T& shared_pointer_wrapper_impl<T,Weakable>::operator*() const
{
	if(m_data == nullptr)
	{
		MAKE_IT_CRASH
	}

	return *m_data;
}
template<typename T, bool Weakable>
shared_pointer_wrapper_impl<T,Weakable>::operator bool() const
{
	return m_data != nullptr;
}
template<typename T, bool Weakable>
void shared_pointer_wrapper_impl<T,Weakable>::clear()
{
	if(m_refCounter)
	{
		clearIfCounterVoid(m_refCounter->decrementSharedReference());
	}
}
template<typename T, bool Weakable>
T* shared_pointer_wrapper_impl<T,Weakable>::get()
{
	return m_data;
}
template<typename T, bool Weakable>
const T* shared_pointer_wrapper_impl<T,Weakable>::get() const
{
	return m_data;
}
template<typename T, bool Weakable>
const IReferenceWrapperDeleter* shared_pointer_wrapper_impl<T,Weakable>::get_deleter() const
{
	return m_deleter;
}
template<typename T, bool Weakable>
bool shared_pointer_wrapper_impl<T,Weakable>::empty() const
{
	return m_data == nullptr;
}
template<typename T, bool Weakable>
shared_pointer_wrapper_impl<T,Weakable>::shared_pointer_wrapper_impl(T* i_data,const tagged_reference_counter& i_refCounter, const IReferenceWrapperDeleter* i_refDeleter, bool i_alreadyIncremented)
: m_data(i_data)
, m_refCounter(i_refCounter)
, m_deleter(i_refDeleter)
{
	if(m_refCounter && !i_alreadyIncremented)
	{
		m_refCounter->incrementSharedReference();
	}
}
template<typename T, bool Weakable>
void shared_pointer_wrapper_impl<T,Weakable>::clearIfCounterVoid(size_t i_currNumRefs)
{
	if(i_currNumRefs == 0)
	{
		DDK_ASSERT(m_refCounter->hasLentReferences() == false,"Still lent references alive while destroying unique reference");

		const short tagCategory = m_refCounter.get_tag();

		reference_counter* refCounter = m_refCounter.extract_pointer();
		if(refCounter->hasWeakReferences() == false)
		{
			if(tagCategory == ReferenceAllocationType::Dynamic)
			{
				::delete refCounter;
			}
			else if(tagCategory == ReferenceAllocationType::Contiguous)
			{
				refCounter->~reference_counter();
			}
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

}
}