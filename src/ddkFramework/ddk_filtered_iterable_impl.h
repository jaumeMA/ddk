#pragma once

#include "ddk_iterable_impl_interface.h"
#include "ddk_function.h"

namespace ddk
{
namespace detail
{

template<typename Function>
class iterable_filter
{
public:
	iterable_filter(const Function& i_filter);

	Function get_filter() const;

private:
	const Function m_filter;
};

template<typename Iterable, typename Filter>
class filtered_iterable_impl : public iterable_impl_interface<typename Iterable::traits>
{
    friend inline auto deduce_adaptor(const filtered_iterable_impl& i_iterableImpl)
    {
        return deduce_adaptor(i_iterableImpl.m_iterable);
    }

public:
    typedef typename Iterable::traits traits;
    typedef typename traits::value_type value_type;
    typedef typename traits::reference reference;
    typedef typename traits::const_reference const_reference;

    TEMPLATE(typename IIterable,typename FFilter)
    REQUIRES(IS_CONSTRUCTIBLE(Iterable,IIterable),IS_CONSTRUCTIBLE(Filter,FFilter))
    filtered_iterable_impl(IIterable&& i_iterable, FFilter&& i_filter);

    TEMPLATE(typename Function,typename Action)
    REQUIRES(IS_CALLABLE_BY(Function,reference))
    iterable_result iterate_impl(Function&& i_try,const Action& i_initialAction);
    TEMPLATE(typename Function,typename Action)
    REQUIRES(IS_CALLABLE_BY(Function,const_reference))
    iterable_result iterate_impl(Function&& i_try,const Action& i_initialAction) const;

private:
    Iterable m_iterable;
    const Filter m_filter;
};

}
}

#include "ddk_filtered_iterable_impl.inl"
