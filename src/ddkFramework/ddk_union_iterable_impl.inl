
#include "ddk_async_exceptions.h"
#include "ddk_fiber_utils.h"

namespace ddk
{
namespace detail
{

template<typename ... Iterables>
union_iterable_impl<Iterables...>::union_iterable_impl(Iterables& ... i_iterables)
: iterable_visitor<detail::union_iterable_impl<Iterables...>>(i_iterables...)
{
}
template<typename ... Iterables>
TEMPLATE(typename Function,typename Action)
REQUIRED(IS_CALLABLE_BY(Function,reference))
iterable_result union_iterable_impl<Iterables...>::iterate_impl(Function&& i_try,Action&& i_initialAction)
{
    loop(std::forward<Function>(i_try),std::forward<Action>(i_initialAction));

    return success;
}
template<typename ... Iterables>
TEMPLATE(typename Function,typename Action)
REQUIRED(IS_CALLABLE_BY(Function,const_reference))
iterable_result union_iterable_impl<Iterables...>::iterate_impl(Function&& i_try,Action&& i_initialAction) const
{
    loop(std::forward<Function>(i_try),std::forward<Action>(i_initialAction));

    return success;
}

}

template<typename ... Iterables>
iterable_adaptor<detail::union_iterable_impl<Iterables...>>::iterable_adaptor(Iterables& ... i_iterables)
: m_adaptors(deduce_adaptor(i_iterables)...)
{
}
template<typename ... Iterables>
auto iterable_adaptor<detail::union_iterable_impl<Iterables...>>::get_value()
{
    typedef typename mpl::make_sequence<0,s_numTypes>::type seq_type;

    return get_value(seq_type{});
}
template<typename ... Iterables>
auto iterable_adaptor<detail::union_iterable_impl<Iterables...>>::get_value() const
{
    typedef typename mpl::make_sequence<0,s_numTypes>::type seq_type;

    return get_value(seq_type{});
}
template<typename ... Iterables>
template<typename Sink>
auto iterable_adaptor<detail::union_iterable_impl<Iterables...>>::forward_value(Sink&& i_sink)
{
    typedef typename mpl::make_sequence<0,s_numTypes>::type seq_type;

    return forward_value(seq_type{},std::forward<Sink>(i_sink));
}
template<typename ... Iterables>
template<typename Sink>
auto iterable_adaptor<detail::union_iterable_impl<Iterables...>>::forward_value(Sink&& i_sink) const
{
    typedef typename mpl::make_sequence<0,s_numTypes>::type seq_type;

    return forward_value(seq_type{},std::forward<Sink>(i_sink));
}
template<typename ... Iterables>
TEMPLATE(typename ActionTag)
REQUIRED(ACTION_TAGS_SUPPORTED(traits,ActionTag))
iterable_action_result<ActionTag> iterable_adaptor<detail::union_iterable_impl<Iterables...>>::perform_action(ActionTag&& i_actionTag)
{
    typedef typename mpl::make_sequence<0,s_numTypes>::type seq_type;

    if (auto actionRes = perform_action(seq_type{},std::forward<ActionTag>(i_actionTag)))
    {
        return success;
    }
    else
    {
        while(actionRes.recoverable() && ++m_currIndex < s_numTypes)
        {
            perform_default_action(seq_type{});

            if (actionRes = perform_action(seq_type{},actionRes.recovery()))
            {
                return success;
            }
        }

        return actionRes;
    }
}
template<typename ... Iterables>
TEMPLATE(typename ActionTag)
REQUIRED(ACTION_TAGS_SUPPORTED(traits,ActionTag))
iterable_action_result<ActionTag> iterable_adaptor<detail::union_iterable_impl<Iterables...>>::perform_action(ActionTag&& i_actionTag) const
{
    typedef typename mpl::make_sequence<0,s_numTypes>::type seq_type;

    if (auto actionRes = perform_action(seq_type{},std::forward<ActionTag>(i_actionTag)))
    {
        return success;
    }
    else
    {
        while (++m_currIndex < s_numTypes && actionRes.recoverable())
        {
            perform_default_action(seq_type{});

            if (actionRes = perform_action(seq_type{},actionRes.recovery()))
            {
                return success;
            }
        }

        return actionRes;
    }
}
template<typename ... Iterables>
template<size_t ... Indexs>
auto iterable_adaptor<detail::union_iterable_impl<Iterables...>>::get_value(const mpl::sequence<Indexs...>&)
{
    typedef reference(*funcType)(tuple<deduced_adaptor<Iterables>...>&);
    static const funcType s_navFuncs[] = { &iterable_adaptor<detail::union_iterable_impl<Iterables...>>::_get_value<Indexs> ... };

    return (*s_navFuncs[m_currIndex])(m_adaptors);
}
template<typename ... Iterables>
template<size_t Index>
typename iterable_adaptor<detail::union_iterable_impl<Iterables...>>::reference iterable_adaptor<detail::union_iterable_impl<Iterables...>>::_get_value(tuple<deduced_adaptor<Iterables>...>& i_adaptors)
{
    return i_adaptors.template get<Index>().get_value();
}
template<typename ... Iterables>
template<size_t ... Indexs>
auto iterable_adaptor<detail::union_iterable_impl<Iterables...>>::get_value(const mpl::sequence<Indexs...>&) const
{
    typedef const_reference(*funcType)(const tuple<deduced_adaptor<Iterables>...>&);
    static const funcType s_navFuncs[] = { &iterable_adaptor<detail::union_iterable_impl<Iterables...>>::_get_value<Indexs> ... };

    return (*s_navFuncs[m_currIndex])(m_adaptors);
}
template<typename ... Iterables>
template<size_t Index>
typename iterable_adaptor<detail::union_iterable_impl<Iterables...>>::const_reference iterable_adaptor<detail::union_iterable_impl<Iterables...>>::_get_value(const tuple<deduced_adaptor<Iterables>...>& i_adaptors)
{
    return i_adaptors.template get<Index>().get_value();
}
template<typename ... Iterables>
template<size_t ... Indexs,typename Sink>
auto iterable_adaptor<detail::union_iterable_impl<Iterables...>>::forward_value(const mpl::sequence<Indexs...>&,Sink&& i_sink)
{
    typedef typename mpl::aqcuire_callable_return_type<Sink>::type return_type;
    typedef return_type(*funcType)(tuple<deduced_adaptor<Iterables>...>&,Sink&&);
    static const funcType s_navFuncs[] = { &iterable_adaptor<detail::union_iterable_impl<Iterables...>>::_forward_value<Indexs,Sink&&> ... };

    return (*s_navFuncs[m_currIndex])(m_adaptors,std::forward<Sink>(i_sink));
}
template<typename ... Iterables>
template<size_t Index,typename Sink>
auto iterable_adaptor<detail::union_iterable_impl<Iterables...>>::_forward_value(tuple<deduced_adaptor<Iterables>...>& i_adaptors, Sink&& i_sink)
{
    return i_adaptors.template get<Index>().forward_value(std::forward<Sink>(i_sink));
}
template<typename ... Iterables>
template<size_t ... Indexs,typename Sink>
auto iterable_adaptor<detail::union_iterable_impl<Iterables...>>::forward_value(const mpl::sequence<Indexs...>& ,Sink&& i_sink) const
{
    typedef typename mpl::aqcuire_callable_return_type<Sink>::type return_type;
    typedef return_type(*funcType)(const tuple<deduced_adaptor<Iterables>...>&,Sink&&);
    static const funcType s_navFuncs[] = { &iterable_adaptor<detail::union_iterable_impl<Iterables...>>::_forward_value<Indexs,Sink&&> ... };

    return (*s_navFuncs[m_currIndex])(m_adaptors,std::forward<Sink>(i_sink));
}
template<typename ... Iterables>
template<size_t Index,typename Sink>
auto iterable_adaptor<detail::union_iterable_impl<Iterables...>>::_forward_value(const tuple<deduced_adaptor<Iterables>...>& i_adaptors, Sink&& i_sink)
{
    return i_adaptors.template get<Index>().forward_value(std::forward<Sink>(i_sink));
}
template<typename ... Iterables>
template<size_t ... Indexs,typename ActionTag>
iterable_action_result<ActionTag> iterable_adaptor<detail::union_iterable_impl<Iterables...>>::perform_action(const mpl::sequence<Indexs...>&,ActionTag&& i_actionTag)
{
    typedef iterable_action_result<ActionTag>(*funcType)(tuple<deduced_adaptor<Iterables>...>&,ActionTag&&);
    static const funcType s_navFuncs[] = { &iterable_adaptor<detail::union_iterable_impl<Iterables...>>::_perform_action<Indexs> ... };

    return (*s_navFuncs[m_currIndex])(m_adaptors,std::forward<ActionTag>(i_actionTag));
}
template<typename ... Iterables>
template<size_t Index,typename ActionTag>
iterable_action_result<ActionTag> iterable_adaptor<detail::union_iterable_impl<Iterables...>>::_perform_action(tuple<deduced_adaptor<Iterables>...>& i_adaptors,ActionTag&& i_actionTag)
{
    return i_adaptors.template get<Index>().perform_action(std::forward<ActionTag>(i_actionTag));
}
template<typename ... Iterables>
template<size_t ... Indexs,typename ActionTag>
iterable_action_result<ActionTag> iterable_adaptor<detail::union_iterable_impl<Iterables...>>::perform_action(const mpl::sequence<Indexs...>&,ActionTag&& i_actionTag) const
{
    typedef iterable_action_result<ActionTag>(*funcType)(const tuple<deduced_adaptor<Iterables>...>&,ActionTag&&);
    static const funcType s_navFuncs[] = { &iterable_adaptor<detail::union_iterable_impl<Iterables...>>::_perform_action<Indexs> ... };

    return (*s_navFuncs[m_currIndex])(m_adaptors,std::forward<ActionTag>(i_actionTag));
}
template<typename ... Iterables>
template<size_t Index,typename ActionTag>
iterable_action_result<ActionTag> iterable_adaptor<detail::union_iterable_impl<Iterables...>>::_perform_action(const tuple<deduced_adaptor<Iterables>...>& i_adaptor,ActionTag&& i_actionTag)
{
    return i_adaptors.template get<Index>().perform_action(std::forward<ActionTag>(i_actionTag));
}
template<typename ... Iterables>
template<size_t ... Indexs>
bool iterable_adaptor<detail::union_iterable_impl<Iterables...>>::perform_default_action(const mpl::sequence<Indexs...>&)
{
    typedef bool(*funcType)(tuple<deduced_adaptor<Iterables>...>&);
    static const funcType s_navFuncs[] = { &iterable_adaptor<detail::union_iterable_impl<Iterables...>>::_perform_default_action<Indexs> ... };

    return (*s_navFuncs[m_currIndex])(m_adaptors);
}
template<typename ... Iterables>
template<size_t Index>
bool iterable_adaptor<detail::union_iterable_impl<Iterables...>>::_perform_default_action(tuple<deduced_adaptor<Iterables>...>& i_adaptor)
{
    typedef mpl::nth_type_of_t<Index,Iterables...> nth_iterable;

    const auto defaultAction = iterable_default_action<nth_iterable>::default_action();

    return static_cast<bool>(defaultAction.apply(i_adaptor.template get<Index>(),[&defaultAction](auto&&) -> no_action { return{}; }));
}
template<typename ... Iterables>
template<size_t ... Indexs>
bool iterable_adaptor<detail::union_iterable_impl<Iterables...>>::perform_default_action(const mpl::sequence<Indexs...>&) const
{
    typedef bool(*funcType)(const tuple<deduced_adaptor<Iterables>...>&);
    static const funcType s_navFuncs[] = { &iterable_adaptor<detail::union_iterable_impl<Iterables...>>::_perform_default_action<Indexs> ... };

    return (*s_navFuncs[m_currIndex])(m_adaptors);
}
template<typename ... Iterables>
template<size_t Index>
bool iterable_adaptor<detail::union_iterable_impl<Iterables...>>::_perform_default_action(const tuple<deduced_adaptor<Iterables>...>& i_adaptor)
{
    typedef mpl::nth_type_of_t<Index,Iterables...> nth_iterable;

    const auto defaultAction = iterable_default_action<nth_iterable>::default_action();

    return static_cast<bool>(defaultAction.apply(i_adaptor.template get<Index>(),[&defaultAction](auto&&) -> no_action { return{}; }));
}

template<typename ... Iterables>
iterable_adaptor<const detail::union_iterable_impl<Iterables...>>::iterable_adaptor(const Iterables& ... i_iterables)
: m_adaptors(deduce_adaptor(i_iterables)...)
{
}
template<typename ... Iterables>
auto iterable_adaptor<const detail::union_iterable_impl<Iterables...>>::get_value() const
{
    typedef typename mpl::make_sequence<0,mpl::num_types<Iterables...>>::type seq_type;

    return get_value(seq_type{});
}
template<typename ... Iterables>
template<typename Sink>
auto iterable_adaptor<const detail::union_iterable_impl<Iterables...>>::forward_value(Sink&& i_sink) const
{
    typedef typename mpl::make_sequence<0,mpl::num_types<Iterables...>>::type seq_type;

    return forward_value(std::forward<Sink>(i_sink),seq_type{});
}
template<typename ... Iterables>
TEMPLATE(typename ActionTag)
REQUIRED(ACTION_TAGS_SUPPORTED(traits,ActionTag))
iterable_action_result<ActionTag> iterable_adaptor<const detail::union_iterable_impl<Iterables...>>::perform_action(ActionTag&& i_actionTag)
{
    typedef typename mpl::make_sequence<0,s_numTypes>::type seq_type;

    if (perform_action(seq_type{},std::forward<ActionTag>(i_actionTag)))
    {
        return true;
    }
    else if (m_currIndex < s_numTypes)
    {
        m_currIndex++;

        return perform_default_action(seq_type{});
    }
    else
    {
        return false;
    }
}
template<typename ... Iterables>
TEMPLATE(typename ActionTag)
REQUIRED(ACTION_TAGS_SUPPORTED(traits,ActionTag))
iterable_action_result<ActionTag> iterable_adaptor<const detail::union_iterable_impl<Iterables...>>::perform_action(ActionTag&& i_actionTag) const
{
    typedef typename mpl::make_sequence<0,s_numTypes>::type seq_type;

    if (const auto performRes = perform_action(seq_type{},std::forward<ActionTag>(i_actionTag)))
    {
        return performRes;
    }
    else if (m_currIndex < s_numTypes)
    {
        m_currIndex++;

        return perform_default_action(seq_type{});
    }
    else
    {
        return false;
    }
}
template<typename ... Iterables>
template<size_t ... Indexs>
auto iterable_adaptor<const detail::union_iterable_impl<Iterables...>>::get_value(const mpl::sequence<Indexs...>&) const
{
    typedef const_reference(*funcType)(const tuple<deduced_adaptor<Iterables>...>&);
    static const funcType s_navFuncs[] = { &iterable_adaptor<detail::union_iterable_impl<Iterables...>>::_get_value<Indexs> ... };

    return (*s_navFuncs[m_currIndex])(m_adaptors);
}
template<typename ... Iterables>
template<size_t Index>
typename iterable_adaptor<const detail::union_iterable_impl<Iterables...>>::const_reference iterable_adaptor<const detail::union_iterable_impl<Iterables...>>::_get_value(const tuple<deduced_adaptor<Iterables>...>& i_adaptors)
{
    return i_adaptors.template get<Index>().get_value();
}
template<typename ... Iterables>
template<typename Sink,size_t ... Indexs>
auto iterable_adaptor<const detail::union_iterable_impl<Iterables...>>::forward_value(Sink&& i_sink,const mpl::sequence<Indexs...>&) const
{
    typedef typename mpl::aqcuire_callable_return_type<Sink>::type return_type;
    typedef return_type(*funcType)(const tuple<deduced_adaptor<Iterables>...>&,Sink&&);
    static const funcType s_navFuncs[] = { &iterable_adaptor<detail::union_iterable_impl<Iterables...>>::_forward_value<Indexs,Sink&&> ... };

    return (*s_navFuncs[m_currIndex])(m_adaptors,std::forward<Sink>(i_sink));
}
template<typename ... Iterables>
template<size_t Index,typename Sink>
auto iterable_adaptor<const detail::union_iterable_impl<Iterables...>>::_forward_value(const tuple<deduced_adaptor<Iterables>...>& i_adaptors,Sink&& i_sink)
{
    return i_adaptors.template get<Index>().forward_value(std::forward<Sink>(i_sink));
}
template<typename ... Iterables>
template<size_t ... Indexs,typename ActionTag>
iterable_action_result<ActionTag> iterable_adaptor<const detail::union_iterable_impl<Iterables...>>::perform_action(const mpl::sequence<Indexs...>&,ActionTag&& i_actionTag)
{
    typedef iterable_action_result<ActionTag>(*funcType)(tuple<deduced_adaptor<Iterables>...>&,ActionTag&&);
    static const funcType s_navFuncs[] = { &iterable_adaptor<detail::union_iterable_impl<Iterables...>>::_perform_action<Indexs> ... };

    return (*s_navFuncs[m_currIndex])(m_adaptors,std::forward<ActionTag>(i_actionTag));
}
template<typename ... Iterables>
template<size_t Index,typename ActionTag>
iterable_action_result<ActionTag> iterable_adaptor<const detail::union_iterable_impl<Iterables...>>::_perform_action(tuple<deduced_adaptor<Iterables>...>& i_adaptor,ActionTag&& i_actionTag)
{
    return i_adaptors.template get<Index>().perform_action(std::forward<ActionTag>(i_actionTag));
}
template<typename ... Iterables>
template<size_t ... Indexs,typename ActionTag>
iterable_action_result<ActionTag> iterable_adaptor<const detail::union_iterable_impl<Iterables...>>::perform_action(const mpl::sequence<Indexs...>&,ActionTag&& i_actionTag) const
{
    typedef iterable_action_result<ActionTag>(*funcType)(const tuple<deduced_adaptor<Iterables>...>&,ActionTag&&);
    static const funcType s_navFuncs[] = { &iterable_adaptor<detail::union_iterable_impl<Iterables...>>::_perform_action<Indexs> ... };

    return (*s_navFuncs[m_currIndex])(m_adaptors,std::forward<ActionTag>(i_actionTag));
}
template<typename ... Iterables>
template<size_t Index,typename ActionTag>
iterable_action_result<ActionTag> iterable_adaptor<const detail::union_iterable_impl<Iterables...>>::_perform_action(const tuple<deduced_adaptor<Iterables>...>& i_adaptor,ActionTag&& i_actionTag)
{
    return i_adaptors.template get<Index>().perform_action(std::forward<ActionTag>(i_actionTag));
}
template<typename ... Iterables>
template<size_t ... Indexs>
bool iterable_adaptor<const detail::union_iterable_impl<Iterables...>>::perform_default_action(const mpl::sequence<Indexs...>&)
{
    typedef bool(*funcType)(iterable_adaptor<const detail::union_iterable_impl<Iterables...>>&);
    static const funcType s_navFuncs[] = { &iterable_adaptor<detail::union_iterable_impl<Iterables...>>::_perform_default_action<Indexs> ... };

    return (*s_navFuncs[m_currIndex])(*this);
}
template<typename ... Iterables>
template<size_t Index>
bool iterable_adaptor<const detail::union_iterable_impl<Iterables...>>::_perform_default_action(tuple<deduced_adaptor<Iterables>...>& i_adaptor)
{
    typedef mpl::nth_type_of_t<Index,Iterables...> nth_iterable;

    const auto defaultAction = iterable_default_action<nth_iterable>::default_action();

    return static_cast<bool>(defaultAction.apply(i_adaptor,[&defaultAction](auto&&) -> no_action { return {}; }));
}
template<typename ... Iterables>
template<size_t ... Indexs>
bool iterable_adaptor<const detail::union_iterable_impl<Iterables...>>::perform_default_action(const mpl::sequence<Indexs...>&) const
{
    typedef bool(*funcType)(iterable_adaptor<const detail::union_iterable_impl<Iterables...>>&);
    static const funcType s_navFuncs[] = { &iterable_adaptor<detail::union_iterable_impl<Iterables...>>::_perform_default_action<Indexs> ... };

    return (*s_navFuncs[m_currIndex])(*this);
}
template<typename ... Iterables>
template<size_t Index>
bool iterable_adaptor<const detail::union_iterable_impl<Iterables...>>::_perform_default_action(const tuple<deduced_adaptor<Iterables>...>& i_adaptor)
{
    typedef mpl::nth_type_of_t<Index,Iterables...> nth_iterable;

    const auto defaultAction = iterable_default_action<nth_iterable>::default_action();

    return static_cast<bool>(defaultAction.apply(i_adaptor,[&defaultAction](auto&&) -> no_action { return {}; }));
}

}