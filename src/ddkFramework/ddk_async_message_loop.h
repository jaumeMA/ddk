#pragma once

#include <list>
#include "ddk_signal_functor.h"
#include "ddk_linked_list.h"
#include "ddk_intrusive_node.h"
#include "ddk_async_defs.h"
#include "ddk_async_message_queue.h"
#include "ddk_async_message_exchange_room.h"
#include "ddk_reference_wrapper.h"

namespace ddk
{

template<typename BuiltInMessageType>
class async_message_loop
{

typedef detail::async_message_exchange_room<BuiltInMessageType> msg_exchange_room;
typedef unique_reference_wrapper<msg_exchange_room> msg_exchange_room_unique_ref;
typedef lent_reference_wrapper<msg_exchange_room> msg_exchange_room_lent_ref;

public:
	async_message_loop(sender_id i_id)
	: m_senderId(i_id)
	{
		pthread_mutex_init(&m_receiverMutex,NULL);
	}
	~async_message_loop()
	{
		pthread_mutex_destroy(&m_receiverMutex);
	}
	template<typename Callable, typename MessageType>
	detail::connection_base& connect(detail::intrusive_node<signal_functor<Callable>>& i_caller, lent_reference_wrapper<async_attachable_message_queue<MessageType>> i_messageQueue)
	{
		detail::connection_base& res = *i_caller;

		pthread_mutex_lock(&m_receiverMutex);

		typename linked_list<msg_exchange_room_unique_ref>::iterator itReceiver = std::find_if(m_receivers.begin(),m_receivers.end(),[&i_messageQueue](const msg_exchange_room_unique_ref& i_receiver){ return *i_receiver == i_messageQueue->get_id(); });

		if(itReceiver == m_receivers.end())
		{
			m_receivers.push(make_unique_reference<detail::async_message_exchange_typed_room<Callable,MessageType,BuiltInMessageType>>(m_senderId,i_caller,i_messageQueue));
		}
		else if(lent_pointer_wrapper<detail::async_message_exchange_typed_room<Callable,MessageType,BuiltInMessageType>> room = dynamic_lent_cast<detail::async_message_exchange_typed_room<Callable,MessageType,BuiltInMessageType>>(lend(*itReceiver)))
		{
			room->addCaller(i_caller);
		}

		pthread_mutex_unlock(&m_receiverMutex);

		return res;
	}
	void disconnect(const detail::connection_base& i_caller)
	{
		typename linked_list<msg_exchange_room_unique_ref>::iterator itReceiver = m_receivers.begin();
		for(;itReceiver != m_receivers.end();++itReceiver)
		{
			if((*itReceiver)->tryRemoveCaller(i_caller))
			{
				return;
			}
		}

		DDK_FAIL("Trying to remove caller with no receiver associated");
	}
	void clear(receiver_id i_id)
	{
		pthread_mutex_lock(&m_receiverMutex);

		typename linked_list<msg_exchange_room_unique_ref>::iterator itReceiver = std::find_if(m_receivers.begin(),m_receivers.end(),[&i_id](const msg_exchange_room_unique_ref& i_receiver){ i_receiver->get_id() == i_id; });

		if(itReceiver != m_receivers.end())
		{
			m_receivers.erase(itReceiver);
		}

		pthread_mutex_unlock(&m_receiverMutex);
	}
	void clear()
	{
		pthread_mutex_lock(&m_receiverMutex);

		m_receivers.clear();

		pthread_mutex_unlock(&m_receiverMutex);
	}
	void push_message(const BuiltInMessageType& i_msg)
	{
		pthread_mutex_lock(&m_receiverMutex);

		typename linked_list<msg_exchange_room_unique_ref>::iterator itReceiver =  m_receivers.begin();
		for(;itReceiver!=m_receivers.end();++itReceiver)
		{
			msg_exchange_room_lent_ref room = lend(*itReceiver);

			room->push_message(i_msg);
		}

		pthread_mutex_unlock(&m_receiverMutex);
	}

private:
	const sender_id m_senderId;
	linked_list<msg_exchange_room_unique_ref> m_receivers;
	mutable pthread_mutex_t m_receiverMutex;
};

}
