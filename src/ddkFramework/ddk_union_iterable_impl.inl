
#include "ddk_async_exceptions.h"
#include "ddk_fiber_utils.h"

namespace ddk
{
namespace detail
{

template<typename ... Iterables>
TEMPLATE(typename ... IIterables)
REQUIRED(IS_CONSTRUCTIBLE(Iterables,IIterables)...)
union_iterable_impl<Iterables...>::union_iterable_impl(IIterables&& ... i_iterables)
: iterable_visitor<detail::union_iterable_impl<Iterables...>>(i_iterables...)
{
}
template<typename ... Iterables>
TEMPLATE(typename Action)
REQUIRED(ACTION_SUPPORTED(traits,Action))
constexpr void union_iterable_impl<Iterables...>::iterate_impl(Action&& i_initialAction)
{
    this->loop(std::forward<Action>(i_initialAction));
}
template<typename ... Iterables>
TEMPLATE(typename Action)
REQUIRED(ACTION_SUPPORTED(traits,Action))
constexpr void union_iterable_impl<Iterables...>::iterate_impl(Action&& i_initialAction) const
{
    this->loop(std::forward<Action>(i_initialAction));
}

template<typename ... Iterables>
union_iterable_adaptor<Iterables...>::union_iterable_adaptor(Iterables& ... i_iterables)
: m_adaptors(deduce_adaptor(i_iterables)...)
{
}
template<typename ... Iterables>
constexpr bool union_iterable_adaptor<Iterables...>::set_current_iterable_index(size_t i_index) const
{
    if (i_index < s_numTypes)
    {
        m_currIndex = i_index;

        return true;
    }
    else
    {
        return false;
    }
}
template<typename ... Iterables>
constexpr size_t union_iterable_adaptor<Iterables...>::get_current_iterable_index() const
{
    return m_currIndex;
}
template<typename ... Iterables>
template<size_t Index>
constexpr auto union_iterable_adaptor<Iterables...>::get_adaptor()
{
    return m_adaptors.template get<Index>();
}
template<typename ... Iterables>
template<size_t Index>
constexpr auto union_iterable_adaptor<Iterables...>::get_adaptor() const
{
    return m_adaptors.template get<Index>();
}
template<typename ... Iterables>
TEMPLATE(typename Adaptor, typename ActionTag)
REQUIRED(ACTION_TAGS_SUPPORTED(Adaptor,ActionTag))
constexpr auto union_iterable_adaptor<Iterables...>::perform_action(Adaptor&& i_adaptor, ActionTag&& i_actionTag)
{
    typedef typename mpl::make_sequence<0,s_numTypes>::type seq_type;

    return perform_action(seq_type{},std::forward<Adaptor>(i_adaptor),std::forward<ActionTag>(i_actionTag));
}
template<typename ... Iterables>
template<size_t ... Indexs,typename Adaptor,typename ActionTag>
constexpr auto union_iterable_adaptor<Iterables...>::perform_action(const mpl::sequence<Indexs...>&, Adaptor&& i_adaptor,ActionTag&& i_actionTag)
{
    typedef iterable_action_tag_result<traits,ActionTag>(*funcType)(deduced_adaptors&,ActionTag&&);
    typedef typename mpl::which_type<mpl::is_const<Adaptor>,const deduced_adaptors,deduced_adaptors>::type deduced_adaptor_t;

    const funcType s_navFuncs[] = { &union_iterable_adaptor<Iterables...>::_perform_action<Indexs,traits,deduced_adaptor_t> ... };

    return (*s_navFuncs[i_adaptor.m_currIndex])(std::forward<Adaptor>(i_adaptor).m_adaptors,std::forward<ActionTag>(i_actionTag));
}
template<typename ... Iterables>
template<size_t Index,typename Traits,typename DeducedAdaptors,typename ActionTag>
constexpr iterable_action_tag_result<Traits,ActionTag> union_iterable_adaptor<Iterables...>::_perform_action(DeducedAdaptors& i_adaptors,ActionTag&& i_actionTag)
{
    return i_adaptors.template get<Index>().perform_action(i_adaptors.template get<Index>(),std::forward<ActionTag>(i_actionTag));
}

template<typename ... Iterables>
union_iterable_const_adaptor<Iterables...>::union_iterable_const_adaptor(const Iterables& ... i_iterables)
: m_adaptors(deduce_adaptor(i_iterables)...)
{
}
template<typename ... Iterables>
constexpr bool union_iterable_const_adaptor<Iterables...>::set_current_iterable_index(size_t i_index) const
{
    if (i_index < s_numTypes)
    {
        m_currIndex = i_index;

        return true;
    }
    else
    {
        return false;
    }
}
template<typename ... Iterables>
constexpr size_t union_iterable_const_adaptor<Iterables...>::get_current_iterable_index() const
{
    return m_currIndex;
}
template<typename ... Iterables>
template<size_t Index>
constexpr auto union_iterable_const_adaptor<Iterables...>::get_adaptor() const
{
    return m_adaptors.template get<Index>();
}
template<typename ... Iterables>
TEMPLATE(typename ActionTag)
REQUIRED(ACTION_TAGS_SUPPORTED(const_traits,ActionTag))
constexpr auto union_iterable_const_adaptor<Iterables...>::perform_action(ActionTag&& i_actionTag) const
{
    typedef typename mpl::make_sequence<0,s_numTypes>::type seq_type;

    return perform_action(seq_type{},std::forward<ActionTag>(i_actionTag));
}
template<typename ... Iterables>
template<size_t ... Indexs,typename ActionTag>
constexpr auto union_iterable_const_adaptor<Iterables...>::perform_action(const mpl::sequence<Indexs...>&,ActionTag&& i_actionTag) const
{
    typedef iterable_action_tag_result<traits,ActionTag>(*funcType)(const deduced_adaptors&,ActionTag&&);
    const funcType s_navFuncs[] = { &union_iterable_const_adaptor<Iterables...>::_perform_action<Indexs,traits> ... };

    return (*s_navFuncs[m_currIndex])(m_adaptors,std::forward<ActionTag>(i_actionTag));
}
template<typename ... Iterables>
template<size_t Index,typename Traits,typename ActionTag>
constexpr iterable_action_tag_result<const_iterable_traits<Traits>,ActionTag> union_iterable_const_adaptor<Iterables...>::_perform_action(const deduced_adaptors& i_adaptors,ActionTag&& i_actionTag)
{
    return i_adaptors.template get<Index>().perform_action(std::forward<ActionTag>(i_actionTag));
}

}

template<typename ... Iterables>
TEMPLATE(typename Adaptor, typename ActionTag)
REQUIRED(ACTION_TAGS_SUPPORTED(Adaptor,ActionTag))
constexpr auto iterable_adaptor<detail::union_iterable_impl<Iterables...>>::perform_action(Adaptor&& i_adaptor, ActionTag&& i_actionTag)
{
    if (auto actionRes = perform_action(std::forward<Adaptor>(i_adaptor),union_iterable_action{ std::forward<ActionTag>(i_actionTag) }))
    {
        return make_result<iterable_action_tag_result<detail::adaptor_traits<Adaptor>,ActionTag>>(actionRes);
    }
    else
    {
        auto actionError = std::move(actionRes).error();

        if (auto recoveryRes = perform_action(std::forward<Adaptor>(i_adaptor),union_iterable_action{ std::move(actionError).recovery() }))
        {
            return make_result<iterable_action_tag_result<detail::adaptor_traits<Adaptor>,ActionTag>>(recoveryRes);
        }
        else
        {
            return make_error<iterable_action_tag_result<detail::adaptor_traits<Adaptor>,ActionTag>>(std::move(recoveryRes).error());
        }
    }
}
template<typename ... Iterables>
template<typename Adaptor, typename ActionTag>
constexpr auto iterable_adaptor<detail::union_iterable_impl<Iterables...>>::perform_action(Adaptor&& i_adaptor, union_iterable_action<ActionTag> i_actionTag)
{
    typedef iterable_adaptor<detail::union_iterable_impl<Iterables...>> adaptor_t;
    typedef union_iterable_action_result<adaptor_t,ActionTag> union_action_result;

    if (auto actionRes = i_actionTag.apply(std::forward<Adaptor>(i_adaptor)))
    {
        return make_result<union_action_result>(actionRes);
    }
    else
    {
        return make_error<union_action_result>(std::move(actionRes).error());
    }
}

template<typename ... Iterables>
TEMPLATE(typename Adaptor, typename ActionTag)
REQUIRED(ACTION_TAGS_SUPPORTED(Adaptor,ActionTag))
constexpr auto iterable_adaptor<const detail::union_iterable_impl<Iterables...>>::perform_action(Adaptor&& i_adaptor, ActionTag&& i_actionTag)
{
    if (auto actionRes = perform_action(std::forward<Adaptor>(i_adaptor),union_iterable_action{ std::forward<ActionTag>(i_actionTag) }))
    {
        return make_result<iterable_action_tag_result<detail::adaptor_traits<Adaptor>,ActionTag>>(actionRes);
    }
    else
    {
        auto actionError = std::move(actionRes).error();

        if (auto recoveryRes = perform_action(std::forward<Adaptor>(i_adaptor),union_iterable_action{ std::move(actionError).recovery() }))
        {
            return make_result<iterable_action_tag_result<detail::adaptor_traits<Adaptor>,ActionTag>>(recoveryRes);
        }
        else
        {
            return make_error<iterable_action_tag_result<detail::adaptor_traits<Adaptor>,ActionTag>>(std::move(recoveryRes).error());
        }
    }
}
template<typename ... Iterables>
template<typename Adaptor, typename ActionTag>
constexpr auto iterable_adaptor<const detail::union_iterable_impl<Iterables...>>::perform_action(Adaptor&& i_adaptor, union_iterable_action<ActionTag> i_actionTag)
{
    typedef iterable_adaptor<const detail::union_iterable_impl<Iterables...>> adaptor_t;
    typedef union_iterable_action_result<adaptor_t,ActionTag> union_action_result;

    if (auto actionRes = i_actionTag.apply(std::forward<Adaptor>(i_adaptor)))
    {
        return make_result<union_action_result>(actionRes);
    }
    else
    {
        return make_error<union_action_result>(std::move(actionRes).error());
    }
}

}