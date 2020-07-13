#pragma once

#include "ddk_async_message_loop.h"
#include "ddk_async_message_queue.h"
#include "ddk_async_message.h"
#include "ddk_reference_wrapper.h"
#include "ddk_intrusive_node.h"
#include "ddk_thread_executor.h"
#include "ddk_signal_connector.h"

#ifdef _WIN32

#pragma warning ( push )

#pragma warning (disable:4355)

#endif

namespace ddk
{

const unsigned int k_defaultProcessTimeInMs = 10 * 1000;

template<typename>
class async_signal;

template<typename Return,typename ... Types>
class async_signal<Return(Types...)> : protected detail::signal_connector
{
	typedef signal_functor<Return(Types...)> signal_functor_t;
	typedef detail::async_builtin_message<typename std::remove_const<typename std::remove_reference<Types>::type>::type ...> builtn_message_type;

public:
	async_signal(unsigned int i_updateTime = k_defaultProcessTimeInMs)
	: m_id(reinterpret_cast<size_t>(this))
	, m_msgLoop(m_id)
	, m_msgQueue(make_executor<thread_polling_executor>(i_updateTime))
	{
	}
	async_signal(thread_executor_unique_ref i_executor)
	: m_id(reinterpret_cast<size_t>(this))
	, m_msgLoop(m_id)
	, m_msgQueue(std::move(i_executor))
	{
	}
    template<typename T, typename baseT, typename ... Args>
    detail::connection_base& connect(T *object, void (baseT::*hook)(Args...)) const
	{
		static_assert(std::is_base_of<baseT,T>::value,"There is a mismatch in method and object types!");

		const std::function<void(Types...)> call = [object,hook](Args&& ... i_args) mutable { (object->*hook)(std::forward<Args>(i_args)...); };

		return m_msgLoop.connect(m_callers.push(call,static_cast<const detail::signal_connector&>(*this)),ddk::lend(m_msgQueue));
	}
    template<typename T, typename baseT, typename MessageType, typename ... Args>
    detail::connection_base& connect(T *object, void (baseT::*hook)(Args...), lent_reference_wrapper<async_attachable_message_queue<MessageType>> i_messageQueue) const
	{
		static_assert(std::is_base_of<baseT,T>::value,"There is a mismatch in method and object types!");

		const std::function<void(Types...)> call = [object,hook](Args&& ... i_args) mutable { (object->*hook)(std::forward<Args>(i_args)...); };

		return m_msgLoop.connect(m_callers.push(call,static_cast<const detail::signal_connector&>(*this)),i_messageQueue);
	}
	template<typename T, typename baseT, typename ... Args>
	detail::connection_base& connect(T *object, void (baseT::*hook)(Args...) const) const
	{
		static_assert(std::is_base_of<baseT, T>::value, "There is a mismatch in method and object types!");

		const std::function<void(Types...)> call = [object,hook](Args&& ... i_args) mutable { (object->*hook)(std::forward<Args>(i_args)...); };

		return m_msgLoop.connect(m_callers.push(call,static_cast<const detail::signal_connector&>(*this)),ddk::lend(m_msgQueue));
	}
	template<typename T, typename baseT, typename MessageType, typename ... Args>
	detail::connection_base& connect(T *object, void (baseT::*hook)(Args...) const, lent_reference_wrapper<async_attachable_message_queue<MessageType>> i_messageQueue) const
	{
		static_assert(std::is_base_of<baseT, T>::value, "There is a mismatch in method and object types!");

		const std::function<void(Types...)> call = [object,hook](Args&& ... i_args) mutable { (object->*hook)(std::forward<Args>(i_args)...); };

		return m_msgLoop.connect(m_callers.push(call,static_cast<const detail::signal_connector&>(*this)),i_messageQueue);
	}
	template<typename ... Args>
    detail::connection_base& connect(void (*hook)(Args...)) const
	{
		const std::function<void(Types...)> call = [hook](Args&& ... i_args) mutable { (*hook)(std::forward<Args>(i_args)...); };

		return m_msgLoop.connect(m_callers.push(call,static_cast<const detail::signal_connector&>(*this)),ddk::lend(m_msgQueue));
	}
	template<typename MessageType, typename ... Args>
    detail::connection_base& connect(void (*hook)(Types...), lent_reference_wrapper<async_attachable_message_queue<MessageType>> i_messageQueue) const
	{
		const std::function<void(Types...)> call = [hook](Args&& ... i_args) mutable { (*hook)(std::forward<Args>(i_args)...); };

		return m_msgLoop.connect(m_callers.push(call,static_cast<const detail::signal_connector&>(*this)),i_messageQueue);
	}
	template<typename Functor>
	detail::connection_base& connect(const Functor& i_functor) const
	{
		const std::function<Return(Types...)> call(i_functor);

		return m_msgLoop.connect(m_callers.push(call,static_cast<const detail::signal_connector&>(*this)),ddk::lend(m_msgQueue));
	}
	template<typename Functor, typename MessageType>
	detail::connection_base& connect(const Functor& i_functor, lent_reference_wrapper<async_attachable_message_queue<MessageType>> i_messageQueue) const
	{
		const std::function<Return(Types...)> call(i_functor);

		return m_msgLoop.connect(m_callers.push(call,static_cast<const detail::signal_connector&>(*this)),i_messageQueue);
	}
	void disconnect()
	{
		m_msgLoop.clear();
		m_callers.clear();
	}
	template<typename ... Args>
	void execute(Args&& ... i_args) const
	{
		m_msgLoop.push_message(builtn_message_type(m_id,std::forward<Args>(i_args) ...));
	}

private:
	void disconnect(const detail::connection_base& i_base) const override final
	{
		typedef typename linked_list<detail::intrusive_node<signal_functor_t>>::iterator iterator;
		iterator itCaller = std::find_if(m_callers.begin(),m_callers.end(),[&i_base](const detail::intrusive_node<signal_functor_t>& i_functor){ return i_functor->get_id() == i_base.get_id(); });

		if(itCaller != m_callers.end())
		{
			m_msgLoop.disconnect((*itCaller).get_value());

			m_callers.erase(std::move(itCaller));
		}
	}

	const sender_id m_id;
	mutable linked_list<detail::intrusive_node<signal_functor_t>> m_callers;
	mutable async_message_loop<builtn_message_type> m_msgLoop;
	mutable async_attachable_message_queue<builtn_message_type> m_msgQueue;
};

}

#ifdef _WIN32

#pragma warning ( pop )

#endif
