#pragma once

#include "ddk_tuple.h"
#include "ddk_function_template_helper.h"

namespace ddk
{
namespace detail
{

template<typename Callable, typename ... NestedCallables>
class union_function
{
    typedef typename mpl::aqcuire_callable_return_type<Callable>::return_type callable_return_type;
    typedef typename mpl::aqcuire_callable_return_type<Callable>::args_type callable_args;
    static const size_t s_num_callables = callable_args::size();

public:
    union_function(const Callable& i_callableTransform, const NestedCallables& ... i_args);
    union_function(const union_function<Callable,NestedCallables...>& other) = default;
    union_function(union_function<Callable,NestedCallables...>&& other) = default;
	template<typename ... Args>
    callable_return_type operator()(Args&& ... i_args) const;

private:
    template<int ... Indexs, typename ... Args>
    inline callable_return_type execute(const mpl::sequence<Indexs...>&, Args&& ... i_args) const;

    Callable m_returnTransform;
	tuple<NestedCallables...> m_nestedCallables;
};

}
}

#include "ddk_union_function.inl"