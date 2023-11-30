#pragma once

#include "ddk_tuple.h"
#include "ddk_iterable_action.h"
#include "ddk_type_concepts.h"
#include "ddk_iterable_action_tag_concepts.h"
#include "ddk_concepts.h"

namespace ddk
{

template<typename Traits>
class type_erasure_iterable_adaptor
{
	template<typename>
	struct acquire_tuple_pointer;

	template<typename ... Actions>
	struct acquire_tuple_pointer<mpl::type_pack<Actions...>>
	{
	private:
		template<typename T>
		using func_type = iterable_action_result<T>(*)(void*,T);

	public:
		typedef tuple<func_type<Actions>...> type;
	};

public:
	typedef typename Traits::value_type value_type;
	typedef typename Traits::reference reference;
	typedef typename Traits::const_reference const_reference;
	typedef long long difference_type;
	typedef typename Traits::tags_t tags_t;

	template<typename Adaptor>
	type_erasure_iterable_adaptor(Adaptor& i_adaptor);

	inline reference get_value();
	inline const_reference get_value() const;
	template<typename Sink>
	inline auto forward_value(Sink&& i_sink);
	template<typename Sink>
	inline auto forward_value(Sink&& i_sink) const;
	TEMPLATE(typename ActionTag)
	REQUIRES(ACTION_TAGS_SUPPORTED(Traits,ActionTag))
	inline auto perform_action(ActionTag&& i_action);
	TEMPLATE(typename ActionTag)
	REQUIRES(ACTION_TAGS_SUPPORTED(Traits,ActionTag))
	inline auto perform_action(ActionTag&& i_action) const;

private:
	typedef const void* type_erased_const_adaptor_t;
	typedef void* type_erased_adaptor_t;
	template<typename T>
	using func_action_performer_type = iterable_action_result<T>(*)(void*,T);
	typedef typename acquire_tuple_pointer<tags_t>::type func_action_performer;

	template<typename Adaptor, size_t ... Indexs>
	type_erasure_iterable_adaptor(Adaptor& i_adaptor, const mpl::sequence<Indexs...>&);
	template<typename Adaptor, typename ActionTag>
	static iterable_action_result<ActionTag> _perform_action(type_erased_adaptor_t i_adaptor, ActionTag i_action);
	template<typename Adaptor,typename ActionTag>
	static iterable_action_result<ActionTag> _perform_action(type_erased_const_adaptor_t i_adaptor,ActionTag i_action);

	void* m_typeErasuredAdaptor = nullptr;
	const func_action_performer m_actionPerformers;
};

}

#include "ddk_type_erasured_iterable_adaptor.inl"