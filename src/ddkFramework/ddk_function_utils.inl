
namespace ddk
{

template<typename Object, typename Return, typename ... Types>
function<Return(Types...)> make_function(Object* i_object, Return(Object::*i_funcPtr)(Types...))
{
    return function<Return(Types...)>(i_object, i_funcPtr);
}
template<typename Object, typename Return, typename ... Types>
function<Return(Types...)> make_function(Object* i_object, Return(Object::*i_funcPtr)(Types...)const)
{
    return function<Return(Types...)>(i_object, reinterpret_cast<Return(Object::*)(Types...)>(i_funcPtr));
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
function<Return(Types...),typename std::enable_if<mpl::is_allocator<Allocator>::value,Allocator>::type> make_function(Object* i_object, Return(Object::*i_funcPtr)(Types...)const, const Allocator& i_allocator)
{
    return function<Return(Types...),Allocator>(i_object, reinterpret_cast<Return(Object::*)(Types...)>(i_funcPtr));
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
resolved_function<Return,detail::unresolved_types<tuple<typename std::enable_if<mpl::is_allocator<Arg>::value == false,Arg>::type,Args...>,Type,Types...>> make_function(Object* i_object, Return(Object::*i_funcPtr)(Type,Types...)const, Arg&& i_arg, Args&& ... i_args)
{
	static_assert(mpl::get_num_types<Types...>::value == mpl::get_num_types<Args...>::value, "Unconsistent number of arguments with number of types");

    const function<Return(Type,Types...)> res(i_object, reinterpret_cast<Return(Object::*)(Types...)>(i_funcPtr));

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
resolved_function<Return,detail::unresolved_types<tuple<Arg,Args...>,Type,Types...>,Allocator> make_function(Object* i_object, Return(Object::*i_funcPtr)(Type,Types...)const, const Allocator& i_allocator, Arg&& i_arg, Args&& ... i_args)
{
	static_assert(mpl::get_num_types<Types...>::value == mpl::get_num_types<Args...>::value, "Unconsistent number of arguments with number of types");

    const function<Return(Type,Types...)> res(i_object, reinterpret_cast<Return(Object::*)(Types...)>(i_funcPtr));

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

template<typename Return, typename ... Types, typename ... Args>
Return eval(const function<Return(Types...)>& i_function, Args&& ... i_args)
{
    return i_function.getFuncPtr()->operator()(std::forward<Args>(i_args)...);
}

template<typename Return, typename ... Types, typename Allocator>
function_view<Return(Types...)> lend(const function<Return(Types...),Allocator>& i_function)
{
    return lend(i_function.m_functionImpl);
}

}
