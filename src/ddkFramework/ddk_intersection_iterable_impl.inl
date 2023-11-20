
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
TEMPLATE(typename Function,typename Action)
REQUIRED(IS_CALLABLE_BY(Function,reference),IS_BASE_OF(shift_action,Action))
void intersection_iterable_impl<Iterables...>::iterate_impl(Function&& i_try,const Action& i_initialAction)
{
    loop(std::forward<Function>(i_try),i_initialAction);
}
template<typename ... Iterables>
TEMPLATE(typename Function,typename Action)
REQUIRED(IS_CALLABLE_BY(Function,const_reference),IS_BASE_OF(shift_action,Action))
void intersection_iterable_impl<Iterables...>::iterate_impl(Function&& i_try,const Action& i_initialAction) const
{
    loop(std::forward<Function>(i_try),i_initialAction);
}

}

template<typename ... Iterables>
iterable_adaptor<detail::intersection_iterable_impl<Iterables...>>::iterable_adaptor(Iterables& ... i_iterables)
: m_adaptors(deduce_adaptor(i_iterables)...)
{
}
template<typename ... Iterables>
auto iterable_adaptor<detail::intersection_iterable_impl<Iterables...>>::get_value()
{
    typedef typename mpl::make_sequence<0,s_numTypes>::type seq_type;

    return get_value(seq_type{});
}
template<typename ... Iterables>
auto iterable_adaptor<detail::intersection_iterable_impl<Iterables...>>::get_value() const
{
    typedef typename mpl::make_sequence<0,s_numTypes>::type seq_type;

    return get_value(seq_type{});
}
template<typename ... Iterables>
template<typename Sink>
auto iterable_adaptor<detail::intersection_iterable_impl<Iterables...>>::forward_value(Sink&& i_sink)
{
    typedef typename mpl::make_sequence<0,s_numTypes>::type seq_type;

    return forward_value(seq_type{},std::forward<Sink>(i_sink));
}
template<typename ... Iterables>
template<typename Sink>
auto iterable_adaptor<detail::intersection_iterable_impl<Iterables...>>::forward_value(Sink&& i_sink) const
{
    typedef typename mpl::make_sequence<0,s_numTypes>::type seq_type;

    return forward_value(seq_type{},std::forward<Sink>(i_sink));
}
template<typename ... Iterables>
bool iterable_adaptor<detail::intersection_iterable_impl<Iterables...>>::valid() const
{
    typedef typename mpl::make_sequence<0,s_numTypes>::type seq_type;

    return valid(seq_type{});
}
template<typename ... Iterables>
TEMPLATE(typename ActionTag)
REQUIRED(ACTION_TAGS_SUPPORTED(traits,ActionTag))
bool iterable_adaptor<detail::intersection_iterable_impl<Iterables...>>::perform_action(const ActionTag& i_actionTag)
{
    typedef typename mpl::make_sequence<0,s_numTypes>::type seq_type;

    return perform_action(seq_type{},i_actionTag);
}
template<typename ... Iterables>
TEMPLATE(typename ActionTag)
REQUIRED(ACTION_TAGS_SUPPORTED(traits,ActionTag))
bool iterable_adaptor<detail::intersection_iterable_impl<Iterables...>>::perform_action(const ActionTag& i_actionTag) const
{
    typedef typename mpl::make_sequence<0,s_numTypes>::type seq_type;

    return perform_action(seq_type{},i_actionTag);
}
template<typename ... Iterables>
template<size_t ... Indexs>
typename iterable_adaptor<detail::intersection_iterable_impl<Iterables...>>::reference iterable_adaptor<detail::intersection_iterable_impl<Iterables...>>::get_value(const mpl::sequence<Indexs...>&)
{
    return { m_adaptors.template get<Indexs>().get_value() ... };
}
template<typename ... Iterables>
template<size_t ... Indexs>
typename iterable_adaptor<detail::intersection_iterable_impl<Iterables...>>::const_reference iterable_adaptor<detail::intersection_iterable_impl<Iterables...>>::get_value(const mpl::sequence<Indexs...>&) const
{
    return { m_adaptors.template get<Indexs>().get_value() ... };
}
template<typename ... Iterables>
template<size_t ... Indexs,typename Sink>
auto iterable_adaptor<detail::intersection_iterable_impl<Iterables...>>::forward_value(const mpl::sequence<Indexs...>& ,Sink&& i_sink)
{
    return ddk::eval(std::forward<Sink>(i_sink),function_arguments{ m_adaptors.template get<Indexs>().get_value() ... });
}
template<typename ... Iterables>
template<size_t ... Indexs,typename Sink>
auto iterable_adaptor<detail::intersection_iterable_impl<Iterables...>>::forward_value(const mpl::sequence<Indexs...>& , Sink&& i_sink) const
{
    return ddk::eval(std::forward<Sink>(i_sink),function_arguments{ m_adaptors.template get<Indexs>().get_value() ... });
}
template<typename ... Iterables>
template<size_t ... Indexs>
bool iterable_adaptor<detail::intersection_iterable_impl<Iterables...>>::valid(const mpl::sequence<Indexs...>&) const
{
    return (m_adaptors.template get<Indexs>().valid() && ...);
}
template<typename ... Iterables>
template<size_t ... Indexs,typename ActionTag>
bool iterable_adaptor<detail::intersection_iterable_impl<Iterables...>>::perform_action(const mpl::sequence<Indexs...>&,const ActionTag& i_actionTag)
{
    return (m_adaptors.template get<Indexs>().perform_action(i_actionTag) && ...);
}
template<typename ... Iterables>
template<size_t ... Indexs,typename ActionTag>
bool iterable_adaptor<detail::intersection_iterable_impl<Iterables...>>::perform_action(const mpl::sequence<Indexs...>&,const ActionTag& i_actionTag) const
{
    return (m_adaptors.template get<Indexs>().perform_action(i_actionTag) && ...);
}

