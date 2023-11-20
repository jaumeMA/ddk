#pragma once

#include "ddk_tuple.h"
#include "ddk_iterable_impl_interface.h"
#include "ddk_iterable_traits.h"

namespace ddk
{
namespace detail
{

template<typename ... Iterables>
class intersection_iterable_impl : public iterable_impl_interface<intersection_iterable_traits<typename Iterables::traits ...>>, protected iterable_visitor<intersection_iterable_impl<Iterables...>>
{
    static const size_t s_numTypes = tuple<Iterables...>::size();

public:
	typedef intersection_iterable_traits<typename Iterables::traits ...> traits;
	typedef typename traits::reference reference;
	typedef typename traits::const_reference const_reference;

    intersection_iterable_impl(Iterables& ... i_iterables);

    TEMPLATE(typename Function,typename Action)
    REQUIRES(IS_CALLABLE_BY(Function,reference))
    void iterate_impl(Function&& i_try,const Action& i_initialAction);
    TEMPLATE(typename Function,typename Action)
    REQUIRES(IS_CALLABLE_BY(Function,const_reference))
    void iterate_impl(Function&& i_try,const Action& i_initialAction) const;
};
template<typename ... Iterables>
intersection_iterable_impl(Iterables&...) -> intersection_iterable_impl<Iterables...>;

}

template<typename ... Iterables>
class iterable_adaptor<detail::intersection_iterable_impl<Iterables...>>
{
	static const size_t s_numTypes = mpl::get_num_types<Iterables...>();
	static_assert(s_numTypes != 0,"You shall provide any iterable");

public:
	typedef detail::intersection_iterable_traits<typename Iterables::traits...> traits;
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
	inline bool valid() const;
	TEMPLATE(typename ActionTag)
	REQUIRES(ACTION_TAGS_SUPPORTED(traits,ActionTag))
	bool perform_action(const ActionTag& i_actionTag);
	TEMPLATE(typename ActionTag)
	REQUIRES(ACTION_TAGS_SUPPORTED(traits,ActionTag))
	bool perform_action(const ActionTag& i_actionTag) const;

private:
	template<size_t ... Indexs>
	inline reference get_value(const mpl::sequence<Indexs...>&);
	template<size_t ... Indexs>
	inline const_reference get_value(const mpl::sequence<Indexs...>&) const;
	template<size_t ... Indexs, typename Sink>
	inline auto forward_value(const mpl::sequence<Indexs...>& , Sink&& i_sink);
	template<size_t ... Indexs, typename Sink>
	inline auto forward_value(const mpl::sequence<Indexs...>& , Sink&& i_sink) const;
	template<size_t ... Indexs>
	inline bool valid(const mpl::sequence<Indexs...>&) const;
	template<size_t ... Indexs,typename ActionTag>
	bool perform_action(const mpl::sequence<Indexs...>&,const ActionTag& i_actionTag);
	template<size_t ... Indexs,typename ActionTag>
	bool perform_action(const mpl::sequence<Indexs...>&,const ActionTag& i_actionTag) const;

	tuple<deduced_adaptor<Iterables>...> m_adaptors;
};

template<typename ... Iterables>
class iterable_adaptor<const detail::intersection_iterable_impl<Iterables...>>
{
	static const size_t s_numTypes = mpl::get_num_types<Iterables...>();
	static_assert(s_numTypes != 0,"You shall provide any iterable");

public:
	typedef detail::intersection_iterable_traits<typename Iterables::traits...> traits;
	typedef typename traits::value_type value_type;
	typedef typename traits::reference reference;
	typedef typename traits::const_reference const_reference;
	typedef typename traits::tags_t tags_t;
	typedef long long difference_type;

	iterable_adaptor(const Iterables& ... i_iterable);
	inline auto get_value() const;
	template<typename Sink>
	inline auto forward_value(Sink&& i_sink) const;
	inline bool valid() const;
	TEMPLATE(typename ActionTag)
	REQUIRES(ACTION_TAGS_SUPPORTED(traits,ActionTag))
	bool perform_action(const ActionTag& i_actionTag);
	TEMPLATE(typename ActionTag)
	REQUIRES(ACTION_TAGS_SUPPORTED(traits,ActionTag))
	bool perform_action(const ActionTag& i_actionTag) const;

private:
	template<size_t ... Indexs>
	inline const_reference get_value(const mpl::sequence<Indexs...>&) const;
	template<size_t ... Indexs,typename Sink>
	inline auto forward_value(const mpl::sequence<Indexs...>& ,Sink&& i_sink) const;
	template<size_t ... Indexs>
	inline bool valid(const mpl::sequence<Indexs...>&) const;
	template<size_t ... Indexs,typename ActionTag>
	bool perform_action(const mpl::sequence<Indexs...>&,const ActionTag& i_actionTag);
	template<size_t ... Indexs,typename ActionTag>
	bool perform_action(const mpl::sequence<Indexs...>&,const ActionTag& i_actionTag) const;

	tuple<deduced_adaptor<Iterables>...> m_adaptors;
};

}

#include "ddk_intersection_iterable_impl.inl"
