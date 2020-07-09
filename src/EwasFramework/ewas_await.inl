#pragma once

namespace ewas
{

template<typename Return, typename ... Types, typename ... Args, typename Result>
awaitable<Return,Result> await(const std::function<Return(Types...)>& i_function, Args&& ... i_args)
{
	return awaitable<Return,Result>(i_function,std::forward<Args>(i_args) ...);
}
template<typename T, typename baseT, typename Return, typename ... Types, typename ... Args, typename Result>
awaitable<Return,Result> await(const T* i_object, Return(baseT::*i_funcPtr)(Types...)const, Args&& ... i_args)
{
	return awaitable<Return,Result>(std::function<Return(Types...)>(std::bind(i_funcPtr,i_object)),std::forward<Args>(i_args) ...);
}
template<typename T, typename baseT, typename Return, typename ... Types, typename ... Args, typename Result>
awaitable<Return,Result> await(T* i_object, Return(baseT::*i_funcPtr)(Types...), Args&& ... i_args)
{
	return awaitable<Return,Result>(std::function<Return(Types...)>(std::bind(i_funcPtr,i_object)),std::forward<Args>(i_args) ...);
}
template<typename Return, typename ... Types, typename ... Args, typename Result>
awaitable<Return,Result> await(Return(*i_funcPtr)(Types...), Args&& ... i_args)
{
	return awaitable<Return,Result>(std::function<Return(Types...)>(i_funcPtr),std::forward<Args>(i_args) ...);
}
//template<typename Functor, typename ... Args, typename Result>
//awaitable<typename mpl::aqcuire_callable_return_type<decltype(&Functor::operator())>::return_type,Result> await(const Functor&, Args&& ... i_args)
//{
//}

}