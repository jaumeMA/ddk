#pragma once

#include "ddk_thread.h"
#include "ddk_reference_wrapper.h"

namespace ddk
{

template<typename Thread>
class attachable
{
	typedef typename Thread::message_type message_type;

public:
	template<typename Arg1>
	attachable(Arg1&& i_arg1)
	: m_updateThread(std::forward<Arg1>(i_arg1))
	{
	}
	template<typename Arg1, typename Arg2>
	attachable(Arg1&& i_arg1, Arg2&& i_arg2)
	: m_updateThread(std::forward<Arg1>(i_arg1),std::forward<Arg2>(i_arg2))
	{
	}
	template<typename Arg1, typename Arg2, typename Arg3>
	attachable(Arg1&& i_arg1, Arg2&& i_arg2, Arg3&& i_arg3)
	: m_updateThread(std::forward<Arg1>(i_arg1),std::forward<Arg2>(i_arg2),std::forward<Arg2>(i_arg3))
	{
	}
	template<typename Arg1, typename Arg2, typename Arg3, typename Arg4>
	attachable(Arg1&& i_arg1, Arg2&& i_arg2, Arg3&& i_arg3, Arg4&& i_arg4)
	: m_updateThread(std::forward<Arg1>(i_arg1),std::forward<Arg2>(i_arg2),std::forward<Arg3>(i_arg3),std::forward<Arg4>(i_arg4))
	{
	}
	~attachable()
	{
	}
	void attach(sender_id i_id, const ddk::function<void(const message_type&)>& i_processor)
	{
		m_updateThread.start(i_id,i_processor);
	}
	void detach(sender_id i_id)
	{
		m_updateThread.stop(i_id);
	}
	void signal_thread()
	{
		m_updateThread.signal_thread();
	}

private:
	Thread m_updateThread;
};

}