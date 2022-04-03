#pragma once

#include "ddk_async_message.h"
#include "ddk_lend_from_this.h"
#include "ddk_lock_free_stack.h"
#include "ddk_linked_list.h"
#include "ddk_optional.h"
#include "ddk_thread_executor_interface.h"
#include "ddk_async_defs.h"
#include "ddk_exclusion_area.h"

namespace ddk
{

template<typename MessageType>
struct async_message_queue
{
public:
	typedef MessageType message_type;

	async_message_queue() = default;
	~async_message_queue() = default;
	virtual void push_message(const MessageType& i_msg);
	virtual optional<MessageType> pop_message();
	bool empty() const;
	receiver_id get_id() const;

private:
	mutex m_mutex; //pending to check problems in multiple consumer pop method.
	multiple_consumer_lock_free_stack<MessageType> m_list;
};

template<typename MessageType>
struct async_attachable_message_queue : async_message_queue<MessageType>, lend_from_this<async_attachable_message_queue<MessageType>,async_attachable_message_queue<MessageType>>
{
public:
	typedef typename async_message_queue<MessageType>::message_type message_type;
    using async_message_queue<MessageType>::empty;
    using async_message_queue<MessageType>::pop_message;

	async_attachable_message_queue(thread_executor_unique_ref i_executor);
	void start(sender_id i_id, const ddk::function<void(const MessageType&)>& i_processor);
	void stop(sender_id i_id);
	void push_message(const MessageType& i_msg) override;
	bool set_affinity(const cpu_set_t& i_set);
	void dispatch_messages();
	void dispatch_message(const message_type& i_msg);
	void lock(Reentrancy i_reentrancy = Reentrancy::NON_REENTRANT);
	void unlock();

private:
	exclusion_area m_exclArea;
	linked_list<std::pair<sender_id,ddk::function<void(const message_type&)>>>	m_receivers;
	thread_executor_unique_ref m_executor;
};

}

#include "ddk_async_message_queue.inl"
