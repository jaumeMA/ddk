//////////////////////////////////////////////////////////////////////////////
//
// Author: Jaume Moragues
// Distributed under the GNU Lesser General Public License, Version 3.0. (See a copy
// at https://www.gnu.org/licenses/lgpl-3.0.ca.html)
//
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ddk_iterable_interface.h"
#include "ddk_iterable_action.h"
#include "ddk_tuple.h"

namespace ddk
{
namespace detail
{

template<typename>
class type_erasure_iterable_impl;

}

template<typename Traits>
class iterable_adaptor<detail::type_erasure_iterable_impl<Traits>>
{
public:
	typedef Traits traits;
	typedef detail::const_iterable_traits<traits> const_traits;
	typedef typename traits::tags_t tags_t;
	typedef typename traits::const_tags_t const_tags_t;

	template<typename Adaptor>
	iterable_adaptor(Adaptor& i_adaptor);

	TEMPLATE(typename Adaptor, typename ActionTag)
	REQUIRES(ACTION_TAGS_SUPPORTED(Adaptor,ActionTag))
	static inline auto perform_action(Adaptor&& i_adaptor, ActionTag&& i_action);

private:
	template<typename>
	struct acquire_tuple_pointer;

	template<typename ... Actions>
	struct acquire_tuple_pointer<mpl::type_pack<Actions...>>
	{
	private:
		template<typename T>
		using func_type = iterable_action_tag_result<traits,T>(*)(void*,T);
		template<typename T>
		using const_func_type = iterable_action_tag_result<const_traits,T>(*)(const void*,T);

	public:
		typedef tuple<func_type<Actions>...> type;
		typedef tuple<const_func_type<Actions>...> const_type;
	};
	typedef void* type_erased_adaptor_t;
	typedef const void* type_erased_const_adaptor_t;
	template<typename T>
	using func_action_performer_type = iterable_action_tag_result<traits,T>(*)(type_erased_adaptor_t,T);
	template<typename T>
	using const_func_action_performer_type = iterable_action_tag_result<const_traits,T>(*)(type_erased_const_adaptor_t,T);
	typedef typename acquire_tuple_pointer<tags_t>::type func_action_performer;
	typedef typename acquire_tuple_pointer<const_tags_t>::const_type const_func_action_performer;

	template<typename Adaptor, size_t ... TagsIndexs, size_t ... ConstTagsIndexs>
	iterable_adaptor(Adaptor& i_adaptor, const mpl::sequence<TagsIndexs...>&, const mpl::sequence<ConstTagsIndexs...>&);
	template<typename Adaptor,size_t ... TagsIndexs,size_t ... ConstTagsIndexs>
	iterable_adaptor(const Adaptor& i_adaptor,const mpl::sequence<TagsIndexs...>&,const mpl::sequence<ConstTagsIndexs...>&);
	template<typename Adaptor, typename ActionTag>
	static iterable_action_tag_result<traits,ActionTag> _perform_action(type_erased_adaptor_t i_adaptor, ActionTag i_action);
	template<typename Adaptor,typename ActionTag>
	static iterable_action_tag_result<const_traits,ActionTag> _const_perform_action(type_erased_const_adaptor_t i_adaptor,ActionTag i_action);

	type_erased_adaptor_t m_typeErasuredAdaptor = nullptr;
	const func_action_performer m_actionPerformers;
	const const_func_action_performer m_constActionPerformers;
};

template<typename Traits>
class iterable_adaptor<const detail::type_erasure_iterable_impl<Traits>>
{
public:
	typedef Traits traits;
	typedef detail::const_iterable_traits<traits> const_traits;
	typedef typename traits::tags_t tags_t;
	typedef typename traits::const_tags_t const_tags_t;

	template<typename Adaptor>
	iterable_adaptor(const Adaptor& i_adaptor);

	TEMPLATE(typename Adaptor, typename ActionTag)
	REQUIRES(ACTION_TAGS_SUPPORTED(Adaptor,ActionTag))
	static inline auto perform_action(Adaptor&& i_adaptor, ActionTag&& i_action);

private:
	template<typename>
	struct acquire_tuple_pointer;

	template<typename ... Actions>
	struct acquire_tuple_pointer<mpl::type_pack<Actions...>>
	{
	private:
		template<typename T>
		using func_type = iterable_action_tag_result<const_traits,T>(*)(const void*,T);

	public:
		typedef tuple<func_type<Actions>...> type;
	};
	typedef const void* type_erased_const_adaptor_t;
	template<typename T>
	using func_action_performer_type = iterable_action_tag_result<const_traits,T>(*)(type_erased_const_adaptor_t,T);
	typedef typename acquire_tuple_pointer<const_tags_t>::type func_action_performer;

	template<typename Adaptor,size_t ... Indexs>
	iterable_adaptor(const Adaptor& i_adaptor,const mpl::sequence<Indexs...>&);
	template<typename Adaptor,typename ActionTag>
	static iterable_action_tag_result<const_traits,ActionTag> _perform_action(type_erased_const_adaptor_t i_adaptor,ActionTag i_action);

	type_erased_const_adaptor_t m_typeErasuredAdaptor = nullptr;
	const func_action_performer m_actionPerformers;
};

}

#include "ddk_type_erasured_iterable_adaptor.inl"