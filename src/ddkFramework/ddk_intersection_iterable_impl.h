#pragma once

#include "ddk_tuple.h"
#include "ddk_iterable_impl_interface.h"
#include "ddk_iterable_valued_traits.h"

namespace ddk
{
namespace detail
{

template<typename Iterable, typename ... Iterables>
class intersection_iterable_impl : public iterable_impl_interface<intersection_iterable_base_traits<typename Iterable::traits,typename Iterables::traits ...>>
{
    static const size_t s_num_iterables = tuple<Iterable,Iterables...>::size();
    typedef iterable_impl_interface<intersection_iterable_base_traits<typename Iterable::traits,typename Iterables::traits ...>> base_t;
    typedef typename intersection_iterable_traits<typename Iterable::traits,typename Iterables::traits ...>::iterable_value iterable_value;
    typedef typename intersection_iterable_traits<typename Iterable::traits,typename Iterables::traits ...>::iterable_const_value iterable_const_value;

public:
    using typename base_t::reference;
    using typename base_t::const_reference;
    using typename base_t::action;

    intersection_iterable_impl(const Iterable& i_iterable, const Iterables& ... i_iterables);
    intersection_iterable_impl(const tuple<Iterable,Iterables...>& i_tupleIterable);

    const tuple<Iterable,Iterables...>& get_iterables() const;
    tuple<Iterable,Iterables...>& get_iterables();

private:
    void iterate_impl(const function<action(reference)>& i_try, const iter::iterable_state& i_initState) override;
    void iterate_impl(const function<action(const_reference)>& i_try, const iter::iterable_state& i_initState) const override;
    size_t size() const override;
    bool empty() const override;

    template<size_t Index, typename ... References>
    inline void iterate_impl(const function<action(reference)>& i_try, const iter::iterable_state& i_initState, typename mpl::nth_type_of<Index,Iterable,Iterables...>::type::iterable_value i_iterableValue, References ... i_references);
    template<size_t Index, typename ... References>
    inline void iterate_impl(const function<action(const_reference)>& i_try, const iter::iterable_state& i_initState, typename mpl::nth_type_of<Index,Iterable,Iterables...>::type::iterable_const_value i_iterableValue, References ... i_references) const;

    tuple<Iterable,Iterables...> m_iterables;
};

}
}

#include "ddk_intersection_iterable_impl.inl"
