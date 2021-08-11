
namespace ddk
{

template<typename T>
unique_pointer_wrapper<T>::unique_pointer_wrapper()
: m_data(nullptr)
, m_refCounter(nullptr)
{
}
template<typename T>
unique_pointer_wrapper<T>::unique_pointer_wrapper(std::nullptr_t)
: m_data(nullptr)
, m_refCounter(nullptr)
{
}
template<typename T>
unique_pointer_wrapper<T>::unique_pointer_wrapper(unique_pointer_wrapper&& other)
: m_data(nullptr)
, m_refCounter(nullptr)
{
	std::swap(m_data,other.m_data);
	std::swap(m_refCounter,other.m_refCounter);
}
template<typename T>
template<typename TT>
unique_pointer_wrapper<T>::unique_pointer_wrapper(unique_pointer_wrapper<TT>&& other)
: m_data(nullptr)
, m_refCounter(nullptr)
{
	static_assert(std::is_base_of<T,TT>::value,"You shall provide inherited classes");

	if((m_data = other.m_data))
	{
		std::swap(m_refCounter,other.m_refCounter);
	}

	other.m_data = nullptr;
}
template<typename T>
unique_pointer_wrapper<T>::~unique_pointer_wrapper()
{
	if(m_refCounter)
	{
		clearIfCounterVoid(m_refCounter->removeStrongReference());
	}
}
template<typename T>
unique_pointer_wrapper<T>& unique_pointer_wrapper<T>::operator=(std::nullptr_t)
{
	if(m_refCounter)
	{
		clearIfCounterVoid(m_refCounter->removeStrongReference());
	}

	return *this;
}
template<typename T>
unique_pointer_wrapper<T>& unique_pointer_wrapper<T>::operator=(unique_pointer_wrapper&& other)
{
	if(m_data != other.m_data)
	{
		if(m_refCounter)
		{
			clearIfCounterVoid(m_refCounter->removeStrongReference());
		}

		m_data = other.m_data;
		other.m_data = nullptr;
		m_refCounter = other.m_refCounter;
		other.m_refCounter = nullptr;
	}

	return *this;
}
template<typename T>
template<typename TT>
unique_pointer_wrapper<T>& unique_pointer_wrapper<T>::operator=(unique_pointer_wrapper<TT>&& other)
{
	static_assert(std::is_base_of<T,TT>::value,"You shall provide inherited classes");

	if(m_data != static_cast<const T*>(other.m_data))
	{
		if(m_refCounter)
		{
			clearIfCounterVoid(m_refCounter->removeStrongReference());
		}

		m_data = other.m_data;
		other.m_data = nullptr;
		m_refCounter = other.m_refCounter;
		other.m_refCounter = nullptr;
	}

	return *this;
}
template<typename T>
bool unique_pointer_wrapper<T>::operator==(std::nullptr_t) const
{
	return m_data == nullptr;
}
template<typename T>
bool unique_pointer_wrapper<T>::operator!=(std::nullptr_t) const
{
	return m_data != nullptr;
}
template<typename T>
T* unique_pointer_wrapper<T>::operator->()
{
	return m_data;
}
template<typename T>
const T* unique_pointer_wrapper<T>::operator->() const
{
	return m_data;
}
template<typename T>
T& unique_pointer_wrapper<T>::operator*()
{
	if(m_data == nullptr)
	{
		MAKE_IT_CRASH
	}

	return *m_data;
}
template<typename T>
const T& unique_pointer_wrapper<T>::operator*() const
{
	if(m_data == nullptr)
	{
		MAKE_IT_CRASH
	}

	return *m_data;
}
template<typename T>
unique_pointer_wrapper<T>::operator bool() const
{
	return m_data != nullptr;
}
template<typename T>
void unique_pointer_wrapper<T>::clear()
{
	if(m_refCounter)
	{
		clearIfCounterVoid(m_refCounter->removeStrongReference());
	}
}
template<typename T>
T* unique_pointer_wrapper<T>::get()
{
	return m_data;
}
template<typename T>
const T* unique_pointer_wrapper<T>::get() const
{
	return m_data;
}
template<typename T>
bool unique_pointer_wrapper<T>::empty() const
{
	return m_data == nullptr;
}
template<typename T>
unique_pointer_wrapper<T>::unique_pointer_wrapper(T* i_data,const tagged_reference_counter& i_refCounter)
: m_data(i_data)
, m_refCounter(i_refCounter)
{
	m_refCounter->addStrongReference();
}
template<typename T>
unique_pointer_wrapper<T>::unique_pointer_wrapper(T* i_data,tagged_pointer<unique_reference_counter>&& i_refCounter)
: m_data(i_data)
, m_refCounter(std::move(i_refCounter))
{
}
template<typename T>
void unique_pointer_wrapper<T>::clearIfCounterVoid(bool i_hasRefs)
{
	if(i_hasRefs == false)
	{
		DDK_ASSERT(m_refCounter->hasLentReferences() == false,"Still lent references alive while destroying unique reference");

		const short tagCategory = m_refCounter.get_tag();
		unique_reference_counter* refCounter = m_refCounter.extract_pointer();

		refCounter->destroy_unique_resource(tagCategory);
	}

	m_refCounter = nullptr;
	m_data = nullptr;
}
template<typename T>
unique_reference_counter* unique_pointer_wrapper<T>::extract_reference_counter()
{
	return m_refCounter.extract_pointer();
}

}