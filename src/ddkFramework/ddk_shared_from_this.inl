
namespace ddk
{

template<typename T,typename TT>
share_from_this<T,TT>::share_from_this(share_from_this&& other)
: m_refCounter(std::move(other.m_refCounter))
, m_deleter(nullptr)
{
	std::swap(m_deleter,other.m_deleter);
}
template<typename T,typename TT>
const IReferenceWrapperDeleter* share_from_this<T,TT>::set_deleter(const IReferenceWrapperDeleter* i_deleter)
{
	return (m_deleter = i_deleter);
}
template<typename T,typename TT>
shared_reference_counter* share_from_this<T,TT>::get_reference_counter()
{
	return &m_refCounter;
}
template<typename T,typename TT>
const shared_reference_counter* share_from_this<T,TT>::get_reference_counter() const
{
	return &m_refCounter;
}
template<typename T,typename TT>
ddk::shared_reference_wrapper<TT> share_from_this<T,TT>::ref_from_this()
{
	if(m_refCounter.hasSharedReferences() == false)
	{
		throw bad_ptr_exception{ "Sharing non reference shareable object" };
	}

	return as_shared_reference(static_cast<TT*>(this),tagged_pointer<shared_reference_counter>(&m_refCounter,ReferenceAllocationType::Embedded),m_deleter);
}
template<typename T,typename TT>
ddk::shared_reference_wrapper<const TT> share_from_this<T,TT>::ref_from_this() const
{
	if(m_refCounter.hasSharedReferences() == false)
	{
		throw bad_ptr_exception{ "Sharing non reference shareable object" };
	}

	return as_shared_reference(static_cast<const TT*>(this),tagged_pointer<shared_reference_counter>(&m_refCounter,ReferenceAllocationType::Embedded),m_deleter);
}
template<typename T,typename TT>
ddk::shared_reference_wrapper<TT> share_from_this<T,TT>::unsafe_ref_from_this()
{
	return as_shared_reference(static_cast<TT*>(this),tagged_pointer<shared_reference_counter>(&m_refCounter,ReferenceAllocationType::Embedded),m_deleter);
}
template<typename T,typename TT>
ddk::shared_reference_wrapper<const TT> share_from_this<T,TT>::unsafe_ref_from_this() const
{
	return as_shared_reference(static_cast<const TT*>(this),tagged_pointer<shared_reference_counter>(&m_refCounter,ReferenceAllocationType::Embedded),m_deleter);
}

}