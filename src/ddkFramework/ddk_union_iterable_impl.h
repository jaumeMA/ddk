#pragma once

#include "ddk_tuple.h"
#include "ddk_iterable_impl_interface.h"
#include "ddk_iterable_traits.h"
#include "ddk_static_visitor.h"

namespace ddk
{
namespace detail
{

template<typename, typename ...>
struct union_iterable_visitor_type;

template<size_t ... Indexs, typename ... Iterables>
struct union_iterable_visitor_type<mpl::sequence<Indexs...>,Iterables...> : public static_visitor<std::pair<size_t,iter::shift_action>>
{
    static const size_t s_num_iterables = tuple<Iterables...>::size();
    typedef size_t(*size_t_func)(const tuple<Iterables...>&);

public:
    union_iterable_visitor_type(size_t i_currIterableIndex, const tuple<Iterables...>& i_iterables);

	std::pair<size_t,iter::shift_action> visit(const iter::stop_action& i_action) const;
    std::pair<size_t,iter::shift_action> visit(const iter::erase_action& i_action) const;
    std::pair<size_t,iter::shift_action> visit(const iter::add_action& i_action) const;
    std::pair<size_t,iter::shift_action> visit(const iter::go_forward_action& i_action) const;
    std::pair<size_t,iter::shift_action> visit(const iter::go_backward_action& i_action) const;
    std::pair<size_t,iter::shift_action> visit(const iter::shift_action& i_action) const;

private:
    template<size_t Index>
    static size_t iterable_size(const tuple<Iterables...>&);

    size_t m_currIterableIndex;
    const tuple<Iterables...>& m_iterables;
    static const size_t_func s_size_funcs[s_num_iterables];
};

template<typename ... Iterables>
using union_iterable_visitor = union_iterable_visitor_type<typename mpl::make_sequence<0,mpl::get_num_types<Iterables...>::value>::type,Iterables...>;

template<typename ... Iterables>
class union_iterable_impl : public iterable_impl_interface<union_iterable_base_traits<typename Iterables::traits ...>>
{
    static const size_t s_num_iterables = tuple<Iterables...>::size();
    typedef iterable_impl_interface<union_iterable_base_traits<typename Iterables::traits ...>> base_t;
	template<size_t Index, typename ... IIterables>
	friend typename union_iterable_impl<IIterables...>::action navigate(union_iterable_impl<IIterables...>& i_iterable, const function<typename union_iterable_impl<IIterables...>::action(typename union_iterable_impl<IIterables...>::reference)>&, const ddk::iter::shift_action&);
	template<size_t Index, typename ... IIterables>
	friend typename union_iterable_impl<IIterables...>::action navigate(const union_iterable_impl<IIterables...>& i_iterable, const function<typename union_iterable_impl<IIterables...>::action(typename union_iterable_impl<IIterables...>::const_reference)>&, const ddk::iter::shift_action&);

public:
    using typename base_t::reference;
    using typename base_t::const_reference;
    using typename base_t::action;

    union_iterable_impl(const Iterables& ... i_iterables);
    union_iterable_impl(const tuple<Iterables...>& i_tupleIterable);

    const tuple<Iterables...>& get_iterables() const;
    tuple<Iterables...>& get_iterables();

private:
    void iterate_impl(const function<action(reference)>& i_try, const iter::shift_action& i_initialAction) override;
    void iterate_impl(const function<action(const_reference)>& i_try, const iter::shift_action& i_initialAction) const override;
    size_t size() const override;
    bool empty() const override;

    template<size_t ... Indexs>
    inline void iterate_impl(const mpl::sequence<Indexs...>&, const function<action(reference)>& i_try, const iter::shift_action& i_initialAction);
    template<size_t ... Indexs>
    inline void iterate_impl(const mpl::sequence<Indexs...>&, const function<action(const_reference)>& i_try, const iter::shift_action& i_initialAction) const;

    tuple<Iterables...> m_iterables;
};

}
}

#include "ddk_union_iterable_impl.inl"
