#pragma once

#include "ddk_tuple.h"
#include "ddk_function_template_helper.h"

namespace ddk
{
namespace detail
{

template<typename Callable, typename ... NestedCallables>
class intersection_function
{
    typedef typename mpl::aqcuire_callable_return_type<Callable>::return_type callable_return_type;
    typedef typename mpl::aqcuire_callable_return_type<Callable>::args_type callable_args;
    static const size_t s_num_callables = callable_args::size();

public:
    intersection_function(const Callable& i_callableTransform, const NestedCallables& ... i_args);
    intersection_function(const Callable& i_callableTransform, const tuple<NestedCallables...>& i_callables);
    intersection_function(const intersection_function<Callable,NestedCallables...>& other) = default;
    intersection_function(intersection_function<Callable,NestedCallables...>&& other) = default;
	template<typename ... Args>
    callable_return_type operator()(Args&& ... i_args) const;

private:
	operator bool() const;
    template<int ... Indexs, typename ... Args>
    inline callable_return_type execute(const mpl::sequence<Indexs...>&, Args&& ... i_args) const;

    const Callable m_returnTransform;
	const tuple<NestedCallables...> m_nestedCallables;
};

template<typename ... Callables>
struct intersected_functions
{
public:
	intersected_functions(const Callables& ... i_callables);

	const tuple<Callables...>& get_callables() const;

private:
	const tuple<Callables...> m_nestedCallables;
};

}

template<typename Callable, typename ... Callables>
detail::intersection_function<Callable,Callables...> operator<=(const Callable& i_callable, const detail::intersected_functions<Callables...>& i_nestedCallable);

}

#include "ddk_intersection_function.inl"