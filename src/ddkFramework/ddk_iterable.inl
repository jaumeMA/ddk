
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
void iterable<Traits>::iterate(const function<void(iterable_value)>& i_try, const function<void(iter::action_result)>& i_finally, const iter::shift_action& i_initialAction)
{
    m_awaitable = await(make_function(m_iterableImpl.get(),&iterable_impl_interface<iterable_base_traits>::iterate_impl,make_function(this,&iterable<Traits>::private_iterate),i_initialAction));

    while(true)
    {
        m_currAction = iterable_base_traits::default_action();

        if(awaited_result<void> res = resume(m_awaitable))
        {
            try
            {
                m_iterableState.apply(m_currAction);

                eval(i_try,make_iterable_value<iterable_value>(*m_iterableValueContainer.template extract<reference>(),make_function(this,&iterable<Traits>::resolve_action),static_cast<iterable_interface&>(*this)));
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
        eval(i_finally,m_iterableState.forward_result());
    }
}
template<typename Traits>
void iterable<Traits>::iterate(const function<void(iterable_const_value)>& i_try, const function<void(iter::action_result)>& i_finally, const iter::shift_action& i_initialAction) const
{
    typedef action(iterable<Traits>::*func_ptr)(const_reference)const;
    static func_ptr privateIteratorFunc = &iterable<Traits>::private_iterate;

    m_awaitable = await(make_function(m_iterableImpl.get(),&iterable_impl_interface<iterable_base_traits>::iterate_impl,make_function(this,&iterable<Traits>::private_iterate),i_initialAction));

    while(true)
    {
        m_currAction = iterable_base_traits::default_action();

        if(awaited_result<void> res = resume(m_awaitable))
        {
            try
            {
                m_iterableState.apply(m_currAction);

                eval(i_try,make_iterable_value<iterable_const_value>(*m_iterableValueContainer.template extract<const_reference>(),make_function(this,&iterable<Traits>::resolve_action),const_cast<iterable_interface&>(static_cast<const iterable_interface&>(*this))));
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
        eval(i_finally,m_iterableState.forward_result());
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
iter::iterable_state& iterable<Traits>::get_state()
{
    return m_iterableState;
}
template<typename Traits>
const iter::iterable_state& iterable<Traits>::get_state() const
{
    return m_iterableState;
}
template<typename Traits>
typename iterable<Traits>::action iterable<Traits>::private_iterate(reference i_value)
{
	iter::action_result actionResult = m_iterableState.forward_result();

	if(actionResult != iter::ActionError::ShiftError && m_currAction.is_base_of<iter::shift_action>())
	{
		const iter::any_action currAction = actionResult.extract();

		//in case of shift action consolidate it against result
		const iter::consolidate_visitor consolidateVisitor(currAction.get_as<iter::shift_action>());

		if(m_currAction.visit(consolidateVisitor))
		{
			//if consolidation has been completed just pass to the another context
			m_iterableValueContainer.template construct<const_reference>(i_value);

			ddk::yield();
		}
	}

	return m_currAction;
}
template<typename Traits>
typename iterable<Traits>::action iterable<Traits>::private_iterate(const_reference i_value) const
{
	iter::action_result actionResult = m_iterableState.forward_result();

	if(actionResult != iter::ActionError::ShiftError && m_currAction.is_base_of<iter::shift_action>())
	{
		const iter::any_action currAction = actionResult.extract();

		//in case of shift action consolidate it against result
		const iter::consolidate_visitor consolidateVisitor(currAction.get_as<iter::shift_action>());

		if(m_currAction.visit(consolidateVisitor))
		{
			//if consolidation has been completed just pass to the another context
			m_iterableValueContainer.template construct<const_reference>(i_value);

			ddk::yield();
		}
	}

	return m_currAction;
}
template<typename Traits>
typename iterable<Traits>::reference iterable<Traits>::resolve_action(const action& i_action)
{
    m_currAction = i_action;

    if (awaited_result<void> res = resume(m_awaitable))
    {
        m_iterableState.apply(m_currAction);

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
        m_iterableState.apply(m_currAction);

        return m_iterableValueContainer.template extract<const_reference>();
    }
    else
    {
        throw suspend_exception{get_current_fiber_id()};
    }
}
template<typename Traits>
bool iterable<Traits>::forward_action(action i_action) const
{
    if(m_currAction != i_action)
    {
        m_currAction = i_action;

        return true;
    }
    else
    {
        return false;
    }
}

}
}
