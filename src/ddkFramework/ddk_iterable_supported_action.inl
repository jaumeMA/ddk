
namespace ddk
{

template<typename Traits>
TEMPLATE(typename Action)
REQUIRED(ACTION_TAGS_SUPPORTED(Traits,typename Action::tags_t))
supported_action<Traits>::supported_action(const Action& i_action)
: m_action(make_function([i_action](type_erasure_iterable_adaptor<Traits> i_adaptorWrapper)
{
	return static_cast<bool>(i_action.apply(i_adaptorWrapper,[](auto&&) {}));
}))
{
}
template<typename Traits>
supported_action<Traits>::supported_action(const stop_action&)
: action_base(false)
{
}
template<typename Traits>
TEMPLATE(typename Adaptor,typename Sink)
REQUIRED(ACTION_TAGS_SUPPORTED(Adaptor,tags_t))
auto supported_action<Traits>::apply(Adaptor&& i_adaptor,Sink&& i_sink) const
{
	typedef decltype(std::declval<Adaptor>().forward_value(std::declval<Sink>())) return_type;

	if (ddk::eval(m_action,i_adaptor))
	{
		return i_adaptor.forward_value(std::forward<Sink>(i_sink));
	}
	else
	{
		return return_type{ stop_iteration };
	}
}


}