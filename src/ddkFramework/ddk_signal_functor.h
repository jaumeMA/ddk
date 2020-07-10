#pragma once

#include <functional>
#include "ewas_connection_base.h"
#include <stdio.h>
#include <tuple>
#include <utility>
#include "ewas_template_helper.h"
#include "ewas_signal_connector.h"

namespace ewas
{

template<typename>
class signal;

template<typename>
class signal_functor;

template<typename Return, typename ... Types>
class signal_functor<Return(Types...)> : public detail::connection_base
{
	typedef std::tuple<typename std::remove_const<typename std::remove_reference<Types>::type>::type ...> tuple_t;

public:
    signal_functor(const std::function<Return(Types...)>& call, const detail::signal_connector& i_connector)
	: connection_base(i_connector)
	, m_call(call)
	{
	}
    signal_functor(const signal_functor&) = delete;
    signal_functor(signal_functor&&) = delete;
	template<typename ... Args>
    Return execute(Args&& ... i_args) const
	{
	  return m_call(std::forward<Args>(i_args) ...);
	}
    Return execute_tuple(const tuple_t& i_args) const
	{
	    typedef typename mpl::create_range_rank<0,mpl::get_num_types<Types...>::value>::type range_seq;

        return _execute(i_args,range_seq{});
	}
    signal_functor& operator=(const signal_functor&) = delete;
    signal_functor& operator=(signal_functor&&) = delete;
    bool operator==(const signal_functor& other) const
	{
		return get_id() == other.get_id();
	}

private:
    template<size_t ... Seq>
      inline Return _execute(const tuple_t& i_args, const mpl::sequence<Seq...>&) const
      {
        return m_call(std::get<Seq>(i_args) ...);
      }

    std::function<Return(Types...)>  m_call;
};

}