template<typename ... Iterables>
iterable_adaptor<const detail::intersection_iterable_impl<Iterables...>>::iterable_adaptor(const Iterables& ... i_iterables)
: m_adaptors(deduce_adaptor(i_iterables)...)
{
}
template<typename ... Iterables>
auto iterable_adaptor<const detail::intersection_iterable_impl<Iterables...>>::get_value() const
{
    typedef typename mpl::make_sequence<0,s_numTypes>::type seq_type;

    return get_value(seq_type{});
}
template<typename ... Iterables>
template<typename Sink>
auto iterable_adaptor<const detail::intersection_iterable_impl<Iterables...>>::forward_value(Sink&& i_sink) const
{
    typedef typename mpl::make_sequence<0,s_numTypes>::type seq_type;

    return forward_value(std::forward<Sink>(i_sink),seq_type{});
}
template<typename ... Iterables>
bool iterable_adaptor<const detail::intersection_iterable_impl<Iterables...>>::valid() const
{
    typedef typename mpl::make_sequence<0,s_numTypes>::type seq_type;

    return valid(seq_type{});
}
template<typename ... Iterables>
TEMPLATE(typename ActionTag)
REQUIRED(ACTION_TAGS_SUPPORTED(traits,ActionTag))
bool iterable_adaptor<const detail::intersection_iterable_impl<Iterables...>>::perform_action(const ActionTag& i_actionTag)
{
    typedef typename mpl::make_sequence<0,s_numTypes>::type seq_type;

    return perform_action(seq_type{},i_actionTag);
}
template<typename ... Iterables>
TEMPLATE(typename ActionTag)
REQUIRED(ACTION_TAGS_SUPPORTED(traits,ActionTag))
bool iterable_adaptor<const detail::intersection_iterable_impl<Iterables...>>::perform_action(const ActionTag& i_actionTag) const
{
    typedef typename mpl::make_sequence<0,s_numTypes>::type seq_type;

    return perform_action(seq_type{},i_actionTag);
}
template<typename ... Iterables>
template<size_t ... Indexs>
typename iterable_adaptor<const detail::intersection_iterable_impl<Iterables...>>::const_reference iterable_adaptor<const detail::intersection_iterable_impl<Iterables...>>::get_value(const mpl::sequence<Indexs...>&) const
{
    return { m_adaptors.template get<Indexs>().get_value() ... };
}
template<typename ... Iterables>
template<size_t ... Indexs, typename Sink>
auto iterable_adaptor<const detail::intersection_iterable_impl<Iterables...>>::forward_value(const mpl::sequence<Indexs...>& , Sink&& i_sink) const
{
    return ddk::eval(std::forward<Sink>(i_sink),m_adaptors.template get<Indexs>().get_value() ...);
}
template<typename ... Iterables>
template<size_t ... Indexs>
bool iterable_adaptor<const detail::intersection_iterable_impl<Iterables...>>::valid(const mpl::sequence<Indexs...>&) const
{
    return (m_adaptors.template get<Indexs>().valid() && ...);
}
template<typename ... Iterables>
template<size_t ... Indexs,typename ActionTag>
bool iterable_adaptor<const detail::intersection_iterable_impl<Iterables...>>::perform_action(const mpl::sequence<Indexs...>&,const ActionTag& i_actionTag)
{
    return (m_adaptors.template get<Indexs>().perform_action(i_actionTag) && ...);
}
template<typename ... Iterables>
template<size_t ... Indexs,typename ActionTag>
bool iterable_adaptor<const detail::intersection_iterable_impl<Iterables...>>::perform_action(const mpl::sequence<Indexs...>&,const ActionTag& i_actionTag) const
{
    return (m_adaptors.template get<Indexs>().perform_action(i_actionTag) && ...);
}

}