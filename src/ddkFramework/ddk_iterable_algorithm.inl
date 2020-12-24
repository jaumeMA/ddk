
#include "ddk_iterable_algorithm_impl.h"
#include "ddk_template_helper.h"

namespace ddk
{
namespace trans
{
namespace detail
{

template<typename ... Iterables>
iterable_pack<Iterables...>::iterable_pack(const Iterables& ... i_iterables)
: m_iterables(i_iterables...)
{
}
template<typename ... Iterables>
template<size_t Index>
const typename mpl::nth_type_of<Index,Iterables...>::type& iterable_pack<Iterables...>::get() const
{
	return m_iterables.template get<Index>();
}

}

template<typename ... Iterables>
detail::iterable_pack<decltype(deduce_iterable(std::declval<const Iterables&>()))...> iterable_sum(const Iterables& ... i_iterables)
{
	return detail::iterable_pack<decltype(deduce_iterable(std::declval<const Iterables&>()))...>{ deduce_iterable(i_iterables) ... };
}

}
}

template<size_t ... Indexs, typename Sink,typename ... Iterables>
ddk::future<ddk::iter::action_result> _execute_transform(const ddk::mpl::sequence<Indexs...>&, Sink&& i_sink,const ddk::trans::detail::iterable_pack<Iterables...>& i_iterables)
{
	typedef typename ddk::detail::intersection_iterable<Iterables...>::related_iterable intersection_iterable_t;
	typedef typename intersection_iterable_t::iterable_value iterable_value;

	return ddk::trans::detail::iterable_transformation_dump(std::forward<Sink>(i_sink),ddk::view::transform([](ddk::values_tuple<typename Iterables::const_reference ...> i_value) { return (i_value.get<Indexs>() + ...); }) <<= ddk::fusion(i_iterables.template get<Indexs>()...));
}
template<typename Sink,typename ... Iterables>
ddk::future<ddk::iter::action_result> operator<<=(Sink&& i_sink,const ddk::trans::detail::iterable_pack<Iterables...>& i_iterables)
{
	return _execute_transform(typename ddk::mpl::make_sequence<0,ddk::mpl::get_num_types<Iterables...>::value>::type{},std::forward<Sink>(i_sink),i_iterables);
}