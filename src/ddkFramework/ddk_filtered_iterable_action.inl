
namespace ddk
{

template<typename ActionTag,typename Filter>
filtered_iterable_action<ActionTag,Filter>::filtered_iterable_action(const ActionTag& i_action,const Filter& i_filter)
: m_actionTag(i_action)
, m_filter(i_filter)
{
}
template<typename ActionTag,typename Filter>
filtered_iterable_action<ActionTag,Filter>::filtered_iterable_action(ActionTag&& i_action,const Filter& i_filter)
: m_actionTag(std::move(i_action))
, m_filter(i_filter)
{
}
template<typename ActionTag,typename Filter>
TEMPLATE(typename Adaptor)
REQUIRED(ACTION_TAGS_SUPPORTED(Adaptor,tags_t))
filtered_iterable_action_result<Adaptor,ActionTag,Filter> filtered_iterable_action<ActionTag,Filter>::apply(Adaptor&& i_adaptor)
{
    typedef filtered_iterable_action_result<Adaptor,ActionTag,Filter> filtered_result;
    typedef typename mpl::remove_qualifiers<Adaptor>::traits traits;
    typedef typename traits::const_reference const_reference;

    if (auto actionRes = std::forward<Adaptor>(i_adaptor).perform_action(std::move(m_actionTag)))
    {
        if (auto sinkRes = std::forward<Adaptor>(i_adaptor).perform_action(k_iterableEmptySink<const_reference>))
        {
            if (ddk::terse_eval(m_filter,sinkRes.get()))
            {
                return make_result<filtered_result>(actionRes);
            }
            else
            {
                return make_error<filtered_result>();
            }
        }
        else
        {
            throw;
        }
    }
    else
    {
        return make_error<filtered_result>(std::move(actionRes).error(),m_filter,false);
    }
}
template<typename ActionTag,typename Filter>
const ActionTag& filtered_iterable_action<ActionTag,Filter>::action() const &
{
    return m_actionTag;
}
template<typename ActionTag,typename Filter>
ActionTag filtered_iterable_action<ActionTag,Filter>::action() &&
{
    return std::move(m_actionTag);
}
template<typename ActionTag,typename Filter>
const Filter& filtered_iterable_action<ActionTag,Filter>::filter() const
{
    return m_filter;
}

template<typename Filter>
filtered_iterable_action<displace_action_tag,Filter>::filtered_iterable_action(const displace_action_tag& i_action,const Filter& i_filter)
: m_actionTag(i_action)
, m_filter(i_filter)
{
}
template<typename Filter>
TEMPLATE(typename Adaptor)
REQUIRED(ACTION_TAGS_SUPPORTED(Adaptor,tags_t))
filtered_iterable_action_result<Adaptor,displace_action_tag,Filter> filtered_iterable_action<displace_action_tag,Filter>::apply(Adaptor&& i_adaptor)
{
    typedef filtered_iterable_action_result<Adaptor,displace_action_tag,Filter> filtered_result;
    typedef typename mpl::remove_qualifiers<Adaptor>::traits traits;
    typedef typename traits::const_reference const_reference;

    displace_action_tag::difference_type prevShift = m_actionTag.displacement();
    displace_action_tag::difference_type postShift = prevShift;

    if (auto actionRes = std::forward<Adaptor>(i_adaptor).perform_action(displace_action_tag{(prevShift > 0) ? 1 : -1}))
    {
        if (auto sinkRes = std::forward<Adaptor>(i_adaptor).perform_action(k_iterableEmptySink<const_reference>))
        {
            if (ddk::terse_eval(m_filter,sinkRes.get()))
            {
                postShift--;
            }

            if (postShift == 0)
            {
                return make_result<filtered_result>(success);
            }
            else
            {
                return make_error<filtered_result>(postShift,m_filter,true);
            }
        }
        else
        {
            throw;
        }
    }

    return make_error<filtered_iterable_action_result<Adaptor,displace_action_tag,Filter>>(prevShift,m_filter,false);
}
template<typename Filter>
const displace_action_tag& filtered_iterable_action<displace_action_tag,Filter>::action() const
{
    return m_actionTag;
}
template<typename Filter>
const Filter& filtered_iterable_action<displace_action_tag,Filter>::filter() const
{
    return m_filter;
}

template<typename Sink,typename Filter>
filtered_iterable_action<sink_action_tag<Sink>,Filter>::filtered_iterable_action(const sink_action_tag<Sink>& i_action,const Filter& i_filter)
: m_actionTag(i_action)
, m_filter(i_filter)
{
}
template<typename Sink,typename Filter>
filtered_iterable_action<sink_action_tag<Sink>,Filter>::filtered_iterable_action(sink_action_tag<Sink>&& i_action,const Filter& i_filter)
: m_actionTag(std::move(i_action))
, m_filter(i_filter)
{
}
template<typename Sink,typename Filter>
TEMPLATE(typename Adaptor)
REQUIRED(ACTION_TAGS_SUPPORTED(Adaptor,tags_t))
filtered_iterable_action_result<Adaptor,sink_action_tag<Sink>,Filter> filtered_iterable_action<sink_action_tag<Sink>,Filter>::apply(Adaptor&& i_adaptor)
{
    typedef filtered_iterable_action_result<Adaptor,sink_action_tag<Sink>,Filter> filtered_result;
    typedef typename mpl::remove_qualifiers<Adaptor>::traits traits;
    typedef typename traits::const_reference const_reference;

    if (auto sinkRes = std::forward<Adaptor>(i_adaptor).perform_action(k_iterableEmptySink<const_reference>))
    {
        if (ddk::terse_eval(m_filter,sinkRes.get()))
        {
            m_actionTag(sinkRes.get());

            return make_result<filtered_result>(sinkRes);
        }
        else
        {
            return make_error<filtered_result>();
        }
    }
    else
    {
        return make_error<filtered_result>(m_actionTag,m_filter);
    }
}
template<typename Sink,typename Filter>
const sink_action_tag<Sink>& filtered_iterable_action<sink_action_tag<Sink>,Filter>::action() const
{
    return m_actionTag;
}
template<typename Sink,typename Filter>
const Filter& filtered_iterable_action<sink_action_tag<Sink>,Filter>::filter() const
{
    return m_filter;
}

}