
#include "ddk_reference_wrapper.h"

namespace ddk
{

template<typename T,typename ... Args>
distributed_value<T> make_distributed_value(Args&& ... i_args)
{
	return { make_distributed_reference<T>(std::forward<Args>(i_args)...) };
}
template<typename T,typename ... Args>
unique_value<T> make_unique_value(Args&& ... i_args)
{
	return { make_unique_reference<T>(std::forward<Args>(i_args)...) };
}
template<typename T>
distributed_value<T> promote_to_value(const distributed_object<T>& i_obj)
{
	return { promote_to_ref(i_obj.m_value),i_obj.m_typeInfo };
}
template<typename T>
unique_value<T> promote_to_value(unique_object<T> i_obj)
{
	return { promote_to_ref(std::move(i_obj.m_value)),i_obj.m_typeInfo };
}
template<typename TT>
lent_value<TT> promote_to_value(const lent_object<TT>& i_obj)
{
	return { promote_to_ref(i_obj.m_value),i_obj .m_typeInfo };
}
template<typename T>
lent_object<T> lend(const distributed_object<T>& i_obj)
{
	return { lend(i_obj.m_value),i_obj.m_typeInfo };
}
template<typename T>
lent_value<T> lend(const distributed_value<T>& i_obj)
{
	return { lend(i_obj.m_value),i_obj.m_typeInfo };
}
template<typename T>
lent_object<T> lend(const unique_object<T>& i_obj)
{
	return { lend(i_obj.m_value),i_obj.m_typeInfo };
}
template<typename T>
lent_value<T> lend(const unique_value<T>& i_obj)
{
	return { lend(i_obj.m_value),i_obj.m_typeInfo };
}

}