#pragma once

#include "ddk_tuple.h"
#include "ddk_iterable_impl_interface.h"
#include "ddk_iterable_valued_traits.h"

namespace ddk
{
namespace detail
{

template<typename ... Iterables>
class intersection_iterable_impl : public iterable_impl_interface<intersection_iterable_base_traits<typename Iterables::traits ...>>
{
    static const size_t s_num_iterables = tuple<Iterables...>::size();
    typedef iterable_impl_interface<intersection_iterable_base_traits<typename Iterables::traits ...>> base_t;
    typedef typename intersection_iterable_traits<typename Iterables::traits ...>::iterable_value iterable_value;
    typedef typename intersection_iterable_traits<typename Iterables::traits ...>::iterable_const_value iterable_const_value;

public:
    using typename base_t::reference;
    using typename base_t::const_reference;
    using typename base_t::action;

    intersection_iterable_impl(const Iterables& ... i_iterables);
    intersection_iterable_impl(const tuple<Iterables...>& i_tupleIterable);

    const tuple<Iterables...>& get_iterables() const;
    tuple<Iterables...>& get_iterables();

private:
    void iterate_impl(const function<action(reference)>& i_try, const iter::iterable_state& i_initState) override;
    void iterate_impl(const function<action(const_reference)>& i_try, const iter::iterable_state& i_initState) const override;
    size_t size() const override;
    bool empty() const override;

    template<size_t ... Indexs>
    inline void iterate_impl(const mpl::sequence<Indexs...>&, const function<action(reference)>& i_try, const iter::iterable_state& i_initState);
    template<size_t ... Indexs>
    inline void iterate_impl(const mpl::sequence<Indexs...>&, const function<action(const_reference)>& i_try, const iter::iterable_state& i_initState) const;
    template<size_t Index>
    typename mpl::nth_type_of<Index,Iterables...>::type::reference private_iterate_impl(typename mpl::nth_type_of<Index,Iterables...>::type& i_iterable, const iter::iterable_state& i_initState);
    template<size_t Index>
    typename mpl::nth_type_of<Index,Iterables...>::type::const_reference private_iterate_impl(const typename mpl::nth_type_of<Index,Iterables...>::type& i_iterable, const iter::iterable_state& i_initState) const;

    tuple<Iterables...> m_iterables;
};

}
}

#include "ddk_intersection_iterable_impl.inl"
