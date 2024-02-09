
#include "ddk_filtered_iterable_action.h"

namespace ddk
{
namespace detail
{

template<typename Function>
iterable_constrain<Function>::iterable_constrain(const Function& i_constrain)
: m_constrain(i_constrain)
{
}
template<typename Function>
Function iterable_constrain<Function>::get_constrain() const
{
	return m_constrain;
}

template<typename Iterable,typename Constrain>
TEMPLATE(typename IIterable,typename CConstrain)
REQUIRED(IS_CONSTRUCTIBLE(Iterable,IIterable),IS_CONSTRUCTIBLE(Constrain,CConstrain))
constrained_iterable_impl<Iterable,Constrain>::constrained_iterable_impl(IIterable&& i_iterable,CConstrain&& i_constrain)
: base_t(std::forward<IIterable>(i_iterable),std::forward<CConstrain>(i_constrain))
{
}
template<typename Iterable,typename Constrain>
TEMPLATE(typename Action)
REQUIRED(ACTION_SUPPORTED(traits,Action))
iterable_result constrained_iterable_impl<Iterable,Constrain>::iterate_impl(Action&& i_initialAction)
{
	return loop(std::forward<Action>(i_initialAction));
}
template<typename Iterable,typename Constrain>
TEMPLATE(typename Action)
REQUIRED(ACTION_SUPPORTED(const_traits,Action))
iterable_result constrained_iterable_impl<Iterable,Constrain>::iterate_impl(Action&& i_initialAction) const
{
	return loop(std::forward<Action>(i_initialAction));
}

}

template<typename Iterable,typename Constrain>
iterable_adaptor<detail::constrained_iterable_impl<Iterable,Constrain>>::iterable_adaptor(Iterable& i_iterable,const Constrain& i_constrain)
: m_adaptor(deduce_adaptor(i_iterable))
, m_constrain(i_constrain)
{
}
template<typename Iterable,typename Constrain>
TEMPLATE(typename ActionTag)
REQUIRED(ACTION_TAGS_SUPPORTED(traits,ActionTag))
auto iterable_adaptor<detail::constrained_iterable_impl<Iterable,Constrain>>::perform_action(ActionTag&& i_actionTag)
{
    filtered_iterable_action filteredAction{ std::forward<ActionTag>(i_actionTag),m_constrain };

    if (auto actionRes = filteredAction.apply(m_adaptor))
    {
        return make_result<iterable_action_tag_result<traits,ActionTag>>(actionRes);
    }
    else
    {
        return make_error<iterable_action_tag_result<traits,ActionTag>>(std::move(actionRes).error());
    }
}
template<typename Iterable,typename Constrain>
TEMPLATE(typename ActionTag)
REQUIRED(ACTION_TAGS_SUPPORTED(const_traits,ActionTag))
auto iterable_adaptor<detail::constrained_iterable_impl<Iterable,Constrain>>::perform_action(ActionTag&& i_actionTag) const
{
    typedef iterable_adaptor<detail::constrained_iterable_impl<Iterable,Constrain>> adaptor_t;
    typedef filtered_iterable_action_result<const adaptor_t,ActionTag,Constrain> filtered_action_result;

    filtered_iterable_action filteredAction{ std::forward<ActionTag>(i_actionTag),m_constrain };

    if (filtered_action_result applyRes = filteredAction.apply(m_adaptor))
    {
        return make_result<iterable_action_tag_result<const_traits,ActionTag>>(applyRes);
    }
    else
    {
        return make_error<iterable_action_tag_result<const_traits,ActionTag>>(std::move(applyRes).error());
    }
}

template<typename Iterable,typename Constrain>
iterable_adaptor<const detail::constrained_iterable_impl<Iterable,Constrain>>::iterable_adaptor(const Iterable& i_iterable,const Constrain& i_constrain)
: m_adaptor(deduce_adaptor(i_iterable))
, m_constrain(i_constrain)
{
}
template<typename Iterable,typename Constrain>
TEMPLATE(typename ActionTag)
REQUIRED(ACTION_TAGS_SUPPORTED(const_traits,ActionTag))
auto iterable_adaptor<const detail::constrained_iterable_impl<Iterable,Constrain>>::perform_action(ActionTag&& i_actionTag) const
{
    typedef iterable_adaptor<detail::constrained_iterable_impl<Iterable,Constrain>> adaptor_t;
    typedef filtered_iterable_action_result<const adaptor_t,ActionTag,Constrain> filtered_action_result;

    filtered_iterable_action filteredAction{ std::forward<ActionTag>(i_actionTag),m_constrain };

    if (filtered_action_result applyRes = filteredAction.apply(m_adaptor))
    {
        return make_result<iterable_action_tag_result<const_traits,ActionTag>>(success);
    }
    else
    {
        return make_error<iterable_action_tag_result<const_traits,ActionTag>>(std::move(applyRes).error());
    }
}

}