
#include "ddk_function_exceptions.h"
#include "ddk_allocator_exceptions.h"

namespace ddk
{

template<typename Return, typename ... Types, typename Allocator>
template<typename AAllocator>
function<Return(Types...),Allocator>::function(const function<Return(Types...),AAllocator>& other)
: m_functionImpl(other.m_functionImpl)
, m_allocator(other.m_allocator)
{
}
template<typename Return, typename ... Types, typename Allocator>
template<typename AAllocator>
function<Return(Types...),Allocator>::function(function<Return(Types...),AAllocator>&& other)
: m_functionImpl(std::move(other.m_functionImpl))
, m_allocator(other.m_allocator)
{
}
template<typename Return, typename ... Types, typename Allocator>
function<Return(Types...),Allocator>::function(std::nullptr_t)
{
}
template<typename Return, typename ... Types, typename Allocator>
TEMPLATE(typename T)
REQUIRED
function<Return(Types...),Allocator>::function(T&& i_functor, const Allocator& i_allocator)
: m_allocator(i_allocator)
{
    typedef detail::functor_impl<typename std::remove_const<typename std::remove_reference<T>::type>::type,Return,Types...> Functor;

    if(void* mem = m_allocator.allocate(1,sizeof(Functor)))
    {
        Functor* newFuncImpl = new(mem) Functor(std::forward<T>(i_functor));

        m_functionImpl = as_shared_reference(newFuncImpl,get_reference_wrapper_deleter<Functor>(m_allocator));
    }
    else
    {
        throw bad_allocation_exception{"Could not allocate function specialization"};
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

        m_functionImpl = as_shared_reference(newFuncImpl,get_reference_wrapper_deleter<Functor>(m_allocator));
    }
    else
    {
        throw bad_allocation_exception{"Could not allocate function specialization"};
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

        m_functionImpl = as_shared_reference(newFuncImpl,get_reference_wrapper_deleter<Functor>(m_allocator));
    }
    else
    {
        throw bad_allocation_exception{"Could not allocate function specialization"};
    }
}
template<typename Return, typename ... Types, typename Allocator>
template<typename T>
function<Return(Types...),Allocator>::function(const T *i_pRef, Return(T::*i_call)(Types...)const, const Allocator& i_allocator)
{
    typedef detail::relative_function_impl<const T,Return,Types...> Functor;

    if(void* mem = m_allocator.allocate(1,sizeof(Functor)))
    {
        Functor* newFuncImpl = new(mem) Functor(i_pRef,i_call);

        m_functionImpl = as_shared_reference(newFuncImpl,get_reference_wrapper_deleter<Functor>(m_allocator));
    }
    else
    {
        throw bad_allocation_exception{"Could not allocate function specialization"};
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
template<typename Return,typename ... Types,typename Allocator>
template<typename ... Args>
Return function<Return(Types...),Allocator>::inline_eval(Args&& ... i_args) const
{
	if(m_functionImpl)
	{
		if constexpr (std::is_same<Return,void>::value)
		{
			m_functionImpl->operator()(std::forward<Args>(i_args) ...);
		}
		else
		{
			return m_functionImpl->operator()(std::forward<Args>(i_args) ...);
		}
	}
	else
	{
		throw call_function_exception{ "Trying to call empty function" };
	}
}
template<typename Return, typename ... Types, typename Allocator>
template<typename ... Args>
resolved_function<Return,detail::unresolved_types<tuple<Args...>,Types...>,Allocator> function<Return(Types...),Allocator>::operator()(Args&& ... i_args) const
{
    if(m_functionImpl)
    {
        resolved_function<Return,detail::unresolved_types<tuple<Args...>,Types...>,Allocator> res;

        res.m_functionImpl = m_functionImpl->specialize(m_allocator,std::forward<Args>(i_args) ...);

        return res;
    }
    else
    {
        throw call_function_exception{"Trying to call empty function"};
    }
}
template<typename Return, typename ... Types, typename Allocator>
template<size_t ... Indexs, typename ... Args>
Return function<Return(Types...),Allocator>::eval_tuple(const mpl::sequence<Indexs...>&, const tuple<Args...>& i_args) const
{
    if(m_functionImpl)
    {
        if constexpr (std::is_same<Return,void>::value)
        {
            m_functionImpl->operator()(i_args.template get<Indexs>() ...);
        }
        else
        {
            return m_functionImpl->operator()(i_args.template get<Indexs>() ...);
        }
    }
    else
    {
        throw call_function_exception{"Trying to call empty function"};
    }
}

//0 args version
template<typename Return, typename Allocator>
template<typename AAllocator>
function<Return(),Allocator>::function(const function<Return(),AAllocator>& other)
: m_functionImpl(other.m_functionImpl)
, m_allocator(other.m_allocator)
{
}
template<typename Return, typename Allocator>
template<typename AAllocator>
function<Return(),Allocator>::function(function<Return(),AAllocator>&& other)
: m_functionImpl(std::move(other.m_functionImpl))
, m_allocator(other.m_allocator)
{
}
template<typename Return, typename Allocator>
function<Return(),Allocator>::function(std::nullptr_t)
{
}
template<typename Return, typename Allocator>
TEMPLATE(typename T)
REQUIRED
function<Return(),Allocator>::function(T&& i_functor, const Allocator& i_allocator)
: m_allocator(i_allocator)
{
    typedef detail::functor_impl<typename std::remove_const<typename std::remove_reference<T>::type>::type,Return> Functor;

    if(void* mem = m_allocator.allocate(1,sizeof(Functor)))
    {
        Functor* newFuncImpl = new Functor(std::forward<T>(i_functor));

        m_functionImpl = as_shared_reference(newFuncImpl,get_reference_wrapper_deleter<Functor>(m_allocator));
    }
    else
    {
        throw bad_allocation_exception{"Could not allocate function specialization"};
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

        m_functionImpl = as_shared_reference(newFuncImpl,get_reference_wrapper_deleter<Functor>(m_allocator));
    }
    else
    {
        throw bad_allocation_exception{"Could not allocate function specialization"};
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

        m_functionImpl = as_shared_reference(newFuncImpl,get_reference_wrapper_deleter<Functor>(m_allocator));
    }
    else
    {
        throw bad_allocation_exception{"Could not allocate function specialization"};
    }
}
template<typename Return, typename Allocator>
template<typename T>
function<Return(),Allocator>::function(const T *i_pRef, Return(T::*i_call)()const, const Allocator& i_allocator)
{
    typedef detail::relative_function_impl<const T,Return> Functor;

    if(void* mem = m_allocator.allocate(1,sizeof(Functor)))
    {
        Functor* newFuncImpl = new Functor(i_pRef,i_call);

        m_functionImpl = as_shared_reference(newFuncImpl,get_reference_wrapper_deleter<Functor>(m_allocator));
    }
    else
    {
        throw bad_allocation_exception{"Could not allocate function specialization"};
    }
}
template<typename Return, typename Allocator>
function<Return(),Allocator>& function<Return(),Allocator>::operator=(std::nullptr_t)
{
    m_functionImpl.clear();

    return *this;
}
template<typename Return,typename Allocator>
Return function<Return(),Allocator>::inline_eval() const
{
	if(m_functionImpl)
	{
		if constexpr(std::is_same<Return,void>::value)
		{
			m_functionImpl->operator()();
		}
		else
		{
			return m_functionImpl->operator()();
		}
	}
	else
	{
		throw call_function_exception{ "Trying to call empty function" };
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
template<typename Return, typename Allocator>
Return function<Return(),Allocator>::eval_tuple(const mpl::sequence<>&) const
{
    if(m_functionImpl)
    {
        if constexpr (std::is_same<Return,void>::value)
        {
            return m_functionImpl->operator()();
        }
        else
        {
            return m_functionImpl->operator()();
        }
    }
    else
    {
        throw call_function_exception{"Trying to call empty function"};
    }
}

}
