
namespace ddk
{
namespace detail
{

template<typename Reference,typename Transform,typename ActionTag>
iterable_transformed_action<Reference,Transform,ActionTag>::iterable_transformed_action(const Transform& i_transform,ActionTag i_actionTag)
: m_transform(i_transform)
, m_actionTag(std::forward<ActionTag>(i_actionTag))
{
}
template<typename Reference,typename Transform,typename ActionTag>
auto iterable_transformed_action<Reference,Transform,ActionTag>::operator*()
{
	return std::forward<ActionTag>(m_actionTag);
}
template<typename Reference,typename Transform,typename ActionTag>
template<typename T>
constexpr Reference iterable_transformed_action<Reference,Transform,ActionTag>::operator()(T&& i_value) const
{
	return  ddk::terse_eval(m_transform,std::forward<T>(i_value));
}

template<typename Reference,typename Transform,typename Sink>
template<typename SSink>
iterable_transformed_action<Reference,Transform,sink_action_tag<Sink>>::iterable_transformed_action(const Transform& i_transform, SSink&& i_actionTag)
: m_transform(i_transform)
, m_actionTag(std::forward<SSink>(i_actionTag))
{
}
template<typename Reference,typename Transform,typename Sink>
iterable_transformed_action<Reference,Transform,sink_action_tag<Sink>>::~iterable_transformed_action()
{
	m_cache.template destroy<Reference>();
}
template<typename Reference,typename Transform,typename Sink>
auto iterable_transformed_action<Reference,Transform,sink_action_tag<Sink>>::operator*()
{
	return sink_action_tag{ [this](auto&& i_value) mutable
	{
		//transport value into outer result
		m_cache.template construct<Reference>(m_actionTag(ddk::terse_eval(m_transform,std::forward<decltype(i_value)>(i_value))));
	} };
}
template<typename Reference,typename Transform,typename Sink>
template<typename T>
constexpr Reference iterable_transformed_action<Reference,Transform,sink_action_tag<Sink>>::operator()(T&& i_args) const
{
	return  m_cache.template get<Reference>();
}

template<typename Transform>
TEMPLATE(typename TTransform)
REQUIRED(IS_CONSTRUCTIBLE(Transform,TTransform))
iterable_transform<Transform>::iterable_transform(TTransform&& i_transform)
: m_transform(std::forward<TTransform>(i_transform))
{
}
template<typename Transform>
template<typename Reference,typename ActionTag>
auto iterable_transform<Transform>::map_action(ActionTag&& i_action) const
{
	return iterable_transformed_action<Reference,Transform,ActionTag>{ m_transform,std::forward<ActionTag>(i_action) };
}

template<typename FromTraits,typename ToTraits>
typename traits_conversion_callable<FromTraits,ToTraits>::to_reference traits_conversion_callable<FromTraits,ToTraits>::operator()(from_reference i_value) const
{
	return i_value;
}
template<typename FromTraits,typename ToTraits>
TEMPLATE(typename T)
REQUIRED(IS_CONVERTIBLE(T,to_const_reference))
typename traits_conversion_callable<FromTraits,ToTraits>::to_const_reference traits_conversion_callable<FromTraits,ToTraits>::operator()(T&& i_value, ...) const
{
	return i_value;
}

template<typename PublicTraits, typename PrivateTraits, typename Iterable, typename Transform>
TEMPLATE(typename IIterable)
REQUIRED(IS_CONSTRUCTIBLE(Iterable,IIterable))
transformed_iterable_impl<PublicTraits,PrivateTraits,Iterable,Transform>::transformed_iterable_impl(IIterable&& i_iterable, const detail::iterable_transform<Transform>& i_transform)
: base_t(std::forward<IIterable>(i_iterable),i_transform)
{
}
template<typename PublicTraits, typename PrivateTraits, typename Iterable,typename Transform>
TEMPLATE(typename Action)
REQUIRED(ACTION_SUPPORTED(traits,Action))
void transformed_iterable_impl<PublicTraits,PrivateTraits,Iterable,Transform>::iterate_impl(Action&& i_initialAction)
{
	loop(std::forward<Action>(i_initialAction));
}
template<typename PublicTraits, typename PrivateTraits, typename Iterable,typename Transform>
TEMPLATE(typename Action)
REQUIRED(ACTION_SUPPORTED(const_traits,Action))
void transformed_iterable_impl<PublicTraits,PrivateTraits,Iterable,Transform>::iterate_impl(Action&& i_initialAction) const
{
	loop(std::forward<Action>(i_initialAction));
}

}

template<typename PublicTraits,typename PrivateTraits,typename Iterable,typename Transform>
iterable_adaptor<detail::transformed_iterable_impl<PublicTraits,PrivateTraits,Iterable,Transform>>::iterable_adaptor(Iterable& i_iterable, const detail::iterable_transform<Transform>& i_transform)
: m_adaptor(deduce_adaptor(i_iterable))
, m_transform(i_transform)
{
}
template<typename PublicTraits,typename PrivateTraits,typename Iterable,typename Transform>
TEMPLATE(typename Adaptor, typename ActionTag)
REQUIRED(ACTION_TAGS_SUPPORTED(Adaptor,ActionTag))
constexpr auto iterable_adaptor<detail::transformed_iterable_impl<PublicTraits,PrivateTraits,Iterable,Transform>>::perform_action(Adaptor&& i_adaptor, ActionTag&& i_actionTag)
{
	if constexpr (IS_ADAPTOR_REPRESENTABLE_BY_ACTION_COND(Adaptor,ActionTag))
	{
		typedef iterable_action_tag_result<detail::adaptor_traits<Adaptor>,ActionTag> transformed_result;

		auto mappedAction = i_adaptor.m_transform.map_action<typename detail::adaptor_traits<Adaptor>::reference>(std::forward<ActionTag>(i_actionTag));
		if (auto actionRes = std::forward<Adaptor>(i_adaptor).m_adaptor.perform_action(std::forward<Adaptor>(i_adaptor).m_adaptor,*mappedAction))
		{
			return make_result<transformed_result>(mappedAction(actionRes.get()));
		}
		else
		{
			return make_error<transformed_result>(std::forward<ActionTag>(i_actionTag));
		}
	}
	else
	{
		return i_adaptor.m_adaptor.perform_action(std::forward<Adaptor>(i_adaptor).m_adaptor,std::forward<ActionTag>(i_actionTag));
	}
}

template<typename PublicTraits,typename PrivateTraits,typename Iterable,typename Transform>
iterable_adaptor<const detail::transformed_iterable_impl<PublicTraits,PrivateTraits,Iterable,Transform>>::iterable_adaptor(const Iterable& i_iterable,const detail::iterable_transform<Transform>& i_transform)
: m_adaptor(deduce_adaptor(i_iterable))
, m_transform(i_transform)
{
}
template<typename PublicTraits,typename PrivateTraits,typename Iterable,typename Transform>
TEMPLATE(typename Adaptor, typename ActionTag)
REQUIRED(ACTION_TAGS_SUPPORTED(Adaptor,ActionTag))
constexpr auto iterable_adaptor<const detail::transformed_iterable_impl<PublicTraits,PrivateTraits,Iterable,Transform>>::perform_action(Adaptor&& i_adaptor, ActionTag&& i_actionTag)
{
	if constexpr (IS_ADAPTOR_REPRESENTABLE_BY_ACTION_COND(Adaptor,ActionTag))
	{
		typedef iterable_action_tag_result<traits,ActionTag> transformed_result;

		auto mappedAction = i_adaptor.m_transform.map_action<typename detail::adaptor_traits<Adaptor>::reference>(std::forward<ActionTag>(i_actionTag));
		if (auto actionRes = std::forward<Adaptor>(i_adaptor).m_adaptor.perform_action(std::forward<Adaptor>(i_adaptor).m_adaptor,*mappedAction))
		{
			return make_result<transformed_result>(mappedAction(actionRes.get()));
		}
		else
		{
			return make_error<transformed_result>(std::forward<ActionTag>(i_actionTag));
		}
	}
	else
	{
		return i_adaptor.m_adaptor.perform_action(std::forward<Adaptor>(i_adaptor).m_adaptor,std::forward<ActionTag>(i_actionTag));
	}
}

}