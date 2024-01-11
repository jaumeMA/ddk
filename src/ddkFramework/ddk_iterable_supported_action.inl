
namespace ddk
{

template<typename Traits>
TEMPLATE(typename Action)
REQUIRED(ACTION_SUPPORTED(Traits,Action))
supported_action<Traits>::supported_action(const Action& i_action)
: m_action(make_function([i_action](iterable_adaptor<detail::type_erasure_iterable_impl<Traits>> i_adaptorWrapper) -> iterable_action_result<action_t>
{
	return i_action.apply(i_adaptorWrapper);
}))
{
}
template<typename Traits>
supported_action<Traits>::supported_action(const stop_action&)
: action_base(false)
{
}
template<typename Traits>
TEMPLATE(typename Adaptor)
REQUIRED(ACTION_TAGS_SUPPORTED(Adaptor,tags_t))
auto supported_action<Traits>::apply(Adaptor& i_adaptor) const
{
	return ddk::eval(m_action,i_adaptor);
}
template<typename Traits>
TEMPLATE(typename Adaptor)
REQUIRED(ACTION_TAGS_SUPPORTED(Adaptor,const_tags_t))
auto supported_action<Traits>::apply(const Adaptor& i_adaptor) const
{
	return ddk::eval(m_action,i_adaptor);
}

template<typename Traits>
TEMPLATE(typename Action)
REQUIRED(ACTION_SUPPORTED(Traits,Action))
const_supported_action<Traits>::const_supported_action(const Action& i_action)
: m_action(make_function([i_action](iterable_adaptor<const detail::type_erasure_iterable_impl<Traits>> i_adaptorWrapper) -> iterable_action_result<action_t>
{
	return i_action.apply(i_adaptorWrapper);
}))
{
}
template<typename Traits>
const_supported_action<Traits>::const_supported_action(const stop_action&)
: action_base(false)
{
}
template<typename Traits>
TEMPLATE(typename Adaptor)
REQUIRED(ACTION_TAGS_SUPPORTED(const Adaptor,const_tags_t))
auto const_supported_action<Traits>::apply(const Adaptor& i_adaptor) const
{
	return ddk::eval(m_action,i_adaptor);
}


}