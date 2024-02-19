
namespace ddk
{
namespace detail
{

template<typename T>
iterable_order<T>::iterable_order(const T& i_order)
: m_order(i_order)
{
}
template<typename T>
const T& iterable_order<T>::order() const
{
	return m_order;
}

template<typename Iterable,typename ActionResolver>
TEMPLATE(typename IIterable,typename AActionResolver)
REQUIRED(IS_CONSTRUCTIBLE(Iterable,IIterable),IS_CONSTRUCTIBLE(ActionResolver,AActionResolver))
ordered_iterable_impl<Iterable,ActionResolver>::ordered_iterable_impl(IIterable&& i_iterable,AActionResolver&& i_actionResolver)
: iterable_visitor<ordered_iterable_impl<Iterable,ActionResolver>>(i_iterable,i_actionResolver)
{
}
template<typename Iterable,typename ActionResolver>
TEMPLATE(typename Action)
REQUIRED(ACTION_SUPPORTED(traits,Action))
void ordered_iterable_impl<Iterable,ActionResolver>::iterate_impl(Action&& i_initialAction)
{
	loop(std::forward<Action>(i_initialAction));
}
template<typename Iterable,typename ActionResolver>
TEMPLATE(typename Action)
REQUIRED(ACTION_SUPPORTED(traits,Action))
void ordered_iterable_impl<Iterable,ActionResolver>::iterate_impl(Action&& i_initialAction) const
{
	loop(std::forward<Action>(i_initialAction));
}

}

template<typename Iterable,typename ActionResolver>
iterable_adaptor<detail::ordered_iterable_impl<Iterable,ActionResolver>>::iterable_adaptor(Iterable& i_iterable, const ActionResolver& i_actionResolver)
: m_adaptor(deduce_adaptor(i_iterable))
, m_actionResolver(i_actionResolver)
{
}
template<typename Iterable,typename ActionResolver>
iterable_adaptor<detail::ordered_iterable_impl<Iterable,ActionResolver>>::iterable_adaptor(detail::ordered_iterable_impl<Iterable,ActionResolver>& i_iterable)
{
}
template<typename Iterable,typename ActionResolver>
TEMPLATE(typename ActionTag)
REQUIRED(ACTION_TAGS_SUPPORTED(traits,ActionTag))
constexpr auto iterable_adaptor<detail::ordered_iterable_impl<Iterable,ActionResolver>>::perform_action(ActionTag&& i_actionTag)
{
	return m_actionResolver(m_adaptor,std::forward<ActionTag>(i_actionTag));
}
template<typename Iterable,typename ActionResolver>
TEMPLATE(typename ActionTag)
REQUIRED(ACTION_TAGS_SUPPORTED(traits,ActionTag))
constexpr auto iterable_adaptor<detail::ordered_iterable_impl<Iterable,ActionResolver>>::perform_action(ActionTag&& i_actionTag) const
{
	return m_actionResolver(m_adaptor,std::forward<ActionTag>(i_actionTag));
}

template<typename Iterable,typename ActionResolver>
iterable_adaptor<const detail::ordered_iterable_impl<Iterable,ActionResolver>>::iterable_adaptor(const Iterable& i_iterable, const ActionResolver& i_actionResolver)
: m_adaptor(deduce_adaptor(i_iterable))
, m_actionResolver(i_actionResolver)
{
}
template<typename Iterable,typename ActionResolver>
TEMPLATE(typename ActionTag)
REQUIRED(ACTION_TAGS_SUPPORTED(traits,ActionTag))
constexpr auto iterable_adaptor<const detail::ordered_iterable_impl<Iterable,ActionResolver>>::perform_action(ActionTag&& i_actionTag) const
{
	return m_actionResolver(m_adaptor,std::forward<ActionTag>(i_actionTag));
}

}