
#pragma warning(disable: 4102)

namespace ddk
{
namespace detail
{

template<typename Function>
iterable_filter<Function>::iterable_filter(const Function& i_filter)
: m_filter(i_filter)
{
}
template<typename Function>
Function iterable_filter<Function>::get_filter() const
{
	return m_filter;
}

template<typename Iterable, typename Filter>
TEMPLATE(typename IIterable,typename FFilter)
REQUIRED(IS_CONSTRUCTIBLE(Iterable,IIterable),IS_CONSTRUCTIBLE(Filter,FFilter))
filtered_iterable_impl<Iterable,Filter>::filtered_iterable_impl(IIterable&& i_iterable,FFilter&& i_filter)
: base_t(std::forward<IIterable>(i_iterable),std::forward<FFilter>(i_filter))
{
}
template<typename Iterable,typename Filter>
TEMPLATE(typename Action)
REQUIRED(ACTION_SUPPORTED(traits,Action))
ddk::iterable_result filtered_iterable_impl<Iterable,Filter>::iterate_impl(Action&& i_initialAction)
{
    return loop(std::forward<Action>(i_initialAction));
}
template<typename Iterable,typename Filter>
TEMPLATE(typename Action)
REQUIRED(ACTION_SUPPORTED(const_traits,Action))
ddk::iterable_result filtered_iterable_impl<Iterable,Filter>::iterate_impl(Action&& i_initialAction) const
{
    return loop(std::forward<Action>(i_initialAction));
}

}

template<typename Iterable,typename Filter>
iterable_adaptor<detail::filtered_iterable_impl<Iterable,Filter>>::iterable_adaptor(Iterable& i_iterable,const Filter& i_filter)
: m_adaptor(deduce_adaptor(i_iterable))
, m_filter(i_filter)
{
}
template<typename Iterable,typename Filter>
TEMPLATE(typename ActionTag)
REQUIRED(ACTION_TAGS_SUPPORTED(traits,ActionTag))
auto iterable_adaptor<detail::filtered_iterable_impl<Iterable,Filter>>::perform_action(ActionTag&& i_actionTag)
{
    if (auto actionRes = perform_action(filtered_iterable_action{ std::forward<ActionTag>(i_actionTag),m_filter }))
    {
        return make_result<iterable_action_tag_result<traits,ActionTag>>(actionRes);
    }
    else
    {
        return make_error<iterable_action_tag_result<traits,ActionTag>>(actionRes.error());
    }
}
template<typename Iterable,typename Filter>
TEMPLATE(typename ActionTag)
REQUIRED(ACTION_TAGS_SUPPORTED(const_traits,ActionTag))
auto iterable_adaptor<detail::filtered_iterable_impl<Iterable,Filter>>::perform_action(ActionTag&& i_actionTag) const
{
    if (auto actionRes = perform_action(filtered_iterable_action{ std::forward<ActionTag>(i_actionTag),m_filter }))
    {
        return make_result<iterable_action_tag_result<const_traits,ActionTag>>(actionRes);
    }
    else
    {
        return make_error<iterable_action_tag_result<const_traits,ActionTag>>(actionRes.error());
    }
}
template<typename Iterable,typename Filter>
template<typename ActionTag>
auto iterable_adaptor<detail::filtered_iterable_impl<Iterable,Filter>>::perform_action(filtered_iterable_action<ActionTag,Filter> i_actionTag)
{
    typedef filtered_iterable_action_result<deduced_adaptor<Iterable>,ActionTag,Filter> filtered_action_result;
    typedef typename filtered_action_result::error_t filtered_action_error;

apply_action:
    if (filtered_action_result applyRes = i_actionTag.apply(m_adaptor))
    {
        return make_result<filtered_result<ActionTag>>(applyRes);
    }
    else
    {
        typedef typename filtered_action_error::recovery_tag recovery_tag;
        typedef filtered_iterable_action<ActionTag,Filter> filtered_action_tag;

        filtered_action_error applyError = applyRes.error();

        if constexpr (IS_SAME_CLASS_COND(recovery_tag,filtered_action_tag) && IS_MOVE_ASSIGNABLE_COND(filtered_action_tag))
        {
            if (applyError)
            {
                i_actionTag = std::move(applyError).recovery();

                goto apply_action;
            }
        }
        else 
        {
            if (applyError)
            {
                if (auto recoveryRes = perform_action(std::move(applyError).recovery()))
                {
                    return make_result<filtered_result<ActionTag>>(recoveryRes);
                }
                else
                {
                    return make_error<filtered_result<ActionTag>>(recoveryRes.error());
                }
            }
        }

        return make_error<filtered_result<ActionTag>>(applyError);
    }
}
template<typename Iterable,typename Filter>
template<typename ActionTag>
auto iterable_adaptor<detail::filtered_iterable_impl<Iterable,Filter>>::perform_action(filtered_iterable_action<ActionTag,Filter> i_actionTag) const
{
    typedef filtered_iterable_action_result<const deduced_adaptor<Iterable>,ActionTag,Filter> filtered_action_result;
    typedef typename filtered_action_result::error_t filtered_action_error;

apply_action:
    if (filtered_action_result applyRes = i_actionTag.apply(m_adaptor))
    {
        return make_result<const_filtered_result<ActionTag>>(applyRes);
    }
    else
    {
        typedef typename filtered_action_error::recovery_tag recovery_tag;
        typedef filtered_iterable_action<ActionTag,Filter> filtered_action_tag;

        filtered_action_error applyError = applyRes.error();

        if constexpr (IS_SAME_CLASS_COND(recovery_tag,filtered_action_tag) && IS_MOVE_ASSIGNABLE_COND(filtered_action_tag))
        {
            if (applyError)
            {
                i_actionTag = std::move(applyError).recovery();

                goto apply_action;
            }
        }
        else
        {
            if (applyError)
            {
                if (auto recoveryRes = perform_action(std::move(applyError).recovery()))
                {
                    return make_result<const_filtered_result<ActionTag>>(recoveryRes);
                }
                else
                {
                    return make_error<const_filtered_result<ActionTag>>(recoveryRes.error());
                }
            }
        }

        return make_error<const_filtered_result<ActionTag>>(applyError);
    }
}

template<typename Iterable,typename Filter>
iterable_adaptor<const detail::filtered_iterable_impl<Iterable,Filter>>::iterable_adaptor(const Iterable& i_iterable,const Filter& i_filter)
: m_adaptor(deduce_adaptor(i_iterable))
, m_filter(i_filter)
{
}
template<typename Iterable,typename Filter>
TEMPLATE(typename ActionTag)
REQUIRED(ACTION_TAGS_SUPPORTED(const_traits,ActionTag))
auto iterable_adaptor<const detail::filtered_iterable_impl<Iterable,Filter>>::perform_action(ActionTag&& i_actionTag) const
{
    if (auto actionRes = perform_action(filtered_iterable_action{ std::forward<ActionTag>(i_actionTag),m_filter }))
    {
        return make_result<iterable_action_tag_result<traits,ActionTag>>(actionRes);
    }
    else
    {
        return make_error<iterable_action_tag_result<traits,ActionTag>>(actionRes.error());
    }
}
template<typename Iterable,typename Filter>
template<typename ActionTag>
auto iterable_adaptor<const detail::filtered_iterable_impl<Iterable,Filter>>::perform_action(filtered_iterable_action<ActionTag,Filter> i_actionTag) const
{
    typedef iterable_adaptor<const detail::filtered_iterable_impl<Iterable,Filter>> adaptor_t;
    typedef filtered_iterable_action_result<adaptor_t,ActionTag,Filter> filtered_action_result;
    typedef typename filtered_action_result::error_t filtered_action_error;

apply_action:
    if (filtered_action_result applyRes = i_actionTag.apply(m_adaptor))
    {
        return make_result<filtered_result<ActionTag>>(applyRes);
    }
    else
    {
        typedef typename filtered_action_error::recovery_tag recovery_tag;
        typedef filtered_iterable_action<ActionTag,Filter> filtered_action_tag;

        filtered_action_error applyError = applyRes.error();

        if constexpr (IS_SAME_CLASS_COND(recovery_tag,filtered_action_tag) && IS_MOVE_ASSIGNABLE_COND(filtered_action_tag))
        {
            if (applyError)
            {
                i_actionTag = std::move(applyError).recovery();

                goto apply_action;
            }
        }
        else
        {
            if (applyError)
            {
                if (auto recoveryRes = perform_action(std::move(applyError).recovery()))
                {
                    return make_result<filtered_result<ActionTag>>(recoveryRes);
                }
                else
                {
                    return make_error<filtered_result<ActionTag>>(recoveryRes.error());
                }
            }
        }

        return make_error<filtered_result<ActionTag>>(applyError);
    }
}

}

#pragma warning(default: 4102)