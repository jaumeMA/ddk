#pragma once

#include "ddk_function_template_helper.h"
#include "ddk_function_arguments.h"

namespace ddk
{
namespace detail
{

template<typename ...>
class intersection_function;

template<typename Callable, typename ... Callables>
class intersection_function<Callable,Callables...>
{
    static const size_t s_num_callables = 1 + mpl::get_num_types<Callables...>::value;

    template<typename ... CallablesB>
    friend inline intersection_function<Callable,Callables...,CallablesB...> operator&(const intersection_function<Callable,Callables...>& i_lhs, const intersection_function<CallablesB...>& i_rhs)
    {
        return intersection_function<Callable,Callables...,CallablesB...>{merge(i_lhs.m_callables,i_rhs.m_callables)};
    }
    template<typename CallableB>
    friend inline intersection_function<Callable,Callables...,CallableB> operator&(const intersection_function<Callable,Callables...>& i_lhs, const CallableB& i_rhs)
    {
        return intersection_function<Callable,Callables...,CallableB>{merge_args(i_lhs.m_callables,i_rhs)};
    }

public:
    typedef function_arguments<typename mpl::aqcuire_callable_return_type<Callable>::return_type, typename mpl::aqcuire_callable_return_type<Callables>::return_type ...> callable_return_type;
    typedef typename mpl::aqcuire_callable_return_type<Callable>::args_type callable_args_type;

    intersection_function(const Callable& i_callable, const Callables& ... i_callables);
    intersection_function(const tuple<Callable,Callables...>& i_callables);
    intersection_function(tuple<Callable,Callables...>&& i_callables);
    intersection_function(const intersection_function<Callable,Callables...>& other);
    intersection_function(intersection_function<Callable,Callables...>&& other);
	template<typename ... Args>
    callable_return_type operator()(Args&& ... i_args) const;

private:
    template<size_t ... Indexs, typename ... Args>
    inline callable_return_type execute(const mpl::sequence<Indexs...>&, Args&& ... i_args) const;

	const tuple<Callable,Callables...> m_callables;
};

template<typename...>
struct resolve_intersection;

template<typename CallableA, typename CallableB>
struct resolve_intersection<CallableA,CallableB>
{
    typedef intersection_function<CallableA,CallableB> type;
};
template<typename CallableA, typename ... CallablesB>
struct resolve_intersection<CallableA,intersection_function<CallablesB...>>
{
    typedef intersection_function<CallableA,CallablesB...> type;
};
template<typename ... CallablesA, typename CallableB>
struct resolve_intersection<intersection_function<CallablesA...>,CallableB>
{
    typedef intersection_function<CallablesA...,CallableB> type;
};
template<typename ... CallablesA, typename ... CallablesB>
struct resolve_intersection<intersection_function<CallablesA...>,intersection_function<CallablesB...>>
{
    typedef intersection_function<CallablesA...,CallablesB...> type;
};

template<typename ... Callables>
using resolved_intersection = typename resolve_intersection<Callables...>::type;

}
}

#include "ddk_intersection_function.inl"
