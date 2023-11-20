
#include "ddk_iterable_interface_utils.h"
#include "ddk_transformed_iterable_impl.h"

namespace ddk
{
namespace detail
{

template<typename Transform>
TEMPLATE(typename TTransform)
REQUIRED(IS_CONSTRUCTIBLE(Transform,TTransform))
iterable_transform<Transform>::iterable_transform(TTransform&& i_transform)
: m_transform(std::forward<TTransform>(i_transform))
{
}
template<typename Transform>
const Transform& iterable_transform<Transform>::get_transform() const
{
	return m_transform;
}

template<typename FromTraits,typename ToTraits>
typename traits_conversion_callable<FromTraits,ToTraits>::to_reference traits_conversion_callable<FromTraits,ToTraits>::operator()(from_reference i_value) const
{
	return i_value;
}
template<typename FromTraits,typename ToTraits>
typename traits_conversion_callable<FromTraits,ToTraits>::to_const_reference traits_conversion_callable<FromTraits,ToTraits>::operator()(from_const_reference i_value) const
{
	return i_value;
}

template<typename PublicTraits, typename PrivateTraits, typename Iterable, typename Transform>
TEMPLATE(typename IIterable,typename TTransform)
REQUIRED(IS_CONSTRUCTIBLE(Iterable,IIterable),IS_CONSTRUCTIBLE(Transform,FFunction))
transformed_iterable_impl<PublicTraits,PrivateTraits,Iterable,Transform>::transformed_iterable_impl(IIterable&& i_iterable,TTransform&& i_transform)
: m_iterable(std::forward<IIterable>(i_iterable))
, m_transform(std::forward<TTransform>(i_transform))
{
}
template<typename PublicTraits, typename PrivateTraits, typename Iterable,typename Transform>
TEMPLATE(typename Function, typename Action)
REQUIRED(IS_CALLABLE_BY(Function,reference),TRANSFORMED_ACTION_TAGS_SUPPORTED(PublicTraits,Function))
void transformed_iterable_impl<PublicTraits,PrivateTraits,Iterable,Transform>::iterate_impl(Function&& i_try,const Action& i_initialAction)
{
	m_iterable.iterate_impl([&](private_reference i_value) { return ddk::eval(std::forward<Function>(i_try),ddk::terse_eval(m_transform,std::forward<private_reference>(i_value))); },i_initialAction);
}
template<typename PublicTraits, typename PrivateTraits, typename Iterable,typename Transform>
TEMPLATE(typename Function, typename Action)
REQUIRED(IS_CALLABLE_BY(Function,reference),TRANSFORMED_ACTION_TAGS_SUPPORTED(PublicTraits,Function))
void transformed_iterable_impl<PublicTraits,PrivateTraits,Iterable,Transform>::iterate_impl(Function&& i_try,const Action& i_initialAction) const
{
	m_iterable.iterate_impl([&](private_const_reference i_value) { return ddk::eval(std::forward<Function>(i_try),ddk::terse_eval(m_transform,std::forward<private_const_reference>(i_value))); },i_initialAction);
}
template<typename PublicTraits,typename PrivateTraits,typename Iterable,typename Transform>
template<typename ... Args>
auto transformed_iterable_impl<PublicTraits,PrivateTraits,Iterable,Transform>::operator()(Args&& ... i_args) const
{
	return ddk::eval(std::forward<Function>(i_try),ddk::eval(m_transform,std::forward<Args>(i_args)...));
}

}
}
