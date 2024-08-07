
namespace ddk
{

template<typename Traits,typename ActionTag,typename Filter>
constexpr filtered_iterable_action_error<Traits,ActionTag,Filter>::filtered_iterable_action_error(const Filter& i_filter)
: m_error(iterable_action_error{ recovery_tag{{},i_filter} })
{
}
template<typename Traits,typename ActionTag,typename Filter>
TEMPLATE(typename ... Args)
REQUIRED(IS_CONSTRUCTIBLE(iterable_action_error,Args...))
constexpr filtered_iterable_action_error<Traits,ActionTag,Filter>::filtered_iterable_action_error(Args&& ... i_args)
: m_error(iterable_action_error{ std::forward<Args>(i_args)... })
{
}
template<typename Traits,typename ActionTag,typename Filter>
constexpr filtered_iterable_action_error<Traits,ActionTag,Filter>::filtered_iterable_action_error(const iterable_action_tag_error<Traits,ActionTag>& i_error,const Filter& i_filter, bool i_filteredOut)
: m_error(iterable_action_error{ filtered_iterable_action_cs<typename recovery_tag::action_tag>(i_error.recovery()),i_filter })
, m_filteredOut(i_filteredOut)
{
}
template<typename Traits,typename ActionTag,typename Filter>
constexpr filtered_iterable_action_error<Traits,ActionTag,Filter>::filtered_iterable_action_error(iterable_action_tag_error<Traits,ActionTag>&& i_error,const Filter& i_filter,bool i_filteredOut)
: m_error(iterable_action_error{ filtered_iterable_action_cs<typename recovery_tag::action_tag>(std::move(i_error).recovery()),i_filter })
,m_filteredOut(i_filteredOut)
{
}
template<typename Traits,typename ActionTag,typename Filter>
template<typename AActionTag>
constexpr filtered_iterable_action_error<Traits,ActionTag,Filter>::filtered_iterable_action_error(const filtered_iterable_action_error<Traits,AActionTag,Filter>& i_error)
: m_error({ filtered_iterable_action_cs<typename recovery_tag::action_tag>(i_error.recovery().action()),i_error.recovery().filter() })
, m_filteredOut(static_cast<bool>(i_error))
{
}
template<typename Traits,typename ActionTag,typename Filter>
template<typename AActionTag>
constexpr filtered_iterable_action_error<Traits,ActionTag,Filter>::filtered_iterable_action_error(filtered_iterable_action_error<Traits,AActionTag,Filter>&& i_error)
: m_error({ filtered_iterable_action_cs<typename recovery_tag::action_tag>(std::move(i_error).recovery().action()),i_error.recovery().filter() })
, m_filteredOut(static_cast<bool>(i_error))
{
}
template<typename Traits,typename ActionTag,typename Filter>
constexpr filtered_iterable_action_error<Traits,ActionTag,Filter>::operator bool() const
{
    return m_filteredOut && m_error.empty() == false;
}
template<typename Traits,typename ActionTag,typename Filter>
TEMPLATE(typename TTraits)
REQUIRED(ADAPTOR_TAGS_EQUIVALENT(Traits,TTraits))
constexpr filtered_iterable_action_error<Traits,ActionTag,Filter>::operator iterable_action_tag_error<TTraits,ActionTag>() const &
{
    typedef iterable_action_recovery_tag<TTraits,ActionTag> action_recovery_tag;

    return filtered_iterable_action_cs<action_recovery_tag>(m_error->recovery().action());
}
template<typename Traits,typename ActionTag,typename Filter>
template<typename TTraits>
constexpr filtered_iterable_action_error<Traits,ActionTag,Filter>::operator iterable_action_tag_error<TTraits,ActionTag>() &&
{
    typedef iterable_action_recovery_tag<TTraits,ActionTag> action_recovery_tag;

    return filtered_iterable_action_cs<action_recovery_tag>(std::move(m_error).extract().recovery().action());
}
template<typename Traits,typename ActionTag,typename Filter>
constexpr auto filtered_iterable_action_error<Traits,ActionTag,Filter>::recovery() const&
{
    return m_error->recovery();
}
template<typename Traits,typename ActionTag,typename Filter>
constexpr auto filtered_iterable_action_error<Traits,ActionTag,Filter>::recovery() &&
{
    return std::move(m_error).extract().recovery();
}

}