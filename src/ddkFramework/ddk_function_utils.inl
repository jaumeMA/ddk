
#include "ddk_composed_function.h"

namespace ddk
{

template<typename Object, typename Return, typename ... Types>
function<Return(Types...)> make_function(Object* i_object, Return(Object::*i_funcPtr)(Types...))
{
    return function<Return(Types...)>(i_object, i_funcPtr);
}
template<typename Object, typename Return, typename ... Types>
function<Return(Types...)> make_function(const Object* i_object, Return(Object::*i_funcPtr)(Types...)const)
{
    return function<Return(Types...)>(i_object,i_funcPtr);
}
template<typename Return, typename ... Types>
function<Return(Types...)> make_function(Return(*i_funcPtr)(Types...))
{
    return function<Return(Types...)>(i_funcPtr);
}
template<typename Functor>
resolved_callable<typename std::enable_if<std::is_class<Functor>::value,Functor>::type> make_function(Functor&& i_functor)
{
    typedef resolved_callable<Functor> function_type;

    return function_type(i_functor);
}
template<typename Object, typename Return, typename ... Types, typename Allocator>
function<Return(Types...),typename std::enable_if<mpl::is_allocator<Allocator>::value,Allocator>::type> make_function(Object* i_object, Return(Object::*i_funcPtr)(Types...), const Allocator& i_allocator)
{
    return function<Return(Types...),Allocator>(i_object, i_funcPtr);
}
template<typename Object, typename Return, typename ... Types, typename Allocator>
function<Return(Types...),typename std::enable_if<mpl::is_allocator<Allocator>::value,Allocator>::type> make_function(const Object* i_object, Return(Object::*i_funcPtr)(Types...)const, const Allocator& i_allocator)
{
    return function<Return(Types...),Allocator>(i_object,i_funcPtr);
}
template<typename Return, typename ... Types, typename Allocator>
function<Return(Types...),typename std::enable_if<mpl::is_allocator<Allocator>::value,Allocator>::type> make_function(Return(*i_funcPtr)(Types...), const Allocator& i_allocator)
{
    return function<Return(Types...),Allocator>(i_funcPtr);
}
template<typename Functor, typename Allocator>
resolved_callable<typename std::enable_if<std::is_class<Functor>::value,Functor>::type,Allocator> make_function(Functor&& i_functor, const Allocator& i_allocator, typename std::enable_if<mpl::is_allocator<Allocator>::value>::type*)
{
    typedef resolved_callable<Functor,Allocator> function_type;

    return function_type(i_functor,i_allocator);
}
template<typename Object, typename Return, typename Type, typename ... Types, typename Arg, typename ... Args>
resolved_function<Return,detail::unresolved_types<tuple<typename std::enable_if<mpl::is_allocator<Arg>::value == false,Arg>::type,Args...>,Type,Types...>> make_function(Object* i_object, Return(Object::*i_funcPtr)(Type,Types...), Arg&& i_arg, Args&& ... i_args)
{
	static_assert(mpl::get_num_types<Types...>::value == mpl::get_num_types<Args...>::value, "Unconsistent number of arguments with number of types");

    const function<Return(Type,Types...)> res(i_object, i_funcPtr);

	return res(std::forward<Arg>(i_arg),std::forward<Args>(i_args) ...);
}
template<typename Object, typename Return, typename Type, typename ... Types, typename Arg, typename ... Args>
resolved_function<Return,detail::unresolved_types<tuple<typename std::enable_if<mpl::is_allocator<Arg>::value == false,Arg>::type,Args...>,Type,Types...>> make_function(const Object* i_object, Return(Object::*i_funcPtr)(Type,Types...)const, Arg&& i_arg, Args&& ... i_args)
{
	static_assert(mpl::get_num_types<Types...>::value == mpl::get_num_types<Args...>::value, "Unconsistent number of arguments with number of types");

    const function<Return(Type,Types...)> res(i_object,i_funcPtr);

	return res(std::forward<Arg>(i_arg),std::forward<Args>(i_args) ...);
}
template<typename Return, typename Type, typename ... Types, typename Arg, typename ... Args>
inline resolved_function<Return,detail::unresolved_types<tuple<typename std::enable_if<mpl::is_allocator<Arg>::value == false,Arg>::type,Args...>,Type,Types...>> make_function(Return(*i_funcPtr)(Type,Types...), Arg&& i_arg, Args&& ... i_args)
{
	static_assert(mpl::get_num_types<Types...>::value == mpl::get_num_types<Args...>::value, "Unconsistent number of arguments with number of types");

    const function<Return(Type,Types...)> res(i_funcPtr);

	return res(std::forward<Arg>(i_arg),std::forward<Args>(i_args) ...);
}
template<typename Functor, typename Arg, typename ... Args>
resolved_spec_callable<typename std::enable_if<std::is_class<Functor>::value,Functor>::type,system_allocator,typename std::enable_if<mpl::is_allocator<Arg>::value==false,Arg>::type,Args...> make_function(Functor&& i_functor, Arg&& i_arg, Args&& ... i_args)
{
	static_assert(mpl::aqcuire_callable_return_type<Functor>::args_type::size() == mpl::get_num_types<Arg,Args...>::value, "Unconsistent number of arguments with number of types");

    typedef resolved_callable<Functor> function_type;

    const function_type res(i_functor);

	return res(std::forward<Arg>(i_arg),std::forward<Args>(i_args) ...);
}
template<typename Object, typename Return, typename Type, typename ... Types, typename Allocator, typename Arg, typename ... Args>
resolved_function<Return,detail::unresolved_types<tuple<Arg,Args...>,Type,Types...>,Allocator> make_function(Object* i_object, Return(Object::*i_funcPtr)(Type,Types...), const Allocator& i_allocator, Arg&& i_arg, Args&& ... i_args)
{
	static_assert(mpl::get_num_types<Types...>::value == mpl::get_num_types<Args...>::value, "Unconsistent number of arguments with number of types");

    const function<Return(Type,Types...)> res(i_object, i_funcPtr);

	return res(std::forward<Arg>(i_arg),std::forward<Args>(i_args) ...);
}
template<typename Object, typename Return, typename Type, typename ... Types, typename Allocator, typename Arg, typename ... Args>
resolved_function<Return,detail::unresolved_types<tuple<Arg,Args...>,Type,Types...>,Allocator> make_function(const Object* i_object, Return(Object::*i_funcPtr)(Type,Types...)const, const Allocator& i_allocator, Arg&& i_arg, Args&& ... i_args)
{
	static_assert(mpl::get_num_types<Types...>::value == mpl::get_num_types<Args...>::value, "Unconsistent number of arguments with number of types");

    const function<Return(Type,Types...)> res(i_object,i_funcPtr);

	return res(std::forward<Arg>(i_arg),std::forward<Args>(i_args) ...);
}
template<typename Return, typename Type, typename ... Types, typename Allocator, typename Arg, typename ... Args>
inline resolved_function<Return,detail::unresolved_types<tuple<Arg,Args...>,Type,Types...>,Allocator> make_function(Return(*i_funcPtr)(Type,Types...), const Allocator& i_allocator, Arg&& i_arg, Args&& ... i_args)
{
	static_assert(mpl::get_num_types<Types...>::value == mpl::get_num_types<Args...>::value, "Unconsistent number of arguments with number of types");

    const function<Return(Type,Types...)> res(i_funcPtr);

	return res(std::forward<Arg>(i_arg),std::forward<Args>(i_args) ...);
}
template<typename Functor, typename Allocator, typename Arg, typename ... Args>
resolved_spec_callable<typename std::enable_if<std::is_class<Functor>::value,Functor>::type,Allocator,Arg,Args...> make_function(Functor&& i_functor, const Allocator& i_allocator, Arg&& i_arg, Args&& ... i_args)
{
	static_assert(mpl::aqcuire_callable_return_type<Functor>::args_type::size() == mpl::get_num_types<Arg,Args...>::value, "Unconsistent number of arguments with number of types");

    typedef resolved_callable<Functor> function_type;

    const function_type res(i_functor);

	return res(std::forward<Arg>(i_arg),std::forward<Args>(i_args) ...);
}

template<typename Return, typename ... Types, typename Allocator>
function_view<Return(Types...)> lend(const function<Return(Types...),Allocator>& i_function)
{
    return lend(i_function.m_functionImpl);
}
template<typename Return, typename Allocator>
Return eval(const function<Return(),Allocator>& i_function)
{
    if(i_function.m_functionImpl)
    {
        if constexpr (std::is_same<Return,void>::value)
        {
            i_function.m_functionImpl->operator()();
        }
        else
        {
            return i_function.m_functionImpl->operator()();
        }
    }
    else
    {
        throw call_function_exception{"Trying to call empty function"};
    }
}
template<typename Return, typename ... Types, typename Allocator, typename Arg, typename ... Args>
resolved_return_type<Arg,Return> eval(const function<Return(Types...),Allocator>& i_function, Arg&& i_arg, Args&& ... i_args)
{
    if(i_function.m_functionImpl)
    {
        if constexpr (std::is_same<Return,void>::value)
        {
            i_function.m_functionImpl->operator()(std::forward<Arg>(i_arg),std::forward<Args>(i_args) ...);
        }
        else
        {
            return i_function.m_functionImpl->operator()(std::forward<Arg>(i_arg),std::forward<Args>(i_args) ...);
        }
    }
    else
    {
        throw call_function_exception{"Trying to call empty function"};
    }
}
template<typename Return, typename ... Types, typename Allocator, typename ... Args>
Return eval(const function<Return(Types...),Allocator>& i_function, const tuple<Args...>& i_args)
{
    if constexpr (std::is_same<Return,void>::value)
    {
        i_function.eval_tuple(typename mpl::make_sequence<0,mpl::get_num_types<Args...>::value>::type{},i_args);
    }
    else
    {
        return i_function.eval_tuple(typename mpl::make_sequence<0,mpl::get_num_types<Args...>::value>::type{},i_args);
    }
}

template<typename ... Callables>
detail::intersection_function<Callables...> make_intersection(const Callables& ... i_callables)
{
    return detail::intersection_function<Callables...>(i_callables...);
}

template<typename ... Callables>
detail::union_function<Callables...> make_union(const Callables& ... i_callables)
{
    return detail::intersection_function<Callables...>(i_callables...);
}

template<typename ReturnDst, typename ... TypesDst, typename ReturnSrc, typename ... TypesSrc>
detail::composed_function<ReturnDst(TypesDst...),ReturnSrc(TypesSrc...)> make_composition(const function<ReturnDst(TypesDst...)>& i_fuscDst, const function<ReturnSrc(TypesSrc...)>& i_funcSrc)
{
    return detail::composed_function<ReturnDst(TypesDst...),ReturnSrc(TypesSrc...)>(i_fuscDst,i_funcSrc);
}

template<typename ReturnA, typename ... TypesA, typename ReturnB, typename ... TypesB>
detail::intersection_function<function<ReturnA(TypesA...)>,function<ReturnB(TypesB...)>> operator&(const function<ReturnA(TypesA...)>& i_lhs, const function<ReturnB(TypesB...)>& i_rhs)
{
    return detail::resolved_intersection<function<ReturnA(TypesA...)>,function<ReturnB(TypesB...)>>(i_lhs,i_rhs);
}

template<typename ReturnA, typename ... TypesA, typename ReturnB, typename ... TypesB>
detail::union_function<function<ReturnA(TypesA...)>,function<ReturnB(TypesB...)>> operator|(const function<ReturnA(TypesA...)>& i_lhs, const function<ReturnB(TypesB...)>& i_rhs)
{
    return detail::union_function<function<ReturnA(TypesA...)>,function<ReturnB(TypesB...)>>(i_lhs,i_rhs);
}

template<typename ReturnA, typename ... TypesA, typename ReturnB, typename ... TypesB>
function<ReturnA(TypesB...)> operator<<=(const function<ReturnA(TypesA...)>& i_lhs, const function<ReturnB(TypesB...)>& i_rhs)
{
    return make_composition(i_lhs,i_rhs);
}
template<typename ReturnA, typename ... TypesA, typename ... CallablesB>
resolved_function<ReturnA,typename detail::intersection_function<CallablesB...>::callable_args_type> operator<<=(const function<ReturnA(TypesA...)>& i_lhs, const detail::intersection_function<CallablesB...>& i_rhs)
{
    return make_composition(i_lhs,resolved_function<typename detail::intersection_function<CallablesB...>::callable_return_type,typename detail::intersection_function<CallablesB...>::callable_args_type>(i_rhs));
}
template<typename ReturnA, typename ... TypesA, typename ... CallablesB>
resolved_function<ReturnA,typename detail::union_function<CallablesB...>::callable_args_type> operator<<=(const function<ReturnA(TypesA...)>& i_lhs, const detail::union_function<CallablesB...>& i_rhs)
{
    return make_composition(i_lhs,resolved_function<typename detail::union_function<CallablesB...>::callable_return_type,typename detail::union_function<CallablesB...>::callable_args_type>(i_rhs));
}

}
