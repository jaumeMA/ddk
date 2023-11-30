#pragma once

#include "ddk_tuple.h"
#include "ddk_iterable.h"
#include "ddk_iterable_visitor.h"

namespace ddk
{
namespace detail
{

template<typename ... Iterables>
class union_iterable_impl : public iterable_impl_interface<union_iterable_traits<typename Iterables::traits ...>>, protected iterable_visitor<union_iterable_impl<Iterables...>>
{
	static const size_t s_numTypes = mpl::num_types<Iterables...>;

public:
	typedef union_iterable_traits<typename Iterables::traits...> traits;
	typedef typename traits::reference reference;
	typedef typename traits::const_reference const_reference;

    union_iterable_impl(Iterables& ... i_iterables);

    TEMPLATE(typename Function,typename Action)
    REQUIRES(IS_CALLABLE_BY(Function,reference))
    iterable_result iterate_impl(Function&& i_try,Action&& i_initialAction);
    TEMPLATE(typename Function,typename Action)
    REQUIRES(IS_CALLABLE_BY(Function,const_reference))
	iterable_result iterate_impl(Function&& i_try,Action&& i_initialAction) const;
};
template<typename ... Iterables>
union_iterable_impl(Iterables& ...) -> union_iterable_impl<Iterables...>;

}

template<typename ... Iterables>
class iterable_adaptor<detail::union_iterable_impl<Iterables...>>
{
	static const size_t s_numTypes = mpl::get_num_types<Iterables...>();

public:
	typedef detail::union_iterable_traits<typename Iterables::traits...> traits;
	typedef typename traits::value_type value_type;
	typedef typename traits::reference reference;
	typedef typename traits::const_reference const_reference;
	typedef typename traits::tags_t tags_t;
	typedef long long difference_type;

	iterable_adaptor(Iterables& ... i_iterable);
	inline auto get_value();
	inline auto get_value() const;
	template<typename Sink>
	inline auto forward_value(Sink&& i_sink);
	template<typename Sink>
	inline auto forward_value(Sink&& i_sink) const;
	TEMPLATE(typename ActionTag)
	REQUIRES(ACTION_TAGS_SUPPORTED(traits,ActionTag))
	iterable_action_result<ActionTag> perform_action(ActionTag&& i_actionTag);
	TEMPLATE(typename ActionTag)
	REQUIRES(ACTION_TAGS_SUPPORTED(traits,ActionTag))
	iterable_action_result<ActionTag> perform_action(ActionTag&& i_actionTag) const;

private:
	template<size_t ... Indexs>
	inline auto get_value(const mpl::sequence<Indexs...>&);
	template<size_t Index>
	static inline reference _get_value(tuple<deduced_adaptor<Iterables>...>& i_adaptor);
	template<size_t ... Indexs>
	inline auto get_value(const mpl::sequence<Indexs...>&) const;
	template<size_t Index>
	static inline const_reference _get_value(const tuple<deduced_adaptor<Iterables>...>& i_adaptor);
	template<size_t ... Indexs,typename Sink>
	inline auto forward_value(const mpl::sequence<Indexs...>&,Sink&& i_sink);
	template<size_t Index, typename Sink>
	static inline auto _forward_value(tuple<deduced_adaptor<Iterables>...>& i_adaptor, Sink&& i_sink);
	template<size_t ... Indexs,typename Sink>
	inline auto forward_value(const mpl::sequence<Indexs...>& ,Sink&& i_sink) const;
	template<size_t Index,typename Sink>
	static inline auto _forward_value(const tuple<deduced_adaptor<Iterables>...>& i_adaptor,Sink&& i_sink);
	template<size_t ... Indexs,typename ActionTag>
	iterable_action_result<ActionTag> perform_action(const mpl::sequence<Indexs...>&,ActionTag&& i_actionTag);
	template<size_t Index,typename ActionTag>
	static iterable_action_result<ActionTag> _perform_action(tuple<deduced_adaptor<Iterables>...>& i_adaptor,ActionTag&& i_actionTag);
	template<size_t ... Indexs,typename ActionTag>
	iterable_action_result<ActionTag> perform_action(const mpl::sequence<Indexs...>&,ActionTag&& i_actionTag) const;
	template<size_t Index,typename ActionTag>
	static iterable_action_result<ActionTag> _perform_action(const tuple<deduced_adaptor<Iterables>...>& i_adaptor,ActionTag&& i_actionTag);
	template<size_t ... Indexs>
	bool perform_default_action(const mpl::sequence<Indexs...>&);
	template<size_t Index>
	static bool _perform_default_action(tuple<deduced_adaptor<Iterables>...>& i_adaptor);
	template<size_t ... Indexs>
	bool perform_default_action(const mpl::sequence<Indexs...>&) const;
	template<size_t Index>
	static bool _perform_default_action(const tuple<deduced_adaptor<Iterables>...>& i_adaptor);

