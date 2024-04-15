
#include "ddk_reference_wrapper.h"

namespace ddk
{

template<typename T,typename TT>
distribute_from_this<T,TT>::distribute_from_this(tagged_reference_counter i_refCounter)
: m_refCounter(i_refCounter)
{
}
template<typename T,typename TT>
ddk::distributed_reference_wrapper<TT> distribute_from_this<T,TT>::distributed_from_this()
{
	return as_shared_reference(static_cast<T*>(this),m_refCounter);
}
template<typename T,typename TT>
ddk::distributed_reference_wrapper<const TT> distribute_from_this<T,TT>::distributed_from_this() const
{
	return as_shared_reference(static_cast<const T*>(this),m_refCounter);
}
template<typename T,typename TT>
typename distribute_from_this<T,TT>::tagged_reference_counter distribute_from_this<T,TT>::get_reference_counter() const
{
	return m_refCounter;
}
template<typename T,typename TT>
void distribute_from_this<T,TT>::set_reference_counter(tagged_reference_counter i_refCounter) const
{
	m_refCounter = i_refCounter;
}

}