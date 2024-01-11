
namespace ddk
{
namespace detail
{

template<typename Transform>
TEMPLATE(typename TTransform)
REQUIRED(IS_CONSTRUCTIBLE(Transform,TTransform))
iterable_transform<Transform>::iterable_transform(TTransform&& i_transform)
: m_transform(std::forward<TTransform>(i_transform))
{
}
template<typename Transform>
const Transform& iterable_transform<Transform>::get_transform() const
{
	return m_transform;
}

template<typename FromTraits,typename ToTraits>
typename traits_conversion_callable<FromTraits,ToTraits>::to_reference traits_conversion_callable<FromTraits,ToTraits>::operator()(from_reference i_value) const
{
	return i_value;
}
template<typename FromTraits,typename ToTraits>
typename traits_conversion_callable<FromTraits,ToTraits>::to_const_reference traits_conversion_callable<FromTraits,ToTraits>::operator()(from_const_reference i_value, ...) const
{
	return i_value;
}

template<typename PublicTraits, typename PrivateTraits, typename Iterable, typename Transform>
TEMPLATE(typename IIterable,typename TTransform)
REQUIRED(IS_CONSTRUCTIBLE(Iterable,IIterable),IS_CONSTRUCTIBLE(Transform,TTransform))
transformed_iterable_impl<PublicTraits,PrivateTraits,Iterable,Transform>::transformed_iterable_impl(IIterable&& i_iterable,TTransform&& i_transform)
: base_t(std::forward<IIterable>(i_iterable),std::forward<TTransform>(i_transform))
{
}
template<typename PublicTraits, typename PrivateTraits, typename Iterable,typename Transform>
TEMPLATE(typename Action)
REQUIRED(ACTION_SUPPORTED(traits,Action))
iterable_result transformed_iterable_impl<PublicTraits,PrivateTraits,Iterable,Transform>::iterate_impl(Action&& i_initialAction)
{
	return loop(std::forward<Action>(i_initialAction));
}
template<typename PublicTraits, typename PrivateTraits, typename Iterable,typename Transform>
TEMPLATE(typename Action)
REQUIRED(ACTION_SUPPORTED(const_traits,Action))
iterable_result transformed_iterable_impl<PublicTraits,PrivateTraits,Iterable,Transform>::iterate_impl(Action&& i_initialAction) const
{
	return loop(std::forward<Action>(i_initialAction));
}

}

template<typename PublicTraits,typename PrivateTraits,typename Iterable,typename Transform>
iterable_adaptor<detail::transformed_iterable_impl<PublicTraits,PrivateTraits,Iterable,Transform>>::iterable_adaptor(Iterable& i_iterable, const Transform& i_transform)
: m_adaptor(deduce_adaptor(i_iterable))
, m_transform(i_transform)
{
}
template<typename PublicTraits,typename PrivateTraits,typename Iterable,typename Transform>
TEMPLATE(typename Sink)
REQUIRED(IS_CALLABLE_BY(Sink,typename traits::const_reference))
auto iterable_adaptor<detail::transformed_iterable_impl<PublicTraits,PrivateTraits,Iterable,Transform>>::perform_action(const sink_action_tag<Sink>& i_actionTag)
{
	typedef iterable_action_tag_result<traits,sink_action_tag<Sink>> transformed_result;
	typedef typename private_traits::const_reference private_const_reference;

	if (auto actionRes = m_adaptor.perform_action(k_iterableEmptySink<private_const_reference>))
	{
		return make_result<transformed_result>(i_actionTag(ddk::terse_eval(m_transform,std::forward<private_const_reference>(actionRes.get()))));
	}
	else
	{
		return make_error<transformed_result>(std::move(i_actionTag));
	}
}
template<typename PublicTraits,typename PrivateTraits,typename Iterable,typename Transform>
TEMPLATE(typename Sink)
REQUIRED(IS_CALLABLE_BY(Sink,typename traits::const_reference))
auto iterable_adaptor<detail::transformed_iterable_impl<PublicTraits,PrivateTraits,Iterable,Transform>>::perform_action(const sink_action_tag<Sink>& i_actionTag) const
{
	typedef iterable_action_tag_result<traits,sink_action_tag<Sink>> transformed_result;
	typedef typename private_traits::const_reference private_const_reference;

	if (auto actionRes = m_adaptor.perform_action(k_iterableEmptySink<private_const_reference>))
	{
		return make_result<transformed_result>(i_actionTag(ddk::terse_eval(m_transform,std::forward<private_const_reference>(actionRes.get()))));
	}
	else
	{
		return make_error<transformed_result>(std::move(i_actionTag));
	}
}
template<typename PublicTraits,typename PrivateTraits,typename Iterable,typename Transform>
TEMPLATE(typename Sink)
REQUIRED(IS_CALLABLE_BY(Sink,typename traits::const_reference))
auto iterable_adaptor<detail::transformed_iterable_impl<PublicTraits,PrivateTraits,Iterable,Transform>>::perform_action(sink_action_tag<Sink>&& i_actionTag)
{
	typedef iterable_action_tag_result<traits,sink_action_tag<Sink>> transformed_result;
	typedef typename private_traits::const_reference private_const_reference;

	if (auto actionRes = m_adaptor.perform_action(k_iterableEmptySink<private_const_reference>))
	{
		return make_result<transformed_result>(i_actionTag(ddk::terse_eval(m_transform,std::forward<private_const_reference>(actionRes.get()))));
	}
	else
	{
		return make_error<transformed_result>(std::move(i_actionTag));
	}
}
template<typename PublicTraits,typename PrivateTraits,typename Iterable,typename Transform>
TEMPLATE(typename Sink)
REQUIRED(IS_CALLABLE_BY(Sink,typename traits::const_reference))
auto iterable_adaptor<detail::transformed_iterable_impl<PublicTraits,PrivateTraits,Iterable,Transform>>::perform_action(sink_action_tag<Sink>&& i_actionTag) const
{
	typedef iterable_action_tag_result<traits,sink_action_tag<Sink>> transformed_result;
	typedef typename private_traits::const_reference private_const_reference;

	if (auto actionRes = m_adaptor.perform_action(k_iterableEmptySink<private_const_reference>))
	{
		return make_result<transformed_result>(i_actionTag(ddk::terse_eval(m_transform,std::forward<private_const_reference>(actionRes.get()))));
	}
	else
	{
		return make_error<transformed_result>(std::move(i_actionTag));
	}
}
template<typename PublicTraits,typename PrivateTraits,typename Iterable,typename Transform>
TEMPLATE(typename ActionTag)
REQUIRED(ACTION_TAGS_SUPPORTED(traits,ActionTag))
auto iterable_adaptor<detail::transformed_iterable_impl<PublicTraits,PrivateTraits,Iterable,Transform>>::perform_action(ActionTag&& i_actionTag)
{
	return m_adaptor.perform_action(std::forward<ActionTag>(i_actionTag));
}
template<typename PublicTraits,typename PrivateTraits,typename Iterable,typename Transform>
TEMPLATE(typename ActionTag)
REQUIRED(ACTION_TAGS_SUPPORTED(const_traits,ActionTag))
auto iterable_adaptor<detail::transformed_iterable_impl<PublicTraits,PrivateTraits,Iterable,Transform>>::perform_action(ActionTag&& i_actionTag) const
{
	return m_adaptor.perform_action(std::forward<ActionTag>(i_actionTag));
}

template<typename PublicTraits,typename PrivateTraits,typename Iterable,typename Transform>
iterable_adaptor<const detail::transformed_iterable_impl<PublicTraits,PrivateTraits,Iterable,Transform>>::iterable_adaptor(const Iterable& i_iterable,const Transform& i_transform)
: m_adaptor(deduce_adaptor(i_iterable))
, m_transform(i_transform)
{
}
template<typename PublicTraits,typename PrivateTraits,typename Iterable,typename Transform>
TEMPLATE(typename Sink)
REQUIRED(IS_CALLABLE_BY(Sink,typename traits::const_reference))
auto iterable_adaptor<const detail::transformed_iterable_impl<PublicTraits,PrivateTraits,Iterable,Transform>>::perform_action(sink_action_tag<Sink>&& i_actionTag) const
{
	typedef iterable_action_tag_result<traits,sink_action_tag<Sink>> transformed_result;
	typedef typename private_traits::const_reference private_const_reference;

	if (auto actionRes = m_adaptor.perform_action(k_iterableEmptyTypedSink<private_const_reference>))
	{
		return make_result<transformed_result>(i_actionTag(ddk::terse_eval(m_transform,std::forward<private_const_reference>(actionRes.get()))));
	}
	else
	{
		return make_error<transformed_result>(std::move(i_actionTag));
	}
}
template<typename PublicTraits,typename PrivateTraits,typename Iterable,typename Transform>
TEMPLATE(typename ActionTag)
REQUIRED(ACTION_TAGS_SUPPORTED(const_traits,ActionTag))
auto iterable_adaptor<const detail::transformed_iterable_impl<PublicTraits,PrivateTraits,Iterable,Transform>>::perform_action(ActionTag&& i_actionTag) const
{
	return m_adaptor.perform_action(std::forward<ActionTag>(i_actionTag));
}

}
