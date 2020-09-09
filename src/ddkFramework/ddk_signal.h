#pragma once

#include "ddk_signal_functor.h"
#include "ddk_connection.h"
#include "ddk_macros.h"
#include "ddk_linked_list.h"
#include "ddk_signal_connector.h"
#include <tuple>

namespace ddk
{

template<typename Return,typename ... Types>
class signal<Return(Types...)> : protected detail::signal_connector
{
  typedef signal_functor<Return(Types...)> signal_functor_t;

public:
	signal() = default;
    signal(const signal<void(Types...)>&) = delete;
    signal<void(Types...)>& operator=(const signal<void(Types...)>&) = delete;

	detail::connection_base& connect(const ddk::function<void(Types...)>& i_function) const
	{
		return m_callers.push(i_function,static_cast<const detail::signal_connector&>(*this));
	}
	void disconnect()
	{
		m_callers.clear();
	}
	template<typename ... Args>
	void execute(Args&& ... i_args) const
	{
		typename linked_list<signal_functor_t>::const_iterator itCaller = m_callers.begin();
		for(;itCaller!=m_callers.end();++itCaller)
		{
			if(itCaller->is_enabled())
			{
				itCaller->execute(std::forward<Args>(i_args) ...);
			}
		}
	}

private:
	void disconnect(const detail::connection_base& i_base) const override final
	{
		typedef typename linked_list<signal_functor_t>::iterator iterator;
		iterator itCaller = std::find_if(m_callers.begin(),m_callers.end(),[&i_base](const signal_functor_t& i_functor){ return i_functor.get_id() == i_base.get_id(); });
		if(itCaller != m_callers.end())
		{
			m_callers.erase(std::move(itCaller));
		}
	}

	mutable linked_list<signal_functor_t> m_callers;
};

}
