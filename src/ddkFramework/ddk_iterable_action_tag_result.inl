
namespace ddk
{

template<typename Traits,typename ActionTag>
TEMPLATE(typename TTraits, typename AActionTag)
REQUIRED(IS_CONSTRUCTIBLE(recovery_tag,iterable_action_recovery_tag<TTraits,AActionTag>))
constexpr iterable_action_tag_error<Traits,ActionTag>::iterable_action_tag_error(const iterable_action_tag_error<TTraits,AActionTag>& other)
: m_recovery(other.recovery())
{
}
template<typename Traits,typename ActionTag>
TEMPLATE(typename TTraits, typename AActionTag)
REQUIRED(IS_CONSTRUCTIBLE(recovery_tag,iterable_action_recovery_tag<TTraits,AActionTag>))
constexpr iterable_action_tag_error<Traits,ActionTag>::iterable_action_tag_error(iterable_action_tag_error<TTraits,AActionTag>&& other)
: m_recovery(std::move(other).recovery())
{
}
template<typename Traits,typename ActionTag>
TEMPLATE(typename ... Args)
REQUIRED(IS_CONSTRUCTIBLE(recovery_tag,Args...))
constexpr iterable_action_tag_error<Traits,ActionTag>::iterable_action_tag_error(Args&& ... i_args)
: m_recovery(std::forward<Args>(i_args)...)
{
}
template<typename Traits,typename ActionTag>
constexpr const typename iterable_action_tag_error<Traits,ActionTag>::recovery_tag& iterable_action_tag_error<Traits,ActionTag>::recovery() const&
{
	return m_recovery;
}
template<typename Traits,typename ActionTag>
constexpr typename iterable_action_tag_error<Traits,ActionTag>::recovery_tag iterable_action_tag_error<Traits,ActionTag>::recovery() &&
{
	return std::move(m_recovery);
}
template<typename Traits,typename ActionTag>
template<typename Result>
constexpr Result iterable_action_tag_error<Traits,ActionTag>::forward() const
{
	return make_error<Result>(m_recovery);
}

}