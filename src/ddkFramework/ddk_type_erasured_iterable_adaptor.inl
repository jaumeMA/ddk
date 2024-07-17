
namespace ddk
{

template<typename Traits>
template<typename Adaptor>
iterable_adaptor<detail::type_erasure_iterable_impl<Traits>>::iterable_adaptor(Adaptor& i_adaptor)
: iterable_adaptor(i_adaptor,typename mpl::make_sequence<0,tags_t::size()>::type{},typename mpl::make_sequence<0,const_tags_t::size()>::type{})
{
}
template<typename Traits>
template<typename Adaptor,size_t ... TagsIndexs, size_t ... ConstTagsIndexs>
iterable_adaptor<detail::type_erasure_iterable_impl<Traits>>::iterable_adaptor(Adaptor& i_adaptor, const mpl::sequence<TagsIndexs...>&, const mpl::sequence<ConstTagsIndexs...>&)
: m_typeErasuredAdaptor(&i_adaptor)
, m_actionPerformers((func_action_performer_type<typename tags_t::template at<TagsIndexs>::type>)&iterable_adaptor<detail::type_erasure_iterable_impl<Traits>>::template _perform_action<Adaptor,typename tags_t::template at<TagsIndexs>::type> ...)
, m_constActionPerformers((const_func_action_performer_type<typename const_tags_t::template at<ConstTagsIndexs>::type>)&iterable_adaptor<detail::type_erasure_iterable_impl<Traits>>::template _const_perform_action<Adaptor,typename const_tags_t::template at<ConstTagsIndexs>::type> ...)
{
}
template<typename Traits>
template<typename Adaptor,size_t ... TagsIndexs,size_t ... ConstTagsIndexs>
iterable_adaptor<detail::type_erasure_iterable_impl<Traits>>::iterable_adaptor(const Adaptor& i_adaptor,const mpl::sequence<TagsIndexs...>&,const mpl::sequence<ConstTagsIndexs...>&)
: m_typeErasuredAdaptor(&const_cast<Adaptor&>(i_adaptor))
, m_actionPerformers((func_action_performer_type<typename tags_t::template at<TagsIndexs>::type>)& iterable_adaptor<detail::type_erasure_iterable_impl<Traits>>::template _perform_action<Adaptor,typename tags_t::template at<TagsIndexs>::type> ...)
, m_constActionPerformers((const_func_action_performer_type<typename const_tags_t::template at<ConstTagsIndexs>::type>)& iterable_adaptor<detail::type_erasure_iterable_impl<Traits>>::template _const_perform_action<Adaptor,typename const_tags_t::template at<ConstTagsIndexs>::type> ...)
{
}
template<typename Traits>
TEMPLATE(typename Adaptor, typename ActionTag)
REQUIRED(ACTION_TAGS_SUPPORTED(Adaptor,ActionTag))
auto iterable_adaptor<detail::type_erasure_iterable_impl<Traits>>::perform_action(Adaptor&& i_adaptor, ActionTag&& i_action)
{
	constexpr size_t posInTags = detail::adaptor_tags<Adaptor>::template pos_in_type_pack<ActionTag,std::is_constructible>();

	static_assert(posInTags < detail::adaptor_tags<Adaptor>::size(), "You shall provide a compatible action tag");

	if constexpr (mpl::is_const<Adaptor>)
	{
		return (*i_adaptor.m_constActionPerformers.template get<posInTags>())(std::forward<Adaptor>(i_adaptor).m_typeErasuredAdaptor,std::forward<ActionTag>(i_action));
	}
	else
	{
		return (*i_adaptor.m_actionPerformers.template get<posInTags>())(std::forward<Adaptor>(i_adaptor).m_typeErasuredAdaptor,std::forward<ActionTag>(i_action));
	}
}
template<typename Traits>
template<typename Adaptor,typename ActionTag>
iterable_action_tag_result<typename iterable_adaptor<detail::type_erasure_iterable_impl<Traits>>::traits,ActionTag> iterable_adaptor<detail::type_erasure_iterable_impl<Traits>>::_perform_action(type_erased_adaptor_t i_adaptor,ActionTag i_action)
{
	return Adaptor::perform_action(*reinterpret_cast<Adaptor*>(i_adaptor),std::move(i_action));
}
template<typename Traits>
template<typename Adaptor,typename ActionTag>
iterable_action_tag_result<typename iterable_adaptor<detail::type_erasure_iterable_impl<Traits>>::const_traits,ActionTag> iterable_adaptor<detail::type_erasure_iterable_impl<Traits>>::_const_perform_action(type_erased_const_adaptor_t i_adaptor,ActionTag i_action)
{
	return Adaptor::perform_action(*reinterpret_cast<const Adaptor*>(i_adaptor),std::move(i_action));
}

template<typename Traits>
template<typename Adaptor>
iterable_adaptor<const detail::type_erasure_iterable_impl<Traits>>::iterable_adaptor(const Adaptor& i_adaptor)
: iterable_adaptor(i_adaptor,typename mpl::make_sequence<0,const_tags_t::size()>::type{})
{
}
template<typename Traits>
template<typename Adaptor,size_t ... Indexs>
iterable_adaptor<const detail::type_erasure_iterable_impl<Traits>>::iterable_adaptor(const Adaptor& i_adaptor,const mpl::sequence<Indexs...>&)
: m_typeErasuredAdaptor(&i_adaptor)
, m_actionPerformers((func_action_performer_type<typename const_tags_t::template at<Indexs>::type>)&iterable_adaptor<const detail::type_erasure_iterable_impl<Traits>>::template _perform_action<Adaptor,typename const_tags_t::template at<Indexs>::type> ...)
{
}
template<typename Traits>
TEMPLATE(typename Adaptor, typename ActionTag)
REQUIRED(ACTION_TAGS_SUPPORTED(Traits,ActionTag))
auto iterable_adaptor<const detail::type_erasure_iterable_impl<Traits>>::perform_action(Adaptor&& i_adaptor, ActionTag&& i_action)
{
	constexpr size_t posInTags = detail::adaptor_tags<Adaptor>::template pos_in_type_pack<ActionTag,std::is_constructible>();

	static_assert(posInTags < detail::adaptor_tags<Adaptor>::size(),"You shall provide a compatible action tag");

	return (*i_adaptor.m_actionPerformers.template get<posInTags>())(std::forward<Adaptor>(i_adaptor).m_typeErasuredAdaptor,std::forward<ActionTag>(i_action));
}
template<typename Traits>
template<typename Adaptor,typename ActionTag>
iterable_action_tag_result<typename iterable_adaptor<const detail::type_erasure_iterable_impl<Traits>>::const_traits,ActionTag> iterable_adaptor<const detail::type_erasure_iterable_impl<Traits>>::_perform_action(type_erased_const_adaptor_t i_adaptor,ActionTag i_action)
{
	return reinterpret_cast<const Adaptor*>(i_adaptor)->perform_action(*reinterpret_cast<const Adaptor*>(i_adaptor),std::move(i_action));
}

}