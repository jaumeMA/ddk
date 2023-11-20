
namespace ddk
{

template<typename Traits>
template<typename Adaptor>
iterable_adaptor_wrapper<Traits>::iterable_adaptor_wrapper(Adaptor& i_adaptor)
: iterable_adaptor_wrapper(i_adaptor,typename mpl::make_sequence<0,tags_t::size()>::type{})
{
}
template<typename Traits>
template<typename Adaptor,size_t ... Indexs>
iterable_adaptor_wrapper<Traits>::iterable_adaptor_wrapper(Adaptor& i_adaptor,const mpl::sequence<Indexs...>&)
: m_typeErasuredAdaptor(&i_adaptor)
, m_actionPerformers((func_action_performer_type<typename tags_t::template at<Indexs>::type>)& iterable_adaptor_wrapper<Traits>::template _perform_action<Adaptor,typename tags_t::template at<Indexs>::type> ...)
, m_validator(&iterable_adaptor_wrapper<Traits>::template _valid<Adaptor>)
{
}
template<typename Traits>
typename iterable_adaptor_wrapper<Traits>::reference iterable_adaptor_wrapper<Traits>::get_value()
{
	//this call should be optimized out by compiler
	return *reinterpret_cast<value_type*>(nullptr);
}
template<typename Traits>
typename iterable_adaptor_wrapper<Traits>::const_reference iterable_adaptor_wrapper<Traits>::get_value() const
{
	//this call should be optimized out by compiler
	return *reinterpret_cast<const value_type*>(nullptr);
}
template<typename Traits>
template<typename Sink>
auto iterable_adaptor_wrapper<Traits>::forward_value(Sink&& i_sink)
{
	//by design this callback is not really needed, added for code completion
	return no_action{};
}
template<typename Traits>
template<typename Sink>
auto iterable_adaptor_wrapper<Traits>::forward_value(Sink&& i_sink) const
{
	//by design this callback is not really needed, added for code completion
	return no_action{};
}
template<typename Traits>
bool iterable_adaptor_wrapper<Traits>::valid() const noexcept
{
	return (*m_validator)(m_typeErasuredAdaptor);
}
template<typename Traits>
TEMPLATE(typename ActionTag)
REQUIRED(ACTION_TAGS_SUPPORTED(Traits,ActionTag))
bool iterable_adaptor_wrapper<Traits>::perform_action(ActionTag&& i_action)
{
	return (*m_actionPerformers.template get<tags_t::template pos_in_type_pack<ActionTag>()>())(m_typeErasuredAdaptor,std::forward<ActionTag>(i_action));
}
template<typename Traits>
TEMPLATE(typename ActionTag)
REQUIRED(ACTION_TAGS_SUPPORTED(Traits,ActionTag))
bool iterable_adaptor_wrapper<Traits>::perform_action(ActionTag&& i_action) const
{
	return (*m_actionPerformers.template get<tags_t::template pos_in_type_pack<ActionTag>()>())(m_typeErasuredAdaptor,std::forward<ActionTag>(i_action));
}
template<typename Traits>
template<typename Adaptor,typename ActionTag>
bool iterable_adaptor_wrapper<Traits>::_perform_action(type_erased_adaptor_t i_adaptor,ActionTag i_action)
{
	return reinterpret_cast<Adaptor*>(i_adaptor)->perform_action(std::move(i_action));
}
template<typename Traits>
template<typename Adaptor,typename ActionTag>
bool iterable_adaptor_wrapper<Traits>::_perform_action(type_erased_const_adaptor_t i_adaptor,ActionTag i_action)
{
	return reinterpret_cast<const Adaptor*>(i_adaptor)->perform_action(std::move(i_action));
}
template<typename Traits>
template<typename Adaptor>
bool iterable_adaptor_wrapper<Traits>::_valid(type_erased_const_adaptor_t i_adaptor)
{
	return reinterpret_cast<const Adaptor*>(i_adaptor)->valid();
}

}