
#include "ddk_composed_function.h"

namespace ddk
{

template<typename Object,typename Return,typename ... Types>
detail::relative_function_impl<Object,Return,Types...> make_member_function(Object* i_object,Return(Object::*i_funcPtr)(Types...))
{
	return detail::relative_function_impl<Object,Return,Types...>(i_object,i_funcPtr);
}
template<typename Object,typename Return,typename ... Types>
detail::relative_function_impl<const Object,Return,Types...> make_member_function(const Object* i_object,Return(Object::*i_funcPtr)(Types...)const)
{
	return std::move(detail::relative_function_impl<const Object,Return,Types...>(i_object,i_funcPtr));
}
template<typename Return,typename ... Types>
detail::free_function_impl<Return,Types...> make_free_function(Return(*i_funcPtr)(Types...))
{
	return std::move(detail::free_function_impl<Return,Types...>(i_funcPtr));
}
TEMPLATE(typename Functor)
REQUIRED(IS_CLASS(Functor),IS_CALLABLE(Functor))
inline detail::resolved_functor_impl<Functor> make_functor_function(Functor&& i_functor)
{
	return { std::forward<Functor>(i_functor) };
}

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
TEMPLATE(typename Functor)
REQUIRED(IS_CLASS(Functor))
resolved_callable<Functor> make_function(Functor&& i_functor)
{
    typedef resolved_callable<Functor> function_type;

    return function_type(std::forward<Functor>(i_functor));
}
TEMPLATE(typename Object, typename Return, typename ... Types, typename Allocator)
REQUIRED(IS_ALLOCATOR(Allocator))
function<Return(Types...),Allocator> make_function(Object* i_object, Return(Object::*i_funcPtr)(Types...), const Allocator& i_allocator)
{
    return function<Return(Types...),Allocator>(i_object, i_funcPtr);
}
TEMPLATE(typename Object, typename Return, typename ... Types, typename Allocator)
REQUIRED(IS_ALLOCATOR(Allocator))
function<Return(Types...),Allocator> make_function(const Object* i_object, Return(Object::*i_funcPtr)(Types...)const, const Allocator& i_allocator)
{
    return function<Return(Types...),Allocator>(i_object,i_funcPtr);
}
TEMPLATE(typename Return, typename ... Types, typename Allocator)
REQUIRED(IS_ALLOCATOR(Allocator))
function<Return(Types...),Allocator> make_function(Return(*i_funcPtr)(Types...), const Allocator& i_allocator)
{
    return function<Return(Types...),Allocator>(i_funcPtr);
}
TEMPLATE(typename Functor, typename Allocator)
REQUIRED(IS_CLASS(Functor),IS_ALLOCATOR(Allocator))
resolved_callable<Functor,Allocator> make_function(Functor&& i_functor, const Allocator& i_allocator)
{
    typedef resolved_callable<Functor,Allocator> function_type;

    return function_type(std::forward<Functor>(i_functor),i_allocator);
}
TEMPLATE(typename Object, typename Return, typename Type, typename ... Types, typename Arg, typename ... Args)
REQUIRED(IS_NOT_ALLOCATOR(Arg))
resolved_function<Return,detail::unresolved_types<tuple<Arg,Args...>,Type,Types...>> make_function(Object* i_object, Return(Object::*i_funcPtr)(Type,Types...), Arg&& i_arg, Args&& ... i_args)
{
	static_assert(mpl::get_num_types<Types...>::value == mpl::get_num_types<Args...>::value, "Unconsistent number of arguments with number of types");

    const function<Return(Type,Types...)> res(i_object, i_funcPtr);

	return res(std::forward<Arg>(i_arg),std::forward<Args>(i_args) ...);
}
TEMPLATE(typename Object, typename Return, typename Type, typename ... Types, typename Arg, typename ... Args)
REQUIRED(IS_NOT_ALLOCATOR(Arg))
resolved_function<Return,detail::unresolved_types<tuple<Arg,Args...>,Type,Types...>> make_function(const Object* i_object, Return(Object::*i_funcPtr)(Type,Types...)const, Arg&& i_arg, Args&& ... i_args)
{
	static_assert(mpl::get_num_types<Types...>::value == mpl::get_num_types<Args...>::value, "Unconsistent number of arguments with number of types");

    const function<Return(Type,Types...)> res(i_object,i_funcPtr);

	return res(std::forward<Arg>(i_arg),std::forward<Args>(i_args) ...);
}
TEMPLATE(typename Return, typename Type, typename ... Types, typename Arg, typename ... Args)
REQUIRED(IS_NOT_ALLOCATOR(Arg))
inline resolved_function<Return,detail::unresolved_types<tuple<Arg,Args...>,Type,Types...>> make_function(Return(*i_funcPtr)(Type,Types...), Arg&& i_arg, Args&& ... i_args)
{
	static_assert(mpl::get_num_types<Types...>::value == mpl::get_num_types<Args...>::value, "Unconsistent number of arguments with number of types");

    const function<Return(Type,Types...)> res(i_funcPtr);

	return res(std::forward<Arg>(i_arg),std::forward<Args>(i_args) ...);
}
TEMPLATE(typename Functor, typename Arg, typename ... Args)
REQUIRED(IS_CLASS(Functor),IS_CALLABLE(Functor),IS_NOT_ALLOCATOR(Arg))
resolved_spec_callable<Functor,system_allocator,Arg,Args...> make_function(Functor&& i_functor, Arg&& i_arg, Args&& ... i_args)
{
	static_assert(mpl::aqcuire_callable_return_type<Functor>::args_type::size() == mpl::get_num_types<Arg,Args...>::value, "Unconsistent number of arguments with number of types");

    typedef resolved_callable<Functor> function_type;

    const function_type res(std::forward<Functor>(i_functor));

	return res(std::forward<Arg>(i_arg),std::forward<Args>(i_args) ...);
}
template<typename Object, typename Return, typename Type, typename ... Types, typename Allocator, typename Arg, typename ... Args>
resolved_function<Return,detail::unresolved_types<tuple<Arg,Args...>,Type,Types...>,Allocator> make_function(Object* i_object, Return(Object::*i_funcPtr)(Type,Types...), const Allocator& i_allocator, Arg&& i_arg, Args&& ... i_args)
{
	static_assert(mpl::get_num_types<Types...>::value == mpl::get_num_types<Args...>::value, "Unconsistent number of arguments with number of types");

    const function<Return(Type,Types...),Allocator> res(i_object, i_funcPtr, i_allocator);

	return res(std::forward<Arg>(i_arg),std::forward<Args>(i_args) ...);
}
template<typename Object, typename Return, typename Type, typename ... Types, typename Allocator, typename Arg, typename ... Args>
resolved_function<Return,detail::unresolved_types<tuple<Arg,Args...>,Type,Types...>,Allocator> make_function(const Object* i_object, Return(Object::*i_funcPtr)(Type,Types...)const, const Allocator& i_allocator, Arg&& i_arg, Args&& ... i_args)
{
	static_assert(mpl::get_num_types<Types...>::value == mpl::get_num_types<Args...>::value, "Unconsistent number of arguments with number of types");

    const function<Return(Type,Types...),Allocator> res(i_object,i_funcPtr,i_allocator);

	return res(std::forward<Arg>(i_arg),std::forward<Args>(i_args) ...);
}
template<typename Return, typename Type, typename ... Types, typename Allocator, typename Arg, typename ... Args>
inline resolved_function<Return,detail::unresolved_types<tuple<Arg,Args...>,Type,Types...>,Allocator> make_function(Return(*i_funcPtr)(Type,Types...), const Allocator& i_allocator, Arg&& i_arg, Args&& ... i_args)
{
	static_assert(mpl::get_num_types<Types...>::value == mpl::get_num_types<Args...>::value, "Unconsistent number of arguments with number of types");

    const function<Return(Type,Types...),Allocator> res(i_funcPtr,i_allocator);

	return res(std::forward<Arg>(i_arg),std::forward<Args>(i_args) ...);
}
TEMPLATE(typename Functor, typename Allocator, typename Arg, typename ... Args)
REQUIRED(IS_CLASS(Functor),IS_CALLABLE(Functor))
resolved_spec_callable<Functor,Allocator,Arg,Args...> make_function(Functor&& i_functor, const Allocator& i_allocator, Arg&& i_arg, Args&& ... i_args)
{
	static_assert(mpl::aqcuire_callable_return_type<Functor>::args_type::size() == mpl::get_num_types<Arg,Args...>::value, "Unconsistent number of arguments with number of types");

    typedef resolved_callable<Functor,Allocator> function_type;

    const function_type res(std::forward<Functor>(i_functor),i_allocator);

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
Return eval(const function<Return(Types...),Allocator>& i_function, const function_arguments<Args...>& i_args)
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

template<typename Return,typename Allocator>
Return eval_unsafe(const function<Return(),Allocator>& i_function)
{
	if constexpr(std::is_same<Return,void>::value)
	{
		i_function.m_functionImpl->operator()();
	}
	else
	{
		return i_function.m_functionImpl->operator()();
	}
}
template<typename Return,typename ... Types,typename Allocator,typename Arg,typename ... Args>
resolved_return_type<Arg,Return> eval_unsafe(const function<Return(Types...),Allocator>& i_function,Arg&& i_arg,Args&& ... i_args)
{
	if constexpr(std::is_same<Return,void>::value)
	{
		i_function.m_functionImpl->operator()(std::forward<Arg>(i_arg),std::forward<Args>(i_args) ...);
	}
	else
	{
		return i_function.m_functionImpl->operator()(std::forward<Arg>(i_arg),std::forward<Args>(i_args) ...);
	}
}
template<typename Return,typename ... Types,typename Allocator,typename ... Args>
Return eval_unsafe(const function<Return(Types...),Allocator>& i_function,const function_arguments<Args...>& i_args)
{
	if constexpr(std::is_same<Return,void>::value)
	{
		i_function.m_functionImpl->operator()(std::forward<Arg>(i_arg),std::forward<Args>(i_args) ...);
	}
	else
	{
		return i_function.m_functionImpl->operator()(std::forward<Arg>(i_arg),std::forward<Args>(i_args) ...);
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

TEMPLATE(typename ... Functions)
REQUIRED(ARE_CALLABLES(Functions...))
inline ddk::detail::intersection_function<Functions...> fusion(const Functions& ... i_functions)
{
	return { i_functions ... };
}

TEMPLATE(typename ... Functions)
REQUIRED(ARE_CALLABLES(Functions...))
inline ddk::detail::union_function<Functions...> concat(const Functions& ... i_functions)
{
	return { i_functions ... };
}

}

template<typename ReturnA, typename ... TypesA, typename ReturnB, typename ... TypesB>
ddk::function<ReturnA(TypesB...)> operator<<=(const ddk::function<ReturnA(TypesA...)>& i_lhs, const ddk::function<ReturnB(TypesB...)>& i_rhs)
{
    return ddk::make_composition(i_lhs,i_rhs);
}
template<typename ReturnA, typename ... TypesA, typename ... CallablesB>
ddk::resolved_function<ReturnA,typename ddk::detail::intersection_function<CallablesB...>::callable_args_type> operator<<=(const ddk::function<ReturnA(TypesA...)>& i_lhs, const ddk::detail::intersection_function<CallablesB...>& i_rhs)
{
    return ddk::make_composition(i_lhs,ddk::resolved_function<typename ddk::detail::intersection_function<CallablesB...>::callable_return_type,typename ddk::detail::intersection_function<CallablesB...>::callable_args_type>(i_rhs));
}
template<typename ReturnA, typename ... TypesA, typename ... CallablesB>
ddk::resolved_function<ReturnA,typename ddk::detail::union_function<CallablesB...>::callable_args_type> operator<<=(const ddk::function<ReturnA(TypesA...)>& i_lhs, const ddk::detail::union_function<CallablesB...>& i_rhs)
{
    return ddk::make_composition(i_lhs,ddk::resolved_function<typename ddk::detail::union_function<CallablesB...>::callable_return_type,typename ddk::detail::union_function<CallablesB...>::callable_args_type>(i_rhs));
}
