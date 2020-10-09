
#include "ddk_await.h"
#include "ddk_transformed_iterable_action_impl.h"

namespace ddk
{
namespace detail
{

template<typename Traits>
iterable<Traits>::iterable(iterable_impl_shared_ref<iterable_base_traits> i_iterableImpl)
: m_iterableImpl(std::move(i_iterableImpl))
{
}
template<typename Traits>
iterable<Traits>::iterable(const iterable& other)
: m_iterableImpl(other.m_iterableImpl)
{
}
template<typename Traits>
iterable<Traits>::iterable(iterable&& other)
: m_iterableImpl(std::move(other.m_iterableImpl))
{
}
template<typename Traits>
template<typename TTraits>
iterable<Traits>::iterable(const iterable<TTraits>& other)
: m_iterableImpl(make_iterable_impl<transformed_iterable_action_impl<Traits,TTraits>>(other.m_iterableImpl,[](const typename TTraits::action& i_action){ return action(i_action); }))
{
}
template<typename Traits>
template<typename TTraits>
iterable<Traits>::iterable(iterable<TTraits>&& other)
: m_iterableImpl(make_iterable_impl<transformed_iterable_action_impl<Traits,TTraits>>(std::move(other.m_iterableImpl),[](const typename TTraits::action& i_action) -> typename Traits::action { return typename Traits::action(i_action); }))
{
}
template<typename Traits>
void iterable<Traits>::iterate(const function<void(iterable_value)>& i_try, const function<void()>& i_finally, const iter::iterable_state& i_initState)
{
    m_awaitable = await(make_function(m_iterableImpl.get(),&iterable_impl_interface<iterable_base_traits>::iterate_impl,make_function(this,&iterable<Traits>::private_iterate),i_initState));

    while(true)
    {
        m_currAction = iterable_base_traits::default_action();

        if(awaited_result<void> res = resume(m_awaitable))
        {
            try
            {
                eval(i_try,make_iterable_value<iterable_value>(*m_iterableValueContainer.template extract<reference>(),make_function(this,&iterable<Traits>::resolve_action)));
            }
            catch(const suspend_exception&)
            {
                break;
            }
        }
        else
        {
            break;
        }
    }

    if(i_finally != nullptr)
    {
        eval(i_finally);
    }
}
template<typename Traits>
void iterable<Traits>::iterate(const function<void(iterable_const_value)>& i_try, const function<void()>& i_finally, const iter::iterable_state& i_initState) const
{
    typedef action(iterable<Traits>::*func_ptr)(const_reference)const;
    static func_ptr privateIteratorFunc = &iterable<Traits>::private_iterate;

    m_awaitable = await(make_function(m_iterableImpl.get(),&iterable_impl_interface<iterable_base_traits>::iterate_impl,make_function(this,&iterable<Traits>::private_iterate),i_initState));

    while(true)
    {
        m_currAction = iterable_base_traits::default_action();

        if(awaited_result<void> res = resume(m_awaitable))
        {
            try
            {
                eval(i_try,make_iterable_value<iterable_const_value>(*m_iterableValueContainer.template extract<const_reference>(),make_function(this,&iterable<Traits>::resolve_action)));
            }
            catch(const suspend_exception&)
            {
                break;
            }
        }
        else
        {
            break;
        }
    }

    if(i_finally != nullptr)
    {
        eval(i_finally);
    }
}
template<typename Traits>
bool iterable<Traits>::operator==(const std::nullptr_t&) const
{
    return m_iterableImpl == nullptr;
}
template<typename Traits>
bool iterable<Traits>::operator!=(const std::nullptr_t&) const
{
    return m_iterableImpl != nullptr;
}
template<typename Traits>
size_t iterable<Traits>::size() const
{
    return m_iterableImpl->size();
}
template<typename Traits>
bool iterable<Traits>::empty() const
{
    return m_iterableImpl->empty();
}
template<typename Traits>
typename iterable<Traits>::action iterable<Traits>::private_iterate(reference i_value)
{
    m_iterableValueContainer.template construct<reference>(i_value);

    ddk::yield();

    return m_currAction;
}
template<typename Traits>
typename iterable<Traits>::action iterable<Traits>::private_iterate(const_reference i_value) const
{
    m_iterableValueContainer.template construct<const_reference>(i_value);

    ddk::yield();

    return m_currAction;
}
template<typename Traits>
typename iterable<Traits>::reference iterable<Traits>::resolve_action(const action& i_action)
{
    m_currAction = i_action;

    if (awaited_result<void> res = resume(m_awaitable))
    {
        return m_iterableValueContainer.template extract<reference>();
    }
    else
    {
        throw suspend_exception{get_current_fiber_id()};
    }
}
template<typename Traits>
typename iterable<Traits>::const_reference iterable<Traits>::resolve_action(const action& i_action) const
{
    m_currAction = i_action;

    if (awaited_result<void> res = resume(m_awaitable))
    {
        return m_iterableValueContainer.template extract<const_reference>();
    }
    else
    {
        throw suspend_exception{get_current_fiber_id()};
    }
}

}
}
