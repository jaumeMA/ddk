
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

}
