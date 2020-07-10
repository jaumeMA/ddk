#pragma once

#include "ewas_intrusive_list.h"
#include "lent_reference_wrapper.h"
#include "ewas_signal_functor.h"
#include "ewas_async_message_queue.h"

namespace ewas
{
namespace detail
{

template<typename MessageType>
struct async_message_exchange_room
{
public:
	async_message_exchange_room(sender_id i_id)
	: m_id(i_id)
	{
	}
	virtual ~async_message_exchange_room(){}
	sender_id get_id() const
	{
		return m_id;
	}

	virtual void push_message(const MessageType& i_msg) = 0;
	virtual bool tryRemoveCaller(const detail::connection_base&) = 0;
	virtual bool operator==(receiver_id i_id) const = 0;

private:
	const sender_id m_id;
};

template<typename Callable, typename MessageType, typename BuiltInMessageType>
struct async_message_exchange_typed_room : async_message_exchange_room<BuiltInMessageType>
{
	typedef signal_functor<Callable> signal_functor_t;
    using async_message_exchange_room<BuiltInMessageType>::get_id;

public:
	async_message_exchange_typed_room(sender_id i_id, detail::intrusive_node<signal_functor_t>& i_caller, lent_reference_wrapper<async_attachable_message_queue<MessageType>> i_msgQueue)
	: async_message_exchange_room<BuiltInMessageType>(i_id)
	, m_msgQueue(i_msgQueue)
	{
		m_callers.push(ewas::lend(i_caller));
		m_msgQueue->start(i_id,std::bind(&async_message_exchange_typed_room<Callable,MessageType,BuiltInMessageType>::broadcast_msg,this,std::placeholders::_1));
	}
	~async_message_exchange_typed_room()
	{
		m_msgQueue->stop(get_id());
	}
	void broadcast_msg(const MessageType& i_msg)
	{
		EWAS_ASSERT(get_id() == i_msg.get_id(), "Forwarding unrelated messages");

		const typename MessageType::tuple_t& forwarded_args = i_msg.forward_message();

		typename intrusive_list<signal_functor_t>::const_iterator itCaller = m_callers.begin();
		for(;itCaller!=m_callers.end();++itCaller)
		{
			if(itCaller->is_enabled())
			{
				itCaller->execute_tuple(forwarded_args);
			}
		}
	}
	void addCaller(detail::intrusive_node<signal_functor_t>& i_caller)
	{
		m_callers.push(ewas::lend(i_caller));
	}
	bool tryRemoveCaller(const detail::connection_base& i_caller)
	{
		typename intrusive_list<signal_functor_t>::iterator itCaller = std::find_if(m_callers.begin(),m_callers.end(),[&i_caller](const signal_functor_t& i_functor){ return i_functor.get_id() == i_caller.get_id();} );

		if(itCaller != m_callers.end())
		{
			m_callers.erase(itCaller);

			return true;
		}
		else
		{
			return false;
		}
	}
	void push_message(const BuiltInMessageType& i_msg) override
	{
		m_msgQueue->push_message(MessageType(i_msg));
	}
	bool operator==(receiver_id i_id) const override
	{
		return m_msgQueue->get_id() == i_id;
	}

private:
	intrusive_list<signal_functor_t> m_callers;
	lent_reference_wrapper<async_attachable_message_queue<MessageType>> m_msgQueue;
};

}
}
