
#include "ddk_iterable_algorithm_impl.h"
#include "ddk_template_helper.h"

namespace ddk
{
namespace trans
{
namespace detail
{

template<typename Transform,typename ... Iterables>
iterable_pack<Transform,Iterables...>::iterable_pack(const Transform& i_transform,const Iterables& ... i_iterables)
: m_transform(i_transform)
, m_iterables(i_iterables...)
{
}
template<typename Transform,typename ... Iterables>
iterable_pack<Transform,Iterables...>::iterable_pack(Transform&& i_transform,const Iterables& ... i_iterables)
: m_transform(std::move(i_transform))
, m_iterables(i_iterables...)
{
}
template<typename Transform, typename ... Iterables>
iterable_pack<Transform,Iterables...>::iterable_pack(const Iterables& ... i_iterables)
: m_iterables(i_iterables...)
{
}
template<typename Transform, typename ... Iterables>
template<size_t Index>
const typename mpl::nth_type_of<Index,Iterables...>::type& iterable_pack<Transform,Iterables...>::get() const
{
	return m_iterables.template get<Index>();
}
template<typename Transform,typename ... Iterables>
Transform iterable_pack<Transform,Iterables...>::get_transform() const
{
	return m_transform;
}

}

TEMPLATE(typename Transform,typename ... Iterables)
REQUIRED(IS_CALLABLE(Transform))
detail::iterable_pack<Transform,decltype(deduce_iterable(std::declval<const Iterables&>()))...> iterable_transform(Transform&& i_transform,const Iterables& ... i_iterables)
{
	return detail::iterable_pack<Transform,decltype(deduce_iterable(std::declval<const Iterables&>()))...>{ std::forward<Transform>(i_transform), deduce_iterable(i_iterables) ... };
}
template<typename Iterable>
detail::iterable_pack<detail::neg_iterable_transform<Iterable>,decltype(deduce_iterable(std::declval<const Iterable&>()))> iterable_neg(const Iterable& i_iterable)
{
	return detail::iterable_pack<detail::neg_iterable_transform<Iterable>,decltype(deduce_iterable(std::declval<const Iterable&>()))>{ deduce_iterable(i_iterable) };
}
template<typename ... Iterables>
detail::iterable_pack<detail::sum_iterable_transform<Iterables...>,decltype(deduce_iterable(std::declval<const Iterables&>()))...> iterable_sum(const Iterables& ... i_iterables)
{
	return detail::iterable_pack<detail::sum_iterable_transform<Iterables...>,decltype(deduce_iterable(std::declval<const Iterables&>()))...>{ deduce_iterable(i_iterables) ... };
}
template<typename ... Iterables>
detail::iterable_pack<detail::subs_iterable_transform<Iterables...>,decltype(deduce_iterable(std::declval<const Iterables&>()))...> iterable_subs(const Iterables& ... i_iterables)
{
	return detail::iterable_pack<detail::subs_iterable_transform<Iterables...>,decltype(deduce_iterable(std::declval<const Iterables&>()))...>{ deduce_iterable(i_iterables) ... };
}
template<typename ... Iterables>
detail::iterable_pack<detail::prod_iterable_transform<Iterables...>,decltype(deduce_iterable(std::declval<const Iterables&>()))...> iterable_prod(const Iterables& ... i_iterables)
{
	return detail::iterable_pack<detail::prod_iterable_transform<Iterables...>,decltype(deduce_iterable(std::declval<const Iterables&>()))...>{ deduce_iterable(i_iterables) ... };
}

}
}

template<size_t ... Indexs, typename Sink, typename Transform,typename ... Iterables>
ddk::future<ddk::iter::action_result> _execute_transform(const ddk::mpl::sequence<Indexs...>&, Sink&& i_sink,const ddk::trans::detail::iterable_pack<Transform,Iterables...>& i_iterables)
{
	typedef typename ddk::detail::intersection_iterable<Iterables...>::related_iterable intersection_iterable_t;
	typedef typename intersection_iterable_t::iterable_value iterable_value;

	return ddk::trans::detail::iterable_transformation_dump(std::forward<Sink>(i_sink),ddk::view::transform([transform = i_iterables.get_transform()](ddk::values_tuple<typename Iterables::const_reference ...> i_value) { return transform(i_value.template get<Indexs>() ...); }) <<= ddk::fusion(i_iterables.template get<Indexs>()...));
}
template<typename Sink,typename Transform,typename Iterable>
ddk::future<ddk::iter::action_result> operator<<=(Sink&& i_sink,const ddk::trans::detail::iterable_pack<Transform,Iterable>& i_iterables)
{
	return ddk::trans::detail::iterable_transformation_dump(std::forward<Sink>(i_sink),ddk::view::transform([transform = i_iterables.get_transform()](typename Iterable::const_reference i_value) { return transform(i_value); }) <<= i_iterables.template get<0>());
}
template<typename Sink, typename Transform,typename ... Iterables>
ddk::future<ddk::iter::action_result> operator<<=(Sink&& i_sink,const ddk::trans::detail::iterable_pack<Transform,Iterables...>& i_iterables)
{
	return _execute_transform(typename ddk::mpl::make_sequence<0,ddk::mpl::get_num_types<Iterables...>()>::type{},std::forward<Sink>(i_sink),i_iterables);
}
