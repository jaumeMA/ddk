#pragma once

#include "ddk_async_message.h"
#include "lend_from_this.h"
#include "ddk_lock_free_stack.h"
#include "ddk_linked_list.h"
#include "ddk_optional.h"
#include "ddk_thread_executor_interface.h"
#include "ddk_async_defs.h"
#include <pthread.h>

namespace ddk
{

template<typename MessageType>
struct async_message_queue
{
public:
	typedef MessageType message_type;

	async_message_queue()
	{
	}
	~async_message_queue()
	{
	}
	virtual void push_message(const MessageType& i_msg)
	{
		m_list.push(i_msg);
	}
	virtual optional<MessageType> pop_message()
	{
		return m_list.pop();
	}
	bool empty() const
	{
		return m_list.empty();
	}
	receiver_id get_id() const
	{
		return receiver_id(reinterpret_cast<size_t>(this));
	}

private:
	lock_free_stack<MessageType> m_list;
};

template<typename MessageType>
struct async_attachable_message_queue : async_message_queue<MessageType>, lend_from_this<async_attachable_message_queue<MessageType>,async_attachable_message_queue<MessageType>>
{
public:
	typedef typename async_message_queue<MessageType>::message_type message_type;
    using async_message_queue<MessageType>::empty;
    using async_message_queue<MessageType>::pop_message;

	async_attachable_message_queue(thread_executor_unique_ref i_executor)
	: m_executor(std::move(i_executor))
	{
		pthread_mutex_init(&m_mutex, NULL);
	}
	~async_attachable_message_queue()
	{
		pthread_mutex_destroy(&m_mutex);
	}
	void start(sender_id i_id, const std::function<void(const MessageType&)>& i_processor)
	{
		pthread_mutex_lock(&m_mutex);

		typename linked_list<std::pair<sender_id,std::function<void(const message_type&)>>>::const_iterator itReceiver = std::find_if(m_receivers.begin(),m_receivers.end(),[&i_id](const std::pair<sender_id,std::function<void(const message_type&)>>& i_pair){ return i_pair.first == i_id; });

		EWAS_ASSERT(itReceiver == m_receivers.end(), "Attempting to connect more than once to the same queue");

		if(itReceiver == m_receivers.end())
		{
			if(m_receivers.empty())
			{
				thread_executor_interface::start_result startRes = m_executor->execute(nullptr,std::bind(&async_attachable_message_queue<MessageType>::dispatch_messages,this));

				EWAS_ASSERT(startRes.hasError() == false, "Error while starting thread executor : " + ewas::formatter<std::string>::format(startRes.getError()));
			}

			m_receivers.push(std::make_pair(i_id,i_processor));
		}

		pthread_mutex_unlock(&m_mutex);
	}
	void stop(sender_id i_id)
	{
		pthread_mutex_lock(&m_mutex);

		typename linked_list<std::pair<sender_id,std::function<void(const message_type&)>>>::iterator itReceiver = std::find_if(m_receivers.begin(),m_receivers.end(),[&i_id](const std::pair<sender_id,std::function<void(const message_type&)>>& i_pair){ return i_pair.first == i_id; });

		EWAS_ASSERT(itReceiver != m_receivers.end(), "Attempting to unconnect from not connected queue");

		if(itReceiver != m_receivers.end())
		{
			m_receivers.erase(itReceiver);

			if(m_receivers.empty())
			{
				thread_executor_interface::resume_result stopRes = m_executor->resume();

				EWAS_ASSERT(stopRes.hasError() == false, "Error while starting thread executor : " + ewas::formatter<std::string>::format(stopRes.getError()));
			}
		}

		pthread_mutex_unlock(&m_mutex);
	}
	virtual void push_message(const MessageType& i_msg)
	{
		async_message_queue<MessageType>::push_message(i_msg);

		m_executor->signal();
	}

private:
	void dispatch_messages()
	{
		pthread_mutex_lock(&m_mutex);

		while(const optional<message_type> currMsgOpt = pop_message())
		{
			const message_type& currMsg = *currMsgOpt;

			typename linked_list<std::pair<sender_id,std::function<void(const message_type&)>>>::const_iterator itReceiver = m_receivers.begin();
			for(;itReceiver!=m_receivers.end();++itReceiver)
			{
				const std::pair<const sender_id,std::function<void(const message_type&)>>& currReceiver = *itReceiver;

				if(currReceiver.first == currMsg.get_id())
				{
					(currReceiver.second)(currMsg);
				}
			}
		}

		pthread_mutex_unlock(&m_mutex);
	}

	pthread_mutex_t	m_mutex;
	linked_list<std::pair<sender_id,std::function<void(const message_type&)>>>	m_receivers;
	thread_executor_unique_ref m_executor;
};

}
