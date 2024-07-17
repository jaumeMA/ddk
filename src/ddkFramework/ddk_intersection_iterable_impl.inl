
#include "ddk_await.h"

namespace ddk
{
namespace detail
{

template<typename ... Iterables>
TEMPLATE(typename ... IIterables)
REQUIRED(IS_CONSTRUCTIBLE(Iterables,IIterables)...)
intersection_iterable_impl<Iterables...>::intersection_iterable_impl(IIterables&& ... i_iterables)
: iterable_visitor<intersection_iterable_impl<Iterables...>>(i_iterables...)
{
}
template<typename ... Iterables>
TEMPLATE(typename Action)
REQUIRED(ACTION_SUPPORTED(traits,Action))
void intersection_iterable_impl<Iterables...>::iterate_impl(Action&& i_initialAction)
{
    this->loop(std::forward<Action>(i_initialAction));
}
template<typename ... Iterables>
TEMPLATE(typename Action)
REQUIRED(ACTION_SUPPORTED(const_traits,Action))
void intersection_iterable_impl<Iterables...>::iterate_impl(Action&& i_initialAction) const
{
    this->loop(std::forward<Action>(i_initialAction));
}

}

template<typename ... Iterables>
iterable_adaptor<detail::intersection_iterable_impl<Iterables...>>::iterable_adaptor(Iterables& ... i_iterables)
: m_adaptors(deduce_adaptor(i_iterables)...)
{
}
template<typename ... Iterables>
TEMPLATE(typename Adaptor, typename ActionTag)
REQUIRED(ACTION_TAGS_SUPPORTED(Adaptor,ActionTag))
auto iterable_adaptor<detail::intersection_iterable_impl<Iterables...>>::perform_action(Adaptor&& i_adaptor, ActionTag&& i_actionTag)
{
    typedef typename mpl::make_sequence<0,s_numTypes>::type seq_type;

    return perform_action(seq_type{},std::forward<Adaptor>(i_adaptor),intersection_action{std::forward<ActionTag>(i_actionTag)});
}
template<typename ... Iterables>
template<size_t ... Indexs,typename Adaptor,typename ActionTag>
auto iterable_adaptor<detail::intersection_iterable_impl<Iterables...>>::perform_action(const mpl::sequence<Indexs...>&,Adaptor&& i_adaptor,intersection_action<ActionTag> i_actionTag)
{
    return i_actionTag.template apply<Indexs...>(std::forward<Adaptor>(i_adaptor).m_adaptors.template get<Indexs>()...);
}

template<typename ... Iterables>
iterable_adaptor<const detail::intersection_iterable_impl<Iterables...>>::iterable_adaptor(const Iterables& ... i_iterables)
: m_adaptors(deduce_adaptor(i_iterables)...)
{
}
template<typename ... Iterables>
TEMPLATE(typename Adaptor, typename ActionTag)
REQUIRED(ACTION_TAGS_SUPPORTED(Adaptor,ActionTag))
constexpr auto iterable_adaptor<const detail::intersection_iterable_impl<Iterables...>>::perform_action(Adaptor&& i_adaptor, ActionTag&& i_actionTag)
{
    typedef typename mpl::make_sequence<0,s_numTypes>::type seq_type;

    return perform_action(seq_type{},std::forward<Adaptor>(i_adaptor),intersection_action{std::forward<ActionTag>(i_actionTag)});
}
template<typename ... Iterables>
template<size_t ... Indexs,typename Adaptor,typename ActionTag>
constexpr auto iterable_adaptor<const detail::intersection_iterable_impl<Iterables...>>::perform_action(const mpl::sequence<Indexs...>&,Adaptor&& i_adaptor,intersection_action<ActionTag> i_actionTag)
{
    return i_actionTag.template apply<Indexs...>(std::forward<Adaptor>(i_adaptor).m_adaptors.template get<Indexs>()...);
}

}