	tuple<deduced_adaptor<Iterables>...> m_adaptors;
	mutable size_t m_currIndex = 0;
};

template<typename ... Iterables>
class iterable_adaptor<const detail::union_iterable_impl<Iterables...>>
{
	static const size_t s_numTypes = mpl::get_num_types<Iterables...>();
	template<typename Iterable, typename ActionTag>
	using adaptor_action_type = decltype(std::declval<deduced_adaptor<Iterable>>().perform_action(std::declval<ActionTag>()));

public:
	typedef detail::union_iterable_traits<const typename Iterables::traits...> traits;
	typedef typename traits::value_type value_type;
	typedef typename traits::reference reference;
	typedef typename traits::const_reference const_reference;
	typedef typename traits::tags_t tags_t;
	typedef long long difference_type;

	iterable_adaptor(const Iterables& ... i_iterable);
	auto get_value() const;
	template<typename Sink>
	inline auto forward_value(Sink&& i_sink) const;
	TEMPLATE(typename ActionTag)
	REQUIRES(ACTION_TAGS_SUPPORTED(traits,ActionTag))
	iterable_action_result<ActionTag> perform_action(ActionTag&& i_actionTag);
	TEMPLATE(typename ActionTag)
	REQUIRES(ACTION_TAGS_SUPPORTED(traits,ActionTag))
	iterable_action_result<ActionTag> perform_action(ActionTag&& i_actionTag) const;

private:
	template<size_t ... Indexs>
	inline auto get_value(const mpl::sequence<Indexs...>&) const;
	template<size_t Index>
	static inline const_reference _get_value(const tuple<deduced_adaptor<Iterables>...>& i_adaptor);
	template<typename Sink,size_t ... Indexs>
	inline auto forward_value(Sink&& i_sink,const mpl::sequence<Indexs...>&) const;
	template<size_t Index,typename Sink>
	static inline auto _forward_value(const tuple<deduced_adaptor<Iterables>...>& i_adaptor,Sink&& i_sink);
	template<size_t ... Indexs,typename ActionTag>
	iterable_action_result<ActionTag> perform_action(const mpl::sequence<Indexs...>&,ActionTag&& i_actionTag);
	template<size_t Index,typename ActionTag>
	static iterable_action_result<ActionTag> _perform_action(tuple<deduced_adaptor<Iterables>...>& i_adaptor,ActionTag&& i_actionTag);
	template<size_t ... Indexs,typename ActionTag>
	iterable_action_result<ActionTag> perform_action(const mpl::sequence<Indexs...>&,ActionTag&& i_actionTag) const;
	template<size_t Index,typename ActionTag>
	static iterable_action_result<ActionTag> _perform_action(const tuple<deduced_adaptor<Iterables>...>& i_adaptor,ActionTag&& i_actionTag);
	template<size_t ... Indexs>
	bool perform_default_action(const mpl::sequence<Indexs...>&);
	template<size_t Index>
	static bool _perform_default_action(tuple<deduced_adaptor<Iterables>...>& i_adaptor);
	template<size_t ... Indexs>
	bool perform_default_action(const mpl::sequence<Indexs...>&) const;
	template<size_t Index>
	static bool _perform_default_action(const tuple<deduced_adaptor<Iterables>...>& i_adaptor);

	tuple<deduced_adaptor<Iterables>...> m_adaptors;
	mutable size_t m_currIndex = 0;
};

}

#include "ddk_union_iterable_impl.inl"
