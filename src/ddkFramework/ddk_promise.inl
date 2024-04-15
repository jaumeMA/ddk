#pragma once

#include "ddk_reference_wrapper.h"
#include "ddk_future.inl"
#include "ddk_reference_wrapper.h"

namespace ddk
{

template<typename T>
promise<T>::promise()
: m_sharedState(make_shared_reference<detail::context_private_async_state<T>>())
, m_refCounter(this)
{
	typedef ddk::tagged_pointer<distributed_reference_counter> tagged_reference_counter;

	set_reference_counter(tagged_reference_counter{ &m_refCounter,ddk::ReferenceAllocationType::Embedded });

	m_sharedState->attach(this->distributed_from_this());
}
template<typename T>
promise<T>::promise(promise<T>&& other)
: m_sharedState(std::move(other.m_sharedState))
, m_refCounter(this)
{
	typedef ddk::tagged_pointer<distributed_reference_counter> tagged_reference_counter;

	set_reference_counter(tagged_reference_counter{ &m_refCounter,ddk::ReferenceAllocationType::Embedded });

	m_sharedState->attach(this->distributed_from_this());
}
template<typename T>
TEMPLATE(typename ... Args)
REQUIRED(IS_CONSTRUCTIBLE(value_type,Args...))
promise<T>::promise(Args&& ... i_args)
: m_sharedState(make_shared_reference<detail::context_private_async_state<T>>(std::forward<Args>(i_args)...))
, m_refCounter(this)
{
	typedef ddk::tagged_pointer<distributed_reference_counter> tagged_reference_counter;

	set_reference_counter(tagged_reference_counter{ &m_refCounter,ddk::ReferenceAllocationType::Embedded });

	m_sharedState->attach(this->distributed_from_this());
}
template<typename T>
promise<T>::~promise()
{
	if (m_sharedState)
	{
		m_sharedState->detach();
	}

	DDK_ASSERT(m_refCounter.hasSharedReferences() == false, "Pending shared references while destroying promise");
}
template<typename T>
promise<T>& promise<T>::operator=(promise<T>&& other)
{
	if (detail::private_async_state_shared_ptr<T> _sharedState = std::move(m_sharedState))
	{
		_sharedState->detach();
	}

	if ((m_sharedState = std::move(other.m_sharedState)))
	{
		m_sharedState->attach(this->distributed_from_this());
	}

	return *this;
}
template<typename T>
void promise<T>::set_value(sink_type i_value)
{
	m_sharedState->set_value(i_value);

	m_sharedState->notify_recipients(false);
}
template<typename T>
void promise<T>::set_exception(const async_exception& i_exception)
{
	m_sharedState->set_exception(i_exception);

	m_sharedState->notify_recipients(false);
}
template<typename T>
void promise<T>::clear()
{
	m_sharedState->clear();
}
template<typename T>
future<T> promise<T>::get_future() const
{
	return { m_sharedState,0 };
}
template<typename T>
void promise<T>::wait() const
{
	return m_sharedState->wait();
}
template<typename T>
void promise<T>::wait_for(unsigned int i_period) const
{
	return m_sharedState->wait_for(i_period);
}
template<typename T>
bool promise<T>::ready() const
{
	return m_sharedState->ready();
}
template<typename T>
void promise<T>::signal() const
{
	return m_sharedState->signal();
}
template<typename T>
typename promise<T>::cancel_result promise<T>::cancel()
{
	if (m_sharedState)
	{
		if (m_sharedState->ready())
		{
			return make_error<cancel_result>(CancelErrorCode::CancelAlreadyExecuted);
		}
		else
		{
			m_sharedState->set_exception(async_exception{"Promise has been cancelled by user",AsyncExceptionCode::Cancelled});

			return success;
		}
	}
	else
	{
		return make_error<cancel_result>(CancelErrorCode::CancelNoAsync);
	}
}
template<typename T>
executor_context_lent_ptr promise<T>::get_execution_context()
{
	return lend(m_sharedState);
}
template<typename T>
executor_context_const_lent_ptr promise<T>::get_execution_context() const
{
	return lend(m_sharedState);
}
template<typename T>
allocator_const_lent_ptr promise<T>::get_async_allocator() const
{
	return nullptr;
}

}
