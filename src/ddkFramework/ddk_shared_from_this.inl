
namespace ddk
{

template<typename T,typename TT>
shared_reference_wrapper<TT> share_from_this<T,TT>::shared_from_this()
{
	if(m_refCounter->hasSharedReferences() == false)
	{
		throw bad_ptr_exception{ "Sharing non reference shareable object" };
	}

	return as_shared_reference(static_cast<TT*>(this),m_refCounter);
}
template<typename T,typename TT>
shared_reference_wrapper<const TT> share_from_this<T,TT>::shared_from_this() const
{
	if(m_refCounter->hasSharedReferences() == false)
	{
		throw bad_ptr_exception{ "Sharing non reference shareable object" };
	}

	return as_shared_reference(static_cast<const TT*>(this),m_refCounter);
}
template<typename T,typename TT>
typename share_from_this<T,TT>::tagged_reference_counter share_from_this<T,TT>::get_reference_counter() const
{
	return m_refCounter;
}
template<typename T,typename TT>
void share_from_this<T,TT>::set_reference_counter(const tagged_reference_counter& i_refCounter) const
{
	m_refCounter = i_refCounter;
}

}