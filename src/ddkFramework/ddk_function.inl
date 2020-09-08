
namespace ddk
{

template<typename Return, typename ... Types, typename Allocator>
function<Return(Types...),Allocator>::function(std::nullptr_t)
{
}
template<typename Return, typename ... Types, typename Allocator>
template<typename T>
function<Return(Types...),Allocator>::function(T&& i_functor, const Allocator& i_allocator, typename std::enable_if<mpl::is_valid_functor<T>::value>::type*)
: m_allocator(i_allocator)
{
    typedef detail::functor_impl<typename std::remove_const<typename std::remove_reference<T>::type>::type,Return,Types...> Functor;

    if(void* mem = m_allocator.allocate(1,sizeof(Functor)))
    {
        Functor* newFuncImpl = new(mem) Functor(i_functor);

        m_functionImpl = as_shared_reference(newFuncImpl,tagged_reference_counter(&newFuncImpl->m_refCounter,ReferenceAllocationType::Embedded),get_reference_wrapper_deleter(m_allocator));
    }
    else
    {
        throw std::exception{};
    }
}
template<typename Return, typename ... Types, typename Allocator>
function<Return(Types...),Allocator>::function(Return(*i_call)(Types...), const Allocator& i_allocator)
: m_allocator(i_allocator)
{
    typedef detail::free_function_impl<Return,Types...> Functor;

    if(void* mem = m_allocator.allocate(1,sizeof(Functor)))
    {
        Functor* newFuncImpl = new(mem) Functor(i_call);

        m_functionImpl = as_shared_reference(newFuncImpl,tagged_reference_counter(&newFuncImpl->m_refCounter,ReferenceAllocationType::Embedded),get_reference_wrapper_deleter(m_allocator));
    }
    else
    {
        throw std::exception{};
    }
}
template<typename Return, typename ... Types, typename Allocator>
template<typename T>
function<Return(Types...),Allocator>::function(T *i_pRef, Return(T::*i_call)(Types...), const Allocator& i_allocator)
: m_allocator(i_allocator)
{
    typedef detail::relative_function_impl<T,Return,Types...> Functor;

    if(void* mem = m_allocator.allocate(1,sizeof(Functor)))
    {
        Functor* newFuncImpl = new(mem) Functor(i_pRef,i_call);

        m_functionImpl = as_shared_reference(newFuncImpl,tagged_reference_counter(&newFuncImpl->m_refCounter,ReferenceAllocationType::Embedded),get_reference_wrapper_deleter(m_allocator));
    }
    else
    {
        throw std::exception{};
    }
}
template<typename Return, typename ... Types, typename Allocator>
function<Return(Types...),Allocator>& function<Return(Types...),Allocator>::operator=(std::nullptr_t)
{
    m_functionImpl.clean();

    return *this;
}
template<typename Return, typename ... Types, typename Allocator>
bool function<Return(Types...),Allocator>::operator==(std::nullptr_t) const
{
    return m_functionImpl.empty();
}
template<typename Return, typename ... Types, typename Allocator>
bool function<Return(Types...),Allocator>::operator!=(std::nullptr_t) const
{
    return m_functionImpl.empty() == false;
}
template<typename Return, typename ... Types, typename Allocator>
template<typename ... Args>
resolved_function<Return,detail::unresolved_types<tuple<Args...>,Types...>,Allocator> function<Return(Types...),Allocator>::operator()(Args&& ... i_args) const
{
    if(m_functionImpl)
    {
        resolved_function<Return,detail::unresolved_types<tuple<Args...>,Types...>,Allocator> res;

        res.m_functionImpl = m_functionImpl->specialize(res.m_allocator,std::forward<Args>(i_args) ...);

        return res;
    }
    else
    {
        throw std::exception{};
    }
}
template<typename Return, typename ... Types, typename Allocator>
template<typename ... Args>
Return function<Return(Types...),Allocator>::eval(Args&& ... i_args) const
{
    if(m_functionImpl)
    {
        return m_functionImpl->operator()(std::forward<Args>(i_args) ...);
    }
    else
    {
        throw std::exception{};
    }
}

//0 args version
template<typename Return, typename Allocator>
function<Return(),Allocator>::function(std::nullptr_t)
{
}
template<typename Return, typename Allocator>
template<typename T>
function<Return(),Allocator>::function(T&& i_functor, const Allocator& i_allocator, typename std::enable_if<mpl::is_valid_functor<T>::value>::type*)
: m_allocator(i_allocator)
{
    typedef detail::functor_impl<typename std::remove_const<typename std::remove_reference<T>::type>::type,Return> Functor;

    if(void* mem = m_allocator.allocate(1,sizeof(Functor)))
    {
        Functor* newFuncImpl = new Functor(i_functor);

        m_functionImpl = as_shared_reference(newFuncImpl,tagged_reference_counter(&newFuncImpl->m_refCounter,ReferenceAllocationType::Embedded),get_reference_wrapper_deleter(m_allocator));
    }
    else
    {
        throw std::exception{};
    }
}
template<typename Return, typename Allocator>
function<Return(),Allocator>::function(Return(*i_call)(), const Allocator& i_allocator)
: m_allocator(i_allocator)
{
    typedef detail::free_function_impl<Return> Functor;

    if(void* mem = m_allocator.allocate(1,sizeof(Functor)))
    {
        Functor* newFuncImpl = new Functor(i_call);

        m_functionImpl = as_shared_reference(newFuncImpl,tagged_reference_counter(&newFuncImpl->m_refCounter,ReferenceAllocationType::Embedded),get_reference_wrapper_deleter(m_allocator));
    }
    else
    {
        throw std::exception{};
    }
}
template<typename Return, typename Allocator>
template<typename T>
function<Return(),Allocator>::function(T *i_pRef, Return(T::*i_call)(), const Allocator& i_allocator)
: m_allocator(i_allocator)
{
    typedef detail::relative_function_impl<T,Return> Functor;

    if(void* mem = m_allocator.allocate(1,sizeof(Functor)))
    {
        Functor* newFuncImpl = new Functor(i_pRef,i_call);

        m_functionImpl = as_shared_reference(newFuncImpl,tagged_reference_counter(&newFuncImpl->m_refCounter,ReferenceAllocationType::Embedded),get_reference_wrapper_deleter(m_allocator));
    }
    else
    {
        throw std::exception{};
    }
}
template<typename Return, typename Allocator>
function<Return(),Allocator>& function<Return(),Allocator>::operator=(std::nullptr_t)
{
    m_functionImpl.clear();

    return *this;
}
template<typename Return, typename Allocator>
function<Return(),Allocator>::operator Return() const
{
    if(m_functionImpl)
    {
        return m_functionImpl->operator()();
    }
    else
    {
        throw std::exception{};
    }
}
template<typename Return, typename Allocator>
Return function<Return(),Allocator>::operator()() const
{
    if(m_functionImpl)
    {
        return m_functionImpl->operator()();
    }
    else
    {
        throw std::exception{};
    }
}
template<typename Return, typename Allocator>
Return function<Return(),Allocator>::eval() const
{
    if(m_functionImpl)
    {
        return m_functionImpl->operator()();
    }
    else
    {
        throw std::exception{};
    }
}
template<typename Return, typename Allocator>
bool function<Return(),Allocator>::operator==(std::nullptr_t) const
{
    return m_functionImpl.empty();
}
template<typename Return, typename Allocator>
bool function<Return(),Allocator>::operator!=(std::nullptr_t) const
{
    return m_functionImpl.empty() == false;
}

template<typename Object, typename Return, typename ... Types>
function<Return(Types...)> make_function(Object* i_object, Return(Object::*i_funcPtr)(Types...))
{
    return function<Return(Types...)>(i_object, i_funcPtr);
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
function<Return(Types...)> make_function(Object* i_object, Return(Object::*i_funcPtr)(Types...), const Allocator& i_allocator)
{
    return function<Return(Types...),Allocator>(i_object, i_funcPtr);
}
template<typename Return, typename ... Types, typename Allocator>
function<Return(Types...)> make_function(Return(*i_funcPtr)(Types...), const Allocator& i_allocator)
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

    function<Return(Type,Types...)> res(i_object, i_funcPtr);

	return res(std::forward<Arg>(i_arg),std::forward<Args>(i_args) ...);
}
template<typename Return, typename Type, typename ... Types, typename Arg, typename ... Args>
inline resolved_function<Return,detail::unresolved_types<tuple<typename std::enable_if<mpl::is_allocator<Arg>::value == false,Arg>::type,Args...>,Type,Types...>> make_function(Return(*i_funcPtr)(Type,Types...), Arg&& i_arg, Args&& ... i_args)
{
	static_assert(mpl::get_num_types<Types...>::value == mpl::get_num_types<Args...>::value, "Unconsistent number of arguments with number of types");

    function<Return(Type,Types...)> res(i_funcPtr);

	return res(std::forward<Arg>(i_arg),std::forward<Args>(i_args) ...);
}
template<typename Functor, typename Arg, typename ... Args>
resolved_spec_callable<typename std::enable_if<std::is_class<Functor>::value,Functor>::type,system_allocator,typename std::enable_if<mpl::is_allocator<Arg>::value==false,Arg>::type,Args...> make_function(Functor&& i_functor, Arg&& i_arg, Args&& ... i_args)
{
	static_assert(mpl::aqcuire_callable_return_type<Functor>::args_type::size == mpl::get_num_types<Arg,Args...>::value, "Unconsistent number of arguments with number of types");

    typedef resolved_callable<Functor> function_type;

    function_type res(i_functor);

	return res(std::forward<Arg>(i_arg),std::forward<Args>(i_args) ...);
}
template<typename Object, typename Return, typename Type, typename ... Types, typename Allocator, typename Arg, typename ... Args>
resolved_function<Return,detail::unresolved_types<tuple<Args...>,Type,Types...>> make_function(Object* i_object, Return(Object::*i_funcPtr)(Type,Types...), const Allocator& i_allocator, Arg&& i_arg, Args&& ... i_args)
{
	static_assert(mpl::get_num_types<Types...>::value == mpl::get_num_types<Args...>::value, "Unconsistent number of arguments with number of types");

    function<Return(Type,Types...)> res(i_object, i_funcPtr);

	return res(std::forward<Arg>(i_arg),std::forward<Args>(i_args) ...);
}
template<typename Return, typename Type, typename ... Types, typename Allocator, typename Arg, typename ... Args>
inline resolved_function<Return,detail::unresolved_types<tuple<Args...>,Type,Types...>> make_function(Return(*i_funcPtr)(Type,Types...), const Allocator& i_allocator, Arg&& i_arg, Args&& ... i_args)
{
	static_assert(mpl::get_num_types<Types...>::value == mpl::get_num_types<Args...>::value, "Unconsistent number of arguments with number of types");

    function<Return(Type,Types...)> res(i_funcPtr);

	return res(std::forward<Arg>(i_arg),std::forward<Args>(i_args) ...);
}
template<typename Functor, typename Allocator, typename Arg, typename ... Args>
resolved_spec_callable<typename std::enable_if<std::is_class<Functor>::value,Functor>::type,system_allocator,Args...> make_function(Functor&& i_functor, const Allocator& i_allocator, Arg&& i_arg, Args&& ... i_args)
{
	static_assert(typename mpl::aqcuire_callable_return_type<Functor>::args_type::size == mpl::get_num_types<Arg,Args...>::value, "Unconsistent number of arguments with number of types");

    typedef resolved_callable<Functor> function_type;

    function_type res(i_functor);

	return res(std::forward<Arg>(i_arg),std::forward<Args>(i_args) ...);
}
template<typename Return, typename ... Types, typename ... Args>
Return eval(const function<Return(Types...)>& i_function, Args&& ... i_args)
{
    return i_function.getFuncPtr()->operator()(std::forward<Args>(i_args)...);
}

}
