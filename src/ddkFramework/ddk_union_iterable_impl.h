#pragma once

#include "ddk_union_iterable_action.h"
#include "ddk_iterable_impl_interface.h"
#include "ddk_iterable_visitor.h"
#include "ddk_iterable_action.h"
#include "ddk_tuple.h"

namespace ddk
{
namespace detail
{

template<typename ... Iterables>
class union_iterable_impl : public iterable_impl_interface<union_iterable_traits<typename Iterables::traits ...>>,public iterable_visitor<union_iterable_impl<Iterables...>>
{
	static const size_t s_numTypes = mpl::num_types<Iterables...>;

public:
	typedef union_iterable_traits<typename Iterables::traits...> traits;
	typedef const_iterable_traits<union_iterable_traits<typename Iterables::traits...>> const_traits;

	union_iterable_impl(Iterables& ... i_iterables);

	TEMPLATE(typename Action)
	REQUIRES(ACTION_SUPPORTED(traits,Action))
	inline iterable_result iterate_impl(Action&& i_initialAction);
	TEMPLATE(typename Action)
	REQUIRES(ACTION_SUPPORTED(const_traits,Action))
	inline iterable_result iterate_impl(Action&& i_initialAction) const;
};
template<typename ... Iterables>
union_iterable_impl(Iterables& ...)->union_iterable_impl<Iterables...>;

template<typename ... Iterables>
class union_iterable_adaptor
{
public:
	static const size_t s_numTypes = mpl::num_types<Iterables...>;
	typedef detail::union_iterable_traits<typename Iterables::traits...> traits;
	typedef detail::const_iterable_traits<traits> const_traits;
	typedef typename traits::tags_t tags_t;
	typedef typename traits::const_tags_t const_tags_t;

	union_iterable_adaptor(Iterables& ... i_iterable);

	inline bool set_current_iterable_index(size_t i_currIndex) const;
	inline size_t get_current_iterable_index() const;
	TEMPLATE(typename ActionTag)
	REQUIRES(ACTION_TAGS_SUPPORTED(traits,ActionTag))
	inline auto perform_action(ActionTag&& i_actionTag);
	TEMPLATE(typename ActionTag)
	REQUIRES(ACTION_TAGS_SUPPORTED(const_traits,ActionTag))
	inline auto perform_action(ActionTag&& i_actionTag) const;

private:
	typedef tuple<deduced_adaptor<Iterables>...> deduced_adaptors;

	template<size_t ... Indexs,typename ActionTag>
	inline auto perform_action(const mpl::sequence<Indexs...>&,ActionTag&& i_actionTag);
	template<size_t ... Indexs,typename ActionTag>
	inline auto perform_action(const mpl::sequence<Indexs...>&,ActionTag&& i_actionTag) const;
	template<size_t Index,typename Traits,typename ActionTag>
	inline static iterable_action_tag_result<Traits,ActionTag> _perform_action(deduced_adaptors& i_adaptor,ActionTag&& i_actionTag);
	template<size_t Index,typename Traits,typename ActionTag>
	inline static iterable_action_tag_result<const_iterable_traits<Traits>,ActionTag> _const_perform_action(const deduced_adaptors& i_adaptor,ActionTag&& i_actionTag);

	deduced_adaptors m_adaptors;
	mutable size_t m_currIndex = 0;
};

template<typename ... Iterables>
class union_iterable_const_adaptor
{
public:
	static const size_t s_numTypes = mpl::num_types<Iterables...>;
	typedef detail::union_iterable_traits<const typename Iterables::traits...> traits;
	typedef detail::const_iterable_traits<traits> const_traits;
	typedef typename traits::tags_t tags_t;
	typedef typename traits::const_tags_t const_tags_t;

	union_iterable_const_adaptor(const Iterables& ... i_iterable);

	inline bool set_current_iterable_index(size_t i_currIndex) const;
	inline size_t get_current_iterable_index() const;
	TEMPLATE(typename ActionTag)
	REQUIRES(ACTION_TAGS_SUPPORTED(const_traits,ActionTag))
	inline auto perform_action(ActionTag&& i_actionTag) const;

private:
	typedef tuple<const deduced_adaptor<Iterables>...> deduced_adaptors;

	template<size_t ... Indexs,typename ActionTag>
	inline auto perform_action(const mpl::sequence<Indexs...>&,ActionTag&& i_actionTag) const;
	template<size_t Index,typename Traits,typename ActionTag>
	inline static iterable_action_tag_result<const_iterable_traits<Traits>,ActionTag> _perform_action(const deduced_adaptors& i_adaptor,ActionTag&& i_actionTag);

	deduced_adaptors m_adaptors;
	mutable size_t m_currIndex = 0;
};

}

template<typename ... Iterables>
class iterable_adaptor<detail::union_iterable_impl<Iterables...>> : public detail::union_iterable_adaptor<Iterables...>
{
public:
	typedef detail::union_iterable_adaptor<Iterables...> adaptor_base;
	using adaptor_base::s_numTypes;
	using adaptor_base::traits;
	using adaptor_base::const_traits;
	using adaptor_base::tags_t;
	using adaptor_base::const_tags_t;
	using adaptor_base::union_iterable_adaptor;
	using adaptor_base::set_current_iterable_index;
	using adaptor_base::get_current_iterable_index;

	TEMPLATE(typename ActionTag)
	REQUIRES(ACTION_TAGS_SUPPORTED(traits,ActionTag))
	inline auto perform_action(ActionTag&& i_actionTag);
	TEMPLATE(typename ActionTag)
	REQUIRES(ACTION_TAGS_SUPPORTED(const_traits,ActionTag))
	inline auto perform_action(ActionTag&& i_actionTag) const;

private:
	template<typename ActionTag>
	inline auto perform_action(union_iterable_action<ActionTag> i_actionTag);
	template<typename ActionTag>
	inline auto perform_action(union_iterable_action<ActionTag> i_actionTag) const;
};

template<typename ... Iterables>
class iterable_adaptor<const detail::union_iterable_impl<Iterables...>> : public detail::union_iterable_const_adaptor<Iterables...>
{
public:
	typedef detail::union_iterable_const_adaptor<Iterables...> adaptor_base;
	using adaptor_base::s_numTypes;
	using adaptor_base::traits;
	using adaptor_base::const_traits;
	using adaptor_base::tags_t;
	using adaptor_base::const_tags_t;
	using adaptor_base::union_iterable_const_adaptor;
	using adaptor_base::set_current_iterable_index;
	using adaptor_base::get_current_iterable_index;

	TEMPLATE(typename ActionTag)
	REQUIRES(ACTION_TAGS_SUPPORTED(const_traits,ActionTag))
	inline auto perform_action(ActionTag&& i_actionTag) const;

private:
	template<typename ActionTag>
	inline auto perform_action(union_iterable_action<ActionTag> i_actionTag) const;
};

}

#include "ddk_union_iterable_impl.inl"