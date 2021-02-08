#pragma once

#include "ddk_tuple.h"
#include "ddk_function_template_helper.h"
#include "ddk_tuple_template_helper.h"
#include "ddk_function_arguments.h"

namespace ddk
{
namespace detail
{

template<size_t,typename ...>
struct sequence_composer;

template<size_t Index>
struct sequence_composer<Index>
{
    typedef mpl::type_pack<> type;
};

template<size_t Index, typename CallableArgs, typename ... CallablesArgs>
struct sequence_composer<Index,CallableArgs,CallablesArgs...>
{
private:
    typedef typename mpl::make_sequence<Index,Index+CallableArgs::size()>::type curr_sequence;
    typedef typename sequence_composer<Index+CallableArgs::size(),CallablesArgs...>::type next_composed_sequence;

public:
    typedef typename mpl::merge_type_packs<mpl::type_pack<curr_sequence>,next_composed_sequence>::type type;
};


template<typename Callable, typename ... Callables>
class union_function
{
    static const size_t s_num_callables = mpl::get_num_types<Callables...>() + 1;

    template<typename ... CallablesB>
    friend inline union_function<Callable,Callables...,CallablesB...> operator|(const union_function<Callable,Callables...>& i_lhs, const union_function<CallablesB...>& i_rhs)
    {
        return union_function<Callable,Callables...,CallablesB...>{merge(i_lhs.m_callables,i_rhs.m_callables)};
    }
    template<typename CallableB>
    friend inline union_function<Callable,Callables...,CallableB> operator|(const union_function<Callable,Callables...>& i_lhs, const CallableB& i_rhs)
    {
        return union_function<Callable,Callables...,CallableB>{merge_args(i_lhs.m_callables,i_rhs)};
    }

public:
	struct callable_tag;
    typedef function_arguments<typename mpl::aqcuire_callable_return_type<Callable>::return_type, typename mpl::aqcuire_callable_return_type<Callables>::return_type ...> callable_return_type;
    typedef typename mpl::merge_type_packs<typename mpl::aqcuire_callable_return_type<Callable>::args_type,typename mpl::aqcuire_callable_return_type<Callables>::args_type ...>::type callable_args_type;

    union_function(const Callable& i_callable, const Callables& ... i_callables);
    union_function(const tuple<Callable,Callables...>& i_callables);
    union_function(const union_function<Callable,Callables...>& other) = default;
    union_function(union_function<Callable,Callables...>&& other) = default;
	template<typename ... Args>
    callable_return_type operator()(Args&& ... i_args) const;

private:
    template<size_t ... Indexs, typename ... Args>
    inline callable_return_type execute(const mpl::sequence<Indexs...>&, const tuple<Args...>& i_args) const;
    template<typename CCallable, size_t ... Indexs, typename ... Args>
    inline typename mpl::aqcuire_callable_return_type<CCallable>::return_type execute(const CCallable& i_callable, const mpl::sequence<Indexs...>&, const tuple<Args...>& i_args) const;

	tuple<Callable,Callables...> m_callables;
};

}
}

#include "ddk_union_function.inl"
