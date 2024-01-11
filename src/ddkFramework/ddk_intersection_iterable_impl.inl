
#include "ddk_await.h"

namespace ddk
{
namespace detail
{

template<typename ... Iterables>
intersection_iterable_impl<Iterables...>::intersection_iterable_impl(Iterables& ... i_iterables)
: iterable_visitor<intersection_iterable_impl<Iterables...>>(i_iterables...)
{
}
template<typename ... Iterables>
TEMPLATE(typename Action)
REQUIRED(ACTION_SUPPORTED(traits,Action))
iterable_result intersection_iterable_impl<Iterables...>::iterate_impl(Action&& i_initialAction)
{
    return loop(std::forward<Action>(i_initialAction));
}
template<typename ... Iterables>
TEMPLATE(typename Action)
REQUIRED(ACTION_SUPPORTED(const_traits,Action))
iterable_result intersection_iterable_impl<Iterables...>::iterate_impl(Action&& i_initialAction) const
{
    return loop(std::forward<Action>(i_initialAction));
}

}

template<typename ... Iterables>
iterable_adaptor<detail::intersection_iterable_impl<Iterables...>>::iterable_adaptor(Iterables& ... i_iterables)
: m_adaptors(deduce_adaptor(i_iterables)...)
{
}
template<typename ... Iterables>
TEMPLATE(typename ActionTag)
REQUIRED(ACTION_TAGS_SUPPORTED(traits,ActionTag))
auto iterable_adaptor<detail::intersection_iterable_impl<Iterables...>>::perform_action(ActionTag&& i_actionTag)
{
    typedef typename mpl::make_sequence<0,s_numTypes>::type seq_type;

    return perform_action(seq_type{},intersection_action{ std::forward<ActionTag>(i_actionTag) });
}
template<typename ... Iterables>
TEMPLATE(typename ActionTag)
REQUIRED(ACTION_TAGS_SUPPORTED(const_traits,ActionTag))
auto iterable_adaptor<detail::intersection_iterable_impl<Iterables...>>::perform_action(ActionTag&& i_actionTag) const
{
    typedef typename mpl::make_sequence<0,s_numTypes>::type seq_type;

    return perform_action(seq_type{},intersection_action{ std::forward<ActionTag>(i_actionTag) });
}
template<typename ... Iterables>
template<size_t ... Indexs,typename ActionTag>
auto iterable_adaptor<detail::intersection_iterable_impl<Iterables...>>::perform_action(const mpl::sequence<Indexs...>&,intersection_action<ActionTag> i_actionTag)
{
    return i_actionTag.template apply<Indexs...>(m_adaptors.template get<Indexs>()...);
}
template<typename ... Iterables>
template<size_t ... Indexs,typename ActionTag>
auto iterable_adaptor<detail::intersection_iterable_impl<Iterables...>>::perform_action(const mpl::sequence<Indexs...>&,intersection_action<ActionTag> i_actionTag) const
{
    return i_actionTag.template apply<Indexs...>(m_adaptors.template get<Indexs>()...);
}

template<typename ... Iterables>
iterable_adaptor<const detail::intersection_iterable_impl<Iterables...>>::iterable_adaptor(const Iterables& ... i_iterables)
: m_adaptors(deduce_adaptor(i_iterables)...)
{
}
template<typename ... Iterables>
TEMPLATE(typename ActionTag)
REQUIRED(ACTION_TAGS_SUPPORTED(traits,ActionTag))
auto iterable_adaptor<const detail::intersection_iterable_impl<Iterables...>>::perform_action(ActionTag&& i_actionTag) const
{
    typedef typename mpl::make_sequence<0,s_numTypes>::type seq_type;

    return perform_action(seq_type{},intersection_action{ std::forward<ActionTag>(i_actionTag) });
}
template<typename ... Iterables>
template<size_t ... Indexs,typename ActionTag>
auto iterable_adaptor<const detail::intersection_iterable_impl<Iterables...>>::perform_action(const mpl::sequence<Indexs...>&,intersection_action<ActionTag> i_actionTag) const
{
    return i_actionTag.template apply<Indexs...>(m_adaptors.template get<Indexs>()...);
}

}