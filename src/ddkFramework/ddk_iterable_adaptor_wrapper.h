#pragma once

#include "ddk_tuple.h"
#include "ddk_type_concepts.h"
#include "ddk_iterable_action_tag_concepts.h"
#include "ddk_concepts.h"

namespace ddk
{

template<typename>
struct acquire_tuple_pointer;
	
template<typename ... Actions>
struct acquire_tuple_pointer<mpl::type_pack<Actions...>>
{
	template<typename T>
	using func_type = bool(*)(void*,T);

public:
	typedef tuple< func_type<Actions>...> type;
};

template<typename Traits>
class iterable_adaptor_wrapper
{
public:
	typedef typename Traits::value_type value_type;
	typedef typename Traits::reference reference;
	typedef typename Traits::const_reference const_reference;
	typedef long long difference_type;
	typedef typename Traits::tags_t tags_t;

	template<typename Adaptor>
	iterable_adaptor_wrapper(Adaptor& i_adaptor);

	inline reference get_value();
	inline const_reference get_value() const;
	template<typename Sink>
	inline auto forward_value(Sink&& i_sink);
	template<typename Sink>
	inline auto forward_value(Sink&& i_sink) const;
	inline bool valid() const noexcept;
	TEMPLATE(typename ActionTag)
	REQUIRES(ACTION_TAGS_SUPPORTED(Traits,ActionTag))
	inline bool perform_action(ActionTag&& i_action);
	TEMPLATE(typename ActionTag)
	REQUIRES(ACTION_TAGS_SUPPORTED(Traits,ActionTag))
	inline bool perform_action(ActionTag&& i_action) const;

private:
	typedef const void* type_erased_const_adaptor_t;
	typedef void* type_erased_adaptor_t;
	template<typename T>
	using func_action_performer_type = bool(*)(void*,T);
	typedef typename acquire_tuple_pointer<tags_t>::type func_action_performer;
	typedef bool(*func_validity)(type_erased_const_adaptor_t);

	template<typename Adaptor, size_t ... Indexs>
	iterable_adaptor_wrapper(Adaptor& i_adaptor, const mpl::sequence<Indexs...>&);
	template<typename Adaptor, typename ActionTag>
	static bool _perform_action(type_erased_adaptor_t i_adaptor, ActionTag i_action);
	template<typename Adaptor,typename ActionTag>
	static bool _perform_action(type_erased_const_adaptor_t i_adaptor,ActionTag i_action);
	template<typename Adaptor>
	static bool _valid(type_erased_const_adaptor_t i_adaptor);

	void* m_typeErasuredAdaptor = nullptr;
	const func_action_performer m_actionPerformers;
	const func_validity m_validator = nullptr;
};

}

#include "ddk_iterable_adaptor_wrapper.inl"